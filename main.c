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

#include <p32xxxx.h>
#include <./plib.h> 
#include <./stdint.h>

#include "stdtypes.h"
#include "Cerebot32MX4.h"



#define servoPeriod_us 3030
#define servo_neutral_us 1500
#define servo_lowest_us 500
#define servo_highest_us 2500

/* ------------------------------------------------------------ */
/*				Set Up of System Clock							*/
/* ------------------------------------------------------------ */
// Configuration Bit settings for System Clock = 80 MHz
//  Primary Osc w/PLL (XT+,HS+,EC+PLL)
//  Input Divider	2x Divider
//	Multiplier		18x Multiplier
//  WDT disabled
//  Other options are don't cares
#pragma config FNOSC = PRIPLL, POSCMOD = HS, FPLLMUL = MUL_20, FPLLIDIV = DIV_2
#pragma config FPBDIV = DIV_2, FPLLODIV = DIV_1 // 80MHz
#pragma config FWDTEN = OFF
#define F_CPU 80000000L

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */
void deviceInit(void);
void appInit(void);

/* ------------------------------------------------------------ */
/*				Interrupt Service Routines						*/

/* ------------------------------------------------------------ */
void __ISR(_TIMER_1_VECTOR, ipl7auto) _Timer1Handler(void) {

    IFS0CLR = (1 << 8); // clear interrupt flag for timer 2
}

void __ISR(_OUTPUT_COMPARE_1_VECTOR, ipl7auto) OC1_IntHandler(void) {

    IFS0CLR = (1 << 10); // clear interrupt flag for output compare 2	
}

/*
 *  main
 */
int main(void) {

    return (EXIT_SUCCESS);
}

void deviceInit() {
    int pbFreq;

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

    //    T1CONbits my = { .TCS = 0, .TSYNC = 0, .TCKPS = 1, .TGATE = 0, .ON = 1}; 
    //    T1CONSET = my;
}