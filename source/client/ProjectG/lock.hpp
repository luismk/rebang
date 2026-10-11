// Original: d:\build\custom\temp\code\client\projectg\lock.hpp
#ifndef PG_CLIENT_PROJECTG_LOCK_HPP
#define PG_CLIENT_PROJECTG_LOCK_HPP

#include <windows.h>

#include "..\..\defines.h"

namespace _client {

namespace _private {

// Holds a lock for its lifetime.
template <class T>
class CLock {
public:
	CLock(T &lock) : m_lock(lock) {   // lock.hpp:14
		m_lock.lock();
	}
	~CLock(void) {   // lock.hpp:18
		m_lock.unlock();
	}

private:
	T &m_lock;   // 0x0
};

} // namespace _private

// A Win32 critical section.
class CCriticalSection {
public:
	CCriticalSection(void) {   // lock.hpp:35
		InitializeCriticalSection(&m_cs);
	}
	virtual ~CCriticalSection(void) {   // lock.hpp:37
		DeleteCriticalSection(&m_cs);
	}
	void lock(void) { EnterCriticalSection(&m_cs); }     // lock.hpp:39
	void unlock(void) { LeaveCriticalSection(&m_cs); }   // lock.hpp:40

private:
	LAYOUT_CHECK_FRIEND;

	// 0x0: vftable
	CRITICAL_SECTION m_cs;   // 0x4
};

} // namespace _client

CHECK_SIZE(_client::CCriticalSection, 0x1C);
BEGIN_LAYOUT_CHECK(_client::CCriticalSection)
	CHECK_FIELD(_client::CCriticalSection, m_cs, 0x4);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_LOCK_HPP
