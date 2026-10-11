// Original: d:\build\custom\temp\code\client\projectg\wtimer.cpp
#include "wtimer.h"

#include <string.h>

#include <vector>

// wtimer.cpp:18
CTimerManager::CTimerManager(void) {
	for (int i = 0; i < 10; i++) {
		m_timers[i].Clear();
	}
}

// wtimer.cpp:27
CTimerManager::~CTimerManager(void) {
	m_activeTimers.clear();
}

// wtimer.cpp:33
bool CTimerManager::AddTimer(float time, const char* name, CTimerEvent* pEvent) {
	for (int i = 0; i < 10; i++) {
		if (m_timers[i].m_active == true) {
			continue;
		}

		m_timers[i].m_active = true;
		m_timers[i].m_name = name;
		m_timers[i].m_pEvent = pEvent;
		m_timers[i].m_time = time;

		m_activeTimers.push_back(&m_timers[i]);

		return true;
	}

	return false;
}

// wtimer.cpp:55
void CTimerManager::AllClear(void) {
	m_activeTimers.clear();

	for (int i = 0; i < 10; i++) {
		m_timers[i].Clear();
	}
}

// wtimer.cpp:65
void CTimerManager::ClearTimer(const char* name) {
	std::list<sTimer*>::iterator it = m_activeTimers.begin();
	while (it != m_activeTimers.end()) {
		sTimer* timer = *it;
		if (strcmp(timer->m_name.c_str(), name) != 0) {
			++it;
			continue;
		}

		timer->Clear();
		m_activeTimers.erase(it);
		break;
	}
}

// wtimer.cpp:83
void CTimerManager::UpdateTimer(float elapsed) {
	std::vector<sTimer*> timers;
	timers.insert(timers.begin(), m_activeTimers.begin(), m_activeTimers.end());

	m_activeTimers.clear();

	int count = timers.size();
	for (int i = 0; i < count; i++) {
		sTimer* timer = timers[i];
		timer->m_elapsed += elapsed;
		if (timer->m_time - timer->m_elapsed < 0.0f) {
			CTimerEvent* pEvent = timer->m_pEvent;
			timer->Clear();
			pEvent->OnTimer();
		} else {
			m_activeTimers.push_back(timer);
		}
	}
}

// wtimer.cpp:121
CTimerEvent::CTimerEvent(void) {
}

// wtimer.cpp:126
CTimerEvent::~CTimerEvent(void) {
	if (!m_name.empty()) {
		CTimerManager::Instance()->ClearTimer(m_name.c_str());
	}
}

// wtimer.cpp:135
void CTimerEvent::ClearTimer(const char* name) {
	CTimerManager::Instance()->ClearTimer(name);
}

// wtimer.cpp:140
bool CTimerEvent::AddTimer(float time, const char* name) {
	if (CTimerManager::Instance()->AddTimer(time, name, this) == true) {
		m_name = name;
		return true;
	}

	return false;
}
