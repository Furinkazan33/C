#include <stdio.h>
#include <stdlib.h>

#ifndef MACROS_H
#define MACROS_H

#define FOR(i, min, max, block) for(size_t i = min; i < max; i++) { block }
#define FFOR(i, mini, maxi, j, minj, maxj, block) FOR(i, mini, maxi, FOR(j, minj, maxj, block))

#define LOG(stream, mess) fprintf(stream, "%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, mess);
#define LOGE(mess) LOG(stderr, mess);
#define LOGS(mess) LOG(stdout, mess);

#define PERROR(var, mess) perror(mess); LOGE(mess " for "#var);
#define MALLOC(var, size, post) var = malloc(size); if(!var) { perror("malloc failed"); LOGE("malloc failed for "#var); post; }
#define REALLOC(var, size, post) { void *tmp = realloc(var, size); if(!tmp) { perror("realloc failed"); LOGE("realloc failed for "#var); post; } else { var = tmp; } }

#define ASSIGN_IF(var, cond, v1, v2) if(cond) { var = v1; } else { var = v2; }

#define ARRAY_ALLOC(a, n, post) MALLOC(a, sizeof(*a) * n, post);
#define ARRAY_SET(a, i, v) memcpy(a + i, v, sizeof(*a))
#define ARRAY_DO(a, n, ptr, block) { void *ptr = NULL; FOR(i, 0, n, ptr = a + i; block;); }


#define TEST_INIT(name) int test_count = 0; char *test_name = name;
#define TEST(id) { printf("%s : ", test_name); int rc = test_##id(); printf("test_"#id); if(rc != 0) { test_count++; puts(" ==> KO"); } else { puts(" ==> OK"); } }
#define TESTA(id, arg) { \
	printf("%s : ", test_name); \
	int rc = test_##id(arg); \
	printf("test_%d(%s)", id, arg); \
	if(rc != 0) { test_count++; puts(" ==> KO"); } else { puts(" ==> OK"); } }

#define TESTB(id, arg1, arg2) { \
	printf("%s : ", test_name); \
	int rc = test_##id(arg1, arg2); \
	printf("test_%d(%s, %s)", id, arg1, arg2); \
	if(rc != 0) { test_count++; puts(" ==> KO"); } else { puts(" ==> OK"); } }

#define TEST_RESULTS() {\
	if(test_count == 0) { fprintf(stdout, "%s : all tests OK\n", test_name); return 0; }\
	else { fprintf(stdout, "%s : %d tests KO\n", test_name, test_count); return 1; }\
}


#endif
