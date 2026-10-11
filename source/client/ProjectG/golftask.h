// Original: d:\build\custom\temp\code\client\projectg\golftask.h
#ifndef PG_CLIENT_PROJECTG_GOLFTASK_H
#define PG_CLIENT_PROJECTG_GOLFTASK_H

#include <map>

#include "..\..\defines.h"
#include "TaskManager.h"

class Fresh;

// The in-game (golf round) task. Layout from the constructor
// (golftask.cpp:79, operator new 0x20C).
class CGolfTask : public CTask {
	friend IObject* CGolfTaskMakeInstance(void);
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // golftask.h:19

	virtual void Register(void);
	virtual void Init(const char *backGround);
	virtual void Process(float dt);
	virtual void Display(void);
	virtual void Destroy(void);
	virtual int OnPacket(WReceivedPacket &packet);
	virtual void OnInitFinished(void);
	virtual void Load(void);
	virtual void OnDisplayWhileLoading(void);

	bool DetermineWhetherToAddActor_GroundItemMan(void);

	virtual ~CGolfTask(void) {}   // golftask.h:33

protected:
	CGolfTask(void);

	void UpdateLoadingInfo(unsigned long uid, unsigned char progress);
	void DrawLoadingInfo(Fresh *pFresh);

private:
	std::map<unsigned long, unsigned char> m_loadingInfo; // 0x1FC (loading progress by player uid)
	float m_unknown208;     // 0x208 (-10.0f from the constructor)
};
BEGIN_LAYOUT_CHECK(CGolfTask)
	CHECK_FIELD(CGolfTask, m_loadingInfo, 0x1FC);
	CHECK_FIELD(CGolfTask, m_unknown208, 0x208);
END_LAYOUT_CHECK
CHECK_SIZE(CGolfTask, 0x20C);

#endif // PG_CLIENT_PROJECTG_GOLFTASK_H
