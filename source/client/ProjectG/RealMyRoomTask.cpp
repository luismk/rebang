// Original: d:\build\custom\temp\code\client\projectg\realmyroomtask.cpp
#include "RealMyRoomTask.h"

#include "ObjectFactory.h"
#include "actor.h"
#include "fresh.h"
#include "frdesktop.h"
#include "frwndmanager.h"
#include "partsinfo.h"
#include "frwnd.h"
#include "wview.h"
#include "QuitWindow.h"
#include "avatar_task.h"
#include "SharedDoc.h"
#include "ClientSetting.h"
#include "RealMyRoomData.h"
#include "camerawork.h"
#include "TransferableItem.h"
#include "ItemStorage.h"
#include "Localize.h"

// Class and actor name pairs of the task's extra actors (unnamed data;
// pairs at 0x0 and 0xC).
extern "C" const char *data_a8d820[5];

// Loading state flags (unnamed data; set by Load).
extern int data_00b79b48;
extern bool data_00b79b4d;

// realmyroomtask.cpp:55 (run-time type and factory registration of CRealMyRoomTask)
const WRTTI CRealMyRoomTask::m_RTTI("CRealMyRoomTask", &CTask::m_RTTI);

IObject* CRealMyRoomTaskMakeInstance(void) {
	return new CRealMyRoomTask;
}

struct __sCRealMyRoomTask {
	__sCRealMyRoomTask(void) {
		ObjectFactory().AddObjectFunctor(CRealMyRoomTaskMakeInstance, "CRealMyRoomTask");
	}
};
static __sCRealMyRoomTask s_registerCRealMyRoomTask;

// realmyroomtask.cpp:81
CRealMyRoomTask::~CRealMyRoomTask(void) {
	COption::Instance()->gSetAvatarNewBie(0);

	m_bDestroying = true;
	m_anotherHand.WaitUntilWorkDone(9999);
	m_anotherHand.BreakWorkingThread();

	DeleteCriticalSection(&m_burdenLock);

	delete CRealMyRoom_AvatarManager::Instance();
	delete CRealMyRoomData::Instance();
	delete CCameraWork::Instance();
	delete CTransferableItem::Instance();

	if (IsLocalContent((localContentType_t)0x71)) {
		delete CItemStorage::Instance();
	}
}

// realmyroomtask.cpp:108
void CRealMyRoomTask::Register(void) {
	SetMainActor(AddActor("CRealMyRoomMain", "RealMyRoom", 0, ""));

	AddActor("CCamera", "Camera", 0x20, "");

	AddActor("CItemBuff", "ItemBuff", 0, "");

	AddActor(data_a8d820[0], data_a8d820[1], 0, "");

	AddActor(data_a8d820[3], data_a8d820[4], 0, GetRTTI()->GetName());

	if (AfxGetPreservedTask()) {
		IActor *pTutorial = AfxGetPreservedTask()->GetActor("TutorialMain");
		if (pTutorial) {
			const char *className = 0;
			const char *name = 0;

			pTutorial << MsgObject(0, 0x14E, (intptr_t)&className, (intptr_t)&name, 0, 0, 0);
			AddActor(className, name, 0, "");
		}
	}
}

// realmyroomtask.cpp:210
void CRealMyRoomTask::Load(void) {
	m_anotherHand.Work(this, 0, 0, false, 0);

	data_00b79b48 = 1;
	data_00b79b4d = true;
}

// realmyroomtask.cpp:228
void CRealMyRoomTask::Init(const char *backGround) {
	if (m_pLoadingFresh) { delete m_pLoadingFresh; m_pLoadingFresh = 0; }

	g_pFresh->GetDesktop()->Init(g_view->GetWidth(), g_view->GetHeight());

	g_pFresh->GetDesktop()->SetWallPaper(backGround, true);

	SetWhisper(false);
}

// realmyroomtask.cpp:398
void CRealMyRoomTask::RestorePreserved(void) {
	g_pFresh->GetDesktop()->SetViewFocus(true);

	SetGameType(2);

	g_pFresh->GetDesktop()->Init(g_view->GetWidth(), g_view->GetHeight());
	g_pFresh->GetDesktop()->SetWallPaper(NULL, true);

	CTask::RestorePreserved();
}

// realmyroomtask.cpp:412
void CRealMyRoomTask::Destroy(void) {
	CTask::Destroy();

	if (!((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false))
		delete CQuitWindow::Instance();
}

// realmyroomtask.cpp:422
void CRealMyRoomTask::Display(void) {
	CTask::Display();

	if (g_pFresh->GetDesktop()) {
		g_pFresh->GetDesktop()->OnDisplay(true, false, true);
	}

	g_pFresh->OnDisplay(true);
}

// realmyroomtask.cpp:434
void CRealMyRoomTask::AddAvatarBurden(const CRealMyRoom_AvatarBurden &burden) {
	EnterCriticalSection(&m_burdenLock);
	m_addedBurdens.push_back(burden);
	LeaveCriticalSection(&m_burdenLock);
}

// realmyroomtask.cpp:442
void CRealMyRoomTask::RemoveAvatarBurden(const CRealMyRoom_AvatarBurden &burden) {
	if (m_bDestroying) {
		return;
	}

	EnterCriticalSection(&m_burdenLock);
	m_removedBurdens.push_back(burden);
	LeaveCriticalSection(&m_burdenLock);
}

// realmyroomtask.cpp:455
void CRealMyRoomTask::DeleteBurdenPet(std::list<CRealMyRoom_AvatarBurden> &burdens, std::list<CRealMyRoom_AvatarBurden> &pending) {
	std::list<CRealMyRoom_AvatarBurden>::iterator it;

	EnterCriticalSection(&m_burdenLock);
	for (it = pending.begin(); it != pending.end(); ++it) {
		burdens.push_back(*it);
	}
	pending.clear();
	LeaveCriticalSection(&m_burdenLock);

	for (it = burdens.begin(); it != burdens.end(); ) {
		if (it->m_pPetFrame) {
			delete it->m_pPetFrame;
		}
		it = burdens.erase(it);
	}
}
