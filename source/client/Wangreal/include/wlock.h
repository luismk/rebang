// Original: d:\build\custom\temp\code\client\wangreal\include\wlock.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WLOCK_H
#define PG_CLIENT_WANGREAL_INCLUDE_WLOCK_H

#include <windows.h>

#include "..\..\defines.h"

class WLock {
public:
	WLock();
	~WLock();

	void Lock();
	void Unlock();
	void UnlockInThread(unsigned long threadID);

	unsigned long m_threadID;                   // 0x0
	_RTL_CRITICAL_SECTION m_criticalSection;    // 0x4
	bool m_locked;                              // 0x1c
};
CHECK_SIZE(WLock, 0x20);
CHECK_FIELD(WLock, m_threadID, 0x0);
CHECK_FIELD(WLock, m_criticalSection, 0x4);
CHECK_FIELD(WLock, m_locked, 0x1c);

#endif
