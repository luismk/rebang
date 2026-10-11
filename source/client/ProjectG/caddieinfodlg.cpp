// Original: d:\build\custom\temp\code\client\projectg\caddieinfodlg.cpp
#include <stdio.h>
#include <string.h>

#include "CaddieInfoDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frstatic.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "commonutil.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "SoundManager.h"
#include "TaskManager.h"
#include "../shared/classdefine.h"

// caddieinfodlg.cpp:21 (run-time type and factory registration of FrCaddieInfoDlg)
const WRTTI FrCaddieInfoDlg::m_RTTI("FrCaddieInfoDlg", &FrForm::m_RTTI);

IObject* FrCaddieInfoDlgMakeInstance(void) {
	return new FrCaddieInfoDlg;
}

struct __sFrCaddieInfoDlg {
	__sFrCaddieInfoDlg(void) {
		ObjectFactory().AddObjectFunctor(FrCaddieInfoDlgMakeInstance, "FrCaddieInfoDlg");
	}
};
static __sFrCaddieInfoDlg s_registerFrCaddieInfoDlg;

// caddieinfodlg.cpp:23
const sFRESH_MSGMAP* FrCaddieInfoDlg::GetMessageMap(void) const {
	return &FrCaddieInfoDlg::_MsgMap;
}

sFRESH_MSGMAP FrCaddieInfoDlg::_MsgMap = { &FrForm::_MsgMap, &FrCaddieInfoDlg::_MsgEntries[0] };

sFRESH_ENTRY FrCaddieInfoDlg::_MsgEntries[] = {
	// caddieinfodlg.cpp:25
	{ "portrait", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieInfoDlg::OnPortraitInit },
	// caddieinfodlg.cpp:26
	{ "vacation", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieInfoDlg::OnVacationInit },
	// caddieinfodlg.cpp:27
	{ "limit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieInfoDlg::OnLimitInit },
	// caddieinfodlg.cpp:28
	{ "name", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieInfoDlg::OnNameInit },
	// caddieinfodlg.cpp:29
	{ "contract", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieInfoDlg::OnContractInit },
	// caddieinfodlg.cpp:30
	{ "caddiefee", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieInfoDlg::OnCaddieFeeInit },
	// caddieinfodlg.cpp:31
	{ "signup", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieInfoDlg::OnSignUpInit },
	// caddieinfodlg.cpp:32
	{ "signup", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCaddieInfoDlg::OnSignUpBtnUp },
	// caddieinfodlg.cpp:33
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieInfoDlg::OnCancelInit },
	// caddieinfodlg.cpp:34
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCaddieInfoDlg::OnCancelBtnUp },
	// caddieinfodlg.cpp:35
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieInfoDlg::OnOkInit },
	// caddieinfodlg.cpp:36
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCaddieInfoDlg::OnOkBtnUp },
	// caddieinfodlg.cpp:37
	{ "warning_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieInfoDlg::OnWarningButtonInit },
	// caddieinfodlg.cpp:38
	{ "warning_text", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCaddieInfoDlg::OnWarningTextInit },

	// caddieinfodlg.cpp:40
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// caddieinfodlg.cpp:44
FrCaddieInfoDlg::FrCaddieInfoDlg(void) {
	memset(&m_caddieInfo, 0, sizeof(m_caddieInfo));

	m_pPortraitArea = 0;
	m_pVacationArea = 0;
	m_pLimitArea = 0;
	m_pNameEdit = 0;
	m_pContractWnd = 0;
	m_pContractStatic = 0;
	m_pCaddieFeeEdit = 0;
	m_pSignUpBtn = 0;
	m_pOkBtn = 0;
	m_pCancelBtn = 0;
	m_pWarningBtn = 0;
	m_pWarningText = 0;

	m_pContractForm = 0;
}

// caddieinfodlg.cpp:74
void FrCaddieInfoDlg::SetCaddieInfo(const sCaddieInfo& info) {
	m_caddieInfo = WSingleton<CSharedDoc>::Instance()->m_caddieMap[(unsigned long)info.id];

	if (m_caddieInfo.contract == 1) {
		m_pWarningBtn->SetStatus(FrButton::PRESSED);
	} else {
		m_pWarningBtn->SetStatus(FrButton::NORMAL);
	}

	std::map<unsigned int, IFF_STRUCT::sCaddie>::const_iterator it;
	for (it = ItemManager()->m_caddieMap.begin(); it != ItemManager()->m_caddieMap.end(); ++it) {
		if (it->second.m_typeId == m_caddieInfo.typeId) {
			if ((it->second.m_level & 0x7F) > WSingleton<CSharedDoc>::Instance()->m_myLevel) {
				m_pSignUpBtn->Enable(false);
			}
			return;
		}
	}
}

// caddieinfodlg.cpp:106
void FrCaddieInfoDlg::OnPortraitInit(intptr_t param) {
	m_pPortraitArea = DYNAMIC_CAST(FrArea, param);
}

// caddieinfodlg.cpp:111
void FrCaddieInfoDlg::OnVacationInit(intptr_t param) {
	m_pVacationArea = DYNAMIC_CAST(FrArea, param);
}

// caddieinfodlg.cpp:116
void FrCaddieInfoDlg::OnLimitInit(intptr_t param) {
	m_pLimitArea = DYNAMIC_CAST(FrArea, param);
}

// caddieinfodlg.cpp:121
void FrCaddieInfoDlg::OnNameInit(intptr_t param) {
	m_pNameEdit = DYNAMIC_CAST(FrEdit, param);
}

// caddieinfodlg.cpp:126
void FrCaddieInfoDlg::OnContractInit(intptr_t param) {
	m_pContractStatic = DYNAMIC_CAST(FrStatic, param);
}

// caddieinfodlg.cpp:131
void FrCaddieInfoDlg::OnCaddieFeeInit(intptr_t param) {
	m_pCaddieFeeEdit = DYNAMIC_CAST(FrEdit, param);
}

// caddieinfodlg.cpp:136
void FrCaddieInfoDlg::OnSignUpInit(intptr_t param) {
	m_pSignUpBtn = DYNAMIC_CAST(FrButton, param);
}

// caddieinfodlg.cpp:141
void FrCaddieInfoDlg::OnOkInit(intptr_t param) {
	m_pOkBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pOkBtn) {
		m_pOkBtn->SetVisible(false);
	}
}

// caddieinfodlg.cpp:149
void FrCaddieInfoDlg::OnCancelInit(intptr_t param) {
	m_pCancelBtn = DYNAMIC_CAST(FrButton, param);
}

// caddieinfodlg.cpp:154
void FrCaddieInfoDlg::OnWarningButtonInit(intptr_t param) {
	m_pWarningBtn = DYNAMIC_CAST(FrButton, param);
}

// caddieinfodlg.cpp:159
void FrCaddieInfoDlg::OnWarningTextInit(intptr_t param) {
	m_pWarningText = DYNAMIC_CAST(FrStatic, param);
}

// caddieinfodlg.cpp:173
void FrCaddieInfoDlg::OnSignUpBtnUp(void) {
	if (m_pSignUpBtn) {
		m_pSignUpBtn->Enable(false);
	}

	if (m_pCancelBtn) {
		m_pCancelBtn->Enable(false);
	}

	IFF_STRUCT::sCaddie* pCaddie = ItemManager()->FindCaddie(m_caddieInfo.typeId);

	if (pCaddie == 0) {
		return;
	}

	if (m_pContractForm == 0) {
		m_pContractForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "cad_contract", 0);
	}
	m_pContractForm->SetMessage("\xC4\xB3\xB5\xF0\xB8\xA6 \xC0\xE7\xB0\xED\xBF\xEB\xC7\xCF\xB8\xE9 \xBE\xC6\xB7\xA1\xBF\xCD \xB0\xB0\xC0\xCC \xB1\xDD\xBE\xD7\xC0\xCC \\c0xffff0000\\c\xB0\xA8\xBC\xD2\\c0xff000000\\c\xC7\xD5\xB4\xCF\xB4\xD9.\n\n\xC4\xB3\xB5\xF0\xB8\xA6 \\c0xffff0000\\c\xC7\xD1\xB4\xDE\xB0\xA3 \\c0xff000000\\c\xC0\xE7\xB0\xED\xBF\xEB \xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);

	FrStatic* pFee = DYNAMIC_CAST(FrStatic, m_pContractForm->FindChildByName("fee"));
	if (pFee) {
		pFee->SetCaption(MakeStr("%d", pCaddie->fee));
	}

	FrStatic* pMyPang = DYNAMIC_CAST(FrStatic, m_pContractForm->FindChildByName("mypang"));
	if (pMyPang) {
		pMyPang->SetCaption(MakeStr("%I64d", WSingleton<CSharedDoc>::Instance()->m_myPang));
	}

	FrStatic* pRemainPang = DYNAMIC_CAST(FrStatic, m_pContractForm->FindChildByName("remainpang"));
	if (pRemainPang) {
		pRemainPang->SetCaption(MakeStr("%I64d", WSingleton<CSharedDoc>::Instance()->m_myPang - pCaddie->fee));
	}

	m_pContractForm->Open((FRESH_RESULT_CALLBACK)&FrCaddieInfoDlg::OnSignUpConfirmDlgResult, 1);
}

// caddieinfodlg.cpp:209
void FrCaddieInfoDlg::OnOkBtnUp(void) {
	SendCaddieWarningCheckOption();

	Close(FrOK, true);
}

// caddieinfodlg.cpp:216
void FrCaddieInfoDlg::OnCancelBtnUp(void) {
	SendCaddieWarningCheckOption();

	Close(FrCANCEL, true);
}

// caddieinfodlg.cpp:232
bool FrCaddieInfoDlg::OnSignUpConfirmDlgResult(int result, FrForm* pForm) {
	m_pContractForm = 0;
	if (result == 1 || result == 2) {
		SendCaddieWarningCheckOption();
	}

	if (result == 1) {
		WSendPacket packet((enumClientPacket)0x39);
		packet.Encode4(m_caddieInfo.id);
		packet.Send(SEND_TO_DEFAULT);

		AfxGetTask()->PassMsgToActor("Shop", MsgObject(0, 1, 0, 0, 0, 0, 0));
		AfxGetTask()->PassMsgToActor("RealMyRoom", MsgObject(0, 1, 0, 0, 0, 0, 0));
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 1, 0, 0, 0, 0, 0));

		g_audio->PlaySfx("caddie_return", 0, 0, 0, 0, 0.5f, 200.0f);
		g_audio->PlayBGM("caddie_return.mp3", true, true);

		Close(FrOK, true);
	} else {
		if (m_pSignUpBtn && m_pSignUpBtn->IsVisible()) {
			m_pSignUpBtn->Enable(true);
		}

		if (m_pCancelBtn && m_pCancelBtn->IsVisible()) {
			m_pCancelBtn->Enable(true);
		}
	}

	return true;
}

// caddieinfodlg.cpp:280
void FrCaddieInfoDlg::SendCaddieWarningCheckOption(void) {
	if (m_pWarningBtn->GetStatus() == FrButton::PRESSED) {
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
}

// caddieinfodlg.cpp:309
bool FrCaddieInfoDlg::OnInit(void) {
	if (m_caddieInfo.typeId == 0)
		return false;

	IFF_STRUCT::sCaddie* pCaddie = ItemManager()->FindCaddie(m_caddieInfo.typeId);


	bool bShown = false;
	if (m_pPortraitArea) {

		if (pCaddie) {


			m_pPortraitArea->SetBgImg(pCaddie->m_icon);


			if (m_pNameEdit) {
				char text[256];
				bShown = true;


				sprintf(text, "\xC0\xCC  \xB8\xA7: %s", pCaddie->m_name);

				if (m_caddieInfo.level <= 3 && (pCaddie->flags & 0xE0) != 0xA0) {


					unsigned long exp = 0;
					unsigned long curExp = 0;
					if (m_caddieInfo.level < 3) {

						exp = WSingleton<CSharedDoc>::Instance()->m_caddieLevelExp[m_caddieInfo.level].m_exp;
						curExp = m_caddieInfo.experience;
					}
					strcat(text, MakeStr("\n\xB7\xB9  \xBA\xA7: %d / %d ( %d / %d )", m_caddieInfo.level + 1, 4, curExp, exp));
				}


				if ((m_caddieInfo.flags & 2) && pCaddie->fee > 0) {

					if (m_caddieInfo.days > 0) {
						strcat(text, MakeStr("\n\xB3\xB2\xC0\xBA \xB0\xED\xBF\xEB\xB1\xE2\xB0\xA3: %d\xC0\xCF", m_caddieInfo.days));
					} else {
						strcat(text, "\n\xC8\xDE\xB0\xA1\xC1\xDF");
					}
				}



				if (m_caddieInfo.cadItemTypeId & 0x1FFF) {

					IFF_STRUCT::sCadItem* pCadItem = ItemManager()->FindCadItem(m_caddieInfo.cadItemTypeId);
					strcat(text, MakeStr("\n\xBE\xF7\xB1\xD7\xB7\xB9\xC0\xCC\xB5\xE5: \\c0xffff0000\\c%s\\c0xff000000\\c", pCadItem->m_name));


					if (m_caddieInfo.hours > 24) {
						strcat(text, MakeStr("\n\xC4\xB3\xB5\xF0\xB2\xD9\xB9\xCC\xB1\xE2 \xB3\xB2\xC0\xBA \xB1\xE2\xB0\xA3: \\c0xffff0000\\c%d\xC0\xCF\\c0xff000000\\c", m_caddieInfo.hours / 24));
					} else {
						strcat(text, MakeStr("\n\xC4\xB3\xB5\xF0\xB2\xD9\xB9\xCC\xB1\xE2 \xB3\xB2\xC0\xBA \xB1\xE2\xB0\xA3: \\c0xffff0000\\c%d\xBD\xC3\xB0\xA3\\c0xff000000\\c", m_caddieInfo.hours));
					}



					if ((m_caddieInfo.flags & 2) && pCaddie->fee > 0 && m_caddieInfo.days > 0) {
						HideContractControls();
					}
				}


				m_pNameEdit->AddText(text, false, true);

				if ((m_caddieInfo.days > 0 && (m_caddieInfo.flags & 3) && pCaddie->fee > 0) || pCaddie->fee == 0 || m_caddieInfo.id == -1) {

					IFF_STRUCT::sDesc* pDesc = ItemManager()->FindDesc(pCaddie->m_typeId);
					if (pDesc) {
						SetMessage(pDesc->m_text, false);
					}
				} else {



					SetMessage(MakeStr("\xC7\xF6\xC0\xE7 \xC4\xB3\xB5\xF0\xB4\xC2 \xC8\xDE\xB0\xA1\xC1\xDF\xC0\xCC\xB9\xC7\xB7\xCE \xC4\xB3\xB5\xF0 \xB2\xD9\xB9\xCC\xB1\xE2\xB8\xA6 \xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. %d \xC6\xCE\xC0\xBB \xC1\xF6\xB1\xDE\xC7\xCF\xBF\xA9 \xC4\xB3\xB5\xF0\xB8\xA6 \xC8\xDE\xB0\xA1\xBA\xB9\xB1\xCD\xBD\xC3\xC5\xB0\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", pCaddie->fee), false);
				}
			}
		} else {

			m_pPortraitArea->SetBgImg("hide");
		}
	}

	if (!bShown) {

		if (m_pNameEdit) {
			m_pNameEdit->AddText("\xC0\xCC\xB8\xA7: \xBA\xD2\xB8\xED\n\n\xB7\xB9\xBA\xA7: \xBA\xD2\xB8\xED", false, true);
		}
		SetMessage("\xBA\xA3\xC0\xCF\xBF\xA1 \xBD\xCE\xC0\xCE \xC1\xA4\xC3\xBC\xBA\xD2\xB8\xED\xC0\xC7 \xC1\xB8\xC0\xE7", false);
	}

	if (pCaddie) {


		if ((m_caddieInfo.flags & 2) && pCaddie->fee > 0) {

			m_pWarningBtn->SetVisible(true);
			m_pWarningText->SetVisible(true);
		} else {



			m_pWarningBtn->SetVisible(false);
			m_pWarningText->SetVisible(false);
		}



		if ((m_caddieInfo.flags & 2) && m_caddieInfo.days == 0 && pCaddie->fee > 0 && m_pCaddieFeeEdit) {



			char fee[64];
			sprintf(fee, "%d \xC6\xCE", pCaddie->fee);
			m_pCaddieFeeEdit->SetLine(1, fee, 0, false, 0);




			SetDesc("'\xC1\xF6\xB1\xDE' \xB9\xF6\xC6\xB0\xC0\xBB \xB4\xAD\xB7\xAF \xC4\xB3\xB5\xF0\xB8\xA6 \xC0\xE7\xB0\xED\xBF\xEB \xC7\xD2 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.");



		} else {

			HideContractControls();
		}



		if (WSingleton<CSharedDoc>::Instance()->m_myLevel < (pCaddie->m_level & 0x7F)) {

			if (m_pLimitArea) {
				m_pLimitArea->SetVisible(true);
			}

			SetDesc("\xB7\xB9\xBA\xA7 \xC1\xA6\xC7\xD1\xC0\xB8\xB7\xCE \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xC4\xB3\xB5\xF0\xC0\xD4\xB4\xCF\xB4\xD9.");
		}
	}




	return true;
}

// caddieinfodlg.cpp:453
void FrCaddieInfoDlg::HideContractControls(void) {
	if (m_pVacationArea) {
		m_pVacationArea->SetVisible(false);
	}

	if (m_pContractWnd) {
		m_pContractWnd->SetVisible(false);
	}

	if (m_pContractStatic) {
		m_pContractStatic->SetVisible(false);
	}

	if (m_pCaddieFeeEdit) {
		m_pCaddieFeeEdit->SetVisible(false);
	}

	if (m_pSignUpBtn) {
		m_pSignUpBtn->SetVisible(false);
	}

	if (m_pLimitArea) {
		m_pLimitArea->SetVisible(false);
	}

	if (m_pCancelBtn) {
		m_pCancelBtn->SetVisible(false);
	}

	if (m_pOkBtn) {
		m_pOkBtn->SetVisible(true);
	}

	SetDesc("");
}

// caddieinfodlg.cpp:492
void FrCaddieInfoDlg::EndContract(int param) {
	if (param == 2) {

		m_caddieInfo.days = 30;
		HideContractControls();

		if (m_pOkBtn && m_pOkBtn->IsVisible()) {
			m_pOkBtn->Enable(true);
		}
	} else {
		if (m_pSignUpBtn && m_pSignUpBtn->IsVisible()) {
			m_pSignUpBtn->Enable(true);
		}

		if (m_pCancelBtn && m_pCancelBtn->IsVisible()) {
			m_pCancelBtn->Enable(true);
		}
	}
}
