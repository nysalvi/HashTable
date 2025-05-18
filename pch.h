#pragma once
#ifdef _MYLIB
	#define DLL __declspec(dllexport)
#else
	#define DLL __declspec(dllimport)
#endif