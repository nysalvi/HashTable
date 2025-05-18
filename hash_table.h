#pragma once
#include "index.h"
#include "stdint.h"
#include "util.h"
#include "entry.h"
#include "sstring.h"
#include "linked_list.h"

/*
typedef struct DLL{
	void* data;
} Object;

typedef struct DLL {
	long usable;
	long numEntries;
	char indices[];
} HashKeys;

typedef struct DLL {
	uint8_t capacity;
	uint8_t size;
	uint8_t embedded;
	uint8_t valid;
	Object* values[1];
} HashValues;
*/

#ifndef PERTURB_SHIFT 
#define PERTURB_SHIFT 5
#endif 

typedef struct DLL {
	IntLinkedList* removedEntriesIdx;
	int64_t length;
	int64_t curLength;
	float loadFactor;
	int sizeIdx;
	size_t indicesPointerSize;
	int64_t indicesLength;
	long (*hashFunction) (String);
	int8_t* indices;
	Entry entries[];
} HashTable;


HashTable* CreateHashTable(const int length, const float loadFactor, const long (*hashFunction) (String idx));
void UpdateHashTable(HashTable* table, String key, void* value);
void DestroyHashTable(HashTable* table);
void* GetValueFromKey(const HashTable* hashTable, String key);

