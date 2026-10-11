// Original: d:\build\custom\temp\code\client\projectg\tutorialtask.cpp
#include "TutorialTask.h"

#include "ObjectFactory.h"
#include "actor.h"
#include "fresh.h"
#include "frdesktop.h"
#include "wview.h"
#include "ClientSetting.h"
#include "mousecursor.h"
#include "SharedDoc.h"
#include "Packet.h"

extern int g_iRecvPacketType;

// tutorialtask.cpp:28 (run-time type and factory registration of CTutorialTask)
const WRTTI CTutorialTask::m_RTTI("CTutorialTask", &CTask::m_RTTI);

IObject* CTutorialTaskMakeInstance(void) {
	return new CTutorialTask;
}

struct __sCTutorialTask {
	__sCTutorialTask(void) {
		ObjectFactory().AddObjectFunctor(CTutorialTaskMakeInstance, "CTutorialTask");
	}
};
static __sCTutorialTask s_registerCTutorialTask;

// tutorialtask.cpp:31
CTutorialTask::CTutorialTask(void) {
	IActor::m_defaultSkipLayer = 0;
	IActor::m_pLayer = 0;
}

// tutorialtask.cpp:44
CTutorialTask::~CTutorialTask(void) {
	CSharedDoc::Instance()->m_gameMode = 0;
}

// tutorialtask.cpp:60
void CTutorialTask::Init(const char *backGround) {
	CTask::Init("tutorial_background.jpg");
	SetWhisper(false);
}

// tutorialtask.cpp:70
void CTutorialTask::Register(void) {
	SetMainActor(AddActor("CTutorialMain", "TutorialMain", 0, ""));
}

// tutorialtask.cpp:75
void CTutorialTask::Load(void) {
	SetProgressBar(false);
	SetTip(false);
	CTask::Load();
}

// tutorialtask.cpp:87
void CTutorialTask::PreserveBack(const char *pNextTask) {
	CTask::PreserveBack(pNextTask);
}

// tutorialtask.cpp:92
void CTutorialTask::RestorePreserved(void) {
	CTask::RestorePreserved();

	g_pFresh->GetDesktop()->SetViewFocus(true);

	WSingleton<COption>::Instance()->vApplyLobbyScreenSize();
	WSingleton<CMouseCursor>::Instance()->SetActive(true);

	g_pFresh->GetDesktop()->Init(g_view->GetWidth(), g_view->GetHeight());
	g_pFresh->GetDesktop()->SetWallPaper("tutorial_background.jpg", true);
}

// tutorialtask.cpp:112
void CTutorialTask::Destroy(void) {
	CTask::Destroy();
}

// tutorialtask.cpp:117
void CTutorialTask::Process(float dt) {
	CTask::Process(dt);
	g_pFresh->OnProcess(dt);
}

// tutorialtask.cpp:123
void CTutorialTask::Display(void) {
	if (!CSharedDoc::Instance()->m_3DBackgroundOff) {
		CTask::Display();
	}
	g_pFresh->OnDisplay(CSharedDoc::Instance()->m_3DBackgroundOff != 0);
}

// tutorialtask.cpp:132
int CTutorialTask::OnPacket(WReceivedPacket &packet) {
	unsigned short type = packet.Decode2();

	g_iRecvPacketType = type;

	switch (g_iRecvPacketType) {
	case 0x11B: {
		unsigned char index = packet.Decode1();
		unsigned char value = packet.Decode1();
		if (index < 3) {
			unsigned long data = packet.Decode4();
			CSharedDoc::Instance()->m_tutorialData[index] = data;
		} else {
			packet.DecodeBuffer(CSharedDoc::Instance()->m_tutorialData, 12);
		}
		CTaskManager::Instance()->PostMsg(NULL, "TutorialMain", 0x151, index, value, 0, 0);
		break;
	}
	case 0x11C: {
		unsigned char value = packet.Decode1();
		CTaskManager::Instance()->PostMsg(NULL, "TutorialMain", 0x153, value, 0, 0, 0);
		break;
	}
	default:
		return OnPacketCommon(packet);
	}
	return 1;
}
