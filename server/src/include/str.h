#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* returns a copy of the src string with the given pattern replaced */
char *str_rpl(char *src, char *pattern, char *rpl) {
	char *res, *found;

	/* rpl at beginning of the src string */
	if(strlen(pattern) == 0) {
		res = (char *)malloc(sizeof(char) *(strlen(src) + strlen(rpl)) + 1);

		if(strlen(rpl) == 0) {
			strcpy(res, src);
		}
		else {
			strcpy(res, rpl);
			strcat(res + strlen(rpl), src);
		}

		return res;
	}

	found = strstr(src, pattern);

	/* no changes */
	if(!found) {
		res = (char *)malloc(sizeof(char) *(strlen(src) + 1));
		if(!res) {
			perror("");
			return NULL;
		}
		strcpy(res, src);

		return res;
	}

	/* delete pattern */
	if(strlen(rpl) == 0) {
		res = (char *)malloc(sizeof(char) *(strlen(src) - strlen(pattern)) + 1);

		if(found == src) {
			strcpy(res, src + strlen(pattern));
		}
		else {
			strncpy(res, src, found - src);
			strcat(res, found + strlen(pattern));
		}

		return res;
	}

	/* normal case, do replace */
	res = (char *)malloc(sizeof(char) *(strlen(src) + strlen(rpl) - strlen(pattern) + 1));
	if(!res) {
		perror("");
		return NULL;
	}

	size_t l = found - src;

	strcpy(res, src);
	strcpy(res+l, rpl);
	l+=strlen(rpl);
	strcpy(res+l, found+strlen(pattern));

	return res;
}


