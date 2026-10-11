// Original: d:\build\custom\temp\code\client\projectg\avatar_task.h
#ifndef PG_CLIENT_PROJECTG_AVATAR_TASK_H
#define PG_CLIENT_PROJECTG_AVATAR_TASK_H

#include <windows.h>
#include <list>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "TaskManager.h"
#include "avatar_avatar.h"

// The avatar (lounge) chat task (operator new 0x23C). Layout from the
// constructor (avatar_task.cpp:41).
class ntAvatarChatTask : public CTask {
	LAYOUT_CHECK_FRIEND;
	friend IObject* ntAvatarChatTaskMakeInstance(void);

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // avatar_task.h:20

	void ShowQuitDialog(void);
	void AddAvatarBurden(const ntAvatarBurden &burden);
	void RemoveAvatarBurden(const ntAvatarBurden &burden);

protected:
	ntAvatarChatTask(void);
	virtual ~ntAvatarChatTask(void);

	virtual void Register(void);
	virtual void Init(const char *backGround);
	virtual void PreLoadInit(void);
	virtual void Process(float dt);
	virtual void Display(void);
	virtual void Destroy(void);
	virtual int OnPacket(WReceivedPacket &packet);
	virtual void Load(void);
	virtual void PreserveBack(const char *);
	virtual void RestorePreserved(void);
	virtual void OnAnotherHand(int nWork, void *pData);

private:
	void DeleteBurdenPet(std::list<ntAvatarBurden> &, std::list<ntAvatarBurden> &);
	void KeepUniqueCompleteList(const ntAvatarBurden &burden);

	std::list<ntAvatarBurden> m_completeList; // 0x1FC (burdens loaded; searched by KeepUniqueCompleteList)
	std::list<ntAvatarBurden> m_addList;      // 0x208 (AddAvatarBurden appends)
	std::list<ntAvatarBurden> m_removeList;   // 0x214 (RemoveAvatarBurden appends)
	CRITICAL_SECTION m_burdenLock;            // 0x220 (guards the burden lists)
	bool m_destroying;                        // 0x238 (set by the destructor; RemoveAvatarBurden then does nothing)
};
CHECK_SIZE(ntAvatarChatTask, 0x23C);
BEGIN_LAYOUT_CHECK(ntAvatarChatTask)
	CHECK_FIELD(ntAvatarChatTask, m_completeList, 0x1FC);
	CHECK_FIELD(ntAvatarChatTask, m_addList, 0x208);
	CHECK_FIELD(ntAvatarChatTask, m_removeList, 0x214);
	CHECK_FIELD(ntAvatarChatTask, m_burdenLock, 0x220);
	CHECK_FIELD(ntAvatarChatTask, m_destroying, 0x238);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_AVATAR_TASK_H
