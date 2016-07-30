#include "bloomfilter.h"

#include <string.h>
#include <stdlib.h>

#define FNV_32_PRIME ((unsigned int)0x01000193)

unsigned int hash0(void *buf, size_t len) {
    unsigned char *bp = (unsigned char *)buf;	/* start of buffer */
    unsigned char *be = bp + len;		/* beyond end of buffer */
	unsigned int hval = 0;
    /*
     * FNV-1a hash each octet in the buffer
     */
    while (bp < be) {
		hval ^= (unsigned int)*bp++;
		hval *= FNV_32_PRIME;	
    }

    /* return our new hash value */
    return hval;
}

unsigned int hash1(void *buf, size_t len) {
	unsigned int h0 = hash0(buf, len);
    return hash0(&h0, sizeof(unsigned int));
}

unsigned int hashI(void *buf, size_t len, int i) {
	return hash0(buf, len) + i * hash1(buf, len);
}

BloomFilter* createBloomFilter(unsigned int k, unsigned int size) {
	if (k == 0 || size == 0) {
		return NULL;
	}
	
	BloomFilter* bloomFilter = (BloomFilter *) malloc(sizeof(BloomFilter));
	if (bloomFilter == NULL) {
		return NULL;
	}
	
	bloomFilter->bV = create_bit_vector(size);
	if (bloomFilter->bV == NULL) {
		free(bloomFilter);
		return NULL;
	}
	
	bloomFilter->k = k;
	
	return bloomFilter;
}

void addInBloomFilter(BloomFilter *bL, int value) {
	if (bL == NULL) {
		return;
	}
	
	unsigned int hVal;
	for (int i = 0; i < bL->k; i++) {
		if (i == 0) {
			hVal = hash0(&value, sizeof(int));
		} else if (i == 1) {
			hVal = hash1(&value, sizeof(int));
		} else {
			hVal = hashI(&value, sizeof(int), i);
		}
		
		setBit(bL->bV, hVal % bL->bV->size);
	}
}

int containsBloomFilter(BloomFilter *bL, int value) {
	if (bL == NULL) {
		return -1;
	}
	
	unsigned int hVal;
	for (int i = 0; i < bL->k; i++) {
		if (i == 0) {
			hVal = hash0(&value, sizeof(int));
		} else if (i == 1) {
			hVal = hash1(&value, sizeof(int));
		} else {
			hVal = hashI(&value, sizeof(int), i);
		}
		if (getBit(bL->bV, hVal % bL->bV->size) == 0) {
			return 0;
		}
	}
	
	return 1;
	
}

void cleanupBloomFilter(BloomFilter **bL) {
	if (bL == NULL) {
		return;
	}
	
	BloomFilter *instance = *bL;
	if (instance == NULL) {
		return;
	}
	
	cleanupBitVector(&(instance->bV));
	
	free(instance);
	*bL = NULL;
}
