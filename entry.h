#pragma once
#include "util.h"
#include "sstring.h"

typedef struct DLL {
	uint32_t insertCounter;
	String key;
	void* value;
} Entry;

typedef struct DLL {
	uint32_t global_insertion_counter;
	uint32_t length;
	Entry* entries;

} EntryManager;