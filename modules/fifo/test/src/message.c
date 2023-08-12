#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <message.h>

message *message_new(int id, char *content) {
	message *res = malloc(sizeof(message));
	if(!res) {
		perror("");
		return NULL;
	}
	res->id = id;
	strncpy(res->content, content, MESS_MAX_LEN);
	return res;
}

int message_cmp(message *m1, message *m2) {
	return m1->id - m2->id;
}
void message_print(message *m) {
	printf("%s %d\n", m->content, m->id);
}
void message_free(message *m) {
	free(m);
}
message *message_copy(message *m) {
	return message_new(m->id, m->content);
}
