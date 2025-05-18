#pragma once
#include <stdint.h>

typedef struct DLL {
	size_t pSize;
	int64_t length;
	uint8_t* indices;
} SparseArrayManager;