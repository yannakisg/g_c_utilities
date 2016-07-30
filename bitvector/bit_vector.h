#ifndef _BIT_VECTOR_H_
#define _BIT_VECTOR_H_

typedef struct _BitVector {
	unsigned int size; //in Bits
	unsigned int buffer_size;
	char *buffer;
}BitVector;

BitVector* create_bit_vector(unsigned int size_in_bits);

int getBit(BitVector *bVector, unsigned int pos);

void setBit(BitVector *bVector, unsigned int pos);

void delBit(BitVector *bVector, unsigned int pos);

void cleanupBitVector(BitVector **bVector);

#endif
