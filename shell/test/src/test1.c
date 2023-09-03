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
	size_t n;

    while(1) {

		cmd = read_cmd();

		if(strlen(cmd) >= 1) {
			print_ps1(ps1);

			cmds = split(cmd, &n);


			int rc = execute(cmds[0], &cmds[1], n - 1);

			if(rc == SHELL_RC_EXIT) {
				return 0;
			}

			/* free */
			for(size_t i = 0; i < n; i++) {
				free(cmds[i]);
			}
			free(cmds);
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



