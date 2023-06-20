#include "str.h"





int main(void) {
	char src[] = "1111pat";
	int idx = str_idx(src, "pat");
	char *found = str_find(src, "pat");

	if(idx) {
		printf("%s\n", src+idx);
	}

	if(found) {
		printf("%s\n", found);
	}

	return 0;
}

