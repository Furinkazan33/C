#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gen_struct.h>

int str_len(char *src) {
    int i = 0;

    while(src[i++] != '\0') {
    }

    return i;
}

char *str_copy(char *src) {
    int n = str_len(src);
    char *result = (char *) malloc(sizeof(char *) * (n + 1));

    for (; n >= 0; n--) {
        result[n] = src[n];
    }

    return result;
}

void str_print(char *src) {
    while(*src != '\0')
    {
        printf("%c", *src++);
    }
    printf("\n");
}

// Returns the index of the first occurence of the given pattern or -1
int str_idx(char *source, char *pattern) {
    int i, j;
    int len = strlen(pattern);

    for (i = 0, j = 0; source[i] != '\0'; i++) {
        if(source[i] == pattern[j]) {
	    j++;
	} else {
	    j = 0;
	}

	if(j == len) {
            return i - len + 1;
	}
    }

    return EXIT_NOT_FOUND;
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
	printf("from %d to %d\n", idx, new_len - len - 1);

        for (i = idx; i < new_len; i++) {
            src[i] = src[i - len];
	    printf("%s\n", src);
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

    if(idx == EXIT_NOT_FOUND) {
        return EXIT_NOT_FOUND;
    }

    //printf("Replacing %s(%d) by %s(%d)\n", pattern, p_len, rpl, rpl_len);

    str_shift(src, idx, rpl_len - p_len);

    //printf("%s\n", src);

    for (i = 0; i < rpl_len; i++) {
        src[idx + i] = rpl[i];
    }

    return EXIT_SUCCESS;
}

int str_split(char *struct_def, char delim[], char **result) {
    int i = 0;

    char *ptr = strtok(struct_def, delim);
    
    while(ptr != NULL) {

	if(i > N_MAX_TOK) { 
	    printf("Max token reached!\n");
	    return EXIT_FAILURE;
	}

	result[i++] = ptr;
	ptr = strtok(NULL, delim);
    }

    result[i] = NULL;

    if(i < 3) {
	printf("Structure definition error : %s\n", struct_def);
	return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

char *str_join(char *defs[], char delim, int end) {
    int i, len;
    
    char *result = (char *) malloc(sizeof(char *) * L_MAX_LINE);

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

    printf("join : %s\n", result);

    return result;
}



