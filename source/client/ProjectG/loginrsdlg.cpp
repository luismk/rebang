// Original: d:\build\custom\temp\code\client\projectg\loginrsdlg.cpp
#include "LoginRsDlg.h"
#include "ObjectFactory.h"
#include "frbutton.h"
#include "NetworkSystem.h"
#include "RankingDlg.h"
#include "user_info.h"
#include "commonutil.h"

// loginrsdlg.cpp:22 (run-time type and factory registration of FrLoginRsDlg)
const WRTTI FrLoginRsDlg::m_RTTI("FrLoginRsDlg", &FrForm::m_RTTI);

IObject* FrLoginRsDlgMakeInstance(void) {
	return new FrLoginRsDlg;
}

struct __sFrLoginRsDlg {
	__sFrLoginRsDlg(void) {
		ObjectFactory().AddObjectFunctor(FrLoginRsDlgMakeInstance, "FrLoginRsDlg");
	}
};
static __sFrLoginRsDlg s_registerFrLoginRsDlg;

// loginrsdlg.cpp:24
const sFRESH_MSGMAP* FrLoginRsDlg::GetMessageMap(void) const {
	return &FrLoginRsDlg::_MsgMap;
}

sFRESH_MSGMAP FrLoginRsDlg::_MsgMap = { &FrForm::_MsgMap, &FrLoginRsDlg::_MsgEntries[0] };

sFRESH_ENTRY FrLoginRsDlg::_MsgEntries[] = {
	// loginrsdlg.cpp:26
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginRsDlg::OnCancelInit },

	// loginrsdlg.cpp:28
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

eLoginToRs FrLoginRsDlg::m_connState;

// loginrsdlg.cpp:35
FrLoginRsDlg::FrLoginRsDlg(void) {
	m_pCancel = 0;
	m_elapsed = 0;
	m_connState = LOGIN_TO_RS_NONE;
}

// loginrsdlg.cpp:42
void FrLoginRsDlg::SetState(eLoginToRs state) {
	m_connState = state;
}

// loginrsdlg.cpp:47
eLoginToRs FrLoginRsDlg::GetState(void) {
	return m_connState;
}

// loginrsdlg.cpp:53
void FrLoginRsDlg::OnProc(const float deltaTime) {
	static float s_spinTime = 0.0f;
	static float s_waitTime = 1.8f;

	m_elapsed += deltaTime;
	if (m_elapsed > 15.0f) {
		if (m_pCancel) {
			m_pCancel->Enable(true);
		}
		SetMessage("\xc1\xf6\xb1\xdd\xc0\xba \xb7\xa9\xc5\xb7 \xbc\xad\xb9\xf6\xbf\xa1 \xc1\xa2\xbc\xd3\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", false);
	} else {
		char spin[3] = { '-', '/', '|' };
		SetMessage(MakeStr("\xb7\xa9\xc5\xb7 \xbc\xad\xb9\xf6\xbf\xa1 \xc1\xa2\xbc\xd3\xc7\xcf\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9. %c", spin[(int)s_spinTime % 3]), false);
		s_spinTime += deltaTime * 10.0f;
	}

	if (m_connState < LOGIN_TO_RS_CONNECTED) {
		WNetworkSystem* pNet = WNetworkSystem::Instance();
		if (pNet) {
			if (m_connState == LOGIN_TO_RS_CONNECTING && pNet->IsConnectionComplete(WNetworkSystem::NET_UNIT_RANK)) {
				if (!pNet->IsConnected(WNetworkSystem::NET_UNIT_RANK)) {
					m_elapsed = 16.0f;
					SetMessage("\xc1\xf6\xb1\xdd\xc0\xba \xb7\xa9\xc5\xb7 \xbc\xad\xb9\xf6\xbf\xa1 \xc1\xa2\xbc\xd3\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", false);
					m_connState = LOGIN_TO_RS_DONE;
				} else {
					s_waitTime = 1.8f;
					m_connState = LOGIN_TO_RS_CONNECTED;
				}
			}
		}
	} else if (m_connState == LOGIN_TO_RS_CONNECTED) {
		s_waitTime -= deltaTime;
		if (s_waitTime < 0.0f) {
			m_connState = LOGIN_TO_RS_DONE;
			if (CUserInfo::Instance()) {
				CUserInfo::Instance()->Close();
			}
			Close(true);
			CRankingInfo::Instance()->Open();
		}
	}
}

// loginrsdlg.cpp:119
void FrLoginRsDlg::OnCancelInit(intptr_t param) {
	m_pCancel = DYNAMIC_CAST(FrButton, param);
	if (m_pCancel) {
		m_pCancel->Enable(false);
	}
}
