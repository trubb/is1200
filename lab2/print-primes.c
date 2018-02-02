/*
 print-prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
 
 Modified 2018-01-31
 By Erik Pettersson

*/


#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6
//////////////////////////////////////////
//		Initialize global variables		//
//////////////////////////////////////////
int count = 0;	// initialize counter for number printing to 0

void print_number(int n) {	// void function does things but does not explicitly return something

	printf("%10d",n);	// print the number, 10 as it's what done below, space for characters?
	
	count++;			// every time we print, increase count by 1 to be able to..

	if (count % COLUMNS == 0) {	// if value of counter / COLUMNS yields no remainder
		printf("\n");			// force newline and continue printing
	}
}


/**
 * is_prime changed to current state by erik pettersson 2018-01-31
**/
int is_prime(int n) {

	int i = 0;
	for (i = 2; i <= ( n/2 ); i++) {	// if we reach a value that is beyond 
										//the range then n is prime, returns 1
		if (n % i == 0) {	// if n / anything that isnt 2 or itself yields 0 remainder
							// then n is not prime, return 0
			return 0;
		}
	}

	return 1; // if it passes the above check it is prime, 1
}

void print_primes(int n) {
	// Should print out all prime numbers less than 'n'
	// with the following formatting. Note that
	// the number of columns is stated in the define
	// COLUMNS

	for (int i = 2; i <= n; i++) {	// since 0, 1 are not primes, start at 2
		if( is_prime(i) ) {
			print_number(i);
		}
	}

/*	printf("%10d ", 2);
	printf("%10d ", 3);
	printf("%10d ", 5);
	printf("%10d ", 7);
	printf("%10d ", 11);
	printf("%10d ", 13);
	printf("\n");
	printf("%10d ", 17);
	printf("%10d ", 19);
*/
	printf("\n");
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]) {

	if(argc == 2) {
		print_primes(atoi(argv[1]));
	}
	else {
		printf("Please state an interger number.\n");
	}

	return 0;
}
