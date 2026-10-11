// Original: d:\build\custom\temp\code\client\projectg\bubble.h
#ifndef PG_CLIENT_PROJECTG_BUBBLE_H
#define PG_CLIENT_PROJECTG_BUBBLE_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"
#include "wmath.h"

class CTalkBox;
class IObject;

IObject* CBubbleMakeInstance(void);

// Two speech balloons shown over the course (the caddie's and another
// speaker's).
class CBubble : public IActor {
	LAYOUT_CHECK_FRIEND;
	friend IObject* CBubbleMakeInstance(void);
public:
	virtual ~CBubble(void) {}   // bubble.h:11
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // bubble.h:13

	struct sBubble {
		sBubble();

		CTalkBox* m_pTalkBox;   // 0x0
		WVector m_pos;          // 0x4, world position the balloon is shown at
		bool m_bShow;           // 0x10
	};

	virtual void OnLoad(void);
	virtual void OnProcess(float elapsed);
	virtual void OnDisplay(void);
	virtual void OnDestroy(void);

protected:
	CBubble(void);

	virtual void HandleMsg(const MsgObject& msg);

	sBubble* m_pBubbles;        // 0x18, new sBubble[2]
	bool m_bEnable;             // 0x1c
};

CHECK_SIZE(CBubble::sBubble, 0x14);
CHECK_FIELD(CBubble::sBubble, m_pTalkBox, 0x0);
CHECK_FIELD(CBubble::sBubble, m_pos, 0x4);
CHECK_FIELD(CBubble::sBubble, m_bShow, 0x10);
CHECK_SIZE(CBubble, 0x20);
BEGIN_LAYOUT_CHECK(CBubble)
	CHECK_FIELD(CBubble, m_pBubbles, 0x18);
	CHECK_FIELD(CBubble, m_bEnable, 0x1c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_BUBBLE_H
