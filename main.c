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


#include <p32xxxx.h>
#include <plib.h> 
#include <stdint.h>

#include "stdtypes.h"
#include "Cerebot32MX4.h"

#define servo_neutral_us 15000
#define servo_lowest_us 5000
#define servo_highest_us 25000


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */
unsigned int change_delay;

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
void T2_intF(void);
void Wait_ms(WORD delay);

/* ------------------------------------------------------------ */
/*				Interrupt Service Routines						*/

/* ------------------------------------------------------------ */
void __ISR(_TIMER_2_VECTOR, ipl7) _Timer2Handler(void) {

    T2_intF();

    //    prtServo1 |= (1 << bnServo1);
    IFS0CLR = (1 << 8); // clear interrupt flag for timer 1
}

void __ISR(_OUTPUT_COMPARE_2_VECTOR, IPL7) _OC2_IntHandler(void) {
    //    prtServo1 &= ~(1 << bnServo1);
    PORTBSET = (1 << bnLed1);

    IFS0CLR = (1 << 10); // clear interrupt flag for output compare 1

}

/*
 *  main
 */
int main(void) {

    deviceInit();
    appInit();
    

    while (1) {
        PORTBSET = (1 << bnLed1);
        PORTBSET = (1 << bnLed2);
        //        OC2R = 5000;
        Wait_ms(1000);
        PORTBCLR = (1 << bnLed1);
        PORTBCLR = (1 << bnLed2);
        OC2R = 25000;
        Wait_ms(1000);
    }

    return 0;
}

void deviceInit() {
    int pbFreq;

    // Configure the device for maximum performance.
    // This macro sets flash wait states, PBCLK divider and DRM wait states
    // based on the specified clock frequency.  
    // It also turns on the cache mode if available.  
    // Returns the PB frequency
    pbFreq = SYSTEMConfigPerformance(F_CPU);

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
    prtLed1_4 &= ~((1 << bnLed1) | (1 << bnLed2)); //LLV
    trisLed1_4 &= ~((1 << bnLed1) | (1 << bnLed2)); //output

    //Set up the i/o pins we are using for the servo channels
    //as outputs.
    prtServo1 &= ~(1 << bnServo1);
    trisServo1 &= ~(1 << bnServo1);
}

void appInit() {
    
    change_delay = 20;
    // Set up timer 1 with overflow and output compare
    OC2CON |= (1 << 1) | (1 << 0); // OCM mode 2, init high, low on compare
    OC2R = 15000; // Start off with the servo in the middle

    IPC2SET = (1 << 20) | (1 << 19) | (1 << 18) | (1 << 16); // Interrupt P7S2 for both
    IFS0CLR = (1 << 10); // Clear the int flags for T1 and OC1
    IEC0SET = (1 << 10); // Enable ints for T1 and OC1  

    TMR2 = 0; // Reset its count
    PR2 = servo_neutral_us; // Set the period to 3030.3us (8/FCPU=0.1us)
    IPC2SET = (1 << 4) | (1 << 3) | (1 << 2) | (1 << 0);
    IFS0CLR = (1 << 8);
    IEC0SET = (1 << 8);
    T2CONSET = (1 << 15) | (1 << 5) | (1 << 4);
    OC2CONSET = (1 << 15); // Enable T1 with no prescale
    prtServo1 |= (1 << bnServo1);

    // Enable multi-vector interrupts.
    INTEnableSystemMultiVectoredInt();
}

void T2_intF(void) {

    OC2R += 100;
    if (change_delay == 0) {
        OC2RSET = 5000;
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