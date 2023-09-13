#include <stdlib.h>

#define CLI_BUF_SIZE 1024
#define CLI_MESS_SIZE 128


void mess_treat(int fd, char *message, int len, int *rc);

int launch_client(int client_id);


