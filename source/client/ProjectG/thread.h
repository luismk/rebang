// Original: d:\build\custom\temp\code\client\projectg\thread.h
#ifndef PG_CLIENT_PROJECTG_THREAD_H
#define PG_CLIENT_PROJECTG_THREAD_H

#include <windows.h>

#include "..\..\defines.h"

inline DWORD WINAPI ThreadFunc(LPVOID pParam);

// A worker thread: Start runs Act on a new thread, Stop waits for it.
class CThread {
	LAYOUT_CHECK_FRIEND;
	friend DWORD WINAPI ThreadFunc(LPVOID pParam);
public:
	// thread.h:10
	CThread(void) {
		m_hThread = INVALID_HANDLE_VALUE;
	}

	// thread.h:15
	virtual ~CThread(void) {}

	// thread.h:19
	virtual void Stop(void) {
		if (m_hThread != INVALID_HANDLE_VALUE) {
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_hThread);
			m_hThread = INVALID_HANDLE_VALUE;
		}
	}

	// thread.h:29
	void Start(void) {
		m_hThread = CreateThread(NULL, 0, ThreadFunc, this, 0, NULL);
	}

protected:
	virtual void Act(void) = 0;

	HANDLE m_hThread; // 0x4
};
CHECK_SIZE(CThread, 0x8);
BEGIN_LAYOUT_CHECK(CThread)
	CHECK_FIELD(CThread, m_hThread, 0x4);
END_LAYOUT_CHECK

// thread.h:40
inline DWORD WINAPI ThreadFunc(LPVOID pParam) {
	static_cast<CThread*>(pParam)->Act();
	return 0;
}

#endif // PG_CLIENT_PROJECTG_THREAD_H
