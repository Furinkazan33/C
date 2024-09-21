#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arrayptr.h"

typedef struct person {
	int id;
	int city;
	char name[64];
	char adress[256];
} person;

typedef struct city {
	int id;
	int country;
	char name[64];
	int population;
} city;

typedef struct country {
	int id;
	char name[64];
} country;


extern arrayptr *persons;
extern arrayptr *cities;
extern arrayptr *countries;


person *person_new(int id, int city, char *name, char *adress);
city *city_new(int id, int country, char *name, int population);
country *country_new(int id, char *name);

void load_persons();
void load_cities();
void load_counties();
city *person_get_city(person *p);




