#include "array.h"

typedef enum {
	T1_NONE = 100,
	WS,
	VALUE,
	VAR_START, VAR_END,
	EQUAL, SEMI
} type_1;

typedef enum { 
	T2_NONE = 100,
	SPACE, TAB, NL, B,
	VALUE_STRING
} type_2;

array *tnz_tpl(char *filepath);




