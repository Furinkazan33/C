#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "person.h"
#include "list.h"

int cmp_id(void *p1, void *p2) {
	return ((person *)p1)->id - ((person *) p2)->id;
}

int main(void) {
	typedef vlist list;

	list *l = NULL;

	person *p1 = person_new(1, 21, "toto 1", "");
	person *p2 = person_new(2, 22, "toto 2", "");
	person *p3 = person_new(3, 23, "toto 3", "");
	person *p4 = person_new(4, 24, "toto 4", "");
	person *p5 = person_new(5, 25, "toto 5", "");
	person *p6 = person_new(6, 26, "toto 6", "");

	l = (list *) list_new(p3); if(!l) { return 1; }
	list *p = l;
	p = list_insert_after(p, p2); if(p == NULL) { return 1; }
	p = list_insert_after(p, p1); if(p == NULL) { return 1; }
	p = list_insert_after(p, p4); if(p == NULL) { return 1; }
	p = list_insert_after(p, p6); if(p == NULL) { return 1; }
	p = list_insert_after(p, p5); if(p == NULL) { return 1; }

	puts("list:");
	LIST_MAP_DATA_ARGS(l, person_write, stdout);
	
	puts("searching p4 and prec:");
	vlist *prec = find_prec(l, p4, cmp_id);
	vlist *found = find(l, p4, cmp_id);
	
	if(prec) {
		printf("prec="); person_write((person *) (prec->data), stdout);
	}
	if(found) {
		printf("found="); person_write((person *) (found->data), stdout);
	}
	
	puts("\npoping n=0, list:");
	p = list_pop(&l, 0);
	LIST_MAP_DATA_ARGS(l, person_write, stdout);
	puts("\npoped 0:");
	person_write((person *) p->data, stdout);
	puts("\nfree poped, assert NULL");
	LIST_FREE(p, person_free);
	assert(p == NULL);
	
	puts("\npoping n=2, list:");
	p = list_pop(&l, 2);
	LIST_MAP_DATA_ARGS(l, person_write, stdout);
	
	puts("\npoped:");
	person_write((person *) p->data, stdout);
	puts("\nfree poped, assert NULL");
	LIST_FREE(p, person_free);
	assert(p == NULL);
	
	puts("\npoping n=3, list:");
	p = list_pop(&l, 3);
	LIST_MAP_DATA_ARGS(l, person_write, stdout);
	puts("\npoped:");
	person_write((person *) p->data, stdout);
	puts("\nfree poped, assert NULL");
	LIST_FREE(p, person_free);
	assert(p == NULL);
	
	puts("\nfree all on list, assert NULL");
	LIST_FREE_ALL(l, person_free);
	assert(l == NULL);

	return 0;
}
