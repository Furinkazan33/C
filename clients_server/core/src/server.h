#include <stdlib.h>


#define SERV_BUF_SIZE 1024
#define SERV_MESS_SIZE 128
#define SERV_MAX_CLI 2

int serv_mess_hdl(int fd, char *message);
int serv_adm_hdl(int fd, char *message);
void serv_sig_catch(int signal);

int server_launch(char *confpath, int (*mess_hdl)(int, char *),	int (*adm_hdl)(int, char *), void (*sig_catch)(int));


