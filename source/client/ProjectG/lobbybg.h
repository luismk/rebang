// Original: d:\build\custom\temp\code\client\projectg\lobbybg.h
#ifndef PG_CLIENT_PROJECTG_LOBBYBG_H
#define PG_CLIENT_PROJECTG_LOBBYBG_H

#include "..\..\defines.h"
#include "actor.h"
#include "RTTI.h"

class IObject;

// The lobby background map: index and name (map number 13 has a far clip).
extern int LOBBYBG_MAP;
extern char *LOBBYBG_MAPNAME;

IObject* ntLobbyBgMakeInstance(void);

// Actor of the lobby's 3D background: loads the lobby map, its sky and
// shadows. Size 0x18 (the `new` of ntLobbyBgMakeInstance).
class ntLobbyBg : public IActor {
	friend IObject* ntLobbyBgMakeInstance(void);
public:
	static const WRTTI m_RTTI;

	// lobbybg.h:23
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

protected:
	ntLobbyBg(void);

public:
	virtual void OnPreLoadInit(void);
	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnProcess(float elapsed);
	virtual void OnDestroy(void);
	virtual void HandleMsg(const MsgObject& msg);

protected:
	void DoReLoad(void);
	void SetSkyColor(unsigned long color);
};

CHECK_SIZE(ntLobbyBg, 0x18);

#endif // PG_CLIENT_PROJECTG_LOBBYBG_H
