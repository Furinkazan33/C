#include <stdio.h>

#define  OPT_1	0x001
#define  OPT_2	0x002
#define  OPT_3	0x004
#define  OPT_4	0x008

int main(void) {

	unsigned int options = OPT_1 | OPT_3;

	if(options & OPT_1) {
		puts("1");
	}
	if(options & OPT_2) {
		puts("2");
	}
	if(options & OPT_3) {
		puts("3");
	}
	if(options & OPT_4) {
		puts("4");
	}
	return 0;
}
