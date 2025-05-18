#pragma once
#include "util.h"
#include "sstring.h"

#ifndef DJBX33A_SUFFIX
#define DJBX33A_SUFFIX 0x4E2A3373C
#endif

#ifndef DJBX33A_LENGTH 
#define DJBX33A_LENGTH 10
#endif 

long DJBX33A(String key);