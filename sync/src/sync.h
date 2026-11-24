#include <stdio.h>
#include <stdlib.h>

#include "macros.h"

// S_ISDIR(attr->st_mode); folder
// S_ISREG(attr->st_mode); regular file


#define CFG_INTERVAL "interval"



/*
 *	client
 */
typedef struct client {
	char *server_config_path;
	char *client_config_path;
	char *files_to_sync_path;
	
	char ***server_config;
	char ***client_config;
	char **files_to_sync;
	
	time_t lastupdate;
	size_t n_updates;
} client;


client *client_new(char *server_config, char *client_config, char *files_to_sync);
void client_free(client *c);
client *client_update(client *c);

