// Original: d:\build\custom\temp\code\client\projectg\tutorialtask.h
#ifndef PG_CLIENT_PROJECTG_TUTORIALTASK_H
#define PG_CLIENT_PROJECTG_TUTORIALTASK_H

#include "..\..\defines.h"
#include "TaskManager.h"

// The tutorial screen. Adds no member to CTask (operator new 0x1FC).
class CTutorialTask : public CTask {
	friend IObject* CTutorialTaskMakeInstance(void);

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // tutorialtask.h:8

	virtual ~CTutorialTask(void);

protected:
	CTutorialTask(void);

	virtual void Register(void);
	virtual void Init(const char *backGround);
	virtual void Process(float dt);
	virtual void Display(void);
	virtual void Destroy(void);
	virtual int OnPacket(WReceivedPacket &packet);
	virtual void Load(void);
	virtual void PreserveBack(const char *pNextTask);
	virtual void RestorePreserved(void);
};

#endif // PG_CLIENT_PROJECTG_TUTORIALTASK_H
