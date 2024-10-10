#ifndef MACROS_H
#define MACROS_H

#define FOR(i, min, max, block) for(size_t i = min; i < max; i++) { block }
#define FFOR(i, mini, maxi, j, minj, maxj, block) FOR(i, mini, maxi, FOR(j, minj, maxj, block))

#define MESSAGE(mess) printf("%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, mess)
#define IF_PERROR(cond, mess, post) if(cond) { perror(mess); MESSAGE(mess); post }
#define IF_RNULL(cond, mess, post) if(cond) { MESSAGE(mess); post; return NULL; }
#define MALLOC(var, size, post) var = malloc(size); IF_PERROR(!var, "malloc failed", post)

#define REALLOC(var, size, post) { \
	void *tmp = realloc(var, size); \
	if(tmp) { var = tmp; } \
	else { perror("realloc failed"); MESSAGE("realloc failed"); post } \
}

#define ASSIGN_IF(var, cond, v1, v2) if(cond) { var = v1; } else { var = v2; }


#define TEST(funcname) puts("test_"#funcname); test_##funcname()
#define TESTA(funcname, arg) puts("test_"#funcname"("arg")"); test_##funcname(arg)

#define ARRAY_OF_PTR_FREE(array) while(*array) { free(*array++); } free(array)
#define ARRAY_OF_PTR_FREES(array, size) for(size_t i = 0; i < size; i++) { free(array[i]); } free(array)


#endif
