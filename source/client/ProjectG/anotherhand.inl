// Original: d:\build\custom\temp\code\client\projectg\anotherhand.inl
// Inline members of CAnotherHand, included at the end of anotherhand.h.

// REALMYROOMMAIN_ANOTHERHAND_INLINE is defined in anotherhand.h.

// anotherhand.inl:8
inline bool CAnotherHand::IsWorkDone(void) {
	EnterCriticalSection(&m_queueLock);
	m_queueEntered = true;
	if (m_queueHead == m_queueTail) {
		m_queueEntered = false;
		LeaveCriticalSection(&m_queueLock);
		return true;
	}
	m_queueEntered = false;
	LeaveCriticalSection(&m_queueLock);
	return false;
}

// anotherhand.inl:28
REALMYROOMMAIN_ANOTHERHAND_INLINE bool CAnotherHand::IsWorking(int nWork) {
	EnterCriticalSection(&m_queueLock);
	m_queueEntered = true;
	if (m_queueHead != m_queueTail && m_queue[m_queueHead].nWork == nWork) {
		m_queueEntered = false;
		LeaveCriticalSection(&m_queueLock);
		return true;
	}
	m_queueEntered = false;
	LeaveCriticalSection(&m_queueLock);
	return false;
}

// anotherhand.inl:48
REALMYROOMMAIN_ANOTHERHAND_INLINE void CAnotherHand::Enter(void) {
	EnterCriticalSection(&m_workLock);
	m_entered = true;
}

// anotherhand.inl:55
REALMYROOMMAIN_ANOTHERHAND_INLINE void CAnotherHand::Leave(void) {
	m_entered = false;
	LeaveCriticalSection(&m_workLock);
}
