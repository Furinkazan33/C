#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "file.h"
#include "macros.h"


char *file_readline_new_string(FILE *f) {
	char *res;
	size_t alloc_len = 128;
	MALLOC(res, sizeof(char) * (alloc_len + 1), LOG_E_MESS("failed to alloc string\n"), )
	
	int c;
	size_t read = 0;
	
	while((c = fgetc(f)) != EOF) {
		if(read == alloc_len) {
			alloc_len *= 2;
			REALLOC(res, sizeof(char) * (alloc_len + 1), LOG_E_MESS("failed to realloc string\n"), )
		}
		
		res[read++] = c;
		
		if(c == '\n') {
			break;
		}
	}
	
	res[read] = '\0';
	
	return res;
}

size_t file_readline_to(FILE *f, char *dest, size_t size) {
	size_t i = 0;

	 int c;	
	while((c = fgetc(f)) != EOF && i < size) {
		dest[i++] = c;

		if(c == '\n') {
			break;
		}
	}
	dest[i] = '\0';

	return i;
}

// result as an arrayh
char *file_readline(FILE *f, char *arrayh_dest) {
	ARRAYH_CNT(arrayh_dest) = 0;
	int c;
	while((c = fgetc(f)) != EOF) {
		ARRAYH_ADD(arrayh_dest, c, return NULL);

		if(c == '\n') {
			break;
		}
	}
	c = '\0';
	ARRAYH_ADD(arrayh_dest, c, return NULL);
	
	return arrayh_dest;
}


char *file_readfile(FILE *f, size_t *len) {
	char *res;

	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	fseek(f, 0, SEEK_SET);

	MALLOC(res, sizeof(char) * (*len + 1), return NULL, );
	fread(res, *len, 1, f);
	res[*len] = 0;
	
	return res;
}


int cfg_fprintf(char **a, void *stream, void *delim) {
	FILE *fc = stream;
	int ret;
	
	ret = fprintf(fc, "%s%s", a[0], (char *) delim); if(ret < 0) return ret;
	if(a[1]) {
		ret = fprintf(fc, "%s", a[1]); if(ret < 0) return ret;
	}
	ret = fprintf(fc, "\n"); if(ret < 0) return ret;
	
	return 0;
}

char ***fd_load_cfg(FILE *fd) {
	char ***res;
	ARRAYH_NEW(res, 10, return NULL, );
	
	char line_buf[CFG_FILE_MAX_LINE_LEN + 1];
	
    while(true) {
		size_t len = file_readline_to(fd, line_buf, CFG_FILE_MAX_LINE_LEN);
		
		// line too long
		if(len == CFG_FILE_MAX_LINE_LEN) {
			LOG_E("line too long in file\n");
			LOG_E("[%s]\n", line_buf);
			return NULL;
		}
		
		// eof
		if(len == 0) { 
			break; 
		}
		
		// ignore comments and empty lines
		if(line_buf[0] == '#' || line_buf[0] == '\n') {
			continue;
		}
		
		char **cfg;
		ARRAYH_NEW(cfg, 2, return NULL, );
		
		char *delim = strchr(line_buf, CHAR_EQUAL); 
		if(!delim) { 
			LOG_E("malformned configuration, delimiter [%c] not found in line\n", CHAR_EQUAL);
			LOG_E("[%s]\n", line_buf);
			return NULL; 
		}
		
		// getting cfg name
		char *name;
		MALLOC(name, sizeof(char) * (delim - line_buf + 1), return NULL, );
		strncpy(name, line_buf, delim - line_buf);
		name[delim - line_buf] = 0;
		ARRAYH_ADD(cfg, name, LOG_E("failed to add name [%s]\n", name); return NULL);

		// getting cfg value
		char *value;
		MALLOC(value, sizeof(char) * (line_buf + len - delim - 1), return NULL, );
		strncpy(value, delim + 1, line_buf + len - delim - 2);
		value[line_buf + len - delim - 2] = 0;
		ARRAYH_ADD(cfg, value, LOG_E("failed to add value [%s]\n", value); return NULL);
				
		// adding cfg
		ARRAYH_ADD(res, cfg, LOG_E("failed to add line"); return NULL);
    }
	
	return res;
}

char ***file_load_cfg(char *path) {
	FILE *fd = fopen(path, "r"); 
	if(!fd) {
		perror(path);
		LOG_E("failed to open file [%s]\n", path);
		return NULL;
	}
	
	char ***res = fd_load_cfg(fd);
	if(!res) {
		LOG_E("failed to load into array\n");
	}
	
    fclose(fd);
	
	return res;
}

char **fd_load_list(FILE *fd) {
	char **res;
	ARRAYH_NEW(res, 10, return NULL, );
	
	char line_buf[FILE_MAX_LINE_LEN];
	
    while(true) {
		size_t len = file_readline_to(fd, line_buf, FILE_MAX_LINE_LEN);
		
		// line too long
		if(len == FILE_MAX_LINE_LEN) {
			LOG_E("line too long in file\n");
			LOG_E("[%s]\n", line_buf);
			return NULL;
		}
		
		// eof
		if(len == 0) { 
			break; 
		}

		// ignore comments and empty lines
		if(line_buf[0] == '#' || line_buf[0] == '\n') {
			continue;
		}
		
		// line is ok for treatment
		char *new_str;
		MALLOC(new_str, sizeof(char) * (strlen(line_buf) + 1), return NULL, );
		strcpy(new_str, line_buf);
		if(new_str[strlen(line_buf) - 1] == '\n') {
			new_str[strlen(line_buf) - 1] = 0;
		}

		ARRAYH_ADD(res, new_str, LOG_E("failed to add file"); return NULL);
    }
	
	return res;
}

char **file_load_list(char *path) {
	FILE *fd = fopen(path, "r"); 
	if(!fd) {
		perror(path);
		LOG_E("failed to open file [%s]\n", path);
		return NULL;
	}
	
	char **res = fd_load_list(fd);
	if(!res) {
		LOG_E("failed to load into array\n");
	}
	
    fclose(fd);
	
	return res;
}

char ***fd_save_cfg(FILE *fd, char ***a) {	
	FOR(i, 0, ARRAYH_CNT(a), { 
		int ret = cfg_fprintf(a[i], fd, STRING_EQUAL); if(ret < 0) return NULL;
	});
	
	return a;
}

char ***file_save_cfg(char *path, char ***a) {
	FILE *fd = fopen(path, "w");
	if(!fd) {
		perror(path);
		LOG_E("failed to open file [%s]\n", path);
		return NULL;
	}
	
	char ***res = fd_save_cfg(fd, a);
	if(!res) {
		LOG_E("failed to save to file\n");
	}
	
    fclose(fd);
	
	return res;
}

char **fd_save_list(FILE *fd, char **a) {
	FOR(i, 0, ARRAYH_CNT(a), { 
		int ret = fprintf(fd, "%s\n", a[i]); if(ret < 0) return NULL;
	});

	return a;
}

char **file_save_list(char *path, char **a) {
	FILE *fd = fopen(path, "w");
	if(!fd) {
		perror(path);
		LOG_E("failed to open file [%s]\n", path);
		return NULL;
	}
	
	char **res = fd_save_list(fd, a);
	if(!res) {
		LOG_E("failed to save to file\n");
	}
	
    fclose(fd);

	return res;
}

