#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <text.h>

/*
	TODO: free memory
*/



void text_print(char *text, int len) {
	int i;
	
	if(len == 0) {
		len = MAX_TEXT_SIZE;
	} 
	
	for(i = 0; text[i] != '\0' && i < len; i++) {
		printf("%c", text[i]);
	}
	printf("\n");
}

void words_print(char **words) {
	int i;
	
	for (i = 0; words[i] != NULL; i++) {
		//text_print(words[i], 0);
		printf("%s-", words[i]);
	}
}

void text_split(char *text, char token, char **words) {
	int i;
	char *p = text;
		
	for(i = 0; *p != '\0'; i++) {
		// Jusqu'à fin du mot
		for(; *p != token && *p != '\0';) {
			p++;
		}
		
		// Allocation et copie du mot courant
		words[i] = (char *) malloc(sizeof(char) * (p - text + 1));
		memcpy(words[i], text, p - text);
		words[i][p - text] = '\0';
		
		// Mot suivant
		p++;
		text = p;	
	}
	words[i] = NULL;
}
