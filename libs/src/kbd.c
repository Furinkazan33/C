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
#include "macros.h"

enum ev_int { RELEASED = 0, PRESSED, REPEATED };

void kbd_reinit(keyboard *k) {
	k->count = 0;
	memset(k->pressed, 0, sizeof(int) * KB_MAX_KEYS);
}

keyboard *_kbd_new(char *file, int fd) {
	keyboard *res = malloc(sizeof(keyboard));
	if(!res) {
		fprintf(stderr, "_kbd_new : call to malloc returned NULL\n");
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



void _kbd_filelist_free(char **list) {
	char **p = list;

	while(p && *p) {
		free(*p);
		p++;
	}
	free(list);
}

void _kbd_filelist_print(char **list) {
	char **p = list;

	while(p && *p) {
		printf("%s\n", *p);
		p++;
	}
}

/* looking for substrings (if not NULL) in name and returns true if found */
int _kbd_filename_match(char *name, char *substring1, char *substring2) {
	#ifdef DEBUG
		LOG_S("[%s][%s][%s]\n", name, substring1, substring2);
	#endif
	
	if(substring1 && !strstr(name, substring1)) {
		return 0;
	}
	if(substring2 && !strstr(name, substring2)) {
		return 0;
	}
	return 1;
}

char **_kbd_filelist_get(char *folder, char *substring1, char *substring2) {
	DIR *dir;
	struct dirent *ent;

	size_t n_found = 0;
	size_t alloc = 5;
	char **res = malloc(sizeof(char *) * (alloc + 1)); //NULL terminated
	char **tmp;

	#ifdef DEBUG
		LOG_S("looking for [%s] [%s] devices in [%s]\n", substring1, substring2, folder);
	#endif

	if((dir = opendir(folder)) != NULL) {
		while((ent = readdir(dir)) != NULL) {
			if(_kbd_filename_match(ent->d_name, substring1, substring2)) {
				/* realloc */
				if(n_found >= alloc) {
					tmp = realloc(res, alloc * 2);
					if(!tmp) {
						LOG_E("realloc error");
						perror("");
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
		LOG_S("failed to get a list of devices");
		perror("");
		return NULL;
	}

	res[n_found] = NULL;

	return res;
}

keyboard *kbd_open(char *folder) {
	assert(folder);

	int fd = -1;
	char **list = _kbd_filelist_get(folder, "kbd", NULL);
	if(!list || !list[0]) {
		LOG_E("no keyboard to open");
		return NULL;
	}

	char **p = list;
	while(p && *p) {
		#ifdef DEBUG
			LOG_S("opening [%s]\n", *p);
		#endif

		fd = open(*p, O_RDONLY);

		if(fd == -1) {
			LOG_E("call to open returned -1");
			perror("");
			p++;
		}
		else {
			#ifdef DEBUG
				LOG_S("opened as [%d]\n", fd);
			#endif
			break;
		}
	}

	if(fd == -1) {
		LOG_E("impossible to open a device");
		perror("");
		return NULL;
	}

	char *kbdpath = malloc(sizeof(char) * (strlen(*p) + 1));
	if(!kbdpath) {
		LOG_E("failed to alloc kbdpath");
		return NULL;
	}

	_kbd_filelist_free(list);

	keyboard *res = _kbd_new(kbdpath, fd);
	if(!res) {
		LOG_E("_kbd_new returned NULL");
		return NULL;
	}

	return res;
}


void kbd_log(FILE *file, keyboard *k) {
	LOG(file, "[fd:%d, count:%d, [", k->fd, k->count);

	for(unsigned short i = 0; i < KB_MAX_KEYS; i++) {
		if(k->pressed[i] > 0) {
			LOG(file, "%hu[%d],", i, k->pressed[i]);
		}
	}
	
	LOG_MESS(file, "]\n");
}

keyboard *kbd_read(keyboard *k) {
	assert(k);

	#ifdef DEBUG
		LOG_S("reading fd [%d]\n", k->fd);
	#endif

	struct input_event ev;
	ssize_t n = read(k->fd, &ev, sizeof(ev));

	if(n == (ssize_t)-1 && errno != EINTR && errno != 0) {
		LOG_E("erreur de lecture du clavier");
		perror("");
		return NULL;
	}
	else if(n != sizeof(ev)) {
		LOG_E("mauvaise taille lue");
		errno = EIO;
		return NULL;
	}
	
	#ifdef DEBUG
		LOG_S("event type [%d]\n", ev.type);
	#endif

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

