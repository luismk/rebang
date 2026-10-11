// Original: d:\build\custom\temp\code\client\projectg\anotherhand.h
#ifndef PG_CLIENT_PROJECTG_ANOTHERHAND_H
#define PG_CLIENT_PROJECTG_ANOTHERHAND_H

#include <windows.h>

#include "..\..\defines.h"

// Receives the jobs a CAnotherHand runs on its worker thread. Only the slot
// the thread calls is known (AnotherHand_ThreadProc, anotherhand.cpp:142);
// its name comes from the overriders (?OnAnotherHand@CTask@@MAEXHPAX@Z).
class CHandOwner {
public:
	CHandOwner(void);

	virtual void OnAnotherHand(int nWork, void *pData) = 0;   // vtable slot 0
};

unsigned int __stdcall AnotherHand_ThreadProc(void *pParam);

// A worker thread that runs jobs for a CHandOwner. Layout from the
// constructor (anotherhand.cpp:18) and the inline helpers.
class CAnotherHand {
	LAYOUT_CHECK_FRIEND;
	friend unsigned int __stdcall AnotherHand_ThreadProc(void *pParam);
public:
	enum {
		MAX_QUEUE = 16
	};

	// One queued job (Work, anotherhand.cpp:92-95).
	struct sWork {
		CHandOwner *pOwner;                  // 0x0
		int nWork;                           // 0x4
		void *pData;                         // 0x8
		int nPriority;                       // 0xC, thread priority to run it at
		DWORD dwQueuedTime;                  // 0x10, timeGetTime() when queued
	};

	CAnotherHand(void);
	virtual ~CAnotherHand(void);

	bool Work(CHandOwner *pOwner, int, void *, bool, int);
	bool WaitUntilWorkDone(unsigned long);
	void BreakWorkingThread(void);
	bool IsWorkDone(void);
	bool IsWorking(int);                     // anotherhand.inl:28

	unsigned long GetThreadID(void) const;   // anotherhand.h:56

	void Enter(void);                        // anotherhand.inl:48
	void Leave(void);                        // anotherhand.inl:55

protected:
	void ResetQueue(void);

	bool m_entered;                          // 0x4, set while m_workLock is held
	bool m_queueEntered;                     // 0x5, set while m_queueLock is held
	HANDLE m_hThread;                        // 0x8
	unsigned long m_threadID;                // 0xC, -1 before the thread starts
	int m_queueHead;                         // 0x10
	int m_queueTail;                         // 0x14
	CRITICAL_SECTION m_queueLock;            // 0x18
	CRITICAL_SECTION m_workLock;             // 0x30
	sWork m_queue[MAX_QUEUE];                // 0x48, ring buffer indexed by m_queueHead/m_queueTail
};

// RealMyRoomMain.cpp defines REALMYROOMMAIN_ANOTHERHAND_INLINE empty before
// its includes: the original holds the out-of-line copies of CAnotherHand's
// inlines (GetThreadID, IsWorking, Enter, Leave) in RealMyRoomMain's object;
// every other unit inlines them.
#ifndef REALMYROOMMAIN_ANOTHERHAND_INLINE
#define REALMYROOMMAIN_ANOTHERHAND_INLINE inline
#endif

// anotherhand.h:56
REALMYROOMMAIN_ANOTHERHAND_INLINE unsigned long CAnotherHand::GetThreadID(void) const {
	return m_threadID;
}

CHECK_SIZE(CAnotherHand::sWork, 0x14);
CHECK_SIZE(CAnotherHand, 0x188);
BEGIN_LAYOUT_CHECK(CAnotherHand)
	CHECK_FIELD(CAnotherHand, m_entered, 0x4);
	CHECK_FIELD(CAnotherHand, m_queueEntered, 0x5);
	CHECK_FIELD(CAnotherHand, m_hThread, 0x8);
	CHECK_FIELD(CAnotherHand, m_threadID, 0xC);
	CHECK_FIELD(CAnotherHand, m_queueHead, 0x10);
	CHECK_FIELD(CAnotherHand, m_queueTail, 0x14);
	CHECK_FIELD(CAnotherHand, m_queueLock, 0x18);
	CHECK_FIELD(CAnotherHand, m_workLock, 0x30);
	CHECK_FIELD(CAnotherHand, m_queue, 0x48);
END_LAYOUT_CHECK

#include "anotherhand.inl"

#endif // PG_CLIENT_PROJECTG_ANOTHERHAND_H
