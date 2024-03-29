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

int message_cmp(void *m1, void *m2) {
	return ((message *)m1)->id - ((message *)m2)->id;
}
int message_write(void *m, void *file) {
	fprintf((FILE *)file, "%s %d\n", ((message *)m)->content, ((message *)m)->id);
	return 1;
}
void message_free(void *m) {
	free((message *)m);
}
message *message_copy(message *m) {
	return message_new(m->id, m->content);
}
