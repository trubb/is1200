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

volatile int *myTRISE;		// declare the pointers volatile and global in order to
volatile int *myPORTE;		// fix previous adressing issues

int mytime = 0x0000; // changed from 5957 in order to make it easier to see real number of ticks

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void ) {
	return;
}

/* Lab-specific initialization goes here */
void labinit( void ) {

	// Set *E to address of TRISE, volatile int pointer
	// The TRISx registers configure the data direction flow through port I/O pins.
	myTRISE = (volatile int *) 0xbf886100;
	
	myPORTE = (volatile int *) 0xbf886110;

	*myPORTE = 0x0; // set whatever myPORTE points at to 0, clear what's there

	// The TRISx register bits determine whether a PORTx I/O pin is an input or an output
	// Set the 8 least significant bits to zero to set them to be output pins
	// If a data direction bit is ‘1’, the corresponding I/O port pin is an input
	*myTRISE = *myTRISE & 0xff00;

	// Initialize port D, set bits 11-5 as inputs.
	// same principle as above, bits that are "1" are inputs
	TRISD = TRISD | 0x0fe0; // changed to | from &

	return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {

	int switches = getsw();
	int button = getbtns();
	
	// eftersom vi har 3 bitar att lira med (med värden 4 2 1)
	// om button == 1 -> knapp 2(001) tryckt, om button == 2 -> knapp 3(010), osv
	// button == 3 -> 011, == 4 -> 100, == 5 -> 101, == 6 -> 110, == 7 -> 111

	// button 2 001
	if(button == 1 || button == 3 || button == 5 || button == 7) {
		mytime = mytime & 0xFF0F; // & mytime and FF0F to mask out bit 1
		mytime = (switches << 4) | mytime;
		// update time to be what the switches are set to, shifted to correct pos
	}
	// button 3 010
	if(button == 2 || button == 3 || button == 6 || button == 7) {
		mytime = mytime & 0xF0FF;
		mytime = (switches << 8) | mytime;
	}
	// button 4 110
	if(button == 4 || button == 5 || button == 6 || button == 7) {
		mytime = mytime & 0x0FFF;
		mytime = (switches << 12) | mytime;
	}

	delay( 1000 );
	time2string( textstring, mytime );
	display_string( 3, textstring );
	display_update();
	tick( &mytime ); // calla tick med mytimes ADRESS
	display_image(96, icon); // show doge?

	// uppgift 1d
	// avreferera myPORTE-pointern och öka det som finns där med 1
	*myPORTE = *myPORTE + 0x1;
	// testa också med ++, borde vara samma resultat som a = a + 0x1
	// görs efter call till tick because reasons
}
