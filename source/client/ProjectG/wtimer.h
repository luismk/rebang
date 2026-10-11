// Original: d:\build\custom\temp\code\client\projectg\wtimer.h
#ifndef PG_CLIENT_PROJECTG_WTIMER_H
#define PG_CLIENT_PROJECTG_WTIMER_H

#include <list>
#include <string>

#include "..\..\defines.h"
#include "singleton.h"

// A client of the timer manager: registers named timers and is called back
// through OnTimer when one of them expires (vtable: scalar deleting
// destructor, then a pure virtual).
class CTimerEvent {
	LAYOUT_CHECK_FRIEND;
public:
	CTimerEvent(void);
	virtual ~CTimerEvent(void);

	virtual void OnTimer(void) = 0;

	void ClearTimer(const char* name);
	bool AddTimer(float time, const char* name);

private:
	std::string m_name;             // 0x04
};
CHECK_SIZE(CTimerEvent, 0x20);
BEGIN_LAYOUT_CHECK(CTimerEvent)
	CHECK_FIELD(CTimerEvent, m_name, 0x04);
END_LAYOUT_CHECK

// Ten timer slots; the running ones are linked in m_activeTimers and
// advanced by UpdateTimer.
class CTimerManager : public WSingleton<CTimerManager> {
	LAYOUT_CHECK_FRIEND;
public:
	struct sTimer {
		// wtimer.h:94
		void Clear(void) {
			m_time = 0.0f;
			m_elapsed = 0.0f;
			m_pEvent = 0;
			m_name.clear();
			m_active = false;
		}

		float m_time;               // 0x00
		float m_elapsed;            // 0x04
		CTimerEvent* m_pEvent;      // 0x08
		std::string m_name;         // 0x0c
		bool m_active;              // 0x28
	};

	CTimerManager(void);
	virtual ~CTimerManager(void);

	bool AddTimer(float time, const char* name, CTimerEvent* pEvent);
	void AllClear(void);
	void ClearTimer(const char* name);
	void UpdateTimer(float elapsed);

private:
	std::list<sTimer*> m_activeTimers;  // 0x28
	sTimer m_timers[10];                // 0x34
};
CHECK_SIZE(CTimerManager::sTimer, 0x2c);
CHECK_SIZE(CTimerManager, 0x1ec);
BEGIN_LAYOUT_CHECK(CTimerManager)
	CHECK_FIELD(CTimerManager, m_activeTimers, 0x28);
	CHECK_FIELD(CTimerManager, m_timers, 0x34);
END_LAYOUT_CHECK

#endif
