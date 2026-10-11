// Original: d:\build\custom\temp\code\client\projectg\detailedroominfodlg.cpp
#include "SharedDoc.h"
#include "Packet.h"
#include "TaskManager.h"
#include "actor.h"
#include "DetailedRoomInfoDlg.h"
#include "ObjectFactory.h"
#include "frbutton.h"
#include "fredit.h"
#include "frlistbox.h"
#include "user_info.h"
#include "../shared/classdefine.h"
#include <stdio.h>
#include <string.h>

typedef std::map<unsigned long, sBriefUserInfo> UserMap;

// detailedroominfodlg.cpp:65 (sort by nickname, ascending; players missing
// from the user map first)
static bool CompareNickAsc(const void* pA, const void* pB) {
	const sRoomUserInfo* a = (const sRoomUserInfo*)pA;
	const sRoomUserInfo* b = (const sRoomUserInfo*)pB;

	UserMap::iterator itA = WSingleton<CSharedDoc>::Instance()->m_userMap.find(a->m_uid);
	UserMap::iterator itB = WSingleton<CSharedDoc>::Instance()->m_userMap.find(b->m_uid);

	if (itA == WSingleton<CSharedDoc>::Instance()->m_userMap.end())
		return true;

	if (itB == WSingleton<CSharedDoc>::Instance()->m_userMap.end())
		return true;




	return strcmp(itA->second.nick, itB->second.nick) < 0 ? true : false;
}

// Minimal stubs for user list handlers used by the message map. These
// implementations are placeholders to satisfy the linker during iterative
// development and must be replaced with full behavior later.
void FrDtRoomDlg::OnUserListInit(intptr_t param) {
	m_pUserList = DYNAMIC_CAST(FrListBox, param);
}

void FrDtRoomDlg::OnUserListOwnerDraw(int) { }

void FrDtRoomDlg::OnJoinBtnUp(void) {
	// Placeholder: attempt to join the room by sending a minimal message
	// or simply close the dialog in this stubbed build.
	Close(FrNONE, true);
}

// detailedroominfodlg.cpp:40 (sort by level, or rank in channels with flag
// 0x80, descending)
static bool CompareLevelDesc(const void* pA, const void* pB) {
	const sRoomUserInfo* a = (const sRoomUserInfo*)pA;
	const sRoomUserInfo* b = (const sRoomUserInfo*)pB;

	if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x80) {

		if (a->m_rankValue > b->m_rankValue)
			return true;
		else if (a->m_rankValue < b->m_rankValue)
			return false;
	} else {

		if (a->m_level > b->m_level)
			return true;

		else if (a->m_level < b->m_level)
			return false;
	}


	return CompareNickAsc(pA, pB);
}

// detailedroominfodlg.cpp:85 (sort by status, descending)
static bool CompareStatusDesc(const void* pA, const void* pB) {
	const sRoomUserInfo* a = (const sRoomUserInfo*)pA;
	const sRoomUserInfo* b = (const sRoomUserInfo*)pB;

	if ((a->m_status & 0x1f) > (b->m_status & 0x1f))
		return true;
	else if ((a->m_status & 0x1f) < (b->m_status & 0x1f))
		return false;

	return CompareNickAsc(pA, pB);
}

// detailedroominfodlg.cpp:98 (sort by level, or rank in channels with flag
// 0x80, ascending)
static bool CompareLevelAsc(const void* pA, const void* pB) {
	const sRoomUserInfo* a = (const sRoomUserInfo*)pA;
	const sRoomUserInfo* b = (const sRoomUserInfo*)pB;

	if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x80) {

		if (a->m_rankValue > b->m_rankValue)
			return false;
		else if (a->m_rankValue < b->m_rankValue)
			return true;
	} else {

		if (a->m_level > b->m_level)
			return false;

		else if (a->m_level < b->m_level)
			return true;
	}


	return CompareNickAsc(pA, pB);
}

// detailedroominfodlg.cpp:123 (sort by nickname, descending; players missing
// from the user map first)
static bool CompareNickDesc(const void* pA, const void* pB) {
	const sRoomUserInfo* a = (const sRoomUserInfo*)pA;
	const sRoomUserInfo* b = (const sRoomUserInfo*)pB;

	UserMap::iterator itA = WSingleton<CSharedDoc>::Instance()->m_userMap.find(a->m_uid);
	UserMap::iterator itB = WSingleton<CSharedDoc>::Instance()->m_userMap.find(b->m_uid);

	if (itA == WSingleton<CSharedDoc>::Instance()->m_userMap.end())
		return true;

	if (itB == WSingleton<CSharedDoc>::Instance()->m_userMap.end())
		return true;




	return strcmp(itA->second.nick, itB->second.nick) >= 0 ? true : false;
}

// detailedroominfodlg.cpp:143 (sort by status, ascending)
static bool CompareStatusAsc(const void* pA, const void* pB) {
	const sRoomUserInfo* a = (const sRoomUserInfo*)pA;
	const sRoomUserInfo* b = (const sRoomUserInfo*)pB;

	if ((a->m_status & 0x1f) > (b->m_status & 0x1f))
		return false;
	else if ((a->m_status & 0x1f) < (b->m_status & 0x1f))
		return true;

	return CompareNickAsc(pA, pB);
}

// detailedroominfodlg.cpp:156 (run-time type and factory registration of FrDtRoomDlg)
const WRTTI FrDtRoomDlg::m_RTTI("FrDtRoomDlg", &FrForm::m_RTTI);

IObject* FrDtRoomDlgMakeInstance(void) {
	return new FrDtRoomDlg;
}

struct __sFrDtRoomDlg {
	__sFrDtRoomDlg(void) {
		ObjectFactory().AddObjectFunctor(FrDtRoomDlgMakeInstance, "FrDtRoomDlg");
	}
};
static __sFrDtRoomDlg s_registerFrDtRoomDlg;

// detailedroominfodlg.cpp:158
const sFRESH_MSGMAP* FrDtRoomDlg::GetMessageMap(void) const {
	return &FrDtRoomDlg::_MsgMap;
}

sFRESH_MSGMAP FrDtRoomDlg::_MsgMap = { &FrForm::_MsgMap, &FrDtRoomDlg::_MsgEntries[0] };

sFRESH_ENTRY FrDtRoomDlg::_MsgEntries[] = {
	// detailedroominfodlg.cpp:160
	{ "gamedesc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDtRoomDlg::OnGameDescInit },
	// detailedroominfodlg.cpp:161
	{ "join", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDtRoomDlg::OnJoinBtnInit },
	// detailedroominfodlg.cpp:162
	{ "join", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDtRoomDlg::OnJoinBtnUp },
	// detailedroominfodlg.cpp:163
	{ "userinfo", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDtRoomDlg::OnUserInfoBtnInit },
	// detailedroominfodlg.cpp:164
	{ "userinfo", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDtRoomDlg::OnUserInfoBtnUp },
	// detailedroominfodlg.cpp:165
	{ "level", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDtRoomDlg::OnLevelHdrInit },
	// detailedroominfodlg.cpp:166
	{ "level", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrDtRoomDlg::OnLevelHdrDown },
	// detailedroominfodlg.cpp:167
	{ "id", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDtRoomDlg::OnIdHdrInit },
	// detailedroominfodlg.cpp:168
	{ "id", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrDtRoomDlg::OnIdHdrDown },
	// detailedroominfodlg.cpp:169
	{ "status", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDtRoomDlg::OnStatusHdrInit },
	// detailedroominfodlg.cpp:170
	{ "status", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrDtRoomDlg::OnStatusHdrDown },
	// detailedroominfodlg.cpp:171
	{ "userlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDtRoomDlg::OnUserListInit },
	// detailedroominfodlg.cpp:172
	{ "userlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDtRoomDlg::OnUserListOwnerDraw },
	// detailedroominfodlg.cpp:173
	{ "userlist", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrDtRoomDlg::OnUserListLBtnDown },
	// detailedroominfodlg.cpp:174
	{ "userlist", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDtRoomDlg::OnUserListRBtnUp },

	// detailedroominfodlg.cpp:176
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// detailedroominfodlg.cpp:182
FrDtRoomDlg::FrDtRoomDlg(void)
	: m_pGameDesc(0), m_pJoinBtn(0), m_pUserInfoBtn(0), m_pGalleryBtn(0), m_pUserList(0) {
	m_pSelectedUser = 0; m_pIdHdr = 0; m_pLevelHdr = 0; m_pStatusHdr = 0; m_sortState = 1;
}

// detailedroominfodlg.cpp:189
bool FrDtRoomDlg::OnInit(void) {
	m_sortState = HEADER_LEVEL + 3;
	OnHeaderLBtnDown(HEADER_LEVEL);

	return true;
}

// detailedroominfodlg.cpp:197
void FrDtRoomDlg::SetRoomInfo(const sRoomInfo& roomInfo) {
	m_roomInfo = roomInfo;
}

// detailedroominfodlg.cpp:202
void FrDtRoomDlg::OnGameDescInit(intptr_t param) {
	m_pGameDesc = DYNAMIC_CAST(FrEdit, param);

	char text[256];
	const char* pName;



	switch (CSharedDoc::Instance()->m_gameDesc.m_descGameType) {
	case 4:
	case 5:
	case 6:
	case 9:
	case 10:
	case 14: {
		IFF_STRUCT::sMatch* pMatch = ItemManager()->FindMatch(CSharedDoc::Instance()->m_gameDesc.m_descMatchId);

		if (pMatch) {
			pName = pMatch->m_name;
			break;
		}
	}
	default:
		pName = CSharedDoc::Instance()->m_gameTypeNames[CSharedDoc::Instance()->m_gameDesc.m_descGameType].name;
	}


	sprintf(text, "\xb0\xd4\xc0\xd3\xc5\xb8\xc0\xd4 : %s%s", pName, CSharedDoc::Instance()->m_gameDesc.m_descGameType == 5 ? "\xc6\xc0\xc0\xfc" : "");

	m_pGameDesc->SetLine(1, text, 0, false, 0);


	unsigned char course = CSharedDoc::Instance()->m_gameDesc.m_descCourse;
	if (course == 0xff) {

		strcpy(text, "\xc4\xda\xbd\xba : RANDOM");

	} else {

		unsigned long typeId = 0x28000000 | ((course > 0x7F && course != 0xFD) ? course - 0x80 : course);
		IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(typeId);

		if (pCourse) {
			sprintf(text, "\xc4\xda\xbd\xba : %s", pCourse->m_name);
		} else {
			strcpy(text, "\xc4\xda\xbd\xba : \xb8\xf0\xb8\xa7");
		}
	}


	m_pGameDesc->SetLine(2, text, 0, false, 0);


















	unsigned char mode = CSharedDoc::Instance()->m_gameDesc.m_descMode;
	if (mode == 2 || mode == 3) {


		sprintf(text, "\xbd\xc3\xc0\xdb\xc8\xa6 : \xb7\xa3\xb4\xfd \xc8\xa6 / \xc8\xa6 \xbc\xf6 : %d \xc8\xa6", CSharedDoc::Instance()->m_gameDesc.m_descHoleCount);

	} else {


		sprintf(text, "\xbd\xc3\xc0\xdb\xc8\xa6 : %d \xc8\xa6 / \xc8\xa6 \xbc\xf6 : %d \xc8\xa6", mode ? 10 : 1, CSharedDoc::Instance()->m_gameDesc.m_descHoleCount);
	}


	m_pGameDesc->SetLine(3, text, 0, false, 0);


	unsigned char type = CSharedDoc::Instance()->m_gameDesc.m_descGameType;
	if (type == 2) {

		text[0] = 0;

	} else {
		switch (type) {
		case 4:
		case 5:
		case 6:
		case 9:
		case 10:
		case 14:
			if (type != 10) {

				sprintf(text, "\xb0\xd4\xc0\xd3\xc1\xa6\xc7\xd1\xbd\xc3\xb0\xa3 : %d \xba\xd0", CSharedDoc::Instance()->m_gameDesc.m_descGameTime / 60000);
				break;
			}
		default:

			if (CSharedDoc::Instance()->m_gameDesc.m_descGameTime == 0) {
				strcpy(text, "\xc5\xb8\xb1\xb8\xc1\xa6\xc7\xd1\xbd\xc3\xb0\xa3 : \xbe\xf8\xc0\xbd");
			} else {


				sprintf(text, "\xc5\xb8\xb1\xb8\xc1\xa6\xc7\xd1\xbd\xc3\xb0\xa3 : %d \xc3\xca", CSharedDoc::Instance()->m_gameDesc.m_descGameTime / 1000);
			}
		}
	}

	m_pGameDesc->SetLine(4, text, 0, false, 0);
}

// detailedroominfodlg.cpp:294
void FrDtRoomDlg::OnJoinBtnInit(intptr_t param) {
	m_pJoinBtn = DYNAMIC_CAST(FrButton, param);


	if (m_pJoinBtn && (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x80)) {
		m_pJoinBtn->Enable(false);
	}
}

// detailedroominfodlg.cpp:431
void FrDtRoomDlg::OnGalleryBtnInit(int param) {
	m_pGalleryBtn = DYNAMIC_CAST(FrButton, param);
}

// detailedroominfodlg.cpp:471
void FrDtRoomDlg::EnableJoinBtn(bool bEnable) {
	if (m_pJoinBtn) {
		m_pJoinBtn->Enable(bEnable && !(WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x80) ? true : false);
	}
}

// detailedroominfodlg.cpp:477
void FrDtRoomDlg::OnProc(const float dt) {
	std::list<void*>::iterator it;

	for (it = m_delList.begin(); it != m_delList.end(); ) {
		m_pUserList->DelItem(*it);
		it = m_delList.erase(it);
	}
}

// detailedroominfodlg.cpp:487
void FrDtRoomDlg::OnUserInfoBtnInit(intptr_t param) {
	m_pUserInfoBtn = DYNAMIC_CAST(FrButton, param);
	m_pUserInfoBtn->Enable(false);
}

// detailedroominfodlg.cpp:493
void FrDtRoomDlg::OnUserInfoBtnUp(void) {
	if (WSingleton<CUserInfo>::Instance()) {

		FrListItem* pItem = m_pUserList->GetSelected();

		if (pItem) {


			sRoomUserInfo* pUser = (sRoomUserInfo*)pItem->pData;

			UserMap::iterator it = WSingleton<CSharedDoc>::Instance()->m_userMap.find(pUser->m_uid);
			if (it != WSingleton<CSharedDoc>::Instance()->m_userMap.end()) {

				WSingleton<CUserInfo>::Instance()->SetInfo(it->second.id, pUser->m_uid, true, true, false, false, it->second.nick);
			}
		}
	}
}

// detailedroominfodlg.cpp:512
void FrDtRoomDlg::OnLevelHdrInit(intptr_t param) {
	m_pLevelHdr = DYNAMIC_CAST(FrButton, param);
}

// detailedroominfodlg.cpp:517
void FrDtRoomDlg::OnLevelHdrDown(void) {
	OnHeaderLBtnDown(HEADER_LEVEL);
}

// detailedroominfodlg.cpp:522
void FrDtRoomDlg::OnIdHdrInit(intptr_t param) {
	m_pIdHdr = DYNAMIC_CAST(FrButton, param);
}

// detailedroominfodlg.cpp:527
void FrDtRoomDlg::OnIdHdrDown(void) {
	OnHeaderLBtnDown(HEADER_ID);
}

// detailedroominfodlg.cpp:532
void FrDtRoomDlg::OnStatusHdrInit(intptr_t param) {
	m_pStatusHdr = DYNAMIC_CAST(FrButton, param);
}

// detailedroominfodlg.cpp:537
void FrDtRoomDlg::OnStatusHdrDown(void) {
	OnHeaderLBtnDown(HEADER_STATUS);
}

// detailedroominfodlg.cpp:542
void FrDtRoomDlg::OnHeaderLBtnDown(eHeader header) {
	if (m_pUserList) {


		if (header == m_sortState) {

			m_sortState = header + 3;

			switch (header) {
			case HEADER_LEVEL:

				m_pUserList->SortItem(CompareLevelAsc);
				break;
			case HEADER_ID:

				m_pUserList->SortItem(CompareNickDesc);
				break;
			case HEADER_STATUS:

				m_pUserList->SortItem(CompareStatusAsc);
				break;
			}
		} else {

			m_sortState = header;

			switch (header) {
			case HEADER_LEVEL:

				m_pUserList->SortItem(CompareLevelDesc);
				break;
			case HEADER_ID:

				m_pUserList->SortItem(CompareNickAsc);
				break;
			case HEADER_STATUS:

				m_pUserList->SortItem(CompareStatusDesc);
				break;
			}
		}
	}
}

// detailedroominfodlg.cpp:791
void FrDtRoomDlg::OnUserListLBtnDown(void) {
	FrListItem* pItem = m_pUserList->GetItemUnderCursor();
	if (pItem) {

		sRoomUserInfo* pUser = (sRoomUserInfo*)pItem->pData;
		m_pUserList->SelectItem(pItem, true);



		m_pSelectedUser = pUser;
		if (m_pUserInfoBtn) m_pUserInfoBtn->Enable(true);
	}
}

// detailedroominfodlg.cpp:806
void FrDtRoomDlg::OnUserListRBtnUp(void) {
	OnUserListLBtnDown();


	if (m_pSelectedUser && WSingleton<CUserInfo>::Instance()) {

		UserMap::iterator it = WSingleton<CSharedDoc>::Instance()->m_userMap.find(m_pSelectedUser->m_uid);
		if (it != WSingleton<CSharedDoc>::Instance()->m_userMap.end()) {

			WSingleton<CUserInfo>::Instance()->SetInfo(it->second.id, m_pSelectedUser->m_uid, true, true, false, false, it->second.nick);
		}
	}
}

// detailedroominfodlg.cpp:820
bool FrDtRoomDlg::OnPasswordDlgResult(int result, FrForm* pForm) {
	if (result == FrOK) {

		FrWnd* pWnd = pForm->FindChildByName("password");

		FrEdit* pEdit = DYNAMIC_CAST(FrEdit, pWnd);

		std::string password;
		if (pEdit) {

			const char* pPassword = pEdit->GetLine(1, false);
			password = pPassword;
			AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x27, (intptr_t)pPassword, 0, 0, 0, 0));
		}


		FrWnd* pOk = pForm->FindChildByName("ok");
		if (pOk) {
			pOk->Enable(false);
		}

		WSendPacket packet((enumClientPacket)9);
		packet.Encode2(m_roomInfo.m_roomNumber);
		packet.EncodeStr(password);
		packet.Send(SEND_TO_DEFAULT);

		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x16, 0, 0, 0, 0, 0));
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 1, 0, 0, 0, 0, 0));

		Close(FrOK, true);
	}


	return true;
}
