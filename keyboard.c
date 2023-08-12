
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

/*
struct input_event {
        struct timeval time;
        unsigned short type;
        unsigned short code;
        unsigned int value;
};
*/
/*
& => del, 1 => 14
tab 15
a => enter, 16 => 28
q => ù, 30 => 40
* 43
w => !, 44 => 53
² 41
l_numlock 58
l_shift 42
< 86
l_ctrl 29
l_alt 56
l_win 125
up 103 down 108 left 105 right 106
*/


enum ev_int { RELEASED = 0, PRESSED, REPEATED };


int main(int argc, char **argv)
{
	if(argc != 1) {
		printf("usage : %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}

    const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    struct input_event ev;
    ssize_t n;
    int fd;

    fd = open(dev, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Cannot open %s : %s.\n", dev, strerror(errno));
        return EXIT_FAILURE;
    }

	printf("%d\n", KEY_A);

	while (1) {
        n = read(fd, &ev, sizeof ev);
        if (n == (ssize_t)-1) {
            if (errno == EINTR)
                continue;
            else
                break;
        }
		else if (n != sizeof ev) {
            errno = EIO;
            break;
        }

		switch(ev.type) {
			case EV_SYN:
				/*
				printf("EV_SYN, ");
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				*/
				break;

			case EV_KEY:
				printf("EV_KEY : ");
				switch(ev.value) {
					case RELEASED:
						printf("RELEASED 0x%04x (%d)\n", (int)ev.code, (int)ev.code);
						break;

					case PRESSED:
						printf("PRESSED  0x%04x (%d)\n", (int)ev.code, (int)ev.code);
						break;

					case REPEATED:
						printf("REPEATED 0x%04x (%d)\n", (int)ev.code, (int)ev.code);
						break;

					default:
						printf("unknown %d\n", ev.value);
						break;
				}
				break;

			case EV_REL:
				printf("EV_REL, ");
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				break;

			case EV_ABS:
				printf("EV_ABS, ");
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				break;

			case EV_MSC:
				/*
				printf("EV_MSC, ");
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				*/
				break;

			case EV_SW:
				printf("EV_SW, ");
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				break;

			case EV_LED:
				printf("EV_LED, ");
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				break;

			case EV_SND:
				printf("EV_SND, ");
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				break;

			case EV_REP:
				printf("EV_REP, ");
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				break;

			case EV_FF:
				printf("EV_FF, ");
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				break;

			case EV_PWR:
				printf("EV_PWR, ");
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				break;

			case EV_FF_STATUS:
				printf("EV_FF_STATUS, ");
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				break;

			default:
				printf("unknown ev.type : %d, ", ev.type);
				printf("code:%d value:%d\n", (int)ev.code, (int)ev.value);
				break;
		}
	}

	fflush(stdout);
	fprintf(stderr, "%s.\n", strerror(errno));

	return EXIT_FAILURE;
}

