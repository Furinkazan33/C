#include <stdlib.h>
#include <stdio.h>
#include <wordexp.h>



int main() {

	wordexp_t we;

	if(!wordexp("$(ls)", &we, WRDE_UNDEF | WRDE_SHOWERR)) {
		printf("%ld words\n", we.we_wordc);

		for(size_t i = 0; i < we.we_wordc; i++) {
			printf("%s\n", we.we_wordv[i]);
		}
	}

	return 0;
}


