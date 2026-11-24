#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "macros.h"



int main(int argc, char **argv) {
    if(argc != 1) {
        LOG_E("usage : %s\n", argv[0]);
		return 1;
    }

	char *filelist[] = {
		"/home/mathieu/C/0_template/libs/macros.h", 
		"/home/mathieu/C/sync/",
	};

	tree *t = filelist_to_tree(filelist, 2);
	tree_map(t, stree_fprintf, 0, stdout, NULL, NULL);
	tree_free(t, free);

    return 0;
}
