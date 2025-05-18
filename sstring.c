#include "sstring.h"
#include <string.h>
#include <stdlib.h>

String* CreateString(char str[]) {
	size_t length = strlen(str);
	String* string = malloc(sizeof(String) + (length - 1));
	string->length = length;
	return string;
}

DLL void DestroyString(String* str) {
	free(str);
}