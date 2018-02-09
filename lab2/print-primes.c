/*
 print-prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
 
 Modified
 By Erik Pettersson
 and Sebastian Andersson

*/


#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

/*
 * Erik Pettersson
*/
int count = 0;	// initialize global counter for number printing to 0

void print_number(int n) {	// a void function does things but does not explicitly return something
	
	printf("%10d",n);	// print the number, 10 as character space (term?) as it's what done below
	
	count++;			// every time we print, increase count by 1 to be able to..

	if (count % COLUMNS == 0) {	// if value of counter / COLUMNS yields no remainder

		printf("\n");			// force newline and continue printing if count is evenly divisible by COLUMNS
	}
}

/**
 * Sebastian Andersson
**/
int is_prime(int n) {

	int i = 0;
	for (i = 2; i <= ( n/2 ); i++) {// i <= n/2 because we cant reasonably divide a number by
									// more than it's half and expect a whole number in return
		if (n % i == 0) {	// if n mod i doesnt yield a remainder, we have a nonprime number
							// return 0 to state that it's not a prime.
			return 0;
		}
	}

	return 1;	// if a number passes the above checks it should be prime
				// return 1 to state that it is
}

/*
 * Erik Pettersson
*/
void print_primes(int n) {
	// Should print out all prime numbers less than 'n'
	// with the following formatting. Note that
	// the number of columns is stated in the define
	// COLUMNS

	for (int i = 2; i <= n; i++) {	// since 0, 1 are not primes, start at 2

		if( is_prime(i) ) {	// if a number is prime we want to print it

			print_number(i); // do so by calling print number with said number
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
