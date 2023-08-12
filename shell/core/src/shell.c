#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

#define CMD_ALLOC_CHARS 64
#define CMDS_ALLOC_POINTERS 10
#define CMDS_ALLOC_CHARS 64

int char_is_separator(char c) {
	return c == ' ' || c == '\t';
}

char *cmds_alloc(size_t size) {
	char *res = malloc(sizeof(char) * (size + 1));
	if(!res) {
		perror("");
		return NULL;
	}
	return res;
}

char **split(char *string, size_t *n) {
	assert(string);

	char *p = string;
	size_t size = CMDS_ALLOC_POINTERS;
	char **cmds;

	cmds = malloc(sizeof(char *) * (size + 1)); if(!cmds) { perror(""); return NULL; }
	cmds[0] = cmds_alloc(CMDS_ALLOC_CHARS); if(!cmds[0]) { return NULL; }

	size_t l;
	for(*n = 0, l = 0; *p != '\0';) {
		/* realloc */
		if(*n >= size) {
			size *= 2;
			cmds = realloc(cmds, (size + 1)); if(!cmds) { perror(""); return NULL; }
		}

		if(char_is_separator(*p)) {
			cmds[*n][l] = '\0';
			l = 0;

			while(char_is_separator(*p)) {
				p++;
			}
			if(*p != '\0') {
				(*n)++;
				cmds[*n] = cmds_alloc(CMDS_ALLOC_CHARS); if(!cmds[*n]) { return NULL; }
			}
		}
		else {
			cmds[*n][l] = *p;
			l++;
			p++;
		}
	}

	cmds[*n][l] = '\0';
	(*n)++;

	return cmds;
}


void print_ps1(char *ps1) {
	printf("%s", ps1);
}

void execute(char *cmd, char **parameters, size_t n) {
	puts("");
	printf("command : [%s], parameters : [", cmd);
	for(size_t i = 0; i < n; i++) {
		printf("%s, ", parameters[i]);
	}
	printf("]\n");
}

char *read_cmd() {
	size_t size = CMD_ALLOC_CHARS;
	char *res = malloc(sizeof(char) * (size + 1));
	if(!res) {
		perror("");
		return NULL;
	}

	char *p = res;
	while((*p = getchar()) != '\n') {
		switch(*p)
    	{
	    	case 91:
				*p = getchar();

				switch(*p) {
					case 65:
		   				printf("\nUp Arrow");
		                break;
					case 66:
		  				printf("\nDown Arrow");
		                break;
					case 67:
				   		printf("\nRight Arrow");
		                break;
					case 68:
				  		printf("\nLeft Arrow");
                		break;
				}
		    default:
				break;
	    }

		p++;
		if((size_t)(p - res) >= size) {
			size *= 2;
			res = realloc(res, size + 1);
			if(!res) {
				perror("");
				return NULL;
			}
		}
	}
	*p = '\0';

	return res;
}

int main(void){
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

			execute(cmds[0], &cmds[1], n - 1);

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



