/*
 * File:   main.c
 * Author: Chris
 *
 * Created on August 24, 2022, 10:36 PM
 *
 * This demo will be using a single Injora INJO025-270 servo
 * 330 Hz (3030.3us) pulse: 1500us center, 500us min, 2500us max. 4us dead-zone
 * operating at 4.8-6.0 VDC
 */

#define _SUPPRESS_PLIB_WARNING

#include <xc.h>
#include <p32xxxx.h>
#include <peripheral/int.h>
#include <lega-c/peripheral/system.h>
#include <peripheral/adc10.h>

#include "stdtypes.h"
#include "Cerebot32MX4.h"

#define servo_period_100us 30303
#define servo_neutral_us 15000
#define servo_lowest_us 5000
#define servo_highest_us 25000

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */
volatile unsigned int change_delay;
volatile unsigned int change_amount; // servo movements, in main
int dir;
/* ------------------------------------------------------------ */
/*				Set Up of System Clock							*/
/* ------------------------------------------------------------ */
// Configuration Bit settings for System Clock = 80 MHz
//  Primary Osc w/PLL (XT+,HS+,EC+PLL)
//  Input Divider	2x Divider
//	Multiplier		18x Multiplier
//  WDT disabled
//  Other options are don't cares

#define F_CPU 80000000L

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */
void deviceInit(void);
void appInit(void);
static void changeServoPeriod(void);
void Wait_ms(WORD delay);
uint32_t mapToPR(uint32_t);

/* ------------------------------------------------------------ */
/*				Interrupt Service Routines						*/

/* ------------------------------------------------------------ */
void
__ISR(_TIMER_2_VECTOR, ipl7auto) Timer2Handler(void) {
    OC2R = mapToPR(ADC1BUF0);
    IFS0bits.T2IF = 0;
}

void
__ISR(_OUTPUT_COMPARE_2_VECTOR, ipl7auto) OC2_IntHandler(void) {
    IFS0bits.OC2IF = 0;

}

/*
 *  main
 */
int main(void) {

    deviceInit();
    appInit();

    while (fTrue) {
        LATBSET = (1 << bnLed1);
        Wait_ms(1000);

        LATBCLR = (1 << bnLed1);
        Wait_ms(1000);
    }

    return 0;
}

void deviceInit() {
    SYSTEMConfigPerformance(F_CPU);

    //Microchip recommends typing unused pins to ground
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    PORTF = 0;
    PORTG = 0;

    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    TRISF = 0;
    TRISG = 0;
}

void appInit() {
    // Output Compare 2 setup
    OC2R = 25000;
    OC2CONbits.ON = 0;
    OC2CONbits.OCM0 = 1; // Compare event toggles OCx pin
    OC2CONbits.OCM1 = 0;
    OC2CONbits.OCM2 = 1;
    OC2CONbits.OCTSEL = 0;
    IPC2bits.OC2IP = 7;
    IPC2bits.OC2IS = 3;
    IFS0bits.OC2IF = 0;
    IEC0bits.OC2IE = 1;

    // Timer2 setup
    PR2 = servo_period_100us;
    TMR2 = 0;
    T2CONbits.TCKPS = 3; // 011 = 1:8 prescale value
    IPC2bits.T2IP = 7;
    IPC2bits.T2IS = 3;
    IFS0bits.T1IF = 0;
    IEC0bits.T2IE = 1;

    //    // Timer3 setup - adc interrupt source
    //    PR3 = 10000;
    //    TMR3 = 0;
    //    T3CONbits.TCKPS = 3; // 011 = 1:8 prescale value
    //    IPC2bits.T2IP = 7;
    //    IPC2bits.T2IS = 3;
    //    IFS0bits.T1IF = 0;
    //    IEC0bits.T2IE = 1;

    AD1PCFGbits.PCFG15 = 0; // AN15 is set to analog input
    TRISBSET = (1 << 15); // TRISB 15 is set to input

    // AD1CHS: ADC Input Select Register
    // Positive Input Select bits for MUX B


    AD1CHSbits.CH0NA = 0; // Channel 0 negative input is VR-
    AD1CHSbits.CH0SA = 0xF; // Channel 0 positive input is AN15 [JB-07]

    AD1CON1bits.FORM = 0; // Output is 16bit integer format
    AD1CON1bits.SSRC = 0b111; // Internal counter ends sampling and starts conversion (auto convert)

    AD1CON2bits.VCFG = 0; // Vdd/Vss are +/- ref
    AD1CON2bits.SMPI = 0; // Conversion is always written to ADC1BUF0

    AD1CON3bits.ADCS = 1; // 4 T_pb clks for the conversion clock
    AD1CON3bits.SAMC = 2; //


    //    T3CONbits.ON = 1;
    AD1CON1bits.ON = 1;
    AD1CON1bits.ASAM = 1;
    T2CONbits.ON = 1; // enable the timer
    OC2CONbits.ON = 1; // enable the output compare

    // Begin with the servo signal high

    // intialize the global variable setting the speed of movement
    change_delay = 500;
    change_amount = 20000 / change_delay;
    dir = -1;

    // Enable multi-vector interrupts.
    INTEnableSystemMultiVectoredInt();
    //    __builtin_enable_interrupts();
}

static void changeServoPeriod(void) {

    change_delay -= 1;
    if (change_delay == 0) {
        dir *= -1;
        change_delay = 500;
        //        T1CONbits.ON = 0;
        //        Wait_ms(1000);
        //        T1CONbits.ON = 1;
    }
    if (dir < 0) {
        OC2R -= change_amount;
    } else {
        OC2R += change_amount;
    }

}

void Wait_ms(WORD delay) {

    WORD i;

    while (delay > 0) {

        for (i = 0; i < 7270; i++) {
            _nop(); //Optimization settings can remove empty loops, fill with nops.
        }//end for
        delay -= 1;
    }//end while
}//end Wait_ms()

uint32_t mapToPR(uint32_t a) {
    return 20000 * a / 1024 + 5000;
}