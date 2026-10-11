// Original: d:\build\custom\temp\code\client\projectg\s5\demonlanddlg.cpp
#include "DemonLandDlg.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frstatic.h"
#include "frarea.h"
#include "frlistbox.h"
#include "frgaugebar.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "commonutil.h"
#include "../client/Wangreal/include/wview.h"
#include "woverlay.h"
#include "wresrcmng.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "Utilities.h"
#include "NetResourceManager.h"

unsigned char GetNextHole(void);

// demonlanddlg.cpp:33 (run-time type and factory registration of FrDemonNextHole)
const WRTTI FrDemonNextHole::m_RTTI("FrDemonNextHole", &FrForm::m_RTTI);

IObject* FrDemonNextHoleMakeInstance(void) {
	return new FrDemonNextHole;
}

struct __sFrDemonNextHole {
	__sFrDemonNextHole(void) {
		ObjectFactory().AddObjectFunctor(FrDemonNextHoleMakeInstance, "FrDemonNextHole");
	}
};
static __sFrDemonNextHole s_registerFrDemonNextHole;

// demonlanddlg.cpp:35
const sFRESH_MSGMAP* FrDemonNextHole::GetMessageMap(void) const {
	return &FrDemonNextHole::_MsgMap;
}

sFRESH_MSGMAP FrDemonNextHole::_MsgMap = { &FrForm::_MsgMap, &FrDemonNextHole::_MsgEntries[0] };

// demonlanddlg.cpp:37
sFRESH_ENTRY FrDemonNextHole::_MsgEntries[] = {
	{ "nextholepattern", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonNextHole::OnAreaPattern_Init },
	{ "nextholepattern", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonNextHole::OnAreaPattern_OwnerDraw },
	{ "nexthole", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonNextHole::OnAreaNextHole_Init },
	{ "nexthole", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonNextHole::OnAreaNextHole_OwnerDraw },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// demonlanddlg.cpp:49
FrDemonNextHole::FrDemonNextHole(void) : m_pPatternArea(0), m_pNextHoleArea(0), m_pPatternBitmap(0), m_state(1) {
}

// demonlanddlg.cpp:54
FrDemonNextHole::~FrDemonNextHole(void) {
}

// demonlanddlg.cpp:59
bool FrDemonNextHole::OnInit(void) {
	m_pPatternBitmap = g_pFresh->GetBitmap("demon_nexthole_pattern");
	if (m_pPatternBitmap == 0)
	{
		return false;
	}

	IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(((GetCurMap() > 0x7F && GetCurMap() != 0xFD) ? GetCurMap() - 0x80 : GetCurMap()) | 0x28000000);
	if (pCourse == 0)
		return false;

	m_pHoleOverlay[0] = g_resrcmng->GetOverlay(MakeStr("%s_1.png", pCourse->m_name), 0);
	m_pHoleOverlay[1] = g_resrcmng->GetOverlay(MakeStr("%s_2.png", pCourse->m_name), 0);

	if (m_pHoleOverlay[0] != 0 || m_pHoleOverlay[1] != 0) {

		m_texWidth = m_pHoleOverlay[0]->GetWidth();
		m_texHeight = m_pHoleOverlay[0]->GetHeight();
	}

	return FrForm::OnInit();
}

// demonlanddlg.cpp:82
void FrDemonNextHole::SetState(unsigned char state) {
	if (m_state == state) {
		if (IsEnabled()) {
			if (m_state == 1) {
				Enable(false);
				SetVisible(false);
			}
		}
		return;
	}

	m_state = state;
	if (m_state == 1) {
		Enable(false);
		SetVisible(false);
	} else {
		Enable(true);
		SetVisible(true);

		SetNextHole(GetNextHole());
	}
}

// demonlanddlg.cpp:115
void FrDemonNextHole::SetNextHole(int hole) {
	if (m_pHoleOverlay[0] == 0 || m_pHoleOverlay[1] == 0)
		return;

	if (hole > 10 || hole == 19 || CSharedDoc::Instance()->m_holeOrder[CSharedDoc::Instance()->m_holes - 1] == GOLFDOC()->m_currentHole) {

		m_holeOverlayIndex = 1;
		hole -= 10;
	} else {

		m_holeOverlayIndex = 0;
	}

	if (hole == 19 || CSharedDoc::Instance()->m_holeOrder[CSharedDoc::Instance()->m_holes - 1] == GOLFDOC()->m_currentHole) {

		S5::UTIL::CalculateUVPos(m_holeUV, m_texWidth, m_texHeight, 140.0f, 93.0f, 8);
	} else {

		S5::UTIL::CalculateUVPos(m_holeUV, m_texWidth, m_texHeight, 140.0f, 93.0f, hole - 1);
	}
}

// demonlanddlg.cpp:141
void FrDemonNextHole::OnAreaPattern_Init(intptr_t param) {
	m_pPatternArea = DYNAMIC_CAST(FrArea, param);
}

// demonlanddlg.cpp:146
void FrDemonNextHole::OnAreaPattern_OwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0)
		return;

	if (m_pPatternBitmap == 0)
		return;

	const WRect& rect = GetRect();

	pGDI->DrawTexture(m_pPatternBitmap, WRect(rect.x + 120.0f, rect.y + 110.0f, 63.0f, 35.0f), 0xFFFFFFFF, 0);
}

// demonlanddlg.cpp:163
void FrDemonNextHole::OnAreaNextHole_Init(intptr_t param) {
	m_pNextHoleArea = DYNAMIC_CAST(FrArea, param);
}

// demonlanddlg.cpp:168
void FrDemonNextHole::OnAreaNextHole_OwnerDraw(int param) {
	if (m_state == 1)
		return;

	const WRect& rect = m_pNextHoleArea->GetRect();

	m_pHoleOverlay[m_holeOverlayIndex]->Render(g_view, m_holeUV, WRect(WPoint(rect.x, rect.y), 140.0f, 93.0f), 0, 0xffffffff, 0.0f, 0);
}

// demonlanddlg.cpp:188 (run-time type and factory registration of FrDemonAttackGauge)
const WRTTI FrDemonAttackGauge::m_RTTI("FrDemonAttackGauge", &FrForm::m_RTTI);

IObject* FrDemonAttackGaugeMakeInstance(void) {
	return new FrDemonAttackGauge;
}

struct __sFrDemonAttackGauge {
	__sFrDemonAttackGauge(void) {
		ObjectFactory().AddObjectFunctor(FrDemonAttackGaugeMakeInstance, "FrDemonAttackGauge");
	}
};
static __sFrDemonAttackGauge s_registerFrDemonAttackGauge;

// demonlanddlg.cpp:190
const sFRESH_MSGMAP* FrDemonAttackGauge::GetMessageMap(void) const {
	return &FrDemonAttackGauge::_MsgMap;
}

sFRESH_MSGMAP FrDemonAttackGauge::_MsgMap = { &FrForm::_MsgMap, &FrDemonAttackGauge::_MsgEntries[0] };

// demonlanddlg.cpp:192
sFRESH_ENTRY FrDemonAttackGauge::_MsgEntries[] = {
	{ "attackgauge", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonAttackGauge::OnImageBarAttackPoint_Init },
	{ "gaugemarker", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonAttackGauge::OnAreaAttackMarker_Init },
	{ "gaugemarker", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonAttackGauge::OnAreaAttackMarker_OwnerDraw },
	{ "blingbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonAttackGauge::OnAreaBlingBar_Init },
	{ "blingbar", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonAttackGauge::OnAreaBlingBar_OwnerDraw },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// demonlanddlg.cpp:204
FrDemonAttackGauge::FrDemonAttackGauge(void) : m_pAttackPointBar(0), m_pAttackMarkerArea(0), m_pAttackPointBitmap(0) {
}

// demonlanddlg.cpp:209
FrDemonAttackGauge::~FrDemonAttackGauge(void) {
}

// demonlanddlg.cpp:214
bool FrDemonAttackGauge::OnInit(void) {
	m_pAttackPointBitmap = g_pFresh->GetBitmap("demon_attack_point");
	m_pGaugeLightBitmap = g_pFresh->GetBitmap("demon_gauge_light");

	return FrForm::OnInit();
}

// demonlanddlg.cpp:222
void FrDemonAttackGauge::SetState(unsigned char state) {
	if (m_state == state)
		return;

	m_state = state;
	if (m_state == 1) {
		Enable(false);
		SetVisible(false);
	} else {
		Enable(true);
		SetVisible(true);
	}
}

// demonlanddlg.cpp:242
void FrDemonAttackGauge::OnImageBarAttackPoint_Init(intptr_t param) {
	m_pAttackPointBar = DYNAMIC_CAST(FrGaugeBarImage, param);
	if (m_pAttackPointBar) {
		m_pAttackPointBar->SetRange(0, 1000, 0);
		m_pAttackPointBar->SetBarSpeed(250.0f);
		m_pAttackPointBar->SetDestPos(1000, false);
	}
}

// demonlanddlg.cpp:253
void FrDemonAttackGauge::OnAreaAttackMarker_Init(intptr_t param) {
	m_pAttackMarkerArea = DYNAMIC_CAST(FrArea, param);
}

// demonlanddlg.cpp:258
void FrDemonAttackGauge::OnAreaAttackMarker_OwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0)
		return;

	if (m_pAttackPointBitmap == 0)
		return;

	const WRect& rect = GetRect();

	pGDI->DrawTexture(m_pAttackPointBitmap, WRect(rect.x + 50.0f, rect.y + 50.0f, 24.0f, 15.0f), 0xFFFFFFFF, 0);
}

// demonlanddlg.cpp:276
void FrDemonAttackGauge::OnAreaBlingBar_Init(intptr_t param) {
	m_pBlingBarArea = DYNAMIC_CAST(FrArea, param);
}

// demonlanddlg.cpp:281
void FrDemonAttackGauge::OnAreaBlingBar_OwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0)
		return;

	if (m_pGaugeLightBitmap == 0)
		return;

	const WRect& rect = m_pAttackPointBar->GetRect();
	float x = rect.x - m_pGaugeLightBitmap->Width() / 2 + m_pAttackPointBar->GetPixelPerHeight();
	float y = rect.y - m_pGaugeLightBitmap->Height() / 2 + 15.0f;

	pGDI->DrawTexture(m_pGaugeLightBitmap, WRect(x, y, 33.0f, 68.0f), 0xFFFFFFFF, 0);
}

// demonlanddlg.cpp:311 (run-time type and factory registration of FrDemonProcessRank)
const WRTTI FrDemonProcessRank::m_RTTI("FrDemonProcessRank", &FrForm::m_RTTI);

IObject* FrDemonProcessRankMakeInstance(void) {
	return new FrDemonProcessRank;
}

struct __sFrDemonProcessRank {
	__sFrDemonProcessRank(void) {
		ObjectFactory().AddObjectFunctor(FrDemonProcessRankMakeInstance, "FrDemonProcessRank");
	}
};
static __sFrDemonProcessRank s_registerFrDemonProcessRank;

// demonlanddlg.cpp:313
const sFRESH_MSGMAP* FrDemonProcessRank::GetMessageMap(void) const {
	return &FrDemonProcessRank::_MsgMap;
}

sFRESH_MSGMAP FrDemonProcessRank::_MsgMap = { &FrForm::_MsgMap, &FrDemonProcessRank::_MsgEntries[0] };

// demonlanddlg.cpp:315
sFRESH_ENTRY FrDemonProcessRank::_MsgEntries[] = {
	{ "leaderpet", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonProcessRank::OnAreaLeaderPet_Init },
	{ "leaderpet", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonProcessRank::OnAreaLeaderPet_OwnerDraw },
	{ "attackerlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonProcessRank::OnListBoxUserInfoList_Init },
	{ "attackerlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonProcessRank::OnListBoxUserInfoList_OwnerDraw },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// demonlanddlg.cpp:324
FrDemonProcessRank::FrDemonProcessRank(void) : m_pUserInfoList(0) {
}

// demonlanddlg.cpp:329
FrDemonProcessRank::~FrDemonProcessRank(void) {
}

// demonlanddlg.cpp:335
bool FrDemonProcessRank::OnInit(void) {
	return FrForm::OnInit();
}

// demonlanddlg.cpp:342
void FrDemonProcessRank::OnAreaLeaderPet_Init(intptr_t param) {
	m_pLeaderPetArea = DYNAMIC_CAST(FrArea, param);
}

// demonlanddlg.cpp:360
void FrDemonProcessRank::OnAreaLeaderPet_OwnerDraw(int) {
}

// demonlanddlg.cpp:401
void FrDemonProcessRank::OnListBoxUserInfoList_Init(intptr_t param) {
	m_pUserInfoList = DYNAMIC_CAST(FrListBox, param);
}

// demonlanddlg.cpp:406
const Bitmap* GetTitleBitmap(const sRivalData* pInfo) {

	if (pInfo->m_titleTypeId != 0) {

		IFF_STRUCT::sSkin* pSkin = ItemManager()->FindSkin(pInfo->m_titleTypeId);
		if (pSkin) {

			return g_pFresh->GetManager()->GetBitmap("TITLES", pSkin->m_icon);
		}
		else {

			return g_pFresh->GetManager()->GetBitmap("LEVELS", MakeStr("level_%03d", pInfo->m_level + 1));
		}
	}

	return g_pFresh->GetManager()->GetBitmap("LEVELS", MakeStr("level_%03d", pInfo->m_level + 1));
}

// demonlanddlg.cpp:429
void FrDemonProcessRank::OnListBoxUserInfoList_OwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {
		FrListItem* pItem = (FrListItem*)param;



		if (pItem) {


			const sRivalData* pInfo = (const sRivalData*)pItem->pData;




			const Bitmap* pTitle = GetTitleBitmap(pInfo);
			WRect rc;	// read uninitialised by the original
			pGDI->DrawTexture(pTitle, WRect(rc.x, rc.y + 8.0f - (int)(rc.h * 0.5f), rc.w, rc.h), 0xFFFFFFFF, 0);


			if (pInfo->m_state == 3)
				pGDI->Print(WPoint(pItem->pos.x + 21.0f, pItem->pos.y + 3.0f), 1, "-");
			else
				pGDI->Print(WPoint(pItem->pos.x + 21.0f, pItem->pos.y + 3.0f), 1, "%d", pInfo->m_rank);






			if (pInfo->m_hasEmblem) {

				const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(pInfo->m_emblemName);

				if (emblem) {

					pGDI->DrawTexture(emblem, WRect(pItem->pos.x + 33.0f, pItem->pos.y + 3.0f - 6.0f, emblem->Width(), emblem->Height()), 0xFFFFFFFF, 0);
				}
			}




			g_pFresh->GetManager()->PrintText(WPoint(pItem->pos.x + 60.0f, pItem->pos.y + 3.0f), 0, pInfo->m_nickname, 90.0f, 0xFFFFFFFF);


			pGDI->Print(WPoint(pItem->pos.x + 179.0f, pItem->pos.y + 3.0f), 2, "%d", pInfo->m_score);


			pGDI->Print(WPoint(pItem->pos.x + 222.0f, pItem->pos.y + 3.0f), 2, "%I64d", pInfo->m_pang);



			if (pInfo->m_state == 2 || GetHoleIndex(pInfo->m_hole) == 0x13) {

				pGDI->Print(WPoint(pItem->pos.x + 255.0f, pItem->pos.y + 3.0f), 2, "End");
			}
			else if (pInfo->m_state == 3) {

				pGDI->Print(WPoint(pItem->pos.x + 255.0f, pItem->pos.y + 3.0f), 2, "Out");
			}
			else {

				pGDI->Print(WPoint(pItem->pos.x + 255.0f, pItem->pos.y + 3.0f), 2, "%d\xC8\xA6", GetHoleIndex(pInfo->m_hole));
			}
		}
	}
}

// demonlanddlg.cpp:511 (run-time type and factory registration of FrDemonFinalScore)
const WRTTI FrDemonFinalScore::m_RTTI("FrDemonFinalScore", &FrForm::m_RTTI);

IObject* FrDemonFinalScoreMakeInstance(void) {
	return new FrDemonFinalScore;
}

struct __sFrDemonFinalScore {
	__sFrDemonFinalScore(void) {
		ObjectFactory().AddObjectFunctor(FrDemonFinalScoreMakeInstance, "FrDemonFinalScore");
	}
};
static __sFrDemonFinalScore s_registerFrDemonFinalScore;

// demonlanddlg.cpp:513
const sFRESH_MSGMAP* FrDemonFinalScore::GetMessageMap(void) const {
	return &FrDemonFinalScore::_MsgMap;
}

sFRESH_MSGMAP FrDemonFinalScore::_MsgMap = { &FrForm::_MsgMap, &FrDemonFinalScore::_MsgEntries[0] };

// demonlanddlg.cpp:515
sFRESH_ENTRY FrDemonFinalScore::_MsgEntries[] = {
	{ "attackgauge", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonFinalScore::OnImageBarAttackPoint_Init },
	{ "giftlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonFinalScore::OnInit_Gift_List },
	{ "giftlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonFinalScore::OnOwnerDraw_Gift_List },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// demonlanddlg.cpp:524
FrDemonFinalScore::FrDemonFinalScore(void) : m_pAttackPointBar(0) {
}

// demonlanddlg.cpp:529
FrDemonFinalScore::~FrDemonFinalScore(void) {
}

// demonlanddlg.cpp:534
bool FrDemonFinalScore::OnInit(void) {
	if (!m_boxOpener.OnInit())
		return false;

	return FrForm::OnInit();
}

// demonlanddlg.cpp:545
void FrDemonFinalScore::OnProc(const float dt) {
	m_boxOpener.OnProc(dt);
}

// demonlanddlg.cpp:550
void FrDemonFinalScore::OnImageBarAttackPoint_Init(intptr_t param) {
	m_pAttackPointBar = DYNAMIC_CAST(FrGaugeBarImage, param);
	if (m_pAttackPointBar) {
		m_pAttackPointBar->SetRange(0, 1000, 0);
		m_pAttackPointBar->SetBarSpeed(250.0f);
		m_pAttackPointBar->SetDestPos(1000, false);
	}
}

// demonlanddlg.cpp:561
void FrDemonFinalScore::OnInit_Gift_List(intptr_t param) {
	m_boxOpener.m_pGiftList = DYNAMIC_CAST(FrListBox, param);
}

// demonlanddlg.cpp:566
void FrDemonFinalScore::OnOwnerDraw_Gift_List(intptr_t param) {
	if (param == 0)
		return;

	m_boxOpener.DrawGiftList((FrListItem*)param);
}

// demonlanddlg.cpp:584 (run-time type and factory registration of FrDemonLandTicket)
const WRTTI FrDemonLandTicket::m_RTTI("FrDemonLandTicket", &FrForm::m_RTTI);

IObject* FrDemonLandTicketMakeInstance(void) {
	return new FrDemonLandTicket;
}

struct __sFrDemonLandTicket {
	__sFrDemonLandTicket(void) {
		ObjectFactory().AddObjectFunctor(FrDemonLandTicketMakeInstance, "FrDemonLandTicket");
	}
};
static __sFrDemonLandTicket s_registerFrDemonLandTicket;

// demonlanddlg.cpp:586
const sFRESH_MSGMAP* FrDemonLandTicket::GetMessageMap(void) const {
	return &FrDemonLandTicket::_MsgMap;
}

sFRESH_MSGMAP FrDemonLandTicket::_MsgMap = { &FrForm::_MsgMap, &FrDemonLandTicket::_MsgEntries[0] };

// demonlanddlg.cpp:588
sFRESH_ENTRY FrDemonLandTicket::_MsgEntries[] = {
	{ "ticketmsg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDemonLandTicket::OnInit_TicketMsg_Static },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// demonlanddlg.cpp:595
FrDemonLandTicket::FrDemonLandTicket(void) : m_pTicketMsgStatic(0), m_ticketNumber(0) {
}

// demonlanddlg.cpp:600
FrDemonLandTicket::~FrDemonLandTicket(void) {
}

// demonlanddlg.cpp:605
void FrDemonLandTicket::SetTicketNumber(int ticketNumber) {
	m_ticketNumber = ticketNumber;

	if (m_pTicketMsgStatic) {
		// "Waiting number: %d. Please wait a moment."
		m_pTicketMsgStatic->SetCaption(MakeStr("\xb4\xeb\xb1\xe2\xb9\xf8\xc8\xa3 : %d \xc0\xd4\xb4\xcf\xb4\xd9. \xc0\xe1\xbd\xc3\xb8\xb8 \xb1\xe2\xb4\xd9\xb7\xc1 \xc1\xd6\xbc\xbc\xbf\xe4.", ticketNumber));
	}
}

// demonlanddlg.cpp:616
void FrDemonLandTicket::OnInit_TicketMsg_Static(intptr_t param) {
	m_pTicketMsgStatic = DYNAMIC_CAST(FrStatic, param);
	if (m_pTicketMsgStatic) {
		// "Requesting entry to the Demon King's land."
		m_pTicketMsgStatic->SetCaption("\xb8\xb6\xbf\xd5\xc0\xc7 \xb6\xa5 \xc0\xd4\xc0\xe5 \xbf\xe4\xc3\xbb \xc1\xdf \xc0\xd4\xb4\xcf\xb4\xd9.");
	}
}

// demonlanddlg.cpp:636
CDemonLandForm::CDemonLandForm() {
	m_pNextHole = 0;
	m_pAttackGauge = 0;
	m_pProcessRank = 0;
	m_pFinalScore = 0;
}

// demonlanddlg.cpp:641
CDemonLandForm::~CDemonLandForm() {
}

// demonlanddlg.cpp:646
bool CDemonLandForm::CreateDemonLandForm(eDemonDlg dlg) {
	switch (dlg) {
	case 0:
		m_pNextHole = CreateForm<FrDemonNextHole>(g_pFresh->GetManager(), 0, "demon_nexthole", 0);
		m_pNextHole->EnableDrag(false);
		m_pNextHole->Open((FRESH_RESULT_CALLBACK)0, 0xc);
		m_pNextHole->SetState(1);
		break;

	case 1:
		m_pAttackGauge = CreateForm<FrDemonAttackGauge>(g_pFresh->GetManager(), 0, "demon_attackgauge", 0);
		m_pAttackGauge->EnableDrag(false);
		m_pAttackGauge->Open((FRESH_RESULT_CALLBACK)0, 0xc);
		break;

	case 2:
		m_pProcessRank = CreateForm<FrDemonProcessRank>(g_pFresh->GetManager(), 0, "demon_processrank", 0);
		m_pProcessRank->EnableDrag(false);
		m_pProcessRank->SetFixed(true);
		m_pProcessRank->Open((FRESH_RESULT_CALLBACK)0, 0xc);
		break;

	case 3:
		break;

	default:
		return false;
	}

	return true;
}

// demonlanddlg.cpp:680
void CDemonLandForm::InitializeForm() {
	m_pNextHole = 0;
	m_pAttackGauge = 0;
	m_pProcessRank = 0;
	m_pFinalScore = 0;
}

// demonlanddlg.cpp:688
void CDemonLandForm::AdjustWindowPos() {
	if (m_pNextHole == 0 || m_pAttackGauge == 0)
		return;

	WRect rect;
	AdjustNextHolePos(rect);
	AdjustAttackGaugePos(rect);
}

// demonlanddlg.cpp:700
FrDemonNextHole* CDemonLandForm::GetDemonNextHole() {
	return m_pNextHole;
}

// demonlanddlg.cpp:705
FrDemonAttackGauge* CDemonLandForm::GetDemonAttackGauge() {
	return m_pAttackGauge;
}

// demonlanddlg.cpp:710
FrDemonProcessRank* CDemonLandForm::GetDemonProcessRank() {
	return m_pProcessRank;
}

// demonlanddlg.cpp:715
FrDemonFinalScore* CDemonLandForm::GetDemonFinalScore() {
	return m_pFinalScore;
}

// demonlanddlg.cpp:720
void CDemonLandForm::ReleaseForm() {
}

// demonlanddlg.cpp:725
void CDemonLandForm::AdjustNextHolePos(WRect& rect) {
	const WRect& wndRect = m_pNextHole->GetRect();

	rect.x = 0;
	rect.y = wndRect.h;
	rect.w = wndRect.w;
	rect.h = wndRect.h;

	float screenWidth = g_view->GetWidth();

	rect.x = screenWidth * 0.5f - rect.w;
	m_pNextHole->SetRect(rect);
}

// demonlanddlg.cpp:743
void CDemonLandForm::AdjustAttackGaugePos(WRect& rect) {
	const WRect& wndRect = m_pAttackGauge->GetRect();

	rect.x = 0;
	rect.y = wndRect.y;
	rect.w = wndRect.w;
	rect.h = wndRect.h;

	float screenWidth = g_view->GetWidth();

	rect.x = screenWidth * 0.5f + wndRect.w;
	m_pAttackGauge->SetRect(rect);
}

