#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <dirent.h>
#include "kbd.h"

enum ev_int { RELEASED = 0, PRESSED, REPEATED };

void kbd_reinit(keyboard *k) {
	k->count = 0;
	memset(k->pressed, 0, sizeof(int) * KB_MAX_KEYS);
}

keyboard *kbd_new(char *file, int fd) {
	keyboard *res = malloc(sizeof(keyboard));
	if(!res) {
		fprintf(stderr, "kbd_new : call to malloc returned NULL\n");
		return NULL;
	}
	res->path = file;
	res->fd = fd;
	kbd_reinit(res);

	return res;
}

void kbd_free(keyboard *k) {
	close(k->fd);
	free(k->path);
	free(k);
}



void kbd_filelist_free(char **list) {
	char **p = list;

	while(p && *p) {
		free(*p);
		p++;
	}
	free(list);
}

void kbd_filelist_print(char **list) {
	char **p = list;

	while(p && *p) {
		printf("%s\n", *p);
		p++;
	}
}

/* looking for substrings (if not NULL) in name and returns true if found */
int kbd_filename_match(char *name, char *substring1, char *substring2) {
	if(substring1 && !strstr(name, substring1)) {
		return 0;
	}
	if(substring2 && !strstr(name, substring2)) {
		return 0;
	}
	return 1;
}

char **kbd_filelist_get(char *folder, char *substring1, char *substring2) {
	DIR *dir;
	struct dirent *ent;

	size_t n_found = 0;
	size_t alloc = 5;
	char **res = malloc(sizeof(char *) * (alloc + 1)); //NULL terminated
	char **tmp;

	printf("kbd_filelist_get : looking for [%s] [%s] devices in [%s]\n", substring1, substring2, folder);

	if((dir = opendir(folder)) != NULL) {
		while((ent = readdir(dir)) != NULL) {
			if(kbd_filename_match(ent->d_name, substring1, substring2)) {
				/* realloc */
				if(n_found >= alloc) {
					tmp = realloc(res, alloc * 2);
					if(!tmp) {
						perror("kbd_filelist_get : realloc error");
						return res;
					}
					res = tmp;
					alloc *= 2;
				}
				res[n_found] = malloc(sizeof(char) * (NAME_MAX + strlen(folder) + 1));
				strcpy(res[n_found], folder);
				strcat(res[n_found], ent->d_name);
				n_found++;
			}
		}
		closedir(dir);
	}
	else {
		perror("kbd_filelist_get : failed to get a list of devices");
		return NULL;
	}

	res[n_found] = NULL;

	return res;
}

keyboard *kbd_open(char *folder) {
	assert(folder);

	int fd = -1;
	char **list = kbd_filelist_get(folder, "kbd", NULL);
	char **p = list;

	while(p && *p) {
#ifdef DEBUG
		fprintf(stdout, "kbd_open : opening %s\n", *p);
#endif
		fd = open(*p, O_RDONLY);

		if(fd == -1) {
			perror("kbd_open : call to open returned -1");
			p++;
		}
		else {
			break;
		}
	}

	if(fd == -1) {
		fprintf(stderr, "kbd_open : impossible to open a device\n");
		return NULL;
	}

	char *kbdpath = malloc(sizeof(char) * (strlen(*p) + 1));
	if(!kbdpath) {
		return NULL;
	}

	kbd_filelist_free(list);

	keyboard *res = kbd_new(kbdpath, fd);
	if(!res) {
		return NULL;
	}

	return res;
}


void kbd_fprintf(FILE *file, keyboard *k) {
	fprintf(file, "[fd:%d, count:%d, [", k->fd, k->count);

	for(unsigned short i = 0; i < KB_MAX_KEYS; i++) {
		if(k->pressed[i] > 0) {
			fprintf(file, "%hu[%d],", i, k->pressed[i]);
		}
	}
	
	printf("]\n");
}

void kbd_printf(keyboard *k) {
	kbd_fprintf(stdout, k);
}

keyboard *kbd_read(keyboard *k) {
	assert(k);

	struct input_event ev;
	ssize_t n = read(k->fd, &ev, sizeof(ev));

	if(n == (ssize_t)-1 && errno != EINTR && errno != 0) {
		perror("kbd_read : erreur de lecture du clavier");
		return 0;
	}
	else if(n != sizeof(ev)) {
		errno = EIO;
		return 0;
	}

	switch(ev.type) {
		case EV_SYN:
			/*
			   printf("EV_SYN, ");
			   printf("code:%hu value:%hu\n", ev.code, ev.value);
			   */
			break;

		case EV_KEY:
			switch(ev.value) {
				case RELEASED:
					if(k->count > 0) { k->count--; }
					k->pressed[ev.code] = 0;
					//printf("RELEASED 0x%04x (%hu)\n", ev.code, ev.code);
					break;

				case PRESSED:
					k->pressed[ev.code]++;
					if(k->pressed[ev.code] == 1) { k->count++; }
					//printf("PRESSED  0x%04x (%hu)\n", ev.code, ev.code);
					break;

				case REPEATED:
					k->pressed[ev.code]++;
					//printf("REPEATED 0x%04x (%hu)\n", ev.code, ev.code);
					break;
			}
			break;

		case EV_REL:
			printf("EV_REL, ");
			printf("code:%hu value:%hu\n", ev.code, ev.value);
			break;

		case EV_ABS:
			printf("EV_ABS, ");
			printf("code:%hu value:%hu\n", ev.code, ev.value);
			break;

		case EV_MSC:
			break;

		case EV_SW:
			printf("EV_SW, ");
			printf("code:%hu value:%hu\n", ev.code, ev.value);
			break;

		case EV_LED:
			printf("EV_LED, ");
			printf("code:%hu value:%hu\n", ev.code, ev.value);
			break;

		case EV_SND:
			printf("EV_SND, ");
			printf("code:%hu value:%hu\n", ev.code, ev.value);
			break;

		case EV_REP:
			printf("EV_REP, ");
			printf("code:%hu value:%hu\n", ev.code, ev.value);
			break;

		case EV_FF:
			printf("EV_FF, ");
			printf("code:%hu value:%hu\n", ev.code, ev.value);
			break;

		case EV_PWR:
			printf("EV_PWR, ");
			printf("code:%hu value:%hu\n", ev.code, ev.value);
			break;

		case EV_FF_STATUS:
			printf("EV_FF_STATUS, ");
			printf("code:%hu value:%hu\n", ev.code, ev.value);
			break;

		default:
			fprintf(stderr, "unknown type=%hu, code=%hu, value=%hu", 
					ev.type, ev.code, ev.value);
			break;
	}

	return k;
}

int kbd_any(keyboard *k) {
	return k->count > 0;
}

