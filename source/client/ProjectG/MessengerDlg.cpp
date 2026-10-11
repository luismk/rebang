// Original: d:\build\custom\temp\code\client\projectg\messengerdlg.cpp
#define MESSENGERDLG_SHAREDDOC_INLINE
#include "messengerdlg.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "AddFriendDlg.h"
#include "buddymanager.h"
#include "messengerchatdlg.h"
#include "NetworkSystem.h"
#include "NewsReader.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "user_info.h"
#include "NoteDlg.h"
#include "../shared/Localize.h"
#include "commonutil.h"
#include "GolfTask.h"
#include "MemoDlg.h"
#include "TaskManager.h"
#include "frwndmanager.h"
#include "GuildNoticeDlg.h"
#include "NetResourceManager.h"
#include "InputManager.h"
#include "wview.h"
#include "LobbyTask.h"
#include "ShopTask.h"
#include "RealMyRoomTask.h"
#include "FamilyTask.h"
#include "avatar_task.h"
#include "ChatMsg.h"
#include "SoundManager.h"
#include "winput.h"
#include <mmsystem.h>

// The mouse and IME input devices (publics ?g_mouse@@3PAVWInputDev@@A, ?g_ime@@3PAVWInputDev@@A).
extern WInputDev* g_mouse;
extern WInputDev* g_ime;

// messengerdlg.cpp:48
bool FriendCompare(const void* a, const void* b) {
	const sFriend* pA = (const sFriend*)a;
	const sFriend* pB = (const sFriend*)b;
	if (pA == NULL)
		return false;

	if (pB == NULL)
		return true;

	if (pA->uid == 0xFFFFFFFF || pB->uid == 0xFFFFFFFF)
		return false;

	if (((pA->flags >> 1) & 1) == 0 && (pB->flags & 2))
		return false;
	if (((pA->flags >> 1) & 1) == 1 && !(pB->flags & 2))
		return true;

	if (((pA->flags >> 4) & 1) == 0 && (pB->flags & 0x10))
		return true;
	if (((pA->flags >> 4) & 1) == 1 && !(pB->flags & 0x10))
		return false;

	if (((pA->flags >> 2) & 1) == 0 && (pB->flags & 4))
		return false;
	if (((pA->flags >> 2) & 1) == 1 && !(pB->flags & 4))
		return true;

	if (((pA->flags >> 2) & 1) == 0 && !(pB->flags & 4)) {
		if (((pA->flags >> 3) & 1) == 1 && !(pB->flags & 8))
			return true;

		if (((pA->flags >> 3) & 1) == 0 && (pB->flags & 8))
			return true;
	}

	return false;
}

// messengerdlg.cpp:94 (run-time type and factory registration of FrMessengerDlg)
const WRTTI FrMessengerDlg::m_RTTI("FrMessengerDlg", &FrForm::m_RTTI);

IObject* FrMessengerDlgMakeInstance(void) {
	return new FrMessengerDlg;
}

struct __sFrMessengerDlg {
	__sFrMessengerDlg(void) {
		ObjectFactory().AddObjectFunctor(FrMessengerDlgMakeInstance, "FrMessengerDlg");
	}
};
static __sFrMessengerDlg s_registerFrMessengerDlg;

// messengerdlg.cpp:96
const sFRESH_MSGMAP* FrMessengerDlg::GetMessageMap(void) const {
	return &FrMessengerDlg::_MsgMap;
}

// messengerdlg.cpp:160
FrMessengerDlg::FrMessengerDlg(void)
	: m_pRightMenu(0), m_pStateMenu(0), m_pMyInfo(0), m_pNoteDlg(0), m_pAddFriendDlg(0), m_pOptionDlg(0), m_pMemoDlg(0) {
	memset(m_groupRow, 0, sizeof(m_groupRow));

	m_groupRow[0].flags |= 2;
	m_groupRow[0].status = CMessengerInfo::Instance()->m_groupState[0];
	m_groupRow[0].uid = 0xFFFFFFFF;

	m_groupRow[1].flags &= ~2;
	m_groupRow[1].status = CMessengerInfo::Instance()->m_groupState[1];
	m_groupRow[1].uid = 0xFFFFFFFF;

	m_groupRow[2].listFlags &= ~2;
	m_groupRow[2].flags &= 0x81;
	m_groupRow[2].status = 5;
	m_groupRow[2].uid = 0xFFFFFFFF;

	m_pFriendList = 0;
	m_pGuildList = 0;
	m_pGuildNoticeBtn = 0;
	m_pStateBtn = 0;
	m_reserved168 = 0;
	m_list0Count = 0;
	m_list0OnCount = 0;
	m_list1Count = 0;
	m_list1OnCount = 0;

	if (IsLocalContent((localContentType_t)0x54))
		m_inviteEnabled = false;
}

// messengerdlg.cpp:230
bool FrMessengerDlg::Close(bool bDestroy) {
	std::list<sChatSlot>::iterator it;
	for (it = CMessengerInfo::Instance()->m_chatDlgList.begin(); it != CMessengerInfo::Instance()->m_chatDlgList.end(); ++it) {
		if (it->pChatDlg != NULL) {
			it->pChatDlg->FindNextTopFocus(false);
			break;
		}
	}

	FrForm::Close(true);

	return true;
}

// messengerdlg.cpp:262
void FrMessengerDlg::SetAddFriendNick(const char* nick, unsigned long uid) {
	if (m_pAddFriendDlg != NULL) {
		m_pAddFriendDlg->SetNick(nick);
		m_pAddFriendDlg->SetFindUID(uid);
	}
}

// messengerdlg.cpp:271
void FrMessengerDlg::SetTargetNick(const char* nick) {
	if (m_pAddFriendDlg != NULL) {
		m_pAddFriendDlg->SetTargetNick(nick);
	}
}

// messengerdlg.cpp:279
void FrMessengerDlg::OpenAddFriendNickDlg(void) {
	OnMessenger_FindLBtnDown();
}

// messengerdlg.cpp:293
bool FrMessengerDlg::OnLButtonDown(const WPoint& pt) {
	m_pRightMenu->SetVisible(false);

	m_pStateMenu->SetVisible(false);

	FrForm::OnLButtonDown(pt);

	return false;
}

// messengerdlg.cpp:314
void FrMessengerDlg::OnMessenger_MyInfoInit(int param) {
	m_pMyInfo = DYNAMIC_CAST(FrArea, param);
}

// messengerdlg.cpp:398
void FrMessengerDlg::OnMessenger_FriendListInit(int param) {
	m_pFriendList = DYNAMIC_CAST(FrListBox, param);
	if (m_pFriendList == NULL)
		return;

	m_pFriendList->UseRightButton(true);

	m_pDownBtn = g_pFresh->GetManager()->GetBitmap("MESSENGER", "down_btn_n");
	m_pUpBtn = g_pFresh->GetManager()->GetBitmap("MESSENGER", "up_btn_n");

	m_pSexIcon[0] = m_pFriendList->GetBitmap("i_male");
	m_pSexIcon[1] = m_pFriendList->GetBitmap("i_female");
	m_pSexIcon[2] = m_pFriendList->GetBitmap("i_male_02");
	m_pSexIcon[3] = m_pFriendList->GetBitmap("i_female_02");
	m_pSexIcon[4] = m_pFriendList->GetBitmap("i_male_03");
	m_pSexIcon[5] = m_pFriendList->GetBitmap("i_female_03");

	m_pStatusIcon[0] = g_pFresh->GetManager()->GetBitmap("MESSENGER", "play_icon");
	m_pStatusIcon[1] = g_pFresh->GetManager()->GetBitmap("MESSENGER", "dive_icon");
	m_pStatusIcon[2] = g_pFresh->GetManager()->GetBitmap("MESSENGER", "block_icon");
	m_pStatusIcon[3] = g_pFresh->GetManager()->GetBitmap("MESSENGER", "busy_icon");
	m_pStatusIcon[4] = NULL; m_selectUID = 0xFFFFFFFF; UpdateFriendList();
}

// messengerdlg.cpp:540
void FrMessengerDlg::OnMessenger_FriendListLBtnDown(void) {
	FindNextTopFocus(false);

	m_pRightMenu->SetVisible(false);
	m_pStateMenu->SetVisible(false);

	FrListItem* pItem = m_pFriendList->GetItemUnderCursor();
	if (pItem == NULL)
		return;

	sFriend* pFriend = (sFriend*)pItem->pData;
	if (pFriend == NULL)
		return;

	if (pFriend->uid == 0xFFFFFFFF) {
		if (pFriend->flags & 2) {
			CMessengerInfo::Instance()->m_groupState[0] = !CMessengerInfo::Instance()->m_groupState[0];
			m_groupRow[0].status = CMessengerInfo::Instance()->m_groupState[0];
		} else {
			CMessengerInfo::Instance()->m_groupState[1] = !CMessengerInfo::Instance()->m_groupState[1];
			m_groupRow[1].status = CMessengerInfo::Instance()->m_groupState[1];
		}

		UpdateFriendList();
		return;
	}

	m_selectUID = pFriend->uid;
}

// messengerdlg.cpp:703
void FrMessengerDlg::OnMessenger_FriendListDBtnDown(void) {
	FrListItem* pItem = m_pFriendList->GetItemUnderCursor();
	if (pItem == NULL)
		return;

	sFriend* pFriend = (sFriend*)pItem->pData;
	if (pFriend == NULL)
		return;

	if (pFriend->uid == 0xFFFFFFFF)
		return;

	m_selectUID = pFriend->uid;

	if ((pFriend->flags & 2) && (pFriend->flags & 4)) {
		OnChatToFriend();
	} else {
		OnNoteToFriend();
	}
}

// messengerdlg.cpp:738
void FrMessengerDlg::OnMessenger_GuildListInit(int param) {
	m_pGuildList = DYNAMIC_CAST(FrListBox, param);
	if (m_pGuildList == NULL)
		return;

	m_pGuildList->UseRightButton(true);

	m_pSexIcon[0] = m_pGuildList->GetBitmap("i_male");
	m_pSexIcon[1] = m_pGuildList->GetBitmap("i_female");
	m_pSexIcon[2] = m_pGuildList->GetBitmap("i_male_02");
	m_pSexIcon[3] = m_pGuildList->GetBitmap("i_female_02");
	m_pSexIcon[4] = m_pGuildList->GetBitmap("i_male_03");
	m_pSexIcon[5] = m_pGuildList->GetBitmap("i_female_03");

	m_pStatusIcon[0] = g_pFresh->GetManager()->GetBitmap("MESSENGER", "play_icon");
	m_pStatusIcon[1] = g_pFresh->GetManager()->GetBitmap("MESSENGER", "dive_icon");
	m_pStatusIcon[2] = g_pFresh->GetManager()->GetBitmap("MESSENGER", "block_icon");
	m_pStatusIcon[3] = g_pFresh->GetManager()->GetBitmap("MESSENGER", "busy_icon");
	m_pStatusIcon[4] = NULL; m_selectUID = 0xFFFFFFFF; UpdateFriendList();

	m_pGuildList->SetVisible(false);
}

// messengerdlg.cpp:956
void FrMessengerDlg::OnMessenger_GuildListLBtnDown(void) {
	FindNextTopFocus(false);

	m_pRightMenu->SetVisible(false);
	m_pStateMenu->SetVisible(false);

	FrListItem* pItem = m_pGuildList->GetItemUnderCursor();
	if (pItem == NULL)
		return;

	sFriend* pFriend = (sFriend*)pItem->pData;
	if (pFriend == NULL)
		return;

	if (pFriend->uid == 0xFFFFFFFF) {
		if (IsLocalContent((localContentType_t)0x7D)) {
			if (pFriend->flags & 2) {
				CMessengerInfo::Instance()->m_groupState[0] = !CMessengerInfo::Instance()->m_groupState[0];
				m_groupRow[0].status = CMessengerInfo::Instance()->m_groupState[0];
			} else {
				CMessengerInfo::Instance()->m_groupState[1] = !CMessengerInfo::Instance()->m_groupState[1];
				m_groupRow[1].status = CMessengerInfo::Instance()->m_groupState[1];
			}
		}

		UpdateFriendList();
		return;
	}

	m_selectUID = pFriend->uid;
}

// messengerdlg.cpp:1033
void FrMessengerDlg::OnMessenger_GuildListRBtnDown(void) {
	OnMessenger_GuildListLBtnDown();

	FrListItem* pItem = m_pGuildList->GetItemUnderCursor();
	if (pItem == NULL)
		return;

	sFriend* pFriend = (sFriend*)pItem->pData;
	if (pFriend == NULL)
		return;

	m_pRightMenu->MoveWindow(WPoint(g_input->GetMousePoint().x, g_input->GetMousePoint().y));

	if (pFriend->uid == 0xFFFFFFFF)
		return;

	m_pRightMenu->SetVisible(true);

	m_pRightMenu->UnableMenuItem("\xC4\xA3\xB1\xB8\xC7\xE3\xB0\xA1");       // "accept friend"
	m_pRightMenu->UnableMenuItem("\xBA\xB0\xB8\xED\xC0\xD4\xB7\xC2");       // "enter alias"
	m_pRightMenu->UnableMenuItem("\xB4\xEB\xC8\xAD\xBB\xF3\xB4\xEB\xBB\xE8\xC1\xA6");   // "remove buddy"
	m_pRightMenu->UnableMenuItem("\xC2\xF7\xB4\xDC\xC7\xCF\xB1\xE2");       // "block"

	if (pFriend->flags & 2) {
		// "send a note" becomes "chat"
		m_pRightMenu->ToggleMenuItem("\xC2\xCA\xC1\xF6\xBA\xB8\xB3\xBB\xB1\xE2", "\xB4\xEB\xC8\xAD\xC7\xCF\xB1\xE2");

		if (IsLocalContent((localContentType_t)0x54) || IsLocalContent((localContentType_t)0x53)) {
			if (pFriend->flags & 0xA0) {
				m_pRightMenu->UnableMenuItem("\xC3\xCA\xB4\xEB\xC7\xCF\xB1\xE2");   // "invite"
				m_pRightMenu->UnableMenuItem("\xB5\xFB\xB6\xF3\xB0\xA1\xB1\xE2");   // "go with"
			}
		}
	} else {
		m_pRightMenu->ToggleMenuItem("\xB4\xEB\xC8\xAD\xC7\xCF\xB1\xE2", "\xC2\xCA\xC1\xF6\xBA\xB8\xB3\xBB\xB1\xE2");

		if (IsLocalContent((localContentType_t)0x54) || IsLocalContent((localContentType_t)0x53)) {
			m_pRightMenu->UnableMenuItem("\xC3\xCA\xB4\xEB\xC7\xCF\xB1\xE2");
			m_pRightMenu->UnableMenuItem("\xB5\xFB\xB6\xF3\xB0\xA1\xB1\xE2");
		}
	}

	if (pFriend->flags & 0x10) {
		m_pRightMenu->UnableMenuItem("\xC3\xCA\xB4\xEB\xC7\xCF\xB1\xE2");
		m_pRightMenu->UnableMenuItem("\xB5\xFB\xB6\xF3\xB0\xA1\xB1\xE2");
	}

	if (IsLocalContent((localContentType_t)0x54)) {
		if (!m_inviteEnabled)
			m_pRightMenu->UnableMenuItem("\xC3\xCA\xB4\xEB\xC7\xCF\xB1\xE2");
	}
}

// messengerdlg.cpp:1120
void FrMessengerDlg::OnMessenger_GuildListDBtnDown(void) {
	FrListItem* pItem = m_pGuildList->GetItemUnderCursor();
	if (pItem == NULL)
		return;

	sFriend* pFriend = (sFriend*)pItem->pData;
	if (pFriend == NULL)
		return;

	if (pFriend->uid == 0xFFFFFFFF)
		return;

	m_selectUID = pFriend->uid;

	if (pFriend->flags & 2) {
		OnChatToFriend();
	} else {
		OnNoteToFriend();
	}
}

// messengerdlg.cpp:1176
void FrMessengerDlg::OnMessenger_RightMenuInit(int param) {
	m_pRightMenu = (FrContextMenuCtrl*)DYNAMIC_CAST(FrWnd, param);
	if (m_pRightMenu != NULL) {
		m_pRightMenu->SetVisible(false);
		m_pRightMenu->SetFixed(true);
	}
}

// messengerdlg.cpp:1296
void FrMessengerDlg::OnMessenger_StateMenuInit(int param) {
	m_pStateMenu = (FrContextMenuCtrl*)DYNAMIC_CAST(FrWnd, param);
	if (m_pStateMenu != NULL) {
		m_pStateMenu->SetVisible(false);
		m_pStateMenu->SetFixed(true);
	}
}

// messengerdlg.cpp:1316
void FrMessengerDlg::OnMessenger_StateMenuLBtnDown(void) {
	FindNextTopFocus(false);

	const char* text = m_pStateMenu->GetSelectMenuText();

	if (strcmp(text, "\xBF\xC2\xB6\xF3\xC0\xCE") == 0) {
		CMessengerInfo::Instance()->SetMyStatus(MSN::STATUS_ONLINE);
	} else if (strcmp(text, "\xB4\xD9\xB8\xA5\xBF\xEB\xB9\xAB\xC1\xDF") == 0) {
		CMessengerInfo::Instance()->SetMyStatus(MSN::STATUS_BUSY);
	}
}

// messengerdlg.cpp:1347
void FrMessengerDlg::OnInfoFriend(void) {
	sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);
	if (pFriend == NULL)
		return;

	if (CUserInfo::Instance() != NULL) {
		CUserInfo::Instance()->SetInfo(pFriend->uid, 0, true, true, true, true, pFriend->nick);
	}
}

// messengerdlg.cpp:1379
void FrMessengerDlg::OnNoteToFriend(void) {
	sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);
	if (pFriend == NULL)
		return;

	WPoint pt;
	pt.x = m_rect.x + 60.0f;
	pt.y = m_rect.y + 60.0f;

	if (CSharedDoc::Instance()->m_myPang < 10) {
		// "Sending a note needs %d pang."
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", this);
		pForm->SetMessage(MakeStr("\xC2\xCA\xC1\xF6\xB8\xA6 \xBA\xB8\xB3\xBB\xB1\xE2 \xC0\xA7\xC7\xD8\xBC\xAD\xB4\xC2 %d\xC6\xCE\xC0\xCC \xC7\xCA\xBF\xE4\xC7\xD5\xB4\xCF\xB4\xD9.", 10), false);
		pForm->SetTimeLimit(5.0f);
		pForm->SetFixed(true);
		pForm->Open((FRESH_RESULT_CALLBACK)0, pt, 3);
		return;
	}

	if (m_pNoteDlg != NULL)
		return;

	m_pNoteDlg = CreateForm<FrNoteDlg>(g_pFresh->GetManager(), this, "note", this);
	m_pNoteDlg->SetNick(pFriend->nick, 0);
	m_pNoteDlg->SetFixed(true);
	m_pNoteDlg->Open((FRESH_RESULT_CALLBACK)&FrMessengerDlg::OnNoteDlgResult, pt, 1);

	WRect rect = m_pNoteDlg->GetRect();
	m_pNoteDlg->Adjust(rect);
	m_pNoteDlg->MoveWindow(WPoint(rect.x, rect.y));
}

// messengerdlg.cpp:1423
void FrMessengerDlg::OnChatToFriend(void) {
	sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);
	if (pFriend == NULL)
		return;

	if (pFriend->flags & 0x10) {
		// "You cannot chat with a blocked buddy. Unblock them and try again."
		CMessengerInfo::Instance()->Notify1("\xC2\xF7\xB4\xDC\xC7\xD1 \xBB\xF3\xB4\xEB\xBF\xCD\xB4\xC2 \xB4\xEB\xC8\xAD\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \xC2\xF7\xB4\xDC\xC0\xBB \xC7\xD8\xC1\xA6\xC7\xCF\xBD\xC3\xB0\xED \xB4\xD9\xBD\xC3 \xBD\xC3\xB5\xB5\xC7\xCF\xBD\xC3\xB1\xE2 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9.");
		return;
	}

	std::list<sChatSlot>& chatList = CMessengerInfo::Instance()->m_chatDlgList;
	int count = chatList.size();

	if (count < 10) {
		std::list<sChatSlot>::iterator it;
		for (it = chatList.begin(); it != chatList.end(); ++it) {
			if (it->pChatDlg == NULL) {
				if (m_selectUID == it->uid) {
					CMessengerInfo::Instance()->AlarmBtnUp(m_selectUID);
				}
			}

			if (it->uid == pFriend->uid && it->pChatDlg != NULL) {
				it->pChatDlg->FindNextTopFocus(false);
				return;
			}
		}

		sChatSlot slot;
		slot.uid = pFriend->uid;

		slot.pChatDlg = CreateForm<FrMessengerChatDlg>(g_pFresh->GetManager(), this, "messenger_chat", 0);
		slot.pChatDlg->SetFixed(true);
		slot.pChatDlg->Open((FRESH_RESULT_CALLBACK)0, 0xC);
		slot.pChatDlg->SetTopmost(true);
		slot.pChatDlg->SetUID(slot.uid);
		slot.pChatDlg->FindNextTopFocus(false);
		chatList.push_back(slot);

		CMessengerInfo::Instance()->RepositionPopupDlg(slot.pChatDlg, count);
	} else {
		// "The chat slots are already full."
		CMessengerInfo::Instance()->Notify1("\xC0\xCC\xB9\xCC \xB4\xEB\xC8\xAD \xBD\xBD\xB7\xB5\xC0\xCC \xB2\xCB \xC3\xA1\xBD\xC0\xB4\xCF\xB4\xD9.");
	}
}

// messengerdlg.cpp:1497
void FrMessengerDlg::OnAcceptFriend(void) {
	sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);
	if (pFriend != NULL) {
		WSendPacket packet(0x19);
		packet.Encode4(pFriend->uid);
		packet.Send((eSendTo)1);
	}
}

// messengerdlg.cpp:1525
void FrMessengerDlg::OnRemoveFriend(void) {
	sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);

	if (pFriend == NULL)
		return;

	WPoint pt;
	pt.x = m_rect.x + 60.0f;
	pt.y = m_rect.y + 60.0f;

	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", this);
	pForm->SetMessage(MakeStr("\\c0xffff0000\\c%s\xB4\xD4\\c0xff000000\\c\xC0\xBB \xC4\xA3\xB1\xB8 \xB8\xAE\xBD\xBA\xC6\xAE\xBF\xA1\xBC\xAD \\c0xffff0000\\c\xBB\xE8\xC1\xA6\\c0xff000000\\c\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", pFriend->nick), false);
	pForm->SetFixed(true);
	pForm->Open((FRESH_RESULT_CALLBACK)&FrMessengerDlg::OnRemoveFriendDlgResult, pt, 0);

	WRect rect = pForm->GetRect();
	pForm->Adjust(rect);
	pForm->MoveWindow(WPoint(rect.x, rect.y));
}

// messengerdlg.cpp:1557
void FrMessengerDlg::OnBlockFriend(void) {
	sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);

	if (pFriend == NULL)
		return;

	WPoint pt;
	pt.x = m_rect.x + 60.0f;
	pt.y = m_rect.y + 60.0f;

	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", this);
	pForm->SetMessage(MakeStr("\\c0xffff0000\\c%s\xB4\xD4\\c0xff000000\\c\xC0\xBB \xC4\xA3\xB1\xB8 \xB8\xAE\xBD\xBA\xC6\xAE\xBF\xA1\xBC\xAD \\c0xffff0000\\c\xC2\xF7\xB4\xDC\\c0xff000000\\c\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", pFriend->nick), false);
	pForm->SetFixed(true);
	pForm->Open((FRESH_RESULT_CALLBACK)&FrMessengerDlg::OnBlockFriendDlgResult, pt, 0);

	WRect rect = pForm->GetRect();
	pForm->Adjust(rect);
	pForm->MoveWindow(WPoint(rect.x, rect.y));
}

// messengerdlg.cpp:1589
void FrMessengerDlg::OnBlockCancelFriend(void) {
	sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);

	if (pFriend == NULL)
		return;

	WPoint pt;
	pt.x = m_rect.x + 60.0f;
	pt.y = m_rect.y + 60.0f;

	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", this);
	pForm->SetMessage(MakeStr("\\c0xffff0000\\c%s\xB4\xD4\\c0xff000000\\c\xC0\xC7 \xC2\xF7\xB4\xDC\xC0\xBB \\c0xffff0000\\c\xC7\xD8\xC1\xA6\\c0xff00000\\c\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", pFriend->nick), false);
	pForm->SetFixed(true);
	pForm->Open((FRESH_RESULT_CALLBACK)&FrMessengerDlg::OnBlockCancelFriendDlgResult, pt, 0);

	WRect rect = pForm->GetRect();
	pForm->Adjust(rect);
	pForm->MoveWindow(WPoint(rect.x, rect.y));
}

// messengerdlg.cpp:1621
void FrMessengerDlg::OnAliasToFriend(void) {
	sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);

	if (pFriend == NULL)
		return;

	if (m_pMemoDlg != NULL)
		return;

	WPoint pt;
	pt.x = m_rect.x + 60.0f;
	pt.y = m_rect.y + 60.0f;

	m_pMemoDlg = CreateForm<FrMemoDlg>(g_pFresh->GetManager(), this, "memo", this);

	m_pMemoDlg->SetFixed(true);
	m_pMemoDlg->Open((FRESH_RESULT_CALLBACK)&FrMessengerDlg::OnMemoDlgResult, pt, 1);

	WRect rect = m_pMemoDlg->GetRect();
	m_pMemoDlg->Adjust(rect);
	m_pMemoDlg->MoveWindow(WPoint(rect.x, rect.y));
}

// messengerdlg.cpp:1967
void FrMessengerDlg::OnMessenger_CloseLBtnDown(void) {
	GetClientRect(CMessengerInfo::Instance()->m_rcMessengerDlg);

	Close(true);

	CMessengerInfo::Instance()->SetGameFocus(true);
}

// messengerdlg.cpp:1985
void FrMessengerDlg::OnMessenger_FriendBtnInit(int param) {
	m_pTabBtn[0] = DYNAMIC_CAST(FrButton, param);

	if (m_pTabBtn[0] != NULL) {
		m_pTabBtn[0]->SetStatus(FrButton::PRESSED);
	}
}

// messengerdlg.cpp:2005
void FrMessengerDlg::OnMessenger_FriendLBtnDown(void) {
	FindNextTopFocus(false);
	RestoreOther(0);

	m_pFriendList->SetVisible(true);
	m_pStateBtn->SetVisible(true);
	m_pGuildList->SetVisible(false);

	m_pGuildNoticeBtn->SetVisible(false);

	m_pRightMenu->SetVisible(false);

	m_pStateMenu->SetVisible(false);
}

// messengerdlg.cpp:2047
void FrMessengerDlg::OnMessenger_GuildBtnInit(int param) {
	m_pTabBtn[1] = DYNAMIC_CAST(FrButton, param);
	if (CSharedDoc::Instance()->GetPangyaUserInfo().guildUID == 0) {
		m_pTabBtn[1]->Enable(false);
	}
}

// messengerdlg.cpp:2066
void FrMessengerDlg::OnMessenger_GuildLBtnDown(void) {
	FindNextTopFocus(false);

	if (CSharedDoc::Instance()->GetPangyaUserInfo().guildUID != 0) {
		RestoreOther(1);

		m_pFriendList->SetVisible(false);
		m_pStateBtn->SetVisible(false);
		m_pGuildList->SetVisible(true);

		m_pRightMenu->SetVisible(false);

		m_pStateMenu->SetVisible(false);
	} else {
		RestoreOther(0);
	}

	m_pRightMenu->SetVisible(false);

	m_pStateMenu->SetVisible(false);
}

// messengerdlg.cpp:2156
void FrMessengerDlg::OnMessenger_StateBtnInit(int param) {
	m_pStateBtn = DYNAMIC_CAST(FrButton, param);
}

// messengerdlg.cpp:2171
void FrMessengerDlg::OnMessenger_StateLBtnDown(void) {
	m_pStateMenu->SetVisible(true);
}

// messengerdlg.cpp:2187
void FrMessengerDlg::OnMessenger_GuildNoticeBtnInit(int param) {
	m_pGuildNoticeBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pGuildNoticeBtn != NULL) {
		m_pGuildNoticeBtn->Enable(false);

		m_pGuildNoticeBtn->SetVisible(false);
	}
}

// messengerdlg.cpp:2210
void FrMessengerDlg::OnMessenger_GuildNoticeLBtnDown(void) {
	if (CMessengerInfo::Instance()->m_pGuildNoticeDlg != NULL)
		return;

	WPoint pt;
	pt.x = m_rect.x + 60.0f;
	pt.y = m_rect.y + 60.0f;

	CMessengerInfo::Instance()->m_pGuildNews = new NewsReader;

	if (NetResourceManager::Instance()->IsRSSDownLoadComplete()) {
		// the guild news the RSS download saved
		CMessengerInfo::Instance()->m_pGuildNews->Load("guildnews.xml", 5);
	}

	CMessengerInfo::Instance()->m_pGuildNoticeDlg = CreateForm<FrGuildNoticeDlg>(g_pFresh->GetManager(), this, "guild_notice", NULL);
	CMessengerInfo::Instance()->m_pGuildNoticeDlg->DataBind(CMessengerInfo::Instance()->m_pGuildNews, 0);
	CMessengerInfo::Instance()->m_pGuildNoticeDlg->SetFixed(true);
	CMessengerInfo::Instance()->m_pGuildNoticeDlg->Open((FRESH_RESULT_CALLBACK)&FrMessengerDlg::OnGuildNoticeDlgResult, pt, 1);
	CMessengerInfo::Instance()->m_pGuildNoticeDlg->SetTopmost(true);
	CMessengerInfo::Instance()->m_pGuildNoticeDlg->FindNextTopFocus(false);

	WRect rect = CMessengerInfo::Instance()->m_pGuildNoticeDlg->GetRect();
	CMessengerInfo::Instance()->m_pGuildNoticeDlg->Adjust(rect);
	CMessengerInfo::Instance()->m_pGuildNoticeDlg->MoveWindow(WPoint(rect.x, rect.y));
}

// messengerdlg.cpp:2253
void FrMessengerDlg::RestoreOther(unsigned long index) {
	for (unsigned long i = 0; i < 2; i++) {
		if (i == index) {
			m_pTabBtn[i]->SetStatus(FrButton::PRESSED);
		} else {
			m_pTabBtn[i]->SetStatus(FrButton::NORMAL);
		}
	}
}

// messengerdlg.cpp:2277
void FrMessengerDlg::OnMessenger_FindLBtnDown(void) {
	if (m_pAddFriendDlg != NULL)
		return;

	WPoint pt;
	pt.x = m_rect.x + 60.0f;
	pt.y = m_rect.y + 60.0f;

	m_pAddFriendDlg = CreateForm<FrAddFriendDlg>(g_pFresh->GetManager(), this, "addfriend", this);

	m_pAddFriendDlg->SetFixed(true);
	m_pAddFriendDlg->Open((FRESH_RESULT_CALLBACK)&FrMessengerDlg::OnAddFriendDlgResult, pt, 1);

	WRect rect = m_pAddFriendDlg->GetRect();
	m_pAddFriendDlg->Adjust(rect);
	m_pAddFriendDlg->MoveWindow(WPoint(rect.x, rect.y));
}

// messengerdlg.cpp:2336
bool FrMessengerDlg::OnNoteDlgResult(int ret, FrForm* pForm) {
	if (ret == 1) {
		sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);
		if (pFriend != NULL) {
			WSendPacket packet((enumClientPacket)0x3C);
			packet.Encode2(0x111);
			packet.Encode4(pFriend->uid);
			packet.EncodeStr(std::string(m_pNoteDlg->GetNote()));
			packet.Encode1(0);
			packet.Send((eSendTo)0);
		}
	}

	m_pNoteDlg = 0;

	return true;
}

// messengerdlg.cpp:2367
bool FrMessengerDlg::OnAddFriendDlgResult(int ret, FrForm* pForm) {
	m_pAddFriendDlg = 0;
	return true;
}

// messengerdlg.cpp:2383
bool FrMessengerDlg::OnOptionDlgResult(int ret, FrForm* pForm) {
	m_pOptionDlg = 0;
	return true;
}

// messengerdlg.cpp:2399
bool FrMessengerDlg::OnMemoDlgResult(int ret, FrForm* pForm) {
	if (ret == 1) {
		sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);
		if (pFriend != NULL) {
			WSendPacket packet(0x1F);
			packet.Encode4(pFriend->uid);

			packet.EncodeStr(std::string(m_pMemoDlg->GetMemo()));
			packet.Send((eSendTo)1);
		}
	}

	m_pMemoDlg = 0;

	return true;
}

// messengerdlg.cpp:2433
bool FrMessengerDlg::OnRemoveFriendDlgResult(int ret, FrForm* pForm) {
	if (ret == 1) {
		sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);
		if (pFriend != NULL) {
			WSendPacket packet(0x1C);
			packet.Encode4(pFriend->uid);
			packet.EncodeStr(std::string(pFriend->nick));

			packet.Send((eSendTo)1);
		}

		m_selectUID = 0xFFFFFFFF;
	}

	return true;
}

// messengerdlg.cpp:2467
bool FrMessengerDlg::OnBlockFriendDlgResult(int ret, FrForm* pForm) {
	if (ret == 1) {
		sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);
		if (pFriend != NULL) {
			WSendPacket packet(0x1A);
			packet.Encode4(pFriend->uid);
			packet.Send((eSendTo)1);
		}
		m_selectUID = 0xFFFFFFFF;
	}

	return true;
}

// messengerdlg.cpp:2500
bool FrMessengerDlg::OnBlockCancelFriendDlgResult(int ret, FrForm* pForm) {
	if (ret == 1) {
		sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);
		if (pFriend != NULL) {
			WSendPacket packet(0x1B);
			packet.Encode4(pFriend->uid);
			packet.Send((eSendTo)1);
		}
		m_selectUID = 0xFFFFFFFF;
	}

	return true;
}

// messengerdlg.cpp:2533
bool FrMessengerDlg::OnGuildNoticeDlgResult(int ret, FrForm* pForm) {
	CMessengerInfo::Instance()->m_pGuildNoticeDlg = 0;
	if (CMessengerInfo::Instance()->m_pGuildNews) { delete CMessengerInfo::Instance()->m_pGuildNews; CMessengerInfo::Instance()->m_pGuildNews = 0; }

	return true;
}

// messengerdlg.cpp:2562
MSN::eStatusType FrMessengerDlg::GetBuddyStatus(sFriend* pFriend) {
	if (pFriend->flags & 0x10)
		return MSN::STATUS_AWAY;

	if (pFriend->flags & 0x80)
		return MSN::STATUS_BUSY;

	if ((pFriend->flags & 0x20) && (pFriend->flags & 2))
		return MSN::STATUS_ACTIVE;

	if ((pFriend->flags & 0x40) && (pFriend->flags & 2))
		return MSN::STATUS_PLAYING;

	return MSN::STATUS_OFFLINE;
}

// messengerdlg.cpp:2590
void FrMessengerDlg::SetCount(void) {
	std::map<unsigned long, sFriend>* pList = BuddyMGR->GetBuddyList();

	if (pList == NULL)
		return;

	m_list0Count = 0;
	m_list0OnCount = 0;
	m_list1Count = 0;
	m_list1OnCount = 0;

	std::map<unsigned long, sFriend>::iterator it;
	for (it = pList->begin(); it != pList->end(); ++it) {
		if (it->second.listFlags & 1)
			m_list0Count++;
		if (it->second.listFlags & 2)
			m_list1Count++;

		if (it->second.flags & 2) {
			if ((it->second.listFlags & 1) && (it->second.flags & 4))
				m_list0OnCount++;
			if (it->second.listFlags & 2)
				m_list1OnCount++;
		}
	}
}

// messengerdlg.cpp:2769
void FrMessengerDlg::OnInviteFriendRealMyRoom(bool bForce) {
	if (!IsLocalContent((localContentType_t)0x56))
		return;

	sFriend* pFriend = BuddyMGR->GetBuddy(m_selectUID);

	bool bInvitable = bForce ? true : ((pFriend->flags >> 2) & 1) != 0;
	if (pFriend == NULL || !bInvitable)
		return;

	if (!g_pFresh->IsCurrentLayout("REALMYROOM") && !g_pFresh->IsCurrentLayout("REALMYROOMVISITOR")) {
		CSharedDoc::Instance()->SetRmrMode(5);
		CSharedDoc::Instance()->m_myRoomOwnerUID = pFriend->uid;

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x222, 0, 0, 0, 0, 0));
	} else {
		// "You cannot visit another user's room right now."
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\xC7\xF6\xC0\xE7 \xC0\xAF\xC0\xFA\xB4\xC2 \xB4\xD9\xB8\xA5 \xC0\xAF\xC0\xFA\xC0\xC7 \xB9\xE6\xC0\xBB \xB9\xE6\xB9\xAE\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xBB\xF3\xC5\xC2 \xC0\xD4\xB4\xCF\xB4\xD9.", 0, 0, 0, 0));
	}
}

// messengerdlg.cpp:2803
CMessengerInfo::CMessengerInfo(void) : m_pMessengerDlg(0), m_notifyElapsed(0), m_notifyActive(false) {
	m_rcMessengerDlg.x = 200.0f;
	m_rcMessengerDlg.y = 100.0f;
	m_rcMessengerDlg.w = 220.0f;
	m_rcMessengerDlg.h = 300.0f;

	m_rcAlarmBtn.x = 663.0f;
	m_rcAlarmBtn.y = 0.0f;
	m_rcAlarmBtn.w = 38.0f;
	m_rcAlarmBtn.h = 32.0f;

	m_myStatus = MSN::STATUS_ONLINE;
	m_pGuildNoticeDlg = 0;
	m_pGuildNews = 0;
	m_reconnectElapsed = 0;

	memset(m_groupState, 1, sizeof(m_groupState));

	m_popupCount = 0;
	m_serverListRequest = 0;
	m_autoAway = false;

	m_chatDlgList.clear();
	m_focusOrder.clear();

	m_serverList.clear();

	m_waitingForServerList = false;
	m_currentServerIndex = 0;
	if (IsLocalContent((localContentType_t)0x53)) {
		CSharedDoc::Instance()->SetGoWithModeEnd();
	}

	if (IsLocalContent((localContentType_t)0x54))
		m_inviteEnabled = false;
}

// The last messenger connection attempts (CMessengerInfo::Open allows five per
// minute). No symbol in the PDB; the type name is invented.
struct sConnectHistory {
	unsigned long ticks[5];          // 0x0, GetTickCount of each attempt
	int count;                       // 0x14
};
static sConnectHistory data_af7784;

// messengerdlg.cpp:2852
void CMessengerInfo::Open(bool) {
	if (WNetworkSystem::Instance()->GetSocket(WNetworkSystem::NET_UNIT_MSN)->m_bConnected != 0 && WNetworkSystem::Instance()->IsConnectionComplete(WNetworkSystem::NET_UNIT_MSN)) {
		Toggle();
		return;
	}

	// "Connecting to the messenger server...."
	Notify2("\xB8\xDE\xBD\xC5\xC0\xFA\xBC\xAD\xB9\xF6\xBF\xA1 \xC1\xA2\xBC\xD3\xC1\xDF....");

	unsigned long now = GetTickCount();

	if (data_af7784.count >= 5) {
		if (now - data_af7784.ticks[4] > 90000) {
			data_af7784.count = 0;
			data_af7784.ticks[0] = 0;
		} else {
			// "Please try again later."
			Notify2("\xC0\xE1\xBD\xC3 \xC8\xC4\xBF\xA1 \xC0\xCC\xBF\xEB\xC7\xD8 \xC1\xD6\xBD\xCA\xBD\xC3\xBF\xC0.");
			return;
		}
	}

	if ((!WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN) && !WNetworkSystem::Instance()->IsConnectionComplete(WNetworkSystem::NET_UNIT_MSN))
		|| WNetworkSystem::Instance()->IsConnectionComplete(WNetworkSystem::NET_UNIT_MSN) == 1) {
		WNetworkSystem::Instance()->Init(WNetworkSystem::NET_UNIT_MSN);
		WNetworkSystem::Instance()->SendMessage(WNetworkSystem::NET_UNIT_MSN, NetworkUnit::EVENT_NEXT);

		if (now - data_af7784.ticks[0] < 60000) {
			data_af7784.ticks[data_af7784.count] = now;
			data_af7784.count++;
			return;
		}

		int i;
		for (i = 0; i < data_af7784.count; i++) {
			if (now - data_af7784.ticks[i] < 60000)
				break;
		}

		if (i < data_af7784.count) {
			int count = data_af7784.count - i;

			for (int j = 0; j < count; j++)
				data_af7784.ticks[j] = data_af7784.ticks[i + j];

			data_af7784.ticks[count] = now;
			data_af7784.count = count + 1;
		} else {
			data_af7784.ticks[0] = now;
			data_af7784.count = 1;
		}
	}
}

// messengerdlg.cpp:2984
bool CMessengerInfo::Close(bool bDestroy) {
	if (m_pGuildNoticeDlg != NULL)
		m_pGuildNoticeDlg->Close(true);

	if (!bDestroy && !IsVisible())
		return true;

	std::list<sChatSlot>& chatList = CMessengerInfo::Instance()->m_chatDlgList;
	std::list<sChatSlot>::iterator it;
	for (it = chatList.begin(); it != chatList.end(); ) {
		if (it->pChatDlg != NULL) {
			if (!it->pChatDlg->IsTopFocus()) {
				++it;
				continue;
			}

			if (!it->pChatDlg->Close(bDestroy))
				return false;
		}

		if (it->bReceived && !bDestroy) {
			++it;
			continue;
		}

		it = chatList.erase(it);

		if (!bDestroy)
			return false;
	}

	if (m_pMessengerDlg != NULL) {
		m_pMessengerDlg->GetClientRect(m_rcMessengerDlg);
		m_pMessengerDlg->Adjust(m_rcMessengerDlg);
		m_pMessengerDlg->SetFadeout(true);

		return false;
	}

	if (bDestroy) {
		// TODO: CSharedDoc 0x4DBD has no member yet (SharedDoc.h is not this unit's).
		(&CSharedDoc::Instance()->m_tutorialFinish)[1] = 0;
		CSharedDoc::Instance()->m_tutorialFinish = 0;
	}

	return true;
}

// messengerdlg.cpp:3067
void CMessengerInfo::CloseWindow(void) {
	SaveCurrentFocus();

	std::list<sChatSlot>& chatList = CMessengerInfo::Instance()->m_chatDlgList;
	std::list<sChatSlot>::iterator it;
	for (it = chatList.begin(); it != chatList.end(); ++it) {
		if (it->pChatDlg != NULL) {
			it->pChatDlg->GetClientRect(it->rcChatDlg);
			it->pChatDlg->Adjust(it->rcChatDlg);
			it->pChatDlg->Close(true);
			it->pChatDlg = 0;
		}
	}

	if (m_pMessengerDlg != NULL) {
		m_pMessengerDlg->GetClientRect(m_rcMessengerDlg);
		m_pMessengerDlg->Adjust(m_rcMessengerDlg);
		m_pMessengerDlg->Close(true);
		m_pMessengerDlg = 0;
	}
}

// messengerdlg.cpp:3108
void CMessengerInfo::Toggle(void) {
	std::list<sChatSlot>& chatList = CMessengerInfo::Instance()->m_chatDlgList;

	bool bVisible = IsVisible() ? true : false;
	bool bChatRestored = false;

	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI))
		SetGameFocus(true);

	if (bVisible == true) {
		SaveCurrentFocus();

		std::list<sChatSlot>::iterator it;
		for (it = chatList.begin(); it != chatList.end(); ++it) {
			if (it->pChatDlg != NULL) {
				it->pChatDlg->GetClientRect(it->rcChatDlg);
				it->pChatDlg->SaveTempChat(*it);
				it->pChatDlg->SetFadeout(true);
			}
		}

		if (m_pMessengerDlg != NULL) {
			m_pMessengerDlg->GetClientRect(m_rcMessengerDlg);
			m_pMessengerDlg->SetFadeout(true);
		}
	} else {
		Notify4(false);

		FrWnd* pLastChat = NULL;

		if (m_focusOrder.size() > 0) {
			std::list<unsigned long>::iterator itFocus;
			for (itFocus = m_focusOrder.begin(); itFocus != m_focusOrder.end(); ++itFocus) {
				unsigned long uid = *itFocus;

				if (uid == 0xFFFFFFFF) {
					if (m_pMessengerDlg == NULL) {
						m_pMessengerDlg = CreateForm<FrMessengerDlg>(g_pFresh->GetManager(), this, "messenger", 0);
						if (m_pMessengerDlg != NULL) {
							if (!((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)))
								m_pMessengerDlg->Adjust(m_rcMessengerDlg);

							m_pMessengerDlg->SetFixed(true);
							m_pMessengerDlg->Open((FRESH_RESULT_CALLBACK)&CMessengerInfo::OnMessengerDlgResult, m_rcMessengerDlg.TopLeft(), 0xC);
							m_pMessengerDlg->SetTopmost(true);
							m_pMessengerDlg->SetFadeout(bVisible);

							if (IsLocalContent((localContentType_t)0x54))
								m_pMessengerDlg->m_inviteEnabled = m_inviteEnabled;
						}
					}
				} else {
					std::list<sChatSlot>::iterator it;
					for (it = chatList.begin(); it != chatList.end(); ++it) {
						if (!it->bReceived && it->pChatDlg == NULL && it->uid == uid) {
							it->pChatDlg = CreateForm<FrMessengerChatDlg>(g_pFresh->GetManager(), this, "messenger_chat", 0);

							if (!((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)))
								it->pChatDlg->Adjust(it->rcChatDlg);

							it->pChatDlg->SetFixed(true);
							it->pChatDlg->Open((FRESH_RESULT_CALLBACK)0, it->rcChatDlg.TopLeft(), 0xC);
							it->pChatDlg->SetUID(it->uid);
							it->pChatDlg->SetTopmost(true);
							it->pChatDlg->SetFadeout(bVisible);

							bChatRestored = true;
							it->pChatDlg->RestoreChat(*it);

							pLastChat = it->pChatDlg;
						}
					}
				}
			}

			if (m_pMessengerDlg == NULL) {
				m_pMessengerDlg = CreateForm<FrMessengerDlg>(g_pFresh->GetManager(), this, "messenger", 0);
				if (m_pMessengerDlg != NULL) {
					if (!((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)))
						m_pMessengerDlg->Adjust(m_rcMessengerDlg);

					m_pMessengerDlg->SetFixed(true);
					m_pMessengerDlg->Open((FRESH_RESULT_CALLBACK)&CMessengerInfo::OnMessengerDlgResult, m_rcMessengerDlg.TopLeft(), 0xC);
					m_pMessengerDlg->SetTopmost(true);
					m_pMessengerDlg->SetFadeout(bVisible);
					m_pMessengerDlg->GetClientRect(m_rcMessengerDlg);

					if (IsLocalContent((localContentType_t)0x54))
						m_pMessengerDlg->m_inviteEnabled = m_inviteEnabled;
				}
			}

			if (pLastChat != NULL)
				pLastChat->FindNextTopFocus(true);
		} else {
			if (m_pMessengerDlg == NULL) {
				m_pMessengerDlg = CreateForm<FrMessengerDlg>(g_pFresh->GetManager(), this, "messenger", 0);
				if (m_pMessengerDlg != NULL) {
					if (!((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)))
						m_pMessengerDlg->Adjust(m_rcMessengerDlg);

					m_pMessengerDlg->SetFixed(true);
					m_pMessengerDlg->Open((FRESH_RESULT_CALLBACK)&CMessengerInfo::OnMessengerDlgResult, m_rcMessengerDlg.TopLeft(), 0xC);
					m_pMessengerDlg->SetTopmost(true);
					m_pMessengerDlg->SetFadeout(bVisible);
					m_pMessengerDlg->GetClientRect(m_rcMessengerDlg);

					if (m_pMessengerDlg != NULL)
						m_pMessengerDlg->UpdateFriendList();

					if (IsLocalContent((localContentType_t)0x54))
						m_pMessengerDlg->m_inviteEnabled = m_inviteEnabled;
				}
			}
		}

		m_focusOrder.clear();
	}

	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)
		&& !CChatMsg::Instance()->IsOpened()) {
		g_pFresh->GetManager()->SetExclusiveKey(!bVisible && bChatRestored);
	}

	g_audio->PlaySfx("ui_icon_boing2", NULL, 0, NULL, NULL, 0.5f, 200.0f);
}

// messengerdlg.cpp:3321
void CMessengerInfo::SaveCurrentFocus(void) {
	std::string name;

	std::list<sChatSlot>& chatList = CMessengerInfo::Instance()->m_chatDlgList;
	FrWndManager* pManager = g_pFresh->GetManager();

	std::list<FrWnd*>::iterator it;
	for (it = pManager->GetTopWndList().begin(); it != pManager->GetTopWndList().end(); ++it) {
		FrWnd* pWnd = *it;

		if ((__rtti_obj = (void*)pWnd) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&FrForm::m_RTTI)) {
			FrForm* pForm = (FrForm*)pWnd;
			pForm->GetWindowName(name);

			if (strcmp("messenger", name.c_str()) == 0) {
				m_focusOrder.push_back((unsigned long)-1);
			} else if (strcmp("messenger_chat", name.c_str()) == 0) {
				std::list<sChatSlot>::iterator itSlot;
				for (itSlot = chatList.begin(); itSlot != chatList.end(); ++itSlot) {
					if (itSlot->pChatDlg == pForm)
						m_focusOrder.push_back(itSlot->uid);
				}
			}
		}
	}
}

// messengerdlg.cpp:3372
void CMessengerInfo::ApplySavedFocus(void) {
	std::list<sChatSlot>& chatList = CMessengerInfo::Instance()->m_chatDlgList;

	std::list<unsigned long>::iterator it;
	for (it = m_focusOrder.begin(); it != m_focusOrder.end(); ++it) {
		unsigned long uid = *it;

		if (uid == 0xFFFFFFFF) {
			if (m_pMessengerDlg != NULL) {
				m_pMessengerDlg->FindNextTopFocus(false);
			}
		} else {
			std::list<sChatSlot>::iterator chat;
			for (chat = chatList.begin(); chat != chatList.end(); ++chat) {
				if (chat->uid == uid) {
					if (chat->pChatDlg != NULL) {
						chat->pChatDlg->FindNextTopFocus(false);
					}
				}
			}
		}
	}
}

// messengerdlg.cpp:3417
void CMessengerInfo::Process(float dt) {
	if (CMessengerInfo::Instance()->m_serverListRequest != 4) {
		if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN)
			&& WNetworkSystem::Instance()->IsConnectionComplete(WNetworkSystem::NET_UNIT_MSN)
			&& WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
			m_reconnectElapsed += dt;

			if (m_reconnectElapsed > 60.0f) {
				m_reconnectElapsed = 0.0f;

				WNetworkSystem::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_MSN);
				Close(true);
				WNetworkSystem::Instance()->Init(WNetworkSystem::NET_UNIT_MSN);
				WNetworkSystem::Instance()->SendMessage(WNetworkSystem::NET_UNIT_MSN, NetworkUnit::EVENT_NEXT);
			}
		} else if (!WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN)) {
			m_reconnectElapsed = 0.0f;
		}
	}

	if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN)
		&& WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)
		&& CMessengerInfo::Instance()->GetMyStatus() != MSN::STATUS_BUSY) {
		bool bChanged = false;

		if (m_autoAway) {
			if (g_mouse->IsUpdated() || g_ime->IsUpdated()) {
				m_autoAway = false;
				bChanged = true;
			}
		} else if (timeGetTime() - g_mouse->GetLastInputTime() > 120000 && timeGetTime() - g_ime->GetLastInputTime() > 120000) {
			m_autoAway = true;
			bChanged = true;
		}

		if (bChanged) {
			if (m_autoAway)
				CMessengerInfo::Instance()->SetMyStatus(MSN::STATUS_PLAYING);
			else
				CMessengerInfo::Instance()->SetMyStatus(MSN::STATUS_ONLINE);
		}
	}

	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)
		&& WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN)) {
		if (g_input->GetButton(LEFT_BUTTON) == 1 && !CChatMsg::Instance()->IsOpened()) {
			g_pFresh->GetManager()->SetExclusiveKey(false);
			SetGameFocus(true);
		}

		if (g_input->GetButton(RIGHT_BUTTON) == 1 || g_input->GetButton(LEFT_BUTTON) == 1) {
			const WPoint& pt = g_pFresh->GetManager()->GetInputState()->mousePos;
			WRect rc;

			bool bOutside = true;
			std::list<sChatSlot>::iterator it;
			for (it = CMessengerInfo::Instance()->m_chatDlgList.begin(); it != CMessengerInfo::Instance()->m_chatDlgList.end(); ++it) {
				if (it->pChatDlg != NULL) {
					it->pChatDlg->GetClientRect(rc);

					if (rc.IsInRect(pt))
						bOutside = false;

					if (it->pChatDlg->GetEmoticonClientRect(rc)) {
						if (rc.IsInRect(pt))
							bOutside = false;
					}
				}
			}

			if (m_pMessengerDlg != NULL) {
				m_pMessengerDlg->GetClientRect(rc);

				if (rc.IsInRect(pt))
					bOutside = false;
			}

			if (bOutside) {
				SetGameFocus(true);
			} else {
				m_gameFocus = false;
			}
		}
	}

	if (m_notifyActive) {
		m_notifyElapsed += dt;

		if (m_notifyElapsed > 4.0f) {
			if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CLobbyTask::m_RTTI)) {
				AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0xA8, 0, 0, 0, 0, 0);
			} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CShopTask::m_RTTI)) {
				AfxGetTask()->GetActor("Shop") << MsgObject(0, 0xA8, 0, 0, 0, 0, 0);
			} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CRealMyRoomTask::m_RTTI)) {
				AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0xA8, 0, 0, 0, 0, 0);
			} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CFamilyTask::m_RTTI)) {
				AfxGetTask()->GetActor("Family") << MsgObject(0, 0xA8, 0, 0, 0, 0, 0);
			} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
				AfxGetTask()->GetActor("GolfRule") << MsgObject(0, 0xA8, 0, 0, 0, 0, 0);
			} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&ntAvatarChatTask::m_RTTI)) {
				AfxGetTask()->GetActor("AvatarChat") << MsgObject(0, 0xA8, 0, 0, 0, 0, 0);
			} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CRealMyRoomTask::m_RTTI)) {
				AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0xA8, 0, 0, 0, 0, 0);
			}

			m_notifyActive = false;
			m_notifyElapsed = 0.0f;
		}
	}
}

// messengerdlg.cpp:3609
bool CMessengerInfo::OnMessengerDlgResult(int ret, FrForm* pForm) {
	m_pMessengerDlg = 0;
	return true;
}

// messengerdlg.cpp:3625
bool CMessengerInfo::OnMessengerChatDlgResult(int ret, FrForm* pForm) {
	std::list<sChatSlot>::iterator it;
	for (it = m_chatDlgList.begin(); it != m_chatDlgList.end(); ) {
		if (it->pChatDlg == pForm) {
			it->pChatDlg = 0;
		}
	}

	return true;
}

// messengerdlg.cpp:3680
void CMessengerInfo::Notify1(const char* msg) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
	if (pForm != NULL) {
		pForm->SetMessage(msg, false);
		pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
		pForm->SetTopmost(true);
		pForm->FindNextTopFocus(false);
	}
}

// messengerdlg.cpp:3702
void CMessengerInfo::Notify2(const char* msg) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CLobbyTask::m_RTTI)) {
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0xA8, (intptr_t)msg, 1, 0, 0, 0);
	} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CShopTask::m_RTTI)) {
		AfxGetTask()->GetActor("Shop") << MsgObject(0, 0xA8, (intptr_t)msg, 1, 0, 0, 0);
	} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CRealMyRoomTask::m_RTTI)) {
		AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0xA8, (intptr_t)msg, 1, 0, 0, 0);
	} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CFamilyTask::m_RTTI)) {
		AfxGetTask()->GetActor("Family") << MsgObject(0, 0xA8, (intptr_t)msg, 1, 0, 0, 0);
	} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		AfxGetTask()->GetActor("GolfRule") << MsgObject(0, 0xA8, (intptr_t)msg, 1, 0, 0, 0);
	} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&ntAvatarChatTask::m_RTTI)) {
		AfxGetTask()->GetActor("AvatarChat") << MsgObject(0, 0xA8, (intptr_t)msg, 1, 0, 0, 0);
	} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CRealMyRoomTask::m_RTTI)) {
		AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0xA8, (intptr_t)msg, 1, 0, 0, 0);
	}

	m_notifyElapsed = 0.0f;
	m_notifyActive = true;
}

// messengerdlg.cpp:3994
void CMessengerInfo::SetOfflineUserNick(const char* nick, unsigned long uid) {
	if (m_pMessengerDlg != NULL) {
		m_pMessengerDlg->SetAddFriendNick(nick, uid);
	}
}

// messengerdlg.cpp:4016
void CMessengerInfo::SetTargetNick(const char* nick) {
	if (m_pMessengerDlg != NULL) {
		m_pMessengerDlg->SetTargetNick(nick);
	}
}

// messengerdlg.cpp:4035
void CMessengerInfo::Refresh(void) {
	if (m_pMessengerDlg != NULL) {
		m_pMessengerDlg->UpdateFriendList();
	}
}

// messengerdlg.cpp:4053
void CMessengerInfo::SetMyStatus(MSN::eStatusType status) {
	if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN) && CMessengerInfo::Instance()->m_serverListRequest == 4) {
		m_myStatus = status;
		WSendPacket packet(0x1D);
		packet.Encode1((unsigned char)m_myStatus);
		packet.Send((eSendTo)1);
	}
}

// messengerdlg.cpp:4078
bool CMessengerInfo::IsVisible(void) {
	std::list<sChatSlot>::iterator it;
	for (it = CMessengerInfo::Instance()->m_chatDlgList.begin(); it != CMessengerInfo::Instance()->m_chatDlgList.end(); ++it) {
		if (it->pChatDlg != NULL) {
			return true;
		}
	}

	return m_pMessengerDlg ? true : false;
}

// messengerdlg.cpp:4113
void CMessengerInfo::RepositionPopupDlg(FrMessengerChatDlg* pDlg, int index) {
	if (pDlg == NULL)
		return;

	if (index == 0) {
		m_popupCount = 1;
		return;
	}

	if (index <= 0)
		return;

	WRect rc = pDlg->GetRect();

	float cx = g_view->GetWidth() - rc.w;
	if (cx <= 0.0f)
		return;
	float cy = g_view->GetHeight() - rc.h;
	if (cy <= 0.0f)
		return;

	int offset = m_popupCount * 20;

	WPoint pt(rc.x + offset * cx / cy, rc.y + offset); if ((int)(g_view->GetWidth() - rc.w) > 0) pt.x = (float)((int)pt.x % (int)(g_view->GetWidth() - rc.w)); if ((int)(g_view->GetHeight() - rc.h) > 0) pt.y = (float)((int)pt.y % (int)(g_view->GetHeight() - rc.h)); pDlg->MoveWindow(pt); m_popupCount++;
}

// messengerdlg.cpp:4151
void CMessengerInfo::SetGameFocus(bool focus) {
	m_gameFocus = focus;

	if (focus) {
		if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
			g_pFresh->GetManager()->ResetTopFocus();
		}
	}
}

// messengerdlg.cpp:4165
void CMessengerInfo::InvalidateCurrentServer(void) {
	int index = 0;
	std::list<MSNServerInfo>::iterator it;
	for (it = m_serverList.begin(); it != m_serverList.end(); ++it, ++index) {
		if (index == m_currentServerIndex) {
			it->m_tried = true;
			return;
		}
	}
}
