/* Example of c source code
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "local.h"

// Defining values
#define I_VALUE 32
#define D_VALUE 1.512
#define C_VALUE 'a'
#define S_VALUE "My string value"


int glob;
int glob_ass = 3;

const int glob_const;
const int glob_const_ass = 3;


int add1(int a) {
	return a + 1;
}

void print_mytype(mytype *mt) {
	printf("%d %s\n", mt->id, mt->name);
}

int main(void) {

	mytype *mt = malloc(sizeof(*mt));
	mt->id = 0;
	strcpy(mt->name, "myname");
	print_mytype(mt);

	int a = 35623; // line comment 
	double b = 12.56;
	char c = 'p';
	char c2 = '\n';
	char *s = "un\\ test' de\" \tstring\n\0";
	int a2 = a;
	int *p = &a;
	*p = 0;

	int (*func_pointer)(int);
	func_pointer = add1;

	printf("%d %f %c %s\n", a, b, c, s);

	char tmp[I_VALUE];
	const char *toto = "1 2 3 4 5";
	const char *tab[4] = { "1", "2", "3", "4" };

	int tata = 3;
	if((tata = func_pointer(tata)) == 4) {
		fprintf(stdout, "ok %d\n", tata);
	}

	/* 
	 * Block comment
	 *
	 * */

	switch(0) {
		case 0:
			tata++;
			break;
		default:
			tata += 2;
			break;
	}


	return (int) b;
}


