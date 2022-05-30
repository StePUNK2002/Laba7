#pragma once

#ifdef MATHLIBRARY_EXPORTS
#define TURN_API __declspec(dllexport)
#else
#define TURN_API __declspec(dllimport)
#endif

#include "windows.h"
class Turn
{
private:
	int size;
	HANDLE heaphandle;
	int* mass;
public:
	int buffer;
	TURN_API Turn();
	TURN_API Turn(int i);
	TURN_API Turn(const Turn&);
	TURN_API Turn ReadFromFile();
	TURN_API int EjectionElemet();
	TURN_API void PushToBuf(int value);
	TURN_API int GetFirstEl();
	TURN_API int GetSize();
	TURN_API void WriteToFile();
	TURN_API void Push(int value);
	TURN_API ~Turn();
	friend DWORD WINAPI FunctionWithThread(PVOID p);
};