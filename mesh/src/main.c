#include <stdio.h>
#include <triangle.h>

#define _POSIX_C_SOURCE 200809L

int main(void) {
	vector *p0 = vect_new(0.0, 0.0, 0.0);
	vector *p1 = vect_new(2.0, 5.0, 0.0);
	vector *p2 = vect_new(10.0, 2.0, 0.0);
	
	triangle *t = tri_new(p0, p1, p2);
	
	tri_print(t);
	
	
	tri_delete(t);
	
	
	return 0;
}