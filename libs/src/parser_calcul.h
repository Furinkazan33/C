#include <stdbool.h>


typedef enum expr_type {
	ET_NONE = 0,
	ET_VALUE,
	ET_COMPOSED,
} expr_type;

typedef struct expr {
	expr_type type;
	float value;
	char op;
	struct expr *left;
	struct expr *right;
} expr;



void expr_free(expr *e);


bool simplify(expr *e);
void expr_fprintf(FILE *f, expr *e);

