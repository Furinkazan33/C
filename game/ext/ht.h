/*
	hashtable qui fait correspondre une key int* positive ou char* a une donnee.
	La table fonctionne par partitions.
	Le nombre de partitions est choisi lors de la creation de la ht.
*/


#include <stdlib.h>
#include <stdbool.h>


typedef struct hashtable_entry {
	void *key;
	void *data;
	struct hashtable_entry *next; // if multiple results
} hashtable_entry;

hashtable_entry *hashtable_entry_new(void *key, size_t size_of_key, void *data);


typedef enum hashtable_type {
	HT_INT = 1,
	HT_STR,
	HT_CHAR,
} hashtable_type;

typedef struct hashtable {
	hashtable_entry **entries;
	size_t partitions;
	int (*cmp)(void *, void *);
	size_t (*hash_key_to_idx)(struct hashtable *ht, void *key);
	hashtable_type type;
	char pad[4];
} hashtable;

hashtable *hashtable_new(hashtable_type type, size_t partitions);
void hashtable_free(hashtable *ht);
hashtable *hashtable_add(hashtable *ht, void *key, void *data);
void *hashtable_find(hashtable *ht, void *value);
void hashtable_log_as_int(hashtable *ht, bool all);
void hashtable_log_as_str(hashtable *ht, bool all);
