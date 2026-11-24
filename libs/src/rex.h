#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "arrayh.h"
#include "macros.h"

//TODO: replace content with tokens

typedef enum token_type {
	TT_NONE = 0,
	TT_GLOBAL,
	TT_IDENT,
	TT_TAG_OPEN,
	TT_TAG_CLOSE,
	TT_TAGNAME,
	TT_CONTENT,
	TT_TEXT,
	TT_VALUE,
	TT_VALUE_INT,
	TT_VALUE_STRING,
	TT_VALUE_FLOAT,
	TT_OPERATOR,
	TT_ANY_STRING,
} token_type;

typedef struct token {
	char *content;
	int type;
	char pad[4];
} token;

token *token_new(int type, char *start, size_t len);

typedef struct group {
	token **tokens;		// array of token *
	struct group **children;	// array of group *
} group;

group *group_new();
void group_free(group *g);
void group_print(group *g, size_t n);




#ifdef DEBUG
	#define READ(group, token_type, res, input_string, f, min, max, ...) { \
		res = true; \
		char *MRN_p = *input_string; \
		char *MRN_start = MRN_p; \
		 \
		int MRN_i = 0; \
		for(; ; MRN_i++) { \
			if(min == max && min >= 0 && MRN_i == min) { \
				break; \
			} \
			if(!f(group, &MRN_p VA_ARGS(__VA_ARGS__))) { \
				break; \
			} \
		} \
		\
		if(min >= 0 && MRN_i < min) { \
			res = false; \
		} \
		else if(max >= 0 && MRN_i > max) { \
			res = false; \
		} \
		else { \
			*input_string = MRN_p; \
		} \
		if(res) { \
			if(MRN_p - MRN_start == 0) { \
				LOG_S("[%s][%d][%d][%d][%s], nogroup for empty match\n", #f, min, MRN_i, max, "true"); \
			} \
			else if(token_type >= 0) { \
				token *t = token_new(token_type, MRN_start, MRN_p - MRN_start); \
				LOG_S("[%s][%d][%d][%d][%s], group=[%s]\n", #f, min, MRN_i, max, "true", t->content); \
				ARRAYH_ADD(group->tokens, t, return false); \
			} \
		} \
		else { \
			LOG_S("[%s][%d][%d][%d][%s]\n", #f, min, MRN_i, max, "false"); \
		} \
	}
#else
	#define READ(group, token_type, res, input_string, f, min, max, ...) { \
		res = true; \
		char *MRN_p = *input_string; \
		char *MRN_start = MRN_p; \
		 \
		int MRN_i = 0; \
		for(; ; MRN_i++) { \
			if(min == max && min >= 0 && MRN_i == min) { \
				break; \
			} \
			if(!f(group, &MRN_p VA_ARGS(__VA_ARGS__))) { \
				break; \
			} \
		} \
		 \
		if(min >= 0 && MRN_i < min) { \
			res = false; \
		} \
		else if(max >= 0 && MRN_i > max) { \
			res = false; \
		} \
		else { \
			*input_string = MRN_p; \
		} \
		if(res) {\
			if(MRN_p - MRN_start > 0 && token_type >= 0) { \
				token *t = token_new(token_type, MRN_start, MRN_p - MRN_start); \
				ARRAYH_ADD(group->tokens, t, return false); \
			} \
		} \
	}
#endif




bool read_string(group *g, char **s, char *string);
bool read_digit(group *g, char **s);
bool read_alpha(group *g, char **s);
bool read_alnum(group *g, char **s);
bool read_ident_first(group *g, char **s);
bool read_ident_other(group *g, char **s);
bool read_whitespace(group *g, char **s);
bool read_inside_string_first(group *g, char **s);
bool read_inside_string_other(group *g, char **s);
bool read_ident(group *g, char **s);
bool read_ident_ws(group *g, char **s);
bool read_value_int(group *g, char **s);
bool read_value_float(group *g, char **s);
bool read_value_string(group *g, char **s);
bool read_value(group *g, char **s);


bool read_c_type(group *g, char **s);
bool read_c_vardeclassign(group *g, char **s);

bool read_xml_tag_name(group *g, char **s);
bool read_xml_tag_open(group *g, char **s);
bool read_xml_tag_close(group *g, char **s, char *name);
bool read_xml_content(group *g, char **s);
bool read_xml(group *g, char **s);
