#include <stdio.h>
#include "database.h"


int main() {
	load_persons();
	load_cities();
	load_counties();

	person *p = arrayptr_get(persons, 1);

	printf("Looking for person %d %s city %d\n", p->id, p->name, p->city);

	city *res = person_get_city(p);

	printf("city found : %d %s\n", res->id, res->name);


	return 0;
}

