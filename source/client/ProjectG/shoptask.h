// Original: d:\build\custom\temp\code\client\projectg\shoptask.h
#ifndef PG_CLIENT_PROJECTG_SHOPTASK_H
#define PG_CLIENT_PROJECTG_SHOPTASK_H

#include "..\..\defines.h"
#include "TaskManager.h"

// The shop screen. Adds no member to CTask (operator new 0x1FC).
class CShopTask : public CTask {
	friend IObject* CShopTaskMakeInstance(void);

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // shoptask.h:14

	virtual ~CShopTask(void);

protected:
	CShopTask(void);

	virtual void Register(void);
	virtual void Init(const char *backGround);
	virtual void Process(float dt);
	virtual void Display(void);
	virtual int OnPacket(WReceivedPacket &packet);
	virtual void Load(void);
};

#endif // PG_CLIENT_PROJECTG_SHOPTASK_H
