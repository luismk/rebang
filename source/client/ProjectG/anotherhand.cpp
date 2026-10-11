// Original: d:\build\custom\temp\code\client\projectg\anotherhand.cpp
#include "AnotherHand.h"

#include <process.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <time.h>

// anotherhand.cpp:18
CAnotherHand::CAnotherHand(void) {
	m_hThread = NULL;
	m_entered = false;
	m_queueEntered = false;
	m_threadID = -1;
	ResetQueue();
	InitializeCriticalSection(&m_queueLock);
	InitializeCriticalSection(&m_workLock);
}

// anotherhand.cpp:30
CAnotherHand::~CAnotherHand(void) {
	if (m_hThread) {
		WaitForSingleObject(m_hThread, 9999);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	if (m_entered) {
		m_entered = false;
		LeaveCriticalSection(&m_workLock);
	}
	if (m_queueEntered) {
		m_queueEntered = false;
		LeaveCriticalSection(&m_queueLock);
	}
	DeleteCriticalSection(&m_queueLock);
	DeleteCriticalSection(&m_workLock);
}

// anotherhand.cpp:60
bool CAnotherHand::Work(CHandOwner *pOwner, int nWork, void *pData, bool bReplace, int nPriority) {
	EnterCriticalSection(&m_queueLock);
	m_queueEntered = true;

	int tail;
	if (!bReplace || m_queueHead == m_queueTail) {
		tail = m_queueTail;
	} else {
		tail = (m_queueHead + 1) % MAX_QUEUE;
		if (tail == m_queueHead) {
			m_queueEntered = false;
			LeaveCriticalSection(&m_queueLock);
			return false;
		}
	}

	int next = (tail + 1) % MAX_QUEUE;
	if (next == m_queueHead) {
		m_queueEntered = false;
		LeaveCriticalSection(&m_queueLock);
		return false;
	}

	m_queueTail = next;
	sWork *pWork = &m_queue[tail];
	pWork->pOwner = pOwner;
	pWork->nWork = nWork;
	pWork->pData = pData;
	pWork->nPriority = nPriority;
	pWork->dwQueuedTime = timeGetTime();

	if (!m_hThread) {
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, AnotherHand_ThreadProc, this, 0, (unsigned int *)&m_threadID);
		if (!m_hThread) {
			m_queueEntered = false;
			LeaveCriticalSection(&m_queueLock);
			m_threadID = -1;
			return false;
		}
	}

	m_queueEntered = false;
	LeaveCriticalSection(&m_queueLock);
	return true;
}

// anotherhand.cpp:121
bool CAnotherHand::WaitUntilWorkDone(unsigned long dwMilliseconds) {
	if (m_hThread) {
		return WaitForSingleObject(m_hThread, dwMilliseconds) == WAIT_TIMEOUT;
	}
	return false;
}

// anotherhand.cpp:130
unsigned int __stdcall AnotherHand_ThreadProc(void *pParam) {
	srand((unsigned int)time(NULL));
	CAnotherHand *pHand = (CAnotherHand *)pParam;
	while (true) {
		CAnotherHand::sWork *pWork = &pHand->m_queue[pHand->m_queueHead];

		if (GetThreadPriority(pHand->m_hThread) != pWork->nPriority)
			SetThreadPriority(pHand->m_hThread, pWork->nPriority);

		pWork->pOwner->OnAnotherHand(pWork->nWork, pWork->pData);

		EnterCriticalSection(&pHand->m_queueLock);
		pHand->m_queueEntered = true;
		pHand->m_queueHead = (pHand->m_queueHead + 1) % CAnotherHand::MAX_QUEUE;
		if (pHand->IsWorkDone()) {
			CloseHandle(pHand->m_hThread);
			pHand->m_hThread = NULL;
			pHand->m_queueEntered = false;
			LeaveCriticalSection(&pHand->m_queueLock);
			return 0;
		}
		pHand->m_queueEntered = false;
		LeaveCriticalSection(&pHand->m_queueLock);
	}
}

// anotherhand.cpp:183
void CAnotherHand::BreakWorkingThread(void) {
	if (m_entered) {
		m_entered = false;
		LeaveCriticalSection(&m_workLock);
	}
	if (m_queueEntered) {
		m_queueEntered = false;
		LeaveCriticalSection(&m_queueLock);
	}
	if (m_hThread) {
		DWORD dwExitCode;
		GetExitCodeThread(m_hThread, &dwExitCode);
		TerminateThread(m_hThread, dwExitCode);
		m_hThread = NULL;
	}
	ResetQueue();
}

// anotherhand.cpp:209
void CAnotherHand::ResetQueue(void) {
	m_queueHead = 0;
	m_queueTail = 0;
}
