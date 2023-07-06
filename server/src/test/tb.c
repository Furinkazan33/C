#include "tb.h"




int main(int argc, char **argv) {
	if(argc != 2) {
		return 1;
	}

	tb *buf = tb_new(argv[1], "new buffer");

	if(!buf) {
		return 1;
	}
	if(!tb_load(buf)) {
		return 1;
	}

	printf("filepath:%s:\n", buf->filepath);
	printf("buffer name:%s:\n", buf->name);
	printf("%ld lines\n", buf->n);

	for(size_t i = 0; i < buf->n; i++) {
		printf(":%s:\n", buf->lines[i]->content);
	}

	return 0;
}
