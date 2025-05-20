#pragma once
#include "table.h"

typedef struct {
	void** data;
	int start;
	int end;
	int (*ToInt) (void*);
} QuickSortArgs;

// use a pointer of QuickSortArgs
unsigned __stdcall QuickSort(void* args);

