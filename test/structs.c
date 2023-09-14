#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef enum s_type {
	S1 = 0,
	S2,
	S3
} s_type;

typedef struct struct1 {
	s_type type;
	int data;
} struct1;

typedef struct struct2 {
	s_type type;
	double data;
} struct2;

typedef struct struct3 {
	s_type type;
	char data[64];
} struct3;


int main(void) {

	struct1 *s1 = malloc(sizeof(*s1)); s1->type = S1;
	struct2 *s2 = malloc(sizeof(*s2)); s2->type = S2;
	struct3 *s3 = malloc(sizeof(*s3)); s3->type = S3;

	s1->data = 1;
	s2->data = 2.0;
	strcpy(s3->data, "3");

	void *a[3] = { s1, s2, s3 };

	for(size_t i = 0; i < 3; i++) {
		switch(*(int *) a[i]) {
			case S1:
				fprintf(stdout, "S1 : %d\n", ((struct1 *)a[i])->data);
				break;
			case S2:
				fprintf(stdout, "S2 : %f\n", ((struct2 *)a[i])->data);
				break;
			case S3:
				fprintf(stdout, "S3 : %s\n", ((struct3 *)a[i])->data);
				break;
			default:
				break;
		}
	}

	return 0;
}
