// Original: d:\build\custom\temp\code\client\projectg\clock.h
#ifndef PG_CLIENT_PROJECTG_CLOCK_H
#define PG_CLIENT_PROJECTG_CLOCK_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"

class IObject;

IObject* CClockMakeInstance(void);

// The shot clock shown on the screen: counts the shot time limit down,
// turns its hand and color and ticks in the last seconds.
class CClock : public IActor {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;

	virtual ~CClock(void) {}   // clock.h:9

	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // clock.h:11

	virtual void OnInit(void);
	virtual void OnProcess(float elapsed);
	virtual void HandleMsg(const MsgObject& msg);

protected:
	friend IObject* CClockMakeInstance(void);   // the factory creates it
	CClock(void);

	void Reset(void);

	PADDING(0x18, 0x1C);
	float m_angle;              // 0x1C, the hand's angle in radians (sent to "Screen")
	unsigned long m_color;      // 0x20, the clock's color
	PADDING(0x24, 0x25);
	bool m_bTicking;            // 0x25, the tick sound is playing
	PADDING(0x26, 0x28);
};

CHECK_SIZE(CClock, 0x28);
BEGIN_LAYOUT_CHECK(CClock)
	CHECK_FIELD(CClock, m_angle, 0x1C);
	CHECK_FIELD(CClock, m_color, 0x20);
	CHECK_FIELD(CClock, m_bTicking, 0x25);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CLOCK_H
