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

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void ) {
	return;
}

/* Lab-specific initialization goes here */
void labinit( void ) {
	
	// Set *E to address of TRISE, volatile pointer
	myTRISE = (volatile int *) 0xbf886100;
	*myPORTE = 0x0; // set whatever porte points at to 0
	
	myPORTE = (volatile int *) 0xbf886110;
	// Set the 8 least significant bits to zero to set them to be output pins
	*myTRISE = *myTRISE & 0xff00;

	// Initialize port D, set bits 11-5 as inputs.
	// om 0-indexerat (vilket det borde vara) så 0xfe0 rätt
	// funkar inte det så pröva med 0x07f0.
	TRISD = TRISD | 0x0fe0; // changed to | from &

	// initialize timer2
	T2CON = 0x0; // stop clock until init done by setting 1st bit to 0
	T2CONSET = 0x70; //	set 0x70, 0111 000 for 1:256 prescaling (clock rate divider)
	TMR2 = 0; // clear timer register
	PR2 = (80000000 / 256) / 10; // set timeperiod for 100ms
	T2CONSET = 0x8000; // start the timer by setting bit 15 in T2CON "on" (1)

	return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {

	int switches = getsw();
	int button = getbtns();
	
	// MÅSTE LISTA UT HUR ADRESSERINGEN FUNGERAR HÄR NEDAN
	// 1, 2, 4 ??????
	// verkar vara eftersom vi har 3 bitar att lira med (med värden 4 2 1)
	// om button == 1 -> knapp 2(001) tryckt, om button == 2 -> knapp 3(010), osv

	// button 2
	if(button == 1 || button == 3 || button == 5 || button == 7){
		mytime = mytime & 0xFF0F;
		mytime = (switches << 4) | mytime;
	}
	// button 3
	if(button == 2 || button == 3 || button == 6 || button == 7){
		mytime = mytime & 0xF0FF;
		mytime = (switches << 8) | mytime;
	}
	// button 4
	if(button == 4 || button == 5 || button == 6 || button == 7){
		mytime = mytime & 0x0FFF;
		mytime = (switches << 12) | mytime;
	}

	// Check time-out event flag.
	// if third bit is set then ohshit things are going down
	if (IFS(0) & 0x100) {

		/*
		 * Question 2.1
		 * Reset all event flags, do 0x0000000000100 instead?
		 *
		 * Clear the timer interrupt status flag
		 * http://ww1.microchip.com/downloads/en/DeviceDoc/61105F.pdf
		 * page 26
		*/
		IFSCLR(0) = 0x0100;

		timeoutcount++;	// increase counter
	}

	if (timeoutcount == 10) {	// when #interrupts reaches 10 do:
		//delay( 1000 ); // shouldnt be here for this right?
		time2string( textstring, mytime );
		display_string( 3, textstring );
		display_update();
		tick( &mytime );
		// uppgift 1d
		// avreferera porte-pointern och öka det som finns där med 1
		(*myPORTE)++;
		// testa också med ++, borde vara samma resultat som a = a + 0x1
		// görs efter call till tick because reasons
		display_image(96, icon);
		timeoutcount = 0;

	}
}
