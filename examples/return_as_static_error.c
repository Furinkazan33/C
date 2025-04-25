#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char _ERROR[256];

typedef struct person { int age; char name[32]; } person;


person *find_person_by_id(int n) {
	if(n == 10) {
		return NULL;
	}
	if(n == 1) {
		strcpy(_ERROR, "irrecuperable error");
		return (person *) &_ERROR;
	}
	person *alloc = malloc(sizeof(person));
	alloc->age = n;
	strcpy(alloc->name, "toto");	
	return alloc;
}



int main() {
	for(int i = 10; i > 0; i--) {
		person *p = find_person_by_id(i);
		if(p == (person *) &_ERROR) {
			printf("[ERROR][%s]\n", &_ERROR);
			return 1;
		}
		else if(!p) {
			printf("Not found\n");
		}
		else {
			printf("%d %s\n", p->age, p->name);
		}
	}
}

