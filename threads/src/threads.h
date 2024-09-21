#include <stdlib.h>
#include <unistd.h>
#include "parray.h"


typedef struct {
	pid_t pid;
	pid_t ppid;
	char name[32];
} thread;







