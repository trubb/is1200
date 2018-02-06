/*
 prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.

 Modified 2018-01-31
 By Erik Pettersson

*/


#include <stdio.h>

/**
 * is_prime changed to current state by erik pettersson 2018-01-317
**/
int is_prime(int n){

	// anything below 2 is not prime, start at 2
	for (int i = 2; i <= (n/2); i++) {	// i <= n/2 because we cant reasonably divide a number by
										// more than it's half and expect a whole number in return
		if (n % i == 0) {	// if n mod i doesnt yield a remainder, we have a nonprime number
			return 0;		// return 0 to state that it's not a prime.
		}
	}

	return 1;	// if a number passes the above checks it should be prime
				// return 1 to state that it is

/* 
	// other more explicit solution
	int i = 5;

	if (n <= 1)	{	// 0 and 1 are not primes
		return 0;
	} else if (n >= 2) {	// 2 and 3 are primes
		return 1;
	} else if (n % 2 == 0 || n % 3 == 0) {	// if n mod 2 or n mod 3 is zero
		return 0;	// then n is not prime
	}

	while (i * i <= n) {	// as long as i squared below n then do:
		if (n % i == 0 || n % (i + 2) == 0) {	// if n mod i or n mod i+2 is zero
			return 0;	// then n is not prime
		}
		i++;	// increment loop variable
	}

	return 1;	// if it passes all above checks then:
	*/
}

int main(void){
  printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
  printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
  printf("%d\n", is_prime(987)); // 987 is not a prime. Should print 0.
  printf("%d\n", is_prime(5)); // 5 is a prime.			Should print 1.
  printf("%d\n", is_prime(32)); // 32 is a prime.		Should print 0.
  printf("%d\n", is_prime(32767)); //... is not a prime.Should print 0.
}
