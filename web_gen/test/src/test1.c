#include <stdlib.h>
#include <stdio.h>
#include "array.h"
#include "XXXX.h"



int test_2() {


	return 0;
}

int test_1() {
	
	// html
	app html = { 1, "html", "app1", NULL, NULL, NULL };
	(&html)->children = array_new(10, sizeof(app));
	
	// body
	app body = { 2, "body", "app1", NULL, NULL, NULL };
	(&body)->children = array_new(10, sizeof(app));
	
	// table
	char *l1[] = { "Company", "Contact", "Country", 0 };
	char *l2[] = { "Alfreds Futterkiste", "Maria Anders", "Germany", 0 };
	char *l3[] = { "Centro comercial Moctezuma", "Francisco Chang", "Mexico", 0 };
	void *data[] = { l1, l2, l3, 0 };
	data_source src = { 1, "ds1", data };
	app table = { 3, "TABLE_W_HEADERS", "app1", NULL, &src, NULL };
	
	
	array_add((&html)->children, &body);
	array_add((&body)->children, &table);
	
	char *res = gen(&html);
	LOG_S("\n%s\n", res)
	
	free(res);
	
	return 0;
}

int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s\n", argv[0]);
		return 1;
	}

	test_1();
test_2();

	return 0;
}

