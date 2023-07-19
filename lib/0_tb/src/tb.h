#include <stdio.h>
#include <inttypes.h>

/* For memory allocations */
#define BUF_LINES_ALLOC 256
#define BUF_LINE_ALLOC 128
#define BUF_LINE_CONTENT_ALLOC 128
#define BUF_NAME_LEN 32
#define BUF_PATH_LEN 256
#define BUF_LINE_TAGS 20

/* Available line marks  */
#define TB_IGN   0x01
#define TB_EMPT  0x02
#define TB_COM   0x04
#define TB_DUP   0x08
#define TB_ERR   0x16
#define TB_NEW   0x32
#define TB_INFO  0x64
#define TB_WARN  0x128
#define TB_LOCK  0x256
#define TB_WRITE 0x512
#define TB_DEL   0x1024
#define TB_F11   0x2048
#define TB_F12   0x4096
#define TB_F13   0x8192
#define TB_F14   0x16384
#define TB_F15   0x32768


typedef struct line {
	uint16_t marks;
	char *content;
	size_t size;
} line;

typedef struct txt_buf {
	char name[BUF_NAME_LEN];
	char filepath[BUF_PATH_LEN];
	size_t size;
	size_t n;
	line **lines;
} txt_buf;

typedef txt_buf tb;

void tb_mark(line *l, int mark);
void tb_free(tb *buf);
void tb_free_line(line *l);
line **tb_alloc_lines(tb *buf);
line *tb_alloc_line(line *dest, size_t min);
line *tb_readline(line *dest, FILE *f, int *n_read);
tb *tb_new(const char *filepath, const char *name);
int tb_load(tb *buf);
int tb_save(tb *buf);

