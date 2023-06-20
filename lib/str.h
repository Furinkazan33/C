#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *str_find(char *src, char *match) {
	char *p = src;
	char *cur = match;

	while(p && *p) {
		/* No matching */
		while(*p && *p != *cur) {
			p++;
		}

		/* Still matching  */
		while(*p && *p == *cur) {
			p++;
			cur++;
		}

		/* Whole matching ? */
		if(cur - match == strlen(match)) {
			return p - strlen(match);
		}

		/* Restart from beginning */
		cur = match;
	}

	return NULL;
}

// Returns the index of the first occurence of the given pattern or -1
int str_idx(char *src, char *pattern) {
	int i, j;

	for (i = 0, j = 0; src[i] != '\0'; i++) {
		if(src[i] == pattern[j]) {
			j++;
		}
		else {
			j = 0;
		}
		if(j == strlen(pattern)) {
			return i - j + 1;
		}
	}

	return -1;
}

// Shift string left or right from idx
void str_shift(char *src, int idx, int len) {
    int i;
    int new_len = strlen(src) + len;

    if(len == 0 || idx < 0 || idx >= strlen(src)) {
        return;
    }

    if (len > 0) {
        src = (char *) realloc(src, (sizeof(char *) * new_len));

        for (i = new_len - 1; i >= idx + len; i--) {
            src[i] = src[i - len];
        }
    }
    else {
        for (i = idx; i < new_len; i++) {
            src[i] = src[i - len];
        }
        src = (char *) realloc(src, (sizeof(char *) * new_len));
    }

    src[new_len] = '\0';
}


// Replace the first iteration of the given pattern
// end of src string is lost if rpl length gt pattern length
int str_rpl(char *src, char *pattern, char *rpl) {
    int p_len = strlen(pattern);
    int rpl_len = strlen(rpl);
    int idx = str_idx(src, pattern);
    int i;

    if(idx == -1) {
        return -1;
    }

    str_shift(src, idx, rpl_len - p_len);

    for (i = 0; i < rpl_len; i++) {
        src[idx + i] = rpl[i];
    }

    return 0;
}

void str_join(char *result, char *defs[], char delim, int end) {
    int i, len;

    for (i = 2, len = 0; defs[i] != NULL; i++) {
        strcat(result, defs[i]);
		len += strlen(defs[i]);
		result[len] = delim;
		len += 1;
		strcat(result, " ");
		len += 1;
    }
    result[len - 3] = delim;
    result[len - 2] = '\0';

    if(end == 1) {
        result[len - 3] = '\0';
    }
}


