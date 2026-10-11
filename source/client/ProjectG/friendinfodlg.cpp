// Original: d:\build\custom\temp\code\client\projectg\friendinfodlg.cpp
#include "FriendInfoDlg.h"
#include "ObjectFactory.h"
#include "TaskManager.h"
#include "frbutton.h"
#include "fredit.h"
#include "SharedDoc.h"

#include <algorithm>

// friendinfodlg.cpp:17 (run-time type and factory registration of FrFriendInfoDlg)
const WRTTI FrFriendInfoDlg::m_RTTI("FrFriendInfoDlg", &FrForm::m_RTTI);

IObject* FrFriendInfoDlgMakeInstance(void) {
	return new FrFriendInfoDlg;
}

struct __sFrFriendInfoDlg {
	__sFrFriendInfoDlg(void) {
		ObjectFactory().AddObjectFunctor(FrFriendInfoDlgMakeInstance, "FrFriendInfoDlg");
	}
};
static __sFrFriendInfoDlg s_registerFrFriendInfoDlg;

// friendinfodlg.cpp:19
const sFRESH_MSGMAP* FrFriendInfoDlg::GetMessageMap(void) const {
	return &FrFriendInfoDlg::_MsgMap;
}

sFRESH_MSGMAP FrFriendInfoDlg::_MsgMap = { &FrForm::_MsgMap, &FrFriendInfoDlg::_MsgEntries[0] };

sFRESH_ENTRY FrFriendInfoDlg::_MsgEntries[] = {
	// friendinfodlg.cpp:21
	{ "id", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFriendInfoDlg::OnIDInit },
	// friendinfodlg.cpp:22
	{ "nick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFriendInfoDlg::OnNickInit },
	// friendinfodlg.cpp:23
	{ "sex", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFriendInfoDlg::OnSexInit },
	// friendinfodlg.cpp:24
	{ "server", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFriendInfoDlg::OnServerInit },
	// friendinfodlg.cpp:25
	{ "channel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFriendInfoDlg::OnChannelInit },
	// friendinfodlg.cpp:26
	{ "whisper", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrFriendInfoDlg::OnWhisperBtnUp },
	// friendinfodlg.cpp:27
	{ "ignore", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFriendInfoDlg::OnIgnoreInit },
	// friendinfodlg.cpp:28
	{ "ignore", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrFriendInfoDlg::OnIgnoreBtnUp },
	// friendinfodlg.cpp:29
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrFriendInfoDlg::OnCloseBtnUp },

	// friendinfodlg.cpp:31
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// friendinfodlg.cpp:34
void FrFriendInfoDlg::OnIDInit(intptr_t param) {
	m_pID = DYNAMIC_CAST(FrEdit, param);
}

// friendinfodlg.cpp:39
void FrFriendInfoDlg::OnNickInit(intptr_t param) {
	m_pNick = DYNAMIC_CAST(FrEdit, param);
}

// friendinfodlg.cpp:44
void FrFriendInfoDlg::OnSexInit(intptr_t param) {
	m_pSex = DYNAMIC_CAST(FrEdit, param);
}

// friendinfodlg.cpp:49
void FrFriendInfoDlg::OnServerInit(intptr_t param) {
	m_pServer = DYNAMIC_CAST(FrEdit, param);
}

// friendinfodlg.cpp:54
void FrFriendInfoDlg::OnChannelInit(intptr_t param) {
	m_pChannel = DYNAMIC_CAST(FrEdit, param);
}

// friendinfodlg.cpp:59
void FrFriendInfoDlg::SetInfo(sFriend *info) {
}

// friendinfodlg.cpp:64
void FrFriendInfoDlg::OnWhisperBtnUp(void) {
	AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x37, (intptr_t)m_pNick->GetLine(1, false), 0, 0, 0, 0));

	Close(true);
}

// friendinfodlg.cpp:71
void FrFriendInfoDlg::OnIgnoreInit(intptr_t param) {
	m_pIgnore = DYNAMIC_CAST(FrButton, param);
}

// friendinfodlg.cpp:76
void FrFriendInfoDlg::OnIgnoreBtnUp(void) {
	std::list<std::string>::iterator it = std::find(WSingleton<CSharedDoc>::Instance()->m_ignoreList.begin(), WSingleton<CSharedDoc>::Instance()->m_ignoreList.end(), m_pNick->GetLine(1, false));
	if (it != WSingleton<CSharedDoc>::Instance()->m_ignoreList.end()) {
		// Already ignored: drop the nickname.
		WSingleton<CSharedDoc>::Instance()->m_ignoreList.erase(it);
		m_pIgnore->SetButtonImg("i58", FrButton::NORMAL);
	}
	else {
		WSingleton<CSharedDoc>::Instance()->m_ignoreList.push_back(m_pNick->GetLine(1, false));
		m_pIgnore->SetButtonImg("i59", FrButton::NORMAL);
	}
}

// friendinfodlg.cpp:91
void FrFriendInfoDlg::OnCloseBtnUp(void) {
	Close(true);
}

// friendinfodlg.cpp:97
bool FrFriendInfoDlg::OnInit(void) {
	FrWnd* pIdStatic = FindChildByName("s_id");
	if (pIdStatic) {
		pIdStatic->SetVisible(false);
	}
	if (m_pID) {
		m_pID->SetVisible(false);
	}

	FrWnd* wnds[8];
	wnds[0] = FindChildByName("s_nick");
	wnds[1] = FindChildByName("s_sex");
	wnds[2] = FindChildByName("s_server");
	wnds[3] = FindChildByName("s_channel");
	wnds[4] = m_pNick;
	wnds[5] = m_pSex;
	wnds[6] = m_pServer;
	wnds[7] = m_pChannel;

	for (int i = 0; i < 8; i++) {
		if (wnds[i]) {
			WRect rect;
			rect = wnds[i]->GetRect();
			rect.y -= 14.0f;
			wnds[i]->SetRect(rect);
		}
	}

	return true;
}
