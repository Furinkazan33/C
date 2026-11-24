#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <stdbool.h>
#include <time.h>
#include "sync.h"
#include "arrayh.h"



/*
 *	client
 */
client *client_new(char *server_config, char *client_config, char *files_to_sync) {
    client *res;
	MALLOC(res, sizeof(*res), return NULL, );
	
	res->server_config_path = server_config;
	res->client_config_path = client_config;
	res->files_to_sync_path = files_to_sync;
	
	res->server_config = NULL;
	res->client_config = NULL;
	res->files_to_sync = NULL;
	
	res->lastupdate = 0;
	res->n_updates = 0;
	
    return res;
}

void client_free(client *c) {
	ARRAYH_DO(c->server_config, i, {
		ARRAYH_DO(c->server_config[i], j, {
			free(c->server_config[i][j]);
		});
		ARRAYH_FREE(c->server_config[i]);
	});
	ARRAYH_FREE(c->server_config);
	
	ARRAYH_DO(c->client_config, i, {
		ARRAYH_DO(c->client_config[i], j, {
			free(c->client_config[i][j]);
		});
		ARRAYH_FREE(c->client_config[i]);
	});
	ARRAYH_FREE(c->client_config);
	
	
	ARRAYH_DO(c->files_to_sync, i, {
		free(c->files_to_sync[i]);
	});
	ARRAYH_FREE(c->files_to_sync);
	
	free(c);
}

client *client_update(client *c) {
	time_t current_time = time(NULL);
	
	char *interval = NULL;
	ARRAYH_DO(c->client_config, i, {
		if(!strcmp(c->client_config[i][0], CFG_INTERVAL)) {
			interval = c->client_config[i][1];
			break;
		}
	});
	
	if(!interval) {
		LOG_E("config not found [%s]\n", CFG_INTERVAL);
		return NULL;
	}

	if(current_time - c->lastupdate >= atoi(interval)) {
		LOG_S("%ld\n", c->lastupdate);
		c->lastupdate = current_time;
		c->n_updates++;
	}
	
	return c;
}


