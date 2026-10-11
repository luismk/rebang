// Original: d:\build\custom\temp\code\client\projectg\lobbytask.h
#ifndef PG_CLIENT_PROJECTG_LOBBYTASK_H
#define PG_CLIENT_PROJECTG_LOBBYTASK_H

#include "..\..\defines.h"
#include "RTTI.h"
#include "TaskManager.h"

// The lobby screen. Adds no member to CTask (operator new 0x1FC).
class CLobbyTask : public CTask {
	friend IObject* CLobbyTaskMakeInstance(void);

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // lobbytask.h:14

	virtual ~CLobbyTask(void);

protected:
	CLobbyTask(void);

	virtual void Register(void);
	virtual void Init(const char *backGround);
	virtual void Process(float dt);
	virtual void Display(void);
	virtual int OnPacket(WReceivedPacket &packet);
	virtual void Load(void);
};

#endif // PG_CLIENT_PROJECTG_LOBBYTASK_H
