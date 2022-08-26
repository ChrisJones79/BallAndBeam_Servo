/************************************************************************
 *																		*
 *	Cerebot32MX4.h	--	General Cerebot 32MX4Interface Declarations		*
 *																		*
 *************************************************************************
 *	Author:		Mark Taylor												*
 *	Copyright 2009, Digilent Inc.										*
 *************************************************************************
 *	Module Description:													*
 *																		*
 *	This header file contains symbol declarations describing ports and	*
 *	pins for access to the on-board i/o devices and interface connector	*
 *	pins on the Digilent Cerebot 32MX4 board.							*
 *																		*
 *************************************************************************
 *	Revision History:													*
 *																		*
 *	09/22/2009(MarkT): created											*
 *																		*
 ************************************************************************/

// PIC32MX460F512L Configuration Bit Settings

// 'C' source line config statements

// DEVCFG3
#pragma config USERID = 0x812           // Enter Hexadecimal value (Enter Hexadecimal value)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = ON             // Secondary Oscillator Enable (Enabled)
#pragma config IESO = ON                // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
#pragma config OSCIOFNC = ON            // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

// DEVCFG0
#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is disabled)
#pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF    

/* ---------------------------------------------------------------
                On-Board I/O Declarations
   --------------------------------------------------------------- */

#define	trisLed1		TRISB
#define	trisLed2		TRISB
#define trisLed3		TRISB
#define	trisLed4		TRISB

#define	trisLed1_4		TRISB

#define	prtLed1	 		PORTB
#define	prtLed2			PORTB
#define	prtLed3			PORTB
#define	prtLed4			PORTB

#define	prtLed1_4		PORTB

#define	bnLed1			10
#define	bnLed2			11
#define	bnLed3 			12
#define	bnLed4 			13


/* ---------------------------------------------------------------
            Symbol Definitions for Servo Connectors
   --------------------------------------------------------------- */

#define	prtServo1		PORTG
#define	prtServo2		PORTG
#define	prtServo3		PORTG
#define	prtServo4		PORTG
#define	prtServo5		PORTG
#define	prtServo6		PORTG
#define	prtServo7		PORTF
#define	prtServo8		PORTF

#define	prtServo1_6 	PORTG
#define	prtServo7_8 	PORTF

#define	trisServo1		TRISG
#define	trisServo2		TRISG
#define	trisServo3		TRISG
#define	trisServo4		TRISG
#define	trisServo5		TRISG
#define	trisServo6		TRISG
#define	trisServo7		TRISF
#define	trisServo8		TRISF

#define	trisServo1_6	TRISG
#define	trisServo7_8	TRISF

#define	bnServo1	12
#define	bnServo2	13
#define	bnServo3	14
#define	bnServo4	15
#define	bnServo5	0
#define	bnServo6	1
#define	bnServo7	0
#define	bnServo8	1

/* ---------------------------------------------------------------
                Interface Connector Declarations
   --------------------------------------------------------------- */

/***************************************************************/







