// Original: d:\build\custom\temp\code\client\projectg\compounddlg.cpp
#include "CompoundDlg.h"
#include "ObjectFactory.h"
#include "Packet.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frgaugebar.h"
#include "commonutil.h"
#include "SharedDoc.h"
#include "SoundManager.h"
#include "UccManager.h"
#include "wfont.h"
#include "wview.h"
#include "wmath.h"
#include "wresrcmng.h"

// Texture file of the title font of FrCompoundDlg (no symbol; contents not
// read yet).
extern "C" char* data_aa87d0[];

// compounddlg.cpp:20 (run-time type and factory registration of FrCompoundDlg)
const WRTTI FrCompoundDlg::m_RTTI("FrCompoundDlg", &FrForm::m_RTTI);

IObject* FrCompoundDlgMakeInstance(void) {
	return new FrCompoundDlg;
}

struct __sFrCompoundDlg {
	__sFrCompoundDlg(void) {
		ObjectFactory().AddObjectFunctor(FrCompoundDlgMakeInstance, "FrCompoundDlg");
	}
};
static __sFrCompoundDlg s_registerFrCompoundDlg;

// compounddlg.cpp:22
const sFRESH_MSGMAP* FrCompoundDlg::GetMessageMap(void) const {
	return &FrCompoundDlg::_MsgMap;
}

sFRESH_MSGMAP FrCompoundDlg::_MsgMap = { &FrForm::_MsgMap, &FrCompoundDlg::_MsgEntries[0] };

// The control names are inferred from the handler names (the data is not
// scored); the order and lines come from the entries' initializer.
sFRESH_ENTRY FrCompoundDlg::_MsgEntries[] = {
	// compounddlg.cpp:24
	{ "yes", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCompoundDlg::OnYesInit },
	// compounddlg.cpp:25
	{ "yes", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCompoundDlg::OnYesBtnUp },
	// compounddlg.cpp:26
	{ "no", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCompoundDlg::OnNoInit },
	// compounddlg.cpp:27
	{ "no", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCompoundDlg::OnNoBtnUp },
	// compounddlg.cpp:28
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCompoundDlg::OnOKInit },
	// compounddlg.cpp:29
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCompoundDlg::OnOKBtnUp },
	// compounddlg.cpp:30
	{ "gauge", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCompoundDlg::OnGaugeBarInit },
	// compounddlg.cpp:31
	{ "etext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCompoundDlg::OnTextEditInit },
	// compounddlg.cpp:32
	{ "compres", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCompoundDlg::OnCompResInit },
	// compounddlg.cpp:33
	{ "compres", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrCompoundDlg::OnCompResOwnerDraw },

	// compounddlg.cpp:35
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// compounddlg.cpp:39
FrCompoundDlg::FrCompoundDlg(void) {
	memset(&m_quest, 0, sizeof(m_quest));
	m_pCompRes = 0;
	m_bShowRes = false;
	memset(m_pBtn, 0, sizeof(m_pBtn));
	m_pGaugeBar = 0;
	m_pTextEdit = 0;
	m_compoundRes = 0xff;
	memset(&m_resItem, 0, sizeof(m_resItem));

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = data_aa87d0;
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:";
	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);
}

// compounddlg.cpp:67
FrCompoundDlg::~FrCompoundDlg(void) {
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// compounddlg.cpp:72
void FrCompoundDlg::SetCompound(IFF_STRUCT::sQuest& quest) {
	m_quest = quest;

	if (m_pTextEdit) {
		m_pTextEdit->ClearLine();
	}

	IFF_STRUCT::sQuestDrop* pDrop = ItemManager()->FindQuestDrop(m_quest.questDropId);

	if (m_pTextEdit) {
		// "Combining the unique item: %s x%d will be consumed."
		m_pTextEdit->AddText(MakeStr("\\c0xffff0000\\c\xc0\xaf\xb4\xcf\xc5\xa9 \xbe\xc6\xc0\xcc\xc5\xdb\\c0xff000000\\c\xc0\xc7 \xc1\xb6\xc7\xd5\xbf\xa1\n\\c0xffff0000\\c%s %d\\c0xff000000\\c\xb0\xb3\xb0\xa1 \xbc\xd2\xba\xf1\xb5\xcb\xb4\xcf\xb4\xd9.",
			pDrop ? pDrop->m_name : "??", m_quest.questDropCount), false, true);
	}

	if (WSingleton<CSharedDoc>::Instance()->CanCompound(m_quest.m_typeId)) {
		// "Combine?"
		m_pTextEdit->AddText("\xc1\xb6\xc7\xd5\xc0\xbb \xc7\xcf\xbd\xc3\xb0\xda\xbd\xc0\xb4\xcf\xb1\xee?", false, true);

		if (m_pBtn[BTN_YES]) {
			m_pBtn[BTN_YES]->SetVisible(true);
		}
		if (m_pBtn[BTN_NO]) {
			m_pBtn[BTN_NO]->SetVisible(true);
		}
	} else {
		// "%s is missing."
		m_pTextEdit->AddText(MakeStr("\\c0xffff0000\\c%s\\c0xff000000\\c\xb0\xa1 \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.", pDrop ? pDrop->m_name : "??"), false, true);

		if (m_pBtn[BTN_OK]) {
			m_pBtn[BTN_OK]->SetVisible(true);
		}
	}
}

// compounddlg.cpp:106
void FrCompoundDlg::SetCompoundRes(unsigned char res, sItemInfo& itemInfo) {
	m_compoundRes = res;
	m_resItem = itemInfo;
}

// compounddlg.cpp:112
void FrCompoundDlg::OnProc(const float dt) {
	if (m_pGaugeBar) {

		if (WAbsBySign(m_pGaugeBar->GetPos() - 100.0f) < WMATH_EPSILON && !m_bShowRes && m_compoundRes != 0xff) {



			m_bShowRes = true;
			if (m_pBtn[BTN_OK]) {
				m_pBtn[BTN_OK]->Enable(true);
			}
			if (m_pTextEdit) {

				m_pTextEdit->ClearLine();

				switch (m_compoundRes) {
				case 0:


					m_pTextEdit->AddText("\xC1\xB6\xC7\xD5\xBF\xA1 \xBD\xC7\xC6\xD0\xC7\xDF\xBD\xC0\xB4\xCF\xB4\xD9.", false, true);
					break;
				case 1: {




					const char* name = ItemManager()->GetItemName(m_resItem.m_typeId);

					m_pTextEdit->AddText(MakeStr("\xC3\xE0\xC7\xCF\xC7\xD5\xB4\xCF\xB4\xD9!\n\\c0xffff0000\\c%s\\c0xff000000\\c\xB8\xA6 \xC8\xB9\xB5\xE6\xC7\xCF\xBC\xCC\xBD\xC0\xB4\xCF\xB4\xD9.\n'\xB8\xB6\xC0\xCC\xB7\xEB > \xC0\xC7\xBB\xF3\xBD\xC7'\xBF\xA1\xBC\xAD \xC8\xAE\xC0\xCE\xC7\xCF\xBC\xBC\xBF\xE4.", name ? name : "??"), false, true);

					if (m_pCompRes) {

						IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(m_resItem.m_typeId);
						if (pCommon) {
							m_pCompRes->SetBgImg(pCommon->m_icon);
						}
						m_pCompRes->SetAlpha(0.0f);
					}


					g_audio->PlaySfx("ui_desktop_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);

					break;
				}
				}
			}
		}




		if (m_bShowRes && m_pCompRes) {
			m_pCompRes->SetAlpha(min(dt + m_pCompRes->GetAlpha(), 1.0f));
		}
	}
}

// compounddlg.cpp:160
void FrCompoundDlg::OnYesInit(intptr_t param) {
	m_pBtn[BTN_YES] = DYNAMIC_CAST(FrButton, param);
	if (m_pBtn[BTN_YES]) {
		m_pBtn[BTN_YES]->SetVisible(false);
	}
}

// compounddlg.cpp:167
void FrCompoundDlg::OnYesBtnUp(void) {
	if (m_pBtn[BTN_YES]) {
		m_pBtn[BTN_YES]->SetVisible(false);
	}
	if (m_pBtn[BTN_NO]) {
		m_pBtn[BTN_NO]->SetVisible(false);
	}
	if (m_pBtn[BTN_OK]) {

		m_pBtn[BTN_OK]->SetVisible(true);
		m_pBtn[BTN_OK]->Enable(false);
	}

	if (m_pGaugeBar) {

		m_pGaugeBar->SetVisible(true);
		m_pGaugeBar->SetDestPos(100, false);
		m_pGaugeBar->SetColor(0xffff0000);
	}

	if (m_pTextEdit) {

		IFF_STRUCT::sQuestDrop* pDrop = ItemManager()->FindQuestDrop(m_quest.questDropId);

		m_pTextEdit->ClearLine();
		m_pTextEdit->AddText(MakeStr("\\c0xffff0000\\c%s\\c0xff000000\\c\xC0\xBB \xC1\xB6\xC7\xD5 \xC1\xDF \xC0\xD4\xB4\xCF\xB4\xD9.\n\xC0\xE1\xBD\xC3\xB8\xB8 \xB1\xE2\xB4\xD9\xB7\xC1 \xC1\xD6\xBC\xBC\xBF\xE4.", pDrop ? pDrop->m_name : ""), false, true);
	}


	WSendPacket packet(0x68);
	packet.Encode4(m_quest.m_typeId);
	packet.Send((eSendTo)0);
}

// compounddlg.cpp:202
void FrCompoundDlg::OnNoInit(intptr_t param) {
	m_pBtn[BTN_NO] = DYNAMIC_CAST(FrButton, param);
	if (m_pBtn[BTN_NO]) {
		m_pBtn[BTN_NO]->SetVisible(false);
	}
}

// compounddlg.cpp:209
void FrCompoundDlg::OnNoBtnUp(void) {
	Close(FrCANCEL, true);
}

// compounddlg.cpp:214
void FrCompoundDlg::OnOKInit(intptr_t param) {
	m_pBtn[BTN_OK] = DYNAMIC_CAST(FrButton, param);
	if (m_pBtn[BTN_OK]) {
		m_pBtn[BTN_OK]->SetVisible(false);
	}
}

// compounddlg.cpp:221
void FrCompoundDlg::OnOKBtnUp(void) {
	Close(FrOK, true);
}

// compounddlg.cpp:226
void FrCompoundDlg::OnGaugeBarInit(intptr_t param) {
	m_pGaugeBar = DYNAMIC_CAST(FrGaugeBar, param);
	if (m_pGaugeBar) {

		m_pGaugeBar->SetVisible(false);



		m_pGaugeBar->SetBarSpeed(10.0f);
	}
}

// compounddlg.cpp:241
void FrCompoundDlg::OnTextEditInit(intptr_t param) {
	m_pTextEdit = DYNAMIC_CAST(FrEdit, param);
}

// compounddlg.cpp:247
void FrCompoundDlg::OnCompResInit(intptr_t param) {
	m_pCompRes = DYNAMIC_CAST(FrArea, param);
}

// compounddlg.cpp:252
void FrCompoundDlg::OnCompResOwnerDraw(void) {
	if (m_bShowRes && m_pCompRes && WAbsBySign(m_pCompRes->GetAlpha() - 1.0f) < WMATH_EPSILON) {

		if ((m_resItem.m_typeId >> 26) == 5 || (m_resItem.m_typeId >> 26) == 0x1c) {

			float x = m_pCompRes->GetRect().x + 68.0f;
			x -= m_pTitleFont->GetTextWidth(g_view, MakeStr("%d", m_resItem.m_count));
			m_pTitleFont->Print(g_view, x, m_pCompRes->GetRect().y + 63.0f, MakeStr("%d", m_resItem.m_count), 0, 0xffffffff, 0);
		}
	}
}

// compounddlg.cpp:265
void FrCompoundDlg::SetDlgText(const char* text) {
	if (m_pTextEdit) {

		m_pTextEdit->ClearLine();
		m_pTextEdit->AddText(text, false, true);
	}
}
