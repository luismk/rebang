// Original: d:\build\custom\temp\code\client\projectg\addfrienddlg.cpp
#include <string.h>

#include "AddFriendDlg.h"
#include "ObjectFactory.h"
#include "NetworkSystem.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "GolfTask.h"

#include "EmoticonDlg.h"
#include "commonutil.h"
#include "frbutton.h"
#include "frdesktop.h"
#include "fredit.h"
#include "fresh.h"
#include "frwndmanager.h"

// addfrienddlg.cpp:22 (run-time type and factory registration of FrAddFriendDlg)
const WRTTI FrAddFriendDlg::m_RTTI("FrAddFriendDlg", &FrForm::m_RTTI);

IObject* FrAddFriendDlgMakeInstance(void) {
	return new FrAddFriendDlg;
}

struct __sFrAddFriendDlg {
	__sFrAddFriendDlg(void) {
		ObjectFactory().AddObjectFunctor(FrAddFriendDlgMakeInstance, "FrAddFriendDlg");
	}
};
static __sFrAddFriendDlg s_registerFrAddFriendDlg;

// addfrienddlg.cpp:24
const sFRESH_MSGMAP* FrAddFriendDlg::GetMessageMap(void) const {
	return &FrAddFriendDlg::_MsgMap;
}

sFRESH_MSGMAP FrAddFriendDlg::_MsgMap = { &FrForm::_MsgMap, &FrAddFriendDlg::_MsgEntries[0] };

sFRESH_ENTRY FrAddFriendDlg::_MsgEntries[] = {
	// addfrienddlg.cpp:26
	{ "id", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAddFriendDlg::OnEditNiclInit },
	// addfrienddlg.cpp:27
	{ "id", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrAddFriendDlg::OnEditNickEnterKey },
	// addfrienddlg.cpp:28
	{ "id", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrAddFriendDlg::OnEditNickDown },
	// addfrienddlg.cpp:29
	{ "nick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAddFriendDlg::OnDispNickInit },
	// addfrienddlg.cpp:30
	{ "check", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAddFriendDlg::OnCheckBtnUp },
	// addfrienddlg.cpp:31
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAddFriendDlg::OnOKBtnUp },
	// addfrienddlg.cpp:32
	{ "emoticon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAddFriendDlg::OnEmoticonInit },
	// addfrienddlg.cpp:33
	{ "emoticon", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAddFriendDlg::OnEmoticonUp },

	// addfrienddlg.cpp:35
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// addfrienddlg.cpp:39
void FrAddFriendDlg::OnEmoticonInit(intptr_t param) {
	m_pEmoticonBtn = DYNAMIC_CAST(FrButton, param);
}

// addfrienddlg.cpp:47
void FrAddFriendDlg::OnEmoticonUp(void) {
	if (m_pEmoticonBtn && !m_pEmoticonDlg) {
		m_pEmoticonDlg = CreateForm<FrEmoticonDlg>(g_pFresh->GetManager(), this, "emoticon", this);
		if (m_pEmoticonDlg) {
			WPoint pt(0, 0);
			const WRect& rect = m_pEmoticonBtn->GetRect();
			pt.x = rect.x - 200.0f;
			pt.y = rect.y + 24.0f;

			m_pEmoticonDlg->SetFixed(true);
			m_pEmoticonDlg->Open((FRESH_RESULT_CALLBACK)&FrAddFriendDlg::OnEmoticonResult, pt, 0);
		}
	}
}

// addfrienddlg.cpp:65
bool FrAddFriendDlg::OnEmoticonResult(int ret, FrForm* pForm) {
	if (m_pEmoticonDlg && ret == FrOK) {
		FrEmoticonDlg* pDlg = DYNAMIC_CAST(FrEmoticonDlg, pForm);
		if (pDlg) {
			const char* icon = pDlg->GetSelectedIcon();
			if (icon) {
				FrEdit* pEdit = DYNAMIC_CAST(FrEdit, FindChildByName("id"));
				if (m_pIdEdit) {
					const char* front = m_pIdEdit->GetEditText_Front();
					const char* comp = m_pIdEdit->GetEditText_Comp();
					const char* end = m_pIdEdit->GetEditText_End();
					m_pIdEdit->SetLine(1, MakeStr("%s%s%s%s", front, comp, icon, end), 0, false, 0);
					m_pIdEdit->SetKeyFocus(true);
				}
			}
		}
	} else {
		if (m_pIdEdit) {
			m_pIdEdit->SetKeyFocus(true);
		}
	}

	m_pEmoticonDlg = 0;
	return true;
}

// addfrienddlg.cpp:97
void FrAddFriendDlg::SetNick(const char* nick) {
	if (m_pNickEdit) {
		m_pNickEdit->SetLine(1, nick, 0, false, 0);
	}
	if (m_pIdEdit) {
		m_pIdEdit->SetLine(1, nick, 0, false, 0);
	}
}

// addfrienddlg.cpp:106
void FrAddFriendDlg::SetTargetNick(const char* nick) {
	if (m_pIdEdit) {
		m_pIdEdit->SetLine(1, nick, 0, false, 0);
	}
}

// addfrienddlg.cpp:114
void FrAddFriendDlg::SetFindUID(unsigned long uid) {
	m_findUID = uid;
}

// addfrienddlg.cpp:119
void FrAddFriendDlg::CheckRequestNickname(const char* nick) {
	if (nick == 0) {
		return;
	}

	if (strlen(nick) < 1) {
		return;
	}

	if (m_pIdEdit) {
		m_pIdEdit->SetLine(1, nick, 0, false, 0);
	}
}

// addfrienddlg.cpp:137
void FrAddFriendDlg::OnEditNiclInit(intptr_t param) {
	m_pIdEdit = DYNAMIC_CAST(FrEdit, param);

	m_pIdEdit->HidePrivacy(true);

	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		g_pFresh->GetManager()->SetExclusiveKey(true);
	}
}

// addfrienddlg.cpp:150
void FrAddFriendDlg::OnEditNickDown(void) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		g_pFresh->GetManager()->SetExclusiveKey(true);
	}
}

// addfrienddlg.cpp:158
bool FrAddFriendDlg::OnEditNickEnterKey(int param) {
	OnCheckBtnUp();
	return false;
}

// addfrienddlg.cpp:164
void FrAddFriendDlg::OnDispNickInit(intptr_t param) {
	m_pNickEdit = DYNAMIC_CAST(FrEdit, param);

	if (m_pNickEdit) {
		m_pNickEdit->HidePrivacy(true);
		m_pNickEdit->SetLine(1, "<\xC8\xAE\xC0\xCE \xBE\xC8\xB5\xCA>", 0, false, 0);
	}
}

// addfrienddlg.cpp:174
void FrAddFriendDlg::OnCheckBtnUp(void) {
	if (m_pNickEdit) {
		m_pNickEdit->ClearLine();
	}

	if (m_pIdEdit) {
		const char* nick = m_pIdEdit->GetLine(1, false);

		if (strcmp(nick, CSharedDoc::Instance()->GetPangyaUserInfo().nickname) == 0) {
			AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x23,
				(intptr_t)"\xB4\xD9\xB8\xA5 \xBB\xE7\xB6\xF7\xC0\xC7 \xB4\xD0\xB3\xD7\xC0\xD3\xC0\xBB \xC0\xD4\xB7\xC2\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4.", 0, 0, 0, 0));
			return;
		}

		int len = strlen(nick);
		if (nick && len > 0 && len < 0x16) {
			if (WSingleton<WNetworkSystem>::Instance()->IsConnected((WNetworkSystem::eNetUnit)1)) {
				WSendPacket packet(0x17);
				packet.EncodeStr(std::string(nick));
				packet.Send((eSendTo)1);
			} else {
				WSendPacket packet((enumClientPacket)7);
				packet.Encode1(0);
				packet.EncodeStr(std::string(nick));
				packet.Send(SEND_TO_DEFAULT);

				AfxGetTask()->GetActor("Lobby") << MsgObject(0, 1, 0, 0, 0, 0, 0);
			}
		} else {
			AfxGetTask()->PassMsgToMainActor(MsgObject(0, 0x23,
				(intptr_t)"\xC4\xA3\xB1\xB8\xC0\xC7 \xB4\xEB\xC8\xAD\xB8\xED\xC0\xBB \xB3\xD6\xBE\xEE\xC1\xD6\xBC\xBC\xBF\xE4", 0, 0, 0, 0));
		}
	}
}

// addfrienddlg.cpp:229
void FrAddFriendDlg::OnOKBtnUp(void) {
	if (m_pIdEdit) {
		if (strlen(m_pIdEdit->GetLine(1, false)) == 0) {
			AfxGetTask()->PassMsgToMainActor(MsgObject(0, 0x23,
				(intptr_t)"\xC4\xA3\xB1\xB8\xC0\xC7 \xB4\xEB\xC8\xAD\xB8\xED\xC0\xBB \xB3\xD6\xBE\xEE\xC1\xD6\xBC\xBC\xBF\xE4", 0, 0, 0, 0));
			return;
		}

		if (strcmp(m_pIdEdit->GetLine(1, false), m_pNickEdit->GetLine(1, false)) != 0) {
			AfxGetTask()->PassMsgToMainActor(MsgObject(0, 0x23,
				(intptr_t)"\xBA\xB8\xB3\xBB\xB1\xE2 \xC0\xFC\xBF\xA1 \xB9\xDE\xB4\xC2 \xC0\xCC\xB0\xA1 \xC8\xAE\xBD\xC7\xC8\xF7 \xB8\xC2\xB4\xC2\xC1\xF6\n\xB2\xC0 \\c0xffff0000\\c'\xC8\xAE\xC0\xCE'\\c0xff000000\\c \xB9\xF6\xC6\xB0\xC0\xBB \xB4\xAD\xB7\xAF \xC8\xAE\xC0\xCE\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4", 0, 0, 0, 0));
			return;
		}

		if (WSingleton<WNetworkSystem>::Instance()->IsConnected((WNetworkSystem::eNetUnit)1)) {
			WSendPacket packet(0x18);
			packet.Encode4(m_findUID);
			packet.EncodeStr(std::string(m_pIdEdit->GetLine(1, false)));
			packet.Send((eSendTo)1);
		} else {
			AfxGetTask()->PassMsgToMainActor(MsgObject(0, 0x23,
				(intptr_t)"\xB8\xDE\xBD\xC5\xC0\xFA \xBC\xAD\xB9\xF6\xBF\xCD \xBF\xAC\xB0\xE1\xC0\xCC \xB5\xC7\xC1\xF6 \xBE\xCA\xBD\xC0\xB4\xCF\xB4\xD9. \xB4\xD9\xC0\xBD\xBF\xA1 \xB4\xD9\xBD\xC3 \xBD\xC3\xB5\xB5\xC7\xCF\xBC\xBC\xBF\xE4.", 0, 0, 0, 0));
		}
	}

	OnFreshOkay();
}

// addfrienddlg.cpp:303
bool FrAddFriendDlg::OnInit(void) {
	SetMessage("\xC3\xA3\xC0\xBB \xC4\xA3\xB1\xB8 \xB4\xEB\xC8\xAD\xB8\xED\xBF\xA1 \xC4\xA3\xB1\xB8\xB0\xA1 \xB5\xC7\xB0\xED \xBD\xCD\xC0\xBA \xB4\xEB\xC8\xAD\xB8\xED\xC0\xBB \xC0\xD4\xB7\xC2\xC7\xCF\xB8\xE9 \xC1\xA2\xBC\xD3\xC1\xDF\xC0\xCC\xC1\xF6 \xBE\xCA\xC0\xBA \xC0\xAF\xC0\xFA\xBF\xA1\xB0\xD4\xB5\xB5 \xC4\xA3\xB1\xB8 \xBF\xE4\xC3\xBB\xC0\xCC \xB0\xA1\xB4\xC9\xC7\xD5\xB4\xCF\xB4\xD9.", false);

	return FrForm::OnInit();
}

// addfrienddlg.cpp:322
void FrAddFriendDlg::OnProc(const float dt) {
	if (!m_bFocused) {
		if (m_elapsed > 0.3f) {
			m_pWndManager->GetDesktop()->ResetKeyFocus();
			m_pIdEdit->SetKeyFocus(true);
			m_bFocused = true;
		}
		m_elapsed += dt;
	}
}

// addfrienddlg.cpp:337
void FrAddFriendDlg::OnCancel(void) {
	if (m_pEmoticonDlg) {
		m_pEmoticonDlg->Close(FrCANCEL, true);
	}

	FrForm::OnCancel();
}
