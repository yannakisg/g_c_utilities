#include "hashmap.h"

#include <stdlib.h>
#include <string.h>

int initHashMap(HashMap *map, unsigned int totalBuckets, void (*fptr)(void *data), unsigned int (*hptr)(void *buf, size_t len), int (*kcmp)(void *key1, void *key2)) {
	if (map == NULL || totalBuckets == 0 || hptr == NULL || kcmp == NULL) {
		return -1;
	}
	
	map->size = 0;
	map->buckets = totalBuckets;
	map->head = (HEntry **) malloc(sizeof(HEntry *) * totalBuckets);
	if (map->head == NULL) {
		return -1;
	}
	int i;
	for (i = 0; i < totalBuckets; i++) {
		map->head[i] = NULL;
	}
	
	map->free_ptr = fptr;
	map->hash_ptr = hptr;
	map->key_cmp = kcmp;
	
	return 1;
}

int putHashMap(HashMap *map, void *key, size_t keyLen, void *data) {
	if (map == NULL || key == NULL || data == NULL || keyLen <= 0) {
		return -1;
	}
	
	unsigned int pos = map->hash_ptr(key, keyLen) % map->buckets;
	HEntry *iter = map->head[pos];
	while (iter != NULL) {
		if (map->key_cmp(iter->key, key) == 0) {
			iter->data = data;
			return 1;
		}
		iter = iter->next;
	}
	HEntry *entry = (HEntry *) malloc(sizeof(HEntry));
	if (entry == NULL) {
		return -1;
	}
	
	entry->data = data;
	entry->key = key;
	entry->next = map->head[pos];
	
	map->head[pos] = entry;	
	map->size += 1;
	
	return 1;
}

void* getHashMap(HashMap *map, void *key, size_t keyLen) {
	if (map == NULL || key == NULL || keyLen <= 0) {
		return NULL;
	}
	
	unsigned int pos = map->hash_ptr(key, keyLen) % map->buckets;
	HEntry *iter = map->head[pos];
	while (iter != NULL) {
		if (map->key_cmp(iter->key, key) == 0) {			
			return iter->data;
		}
		iter = iter->next;
	}
	
	return NULL;
}

int deleteHashMap(HashMap *map, void *key, size_t keyLen) {
	if (map == NULL || key == NULL || keyLen <= 0) {
		return -1;
	}
	
	unsigned int pos = map->hash_ptr(key, keyLen) % map->buckets;
	HEntry *iter = map->head[pos];
	HEntry *prev = NULL;
	while (iter != NULL) {
		if (map->key_cmp(iter->key, key) == 0) {
			if (map->free_ptr != NULL) {
				map->free_ptr(iter->data);
			}
			if (prev != NULL) {
				prev->next = iter->next;
			} else {
				map->head[pos] = iter->next;
			}
			
			free(iter);
			map->size -= 1;
			return 1;
		}
		prev = iter;
		iter = iter->next;
	}
	
	return -1;
}

int containsHashMap(HashMap *map, void *key, size_t keyLen) {
	if (map == NULL || key == NULL || keyLen <= 0) {
		return -1;
	}
	
	unsigned int pos = map->hash_ptr(key, keyLen) % map->buckets;
	HEntry *iter = map->head[pos];
	while (iter != NULL) {
		if (map->key_cmp(iter->key, key) == 0) {
			return 1;
		}
		iter = iter->next;
	}
	
	return -1;
}

void cleanupHashMap(HashMap *map) {
	if (map == NULL) {
		return;
	}
	
	int i;
	for (i = 0; i < map->buckets; i++) {
		HEntry *iter = map->head[i];
		while (iter != NULL) {
			HEntry *t = iter;			
			iter = t->next;
			
			if (map->free_ptr != NULL) {
				map->free_ptr(t->data);
			}
			free(t);
		}
	}
	
	free(map->head);
}
