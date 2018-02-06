/*
 Modified 2018-01-31
 By Erik Pettersson

*/

#include <stdio.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";

int count = 0;	// counter variable

int list1[30];	// a list to contain text1
int list2[30];	// a list to contain text2

void copycodes(char* text_in, int* list_in, int* count){

	while(*text_in != 0) {

		*list_in = *text_in;	// store word $t0, 0($a1)
		// in the place list_in is located, store text_in

		// increment the parameters
		text_in++;				// add immediate $a0, $a0, 1
		list_in++;				// add immediate $a1, $a1, 4
		(*count)++;				// add immediate $t1, $t1, 1
		/* count need to be incremented like this due to being passed as an adress
		 * and * and ++ have the same precedence meaning that we need to force
		 * the order by using parenteses or else it wont work
		*/
	}
}

void work(){
	//			$a0, $a1, $a2
	copycodes(text1, list1, &count);	// call for text 1
	copycodes(text2, list2, &count);	// call for text 2
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

int main(void){
	work();

	printf("\nlist1: ");
	printlist(list1);
	printf("\nlist2: ");
	printlist(list2);
	printf("\nCount = %d\n", count);

	endian_proof((char*) &count);
}
