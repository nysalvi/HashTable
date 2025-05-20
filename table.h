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
	SparseArrayManager* indexManager;
	EntryManager* entryManager;	
	float loadFactor;
	uint64_t(*hashFunction) (String*);

} Table;


Table* CreateTable(const int length, const float loadFactor, const uint64_t(*hashFunction) (String* idx));
void InsertEntry(Table* table, String* key, void* value);
void DeleteTableEntry(Table* table, String* key, uint8_t deleteValue);
void DestroyTable(Table* table, uint8_t deleteKeys, uint8_t deleteData);
void* GetValueFromKey(const Table* Table, const String* key);
Table* ResizeTable(Table* table);
