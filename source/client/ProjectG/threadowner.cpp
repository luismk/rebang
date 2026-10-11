// Original: d:\build\custom\temp\code\client\projectg\threadowner.cpp
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <wchar.h>

#include "ThreadOwner.h"

namespace Py {
namespace Threading {

// threadowner.cpp:13
unsigned int __stdcall ThreadOwner::ThreadProc(void* param) {
	ThreadOwner* owner = (ThreadOwner*)param;
	HANDLE finishEvent = owner->m_finishEvent;
	wchar_t message[256];

	swprintf(message, L"The Thread 'Win32 Thread' (%#x) be named '%s' was started.\n", GetCurrentThreadId(), owner->m_threadName);

	OutputDebugStringW(message);
	if (!owner->OnThreadProc()) {
		while (WaitForSingleObject(finishEvent, owner->m_waitTime) != WAIT_OBJECT_0) {
			owner->OnUpdateThreadProc();
		}
	}

	swprintf(message, L"The Thread 'Win32 Thread' (%#x) be named '%s' was finished.\n", GetCurrentThreadId(), owner->m_threadName);

	OutputDebugStringW(message);
	CloseHandle(finishEvent);

	return 0;
}

// threadowner.cpp:38
ThreadOwner::ThreadOwner(const wchar_t* name) : m_waitTime(0) {
	SetThreadName(name);

	m_thread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, CREATE_SUSPENDED, NULL);
	m_finishEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
}

// threadowner.cpp:46
ThreadOwner::~ThreadOwner() {
	FinishThread();
	if (m_thread != NULL) {
		CloseHandle(m_thread);
		m_thread = NULL;
	}
}

// threadowner.cpp:56
void ThreadOwner::SetThreadName(const wchar_t* name) {
	if (name != NULL) {
		wcsncpy(m_threadName, name, 31);
	} else {
		wcscpy(m_threadName, L"Unnamed");
	}
}

// threadowner.cpp:64
bool ThreadOwner::StartThread(unsigned long waitTime) {
	m_waitTime = waitTime;
	return ::ResumeThread(m_thread) != (DWORD)-1;
}

// threadowner.cpp:70
bool ThreadOwner::PauseThread() {
	return SuspendThread(m_thread) != (DWORD)-1;
}

// threadowner.cpp:75
bool ThreadOwner::ResumeThread() {
	return ::ResumeThread(m_thread) != (DWORD)-1;
}

// threadowner.cpp:80
void ThreadOwner::FinishEvent() {
	if (m_finishEvent != NULL) {
		SetEvent(m_finishEvent);
		m_finishEvent = NULL;
	}
}

// threadowner.cpp:89
void ThreadOwner::TerminateThread() {
	::TerminateThread(m_thread, 4);

	CloseHandle(m_finishEvent);
	m_finishEvent = NULL;
}

// threadowner.cpp:97
void ThreadOwner::FinishThread() {
	if (m_finishEvent != NULL) {
		SetEvent(m_finishEvent);
		m_finishEvent = NULL;

		if (WaitForSingleObject(m_thread, 5000) == WAIT_TIMEOUT) {
			::TerminateThread(m_thread, 6);
		}
	}
}

} // namespace Threading
} // namespace Py
