#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

/*
 * returnerar data från SWx
 * switches are bits 11, 10, 9, 8
 * so we shift them right 8 steps
 * to give return value at 4 lsb
 */
int getsw(void) { // called as "getsw();"

	return (PORTD >> 8) & 0x000F;
}

/*
 * returnerar data från BTNx
 * buttons are bits 7,6,5
 * so we shift them right 5 steps
 * to give return value at 3 lsb
 */
int getbtns(void) { // called as "getbtns();"

	return (PORTD >> 5) & 0x0007;
}
