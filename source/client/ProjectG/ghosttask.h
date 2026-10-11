// Original: d:\build\custom\temp\code\client\projectg\ghosttask.h
#ifndef PG_CLIENT_PROJECTG_GHOSTTASK_H
#define PG_CLIENT_PROJECTG_GHOSTTASK_H

#include "..\..\defines.h"
#include "TaskManager.h"

// The ghost (replay) screen. Adds no member to CTask (operator new 0x1FC).
class CGhostTask : public CTask {
	friend IObject* CGhostTaskMakeInstance(void);

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ghosttask.h:4

	virtual ~CGhostTask(void) {}   // ghosttask.h:7

protected:
	CGhostTask(void);

	virtual void Register(void);
	virtual void Init(const char *backGround);
	virtual void Process(float dt);
	virtual void Display(void);
	virtual int OnPacket(WReceivedPacket &packet);
	virtual void Load(void);
};

#endif // PG_CLIENT_PROJECTG_GHOSTTASK_H
