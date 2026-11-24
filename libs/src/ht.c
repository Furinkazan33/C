#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "ht.h"
#include "macros.h"



/* 	
	hash functions
*/

size_t hash_int16_to_idx(hashtable *ht, void *key) {
	return (* (int16_t *) key) % ht->partitions;
}

size_t hash_int32_to_idx(hashtable *ht, void *key) {
	return (* (int32_t *) key) % ht->partitions;
}

size_t hash_int64_to_idx(hashtable *ht, void *key) {
	return (* (int64_t *) key) % ht->partitions;
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
hashtable_entry *hashtable_entry_new(hashtable *t, void *key, void *data) {
	hashtable_entry *res;
	MALLOC(res, sizeof(*res), return NULL, )
	
	size_t size_of_key;
	
	switch(t->type) {
		case HT_INT16:
			size_of_key = sizeof(int16_t);
			break;
		case HT_INT32:
			size_of_key = sizeof(int32_t);
			break;
		case HT_INT64:
			size_of_key = sizeof(int64_t);
			break;	
		case HT_STR:
			size_of_key = sizeof(char) * (strlen(key) + 1);
			break;
		case HT_CHAR:
			size_of_key = sizeof(char);
			break;
	}
	
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

int64_t hashtable_entry_cmp_char(void *p, void *key) {
	return *(char *) p - *(char *) key;
}

int64_t hashtable_entry_cmp_int16(void *p, void *key) {
	return * (int16_t *) p - * (int16_t *) key;
}

int64_t hashtable_entry_cmp_int32(void *p, void *key) {
	return * (int32_t *) p - * (int32_t *) key;
}

int64_t hashtable_entry_cmp_int64(void *p, void *key) {
	return * (int64_t *) p - * (int64_t *) key;
}

int64_t hashtable_entry_cmp_string(void *p, void *key) {
	return (int64_t) strcmp((char *) p, (char *) key);
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
		case HT_INT16:
			res->cmp = hashtable_entry_cmp_int16;
			res->hash = hash_int16_to_idx;
			break;
		case HT_INT32:
			res->cmp = hashtable_entry_cmp_int32;
			res->hash = hash_int32_to_idx;
			break;
		case HT_INT64:
			res->cmp = hashtable_entry_cmp_int64;
			res->hash = hash_int64_to_idx;
			break;	
		case HT_STR:
			res->cmp = hashtable_entry_cmp_string;
			res->hash = hash_str_to_idx;
			break;
		case HT_CHAR:
			res->cmp = hashtable_entry_cmp_char;
			res->hash = hash_str_to_idx;
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
	if(!t->hash) {
		LOG_E("Missing hash function")
		return NULL;
	}
	
	// creating new entry
	hashtable_entry *entry = NULL;
	switch(t->type) {
		case HT_INT16:
			#ifdef HT_DEBUG
				LOG_S("as int, key=[%d]", *(int16_t *) key)
			#endif
			entry = hashtable_entry_new(t, key, data);
			break;
		case HT_INT32:
			#ifdef HT_DEBUG
				LOG_S("as int, key=[%d]", *(int32_t *) key)
			#endif
			entry = hashtable_entry_new(t, key, data);
			break;
		case HT_INT64:
			#ifdef HT_DEBUG
				LOG_S("as int, key=[%" PRId64 "]", *(int64_t *) key)
			#endif
			entry = hashtable_entry_new(t, key, data);
			break;
		case HT_STR:
			#ifdef HT_DEBUG
				LOG_S("as str, key=[%s]", (char *) key)
			#endif
			entry = hashtable_entry_new(t, key, data);
			break;
		case HT_CHAR:
			#ifdef HT_DEBUG
				LOG_S("as char, key=[%c]", *((char *) key))
			#endif
			entry = hashtable_entry_new(t, key, data);
			break;
	}
	
	// getting index partition
	unsigned int idx = t->hash(t, key);
	
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
	unsigned int idx = ht->hash(ht, key);
	return ht->entries[idx];
}

void **hashtable_find(hashtable *ht, void *key, bool find_all, size_t *n_found, size_t init_alloc_found) {
	if(!ht->cmp) {
		LOG_E("Missing compare function")
		return NULL;
	}
	
	int idx = ht->hash(ht, key);
	
	*n_found = 0;
	void **res;
	size_t res_alloc = init_alloc_found;
	MALLOC(res, sizeof(*res) * res_alloc, LOG_E_MESS("failed to alloc results\n"); return NULL;, )
	
	hashtable_entry *e = ht->entries[idx];
	
	while(e) {
		if(!ht->cmp(e->key, key)) {
			// realloc results
			if(*n_found == res_alloc) {
				#ifdef HT_DEBUG
					LOG_S("realloc [%ld] => [%ld]\n", res_alloc, res_alloc * 2);
				#endif
			
				res_alloc *= 2;
				REALLOC(res, sizeof(*res) * res_alloc, LOG_E_MESS("failed to realloc results\n"); return NULL;, )
			}
			
			res[(*n_found)++] = e->data;
			
			#ifdef HT_DEBUG
				LOG_S("found [%ld]\n", *n_found);
			#endif
			
			if(!find_all) {
				return res;
			}
		}
		e = e->next;
	}
	
	return res;
}

void hashtable_log_as_int(hashtable *ht, bool all) {
	LOG_S("[%ld] partitions\n", ht->partitions);
	
	for(size_t i = 0; i < ht->partitions; i++) {
		
		hashtable_entry *e = ht->entries[i];
		
		if(e) {
			LOG_S("[%ld]=>", i)
		}
		
		if(all) {
			hashtable_entry *p = e;
			
			while(p) {
				printf("[%" PRId64 "]", *(int64_t *) p->data);
				p = p->next;
			}
		}
		else {
			printf("[%" PRId64 "]", *(int64_t *) e->data);
			
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
	LOG_S("[%ld] partitions\n", ht->partitions);
	
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

