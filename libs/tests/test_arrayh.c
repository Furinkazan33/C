#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "arrayh.h"

typedef struct person {
	int id;
	char name[32];
} person;

#define  PERSON_LOG(p, fd) LOG(fd, "person[%d][%s]\n", (p).id, (p).name);


// array of person
bool test_1() {
	person *array;
	ARRAYH_NEW(array, 1, return false, );
	
	assert(ARRAYH_HEAD(array)->count == 0);
	assert(ARRAYH_HEAD(array)->capacity == 1);
		
	person p1 = { 1, "person p1" };
	person p2 = { 2, "person p2" };
	person p3 = { 3, "person p3" };

	ARRAYH_ADD(array, p1, return false);
	ARRAYH_ADD(array, p2, return false);
	ARRAYH_ADD(array, p3, return false);
	
	assert(ARRAYH_HEAD(array)->count == 3);
	assert(ARRAYH_HEAD(array)->capacity == 4);
	
	#ifdef DEBUG
		ARRAYH_LOG(array);
		ARRAYH_MAP_ARGS(array, PERSON_LOG, stdout);
	#endif
	
	//ARRAYH_DEL(array, &array[1]);
	ARRAYH_DEL_IDX(array, 1);
	
	assert(ARRAYH_HEAD(array)->count == 2);
	assert(ARRAYH_HEAD(array)->capacity == 4);
	
	#ifdef DEBUG
		ARRAYH_LOG(array);
		ARRAYH_MAP_ARGS(array, PERSON_LOG, stdout);
	#endif
	
	
	ARRAYH_FREE(array);
	return true;
}


// array of person *
bool test_2() {
	person **array;
	ARRAYH_NEW(array, 1, return false, );
	
	assert(ARRAYH_HEAD(array)->count == 0);
	assert(ARRAYH_HEAD(array)->capacity == 1);
		
	person p1 = { 1, "person p1" }; person *pp1 = &p1;
	person p2 = { 2, "person p2" }; person *pp2 = &p2;
	person p3 = { 3, "person p3" }; person *pp3 = &p3;

	ARRAYH_ADD(array, pp1, return false);
	ARRAYH_ADD(array, pp2, return false);
	ARRAYH_ADD(array, pp3, return false);
	
	assert(ARRAYH_HEAD(array)->count == 3);
	assert(ARRAYH_HEAD(array)->capacity == 4);
	
	#ifdef DEBUG
		ARRAYH_LOG(array);
		ARRAYH_MAP_PTR_ARGS(array, PERSON_LOG, stdout);
	#endif
	
	//ARRAYH_DEL(array, &array[1]);
	ARRAYH_DEL_IDX(array, 1);
	
	assert(ARRAYH_HEAD(array)->count == 2);
	assert(ARRAYH_HEAD(array)->capacity == 4);
	
	#ifdef DEBUG
		ARRAYH_LOG(array);
		ARRAYH_MAP_PTR_ARGS(array, PERSON_LOG, stdout);
	#endif
	
	
	ARRAYH_FREE(array);
	return true;
}



int main() {
	
	IF_LOG_E_MESS(!test_1(), return 1, "test_1 error");
	
	IF_LOG_E_MESS(!test_2(), return 1, "test_2 error");

	return 0;
}


