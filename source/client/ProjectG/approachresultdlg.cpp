// Original: d:\build\custom\temp\code\client\projectg\approachresultdlg.cpp
#include "ApproachResultDlg.h"
#include "ObjectFactory.h"
#include "exhibition.h"
#include "fresh.h"
#include "frarea.h"
#include "frlistbox.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "GolfDoc.h"
#include "SharedDoc.h"
#include "parttidlist.h"
#include "wfont.h"
#include "wresrcmng.h"
#include "wview.h"
#include <stdio.h>

// Texture files of the two title fonts of FrApproachResultDlg (no symbol).
extern "C" char* data_a94d20[2] = { "[font_wind.jpg", "[bigfont.jpg" };

// approachresultdlg.cpp:12 (run-time type and factory registration of FrApproachResultDlg)
const WRTTI FrApproachResultDlg::m_RTTI("FrApproachResultDlg", &FrForm::m_RTTI);

IObject* FrApproachResultDlgMakeInstance(void) {
	return new FrApproachResultDlg;
}

struct __sFrApproachResultDlg {
	__sFrApproachResultDlg(void) {
		ObjectFactory().AddObjectFunctor(FrApproachResultDlgMakeInstance, "FrApproachResultDlg");
	}
};
static __sFrApproachResultDlg s_registerFrApproachResultDlg;

// approachresultdlg.cpp:14
const sFRESH_MSGMAP* FrApproachResultDlg::GetMessageMap(void) const {
	return &FrApproachResultDlg::_MsgMap;
}

sFRESH_MSGMAP FrApproachResultDlg::_MsgMap = { &FrForm::_MsgMap, &FrApproachResultDlg::_MsgEntries[0] };

sFRESH_ENTRY FrApproachResultDlg::_MsgEntries[] = {
	// approachresultdlg.cpp:17
	{ "left_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachResultDlg::OnLeftAreaInit },
	// approachresultdlg.cpp:18
	{ "left_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachResultDlg::OnLeftAreaOwnerDraw },
	// approachresultdlg.cpp:19
	{ "left_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachResultDlg::OnLeftListInit },
	// approachresultdlg.cpp:20
	{ "left_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachResultDlg::OnLeftListOwnerDraw },
	// approachresultdlg.cpp:21
	{ "right_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachResultDlg::OnRightAreaInit },
	// approachresultdlg.cpp:22
	{ "right_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachResultDlg::OnRightAreaOwnerDraw },
	// approachresultdlg.cpp:23
	{ "right_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachResultDlg::OnRightListInit },
	// approachresultdlg.cpp:24
	{ "right_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachResultDlg::OnRightListOwnerDraw },

	// approachresultdlg.cpp:26
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// approachresultdlg.cpp:29 (sort predicate of the result lists)
bool ApproachResultCompare(const void* lhs, const void* rhs) {
	const sApproachResultData* pLhs = (const sApproachResultData*)lhs;
	const sApproachResultData* pRhs = (const sApproachResultData*)rhs;

	return pLhs->m_unk09 < pRhs->m_unk09 ? true : false;
}

// approachresultdlg.cpp:40
FrApproachResultDlg::FrApproachResultDlg(void) {
	ClearVariables();
}

// approachresultdlg.cpp:46
FrApproachResultDlg::~FrApproachResultDlg(void) {
	m_pTitleFont2->Erase();
	m_pTitleFont1->Erase();
	if (g_resrcmng && m_pTitleFont2) { g_resrcmng->Release(m_pTitleFont2); m_pTitleFont2 = 0; }
	if (g_resrcmng && m_pTitleFont1) { g_resrcmng->Release(m_pTitleFont1); m_pTitleFont1 = 0; }
	if (m_pPartTidList) { delete m_pPartTidList; m_pPartTidList = 0; }
	if (m_pExhibition) { delete m_pExhibition; m_pExhibition = 0; }
}

// approachresultdlg.cpp:57
bool FrApproachResultDlg::OnInit(void) {
	return FrForm::OnInit();
}

// approachresultdlg.cpp:62
void FrApproachResultDlg::OnProc(const float dt) {
	FrForm::OnProc(dt);

	if (m_pExhibition) {
		m_pExhibition->Process(dt, true);
	} else {
		ShowPet();
	}
}

// approachresultdlg.cpp:81
void FrApproachResultDlg::ClearVariables(void) {
	tagWTITLEFONT bigInfo;
	memset(&bigInfo, 0, sizeof(bigInfo));

	m_pLeftArea = 0;
	m_pLeftList = 0;
	m_pLeftAreaBitmap = 0;
	m_pLeftTab1 = 0;
	m_pLeftTab2 = 0;
	m_pExhibition = 0;
	m_pPartTidList = 0;
	m_pRightList = 0;
	m_pRightArea = 0;
	m_pTitleFont1 = 0;
	m_pTitleFont2 = 0;

	bigInfo.filename = &data_a94d20[1];
	bigInfo.fontw = 32;
	bigInfo.fonth = 32;
	bigInfo.numPages = 1;
	bigInfo.texw = 256;
	bigInfo.texh = 128;
	bigInfo.pCharSet = "1234567890ym%-./x ";
	m_pTitleFont1 = g_resrcmng->GetTitleFont();
	m_pTitleFont1->Create(&bigInfo);

	tagWTITLEFONT smallInfo;
	memset(&smallInfo, 0, sizeof(smallInfo));
	smallInfo.filename = data_a94d20;
	smallInfo.fontw = 16;
	smallInfo.fonth = 16;
	smallInfo.numPages = 1;
	smallInfo.texw = 128;
	smallInfo.texh = 64;
	smallInfo.pCharSet = "1234567890ym%-.?/:";
	m_pTitleFont2 = g_resrcmng->GetTitleFont();
	m_pTitleFont2->Create(&smallInfo);

	char boxName[32] = "\0";
	char treasureName[32] = "\0";

	switch (GetCurMap()) {
	case 2:
	case 3:
	case 11:
	case 16:
	case 19:
		strcpy(boxName, "th_box_1");
		strcpy(treasureName, "treasurebox_a");
		break;
	case 0:
	case 1:
	case 6:
	case 7:
		strcpy(boxName, "th_box_2");
		strcpy(treasureName, "treasurebox_b");
		break;
	case 10:
	case 13:
	case 15:
		strcpy(boxName, "th_box_3");
		strcpy(treasureName, "treasurebox_c");
		break;
	case 4:
	case 5:
	case 8:
	case 9:
	case 14:
		strcpy(boxName, "th_box_4");
		strcpy(treasureName, "treasurebox_d");
		break;
	}

	m_pRankGold = g_pFresh->GetBitmap("rank_gold");
	m_pRankSilver = g_pFresh->GetBitmap("rank_silver");
	m_pRankBronze = g_pFresh->GetBitmap("rank_bronze");
	m_pPrizeGoodLuck = g_pFresh->GetBitmap("prize_goodluck");
	m_pTreasureBox = g_pFresh->GetBitmap(treasureName);

	m_pBoxBitmap = g_pFresh->GetBitmap(boxName);

	m_bApproachEnd = false;
}

// approachresultdlg.cpp:188
void FrApproachResultDlg::OnLeftAreaInit(intptr_t param) {
	m_pLeftArea = DYNAMIC_CAST(FrArea, param);

	if (g_view->GetWidth() < 700.0f) {

		m_pLeftArea->SetVisible(false);
		m_pLeftArea->Enable(false);
		return;
	}

	char bgName[64];
	sprintf(bgName, "30in_back_%s", WSingleton<CSharedDoc>::Instance()->m_pCurCourse ? WSingleton<CSharedDoc>::Instance()->m_pCurCourse->m_mapName : 0);
	m_pLeftAreaBitmap = g_pFresh->GetBitmap(bgName);

	m_pLeftTab1 = g_pFresh->GetBitmap("approach_tab1");
	m_pLeftTab2 = g_pFresh->GetBitmap("approach_tab2");
}

// approachresultdlg.cpp:343
void FrApproachResultDlg::OnLeftListInit(intptr_t param) {
	m_pLeftList = DYNAMIC_CAST(FrListBox, param);


	m_pLeftList->ClearItem();



	std::list<sApproachResultData*>& results = GOLFDOC()->GetApproachCurHoleResult();




	for (std::list<sApproachResultData*>::iterator it = results.begin(); it != results.end(); ++it) {

		sApproachResultData* pData = *it;
		if (pData) {

			if (pData->m_unk09 <= 3 && pData->m_unk09 > 1)

				m_pLeftList->AddItem(pData);
		}
	}


	m_pLeftList->SortItem(ApproachResultCompare);
}

// approachresultdlg.cpp:486
void FrApproachResultDlg::OnRightAreaInit(intptr_t param) {
	m_pRightArea = DYNAMIC_CAST(FrArea, param);


	m_pRightTab1 = g_pFresh->GetBitmap("approach_tab3");
	m_pRightTab2 = g_pFresh->GetBitmap("approach_tab4");
}

// approachresultdlg.cpp:495
void FrApproachResultDlg::OnRightAreaOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (!pGDI)
		return;


	WRect rect = GetRect();
	if (m_pRightTab1)

		pGDI->DrawTexture(m_pRightTab1, WRect(rect.x + 365.0f, rect.y + 2.0f, m_pRightTab1->Width(), m_pRightTab1->Height()), 0xFFFFFFFF, 0);

	if (m_pRightTab2)

		pGDI->DrawTexture(m_pRightTab2, WRect(rect.x + 621.0f, rect.y + 2.0f, m_pRightTab2->Width(), m_pRightTab2->Height()), 0xFFFFFFFF, 0);
}

// approachresultdlg.cpp:512
void FrApproachResultDlg::OnRightListInit(intptr_t param) {
	m_pRightList = DYNAMIC_CAST(FrListBox, param);



	m_pRightList->ClearItem();


	std::list<sApproachResultData*>& results = GOLFDOC()->GetApproachCurHoleResult();




	for (std::list<sApproachResultData*>::iterator it = results.begin(); it != results.end(); ++it) {

		sApproachResultData* pData = *it;
		if (pData)
			m_pRightList->AddItem(pData);
	}


	m_pRightList->SortItem(ApproachResultCompare);
}

// approachresultdlg.cpp:704
void FrApproachResultDlg::SetApproachEnd(bool bEnd) {
	m_bApproachEnd = bEnd;
}

// approachresultdlg.cpp:709
bool FrApproachResultDlg::GetApproachEnd(void) const {
	return m_bApproachEnd;
}

// approachresultdlg.cpp:715
void FrApproachResultDlg::ShowPet(void) {
	if (!m_pLeftAreaBitmap) {
		return;
	}
	std::list<sApproachResultData*>& results = GOLFDOC()->GetApproachCurHoleResult();
	if (results.empty()) {
		return;
	}

	sApproachResultData* pData = NULL;
	std::list<sApproachResultData*>::iterator it;
	for (it = results.begin(); it != results.end(); ++it) {
		pData = *it;
		if (pData) {
			if (pData->m_unk09 == 1) {
				break;
			}
		}

		pData = NULL;
	}

	if (!pData) {
		return;
	}

	sRivalData* pResult = &WSingleton<CSharedDoc>::Instance()->m_gameResultList[WSingleton<CSharedDoc>::Instance()->GetIndex(pData->m_uid)];

	if (WSingleton<CSharedDoc>::Instance()->IsCurrentMissionBlind() && pResult->m_uid != WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid) {
		return;
	}

	std::map<unsigned long, sUserInfoTime>::iterator itInfo;
	if (!WSingleton<CSharedDoc>::Instance()->FindUserInfoTimeUID(pResult->m_id, itInfo)) {
		return;
	}

	if (WSingleton<CSharedDoc>::Instance()->GetUserInfoMod() != 0) {
		return;
	}

	WRect rect(GetRect().x + 5.0f,
		GetRect().y - 25.0f, m_pLeftAreaBitmap->Width(), m_pLeftAreaBitmap->Height());

	m_pExhibition = new CExhibition(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f, 0.33f, false);
	m_pExhibition->SetDistance(g_view->GetWidth() * 0.0275f, true, false);
	m_pExhibition->SetArea(rect.w, rect.h);
	m_pExhibition->SetControlFlag(0x10);

	m_pPartTidList = new CPartTidList;

	if (!m_pPartTidList->SetTids(&itInfo->second.m_userInfo.m_characterInfo, 0xFF) || !m_pPartTidList->IsComboValid()) {
		m_pPartTidList->SetDefaultTids();
	}

	char motion[64] = "1\xB5\xEE\xB8\xF0\xBC\xC7";

	m_pExhibition->SetModel(*m_pPartTidList, itInfo->second.m_userInfo.m_characterInfo.auxParts.tids, motion, 0.0f, 0);
}

// approachresultdlg.cpp:786 (run-time type and factory registration of FrApproachEndDlg)
const WRTTI FrApproachEndDlg::m_RTTI("FrApproachEndDlg", &FrForm::m_RTTI);

IObject* FrApproachEndDlgMakeInstance(void) {
	return new FrApproachEndDlg;
}

struct __sFrApproachEndDlg {
	__sFrApproachEndDlg(void) {
		ObjectFactory().AddObjectFunctor(FrApproachEndDlgMakeInstance, "FrApproachEndDlg");
	}
};
static __sFrApproachEndDlg s_registerFrApproachEndDlg;

// approachresultdlg.cpp:788
const sFRESH_MSGMAP* FrApproachEndDlg::GetMessageMap(void) const {
	return &FrApproachEndDlg::_MsgMap;
}

sFRESH_MSGMAP FrApproachEndDlg::_MsgMap = { &FrForm::_MsgMap, &FrApproachEndDlg::_MsgEntries[0] };

sFRESH_ENTRY FrApproachEndDlg::_MsgEntries[] = {
	// approachresultdlg.cpp:790
	{ "draw_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachEndDlg::OnDrawAreaInit },
	// approachresultdlg.cpp:791
	{ "draw_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachEndDlg::OnDrawAreaOwnerDraw },
	// approachresultdlg.cpp:792
	{ "app_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachEndDlg::OnListInit },
	// approachresultdlg.cpp:793
	{ "app_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrApproachEndDlg::OnListOwnerDraw },

	// approachresultdlg.cpp:795
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// approachresultdlg.cpp:798
FrApproachEndDlg::FrApproachEndDlg(void) {
	m_pDrawArea = 0;
	m_pList = 0;
	m_pBitmap[0] = 0;
	m_pBitmap[1] = 0;
	m_pBitmap[2] = 0;
	m_pBitmap[3] = 0;
}

// approachresultdlg.cpp:808
FrApproachEndDlg::~FrApproachEndDlg(void) {
}

// approachresultdlg.cpp:813
void FrApproachEndDlg::OnDrawAreaInit(intptr_t param) {
	m_pDrawArea = DYNAMIC_CAST(FrArea, param);


	if (m_pDrawArea) {

		m_pBitmap[0] = g_pFresh->GetBitmap("approach_result1");
		m_pBitmap[1] = g_pFresh->GetBitmap("approach_result2");
		m_pBitmap[2] = g_pFresh->GetBitmap("approach_result3");
		m_pBitmap[3] = g_pFresh->GetBitmap("approach_result4");
	}
}

// approachresultdlg.cpp:827
void FrApproachEndDlg::OnDrawAreaOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (!pGDI)
		return;




	WRect rect = GetRect();
	if (m_pBitmap[0])




		pGDI->DrawTexture(m_pBitmap[0], WRect(rect.x + 2.0f, rect.y + 23.0f, m_pBitmap[0]->Width(), m_pBitmap[0]->Height()), 0xFFFFFFFF, 0);

	if (m_pBitmap[1])




		pGDI->DrawTexture(m_pBitmap[1], WRect(rect.x + 258.0f, rect.y + 23.0f, m_pBitmap[1]->Width(), m_pBitmap[1]->Height()), 0xFFFFFFFF, 0);

	if (m_pBitmap[2])




		pGDI->DrawTexture(m_pBitmap[2], WRect(rect.x + 12.0f, rect.y + 313.0f, m_pBitmap[2]->Width(), m_pBitmap[2]->Height()), 0xFFFFFFFF, 0);

	if (m_pBitmap[3])




		pGDI->DrawTexture(m_pBitmap[3], WRect(rect.x + 268.0f, rect.y + 313.0f, m_pBitmap[3]->Width(), m_pBitmap[3]->Height()), 0xFFFFFFFF, 0);
}

// approachresultdlg.cpp:865
void FrApproachEndDlg::OnListInit(intptr_t param) {
	m_pList = DYNAMIC_CAST(FrListBox, param);



	if (m_pList) {

		m_pList->ClearItem();

		std::list<sApproachResultData*>& results = GOLFDOC()->GetApproachGameResult();


		for (std::list<sApproachResultData*>::iterator it = results.begin(); it != results.end(); ++it) {

			sApproachResultData* pData = *it;
			if (pData) {

				if (pData->m_unk09 > 0 || pData->m_unk09 <= 30)

					m_pList->AddItem(pData);
			}
		}


		m_pList->SortItem(ApproachResultCompare);
	}
}

// approachresultdlg.cpp:1040 (run-time type and factory registration of FrAppTreasureGiftDlg)
const WRTTI FrAppTreasureGiftDlg::m_RTTI("FrAppTreasureGiftDlg", &FrForm::m_RTTI);

IObject* FrAppTreasureGiftDlgMakeInstance(void) {
	return new FrAppTreasureGiftDlg;
}

struct __sFrAppTreasureGiftDlg {
	__sFrAppTreasureGiftDlg(void) {
		ObjectFactory().AddObjectFunctor(FrAppTreasureGiftDlgMakeInstance, "FrAppTreasureGiftDlg");
	}
};
static __sFrAppTreasureGiftDlg s_registerFrAppTreasureGiftDlg;

// approachresultdlg.cpp:1042
const sFRESH_MSGMAP* FrAppTreasureGiftDlg::GetMessageMap(void) const {
	return &FrAppTreasureGiftDlg::_MsgMap;
}

sFRESH_MSGMAP FrAppTreasureGiftDlg::_MsgMap = { &FrForm::_MsgMap, &FrAppTreasureGiftDlg::_MsgEntries[0] };

sFRESH_ENTRY FrAppTreasureGiftDlg::_MsgEntries[] = {
	// approachresultdlg.cpp:1045
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// approachresultdlg.cpp:1048
FrAppTreasureGiftDlg::FrAppTreasureGiftDlg(void) {
}

// approachresultdlg.cpp:1052
FrAppTreasureGiftDlg::~FrAppTreasureGiftDlg(void) {
}
