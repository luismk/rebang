// Original: d:\build\custom\temp\code\client\projectg\scoredlg.cpp
#include <string.h>
#include "ScoreDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "frlistbox.h"
#include "SharedDoc.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "commonutil.h"
#include "wview.h"
#include "bitmap.h"
#include "NetResourceManager.h"
#include "ProjectG.h"

// scoredlg.cpp:27
void ResizeScoreBar23(FrArea* pBar2, FrArea* pBar3, float left, float right) {
	if (pBar2 && pBar3) {
		WRect rc;

		right -= 7.0f;
		rc = pBar3->GetRect();
		right -= rc.w;
		rc.x = left + right;
		pBar3->SetRect(rc);

		rc = pBar2->GetRect();
		rc.w = right - (rc.x - left);
		pBar2->SetRect(rc);
	}

}

// Minimal stubs for functions declared in the header but not yet implemented
// (present to satisfy linker; full implementations can be added later).
void FrScoreDlg::DrawGraph(FrListItem* pItem) {
	// stub: no-op drawing for graph
	(void)pItem;
}

void FrScoreDlg::OnTotalListOwnerDraw(int param) {
	// stub: owner-draw handler placeholder
	(void)param;
}
// scoredlg.cpp:48 (run-time type and factory registration of FrScoreDlg)
const WRTTI FrScoreDlg::m_RTTI("FrScoreDlg", &FrForm::m_RTTI);

IObject* FrScoreDlgMakeInstance(void) {
	return new FrScoreDlg;
}

struct __sFrScoreDlg {
	__sFrScoreDlg(void) {
		ObjectFactory().AddObjectFunctor(FrScoreDlgMakeInstance, "FrScoreDlg");
	}
};
static __sFrScoreDlg s_registerFrScoreDlg;

// scoredlg.cpp:50
const sFRESH_MSGMAP* FrScoreDlg::GetMessageMap(void) const {
	return &FrScoreDlg::_MsgMap;
}

sFRESH_MSGMAP FrScoreDlg::_MsgMap = { &FrForm::_MsgMap, &FrScoreDlg::_MsgEntries[0] };

sFRESH_ENTRY FrScoreDlg::_MsgEntries[] = {
	// scoredlg.cpp:52
	{ "front9btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnFront9BtnInit },
	// scoredlg.cpp:53
	{ "front9btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreDlg::OnFront9BtnUp },
	// scoredlg.cpp:54
	{ "back9btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnBack9BtnInit },
	// scoredlg.cpp:55
	{ "back9btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreDlg::OnBack9BtnUp },
	// scoredlg.cpp:56
	{ "shotbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnShotBtnInit },
	// scoredlg.cpp:57
	{ "shotbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreDlg::OnShotBtnUp },
	// scoredlg.cpp:58
	{ "graphbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnGraphBtnInit },
	// scoredlg.cpp:59
	{ "graphbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreDlg::OnGraphBtnUp },
	// scoredlg.cpp:60
	{ "pangbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnPangBtnInit },
	// scoredlg.cpp:61
	{ "pangbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreDlg::OnPangBtnUp },
	// scoredlg.cpp:62
	{ "versusbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnVersusBtnInit },
	// scoredlg.cpp:63
	{ "versusbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreDlg::OnVersusBtnUp },
	// scoredlg.cpp:64
	{ "puttbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnPuttBtnInit },
	// scoredlg.cpp:65
	{ "hole_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnHoleListInit },
	// scoredlg.cpp:66
	{ "hole_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnHoleListOwnerDraw },
	// scoredlg.cpp:67
	{ "par_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnParListInit },
	// scoredlg.cpp:68
	{ "par_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnParListOwnerDraw },
	// scoredlg.cpp:69
	{ "golfer_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnGolferListInit },
	// scoredlg.cpp:70
	{ "golfer_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnGolferListOwnerDraw },
	// scoredlg.cpp:71
	{ "record_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnRecordListInit },
	// scoredlg.cpp:72
	{ "record_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnRecordListOwnerDraw },
	// scoredlg.cpp:73
	{ "total_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnTotalListInit },
	// scoredlg.cpp:74
	{ "total_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnTotalListOwnerDraw },
	// scoredlg.cpp:75
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnCancelBtnInit },
	// scoredlg.cpp:76
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreDlg::OnCancelBtnUp },
	// scoredlg.cpp:77
	{ "close", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnCloseBtnInit },
	// scoredlg.cpp:78
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreDlg::OnCloseBtnUp },

	// scoredlg.cpp:81
	{ "blankpanel1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnBlankPanel01Init },
	// scoredlg.cpp:82
	{ "blankpanel1", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnBlankPanel01OwnerDraw },
	// scoredlg.cpp:83
	{ "blankpanel2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnBlankPanel02Init },
	// scoredlg.cpp:84
	{ "blankpanel2", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnBlankPanel02OwnerDraw },
	// scoredlg.cpp:85
	{ "blankpanel3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnBlankPanel03Init },
	// scoredlg.cpp:86
	{ "blankpanel3", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreDlg::OnBlankPanel03OwnerDraw },

	// scoredlg.cpp:89
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// scoredlg.cpp:94
FrScoreDlg::FrScoreDlg(void) {
	m_bFront9 = true;








	m_pBar2 = 0;
	m_pBar3 = 0;
	m_pFront9Btn = 0;
	m_pBack9Btn = 0;
	m_values120[0] = 0;
	m_values120[1] = 0;
	m_values120[2] = 0;
	m_values120[3] = 0;
	m_pCancelBtn = 0;
	m_pCloseBtn = 0;
	m_type = WSingleton<CSharedDoc>::Instance()->m_roomType == 6 ? 3 : 0;


	m_time = 0;
	m_pPoint = 0;
	m_pGuildRival = 0;

	memset(m_pTypeBtn, 0, sizeof(m_pTypeBtn));
	memset(m_pList, 0, sizeof(m_pList));
	memset(m_pDigit, 0, sizeof(m_pDigit));
	memset(m_guildPlayerUID, 0, sizeof(m_guildPlayerUID));

	LoadBitmaps();

	m_bShowBlankPanel = WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomMode == 14;
	m_pBlankPanel[0] = 0;
	m_pBlankPanel[1] = 0;
	m_pBlankPanel[2] = 0;
}

// scoredlg.cpp:132
void FrScoreDlg::SetPlayer(unsigned long uid) {
	if (WSingleton<CSharedDoc>::Instance()->GetIndex(uid) == 0xFF)
		return;



	sRivalData* pData = &WSingleton<CSharedDoc>::Instance()->m_gameResultList[WSingleton<CSharedDoc>::Instance()->GetIndex(uid)];

	for (int i = 0; i < 5; i++) {
		if (m_pList[i]) {


			m_pList[i]->ClearItem();
			m_pList[i]->AddItem(pData);
		}
	}

	m_bFront9 = WSingleton<CSharedDoc>::Instance()->m_holes <= 9;
	SetFrontBackBtn();

	Resize();
}

// scoredlg.cpp:154
void FrScoreDlg::SetVisibleCloseBtn(bool bVisible) {
	if (m_pCloseBtn) {
		m_pCloseBtn->SetVisible(bVisible);
	}
}

// scoredlg.cpp:160
void FrScoreDlg::SetVisibleCancelBtn(bool bVisible) {
	if (m_pCancelBtn) {
		m_pCancelBtn->SetVisible(bVisible);
	}
}

// scoredlg.cpp:247
void FrScoreDlg::Resize(void) {
	WRect rc = GetRect();


	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {

		rc.h = 255.0f;
	}
	else {

		rc.h = 197.0f;
	}


	ResizeScoreBar23(m_pBar2, m_pBar3, rc.x, rc.w);
	SetRect(rc);
}

// scoredlg.cpp:266
void FrScoreDlg::LoadBitmaps(void) {

	for (int i = 0; i < 10; i++) {
		m_pDigit[i] = g_pFresh->GetManager()->GetBitmap("SCORECARD", MakeStr("%d", i));
	}
	m_pPlus = g_pFresh->GetManager()->GetBitmap("SCORECARD", "+");
	m_pMinus = g_pFresh->GetManager()->GetBitmap("SCORECARD", "-");


	m_pPoint = g_pFresh->GetManager()->GetBitmap("SCORECARD", "point");


	m_pWin = g_pFresh->GetManager()->GetBitmap("SCORECARD", "win");
	m_pLose = g_pFresh->GetManager()->GetBitmap("SCORECARD", "lose");
	m_pDraw = g_pFresh->GetManager()->GetBitmap("SCORECARD", "draw");
	m_pVersusMinus = g_pFresh->GetManager()->GetBitmap("SCORECARD", "-");


	m_pMassIcon = g_pFresh->GetManager()->GetBitmap("golf", "pc_icon_mass");
}

// scoredlg.cpp:291
void FrScoreDlg::OnBar2Init(intptr_t param) {
	m_pBar2 = DYNAMIC_CAST(FrArea, param);
}

// scoredlg.cpp:296
void FrScoreDlg::OnBar3Init(intptr_t param) {
	m_pBar3 = DYNAMIC_CAST(FrArea, param);
}

// scoredlg.cpp:301
void FrScoreDlg::OnFront9BtnInit(intptr_t param) {
	m_pFront9Btn = DYNAMIC_CAST(FrButton, param);
	SetFrontBackBtn();
}

// scoredlg.cpp:307
void FrScoreDlg::OnFront9BtnUp(void) {

	m_bFront9 = true;
	SetFrontBackBtn();
}

// scoredlg.cpp:314
void FrScoreDlg::OnBack9BtnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_holes <= 9) {
		return;
	}



	m_bFront9 = false;

	SetFrontBackBtn();
}

// scoredlg.cpp:325
void FrScoreDlg::OnShotBtnUp(void) {
	if (m_type == 0) {
		return;
	}


	m_type = 0;
	SetTypeBtn();
}

// scoredlg.cpp:334
void FrScoreDlg::OnGraphBtnUp(void) {
	if (m_type == 1) {
		return;
	}


	m_type = 1;
	SetTypeBtn();
}

// scoredlg.cpp:343
void FrScoreDlg::OnPangBtnUp(void) {
	if (m_type == 2) {
		return;
	}


	m_type = 2;
	SetTypeBtn();
}

// scoredlg.cpp:352
void FrScoreDlg::OnVersusBtnUp(void) {
	if (m_type == 3) {
		return;
	}


	m_type = 3;
	SetTypeBtn();
}

// scoredlg.cpp:361
void FrScoreDlg::OnPuttBtnInit(intptr_t param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	pBtn->SetVisible(false);
}

// scoredlg.cpp:367
void FrScoreDlg::OnBack9BtnInit(intptr_t param) {
	m_pBack9Btn = DYNAMIC_CAST(FrButton, param);

	if (WSingleton<CSharedDoc>::Instance()->m_holes <= 9) {
		m_pBack9Btn->SetVisible(false);
	}

	SetFrontBackBtn();
}

// scoredlg.cpp:387
void FrScoreDlg::OnShotBtnInit(intptr_t param) {
	m_pTypeBtn[0] = DYNAMIC_CAST(FrButton, param);

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {

		m_pTypeBtn[0]->SetVisible(false);
	}
}

// scoredlg.cpp:397
void FrScoreDlg::OnGraphBtnInit(intptr_t param) {
	m_pTypeBtn[1] = DYNAMIC_CAST(FrButton, param);

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {

		if (m_pTypeBtn[1]) {
			m_pTypeBtn[1]->SetVisible(false);
		}
	}
}

// scoredlg.cpp:408
void FrScoreDlg::OnPangBtnInit(intptr_t param) {
	m_pTypeBtn[2] = DYNAMIC_CAST(FrButton, param);
}

// scoredlg.cpp:413
void FrScoreDlg::OnVersusBtnInit(intptr_t param) {
	m_pTypeBtn[3] = DYNAMIC_CAST(FrButton, param);
	if (m_pTypeBtn[3]) {


		if (WSingleton<CSharedDoc>::Instance()->m_roomType != 6) {
			m_pTypeBtn[3]->SetVisible(false);
		}
	}
}

// scoredlg.cpp:423
void FrScoreDlg::OnHoleListInit(intptr_t param) {
	m_pList[4] = DYNAMIC_CAST(FrListBox, param);
}

// scoredlg.cpp:428
void FrScoreDlg::OnHoleListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (!pItem)
		return;

	float w = (float)m_pList[4]->GetItemWidth();
	float h = (float)m_pList[4]->GetItemHeight();

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (!pGDI)
		return;

	float x = w * 0.0555f + pItem->pos.x;
	float y = h * 0.5f + pItem->pos.y - 6.0f;

	if (!pItem->pData)
		return;

	for (int i = m_bFront9 ? 0 : 9; i < (m_bFront9 ? 9 : 18); i++) {
		if (i <= WSingleton<CSharedDoc>::Instance()->m_holes &&
			WSingleton<CSharedDoc>::Instance()->m_courseHoleOn[WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] - 1] != 0) {
			DrawDigit(pGDI, MakeStr("%d", WSingleton<CSharedDoc>::Instance()->m_holeOrder[i]), WPoint(x, y), 0xFFFFFFFF, FrGDI::ALIGN_CENTER, false);
			x += w * 0.111f;
		}
	}
}

// scoredlg.cpp:461
void FrScoreDlg::OnParListInit(intptr_t param) {
	m_pList[3] = DYNAMIC_CAST(FrListBox, param);
}

// scoredlg.cpp:466
void FrScoreDlg::OnParListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (!pItem)
		return;

	float w = (float)m_pList[3]->GetItemWidth();
	float h = (float)m_pList[3]->GetItemHeight();

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (!pGDI)
		return;

	float baseX = w * 0.0555f;
	float x = baseX + pItem->pos.x;
	float y = h * 0.5f + pItem->pos.y - 6.0f;

	if (!pItem->pData)
		return;

	int total = 0;
	for (int i = m_bFront9 ? 0 : 9; i < (m_bFront9 ? 9 : 18); i++) {
		if (i <= WSingleton<CSharedDoc>::Instance()->m_holes &&
			WSingleton<CSharedDoc>::Instance()->m_courseHoleOn[WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] - 1] > 0) {
			total += WSingleton<CSharedDoc>::Instance()->m_courseHoleOn[WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] - 1];
			DrawDigit(pGDI, MakeStr("%d", WSingleton<CSharedDoc>::Instance()->m_courseHoleOn[WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] - 1]), WPoint(x, y), 0xFF9FF5B8, FrGDI::ALIGN_CENTER, false);
		}
		x += 38.0f;
	}

	DrawDigit(pGDI, MakeStr("%d", total), WPoint(baseX + pItem->pos.x + 355.0f, y), 0xFF9FF5B8, FrGDI::ALIGN_CENTER, false);
}

// scoredlg.cpp:505
void FrScoreDlg::OnGolferListInit(intptr_t param) {
	m_pList[0] = DYNAMIC_CAST(FrListBox, param);
}

// scoredlg.cpp:510
void FrScoreDlg::OnGolferListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (!pItem)
		return;




	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (!pGDI)
		return;


	sRivalData* pData = (sRivalData*)pItem->pData;
	if (!pData)
		return;




	WRect rc(pItem->pos.x, pItem->pos.y, m_pList[0]->GetItemWidth(), m_pList[0]->GetItemHeight());


	if (pData->m_hasEmblem) {


		const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(pData->m_emblemName);




		if (emblem) {

			pGDI->DrawTexture(emblem, WRect(pItem->pos.x, pItem->pos.y + 20.0f, emblem->Width(), emblem->Height()), 0xFFFFFFFF, 0);
		}
	}



	if (WSingleton<CProjectG>::Instance()->HidePrivacy())
		return;










	pGDI->Box(rc, 0x809B9DFF, 0, 0.0f);

	WPoint pt(pItem->pos.x + 26.0f, pItem->pos.y + 26.0f);


	pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	pGDI->SetTextStyle(0);

	g_pFresh->GetManager()->PrintText(pt, 0, pData->m_nickname, 100.0f, 0xFFFFFFFF);




	if (m_pMassIcon && pItem->pData && (((sRivalData*)pItem->pData)->m_identity & 0x80)) {


		rc.x = pItem->pos.x + 26.0f;

		rc.y = pItem->pos.y + 13.0f;
		rc.w = (float)m_pMassIcon->Width();
		rc.h = (float)m_pMassIcon->Height();
		pGDI->DrawTexture(m_pMassIcon, rc, 0xFFFFFFFF, 0);
	}
}

// scoredlg.cpp:602
void FrScoreDlg::OnRecordListInit(intptr_t param) {
	m_pList[1] = DYNAMIC_CAST(FrListBox, param);
}

// scoredlg.cpp:607
void FrScoreDlg::OnRecordListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (!pItem)
		return;


	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (!pGDI)
		return;



	if (!pItem->pData)
		return;




	WRect rc(pItem->pos.x, pItem->pos.y, (float)m_pList[1]->GetItemWidth(), (float)m_pList[1]->GetItemHeight());









	pGDI->Box(rc, 0x809B9DFF, 0, 0.0f);

	switch (m_type) {
	case 0:

		DrawShot(pItem);
		break;
	case 1:

		DrawGraph(pItem);
		break;
	case 2:

		DrawPang(pItem);
		break;
	case 3:

		DrawVersus(pItem);
		break;
	}
}

// scoredlg.cpp:655
void FrScoreDlg::OnTotalListInit(intptr_t param) {
	m_pList[2] = DYNAMIC_CAST(FrListBox, param);
}

// scoredlg.cpp:760
void FrScoreDlg::OnCancelBtnInit(intptr_t param) {
	m_pCancelBtn = DYNAMIC_CAST(FrButton, param);
	m_pCancelBtn->SetVisible(false);
}

// scoredlg.cpp:766
void FrScoreDlg::OnCancelBtnUp(void) {
	Close(FrOK, true);
}

// scoredlg.cpp:772
void FrScoreDlg::OnCloseBtnInit(intptr_t param) {
	if (m_pCloseBtn == 0) {

		m_pCloseBtn = DYNAMIC_CAST(FrButton, param);


	}
	if (m_pCloseBtn) {
		m_pCloseBtn->SetVisible(false);
	}
}

// scoredlg.cpp:783
void FrScoreDlg::OnCloseBtnUp(void) {
	Close(FrOK, true);
}

// scoredlg.cpp:801
void FrScoreDlg::OnBlankPanel01Init(intptr_t param) {
	m_pBlankPanel[0] = DYNAMIC_CAST(FrArea, param);
	if (m_pBlankPanel[0]) {


		m_pBlankPanel[0]->SetVisible(m_bShowBlankPanel);
	}
}

// scoredlg.cpp:810
void FrScoreDlg::OnBlankPanel01OwnerDraw(int param) {
	if (m_pBlankPanel[0]) {


		m_pBlankPanel[0]->SetVisible(m_bShowBlankPanel);
	}
}

// scoredlg.cpp:819
void FrScoreDlg::OnBlankPanel02Init(intptr_t param) {
	m_pBlankPanel[1] = DYNAMIC_CAST(FrArea, param);
	if (m_pBlankPanel[1]) {


		m_pBlankPanel[1]->SetVisible(m_bShowBlankPanel);
	}
}

// scoredlg.cpp:828
void FrScoreDlg::OnBlankPanel02OwnerDraw(int param) {
	if (m_pBlankPanel[1]) {


		m_pBlankPanel[1]->SetVisible(m_bShowBlankPanel);
	}
}

// scoredlg.cpp:837
void FrScoreDlg::OnBlankPanel03Init(intptr_t param) {
	m_pBlankPanel[2] = DYNAMIC_CAST(FrArea, param);
	if (m_pBlankPanel[2]) {


		m_pBlankPanel[2]->SetVisible(m_bShowBlankPanel);
	}
}

// scoredlg.cpp:846
void FrScoreDlg::OnBlankPanel03OwnerDraw(int param) {
	if (m_pBlankPanel[2]) {


		m_pBlankPanel[2]->SetVisible(m_bShowBlankPanel);
	}
}

// scoredlg.cpp:857
void FrScoreDlg::OnProc(const float dt) {
	if (m_time < 5.0f) {
		m_time += dt;
	}
}

// scoredlg.cpp:863
void FrScoreDlg::OnDraw(void) {




	float height = WSingleton<CSharedDoc>::Instance()->m_roomType == 6 ? 116.0f : 58.0f;

	WPoint from;
	WPoint to;

	to.x = from.x = m_rect.x + 129.0f;
	from.y = m_rect.y + 126.0f;
	to.y = from.y + height;




	for (int i = 0; i < 10; i++) {
		g_view->DrawLine2D(from, to, 0xFF000000, 0xFF000000, 0);
		from.x += 38.0f;
		to.x += 38.0f;
	}


	from.x += 27.0f;
	to.x += 27.0f;


	g_view->DrawLine2D(from, to, 0xFF000000, 0xFF000000, 0);
}

// scoredlg.cpp:895
void FrScoreDlg::SetFrontBackBtn(void) {
	m_time = 0;

	if (m_pFront9Btn) {
		m_pFront9Btn->SetStatus(m_bFront9 ? FrButton::PRESSED : FrButton::NORMAL);
	}
	if (m_pBack9Btn) {
		m_pBack9Btn->SetStatus(m_bFront9 ? FrButton::NORMAL : FrButton::PRESSED);
	}
}

// scoredlg.cpp:906
void FrScoreDlg::SetTypeBtn(void) {
	m_time = 0;



	for (int i = 0; i < 4; i++) {
		if (m_pTypeBtn[i]) {
			m_pTypeBtn[i]->SetStatus(m_type == i ? FrButton::PRESSED : FrButton::NORMAL);
		}
	}
}

// scoredlg.cpp:917
void FrScoreDlg::DrawShot(FrListItem* pItem) {

	float w = (float)m_pList[4]->GetItemWidth();
	float h = (float)m_pList[4]->GetItemHeight();

	int shown = 0;
	int curHole = GetMyCurHoleInGuildMatch();

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (!pGDI)
		return;

	sRivalData* pData = (sRivalData*)pItem->pData;
	if (!pData)
		return;

	int start = m_bFront9 ? 0 : 9;

	int end = start - (int)(m_time * -20.0f) + 1;
	if (end > start + 9) {
		end = start + 9;
	}

	for (int i = start; i < end; i++) {

		if (i <= WSingleton<CSharedDoc>::Instance()->m_holes) {

			unsigned char order = WSingleton<CSharedDoc>::Instance()->m_holeOrder[i];
			if (WSingleton<CSharedDoc>::Instance()->m_courseHoleOn[order - 1] > 0) {

				if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
					if (i >= curHole)
						return;
				}

				if (pData->m_holeShots[order - 1] > 0) {

					int col = shown;
					if (col >= 9) {
						col -= 9;
					}

					float x = ((float)col + 0.5f) * w * 0.1111111f + pItem->pos.x;
					float y = h * 0.5f + pItem->pos.y - 6.0f;

					unsigned long color;
					switch (pData->m_holeScore[order - 1]) {
					case -4:
					case -3:
					case -2:
						color = 0xFFFF4F4F;
						break;
					case -1:
						color = 0xFFFFBD42;
						break;
					case 0:
						color = WSingleton<CSharedDoc>::Instance()->m_courseHoleOn[WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] - 1] < pData->m_holeShots[WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] - 1] ? 0xFF6F6F71 : 0xFFFFFFFF;
						break;
					case 1:
						color = 0xFFB9B9B9;
						break;
					default:
						color = 0xFF6F6F71;
						break;
					}

					DrawDigit(pGDI, MakeStr("%d", pData->m_holeShots[WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] - 1]), WPoint(x, y), color, FrGDI::ALIGN_CENTER, false);
				}

				shown++;
			}
		}
	}
}

// scoredlg.cpp:1086
void FrScoreDlg::DrawPang(FrListItem* pItem) {

	float w = (float)m_pList[4]->GetItemWidth();
	float h = (float)m_pList[4]->GetItemHeight();

	int shown = 0;
	int curHole = GetMyCurHoleInGuildMatch();

	sRivalData* pData = (sRivalData*)pItem->pData;

	int start = m_bFront9 ? 0 : 9;

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	int end = start - (int)(m_time * -20.0f) + 1;
	if (end > start + 9) {
		end = start + 9;
	}

	for (int i = start; i < end; i++) {

		if (i <= WSingleton<CSharedDoc>::Instance()->m_holes) {

			unsigned char order = WSingleton<CSharedDoc>::Instance()->m_holeOrder[i];
			if (WSingleton<CSharedDoc>::Instance()->m_courseHoleOn[order - 1] > 0) {

				if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
					if (i >= curHole)
						return;
				}

				if (pData->m_holeShots[order - 1] > 0) {

					int col = shown;
					if (col >= 9) {
						col -= 9;
					}

					float x = (float)(int)(((float)col + 0.5f) * w * 0.1111111f + pItem->pos.x + 2.0f);
					float y = (float)(int)(h * 0.5f + pItem->pos.y - 6.0f);

					DrawDigit(pGDI, MakeStr("%d", (int)pData->m_holePang[WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] - 1]), WPoint(x, y), 0xFFE98919, FrGDI::ALIGN_CENTER, true);
				}

				shown++;
			}
		}
	}
}

// scoredlg.cpp:1124
void FrScoreDlg::DrawVersus(FrListItem* pItem) {

	float itemW = (float)m_pList[1]->GetItemWidth();
	float itemH = (float)m_pList[1]->GetItemHeight();



	int col = 0;

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (!pGDI)
		return;


	sRivalData* pMine = (sRivalData*)pItem->pData;
	sRivalData* pRival;


	if (pMine->m_uid == m_guildPlayerUID[0]) {

		pRival = &WSingleton<CSharedDoc>::Instance()->m_gameResultList[WSingleton<CSharedDoc>::Instance()->GetIndex(m_guildPlayerUID[1])];

	} else {

		pRival = &WSingleton<CSharedDoc>::Instance()->m_gameResultList[WSingleton<CSharedDoc>::Instance()->GetIndex(m_guildPlayerUID[0])];
	}


	if (!pRival)
		return;


	int start = m_bFront9 ? 0 : 9;
	int end = start - (int)(m_time * -20.0f) + 1;
	if (end > start + 9) {
		end = start + 9;
	}

	for (int i = start; i < end; i++) {



		CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
		if (pDoc->m_courseHoleOn[pDoc->m_holeOrder[i] - 1] > 0) {

			int x = col >= 9 ? col - 9 : col;





			WPoint pt(pItem->pos.x + (x + 0.5f) * itemW * 0.11111111f, pItem->pos.y + itemH * 0.5f - 6.0f);
			const Bitmap* pBmp = 0;
			switch (GetGuildVersus(pMine, pRival, WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] - 1)) {
			case 0:

				pBmp = m_pWin;
				break;
			case 1:

				pBmp = m_pLose;
				break;
			case 2:

				pBmp = m_pDraw;
				break;
			case 3:

				pBmp = m_pVersusMinus;
				break;
			}







			if (pBmp) {
				WRect rc;
				rc.y = pt.y;
				rc.x = (float)(int)(pt.x - pBmp->Width() * 0.5f);
				rc.w = (float)pBmp->Width();
				rc.h = (float)pBmp->Height();

				pGDI->DrawTexture(pBmp, rc, 0xFFFFFFFF, 0);
			}

			col++;
		}
	}

}

// scoredlg.cpp:1210
void FrScoreDlg::DrawDigit(FrGraphicInterface* pGDI, char* text, WPoint pos, unsigned long color, FrGDI::eTextAlign align, bool bShadow) {

	int len = (int)strlen(text);
	const Bitmap* bitmaps[16];
	int totalWidth = 0;

	if (align != FrGDI::ALIGN_LEFT) {
		totalWidth = 0;
		for (int i = 0; i < len; i++) {
			switch (text[i]) {
			case '+':
				bitmaps[i] = m_pPlus;
				break;
			case '-':
				bitmaps[i] = m_pMinus;
				break;
			default:
				bitmaps[i] = m_pDigit[text[i] - '0'];
				break;
			}
			totalWidth += bitmaps[i]->Width() - (bShadow ? (len < 4 ? 0 : 2) : 0);
		}
	}

	WRect rc;
	switch (align) {
	case FrGDI::ALIGN_LEFT:
		rc.x = (float)(int)pos.x;
		break;
	case FrGDI::ALIGN_CENTER:
		rc.x = (float)(int)(pos.x - totalWidth * 0.5f);
		break;
	case FrGDI::ALIGN_RIGHT:
		rc.x = (float)(int)(pos.x - totalWidth);
		break;
	}

	rc.y = pos.y;
	for (int i = 0; i < len; i++) {
		rc.w = (float)bitmaps[i]->Width();
		rc.h = (float)bitmaps[i]->Height();

		pGDI->DrawTexture(bitmaps[i], rc, color, 0);
		rc.x += rc.w - (bShadow ? (len < 4 ? 0 : 2) : 0);
	}
}

// scoredlg.cpp:1272
int FrScoreDlg::GetGuildVersus(sRivalData* pMine, sRivalData* pRival, int hole) {
	if (pMine->m_state != 0 && pRival->m_state != 0) {


		if (pMine->m_holeShots[hole] == 0 && pRival->m_holeShots[hole] == 0) {

			return 3;
		}

		if (pMine->m_holeShots[hole] == 0 && pRival->m_holeShots[hole] != 0) {

			return 1;
		}

		if (pMine->m_holeShots[hole] != 0 && pRival->m_holeShots[hole] == 0) {

			return 0;
		}
	}


	if (pMine->m_holeShots[hole] == 0 || pRival->m_holeShots[hole] == 0) {
		return 3;
	}


	if (pMine->m_holeShots[hole] < pRival->m_holeShots[hole]) {
		return 0;
	}

	if (pMine->m_holeShots[hole] > pRival->m_holeShots[hole]) {
		return 1;
	}

	return pMine->m_holeShots[hole] == pRival->m_holeShots[hole] ? 2 : 3;
}

// scoredlg.cpp:1314
unsigned long FrScoreDlg::GetScoreColor(int score) {
	switch (score) {
	case -4:
	case -3:
	case -2:
		return 0xFFFF0000;
	case -1:
		return 0xFFFFBB4F;
	case 0:
		return 0xFFFFFFFF;
	case 1:
		return 0xFF82C3FF;
	default:
		return 0xFF0036FF;
	}
}

// scoredlg.cpp:1339
int FrScoreDlg::GetMyCurHoleInGuildMatch(void) {
	CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
	if (pDoc->m_roomType != 6) {
		return 0;
	}

	if (m_pGuildRival == 0) {
		return 0;
	}


	int start = m_bFront9 ? 0 : 9;
	int end = start - (int)(m_time * -20.0f) + 1;
	if (end > start + 9) {
		end = start + 9;
	}

	int i;
	for (i = start; i < end; i++) {

		if (i <= pDoc->m_holes) {

			if (WSingleton<CSharedDoc>::Instance()->m_courseHoleOn[WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] - 1] > 0) {

				if (!(m_pGuildRival->m_holeShots[WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] - 1] > 0)) {
					break;
				}
			}
		}
	}

	return i;
}
