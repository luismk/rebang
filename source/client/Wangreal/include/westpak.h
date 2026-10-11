// Original: d:\build\custom\temp\code\client\wangreal\include\westpak.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WESTPAK_H
#define PG_CLIENT_WANGREAL_INCLUDE_WESTPAK_H

#include "..\..\defines.h"

// Abstract file: the pak readers, the memory copy and the plain files.
class cFile {
	LAYOUT_CHECK_FRIEND;
public:
	cFile() { m_nLen = 0; }
	virtual ~cFile() {}
	virtual bool Open(const char* filename) { return false; }
	virtual int Read(void* buffer, int size) = 0;
	virtual int GetByte() = 0;
	virtual int Tell() = 0;
	virtual void Seek(int offset, int origin) = 0;

	int Length() {
		return m_nLen;
	}
	bool __cdecl Scan(const char* format, ...);

protected:
	int m_nLen; // 0x4
};
CHECK_SIZE(cFile, 0x8);
BEGIN_LAYOUT_CHECK(cFile)
	CHECK_FIELD(cFile, m_nLen, 0x4);
END_LAYOUT_CHECK

cFile* __cdecl GetCFile(const char* filename, int mode, int unknown);
void __cdecl CloseCFile(cFile* file);

#endif // PG_CLIENT_WANGREAL_INCLUDE_WESTPAK_H
