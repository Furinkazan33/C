#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PERSON_SIZES { 4, 4, 32, 64 }
#define PERSON_OFFSETS { 0, 4, 8, 40, 104 }
#define PERSON_SIZE 104
char *person_new(int id, int age, char *name, char *desc) {
	char *res = malloc(PERSON_SIZE);
	memcpy(res + 0, &id, 4); 
	memcpy(res + 4, &age, 4); 
	memcpy(res + 8, name, 32); 
	memcpy(res + 40, desc, 64);
	return res;
}

int stoi(char *s) { return *((int *) s); }
void person_print(char *p) { 
	printf("%d %d %s %s\n", stoi(p), stoi(p + 4), p + 8, p + 40);
}


int main() {
	char *p = person_new(1, 23, "toto", "la desc de toto");
	person_print(p);
}

