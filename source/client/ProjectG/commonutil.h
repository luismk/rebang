#pragma once

char* __cdecl MakeStr(const char* format, ...);
unsigned long htoi(const char* str);

template <class T>
void sequence_delete(T first, T last)
{
	while (first != last)
		delete *first++;
}
