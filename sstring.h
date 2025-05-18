#pragma once
#include <stdint.h>

typedef struct DLL {
	uint8_t length;
	char* data;
} String;

DLL String* CreateString(char str[]);

DLL void DestroyString(String* str);