#pragma once
#include "table.h"

typedef struct {
	void** data;
	uint64_t start;
	uint64_t end;
	int (*ToInt) (void*);
} QuickSortArgs;

// use a pointer of QuickSortArgs
unsigned __stdcall QuickSort(void* args);

