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

#include "Cerebot32MX4.h"
#include <xc.h>
#include <sys/attribs.h>

#include "stdtypes.h"
#include "Cerebot32MX4.h"

#define servo_period_100us_ctr 30303
#define servo_neutral_us 15000
#define servo_lowest_us 5000
#define servo_highest_us 25000

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */
unsigned int change_delay; // servo movements, in main

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
void changeServoPeriod(void);
void Wait_ms(WORD delay);

/* ------------------------------------------------------------ */
/*				Interrupt Service Routines						*/

/* ------------------------------------------------------------ */
void 
__ISR(_TIMER_2_VECTOR, ipl7auto) Timer2Handler(void) {

    LATGSET = (1 << bnServo1);
    IFS0bits.T2IF = 0;
}

void 
__ISR(_OUTPUT_COMPARE_2_VECTOR, ipl7auto) OC2_IntHandler(void) {

    changeServoPeriod();
    LATG = 0;
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
        LATBSET = (1 << bnLed2);
        Wait_ms(1000);
        
        LATBCLR = (1 << bnLed1);
        LATBCLR = (1 << bnLed2);
        Wait_ms(1000);
    }

    return 0;
}

void deviceInit() {
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
    OC2R = servo_neutral_us;
    OC2CONbits.OCM = 3;  // Compare event toggles OCx pin
    IPC2bits.OC2IP = 7;
    IPC2bits.OC2IS = 3;
    IFS0bits.OC2IF = 0;
    IEC0bits.OC2IE = 1;    
    
    // Timer2 setup
    PR2 = servo_period_100us_ctr;
    TMR2 = 0;
    T2CONbits.TCKPS = 3; // 011 = 1:8 prescale value
    IPC2bits.T2IP = 7;
    IPC2bits.T2IS = 3;
    IFS0bits.T1IF = 0;
    IEC0bits.T2IE = 1;
    
    T2CONbits.ON = 1;   // enable the timer
    OC2CONbits.ON = 1;  // enable the output compare
    
    // Begin with the servo signal high
    LATGSET = (1 << bnServo1);
    
    // intialize the global variable setting the speed of movement
    change_delay = 20;
    
   // Enable multi-vector interrupts.
    __builtin_enable_interrupts();
}

void changeServoPeriod(void) {

    OC2R += 100;
    
    if (change_delay == 0) {
        OC2R = 5000;
        change_delay = 20;
    }

    change_delay -= 1;
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