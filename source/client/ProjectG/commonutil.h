#pragma once

char* __cdecl MakeStr(const char* format, ...);

template <class T>
void sequence_delete(T first, T last)
{
	while (first != last)
		delete *first++;
}
