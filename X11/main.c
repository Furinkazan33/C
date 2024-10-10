#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>


int loop_window(Display *d, Window w) {
	XEvent event;
	Atom delete = XInternAtom(d, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(d, w, &delete, 1);

	while(event.type != ClientMessage && event.xclient.data.l[0] != delete) {
		XNextEvent(d, &event);
	}
	XUnmapWindow(d, w);
	XDestroyWindow(d, w);
	XCloseDisplay(d);
}


void test() {
	XEvent event_return;
	Display *d = XOpenDisplay(NULL);
	Window rootWindow = XDefaultRootWindow(d);

	Window w = XCreateSimpleWindow(d, rootWindow, 
			50, 50, 
			400, 250, 
			0, 0, 
			0xFFFFFFFF);

	XMapWindow(d, w);
	XFlush(d);

	loop_window(d, w);

}



int main() {
	test();

	return 0;
}
