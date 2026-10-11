// Original: d:\build\custom\temp\code\client\projectg\logo.h
#ifndef PG_CLIENT_PROJECTG_LOGO_H
#define PG_CLIENT_PROJECTG_LOGO_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"

class IObject;
class WOverlay;

IObject* CLogoMakeInstance(void);

// The course logo ("[logo_<map>.png") faded in over the screen's top center.
class CLogo : public IActor {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;

	virtual ~CLogo(void) {}

	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	virtual void OnDestroy(void);
	virtual void OnInit(void);
	virtual void HandleMsg(const MsgObject& msg);
	virtual void OnProcess(float elapsed);
	virtual void OnDisplay(void);
	virtual void OnLoad(void);

protected:
	friend IObject* CLogoMakeInstance(void);   // the factory creates it
	CLogo(void);

	WOverlay* m_pOverlay;   // 0x18, the logo, loaded once the round started
	float m_alpha;          // 0x1C, fade-in from 0 to 1 over three seconds
};

CHECK_SIZE(CLogo, 0x20);
BEGIN_LAYOUT_CHECK(CLogo)
	CHECK_FIELD(CLogo, m_pOverlay, 0x18);
	CHECK_FIELD(CLogo, m_alpha, 0x1C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_LOGO_H
