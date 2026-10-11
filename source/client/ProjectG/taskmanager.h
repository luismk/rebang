// Original: d:\build\custom\temp\code\client\projectg\taskmanager.h
#ifndef PG_CLIENT_PROJECTG_TASKMANAGER_H
#define PG_CLIENT_PROJECTG_TASKMANAGER_H

#include <list>
#include <queue>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "ObjectFactory.h"
#include "wlist.h"
#include "AnotherHand.h"
#include "frcmdtarget.h"
#include "actor.h"
#include "work.h"

class FrForm;
class FrGaugeBar;
class Fresh;
class IActor;
class MsgObject;
class CWork;
class CBackGround;
class WReceivedPacket;

// Entry of a task's actor list: the actor comes first. CTask::AddActor
// allocates it as a char array, sizeof(sActor) plus the class name's length.
struct sActor {
	IActor *m_pActor;   // 0x0
	bool m_unknown4;    // 0x4 (cleared by CTask::AddActor)
	const char *m_name; // 0x8 (the actor's name, its key in the list)
};
CHECK_SIZE(sActor, 0xC);
CHECK_FIELD(sActor, m_unknown4, 0x4);
CHECK_FIELD(sActor, m_name, 0x8);

// A game screen: an IObject (RTTI), the owner of its CAnotherHand worker
// thread and the target of its Fresh windows' messages. Layout from the
// constructor (taskmanager.cpp:506); only the members this unit's code uses
// so far are named.
class CTask : public IObject, public CHandOwner, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;
	friend class CProjectG;
	friend class CTaskManager;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // taskmanager.h:47

	// One line of tip.txt: its number, then the tip's text (LoadTipText).
	struct TIP_ITEM {
		int m_number;       // 0x0 (the line's first token)
		std::string m_text; // 0x4 (the rest of the line)
	};

	CTask(void);
	virtual ~CTask(void);
	// Virtuals in the order of ??_7CTask@@6BIObject@@@ (after GetRTTI and
	// the destructor, IObject's slots).
	virtual void ApplyScreenSize(void);
	virtual void Register(void) {}   // taskmanager.h:76
	virtual void Init(const char *backGround);
	virtual void PreLoadInit(void);
	virtual void Reload(void);
	virtual void ProcessNetSyncActor(bool, float, int);
	virtual void Process(float dt);
	virtual void Display(void);
	virtual void Destroy(void);
	virtual void SendGlobalMsg(MsgObject msg);
	virtual void SendMsgToMainActor(MsgObject msg);
	// taskmanager.h:97
	virtual int OnPacket(WReceivedPacket &packet)
	{
		return 0;
	}
	virtual void OnInitFinished(void);

	IActor *GetMainActor(void) const;
	IActor *GetActor(const char *name);
	IActor *AddActor(const char *className, const char *name, int, const std::string &);
	void SetInited(bool inited);
	void PushMsg(void *pMsg);
	bool IsRecieveWhisper(void);
	bool IsInited(void);   // taskmanager.h:105 (defined below the class)
	void SetPause(bool pause);   // taskmanager.h:106
	void SetWhisper(bool b) { m_bRecieveWhisper = b; }   // taskmanager.h:108

	// Has the named actor of this task handle msg at once (nothing if the
	// task has no such actor). Always inlined: CTask is a friend of IActor,
	// which keeps HandleMsg protected, so dialogs reach an actor through
	// here (AfxGetTask()->PassMsgToActor("Lobby", MsgObject(...))).
	void PassMsgToActor(const char *name, const MsgObject &msg) {
		IActor *pActor = GetActor(name);
		if (pActor) {
			pActor->HandleMsg(msg);
		}
	}
	// Same for the main actor, by reference (SendMsgToMainActor is the
	// virtual, by-value form).
	void PassMsgToMainActor(const MsgObject &msg) {
		IActor *pMainActor = m_mainActor;
		if (pMainActor) {
			pMainActor->HandleMsg(msg);
		}
	}

protected:
	virtual void OnProcessWhileLoading(float dt);
	virtual void OnDisplayWhileLoading(void);
	virtual void Load(void);
	virtual void PreserveBack(const char *);
	virtual void RestorePreserved(void);

	void SetMainActor(IActor *pActor);
	bool OnTipDlgResult(int result, FrForm *pForm);
	FrForm *LoadTipText(int, int);
	void SetProgressBar(bool show);
	void SetTip(bool b) { m_unknown59 = b; }   // taskmanager.h:148
	void SetLoadingBackGround(const std::string &backGround);
	bool OnOverlappedDlg(int result, FrForm *pForm);
	int OnPacketCommon(WReceivedPacket &packet);
	virtual void OnAnotherHand(int nWork, void *pData);   // taskmanager.cpp:710, CHandOwner's slot

	// Actor every task shares as a stand-in (taskmanager.cpp:268).
	static IActor *ms_pDummyActor;

private:
	int _RemoveWareHouseItem(unsigned long typeId, IActor *pActor, int count);
	int _RemoveWareHouseItem(unsigned long typeId, const char *actorName, int count);
	void _ReFreshMyRoomGift(bool, std::string);

protected:
	bool m_unknownC;            // 0xC (cleared by the constructor; set by CLobbyTask's)
	WList<sActor *> m_actorList; // 0x10
private:
	IActor *m_mainActor;   // 0x30 (set by SetMainActor)
	CTaskDoc *m_pDoc;      // 0x34 (handed to every actor AddActor creates; deleted by the destructor)
protected:
	bool m_unknown38;      // 0x38 (set by the constructor; cleared by CGolfTask's)
	std::string m_loadingBackGround; // 0x3C
private:
	bool m_bProgressBar;    // 0x58
	bool m_unknown59;       // 0x59 (show a tip while loading; SetTip, set by the constructor)
protected:
	Fresh *m_pLoadingFresh; // 0x5C (displayed while loading)
private:
	FrForm *m_pTipForm;     // 0x60 (the loading screen's tip form, from LoadTipText; cleared by OnTipDlgResult)
protected:
	FrGaugeBar *m_tipDlgState; // 0x64 (the loading progress gauge, given its range by PreLoadInit; cleared by OnTipDlgResult)
private:
protected:
	CAnotherHand m_anotherHand; // 0x68
private:
	unsigned int m_stateFlags; // 0x1F0 (bit 1: inited)
	bool m_bRecieveWhisper; // 0x1F4
	bool m_pause;           // 0x1F5
	UNKNOWN(0x1F6, 0x1F8);
	float m_netSyncTime;    // 0x1F8 (time not yet handed to ProcessNetSyncActor, in 0.02 s steps)
};
BEGIN_LAYOUT_CHECK(CTask)
	CHECK_FIELD(CTask, m_unknownC, 0xC);
	CHECK_FIELD(CTask, m_actorList, 0x10);
	CHECK_FIELD(CTask, m_pDoc, 0x34);
	CHECK_FIELD(CTask, m_unknown38, 0x38);
	CHECK_FIELD(CTask, m_mainActor, 0x30);
	CHECK_FIELD(CTask, m_loadingBackGround, 0x3C);
	CHECK_FIELD(CTask, m_bProgressBar, 0x58);
	CHECK_FIELD(CTask, m_unknown59, 0x59);
	CHECK_FIELD(CTask, m_pLoadingFresh, 0x5C);
	CHECK_FIELD(CTask, m_pTipForm, 0x60);
	CHECK_FIELD(CTask, m_tipDlgState, 0x64);
	CHECK_FIELD(CTask, m_anotherHand, 0x68);
	CHECK_FIELD(CTask, m_stateFlags, 0x1F0);
	CHECK_FIELD(CTask, m_bRecieveWhisper, 0x1F4);
	CHECK_FIELD(CTask, m_pause, 0x1F5);
	CHECK_FIELD(CTask, m_netSyncTime, 0x1F8);
END_LAYOUT_CHECK
CHECK_SIZE(CTask::TIP_ITEM, 0x20);
CHECK_FIELD(CTask::TIP_ITEM, m_text, 0x4);

// GolfTask.cpp defines GOLFTASK_TASKMANAGER_INLINE empty before its first
// include, so its object emits the out-of-line copy of CTask::IsInited, as
// the original's did.
#ifndef GOLFTASK_TASKMANAGER_INLINE
#define GOLFTASK_TASKMANAGER_INLINE inline
#endif

// taskmanager.h:105
GOLFTASK_TASKMANAGER_INLINE bool CTask::IsInited(void) {
	return (m_stateFlags >> 1) & 1;
}

// Only the members this project's code uses so far are declared.
class CTaskManager : public WSingleton<CTaskManager> {
	LAYOUT_CHECK_FRIEND;
	friend class CProjectG;

public:
	// Navigation history entry: the task (ChangeTask's arguments), then the
	// actor and the layout NaviGoForward/NaviGoBackward reopen in it.
	struct history_t {
		const char *m_taskName;   // 0x0
		const char *m_docName;    // 0x4
		const char *m_actorName;  // 0x8
		const char *m_layoutName; // 0xC (sent to the actor as message 0's param1)
	};

	// A message queued by PostMsg for the named actor.
	struct sPostMsg {
		std::string m_target;  // 0x0
		MsgObject m_msg;       // 0x1C
	};

	CTaskManager(void);
	virtual ~CTaskManager(void);

	CTask *GetPreservedTask(void) const;
	void ReleaseWork(void);
	void CreateTask(const char *taskName, const char *param, bool preserve);
	void ChangeTask(const char *taskName, const char *param, bool preserve);
	void PreProcess(void);
	void ProcessTask(float dt);
	CTask *GetCurrentTask(void) const;
	void DisplayTask(void) const;
	void ClearTask(void);
	bool IsNaviForwardEnable(void);
	bool IsNaviBackwardEnable(void);
	int PostMsg(const IActor *pSender, const char *pTarget, int msg, intptr_t param1, intptr_t param2, intptr_t param3, unsigned long param4);
	void NaviDisplayDebugInfo(void);
	void NaviGoBackward(void);
	void NaviGoForward(void);
	void NaviOpenLayout(const char *taskName, const char *docName, const char *actorName, const char *layoutName);

protected:
	void NaviAddHistory(const char *taskName, const char *docName, const char *actorName, const char *layoutName);
	void NaviClearHistory(void);
	bool IsNaviForwardSameHistory(const char *layoutName);

private:
	bool m_bReservedTask;           // 0x28 (a task change is pending)
	bool m_reservedPreserve;        // 0x29
	UNKNOWN(0x2A, 0x2C);
	const char *m_reservedTaskName; // 0x2C
	const char *m_reservedParam;    // 0x30
	CTask *m_currentTask;     // 0x34
	CTask *m_preservedTask;   // 0x38
	CBackGround *m_pBackGround; // 0x3C (drawn over the task, recreated by ChangeTask)
	float m_backGroundAlpha;    // 0x40 (0..1)
	std::queue<sPostMsg> m_postMsgQueue;       // 0x44 (PostMsg)
	std::list<history_t> m_history;            // 0x58
	std::list<history_t>::iterator m_curHistory; // 0x64
	int m_naviState;          // 0x68 (NaviOpenLayout; cleared by NaviGoForward)
	CWork *m_pWork;           // 0x6C (SetWork, ReleaseWork)
};
CHECK_SIZE(CTaskManager::history_t, 0x10);
CHECK_SIZE(CTaskManager::sPostMsg, 0x3C);
CHECK_SIZE(CTaskManager, 0x70);
BEGIN_LAYOUT_CHECK(CTaskManager)
	CHECK_FIELD(CTaskManager, m_bReservedTask, 0x28);
	CHECK_FIELD(CTaskManager, m_reservedTaskName, 0x2C);
	CHECK_FIELD(CTaskManager, m_currentTask, 0x34);
	CHECK_FIELD(CTaskManager, m_history, 0x58);
	CHECK_FIELD(CTaskManager, m_curHistory, 0x64);
	CHECK_FIELD(CTaskManager, m_preservedTask, 0x38);
	CHECK_FIELD(CTaskManager, m_pBackGround, 0x3C);
	CHECK_FIELD(CTaskManager, m_backGroundAlpha, 0x40);
	CHECK_FIELD(CTaskManager, m_postMsgQueue, 0x44);
	CHECK_FIELD(CTaskManager, m_naviState, 0x68);
	CHECK_FIELD(CTaskManager, m_pWork, 0x6C);
END_LAYOUT_CHECK

// Per-task document object: an IObject (inheriting its GetRTTI) holding
// the task it belongs to.
class CTaskDoc : public IObject {
	LAYOUT_CHECK_FRIEND;
	friend class CTaskManager;

public:
	CTaskDoc(void);
	virtual ~CTaskDoc(void);
	CTask *GetTask(void) const;

private:
	CTask *m_pTask;   // 0x4
};
BEGIN_LAYOUT_CHECK(CTaskDoc)
	CHECK_FIELD(CTaskDoc, m_pTask, 0x4);
END_LAYOUT_CHECK

int AfxPostMsg(const IActor *pSender, const char *pTarget, int msg, intptr_t param1, intptr_t param2, intptr_t param3, unsigned long param4);   // taskmanager.h:1193
// taskmanager.h:1201 (out of line only in ClientSetting; inlined elsewhere)
inline CTask *AfxGetTask(void) {
	if (!CTaskManager::Instance()) {
		return 0;
	}
	return CTaskManager::Instance()->GetCurrentTask();
}

// TaskMain.cpp defines TASKMAIN_TASKMANAGER_INLINE empty before its first
// include, so its object emits the out-of-line copies of GetPreservedTask
// and AfxGetPreservedTask, as the original's did.
#ifndef TASKMAIN_TASKMANAGER_INLINE
#define TASKMAIN_TASKMANAGER_INLINE inline
#endif

// taskmanager.h:260
TASKMAIN_TASKMANAGER_INLINE CTask *CTaskManager::GetPreservedTask(void) const {
	return m_preservedTask;
}

// taskmanager.h:1210
TASKMAIN_TASKMANAGER_INLINE CTask *AfxGetPreservedTask(void) {
	if (CTaskManager::Instance() == NULL)
		return NULL;

	return CTaskManager::Instance()->GetPreservedTask();
}

// TaskManager.cpp defines TASKMANAGER_TASKMANAGER_INLINE empty before its
// first include, so its object emits the out-of-line copy of ReleaseWork, as
// the original's did.
#ifndef TASKMANAGER_TASKMANAGER_INLINE
#define TASKMANAGER_TASKMANAGER_INLINE inline
#endif

// taskmanager.h:309
TASKMANAGER_TASKMANAGER_INLINE void CTaskManager::ReleaseWork(void) {
	CWork *work = m_pWork;
	m_pWork = 0;
	if (work)
		delete work;
}

#endif // PG_CLIENT_PROJECTG_TASKMANAGER_H
