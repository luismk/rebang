// Original: d:\build\custom\temp\code\client\projectg\bongdarishopdlg.cpp
// This object emitted the out-of-line copies of CSharedDoc::GetBsUsableTimes,
// GetBsUsableBonusTimes and GetBsRemainedBonusTimes (shareddoc.h:550-554).
#define BONGDARISHOPDLG_SHAREDDOC_INLINE

#include "BongdariShopDlg.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "SetItemDlg.h"
#include "SetItemDlg.h"
#include "TaskManager.h"
#include "Packet.h"
#include "../shared/Localize.h"
#include "bitmap.h"
#include "UccManager.h"
#include "commonutil.h"
#include "exhibition.h"
#include "frdesktop.h"
#include "fresh.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frlistbox.h"
#include "frscrollbar.h"
#include "frtextbutton.h"
#include "frviewer.h"
#include "frgraphicinterface.h"
#include "InputManager.h"
#include "binstr.h"
#include "wview.h"
#include "frwndmanager.h"
#include "wfont.h"
#include "wresrcmng.h"
#include "SimpleOverlay.h"
#include "SoundManager.h"

#include <stdlib.h>
#include <string.h>

// Nameless data block of this unit's references; the file name list of the
// warning dialog's digits font is at 0xa98ab0 (+0xe38, entry 910).
extern "C" char* data_a97c78[];
extern char* g_strIndex[];
extern WPoint g_arrPtItemSilhouette[];

// Random item tables of the Bongdari machine (100 type ids each), shuffled by
// ShuffleRandomDataItemSilhouette.
unsigned long g_arrdwRandNumCookieItem0[100];
unsigned long g_arrdwRandNumCookieItem1[100];
unsigned long g_arrdwRandNumCookieItem2[100];
unsigned long g_arrdwRandSelectCookieItem[100];
unsigned long g_arrdwRandSelectPangItem[100];

// bongdarishopdlg.h: no line record (empty constructor of a nested helper class)
FrTotalItemDlg::MiniBtn::MiniBtn(void) {
}

// bongdarishopdlg.h: no line record (empty constructor of a nested helper class)
FrClosedBonusBallResultDlg::MiniBtn::MiniBtn(void) {
}

// bongdarishopdlg.h: no line record (empty constructor of a nested helper struct)
FrBongdariShopDlg::sMovingData::sMovingData(void) {
}

// bongdarishopdlg.cpp:76 (run-time type and factory registration of
// FrBongdariShopDlg)
const WRTTI FrBongdariShopDlg::m_RTTI("FrBongdariShopDlg", &FrForm::m_RTTI);

IObject* FrBongdariShopDlgMakeInstance(void) {
	return new FrBongdariShopDlg;
}

struct __sFrBongdariShopDlg {
	__sFrBongdariShopDlg(void) {
		ObjectFactory().AddObjectFunctor(FrBongdariShopDlgMakeInstance, "FrBongdariShopDlg");
	}
};
static __sFrBongdariShopDlg s_registerFrBongdariShopDlg;

// bongdarishopdlg.cpp:78
const sFRESH_MSGMAP* FrBongdariShopDlg::GetMessageMap(void) const {
	return &FrBongdariShopDlg::_MsgMap;
}

sFRESH_MSGMAP FrBongdariShopDlg::_MsgMap = { &FrForm::_MsgMap, &FrBongdariShopDlg::_MsgEntries[0] };

sFRESH_ENTRY FrBongdariShopDlg::_MsgEntries[] = {
	// bongdarishopdlg.cpp:80
	{ "bongdari_shop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdariShopDlg::OnBongdariShopInit },
	// bongdarishopdlg.cpp:81
	{ "aboutshop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdariShopDlg::OnAboutShopBnInit },
	// bongdarishopdlg.cpp:82
	{ "aboutshop", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdariShopDlg::OnAboutShopBnUp },
	// bongdarishopdlg.cpp:83
	{ "totalitem", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdariShopDlg::OnTotalItemBnInit },
	// bongdarishopdlg.cpp:84
	{ "totalitem", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdariShopDlg::OnTotalItemBnUp },
	// bongdarishopdlg.cpp:85
	{ "bongdarishopnotice", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdariShopDlg::OnBongdarishopNoticeBnInit },
	// bongdarishopdlg.cpp:86
	{ "bongdarishopnotice", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdariShopDlg::OnBongdarishopNoticeBnUp },
	// bongdarishopdlg.cpp:87
	{ "close", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdariShopDlg::OnCloseBnInit },
	// bongdarishopdlg.cpp:88
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdariShopDlg::OnCloseBnUp },
	// bongdarishopdlg.cpp:89
	{ "bongdari", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdariShopDlg::OnBongdariInit },
	// bongdarishopdlg.cpp:90
	{ "bongdari", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrBongdariShopDlg::OnBongdariOwnerDraw },
	// bongdarishopdlg.cpp:91
	{ "bonusball01", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdariShopDlg::OnBonusBall01BnInit },
	// bongdarishopdlg.cpp:92
	{ "bonusball01", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrBongdariShopDlg::OnBonusBall01OwnerDraw },
	// bongdarishopdlg.cpp:93
	{ "bonusball02", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdariShopDlg::OnBonusBall02BnInit },
	// bongdarishopdlg.cpp:94
	{ "bonusball02", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrBongdariShopDlg::OnBonusBall02OwnerDraw },
	// bongdarishopdlg.cpp:95
	{ "bonusball03", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdariShopDlg::OnBonusBall03BnInit },
	// bongdarishopdlg.cpp:96
	{ "bonusball03", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrBongdariShopDlg::OnBonusBall03OwnerDraw },
	// bongdarishopdlg.cpp:97
	{ "bonusball04", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdariShopDlg::OnBonusBall04BnInit },
	// bongdarishopdlg.cpp:98
	{ "bonusball04", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrBongdariShopDlg::OnBonusBall04OwnerDraw },
	// bongdarishopdlg.cpp:99
	{ "bonusball05", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdariShopDlg::OnBonusBall05BnInit },
	// bongdarishopdlg.cpp:100
	{ "bonusball05", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrBongdariShopDlg::OnBonusBall05OwnerDraw },

	// bongdarishopdlg.cpp:102
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// bongdarishopdlg.cpp:237
FrBongdariShopDlg::~FrBongdariShopDlg(void) {
	Clear();

	SendReBuildMyInventory();
	SendDataClosedBonusBall();
	OpenClosedBonusBallResultDlg();

	if (IsLocalContent((localContentType_t)0x59)) {
		if ((!CTaskManager::Instance() ? 0 : CTaskManager::Instance()->GetCurrentTask()) != 0) {
			WSendPacket packet((enumClientPacket)0xB9);
			packet.Encode1(1);
			packet.Send(SEND_TO_DEFAULT);
		}
	}
}

// bongdarishopdlg.cpp:258
void FrBongdariShopDlg::OnBongdariShopInit(int) {
}

// bongdarishopdlg.cpp:262
void FrBongdariShopDlg::OnAboutShopBnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:267
void FrBongdariShopDlg::OnAboutShopBnUp(void) {
	OpenAboutShopDlg();
}

// bongdarishopdlg.cpp:273
void FrBongdariShopDlg::OnTotalItemBnInit(intptr_t param) {
	m_pTotalItemBn = DYNAMIC_CAST(FrButton, param);
	if (m_pTotalItemBn) {
		m_pTotalItemBn->EnableBlink(false);
	}
}

// bongdarishopdlg.cpp:280
void FrBongdariShopDlg::OnTotalItemBnUp(void) {
	OpenTotalItemDlg();
}

// bongdarishopdlg.cpp:286
void FrBongdariShopDlg::OnBongdarishopNoticeBnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:291
void FrBongdariShopDlg::OnBongdarishopNoticeBnUp(void) {
	OpenBongdarishopNoticeDlg(false);
}

// bongdarishopdlg.cpp:297
void FrBongdariShopDlg::OnCloseBnInit(intptr_t param) {
	m_pExitBn = DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:302
void FrBongdariShopDlg::OnCloseBnUp(void) {
	OnFreshOkay();
}

// bongdarishopdlg.cpp:308
void FrBongdariShopDlg::OnBongdariInit(intptr_t param) {
	m_pBongdariArea = DYNAMIC_CAST(FrArea, param);
	if (m_pBongdariArea) {
		m_pBongdariArea->SetMouseEvent(false);
	}
}

// bongdarishopdlg.cpp:316
void FrBongdariShopDlg::OnBongdariOwnerDraw(void) {
	DrawShadow();

	DisplayItemSilhouette();

	DrawUserInfo();

	switch (m_shopState) {
	case 0:
		DrawTextBongdariClick();
		break;

	case 4:
		DrawTextBonusBallClick();
		break;
	}

	g_view->Flush(0);

	if (m_pBongdari) {
		WRect rc;
		rc.x = m_rect.x + 4.0f;
		rc.y = m_rect.y + 49.0f;
		rc.w = 607.0f;
		rc.h = 455.25f;
		m_pBongdari->DisplayClip(rc, 55.0f, 0.1f, 100.0f);
	}
}

// bongdarishopdlg.cpp:353
void FrBongdariShopDlg::OnBonusBall01BnInit(intptr_t param) {
	m_pBonusBallArea[0] = DYNAMIC_CAST(FrArea, param);

	if (m_pBonusBallArea[0]) {
		m_pBonusBallArea[0]->SetVisible(false);
		m_pBonusBallArea[0]->SetMouseEvent(false);
	}
}

// bongdarishopdlg.cpp:369
void FrBongdariShopDlg::OnBonusBall02BnInit(intptr_t param) {
	m_pBonusBallArea[1] = DYNAMIC_CAST(FrArea, param);

	if (m_pBonusBallArea[1]) {
		m_pBonusBallArea[1]->SetVisible(false);
		m_pBonusBallArea[1]->SetMouseEvent(false);
	}
}

// bongdarishopdlg.cpp:385
void FrBongdariShopDlg::OnBonusBall03BnInit(intptr_t param) {
	m_pBonusBallArea[2] = DYNAMIC_CAST(FrArea, param);

	if (m_pBonusBallArea[2]) {
		m_pBonusBallArea[2]->SetVisible(false);
		m_pBonusBallArea[2]->SetMouseEvent(false);
	}
}

// bongdarishopdlg.cpp:401
void FrBongdariShopDlg::OnBonusBall04BnInit(intptr_t param) {
	m_pBonusBallArea[3] = DYNAMIC_CAST(FrArea, param);

	if (m_pBonusBallArea[3]) {
		m_pBonusBallArea[3]->SetVisible(false);
		m_pBonusBallArea[3]->SetMouseEvent(false);
	}
}

// bongdarishopdlg.cpp:417
void FrBongdariShopDlg::OnBonusBall05BnInit(intptr_t param) {
	m_pBonusBallArea[4] = DYNAMIC_CAST(FrArea, param);

	if (m_pBonusBallArea[4]) {
		m_pBonusBallArea[4]->SetVisible(false);
		m_pBonusBallArea[4]->SetMouseEvent(false);
	}
}

// bongdarishopdlg.cpp:364
void FrBongdariShopDlg::OnBonusBall01OwnerDraw(void) {
	DrawBonusBall(0);
}

// bongdarishopdlg.cpp:380
void FrBongdariShopDlg::OnBonusBall02OwnerDraw(void) {
	DrawBonusBall(1);
}

// bongdarishopdlg.cpp:396
void FrBongdariShopDlg::OnBonusBall03OwnerDraw(void) {
	DrawBonusBall(2);
}

// bongdarishopdlg.cpp:412
void FrBongdariShopDlg::OnBonusBall04OwnerDraw(void) {
	DrawBonusBall(3);
}

// bongdarishopdlg.cpp:428
void FrBongdariShopDlg::OnBonusBall05OwnerDraw(void) {
	DrawBonusBall(4);
}

// bongdarishopdlg.cpp:442
bool FrBongdariShopDlg::OnInit(void) {
	EnableDrag(false);
	OpenBongdarishopNoticeDlg(true);
	ShowBongdari();
	InitCouponBongdariShop();

	m_ballPos.x += m_rect.x;
	m_ballPos.y += m_rect.y;

	return true;
}

// bongdarishopdlg.cpp:620
bool FrBongdariShopDlg::OnLButtonUp(const WPoint& pt) {
	FrForm::OnLButtonUp(pt);

	if (m_pBongdariArea) {
		if (m_shopState == 0) {
			if (!m_bUsingBonusBall && m_bBongdariPressed) {
				if (WRect(m_pBongdariArea->GetRect()).IsInRect(pt)) {
					if (WSingleton<CSharedDoc>::Instance()->m_myLevel == 0) {
						OpenBongdarishopWarningDlg(0, 0);
					}
					else {
						m_bUsingBonusBall = true;
						WSendPacket packet((enumClientPacket)0x6D);
						packet.Send(SEND_TO_DEFAULT);

						g_audio->PlaySfx("\xB9\xF6\xC6\xB0\xC5\xAC\xB8\xAF", 0, 0, 0, 0, 0.5f, 200.0f);

						EnableExitButton(false);

						m_animTime = 0.0f;
					}
				}
			}
		}
	}

	m_bBongdariPressed = false;

	return false;
}

// bongdarishopdlg.cpp:662
void FrBongdariShopDlg::OnCancel(void) {
}

// bongdarishopdlg.cpp:669
void FrBongdariShopDlg::OpenAboutShopDlg(void) {
	FrAboutShopDlg* pDlg = CreateForm<FrAboutShopDlg>(g_pFresh->GetManager(), this, "about_shop", 0);

	if (pDlg) {
		pDlg->Open((FRESH_RESULT_CALLBACK)&FrBongdariShopDlg::OnAboutShopDlgResult, 3);
		m_bSubDlgOpen = true;
	}
}

// bongdarishopdlg.cpp:681
void FrBongdariShopDlg::OpenTotalItemDlg(void) {
	FrTotalItemDlg* pDlg = CreateForm<FrTotalItemDlg>(g_pFresh->GetManager(), this, "total_item", 0);

	if (pDlg) {
		pDlg->SetListItem(&m_totalItemList);
		pDlg->Open((FRESH_RESULT_CALLBACK)&FrBongdariShopDlg::OnTotalItemDlgResult, 3);
		m_bSubDlgOpen = true;
	}
}

// bongdarishopdlg.cpp:694
void FrBongdariShopDlg::OpenBongdarishopNoticeDlg(bool b) {
	FrBongdarishopNoticeDlg* pDlg = CreateForm<FrBongdarishopNoticeDlg>(g_pFresh->GetManager(), this, "bongdarishop_notice", 0);

	if (pDlg) {
		pDlg->Open((FRESH_RESULT_CALLBACK)&FrBongdariShopDlg::OnBongdarishopNoticeDlgResult, 0xC0);
		pDlg->SetButtonType(b);
		pDlg->m_flag = b ? 0xC1 : 3;
		m_bSubDlgOpen = true;
	}
}

// bongdarishopdlg.cpp:717
void FrBongdariShopDlg::OpenBongdarishopRareItemDlg(int index) {
	FrBongdarishopRareItemDlg* pDlg = CreateForm<FrBongdarishopRareItemDlg>(g_pFresh->GetManager(), this, "bongdarishop_rareitem", 0);

	if (pDlg) {
		pDlg->Open((FRESH_RESULT_CALLBACK)&FrBongdariShopDlg::OnBongdarishopRareItemDlgResult, 3);

		if (index >= 0 && index < m_bonusBallCount) {
			pDlg->SetRareItem(&m_bonusBalls[index]);
			m_bSubDlgOpen = true;
		}
	}
}

// bongdarishopdlg.cpp:734
void FrBongdariShopDlg::OpenBongdarishopWarningDlg(int a, int b) {
	FrBongdarishopWarningDlg* pDlg = CreateForm<FrBongdarishopWarningDlg>(g_pFresh->GetManager(), this, "bongdarishop_warning", 0);

	if (pDlg) {
		pDlg->Open((FRESH_RESULT_CALLBACK)&FrBongdariShopDlg::OnBongdarishopWarningDlgResult, 0x40);
		pDlg->SetImgWarningBack(a, b);
		m_bSubDlgOpen = true;
	}
}

// bongdarishopdlg.cpp:747
void FrBongdariShopDlg::OpenClosedBonusBallResultDlg(void) {
	if (AfxGetTask()) {
		FrClosedBonusBallResultDlg* pDlg = CreateForm<FrClosedBonusBallResultDlg>(g_pFresh->GetManager(), 0, "closed_bonusball", 0);

		if (pDlg) {
			pDlg->Open((FRESH_RESULT_CALLBACK)0, 3);
			pDlg->SetClosedBonusBall(m_bonusBalls);
		}
	}
}

// bongdarishopdlg.cpp:764
bool FrBongdariShopDlg::OnAboutShopDlgResult(int, FrForm*) {
	m_bSubDlgOpen = false;
	return true;
}

// bongdarishopdlg.cpp:771
bool FrBongdariShopDlg::OnTotalItemDlgResult(int, FrForm*) {
	m_bSubDlgOpen = false;
	return true;
}

// bongdarishopdlg.cpp:778
bool FrBongdariShopDlg::OnBongdarishopNoticeDlgResult(int ret, FrForm*) {
	if (ret == 1) {
		FrBongdarishopNoticeDlg* pDlg = FrStaticCast<FrBongdarishopNoticeDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdarishop_notice"));

		if (pDlg) {
			if (pDlg->GetMoveLayoutTid()) {
				OpenAboutShopDlg();
			}
		}
	}

	m_bSubDlgOpen = false;
	return true;
}

// bongdarishopdlg.cpp:800
bool FrBongdariShopDlg::OnBongdarishopRareItemDlgResult(int, FrForm*) {
	m_bSubDlgOpen = false;
	return true;
}

// bongdarishopdlg.cpp:807
bool FrBongdariShopDlg::OnBongdarishopWarningDlgResult(int, FrForm*) {
	m_bSubDlgOpen = false;
	return true;
}

// bongdarishopdlg.cpp:814
void FrBongdariShopDlg::ControlNoticeDlg(void) {
	FrBongdarishopNoticeDlg* pDlg = FrStaticCast<FrBongdarishopNoticeDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdarishop_notice"));

	if (!pDlg) {
		return;
	}

	if (g_input->GetDown("\xC1\xC2", false)) {
		pDlg->PrevIndex();
	} else if (g_input->GetDown("\xBF\xEC", false)) {
		pDlg->NextIndex();
	}
}

// bongdarishopdlg.cpp:835
void FrBongdariShopDlg::InitBongdariShop(void) {
	for (int i = 0; i < 9; i++) {
		m_silhouettes[i].m_slot = -1;
		m_silhouettes[i].m_pImage = m_pDefaultSilhouetteImage;
		m_silhouettes[i].m_slotParam = 0;
		m_silhouettes[i].m_grade = 0;
		m_silhouettes[i].m_state = 0;
		m_silhouettes[i].m_scale = 10.0f;
	}

	memset(m_bonusBalls, 0, sizeof(m_bonusBalls));
	m_bonusBallCount = 0;

	for (int j = 0; j < 5; j++) { if (m_pPetBonusBall[j]) { delete m_pPetBonusBall[j]; m_pPetBonusBall[j] = 0; } }

	memset(m_movingData, 0, sizeof(m_movingData));
}

// bongdarishopdlg.cpp:859
void FrBongdariShopDlg::SetBongdariShop(void) {
	if (m_bonusBallCount <= 5) {
		DeletePetBonusBall();
		SetRandomDataItemSilhouette();
		InitCouponBongdariShop();

		m_shopState = 1;
	}
}

// bongdarishopdlg.cpp:871
void FrBongdariShopDlg::SetBonusBall(int count, sBonusBall* pBalls) {
	if (pBalls) {
		if (count <= 5) {
			memcpy(m_bonusBalls, pBalls, count * sizeof(sBonusBall));
			m_bonusBallCount = count;
		}
	}
}

// bongdarishopdlg.cpp:902
void FrBongdariShopDlg::Clear(void) {
	SendStateBongdariShop(false);

	if (m_pUserInfoUI) { delete m_pUserInfoUI; m_pUserInfoUI = 0; }

	if (m_pTitleFont29c) {
		m_pTitleFont29c->Erase();
	}

	if (m_pTitleFont2a0) {
		m_pTitleFont2a0->Erase();
	}

	if (m_pTitleFont2a4) {
		m_pTitleFont2a4->Erase();
	}

	if (g_resrcmng && m_pTitleFont2a4) { g_resrcmng->Release(m_pTitleFont2a4); m_pTitleFont2a4 = 0; }
	if (g_resrcmng && m_pTitleFont29c) { g_resrcmng->Release(m_pTitleFont29c); m_pTitleFont29c = 0; }
	if (g_resrcmng && m_pTitleFont2a0) { g_resrcmng->Release(m_pTitleFont2a0); m_pTitleFont2a0 = 0; }
	if (m_pBongdari) { delete m_pBongdari; m_pBongdari = 0; }

	DeletePetBonusBall();

	FrForm* pAbout = FrStaticCast<FrForm>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("about_shop"));
	if (pAbout) {
		pAbout->Close(FrCANCEL, true);
	}

	FrForm* pTotal = FrStaticCast<FrForm>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("total_item"));
	if (pTotal) {
		pTotal->Close(FrOK, true);
	}

	FrForm* pNotice = FrStaticCast<FrForm>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdarishop_notice"));
	if (pNotice) {
		pNotice->Close(FrOK, true);
	}

	FrForm* pRare = FrStaticCast<FrForm>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdarishop_rareitem"));
	if (pRare) {
		pRare->Close(FrCANCEL, true);
	}

	if ((!CTaskManager::Instance() ? 0 : CTaskManager::Instance()->GetCurrentTask()) != 0) {
		for (int i = 0; i < m_bonusBallCount; i++) {
			UpdateListMyItem(&m_bonusBalls[i]);
		}
	}
}

// bongdarishopdlg.cpp:1039
void FrBongdariShopDlg::InitCouponBongdariShop(void) {
	m_couponCount = 0;

	for (std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_itemInfoList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_itemInfoList.end(); ++it) {
		if (ItemManager()->GetCouponKind((*it).m_typeId) == 2) {
			m_couponCount += (*it).m_clubStatLevel[0];
		}
	}
}

// bongdarishopdlg.cpp:1055
void FrBongdariShopDlg::SetDataFee(int fee) {
	if (fee > m_dataFee) {
		m_dataFee = fee;
		m_dataFee3d0 = 0;
		m_dataFee3d4 = 0x200;
	}
}

// bongdarishopdlg.cpp:1066
void FrBongdariShopDlg::UpdateUsableTimes(void) {
	m_usableTimes[0] = WSingleton<CSharedDoc>::Instance()->GetBsUsableTimes();
	m_usableTimes[1] = WSingleton<CSharedDoc>::Instance()->GetBsUsableBonusTimes();
	m_usableTimes[2] = WSingleton<CSharedDoc>::Instance()->GetBsRemainedBonusTimes();
}

// bongdarishopdlg.cpp:1074
void FrBongdariShopDlg::ShowDiscountUsableTimes(void) {
	if (m_usableTimes[0] > 1 || m_usableTimes[0] == -1) {
		m_bShowDiscount = true;
		m_discountTime = 0;
		m_discountState = 0x200;
	} else if (m_usableTimes[1] > 0) {
		m_bShowBonusDiscount = true;
		m_discountTime = 0;
		m_discountState = 0x200;
	} else if (m_usableTimes[1] == 0 && m_usableTimes[2] == 0) {
		OpenBongdarishopWarningDlg(1, 0);
	} else {
		OpenBongdarishopWarningDlg(2, m_usableTimes[2]);
	}

	if (m_usableTimes[0] == -1)
		m_bShowDiscount = false;
	if (m_usableTimes[1] == -1)
		m_bShowBonusDiscount = false;
}

// bongdarishopdlg.cpp:1459
void FrBongdariShopDlg::SetPosMovingClickText(float dt) {
	if (m_clickTextDown == false) {
		m_clickTextY += dt * 10.0f;

		if (m_clickTextY >= 5.0f)
			m_clickTextDown = true;

	} else if (m_clickTextDown == true) {
		m_clickTextY -= dt * 10.0f;

		if (m_clickTextY <= 0.0f)
			m_clickTextDown = false;
	}
}

// bongdarishopdlg.cpp:1478
void FrBongdariShopDlg::DisplayItemSilhouette(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI && m_bonusBallCount <= 5) {
		float baseX = GetRect().x;
		float baseY = GetRect().y;
		char text[8] = { 0 };
		unsigned long color = 0xffffffff;

		for (int i = 0; i < 9; i++) {
			const Bitmap* pBmp = 0;

			switch (m_silhouettes[i].m_state) {
			case 0:
				pBmp = m_pDefaultSilhouetteImage;
				color = 0xffffffff;
				break;
			case 1:
				switch (m_silhouettes[i].m_grade) {
				case 1:
					pBmp = m_silhouettes[i].m_pImage;
					color = 0xa02020a0;
					break;
				case 2:
					pBmp = m_silhouettes[i].m_pImage;
					color = 0xa0a02020;
					break;
				default:
					pBmp = m_silhouettes[i].m_pImage;
					color = 0xa0202020;
					break;
				}
				break;
			case 2:
				pBmp = m_silhouettes[i].m_pImage;
				break;
			}

			if (pBmp) {
				float offX = 0.0f;
				float offY = 0.0f;

				if (m_silhouettes[i].m_state > 0 && m_silhouettes[i].m_state <= 2) {
					if (ItemManager()->GetCouponKind(m_silhouettes[i].m_slotParam)) {
						offX = -7.0f;
						offY = 2.0f;
					}
				}

				WRect rect(baseX + offX + g_arrPtItemSilhouette[i].x, baseY + g_arrPtItemSilhouette[i].y + offY, (float)pBmp->Width(), (float)pBmp->Height());

				pGDI->DrawTexture(pBmp, rect, color, 0);

				if (m_silhouettes[i].m_state == 2 && m_silhouettes[i].m_slot >= 0) {
					int count = m_bonusBalls[m_silhouettes[i].m_slot].m_count;

					if (count > 1 && m_pTitleFont2a0) {
						sprintf(text, "%d", count);
						m_pTitleFont2a0->Print(g_view, rect.x + 32.0f, rect.y + 30.0f, text, 0, 0xffffffff, 0);
					}
				}

				if (m_silhouettes[i].m_scale >= 0.0f && (int)m_silhouettes[i].m_scale < 8) {
					if (m_pUserInfoUI) {
						m_pUserInfoUI->Render(MakeStr("%d", (int)m_silhouettes[i].m_scale), baseX + g_arrPtItemSilhouette[i].x - 25.0f, baseY + g_arrPtItemSilhouette[i].y - 43.0f, 1.5f, 0);
					}
				}
			}
		}
	}
}

// bongdarishopdlg.cpp:1574
void FrBongdariShopDlg::DrawShadow(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		if (m_pShadowImage) {
			if (m_shopState == 0) {
				if (m_pBongdari) {
					const WRect& rc = m_pBongdariArea->GetRect();
					float w = m_pShadowImage->Width() * 1.5f;
					float h = m_pShadowImage->Height() * 1.5f;

					WRect rect(rc.x + rc.w * 0.5f - w * 0.5f, rc.y + rc.h - 23.0f - h * 0.5f, w, h);

					pGDI->DrawTexture(m_pShadowImage, rect, 0x90ffffff, 0);
				}
			}

			for (int i = 0; i < m_bonusBallCount; i++) {
				if (m_pPetBonusBall[i]) {
					float dist = fabsf(m_movingData[i].m_height - m_movingData[i].m_shadowY);

					if (dist < 40.0f) {
						unsigned char alpha = 0x90;

						if (dist > 0.0f) {
							alpha = (unsigned char)(144.0f / (dist * 1.5f));
						}

						float x = m_movingData[i].m_shadowX;
						float y = m_movingData[i].m_shadowY;
						float w, h, yOffset;

						if (m_selectedBall == i) {
							w = m_pShadowImage->Width() * 0.7f;
							h = m_pShadowImage->Height() * 0.7f;
							yOffset = 36.0f;
						} else {
							w = m_pShadowImage->Width() * 0.55f;
							h = m_pShadowImage->Height() * 0.55f;
							yOffset = 28.0f;
						}

						WRect rect(x - w * 0.5f, y - h * 0.5f + yOffset, w, h);

						pGDI->DrawTexture(m_pShadowImage, rect, (alpha << 24) | 0xffffff, 0);
					}
				}
			}
		}
	}
}

// bongdarishopdlg.cpp:1641
void FrBongdariShopDlg::DrawTextBongdariClick(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		if (m_pBongdariArea) {
			if (m_pClickTextImage) {
				const WRect& rc = m_pBongdariArea->GetRect();

				pGDI->DrawTexture(m_pClickTextImage, WRect(rc.x - 13.0f, rc.y - 100.0f + m_clickTextY, m_pClickTextImage->Width(), m_pClickTextImage->Height()), 0xffffffff, 0);
			}
		}
	}
}

// bongdarishopdlg.cpp:1657
void FrBongdariShopDlg::DrawTextBonusBallClick(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		if (m_pBonusClickTextImage) {
			int i;

			for (i = 0; i < 5; i++) {
				if (!m_pBonusBallArea[i])
					return;
			}

			if (m_selectedBall >= 0 && m_selectedBall < m_bonusBallCount) {
				WPoint pos = m_pBonusBallArea[m_selectedBall]->GetRect().TopLeft();

				WRect rc(pos.x - 10.0f, pos.y - 85.0f + m_clickTextY, (float)m_pBonusClickTextImage->Width(), (float)m_pBonusClickTextImage->Height());

				pGDI->DrawTexture(m_pBonusClickTextImage, rc, 0xffffffff, 0);
			}
		}
	}
}

// bongdarishopdlg.cpp:1887
void FrBongdariShopDlg::SetOpeningBonusBallInfo(sBonusBall* pBall) {
	char text[256];   // unused; the original's frame keeps its /GS cookie

	if (pBall) {
		IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(pBall->m_typeId);

		if (pItem) {
			SetMessage(MakeStr("%s %d\xB0\xB3 \xC8\xB9\xB5\xE6", pItem->m_name, pBall->m_count), false);
		}
	} else {
		SetMessage("\xC8\xB9\xB5\xE6 \xBE\xC6\xC0\xCC\xC5\xDB \xC1\xA4\xBA\xB8\xB0\xA1 \xC7\xA5\xBD\xC3\xB5\xCB\xB4\xCF\xB4\xD9", false);
	}
}

// bongdarishopdlg.cpp:1908
void FrBongdariShopDlg::ShowBongdari(void) {
	if (m_pBongdari) { delete m_pBongdari; m_pBongdari = 0; }

	if (m_pBongdariArea) {
		WRect rect = m_pBongdariArea->GetRect();
		float distance = g_view->GetWidth() * 0.0421875f;

		m_pBongdari = new CExhibition(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f,
			(g_view->GetWidth() * 0.0015625f - (1.0f / g_view->GetRatio() - 1.3333334f) * 0.7f) / g_view->GetHeight() * 69.0f, true);

		m_pBongdari->SetDistanceRange(distance - 5.0f, distance + 5.0f);
		m_pBongdari->SetDistance(distance, true, true);
		m_pBongdari->SetArea(rect.w, rect.h);

		m_pBongdari->SetModel("bon_shop", (eExhibitType)1);
	}
}

// bongdarishopdlg.cpp:1966
void FrBongdariShopDlg::DeletePetBonusBall(void) {
	for (int i = 0; i < 5; i++) {
		if (m_pPetBonusBall[i]) { delete m_pPetBonusBall[i]; m_pPetBonusBall[i] = 0; }
	}
}

// bongdarishopdlg.cpp:1977
void FrBongdariShopDlg::SetStartAnimationBongdari(void) {
	if (m_pBongdari) {
		m_bAnimating = true;
		m_pBongdari->SetMotion(data_a97c78[rand() % 5 + 1], false, 0.0f);
	}
}

// bongdarishopdlg.cpp:2252
void FrBongdariShopDlg::ShuffleRandomDataItemSilhouette(void) {
	int r;
	unsigned long tmp;

	for (int i = 0; i < 100; i++) {
		r = rand() % 100;
		tmp = g_arrdwRandNumCookieItem0[i];
		g_arrdwRandNumCookieItem0[i] = g_arrdwRandNumCookieItem0[r];
		g_arrdwRandNumCookieItem0[r] = tmp;

		r = rand() % 100;
		tmp = g_arrdwRandNumCookieItem1[i];
		g_arrdwRandNumCookieItem1[i] = g_arrdwRandNumCookieItem1[r];
		g_arrdwRandNumCookieItem1[r] = tmp;

		r = rand() % 100;
		tmp = g_arrdwRandNumCookieItem2[i];
		g_arrdwRandNumCookieItem2[i] = g_arrdwRandNumCookieItem2[r];
		g_arrdwRandNumCookieItem2[r] = tmp;
	}

	for (int j = 0; j < 100; j++) {
		r = rand() % 100;
		tmp = g_arrdwRandSelectCookieItem[j];
		g_arrdwRandSelectCookieItem[j] = g_arrdwRandSelectCookieItem[r];
		g_arrdwRandSelectCookieItem[r] = tmp;
	}

	for (int k = 0; k < 100; k++) {
		r = rand() % 100;
		tmp = g_arrdwRandSelectPangItem[k];
		g_arrdwRandSelectPangItem[k] = g_arrdwRandSelectPangItem[r];
		g_arrdwRandSelectPangItem[r] = tmp;
	}
}

// bongdarishopdlg.cpp:2473
unsigned long FrBongdariShopDlg::SelectDifferentTypeId(int type) {
	unsigned long* pTable;

	if (type == 0) {
		pTable = g_arrdwRandSelectPangItem;
	} else if (type == 1) {
		pTable = g_arrdwRandSelectCookieItem;
	} else {
		return 0;
	}

	int index = rand() % 100;
	unsigned long typeId;
	bool bRetry;

	do {
		typeId = pTable[index];
		bRetry = false;

		for (int i = 0; i < 9; i++) {
			if (m_silhouettes[i].m_slotParam == 0)
				break;

			if (m_silhouettes[i].m_slotParam == typeId) {
				index = rand() % 100;
				bRetry = true;
				break;
			}
		}
	} while (bRetry);

	return typeId;
}

// bongdarishopdlg.cpp:2540
void FrBongdariShopDlg::AddListTotalItem(sBonusBall* pBall) {
	if (pBall == 0)
		return;

	for (std::list<sBonusBall>::iterator it = m_totalItemList.begin(); it != m_totalItemList.end(); ++it) {
		if ((*it).m_typeId == pBall->m_typeId) {
			if ((*it).m_bUnitCount == false) {
				(*it).m_count += pBall->m_count;
			} else {
				(*it).m_unitCount += pBall->m_unitCount;
			}
			return;
		}
	}

	m_totalItemList.push_back(*pBall);
}

// bongdarishopdlg.cpp:2744
void FrBongdariShopDlg::SendReBuildMyInventory(void) {
	if (IsLocalContent((localContentType_t)1)) {
		if (AfxGetTask()) {
			if (AfxGetTask()->GetActor("Shop"))
				AfxGetTask()->GetActor("Shop") << MsgObject(0, 0xB0, 0, 0, 0, 0, 0);
			if (AfxGetTask()->GetActor("RealMyRoom"))
				AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0xB0, 0, 0, 0, 0, 0);
		}
	}
}

// bongdarishopdlg.cpp:2758
void FrBongdariShopDlg::SendDataClosedBonusBall(void) {
	if (AfxGetTask()) {
		bool bClosed = false;

		for (int i = 0; i < 5; i++) {
			if (m_bonusBalls[i].m_typeId != 0 && m_bonusBalls[i].m_index >= 0) {
				bClosed = true;
				break;
			}
		}

		if (!bClosed)
			return;

		if (IsLocalContent((localContentType_t)1)) {
			if (AfxGetTask()->GetActor("GolfRule"))
				AfxGetTask()->GetActor("GolfRule") << MsgObject(0, 0xB1, (intptr_t)m_bonusBalls, 0, 0, 0, 0);
		}
	}
}

// bongdarishopdlg.cpp:2786
void FrBongdariShopDlg::EnableExitButton(bool enable) {
	if (m_animTime < 8.0f)
		return;

	if (m_pExitBn) m_pExitBn->Enable(enable);
}

// bongdarishopdlg.cpp:2794
void FrBongdariShopDlg::DrawBonusBall(unsigned int index) {
	if (index < 5) {
		if (m_shopState >= 3 && m_shopState <= 4) {
			if (m_pPetBonusBall[index]) {
				float distance = 3.5f;
				if (m_selectedBall == index)
					distance = 2.9f;

				m_pPetBonusBall[index]->SetDistance((distance - m_movingData[index].m_distance) * g_view->GetWidth() * 0.0015625f, true, true);
				m_pPetBonusBall[index]->Display(0.1f, 100.0f);
			}
		}
	}
}

// bongdarishopdlg.cpp:2819 (run-time type and factory registration of
// FrAboutShopDlg)
const WRTTI FrAboutShopDlg::m_RTTI("FrAboutShopDlg", &FrForm::m_RTTI);

IObject* FrAboutShopDlgMakeInstance(void) {
	return new FrAboutShopDlg;
}

struct __sFrAboutShopDlg {
	__sFrAboutShopDlg(void) {
		ObjectFactory().AddObjectFunctor(FrAboutShopDlgMakeInstance, "FrAboutShopDlg");
	}
};
static __sFrAboutShopDlg s_registerFrAboutShopDlg;

// bongdarishopdlg.cpp:2821
const sFRESH_MSGMAP* FrAboutShopDlg::GetMessageMap(void) const {
	return &FrAboutShopDlg::_MsgMap;
}

sFRESH_MSGMAP FrAboutShopDlg::_MsgMap = { &FrForm::_MsgMap, &FrAboutShopDlg::_MsgEntries[0] };

sFRESH_ENTRY FrAboutShopDlg::_MsgEntries[] = {
	// bongdarishopdlg.cpp:2823
	{ "about_shop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnAboutShopInit },
	// bongdarishopdlg.cpp:2824
	{ "aboutviewer", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnAboutViewerInit },
	// bongdarishopdlg.cpp:2825
	{ "index01", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnIndex01Init },
	// bongdarishopdlg.cpp:2826
	{ "index01", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnIndex01BnUp },
	// bongdarishopdlg.cpp:2827
	{ "index02", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnIndex02Init },
	// bongdarishopdlg.cpp:2828
	{ "index02", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnIndex02BnUp },
	// bongdarishopdlg.cpp:2829
	{ "index03", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnIndex03Init },
	// bongdarishopdlg.cpp:2830
	{ "index03", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnIndex03BnUp },
	// bongdarishopdlg.cpp:2831
	{ "index04", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnIndex04Init },
	// bongdarishopdlg.cpp:2832
	{ "index04", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnIndex04BnUp },
	// bongdarishopdlg.cpp:2833
	{ "index05", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnIndex05Init },
	// bongdarishopdlg.cpp:2834
	{ "index05", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnIndex05BnUp },
	// bongdarishopdlg.cpp:2835
	{ "index06", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnIndex06Init },
	// bongdarishopdlg.cpp:2836
	{ "index06", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnIndex06BnUp },
	// bongdarishopdlg.cpp:2837
	{ "index07", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnIndex07Init },
	// bongdarishopdlg.cpp:2838
	{ "index07", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnIndex07BnUp },
	// bongdarishopdlg.cpp:2839
	{ "index08", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnIndex08Init },
	// bongdarishopdlg.cpp:2840
	{ "index08", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnIndex08BnUp },
	// bongdarishopdlg.cpp:2841
	{ "index09", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnIndex09Init },
	// bongdarishopdlg.cpp:2842
	{ "index09", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnIndex09BnUp },
	// bongdarishopdlg.cpp:2843
	{ "index10", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnIndex10Init },
	// bongdarishopdlg.cpp:2844
	{ "index10", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnIndex10BnUp },
	// bongdarishopdlg.cpp:2845
	{ "backward", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnBackwardBnInit },
	// bongdarishopdlg.cpp:2846
	{ "backward", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnBackwardBnUp },
	// bongdarishopdlg.cpp:2847
	{ "forward", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnForwardBnInit },
	// bongdarishopdlg.cpp:2848
	{ "forward", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnForwardBnUp },
	// bongdarishopdlg.cpp:2849
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAboutShopDlg::OnOkBnInit },
	// bongdarishopdlg.cpp:2850
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAboutShopDlg::OnOkBnUp },

	// bongdarishopdlg.cpp:2852
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// bongdarishopdlg.cpp:2855
FrAboutShopDlg::FrAboutShopDlg(void) {
	for (int i = 0; i < 10; i++) {
		m_pIndexBn[i] = 0;
	}

	m_curIndex = 0;
	m_pBackwardBn = 0;
	m_pForwardBn = 0;
}

// bongdarishopdlg.cpp:2869
FrAboutShopDlg::~FrAboutShopDlg(void) {
}

// bongdarishopdlg.cpp:2873
void FrAboutShopDlg::OnAboutShopInit(int) {
}

// bongdarishopdlg.cpp:2877
void FrAboutShopDlg::OnAboutViewerInit(intptr_t param) {
	m_pAboutViewer = DYNAMIC_CAST(FrViewer, param);
}

// bongdarishopdlg.cpp:2883
void FrAboutShopDlg::OnIndex01Init(intptr_t param) {
	m_pIndexBn[0] = DYNAMIC_CAST(FrTextButton, param);
}

// bongdarishopdlg.cpp:2888
void FrAboutShopDlg::OnIndex01BnUp(void) {
	SetImgAboutIndex(0);
}

// bongdarishopdlg.cpp:2893
void FrAboutShopDlg::OnIndex02Init(intptr_t param) {
	m_pIndexBn[1] = DYNAMIC_CAST(FrTextButton, param);
}

// bongdarishopdlg.cpp:2898
void FrAboutShopDlg::OnIndex02BnUp(void) {
	SetImgAboutIndex(1);
}

// bongdarishopdlg.cpp:2903
void FrAboutShopDlg::OnIndex03Init(intptr_t param) {
	m_pIndexBn[2] = DYNAMIC_CAST(FrTextButton, param);
}

// bongdarishopdlg.cpp:2908
void FrAboutShopDlg::OnIndex03BnUp(void) {
	SetImgAboutIndex(2);
}

// bongdarishopdlg.cpp:2913
void FrAboutShopDlg::OnIndex04Init(intptr_t param) {
	m_pIndexBn[3] = DYNAMIC_CAST(FrTextButton, param);
}

// bongdarishopdlg.cpp:2918
void FrAboutShopDlg::OnIndex04BnUp(void) {
	SetImgAboutIndex(3);
}

// bongdarishopdlg.cpp:2923
void FrAboutShopDlg::OnIndex05Init(intptr_t param) {
	m_pIndexBn[4] = DYNAMIC_CAST(FrTextButton, param);
}

// bongdarishopdlg.cpp:2928
void FrAboutShopDlg::OnIndex05BnUp(void) {
	SetImgAboutIndex(4);
}

// bongdarishopdlg.cpp:2933
void FrAboutShopDlg::OnIndex06Init(intptr_t param) {
	m_pIndexBn[5] = DYNAMIC_CAST(FrTextButton, param);
}

// bongdarishopdlg.cpp:2938
void FrAboutShopDlg::OnIndex06BnUp(void) {
	SetImgAboutIndex(5);
}

// bongdarishopdlg.cpp:2943
void FrAboutShopDlg::OnIndex07Init(intptr_t param) {
	m_pIndexBn[6] = DYNAMIC_CAST(FrTextButton, param);
}

// bongdarishopdlg.cpp:2948
void FrAboutShopDlg::OnIndex07BnUp(void) {
	SetImgAboutIndex(6);
}

// bongdarishopdlg.cpp:2953
void FrAboutShopDlg::OnIndex08Init(intptr_t param) {
	m_pIndexBn[7] = DYNAMIC_CAST(FrTextButton, param);
}

// bongdarishopdlg.cpp:2958
void FrAboutShopDlg::OnIndex08BnUp(void) {
	SetImgAboutIndex(7);
}

// bongdarishopdlg.cpp:2963
void FrAboutShopDlg::OnIndex09Init(intptr_t param) {
	m_pIndexBn[8] = DYNAMIC_CAST(FrTextButton, param);
}

// bongdarishopdlg.cpp:2968
void FrAboutShopDlg::OnIndex09BnUp(void) {
	SetImgAboutIndex(8);
}

// bongdarishopdlg.cpp:2973
void FrAboutShopDlg::OnIndex10Init(intptr_t param) {
	m_pIndexBn[9] = DYNAMIC_CAST(FrTextButton, param);
}

// bongdarishopdlg.cpp:2978
void FrAboutShopDlg::OnIndex10BnUp(void) {
	SetImgAboutIndex(9);
}

// bongdarishopdlg.cpp:2983
void FrAboutShopDlg::OnBackwardBnInit(intptr_t param) {
	m_pBackwardBn = DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:2988
void FrAboutShopDlg::OnBackwardBnUp(void) {
	SetImgAboutIndex(--m_curIndex);
	m_pBackwardBn->SetPushDelay(0.0f);
}

// bongdarishopdlg.cpp:2994
void FrAboutShopDlg::OnForwardBnInit(intptr_t param) {
	m_pForwardBn = DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:2999
void FrAboutShopDlg::OnForwardBnUp(void) {
	SetImgAboutIndex(++m_curIndex);
	m_pForwardBn->SetPushDelay(0.0f);
}

// bongdarishopdlg.cpp:3005
void FrAboutShopDlg::OnOkBnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:3010
void FrAboutShopDlg::OnOkBnUp(void) {
	OnFreshOkay();
}

// bongdarishopdlg.cpp:3015
bool FrAboutShopDlg::OnInit(void) {
	SetImgAboutIndex(0);

	return true;
}

// bongdarishopdlg.cpp:3023
void FrAboutShopDlg::SetImgAboutIndex(int index) {
	for (int i = 0; i < 10; i++) {
		m_pIndexBn[i]->Unselect(0);
	}

	if (m_pBackwardBn) {
		m_pBackwardBn->Enable(true);
	}

	if (m_pForwardBn) {
		m_pForwardBn->Enable(true);
	}

	if (index <= 0) {
		index = 0;
		m_pIndexBn[0]->Select(0);

		if (m_pBackwardBn) {
			m_pBackwardBn->Enable(false);
		}
	} else if (index >= 9) {
		index = 9;
		m_pIndexBn[9]->Select(0);

		if (m_pForwardBn) {
			m_pForwardBn->Enable(false);
		}
	}

	m_pIndexBn[index]->Select(0);

	m_curIndex = index;
	if (m_pAboutViewer) {
		m_pAboutViewer->Open(MakeStr("help_bs_%02d.jpg", index + 1));
	}
}

// bongdarishopdlg.cpp:3078 (run-time type and factory registration of
// FrTotalItemDlg)
const WRTTI FrTotalItemDlg::m_RTTI("FrTotalItemDlg", &FrForm::m_RTTI);

IObject* FrTotalItemDlgMakeInstance(void) {
	return new FrTotalItemDlg;
}

struct __sFrTotalItemDlg {
	__sFrTotalItemDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTotalItemDlgMakeInstance, "FrTotalItemDlg");
	}
};
static __sFrTotalItemDlg s_registerFrTotalItemDlg;

// bongdarishopdlg.cpp:3080
const sFRESH_MSGMAP* FrTotalItemDlg::GetMessageMap(void) const {
	return &FrTotalItemDlg::_MsgMap;
}

sFRESH_MSGMAP FrTotalItemDlg::_MsgMap = { &FrForm::_MsgMap, &FrTotalItemDlg::_MsgEntries[0] };

sFRESH_ENTRY FrTotalItemDlg::_MsgEntries[] = {
	// bongdarishopdlg.cpp:3082
	{ "total_item", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTotalItemDlg::OnTotalItemInit },
	// bongdarishopdlg.cpp:3083
	{ "totalitemlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTotalItemDlg::OnTotalItemListInit },
	// bongdarishopdlg.cpp:3084
	{ "totalitemlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTotalItemDlg::OnTotalItemListOwnerDraw },
	// bongdarishopdlg.cpp:3085
	{ "totalitemlist", FRCMD_RBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrTotalItemDlg::OnTotalItemListRBnDown },
	// bongdarishopdlg.cpp:3086
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTotalItemDlg::OnOkBnInit },
	// bongdarishopdlg.cpp:3087
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTotalItemDlg::OnOkBnUp },

	// bongdarishopdlg.cpp:3089
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// bongdarishopdlg.cpp:3092
FrTotalItemDlg::FrTotalItemDlg(void) {
	m_pTotalItemList = 0;
	m_pIconBase = g_pFresh->GetManager()->GetBitmap("ITEMS", "shop_icon_base02");

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = &data_a97c78[914];
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:";

	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);
}

// bongdarishopdlg.cpp:3114
FrTotalItemDlg::~FrTotalItemDlg(void) {
	m_bonusBallList.clear();
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// bongdarishopdlg.cpp:3120
void FrTotalItemDlg::OnTotalItemInit(int) {
}

// bongdarishopdlg.cpp:3124
void FrTotalItemDlg::OnTotalItemListInit(intptr_t param) {
	m_pTotalItemList = DYNAMIC_CAST(FrListBox, param);
	m_pTotalItemList->UseRightButton(true);

	m_clockBtn.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "clock");
	m_clockBtn.rect = WRect(50.0f, 35.0f, (float)m_clockBtn.pBitmap->Width(), (float)m_clockBtn.pBitmap->Height());
	m_deleteBtn.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "delete");
	m_deleteBtn.rect = WRect(50.0f, 35.0f, (float)m_deleteBtn.pBitmap->Width(), (float)m_deleteBtn.pBitmap->Height());
}

// bongdarishopdlg.cpp:3135
void FrTotalItemDlg::OnTotalItemListOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (!pGDI)
		return;

	FrListItem* pItem = (FrListItem*)param;

	if (!pItem)
		return;

	sBonusBall* pBall = (sBonusBall*)pItem->pData;

	if (!pBall->m_typeId)
		return;

	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pBall->m_typeId);

	if (!pCommon)
		return;

	float offsetX = 0.0f;
	float offsetY = 0.0f;
	const Bitmap* pBitmap = g_pFresh->GetBitmap(pCommon->m_icon);

	if ((pBall->m_typeId >> 26) == 2) {
		offsetX = -12.0f;
		offsetY = -20.0f;
	}

	if ((pBall->m_typeId >> 26) == 31) {
		offsetX = -2.0f;
		offsetY = -9.0f;
	}

	if (!pBitmap)
		return;

	if (!m_pIconBase)
		return;

	WRect rcBase(pItem->pos.x, pItem->pos.y, (float)m_pIconBase->Width(), (float)m_pIconBase->Height());

	pGDI->DrawTexture(m_pIconBase, rcBase, 0xffffffff, 0);

	WRect rc(offsetX + pItem->pos.x + 18.0f, offsetY + pItem->pos.y + 11.0f, (float)pBitmap->Width(), (float)pBitmap->Height());

	pGDI->DrawTexture(pBitmap, rc, 0xffffffff, 0);
	pGDI->SetTextColor(0xff000000, 0xffffffff);
	pGDI->SetTextStyle(4);

	if (pBall->m_bUnitCount) {
		if (pBall->m_unitCount > 0) {
			pGDI->DrawTexture(m_clockBtn.pBitmap, m_clockBtn.rect + pItem->pos, 0xffffffff, 0);
		}
	} else if (pBall->m_count > 1) {
		char* text = MakeStr("%d", pBall->m_count);
		float x = pItem->pos.x + 70.0f;

		for (unsigned int i = 0; i < strlen(text); i++) {
			x -= m_pTitleFont->GetCharWidthA(g_view, text[i] - '0');
		}

		m_pTitleFont->Print(g_view, x, pItem->pos.y + 45.0f, text, 0, 0xffffffff, 0);
	}
}

// bongdarishopdlg.cpp:3213
void FrTotalItemDlg::OnTotalItemListRBnDown(void) {
	if (m_pTotalItemList) {
		FrListItem* pItem = m_pTotalItemList->GetItemUnderCursor();

		if (pItem) {
			sBonusBall* pBall = (sBonusBall*)pItem->pData;

			if (pBall) {
				OpenInformation(pBall->m_typeId);
			}
		}
	}
}

// bongdarishopdlg.cpp:3229
void FrTotalItemDlg::OpenInformation(unsigned long typeId) {
	if (FrStaticCast<FrWnd>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("information")))
		return;

	if (FrStaticCast<FrWnd>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("information_setitem")))
		return;

	IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(typeId);

	if (!pItem)
		return;

	if ((pItem->m_typeId >> 26) == 9) {
		FrSetItemDlg* pSetDlg = CreateForm<FrSetItemDlg>(g_pFresh->GetManager(), this, "information_setitem", 0);

		if (!pSetDlg)
			return;

		pSetDlg->SetSetItem(pItem->m_typeId);
		pSetDlg->Open(0, 3);
		return;
	}

	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "information", 0);

	if (!pForm)
		return;

	FrArea* pPortrait = DYNAMIC_CAST(FrArea, pForm->FindChildByName("portrait"));

	if (pPortrait) {
		pPortrait->SetBgImg(pItem->m_icon);
	}

	FrEdit* pName = DYNAMIC_CAST(FrEdit, pForm->FindChildByName("name"));
	FrArea* pLevel = DYNAMIC_CAST(FrArea, pForm->FindChildByName("level"));

	if (pName && pLevel) {
		pName->AddText(MakeStr("\xC0\xCC\xB8\xA7: %s", pItem->m_name), false, true);
		pName->AddText("\n", false, true);

		if (pItem->m_level & 0x80) {
			pLevel->SetBgImg(MakeStr("level_%03d", (pItem->m_level & 0x7F) + 1));
			pName->AddText(MakeStr("\xB7\xB9\xBA\xA7:                \xC0\xCC\xC7\xCF", pItem->m_name), false, true);
		} else {
			pLevel->SetBgImg(MakeStr("level_%03d", (pItem->m_level & 0x7F) + 1));
			pName->AddText(MakeStr("\xB7\xB9\xBA\xA7:                \xC0\xCC\xBB\xF3", pItem->m_name), false, true);
		}
	}

	IFF_STRUCT::sDesc* pDesc = ItemManager()->FindDesc(pItem->m_typeId);

	if (pDesc) {
		pForm->SetMessage(pDesc->m_text, false);
	}

	pForm->Open(0, 3);
}

// bongdarishopdlg.cpp:3314
void FrTotalItemDlg::OnOkBnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:3319
void FrTotalItemDlg::OnOkBnUp(void) {
	OnFreshOkay();
}

// bongdarishopdlg.cpp:3324
bool FrTotalItemDlg::OnInit(void) {
	return true;
}

// bongdarishopdlg.cpp:3330
void FrTotalItemDlg::SetListItem(std::list<sBonusBall>* pList) {
	if (pList) {
		m_bonusBallList.assign(pList->begin(), pList->end());

		if (m_bonusBallList.size() <= 0) {
			SetDesc("\xC8\xB9\xB5\xE6\xC7\xCF\xBD\xC5 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xCC \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.");
		} else {
			SetDesc("\xC8\xB9\xB5\xE6\xC7\xCF\xBD\xC5 \xB8\xF0\xB5\xE7 \xBE\xC6\xC0\xCC\xC5\xDB \xB8\xF1\xB7\xCF\xC0\xD4\xB4\xCF\xB4\xD9.");

			for (std::list<sBonusBall>::iterator it = m_bonusBallList.begin(); it != m_bonusBallList.end(); ++it) {
				m_pTotalItemList->AddItem(&*it);
			}
		}
	}
}

// bongdarishopdlg.cpp:3358 (run-time type and factory registration of
// FrBongdarishopNoticeDlg)
const WRTTI FrBongdarishopNoticeDlg::m_RTTI("FrBongdarishopNoticeDlg", &FrForm::m_RTTI);

IObject* FrBongdarishopNoticeDlgMakeInstance(void) {
	return new FrBongdarishopNoticeDlg;
}

struct __sFrBongdarishopNoticeDlg {
	__sFrBongdarishopNoticeDlg(void) {
		ObjectFactory().AddObjectFunctor(FrBongdarishopNoticeDlgMakeInstance, "FrBongdarishopNoticeDlg");
	}
};
static __sFrBongdarishopNoticeDlg s_registerFrBongdarishopNoticeDlg;

// bongdarishopdlg.cpp:3360
const sFRESH_MSGMAP* FrBongdarishopNoticeDlg::GetMessageMap(void) const {
	return &FrBongdarishopNoticeDlg::_MsgMap;
}

sFRESH_MSGMAP FrBongdarishopNoticeDlg::_MsgMap = { &FrForm::_MsgMap, &FrBongdarishopNoticeDlg::_MsgEntries[0] };

sFRESH_ENTRY FrBongdarishopNoticeDlg::_MsgEntries[] = {
	// bongdarishopdlg.cpp:3362
	{ "bongdarishop_notice", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopNoticeDlg::OnBongdarishopNoticeInit },
	// bongdarishopdlg.cpp:3363
	{ "noticeviewer", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopNoticeDlg::OnNoticeViewerInit },
	// bongdarishopdlg.cpp:3364
	{ "noticeviewer", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdarishopNoticeDlg::OnNoticeViewerBnUp },
	// bongdarishopdlg.cpp:3365
	{ "listindex", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopNoticeDlg::OnListIndexInit },
	// bongdarishopdlg.cpp:3366
	{ "listindex", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdarishopNoticeDlg::OnListIndexLBnUp },
	// bongdarishopdlg.cpp:3367
	{ "listindex", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopNoticeDlg::OnListIndexOwnerDraw },
	// bongdarishopdlg.cpp:3368
	{ "backward", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopNoticeDlg::OnBackwardBnInit },
	// bongdarishopdlg.cpp:3369
	{ "backward", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdarishopNoticeDlg::OnBackwardBnUp },
	// bongdarishopdlg.cpp:3370
	{ "forward", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopNoticeDlg::OnForwardBnInit },
	// bongdarishopdlg.cpp:3371
	{ "forward", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdarishopNoticeDlg::OnForwardBnUp },
	// bongdarishopdlg.cpp:3372
	{ "in", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopNoticeDlg::OnInBnInit },
	// bongdarishopdlg.cpp:3373
	{ "in", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdarishopNoticeDlg::OnInBnUp },
	// bongdarishopdlg.cpp:3374
	{ "out", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopNoticeDlg::OnOutBnInit },
	// bongdarishopdlg.cpp:3375
	{ "out", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdarishopNoticeDlg::OnOutBnUp },
	// bongdarishopdlg.cpp:3376
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopNoticeDlg::OnOkBnInit },
	// bongdarishopdlg.cpp:3377
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdarishopNoticeDlg::OnOkBnUp },

	// bongdarishopdlg.cpp:3379
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// bongdarishopdlg.cpp:3382
FrBongdarishopNoticeDlg::FrBongdarishopNoticeDlg(void) {
	m_pListIndex = 0;
	m_curIndex = 0;
	m_moveLayoutTid = 0;
	m_pBackwardBn = 0;
	m_pForwardBn = 0;
	m_pNoticeViewer = 0;
	m_pInBn = 0;
	m_pOutBn = 0;
	m_pOkBn = 0;
}

// bongdarishopdlg.cpp:3395
FrBongdarishopNoticeDlg::~FrBongdarishopNoticeDlg(void) {
}

// bongdarishopdlg.cpp:3399
void FrBongdarishopNoticeDlg::OnBongdarishopNoticeInit(int) {
}

// bongdarishopdlg.cpp:3403
void FrBongdarishopNoticeDlg::OnNoticeViewerInit(intptr_t param) {
	m_pNoticeViewer = DYNAMIC_CAST(FrViewer, param);
}

// bongdarishopdlg.cpp:3434
void FrBongdarishopNoticeDlg::OnListIndexInit(intptr_t param) {
	m_pListIndex = DYNAMIC_CAST(FrListBox, param);
}

// bongdarishopdlg.cpp:3408
void FrBongdarishopNoticeDlg::OnNoticeViewerBnUp(void) {
	if (m_curIndex >= 0) {
		const WPoint& mousePos = g_pFresh->GetManager()->GetMousePos();
		WPoint mouse(mousePos.x, mousePos.y);

		std::list<sNoticeImage::sBn>::iterator it;
		for (it = m_noticeImages[m_curIndex].buttons.begin(); it != m_noticeImages[m_curIndex].buttons.end(); ++it) {
			WPoint pos = GetRect().TopLeft();
			WRect rect(pos.x + it->x, pos.y + it->y, it->w, it->h);

			if (mouse.x >= rect.x && mouse.x <= rect.x + rect.w && mouse.y >= rect.y && mouse.y <= rect.y + rect.h) {
				m_moveLayoutTid = it->index;
				Close(FrOK, true);
				return;
			}
		}

		Close(FrNONE, true);
	}
}

// bongdarishopdlg.cpp:3439
void FrBongdarishopNoticeDlg::OnListIndexLBnUp(void) {
	FrListItem* pItem = m_pListIndex->GetItemUnderCursor();

	if (pItem) {
		const char* text = (const char*)pItem->pData;

		if (text) {
			m_curIndex = atoi(text) - 1;
			SetImgNotice(m_curIndex);
		}
	}
}

// bongdarishopdlg.cpp:3453
void FrBongdarishopNoticeDlg::OnListIndexOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;

	if (pItem) {
		const char* text = (const char*)pItem->pData;

		if (text) {
			FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

			if (pGDI) {
				int index = atoi(text);

				if (m_curIndex == index - 1) {
					pGDI->SetTextColor(0xffff0000, 0xffffffff);
					pGDI->SetTextStyle(1);
				} else {
					pGDI->SetTextColor(0xff000000, 0xffffffff);
					pGDI->SetTextStyle(0);
				}

				pGDI->Print(pItem->pos, 0, text);
			}
		}
	}
}

// bongdarishopdlg.cpp:3483
void FrBongdarishopNoticeDlg::OnBackwardBnInit(intptr_t param) {
	m_pBackwardBn = DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:3488
void FrBongdarishopNoticeDlg::OnBackwardBnUp(void) {
	SetImgNotice(--m_curIndex);
	m_pBackwardBn->SetPushDelay(0.0f);
}

// bongdarishopdlg.cpp:3494
void FrBongdarishopNoticeDlg::OnForwardBnInit(intptr_t param) {
	m_pForwardBn = DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:3499
void FrBongdarishopNoticeDlg::OnForwardBnUp(void) {
	SetImgNotice(++m_curIndex);
	m_pForwardBn->SetPushDelay(0.0f);
}

// bongdarishopdlg.cpp:3505
void FrBongdarishopNoticeDlg::OnInBnInit(intptr_t param) {
	m_pInBn = DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:3510
void FrBongdarishopNoticeDlg::OnInBnUp(void) {
	OnFreshOkay();
}

// bongdarishopdlg.cpp:3515
void FrBongdarishopNoticeDlg::OnOutBnInit(intptr_t param) {
	m_pOutBn = DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:3520
void FrBongdarishopNoticeDlg::OnOutBnUp(void) {
	FrBongdariShopDlg* pShop = FrStaticCast<FrBongdariShopDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdari_shop"));

	if (pShop) {
		pShop->Close(FrCANCEL, true);
	}

	OnFreshCancel();
}

// bongdarishopdlg.cpp:3534
void FrBongdarishopNoticeDlg::OnOkBnInit(intptr_t param) {
	m_pOkBn = DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:3539
void FrBongdarishopNoticeDlg::OnOkBnUp(void) {
	OnFreshOkay();
}

// bongdarishopdlg.cpp:3544
bool FrBongdarishopNoticeDlg::OnInit(void) {
	LoadInitFile("bs_ppl.ini");
	SetImgNotice(0);

	return true;
}

// bongdarishopdlg.cpp:3553
void FrBongdarishopNoticeDlg::LoadInitFile(const char* fileName) {
	LoadNoticeImageFile(fileName, m_noticeImages, 5, 0);

	if (m_pBackwardBn) {
		float y = m_pBackwardBn->GetRect().y;
		m_pBackwardBn->MoveWindow(WPoint(GetRect().x + GetRect().w * 0.5f - (m_noticeImages.size() * 7.5f + 20.0f), y));
	}

	if (m_pForwardBn) {
		float y = m_pForwardBn->GetRect().y;
		m_pForwardBn->MoveWindow(WPoint(GetRect().x + (GetRect().w * 0.5f + m_noticeImages.size() * 7.5f), y));
	}

	if (m_noticeImages.size() > 0 && m_noticeImages.size() <= 5) {
		for (unsigned char i = 0; i < m_noticeImages.size(); i++) {
			m_pListIndex->AddItem(g_strIndex[i]);
		}

		float x = m_pListIndex->GetRect().x, y = m_pListIndex->GetRect().y;
		m_pListIndex->MoveWindow(WPoint(x + (5 - m_noticeImages.size()) * 7.5f, y));
	}
}

// bongdarishopdlg.cpp:3595
void FrBongdarishopNoticeDlg::SetButtonType(bool b) {
	if (m_pInBn) m_pInBn->SetVisible(b);
	if (m_pOutBn) m_pOutBn->SetVisible(b);
	if (m_pOkBn) m_pOkBn->SetVisible(!b);
}

// bongdarishopdlg.cpp:3603
void FrBongdarishopNoticeDlg::SetImgNotice(int index) {
	int count = m_noticeImages.size();

	if (count > 0) {
		if (m_pBackwardBn) {
			m_pBackwardBn->Enable(true);
		}

		if (m_pForwardBn) {
			m_pForwardBn->Enable(true);
		}

		if (index <= 0) {
			index = 0;
			if (m_pBackwardBn) {
				m_pBackwardBn->Enable(false);
			}
		}

		if (index >= count - 1) {
			index = count - 1;
			if (m_pForwardBn) {
				m_pForwardBn->Enable(false);
			}
		}

		m_curIndex = index;
		if (m_pNoticeViewer) {
			m_pNoticeViewer->Open(MakeStr(m_noticeImages[index].name));
		}
	}
}

// bongdarishopdlg.cpp:3652
void FrBongdarishopNoticeDlg::PrevIndex(void) {
	OnBackwardBnUp();
}

// bongdarishopdlg.cpp:3658
void FrBongdarishopNoticeDlg::NextIndex(void) {
	OnForwardBnUp();
}

// bongdarishopdlg.cpp:3663 (run-time type and factory registration of
// FrBongdarishopRareItemDlg)
const WRTTI FrBongdarishopRareItemDlg::m_RTTI("FrBongdarishopRareItemDlg", &FrForm::m_RTTI);

IObject* FrBongdarishopRareItemDlgMakeInstance(void) {
	return new FrBongdarishopRareItemDlg;
}

struct __sFrBongdarishopRareItemDlg {
	__sFrBongdarishopRareItemDlg(void) {
		ObjectFactory().AddObjectFunctor(FrBongdarishopRareItemDlgMakeInstance, "FrBongdarishopRareItemDlg");
	}
};
static __sFrBongdarishopRareItemDlg s_registerFrBongdarishopRareItemDlg;

// bongdarishopdlg.cpp:3665
const sFRESH_MSGMAP* FrBongdarishopRareItemDlg::GetMessageMap(void) const {
	return &FrBongdarishopRareItemDlg::_MsgMap;
}

sFRESH_MSGMAP FrBongdarishopRareItemDlg::_MsgMap = { &FrForm::_MsgMap, &FrBongdarishopRareItemDlg::_MsgEntries[0] };

sFRESH_ENTRY FrBongdarishopRareItemDlg::_MsgEntries[] = {
	// bongdarishopdlg.cpp:3667
	{ "bongdarishop_rareitem", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopRareItemDlg::OnBongdarishopRareItemInit },
	// bongdarishopdlg.cpp:3668
	{ "rareitemicon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopRareItemDlg::OnRareItemIconInit },
	// bongdarishopdlg.cpp:3669
	{ "rareitemname", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopRareItemDlg::OnRareItemNameInit },
	// bongdarishopdlg.cpp:3670
	{ "rareitemability", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopRareItemDlg::OnRareItemAbilityInit },
	// bongdarishopdlg.cpp:3671
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopRareItemDlg::OnOkBnInit },
	// bongdarishopdlg.cpp:3672
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdarishopRareItemDlg::OnOkBnUp },

	// bongdarishopdlg.cpp:3674
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// bongdarishopdlg.cpp:3677
FrBongdarishopRareItemDlg::FrBongdarishopRareItemDlg(void) {
	m_pRareItemIcon = 0;
	m_pRareItemName = 0;
	m_pRareItemAbility = 0;
}

// bongdarishopdlg.cpp:3684
FrBongdarishopRareItemDlg::~FrBongdarishopRareItemDlg(void) {
}

// bongdarishopdlg.cpp:3688
void FrBongdarishopRareItemDlg::OnBongdarishopRareItemInit(int) {
}

// bongdarishopdlg.cpp:3692
void FrBongdarishopRareItemDlg::OnRareItemIconInit(intptr_t param) {
	m_pRareItemIcon = DYNAMIC_CAST(FrArea, param);
}

// bongdarishopdlg.cpp:3697
void FrBongdarishopRareItemDlg::OnRareItemNameInit(intptr_t param) {
	m_pRareItemName = DYNAMIC_CAST(FrEdit, param);
}

// bongdarishopdlg.cpp:3702
void FrBongdarishopRareItemDlg::OnRareItemAbilityInit(intptr_t param) {
	m_pRareItemAbility = DYNAMIC_CAST(FrEdit, param);
}

// bongdarishopdlg.cpp:3707
void FrBongdarishopRareItemDlg::OnOkBnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:3712
void FrBongdarishopRareItemDlg::OnOkBnUp(void) {
	OnFreshOkay();
}

// bongdarishopdlg.cpp:3717
bool FrBongdarishopRareItemDlg::OnInit(void) {
	return true;
}

// Ten bytes of stat values (five shorts) copied as one unit.
struct sStatBonus5 {
	short m_value[5];
};

// bongdarishopdlg.cpp:3723
void FrBongdarishopRareItemDlg::SetRareItem(sBonusBall* pBall) {
	if (!pBall)
		return;

	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pBall->m_typeId);

	if (!pCommon)
		return;

	if (m_pRareItemIcon) {
		const Bitmap* pBitmap;

		switch (pBall->m_typeId >> 26) {
		case 2:
			pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS_FASHION", pCommon->m_icon);
			break;
		default:
			pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS", pCommon->m_icon);
			break;
		}

		m_pRareItemIcon->SetBgImg(pBitmap);
	}

	if (m_pRareItemName) {
		m_pRareItemName->AddText(MakeStr("\\cT\\c%s\\cN\\c", pCommon->m_name), false, true);
	}

	sStatBonus5 base;
	sStatBonus5 slot;
	memset(&slot, 0, sizeof(slot));

	switch (pBall->m_typeId >> 26) {
	case 2: {
		IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(pBall->m_typeId);

		if (!pPart)
			return;

		base = *(const sStatBonus5*)pPart->m_baseStat;
		slot = *(const sStatBonus5*)pPart->m_slotStat;
		break;
	}
	case 7: {
		IFF_STRUCT::sCaddie* pCaddie = ItemManager()->FindCaddie(pBall->m_typeId);

		if (!pCaddie)
			return;

		base = *(const sStatBonus5*)pCaddie->m_levels;
		break;
	}
	case 8: {
		IFF_STRUCT::sCadItem* pCadItem = ItemManager()->FindCadItem(pBall->m_typeId);

		if (!pCadItem)
			return;

		base = *(const sStatBonus5*)pCadItem->m_levels;
		break;
	}
	default:
		return;
	}

	const char* statName = "";

	for (int i = 0; i < 5; i++) {
		if (base.m_value[i] == 0 && slot.m_value[i] == 0)
			continue;

		switch (i) {
		case 0:
			statName = "\xC6\xC4\xBF\xF6";
			break;
		case 1:
			statName = "\xC4\xC1\xC6\xAE\xB7\xD1";
			break;
		case 2:
			statName = "\xC1\xA4\xC8\xAE\xB5\xB5";
			break;
		case 3:
			statName = "\xBD\xBA\xC7\xC9";
			break;
		case 4:
			statName = "\xC4\xBF\xBA\xEA";
			break;
		}

		if (base.m_value[i] <= 0 && slot.m_value[i] <= 0)
			continue;

		char* text;

		if (base.m_value[i] == 0)
			text = MakeStr("\\c0xff0000ff\\c%s\xBD\xBD\xB7\xD4%+d\\c0xff000000\\c", statName, slot.m_value[i]);
		else if (slot.m_value[i] == 0)
			text = MakeStr("\\c0xff0000ff\\c%s%+d\\c0xff000000\\c", statName, base.m_value[i]);
		else
			text = MakeStr("\\c0xff0000ff\\c%s%+d/\xBD\xBD\xB7\xD4%+d\\c0xff000000\\c", statName, base.m_value[i], slot.m_value[i]);

		m_pRareItemAbility->AddText(text, false, true);
	}
}

// bongdarishopdlg.cpp:3863 (run-time type and factory registration of
// FrBongdarishopWarningDlg)
const WRTTI FrBongdarishopWarningDlg::m_RTTI("FrBongdarishopWarningDlg", &FrForm::m_RTTI);

IObject* FrBongdarishopWarningDlgMakeInstance(void) {
	return new FrBongdarishopWarningDlg;
}

struct __sFrBongdarishopWarningDlg {
	__sFrBongdarishopWarningDlg(void) {
		ObjectFactory().AddObjectFunctor(FrBongdarishopWarningDlgMakeInstance, "FrBongdarishopWarningDlg");
	}
};
static __sFrBongdarishopWarningDlg s_registerFrBongdarishopWarningDlg;

// bongdarishopdlg.cpp:3865
const sFRESH_MSGMAP* FrBongdarishopWarningDlg::GetMessageMap(void) const {
	return &FrBongdarishopWarningDlg::_MsgMap;
}

sFRESH_MSGMAP FrBongdarishopWarningDlg::_MsgMap = { &FrForm::_MsgMap, &FrBongdarishopWarningDlg::_MsgEntries[0] };

sFRESH_ENTRY FrBongdarishopWarningDlg::_MsgEntries[] = {
	// bongdarishopdlg.cpp:3867
	{ "bongdarishop_warning", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopWarningDlg::OnBongdarishopWarningInit },
	// bongdarishopdlg.cpp:3868
	{ "warning_back", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopWarningDlg::OnWarningBackInit },
	// bongdarishopdlg.cpp:3869
	{ "warning_back", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopWarningDlg::OnWarningBackOwnerDraw },
	// bongdarishopdlg.cpp:3870
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBongdarishopWarningDlg::OnOkBnInit },
	// bongdarishopdlg.cpp:3871
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrBongdarishopWarningDlg::OnOkBnUp },

	// bongdarishopdlg.cpp:3873
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// bongdarishopdlg.cpp:3876
FrBongdarishopWarningDlg::FrBongdarishopWarningDlg(void) {
	m_pWarningBack = 0;
	m_bonusPrice = 0;
	m_backType = 0;

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = &data_a97c78[910];
	info.fontw = 9;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 32;
	info.pCharSet = "1234567890,-";

	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);
}

// bongdarishopdlg.cpp:3899
FrBongdarishopWarningDlg::~FrBongdarishopWarningDlg(void) {
	if (m_pTitleFont) {
		m_pTitleFont->Erase();
	}

	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// bongdarishopdlg.cpp:3909
void FrBongdarishopWarningDlg::OnBongdarishopWarningInit(int) {
}

// bongdarishopdlg.cpp:3913
void FrBongdarishopWarningDlg::OnWarningBackInit(intptr_t param) {
	m_pWarningBack = DYNAMIC_CAST(FrArea, param);
}

// bongdarishopdlg.cpp:3918
void FrBongdarishopWarningDlg::OnOkBnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:3923
void FrBongdarishopWarningDlg::OnOkBnUp(void) {
	FrBongdariShopDlg* pShop = FrStaticCast<FrBongdariShopDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdari_shop"));

	if (pShop) {
		pShop->Close(FrCANCEL, true);
	}

	OnFreshOkay();
}

// bongdarishopdlg.cpp:3937
bool FrBongdarishopWarningDlg::OnInit(void) {
	return true;
}

// bongdarishopdlg.cpp:3943
void FrBongdarishopWarningDlg::SetImgWarningBack(int type, int price) {
	if (m_pWarningBack) {
		if (type == 0) {
			m_pWarningBack->SetBgImg(g_pFresh->GetBitmap("warning_back"));
		} else if (type == 1) {
			m_pWarningBack->SetBgImg(g_pFresh->GetBitmap("warning_back_2"));
		} else if (type == 2) {
			m_bonusPrice = price;
			m_backType = 2;
			m_pWarningBack->SetBgImg(g_pFresh->GetBitmap("warning_back_bonus"));
		}
	}
}

// bongdarishopdlg.cpp:3964
void FrBongdarishopWarningDlg::OnWarningBackOwnerDraw(int) {
	if (m_backType == 2) {
		WRect rc;
		rc = GetRect();

		char buf[32] = { 0 };
		MakePriceStrA(buf, m_bonusPrice);

		m_pTitleFont->Print(g_view, rc.x + 170.0f, rc.y + 50.0f, buf, 0, 0xffffffff, 0);
	}
}

// bongdarishopdlg.cpp:3978 (run-time type and factory registration of
// FrClosedBonusBallResultDlg)
const WRTTI FrClosedBonusBallResultDlg::m_RTTI("FrClosedBonusBallResultDlg", &FrForm::m_RTTI);

IObject* FrClosedBonusBallResultDlgMakeInstance(void) {
	return new FrClosedBonusBallResultDlg;
}

struct __sFrClosedBonusBallResultDlg {
	__sFrClosedBonusBallResultDlg(void) {
		ObjectFactory().AddObjectFunctor(FrClosedBonusBallResultDlgMakeInstance, "FrClosedBonusBallResultDlg");
	}
};
static __sFrClosedBonusBallResultDlg s_registerFrClosedBonusBallResultDlg;

// bongdarishopdlg.cpp:3980
const sFRESH_MSGMAP* FrClosedBonusBallResultDlg::GetMessageMap(void) const {
	return &FrClosedBonusBallResultDlg::_MsgMap;
}

sFRESH_MSGMAP FrClosedBonusBallResultDlg::_MsgMap = { &FrForm::_MsgMap, &FrClosedBonusBallResultDlg::_MsgEntries[0] };

sFRESH_ENTRY FrClosedBonusBallResultDlg::_MsgEntries[] = {
	// bongdarishopdlg.cpp:3982
	{ "closed_bonusball", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrClosedBonusBallResultDlg::OnClosedBonusBallResultInit },
	// bongdarishopdlg.cpp:3983
	{ "closedbonusballlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrClosedBonusBallResultDlg::OnClosedBonusBallListInit },
	// bongdarishopdlg.cpp:3984
	{ "closedbonusballlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrClosedBonusBallResultDlg::OnClosedBonusBallListOwnerDraw },
	// bongdarishopdlg.cpp:3985
	{ "closedbonusballname01", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrClosedBonusBallResultDlg::OnClosedBonusBallName01Init },
	// bongdarishopdlg.cpp:3986
	{ "closedbonusballability01", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrClosedBonusBallResultDlg::OnClosedBonusBallAbility01Init },
	// bongdarishopdlg.cpp:3987
	{ "closedbonusballname02", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrClosedBonusBallResultDlg::OnClosedBonusBallName02Init },
	// bongdarishopdlg.cpp:3988
	{ "closedbonusballability02", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrClosedBonusBallResultDlg::OnClosedBonusBallAbility02Init },
	// bongdarishopdlg.cpp:3989
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrClosedBonusBallResultDlg::OnOkBnInit },
	// bongdarishopdlg.cpp:3990
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrClosedBonusBallResultDlg::OnOkBnUp },

	// bongdarishopdlg.cpp:3992
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// bongdarishopdlg.cpp:3995
FrClosedBonusBallResultDlg::FrClosedBonusBallResultDlg(void) {
	m_pClosedBonusBallList = 0;
	m_pNameEdit[0] = 0;
	m_pNameEdit[1] = 0;
	m_pAbilityEdit[0] = 0;
	m_pAbilityEdit[1] = 0;

	m_selected = -1;
	memset(m_closedBalls, 0, sizeof(m_closedBalls));
}

// bongdarishopdlg.cpp:4008
FrClosedBonusBallResultDlg::~FrClosedBonusBallResultDlg(void) {
}

// bongdarishopdlg.cpp:4012
void FrClosedBonusBallResultDlg::OnClosedBonusBallResultInit(int) {
}

// bongdarishopdlg.cpp:4016
void FrClosedBonusBallResultDlg::OnClosedBonusBallListInit(intptr_t param) {
	m_pClosedBonusBallList = DYNAMIC_CAST(FrListBox, param);

	m_clockBtn.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "clock");
	m_clockBtn.rect = WRect(50.0f, 35.0f, (float)m_clockBtn.pBitmap->Width(), (float)m_clockBtn.pBitmap->Height());
	m_deleteBtn.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "delete");
	m_deleteBtn.rect = WRect(50.0f, 35.0f, (float)m_deleteBtn.pBitmap->Width(), (float)m_deleteBtn.pBitmap->Height());
}

// bongdarishopdlg.cpp:4026
void FrClosedBonusBallResultDlg::OnClosedBonusBallListOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (!pGDI)
		return;

	FrListItem* pItem = (FrListItem*)param;

	if (!pItem)
		return;

	sBonusBall* pBall = (sBonusBall*)pItem->pData;

	if (!pBall->m_typeId)
		return;

	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pBall->m_typeId);

	if (!pCommon)
		return;

	float offsetX = 0.0f;
	float offsetY = 0.0f;
	const Bitmap* pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS", pCommon->m_icon);

	if (!pBitmap) {
		pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS_FASHION", pCommon->m_icon);
		offsetX = -12.0f;
		offsetY = -20.0f;

		if (!pBitmap)
			return;
	}

	WRect rc(offsetX + pItem->pos.x + 18.0f, offsetY + pItem->pos.y + 11.0f, (float)pBitmap->Width(), (float)pBitmap->Height());

	pGDI->DrawTexture(pBitmap, rc, 0xffffffff, 0);
	pGDI->SetTextColor(0xff000000, 0xffffffff);
	pGDI->SetTextStyle(4);

	if (pBall->m_bUnitCount) {
		if (pBall->m_unitCount > 0) {
			pGDI->DrawTexture(m_clockBtn.pBitmap, m_clockBtn.rect + pItem->pos, 0xffffffff, 0);
		}
	}
}

// bongdarishopdlg.cpp:4078
void FrClosedBonusBallResultDlg::OnClosedBonusBallName01Init(intptr_t param) {
	m_pNameEdit[0] = DYNAMIC_CAST(FrEdit, param);
}

// bongdarishopdlg.cpp:4083
void FrClosedBonusBallResultDlg::OnClosedBonusBallAbility01Init(intptr_t param) {
	m_pAbilityEdit[0] = DYNAMIC_CAST(FrEdit, param);
}

// bongdarishopdlg.cpp:4088
void FrClosedBonusBallResultDlg::OnClosedBonusBallName02Init(intptr_t param) {
	m_pNameEdit[1] = DYNAMIC_CAST(FrEdit, param);
}

// bongdarishopdlg.cpp:4093
void FrClosedBonusBallResultDlg::OnClosedBonusBallAbility02Init(intptr_t param) {
	m_pAbilityEdit[1] = DYNAMIC_CAST(FrEdit, param);
}

// bongdarishopdlg.cpp:4098
void FrClosedBonusBallResultDlg::OnOkBnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// bongdarishopdlg.cpp:4103
void FrClosedBonusBallResultDlg::OnOkBnUp(void) {
	OnFreshOkay();
}

// bongdarishopdlg.cpp:4108
bool FrClosedBonusBallResultDlg::OnInit(void) {
	return true;
}

// bongdarishopdlg.cpp:4113
void FrClosedBonusBallResultDlg::OnProc(const float dt) {
	if (m_pClosedBonusBallList) {
		FrScrollBar* pScrollBar = m_pClosedBonusBallList->GetScrollBar();

		if (pScrollBar) {
			int top = (int)(pScrollBar->GetCurTopRow() + 0.5f);

			if (m_selected != top) {
				m_selected = top;
				SetEditDataClosedBonusBall(0, top);

				if (m_pClosedBonusBallList->m_itemList.size() > 1) {
					SetEditDataClosedBonusBall(1, m_selected + 1);
				}
			}
		}
	}
}

// bongdarishopdlg.cpp:4135
void FrClosedBonusBallResultDlg::SetClosedBonusBall(sBonusBall* pBalls) {
	if (pBalls) {
		bool bFound = false;

		for (int i = 0; i < 5; i++) {
			if (pBalls[i].m_typeId != 0 && pBalls[i].m_index >= 0) {
				bFound = true;
				break;
			}
		}

		if (!bFound) {
			SetDesc("\xC8\xB9\xB5\xE6\xC7\xD1 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xCC \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.");
			OnFreshOkay();
			return;
		}

		SetDesc("\xC8\xB9\xB5\xE6 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBA \xB8\xB6\xC0\xCC\xB7\xEB\xBF\xA1\xBC\xAD \xC8\xAE\xC0\xCE\xC7\xCF\xBC\xBC\xBF\xE4.");

		memcpy(m_closedBalls, pBalls, sizeof(m_closedBalls));

		for (int j = 0; j < 5; j++) {
			if (m_closedBalls[j].m_typeId != 0 && m_closedBalls[j].m_index >= 0)
				m_pClosedBonusBallList->AddItem(&m_closedBalls[j]);
		}
	}
}

// bongdarishopdlg.cpp:4173
void FrClosedBonusBallResultDlg::SetEditDataClosedBonusBall(int editIdx, int listIdx) {
	if (!m_pClosedBonusBallList)
		return;

	FrListItem* pListItem = 0;
	int n = 0;

	for (FrListBox::ITEM_LIST::iterator it = m_pClosedBonusBallList->m_itemList.begin(); it != m_pClosedBonusBallList->m_itemList.end(); ++it, ++n) {
		if (n == listIdx) {
			pListItem = *it;
			break;
		}
	}

	if (!pListItem)
		return;

	sBonusBall* pBall = (sBonusBall*)pListItem->pData;

	if (!pBall)
		return;

	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pBall->m_typeId);

	if (!pCommon)
		return;

	if (!m_pNameEdit[editIdx] || !m_pAbilityEdit[editIdx])
		return;

	m_pNameEdit[editIdx]->ClearLine();
	m_pAbilityEdit[editIdx]->ClearLine();

	m_pNameEdit[editIdx]->AddText(MakeStr("%s", pCommon->m_name), false, true);

	sStatBonus5 base;
	sStatBonus5 slot;
	memset(&slot, 0, sizeof(slot));
	bool bFound = false;

	switch (pBall->m_typeId >> 26) {
	case 2: {
		IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(pBall->m_typeId);

		if (pPart) {
			base = *(const sStatBonus5*)pPart->m_baseStat;
			slot = *(const sStatBonus5*)pPart->m_slotStat;
			bFound = true;
		}
		break;
	}
	case 7: {
		IFF_STRUCT::sCaddie* pCaddie = ItemManager()->FindCaddie(pBall->m_typeId);

		if (pCaddie) {
			base = *(const sStatBonus5*)pCaddie->m_levels;
			bFound = true;
		}
		break;
	}
	case 8: {
		IFF_STRUCT::sCadItem* pCadItem = ItemManager()->FindCadItem(pBall->m_typeId);

		if (pCadItem) {
			base = *(const sStatBonus5*)pCadItem->m_levels;
			bFound = true;
		}
		break;
	}
	}

	if (!bFound) {
		m_pAbilityEdit[editIdx]->AddText(MakeStr("%d\xB0\xB3", pBall->m_count), false, true);
		return;
	}

	const char* statName = "";

	for (int i = 0; i < 5; i++) {
		if (base.m_value[i] == 0 && slot.m_value[i] == 0)
			continue;

		switch (i) {
		case 0:
			statName = "\xC6\xC4\xBF\xF6";
			break;
		case 1:
			statName = "\xC4\xC1\xC6\xAE\xB7\xD1";
			break;
		case 2:
			statName = "\xC1\xA4\xC8\xAE\xB5\xB5";
			break;
		case 3:
			statName = "\xBD\xBA\xC7\xC9";
			break;
		case 4:
			statName = "\xC4\xBF\xBA\xEA";
			break;
		}

		if (base.m_value[i] <= 0 && slot.m_value[i] <= 0)
			continue;

		char* text;

		if (base.m_value[i] == 0)
			text = MakeStr("\\c0xff0000ff\\c%s\xBD\xBD\xB7\xD4%+d\\c0xff000000\\c", statName, slot.m_value[i]);
		else if (slot.m_value[i] == 0)
			text = MakeStr("\\c0xff0000ff\\c%s%+d\\c0xff000000\\c", statName, base.m_value[i]);
		else
			text = MakeStr("\\c0xff0000ff\\c%s%+d/\xBD\xBD\xB7\xD4%+d\\c0xff000000\\c", statName, base.m_value[i], slot.m_value[i]);

		m_pAbilityEdit[editIdx]->AddText(text, false, true);
	}
}
