#include "hash_table.h"
#include <stdlib.h>
#include "util.h"
#include "sstring.h"

void StartSparseArrayAsUnused(HashTable* table) {
	size_t pointerSize = table->indicesPointerSize;
	IDX_TYPE empty = IDX_UNUSED;
	for (int8_t i = 0; i < table->indicesLength; i += pointerSize) {
		int8_t* p = &(table->indices[i]);
		memcpy(p, &empty, pointerSize);
	}
}

HashTable* CreateHashTable(const int length, const float loadFactor, const long (*hashFunction) (String idx)) {
	int sizeIdx = GetSizeIdx(length);
	uint32_t indicesLength = length * 2;
	size_t pointerSize = SIZE[sizeIdx];
	
	HashTable* hashTable = malloc(sizeof(HashTable) + indicesLength * pointerSize);
	hashTable->indices = hashTable + 1;
	if (!hashTable)
		return NULL;
	//hashTable->indices = hashTable->indices + pointerSize;
	hashTable->removedEntriesIdx = NULL;


	//HashTable* hashTable = malloc(sizeof(HashTable) + (sizeof(Entry) * (length - 1)));
	
	hashTable->length = length;
	hashTable->curLength = 0;
	hashTable->loadFactor = loadFactor;
	hashTable->hashFunction = hashFunction;
	hashTable->indicesLength = indicesLength;

	hashTable->sizeIdx = sizeIdx;
	hashTable->indicesPointerSize = pointerSize;	
	//hashTable->indices = malloc(indicesLength * pointerSize);
	hashTable->entries = malloc(sizeof(Entry) * length);
	StartSparseArrayAsUnused(hashTable);

	return hashTable;
}

void DestroyHashTable(HashTable* table) {
	free(table);
}

inline int8_t* GetSparseIndexPointer(const HashTable* table, long sparseIdx) {
	return table->indices + sparseIdx * table->indicesPointerSize;
}

Entry* GetEntryFromSparseIndex(const HashTable* table, long sparseIndex) {
	int8_t* pSparseIndex = GetSparseIndexPointer(table, sparseIndex);
	int64_t entriesIdx = GetDenseIndexFromSparseIndexPointer(pSparseIndex, table->indicesPointerSize);
	return &(table->entries[entriesIdx]);
}
inline Entry* GetEntryFromDenseIndex(const HashTable* table, const long denseIndex) {
	return &(table->entries[denseIndex]);
}

void* GetValueFromSparseIndex(const HashTable* table, long sparseIdx) {
	Entry* entry = GetEntryFromSparseIndex(table, sparseIdx);
	return entry->value;
}


long GetSparseIndexFromKey(HashTable* table, const String key) {
	const long mask = table->indicesLength - 1;
	const long hash = table->hashFunction(key);

	long perturb = hash;
	long i = hash & mask;

	for (long j = 0; j < table->indicesLength; j++) {
		int8_t* p = GetSparseIndexPointer(table, i);
		long denseIdx = GetDenseIndexFromSparseIndexPointer(p, table->indicesPointerSize);

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
	return -1;
}


void* GetValueFromKey(const HashTable* table, String key) {
	long sparseIdx = GetSparseIndexFromKey(table, key);
	void* data = GetValueFromSparseIndex(table, sparseIdx);
	return data;
}


void UpdateHashTable(HashTable* table, String key, void* value) {
	long sparseIdx = GetSparseIndexFromKey(table, key);
	int8_t* p = GetSparseIndexPointer(table, sparseIdx);
	memcpy(p, &(table->curLength), table->indicesPointerSize);

	Entry* entry = &(table->entries[table->curLength]);
	entry->key = key;
	entry->value = value;
	table->curLength++;
}

