#include <stdint.h>
#include "util.h"

size_t SIZE[] = { sizeof(int8_t), sizeof(int16_t), sizeof(int32_t), sizeof(int64_t) };

int64_t GetDenseIndexFromSparseIndexPointer(int8_t* p, size_t size) {
	if (size == SIZE[0])
		return *p;
	if (size == SIZE[1])
		return *((int16_t*)p);
	if (size == SIZE[2])
		return *((int32_t*)p);
	return *((int64_t*)p);
}


uint64_t nextPowerOf2(uint64_t n) {
	if (n == 0)
		return 1;
	n--;

	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n |= n >> 32;

	return n + 1;
}

int GetSizeIdx(uint64_t length) {
	int sizeIdx = 0;

	sizeIdx = length > 127 ? 1 : sizeIdx; // bigger than signed byte
	sizeIdx = length > 32767 ? 2 : sizeIdx; // bigger than 2^16 signed
	sizeIdx = length > 2147483647 ? 3 : sizeIdx; // bigger than 2^32 signed

	return sizeIdx;
}
