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
	#define LOG(LG_stream, LG_fmt, ...) \
		fprintf(LG_stream, "%s:%s:%d:" LG_fmt, __FILE__, __func__, __LINE__, __VA_ARGS__);
		
	#define LOG_E(LGE_fmt, ...) \
		LOG(stderr, LGE_fmt, __VA_ARGS__)
	
	#define LOG_S(LGS_fmt, ...) \
		LOG(stdout, LGS_fmt, __VA_ARGS__)

	#define LOG_MESS(LGM_stream, LGM_mess) \
		fprintf(LGM_stream, "%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, LGM_mess);
	
	#define LOG_E(LGEM_mess) \
		LOG_MESS(stderr, LGEM_mess)
	
	#define LOG_S(LGSM_mess) \
		LOG_MESS(stdout, LGSM_mess)

	#define LOG_E_STR_ERRNO() \
		LOG_E(strerror(errno));

	#define IF_ERR(IFE_cond, IFE_post) \
		if(IFE_cond) { \
			LOG_E_STR_ERRNO(); \
			IFE_post; \
		}
	
	#define IF_LOG_E_MESS(IFLGEM_cond, IFLGEM_post, IFLGEM_mess) \
		if(IFLGEM_cond) { \
			LOG_E(IFLGEM_mess); \
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
	#define MALLOC(MC_var, MC_size, MC_posterror, MC_postsuccess) \
		MC_var = malloc(MC_size); \
		if(!MC_var) { \
			LOG_E_STR_ERRNO() \
			LOG_E("malloc failed for "#MC_var); \
			MC_posterror; \
		} \
		MC_postsuccess; \
		
	#define CALLOC(CAC_var, CAC_nmemb, CAC_size, CAC_posterror, CAC_postsuccess) \
		CAC_var = calloc(CAC_nmemb, CAC_size); \
		if(!CAC_var) { \
			LOG_E_STR_ERRNO() \
			LOG_E("calloc failed for "#CAC_var); \
			CAC_posterror; \
		} \
		CAC_postsuccess; \
	
	#define REALLOC(RC_var, RC_size, RC_posterror, RC_postsuccess) { \
		void *tmp = realloc(RC_var, RC_size); \
		if(!tmp) { \
			LOG_E_STR_ERRNO() \
			LOG_E("realloc failed for "#RC_var); \
			RC_posterror; \
		} \
		RC_var = tmp; \
		RC_postsuccess; \
	}


	/* 
		arrays
	*/
	#define ARRAY_REALLOC(AR_array_ptr, AR_count, AR_capacity_var, AR_posterror) { \
		if(AR_count == AR_capacity_var) { \
			REALLOC(AR_array_ptr, (AR_capacity_var) * 2 * sizeof(*(AR_array_ptr)), AR_posterror, (AR_capacity_var) *= 2) \
		} \
	}

	#define ARRAY_ADD(AA_array_ptr, AA_count_var, AA_item_ptr) { \
		memcpy(&(AA_array_ptr)[AA_count_var], AA_item_ptr, sizeof(*(AA_item_ptr))); \
		(AA_count_var)++; \
	}

	#define ARRAY_SHIFT_L(ASL_array_ptr, ASL_count, ASL_idx) { \
		for(size_t i = ASL_idx; i < ASL_count - 1; i++) { \
			memcpy(&(ASL_array_ptr)[i], &(ASL_array_ptr)[i + 1], sizeof(*(ASL_array_ptr))); \
		} \
	}

	#define ARRAY_DEL(AD_array_ptr, AD_count_var, AD_idx) { \
		ARRAY_SHIFT_L(AD_array_ptr, AD_count_var, AD_idx) \
		(AD_count_var)--; \
	}
	
	#define ARRAY_SHIFT_R(ASR_array_ptr, ASR_count, ASR_idx) { \
		for(size_t i = ASR_count; i > ASR_idx; i--) { \
			memcpy(&(ASR_array_ptr)[i], &(ASR_array_ptr)[i - 1], sizeof(*(ASR_array_ptr))); \
		} \
	}


	
#endif
