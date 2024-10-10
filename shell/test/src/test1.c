#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "shell.h"

int main(void) {
    static struct termios oldt, newt;
	char *ps1 = ">> ";

    /*
	 * tcgetattr gets the parameters of the current terminal
	 * STDIN_FILENO will tell tcgetattr that it should write
	 * the settings of stdin to oldt.
	 * */
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    /*
	 * ICANON normally takes care that one line at a time will
	 * be processed that means it will return if it sees a
	 * "\n" or an EOF or an EOL.
	 * */
    newt.c_lflag &= ~(ICANON);

    /*
	 * Those new settings will be set to STDIN. TCSANOW tells
	 * tcsetattr to change attributes immediately.
	 * */
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	print_ps1(ps1);

	char *cmd;
	char **cmds;

    while(1) {
		cmd = read_cmd();

		if(strlen(cmd) > 0) {
			print_ps1(ps1);

			cmds = split(cmd);

			if (execute(cmds) == SHELL_RC_EXIT) {
				return 0;
			}
			cmds_free(cmds);
		}

		print_ps1(ps1);

		free(cmd);
	}

	/*
	 * restore old settings
	 * */
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return 0;
}



