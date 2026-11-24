#include "filediff.h"
#include "macros.h"


int _filediff_ref_cmp_path(const void *d1, const void *d2) {
	const filediff *fd1 = * (filediff **) d1;
	const filediff *fd2 = * (filediff **) d2;
	
	return strcmp(fd1->f->path, fd2->f->path);
}

int main() {
	char *filelist1[] = {
		"/home/mathieu/C/0_template", 
		"/home/mathieu/C/0_template/libs/", 
		"/home/mathieu/C/0_template/libs/macros.h", 
		"/home/mathieu/C/sync/",
	};
	size_t len1 = 4;
	
	char *filelist2[] = {
		"/home/mathieu/C/0_template", 
		"/home/mathieu/C/0_template/libs/macros.h", 
		"/home/mathieu/C/", 
	};
	size_t len2 = 3;
	
	filediff **fd1 = files_to_filediff(filelist1, len1); if(!fd1) return 1;
	FOR(i, 0, len1, {
		LOG_S("[%s][%s][%d]\n", fd1[i]->f->path, dt_tos(fd1[i]->type), fd1[i]->f->mtime);
	});
	
	filediff **fd2 = files_to_filediff(filelist2, len2); if(!fd2) return 1;
	fd2[0]->f->mtime = 1;
	
	puts("");
	
	FOR(i, 0, len2, {
		LOG_S("[%s][%s][%d]\n", fd2[i]->f->path, dt_tos(fd2[i]->type), fd2[i]->f->mtime);
	});
	
	size_t len;
	filediff **res = fds_compare(fd1, len1, fd2, len2, &len); if(!res) return 1;
	qsort(res, len, sizeof(filediff **), _filediff_ref_cmp_path);
	
	puts("");
	
	FOR(i, 0, len, {
		LOG_S("[%s][%s][%d]\n", res[i]->f->path, dt_tos(res[i]->type), res[i]->f->mtime);
	});
	
	puts("");
	
	
	filediff **fuse = fds_fuse(fd1, len1, fd2, len2, &len);
	qsort(fuse, len, sizeof(filediff **), _filediff_ref_cmp_path);
	FOR(i, 0, len, {
		LOG_S("[%s][%s][%d]\n", fuse[i]->f->path, dt_tos(fuse[i]->type), fuse[i]->f->mtime);
	});
	
	
	
	
	return 0;
}