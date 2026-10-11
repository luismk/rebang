// Original: d:\build\custom\temp\code\client\projectg\realmyroomtask.h
#ifndef PG_CLIENT_PROJECTG_REALMYROOMTASK_H
#define PG_CLIENT_PROJECTG_REALMYROOMTASK_H

#include <windows.h>
#include <list>

#include "..\..\defines.h"
#include "TaskManager.h"
#include "RealMyRoom_Avatar.h"

// The my-room screen (operator new 0x23C). Layout from the constructor
// (realmyroomtask.cpp:58).
class CRealMyRoomTask : public CTask {
	LAYOUT_CHECK_FRIEND;
	friend IObject* CRealMyRoomTaskMakeInstance(void);

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // realmyroomtask.h:24

	void AddAvatarBurden(const CRealMyRoom_AvatarBurden &burden);
	void RemoveAvatarBurden(const CRealMyRoom_AvatarBurden &burden);

protected:
	CRealMyRoomTask(void);
	virtual ~CRealMyRoomTask(void);

	virtual void Register(void);
	virtual void Init(const char *backGround);
	virtual void PreLoadInit(void);
	virtual void Process(float dt);
	virtual void Display(void);
	virtual void Destroy(void);
	virtual int OnPacket(WReceivedPacket &packet);
	virtual void Load(void);
	virtual void RestorePreserved(void);
	virtual void OnAnotherHand(int nWork, void *pData);

private:
	void DeleteBurdenPet(std::list<CRealMyRoom_AvatarBurden> &burdens, std::list<CRealMyRoom_AvatarBurden> &pending);

	std::list<CRealMyRoom_AvatarBurden> m_avatarBurdens;   // 0x1FC
	std::list<CRealMyRoom_AvatarBurden> m_addedBurdens;    // 0x208 (filled by AddAvatarBurden)
	std::list<CRealMyRoom_AvatarBurden> m_removedBurdens;  // 0x214 (filled by RemoveAvatarBurden)
	CRITICAL_SECTION m_burdenLock;                         // 0x220
	bool m_bDestroying;                                    // 0x238 (set by the destructor)
};
CHECK_SIZE(CRealMyRoomTask, 0x23C);
BEGIN_LAYOUT_CHECK(CRealMyRoomTask)
	CHECK_FIELD(CRealMyRoomTask, m_avatarBurdens, 0x1FC);
	CHECK_FIELD(CRealMyRoomTask, m_addedBurdens, 0x208);
	CHECK_FIELD(CRealMyRoomTask, m_removedBurdens, 0x214);
	CHECK_FIELD(CRealMyRoomTask, m_burdenLock, 0x220);
	CHECK_FIELD(CRealMyRoomTask, m_bDestroying, 0x238);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_REALMYROOMTASK_H
