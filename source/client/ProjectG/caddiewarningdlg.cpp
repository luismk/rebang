// Original: d:\build\custom\temp\code\client\projectg\caddiewarningdlg.cpp
#include <string.h>

#include "CaddieWarningDlg.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "Packet.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frstatic.h"
#include "commonutil.h"
#include "../shared/ItemManager.h"
#include "../shared/classdefine.h"
#include <stdio.h>

// caddiewarningdlg.cpp:19 (run-time type and factory registration of FrCaddieWarningDlg)
const WRTTI FrCaddieWarningDlg::m_RTTI("FrCaddieWarningDlg", &FrForm::m_RTTI);

IObject* FrCaddieWarningDlgMakeInstance(void) {
	return new FrCaddieWarningDlg;
}

struct __sFrCaddieWarningDlg {
	__sFrCaddieWarningDlg(void) {
		ObjectFactory().AddObjectFunctor(FrCaddieWarningDlgMakeInstance, "FrCaddieWarningDlg");
	}
};
static __sFrCaddieWarningDlg s_registerFrCaddieWarningDlg;

// caddiewarningdlg.cpp:21
const sFRESH_MSGMAP* FrCaddieWarningDlg::GetMessageMap(void) const {
	return &FrCaddieWarningDlg::_MsgMap;
}

sFRESH_MSGMAP FrCaddieWarningDlg::_MsgMap = { &FrForm::_MsgMap, &FrCaddieWarningDlg::_MsgEntries[0] };

sFRESH_ENTRY FrCaddieWarningDlg::_MsgEntries[] = {
	// caddiewarningdlg.cpp:23
	{ "portrait", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieWarningDlg::OnPortraitInit },
	// caddiewarningdlg.cpp:24
	{ "vacation", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieWarningDlg::OnVacationInit },
	// caddiewarningdlg.cpp:25
	{ "limit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieWarningDlg::OnLimitInit },
	// caddiewarningdlg.cpp:26
	{ "name", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieWarningDlg::OnNameInit },
	// caddiewarningdlg.cpp:27
	{ "message", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieWarningDlg::OnMessageInit },
	// caddiewarningdlg.cpp:28
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieWarningDlg::OnOkInit },
	// caddiewarningdlg.cpp:29
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCaddieWarningDlg::OnOkBtnUp },
	// caddiewarningdlg.cpp:30
	{ "warning_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieWarningDlg::OnWarningButtonInit },
	// caddiewarningdlg.cpp:31
	{ "warning_text", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieWarningDlg::OnWarningTextInit },

	// caddiewarningdlg.cpp:33
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// caddiewarningdlg.cpp:36
FrCaddieWarningDlg::FrCaddieWarningDlg(void) {
	memset(&m_caddieInfo, 0, sizeof(sCaddieInfo));

	m_pPortrait = 0;
	m_pVacation = 0;
	m_pLimit = 0;
	m_pName = 0;
	m_pMessage = 0;
	m_pOk = 0;
	m_pWarningButton = 0;
	m_pWarningText = 0;
}

// caddiewarningdlg.cpp:59
void FrCaddieWarningDlg::SetCaddieInfo(const sCaddieInfo& info) {


	m_caddieInfo = info;
	m_caddieInfo.contract = 1;
}

// caddiewarningdlg.cpp:67
void FrCaddieWarningDlg::OnPortraitInit(intptr_t param) {
	m_pPortrait = DYNAMIC_CAST(FrArea, param);
}

// caddiewarningdlg.cpp:72
void FrCaddieWarningDlg::OnVacationInit(intptr_t param) {
	m_pVacation = DYNAMIC_CAST(FrArea, param);
}

// caddiewarningdlg.cpp:77
void FrCaddieWarningDlg::OnLimitInit(intptr_t param) {
	m_pLimit = DYNAMIC_CAST(FrArea, param);

	if (m_pLimit) {
		m_pLimit->SetVisible(false);
	}
}

// caddiewarningdlg.cpp:85
void FrCaddieWarningDlg::OnNameInit(intptr_t param) {
	m_pName = DYNAMIC_CAST(FrEdit, param);
}

// caddiewarningdlg.cpp:90
void FrCaddieWarningDlg::OnMessageInit(intptr_t param) {
	m_pMessage = DYNAMIC_CAST(FrEdit, param);
}

// caddiewarningdlg.cpp:95
void FrCaddieWarningDlg::OnOkInit(intptr_t param) {
	m_pOk = DYNAMIC_CAST(FrButton, param);
}

// caddiewarningdlg.cpp:100
void FrCaddieWarningDlg::OnWarningButtonInit(intptr_t param) {
	m_pWarningButton = DYNAMIC_CAST(FrButton, param);
}

// caddiewarningdlg.cpp:105
void FrCaddieWarningDlg::OnWarningTextInit(intptr_t param) {
	m_pWarningText = DYNAMIC_CAST(FrStatic, param);
}

// caddiewarningdlg.cpp:119
void FrCaddieWarningDlg::OnOkBtnUp(void) {
	if (m_pOk) {
		m_pOk->Enable(false);
	}

	IFF_STRUCT::sCaddie* pCaddie = ItemManager()->FindCaddie(m_caddieInfo.typeId);

	if (!pCaddie) {

		Close(FrOK, true);
		return;
	}

	if (m_pWarningButton->GetStatus() == FrButton::PRESSED) {

		WSingleton<CSharedDoc>::Instance()->m_caddieMap[(unsigned long)m_caddieInfo.id].contract = 1;
		m_caddieInfo.contract = 1;

	} else {

		WSingleton<CSharedDoc>::Instance()->m_caddieMap[(unsigned long)m_caddieInfo.id].contract = 0;
		m_caddieInfo.contract = 0;
	}

	WSendPacket packet((enumClientPacket)0x6B);

	packet.Encode4(m_caddieInfo.id);
	packet.Encode1(m_caddieInfo.contract);
	packet.Send(SEND_TO_DEFAULT);

	Close(FrOK, true);
}

// caddiewarningdlg.cpp:161
bool FrCaddieWarningDlg::OnInit(void) {
	if (m_caddieInfo.typeId == 0) {
		return false;
	}

	IFF_STRUCT::sCaddie* pCaddie = ItemManager()->FindCaddie(m_caddieInfo.typeId);
	bool shown = false;

	if (m_pPortrait) {
		if (pCaddie) {
			m_pPortrait->SetBgImg(pCaddie->m_icon);

			if (m_pName) {
				char text[256] = { 0 };

				sprintf(text, "\xC0\xCC  \xB8\xA7: %s", pCaddie->m_name);

				unsigned char level = m_caddieInfo.level;
				if (level < 3 && (pCaddie->flags & 0xE0) != 0xA0) {
					unsigned long levelExp = CSharedDoc::Instance()->m_caddieLevelExp[level].m_exp;
					unsigned long exp = m_caddieInfo.experience;

					strcat(text, MakeStr("\n\xB7\xB9  \xBA\xA7: %d / %d ( %d / %d )", level + 1, 3, exp, levelExp));
				}

				if ((m_caddieInfo.flags & 2) && pCaddie->fee > 0) {
					// "\nOn vacation"
					strcat(text, "\n\xC8\xDE\xB0\xA1\xC1\xDF");
				}

				m_pName->AddText(text, false, true);

				SetMessage("\\cT\\c\\c0xffff0000\\c\xC7\xF6\xC0\xE7 \xC4\xB3\xB5\xF0\xB0\xA1 \xC8\xDE\xB0\xA1\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.\n\xB0\xD4\xC0\xD3 \xBD\xC3\xC0\xDB \xC0\xFC\xBF\xA1 \xC0\xE7\xB0\xED\xBF\xEB\xC7\xCF\xBD\xC3\xB4\xC2 \xB0\xCD \xC0\xD8\xC1\xF6 \xB8\xB6\xBC\xBC\xBF\xE4!\\cN\\c\\c0xff000000\\c\n\n(\xBE\xCB\xB8\xB2\xC0\xBB \xB9\xDE\xC1\xF6 \xBE\xCA\xC0\xB8\xBD\xC3\xB7\xC1\xB8\xE9 \xBF\xF9\xB1\xDE \xB3\xAF\xC2\xA5 \xBE\xCB\xB8\xB2 \xB1\xE2\xB4\xC9 \xC3\xBC\xC5\xA9\xB8\xA6 \xB2\xF6 \xC8\xC4 \xC8\xAE\xC0\xCE \xB9\xF6\xC6\xB0\xC0\xBB \xB4\xAD\xB7\xAF\xC1\xD6\xBC\xBC\xBF\xE4)", false);
				shown = true;
			}
		} else {
			m_pPortrait->SetBgImg("hide");
		}
	}

	if (!shown) {
		if (m_pName) {
			// "Name: unknown\n\nLevel: unknown"
			m_pName->AddText("\xC0\xCC\xB8\xA7: \xBA\xD2\xB8\xED\n\n\xB7\xB9\xBA\xA7: \xBA\xD2\xB8\xED", false, true);
		}
		SetMessage("\xBA\xA3\xC0\xCF\xBF\xA1 \xBD\xCE\xC0\xCE \xC1\xA4\xC3\xBC\xBA\xD2\xB8\xED\xC0\xC7 \xC1\xB8\xC0\xE7", false);
	}

	if (pCaddie) {
		if (m_pWarningButton) {
			if (m_caddieInfo.contract == 1)
				m_pWarningButton->SetStatus(FrButton::PRESSED);
			else
				m_pWarningButton->SetStatus(FrButton::NORMAL);
		}

		if (CSharedDoc::Instance()->m_myLevel < (pCaddie->m_level & 0x7F)) {
			if (m_pLimit) {
				m_pLimit->SetVisible(true);
			}
			// "A caddie you cannot use because of the level limit."
			SetDesc("\xB7\xB9\xBA\xA7 \xC1\xA6\xC7\xD1\xC0\xB8\xB7\xCE \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xC4\xB3\xB5\xF0\xC0\xD4\xB4\xCF\xB4\xD9.");
		}
	}

	return true;
}
