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

#define TMR2PERIOD ((80000000 / 256) / 10) /* 100ms */

int mytime = 0x5957;

int timeoutcount = 0;

int prime = 1234567;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void ) {
	
	timeoutcount++;

	if (timeoutcount == 10) {

		time2string( textstring, mytime );
		display_string( 3, textstring );
		display_update();
		tick( &mytime );
		timeoutcount = 0;
	}

	// question 1 task 3
	// Clear the timer interrupt status flag
	// http://ww1.microchip.com/downloads/en/DeviceDoc/61105F.pdf
	// page 26
	IFSCLR(0) = 0x100;
	// wrap this in the above if?

}

/* Lab-specific initialization goes here */
void labinit( void ) {
	
	// Set *E to address of TRISE, volatile pointer so nothing explodes
	volatile int *E = (volatile int *) 0xbf886100;
	
	// Set the 8 least significant bits to zero to set them to be output pins
	*E = *E & 0xff00;

	// Initialize port D, set bits 11-5 as inputs.
	// om 0-indexerat (vilket det borde vara) så 0xfe0 rätt
	// funkar inte det så pröva med 0x07f0.
	TRISD = TRISD & 0x0fe0;

	// initialize timer2
	T2CONSET = 0x70; //	set 0x70, 0111 000 for 1:256 prescaling (clock rate divider)
	PR2 = TMR2PERIOD; // set timeperiod as #DEFINEd above
	TMR2 = 0; // clear timer register
	T2CONSET = 0x8000; // start the timer by setting bit 15 in T2CON "on" (1)

	IPC(2) = 7;	// set interrupt priority for timer 2 to 7, highest

	IEC(0) = 0x100;	// set bit no 8 to enable interrupt

	enable_interrupt();	// call enable interrupts

	return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {

	prime = nextprime( prime );
	display_string( 0, itoaconv( prime ) );
	display_update();
}
