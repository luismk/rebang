// Original: d:\build\custom\temp\code\client\projectg\frhalloweendlg.cpp
#include "FrHalloweenDlg.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frarea.h"
#include "frbutton.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "commonutil.h"
#include "frviewer.h"
#include "contentsdoc.h"
#include "SharedDoc.h"
#include <string.h>

// frhalloweendlg.cpp:11 (run-time type and factory registration of FrHalloweenEventGiftDlg)
const WRTTI FrHalloweenEventGiftDlg::m_RTTI("FrHalloweenEventGiftDlg", &FrForm::m_RTTI);

IObject* FrHalloweenEventGiftDlgMakeInstance(void) {
	return new FrHalloweenEventGiftDlg;
}

struct __sFrHalloweenEventGiftDlg {
	__sFrHalloweenEventGiftDlg(void) {
		ObjectFactory().AddObjectFunctor(FrHalloweenEventGiftDlgMakeInstance, "FrHalloweenEventGiftDlg");
	}
};
static __sFrHalloweenEventGiftDlg s_registerFrHalloweenEventGiftDlg;

// frhalloweendlg.cpp:13
const sFRESH_MSGMAP* FrHalloweenEventGiftDlg::GetMessageMap(void) const {
	return &FrHalloweenEventGiftDlg::_MsgMap;
}

sFRESH_MSGMAP FrHalloweenEventGiftDlg::_MsgMap = { &FrForm::_MsgMap, &FrHalloweenEventGiftDlg::_MsgEntries[0] };

sFRESH_ENTRY FrHalloweenEventGiftDlg::_MsgEntries[] = {
	// frhalloweendlg.cpp:15
	{ "giftframe_01", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrHalloweenEventGiftDlg::OnInitFrame01 },
	// frhalloweendlg.cpp:16
	{ "giftframe_01_cover", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrHalloweenEventGiftDlg::OnInitFrameCover },

	// frhalloweendlg.cpp:18
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frhalloweendlg.cpp:23
FrHalloweenEventGiftDlg::FrHalloweenEventGiftDlg(void)
	: m_pFrame01(0), m_pFrameCover(0) {
}

// frhalloweendlg.cpp:27
FrHalloweenEventGiftDlg::~FrHalloweenEventGiftDlg(void) {
}

// frhalloweendlg.cpp:31
bool FrHalloweenEventGiftDlg::OnInit(void) {
	return FrForm::OnInit();
}

// frhalloweendlg.cpp:36
void FrHalloweenEventGiftDlg::OnInitFrame01(intptr_t param) {
	m_pFrame01 = DYNAMIC_CAST(FrArea, param);

	if (m_pFrame01) {


		CHalloweenEvent* pEvent = (CHalloweenEvent*)WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x21);



		if (pEvent) {

			if (pEvent->GetGiftCnt() > 1) {
				m_pFrame01->Enable(false);
				m_pFrame01->SetVisible(false);
				return;
			}
		}

		m_pFrame01->Enable(true);
		m_pFrame01->SetVisible(true);
	}
}

// frhalloweendlg.cpp:66
void FrHalloweenEventGiftDlg::OnInitFrameCover(intptr_t param) {
	m_pFrameCover = DYNAMIC_CAST(FrArea, param);

	if (m_pFrameCover) {


		CHalloweenEvent* pEvent = (CHalloweenEvent*)WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x21);



		if (pEvent) {

			if (pEvent->GetGiftCnt() > 1) {
				m_pFrameCover->Enable(true);
				m_pFrameCover->SetVisible(true);
				return;
			}
		}

		m_pFrameCover->Enable(false);
		m_pFrameCover->SetVisible(false);
	}
}

// frhalloweendlg.cpp:99 (run-time type and factory registration of FrHalloweenEventDescDlg)
const WRTTI FrHalloweenEventDescDlg::m_RTTI("FrHalloweenEventDescDlg", &FrForm::m_RTTI);

IObject* FrHalloweenEventDescDlgMakeInstance(void) {
	return new FrHalloweenEventDescDlg;
}

struct __sFrHalloweenEventDescDlg {
	__sFrHalloweenEventDescDlg(void) {
		ObjectFactory().AddObjectFunctor(FrHalloweenEventDescDlgMakeInstance, "FrHalloweenEventDescDlg");
	}
};
static __sFrHalloweenEventDescDlg s_registerFrHalloweenEventDescDlg;

// frhalloweendlg.cpp:101
const sFRESH_MSGMAP* FrHalloweenEventDescDlg::GetMessageMap(void) const {
	return &FrHalloweenEventDescDlg::_MsgMap;
}

sFRESH_MSGMAP FrHalloweenEventDescDlg::_MsgMap = { &FrForm::_MsgMap, &FrHalloweenEventDescDlg::_MsgEntries[0] };

sFRESH_ENTRY FrHalloweenEventDescDlg::_MsgEntries[] = {
	// frhalloweendlg.cpp:103
	{ "desc_view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrHalloweenEventDescDlg::OnInitDescView },

	// frhalloweendlg.cpp:105
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frhalloweendlg.cpp:109
FrHalloweenEventDescDlg::FrHalloweenEventDescDlg(void) {
	m_pDescView = 0;
	m_bScroll = false;
}

// frhalloweendlg.cpp:115
FrHalloweenEventDescDlg::~FrHalloweenEventDescDlg(void) {
}

// frhalloweendlg.cpp:119
bool FrHalloweenEventDescDlg::OnInit(void) {
	if (m_pDescView) {


		bool bScroll = false;
		float offset = m_pDescView->GetScrollBarOffset() * 0.5f;


		int width = m_pDescView->GetSrcWidth();

		width -= (int)m_pDescView->GetViewWidth();

		int height = m_pDescView->GetSrcHeight();

		if (height > 0x1cc) {








			m_bScroll = bScroll = true;
			height = 0x1cc;
			m_pDescView->ShowScrollBar(true);
		} else {
			m_bScroll = false;
			m_pDescView->ShowScrollBar(false);
		}

		WRect rect = m_pDescView->GetRect();



		rect.w += width;
		height -= (int)m_pDescView->GetViewHeight();
		rect.h += height;
		if (bScroll) {
			rect.w -= offset * 2;
		}


		m_pDescView->SetRect(rect);
	}

	return FrForm::OnInit();
}

// frhalloweendlg.cpp:162
void FrHalloweenEventDescDlg::OnInitDescView(intptr_t param) {
	m_pDescView = DYNAMIC_CAST(FrViewer, param);

	if (m_pDescView) {

		m_pDescView->ShowScrollBar(false);
		m_pDescView->Open("hw_explanation.jpg");
	}
}

// frhalloweendlg.cpp:179 (run-time type and factory registration of FrHalloweenEventDlg)
const WRTTI FrHalloweenEventDlg::m_RTTI("FrHalloweenEventDlg", &FrForm::m_RTTI);

IObject* FrHalloweenEventDlgMakeInstance(void) {
	return new FrHalloweenEventDlg;
}

struct __sFrHalloweenEventDlg {
	__sFrHalloweenEventDlg(void) {
		ObjectFactory().AddObjectFunctor(FrHalloweenEventDlgMakeInstance, "FrHalloweenEventDlg");
	}
};
static __sFrHalloweenEventDlg s_registerFrHalloweenEventDlg;

// frhalloweendlg.cpp:181
const sFRESH_MSGMAP* FrHalloweenEventDlg::GetMessageMap(void) const {
	return &FrHalloweenEventDlg::_MsgMap;
}

sFRESH_MSGMAP FrHalloweenEventDlg::_MsgMap = { &FrForm::_MsgMap, &FrHalloweenEventDlg::_MsgEntries[0] };

sFRESH_ENTRY FrHalloweenEventDlg::_MsgEntries[] = {
	// frhalloweendlg.cpp:183
	{ "desc_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrHalloweenEventDlg::OnInitDescButton },
	// frhalloweendlg.cpp:184
	{ "desc_button", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrHalloweenEventDlg::OnLBDownDescButton },
	// frhalloweendlg.cpp:185
	{ "map_icon_area", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrHalloweenEventDlg::OnMapIconOwnerDraw },
	// frhalloweendlg.cpp:186
	{ "current_candy", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrHalloweenEventDlg::OnCurrentItemOwnerDraw },

	// frhalloweendlg.cpp:188
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frhalloweendlg.cpp:198
FrHalloweenEventDlg::FrHalloweenEventDlg(void)
	: m_pCurrentItemImage(0), m_pDescDlg(0), m_currentMapIndex(0) {
	memset(m_mapFlag, 0, 0x4f);   // 79 bytes: the original clears 3 bytes past the flags





	for (int i = 0; i < 4; i++) {
		m_currentItemCount[i] = 0;
		m_currentItemPos[i] = WPoint(0.0f, 0.0f);
	}
	memset(m_mapImage, 0, sizeof(m_mapImage));
}

// frhalloweendlg.cpp:211
FrHalloweenEventDlg::~FrHalloweenEventDlg(void) {
}

// frhalloweendlg.cpp:215
bool FrHalloweenEventDlg::OnInit(void) {
	m_mapFlag[0] = CMapTypeAdapter::MAP_FLAG_BLUE_LAGOON;
	m_mapFlag[1] = CMapTypeAdapter::MAP_FLAG_BLUE_WATER;
	m_mapFlag[2] = CMapTypeAdapter::MAP_FLAG_SEPIA_WIND;
	m_mapFlag[3] = CMapTypeAdapter::MAP_FLAG_WIND_HILL;
	m_mapFlag[4] = CMapTypeAdapter::MAP_FLAG_WIZ_WIZ;
	m_mapFlag[5] = CMapTypeAdapter::MAP_FLAG_WEST_WIZ;
	m_mapFlag[6] = CMapTypeAdapter::MAP_FLAG_BLUE_MOON;
	m_mapFlag[7] = CMapTypeAdapter::MAP_FLAG_SILVIA_CANNON;
	m_mapFlag[8] = CMapTypeAdapter::MAP_FLAG_ICE_CANNON;
	m_mapFlag[9] = CMapTypeAdapter::MAP_FLAG_WHITE_WIZ;
	m_mapFlag[10] = CMapTypeAdapter::MAP_FLAG_SHINING_SAND;
	m_mapFlag[11] = CMapTypeAdapter::MAP_FLAG_PINK_WIND;
	m_mapFlag[13] = CMapTypeAdapter::MAP_FLAG_DEEP_INFERNO;
	m_mapFlag[14] = CMapTypeAdapter::MAP_FLAG_ICE_SPA;




	m_mapImage[0].m_pImage = g_pFresh->GetBitmap("map_bluelagoon_on"); m_mapImage[0].m_pImageOver = g_pFresh->GetBitmap("map_bluelagoon_off");
	m_mapImage[1].m_pImage = g_pFresh->GetBitmap("map_bluewater_on"); m_mapImage[1].m_pImageOver = g_pFresh->GetBitmap("map_bluewater_off");
	m_mapImage[2].m_pImage = g_pFresh->GetBitmap("map_sepiawind_on"); m_mapImage[2].m_pImageOver = g_pFresh->GetBitmap("map_sepiawind_off");
	m_mapImage[3].m_pImage = g_pFresh->GetBitmap("map_windhill_on"); m_mapImage[3].m_pImageOver = g_pFresh->GetBitmap("map_windhill_off");
	m_mapImage[4].m_pImage = g_pFresh->GetBitmap("map_wizwiz_on"); m_mapImage[4].m_pImageOver = g_pFresh->GetBitmap("map_wizwiz_off");
	m_mapImage[5].m_pImage = g_pFresh->GetBitmap("map_westwiz_on"); m_mapImage[5].m_pImageOver = g_pFresh->GetBitmap("map_westwiz_off");
	m_mapImage[6].m_pImage = g_pFresh->GetBitmap("map_bluemoon_on"); m_mapImage[6].m_pImageOver = g_pFresh->GetBitmap("map_bluemoon_off");
	m_mapImage[7].m_pImage = g_pFresh->GetBitmap("map_silviacannon_on"); m_mapImage[7].m_pImageOver = g_pFresh->GetBitmap("map_silviacannon_off");
	m_mapImage[8].m_pImage = g_pFresh->GetBitmap("map_icecannon_on"); m_mapImage[8].m_pImageOver = g_pFresh->GetBitmap("map_icecannon_off");
	m_mapImage[9].m_pImage = g_pFresh->GetBitmap("map_whitewiz_on"); m_mapImage[9].m_pImageOver = g_pFresh->GetBitmap("map_whitewiz_off");
	m_mapImage[10].m_pImage = g_pFresh->GetBitmap("map_shining_on"); m_mapImage[10].m_pImageOver = g_pFresh->GetBitmap("map_shining_off");
	m_mapImage[11].m_pImage = g_pFresh->GetBitmap("map_pinkwind_on"); m_mapImage[11].m_pImageOver = g_pFresh->GetBitmap("map_pinkwind_off");
	m_mapImage[13].m_pImage = g_pFresh->GetBitmap("map_deepinferno_on"); m_mapImage[13].m_pImageOver = g_pFresh->GetBitmap("map_deepinferno_off");
	m_mapImage[14].m_pImage = g_pFresh->GetBitmap("map_icespa_on"); m_mapImage[14].m_pImageOver = g_pFresh->GetBitmap("map_icespa_off");



	m_mapImageRect[9] = WRect(282.0f, 199.0f, 328.0f, 108.0f);
	m_mapImageRect[10] = WRect(463.0f, 302.0f, 516.0f, 218.0f);
	m_mapImageRect[4] = WRect(214.0f, 199.0f, 245.0f, 208.0f);
	m_mapImageRect[13] = WRect(332.0f, 260.0f, 393.0f, 178.0f);
	m_mapImageRect[0] = WRect(296.0f, 399.0f, 336.0f, 420.0f);
	m_mapImageRect[7] = WRect(236.0f, 467.0f, 293.0f, 382.0f);
	m_mapImageRect[1] = WRect(207.0f, 430.0f, 242.0f, 334.0f);
	m_mapImageRect[6] = WRect(79.0f, 377.0f, 178.0f, 323.0f);
	m_mapImageRect[11] = WRect(209.0f, 336.0f, 257.0f, 259.0f);
	m_mapImageRect[3] = WRect(196.0f, 271.0f, 240.0f, 190.0f);
	m_mapImageRect[2] = WRect(120.0f, 309.0f, 177.0f, 236.0f);
	m_mapImageRect[8] = WRect(45.0f, 258.0f, 103.0f, 189.0f);
	m_mapImageRect[5] = WRect(141.0f, 225.0f, 184.0f, 150.0f);
	m_mapImageRect[14] = WRect(65.0f, 199.0f, 120.0f, 208.0f);



	m_mapSubRect[9] = WRect(286.0f, 198.0f, 322.0f, 212.0f);
	m_mapSubRect[10] = WRect(467.0f, 307.0f, 490.0f, 321.0f);
	m_mapSubRect[4] = WRect(209.0f, 198.0f, 245.0f, 207.0f);
	m_mapSubRect[13] = WRect(334.0f, 265.0f, 370.0f, 279.0f);
	m_mapSubRect[0] = WRect(292.0f, 399.0f, 328.0f, 416.0f);
	m_mapSubRect[7] = WRect(249.0f, 472.0f, 285.0f, 486.0f);
	m_mapSubRect[1] = WRect(197.0f, 422.0f, 233.0f, 436.0f);
	m_mapSubRect[6] = WRect(104.0f, 386.0f, 115.0f, 387.0f);
	m_mapSubRect[11] = WRect(209.0f, 348.0f, 245.0f, 362.0f);
	m_mapSubRect[3] = WRect(205.0f, 275.0f, 241.0f, 289.0f);
	m_mapSubRect[2] = WRect(128.0f, 320.0f, 164.0f, 334.0f);
	m_mapSubRect[8] = WRect(54.0f, 258.0f, 90.0f, 272.0f);
	m_mapSubRect[5] = WRect(142.0f, 234.0f, 178.0f, 248.0f);
	m_mapSubRect[14] = WRect(74.0f, 198.0f, 110.0f, 207.0f);



	m_pMarkImage = g_pFresh->GetBitmap("hw_mark");



	m_pCurrentItemImage = g_pFresh->GetBitmap("hw_ticket_frame");


	m_currentItemPos[0] = WPoint(115.0f, 60.0f);
	m_currentItemPos[1] = WPoint(115.0f, 77.0f);
	m_currentItemPos[2] = WPoint(115.0f, 94.0f);
	m_currentItemPos[3] = WPoint(115.0f, 111.0f);
	m_itemFramePos = WPoint(659.0f, 496.0f);





	// The four candy item types counted by the window.
	unsigned long candyType[4];
	candyType[0] = 0x49;
	candyType[1] = 0x4b;
	candyType[2] = 0x4a;

	candyType[3] = 0x4c;

	for (int i = 0; i < 4; i++) {
		CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance(); std::list<sItemInfo>::iterator it = pDoc->m_itemInfoList.begin(); while (it != pDoc->m_itemInfoList.end() && (*it).m_typeId != (candyType[i] | 0x1A000000)) ++it;
		if (it != pDoc->m_itemInfoList.end()) {

			m_currentItemCount[i] = (*it).m_count;

		} else {

			m_currentItemCount[i] = 0;
		}
	}


	return true;
}

// frhalloweendlg.cpp:327
void FrHalloweenEventDlg::OnInitDescButton(intptr_t param) {
	m_pDescButton = DYNAMIC_CAST(FrButton, param);


	m_pDescButton->SetPushDelay(0.0f);
	WRect rect = m_pDescButton->GetRect();
	rect.x = m_rect.x + 352.0f;
	rect.y = m_rect.y + 151.0f;

	m_pDescButton->SetRect(rect);
}

// frhalloweendlg.cpp:339
void FrHalloweenEventDlg::OnLBDownDescButton(void) {
	if (m_pDescDlg == 0) {

		m_pDescDlg = CreateForm<FrHalloweenEventDescDlg>(g_pFresh->GetManager(), this, "halloween2007_event_desc_dlg", 0);

		if (m_pDescDlg) {

			WRect rect = m_pDescButton->GetRect();

			m_pDescDlg->SetIconRect(rect);
			m_pDescDlg->Open((FRESH_RESULT_CALLBACK)&FrHalloweenEventDlg::OnDescDlgResult, 0x11);
		}
	}
}

// frhalloweendlg.cpp:355
bool FrHalloweenEventDlg::OnDescDlgResult(int result, FrForm* pForm) {
	m_pDescDlg = 0;
	return true;
}

// frhalloweendlg.cpp:361
void FrHalloweenEventDlg::SetMapImage(eMapType type, const char* image, const char* imageOver) {
	CMapTypeAdapter::eMapIndex index = CMapTypeAdapter::GetMapIndex(type);
	m_mapImage[index].m_pImage = g_pFresh->GetBitmap(image);
	m_mapImage[index].m_pImageOver = g_pFresh->GetBitmap(imageOver);
}

// frhalloweendlg.cpp:369
void FrHalloweenEventDlg::SetMapImageRect(eMapType type, WRect& rect) {
	m_mapImageRect[CMapTypeAdapter::GetMapIndex(type)] = rect;
}

// frhalloweendlg.cpp:374
void FrHalloweenEventDlg::SetMapFlagVector(eMapType type) {
	m_mapFlag[CMapTypeAdapter::GetMapIndex(type)] = CMapTypeAdapter::GetMapFlag(type);
}

// frhalloweendlg.cpp:380 (body empty in this build)
void FrHalloweenEventDlg::DrawMapImage(void) {
}

// frhalloweendlg.cpp:437
void FrHalloweenEventDlg::OnMapIconOwnerDraw(void) {
}

// frhalloweendlg.cpp:442
void FrHalloweenEventDlg::OnCurrentItemOwnerDraw(int param) {
	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();

	if (pDevice) {

		int oldSpace = pDevice->SetSpace(0);
		WRect rect = m_rect;


		float w = (float)m_pCurrentItemImage->Width();
		float h = (float)m_pCurrentItemImage->Height();


		WRect dest(rect.x - w + 40.0f, rect.y + (rect.h - h) * 0.5f, w, h);
		pDevice->DrawTexture(m_pCurrentItemImage, dest, 0xffffffff, 0);




		pDevice->SetTextColor(0xffffff00, 0xffffffff);

		for (int i = 0; i < 4; i++) {
			WPoint pos = m_currentItemPos[i];




			pos.x += dest.x;
			pos.y += dest.y;
			pDevice->PrintText(pos, 2, MakeStr("%d", m_currentItemCount[i]), 0);
		}

		pDevice->SetTextColor(0xff000000, 0xffffffff);













		pDevice->SetSpace(oldSpace);
	}
}
