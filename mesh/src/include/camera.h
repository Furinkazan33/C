#include <vect.h>

#define _POSIX_C_SOURCE 200809L

typedef struct camera{
	vect *origin;
	vect *axis;
	float field_of_view;
};
