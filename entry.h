#pragma once
#include "util.h"
#include "sstring.h"

typedef struct DLL {
	uint32_t insertCounter;
	String* key;
	void* value;
} Entry;

typedef struct DLL {
	uint32_t global_insertion_counter;
	uint32_t length;
	uint32_t usedLength;
	Entry** entries;
} EntryManager;

int EntryToInt(void* entry);
EntryManager* InitEntryManager(const int length);