#include <string.h>
#include <stdio.h>
#include "../cJSON/cJSON.h"

#define MAX_PASSENGERS 5
#define MAX_STR_LEN 128

typedef struct passenger {
	char name[MAX_STR_LEN];
	int age;
} passenger;

typedef struct car {
	char model[MAX_STR_LEN];
	char manufacturer[MAX_STR_LEN];
	passenger passengers[MAX_PASSENGERS];
} car;

int main() {

	car c;
	strcpy(c.manufacturer, "Ferrari");
	strcpy(c.model, "F11");
	strcpy(c.passengers[0].name, "Mathieu");
	c.passengers[0].age = 40;
	strcpy(c.passengers[1].name, "Fen");
	c.passengers[1].age = 40;
	strcpy(c.passengers[2].name, "Julie");
	c.passengers[2].age = 10;
	strcpy(c.passengers[3].name, "Lucie");
	c.passengers[3].age = 7;


	cJSON *car = cJSON_CreateObject();
	cJSON_AddStringToObject(car, "manufacturer", c.manufacturer);
	cJSON_AddStringToObject(car, "model", c.model);
	cJSON *passengers = cJSON_AddArrayToObject(car, "passengers");

	for(int i=0; i<4; i++) {
		cJSON *p = cJSON_CreateObject();
		cJSON_AddStringToObject(p, "name", c.passengers[i].name);
		cJSON_AddNumberToObject(p, "name", c.passengers[i].age);
		cJSON_AddItemToArray(passengers, p);
	}

	char *string = cJSON_Print(car);

	printf("%s\n", string);

	return 0;
}


