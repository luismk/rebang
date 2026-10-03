#pragma once

#include "actor.h"

extern int LOBBYBG_MAP;
extern char* LOBBYBG_MAPNAME;

class ntLobbyBg : public IActor
{
public:
	DECLARE_OBJECT(ntLobbyBg)

	virtual void OnPreLoadInit();
	virtual void OnLoad();
	virtual void OnInit();
	virtual void OnProcess(float delta);
	virtual void OnDestroy();
	virtual void HandleMsg(const MsgObject& msg);

protected:
	ntLobbyBg();

	void DoReLoad();
	void SetSkyColor(unsigned long color);
};
