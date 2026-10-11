// Original: d:\build\custom\temp\code\client\projectg\mascotinfodlg.cpp
#include <string.h>
#include "MascotInfoDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frstatic.h"
#include "logininfo.h"
#include "mascot.h"
#include "commonutil.h"
#include "SharedDoc.h"
#include "../shared/classdefine.h"
#include "Packet.h"
#include "TaskManager.h"
#include "actor.h"
#include "binstr.h"
#include "fresh.h"
#include "frwndmanager.h"

// mascotinfodlg.cpp:21 (run-time type and factory registration of FrMascotInfoDlg)
const WRTTI FrMascotInfoDlg::m_RTTI("FrMascotInfoDlg", &FrForm::m_RTTI);

IObject* FrMascotInfoDlgMakeInstance(void) {
	return new FrMascotInfoDlg;
}

struct __sFrMascotInfoDlg {
	__sFrMascotInfoDlg(void) {
		ObjectFactory().AddObjectFunctor(FrMascotInfoDlgMakeInstance, "FrMascotInfoDlg");
	}
};
static __sFrMascotInfoDlg s_registerFrMascotInfoDlg;

// mascotinfodlg.cpp:23
const sFRESH_MSGMAP* FrMascotInfoDlg::GetMessageMap(void) const {
	return &FrMascotInfoDlg::_MsgMap;
}

sFRESH_MSGMAP FrMascotInfoDlg::_MsgMap = { &FrForm::_MsgMap, &FrMascotInfoDlg::_MsgEntries[0] };

sFRESH_ENTRY FrMascotInfoDlg::_MsgEntries[] = {
	// mascotinfodlg.cpp:25
	{ "portrait", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMascotInfoDlg::OnPortraitInit },
	// mascotinfodlg.cpp:26
	{ "vacation", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMascotInfoDlg::OnVacationInit },
	// mascotinfodlg.cpp:27
	{ "limit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMascotInfoDlg::OnLimitInit },
	// mascotinfodlg.cpp:28
	{ "name", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMascotInfoDlg::OnNameInit },
	// mascotinfodlg.cpp:29
	{ "text_message", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMascotInfoDlg::OnTextMessageInit },
	// mascotinfodlg.cpp:30
	{ "mascot_msg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMascotInfoDlg::OnMascotMsgInit },
	// mascotinfodlg.cpp:31
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMascotInfoDlg::OnOkInit },
	// mascotinfodlg.cpp:32
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMascotInfoDlg::OnOkBtnUp },
	// mascotinfodlg.cpp:33
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMascotInfoDlg::OnCancelInit },
	// mascotinfodlg.cpp:34
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMascotInfoDlg::OnCancelBtnUp },

	// mascotinfodlg.cpp:36
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// mascotinfodlg.cpp:40
FrMascotInfoDlg::FrMascotInfoDlg(void) {
	memset(&m_info, 0, sizeof(m_info));
	m_pPortrait = 0;
	m_pVacation = 0;
	m_pLimit = 0;
	m_pName = 0;
	m_pMascotMsg = 0;
	m_pOk = 0;
	m_pCancel = 0;
	m_pConfirmDlg = 0;
	m_pPuppet = 0;
}

// mascotinfodlg.cpp:57
void FrMascotInfoDlg::SetMascotInfo(const sMascotInfo& info, WPuppet* pPuppet) {


	m_info = info;
	m_msg = m_info.message;
	m_pPuppet = pPuppet;
}

// mascotinfodlg.cpp:75
void FrMascotInfoDlg::OnPortraitInit(intptr_t param) {
	m_pPortrait = DYNAMIC_CAST(FrArea, param);
}

// mascotinfodlg.cpp:80
void FrMascotInfoDlg::OnVacationInit(intptr_t param) {
	m_pVacation = DYNAMIC_CAST(FrArea, param);

	if (m_pVacation) {
		m_pVacation->SetVisible(false);
	}
}

// mascotinfodlg.cpp:88
void FrMascotInfoDlg::OnLimitInit(intptr_t param) {
	m_pLimit = DYNAMIC_CAST(FrArea, param);


	if (m_pLimit) {
		m_pLimit->SetVisible(false);
	}
}

// mascotinfodlg.cpp:96
void FrMascotInfoDlg::OnNameInit(intptr_t param) {
	m_pName = DYNAMIC_CAST(FrEdit, param);
}

// mascotinfodlg.cpp:101
void FrMascotInfoDlg::OnTextMessageInit(intptr_t param) {
	m_pTextMessage = DYNAMIC_CAST(FrStatic, param);


	if (m_pTextMessage) {
		m_pTextMessage->SetVisible(false);
	}
}

// mascotinfodlg.cpp:109
void FrMascotInfoDlg::OnMascotMsgInit(intptr_t param) {
	m_pMascotMsg = DYNAMIC_CAST(FrEdit, param);


	if (m_pMascotMsg) {
		m_pMascotMsg->SetVisible(false);
	}
}

// mascotinfodlg.cpp:119
void FrMascotInfoDlg::OnOkInit(intptr_t param) {
	m_pOk = DYNAMIC_CAST(FrButton, param);


	if (m_pOk) {
		m_pOk->Enable(false);
	}
}

// mascotinfodlg.cpp:127
void FrMascotInfoDlg::OnOkBtnUp(void) {
	if (strcmp(m_msg.c_str(), m_info.message) != 0) {

		if (m_info.typeId == 0x40000004) {
			if (strlen(m_msg.c_str()) > 8) {
				AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)"\xB8\xDE\xBC\xBC\xC1\xF6 \xC3\xD6\xB4\xEB \xB1\xE6\xC0\xCC\xB4\xC2 \xBF\xB5\xB9\xAE 8\xB1\xDB\xC0\xDA, \xC7\xD1\xB1\xDB 4\xB1\xDB\xC0\xDA \xC0\xD4\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
				return;
			}
		}
		else {
			if (strlen(m_msg.c_str()) > 16) {
				AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)"\xB8\xDE\xBC\xBC\xC1\xF6 \xC3\xD6\xB4\xEB \xB1\xE6\xC0\xCC\xB4\xC2 \xBF\xB5\xB9\xAE 16\xB1\xDB\xC0\xDA, \xC7\xD1\xB1\xDB 8\xB1\xDB\xC0\xDA \xC0\xD4\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
				return;
			}
		}

		cTokenV token;
		token.Init(m_msg.c_str(), strlen(m_msg.c_str()));
		int count = token.GetTokenNum(0, "'", 1) - 1;

		if (count > 0) {
			AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)"[']\xB4\xC2 \xB8\xDE\xBC\xBC\xC1\xF6\xBF\xA1 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
		}
		else {
			char text[256];
			strcpy(text, m_msg.c_str());

			if (WSingleton<CSharedDoc>::Instance()->m_chatManager.Filtering(text)) {
				AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)"\xC0\xFB\xB4\xE7\xC7\xCF\xC1\xF6\xBE\xCA\xC0\xBA \xB4\xDC\xBE\xEE\xB0\xA1 \xC6\xF7\xC7\xD4\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
			}
			else {
				if (!m_pConfirmDlg) {
					m_pConfirmDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "mascot_msg", 0);
				}

				if (m_pConfirmDlg) {
					m_pConfirmDlg->SetMessage("\xB8\xB6\xBD\xBA\xC4\xDA\xC6\xAE \xB8\xDE\xBC\xBC\xC1\xF6\xB8\xA6 \xBA\xAF\xB0\xE6 \xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
					m_pConfirmDlg->Open((FRESH_RESULT_CALLBACK)&FrMascotInfoDlg::OnChangeMsgConfirmDlgResult, 1);
				}
			}
		}
	}
}

// mascotinfodlg.cpp:274
void FrMascotInfoDlg::OnCancelInit(intptr_t param) {
	m_pCancel = DYNAMIC_CAST(FrButton, param);
}

// mascotinfodlg.cpp:279
void FrMascotInfoDlg::OnCancelBtnUp(void) {
	if (strcmp(m_info.message, m_msg.c_str()) != 0) {

		IFF_STRUCT::sMascot* pMascot = ItemManager()->FindMascot(m_info.typeId);

		if (m_pPuppet && pMascot) {
			CMascot::ChangeMascotMsgInTex(m_info.message, pMascot, m_pPuppet, MakeStr("myroom%02d", pMascot->m_typeId & 0x3FFFFFF));
		}
	}

	OnFreshCancel();
}

// mascotinfodlg.cpp:292
bool FrMascotInfoDlg::OnChangeMsgConfirmDlgResult(int result, FrForm* pForm) {
	m_pConfirmDlg = 0;

	if (result == 1) {

		if (CLoginInfo::Instance()->IsPcBang() && m_info.pcBang) {

			WSendPacket packet((enumClientPacket)0x97);
			packet.Encode1(1);
			packet.Encode4(m_info.id);
			packet.EncodeStr(m_msg);
			packet.Send(SEND_TO_DEFAULT);
		}
		else {
			WSendPacket packet((enumClientPacket)0x73);
			packet.Encode4(m_info.id);
			packet.EncodeStr(m_msg);
			packet.Send(SEND_TO_DEFAULT);
		}
		AfxGetTask()->PassMsgToActor("RealMyRoom", MsgObject(0, 1, 0, 0, 0, 0, 0));

		Close(FrOK, true);
	}



	return true;
}

// mascotinfodlg.cpp:506
void FrMascotInfoDlg::OnProc(const float dt) {
	if (m_pMascotMsg) {

		if (stricmp(m_pMascotMsg->GetLine(1, false), m_msg.c_str()) != 0) {

			m_msg = m_pMascotMsg->GetLine(1, false);

			IFF_STRUCT::sMascot* pMascot = ItemManager()->FindMascot(m_info.typeId);

			if (m_pPuppet && pMascot) {

				CMascot::ChangeMascotMsgInTex(m_msg.c_str(), pMascot, m_pPuppet, MakeStr("myroom%02d", pMascot->m_typeId & 0x3FFFFFF));
			}
		}
	}
}
