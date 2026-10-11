// Original: d:\build\custom\temp\code\client\projectg\slicestr.h
#ifndef PG_CLIENT_PROJECTG_SLICESTR_H
#define PG_CLIENT_PROJECTG_SLICESTR_H

#include "..\..\defines.h"

// Reads "*key value" options out of a name string (scanf-style).
class WSliceStr {
	LAYOUT_CHECK_FRIEND;
public:
	// slicestr.h:8
	WSliceStr(const char* str)
	{
		m_str = str;
	}

	bool __cdecl Get(const char* key, const char* format, ...);

private:
	const char* m_str;   // 0x0
};
CHECK_SIZE(WSliceStr, 0x4);

BEGIN_LAYOUT_CHECK(WSliceStr)
	CHECK_FIELD(WSliceStr, m_str, 0x0);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SLICESTR_H
