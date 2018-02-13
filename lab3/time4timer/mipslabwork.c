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
/*
#define TMR2PERIOD ((80000000 / 256) / 10) // 100ms

volatile int *E;
volatile int *porte;

int mytime = 0x0000; // changed from 5957 in order to make it easier to see real number of ticks

int timeoutcount = 0;

char textstring[] = "text, more text, and even more text!";

// Interrupt Service Routine
void user_isr( void ) {
	return;
}

// Lab-specific initialization goes here
void labinit( void ) {
	
	// Set *E to address of TRISE, volatile pointer
	E = (volatile int *) 0xbf886100;
	
	porte = (volatile int *) 0xbf886110;

	*porte = 0x0; // set whatever porte points at to 0

	// Set the 8 least significant bits to zero to set them to be output pins
	*E = *E & 0xff00;

	// Initialize port D, set bits 11-5 as inputs.
	// om 0-indexerat (vilket det borde vara) så 0xfe0 rätt
	// funkar inte det så pröva med 0x07f0.
	TRISD = TRISD | 0x0fe0; // changed to | from &

	// initialize timer2
	T2CON = 0x0; // stop clock until init done by setting 1st bit to 0
	T2CONSET = 0x70; //	set 0x70, 0111 000 for 1:256 prescaling (clock rate divider)
	TMR2 = 0; // clear timer register
	PR2 = TMR2PERIOD; // set timeperiod as #DEFINEd above ((80000000 / 256) / 10))
	T2CONSET = 0x8000; // start the timer by setting bit 15 in T2CON "on" (1)

	return;
}

// This function is called repetitively from the main program
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

		//
		 * Question 2.1
		 * Reset all event flags, do 0x0000000000100 instead?
		 *
		 * Clear the timer interrupt status flag
		// * http://ww1.microchip.com/downloads/en/DeviceDoc/61105F.pdf
		 //* page 26
		/
		IFSCLR(0) = 0x100;

		timeoutcount++;	// increase counter
	}

	if (timeoutcount == 10) {	// when #interrupts reaches 10 do:
		//delay( 1000 ); // shouldnt be here for this right?
		time2string( textstring, mytime );
		display_string( 3, textstring );
		display_update();
		tick( &mytime );
		display_image(96, icon);
		timeoutcount = 0;

		// uppgift 1d
		// avreferera porte-pointern och öka det som finns där med 1
		(*porte)++;
		// testa också med ++, borde vara samma resultat som a = a + 0x1
		// görs efter call till tick because reasons
	}
}
*/

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

volatile int* myTRISE;    // Pointer to initialisation register for LEDs
volatile int* myPORTE;    // Pointer to LEDs I/O

int timeoutcount = 0;     // Global variable to count 10 timeouts

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  myTRISE = (volatile int*) 0xbf886100; // LEDs initialisation register
  *myTRISE &= ~0xff;                    // Initialise bits 0-7 as outputs (set to 0)
  myPORTE = (volatile int*) 0xbf886110; // LEDs I/O

  TRISD |= 0x0fe0;  // Initialise bits 5-11 as inputs (set to 1)

  T2CON = 0x0;                  // Stop clock until initialisation is done (first bit to 0)
  T2CONSET = 0x70;              // Apply prescale 1:256
  TMR2 = 0x0;                   // Timer set to 0
  PR2 = (80000000 / 256) / 10;  // Load period register
  T2CONSET = 0x8000;            // Start timer

  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  int btns = getbtns();
  if (btns) {
  	int sw = getsw();
  	if(btns & 0x4){
      mytime = (mytime & 0x0fff) | (sw << 12);
    }
    if(btns & 0x2){
      mytime = (mytime & 0xf0ff) | (sw << 8);
    }
    if(btns & 0x1){
      mytime = (mytime & 0xff0f) | (sw << 4);
    }
  }

  if (IFS(0) & 0x0100) {  // It the timeout flag is 1 (100ms):
    timeoutcount++;       // Increment timeoutcount
    IFSCLR(0)=0x100;      // Clear timeout flag (bit 8 to 0)
  }

  if (timeoutcount == 10) {
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
    (*myPORTE)++;
    display_image(96, icon);
    timeoutcount = 0; // Reset timeoutcount
  }
}
