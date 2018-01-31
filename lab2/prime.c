/*
 prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.

 Modified 2018-01-31
 By Erik Pettersson

*/


#include <stdio.h>

/*
	 function is_prime(n)
     if n ≤ 1
        return false
     else if n ≤ 3
        return true
     else if n mod 2 = 0 or n mod 3 = 0
        return false
     let i ← 5
     while i * i ≤ n
        if n mod i = 0 or n mod (i + 2) = 0
            return false
        i ← i + 1
     return true
*/

/**
 * is_prime changed to current state by erik pettersson 2018-01-31
 * determines if a number is prime by using the above defined methodology
**/
int is_prime(int n){
 
	int i = 5;

	if (n <= 1)	{	// 0 and 1 are not primes
		return 0;
	} else if (n <= 3) {	// 2 and 3 are primes
		return 1;
	} else if (n % 2 == 0 || n % 3 == 0) {	// if n mod 2 or n mod 3 is zero
		return 0;	// then n is not prime
	}

	while (i * i <= n) {
		if (n % i == 0 | n % (i + 2) == 0) {	// if n mod i or n mod i+2 is zero
			return 0;	// then n is not prime
		}
		i++;	// increment loop variable
	}

	return 1;	// if it passes: should be prime
}

int main(void){
  printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
  printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
  printf("%d\n", is_prime(987)); // 987 is not a prime. Should print 0.
}
