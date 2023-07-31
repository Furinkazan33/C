#include <stdlib.h>
#include <stdio.h>

int cmp3(int ref, int a, int b) {
	return abs(a - ref) - abs(b - ref);
}

void sort_nearest(int ref, int *tab, int size, int (*cmp)(int, int, int)) {
	int min;

	for(int i = 0; i < size - 1; i++) {
		for(int j = i + 1; j < size; j++) {
			if(cmp(ref, tab[j], tab[i]) < 0) {
				min = tab[j];
				tab[j] = tab[i];
				tab[i] = min;
			}
		}
	}
}

void print_tab(int *tab, int size) {
	for(int i = 0; i < size; i++) {
		printf("%d ", *(tab + i));
	}
	printf("\n");
}

int main(void) {
	int tab[8] = { 10, 5, 6, 7, 8, 2, 11, 1 };

	sort_nearest(6, tab, 8, cmp3);

	print_tab(tab, 8);

	return 0;
}
