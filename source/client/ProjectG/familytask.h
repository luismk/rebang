// Original: d:\build\custom\temp\code\client\projectg\familytask.h
#ifndef PG_CLIENT_PROJECTG_FAMILYTASK_H
#define PG_CLIENT_PROJECTG_FAMILYTASK_H

#include "..\..\defines.h"
#include "TaskManager.h"

// The family screen. Adds no member to CTask (operator new 0x1FC).
class CFamilyTask : public CTask {
	friend IObject* CFamilyTaskMakeInstance(void);

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	virtual ~CFamilyTask(void);

protected:
	CFamilyTask(void);

	virtual void Register(void);
	virtual void Init(const char *backGround);
	virtual void Process(float dt);
	virtual void Display(void);
	virtual int OnPacket(WReceivedPacket &packet);
	virtual void Load(void);
};

#endif // PG_CLIENT_PROJECTG_FAMILYTASK_H
