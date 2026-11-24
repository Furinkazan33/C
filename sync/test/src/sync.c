#include <stdlib.h>
#include <stdio.h>
#include "sync.h"
#include "tree.h"
#include "file.h"


int _strcmp_on_ptr(const void *d1, const void *d2) {
	return strcmp(*(const char **) d1, *(const char **) d2);
}


int test_1() {
	client *c = client_new("server_config_file", "config_file", "syncs_list");
	
	c->server_config = file_load_cfg(c->server_config_path); if(!c->server_config) return 1;
	c->client_config = file_load_cfg(c->client_config_path); if(!c->client_config) return 1;
	c->files_to_sync = file_load_list(c->files_to_sync_path); if(!c->files_to_sync) return 1;
	
	// sorting files list on path
	qsort(c->files_to_sync, ARRAYH_CNT(c->files_to_sync), sizeof(char ***), _strcmp_on_ptr);

	
	printf("from file \"%s\"\n", c->server_config_path);
	FOR(i, 0, ARRAYH_CNT(c->server_config), { 
		cfg_fprintf(c->server_config[i], stdout, STRING_EQUAL);
	});
	
	printf("from file \"%s\"\n", c->client_config_path);
	FOR(i, 0, ARRAYH_CNT(c->client_config), { 
		cfg_fprintf(c->client_config[i], stdout, STRING_EQUAL);
	});

	printf("from file \"%s\"\n", c->files_to_sync_path);
	FOR(i, 0, ARRAYH_CNT(c->files_to_sync), { 
		fprintf(stdout, "%s\n", c->files_to_sync[i]);
	});
	
	while(c->n_updates < 4) {
		if(!client_update(c)) return 1;
	}
	
	client_free(c);

    return 0;
}




int main(int argc, char **argv) {
    if(argc != 1) {
        LOG_E("usage : %s\n", argv[0])
            return 1;
    }

	char *filelist[] = {
		"/home/mathieu/C/0_template/libs/macros.h", 
		"/home/mathieu/C/sync/",
	};

	tree *t = filelist_to_tree(filelist, 2);	
	tree_map(t, stree_fprintf, 0, stdout, NULL, NULL);
	tree_free(t, free);

	int ret = test_1(); if(ret != 0) { return ret; }
	


    return 0;
}


