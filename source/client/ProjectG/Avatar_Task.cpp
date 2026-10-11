// Original: d:\build\custom\temp\code\client\projectg\avatar_task.cpp
#include "TaskManager.h"
#include "avatar_avatar.h"
#include "avatar_task.h"
#include "ChatMsg.h"
#include "ClientSetting.h"
#include "QuitWindow.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frdesktop.h"
#include "frwndmanager.h"
#include "wview.h"
#include "../shared/Localize.h"
#include "partsinfo.h"
#include "SharedDoc.h"
#include "fx.h"
#include "actor.h"
#include "mousecursor.h"
#include "InputManager.h"
#include "frgaugebar.h"
#include "scenemanager.h"
#include "Packet.h"
#include "UccLibrary.h"
#include "../shared/s5/specialprize/specialprizeitem.h"
#include <algorithm>

// The class and actor names of the task's last actor (two adjacent pointers
// of unnamed data).
extern const char *data_00a8438c;
extern const char *data_00a84390;

// The loading parameters CTask::Load hands to the loading thread (layout in
// taskmanager.cpp, where it is defined; its header is not known yet).
struct sLoadParam {
	WList<sActor *> *m_pActorList; // 0x0
	int m_actorCount; // 0x4
	int m_progress;   // 0x8
	bool m_bInited;   // 0xC
	bool m_bLoaded;   // 0xD
};
extern sLoadParam g_LoadParam;

// Type of the packet being handled (set by every task's OnPacket).
extern int g_iRecvPacketType;

// Set once the avatar loading thread loaded the player's own avatar.
extern bool g_SALoadingState;

// avatar_avatar.h:53
bool ntAvatarBurden::operator<(const ntAvatarBurden &other) {
	return m_key < other.m_key;
}

// avatar_avatar.h:58 (a burden whose pet frame is the -1 sentinel never matches)
bool ntAvatarBurden::operator==(const ntAvatarBurden &other) {
	return m_key == other.m_key && m_pPetFrame != (CPetFrame *)-1 && other.m_pPetFrame != (CPetFrame *)-1;
}

// clientsetting.h:409
void COption::gSetAvatarNewBie(int newBie) {
	m_option.m_avatarNewBie = newBie;
}

// taskmanager.h:102
void CTask::OnInitFinished(void) {
}

// taskmanager.h:107
bool CTask::IsRecieveWhisper(void) {
	return m_bRecieveWhisper;
}

// avatar_task.cpp:38 (run-time type and factory registration of ntAvatarChatTask)
const WRTTI ntAvatarChatTask::m_RTTI("ntAvatarChatTask", &CTask::m_RTTI);

IObject* ntAvatarChatTaskMakeInstance(void) {
	return new ntAvatarChatTask;
}

struct __sntAvatarChatTask {
	__sntAvatarChatTask(void) {
		ObjectFactory().AddObjectFunctor(ntAvatarChatTaskMakeInstance, "ntAvatarChatTask");
	}
};
static __sntAvatarChatTask s_registerntAvatarChatTask;

// avatar_task.cpp:41
ntAvatarChatTask::ntAvatarChatTask(void) {
	m_unknown38 = false;
	m_destroying = false;
	InitializeCriticalSection(&m_burdenLock);
}

// avatar_task.cpp:52
ntAvatarChatTask::~ntAvatarChatTask(void) {
	COption::Instance()->gSetAvatarNewBie(0);

	COption::Instance()->vSetMinimumScreenSize(0, 800, 600);

	m_destroying = true;
	m_anotherHand.WaitUntilWorkDone(9999);
	m_anotherHand.BreakWorkingThread();

	DeleteCriticalSection(&m_burdenLock);

	delete ntAvatarManager::Instance();
}

// avatar_task.cpp:68
void ntAvatarChatTask::PreLoadInit(void) {
	sActor *pActor = m_actorList.Start();
	while (pActor) {

		pActor->m_pActor->OnPreLoadInit();
		pActor = m_actorList.Next();
	}

	IActor::m_defaultSkipLayer = 0;
	IActor::m_pLayer = 0;
	SetLoadingBackGround("loading_background.jpg");

	CChatMsg::Instance()->ResetUiPosition();

	new CQuitWindow;

	SetGameType(2);

	pActor = m_actorList.Start();
	while (pActor) {

		pActor->m_pActor->OnLoad();
		pActor = m_actorList.Next();
	}

	pActor = m_actorList.Start();
	while (pActor) {

		pActor->m_pActor->OnInit();
		pActor = m_actorList.Next();
	}

	if (!IsInited()) {
		// TODO: m_stateFlags |= 2; (CTask::m_stateFlags 0x1F0 is private)
		OnInitFinished();
	}

	g_LoadParam.m_actorCount = 1;
	g_LoadParam.m_progress = 0;

	if (m_tipDlgState) {
		m_tipDlgState->SetRange(0, 100, 0);
	}

	m_pLoadingFresh = new Fresh;
	m_tipDlgState = NULL;

	m_pLoadingFresh->Init("loading.xml", false,
		m_loadingBackGround != "" ?
		m_loadingBackGround.c_str()
		:
		NULL);

	m_pLoadingFresh->OpenLayout("LOADING", NULL, true);

	// TODO: CTask::m_unknown59 (0x59) and m_pTipForm (0x60) are private:
	// if (m_unknown59) {
	//	m_pTipForm = LoadTipText(0, 0);
	//	if (m_pTipForm) {
	//		m_tipDlgState = DYNAMIC_CAST(FrGaugeBar, m_pTipForm->FindChildByName("progress"));
	//		if (m_tipDlgState) {
	//			m_tipDlgState->SetBarSpeed(200.0f);
	//		}
	//	}
	// }

	if (CMouseCursor::Instance()) {
		CMouseCursor::Instance()->SetActive(true);
		CMouseCursor::Instance()->SetMode(2);
	}
}

// avatar_task.cpp:143
void ntAvatarChatTask::Load(void) {
	m_anotherHand.Work(this, 0, 0, false, 0);
}

// avatar_task.cpp:152
void ntAvatarChatTask::Init(const char *backGround) {
	if (m_pLoadingFresh) { delete m_pLoadingFresh; m_pLoadingFresh = 0; }

	g_pFresh->GetDesktop()->Init(g_view->GetWidth(), g_view->GetHeight());

	g_pFresh->GetDesktop()->SetWallPaper(backGround, true);

	CChatMsg::Instance()->SetActive(true, true, true);
}

// avatar_task.cpp:163
void ntAvatarChatTask::AddAvatarBurden(const ntAvatarBurden &burden) {
	EnterCriticalSection(&m_burdenLock);
	m_addList.push_back(burden);
	LeaveCriticalSection(&m_burdenLock);
}

// avatar_task.cpp:171
void ntAvatarChatTask::RemoveAvatarBurden(const ntAvatarBurden &burden) {
	if (m_destroying) {
		return;
	}

	EnterCriticalSection(&m_burdenLock);
	m_removeList.push_back(burden);
	LeaveCriticalSection(&m_burdenLock);
}

// avatar_task.cpp:184
void ntAvatarChatTask::DeleteBurdenPet(std::list<ntAvatarBurden> &burdens, std::list<ntAvatarBurden> &pending) {
	EnterCriticalSection(&m_burdenLock);
	for (std::list<ntAvatarBurden>::iterator pendingIt = pending.begin(); pendingIt != pending.end(); ++pendingIt) {
		burdens.push_back(*pendingIt);
	}
	pending.clear();
	LeaveCriticalSection(&m_burdenLock);

	std::list<ntAvatarBurden>::iterator it = burdens.begin();
	while (it != burdens.end()) {
		CPetFrame *pPetFrame = it->m_pPetFrame;
		if (pPetFrame != NULL && pPetFrame != (CPetFrame *)-1) {
			delete pPetFrame;
		}
		it = burdens.erase(it);
	}
}

// avatar_task.cpp:207
void ntAvatarChatTask::KeepUniqueCompleteList(const ntAvatarBurden &burden) {
	std::list<ntAvatarBurden>::iterator it = std::find(m_completeList.begin(), m_completeList.end(), burden);
	if (it != m_completeList.end()) {
		m_removeList.push_back(*it);
		m_completeList.erase(it);
	}
}

// avatar_task.cpp:222
void ntAvatarChatTask::OnAnotherHand(int nWork, void *pData) {
	std::list<ntAvatarBurden> burdens;

	while (!m_destroying) {
		EnterCriticalSection(&m_burdenLock);
		for (std::list<ntAvatarBurden>::iterator addIt = m_addList.begin(); addIt != m_addList.end(); ++addIt) {
			burdens.push_back(*addIt);
		}
		m_addList.clear();
		LeaveCriticalSection(&m_burdenLock);

		if (burdens.size() > 1) {
			burdens.reverse();
			burdens.sort();
			burdens.unique();
		}

		// Burdens whose pet frame is the -1 sentinel are only cancellations.
		std::list<ntAvatarBurden>::iterator it = burdens.begin();
		while (!m_destroying && it != burdens.end()) {
			if (it->m_pPetFrame == (CPetFrame *)-1) {
				it->m_pPetFrame = NULL;
				EnterCriticalSection(&m_burdenLock);
				KeepUniqueCompleteList(*it);
				LeaveCriticalSection(&m_burdenLock);

				unsigned long key = it->m_key;
				it = burdens.erase(it);
				if (it->m_key == key) {
					it = burdens.erase(it);
				}
			} else {
				++it;
			}
		}

		// The player's own avatar first.
		// TODO: the original tests ntAvatarManager::m_pMyAvatar (0xA4, private)
		// here (!= 0) and below (== FindAvatarByGuid(...)).
		if (ntAvatarManager::Instance()) {
			it = burdens.begin();
			while (!m_destroying && it != burdens.end()) {
				if (ntAvatarManager::Instance()->FindAvatarByGuid(it->m_key) == NULL) {
					if (it->Load()) {
						EnterCriticalSection(&m_burdenLock);

						KeepUniqueCompleteList(*it);
						m_completeList.push_back(*it);
						LeaveCriticalSection(&m_burdenLock);
					}

					it = burdens.erase(it);

					g_LoadParam.m_progress = 1;
					g_LoadParam.m_bLoaded = true;
					if (!g_SALoadingState) {
						g_SALoadingState = true;
					}
				} else {
					++it;
				}
			}
		}

		it = burdens.begin();
		while (!m_destroying && it != burdens.end()) {
			if (it->Load()) {
				EnterCriticalSection(&m_burdenLock);

				KeepUniqueCompleteList(*it);
				m_completeList.push_back(*it);
				LeaveCriticalSection(&m_burdenLock);

				if (!m_destroying) {
					Sleep(15);
				}
			}

			it = burdens.erase(it);
		}

		DeleteBurdenPet(burdens, m_removeList);

		if (!m_destroying) {
			Sleep(500);
		}
	}

	DeleteBurdenPet(burdens, m_removeList);
	DeleteBurdenPet(burdens, m_completeList);
}

// avatar_task.cpp:366
void ntAvatarChatTask::Register(void) {
	SetMainActor(AddActor("ntAvatarChatMain", "AvatarChat", 0, ""));

	AddActor("CNPC", "NPC", 0, "");

	if (IsLocalContent((localContentType_t)0x77)) {
		AddActor("CPrivateTrade", "PrivateTrade", 0, "");
	}

	AddActor(data_00a8438c, data_00a84390, 0, GetRTTI()->GetName());
}

// avatar_task.cpp:393
void ntAvatarChatTask::PreserveBack(const char *name) {
	CTask::PreserveBack(name);

	WSingleton<CChatMsg>::Instance()->Close(true, true);
}

// avatar_task.cpp:400
void ntAvatarChatTask::RestorePreserved(void) {
	if (CSharedDoc::Instance()->m_roomType == 8) {
		if (CFx::Instance()) {
			CFx::Instance()->SetActive(true);
		}
	}

	SetGameType(2);

	CTask::RestorePreserved();

	FrDesktop *pDesktop = g_pFresh->GetDesktop();
	pDesktop->SetViewFocus(true);
	pDesktop->Init(g_view->GetWidth(), g_view->GetHeight());
	pDesktop->SetWallPaper(0, true);

	CChatMsg::Instance()->ResetUiPosition();
}

// avatar_task.cpp:421
void ntAvatarChatTask::Destroy(void) {
	CTask::Destroy();

	delete CQuitWindow::Instance();
}

// avatar_task.cpp:428
void ntAvatarChatTask::ShowQuitDialog(void) {
	if (CChatMsg::Instance()->IsOpened()) {
		CChatMsg::Instance()->Close(true, true);
	} else {
		if (CQuitWindow::Instance()) {
			CQuitWindow::Instance()->Process();
		}
	}
}

// avatar_task.cpp:441
void ntAvatarChatTask::Process(float dt) {
	if (g_input->GetDown("ESCAPE", true) == 1 && !g_pFresh->GetManager()->HasEscKeyWindow()) {
		if (CSharedDoc::Instance()->GetStateTrade()) {
			// "You cannot leave the chat room while using the shop."
			CChatMsg::Instance()->AddChatMsg(std::string("\xbb\xf3\xc1\xa1 \xc0\xcc\xbf\xeb\xc1\xdf\xbf\xa1\xb4\xc2 \xb4\xeb\xc8\xad\xb9\xe6\xbf\xa1\xbc\xad \xb3\xaa\xb0\xa5\xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9."), 0xFFFF00, true, false);
		} else {
			ShowQuitDialog();
		}
	}

	// Hand the burdens the loading thread completed to their avatars.
	std::list<ntAvatarBurden> completed;

	EnterCriticalSection(&m_burdenLock);
	for (std::list<ntAvatarBurden>::iterator completeIt = m_completeList.begin(); completeIt != m_completeList.end(); ++completeIt) {
		completed.push_back(*completeIt);
	}
	m_completeList.clear();
	LeaveCriticalSection(&m_burdenLock);

	for (std::list<ntAvatarBurden>::iterator it = completed.begin(); it != completed.end(); ++it) {
		ntAvatarBurden &burden = *it;

		ntAvatar *pAvatar = ntAvatarManager::Instance()->FindAvatarByGuid(burden.m_key);
		if (pAvatar) {
			// TODO (avatar_task.cpp:470-484): needs `friend class
			// ntAvatarChatTask;` in ntAvatar and its burden members
			// (avatar_avatar.h, owned by another worker):
			// The avatar's previous pet frame goes back to the loading thread.
			//	if (pAvatar->m_pPetFrame) {
			//		RemoveAvatarBurden(ntAvatarBurden(pAvatar->m_uid, pAvatar->m_guid, pAvatar->m_characterInfo, pAvatar->m_pPetFrame));
			//
			//		pAvatar->m_pPetFrame = NULL;
			//	}
			//
			//	pAvatar->m_burden = burden;          // 0x144 (ntAvatarBurden, 0x294 bytes)
			//	pAvatar->m_burdenReady = true;       // 0x3D8
		} else {
			RemoveAvatarBurden(burden);
		}
	}

	CTask::Process(dt);

	g_pFresh->OnProcess(dt);

	CChatMsg::Instance()->ProcessWindow(dt);

	FrWnd *pKeyFocused = g_pFresh->GetManager()->GetKeyFocused();
	if (pKeyFocused && pKeyFocused->IsVisible()) {
		g_pFresh->GetManager()->SetExclusiveKey(true);
	} else {
		g_pFresh->GetManager()->SetExclusiveKey(false);
	}
}

// avatar_task.cpp:506
void ntAvatarChatTask::Display(void) {
	CTask::Display();

	if (g_pFresh->GetDesktop()) {
		g_pFresh->GetDesktop()->OnDisplay(true, false, true);
	}

	if (!CChatMsg::Instance()->HasBeenPrinted()) {
		CChatMsg::Instance()->PrintInScreen();
		CChatMsg::Instance()->DrawChatWindow();
		CChatMsg::Instance()->PrintInChatWindow();
	}

	g_pFresh->OnDisplay(true);
}

// avatar_task.cpp:523
// FIRST PASS: only the switch skeleton and the small cases are written; the
// other cases carry the line range and calls of the original.
int ntAvatarChatTask::OnPacket(WReceivedPacket &packet) {
	unsigned short type = packet.Decode2();

	g_iRecvPacketType = type;

	switch (g_iRecvPacketType) {
	case 0x46: {
		static sSlotInfo slotInfo;
		static sCharacterInfo charInfo;

		unsigned char mode = packet.Decode1();
		unsigned short check = packet.Decode2();

		if (check == 0xFFFF) {
			// The original's list is a std::list<sSlotInfo> (SharedDoc.h
			// types it with sRoomPlayer, the same 0x152-byte layout).
			std::list<sSlotInfo> &players = (std::list<sSlotInfo> &)WSingleton<CSharedDoc>::Instance()->m_roomPlayerList;

			switch (mode) {
			case 0:
			case 5: {
				if (mode == 0) {
					players.clear();
				}
				int count = packet.Decode1();

				for (int i = 0; i < count; i++) {
					packet.DecodeBuffer(&slotInfo, 0x152);

					if (!IsMassGame(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType)) {
						packet.DecodeBuffer(&charInfo, 0x1BC);
						if (IsLocalContent((localContentType_t)0x57)) {
							RequestUccInfo(charInfo);
						}
					}

					players.push_back(slotInfo);

					GetActor("AvatarChat") << MsgObject(0, 0xE4, (intptr_t)&slotInfo, (intptr_t)&charInfo, 0, 0, 0);
				}
				break;
			}

			case 7: {
				packet.Decode1();

				packet.DecodeBuffer(&slotInfo, 0x152);

				if (!IsMassGame(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType)) {
					packet.DecodeBuffer(&charInfo, 0x1BC);
					if (IsLocalContent((localContentType_t)0x57)) {
						RequestUccInfo(charInfo);
					}
				}

				std::list<sSlotInfo>::iterator it = std::find(players.begin(), players.end(), slotInfo.m_uid);
				if (it == players.end()) {
					players.push_back(slotInfo);

					GetActor("AvatarChat") << MsgObject(0, 0xE4, (intptr_t)&slotInfo, (intptr_t)&charInfo, 0, 0, 0);
				}
				break;
			}

			case 1: {
				packet.DecodeBuffer(&slotInfo, 0x152);

				if (!IsMassGame(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType)) {
					packet.DecodeBuffer(&charInfo, 0x1BC);
					if (IsLocalContent((localContentType_t)0x57)) {
						RequestUccInfo(charInfo);
					}
				}

				std::list<sSlotInfo>::iterator it = std::find(players.begin(), players.end(), slotInfo.m_uid);
				if (it == players.end()) {
					players.push_back(slotInfo);

					GetActor("AvatarChat") << MsgObject(0, 0xE4, (intptr_t)&slotInfo, (intptr_t)&charInfo, 0, 0, 0);
				}
				break;
			}

			case 2: {
				unsigned long uid = packet.Decode4();

				std::list<sSlotInfo>::iterator it = std::find(players.begin(), players.end(), uid);

				if (it != players.end()) {
					players.erase(it);

					GetActor("AvatarChat") << MsgObject(0, 0xE5, uid, 0, 0, 0, 0);
				}
				break;
			}

			case 3: {
				unsigned long uid = packet.Decode4();

				packet.DecodeBuffer(&slotInfo, 0x152);

				std::list<sSlotInfo>::iterator it = std::find(players.begin(), players.end(), uid);
				if (it != players.end()) {
					GetActor("AvatarChat") << MsgObject(0, 0xE6, uid, (intptr_t)&slotInfo, 0, 0, 0);
				}
				break;
			}
			}

			WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_userCount = (unsigned char)players.size();
		}
		break;
	}

	case 0x7A: {
		unsigned long guid = packet.Decode4();
		unsigned short value = packet.Decode2();
		GetActor("AvatarChat") << MsgObject(0, 0x5F, guid, value, 0, 0, 0);
		break;
	}

	case 0x7F: {
		std::string nickName;
		packet.DecodeStr(nickName);
		// "<nick>: you are about to force this player out.\nDo you agree?"
		nickName += "\xb4\xd4\xc0\xbb \xb0\xad\xc1\xa6 \xc5\xf0\xc0\xe5\xbd\xc3\xc5\xb0\xb7\xc1 \xc7\xd5\xb4\xcf\xb4\xd9.\n\xc2\xf9\xbc\xba\xc7\xcf\xbd\xca\xb4\xcf\xb1\xee?";
		GetActor("AvatarChat") << MsgObject(0, 0x31, (intptr_t)nickName.c_str(), packet.Decode4(), 0, 0, 0);
		break;
	}

	case 0x3F: {
		std::string text;
		packet.DecodeStr(text);
		GetActor("avatarChatMain") << MsgObject(0, 3, (intptr_t)text.c_str(), 0xFFFF7878, 0, 0, 0);
		break;
	}

	case 0x3E:
		// TODO (705-863): inner switch at line 725 (11 entries).
		break;

	case 0x82:
		// TODO (870-929)
		break;

	case 0xC2:
		// TODO (935-987): inner switch at line 941 (6 entries).
		break;

	case 0x9C: {
		unsigned char weather = packet.Decode1();
		packet.Decode1();
		packet.Decode1();

		CSceneManager::Instance()->SetWeather(weather, false);
		break;
	}

	case 0x8C: {
		unsigned long guid = packet.Decode4();
		unsigned char value = packet.Decode1();
		GetActor("AvatarChat") << MsgObject(0, 0x3D, guid, value, 0, 0, 0);
		break;
	}

	case 0x76: {
		unsigned long guid = packet.Decode4();
		unsigned char value = packet.Decode1();

		GetActor("AvatarChat") << MsgObject(0, 0x29, guid, value, 0, 0, 0);
		break;
	}

	case 0x92:
		// TODO (1024-1047): CSharedDoc+0x5040 pointer, its byte 0x17D = (Decode1() == 1).
		break;

	case 0x258:
		break;

	case 0x25D: {
		ntAvatar *pAvatar = ntAvatarManager::Instance()->FindAvatarByGuid(packet.Decode4());
		if (pAvatar) {
			for (int i = 0; i < 5; i++) {
				SpecialPrizeItem &item = pAvatar->GetMySpItem((eSPAVILITYTYPE)i);

				float value = 0;
				packet.DecodeBuffer(&value, 4);
				item.setvalue(value);
			}
		}
		break;
	}

	default:
		return OnPacketCommon(packet);
	}

	return 1;
}
