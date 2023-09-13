#include <stdio.h>

#define SIZE 38

int add1(int a) {
	return a + 1;
}

int main(void) {

	int a = 35623; // line comment 
	double b = 12.56;
	char c = 'p';
	char c2 = '\n';
	char *s = "un\\ test' de\" \tstring\n\0";
	int a2 = a;
	int *p = a;
	*p = 0;

	int *(func_pointer)(int);
	func_pointer = add1;

	printf("%d %f %c %s\n", a, b, c, s);

	char tmp[SIZE];
	const char *toto = "1 2 3 4 5";
	const char *tab[4] = { "1", "2", 
		"3", "4"
	}

	int tata = 3;
	if((tata = func_pointer(tata)) == 4) {
		puts("ok");
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


	return a2;
}


