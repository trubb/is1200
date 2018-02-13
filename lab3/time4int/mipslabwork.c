/* mipslabwork.c

	This file written 2015 by F Lundevall
	Updated 2017-04-21 by F Lundevall

	This file should be changed by YOU! So you must
	add comment(s) here with your name(s) and date(s):

	This file modified 2017-04-31 by Ture Teknolog

	For copyright and licensing, see file COPYING */

#include <stdint.h>	/* Declarations of uint_32 and the like */
#include <pic32mx.h>	/* Declarations of system-specific addresses etc */
#include "mipslab.h"	/* Declatations for these labs */

volatile int *myTRISE;
volatile int *myPORTE;

int mytime = 0x0000; // changed from 5957 in order to make it easier to see real number of ticks

int timeoutcount = 0;

int prime = 1234567;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void ) {
	
	if (IFS(0) & 0x0100) {
	
		timeoutcount++;

		if (timeoutcount == 10) {

			time2string( textstring, mytime );
			display_string( 3, textstring );
			display_update();
			tick( &mytime );
			timeoutcount = 0;

			// question 3.1
			// Clear the timer interrupt status flag
			// http://ww1.microchip.com/downloads/en/DeviceDoc/61105F.pdf
			// page 26
			IFSCLR(0) = 0x100;
		}
	}
}

/* Lab-specific initialization goes here */
void labinit( void ) {
	
	// Set *E to address of TRISE, volatile pointer
	myTRISE = (volatile int *) 0xbf886100;
	// Set the 8 least significant bits to zero to set them to be output pins
	*myTRISE = *myTRISE & 0xff00;

	myPORTE = (volatile int *) 0xbf886110;
	*myPORTE = 0x0; // set whatever porte points at to 0

	// Initialize port D, set bits 11-5 as inputs.
	// om 0-indexerat (vilket det borde vara) så 0xfe0 rätt
	// funkar inte det så pröva med 0x07f0.
	TRISD = TRISD | 0x0fe0; // changed to | from &

	// initialize timer2
	T2CON = 0x0; // stop clock until init done by setting 1st bit to 0
	T2CONSET = 0x70; //	set 0x70, 0111 000 for 1:256 prescaling (clock rate divider)
	TMR2 = 0x0; // clear timer register
	PR2 = (80000000 / 256) / 10; // set timeperiod for 100ms
	T2CONSET = 0x8000; // start the timer by setting bit 15 in T2CON "on" (1)

	IPCSET(2) = 0X1F;	// enable timer 2 and set interrupt priority to 7 - highest

	IECSET(0) = 0x100000000;	// (256) - interrupt enable reg, sets bit 8 to enable interrupts
	
	IECSET(0) = 0x800; // enable external interrupt #2

	enable_interrupt();	// call enable interrupts

	return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {

	prime = nextprime( prime );
	display_string( 0, itoaconv( prime ) );
	display_update();
}
