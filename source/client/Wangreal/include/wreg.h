// Original: d:\build\custom\temp\code\client\wangreal\include\wreg.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WREG_H
#define PG_CLIENT_WANGREAL_INCLUDE_WREG_H

#include <windows.h>

#include "..\..\defines.h"

class WRegistry {
	LAYOUT_CHECK_FRIEND;

public:
	WRegistry();
	~WRegistry();

	void SetLocation(const char* location);
	void PassCode(int code);
	bool __cdecl Read(char* name, char* buffer, int size);
	int __cdecl Scan(char* name, char* format, ...);
	void Write(char* name, unsigned long value);
	void __cdecl Write(char* name, const char* format, ...);

private:
	HKEY__* m_root;             // 0x0
	char m_location[128];       // 0x4
	int m_code;                 // 0x84
};
CHECK_SIZE(WRegistry, 0x88);
BEGIN_LAYOUT_CHECK(WRegistry)
	CHECK_FIELD(WRegistry, m_root, 0x0);
	CHECK_FIELD(WRegistry, m_location, 0x4);
	CHECK_FIELD(WRegistry, m_code, 0x84);
END_LAYOUT_CHECK

#endif
