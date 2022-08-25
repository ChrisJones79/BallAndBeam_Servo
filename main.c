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

#include "Cerebot32MX4.h"
#include <p32xxxx.h>
#include <plib.h> 
//#include <stdint.h>

#include "stdtypes.h"

#define servoPeriod_us 3030
#define servo_neutral_us 1500
#define servo_lowest_us 500
#define servo_highest_us 2500


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */
volatile int t_chg = 5;
volatile int chg_cur = 0;
volatile int m_chg = 20; // The period and mag of the hard-coded
volatile int m_cur = 0;
volatile int dt = 1;
// servo movements, in main

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

/* ------------------------------------------------------------ */
/*				Interrupt Service Routines						*/

/* ------------------------------------------------------------ */
void __ISR(_TIMER_1_VECTOR, ipl7auto) Timer1Handler(void) {
    prtServo1 = (1 << bnServo1);
    if (chg_cur == 0) {
        chg_cur = t_chg;

        if ((m_cur >= servo_highest_us) || (m_cur <= servo_lowest_us)) {
            dt *= -1;
        }

        m_cur = m_cur + m_chg * dt;
        OC1R = m_cur;
    } else {
        chg_cur--;
    }

    IFS0CLR = (1 << 4); // clear interrupt flag for timer 1
}

void __ISR(_OUTPUT_COMPARE_1_VECTOR, ipl7auto) OC1_IntHandler(void) {
    prtServo1 &= ~(1 << bnServo1);
    IFS0CLR = (1 << 6); // clear interrupt flag for output compare 1	
}

/*
 *  main
 */
int main(void) {

    deviceInit();
    appInit();

    m_cur = servo_neutral_us;

    while (1) {
        _nop();
    }

    return (EXIT_SUCCESS);
}

void deviceInit() {
    //    int pbFreq;

    // Configure the device for maximum performance.
    // This macro sets flash wait states, PBCLK divider and DRM wait states
    // based on the specified clock frequency.  
    // It also turns on the cache mode if available.  
    // Returns the PB frequency
    //pbFreq = SYSTEMConfigPerformance(F_CPU);

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

    // Set the LEDs as outputs
    prtLed1_4 &= ~((1 << bnLed1) | (1 << bnLed2) | (1 << bnLed3) | (1 << bnLed4)); //LLV
    trisLed1_4 &= ~((1 << bnLed1) | (1 << bnLed2) | (1 << bnLed3) | (1 << bnLed4)); //output

    //Set up the i/o pins we are using for the servo channels
    //as outputs.
    prtServo1 &= ~(1 << bnServo1);
    trisServo1 &= ~(1 << bnServo1);
}

void appInit() {
    // Set up timer 1 with overflow and output compare
    OC1CON |= (2 << 0); // OCM mode 2, init high, low on compare
    OC1R |= servo_neutral_us; // Start off with the servo in the middle
    IPC1 |= (7 << 2) | (2 << 0) | (7 << 18) | (2 << 16); // Interrupt P7S2 for both
    IFS0 &= ~(1 << 4) | ~(1 << 6); // Clear the int flags for T1 and OC1
    IEC0 |= (1 << 4) | (1 << 6); // Enable ints for T1 and OC1  

    T1CON &= ~0xffff; // Clear T1 control reg
    TMR1 = 0x0; // Reset its count
    PR1 = 30303U; // Set the period to 3030.3us (8/FCPU=0.1us)
    T1CON |= (1 << 15); // Enable T1 with no prescale    

    // Enable multi-vector interrupts.
    INTEnableSystemMultiVectoredInt();
    return;
}