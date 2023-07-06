#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define STR_MAX_LEN 40

typedef enum C_TYPE { TXT_BUF = 0, IMG, VID } C_TYPE;
int w_id = 0;

typedef struct window {
	int id;
	int x, y, w, h;
	int bar_x, bar_y, bar_w, bar_h;
	char name[STR_MAX_LEN];
	C_TYPE type;
	void *content;
} window;

int w_new_id() {
	return ++w_id;
}

window *w_new(char *name, C_TYPE type, void *content, int x, int y, int w, int h) {
	window *res = (window *)malloc(sizeof(window));

	res->id = w_new_id();
	strncpy(res->name, name, STR_MAX_LEN);
	res->type = type;
	res->x = x; res->y = y;	res->w = w;	res->h = h;
	res->content = content;

	return res;
}


