#include "util.h"
#include "djbx33a.h"
#include "sstring.h"

uint64_t DJBX33A(String key) {
	int length = key.length;
	if (!length)
		return 0;
	long hash = 5381;
	const uint8_t* p = &(key.data);

	if (length <= DJBX33A_LENGTH) {
		for (int i = 0; i < length; i++) {
			hash = ((hash << 5) + hash) + *p++;
		}
	}

	hash ^= length;
	hash ^= DJBX33A_SUFFIX;
	return hash;
}