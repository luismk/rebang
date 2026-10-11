// Original: d:\build\custom\temp\code\client\projectg\dlgquickstartsetting.cpp
#include "DlgQuickStartSetting.h"
#include "ObjectFactory.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "frarea.h"
#include "frbutton.h"
#include "frcombobox.h"
#include "fresh.h"
#include "frwndmanager.h"
#include <string.h>
#include <algorithm>

template <> CMatchingSystem* WSingleton<CMatchingSystem>::m_pInstance = NULL;

// dlgquickstartsetting.cpp:14 (run-time type and factory registration of FrQuickResultDlg)
const WRTTI FrQuickResultDlg::m_RTTI("FrQuickResultDlg", &FrForm::m_RTTI);

IObject* FrQuickResultDlgMakeInstance(void) {
	return new FrQuickResultDlg;
}

struct __sFrQuickResultDlg {
	__sFrQuickResultDlg(void) {
		ObjectFactory().AddObjectFunctor(FrQuickResultDlgMakeInstance, "FrQuickResultDlg");
	}
};
static __sFrQuickResultDlg s_registerFrQuickResultDlg;

// dlgquickstartsetting.cpp:16
const sFRESH_MSGMAP* FrQuickResultDlg::GetMessageMap(void) const {
	return &FrQuickResultDlg::_MsgMap;
}

sFRESH_MSGMAP FrQuickResultDlg::_MsgMap = { &FrForm::_MsgMap, &FrQuickResultDlg::_MsgEntries[0] };

sFRESH_ENTRY FrQuickResultDlg::_MsgEntries[] = {
	// dlgquickstartsetting.cpp:18
	{ "ok1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickResultDlg::OnOkBtnInit },
	// dlgquickstartsetting.cpp:19
	{ "ok1", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuickResultDlg::OnOkBtnUp },

	// dlgquickstartsetting.cpp:22
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// dlgquickstartsetting.cpp:25
FrQuickResultDlg::FrQuickResultDlg(void) {
}

// dlgquickstartsetting.cpp:29
FrQuickResultDlg::~FrQuickResultDlg(void) {

}

// dlgquickstartsetting.cpp:36
void FrQuickResultDlg::OnOkBtnInit(intptr_t param) {
	m_pOkBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pOkBtn) {

		m_pOkBtn->Enable(false);
	}

}

// dlgquickstartsetting.cpp:45
void FrQuickResultDlg::OnOkBtnUp(void) {
	OnOK();
}

// dlgquickstartsetting.cpp:50
void FrQuickResultDlg::SetContents(const char* contents) {
	if (contents) {


		SetMessage(contents, false);
	}
}

// dlgquickstartsetting.cpp:58
void FrQuickResultDlg::SetOkbtnEnable(bool bEnable) {
	if (m_pOkBtn) {

		m_pOkBtn->Enable(bEnable);
	}

}

// dlgquickstartsetting.cpp:67 (run-time type and factory registration of FrQuickStartDlg)
const WRTTI FrQuickStartDlg::m_RTTI("FrQuickStartDlg", &FrForm::m_RTTI);

IObject* FrQuickStartDlgMakeInstance(void) {
	return new FrQuickStartDlg;
}

struct __sFrQuickStartDlg {
	__sFrQuickStartDlg(void) {
		ObjectFactory().AddObjectFunctor(FrQuickStartDlgMakeInstance, "FrQuickStartDlg");
	}
};
static __sFrQuickStartDlg s_registerFrQuickStartDlg;

// dlgquickstartsetting.cpp:69
const sFRESH_MSGMAP* FrQuickStartDlg::GetMessageMap(void) const {
	return &FrQuickStartDlg::_MsgMap;
}

sFRESH_MSGMAP FrQuickStartDlg::_MsgMap = { &FrForm::_MsgMap, &FrQuickStartDlg::_MsgEntries[0] };

sFRESH_ENTRY FrQuickStartDlg::_MsgEntries[] = {
	// dlgquickstartsetting.cpp:72
	{ "stroke", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartDlg::OnStrokeBtnInit },
	// dlgquickstartsetting.cpp:73
	{ "stroke", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuickStartDlg::OnStrokeBtnUp },
	// dlgquickstartsetting.cpp:74
	{ "strokestop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartDlg::OnStrokeStopInit },

	// dlgquickstartsetting.cpp:76
	{ "tournament", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartDlg::OnTournamentBtnInit },
	// dlgquickstartsetting.cpp:77
	{ "tournament", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuickStartDlg::OnTournamentBtnUp },
	// dlgquickstartsetting.cpp:78
	{ "tournamentstop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartDlg::OnTournamenStopInit },

	// dlgquickstartsetting.cpp:80
	{ "battle", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartDlg::OnBattleBtnInit },
	// dlgquickstartsetting.cpp:81
	{ "battle", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuickStartDlg::OnBattleBtnUp },
	// dlgquickstartsetting.cpp:82
	{ "battlestop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartDlg::OnBattleStopInit },

	// dlgquickstartsetting.cpp:84
	{ "chat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartDlg::OnChatBtnInit },
	// dlgquickstartsetting.cpp:85
	{ "chat", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuickStartDlg::OnChatBtnUp },
	// dlgquickstartsetting.cpp:86
	{ "chatstop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartDlg::OnChatStopInit },

	// dlgquickstartsetting.cpp:88
	{ "option", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartDlg::OnOptionBtnInit },
	// dlgquickstartsetting.cpp:89
	{ "option", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuickStartDlg::OnOptionBtnUp },


	// dlgquickstartsetting.cpp:92
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// dlgquickstartsetting.cpp:95
FrQuickStartDlg::FrQuickStartDlg(void) {
	m_pSettingDlg = 0;
}

// dlgquickstartsetting.cpp:100
FrQuickStartDlg::~FrQuickStartDlg(void) {

}

// dlgquickstartsetting.cpp:105
bool FrQuickStartDlg::OnInit(void) {


	// dlgquickstartsetting.cpp:109
	bool bStroke = std::find(CMatchingSystem::Instance()->m_myIntrests.begin(), CMatchingSystem::Instance()->m_myIntrests.end(), (unsigned char)0) != CMatchingSystem::Instance()->m_myIntrests.end();
	if (m_pStrokeBtn) {

		m_pStrokeBtn->Enable(bStroke);
		m_pStrokeBtn->SetVisible(bStroke);
	}

	if (m_pStrokeStop) {

		if (bStroke) m_pStrokeStop->SetVisible(false); else m_pStrokeStop->SetVisible(true);
	}

	// dlgquickstartsetting.cpp:122
	bool bTournament = std::find(CMatchingSystem::Instance()->m_myIntrests.begin(), CMatchingSystem::Instance()->m_myIntrests.end(), (unsigned char)4) != CMatchingSystem::Instance()->m_myIntrests.end();
	if (m_pTournamentBtn) {

		m_pTournamentBtn->Enable(bTournament);
		m_pTournamentBtn->SetVisible(bTournament);
	}

	if (m_pTournamentStop) {

		if (bTournament) m_pTournamentStop->SetVisible(false); else m_pTournamentStop->SetVisible(true);
	}

	// dlgquickstartsetting.cpp:135
	bool bBattle = std::find(CMatchingSystem::Instance()->m_myIntrests.begin(), CMatchingSystem::Instance()->m_myIntrests.end(), (unsigned char)10) != CMatchingSystem::Instance()->m_myIntrests.end();
	if (m_pBattleBtn) {

		m_pBattleBtn->Enable(bBattle);
		m_pBattleBtn->SetVisible(bBattle);
	}

	if (m_pBattleStop) {

		if (bBattle) m_pBattleStop->SetVisible(false); else m_pBattleStop->SetVisible(true);
	}

	// dlgquickstartsetting.cpp:148
	bool bChat = std::find(CMatchingSystem::Instance()->m_myIntrests.begin(), CMatchingSystem::Instance()->m_myIntrests.end(), (unsigned char)2) != CMatchingSystem::Instance()->m_myIntrests.end();
	if (m_pChatBtn) {

		m_pChatBtn->Enable(bChat);
		m_pChatBtn->SetVisible(bChat);
	}

	if (m_pChatStop) {

		if (bChat) m_pChatStop->SetVisible(false); else m_pChatStop->SetVisible(true);
	}


	return true;
}

// dlgquickstartsetting.cpp:164
void FrQuickStartDlg::OnStrokeStopInit(intptr_t param) {
	m_pStrokeStop = DYNAMIC_CAST(FrArea, param);
}

// dlgquickstartsetting.cpp:169
void FrQuickStartDlg::OnTournamenStopInit(intptr_t param) {
	m_pTournamentStop = DYNAMIC_CAST(FrArea, param);
}

// dlgquickstartsetting.cpp:174
void FrQuickStartDlg::OnBattleStopInit(intptr_t param) {
	m_pBattleStop = DYNAMIC_CAST(FrArea, param);
}

// dlgquickstartsetting.cpp:179
void FrQuickStartDlg::OnChatStopInit(intptr_t param) {
	m_pChatStop = DYNAMIC_CAST(FrArea, param);
}

// dlgquickstartsetting.cpp:184
void FrQuickStartDlg::OnStrokeBtnInit(intptr_t param) {
	m_pStrokeBtn = DYNAMIC_CAST(FrButton, param);
}

// dlgquickstartsetting.cpp:189
void FrQuickStartDlg::OnStrokeBtnUp(void) {
	if (m_pStrokeBtn->IsEnabled()) {

		SendRequestMatching(0);
		OnOK();
	}

}

// dlgquickstartsetting.cpp:198
void FrQuickStartDlg::OnTournamentBtnInit(intptr_t param) {
	m_pTournamentBtn = DYNAMIC_CAST(FrButton, param);



}

// dlgquickstartsetting.cpp:206
void FrQuickStartDlg::OnTournamentBtnUp(void) {
	if (m_pTournamentBtn->IsEnabled()) {

		SendRequestMatching(4);
		OnOK();
	}

}

// dlgquickstartsetting.cpp:215
void FrQuickStartDlg::OnBattleBtnInit(intptr_t param) {
	m_pBattleBtn = DYNAMIC_CAST(FrButton, param);



}

// dlgquickstartsetting.cpp:223
void FrQuickStartDlg::OnBattleBtnUp(void) {
	if (m_pBattleBtn->IsEnabled()) {

		SendRequestMatching(10);
		OnOK();
	}

}

// dlgquickstartsetting.cpp:232
void FrQuickStartDlg::OnChatBtnInit(intptr_t param) {
	m_pChatBtn = DYNAMIC_CAST(FrButton, param);



}

// dlgquickstartsetting.cpp:240
void FrQuickStartDlg::OnChatBtnUp(void) {
	if (m_pChatBtn->IsEnabled()) {

		SendRequestMatching(2);
		OnOK();
	}

}

// dlgquickstartsetting.cpp:249
void FrQuickStartDlg::OnOptionBtnInit(intptr_t param) {
	m_pOptionBtn = DYNAMIC_CAST(FrButton, param);



}

// dlgquickstartsetting.cpp:257
void FrQuickStartDlg::OnOptionBtnUp(void) {
	m_pSettingDlg = CreateForm<FrQuickStartSettingDlg>(g_pFresh->GetManager(), this, "quick_start_setting", 0);

	if (m_pSettingDlg) {


		m_pSettingDlg->Open((FRESH_RESULT_CALLBACK)&FrQuickStartDlg::OnQuickStartSettingDlgResult, WPoint(468.0f, 158.0f), 0xc0);
	}
}

// dlgquickstartsetting.cpp:267
bool FrQuickStartDlg::OnQuickStartSettingDlgResult(int ret, FrForm* pForm) {
	switch (ret) {
	case FrOK:


		m_pSettingDlg->UpdateData();
		OnInit();
		break;
	}










	m_pSettingDlg = 0;

	return true;
}

// dlgquickstartsetting.cpp:290
void FrQuickStartDlg::SendRequestMatching(unsigned char code) {
	if (code != 16) {

		CSharedDoc::Instance()->SetUserInfoMod(3);

		WSendPacket packet((enumClientPacket)0xb7);
		packet.Encode1(code);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// dlgquickstartsetting.cpp:303 (run-time type and factory registration of FrQuickStartSettingDlg)
const WRTTI FrQuickStartSettingDlg::m_RTTI("FrQuickStartSettingDlg", &FrForm::m_RTTI);

IObject* FrQuickStartSettingDlgMakeInstance(void) {
	return new FrQuickStartSettingDlg;
}

struct __sFrQuickStartSettingDlg {
	__sFrQuickStartSettingDlg(void) {
		ObjectFactory().AddObjectFunctor(FrQuickStartSettingDlgMakeInstance, "FrQuickStartSettingDlg");
	}
};
static __sFrQuickStartSettingDlg s_registerFrQuickStartSettingDlg;

// dlgquickstartsetting.cpp:305
const sFRESH_MSGMAP* FrQuickStartSettingDlg::GetMessageMap(void) const {
	return &FrQuickStartSettingDlg::_MsgMap;
}

sFRESH_MSGMAP FrQuickStartSettingDlg::_MsgMap = { &FrForm::_MsgMap, &FrQuickStartSettingDlg::_MsgEntries[0] };

sFRESH_ENTRY FrQuickStartSettingDlg::_MsgEntries[] = {
	// dlgquickstartsetting.cpp:308
	{ "chk_stroke", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartSettingDlg::OnChkStrokeBtnInit },
	// dlgquickstartsetting.cpp:309
	{ "chk_stroke", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuickStartSettingDlg::OnChkStrokeBtnUp },
	// dlgquickstartsetting.cpp:310
	{ "chk_tournament", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartSettingDlg::OnChkTournamentBtnInit },
	// dlgquickstartsetting.cpp:311
	{ "chk_tournament", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuickStartSettingDlg::OnChkTournamentBtnUp },
	// dlgquickstartsetting.cpp:312
	{ "chk_battle", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartSettingDlg::OnChkBattleBtnInit },
	// dlgquickstartsetting.cpp:313
	{ "chk_battle", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuickStartSettingDlg::OnChkBattleBtnUp },
	// dlgquickstartsetting.cpp:314
	{ "chk_chat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartSettingDlg::OnChkChatBtnInit },
	// dlgquickstartsetting.cpp:315
	{ "chk_chat", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrQuickStartSettingDlg::OnChkChatBtnUp },

	// dlgquickstartsetting.cpp:317
	{ "combo_condition_1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartSettingDlg::OnCmbCondition1BoxInit },
	// dlgquickstartsetting.cpp:318
	{ "combo_condition_2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartSettingDlg::OnCmbCondition2BoxInit },
	// dlgquickstartsetting.cpp:319
	{ "combo_condition_3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickStartSettingDlg::OnCmbCondition3BoxInit },

	// dlgquickstartsetting.cpp:321
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// dlgquickstartsetting.cpp:330
FrQuickStartSettingDlg::FrQuickStartSettingDlg(void) {


	for (int i = 0; i < 16; i++) m_checked[i] = 0;

}

// dlgquickstartsetting.cpp:338
FrQuickStartSettingDlg::~FrQuickStartSettingDlg(void) {

}

// dlgquickstartsetting.cpp:344
void FrQuickStartSettingDlg::OnChkStrokeBtnInit(intptr_t param) {
	m_pChkStrokeBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pChkStrokeBtn) {

		CMatchingSystem* pSystem = CMatchingSystem::Instance();


		if (pSystem) {

			if (pSystem->CheckIntrestCode(0)) {

				m_pChkStrokeBtn->SetStatus(FrButton::PRESSED);
				m_checked[0] = 1;



			} else {
				m_pChkStrokeBtn->SetStatus(FrButton::NORMAL);
			}
		}
	}
}

// dlgquickstartsetting.cpp:368
void FrQuickStartSettingDlg::OnChkStrokeBtnUp(void) {



	if (CMatchingSystem::Instance() && m_pChkStrokeBtn) {

		m_checked[0] = m_pChkStrokeBtn->GetStatus() == FrButton::PRESSED;
	}

}

// dlgquickstartsetting.cpp:380
void FrQuickStartSettingDlg::OnChkTournamentBtnInit(intptr_t param) {
	m_pChkTournamentBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pChkTournamentBtn) {

		CMatchingSystem* pSystem = CMatchingSystem::Instance();


		if (pSystem) {

			if (pSystem->CheckIntrestCode(4)) {

				m_pChkTournamentBtn->SetStatus(FrButton::PRESSED);
				m_checked[4] = 1;



			} else {
				m_pChkTournamentBtn->SetStatus(FrButton::NORMAL);
			}
		}
	}
}

// dlgquickstartsetting.cpp:404
void FrQuickStartSettingDlg::OnChkTournamentBtnUp(void) {



	if (CMatchingSystem::Instance() && m_pChkTournamentBtn) {

		m_checked[4] = m_pChkTournamentBtn->GetStatus() == FrButton::PRESSED;
	}

}

// dlgquickstartsetting.cpp:416
void FrQuickStartSettingDlg::OnChkBattleBtnInit(intptr_t param) {
	m_pChkBattleBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pChkBattleBtn) {

		CMatchingSystem* pSystem = CMatchingSystem::Instance();


		if (pSystem) {

			if (pSystem->CheckIntrestCode(10)) {

				m_pChkBattleBtn->SetStatus(FrButton::PRESSED);
				m_checked[10] = 1;



			} else {
				m_pChkBattleBtn->SetStatus(FrButton::NORMAL);
			}
		}
	}
}

// dlgquickstartsetting.cpp:440
void FrQuickStartSettingDlg::OnChkBattleBtnUp(void) {



	if (CMatchingSystem::Instance() && m_pChkBattleBtn) {

		m_checked[10] = m_pChkBattleBtn->GetStatus() == FrButton::PRESSED;
	}

}

// dlgquickstartsetting.cpp:452
void FrQuickStartSettingDlg::OnChkChatBtnInit(intptr_t param) {
	m_pChkChatBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pChkChatBtn) {

		CMatchingSystem* pSystem = CMatchingSystem::Instance();


		if (pSystem) {

			if (pSystem->CheckIntrestCode(2)) {

				m_pChkChatBtn->SetStatus(FrButton::PRESSED);
				m_checked[2] = 1;



			} else {
				m_pChkChatBtn->SetStatus(FrButton::NORMAL);
			}
		}
	}
}

// dlgquickstartsetting.cpp:477
void FrQuickStartSettingDlg::OnChkChatBtnUp(void) {



	if (CMatchingSystem::Instance() && m_pChkChatBtn) {

		m_checked[2] = m_pChkChatBtn->GetStatus() == FrButton::PRESSED;
	}

}

// dlgquickstartsetting.cpp:490
void FrQuickStartSettingDlg::OnCmbCondition1BoxInit(intptr_t param) {
	m_pCmbCondition1 = DYNAMIC_CAST(FrComboBox, param);

	if (m_pCmbCondition1) {

		CMatchingSystem* pSystem = CMatchingSystem::Instance();


		if (pSystem) {

			m_pCmbCondition1->AddString("\xc1\xb6\xb0\xc7\xbc\xb3\xc1\xa4\xbe\xc8\xc7\xd4");

			std::vector<sIntrest> intrests;
			if (pSystem->GetDisplayIntrests(std::string("\xb0\xd4\xc0\xd3"), &intrests)) {

				if (!intrests.empty()) {

					for (std::vector<sIntrest>::iterator it = intrests.begin(); it != intrests.end(); ++it) {

						m_pCmbCondition1->AddString((*it).name);
					}


					m_pCmbCondition1->SetLine(1, pSystem->GetNameChecked(std::string("\xb0\xd4\xc0\xd3")).c_str(), 0, false, 0);

					return;
				}
			}





			// the original sets the first combo box for every condition
			m_pCmbCondition1->SetLine(1, "\xc1\xb6\xb0\xc7\xbc\xb3\xc1\xa4\xbe\xc8\xc7\xd4", 0, false, 0);
		}
	}
}

// dlgquickstartsetting.cpp:532
void FrQuickStartSettingDlg::OnCmbCondition2BoxInit(intptr_t param) {
	m_pCmbCondition2 = DYNAMIC_CAST(FrComboBox, param);

	if (m_pCmbCondition2) {

		CMatchingSystem* pSystem = CMatchingSystem::Instance();


		if (pSystem) {

			m_pCmbCondition2->AddString("\xc1\xb6\xb0\xc7\xbc\xb3\xc1\xa4\xbe\xc8\xc7\xd4");

			std::vector<sIntrest> intrests;
			if (pSystem->GetDisplayIntrests(std::string("\xc3\xeb\xb9\xcc"), &intrests)) {

				if (!intrests.empty()) {

					for (std::vector<sIntrest>::iterator it = intrests.begin(); it != intrests.end(); ++it) {

						m_pCmbCondition2->AddString((*it).name);
					}


					m_pCmbCondition2->SetLine(1, pSystem->GetNameChecked(std::string("\xc3\xeb\xb9\xcc")).c_str(), 0, false, 0);

					return;
				}
			}





			// the original sets the first combo box for every condition
			m_pCmbCondition1->SetLine(1, "\xc1\xb6\xb0\xc7\xbc\xb3\xc1\xa4\xbe\xc8\xc7\xd4", 0, false, 0);
		}
	}
}

// dlgquickstartsetting.cpp:586
void FrQuickStartSettingDlg::OnCmbCondition3BoxInit(intptr_t param) {
	m_pCmbCondition3 = DYNAMIC_CAST(FrComboBox, param);

	if (m_pCmbCondition3) {

		CMatchingSystem* pSystem = CMatchingSystem::Instance();


		if (pSystem) {

			m_pCmbCondition3->AddString("\xc1\xb6\xb0\xc7\xbc\xb3\xc1\xa4\xbe\xc8\xc7\xd4");

			std::vector<sIntrest> intrests;
			if (pSystem->GetDisplayIntrests(std::string("\xbf\xac\xbe\xd6"), &intrests)) {

				if (!intrests.empty()) {

					for (std::vector<sIntrest>::iterator it = intrests.begin(); it != intrests.end(); ++it) {

						m_pCmbCondition3->AddString((*it).name);
					}


					m_pCmbCondition3->SetLine(1, pSystem->GetNameChecked(std::string("\xbf\xac\xbe\xd6")).c_str(), 0, false, 0);

					return;
				}
			}





			// the original sets the first combo box for every condition
			m_pCmbCondition1->SetLine(1, "\xc1\xb6\xb0\xc7\xbc\xb3\xc1\xa4\xbe\xc8\xc7\xd4", 0, false, 0);
		}
	}
}

// dlgquickstartsetting.cpp:640
void FrQuickStartSettingDlg::UpdateData(void) {


	CMatchingSystem* pSystem = CMatchingSystem::Instance();

	if (pSystem) {

		std::vector<unsigned char> codes;
		unsigned char code;



		for (int i = 0; i < 16; i++) {

			if (m_checked[i]) {

				codes.push_back((unsigned char)i);
			}
		}



		// dlgquickstartsetting.cpp:663
		if (strcmp(m_pCmbCondition1->GetLine(1, false), "\xc1\xb6\xb0\xc7\xbc\xb3\xc1\xa4\xbe\xc8\xc7\xd4") != 0) {

			code = pSystem->GetDisplayIntrestCode(std::string("\xb0\xd4\xc0\xd3"), std::string(m_pCmbCondition1->GetLine(1, false)));
			if (code != 0xff) {

				codes.push_back(code);
			}
		}

		// dlgquickstartsetting.cpp:672
		if (strcmp(m_pCmbCondition2->GetLine(1, false), "\xc1\xb6\xb0\xc7\xbc\xb3\xc1\xa4\xbe\xc8\xc7\xd4") != 0) {

			code = pSystem->GetDisplayIntrestCode(std::string("\xc3\xeb\xb9\xcc"), std::string(m_pCmbCondition2->GetLine(1, false)));
			if (code != 0xff) {

				codes.push_back(code);
			}
		}

		// dlgquickstartsetting.cpp:681
		if (strcmp(m_pCmbCondition3->GetLine(1, false), "\xc1\xb6\xb0\xc7\xbc\xb3\xc1\xa4\xbe\xc8\xc7\xd4") != 0) {

			code = pSystem->GetDisplayIntrestCode(std::string("\xbf\xac\xbe\xd6"), std::string(m_pCmbCondition3->GetLine(1, false)));
			if (code != 0xff) {

				codes.push_back(code);
			}
		}



		pSystem->ClearMyIntrests();


		unsigned char count = (unsigned char)codes.size();
		WSendPacket packet((enumClientPacket)0xb6);
		packet.Encode1(count);

		for (int j = 0; j < count; j++) {

			packet.Encode1(codes[j]);
			pSystem->SetMyIntrest(codes[j]);
		}


		packet.Encode1(1);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// dlgquickstartsetting.cpp:714
CMatchingSystem::CMatchingSystem(void) {
}

// dlgquickstartsetting.cpp:718
CMatchingSystem::~CMatchingSystem(void) {
}

// dlgquickstartsetting.cpp:722
void CMatchingSystem::Clear(void) {
	m_myIntrests.clear();
}

// dlgquickstartsetting.cpp:727
int CMatchingSystem::SetDisplayIntrest(const sIntrest& intrest) {
	m_displayIntrests.push_back(intrest);
	return 1;
}

// dlgquickstartsetting.cpp:734
const char* CMatchingSystem::GetGroup(unsigned char code) {
	std::vector<sIntrest>::iterator it;
	for (it = m_displayIntrests.begin(); it != m_displayIntrests.end(); ++it) {

		if ((*it).code == code) {
			return (*it).group;
		}
	}




	return 0;
}

// dlgquickstartsetting.cpp:748
const sIntrest* CMatchingSystem::GetIntrestInfo(unsigned char code) {
	const sIntrest* pIntrest = 0;

	std::vector<sIntrest>::iterator it;
	for (it = m_displayIntrests.begin(); it != m_displayIntrests.end(); ++it) {

		if ((*it).code == code) {
			pIntrest = &(*it);
			break;
		}
	}



	return pIntrest;
}

// dlgquickstartsetting.cpp:765
unsigned char CMatchingSystem::GetDisplayIntrestCode(const std::string& group, const std::string& name) {


	std::vector<sIntrest>::iterator it;
	for (it = m_displayIntrests.begin(); it != m_displayIntrests.end(); ++it) {

		if (strcmp(group.c_str(), (*it).group) == 0 && strcmp(name.c_str(), (*it).name) == 0) {
			return (*it).code;
		}
	}




	return 0xff;
}

// dlgquickstartsetting.cpp:782
int CMatchingSystem::SetMyIntrest(unsigned char code) {
	m_myIntrests.push_back(code);
	return 1;
}

// dlgquickstartsetting.cpp:788
int CMatchingSystem::CheckIntrestCode(unsigned char code) {
	return std::find(m_myIntrests.begin(), m_myIntrests.end(), code) != m_myIntrests.end() ? 1 : 0;
}

// dlgquickstartsetting.cpp:794
std::string CMatchingSystem::GetNameChecked(const std::string& group) {
	unsigned char count = (unsigned char)m_myIntrests.size();
	if (count) {


		for (unsigned char i = 0; i < count; i++) {


			const sIntrest* pIntrest = GetIntrestInfo(m_myIntrests[i]);
			if (pIntrest) {

				if (strcmp(group.c_str(), pIntrest->group) == 0) {

					return std::string(pIntrest->name);
				}
			}
		}
	}







	return std::string("\xc1\xb6\xb0\xc7\xbc\xb3\xc1\xa4\xbe\xc8\xc7\xd4");
}

// dlgquickstartsetting.cpp:821
int CMatchingSystem::GetDisplayIntrests(const std::string& group, std::vector<sIntrest>* pIntrests) {
	int ret = 0;




	std::vector<sIntrest>::iterator it;
	for (it = m_displayIntrests.begin(); it != m_displayIntrests.end(); ++it) {
		if (strcmp(group.c_str(), (*it).group) == 0) {


			pIntrests->push_back(*it);

			ret = 1;
		}
	}



	return ret;
}
