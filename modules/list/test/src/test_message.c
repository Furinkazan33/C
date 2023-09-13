#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <message.h>
#include "list.h"


int main(void) {
	list *l = NULL;
	list *p = NULL;
	message *m = NULL;

	for(int i = 0; i < 5000; i++) {
   		m = message_new(i, "Hello you !"); if(!m) { return 1; }
		if(!l) {
			l = list_new(m); if(!l) { return 1; }
			p = l;
		}
		else {
			p = list_insert_after(p, m); if(!p) { return 1; }
		}
	}

	list_map2(l, message_write, stdout);

	list_free_all(l, message_free);

	return 0;
}
