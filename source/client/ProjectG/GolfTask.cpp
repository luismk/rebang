// Original: d:\build\custom\temp\code\client\projectg\golftask.cpp
// The original holds the out-of-line copy of CMouseCursor::SetTempActive
// (mousecursor.h) in this object.
#define GOLFTASK_MOUSECURSOR_INLINE
// ... and of CTask::IsInited (taskmanager.h).
#define GOLFTASK_TASKMANAGER_INLINE

#include "GolfTask.h"

#include "CapturedBg.h"
#include "ChatMsg.h"
#include "ClientSetting.h"
#include "GolfDoc.h"
#include "ObjectFactory.h"
#include "Packet.h"
#include "QuitWindow.h"
#include "SharedDoc.h"
#include "actor.h"
#include "commonutil.h"
#include "fresh.h"
#include "frdesktop.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "mousecursor.h"
#include "wview.h"

// shareddoc.inl (the overload without a game type, 0x40fce0; not declared
// in SharedDoc.h yet).
int IsMassGame(void);

// File-level actor class and name strings (data 0xa7e3b0..0xa7e3c0; their
// header of origin is not recovered yet).
static const char* s_guildActorClass = "CGuildActor";
static const char* s_guildActorName = "GuildActor";
static const char* s_gatewayActorClass = "CGatewayActor";
static const char* s_gatewayActorName = "Gateway";

// clientsetting.h:422
int COption::gGetCutinFlag(void) {
	return m_option.m_cutinFlag;
}

// golftask.cpp:75 (run-time type and factory registration of CGolfTask)
const WRTTI CGolfTask::m_RTTI("CGolfTask", &CTask::m_RTTI);

IObject* CGolfTaskMakeInstance(void) {
	return new CGolfTask;
}

struct __sCGolfTask {
	__sCGolfTask(void) {
		ObjectFactory().AddObjectFunctor(CGolfTaskMakeInstance, "CGolfTask");
	}
};
static __sCGolfTask s_registerCGolfTask;

// golftask.cpp:79
CGolfTask::CGolfTask(void) {
	m_unknown38 = false;
	IActor::m_defaultSkipLayer = 0x622021;
	m_unknown208 = -10.0f;
}

// golftask.cpp:99
void CGolfTask::Init(const char *backGround) {
	CTask::Init(0);
}

// golftask.cpp:104
void CGolfTask::Register(void) {
	SetMainActor(AddActor("CGolfRule", "GolfRule", 0x20021, ""));

	AddActor("CClock", "Clock", 0x20, "");
	AddActor("CGallery", "Gallery", 0, "");
	AddActor("CCamera", "Camera", 0x20, "");

	AddActor("CGolfBg", "GolfBg", 0x22001, "");

	AddActor("CGolfPlayer", "Player", 0x22000, "");
	AddActor("CGolfCaddie", "Caddie", 0x2000, "");
	AddActor("CMascot", "Mascot", 0x2000, "");
	AddActor("CNPC", "NPC", 0x2001, "");

	AddActor("CGrid", "Grid", 0x20, "");
	AddActor("CSpot", "Spot", 0x20, "");
	AddActor("CBall", "Ball", 0x2000, "");

	AddActor("CGuideLine", "GuideLine", 0x20, "");
	AddActor("CTrajectory", "Trajectory", 0, "");
	AddActor("CParticleEffect", "Effect", 0x20001, "");
	AddActor("CFx", "Fx", 0x20001, "");

	AddActor("CAmbientSound", "Ambient", 1, "");

	AddActor("CBubble", "Bubble", 0x20, "");
	AddActor("CLetter", "Letter", 0x20020, "");
	AddActor("CScreen", "Screen", 0x20, "");
	AddActor("CItemWindow", "Item", 0x2000, "");
	AddActor("CSpin", "Spin", 0, "");
	AddActor("CLogo", "Logo", 1, "");
	AddActor("CScore", "Score", 0x222020, "");
	AddActor("CGuideBook", "GuideBook", 0x22020, "");

	AddActor(s_guildActorClass, s_guildActorName, 0, "");

	AddActor(s_gatewayActorClass, s_gatewayActorName, 0, GetRTTI()->GetName());

	CTask *pPreservedTask = AfxGetPreservedTask();
	if (pPreservedTask) {
		IActor *pTutorial = pPreservedTask->GetActor("TutorialMain");
		if (pTutorial) {
			const char *className = 0;
			const char *actorName = 0;
			pTutorial << MsgObject(0, 0x14E, (intptr_t)&className, (intptr_t)&actorName, 0, 0, 0);
			AddActor(className, actorName, 0x20, "");
		}
	} else {
		AddActor("CCutin", "Cutin", 0, "");
	}

	AddActor("ShotPreview", "ShotPreview", 0, "");

	if (DetermineWhetherToAddActor_GroundItemMan())
		AddActor("GroundItemMan", "GroundItemMan", 0, "");
	AddActor("CMagicCarpetHelper", "MagicCarpetHelper", 0, "");
}

// golftask.cpp:213
void CGolfTask::Load(void) {
	IActor::m_pLayer = &GOLFDOC()->GameMode();

	new CQuitWindow;

	SetLoadingBackGround("loading_background.jpg");

	g_pFresh->GetDesktop()->SetViewFocus(true);

	CChatMsg::Instance()->ResetUiPosition();

	CTask::Load();
}

// golftask.cpp:228
void CGolfTask::Destroy(void) {
	CTask::Destroy();

	g_pFresh->GetDesktop()->SetWallPaper("gameroom_background.jpg", true);
	g_pFresh->GetDesktop()->SetBgColor(0xFFFFFFFF);
	CMouseCursor::Instance()->SetTempActive(false);

	if (CQuitWindow::Instance())
		delete CQuitWindow::Instance();
}

// golftask.cpp:387
void CGolfTask::Display(void) {
	CTask::Display();

	if (GOLFDOC()->GameMode() & 0x400000) {
		GetActor("Score") << MsgObject(0, 0x12F, 3, 0, 0, 0, 0);
		GetActor("GolfRule") << MsgObject(0, 0x78, 2, 0, 0, 0, 0);
	}

	if (g_pFresh->GetDesktop()) {
		g_pFresh->GetDesktop()->OnDisplay(true, false, true);
	}

	if (!CChatMsg::Instance()->HasBeenPrinted()) {
		CChatMsg::Instance()->PrintInScreen();
		CChatMsg::Instance()->DrawChatWindow();
		CChatMsg::Instance()->PrintInChatWindow();
	}

	if (!CCapturedBg::Instance()->IsCaptured() || GOLFDOC()->m_gameState != 15) {
		g_pFresh->OnDisplay(false);
	}

	if (GOLFDOC()->GameMode() & 0x10) {
		if (CCapturedBg::Instance()) {
			CCapturedBg::Instance()->Capture();
		}
	}

	if ((GOLFDOC()->GameMode() & 1) && OnlinePlay() && !IsMassGame()) {
		DrawLoadingInfo(g_pFresh);
	}
}

// golftask.cpp:433
void CGolfTask::OnDisplayWhileLoading(void) {
	CTask::OnDisplayWhileLoading();
	OnlinePlay();
}

// golftask.cpp:444
void CGolfTask::UpdateLoadingInfo(unsigned long uid, unsigned char progress) {
	std::list<sRoomPlayer>::iterator it;
	for (it = CSharedDoc::Instance()->m_roomPlayerList.begin(); it != CSharedDoc::Instance()->m_roomPlayerList.end(); ++it) { if (it->m_uid == uid) break; }
	if (it != CSharedDoc::Instance()->m_roomPlayerList.end()) {
		m_loadingInfo[it->m_uid] = progress;
	}
}

// golftask.cpp:451
void CGolfTask::DrawLoadingInfo(Fresh *pFresh) {
	FrGraphicInterface *pGDI = pFresh->GetManager()->GetGDI();
	if (pGDI == 0) {
		return;
	}

	int offset = 0;
	float y = g_view->GetHeight() - 100.0f;

	for (std::list<sRoomPlayer>::iterator it = CSharedDoc::Instance()->m_roomPlayerList.begin(); it != CSharedDoc::Instance()->m_roomPlayerList.end(); ++it) {
		sRoomPlayer &player = *it;
		if (player.m_uid == CSharedDoc::Instance()->GetPangyaUserInfo().guid) {
			continue;
		}

		unsigned char progress = m_loadingInfo[player.m_uid];

		pGDI->SetTextColor(0xFFFFFFFF, 0xFF000000);
		pGDI->SetTextStyle(2);

		char *text;
		if (progress == 10) {
			text = MakeStr("%s\xb4\xd4\xc0\xba \xb7\xce\xb5\xf9 \xbf\xcf\xb7\xe1 \xc0\xd4\xb4\xcf\xb4\xd9.", player.m_nickname);
		} else {
			text = MakeStr("%s\xb4\xd4\xc0\xba \xc7\xf6\xc0\xe7 %d%% \xb7\xce\xb5\xf9 \xc1\xdf \xc0\xd4\xb4\xcf\xb4\xd9.", player.m_nickname, progress * 10);
		}

		pFresh->GetManager()->PrintText(WPoint(20.0f, y + offset), 0, text, -1.0f, 0xFFFFFFFF);
		offset += 20;
	}
}

// golftask.cpp:492
void CGolfTask::OnInitFinished(void) {
	if (OnlinePlay()) {
		WSendPacket packet((enumClientPacket)0x11);
		packet.Send(SEND_TO_DEFAULT);
	}

	if (CChatMsg::Instance()) {
		CChatMsg::Instance()->Close(true, true);
		CChatMsg::Instance()->EnableCaretMove(true);
	}

	GetActor("GolfRule") << MsgObject(0, 0x29, 0, 0, 0, 0, 0);

	if (GalleryMode()) {
		switch (CSharedDoc::Instance()->m_roomType) {
		case 4:
		case 5:
		case 6:
		case 9:
		case 10: {
			WSendPacket packet((enumClientPacket)0x3F);
			packet.Encode4(CSharedDoc::Instance()->m_roomPlayers[0].m_guid);
			packet.Send(SEND_TO_DEFAULT);
			break;
		}
		}
	}
}

// golftask.cpp:2699
bool CGolfTask::DetermineWhetherToAddActor_GroundItemMan(void) {
	bool bAdd = false;
	switch (CSharedDoc::Instance()->m_roomType) {
	case 0:
	case 1:
	case 3:
	case 4:
	case 5:
	case 6:
		if (CSharedDoc::Instance()->m_playingMode != 2 && CSharedDoc::Instance()->m_gameMode != 1) {
			bAdd = true;
		}
		break;
	}
	return bAdd;
}
