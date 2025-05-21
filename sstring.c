#include "sstring.h"
#include <string.h>
#include <stdlib.h>

String* CreateString(char str[]) {	
	size_t length = strlen(str);
	String* string = malloc(sizeof(String) + length + 1);
	string->data = string + 1;
	strcpy_s(string->data, length+1, str);
	return string;
}

DLL void DestroyString(String* str) {
	free(str);
}