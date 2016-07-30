#include "bit_vector.h"

#include <string.h>
#include <stdlib.h>

BitVector* create_bit_vector(unsigned int size_in_bits) {
	BitVector *bitVector = (BitVector *) malloc(sizeof(BitVector));
	if (bitVector == NULL) {
		return NULL;
	}
	
	bitVector->buffer = NULL;
	bitVector->size = size_in_bits;
	bitVector->buffer_size = (size_in_bits % 8 == 0) ? size_in_bits >> 3 : (size_in_bits >> 3)+ 1;
	
	if (size_in_bits > 0) {
		bitVector->buffer = malloc(sizeof(char) * bitVector->buffer_size);
		if (bitVector->buffer == NULL) {
			free(bitVector);
			return NULL;
		}
	}
	
	return bitVector;
}

char _setBit(char ch, unsigned int pos) {
	return (ch | (1 << pos));
}

char _delBit(char ch, unsigned int pos) {
	
	return ch & (~(1 << pos));
}

int _getBit(char ch, unsigned int pos) {
	return (ch >> pos) & 1;
}

int getBit(BitVector *bVector, unsigned int pos) {
	if (bVector == NULL || pos >= bVector->size) {
		return 0;
	}
	
	unsigned int bufferPos = pos >> 3;
	unsigned int bitPos = pos % 8;
	
	return _getBit(bVector->buffer[bufferPos], bitPos);
}

void setBit(BitVector *bVector, unsigned int pos) {
	if (bVector == NULL || pos >= bVector->size) {
		return;
	}
	
	unsigned int bufferPos = pos >> 3;
	unsigned int bitPos = pos % 8;
	
	bVector->buffer[bufferPos] = _setBit(bVector->buffer[bufferPos], bitPos);
}

void delBit(BitVector *bVector, unsigned int pos) {
	if (bVector == NULL || pos >= bVector->size) {
		return;
	}
	
	unsigned int bufferPos = pos >> 3;
	unsigned int bitPos = pos % 8;
	bVector->buffer[bufferPos] = _delBit(bVector->buffer[bufferPos], bitPos);
}

void cleanupBitVector(BitVector **bVector) {
	if (bVector == NULL) {
		return;
	}
	
	BitVector *instance = *bVector;
	if (instance == NULL) {
		return;
	} else {
		if (instance->buffer != NULL) {
			free(instance->buffer);
		}
		
		free(instance);
		*bVector = NULL;
	}
}
