#include <stdio.h>
#include "macros.h"

/* 
	array helpers with 1 malloc consisting of a header (struct arrayh) followed by the array.

	- usage example:
		person *array;
		ARRAYH_NEW(array, 1, return 1, 
		{
			assert(ARRAYH_CNT(array) == 0);
			assert(ARRAYH_CAP(array) == 1);
		})
	
	- ARRAYH_DEL and ARRAYH_DEL_IDX could be improved (see ARRAY_SHIFT_L and ARRAY_SHIFT_R).
*/

#ifndef ARRAYH_H
	#define ARRAYH_H

	typedef struct arrayh {
		size_t count;
		size_t capacity;
		char pad[8];
	} arrayh;


	#define ARRAYH_HEAD(array_ptr) (&(((arrayh *)(array_ptr))[-1]))
	
	#define ARRAYH_CAP(array_ptr) ARRAYH_HEAD(array_ptr)->capacity
	
	#define ARRAYH_CNT(array_ptr) ARRAYH_HEAD(array_ptr)->count

	#define ARRAYH_NEW(array_var, capacity_var, posterror, postsuccess) { \
		arrayh *AHN_varname_tmp; \
		MALLOC(AHN_varname_tmp, sizeof(arrayh) + (capacity_var) * sizeof(*array_var), posterror, { \
			array_var = (void *) (AHN_varname_tmp + 1); \
			ARRAYH_CNT(array_var) = 0; \
			ARRAYH_CAP(array_var) = capacity_var; \
		}) \
		memset(array_var, 0, (capacity_var) * sizeof(*array_var)); \
		postsuccess; \
	}
	
	#define ARRAYH_FREE(array_ptr) free(ARRAYH_HEAD(array_ptr))

	#define ARRAYH_LOG(array_ptr) LOG_S("capacity=[%ld], count=[%ld], item size=[%ld]\n", ARRAYH_CAP(array_ptr), ARRAYH_CNT(array_ptr), sizeof(*(array_ptr)))
	
	#define ARRAYH_REALLOC(array_var, posterror) { \
		if(ARRAYH_CNT(array_var) == ARRAYH_CAP(array_var)) { \
			size_t AHR_old_size = ARRAYH_CAP(array_var); \
			size_t AHR_new_size = AHR_old_size * 2; \
			arrayh *h = ARRAYH_HEAD(array_var); \
			REALLOC(h, sizeof(arrayh) + AHR_new_size * sizeof(*array_var), posterror, );\
			array_var = (void *) (h + 1); \
			ARRAYH_CAP(array_var) = AHR_new_size; \
			memset(array_var + AHR_old_size, 0, (AHR_new_size - AHR_old_size) * sizeof(*array_var)); \
		} \
	}

	#define ARRAYH_ADD(array_var, item_var, posterror) { \
		ARRAYH_REALLOC(array_var, posterror); \
		ARRAY_ADD(array_var, ARRAYH_CNT(array_var), item_var) \
	}
	
	#define ARRAYH_DEL(array_ptr, item_ptr) { \
		ARRAYH_DEL_IDX(array_ptr, ((char *) (item_ptr) - (char *) (array_ptr)) / sizeof(*(array_ptr))); \
	}

	#define ARRAYH_DEL_IDX(array_ptr, idx_var) { \
		ARRAY_SHIFT_L(array_ptr, ARRAYH_CNT(array_ptr), idx_var) \
		ARRAYH_CNT(array_ptr)--; \
	}
	
	#define ARRAYH_DO(array_ptr, i_var, block) { \
		for(size_t i_var = 0; i_var < ARRAYH_CNT(array_ptr); i_var++) { \
			block; \
		} \
	}
	
	#define ARRAYH_MAP(array_ptr, map_fun) { \
		for(size_t i = 0; i < ARRAYH_CNT(array_ptr); i++) { \
			map_fun((array_ptr)[i]); \
		} \
	}
	
	#define ARRAYH_MAP_ARGS(array_ptr, map_fun, ...) { \
		for(size_t i = 0; i < ARRAYH_CNT(array_ptr); i++) { \
			map_fun((array_ptr)[i], __VA_ARGS__); \
		} \
	}
	
	#define ARRAYH_MAP_PTR(array_ptr, map_fun) { \
		for(size_t i = 0; i < ARRAYH_CNT(array_ptr); i++) { \
			map_fun(*((array_ptr)[i])); \
		} \
	}
	
	#define ARRAYH_MAP_PTR_ARGS(array_ptr, map_fun, ...) { \
		for(size_t i = 0; i < ARRAYH_CNT(array_ptr); i++) { \
			map_fun(*((array_ptr)[i]), __VA_ARGS__); \
		} \
	}
	
	#define ARRAYH_FIND(array_ptr, search_var, cmp_func, result_var) { \
		result_var = NULL; \
		for(size_t i = 0; i < ARRAYH_CNT(array_ptr); i++) { \
			if(!(cmp_func)(search_var, (array_ptr)[i])) { \
				result_var = (array_ptr)[i]; \
				break; \
			} \
		} \
	}

#endif
