#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef MACROS_H
	#define MACROS_H
	
	/* 
		- every array variable has to be typed (no generic void * or char *).
		
		- every variable name suffixed with _var has to be a variable (not a value). Otherwise, can be both.
		
		- every variable name suffixed with _ptr is a pointer.
		
		- ARRAY_SHIFT_L and ARRAY_SHIFT_R could be improved by a double memcpy using a temp buffer
	*/

	#define VA_ARGS(...) , ##__VA_ARGS__

	#define STR_EVALUATE(x)   #x
	#define STRINGIFY(x)      STR_EVALUATE(x)


	/*
		walks through
	*/
	#define FOR(FOR_i_var, FOR_min, FOR_max, FOR_block) \
		for(size_t FOR_i_var = FOR_min; FOR_i_var < FOR_max; FOR_i_var++) { \
			FOR_block; \
		}
	
	#define FFOR(FFOR_i_var, FFOR_mini, FFOR_maxi, FFOR_j_var, FFOR_minj, FFOR_maxj, FFOR_block) \
		FOR(FFOR_i_var, FFOR_mini, FFOR_maxi, \
			FOR(FFOR_j_var, FFOR_minj, FFOR_maxj, FFOR_block))

	
	/*
		assignments
	*/
	#define ASSIGN_IF(AI_var, AI_cond, AI_v1, AI_v2) \
		if(AI_cond) { \
			AI_var = AI_v1; \
		} \
		else { \
			AI_var = AI_v2; \
		}
	
	
	/*
		logging
	*/
	#define LOG(LG_stream, ...) {\
		fprintf(LG_stream, "%s:%s:%d:", __FILE__, __func__, __LINE__); \
		fprintf(LG_stream, __VA_ARGS__); \
	}
	
	#define LOG_E(...) \
		LOG(stderr, __VA_ARGS__)
	
	#define LOG_S(...) \
		LOG(stdout, __VA_ARGS__)

	#define LOG_MESS(LG_stream, mess) \
		LOG(LG_stream, "%s", mess)
	
	#define LOG_S_MESS(mess) \
		LOG_MESS(stdout, mess)
	
	#define LOG_E_MESS(mess) \
		LOG_MESS(stderr, mess)

	#define LOG_E_STR_ERRNO() \
		LOG_E_MESS(strerror(errno))

	#define IF_ERR(IFE_cond, IFE_post) \
		if(IFE_cond) { \
			LOG_E_STR_ERRNO(); \
			IFE_post; \
		}
	
	#define IF_LOG_E_MESS(IFLGEM_cond, IFLGEM_post, IFLGEM_mess) \
		if(IFLGEM_cond) { \
			LOG_E_MESS(IFLGEM_mess); \
			IFLGEM_post; \
		}
	
	#define IF_LOG_E(IFLGE_cond, IFLGE_post, IFLGE_fmt, ...) \
		if(IFLGE_cond) { \
			LOG_E(IFLGE_fmt, __VA_ARGS__); \
			IFLGE_post; \
		}
	
	
	/*
		files
	*/
	#define FOPEN(FO_fd, FO_path, FO_mode, FO_postrror, FO_postsuccess) \
		FO_fd = fopen(FO_path, FO_mode); \
		if(!FO_fd) { \
			LOG_E("failed to open file [%s]\n", FO_path); \
			LOG_E_STR_ERRNO(); \
			FO_postrror; \
		} \
		FO_postsuccess; \

		
	#define OPEN(O_fd, O_path, O_flags, O_postrror, O_success) \
		O_fd = open(O_path, O_flags); \
		if(!O_fd) { \
			LOG_E("failed to open file [%s]\n", O_path); \
			LOG_E_STR_ERRNO() \
			O_postrror; \
		} \
		O_success; \
		
	
	/*
		memory
	*/
	#define MALLOC(MC_var, size, posterror, postsuccess) \
		MC_var = malloc(size); \
		if(!MC_var) { \
			LOG_E_STR_ERRNO() \
			LOG_E_MESS("malloc failed for "#MC_var); \
			posterror; \
		} \
		postsuccess; \
		
	#define CALLOC(CAC_var, CAC_nmemb, CAC_size, posterror, postsuccess) \
		CAC_var = calloc(CAC_nmemb, CAC_size); \
		if(!CAC_var) { \
			LOG_E_STR_ERRNO() \
			LOG_E_MESS("calloc failed for "#CAC_var); \
			posterror; \
		} \
		postsuccess; \
	
	#define REALLOC(RC_var, size, posterror, postsuccess) { \
		void *tmp = realloc(RC_var, size); \
		if(!tmp) { \
			LOG_E_STR_ERRNO(); \
			LOG_E_MESS("realloc failed for "#RC_var); \
			posterror; \
		} \
		RC_var = tmp; \
		postsuccess; \
	}


	/* 
		arrays
	*/
	#define ARRAY_REALLOC(array_var, array_count, capacity_ptr, posterror) { \
		if(array_count == capacity_ptr) { \
			REALLOC(array_var, (capacity_ptr) * 2 * sizeof(*array_var), posterror, (capacity_ptr) *= 2) \
		} \
	}

	#define ARRAY_ADD(array_ptr, array_count_ptr, item_ptr) { \
		memcpy(&(array_ptr)[array_count_ptr], &(item_ptr), sizeof(item_ptr)); \
		(array_count_ptr)++; \
	}

	#define ARRAY_SHIFT_L(array_ptr, array_count, start_idx) { \
		for(size_t i = start_idx; i < array_count - 1; i++) { \
			memcpy(&(array_ptr)[i], &(array_ptr)[i + 1], sizeof(*(array_ptr))); \
		} \
	}

	#define ARRAY_DEL(array_ptr, array_count_ptr, idx) { \
		ARRAY_SHIFT_L(array_ptr, array_count_ptr, idx) \
		(array_count_ptr)--; \
	}
	
	#define ARRAY_SHIFT_R(array_ptr, array_count, start_idx) { \
		for(size_t i = array_count; i > start_idx; i--) { \
			memcpy(&(array_ptr)[i], &(array_ptr)[i - 1], sizeof(*(array_ptr))); \
		} \
	}
	
	#define ARRAY_FREE(array_ptr, len) { \
		for(size_t i = 0; i < len; i++) { \
			if((array_ptr)[i] != NULL) { \
				free((array_ptr)[i]); \
			} \
		} \
		free((array_ptr)); \
	}
	
	#define ARRAY_FREE_F(array_ptr, len, free_func) { \
		for(size_t i = 0; i < len; i++) { \
			if((array_ptr)[i] != NULL) { \
				(free_func)((array_ptr)[i]); \
			} \
		} \
		free(array_ptr); \
	}
	
#endif
