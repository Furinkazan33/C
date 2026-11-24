#include <stdio.h>
#include "ht.h"
#include "macros.h"



/* 	
	hash functions
*/
size_t hash_int_to_idx(hashtable *ht, void *key) {
	return (* (int *) key) % ht->partitions;
}

// on se base sur le premier char pour choisir la partition
size_t hash_str_to_idx(hashtable *ht, void *key) {	
	unsigned char c = *(char *) key;
	
	// valeurs les plus utilisees pour du texte [32, 127]
	if(c >= 32 && c <= 127) {
		c -= 32;
		return c % ht->partitions;
	}
	else {
		// last partition for everything else
		return ht->partitions - 1;
	}
}


/*	
	hashtable_entry 
*/
hashtable_entry *hashtable_entry_new(void *key, size_t size_of_key, void *data) {
	hashtable_entry *res;
	MALLOC(res, sizeof(*res), return NULL, )
	
	MALLOC(res->key, size_of_key, return NULL, )
	memcpy(res->key, key, size_of_key);
	res->data = data;
	res->next = NULL;
	
	return res;
}

void hashtable_entry_free(hashtable_entry *e) {
	while(e) {
		hashtable_entry *p = e->next;
		free(e->key);
		free(e);
		e = p;
	}
}

int hashtable_entry_cmp_int(void *p, void *key) {
	return * (int *) p - * (int *) key;
}




/*	
	hashtable
*/
hashtable *hashtable_new(hashtable_type type, size_t partitions) {
	hashtable *res;
	MALLOC(res, sizeof (*res), return NULL, )
	
	res->partitions = partitions;
	res->type = type;
	
	switch(type) {
		case HT_INT:
			res->cmp = hashtable_entry_cmp_int;
			res->hash_key_to_idx = hash_int_to_idx;
			break;
		case HT_STR:
			res->cmp = (int (*)(void *, void *)) strcmp;
			res->hash_key_to_idx = hash_str_to_idx;
			break;
		case HT_CHAR:
			res->cmp = hashtable_entry_cmp_int;
			res->hash_key_to_idx = hash_str_to_idx;
			break;
		default:
			LOG_E("no such type:[%d]\n", type);
			return NULL;
	}
	
	MALLOC(res->entries, sizeof(*res->entries) * partitions, return NULL, )
	
	for(size_t i = 0; i < partitions; i++) {
		res->entries[i] = NULL;
	}
	
	return res;
}

void hashtable_free(hashtable *ht) {
	for(size_t p = 0; p < ht->partitions; p++) {
		hashtable_entry_free(ht->entries[p]);
	}
	free(ht);
}

hashtable *hashtable_add(hashtable *t, void *key, void *data) {
	if(!t->hash_key_to_idx) {
		LOG_E("Missing hash function")
		return NULL;
	}
	
	// creating new entry
	hashtable_entry *entry = NULL;
	switch(t->type) {
		case HT_INT:
			#ifdef HT_DEBUG
				LOG_S("as int, key=[%d]", *(int *) key)
			#endif
			entry = hashtable_entry_new(key, sizeof(int), data);
			break;
		case HT_STR:
			#ifdef HT_DEBUG
				LOG_S("as str, key=[%s]", (char *) key)
			#endif
			entry = hashtable_entry_new(key, sizeof(char) * (strlen(key) + 1), data);
			break;
		case HT_CHAR:
			#ifdef HT_DEBUG
				LOG_S("as char, key=[%c]", *((char *) key))
			#endif
			entry = hashtable_entry_new(key, sizeof(char), data);
			break;
	}
	
	// getting index partition
	unsigned int idx = t->hash_key_to_idx(t, key);
	
	#ifdef HT_DEBUG
		printf(", idx=[%d]\n", idx);
	#endif

	// adding new entry as head
	if(t->entries[idx]) {
		entry->next = t->entries[idx];
	}
	t->entries[idx] = entry;	

	return t;
}

hashtable_entry *hashtable_get_partition(hashtable *ht, void *key) {
	unsigned int idx = ht->hash_key_to_idx(ht, key);
	return ht->entries[idx];
}

void *hashtable_find(hashtable *ht, void *key) {
	if(!ht->cmp) {
		LOG_E("Missing compare function")
		return NULL;
	}
	
	int idx = ht->hash_key_to_idx(ht, key);
	
	if(idx < 0) {
		return NULL;
	}
	
	hashtable_entry *e = ht->entries[idx];
	
	while(e) {
		if(!ht->cmp(e->key, key)) {
			return e->data;
		}
		e = e->next;
	}
	
	return NULL;
}

void hashtable_log_as_int(hashtable *ht, bool all) {
	for(size_t i = 0; i < ht->partitions; i++) {
		
		hashtable_entry *e = ht->entries[i];
		
		if(e) {
			LOG_S("[%ld]=>", i)
		}
		
		if(all) {
			hashtable_entry *p = e;
			
			while(p) {
				printf("[%d]", *(int *) p->data);
				p = p->next;
			}
		}
		else {
			printf("[%d]", *(int *) e->data);
			
			int more = 0;
			for(hashtable_entry *p = e; p; p = p->next) {
				more++;
			}
			
			if(more > 0) {
				printf(" ... %d more", more);
			}
		}
		
		if(e) {
			printf("\n");
		}
	}
}

void hashtable_log_as_str(hashtable *ht, bool all) {
	for(size_t i = 0; i < ht->partitions; i++) {
		
		hashtable_entry *e = ht->entries[i];
		
		if(e) {
			LOG_S("[%ld]=>", i)
		}
		
		if(all) {
			hashtable_entry *p = e;
			
			while(p) {
				printf("[%s]", (char *) p->data);
				p = p->next;
			}
		}
		else {
			printf("[%s]", (char *) e->data);
			
			int more = 0;
			for(hashtable_entry *p = e; p; p = p->next) {
				more++;
			}
			
			if(more > 0) {
				printf(" ... %d more", more);
			}
		}
		
		if(e) {
			printf("\n");
		}
	}
}

