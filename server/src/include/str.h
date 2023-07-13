#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int str_add(char *buffer, size_t max_len, char *src, int len) {
	if(strlen(buffer) + len >= max_len) {
		fprintf(stderr, "Message too long !\n");
		return 0;
	}

	strncat(buffer, src, len);
	return 1;
}

int add_while() {
	int max = 256;
	char buffer[max];
	char *found;

	/* read */
	while(1) {
		char *read = "sdfsdfsdf";

		found = strchr(read, ';');

		if(found) {
			if(!str_add(buffer, max, read, found - read)) {
				return 1;
			}

			/* treat */

			memset(buffer, 0, max);
		}
		else {
			if(!str_add(buffer, max, read, strlen(read))) {
				return 1;
			}
		}
	}
}




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


