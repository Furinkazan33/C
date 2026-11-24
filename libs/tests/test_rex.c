#include <stdio.h>
#include <stdbool.h>
#include "rex.h"


bool test_c() {
	char *tag = "static const char *s1=\"toto\";";
	char **p = &tag;
	bool res;
	
	group *g = group_new();
	READ(g, TT_GLOBAL, res, p, read_c_vardeclassign, 1, 1);
	
	#ifdef DEBUG
		group_print(g, 0);
	#endif
	
	group_free(g);
	
	return res;
}

bool test_xml() {
	char *tag = "<html><head></head><body>before<p id=12>text</p>after</body></html>  ";
	char **p = &tag;
	bool res;
	
	group *g = group_new();
	READ(g, TT_GLOBAL, res, p, read_xml, 1, 1);
	
	#ifdef DEBUG
		group_print(g, 0);
	#endif
	
	group_free(g);
	
	return res;
}

int main() {
	
	IF_LOG_E_MESS(!test_c(), return 1, "test_c error")
	
	IF_LOG_E_MESS(!test_xml(), return 1, "text_xml error")
	
	return 0;
}
