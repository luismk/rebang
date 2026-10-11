// Original: d:\build\custom\temp\code\client\projectg\downgradedlg.cpp
#include <string.h>
#include "DowngradeDlg.h"
#include "HelpDlg.h"
#include "ObjectFactory.h"
#include "TaskManager.h"
#include "actor.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "fresh.h"
#include "frgaugebar.h"
#include "commonutil.h"
#include "SharedDoc.h"
#include "../shared/classdefine.h"

// downgradedlg.cpp:18 (run-time type and factory registration of FrDowngradeDlg)
const WRTTI FrDowngradeDlg::m_RTTI("FrDowngradeDlg", &FrForm::m_RTTI);

IObject* FrDowngradeDlgMakeInstance(void) {
	return new FrDowngradeDlg;
}

struct __sFrDowngradeDlg {
	__sFrDowngradeDlg(void) {
		ObjectFactory().AddObjectFunctor(FrDowngradeDlgMakeInstance, "FrDowngradeDlg");
	}
};
static __sFrDowngradeDlg s_registerFrDowngradeDlg;

// downgradedlg.cpp:20
const sFRESH_MSGMAP* FrDowngradeDlg::GetMessageMap(void) const {
	return &FrDowngradeDlg::_MsgMap;
}

sFRESH_MSGMAP FrDowngradeDlg::_MsgMap = { &FrForm::_MsgMap, &FrDowngradeDlg::_MsgEntries[0] };

sFRESH_ENTRY FrDowngradeDlg::_MsgEntries[] = {
	// downgradedlg.cpp:22
	{ "title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnTitleInit },
	{ "help", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnHelpBtnInit },
	{ "help", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDowngradeDlg::OnHelpBtnUp },
	{ "power", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnPowerInit },
	{ "power", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDowngradeDlg::OnPowerBtnUp },
	{ "control", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnControlInit },
	{ "control", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDowngradeDlg::OnControlBtnUp },
	{ "impact", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnImpactInit },
	{ "impact", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDowngradeDlg::OnImpactBtnUp },
	{ "spin", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnSpinInit },
	{ "spin", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDowngradeDlg::OnSpinBtnUp },
	{ "curve", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnCurveInit },
	{ "curve", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDowngradeDlg::OnCurveBtnUp },
	{ "epower1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnPowerEdit1Init },
	{ "econtrol1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnControlEdit1Init },
	{ "eimpact1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnImpactEdit1Init },
	{ "espin1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnSpinEdit1Init },
	{ "ecurve1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnCurveEdit1Init },
	{ "epower2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnPowerEdit2Init },
	{ "econtrol2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnControlEdit2Init },
	{ "eimpact2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnImpactEdit2Init },
	{ "espin2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnSpinEdit2Init },
	{ "ecurve2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnCurveEdit2Init },
	{ "powerbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnPowerBarInit },
	{ "controlbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnControlBarInit },
	{ "impactbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnImpactBarInit },
	{ "spinbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnSpinBarInit },
	{ "curvebar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnCurveBarInit },
	{ "help1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnHelpEdit1Init },
	// downgradedlg.cpp:51
	{ "help2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDowngradeDlg::OnHelpEdit2Init },

	// downgradedlg.cpp:53
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// downgradedlg.cpp:56
FrDowngradeDlg::FrDowngradeDlg(void) {
	memset(m_pBar, 0, sizeof(m_pBar));
	memset(m_pButton, 0, sizeof(m_pButton));
	memset(m_pEdit1, 0, sizeof(m_pEdit1));
	memset(m_pEdit2, 0, sizeof(m_pEdit2));

	m_itemId = 0;
	m_pHelpDlg = 0;
}

// downgradedlg.cpp:67
void FrDowngradeDlg::SetInfo(unsigned long typeId, char* name, unsigned long* pLevel, unsigned long* pEnchant, int bDowngrade) {
	m_typeId = typeId;

	m_pName = name;
	m_pLevel = pLevel;
	m_pEnchant = pEnchant;

	m_bDowngrade = bDowngrade;
	m_bClub = 0;

	if (m_bDowngrade == 0) {

		for (int i = 0; i < 5; i++) {

			if (m_pButton[i]) {
				m_pButton[i]->SetButtonImg("level_up", FrButton::NORMAL);
			}
		}
		if (m_pTitle) {
			m_pTitle->SetBgImg("cha_up");
		}
		if (m_pHelp1) {
			// "If you are wearing clothes that can raise abilities, you can pay a certain amount of pang to upgrade your character's abilities."
			m_pHelp1->AddText("\xB4\xC9\xB7\xC2\xC0\xBB \xC5\xB0\xBF\xEF \xBC\xF6 \xC0\xD6\xB4\xC2 \xC0\xC7\xBB\xF3\xC0\xBB \xB0\xAE\xC3\xDF\xBE\xEE \xC0\xD4\xBE\xFA\xB4\xD9\xB8\xE9 \xC0\xCF\xC1\xA4\xB7\xAE\xC0\xC7 \xC6\xCE\xC0\xBB \xC1\xF6\xBA\xD2\xC7\xCF\xBF\xA9 \xBF\xA9\xB7\xAF\xBA\xD0\xC0\xC7 \xC4\xB3\xB8\xAF\xC5\xCD\xC0\xC7 \xB4\xC9\xB7\xC2\xC0\xBB \xBE\xF7\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5 \xC7\xD2 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", false, true);
		}
		if (m_pHelp2) {
			// "Combined with other equipment, power of 20 or more costs control and accuracy, so decide on power upgrades carefully."
			m_pHelp2->AddText("\xB4\xD9\xB8\xA5 \xC0\xE5\xBA\xF1\xBF\xCD \xC1\xB6\xC7\xD5\xBD\xC3 \xC6\xC4\xBF\xF6 \xB4\xC9\xB7\xC2\xC0\xCC 20 \xC0\xCC\xBB\xF3\xC0\xCF \xB0\xE6\xBF\xEC \xC4\xC1\xC6\xAE\xB7\xD1\xB0\xFA \xC1\xA4\xC8\xAE\xB5\xB5 \xB4\xC9\xB7\xC2\xC0\xCC \xC1\xD9\xBE\xEE\xB5\xE5\xB4\xC2 \xC6\xD0\xB3\xCE\xC6\xBC\xB0\xA1 \xC0\xD6\xC0\xB8\xB4\xCF \xC6\xC4\xBF\xF6 \xB4\xC9\xB7\xC2 \xBE\xF7\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5\xB4\xC2 \xBD\xC5\xC1\xDF\xC8\xF7 \xB0\xE1\xC1\xA4\xC7\xCF\xBD\xC3\xB1\xE2 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9.", false, true);
		}
	}
	else {
		if (m_pTitle) {
			m_pTitle->SetBgImg("cha_dn");
		}
		if (m_pHelp1) {
			// "When a bad upgrade has broken your character's balance, you can downgrade its abilities."
			m_pHelp1->AddText("\xC0\xDF\xB8\xF8\xB5\xC8 \xBE\xF7\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5\xB7\xCE \xC4\xB3\xB8\xAF\xC5\xCD \xB4\xC9\xB7\xC2\xC0\xC7 \xB9\xEB\xB7\xB1\xBD\xBA\xB0\xA1 \xB9\xAB\xB3\xCA\xC1\xAE \xBB\xE7\xBF\xEB\xC7\xCF\xB1\xE2 \xBE\xEE\xB7\xC1\xBF\xEF \xB6\xA7, \xC4\xB3\xB8\xAF\xC5\xCD\xC0\xC7 \xB4\xC9\xB7\xC2\xC4\xA1\xB8\xA6 \xB4\xD9\xBF\xEE\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5 \xC7\xD2 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", false, true);
		}
		if (m_pHelp2) {
			// "-Upgrading the character again after a downgrade costs the usual pang."
			m_pHelp2->AddText("-\xB4\xC9\xB7\xC2 \xB4\xD9\xBF\xEE\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5\xB8\xA6 \xC0\xCC\xBF\xEB\xC7\xD8 \xC4\xB3\xB8\xAF\xC5\xCD\xC0\xC7 \xB4\xC9\xB7\xC2\xC0\xBB \xC0\xE7\xC1\xB6\xC0\xFD\xC7\xD1 \xC8\xC4 \xB4\xD9\xBD\xC3 \xC4\xB3\xB8\xAF\xC5\xCD\xC0\xC7 \xB4\xC9\xB7\xC2\xC0\xBB \xBE\xF7\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5 \xC7\xD2 \xB6\xA7\xB4\xC2 \xC6\xF2\xBB\xF3\xBD\xC3\xBF\xCD \xB0\xB0\xC0\xBA \xC6\xCE\xC0\xCC \xBC\xD2\xB8\xF0\xB5\xCB\xB4\xCF\xB4\xD9.", false, true);
		}
	}

	SetLevelBar();
}

// downgradedlg.cpp:109
void FrDowngradeDlg::SetInfo(unsigned long itemId, int bDowngrade) {
	m_itemId = itemId;
	m_bDowngrade = bDowngrade;
	m_bClub = 1;

	if (m_bDowngrade == 0) {

		for (int i = 0; i < 5; i++) {

			if (m_pButton[i]) {
				m_pButton[i]->SetButtonImg("level_up", FrButton::NORMAL);
			}
		}
		if (m_pTitle) {
			m_pTitle->SetBgImg("club_up");
		}
		if (m_pHelp1) {
			// "If you own a club that can raise abilities, you can pay a certain amount of pang to upgrade your club's abilities."
			m_pHelp1->AddText("\xB4\xC9\xB7\xC2\xC0\xBB \xC5\xB0\xBF\xEF \xBC\xF6 \xC0\xD6\xB4\xC2 \xC5\xAC\xB7\xB4\xC0\xBB \xBC\xD2\xC0\xAF\xC7\xCF\xB0\xED \xC0\xD6\xB4\xD9\xB8\xE9 \xC0\xCF\xC1\xA4\xB7\xAE\xC0\xC7 \xC6\xCE\xC0\xBB \xC1\xF6\xBA\xD2\xC7\xCF\xBF\xA9 \xBF\xA9\xB7\xAF\xBA\xD0\xC0\xC7 \xC5\xAC\xB7\xB4\xC0\xC7 \xB4\xC9\xB7\xC2\xC0\xBB \xBE\xF7\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5 \xC7\xD2 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", false, true);
		}
		if (m_pHelp2) {
			// "Combined with other equipment, power of 20 or more costs control and accuracy, so decide on power upgrades carefully."
			m_pHelp2->AddText("\xB4\xD9\xB8\xA5 \xC0\xE5\xBA\xF1\xBF\xCD \xC1\xB6\xC7\xD5\xBD\xC3 \xC6\xC4\xBF\xF6 \xB4\xC9\xB7\xC2\xC0\xCC 20 \xC0\xCC\xBB\xF3\xC0\xCF \xB0\xE6\xBF\xEC \xC4\xC1\xC6\xAE\xB7\xD1\xB0\xFA \xC1\xA4\xC8\xAE\xB5\xB5 \xB4\xC9\xB7\xC2\xC0\xCC \xC1\xD9\xBE\xEE\xB5\xE5\xB4\xC2 \xC6\xD0\xB3\xCE\xC6\xBC\xB0\xA1 \xC0\xD6\xC0\xB8\xB4\xCF \xC6\xC4\xBF\xF6 \xB4\xC9\xB7\xC2 \xBE\xF7\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5\xB4\xC2 \xBD\xC5\xC1\xDF\xC8\xF7 \xB0\xE1\xC1\xA4\xC7\xCF\xBD\xC3\xB1\xE2 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9.", false, true);
		}
	}
	else {
		if (m_pTitle) {
			m_pTitle->SetBgImg("club_dn");
		}
		if (m_pHelp1) {
			// "When a bad upgrade has broken your club's balance, you can downgrade its abilities."
			m_pHelp1->AddText("\xC0\xDF\xB8\xF8\xB5\xC8 \xBE\xF7\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5\xB7\xCE \xC5\xAC\xB7\xB4 \xB4\xC9\xB7\xC2\xC0\xC7 \xB9\xEB\xB7\xB1\xBD\xBA\xB0\xA1 \xB9\xAB\xB3\xCA\xC1\xAE \xBB\xE7\xBF\xEB\xC7\xCF\xB1\xE2 \xBE\xEE\xB7\xC1\xBF\xEF \xB6\xA7, \xC5\xAC\xB7\xB4\xC0\xC7 \xB4\xC9\xB7\xC2\xC4\xA1\xB8\xA6 \xB4\xD9\xBF\xEE\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5 \xC7\xD2 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", false, true);
		}
		if (m_pHelp2) {
			// "-Upgrading the club again after a downgrade costs the usual pang."
			m_pHelp2->AddText("-\xB4\xC9\xB7\xC2 \xB4\xD9\xBF\xEE\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5\xB8\xA6 \xC0\xCC\xBF\xEB\xC7\xD8 \xC5\xAC\xB7\xB4\xC0\xC7 \xB4\xC9\xB7\xC2\xC0\xBB \xC0\xE7\xC1\xB6\xC0\xFD\xC7\xD1 \xC8\xC4 \xB4\xD9\xBD\xC3 \xC5\xAC\xB7\xB4\xC0\xC7 \xB4\xC9\xB7\xC2\xC0\xBB \xBE\xF7\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5 \xC7\xD2 \xB6\xA7\xB4\xC2 \xC6\xF2\xBB\xF3\xBD\xC3\xBF\xCD \xB0\xB0\xC0\xBA \xC6\xCE\xC0\xCC \xBC\xD2\xB8\xF0\xB5\xCB\xB4\xCF\xB4\xD9.", false, true);
		}
	}

	SetLevelBar();
}

// downgradedlg.cpp:147
void FrDowngradeDlg::OnTitleInit(intptr_t param) {
	m_pTitle = DYNAMIC_CAST(FrArea, param);
}

// downgradedlg.cpp:152
void FrDowngradeDlg::OnHelpBtnInit(intptr_t param) {
	m_pHelpBtn = DYNAMIC_CAST(FrButton, param);
}

// downgradedlg.cpp:157
void FrDowngradeDlg::OnHelpBtnUp(void) {
	if (m_pHelpDlg == 0) {

		m_pHelpDlg = CreateForm<FrHelpDlg>(g_pFresh->GetManager(), this, "help", 0);
		m_pHelpDlg->Open((FRESH_RESULT_CALLBACK)&FrDowngradeDlg::OnHelpDlgResult, 1);
	}
}

// downgradedlg.cpp:166
bool FrDowngradeDlg::OnHelpDlgResult(int ret, FrForm* pForm) {
	m_pHelpDlg = 0;
	return true;
}

// downgradedlg.cpp:173
void FrDowngradeDlg::OnHelpEdit1Init(intptr_t param) {
	m_pHelp1 = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:178
void FrDowngradeDlg::OnHelpEdit2Init(intptr_t param) {
	m_pHelp2 = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:184
void FrDowngradeDlg::OnPowerBarInit(intptr_t param) {
	m_pBar[0] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pBar[0]->SetRange(0, 50, 0);
}

// downgradedlg.cpp:191
void FrDowngradeDlg::OnControlBarInit(intptr_t param) {
	m_pBar[1] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pBar[1]->SetRange(0, 50, 0);
}

// downgradedlg.cpp:198
void FrDowngradeDlg::OnImpactBarInit(intptr_t param) {
	m_pBar[2] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pBar[2]->SetRange(0, 50, 0);
}

// downgradedlg.cpp:204
void FrDowngradeDlg::OnSpinBarInit(intptr_t param) {
	m_pBar[3] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pBar[3]->SetRange(0, 50, 0);
}

// downgradedlg.cpp:211
void FrDowngradeDlg::OnCurveBarInit(intptr_t param) {
	m_pBar[4] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pBar[4]->SetRange(0, 50, 0);
}

// downgradedlg.cpp:217
void FrDowngradeDlg::OnPowerInit(intptr_t param) {
	m_pButton[0] = DYNAMIC_CAST(FrButton, param);
}

// downgradedlg.cpp:222
void FrDowngradeDlg::OnControlInit(intptr_t param) {
	m_pButton[1] = DYNAMIC_CAST(FrButton, param);
}

// downgradedlg.cpp:227
void FrDowngradeDlg::OnImpactInit(intptr_t param) {
	m_pButton[2] = DYNAMIC_CAST(FrButton, param);
}

// downgradedlg.cpp:232
void FrDowngradeDlg::OnSpinInit(intptr_t param) {
	m_pButton[3] = DYNAMIC_CAST(FrButton, param);
}

// downgradedlg.cpp:237
void FrDowngradeDlg::OnCurveInit(intptr_t param) {
	m_pButton[4] = DYNAMIC_CAST(FrButton, param);
}

// downgradedlg.cpp:242
void FrDowngradeDlg::OnPowerBtnUp(void) {
	OpenDowngrade(0);
}

// downgradedlg.cpp:247
void FrDowngradeDlg::OnControlBtnUp(void) {
	OpenDowngrade(1);
}

// downgradedlg.cpp:252
void FrDowngradeDlg::OnImpactBtnUp(void) {
	OpenDowngrade(2);
}

// downgradedlg.cpp:257
void FrDowngradeDlg::OnSpinBtnUp(void) {
	OpenDowngrade(3);
}

// downgradedlg.cpp:262
void FrDowngradeDlg::OnCurveBtnUp(void) {
	OpenDowngrade(4);
}

// downgradedlg.cpp:267
void FrDowngradeDlg::OpenDowngrade(unsigned char stat) {
	// 0: character upgrade, 1: club upgrade, 2: character downgrade, 3: club downgrade
	unsigned char type = m_bDowngrade ? 2 : 0;




	if (m_bClub == 1) {
		type += 1;
	}
	AfxGetTask()->PassMsgToActor("RealMyRoom", MsgObject(0, 0xD4, type, stat, 0, 0, 0));
}
// downgradedlg.cpp:282
void FrDowngradeDlg::OnPowerEdit1Init(intptr_t param) {
	m_pEdit1[0] = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:287
void FrDowngradeDlg::OnControlEdit1Init(intptr_t param) {
	m_pEdit1[1] = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:292
void FrDowngradeDlg::OnImpactEdit1Init(intptr_t param) {
	m_pEdit1[2] = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:297
void FrDowngradeDlg::OnSpinEdit1Init(intptr_t param) {
	m_pEdit1[3] = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:302
void FrDowngradeDlg::OnCurveEdit1Init(intptr_t param) {
	m_pEdit1[4] = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:307
void FrDowngradeDlg::OnPowerEdit2Init(intptr_t param) {
	m_pEdit2[0] = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:312
void FrDowngradeDlg::OnControlEdit2Init(intptr_t param) {
	m_pEdit2[1] = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:317
void FrDowngradeDlg::OnImpactEdit2Init(intptr_t param) {
	m_pEdit2[2] = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:322
void FrDowngradeDlg::OnSpinEdit2Init(intptr_t param) {
	m_pEdit2[3] = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:327
void FrDowngradeDlg::OnCurveEdit2Init(intptr_t param) {
	m_pEdit2[4] = DYNAMIC_CAST(FrEdit, param);
}

// downgradedlg.cpp:332
void FrDowngradeDlg::SetLevelNumber(int stat) {
	if (m_pEdit1[stat] == 0) {
		return;
	}

	unsigned char level;
	unsigned char maxLevel;
	if (m_bClub == 0) {

		unsigned char itemLevel = ItemManager()->GetItemLevel((eLvlType)stat, m_pLevel, m_pEnchant);
		level = itemLevel + ItemManager()->GetCharLevel((eLvlType)stat, m_typeId, m_pName);
		maxLevel = itemLevel + ItemManager()->GetCharCapacity((eLvlType)stat, m_typeId, m_pLevel, m_pEnchant, CSharedDoc::Instance()->m_myLevel);

	} else {

		std::map<unsigned int, sItemInfo>::iterator it = CSharedDoc::Instance()->m_myItemMap.find(m_itemId);
		IFF_STRUCT::sClubSet* pClubSet = ItemManager()->FindClubSet(it->second.m_typeId);
		if (pClubSet == 0) {
			return;
		}

		level = (unsigned char)(pClubSet->m_baseStat[stat] + it->second.m_clubStatLevel[stat]);
		maxLevel = (unsigned char)pClubSet->m_maxStat[stat];
	}

	m_pEdit1[stat]->SetLine(1, MakeStr("%d", level > maxLevel ? maxLevel : level), 0xFFFF0000, false, 0);
}

// downgradedlg.cpp:359
void FrDowngradeDlg::SetEnchantNumber(int stat) {
	if (m_pEdit2[stat] == 0) {
		return;
	}

	if (m_bClub == 0) {

		unsigned short capacity = ItemManager()->GetCharCapacity((eLvlType)stat, 0, m_pLevel, m_pEnchant, CSharedDoc::Instance()->m_myLevel);

		m_pEdit2[stat]->SetLine(1, MakeStr("%d/%d", m_pName ? m_pName[stat] : 0, capacity), 0xFF000000, false, 0);

	} else {

		std::map<unsigned int, sItemInfo>::iterator it = CSharedDoc::Instance()->m_myItemMap.find(m_itemId);
		IFF_STRUCT::sClubSet* pClubSet = ItemManager()->FindClubSet(it->second.m_typeId);
		if (pClubSet == 0) {
			return;
		}

		unsigned short maxValue = pClubSet->m_maxStat[stat];
		m_pEdit2[stat]->SetLine(1, MakeStr("%d/%d", it->second.m_clubStatLevel[stat], maxValue - pClubSet->m_baseStat[stat]), 0xFF000000, false, 0);
	}
}

// downgradedlg.cpp:384
void FrDowngradeDlg::SetLevelBar(void) {
	int i;
	int value;
	int maxValue;
	if (m_bClub == 0) {



		for (i = 0; i < 5; i++) {

			int itemLevel = ItemManager()->GetItemLevel((eLvlType)i, m_pLevel, m_pEnchant);
			maxValue = itemLevel + ItemManager()->GetCharCapacity((eLvlType)i, m_typeId, m_pLevel, m_pEnchant, CSharedDoc::Instance()->m_myLevel);
			value = itemLevel + ItemManager()->GetCharLevel((eLvlType)i, m_typeId, m_pName);

			if (m_pBar[i]) {

				m_pBar[i]->SetDestPos(value, false);
				m_pBar[i]->SetExpand(maxValue - value);
			}

			if (m_pButton[i]) {

				if (m_bDowngrade == 0)
					m_pButton[i]->Enable(value < maxValue);
				else
					m_pButton[i]->Enable(m_pName[i] > 0);
			}

			SetLevelNumber(i);
			SetEnchantNumber(i);
		}

	} else {

		std::map<unsigned int, sItemInfo>::iterator it = CSharedDoc::Instance()->m_myItemMap.find(m_itemId);
		if (it == CSharedDoc::Instance()->m_myItemMap.end()) {
			return;
		}

		IFF_STRUCT::sClubSet* pClubSet = ItemManager()->FindClubSet(it->second.m_typeId);
		if (pClubSet == 0) {
			return;
		}

		for (i = 0; i < 5; i++) {

			maxValue = pClubSet->m_maxStat[i];
			value = pClubSet->m_baseStat[i] + it->second.m_clubStatLevel[i];

			if (m_pBar[i]) {

				m_pBar[i]->SetDestPos(value, false);
				m_pBar[i]->SetExpand(maxValue - value);
			}

			if (m_pButton[i]) {

				if (m_bDowngrade == 0)
					m_pButton[i]->Enable(value < maxValue);
				else
					m_pButton[i]->Enable(it->second.m_clubStatLevel[i] > 0);
			}

			SetLevelNumber(i);
			SetEnchantNumber(i);
		}
	}
}
