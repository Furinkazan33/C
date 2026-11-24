#define m_cast(m) ((message *)m)

#define MESS_MAX_LEN 511

typedef struct message {
	int id;
	char pad[4];
	char content[MESS_MAX_LEN + 1];
} message;

message *message_new(int id, char *content);
int message_cmp(message *m1, message *m2);
void message_fprintf(message *m, FILE *out);
void message_print(message *m);
void message_free(message *m);
message *message_copy(message *m);


