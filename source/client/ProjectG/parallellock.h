// Original: d:\build\custom\temp\code\client\projectg\parallellock.h
#ifndef PG_CLIENT_PROJECTG_PARALLELLOCK_H
#define PG_CLIENT_PROJECTG_PARALLELLOCK_H

#include <windows.h>
#include "..\..\defines.h"

namespace Py {
namespace Threading {

// Interface of the locks; the constructor stores no vptr (novtable).
class __declspec(novtable) IParallelLock {
public:
	virtual void Lock() = 0;        // vtable slot 0
	virtual void Unlock() = 0;      // vtable slot 1
};
CHECK_SIZE(IParallelLock, 0x4);

// Scoped lock: locks at construction, unlocks at destruction.
class SingleLock {
public:
	SingleLock(IParallelLock* lock)
		: m_lock(lock) {
		m_lock->Lock();
	}
	~SingleLock()
	{
		m_lock->Unlock();
	}

private:
	IParallelLock* m_lock;          // 0x0

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(SingleLock, 0x4);
BEGIN_LAYOUT_CHECK(SingleLock)
	CHECK_FIELD(SingleLock, m_lock, 0x0);
END_LAYOUT_CHECK

class CriticalSection : public IParallelLock {
public:
	CriticalSection()
	{
		InitializeCriticalSection(&m_cs);
	}
	~CriticalSection()
	{
		DeleteCriticalSection(&m_cs);
	}
	virtual void Lock()
	{
		EnterCriticalSection(&m_cs);
	}
	virtual void Unlock()
	{
		LeaveCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION m_cs;          // 0x4

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(CriticalSection, 0x1C);
BEGIN_LAYOUT_CHECK(CriticalSection)
	CHECK_FIELD(CriticalSection, m_cs, 0x4);
END_LAYOUT_CHECK

} // namespace Threading
} // namespace Py

#endif
