#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

arrayptr *persons;
arrayptr *cities;
arrayptr *countries;

person *person_new(int id, int city, char *name, char *adress) {
	person *res = malloc(sizeof(*res));
	res->id = id;
	res->city = city;
	strcpy(res->name, name);
	strcpy(res->adress, adress);
	return res;
}

city *city_new(int id, int country, char *name, int population) {
	city *res = malloc(sizeof(*res));
	res->id = id;
	res->country = country;
	strcpy(res->name, name);
	res->population = population;
	return res;
}

country *country_new(int id, char *name) {
	country *res = malloc(sizeof(*res));
	res->id=id;
	strcpy(res->name, name);
	return res;
}

void load_persons() {
	persons = arrayptr_new(10);
	arrayptr_append(persons, person_new(1, 1, "P1", "A1"));
	arrayptr_append(persons, person_new(2, 2, "P2", "A2"));
	arrayptr_append(persons, person_new(3, 1, "P3", "A3"));
}

void load_cities() {
	cities = arrayptr_new(10);
	arrayptr_append(cities, city_new(1, 1, "City1", 1000));
	arrayptr_append(cities, city_new(2, 1, "City2", 200000));
	arrayptr_append(cities, city_new(3, 1, "City3", 3000000));
}

void load_counties() {
	countries = arrayptr_new(10);
	arrayptr_append(countries, country_new(1, "France"));
	arrayptr_append(countries, country_new(2, "Allemagne"));
}

int city_cmp_id(void *c1, void *c2) {
	city *cur = c1;
	city *search = c2;
	return cur->id - search->id;
}

city *person_get_city(person *p) {
	city search = { p->city, 0, {0}, 0 };
	city *res = arrayptr_find(cities, &search, city_cmp_id);

	if(!res) {
		fprintf(stderr, "person_get_city : cannot find city with id=%d\n", p->city);
	}
	return res;
}



