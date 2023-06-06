#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage(char *cmd) {
	printf("usage : %s [list]\n", cmd);
}

int compare_as_int(const char *s1, const char *s2) {
	return atoi(s1) > atoi(s2);
}

void sort(char **tab, int n, int (*compare)(const char *, const char *)) {
	char *tmp;

	for(int i = 0; i < n; i++) {
		for(int j = i + 1; j < n; j++) {
			if(compare(tab[i], tab[j]) > 0) {
				tmp = tab[j];
				tab[j] = tab[i];
				tab[i] = tmp;
			}
		}
	}
}

void print_tab(char **tab, int n) {
	for(int i = 0; i < n; i++) {
		printf("%s ", tab[i]);
	}
	printf("\n");
}

int main(int argc, char **argv) {
	char **args = argv;

	if(argc < 2) {
		usage(argv[0]);
		return 1;
	}

	args++;
	argc--;

	sort(args, argc, compare_as_int);
	print_tab(args, argc);

	sort(args, argc, strcmp);
	print_tab(args, argc);

	return 0;
}
