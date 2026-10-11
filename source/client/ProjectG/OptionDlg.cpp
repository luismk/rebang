// Original: d:\build\custom\temp\code\client\projectg\optiondlg.cpp
#include "NetworkSystem.h"
#include "OptionDlg.h"
#include "ClientSetting.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frbutton.h"
#include "frstatic.h"
#include "fredit.h"
#include "frcombobox.h"
#include "frgaugebar.h"
#include "ProjectG.h"
#include "SharedDoc.h"
#include "SoundManager.h"
#include "SchoolDlg.h"
#include "TaskManager.h"
#include "GolfTask.h"
#include "ChatMsg.h"
#include "messengerdlg.h"
#include "LobbyTask.h"
#include "Localize.h"
#include "InputManager.h"
#include "wresrcmng.h"
#include "commonutil.h"
#include "wutil.h"
#include <string.h>

// clientsetting.h:386
float COption::gGetMouseSensitivity(void) const {
	return m_option.m_mouseSensitivity;
}

// optiondlg.h:261
void FrOptionDlg::sTab::Init(void) {
}

// optiondlg.cpp:125
FrOptionDlg::sTab::sTab(void) : m_bSelected(true), m_pTabBtn(0), m_numWnds(0), m_ppWnds(0), m_pppHoverWnds(0) {
}

// optiondlg.cpp:130
void FrOptionDlg::sTab::SetSelected(bool bSelected) {
	m_bSelected = bSelected;

	if (!bSelected) {
		m_pTabBtn->SetStatus(FrButton::NORMAL);
	}

	if (m_ppWnds) {
		for (int i = 0; i < m_numWnds; i++) {
			if (m_ppWnds[i]) {
				if (m_pbDisabled && m_pbDisabled[i]) {
					m_ppWnds[i]->Enable(false);
				} else {
					m_ppWnds[i]->Enable(m_bSelected);
				}

				m_ppWnds[i]->SetVisible(m_bSelected);
			}
		}
	}

	if (m_pppHoverWnds) {
		for (int i = 0; i < m_numWnds; i++) {
			if (m_pppHoverWnds[i] && *m_pppHoverWnds[i]) {
				if (m_pbDisabled && m_pbDisabled[i]) {
					(*m_pppHoverWnds[i])->Enable(false);
				} else {
					(*m_pppHoverWnds[i])->Enable(m_bSelected);
				}

				(*m_pppHoverWnds[i])->EnableHover(m_bSelected, 0);
				(*m_pppHoverWnds[i])->SetVisible(m_bSelected);
			}
		}
	}
}

// optiondlg.cpp:173
FrOptionDlg::sGameTab::sGameTab(void) {
	m_numWnds = 13;
	m_ppWnds = (FrWnd**)m_pStatics;
	m_pppHoverWnds = m_pHoverWnds;
	m_pbDisabled = 0;

	memset(m_pStatics, 0, sizeof(m_pStatics));

	m_pMsSensiGauge = 0;
	m_pWhisperBtn = 0;
	m_pInvitationBtn = 0;
	m_pFriendConfirmBtn = 0;
	m_pTransChatWinBtn = 0;
	m_pUiEffectBtn = 0;
	m_pNickEdit = 0;
	m_pChangeNickBtn = 0;
	m_pSchoolEdit = 0;
	m_pChangeSchoolBtn = 0;
	m_pPowerGaugeBtn = 0;
	m_pHideGUIBtn = 0;
	m_pHidePIBtn = 0;
	// m_pTerrainBtn is not cleared (the original skips it).
	m_pCutinDisplayBtn = 0;

	m_pHoverWnds[0] = (FrWnd**)&m_pMsSensiGauge;
	m_pHoverWnds[1] = (FrWnd**)&m_pWhisperBtn;
	m_pHoverWnds[2] = (FrWnd**)&m_pInvitationBtn;
	m_pHoverWnds[3] = (FrWnd**)&m_pFriendConfirmBtn;
	m_pHoverWnds[4] = (FrWnd**)&m_pTransChatWinBtn;
	m_pHoverWnds[5] = (FrWnd**)&m_pUiEffectBtn;
	m_pHoverWnds[6] = (FrWnd**)&m_pNickEdit;
	m_pHoverWnds[7] = (FrWnd**)&m_pSchoolEdit;
	m_pHoverWnds[8] = (FrWnd**)&m_pPowerGaugeBtn;
	m_pHoverWnds[9] = (FrWnd**)&m_pHideGUIBtn;
	m_pHoverWnds[10] = (FrWnd**)&m_pHidePIBtn;
	m_pHoverWnds[11] = (FrWnd**)&m_pTerrainBtn;

	m_pHoverWnds[12] = (FrWnd**)&m_pCutinDisplayBtn;

	m_bEnableChangeNick = false;
}

// optiondlg.cpp:216
void FrOptionDlg::sGameTab::SetSelected(bool bSelected) {
	sTab::SetSelected(bSelected);

	if (bSelected) {
		if (m_pChangeNickBtn) {
			m_pChangeNickBtn->SetVisible(true);
			m_pChangeNickBtn->Enable(m_bEnableChangeNick);
		}

		if (m_pChangeSchoolBtn) {
			m_pChangeSchoolBtn->SetVisible(true);
		}
	} else {
		if (m_pChangeNickBtn) {
			m_pChangeNickBtn->SetVisible(false);
		}

		if (m_pChangeSchoolBtn) {
			m_pChangeSchoolBtn->SetVisible(false);
		}
	}
}

// optiondlg.cpp:242
void FrOptionDlg::sGameTab::Init(void) {
	if (OnlinePlay() && !((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI))) {
		m_bEnableChangeNick = IsEnableChangeNickname();
	}
}

// optiondlg.cpp:250
bool FrOptionDlg::sGameTab::IsEnableChangeNickname(void) {
	if (IsLocalContent((localContentType_t)0x82)) {
		if (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CLobbyTask::m_RTTI) : false)
			&& (g_pFresh->IsCurrentLayout("GAMEROOM") || g_pFresh->IsCurrentLayout("GAMEROOM_EXT") || g_pFresh->IsCurrentLayout("GAMEROOM_EXTRES"))) {
			return false;
		}

		return true;
	}

	return false;
}

// optiondlg.cpp:278
FrOptionDlg::sVideoTab::sVideoTab(void) {
	m_numWnds = 13;
	m_ppWnds = (FrWnd**)m_pStatics;
	m_pppHoverWnds = m_pHoverWnds;
	m_pbDisabled = m_bDisabled;

	memset(m_pStatics, 0, sizeof(m_pStatics));
	memset(m_bDisabled, 0, sizeof(m_bDisabled));

	m_pFullScreenBtn = 0;
	m_pFillScreenBtn = 0;
	m_pWideModeBtn = 0;
	m_pOverallCmb = 0;
	m_pScrResCmb = 0;
	m_pTnlCmb = 0;
	m_pTexResCmb = 0;
	m_pShadowBtn = 0;
	m_pShadowmapBtn = 0;
	m_pReflectionBtn = 0;
	m_pLodBtn = 0;
	m_pNpcBtn = 0;
	m_pParticleCmb = 0;

	m_pHoverWnds[0] = (FrWnd**)&m_pFullScreenBtn;
	m_pHoverWnds[1] = (FrWnd**)&m_pFillScreenBtn;
	m_pHoverWnds[2] = (FrWnd**)&m_pWideModeBtn;
	m_pHoverWnds[3] = (FrWnd**)&m_pOverallCmb;
	m_pHoverWnds[4] = (FrWnd**)&m_pScrResCmb;
	m_pHoverWnds[5] = (FrWnd**)&m_pTnlCmb;
	m_pHoverWnds[6] = (FrWnd**)&m_pTexResCmb;
	m_pHoverWnds[7] = (FrWnd**)&m_pShadowBtn;
	m_pHoverWnds[8] = (FrWnd**)&m_pShadowmapBtn;
	m_pHoverWnds[9] = (FrWnd**)&m_pReflectionBtn;
	m_pHoverWnds[10] = (FrWnd**)&m_pLodBtn;
	m_pHoverWnds[11] = (FrWnd**)&m_pNpcBtn;
	m_pHoverWnds[12] = (FrWnd**)&m_pParticleCmb;

	// Options the video card cannot do are greyed out.
	if (!sOption::IsSupportShadow()) {
		m_bDisabled[7] = true;
	}

	if (!sOption::IsSupportReflection()) {
		m_bDisabled[9] = true;
	}

	if (!sOption::IsSupportLod()) {
		m_bDisabled[10] = true;
	}
}

// optiondlg.cpp:330
FrOptionDlg::sAudioTab::sAudioTab(void) {
	m_numWnds = 5;
	m_ppWnds = (FrWnd**)m_pStatics;
	m_pppHoverWnds = m_pHoverWnds;
	m_pbDisabled = m_bDisabled;

	memset(m_pStatics, 0, sizeof(m_pStatics));

	if (g_audio->GetMSS()) {
		memset(m_bDisabled, 0, sizeof(m_bDisabled));
	} else {
		memset(m_bDisabled, 1, sizeof(m_bDisabled));
	}

	m_pHoverWnds[0] = (FrWnd**)&m_pEnableBtn;
	m_pHoverWnds[1] = (FrWnd**)&m_pOverallCmb;
	m_pHoverWnds[2] = (FrWnd**)&m_pSfxGauge;
	m_pHoverWnds[3] = (FrWnd**)&m_pBgmGauge;
	m_pHoverWnds[4] = (FrWnd**)&m_pSpeakerCmb;

	for (int i = 0; i < 5; i++) {
		*m_pHoverWnds[i] = 0;
	}
}

// optiondlg.cpp:359
FrOptionDlg::sMacroTab::sMacroTab(void) {
	m_numWnds = 8;
	m_ppWnds = (FrWnd**)m_pStatics;
	m_pppHoverWnds = m_pHoverWnds;
	m_pbDisabled = m_bDisabled;

	memset(m_pStatics, 0, sizeof(m_pStatics));
	memset(m_bDisabled, 0, sizeof(m_bDisabled));

	for (int i = 0; i < 8; i++) {
		m_pMacroEdits[i] = 0;
		m_pHoverWnds[i] = (FrWnd**)&m_pMacroEdits[i];
	}
}

// optiondlg.cpp:381 (run-time type and factory registration of FrChangeNickDlg)
const WRTTI FrChangeNickDlg::m_RTTI("FrChangeNickDlg", &FrForm::m_RTTI);

IObject* FrChangeNickDlgMakeInstance(void) {
	return new FrChangeNickDlg;
}

struct __sFrChangeNickDlg {
	__sFrChangeNickDlg(void) {
		ObjectFactory().AddObjectFunctor(FrChangeNickDlgMakeInstance, "FrChangeNickDlg");
	}
};
static __sFrChangeNickDlg s_registerFrChangeNickDlg;

// optiondlg.cpp:383
const sFRESH_MSGMAP* FrChangeNickDlg::GetMessageMap(void) const {
	return &FrChangeNickDlg::_MsgMap;
}

// optiondlg.cpp:398
FrChangeNickDlg::FrChangeNickDlg(void) : m_szNick(0) {
	CSharedDoc* pDoc = CSharedDoc::Instance();
	std::list<sItemInfo>::iterator it;
	for (it = pDoc->m_itemInfoList.begin(); it != pDoc->m_itemInfoList.end(); ++it) {
		if ((*it).m_typeId == 0x1A000003) {
			break;
		}
	}

	m_bHasTicket = it != pDoc->m_itemInfoList.end();

	m_pOptionDlg = 0;
}

// optiondlg.cpp:414
void FrChangeNickDlg::OnPriceStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (!pStatic) {
		return;
	}

	if (m_bHasTicket || (CSharedDoc::Instance()->GetPangyaUserInfo().identity & 0x204)) {
		pStatic->SetVisible(false);
	}
}

// optiondlg.cpp:430
void FrChangeNickDlg::OnPriceInit(intptr_t param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (!pStatic) {
		return;
	}

	if (m_bHasTicket || (CSharedDoc::Instance()->GetPangyaUserInfo().identity & 0x204)) {
		pStatic->SetVisible(false);
	} else {
		// 49 cookies.
		if (IsLocalCountry(30)) {
			pStatic->SetCaption(MakeStr("%d\xB0\xB3(CK)", 49));
		} else {
			pStatic->SetCaption(MakeStr("%d\xB0\xB3", 49));
		}
	}
}

// optiondlg.cpp:454
void FrChangeNickDlg::OnMyCookieStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (!pStatic) {
		return;
	}

	if (m_bHasTicket || (CSharedDoc::Instance()->GetPangyaUserInfo().identity & 0x204)) {
		pStatic->SetVisible(false);
	}
}

// optiondlg.cpp:470
void FrChangeNickDlg::OnMyCookieInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (!pStatic) {
		return;
	}

	if (m_bHasTicket || (CSharedDoc::Instance()->GetPangyaUserInfo().identity & 0x204)) {
		pStatic->SetVisible(false);
	} else {
		if (IsLocalCountry(30)) {
			pStatic->SetCaption(MakeStr("%I64d\xB0\xB3(CK)", CSharedDoc::Instance()->m_myCash));
		} else {
			pStatic->SetCaption(MakeStr("%I64d\xB0\xB3", CSharedDoc::Instance()->m_myCash));
		}
	}
}

// optiondlg.cpp:495
void FrChangeNickDlg::OnRemainCookieStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (!pStatic) {
		return;
	}

	if (m_bHasTicket || (CSharedDoc::Instance()->GetPangyaUserInfo().identity & 0x204)) {
		pStatic->SetVisible(false);
	}
}

// optiondlg.cpp:511
void FrChangeNickDlg::OnRemainCookieInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (!pStatic) {
		return;
	}

	if (m_bHasTicket || (CSharedDoc::Instance()->GetPangyaUserInfo().identity & 0x204)) {
		pStatic->SetVisible(false);
	} else {
		if (CSharedDoc::Instance()->m_myCash < 49) {
			// "%I64d cookies short".
			pStatic->SetCaption(MakeStr("%I64d\xB0\xB3 \xBA\xCE\xC1\xB7", 49 - CSharedDoc::Instance()->m_myCash));
		} else {
			if (IsLocalCountry(30)) {
				pStatic->SetCaption(MakeStr("%I64d\xB0\xB3(CK)", CSharedDoc::Instance()->m_myCash - 49));
			} else {
				pStatic->SetCaption(MakeStr("%I64d\xB0\xB3", CSharedDoc::Instance()->m_myCash - 49));
			}
		}
	}
}

// optiondlg.cpp:545
void FrChangeNickDlg::OnCheckoutInit(intptr_t param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	if (!pBtn) {
		return;
	}

	CSharedDoc* pDoc = CSharedDoc::Instance();
	if (pDoc->m_myCash < 49 && !m_bHasTicket && !(pDoc->GetPangyaUserInfo().identity & 0x204)) {
		pBtn->Enable(false);
	}
}

// optiondlg.cpp:561
void FrChangeNickDlg::OnCheckoutBtnUp(void) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_okcancel", 0);
	// "Do you want to change your nickname?"
	pForm->SetMessage("\\c0xffff0000\\c\xB4\xEB\xC8\xAD\xB8\xED\xC0\xBB \xBA\xAF\xB0\xE6 \xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);

	pForm->Open(static_cast<FRESH_RESULT_CALLBACK>(&FrChangeNickDlg::OnChangeNickConfirmDlgResult), 1);
}

// optiondlg.cpp:745 (run-time type and factory registration of FrOptionDlg)
const WRTTI FrOptionDlg::m_RTTI("FrOptionDlg", &FrForm::m_RTTI);

FrOptionDlg::eTabType FrOptionDlg::ms_eCurTab;

IObject* FrOptionDlgMakeInstance(void) {
	return new FrOptionDlg;
}

struct __sFrOptionDlg {
	__sFrOptionDlg(void) {
		ObjectFactory().AddObjectFunctor(FrOptionDlgMakeInstance, "FrOptionDlg");
	}
};
static __sFrOptionDlg s_registerFrOptionDlg;

// optiondlg.cpp:747
const sFRESH_MSGMAP* FrOptionDlg::GetMessageMap(void) const {
	return &FrOptionDlg::_MsgMap;
}

// optiondlg.cpp:917
FrOptionDlg::FrOptionDlg(void) : m_bApplyScrSize(false) {
	m_option = *COption::Instance()->GetOption();
	m_bChanged1 = false;
	m_bChanged2 = false;
	m_pOkBtn = 0;
	m_pTabs[TAB_GAME] = &m_gameTab;
	m_pTabs[TAB_VIDEO] = &m_videoTab;
	m_pTabs[TAB_AUDIO] = &m_audioTab;
	m_pTabs[TAB_MACRO] = &m_macroTab;

	m_state1 = 0;
	m_pSchoolDlg = 0;

	m_pHintEdit = 0;
	m_hoverCount = 0;

	g_pFresh->GetManager()->SetExclusiveKey(true);
}

// optiondlg.cpp:938
FrOptionDlg::~FrOptionDlg(void) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		if (!CChatMsg::Instance()->IsOpened()) {
			g_pFresh->GetManager()->SetExclusiveKey(false);
		}
	}
}

// optiondlg.cpp:949
void FrOptionDlg::SetApplyScrSize(bool bApply) {
	m_bApplyScrSize = bApply;
}

// optiondlg.cpp:954
void FrOptionDlg::OpenTab(eTabType type) {
	if (type < TAB_GAME || type >= 4) {
		return;
	}

	SetTab(type);
}

// optiondlg.cpp:962
void FrOptionDlg::OnOkBtnInit(intptr_t param) {
	m_pOkBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:967
void FrOptionDlg::OnGameTabInit(int param) {
	m_pTabs[TAB_GAME]->m_pTabBtn = DYNAMIC_CAST(FrButton, param);
	m_pTabs[TAB_GAME]->Init();
}

// optiondlg.cpp:973
void FrOptionDlg::OnGameTabUp(void) {
	SetTab(TAB_GAME);
}

// optiondlg.cpp:978
void FrOptionDlg::OnVideoTabInit(int param) {
	m_pTabs[TAB_VIDEO]->m_pTabBtn = DYNAMIC_CAST(FrButton, param);
	m_pTabs[TAB_VIDEO]->Init();
}

// optiondlg.cpp:984
void FrOptionDlg::OnVideoTabUp(void) {
	SetTab(TAB_VIDEO);
}

// optiondlg.cpp:989
void FrOptionDlg::OnAudioTabInit(int param) {
	m_pTabs[TAB_AUDIO]->m_pTabBtn = DYNAMIC_CAST(FrButton, param);
	m_pTabs[TAB_AUDIO]->Init();
}

// optiondlg.cpp:995
void FrOptionDlg::OnAudioTabUp(void) {
	SetTab(TAB_AUDIO);
}

// optiondlg.cpp:1000
void FrOptionDlg::OnMacroTabInit(int param) {
	m_pTabs[TAB_MACRO]->m_pTabBtn = DYNAMIC_CAST(FrButton, param);
	m_pTabs[TAB_MACRO]->Init();
}

// optiondlg.cpp:1006
void FrOptionDlg::OnMacroTabUp(void) {
	SetTab(TAB_MACRO);
}

// optiondlg.cpp:1011
void FrOptionDlg::gOnMsSensiStaticInit(int param) {
	m_gameTab.m_pStatics[0] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1016
void FrOptionDlg::gOnMsSensiGaugeInit(int param) {
	m_gameTab.m_pMsSensiGauge = DYNAMIC_CAST(FrGaugeBarEx, param);
}

// optiondlg.cpp:1021
void FrOptionDlg::gOnWhisperStaticInit(int param) {
	m_gameTab.m_pStatics[1] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1026
void FrOptionDlg::gOnWhisperBtnInit(intptr_t param) {
	m_gameTab.m_pWhisperBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1031
void FrOptionDlg::gOnWhisperBtnUp(void) {
	m_option.m_whisperAllowed = m_gameTab.m_pWhisperBtn->GetStatus() == FrButton::PRESSED;
}

// optiondlg.cpp:1036
void FrOptionDlg::gOnInvitationStaticInit(int param) {
	m_gameTab.m_pStatics[2] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1041
void FrOptionDlg::gOnInvitationBtnInit(int param) {
	m_gameTab.m_pInvitationBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1046
void FrOptionDlg::gOnInvitationBtnUp(void) {
	m_option.m_invitationAllowed = m_gameTab.m_pInvitationBtn->GetStatus() == FrButton::PRESSED;
}

// optiondlg.cpp:1051
void FrOptionDlg::gOnFriendConfirmStaticInit(int param) {
	m_gameTab.m_pStatics[3] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1056
void FrOptionDlg::gOnFriendConfirmBtnInit(int param) {
	m_gameTab.m_pFriendConfirmBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1061
void FrOptionDlg::gOnFriendConfirmBtnUp(void) {
	m_option.m_friendConfirmNeeded = m_gameTab.m_pFriendConfirmBtn->GetStatus() == FrButton::PRESSED;
}

// optiondlg.cpp:1066
void FrOptionDlg::gOnTransChatWinStaticInit(int param) {
	m_gameTab.m_pStatics[4] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1071
void FrOptionDlg::gOnTransChatWinBtnInit(int param) {
	m_gameTab.m_pTransChatWinBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1076
void FrOptionDlg::gOnTransChatWinBtnUp(void) {
	m_option.m_chatWinTransparent = m_gameTab.m_pTransChatWinBtn->GetStatus() == FrButton::PRESSED;
}

// optiondlg.cpp:1081
void FrOptionDlg::gOnUiEffectStaticInit(int param) {
	m_gameTab.m_pStatics[5] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1086
void FrOptionDlg::gOnTerrainTooltipStaticInit(int param) {
	m_gameTab.m_pStatics[11] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1091
void FrOptionDlg::gOnUiEffectBtnInit(int param) {
	m_gameTab.m_pUiEffectBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1096
void FrOptionDlg::gOnUiEffectBtnUp(void) {
	m_option.m_uiEffect = m_gameTab.m_pUiEffectBtn->GetStatus() == FrButton::PRESSED;
}

// optiondlg.cpp:1101
void FrOptionDlg::gOnNickStaticInit(intptr_t param) {
	m_gameTab.m_pStatics[6] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1106
void FrOptionDlg::gOnNickEditInit(int param) {
	m_gameTab.m_pNickEdit = DYNAMIC_CAST(FrEdit, param);
	m_gameTab.m_pNickEdit->HidePrivacy(true);

	if (!m_gameTab.m_bEnableChangeNick) {
		m_gameTab.m_pNickEdit->SetReadOnly(true);
		m_gameTab.m_pNickEdit->SetFontColor(0xff888888);
	}
}

// optiondlg.cpp:1119
void FrOptionDlg::gOnChangeNickBtnInit(int param) {
	m_gameTab.m_pChangeNickBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1237
bool FrOptionDlg::gOnNickEditEnterKey(int key) {
	return false;
}

// optiondlg.cpp:1242
void FrOptionDlg::gOnSchoolStaticInit(int param) {
	m_gameTab.m_pStatics[7] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1247
void FrOptionDlg::gOnSchoolEditInit(int param) {
	m_gameTab.m_pSchoolEdit = DYNAMIC_CAST(FrEdit, param);
	m_gameTab.m_pSchoolEdit->SetReadOnly(true);
}

// optiondlg.cpp:1253
void FrOptionDlg::gOnChangeSchoolBtnInit(int param) {
	m_gameTab.m_pChangeSchoolBtn = DYNAMIC_CAST(FrButton, param);

	// The school can only be changed from the lobby, online.
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CLobbyTask::m_RTTI)
		&& WSingleton<WNetworkSystem>::Instance()->IsConnected((WNetworkSystem::eNetUnit)0)) {
		m_gameTab.m_pChangeSchoolBtn->Enable(true);
	} else {
		m_gameTab.m_pChangeSchoolBtn->Enable(false);
	}
}

// optiondlg.cpp:1267
void FrOptionDlg::gOnChangeSchoolBtnUp(void) {
	if (m_gameTab.m_pSchoolEdit && !m_pSchoolDlg) {
		m_pSchoolDlg = CreateForm<FrSchoolDlg>(g_pFresh->GetManager(), this, "search_school", 0);
		m_pSchoolDlg->Open(static_cast<FRESH_RESULT_CALLBACK>(&FrOptionDlg::OnChangeSchoolDlgResult), 1);
	}
}

// optiondlg.cpp:1276
bool FrOptionDlg::gOnSchoolEditEnterKey(int key) {
	return false;
}

// optiondlg.cpp:1282
void FrOptionDlg::gOnPowerGaugeStaticInit(int param) {
	m_gameTab.m_pStatics[8] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1288
void FrOptionDlg::gOnPowerGaugeBtnInit(int param) {
	m_gameTab.m_pPowerGaugeBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1294
void FrOptionDlg::gOnPowerGaugeBtnUp(void) {
	m_option.m_powerGauge = m_gameTab.m_pPowerGaugeBtn->GetStatus() == FrButton::PRESSED;
}

// optiondlg.cpp:1299
void FrOptionDlg::gOnHideGUIStaticInit(int param) {
	m_gameTab.m_pStatics[9] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1304
void FrOptionDlg::gOnHideGUIBtnInit(int param) {
	m_gameTab.m_pHideGUIBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1309
void FrOptionDlg::gOnHideGUIBtnUp(void) {
	m_option.m_captureHideGUI = m_gameTab.m_pHideGUIBtn->GetStatus() == FrButton::PRESSED;
	CProjectG::Instance()->ResetCaptuerNum();
}

// optiondlg.cpp:1315
void FrOptionDlg::gOnHidePIStaticInit(int param) {
	m_gameTab.m_pStatics[10] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1320
void FrOptionDlg::gOnHidePIBtnInit(int param) {
	m_gameTab.m_pHidePIBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1325
void FrOptionDlg::gOnTerrainBtnInit(int param) {
	m_gameTab.m_pTerrainBtn = DYNAMIC_CAST(FrButton, param);

	if (COption::Instance()->gGetTerrainTooltip()) {
		m_gameTab.m_pTerrainBtn->SetStatus(FrButton::PRESSED);
	}
}

// optiondlg.cpp:1335
void FrOptionDlg::gOnTerrainBtnUp(void) {
}

// optiondlg.cpp:1341
void FrOptionDlg::gOnCutinDisplayStaticInit(int param) {
	m_gameTab.m_pStatics[12] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1347
void FrOptionDlg::gOnCutinDisplayBtnInit(int param) {
	m_gameTab.m_pCutinDisplayBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1353
void FrOptionDlg::gOnCutinDisplayBtnUp(void) {
	m_option.m_cutinFlag = m_gameTab.m_pCutinDisplayBtn->GetStatus() == FrButton::PRESSED;
}

// optiondlg.cpp:1359
void FrOptionDlg::gOnHidePIBtnUp(void) {
	m_option.m_captureHidePI = m_gameTab.m_pHidePIBtn->GetStatus() == FrButton::PRESSED;
	CProjectG::Instance()->ResetCaptuerNum();
}

// optiondlg.cpp:1365
void FrOptionDlg::vOnFullScreenStaticInit(int param) {
	m_videoTab.m_pStatics[0] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1370
void FrOptionDlg::vOnFullScreenBtnInit(int param) {
	m_videoTab.m_pFullScreenBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1409
void FrOptionDlg::vOnFillScreenStaticInit(int param) {
	m_videoTab.m_pStatics[1] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1414
void FrOptionDlg::vOnFillScreenBtnInit(int param) {
	m_videoTab.m_pFillScreenBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1419
void FrOptionDlg::vOnFillScreenBtnUp(void) {
	if (m_videoTab.m_pFillScreenBtn && !m_videoTab.m_pFillScreenBtn->IsEnabled())
		return;

	m_option.m_fillScreen = m_videoTab.m_pFillScreenBtn->GetStatus() == FrButton::PRESSED;
}

// optiondlg.cpp:1428
void FrOptionDlg::vOnWideModeStaticInit(int param) {
	m_videoTab.m_pStatics[2] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1433
void FrOptionDlg::vOnWideModeBtnInit(int param) {
	m_videoTab.m_pWideModeBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1472
void FrOptionDlg::vOnOverallStaticInit(int param) {
	m_videoTab.m_pStatics[3] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1477
void FrOptionDlg::vOnOverallCmbInit(int param) {
	m_videoTab.m_pOverallCmb = DYNAMIC_CAST(FrComboBox, param);
}

// optiondlg.cpp:1482
void FrOptionDlg::vOnScrResStaticInit(int param) {
	m_videoTab.m_pStatics[4] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1487
void FrOptionDlg::vOnScrResCmbInit(int param) {
	m_videoTab.m_pScrResCmb = DYNAMIC_CAST(FrComboBox, param);
}

// optiondlg.cpp:1498
void FrOptionDlg::vOnTnlStaticInit(int param) {
	m_videoTab.m_pStatics[5] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1503
void FrOptionDlg::vOnTnlCmbInit(int param) {
	m_videoTab.m_pTnlCmb = DYNAMIC_CAST(FrComboBox, param);
}

// optiondlg.cpp:1509
void FrOptionDlg::vOnTexResStaticInit(int param) {
	m_videoTab.m_pStatics[6] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1514
void FrOptionDlg::vOnTexResCmbInit(int param) {
	m_videoTab.m_pTexResCmb = DYNAMIC_CAST(FrComboBox, param);
}

// optiondlg.cpp:1519
void FrOptionDlg::vOnShadowStaticInit(int param) {
	m_videoTab.m_pStatics[7] = DYNAMIC_CAST(FrStatic, param);

	if (!sOption::IsSupportShadow()) {
		m_videoTab.m_pStatics[7]->SetTextColor(0xff808080, 0xffffffff);
	}
}

// optiondlg.cpp:1527
void FrOptionDlg::vOnShadowBtnInit(int param) {
	m_videoTab.m_pShadowBtn = DYNAMIC_CAST(FrButton, param);

	if (!sOption::IsSupportShadow()) {
		m_videoTab.m_pShadowBtn->Enable(false);
	}
}

// optiondlg.cpp:1535
void FrOptionDlg::vOnShadowBtnUp(void) {
	m_option.m_shadowEnabled = m_videoTab.m_pShadowBtn->GetStatus() == FrButton::PRESSED;
	m_option.m_overallLevel = -1;
	m_videoTab.m_pOverallCmb->SetLine(1, m_option.vGetOverallText(m_option.m_overallLevel), 0, false, 0);
}

// optiondlg.cpp:1542
void FrOptionDlg::vOnShadowmapStaticInit(int param) {
	m_videoTab.m_pStatics[8] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1547
void FrOptionDlg::vOnShadowmapBtnInit(int param) {
	m_videoTab.m_pShadowmapBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1552
void FrOptionDlg::vOnShadowmapBtnUp(void) {
	m_option.m_shadowmapEnabled = m_videoTab.m_pShadowmapBtn->GetStatus() == FrButton::PRESSED;
	m_option.m_overallLevel = -1;
	m_videoTab.m_pOverallCmb->SetLine(1, m_option.vGetOverallText(m_option.m_overallLevel), 0, false, 0);
}

// optiondlg.cpp:1559
void FrOptionDlg::vOnReflectionStaticInit(int param) {
	m_videoTab.m_pStatics[9] = DYNAMIC_CAST(FrStatic, param);

	if (!sOption::IsSupportReflection()) {
		m_videoTab.m_pStatics[9]->SetTextColor(0xff808080, 0xffffffff);
	}
}

// optiondlg.cpp:1567
void FrOptionDlg::vOnReflectionBtnInit(int param) {
	m_videoTab.m_pReflectionBtn = DYNAMIC_CAST(FrButton, param);

	if (!sOption::IsSupportReflection()) {
		m_videoTab.m_pReflectionBtn->Enable(false);
	}
}

// optiondlg.cpp:1575
void FrOptionDlg::vOnReflectionBtnUp(void) {
	m_option.m_reflectionEnabled = m_videoTab.m_pReflectionBtn->GetStatus() == FrButton::PRESSED;
	m_option.m_overallLevel = -1;
	m_videoTab.m_pOverallCmb->SetLine(1, m_option.vGetOverallText(m_option.m_overallLevel), 0, false, 0);
}

// optiondlg.cpp:1588
void FrOptionDlg::vOnLodStaticInit(int param) {
	m_videoTab.m_pStatics[10] = DYNAMIC_CAST(FrStatic, param);

	if (!sOption::IsSupportLod()) {
		m_videoTab.m_pStatics[10]->SetTextColor(0xff808080, 0xffffffff);
	}
}

// optiondlg.cpp:1596
void FrOptionDlg::vOnLodBtnInit(int param) {
	m_videoTab.m_pLodBtn = DYNAMIC_CAST(FrButton, param);

	if (!sOption::IsSupportLod()) {
		m_videoTab.m_pLodBtn->Enable(false);
	}
}

// optiondlg.cpp:1604
void FrOptionDlg::vOnLodBtnUp(void) {
	m_option.m_lodEnabled = m_videoTab.m_pLodBtn->GetStatus() == FrButton::PRESSED;
	m_option.m_overallLevel = -1;
	m_videoTab.m_pOverallCmb->SetLine(1, m_option.vGetOverallText(m_option.m_overallLevel), 0, false, 0);
}

// optiondlg.cpp:1612
void FrOptionDlg::vOnNpcStaticInit(int param) {
	m_videoTab.m_pStatics[11] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1617
void FrOptionDlg::vOnNpcBtnInit(int param) {
	m_videoTab.m_pNpcBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1622
void FrOptionDlg::vOnNpcBtnUp(void) {
	m_option.m_npcEnabled = m_videoTab.m_pNpcBtn->GetStatus() == FrButton::PRESSED;
	m_option.m_overallLevel = -1;
	m_videoTab.m_pOverallCmb->SetLine(1, m_option.vGetOverallText(m_option.m_overallLevel), 0, false, 0);
}

// optiondlg.cpp:1629
void FrOptionDlg::vOnParticleStaticInit(int param) {
	m_videoTab.m_pStatics[12] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1634
void FrOptionDlg::vOnParticleCmbInit(int param) {
	m_videoTab.m_pParticleCmb = DYNAMIC_CAST(FrComboBox, param);
}

// optiondlg.cpp:1639
void FrOptionDlg::aOnEnableStaticInit(int param) {
	m_audioTab.m_pStatics[0] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1644
void FrOptionDlg::aOnEnableBtnInit(int param) {
	m_audioTab.m_pEnableBtn = DYNAMIC_CAST(FrButton, param);
}

// optiondlg.cpp:1649
void FrOptionDlg::aOnEnableBtnUp(void) {
	if (m_audioTab.m_pEnableBtn->GetStatus() == FrButton::PRESSED) {
		m_option.m_mssEnabled = 1;
		g_audio->SetVolume(m_option.m_sfxVolume);
		g_audio->SetBGMVolume(m_option.m_bgmVolume);
	} else {
		m_option.m_mssEnabled = 0;
		g_audio->SetVolume(0);
		g_audio->SetBGMVolume(0);
	}
}

// optiondlg.cpp:1665
void FrOptionDlg::aOnOverallStaticInit(int param) {
	m_audioTab.m_pStatics[1] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1670
void FrOptionDlg::aOnOverallCmbInit(int param) {
	m_audioTab.m_pOverallCmb = DYNAMIC_CAST(FrComboBox, param);
}

// optiondlg.cpp:1675
void FrOptionDlg::aOnSfxStaticInit(int param) {
	m_audioTab.m_pStatics[2] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1680
void FrOptionDlg::aOnSfxGaugeInit(int param) {
	m_audioTab.m_pSfxGauge = DYNAMIC_CAST(FrGaugeBarEx, param);
}

// optiondlg.cpp:1685
void FrOptionDlg::aOnBgmStaticInit(int param) {
	m_audioTab.m_pStatics[3] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1690
void FrOptionDlg::aOnBgmGaugeInit(int param) {
	m_audioTab.m_pBgmGauge = DYNAMIC_CAST(FrGaugeBarEx, param);
}

// optiondlg.cpp:1695
void FrOptionDlg::aOnSpeakerStaticInit(int param) {
	m_audioTab.m_pStatics[4] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1700
void FrOptionDlg::aOnSpeakerCmbInit(int param) {
	m_audioTab.m_pSpeakerCmb = DYNAMIC_CAST(FrComboBox, param);
}

// optiondlg.cpp:1705
void FrOptionDlg::mOnMacroStatic1Init(int param) {
	m_macroTab.m_pStatics[0] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1710
void FrOptionDlg::mOnMacroStatic2Init(int param) {
	m_macroTab.m_pStatics[1] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1715
bool FrOptionDlg::mOnMacroEnterKey(int key) {
	OnOK();
	Close(FrOK, true);
	return false;
}
// optiondlg.cpp:1722
void FrOptionDlg::mOnMacroEnterKeyLBtnDown(void) {
	OnLButtonDown(g_pFresh->GetManager()->GetMousePos());
}

// optiondlg.cpp:1727
void FrOptionDlg::mOnMacroStatic3Init(int param) {
	m_macroTab.m_pStatics[2] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1732
void FrOptionDlg::mOnMacroStatic4Init(int param) {
	m_macroTab.m_pStatics[3] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1737
void FrOptionDlg::mOnMacroStatic5Init(int param) {
	m_macroTab.m_pStatics[4] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1742
void FrOptionDlg::mOnMacroStatic6Init(int param) {
	m_macroTab.m_pStatics[5] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1747
void FrOptionDlg::mOnMacroStatic7Init(int param) {
	m_macroTab.m_pStatics[6] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1752
void FrOptionDlg::mOnMacroStatic8Init(int param) {
	m_macroTab.m_pStatics[7] = DYNAMIC_CAST(FrStatic, param);
}

// optiondlg.cpp:1757
void FrOptionDlg::mOnMacro1Init(int param) {
	m_macroTab.m_pMacroEdits[0] = DYNAMIC_CAST(FrEdit, param);
}

// optiondlg.cpp:1762
void FrOptionDlg::mOnMacro2Init(int param) {
	m_macroTab.m_pMacroEdits[1] = DYNAMIC_CAST(FrEdit, param);
}

// optiondlg.cpp:1767
void FrOptionDlg::mOnMacro3Init(int param) {
	m_macroTab.m_pMacroEdits[2] = DYNAMIC_CAST(FrEdit, param);
}

// optiondlg.cpp:1772
void FrOptionDlg::mOnMacro4Init(int param) {
	m_macroTab.m_pMacroEdits[3] = DYNAMIC_CAST(FrEdit, param);
}

// optiondlg.cpp:1777
void FrOptionDlg::mOnMacro5Init(int param) {
	m_macroTab.m_pMacroEdits[4] = DYNAMIC_CAST(FrEdit, param);
}

// optiondlg.cpp:1782
void FrOptionDlg::mOnMacro6Init(int param) {
	m_macroTab.m_pMacroEdits[5] = DYNAMIC_CAST(FrEdit, param);
}

// optiondlg.cpp:1787
void FrOptionDlg::mOnMacro7Init(int param) {
	m_macroTab.m_pMacroEdits[6] = DYNAMIC_CAST(FrEdit, param);
}

// optiondlg.cpp:1792
void FrOptionDlg::mOnMacro8Init(int param) {
	m_macroTab.m_pMacroEdits[7] = DYNAMIC_CAST(FrEdit, param);
}

// optiondlg.cpp:1797
bool FrOptionDlg::OnInit(void) {
	SetGameColtrols();
	InitVideoControls();
	SetVideoColtrols(true);
	SetAudioColtrols();
	SetMacroColtrols();

	eTabType curTab = ms_eCurTab;

	ms_eCurTab = (eTabType)4;
	SetTab(curTab);
	m_pTabs[ms_eCurTab]->m_pTabBtn->SetStatus(FrButton::PRESSED);

	return FrForm::OnInit();
}

// optiondlg.cpp:1824
bool FrOptionDlg::OnLButtonDown(const WPoint& pt) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		if (g_input->GetButton(LEFT_BUTTON) == 1) {
			g_pFresh->GetManager()->SetExclusiveKey(true);
			CMessengerInfo::Instance()->SetGameFocus(false);
		}
	}

	return FrForm::OnLButtonDown(pt);
}

// optiondlg.cpp:1838
void FrOptionDlg::OnProc(const float deltaTime) {
	if (g_input->GetDown("TAB", true) && !g_input->Get("LALT", true) && !g_input->Get("RALT", true)) {
		switch (ms_eCurTab) {
		case TAB_GAME:
			SetTab(TAB_VIDEO);
			break;
		case TAB_VIDEO:
			SetTab(TAB_AUDIO);
			break;
		case TAB_AUDIO:
			SetTab(TAB_MACRO);
			break;
		case TAB_MACRO:
			SetTab(TAB_GAME);
			break;
		}
	}

	if (m_gameTab.m_pTabBtn && m_gameTab.m_pTabBtn->IsEnabled() && m_gameTab.m_pMsSensiGauge) {
		float pos = m_gameTab.m_pMsSensiGauge->GetPos();
		float sens;

		if (pos <= 50.0f) {
			sens = pos * 0.02f;
		} else {
			sens = pos * 0.01f + 0.5f;
		}

		if (fabs(m_option.m_mouseSensitivity - sens) > g_EPSILON) {
			m_option.m_mouseSensitivity = sens;
			g_input->SetSensitivity(sens);
		}
	}

	if (m_videoTab.m_pTabBtn && m_videoTab.m_pTabBtn->IsEnabled()) {
		bool bOverall = false;
		bool bRes = false;
		bool bAny = false;
		bool bParticle = false;

		if (m_videoTab.m_pOverallCmb) {
			const char* text = m_option.vGetOverallText(m_option.m_overallLevel);
			const char* line = m_videoTab.m_pOverallCmb->GetLine(1, false);

			if (strcmp(text, line) != 0) {
				m_option.vResetByOverall(line);
				bOverall = true;
			}
		}

		if (!bOverall && m_videoTab.m_pScrResCmb) {
			const char* text = COption::Instance()->vGetScrResText(m_option.m_screenWidth, m_option.m_screenHeight, m_option.m_screenBpp, m_option.m_windowed != 0);
			const char* line = m_videoTab.m_pScrResCmb->GetLine(1, false);

			if (strcmp(text, line) != 0) {
				COption::Instance()->vGetScrResLvl(line, &m_option.m_screenWidth, &m_option.m_screenHeight, &m_option.m_screenBpp);
				bRes = true;
				bAny = true;
			}
		}

		if (m_videoTab.m_pFillScreenBtn) {
			if (m_option.m_windowed == 0) {
				m_videoTab.m_pFillScreenBtn->Enable(false);
			} else {
				m_videoTab.m_pFillScreenBtn->Enable(true);

				if (m_option.m_fillScreen == 0) {
					m_gameTab.m_pPowerGaugeBtn->Enable(false);
				} else {
					m_gameTab.m_pPowerGaugeBtn->Enable(true);
				}
			}
		}

		if (bOverall) {
			SetVideoColtrols(false);
		} else {
			if (m_videoTab.m_pTexResCmb) {
				const char* text = m_option.vGetTexResText(m_option.m_mipmapLevel);
				const char* line = m_videoTab.m_pTexResCmb->GetLine(1, false);

				if (strcmp(text, line) != 0) {
					m_option.m_mipmapLevel = m_option.vGetTexResLvl(line);
					bRes = true;
					bAny = true;
				}
			}

			if (m_videoTab.m_pParticleCmb) {
				const char* text = m_option.vGetParticleText(m_option.m_particleLevel);
				const char* line = m_videoTab.m_pParticleCmb->GetLine(1, false);

				if (strcmp(text, line) != 0) {
					m_option.m_particleLevel = m_option.vGetParticleLvl(line);
					bAny = true;
					bParticle = true;
				}
			}

			if (bRes || bParticle) {
				SetVideoColtrols(false);

				if (bAny) {
					m_option.m_overallLevel = -1;
					m_videoTab.m_pOverallCmb->SetLine(1, m_option.vGetOverallText(-1), 0, false, 0);
				}
			}
		}
	}

	if (m_audioTab.m_pTabBtn && m_audioTab.m_pTabBtn->IsEnabled()) {
		if (m_audioTab.m_pSfxGauge) {
			float vol = m_audioTab.m_pSfxGauge->GetPos() * 0.01f;

			if (fabs(m_option.m_sfxVolume - vol) > g_EPSILON) {
				m_option.m_sfxVolume = vol;

				if (m_option.m_mssEnabled == 1) {
					COption::Instance()->aApplySfxVolume(vol);
				}
			}
		}

		if (m_audioTab.m_pBgmGauge) {
			float vol = m_audioTab.m_pBgmGauge->GetPos() * 0.01f;

			if (fabs(m_option.m_bgmVolume - vol) > g_EPSILON) {
				m_option.m_bgmVolume = vol;

				if (m_option.m_mssEnabled == 1) {
					COption::Instance()->aApplyBgmVolume(vol);
				}
			}
		}

		if (m_audioTab.m_pSpeakerCmb) {
			const char* text = m_option.aGetSpeakerText(m_option.m_speakerType);
			const char* line = m_audioTab.m_pSpeakerCmb->GetLine(1, false);

			if (strcmp(text, line) != 0) {
				m_option.m_speakerType = m_option.aGetSpeakerLvl(line);
			}
		}
	}
}

// optiondlg.cpp:2090
void FrOptionDlg::InitVideoControls(void) {
	int i;

	for (i = 0; i < 3; i++) {
		const char* text = m_option.vGetOverallText(i);
		m_videoTab.m_pOverallCmb->AddString(text);
	}

	COption::Instance()->CheckDisplayModeList(g_resrcmng->VideoReference(), m_option.m_windowed != 0, m_option.m_wideMode != 0);
	m_videoTab.m_pScrResCmb->ClearList();

	for (i = 0; i < COption::Instance()->vGetScrResNum(); i++) {
		const char* text = COption::Instance()->vGetScrResText(i);

		if (strlen(text)) {
			m_videoTab.m_pScrResCmb->AddString(text);
		}
	}

	if (sOption::sbHwTnlSupport) {
		const char* text = m_option.vGetTnlText(1);
		m_videoTab.m_pTnlCmb->AddString(text);
	}

	for (i = 0; i < 3; i++) {
		const char* text = m_option.vGetTexResText(i);
		m_videoTab.m_pTexResCmb->AddString(text);
	}

	for (i = 0; i < 3; i++) {
		const char* text = m_option.vGetParticleText(i);
		m_videoTab.m_pParticleCmb->AddString(text);
	}
}

// optiondlg.cpp:2213
void FrOptionDlg::SetAudioColtrols(void) {
	if (m_option.m_mssEnabled) {
		m_audioTab.m_pEnableBtn->SetStatus(FrButton::PRESSED);
	}

	for (int i = 0; i < 3; i++) {
		m_audioTab.m_pOverallCmb->AddString(m_option.aGetOverallText(i));
	}

	for (int i = 0; i < 6; i++) {
		m_audioTab.m_pSpeakerCmb->AddString(m_option.aGetSpeakerText(i));
	}

	m_audioTab.m_pOverallCmb->SetLine(1, m_option.aGetOverallText(m_option.m_audioOverall), 0, false, 0);

	m_audioTab.m_pSpeakerCmb->SetLine(1, m_option.aGetSpeakerText(m_option.m_speakerType), 0, false, 0);

	m_audioTab.m_pSfxGauge->SetPos(Between(0, (int)(m_option.m_sfxVolume * 100.0f), 100));
	m_audioTab.m_pBgmGauge->SetPos(Between(0, (int)(m_option.m_bgmVolume * 100.0f), 100));
}

// optiondlg.cpp:2243
void FrOptionDlg::SetMacroColtrols(void) {
	for (int i = 0; i < 8; i++) {
		CSharedDoc::Instance()->m_chatManager.FilteringHack(m_option.m_macroText[i], false);
		m_macroTab.m_pMacroEdits[i]->SetLine(1, m_option.m_macroText[i], 0, false, 0);
	}
}

// optiondlg.cpp:2254
void FrOptionDlg::SetTab(eTabType type) {
	if (m_hoverCount) {
		m_hoverCount = 0;
	}

	if (type < TAB_GAME || type >= 4 || type == ms_eCurTab) {
		return;
	}

	ms_eCurTab = type;
	g_audio->PlaySfx("ui_button_ok_click", 0, 0, 0, 0, 0.5f, 200.0f);

	for (int i = 0; i < 4; i++) {
		m_pTabs[i]->SetSelected(i == type);
		m_pTabs[i]->m_pTabBtn->SetStatus(i == type ? FrButton::PRESSED : FrButton::NORMAL);

		if (!g_audio->GetMSS()) {
			if (type == TAB_AUDIO) {
				m_pHintEdit->ClearLine();
				// "No audio device is available."
				m_pHintEdit->AddText("\xbb\xe7\xbf\xeb\xc7\xd2 \xbc\xf6 \xc0\xd6\xb4\xc2 \xbf\xc0\xb5\xf0\xbf\xc0 \xc0\xe5\xc4\xa1\xb0\xa1 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", false, true);
				m_pHintEdit->SetVisible(true);
			} else {
				m_pHintEdit->SetVisible(false);
			}
		}
	}
}

// optiondlg.cpp:2302
void FrOptionDlg::OnCancel(void) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI) && !CChatMsg::Instance()->IsOpened()) {
		g_pFresh->GetManager()->SetExclusiveKey(true);

		CMessengerInfo::Instance()->SetGameFocus(true);
	}

	g_input->SetSensitivity(Between(0.5f, COption::Instance()->gGetMouseSensitivity(), 2.0f));
	if (m_audioTab.m_pEnableBtn->GetStatus() == FrButton::PRESSED) {
		g_audio->SetBGMVolume(COption::Instance()->aGetBgmVolume());
		g_audio->SetVolume(COption::Instance()->aGetSfxVolume());
	} else {
		g_audio->SetVolume(0);
		g_audio->SetBGMVolume(0);
	}

	FrForm::OnCancel();
}

// optiondlg.cpp:2326
void FrOptionDlg::OnOK(void) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI) && !CChatMsg::Instance()->IsOpened()) {
		g_pFresh->GetManager()->SetExclusiveKey(true);

		CMessengerInfo::Instance()->SetGameFocus(true);
	}

	m_bChanged1 = true;
	m_bChanged2 = true;

	CloseDlg();
}

// optiondlg.cpp:2398
void FrOptionDlg::EnableChangeNickControls(bool bEnable) {
	if (m_gameTab.m_pChangeNickBtn && m_gameTab.m_pChangeNickBtn->IsVisible()) {
		m_gameTab.m_pChangeNickBtn->Enable(bEnable);
	}

	if (m_pOkBtn && m_pOkBtn->IsVisible()) {
		m_pOkBtn->Enable(bEnable);
	}
}

// optiondlg.cpp:2413
void FrOptionDlg::tOnHintInit(int param) {
	m_pHintEdit = DYNAMIC_CAST(FrEdit, param);
	m_pHintEdit->SetVisible(false);
}

// optiondlg.cpp:2532
void FrOptionDlg::ProcessHoverMessage(FrWnd* pWnd) {
	if (!m_pHintEdit || !m_pTabs[ms_eCurTab]->m_bSelected) {
		return;
	}

	if (m_hoverCount > 0) {
		if (pWnd) {
			for (int i = 0; i < m_pTabs[ms_eCurTab]->m_numWnds; i++) {
				if (*m_pTabs[ms_eCurTab]->m_pppHoverWnds[i] == pWnd) {
					const char* hint = MakeHint(ms_eCurTab, i);
					if (hint) {
						m_pHintEdit->ClearLine();
						m_pHintEdit->AddText(hint, false, true);
						m_pHintEdit->SetVisible(true);
					}
				}
			}
		}
	} else if (m_hoverCount == 0) {
		m_pHintEdit->SetVisible(false);
	}
}

// optiondlg.cpp:2567
void FrOptionDlg::OnHoverOn(int param) {
	m_hoverCount++;
	ProcessHoverMessage(DYNAMIC_CAST(FrWnd, param));
}

// optiondlg.cpp:2573
void FrOptionDlg::OnHoverOff(int param) {
	m_hoverCount--;
	ProcessHoverMessage(0);
}

// optiondlg.cpp:2579
bool FrOptionDlg::OnChangeNickConfirmDlgResult(int result, FrForm* pForm) {
	m_state1 = 0;
	EnableChangeNickControls(true);

	return true;
}
