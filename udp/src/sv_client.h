#include <stdlib.h>
#include <time.h>

#define CL_STATUS_INFO_LEN 32

#define CL_FIRSTNAME_LEN 128
#define CL_LASTNAME_LEN 128
#define CL_PSEUDO_LEN 32
#define CL_EMAIL_LEN 128
#define CL_COMMENT_LEN 128


typedef enum cl_status {
	CLS_NONE=0,
	CLS_REGISTERING,
	CLS_REGISTERED,
	CLS_LOGGED_IN, 
	CLS_LOGGED_OUT,
	CLS_KICKED,
	CLS_BANNED,
} cl_status;


struct cl_connection {
	int id;
	int socket;
	char ip[16];
	time_t time;
	bool active; // a client is connected to the socket, identified or not
} cl_connection;

// client as a person that can be logged to his account via a connection
typedef struct client {
	int id;
	
	cl_connection connection;
	
	enum cl_status status;
	char status_info[CL_STATUS_INFO_LEN];
	
	char firstname[CL_FIRSTNAME_LEN];
	char lastname[CL_LASTNAME_LEN];
	char pseudo[CL_PSEUDO_LEN];
	char email[CL_EMAIL_LEN];
	char comment[CL_COMMENT_LEN];
} client;

// if(c.status >= CLS_LOGGED_OUT) { give reason why }
