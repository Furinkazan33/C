#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct result { 
	size_t error;
	size_t count;
	void *data;
	char error_message[256];
} result;

static result _RESULT;

typedef struct person { 
	int age; 
	char name[32]; 
	char info[256]; 
} person;

#define INFO 1
#define WARN 2
#define ERROR 3

void find_person(int n) {
	if(n == 10) {
		_RESULT.error = 0;
		_RESULT.count = 0;
	}
	else if(n == 8) {
		_RESULT.error = 0;
		_RESULT.count = 2;
		person *alloc = malloc(sizeof(person) * 2);
		
		alloc->age = n;
		strcpy(alloc->name, "toto");
		
		(alloc + 1)->age = n;
		strcpy((alloc + 1)->name, "tata");
		
		(&_RESULT)->data = alloc;
	}
	else if(n == 5) {
		_RESULT.error = INFO;
		strcpy((&_RESULT)->error_message, "simple info");
	}
	else if(n == 3) {
		_RESULT.error = ERROR;
		strcpy((&_RESULT)->error_message, "error");
	}
	else {
		_RESULT.error = 0;
		_RESULT.count = 1;
		person *alloc = malloc(sizeof(person));
		
		alloc->age = n;
		strcpy(alloc->name, "toto");
		(&_RESULT)->data = alloc;
	}
}


int main() {
	result *r = &_RESULT;
	
	for(int i = 10; i > 0; i--) {
		find_person(i);
		
		if(!r->error) {
			printf("found %ld : ", r->count);
			
			for(size_t j = 0; j < r->count; j++) {
				person *p = r->data;
				printf("[%d %s] ", (p + j)->age, (p + j)->name);
			}
			printf("\n");
		}
		else {
			printf("[error_message][%s]\n", r->error_message);
			
			if(r->error == ERROR) {
				return 1;
			}
		}
	}
	
	return 0;
}

