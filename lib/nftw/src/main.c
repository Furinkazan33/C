#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 1			/* Required under GLIBC for nftw() */
#define _XOPEN_SOURCE_EXTENDED 1	/* Same */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <ftw.h>	/* gets <sys/types.h> and <sys/stat.h> for us */
#include <unistd.h>	/* for getdtablesize(), getcwd() declarations */
#include <limits.h> /* for PATH_MAX */

#define SPARE_FDS 5	/* fds for use by other functions, see text */

extern int process(const char *file, const struct stat *sb,
		   int flag, struct FTW *s);


void usage(const char *name)
{
	fprintf(stderr, "usage: %s directory\n", name);
	exit(1);
}

int main(int argc, char **argv)
{
	int nfds;
	int flags = FTW_PHYS;

	if (argc != 2) {
		usage(argv[0]);
	}

	nfds = getdtablesize() - SPARE_FDS;	/* leave some spare descriptors */

	nftw(argv[1], process, nfds, flags);

	return 0;
}

/* Print out each file at the right level */
int process(const char *file, __attribute__((unused))const struct stat *sb, int flag, struct FTW *s) {
	int retval = 0;
	const char *name = file + s->base;

	printf("%*s", s->level * 4, "");	/* indent over */

	switch (flag) {
		case FTW_F:
			printf("%s (file)\n", name);
			break;
		case FTW_D:
			printf("%s (directory)\n", name);
			break;
		case FTW_DNR:
			printf("%s (unreadable directory)\n", name);
			break;
		case FTW_SL:
			printf("%s (symbolic link)\n", name);
			break;
		case FTW_NS:
			printf("%s (stat failed): %s\n", name, strerror(errno));
			break;
		case FTW_DP:
		case FTW_SLN:
			printf("%s: FTW_DP or FTW_SLN: can't happen!\n", name);
			retval = 1;
			break;
		default:
			printf("%s: unknown flag %d: can't happen!\n", name, flag);
			retval = 1;
			break;
	}

	return retval;
}
