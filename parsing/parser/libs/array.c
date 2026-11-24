#include <string.h>
#include <unistd.h>
#include "array.h"
#include "macros.h"

// capacity + 1 => ARRAY_GET(a, a->count) is used as to test end of array
array *array_new(size_t capacity, size_t item_size) {
	array *res;
	MALLOC(res, sizeof(*res), return NULL, {
		MALLOC(res->items, item_size * capacity, free(res); return NULL, {
			res->item_size = item_size;
			res->count = 0;
			res->capacity = capacity;
			return res;
		});
	});
}

array *array_dump_infos(array *a, FILE *fd) {
	fwrite(&(a->item_size), sizeof(size_t), 1, fd);
	fwrite(&(a->capacity), sizeof(size_t), 1, fd);
	fwrite(&(a->count), sizeof(size_t), 1, fd);
	return a;
}

array *array_dump_items(array *a, FILE *fd, void *(*item_dump)(FILE *, void *)) {
	if(item_dump != NULL) {
		ARRAY_DO(a, idx, void *, item, item_dump(fd, item))
	}
	else {
		fwrite(a->items, a->item_size, a->count, fd);
	}
	return a;
}

array *array_dump(array *a, FILE *fd, void *(*item_dump)(FILE *, void *)) {
	array_dump_infos(a, fd);
	
	array_dump_items(a, fd, item_dump);
	
	return a;
}

array *array_load(FILE *fd, void (*item_load)(void *, FILE *)) {
	size_t item_size;
	size_t capacity;
	size_t count;
	
	fread(&item_size, sizeof(size_t), 1, fd);
	fread(&capacity, sizeof(size_t), 1, fd);
	fread(&count, sizeof(size_t), 1, fd);

	array *res = array_new(capacity, item_size); if(!res) { LOG_E("error creating array"); return NULL; }
	res->count = count;
	
	if(item_load != NULL) {
		ARRAY_DO(res, idx, void *, item, item_load(item, fd))
	}
	else {
		fread(res->items, res->item_size * res->count, 1, fd);
	}
	
	#ifdef DEBUG_ARRAY
		LOG_S("item_size=[%ld], capacity=[%ld], count=[%ld]\n", res->item_size, res->capacity, res->count)
	#endif

	return res;
}

void array_free(array *a) {
	free(a->items);
	free(a);
}

// capacity + 1 => ARRAY_GET(a, a->count) is used as to test end of array
array *array_add(array *a, void *item) {
	if(a->count == a->capacity) {
		REALLOC(a->items, a->item_size * (a->capacity + 1) * ARRAY_REALLOC_COEF, return NULL, {
			a->capacity *= ARRAY_REALLOC_COEF;
		});
	}

	memcpy(ARRAY_GET(a, a->count, void *), item, a->item_size);
	a->count++;

	return a;
}

array *array_del(array *a, void *data) {
	size_t idx = ((char *) data - a->items) / a->item_size;
	return array_del_idx(a, idx);
}

array *array_del_idx(array *a, size_t idx) {
	for(size_t i = a->count - 1; i > idx; i--) {
		memcpy(ARRAY_GET(a, i - 1, void *), ARRAY_GET(a, i, void *), a->item_size);
	}
	a->count--;
	return a;
}

int array_find_idx(array *a, void *search, int (*cmp)(const void *, const void *)) {
	FOR(i, 0, a->count, if(!cmp(ARRAY_GET(a, i, void *), search)) { return i; });
	return -1;
}

void *array_find(array *a, void *search, int (*cmp)(const void *, const void *)) {
	FOR(i, 0, a->count, if(!cmp(ARRAY_GET(a, i, void *), search)) { return ARRAY_GET(a, i, void *); });
	return NULL;
}

int array_find_rec_idx(array *a, void *search, int (*cmp)(const void *, const void *), size_t *idx) {
	for(; *idx < a->count; (*idx)++) {
		#ifdef DEBUG
			LOG_S("*idx=%ld, count=%ld\n", *idx, a->count);
		#endif
		if(!cmp(ARRAY_GET(a, *idx, void *), search)) {
			#ifdef DEBUG
				LOG_S("found, *idx=%ld\n", *idx);
			#endif
			
			return (*idx)++;
		}
	}
	return -1;
}

void *array_find_rec(array *a, void *search, int (*cmp)(const void *, const void *), size_t *idx) {
	for(; *idx < a->count; (*idx)++) {
		#ifdef DEBUG
			LOG_S("*idx=%ld, count=%ld\n", *idx, a->count);
		#endif
		if(!cmp(ARRAY_GET(a, *idx, void *), search)) {
			#ifdef DEBUG
				LOG_S("found, *idx=%ld\n", *idx);
			#endif
			
			return ARRAY_GET(a, (*idx)++, void *);
		}
	}
	return NULL;
}

array *array_find_all_idx(array *a, void *search, int (*cmp)(const void *, const void *)) {
	array *res = array_new(ARRAY_INIT_ALLOC, sizeof(int));
	int found;
	
	if(!res) {
		LOG_E("array_new returned NULL");
		return NULL;
	}

	for(size_t i = 0; (found = array_find_rec_idx(a, search, cmp, &i)); ) {
		if(!array_add(res, &found)) {
			LOG_E("failed to append found\n");
			ARRAY_FREE(res);
			return NULL;
		}
	}

	return res;
}

array *array_find_all(array *a, void *search, int (*cmp)(const void *, const void *)) {
	array *res = array_new(ARRAY_INIT_ALLOC, a->item_size);
	void *found = NULL;
	
	if(!res) {
		LOG_E("array_new returned NULL");
		return NULL;
	}

	for(size_t i = 0; (found = array_find_rec(a, search, cmp, &i)); ) {
		if(!array_add(res, found)) {
			LOG_E("failed to append found\n");
			ARRAY_FREE(res);
			return NULL;
		}
	}

	return res;
}
