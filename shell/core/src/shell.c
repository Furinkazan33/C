#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "shell.h"

int char_is_separator(char c) {
	return c == ' ' || c == '\t';
}

char *cmds_alloc(size_t size) {
	char *res = malloc(sizeof(char) * (size + 1));
	if(!res) {
		perror("");
		return NULL;
	}
	return res;
}

char **split(char *s) {
	size_t tokens_len = 4;
	size_t token_len = 8;
	size_t n = 0;
	size_t i = 0;
	char **res = malloc(sizeof(*res) * (tokens_len + 1)); // NULL terminated

	while(s && *s) {
		// new token
		if(i == 0) {
			token_len = 8;
			res[n] = malloc(sizeof(char) * (token_len + 1));
		}

		// realloc array
		if(n == tokens_len) {
			tokens_len *= 2;
			res = realloc(res, sizeof(char *) * (tokens_len + 1));
		}

		while(isblank(*s)) { s++; }

		while(s && *s && !isblank(*s)) {
			// realloc token
			if(i == token_len) {
				token_len *= 2;
				res[n] = realloc(res[n], sizeof(char) * (token_len + 1));
			}
			res[n][i++] = *s++;
		}
		// only blanks have been read
		if(i == 0) {
			free(res[n]);
		}
		else {
			if(!s || !*s) {
				res[n++][i] = '\0';
			}

			if(s && *s && isblank(*s)) {
				res[n++][i] = '\0';
				i = 0;
				s++;
			}
		}
	}
	res[n] = NULL;

	return res;
}


void print_ps1(char *ps1) {
	printf("%s", ps1);
}

static const char *commands[] = {
	"help",
	"echo",
	0
};


int help(char **cmd) {
	if(!cmd) {
		printf("list of available commands :\n");
		for(size_t i = 0; commands[i]; i++) {
			printf("%s\n", commands[i]);
		}
		return 0;
	}

	printf("help for [%s]\n", cmd[1]);
	printf("TODO\n");

	return 0;
}

int execute(char **cmd) {
#ifdef DEBUG
	printf("command : [%s]", cmd[0]);
	if(cmd[1]) {
		printf(", parameters : [");
		for(size_t i = 1; cmd[i]; i++) {
			printf("%s, ", cmd[i]);
		}
		printf("]");
	}
	printf("\n");
#endif

	if(!strcmp(cmd[0], "exit")) {
		return SHELL_RC_EXIT;
	}

	if(!strcmp(cmd[0], "help")) {
		if(!cmd[1]) {
			help(0);
		}
		else {
			help(cmd);
		}
	}

	return SHELL_RC_OK;
}

char *read_cmd() {
	size_t size = CMD_ALLOC_CHARS;
	char *res = malloc(sizeof(char) * (size + 1));
	if(!res) {
		perror("");
		return NULL;
	}

	char *p = res;
	while((*p = getchar()) != '\n') {
		switch(*p)
		{
			case 91:
				*p = getchar();

				switch(*p) {
					case 65:
						printf("\nUp Arrow");
						break;
					case 66:
						printf("\nDown Arrow");
						break;
					case 67:
						printf("\nRight Arrow");
						break;
					case 68:
						printf("\nLeft Arrow");
						break;
				}
			default:
				break;
		}

		p++;
		if((size_t)(p - res) >= size) {
			size *= 2;
			res = realloc(res, size + 1);
			if(!res) {
				perror("");
				return NULL;
			}
		}
	}
	*p = '\0';

	return res;
}

void cmds_free(char **cmds) {
	for(size_t i = 0; cmds[i]; i++) {
		free(cmds[i]);
	}
	free(cmds);
}

