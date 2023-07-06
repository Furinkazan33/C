#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ascii_print(int c, int line) {
	if(c == 30 || c == 31 || c >= 127) {
		if(line == 0) {
			printf("  ");
		}
	}
	else {
		printf("%2c", c);
	}
}

void ascii_print_table(int line) {
	int i, j;

	if(line == 0) {
		printf("  ");
		for(i = 0; i < 10; i++) {
			printf("%2d", i);
		}

		printf("\n");
	}

	for(j = 3; j < 13; j++) {
		if(line == 0) {
			printf("%2d", j);
		}
		for(i = 0; i < 10; i++) {
			ascii_print(j * 10 + i, line);
		}
		if(line == 0) {
			printf("\n");
		}
	}
	if(line >= 0) {
		printf("\n");
	}
}

int is_int(char *s) {
	for(size_t i = 0; i < strlen(s); i++) {
		if(s[i] < 48 || s[i] > 57) {
			 return 1;
		}
	}
	return 0;
}

int main(int argc, char **argv) {
	if(argc >= 2) {
		if(argv[1][0] == '-') {
			if(argv[1][1] == 'l') {
				ascii_print_table(1);
			}
			else {

				for(int i = 2; i < argc; i++) {
					printf("%c\n", atoi(argv[i]));
				}
			}
		}
		else {
			for(int i = 1; i < argc; i++) {
				printf("%d\n", argv[i][0]);
			}
		}
	}
	else {
		ascii_print_table(0);
	}

	return 0;
}
