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
	uint64_t(*hashFunction) (String);

} Table;


Table* CreateTable(const int length, const float loadFactor, const uint64_t(*hashFunction) (String idx));
void InsertEntry(Table* table, String key, void* value);
void DeleteEntryTable(Table* table, String key);
void DestroyTable(Table* table);
void* GetValueFromKey(const Table* Table, const String key);
Table* ResizeTable(Table* table);
