#include <stdio.h>
#include <stdlib.h>

#ifndef MACROS_H
#define MACROS_H

#define FOR(i, min, max, block) for(size_t i = min; i < max; i++) { block }
#define FFOR(i, mini, maxi, j, minj, maxj, block) FOR(i, mini, maxi, FOR(j, minj, maxj, block))

#define LOG(stream, mess, pre) fprintf(stream, "%s%s:%s:%d:%s\n", pre, __FILE__, __func__, __LINE__, mess)
#define LOGE(mess, pre) LOG(stderr, mess, pre)
#define IF_PERROR(cond, mess, pre, post) if(cond) { perror("perror"); LOG(stderr, mess, pre); post; }
#define IF_LOG(cond, stream, mess, pre, post) if(cond) { LOG(stream, mess, pre); post; }
#define IF_LOGE(cond, mess, pre, post) IF_LOG(cond, stderr, mess, pre, post)

#define MALLOC(var, size, pre, post) var = malloc(size); IF_PERROR(!var, "malloc failed for "#var, pre, post)
#define REALLOC(var, size, pre, post) { void *tmp = realloc(var, size); IF_PERROR(!tmp, "realloc failed", pre, post) else { var = tmp; } }

#define ASSIGN_IF(var, cond, v1, v2) if(cond) { var = v1; } else { var = v2; }

#define ARRAY_ALLOC(a, n, post) MALLOC(a, sizeof(*a) * n, post);
#define ARRAY_SET(a, i, v) memcpy(a + i, v, sizeof(*a))
#define ARRAY_DO(a, n, ptr, block) { void *ptr = NULL; FOR(i, 0, n, ptr = a + i; block;); }


#define TEST_INIT(name) int test_count = 0; char *test_name = name;
#define TEST(funcname) { printf("%s : ", test_name); int rc = test_##funcname(); printf("test_"#funcname); if(rc != 0) { test_count++; puts(" ==> KO"); } else { puts(" ==> OK"); } }
#define TESTA(funcname, arg) { printf("%s : ", test_name); int rc = test_##funcname(arg); printf("test_"#funcname"("arg")"); if(rc != 0) { test_count++; puts(" ==> KO"); } else { puts(" ==> OK"); } }
#define TEST_RESULTS() {\
	if(test_count == 0) { fprintf(stdout, "%s : all tests OK\n", test_name); return 0; }\
	else { fprintf(stdout, "%s : %d tests KO\n", test_name, test_count); return 1; }\
}


#endif
