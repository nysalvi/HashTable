#include "table.h"
#include <stdlib.h>
#include "util.h"
#include "sstring.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

Table* CreateTable(const uint64_t length, const float loadFactor, const long (*hashFunction) (String* idx)) {
	Table* table = malloc(sizeof(Table));
	if (!table)
		return NULL;

	uint64_t indicesLength = length * 2;
	table->indexManager = InitSparseArrayManager(indicesLength);
	if (!(table->indexManager))
		return NULL;
	table->entryManager = InitEntryManager(length);
	if (!(table->entryManager))
		return NULL;

	table->removedEntriesIdx = InitIntLinkedList((int64_t) {0}, 0);
	table->loadFactor = loadFactor;
	table->hashFunction = hashFunction;
		
	return table;
}

void DestroyTable(Table* table, uint8_t deleteKeys, uint8_t deleteData) {
	uint64_t length = table->entryManager->length;
	for (uint64_t i = 0; i < length; i++) {
		Entry* entry = table->entryManager->entries[i];
		if (deleteKeys)
			free(entry->key);
		if (deleteData)
			free(entry->value);
		free(entry);
	}	

	free(table->indexManager);
	free(table->entryManager);
	free(table);
}


Table* ResizeTable(Table* table) {
	uint64_t entriesUsed = table->entryManager->usedLength;
	uint64_t oldLength = table->entryManager->length;
	uint64_t newLength = oldLength * 2;

	float loadFactor = table->loadFactor;
	uint64_t(*hashFunction) (String* idx) = table->hashFunction;
	
	Entry** entries = table->entryManager->entries;
	DestroyTable(table, 0, 0);

	Table* newTable = CreateTable(newLength, loadFactor, hashFunction);

	for (int64_t i = 0; i < entriesUsed; i++) {
		Entry* entry = entries[i];
		if (entry == NULL || !entry->key->length)
			continue;

	}
	return newTable;
}


inline int8_t* GetSparseIndexPointer(const Table* table, uint64_t sparseIdx) {
	return table->indexManager->indices + sparseIdx * table->indexManager->pSize;
}

Entry* GetEntryFromSparseIndex(const Table* table, uint64_t sparseIndex) {
	int8_t* pSparseIndex = GetSparseIndexPointer(table, sparseIndex);
	int64_t entriesIdx = GetDenseIndexFromSparseIndexPointer(pSparseIndex, table->indexManager->pSize);
	return table->entryManager->entries[entriesIdx];
}
inline Entry* GetEntryFromDenseIndex(const Table* table, const uint64_t denseIndex) {
	return table->entryManager->entries[denseIndex];
}

void* GetValueFromSparseIndex(const Table* table, uint64_t sparseIdx) {
	Entry* entry = GetEntryFromSparseIndex(table, sparseIdx);
	return entry->value;
}

uint64_t GetSparseIndexFromKey(Table* table, const String* key) {
	const uint64_t mask = table->indexManager->length - 1;
	const uint64_t hash = table->hashFunction(key);

	uint64_t perturb = hash;
	uint64_t i = hash & mask;

	for (uint64_t j = 0; j < table->indexManager->length; j++) {
		int8_t* p = GetSparseIndexPointer(table, i);
		int64_t denseIdx = GetDenseIndexFromSparseIndexPointer(p, table->indexManager->pSize);

		if (denseIdx == IDX_UNUSED)
			return i;
		else {
			Entry* entry = GetEntryFromDenseIndex(table, denseIdx);
			if (!strcmp(entry->key->data, key->data))
				return i;
		}
		perturb >>= PERTURB_SHIFT;
		i = mask & (i * 5 + perturb + 1);
	}
	printf("error: list is full and key is not in the list;");
	exit(-1);	
}
/*
Index* GetIndex(Table* table, String* key) {
	uint64_t i = GetSparseIndexFromKey(table, key);
	int8_t* p = GetSparseIndexPointer(table, i);
	int64_t denseIdx = GetDenseIndexFromSparseIndexPointer(p, table->indexManager->pSize);

	Index* index = malloc(sizeof(Index));
	index->p = p;
	index->pSize = table->indexManager->pSize;
	index->denseIdx = denseIdx;
	return index;
}
*/
void* GetValueFromKey(const Table* table, const String* key) {
	uint64_t sparseIdx = GetSparseIndexFromKey(table, key);		
	void* data = GetValueFromSparseIndex(table, sparseIdx);
	return data;
}


void InsertEntry(Table* table, String* key, void* value) {
	uint64_t sparseIdx = GetSparseIndexFromKey(table, key);
	int8_t* p = GetSparseIndexPointer(table, sparseIdx);
	
	
	Entry* entry = malloc(sizeof(Entry));
	int64_t denseIdx;
	if (table->removedEntriesIdx && table->removedEntriesIdx->size) {
		denseIdx = PopIntNode(table->removedEntriesIdx);
		//entry = table->entryManager->entries[denseIdx];		
	}
	else {
		denseIdx = table->entryManager->usedLength;
		//entry = table->entryManager->entries[denseIdx];		
		table->entryManager->usedLength++;	
	}
	table->entryManager->entries[denseIdx] = entry;
	memcpy(p, &denseIdx, table->indexManager->pSize);	
	entry->insertCounter = table->entryManager->global_insertion_counter++;
	entry->key = key;
	entry->value = value;
}

void DeleteTableEntry(Table* table, String* key, uint8_t deleteData) {
	uint64_t sparseIdx = GetSparseIndexFromKey(table, key);
	int8_t* p = GetSparseIndexPointer(table, sparseIdx);
	int64_t denseIdx = GetDenseIndexFromSparseIndexPointer(p, table->indexManager->pSize);
	Entry* entry = table->entryManager->entries[denseIdx];
	if (deleteData)
		free(entry->value);
	free(entry);
	table->entryManager->entries[denseIdx] = NULL;
	
	PushIntNode(table->removedEntriesIdx, denseIdx);	
	IDX_TYPE unused = IDX_UNUSED;
	memcpy(p, &unused, table->indexManager->pSize);
}
