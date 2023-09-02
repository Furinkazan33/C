#include <sf.h>

#define m_cast(m) ((message *)m)

#define MESS_MAX_LEN 512

typedef struct message {
	int id;
	char content[MESS_MAX_LEN];
} message;

message *message_new(int id, char *content);
int message_cmp(void *m1, void *m2);
void message_write(void *m, FILE *file);
void message_free(void *m);
message *message_copy(message *m);


