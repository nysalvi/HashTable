#pragma once
#include "index.h"
#include "stdint.h"
#include "util.h"
#include "entry.h"
#include "sstring.h"
#include "linked_list.h"

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
	uint64_t (*hashFunction) (String);
	Entry* entries;
	int8_t* indices;
} HashTable;


HashTable* CreateHashTable(const int length, const float loadFactor, const uint64_t (*hashFunction) (String idx));
void UpdateHashTable(HashTable* table, String key, void* value);
void DeleteEntryHashTable(HashTable* table, String key);
void DestroyHashTable(HashTable* table);
void* GetValueFromKey(const HashTable* hashTable, String key);
HashTable* ResizeHashTable(HashTable* table);
