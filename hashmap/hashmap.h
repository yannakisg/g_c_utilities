#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <stdlib.h>

typedef struct _HEntry {
	struct _HEntry *next;
	void *key;
	void *data;
} HEntry;

typedef struct _HashMap {
	int size;
	int buckets;
	HEntry **head;
	void (*free_ptr)(void *data);
	unsigned int (*hash_ptr)(void *buf, size_t len);
	int (*key_cmp) (void *key1, void *key2);	
} HashMap;

int initHashMap(HashMap *map, unsigned int totalBuckets, void (*fptr)(void *data), unsigned int (*hptr)(void *buf, size_t len), int (*kcmp)(void *key1, void *key2));

int putHashMap(HashMap *map, void *key, size_t keyLen, void *data);

void* getHashMap(HashMap *map, void *key, size_t keyLen);

int deleteHashMap(HashMap *map, void *key, size_t keyLen);

int containsHashMap(HashMap *map, void *key, size_t keyLen);

void cleanupHashMap(HashMap *map);

#endif
