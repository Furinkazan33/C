#include <stdlib.h>
#include "lexer.h"
#include "arrayptr.h"



typedef enum {
	P_NONE = 0,
	P_DIRECTIVE,
	P_FUNC_DEF,
	P_FUNC_CALL,
	P_FUNC_PARAM,
	P_FUNC_PARAMS,
	P_FUNC_BLOCK,
	P_VAR_DECL,
	P_VAR_ASS,
} P_STRUCT_TYPE;


typedef struct pstruct {
	P_STRUCT_TYPE type;
	size_t len; // end - start
	LEX_token *start;
	LEX_token *end;
} pstruct;


LEX_token *parser_get_funcname(pstruct *function);

arrayptr *parser(arrayptr *lextokens);


