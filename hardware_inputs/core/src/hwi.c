#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <dirent.h>
#include <string.h>
#include "hwi.h"


int _hwi_debug = 0;

void hwi_debug() {
	_hwi_debug = !_hwi_debug;
}

void hwi_filelist_free(char **list) {
	char **p = list;

	while(p && *p) {
		free(*p);
		p++;
	}
	free(list);
}

void hwi_filelist_print(char **list) {
	char **p = list;

	while(p && *p) {
		printf("%s\n", *p);
		p++;
	}
}

/* looking for substrings (if not NULL) in name and returns true if found */
bool hwi_filename_match(char *name, char *substring1, char *substring2) {
	if(substring1 && !strstr(name, substring1)) {
		return false;
	}
	if(substring2 && !strstr(name, substring2)) {
		return false;
	}
	return true;
}

char **hwi_filelist_get(char *folder, char *substring1, char *substring2) {
	DIR *dir;
	struct dirent *ent;

	size_t n_found = 0;
	size_t alloc = 5;
	char **res = malloc(sizeof(char *) * (alloc + 1)); //NULL terminated
	char **tmp;

	printf("hwi_get_list : looking for [%s] [%s] devices in [%s]\n", substring1, substring2, folder);

	if((dir = opendir(folder)) != NULL) {
		while((ent = readdir(dir)) != NULL) {
			if(hwi_filename_match(ent->d_name, substring1, substring2)) {
				/* realloc */
				if(n_found >= alloc) {
					tmp = realloc(res, alloc * 2);
					if(!tmp) {
						perror("hwi_get_list : realloc error");
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
		perror("hwi_get_list : failed to get a list of devices");
		return NULL;
	}

	res[n_found] = NULL;

	return res;
}

int hwi_file_open(char *path, int mode) {
	int fd;

	printf("hwi_file_open : opening device %s\n", path);

	fd = open(path, mode);
	if(fd == -1) {
		perror("hwi_file_open : error opening device");
		return -1;
	}

	printf("hwi_file_open : success\n");

	return fd;
}

int hwi_filelist_open(char **list, int mode) {
	int fd;
	char **p = list;

	printf("hwi_filelist_open : calling hwi_file_open on list\n");
	while(p && *p) {
		fd = hwi_file_open(*p, mode);

		if(fd == -1) {
			p++;
		}
		else {
			break;
		}
	}

	if(fd == -1) {
		fprintf(stderr, "hwi_filelist_open : impossible to open a device\n");
		return -1;
	}

	return fd;
}

void hwi_close(int fd) {
	printf("hwi_close : closing device [%d]\n", fd);
	close(fd);
}

int hwi_open(char *folder) {
	int fd = -1;
	char **list = hwi_filelist_get(folder, "kbd", NULL);

	if(list && *list) {
		hwi_filelist_print(list);

		fd = hwi_filelist_open(list, O_RDONLY);

		if(fd == -1) {
			fprintf(stderr, "hwi_open : impossible to open founded keyboards\n");
		}
	}
	else {
		fprintf(stderr, "hwi_open : no keyboard found\n");
	}

	hwi_filelist_free(list);

	return fd;
}

void hwi_print(bool *pressed) {
	for(unsigned short i = 0; i < KB_MAX_KEYS; i++) {
		if(pressed[i]) {
			printf("%hu ", i);
		}
	}
	printf("\n");
}

enum ev_int { RELEASED = 0, PRESSED, REPEATED };

int hwi_read(int fd, struct input_event ev, bool *pressed) {
	ssize_t n = read(fd, &ev, sizeof(ev));

	if(n == (ssize_t)-1 && errno != EINTR && errno != 0) {
		perror("hwi_read : erreur de lecture du clavier");
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
					pressed[ev.code] = false;
					//printf("RELEASED 0x%04x (%hu)\n", ev.code, ev.code);
					break;

				case PRESSED:
					pressed[ev.code] = true;
					//printf("PRESSED  0x%04x (%hu)\n", ev.code, ev.code);
					break;

				case REPEATED:
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
			fprintf(stderr, "unknown ev.type=%hu, code=%hu, value=%hu", ev.type, ev.code, ev.value);
			break;
	}

	if(_hwi_debug) {
		hwi_print(pressed);
	}

	return 1;
}


