#include <string.h>
#include <sys/stat.h>
#include "filediff.h"
#include "macros.h"

const char *dt_tos(diff_type t) {
	switch(t) {
		default:
		case DT_NONE:
			return "DT_NONE";
		case DT_DELETED:
			return "DT_DELETED";
		case DT_CREATED:
			return "DT_CREATED";
		case DT_TIME:
			return "DT_TIME";
	}
}

char *fd_strdup(const char *src) {
	char *res;
	MALLOC(res, sizeof(char) * (strlen(src) + 1), return NULL, );
	strcpy(res, src);
	
	return res;
}


file *file_new(char *path, char *content, int mtime) {
	file *res;
	MALLOC(res, sizeof(*res), return NULL, );
	
	res->mtime = mtime;
	res->path = NULL;
	res->content = NULL;
	
	if(path) {
		res->path = fd_strdup(path); if(!res->path) return NULL;
	}
	
	if(content) {
		res->content = fd_strdup(content); if(!res->content) return NULL;
	}	
	
	return res;
}

void file_free(file *f) {
	free(f->path);
	free(f->content);
	free(f);
}

filediff *filediff_new(char *path, char *content, int mtime, diff_type type) {
	filediff *res;
	MALLOC(res, sizeof(*res), return NULL, );
	
	res->f = file_new(path, content, mtime); 
	if(!res->f) {
		LOG_E("failed to create file");
		return NULL;
	}
	res->type = type;
	res->treated = false;
	
	return res;
}

filediff *filediff_copy(filediff *fd) {
	filediff *res = filediff_new(fd->f->path, fd->f->content, fd->f->mtime, fd->type);
	if(!res) {
		return NULL;
	}
	res->treated = fd->treated;
	return res;
}

void filediff_free(filediff *diff) {
	file_free(diff->f);
	free(diff);
}

filediff **files_to_filediff(char **files, size_t len) {
	struct stat attr;
	
	filediff **res;
	MALLOC(res, sizeof(*res) * len, return NULL, );
	
	for(size_t i = 0; i < len; i++) {
		#ifdef DEBUG
			LOG_S("[%s]\n", files[i]);
		#endif
		stat(files[i], &attr);
		res[i] = filediff_new(files[i], NULL, attr.st_mtime, 0); if(!res[i]) return NULL;
	}
	
	return res;
}

// compare first list against second
filediff **fds_compare(filediff **fd1, size_t len1, filediff **fd2, size_t len2, size_t *len) {
	filediff **res;
	MALLOC(res, sizeof(*res) * (len1 + len2), return NULL, );
	*len = 0;
	filediff *new_diff = NULL;
	
	// find files from a1 in a2 by path
	for(size_t i = 0; i < len1; i++) {
		#ifdef DEBUG
			LOG_S("searching [%s]\n", fd1[i]->f->path);
		#endif
		
		filediff *found = NULL;
		
		for(size_t f = 0; f < len2; f++) {
			// found
			if(!strcmp(fd2[f]->f->path, fd1[i]->f->path)) {
				#ifdef DEBUG
					LOG_S("found");
				#endif
				
				found = fd2[f];
				
				// mark as treated
				found->treated = true;
				
				// file has changed
				if(fd1[i]->f->mtime - found->f->mtime != 0) {
					new_diff = filediff_new(fd1[i]->f->path, NULL, fd1[i]->f->mtime, DT_TIME); if(!new_diff) return NULL;
					res[(*len)++] = new_diff;
				}
				
				break;
			}
		}
		
		if(!found) {
			#ifdef DEBUG
				LOG_S("not found");
			#endif
			
			// deleted
			new_diff = filediff_new(fd1[i]->f->path, NULL, 0, DT_DELETED); if(!new_diff) return NULL;
			res[(*len)++] = new_diff;
		}
	}
	
	for(size_t j = 0; j < len2; j++) {
		// new files
		if(!fd2[j]->treated) {
			#ifdef DEBUG
				LOG_S("not treated");
			#endif
			
			new_diff = filediff_new(fd2[j]->f->path, NULL, fd2[j]->f->mtime, DT_CREATED); if(!new_diff) return NULL;
			res[(*len)++] = new_diff;
		}
	}
	
	return res;
}

//TODO que faire si types !=
filediff **fds_fuse(filediff **fds1, size_t len1, filediff **fds2, size_t len2, size_t *len) {
	filediff **res;
	MALLOC(res, sizeof(*res) * (len1 + len2), return NULL, );
	
	*len = 0;
	
	for(size_t i = 0; i < len1; i++) {
		res[(*len)++] = filediff_copy(fds1[i]);
	}
	for(size_t i = 0; i < len2; i++) {
		res[(*len)++] = filediff_copy(fds2[i]);
	}
	
	return res;
}