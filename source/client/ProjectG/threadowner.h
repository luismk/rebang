// Original: d:\build\custom\temp\code\client\projectg\threadowner.h
#ifndef PG_CLIENT_PROJECTG_THREADOWNER_H
#define PG_CLIENT_PROJECTG_THREADOWNER_H

#include <windows.h>

#include "..\..\defines.h"

namespace Py {
namespace Threading {

// Owns a worker thread created suspended by the constructor; StartThread
// resumes it, and ThreadProc then calls OnUpdateThreadProc every
// m_waitTime milliseconds until the finish event is signalled.
class ThreadOwner {
public:
	void SetThreadName(const wchar_t* name);
	bool StartThread(unsigned long waitTime);
	bool PauseThread();
	bool ResumeThread();
	void FinishEvent();
	void TerminateThread();
	void FinishThread();

protected:
	ThreadOwner(const wchar_t* name);
	virtual ~ThreadOwner();                     // vtable slot 0 (??_G)

	// threadowner.h:47
	virtual bool OnThreadProc() {               // vtable slot 1: true skips the update loop
		return false;
	}

	// threadowner.h:53
	virtual void OnUpdateThreadProc() {         // vtable slot 2: one tick of the update loop
	}

private:
	static unsigned int __stdcall ThreadProc(void* param);

	HANDLE m_thread;                            // 0x4 (_beginthreadex, CREATE_SUSPENDED)
	HANDLE m_finishEvent;                       // 0x8 (manual-reset, unsignalled)
	unsigned long m_waitTime;                   // 0xC (StartThread's argument)
	wchar_t m_threadName[32];                   // 0x10 (wcsncpy of 31 characters)

	LAYOUT_CHECK_FRIEND;
};
// Size assumed: the end of the last member the unit's code touches.
CHECK_SIZE(ThreadOwner, 0x50);
BEGIN_LAYOUT_CHECK(ThreadOwner)
	CHECK_FIELD(ThreadOwner, m_thread, 0x4);
	CHECK_FIELD(ThreadOwner, m_finishEvent, 0x8);
	CHECK_FIELD(ThreadOwner, m_waitTime, 0xC);
	CHECK_FIELD(ThreadOwner, m_threadName, 0x10);
END_LAYOUT_CHECK

} // namespace Threading
} // namespace Py

#endif // PG_CLIENT_PROJECTG_THREADOWNER_H
