/*
	hashtable partitionnee qui contient des hashtable_entry (key, data).
	La partition est determinee a partir de la cle de type hashtable_type.
	A une meme key peut correspondre plusieurs hashtable_entry.	
	Le nombre de partitions est choisi lors de la creation de la ht.

	Lors de l'ajout dans la table, la key est copiee.
*/


#include <stdlib.h>
#include <stdbool.h>

typedef enum hashtable_type {
	HT_INT16 = 1,
	HT_INT32,
	HT_INT64,
	HT_STR,
	HT_CHAR,
} hashtable_type;

typedef struct hashtable_entry {
	void *key;
	void *data;
	
	// if non-unicity on key
	struct hashtable_entry *prec;
	struct hashtable_entry *next;
} hashtable_entry;

typedef struct hashtable {
	hashtable_entry **entries;
	size_t partitions;
	int64_t (*cmp)(void *, void *);
	size_t (*hash)(struct hashtable *t, void *key);
	hashtable_type type;
	char pad[4];
} hashtable;


hashtable *hashtable_new(hashtable_type type, size_t partitions);
void hashtable_free(hashtable *t);
hashtable *hashtable_add(hashtable *t, void *key, void *data);
void **hashtable_find(hashtable *t, void *key, size_t *n_found, size_t init_alloc_found);
void hashtable_delete(hashtable *t, void *key);
void hashtable_log_as_int(hashtable *t, bool all);
void hashtable_log_as_str(hashtable *t, bool all);
