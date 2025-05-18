#pragma once
#include <stdint.h>
#include <stdlib.h>

extern size_t SIZE[];

extern int64_t SIZE_MASK_AND[];

extern int64_t SIZE_MASK_OR[];

typedef enum DLL {
	SUCCESS = 0,
	DUPLICATE_KEY = 1,
	ERROR = 2
} MESSAGE;

typedef enum DLL {
	IDX_UNUSED = -1,
	IDX_DUMMY = -2,
	IDX_PENDING = -3
} IDX_TYPE;

uint64_t nextPowerOf2(uint64_t n);

int64_t GetDenseIndexFromSparseIndexPointer(int8_t* p, size_t size);

int GetSizeIdx(uint64_t length);
