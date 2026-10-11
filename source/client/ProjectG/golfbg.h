// Original: d:\build\custom\temp\code\client\projectg\golfbg.h
#ifndef PG_CLIENT_PROJECTG_GOLFBG_H
#define PG_CLIENT_PROJECTG_GOLFBG_H

#include "..\..\defines.h"
#include "actor.h"
#include "RTTI.h"

class IObject;
class WVector;

// Actor of a round's background: the sky, the fog, the shadows and the
// weather of the course. Size 0x1c (the `new` of CGolfBgMakeInstance).
class CGolfBg : public IActor {
	LAYOUT_CHECK_FRIEND;
public:
	// golfbg.h:13
	virtual ~CGolfBg(void) {}

	static const WRTTI m_RTTI;

	// golfbg.h:15
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

protected:
	friend IObject* CGolfBgMakeInstance(void);
	CGolfBg(void);

public:
	virtual void OnDestroy(void);
	virtual void OnPreLoadInit(void);
	virtual void OnInit(void);
	virtual void OnLoad(void);
	virtual void HandleMsg(const MsgObject& msg);

protected:
	void SetGridCenter(WVector& center);
	void SetStartPoint(const WVector& point);
	void SetHolePoint(const WVector& point);
	void SetSkyColor(unsigned long color);
	void SetLowestPoint(void);
	const char* GetSkyBoxName(void);
	void SetRenderWeather(unsigned char weather);

	// golfbg.h:49
	unsigned char GetRenderWeather(void) { return m_renderWeather; }

	void DoInitAfterLoad(void);
	void DoReLoad(void);

	unsigned char m_renderWeather; // 0x18
};
CHECK_SIZE(CGolfBg, 0x1c);
BEGIN_LAYOUT_CHECK(CGolfBg)
	CHECK_FIELD(CGolfBg, m_renderWeather, 0x18);
END_LAYOUT_CHECK

IObject* CGolfBgMakeInstance(void);

#endif
