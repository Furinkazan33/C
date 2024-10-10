#include <stdlib.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

#define CMD_ALLOC_CHARS 64
#define CMDS_ALLOC_POINTERS 10
#define CMDS_ALLOC_CHARS 64

#define SHELL_RC_OK 0
#define SHELL_RC_ERROR 1
#define SHELL_RC_EXIT 2

int char_is_separator(char c);
char *cmds_alloc(size_t size);
char **split(char *s);
void print_ps1(char *ps1);
int execute(char **cmd);
char *read_cmd();

void cmds_free(char **cmds);
