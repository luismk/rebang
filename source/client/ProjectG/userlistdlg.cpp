// Original: d:\build\custom\temp\code\client\projectg\userlistdlg.cpp
#include "UserListDlg.h"
#include "AddFriendDlg.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "Localize.h"
#include "frbutton.h"
#include "frlistbox.h"
#include "frscrollbar.h"
#include "TaskManager.h"
#include "actor.h"
#include "fresh.h"
#include "user_info.h"

#include <string.h>

// userlistdlg.cpp:35 (run-time type and factory registration of FrUserListDlg)
const WRTTI FrUserListDlg::m_RTTI("FrUserListDlg", &FrForm::m_RTTI);

IObject* FrUserListDlgMakeInstance(void) {
	return new FrUserListDlg;
}

// Minimal stubs to satisfy linker when full behaviors are not required yet.
void FrUserListDlg::OnInviteBtnUp(void) {
	// stub: perform basic invite action placeholder
	// real implementation should open invite dialog / send invite
	if (m_pInviteBtn) {
		// disable invite to avoid repeated calls
		m_pInviteBtn->Enable(false);
	}
}

void FrUserListDlg::OnUserListOwnerDraw(int param) {
	// stub: no-op owner draw
	(void)param;
}

struct __sFrUserListDlg {
	__sFrUserListDlg(void) {
		ObjectFactory().AddObjectFunctor(FrUserListDlgMakeInstance, "FrUserListDlg");
	}
};
static __sFrUserListDlg s_registerFrUserListDlg;

// userlistdlg.cpp:37
const sFRESH_MSGMAP* FrUserListDlg::GetMessageMap(void) const {
	return &FrUserListDlg::_MsgMap;
}

sFRESH_MSGMAP FrUserListDlg::_MsgMap = { &FrForm::_MsgMap, &FrUserListDlg::_MsgEntries[0] };

sFRESH_ENTRY FrUserListDlg::_MsgEntries[] = {
	// userlistdlg.cpp:39
	{ "invite", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUserListDlg::OnInviteBtnInit },
	// userlistdlg.cpp:40
	{ "invite", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUserListDlg::OnInviteBtnUp },
	// userlistdlg.cpp:41
	{ "whisper", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUserListDlg::OnWhisperBtnInit },
	// userlistdlg.cpp:42
	{ "whisper", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUserListDlg::OnWhisperBtnUp },
	// userlistdlg.cpp:43
	{ "addfriend", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUserListDlg::OnFriendBtnUp },
	// userlistdlg.cpp:44
	{ "sex", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUserListDlg::OnSortBtnInit },
	// userlistdlg.cpp:45
	{ "sex", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUserListDlg::OnSexBtnUp },
	// userlistdlg.cpp:46
	{ "level", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUserListDlg::OnSortBtnInit },
	// userlistdlg.cpp:47
	{ "level", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUserListDlg::OnLevelBtnUp },
	// userlistdlg.cpp:48
	{ "guild", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUserListDlg::OnSortBtnInit },
	// userlistdlg.cpp:49
	{ "guild", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUserListDlg::OnGuildBtnUp },
	// userlistdlg.cpp:50
	{ "id", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUserListDlg::OnSortBtnInit },
	// userlistdlg.cpp:51
	{ "id", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUserListDlg::OnIdBtnUp },
	// userlistdlg.cpp:52
	{ "user", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUserListDlg::OnUserListInit },
	// userlistdlg.cpp:53
	{ "user", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUserListDlg::OnUserListOwnerDraw },
	// userlistdlg.cpp:54
	{ "user", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrUserListDlg::OnUserListLBtnDown },
	// userlistdlg.cpp:55
	{ "user", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUserListDlg::OnUserListRBtnUp },
	// userlistdlg.cpp:56
	{ "user", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&FrUserListDlg::OnUserListDClick },

	// userlistdlg.cpp:58
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// userlistdlg.cpp:65
FrUserListDlg::FrUserListDlg(void) {
	m_pWhisperBtn = 0; m_pInviteBtn = 0; m_pUserList = 0;
	m_selectedUID = 0xffffffff;
	m_pInviteDelayDlg = 0;
	m_selectedID = 0xffffffff;
	m_sortType = 3;
	m_sortReverse = false;

	m_pAddFriendDlg = 0;
}

// userlistdlg.cpp:77
void FrUserListDlg::MakeUserList(std::map<unsigned long, sBriefUserInfo>& userMap) {
	m_pUserList->ClearItem();
	for (std::map<unsigned long, sBriefUserInfo>::iterator it = userMap.begin(); it != userMap.end(); ++it) {
		// leave the local player out
		if (it->second.uid == WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid)
			continue;

		// only an administrator sees the users with identity 0x14
		if (!((WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity >> 2) & 1) && (it->second.identity & 0x14))
			continue;

		m_pUserList->AddItem(&it->second);
	}
}

// userlistdlg.cpp:92
void FrUserListDlg::OnProc(const float dt) {
	if (m_pInviteDelayDlg) {
		m_inviteDelayTime -= dt;

		if (m_inviteDelayTime <= 0.0f) {
			m_pInviteDelayDlg->Close(true);
			m_pInviteDelayDlg = 0;
		}
	}
}

// userlistdlg.cpp:109
bool UserList_UserSexCompare(const void* a, const void* b) {
	const sBriefUserInfo* pA = (const sBriefUserInfo*)a;
	const sBriefUserInfo* pB = (const sBriefUserInfo*)b;

	return pA->charType % 2 > pB->charType % 2;
}

// userlistdlg.cpp:117
bool UserList_UserSexRevCompare(const void* a, const void* b) {
	const sBriefUserInfo* pA = (const sBriefUserInfo*)a;
	const sBriefUserInfo* pB = (const sBriefUserInfo*)b;

	return pA->charType % 2 < pB->charType % 2;
}

// userlistdlg.cpp:125
bool UserList_UserLevelCompare(const void* a, const void* b) {
	const sBriefUserInfo* pA = (const sBriefUserInfo*)a;
	const sBriefUserInfo* pB = (const sBriefUserInfo*)b;

	if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x80)
		return pA->rankValue > pB->rankValue;

	return pA->level > pB->level;
}

// userlistdlg.cpp:136
bool UserList_UserLevelRevCompare(const void* a, const void* b) {
	const sBriefUserInfo* pA = (const sBriefUserInfo*)a;
	const sBriefUserInfo* pB = (const sBriefUserInfo*)b;

	if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x80)
		return pA->rankValue < pB->rankValue;

	return pA->level < pB->level;
}

// userlistdlg.cpp:147
bool UserList_UserNickCompare(const void* a, const void* b) {
	const sBriefUserInfo* pA = (const sBriefUserInfo*)a;
	const sBriefUserInfo* pB = (const sBriefUserInfo*)b;

	return _strcmpi(pA->nick, pB->nick) < 0;
}

// userlistdlg.cpp:155
bool UserList_UserNickRevCompare(const void* a, const void* b) {
	const sBriefUserInfo* pA = (const sBriefUserInfo*)a;
	const sBriefUserInfo* pB = (const sBriefUserInfo*)b;

	return _strcmpi(pA->nick, pB->nick) > 0;
}

// userlistdlg.cpp:163
bool UserList_UserGuildRevCompare(const void* a, const void* b) {
	const sBriefUserInfo* pA = (const sBriefUserInfo*)a;
	const sBriefUserInfo* pB = (const sBriefUserInfo*)b;

	return pA->guildUID < pB->guildUID;
}

// userlistdlg.cpp:171
bool UserList_UserGuildCompare(const void* a, const void* b) {
	const sBriefUserInfo* pA = (const sBriefUserInfo*)a;
	const sBriefUserInfo* pB = (const sBriefUserInfo*)b;

	return pA->guildUID > pB->guildUID;
}

// userlistdlg.cpp:179
void FrUserListDlg::OnSortBtnInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
	pButton->SetPushDelay(0.0f);
}

// userlistdlg.cpp:185
void FrUserListDlg::OnSexBtnUp(void) {
	if (m_sortType != 0) {
		m_sortType = 0;
	} else {
		m_sortReverse = !m_sortReverse;
	}

	SortUserList();

	if (m_pUserList && m_pUserList->GetScrollBar()) {
		m_pUserList->GetScrollBar()->ScrollToFirst();
	}
}

// userlistdlg.cpp:200
void FrUserListDlg::OnLevelBtnUp(void) {
	if (m_sortType != 1) {
		m_sortType = 1;
	} else {
		m_sortReverse = !m_sortReverse;
	}

	SortUserList();

	if (m_pUserList && m_pUserList->GetScrollBar()) {
		m_pUserList->GetScrollBar()->ScrollToFirst();
	}
}

// userlistdlg.cpp:225
void FrUserListDlg::OnGuildBtnUp(void) {
	if (m_sortType != 2) {
		m_sortType = 2;
	} else {
		m_sortReverse = !m_sortReverse;
	}

	SortUserList();

	if (m_pUserList && m_pUserList->GetScrollBar()) {
		m_pUserList->GetScrollBar()->ScrollToFirst();
	}
}

// userlistdlg.cpp:241
void FrUserListDlg::OnIdBtnUp(void) {
	if (m_sortType != 3) {
		m_sortType = 3;
	} else {
		m_sortReverse = !m_sortReverse;
	}

	SortUserList();

	if (m_pUserList && m_pUserList->GetScrollBar()) {
		m_pUserList->GetScrollBar()->ScrollToFirst();
	}
}

// userlistdlg.cpp:256
void FrUserListDlg::SortUserList(void) {
	if (m_pUserList) {
		switch (m_sortType) {
		case 0:
			if (m_sortReverse) {
				m_pUserList->SortItem(UserList_UserSexRevCompare);
			} else {
				m_pUserList->SortItem(UserList_UserSexCompare);
			}
			break;
		case 1:
			if (m_sortReverse) {
				m_pUserList->SortItem(UserList_UserLevelRevCompare);
			} else {
				m_pUserList->SortItem(UserList_UserLevelCompare);
			}
			break;
		case 2:
			if (m_sortReverse) {
				m_pUserList->SortItem(UserList_UserNickRevCompare);
			} else {
				m_pUserList->SortItem(UserList_UserNickCompare);
			}
			break;
		case 3:
			if (m_sortReverse) {
				m_pUserList->SortItem(UserList_UserGuildRevCompare);
			} else {
				m_pUserList->SortItem(UserList_UserGuildCompare);
			}
			break;
		}
	}
}

// userlistdlg.cpp:294
void FrUserListDlg::EnableRightButton(bool enable) {
	m_pUserList->UseRightButton(enable);
}

// userlistdlg.cpp:299
void FrUserListDlg::OnUserListInit(intptr_t param) {
	m_pUserList = (FrListBox*)param;
	if (m_pUserList) {
		m_pUserList->UseRightButton(true);

		m_pMaleBmp = m_pUserList->GetBitmap("i_male");
		m_pFemaleBmp = m_pUserList->GetBitmap("i_female");
		m_pMale02Bmp = m_pUserList->GetBitmap("i_male_02");
		m_pFemale02Bmp = m_pUserList->GetBitmap("i_female_02");
		m_pMale03Bmp = m_pUserList->GetBitmap("i_male_03");
		m_pFemale03Bmp = m_pUserList->GetBitmap("i_female_03");

		m_pMaleMannerBmp = m_pUserList->GetBitmap("i_male_manner");
		m_pFemaleMannerBmp = m_pUserList->GetBitmap("i_female_manner");

		m_pMaleAngelBmp = m_pUserList->GetBitmap("i_male_angel");
		m_pFemaleAngelBmp = m_pUserList->GetBitmap("i_female_angel");

		m_pInGameBmp = m_pUserList->GetBitmap("ingame");
	}
}

// userlistdlg.cpp:467
void FrUserListDlg::OnUserListLBtnDown(void) {
	FrListItem* pItem = m_pUserList->GetItemUnderCursor();
	if (pItem == 0)
		return;

	sBriefUserInfo* pInfo = (sBriefUserInfo*)pItem->pData;
	m_selectedUID = pInfo->uid;
	m_selectedID = pInfo->id;

	if (m_pInviteBtn) {
		m_pInviteBtn->Enable(m_selectedUID != WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid && pInfo->roomNumber == 0xFFFF && !(WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x80));
	}

	if (m_pWhisperBtn)
		m_pWhisperBtn->Enable(m_selectedUID != WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid);

	if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 6)
		m_pInviteBtn->Enable(false);
}

// userlistdlg.cpp:491
void FrUserListDlg::OnUserListRBtnUp(void) {
	FrListItem* pItem = m_pUserList->GetItemUnderCursor();
	if (pItem == 0)
		return;

	sBriefUserInfo* pInfo = (sBriefUserInfo*)pItem->pData;

	if (pInfo == 0)
		return;

	m_selectedUID = pInfo->uid;
	m_selectedID = pInfo->id;

	if (m_pInviteBtn) {
		m_pInviteBtn->Enable(!GalleryMode() && pInfo->roomNumber == 0xFFFF && !(WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x80));
	}

	if (m_pWhisperBtn)
		m_pWhisperBtn->Enable(m_selectedUID != WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid);

	if (CUserInfo::Instance()) {
		std::map<unsigned long, sBriefUserInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_userMap.find(m_selectedUID);
		if (it != WSingleton<CSharedDoc>::Instance()->m_userMap.end()) {
			CUserInfo::Instance()->SetInfo(it->second.id, m_selectedUID, true, true, false, false, it->second.nick);
		}
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 6)
		m_pInviteBtn->Enable(false);
}

// userlistdlg.cpp:527
void FrUserListDlg::OnUserListDClick(void) {
	if ((WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity & 0xe) == 0xe) {
		OnInviteBtnUp();
	}
}

// userlistdlg.cpp:533
void FrUserListDlg::OnInviteBtnInit(intptr_t param) {
	m_pInviteBtn = (FrButton*)param;

	if (m_pInviteBtn) {
		m_pInviteBtn->Enable(false);
	}
}

// userlistdlg.cpp:591
void FrUserListDlg::OnFriendBtnUp(void) {
	m_pAddFriendDlg = CreateForm<FrAddFriendDlg>(g_pFresh->GetManager(), this, "addfriend", 0);

	if (m_selectedUID != -1 && m_selectedUID != CSharedDoc::Instance()->GetPangyaUserInfo().guid) {
		std::map<unsigned long, sBriefUserInfo>::iterator it = CSharedDoc::Instance()->m_userMap.find(m_selectedUID);
		if (it != CSharedDoc::Instance()->m_userMap.end()) {
			m_pAddFriendDlg->SetNick(it->second.nick);
		}
	}

	m_pAddFriendDlg->Open(0, 1);
}

// userlistdlg.cpp:612
void FrUserListDlg::OnWhisperBtnInit(intptr_t param) {
	m_pWhisperBtn = (FrButton*)param;
	m_pWhisperBtn->Enable(false);
}

// userlistdlg.cpp:618
void FrUserListDlg::OnWhisperBtnUp(void) {
	if (m_selectedUID == -1) {
		if (m_pWhisperBtn) {
			m_pWhisperBtn->Enable(false);
		}
	} else {
		std::map<unsigned long, sBriefUserInfo>::iterator it = CSharedDoc::Instance()->m_userMap.find(m_selectedUID);
		if (it != CSharedDoc::Instance()->m_userMap.end()) {
			AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x37, (intptr_t)it->second.nick, 0, 0, 0, 0));
		}
	}
}

// userlistdlg.cpp:634
void FrUserListDlg::AddUser(void* data) {
	if (m_pUserList) {
		m_pUserList->AddItem(data);
	}
}

// userlistdlg.cpp:640
void FrUserListDlg::DelUser(void* data, int cmpLen) {
	if (m_pUserList) {
		m_pUserList->DelItem(data, cmpLen);
	}
}

// userlistdlg.cpp:647
void FrUserListDlg::SetAddFriendDlgNick(const char* nick, const char* unused) {
	if (m_pAddFriendDlg) {
		m_pAddFriendDlg->SetNick(nick);
	}
}

// userlistdlg.cpp:653
bool FrUserListDlg::OnAddFriendDlgResult(int result, FrForm* pForm) {
	m_pAddFriendDlg = 0;
	return true;
}

// userlistdlg.cpp:660
void FrUserListDlg::CloseInviteDelayDlg(void) {
	if (IsLocalContent((localContentType_t)0x54)) {
		if (m_pInviteDelayDlg) {
			m_pInviteDelayDlg->Close(true);
			m_pInviteDelayDlg = 0;
		}
	}
}

// userlistdlg.cpp:672
void FrUserListDlg::SetRequestFriendData(const char* nick, unsigned long uid) {
	if (nick) {
		if (m_pAddFriendDlg) {
			m_pAddFriendDlg->SetNick(nick);
			m_pAddFriendDlg->SetFindUID(uid);
		}
	}
}
