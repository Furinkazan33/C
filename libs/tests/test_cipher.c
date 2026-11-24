#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "cipher.h"


int test_fill_left() {
	
	char *src = "1234";
	char *res = fill_left(src, 8);
	
	#ifdef DEBUG
		printf("asserting fill_left(1234, 8)=00001234, result: [%s]\n", res);
	#endif
	
	assert(!strcmp(res, "00001234"));
	
	return 0;
}

int test_base_to_decimal() {
	int res = base_to_decimal("1010", 2);
	
	#ifdef DEBUG
		printf("asserting base_to_decimal(1010, 2)=10, result: [%d]\n", res);
	#endif
	
	assert(res == 10);
	
	return 0;
}

int test_decimal_to_base() {
	char *res = decimal_to_base(10, 2);
	
	#ifdef DEBUG
		printf("asserting decimal_to_base(10, 2)=1010, result: [%s]\n", res);
	#endif
	
	assert(!strcmp(res, "1010"));
	
	return 0;
}

int test_base_to_base() {
	char *res = base_to_base("1010", 2, 10);
	
	#ifdef DEBUG
		printf("asserting base_to_base(1010, 2, 10)=10, result: [%s]\n", res);
	#endif
	
	assert(!strcmp(res, "10"));
	
	return 0;
}


int main() {
	
	test_fill_left();
	
	test_base_to_decimal();
	
	test_decimal_to_base();
	
	test_base_to_base();
	

	return 0;
}
