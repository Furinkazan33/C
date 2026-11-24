#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "rex.h"

token *token_new(int type, char *start, size_t len) {
	token *res;
	MALLOC(res, sizeof(*res), return NULL, );
	
	res->type = type;
	res->content = strndup(start, len); 
	if(!res->content) {
		LOG_S("failed to copy content");
		return NULL;
	}
	
	return res;
}

void token_free(token *t) {
	free(t->content);
	free(t);
}

group *group_new() {
	group *res = malloc(sizeof(group));
	ARRAYH_NEW(res->tokens, 5, return NULL, );
	ARRAYH_NEW(res->children, 5, return NULL, );
	return res;
}

void group_free(group *g) {
	FOR(i, 0, ARRAYH_CNT(g->tokens), token_free(g->tokens[i]))
	ARRAYH_FREE(g->tokens);

	if(g->children) {
		FOR(i, 0, ARRAYH_CNT(g->children), group_free(g->children[i]))
		ARRAYH_FREE(g->children);
	}
	free(g);
}


const char *token_type_tos(token_type type) {
	switch(type) {
		default:
		case TT_NONE:
			return "NONE";
		case TT_GLOBAL:
			return "GLOBAL";
		case TT_IDENT:
			return "IDENT";
		case TT_TAG_OPEN:
			return "TAG_OPEN";
		case TT_TAG_CLOSE:
			return "TAG_CLOSE";
		case TT_TAGNAME:
			return "TAGNAME";
		case TT_VALUE:
			return "VALUE";
		case TT_VALUE_INT:
			return "VALUE_INT";
		case TT_VALUE_STRING:
			return "VALUE_STRING";
		case TT_VALUE_FLOAT:
			return "VALUE_FLOAT";
		case TT_OPERATOR:
			return "OPERATOR";
		case TT_ANY_STRING:
			return "ANY_STRING";
		case TT_CONTENT:
			return "CONTENT";
		case TT_TEXT:
			return "TEXT";
	}
}

void group_print(group *g, size_t n) {
	for(size_t idx = 0; idx < ARRAYH_CNT(g->tokens); idx++) {
		for(size_t i = 0; i < n; i++) { 
			printf("  "); 
		}
		printf("[%ld][%s][%s]\n", idx, token_type_tos(g->tokens[idx]->type), g->tokens[idx]->content);
	}
	
	for(size_t idx = 0; idx < ARRAYH_CNT(g->children); idx++) {
		group_print(g->children[idx], n + 1);
	}
}

bool read_string(group *g, char **s, char *string) {
	(void) g;
	char *s_tmp = *s;
	
	while(*s_tmp == *string) {
		s_tmp++;
		string++;
	}
	if(*string == '\0') {
		*s = s_tmp;
		return true;
	}
	else {
		return false;
	}
}

bool read_digit(group *g, char **s) {
	(void) g;

	if(isdigit(**s)) {
		(*s)++;
		return true;
	}
	else {
		return false;
	}
}

bool read_alpha(group *g, char **s) {
	(void) g;
	if(isalpha(**s)) {
		(*s)++;
		return true;
	}
	else {
		return false;
	}
}

bool read_alnum(group *g, char **s) {
	(void) g;
	if(isalnum(**s)) {
		(*s)++;
		return true;
	}
	else {
		return false;
	}
}

bool read_ident_first(group *g, char **s) {
	(void) g;
	if(isalpha(**s) || **s == '_') {
		(*s)++;
		return true;
	}
	else {
		return false;
	}
}

bool read_ident_other(group *g, char **s) {
	(void) g;
	if(isalnum(**s) || **s == '_') {
		(*s)++;
		return true;
	}
	else {
		return false;
	}
}


bool read_whitespace(group *g, char **s) {
	(void) g;
	if(isspace(**s)) {
		(*s)++;
		return true;
	}
	else {
		return false;
	}
}

bool read_inside_string_first(group *g, char **s) {
	(void) g;
	if(**s != '"') {
		(*s)++;
		return true;
	}
	else {
		return false;
	}
}

bool read_inside_string_other(group *g, char **s) {
	(void) g;
	if(**s != '"' || (**s == '"' && *(*s - 1) == '\\')) {
		(*s)++;
		return true;
	}
	else {
		return false;
	}
}

bool read_ident(group *g, char **s) {
	bool res;
	char *s_tmp = *s;
	
	READ(g, -1, res, &s_tmp, read_ident_first, 1, -1)
	if(res) { 
		READ(g, -1, res, &s_tmp, read_ident_other, 0, -1)
		if(res) {
			*s = s_tmp;
			return true;
		}
	}
	
	return false;
}

bool read_ident_ws(group *g, char **s) {
	bool res;
	char *s_tmp = *s;
	
	READ(g, TT_IDENT, res, &s_tmp, read_ident, 1, 1) 
	if(res) {
		READ(g, -1, res, &s_tmp, read_whitespace, 1, -1)
		if(res) {
			*s = s_tmp;
			return true;
		}
	}
	
	return false;
}

bool read_c_type(group *g, char **s) {
	bool res;
	char *s_tmp = *s;
	
	READ(g, TT_IDENT, res, &s_tmp, read_ident, 1, 1); if(!res) return false;
	READ(g, -1, res, &s_tmp, read_whitespace, 0, -1);
	READ(g, TT_OPERATOR, res, &s_tmp, read_string, 0, 3, "*");
	
	*s = s_tmp;
	
	return true;
}

bool read_value_int(group *g, char **s) {
	bool res;
	
	READ(g, -1, res, s, read_digit, 1, -1)
	
	return res;
}

bool read_value_float(group *g, char **s) {
	bool res;
	char *s_tmp = *s;
	
	READ(g, -1, res, &s_tmp, read_string, 1, 1, ".")
	if(res) {
		READ(g, -1, res, &s_tmp, read_value_int, 1, 1)
	}
	else {
		READ(g, -1, res, &s_tmp, read_value_int, 1, 1) if(!res) return false;
		READ(g, -1, res, &s_tmp, read_string, 1, 1, ".") if(!res) return false;
		READ(g, -1, res, &s_tmp, read_value_int, 0, 1)
	}
	
	if(res) {
		*s = s_tmp;
		return true;
	}
	
	return false;
}

bool read_value_string(group *g, char **s) {
	bool res;
	char *s_tmp = *s;
	
	READ(g, -1, res, &s_tmp, read_string, 1, 1, "\"") if(!res) return false;
	READ(g, -1, res, &s_tmp, read_inside_string_first, 0, -1)
	READ(g, -1, res, &s_tmp, read_inside_string_other, 0, -1)
	READ(g, -1, res, &s_tmp, read_string, 1, 1, "\"")

	if(res) {
		*s = s_tmp;
		return true;
	}
	
	return false;
}

bool read_value(group *g, char **s) {
	bool res;
	char *s_tmp = *s;
	
	READ(g, TT_VALUE_FLOAT, res, &s_tmp, read_value_float, 1, 1) 
	if(res) {
		*s = s_tmp;
		return true;
	}
	
	READ(g, TT_VALUE_INT, res, &s_tmp, read_value_int, 1, 1) 
	if(res) {
		*s = s_tmp;
		return true;
	}
	
	READ(g, TT_VALUE_STRING, res, &s_tmp, read_value_string, 1, 1)
	if(res) {
		*s = s_tmp;
		return true;
	}
	
	return false;
}

bool read_c_vardeclassign(group *g, char **s) {
	bool res;
	char *s_tmp = *s;
	
	READ(g, TT_IDENT, res, &s_tmp, read_ident, 1, 1); if(!res) return false;
	
	while(res) {
		READ(g, -1, res, &s_tmp, read_whitespace, 1, -1);
		if(res) READ(g, TT_IDENT, res, &s_tmp, read_ident, 1, 1);
	}
	
	READ(g, TT_OPERATOR, res, &s_tmp, read_string, 0, 3, "*");
	if(res) READ(g, TT_IDENT, res, &s_tmp, read_ident, 1, 1); if(!res) return false;
	
	READ(g, TT_OPERATOR, res, &s_tmp, read_string, 1, 1, "=");
	if(res) {
		READ(g, -1, res, &s_tmp, read_whitespace, 0, -1);
		READ(g, -1, res, &s_tmp, read_value, 1, 1); if(!res) return false;
	}
	
	if(res) {
		*s = s_tmp;
		return true;
	}
	
	return false;
}


bool read_xml_tag_name(group *g, char **s) {
	(void) g;
	
	if(!isalpha(**s)) return false;
	
	while(isalpha(**s)) {
		(*s)++;
	}

	return true;
}

bool read_xml_property(group *g, char **s) {
	bool res;
	char *s_tmp = *s;
	
	READ(g, TT_IDENT, res, &s_tmp, read_ident, 1, 1); if(!res) return false;
	
	READ(g, TT_OPERATOR, res, &s_tmp, read_string, 1, 1, "="); if(!res) return false;
	
	READ(g, -1, res, &s_tmp, read_value, 1, 1); if(!res) return false;

	*s = s_tmp;

	return true;
}

bool read_xml_tag_open(group *g, char **s) {
	bool res;
	char *s_tmp = *s;
	
	if(**s != '<') return false;
	s_tmp++;
	
	READ(g, TT_TAGNAME, res, &s_tmp, read_xml_tag_name, 1, 1); if(!res) return false;
	
	READ(g, -1, res, &s_tmp, read_whitespace, 0, -1);
	
	READ(g, -1, res, &s_tmp, read_xml_property, 0, -1); if(!res) return false;
	
	if(*s_tmp != '>') return false;
	s_tmp++;
	
	*s = s_tmp;
	
	return true;
}

bool read_xml_tag_close(group *g, char **s, char *name) {
	(void) g;
	char *s_tmp = *s;
	
	if(*s_tmp != '<') return false;
	s_tmp++;
	
	if(*s_tmp != '/') return false;
	s_tmp++;
	
	if(strncmp(s_tmp, name, strlen(name)) != 0) return false;
	s_tmp+=strlen(name);
	
	if(*s_tmp != '>') return false;
	s_tmp++;
	
	*s = s_tmp;
	
	return true;
}

bool read_xml_content(group *g, char **s) {
	(void) g;
	char *s_tmp = *s;
	
	while(*s_tmp != '<') {
		s_tmp++;
	}
	
	if(*s != s_tmp) {
		*s = s_tmp;
		return true;
	}
	
	return false;
}

bool read_xml(group *g, char **s) {
	bool res;
	char *s_tmp = *s;
	
	READ(g, TT_TAG_OPEN, res, &s_tmp, read_xml_tag_open, 1, 1); if(!res) return false;
	char *tagname = g->tokens[0]->content;
	
	while(res) {
		group *child = group_new();
		ARRAYH_ADD(g->children, child, return false);
		
		READ(child, TT_TEXT, res, &s_tmp, read_xml_content, 1, 1);
		
		if(!res) {
			READ(child, TT_GLOBAL, res, &s_tmp, read_xml, 1, 1);
		}
	}
	
	READ(g, TT_TAG_CLOSE, res, &s_tmp, read_xml_tag_close, 1, 1, tagname); if(!res) return false;
	
	*s = s_tmp;
	
	return true;
}


