#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "kbd.h"
#include "macros.h"


#ifdef WIN32
	#include <winuser.h>
	
	int test_win32() {
		HWND hwnd;
		MSG msg;
		
		int fDone = 0; 
		while(!fDone) { 

			while(PeekMessage(&msg, hwnd,  0, 0, PM_REMOVE)) {
				switch(msg.message) 
				{ 
					case WM_LBUTTONDOWN: 
					case WM_RBUTTONDOWN: 
					case WM_KEYDOWN: 
						// 
						// Perform any required cleanup. 
						// 
						fDone = 1; 
				} 
			}
		}
		return 0;
	}
#endif


int test_linux() {
	keyboard *k = kbd_open("/dev/input/by-path/"); if(!k) return 1;

	#ifdef DEBUG
		LOG_S("keyboard open ok");
	#endif

	while(1) {
		if(!kbd_read(k)) return 1;
		
		#ifdef DEBUG
			kbd_log(stdout, k);
		#endif
		
		if(k->pressed[KEY_LEFTCTRL] && k->pressed[KEY_Q] == 1) {
			puts("CTRL A");
		}
	}

	kbd_free(k);
	
	return 0;
}


int main(int argc, char **argv)
{
	if(argc != 1) {
		printf("usage : %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if(!test_linux()) return 1;


	return EXIT_SUCCESS;
}

