#ifndef _BLOOMFILTER_H_
#define _BLOOMFILTER_H_

#include "../bitvector/bit_vector.h"

typedef struct _BloomFilter {
	unsigned int k;
	BitVector *bV;
}BloomFilter;

BloomFilter* createBloomFilter(unsigned int k, unsigned int size);

void addInBloomFilter(BloomFilter *bL, int value);

int containsBloomFilter(BloomFilter *bL, int value);

void cleanupBloomFilter(BloomFilter **bL);

#endif
