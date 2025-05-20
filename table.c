#include "table.h"
#include <stdlib.h>
#include "util.h"
#include "sstring.h"
#include <string.h>
#include <stdio.h>

void StartSparseArrayAsUnused(Table* table) {
	size_t pointerSize = table->indexManager->pSize;
	IDX_TYPE empty = IDX_UNUSED;
	for (uint64_t i = 0; i < table->indexManager->length; i += pointerSize) {
		int8_t* p = &(table->indexManager->indices[i]);
		memcpy(p, &empty, pointerSize);
	}
}
void StartDenseArrayAsEmpty(Table* table) {
	for (uint64_t i = 0; i < table->entryManager->length; i++) {
		table->entryManager->entries[i].key.length = 0;
	}
}

Table* CreateTable(const int length, const float loadFactor, const long (*hashFunction) (String idx)) {
	int sizeIdx = GetSizeIdx(length);	
	uint64_t indicesLength = length * 2;
	size_t pointerSize = SIZE_POINTER[sizeIdx];

	Table* table = malloc(sizeof(Table));
	table->indexManager = malloc(sizeof(SparseArrayManager) + pointerSize * indicesLength);
	table->indexManager->indices = table->indexManager + 1;
	table->indexManager->length = indicesLength;

	if (!table)
		return NULL;
	//Table->indices = Table->indices + pointerSize;
	table->removedEntriesIdx = NULL;


	//Table* Table = malloc(sizeof(Table) + (sizeof(Entry) * (length - 1)));
	table->entryManager = malloc(sizeof(EntryManager) + sizeof(Entry) * length);

	table->entryManager->length = length;
	table->entryManager->usedLength = 0;
	table->loadFactor = loadFactor;
	table->hashFunction = hashFunction;
	table->indexManager->length = indicesLength;
	
	table->indexManager->pSize = pointerSize;	
	table->entryManager->entries = malloc(sizeof(Entry) * length);
	StartSparseArrayAsUnused(table);
	StartDenseArrayAsEmpty(table);
	return table;
}

void DestroyTable(Table* table, uint8_t removeEntries) {
	if (removeEntries) {
		uint64_t length = table->entryManager->length;
		for (uint64_t i = 0; i < length; i++) {
			free(table->entryManager->entries[i].value);
		}
	}

	free(table->entryManager);
	free(table->indexManager);
	free(table);
}


Table* ResizeTable(Table* table) {
	uint64_t entriesUsed = table->entryManager->usedLength;
	uint64_t oldLength = table->entryManager->length;
	uint64_t newLength = oldLength * 2;

	float loadFactor = table->loadFactor;
	uint64_t(*hashFunction) (String idx) = table->hashFunction;
	
	Entry* entries = table->entryManager->entries;
	DestroyTable(table, 0);

	Table* newTable = CreateTable(newLength, loadFactor, hashFunction);

	for (int64_t i = 0; i < entriesUsed; i++) {
		Entry entry = entries[i];
		if (!entry.key.length)
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
	return &(table->entryManager->entries[entriesIdx]);
}
inline Entry* GetEntryFromDenseIndex(const Table* table, const uint64_t denseIndex) {
	return &(table->entryManager->entries[denseIndex]);
}

void* GetValueFromSparseIndex(const Table* table, uint64_t sparseIdx) {
	Entry* entry = GetEntryFromSparseIndex(table, sparseIdx);
	return entry->value;
}

uint64_t GetSparseIndexFromKey(Table* table, const String key) {
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
			if (!strcmp(entry->key.data, key.data))
				return i;
		}
		perturb >>= PERTURB_SHIFT;
		i = mask & (i * 5 + perturb + 1);
	}
	printf("error: list is full and key is not in the list;");
	exit(-1);	
}


void* GetValueFromKey(const Table* table, const String key) {
	int64_t sparseIdx = GetSparseIndexFromKey(table, key);		
	void* data = GetValueFromSparseIndex(table, sparseIdx);
	return data;
}


void InsertEntry(Table* table, String key, void* value) {
	uint64_t sparseIdx = GetSparseIndexFromKey(table, key);
	int8_t* p = GetSparseIndexPointer(table, sparseIdx);
	
	
	Entry* entry;
	uint64_t denseIdx;
	if (table->removedEntriesIdx && table->removedEntriesIdx->size) {
		denseIdx = PopIntNode(table->removedEntriesIdx);
		entry = &(table->entryManager->entries[denseIdx]);
	}
	else {
		denseIdx = table->entryManager->usedLength;
		entry = &(table->entryManager->entries[denseIdx]);
		table->entryManager->usedLength++;	
	}

	memcpy(p, &denseIdx, table->indexManager->pSize);	
	entry->insertCounter = table->entryManager->global_insertion_counter++;
	entry->key = key;
	entry->value = value;
}

void DeleteEntryTable(Table* table, String key, uint8_t deleteData) {
	uint64_t sparseIdx = GetSparseIndexFromKey(table, key);
	int8_t* p = GetSparseIndexPointer(table, sparseIdx);
	uint64_t denseIdx = GetDenseIndexFromSparseIndexPointer(p, table->indexManager->pSize);

	Entry* entry = table->entryManager->entries + denseIdx;
	if (deleteData)
		free(entry);
}
