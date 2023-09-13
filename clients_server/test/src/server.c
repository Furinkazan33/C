#include "usage.h"
#include "server.h"



int main(int argc, char **argv) {
	if(argc != 2) {
		usage(argc, argv, "", "config_file_path");
		exit(1);
	}

	return server_launch(argv[1], serv_mess_hdl, serv_adm_hdl, serv_sig_catch);
}


