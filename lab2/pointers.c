/*
 Modified
 By Erik Pettersson
 and Sebastian Andersson
*/

#include <stdio.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";

/*
 * Erik Pettersson
*/
int count = 0;	// counter variable

int list1[30];	// a list to contain text1
int list2[30];	// a list to contain text2

void copycodes(char* text_in, int* list_in, int* count){

	while(*text_in != 0) {	// as long as we havent reached NUL

		*list_in = *text_in;	// store word $t0, 0($a1)
		// in the place list_in is located, store text_in

		// increment the parameters
		text_in++;				// add immediate $a0, $a0, 1
		list_in++;				// add immediate $a1, $a1, 4
		(*count)++;				// add immediate $t1, $t1, 1
		/* count need to be incremented like this due to being passed as an adress
		 * and * and ++ have the same precedence meaning that we need to force
		 * the order by using parenteses or else it wont work, highly confusing
		 * but we dereference count so that we can increment it's value.
		*/
	}
}

/*
 * Erik Pettersson
*/
void work(){
	//			$a0, $a1, $a2
	copycodes(text1, list1, &count);	// call for text 1
	copycodes(text2, list2, &count);	// call for text 2
	/* text# is a pointer, list1 a globally reachable array,
	 * and &count is the adress of the count variable meaning that
	 * we do not create a local list/count variable in copycodes */
}

void printlist(const int* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char* c){
	printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n", 
		(int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));

}

/* in order to make sure that your above thing does the expected thing:
 * for pointers c+x is the same as c[x]
 * Erik Pettersson
*/
void endian_proofer(const char* c){
	printf("\nEndian experiment checker: 0x%02x,0x%02x,0x%02x,0x%02x\n", 
		(int) c[0],(int) c[1], (int) c[2], (int) c[3]);

}
/*
 * Erik Pettersson
*/
void endian_proofest(const char* c){
	printf("\nEndian experiment checker: 0x%02x,0x%02x,0x%02x,0x%02x\n", 
		(int) c[3],(int) c[2], (int) c[1], (int) c[0]);

}

int main(void){
	work();

	printf("\nlist1: ");
	printlist(list1);
	printf("\nlist2: ");
	printlist(list2);
	printf("\nCount = %d\n", count);

	endian_proof((char*) &count);
	endian_proofer((char*) &count);
	endian_proofest((char*) &count);

}

/*

1. text1/text2 är redan pointers, count skickas adressen av:
copycodes(text1, list1, &count);

2. Pointer ~array, men i minnet, minnet är "en stor array".
	Incrementera en pointer -> kliva ett steg framåt
	storlek på steget beror på type.

3. Increment pointer -> gå vidare i minnet, hittar nåt annat
   Increment avrefererad pointer -> öka det pointern refererar till

	(*count)++;
	count need to be incremented like this due to being passed as an adress
	and * and ++ have the same precedence meaning that we need to force
	the order by using parenteses or else it wont work, highly confusing
	but we dereference count so that we can increment it's value.

4. *list_in = *text_in;
	i utgångsläget utför vi list_in[0] = text_in[0], nästa varv:
	list[1] = text_in[1]. osv.
	tills vi nått slutet på strängen - det vet vi tack vare att *text_in == 0 inträffar då

	store word $t0, 0($a1) innebär samma sak som ovan för ett givet varv.

	Dereferencing innebär att vi kommer arbeta med det som pointern pekar på,
	istället för pekaren ("objektet"/variabeln istället för ~minnesadresser där de finns)

5. Little-endian, endian_proof skriver ut från en låg adress till högre adresser
   endian_proof försöker lura oss genom att skriva ut från c[0] till c[3]
   endian_proofest skriver ut hög -> låg, där ser vi att de högre inte är fyllda
   Alltså bör systemet som vi kört på vara little-endian.
   En char är (bör) vara 8 bitar, vi kliver då 8 bitar uppåt för varje inkrementation av c,
   eftersom c är en char-pointer.

	Big endian
	+ behöver inte översättas om man ska skicka data via nätverk - använder generellt big endian
	+ lättläst, "som vi förväntar oss"
	ex. ettusen tvåhundra trettiofyra (1234)
	minne lo -> hi: 1234
	- leder till fet förvirring om man lärt sig att man räknar "höga värden till vänster"
	bit 31 -> bit 0 (som man gör om man räknar binärt, OCH decimalt, coolt!)
	1101 == 8+4+1, inte 1+2+8

	Little endian
	+ blir alltid samma värde oavsett om man läser 1, 2, 4, 8, 32, 64... bytes
	0x41 == 0x0041 == 0x000041
	-> leder till att man lätt kan casta värden mellan storlekar.

*/
