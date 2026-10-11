// Original: d:\build\custom\temp\code\client\projectg\enchantdlg.cpp
#include "EnchantDlg.h"
#include "SharedDoc.h"
#include "ObjectFactory.h"
#include "Packet.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frgaugebar.h"
#include "../shared/Localize.h"
#include "actor.h"
#include "TaskManager.h"
#include "SoundManager.h"
#include "commonutil.h"
#include "wmath.h"

// enchantdlg.cpp:17 (run-time type and factory registration of FrEnchantDlg)
const WRTTI FrEnchantDlg::m_RTTI("FrEnchantDlg", &FrForm::m_RTTI);

IObject* FrEnchantDlgMakeInstance(void) {
	return new FrEnchantDlg;
}

struct __sFrEnchantDlg {
	__sFrEnchantDlg(void) {
		ObjectFactory().AddObjectFunctor(FrEnchantDlgMakeInstance, "FrEnchantDlg");
	}
};
static __sFrEnchantDlg s_registerFrEnchantDlg;

// enchantdlg.cpp:19
const sFRESH_MSGMAP* FrEnchantDlg::GetMessageMap(void) const {
	return &FrEnchantDlg::_MsgMap;
}

sFRESH_MSGMAP FrEnchantDlg::_MsgMap = { &FrForm::_MsgMap, &FrEnchantDlg::_MsgEntries[0] };

sFRESH_ENTRY FrEnchantDlg::_MsgEntries[] = {
	// enchantdlg.cpp:21
	{ "yes", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEnchantDlg::OnYesInit },
	// enchantdlg.cpp:22
	{ "yes", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrEnchantDlg::OnYesBtnUp },
	// enchantdlg.cpp:23
	{ "no", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEnchantDlg::OnNoInit },
	// enchantdlg.cpp:24
	{ "no", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrEnchantDlg::OnNoBtnUp },
	// enchantdlg.cpp:25
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEnchantDlg::OnOKInit },
	// enchantdlg.cpp:26
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrEnchantDlg::OnOKBtnUp },
	// enchantdlg.cpp:27
	{ "gauge", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEnchantDlg::OnGaugeBarInit },
	// enchantdlg.cpp:28
	{ "etext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEnchantDlg::OnTextEditInit },
	// enchantdlg.cpp:29
	{ "title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEnchantDlg::OnTitleInit },
	// enchantdlg.cpp:30
	{ "base", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEnchantDlg::OnBaseInit },

	// enchantdlg.cpp:32
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// enchantdlg.cpp:36
FrEnchantDlg::FrEnchantDlg(void) {
	m_grade = 0;
	m_gradeName = 0;
	m_bStarted = false;
	m_price = 0;
	m_resultCode = 0;
}

// enchantdlg.cpp:45
void FrEnchantDlg::OnTitleInit(intptr_t param) {
	m_pTitle = DYNAMIC_CAST(FrArea, param);
}

// enchantdlg.cpp:50
void FrEnchantDlg::OnBaseInit(intptr_t param) {
	m_pBase = DYNAMIC_CAST(FrArea, param);
}

// enchantdlg.cpp:55
void FrEnchantDlg::SetType(unsigned char type, unsigned char grade) {
	m_type = type;
	m_grade = grade;

	switch (m_grade) {
	case 0:

		m_gradeColor = 0xffff0000;
		m_gradeName = "\xc6\xc4\xbf\xf6";
		break;

	case 1:
		m_gradeColor = 0xffffa100;
		m_gradeName = "\xc4\xc1\xc6\xae\xb7\xd1";
		break;

	case 2:
		m_gradeColor = 0xff14cf00;
		m_gradeName = "\xc1\xa4\xc8\xae\xb5\xb5";
		break;

	case 3:
		m_gradeColor = 0xff00c9ca;
		m_gradeName = "\xbd\xba\xc7\xc9";
		break;

	case 4:
		m_gradeColor = 0xff941de5;
		m_gradeName = "\xc4\xbf\xba\xea";
		break;
	}

	IFF_STRUCT::sEnchant* pEnchant;
	if (m_type & 1) {

		m_itemValue = CSharedDoc::Instance()->m_myClubId;

		if (m_pTitle) {
			m_pTitle->SetBgImg((m_type & 2) ? "club_dn" : "club_up");
		}
		if (m_pBase) {
			m_pBase->SetBgImg("club_up_base");
		}

		std::map<unsigned int, sItemInfo>::iterator it = CSharedDoc::Instance()->m_myItemMap.find(m_itemValue);
		if (it == CSharedDoc::Instance()->m_myItemMap.end()) {
			return;
		}




		pEnchant = ItemManager()->FindEnchant(((m_grade | 0x340) << 20) | (*it).second.m_clubStatLevel[m_grade]);
	} else {

		m_itemValue = CSharedDoc::Instance()->m_myCharacterId;

		if (m_pTitle) {
			m_pTitle->SetBgImg((m_type & 2) ? "cha_dn" : "cha_up");
		}
		if (m_pBase) {
			m_pBase->SetBgImg("cha_up_base");
		}

		std::map<unsigned int, sCharacterInfo>::iterator it = CSharedDoc::Instance()->m_characterMap.find(m_itemValue);
		if (it == CSharedDoc::Instance()->m_characterMap.end()) {
			return;
		}

		// The character's stat levels: one signed byte per stat at 0x6A
		// (sCharacterInfo::unknown6A, still declared as ulong + byte).
		pEnchant = ItemManager()->FindEnchant(((m_grade | 0x340) << 20) | ((const char*)&(*it).second.unknown6A)[m_grade]);
	}
	if (pEnchant == 0) {
		return;
	}

	if (m_type & 2) {
		m_price = 0;
	} else {
		m_price = (unsigned int)pEnchant->m_value;
	}

	if (m_pTextEdit) {
		m_pTextEdit->ClearLine();
	}

	if (CSharedDoc::Instance()->m_myPang >= m_price) {

		if (m_pTextEdit) {

			m_pTextEdit->SetClientRect(WRect(45.0f, 45.0f, 200.0f, 100.0f));

			if (m_type & 2) {

				m_pTextEdit->AddLine(MakeStr("%s\xc0\xc7 \\c0xffff0000\\c%s\xb4\xc9\xb7\xc2\\c0xff000000\\c\xc0\xbb", (m_type & 1) ? "\xc0\xe5\xba\xf1" : "\xc4\xb3\xb8\xaf\xc5\xcd", m_gradeName), 0, false);
				m_pTextEdit->AddLine("\xb4\xd9\xbf\xee\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5 \xc7\xcf\xbd\xc3\xb0\xda\xbd\xc0\xb4\xcf\xb1\xee?", 0, false);

			} else {

				m_pTextEdit->AddLine(MakeStr("%s\xc0\xc7 \\c0xffff0000\\c%s\xb4\xc9\xb7\xc2\\c0xff000000\\c\xc0\xbb %s", (m_type & 1) ? "\xc0\xe5\xba\xf1" : "\xc4\xb3\xb8\xaf\xc5\xcd", m_gradeName, "\xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5"), 0, false);
				m_pTextEdit->AddLine(MakeStr("\xc7\xcf\xb1\xe2 \xc0\xa7\xc7\xd8\xbc\xad\xb4\xc2 \\c0xffff0000\\c%I64d\xc6\xce\\c0xff000000\\c\xc0\xcc \xc7\xca\xbf\xe4\xc7\xd5\xb4\xcf\xb4\xd9.", m_price), 0, false);
				m_pTextEdit->AddLine(MakeStr("%s \xc7\xcf\xbd\xc3\xb0\xda\xbd\xc0\xb4\xcf\xb1\xee?", (m_type & 2) ? "\xb4\xd9\xbf\xee\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5" : "\xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5"), 0, false);
			}
		}



		if (m_pYesBtn) {
			m_pYesBtn->SetVisible(true);
		}
		if (m_pNoBtn) {

			m_pNoBtn->SetVisible(true);
		}
	} else {

		if (m_pTextEdit) {

			m_pTextEdit->SetClientRect(WRect(45.0f, 55.0f, 200.0f, 100.0f));
			m_pTextEdit->AddLine(MakeStr("\\c0xffff0000\\c%s\xb4\xc9\xb7\xc2\\c0xff000000\\c\xc0\xbb %s", m_gradeName, (m_type & 2) ? "\xb4\xd9\xbf\xee\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5" : "\xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5"), 0, false);
			m_pTextEdit->AddLine(MakeStr("\xc7\xcf\xb1\xe2 \xc0\xa7\xc7\xd8\xbc\xad\xb4\xc2 \\c0xffff0000\\c%I64d\xc6\xce\\c0xff000000\\c\xc0\xcc \xb4\xf5 \xc7\xca\xbf\xe4\xc7\xd5\xb4\xcf\xb4\xd9.", m_price - CSharedDoc::Instance()->m_myPang), 0, false);
		}



		if (m_pOKBtn) {
			m_pOKBtn->SetVisible(true);
		}
	}
}

// enchantdlg.cpp:174
void FrEnchantDlg::OnProc(const float dt) {
	if (m_pGaugeBar) {


		if (WAbsBySign(m_pGaugeBar->GetPos() - 100.0f) < 0.00001f && !m_bStarted && m_resultCode) {



			m_bStarted = true;
			if (m_pOKBtn) {
				m_pOKBtn->Enable(true);
			}
			if (m_pTextEdit) {

				m_pTextEdit->ClearLine();

				switch (m_resultCode) {
				case 1:


					m_pTextEdit->SetClientRect(WRect(45.0f, 60.0f, 200.0f, 80.0f));
					if (m_type & 1) {
						m_pTextEdit->AddLine(MakeStr("\xc0\xe5\xba\xf1 \xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xb8\xa6 \xbc\xba\xb0\xf8\xc7\xdf\xbd\xc0\xb4\xcf\xb4\xd9.", m_gradeName), 0, false);
					} else {
						m_pTextEdit->AddLine(MakeStr("\xc4\xb3\xb8\xaf\xc5\xcd \xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xb8\xa6 \xbc\xba\xb0\xf8\xc7\xdf\xbd\xc0\xb4\xcf\xb4\xd9.", m_gradeName), 0, false);
					}

					g_audio->PlaySfx("enchant_up", 0, 0, 0, 0, 0.5f, 200.0f);

					AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0xd3, m_type & 1, 0, 0, 0, 0);
					break;

				case 2:


					m_pTextEdit->SetClientRect(WRect(45.0f, 60.0f, 200.0f, 80.0f));
					if (m_type & 1) {
						m_pTextEdit->AddLine(MakeStr("\xc0\xe5\xba\xf1 \xb4\xd9\xbf\xee\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xb8\xa6 \xbc\xba\xb0\xf8\xc7\xdf\xbd\xc0\xb4\xcf\xb4\xd9.", m_gradeName), 0, false);
					} else {
						m_pTextEdit->AddLine(MakeStr("\xc4\xb3\xb8\xaf\xc5\xcd \xb4\xd9\xbf\xee\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xb8\xa6 \xbc\xba\xb0\xf8\xc7\xdf\xbd\xc0\xb4\xcf\xb4\xd9.", m_gradeName), 0, false);
					}




					g_audio->PlayBGM("enchant_down.mp3", true, true);


					AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0xd3, m_type & 1, 0, 0, 0, 0);
					break;

				case 6:
					if (m_type & 2) {
						m_pTextEdit->AddLine("\xb4\xd9\xbf\xee\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xb8\xa6 \xbd\xc7\xc6\xd0\xc7\xdf\xbd\xc0\xb4\xcf\xb4\xd9.", 0, false);
					} else {
						m_pTextEdit->AddLine("\xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xb8\xa6 \xbd\xc7\xc6\xd0\xc7\xdf\xbd\xc0\xb4\xcf\xb4\xd9.", 0, false);
					}
					break;

				case 3:
					if (m_type & 2) {
						m_pTextEdit->AddLine("\xb4\xd9\xbf\xee\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xb8\xa6 \xc0\xa7\xc7\xd1 \xc6\xce\xc0\xcc \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.", 0, false);
					} else {
						m_pTextEdit->AddLine("\xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xb8\xa6 \xc0\xa7\xc7\xd1 \xc6\xce\xc0\xcc \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.", 0, false);
					}
					break;

				case 4:
					m_pTextEdit->AddLine("\xb4\xf5 \xc0\xcc\xbb\xf3 \xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xc7\xd2 \xbc\xf6 \xc0\xd6\xb4\xc2 \xbd\xbd\xb7\xd4\xc0\xcc \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, false);
					break;

				case 5:
					m_pTextEdit->AddLine("\xb4\xf5 \xc0\xcc\xbb\xf3 \xb4\xd9\xbf\xee\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5 \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, false);
					break;
				}
			}
		}
	}
}

// enchantdlg.cpp:246
void FrEnchantDlg::OnYesInit(intptr_t param) {
	m_pYesBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pYesBtn) {
		m_pYesBtn->SetVisible(false);
	}
}

// enchantdlg.cpp:253
void FrEnchantDlg::OnYesBtnUp(void) {
	if (m_pYesBtn) {
		m_pYesBtn->SetVisible(false);
	}
	if (m_pNoBtn) {
		m_pNoBtn->SetVisible(false);
	}
	if (m_pOKBtn) {

		m_pOKBtn->SetVisible(true);
		m_pOKBtn->Enable(false);
	}

	if (m_pGaugeBar) {

		m_pGaugeBar->SetVisible(true);
		m_pGaugeBar->SetDestPos(100, false);
		m_pGaugeBar->SetColor(m_gradeColor);
		m_pGaugeBar->SetBarSpeed(300.0f);
	}






	if (m_pTextEdit) {

		m_pTextEdit->ClearLine();
		m_pTextEdit->SetClientRect(WRect(45.0f, 55.0f, 220.0f, 100.0f));

		if (m_type & 1) {

			if (m_type & 2) {
				m_pTextEdit->AddLine("\xc0\xe5\xba\xf1\xc0\xc7 \xbc\xd3\xbc\xba\xc0\xbb \xb4\xd9\xbf\xee\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5 \xc1\xdf\xc0\xd4\xb4\xcf\xb4\xd9.", 0, false);
			} else {
				m_pTextEdit->AddLine("\xc0\xe5\xba\xf1\xc0\xc7 \xbc\xd3\xbc\xba\xc0\xbb \xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5 \xc1\xdf\xc0\xd4\xb4\xcf\xb4\xd9.", 0, false);
			}

		} else {

			if (m_type & 2) {
				m_pTextEdit->AddLine("\xc4\xb3\xb8\xaf\xc5\xcd \xb4\xc9\xb7\xc2\xc0\xbb \xb4\xd9\xbf\xee\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5 \xc1\xdf\xc0\xd4\xb4\xcf\xb4\xd9.", 0, false);
			} else {
				m_pTextEdit->AddLine("\xc4\xb3\xb8\xaf\xc5\xcd \xb4\xc9\xb7\xc2\xc0\xbb \xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5 \xc1\xdf\xc0\xd4\xb4\xcf\xb4\xd9.", 0, false);
			}
		}


		m_pTextEdit->AddLine("\xc0\xe1\xbd\xc3\xb8\xb8 \xb1\xe2\xb4\xd9\xb7\xc1 \xc1\xd6\xbc\xbc\xbf\xe4.", 0, false);
	}



	WSendPacket packet((enumClientPacket)0x4b);
	packet.Encode1(m_type);







	packet.Encode1(m_grade);
	packet.Encode4(m_itemValue);
	if (IsLocalContent((localContentType_t)0x7f)) {

		std::map<unsigned int, sCharacterInfo>::iterator it = CSharedDoc::Instance()->m_characterMap.find(m_itemValue);
		packet.EncodeBuffer(&(*it).second, sizeof(sCharacterInfo));
	}

	packet.Send(SEND_TO_DEFAULT);
}

// enchantdlg.cpp:322
void FrEnchantDlg::OnNoInit(intptr_t param) {
	m_pNoBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pNoBtn) {
		m_pNoBtn->SetVisible(false);
	}
}

// enchantdlg.cpp:329
void FrEnchantDlg::OnNoBtnUp(void) {
	Close(FrNONE, true);
}

// enchantdlg.cpp:334
void FrEnchantDlg::OnOKInit(intptr_t param) {
	m_pOKBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pOKBtn) {
		m_pOKBtn->SetVisible(false);
	}
}

// enchantdlg.cpp:341
void FrEnchantDlg::OnOKBtnUp(void) {
	Close(FrOK, true);
}

// enchantdlg.cpp:346
void FrEnchantDlg::OnGaugeBarInit(intptr_t param) {
	m_pGaugeBar = DYNAMIC_CAST(FrGaugeBar, param);
	if (m_pGaugeBar) {
		m_pGaugeBar->SetVisible(false);
		m_pGaugeBar->SetBarSpeed(10.0f);
	}
}

// enchantdlg.cpp:357
void FrEnchantDlg::OnTextEditInit(intptr_t param) {
	m_pTextEdit = DYNAMIC_CAST(FrEdit, param);
}
