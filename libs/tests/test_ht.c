#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#include "ht.h"
#include "macros.h"

int test_char() {
	hashtable *ht = hashtable_new(HT_CHAR, 30);
	
	char keys[] = { 'a', 'b', 'c', 'd', 'e', 'e' };
	char *data[] = { "d1", "d2", "d3", "d4", "d5", "d6" };
	
	for(int i = 0; i < 6; i++) {
		hashtable_add(ht, &keys[i], data[i]);
	}
	
	hashtable_log_as_str(ht, true);

	size_t n_found;
	
	for(int i = 0; i < 5; i++) {
		char **found = (char **) hashtable_find(ht, &keys[i], &n_found, 1); if(!found) { return 1; }
		
		LOG_S("n_found:[%ld]\n", n_found);
		
		for(size_t n = 0; n < n_found; n++) {
			LOG_S("[%s]\n", found[n]);
		}
		
		free(found);
	}
	
	hashtable_delete(ht, &keys[4]);
	hashtable_log_as_str(ht, true);

	hashtable_free(ht);
	
	return 0;
}

int test_str() {
	hashtable *ht = hashtable_new(HT_STR, 30);
	
	char *keys[] = { "k1", "k2", "k3", "k4", "k5", "k5" };
	char *data[] = { "d1", "d2", "d3", "d4", "d5", "d6" };
	
	for(int i = 0; i < 6; i++) {
		hashtable_add(ht, keys[i], data[i]);
	}
	
	hashtable_log_as_str(ht, true);

	size_t n_found;
	
	for(int i = 0; i < 5; i++) {
		char **found = (char **) hashtable_find(ht, keys[i], &n_found, 1); if(!found) { return 1; }
		
		LOG_S("n_found:[%ld]\n", n_found);
		
		for(size_t n = 0; n < n_found; n++) {
			LOG_S("[%s]\n", found[n]);
		}
		
		free(found);
	}
	
	
	hashtable_delete(ht, keys[4]);
	hashtable_log_as_str(ht, true);
	

	hashtable_free(ht);
	
	return 0;
}

int test_int64() {
	hashtable *ht = hashtable_new(HT_INT64, 30);
	
	int64_t keys[] = { LLONG_MIN, 0, LLONG_MAX, LLONG_MAX, LLONG_MAX, LLONG_MAX };
	char *data[] = { "d1", "d2", "d3", "d4", "d5", "d6" };
	
	for(int i = 0; i < 6; i++) {
		hashtable_add(ht, &(keys[i]), data[i]);
	}
	
	hashtable_log_as_str(ht, true);
	
	size_t n_found;
	
	for(int i = 0; i < 3; i++) {
		char **found = (char **) hashtable_find(ht, &keys[i], &n_found, 1); if(!found) { return 1; }
		
		LOG_S("n_found:[%ld]\n", n_found);
		
		for(size_t n = 0; n < n_found; n++) {
			LOG_S("[%s]\n", found[n]);
		}
		
		free(found);
	}

	hashtable_delete(ht, &keys[2]);
	hashtable_log_as_str(ht, true);
	
	hashtable_free(ht);
	
	return 0;
}


int main() {
	puts("=========================");
	puts("test_char");
	puts("=========================");
	if(test_char() != 0) return 1;
	
	puts("=========================");
	puts("test_str");
	puts("=========================");
	if(test_str() != 0) return 1;
	
	puts("=========================");
	puts("test_int64");
	puts("=========================");
	if(test_int64() != 0) return 1;
	
	
	return 0;
}
