#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"

enum message_type {
	NONE = 1 << 0, 
	
	USER_INPUT = 1 << 1, 
	USER_MESSAGE = 1 << 2, 
	USER = USER_INPUT | USER_MESSAGE,
	
	LOG_ERROR = 1 << 4,
	LOG_INFO = 1 << 5,
	LOG_DEBUG = 1 << 6,
	LOG = LOG_ERROR | LOG_INFO | LOG_DEBUG,
	
	ALL = NONE | USER | LOG,
};


char *message_type_tos(int type) {
	switch(type) {
		case NONE:
			return "NONE";
			
		case USER_INPUT:
			return "USER_INPUT";
		case USER_MESSAGE:
			return "USER_MESSAGE";
		case USER:
			return "USER";
			
		case LOG_ERROR:
			return "LOG_ERROR";
		case LOG_INFO:
			return "LOG_INFO";
		case LOG_DEBUG:
			return "LOG_DEBUG";
		case LOG:
			return "LOG";
			
		case ALL:
			return "ALL";
			
		default:
			return NULL;
	}
}

typedef struct message {
	enum message_type type;
	char message[60];
	struct message *next;
} message;


void message_log(message *list, int flags) {
	char *type = message_type_tos(flags);
	
	if(type) {
		LOG_S("logging messages of type [%s]\n", type);
	}
	else {
		LOG_S("logging messages of type [%d]\n", flags);
	}

	for(message *m = list; m; m = m->next) {
		if(m->type & flags) {
			LOG_S("[%s]\n", m->message);
		}
	}
}


int main() {
	message m9 = { USER, "9 message type USER", NULL }; 
	message m8 = { LOG_INFO, "8 message type LOG_INFO", &m9 };
	message m7 = { LOG_DEBUG, "7 message type LOG_DEBUG", &m8  };
	message m6 = { LOG_INFO, "6 message type LOG_INFO", &m7  };
	message m5 = { LOG_ERROR, "5 message type LOG_ERROR", &m6  };
	message m4 = { USER_MESSAGE, "4 message type USER_MESSAGE", &m5 };
	message m3 = { USER_INPUT, "3 message type USER_INPUT", &m4  };
	message m2 = { USER_INPUT, "2 message type USER_INPUT", &m3  };
	message m1 = { NONE, "1 message type NONE", &m2 };

	message_log(&m1, ALL);


	return 0;
}