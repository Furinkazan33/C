#include <stdlib.h>
#include <stdint.h>
#include "bigint.h"
#include "macros.h"


bigint *bigint_new(uint64_t value) {
	bigint *res;
	MALLOC(res, sizeof(*res), return NULL, );
	memset(res, 0, sizeof(uint64_t));
	memcpy(res[0], &value, sizeof(uint64_t));
	
	
	return res;
}

void bigint_print(bigint *a) {
	printf("%zu\n", a[0]);
}

bigint *bigint_add(bigint *a, bigint *b) {
	bigint *res = bigint_new(0);
	
	
	
	return res;
}





int main() {
	bigint *a = bigint_new(18446744073709551615);
	bigint *b = bigint_new(20);


	bigint_print(a);


}

