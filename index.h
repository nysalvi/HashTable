#pragma once
#include <stdint.h>

typedef struct DLL {
	int64_t length;
	size_t pSize;
	uint8_t* indices;
} SparseArrayManager;