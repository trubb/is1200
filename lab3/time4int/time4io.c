#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

/*
 * returnerar data från SWx
 */
int getsw(void) { // called as "getsw();"

	return (PORTD >> 8) & 0x000F;
}

/*
 * returnerar data från BTNx
 */
int getbtns(void) { // called as "getbtns();"

	return (PORTD >> 5) & 0x0007;
}
