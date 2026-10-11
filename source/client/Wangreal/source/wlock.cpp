// Original: d:\build\custom\temp\code\client\wangreal\source\wlock.cpp
#include "../include/wlock.h"

// wlock.cpp:9
WLock::WLock() {
	m_threadID = 0xFFFFFFFF;
	m_locked = false;
	InitializeCriticalSection(&m_criticalSection);
}

// wlock.cpp:18
WLock::~WLock() {
	DeleteCriticalSection(&m_criticalSection);
}

// wlock.cpp:23
void WLock::Lock() {
	// Locking twice from the same thread would deadlock: crash on purpose.
	DWORD threadID = GetCurrentThreadId();
	if (threadID == m_threadID) {
		*(int*)0xDEAD20CC = 0;
	}
	EnterCriticalSection(&m_criticalSection);
	m_threadID = threadID;
	m_locked = true;
}

// wlock.cpp:40
void WLock::Unlock() {
	if (m_locked) {
		m_threadID = 0xFFFFFFFF;
		m_locked = false;
		LeaveCriticalSection(&m_criticalSection);
	}
}

// wlock.cpp:54
void WLock::UnlockInThread(unsigned long threadID) {
	if (threadID == m_threadID) {
		if (m_locked) {
			m_threadID = 0xFFFFFFFF;
			m_locked = false;
			LeaveCriticalSection(&m_criticalSection);
		}
	}
}
