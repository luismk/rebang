#pragma once

#include <string>

class cFile;
class WResourceManager;

char* __cdecl MakeStr(const char* format, ...);
unsigned long htoi(const char* str);
bool str_Replace(std::string& str, const std::string& from,
	const std::string& to);

template <class T>
void sequence_delete(T first, T last)
{
	while (first != last)
		delete *first++;
}

class WFile
{
public:
	WFile();
	WFile(const char* filename);
	virtual ~WFile();

	cFile* Open(const char* filename, unsigned int option);
	virtual void Close();
	virtual int Read(void* buffer, int size);
	virtual int GetByte();
	virtual int Tell();
	virtual void Seek(int offset, int origin);
	virtual int Length();

	static void SetResourceManager(WResourceManager* pResMng);

protected:
	const char* m_filename;
	cFile* m_pFile;

	static WResourceManager* m_pResMng;
};
