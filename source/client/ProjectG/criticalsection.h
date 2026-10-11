// Original: d:\build\custom\temp\code\client\projectg\criticalsection.h
#ifndef PG_CLIENT_PROJECTG_CRITICALSECTION_H
#define PG_CLIENT_PROJECTG_CRITICALSECTION_H

#include <windows.h>

#include "..\..\defines.h"

// A Win32 critical section (size 0x18: the CRITICAL_SECTION alone, no
// vtable; its constructor passes this to InitializeCriticalSection).
class CCriticalSection {
public:
	// criticalsection.h:20
	CCriticalSection(void) {
		InitializeCriticalSection(&m_cs);
	}

	// criticalsection.h:29
	~CCriticalSection(void) {
		DeleteCriticalSection(&m_cs);
	}

	// criticalsection.h:37
	void Enter(void) {
		EnterCriticalSection(&m_cs);
	}

	// criticalsection.h:45
	void Leave(void) {
		LeaveCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION m_cs;          // 0x0
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(CCriticalSection, 0x18);
BEGIN_LAYOUT_CHECK(CCriticalSection)
	CHECK_FIELD(CCriticalSection, m_cs, 0x0);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CRITICALSECTION_H
