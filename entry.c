#include <stdint.h>
#include <stdlib.h>
#include "entry.h"

void StartDenseArrayAsEmpty(EntryManager* manager) {
	for (uint64_t i = 0; i < manager->length; i++) {
		manager->entries[i] = NULL;
	}
}

int EntryToInt(void* entry) {
	return ((Entry*)entry)->insertCounter;
}

EntryManager* InitEntryManager(const uint64_t length) {
	EntryManager* entryManager = malloc(sizeof(EntryManager) + sizeof(Entry*) * length);
	entryManager->entries = entryManager + 1;
	//table->entryManager->entries = malloc(sizeof(Entry) * length);
	entryManager->length = length;
	entryManager->usedLength = 0;
	entryManager->global_insertion_counter = 0;
	StartDenseArrayAsEmpty(entryManager);
	return entryManager;
}