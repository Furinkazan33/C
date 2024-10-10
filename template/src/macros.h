#include <stdio.h>
#include <stdlib.h>

#ifndef MACROS_H
#define MACROS_H

#define FOR(i, min, max, block) for(size_t i = min; i < max; i++) { block }
#define FFOR(i, mini, maxi, j, minj, maxj, block) FOR(i, mini, maxi, FOR(j, minj, maxj, block))

#define MESSAGE(mess) printf("%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, mess)
#define IF_PERROR(cond, mess, post) if(cond) { perror(mess); MESSAGE(mess); post; return NULL; }
#define IF_RNULL(cond, mess, post) if(cond) { MESSAGE(mess); post; return NULL; }

#define MALLOC(var, size, post) var = malloc(size); IF_PERROR(!var, "malloc failed", post)
#define REALLOC(var, tmp, size, post) tmp = realloc(var, size); IF_PERROR(!tmp, "realloc failed", post) else { var = tmp; }

#define ASSIGN_IF(var, cond, v1, v2) if(cond) { var = v1; } else { var = v2; }

#define TEST_INIT(name) int test_count = 0; char *test_name = name;
#define TEST(funcname) { printf("%s : ", test_name); int rc = test_##funcname(); printf("test_"#funcname); if(rc != 0) { test_count++; puts(" ==> KO"); } else { puts(" ==> OK"); } }
#define TESTA(funcname, arg) { printf("%s : ", test_name); int rc = test_##funcname(arg); printf("test_"#funcname"("arg")"); if(rc != 0) { test_count++; puts(" ==> KO"); } else { puts(" ==> OK"); } }
#define TEST_RESULTS() {\
	if(test_count == 0) { fprintf(stdout, "%s : all tests OK\n", test_name); return 0; }\
	else { fprintf(stdout, "%s : %d tests KO\n", test_name, test_count); return 1; }\
}


#endif
