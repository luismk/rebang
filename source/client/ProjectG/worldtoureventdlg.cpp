// Original: d:\build\custom\temp\code\client\projectg\worldtoureventdlg.cpp
#include "WorldTourEventDlg.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frarea.h"
#include "frbutton.h"
#include "frviewer.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "contentsdoc.h"
#include "Packet.h"
#include "SharedDoc.h"
#include <string.h>

// worldtoureventdlg.cpp:11 (run-time type and factory registration of FrWorldTourEventDlg)
const WRTTI FrWorldTourEventDlg::m_RTTI("FrWorldTourEventDlg", &FrForm::m_RTTI);

IObject* FrWorldTourEventDlgMakeInstance(void) {
	return new FrWorldTourEventDlg;
}

struct __sFrWorldTourEventDlg {
	__sFrWorldTourEventDlg(void) {
		ObjectFactory().AddObjectFunctor(FrWorldTourEventDlgMakeInstance, "FrWorldTourEventDlg");
	}
};
static __sFrWorldTourEventDlg s_registerFrWorldTourEventDlg;

// worldtoureventdlg.cpp:13
const sFRESH_MSGMAP* FrWorldTourEventDlg::GetMessageMap(void) const {
	return &FrWorldTourEventDlg::_MsgMap;
}

sFRESH_MSGMAP FrWorldTourEventDlg::_MsgMap = { &FrForm::_MsgMap, &FrWorldTourEventDlg::_MsgEntries[0] };

sFRESH_ENTRY FrWorldTourEventDlg::_MsgEntries[] = {
	// worldtoureventdlg.cpp:15
	{ "map_icon_area", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrWorldTourEventDlg::OnMapIconOwnerDraw },

	// worldtoureventdlg.cpp:17
	{ "show_desc_left_top", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrWorldTourEventDlg::OnInitGiftImageLeftTop },
	// worldtoureventdlg.cpp:18
	{ "show_desc_right_top", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrWorldTourEventDlg::OnInitGiftImageRightTop },
	// worldtoureventdlg.cpp:19
	{ "show_desc_left_bottom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrWorldTourEventDlg::OnInitGiftImageLeftBottom },
	// worldtoureventdlg.cpp:20
	{ "show_desc_right_bottom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrWorldTourEventDlg::OnInitGiftImageRightBottom },

	// worldtoureventdlg.cpp:22
	{ "desc_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrWorldTourEventDlg::OnInitDescButton },
	// worldtoureventdlg.cpp:23
	{ "desc_button", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrWorldTourEventDlg::OnLBDownDescButton },
	// worldtoureventdlg.cpp:24
	{ "gift_button_cover", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrWorldTourEventDlg::OnInitGiftButtonCover },
	// worldtoureventdlg.cpp:25
	{ "gift_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrWorldTourEventDlg::OnInitGiftButton },
	// worldtoureventdlg.cpp:26
	{ "gift_button", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrWorldTourEventDlg::OnLBDownGiftButton },
	// worldtoureventdlg.cpp:27
	{ "course_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrWorldTourEventDlg::OnInitCourseButton },
	// worldtoureventdlg.cpp:28
	{ "course_button", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrWorldTourEventDlg::OnLBDownCourseButton },

	// worldtoureventdlg.cpp:30
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// worldtoureventdlg.cpp:38
FrWorldTourEventDlg::FrWorldTourEventDlg(void) {
	ClearVariables();
}

// worldtoureventdlg.cpp:44
FrWorldTourEventDlg::~FrWorldTourEventDlg(void) {
}

// worldtoureventdlg.cpp:49
void FrWorldTourEventDlg::ClearVariables(void) {
	m_time = 0;

	m_bInit = false;
	m_mapFlag = 0;

	memset(m_mapFlagVector, 0, sizeof(m_mapFlagVector));
	memset(m_mapImageRect, 0, sizeof(m_mapImageRect));
	memset(m_mapImage, 0, sizeof(m_mapImage));

	m_pStampImage = 0;

	m_pDescButton = 0;
	m_pCourseButton = 0;
	m_pGiftButton = 0;

	m_pGiftImageLeftTop = 0;
	m_pGiftImageRightTop = 0;
	m_pGiftImageLeftBottom = 0;
	m_pGiftImageRightBottom = 0;

	m_pGiftDlg = 0;
	m_pDescDlg = 0;
}

// worldtoureventdlg.cpp:76
void FrWorldTourEventDlg::SetMapFlag(unsigned long mapFlag) {
	m_mapFlag = mapFlag;
}

// worldtoureventdlg.cpp:88
void FrWorldTourEventDlg::SetMapFlagVector(eMapType type) {
	m_mapFlagVector[CMapTypeAdapter::GetMapIndex(type)] = CMapTypeAdapter::GetMapFlag(type);
}

// worldtoureventdlg.cpp:93
void FrWorldTourEventDlg::SetMapImageRect(eMapType type, WRect& rect) {
	m_mapImageRect[CMapTypeAdapter::GetMapIndex(type)] = rect;
}

// worldtoureventdlg.cpp:99
void FrWorldTourEventDlg::SetMapImage(eMapType type, const char* image, const char* imageOver) {
	CMapTypeAdapter::eMapIndex index = CMapTypeAdapter::GetMapIndex(type);
	m_mapImage[index].m_pImage = g_pFresh->GetBitmap(image);
	m_mapImage[index].m_pImageOver = g_pFresh->GetBitmap(imageOver);
}

// worldtoureventdlg.cpp:106
bool FrWorldTourEventDlg::OnInit(void) {
	// Course bits, image rectangles, images and stamp rectangles, indexed by
	// course id (MAP_WIZ_CITY, 19, writes past the 17-slot tables).
	m_mapFlagVector[MAP_BLUE_LAGOON] = CMapTypeAdapter::MAP_FLAG_BLUE_LAGOON;
	m_mapFlagVector[MAP_BLUE_WATER] = CMapTypeAdapter::MAP_FLAG_BLUE_WATER;
	m_mapFlagVector[MAP_SEPIA_WIND] = CMapTypeAdapter::MAP_FLAG_SEPIA_WIND;
	m_mapFlagVector[MAP_WIND_HILL] = CMapTypeAdapter::MAP_FLAG_WIND_HILL;
	m_mapFlagVector[MAP_WIZ_WIZ] = CMapTypeAdapter::MAP_FLAG_WIZ_WIZ;
	m_mapFlagVector[MAP_WEST_WIZ] = CMapTypeAdapter::MAP_FLAG_WEST_WIZ;
	m_mapFlagVector[MAP_BLUE_MOON] = CMapTypeAdapter::MAP_FLAG_BLUE_MOON;
	m_mapFlagVector[MAP_SILVIA_CANNON] = CMapTypeAdapter::MAP_FLAG_SILVIA_CANNON;
	m_mapFlagVector[MAP_ICE_CANNON] = CMapTypeAdapter::MAP_FLAG_ICE_CANNON;
	m_mapFlagVector[MAP_WHITE_WIZ] = CMapTypeAdapter::MAP_FLAG_WHITE_WIZ;
	m_mapFlagVector[MAP_SHINING_SAND] = CMapTypeAdapter::MAP_FLAG_SHINING_SAND;
	m_mapFlagVector[MAP_PINK_WIND] = CMapTypeAdapter::MAP_FLAG_PINK_WIND;
	m_mapFlagVector[MAP_DEEP_INFERNO] = CMapTypeAdapter::MAP_FLAG_DEEP_INFERNO;
	m_mapFlagVector[MAP_ICE_SPA] = CMapTypeAdapter::MAP_FLAG_ICE_SPA;
	m_mapFlagVector[MAP_LOST_SEAWAY] = CMapTypeAdapter::MAP_FLAG_LOST_SEAWAY;
	m_mapFlagVector[MAP_EASTERN_VALLEY] = CMapTypeAdapter::MAP_FLAG_EASTERN_VALLEY;
	m_mapFlagVector[MAP_WIZ_CITY] = CMapTypeAdapter::MAP_FLAG_WIZ_CITY;
	m_mapImageRect[MAP_BLUE_LAGOON] = WRect(296.0f, 410.0f, 0, 0);
	m_mapImageRect[MAP_BLUE_WATER] = WRect(218.0f, 424.0f, 0, 0);
	m_mapImageRect[MAP_SEPIA_WIND] = WRect(139.0f, 310.0f, 0, 0);
	m_mapImageRect[MAP_WIND_HILL] = WRect(214.0f, 272.0f, 0, 0);
	m_mapImageRect[MAP_WIZ_WIZ] = WRect(229.0f, 216.0f, 0, 0);
	m_mapImageRect[MAP_WEST_WIZ] = WRect(173.0f, 222.0f, 0, 0);
	m_mapImageRect[MAP_BLUE_MOON] = WRect(116.0f, 385.0f, 0, 0);
	m_mapImageRect[MAP_SILVIA_CANNON] = WRect(223.0f, 470.0f, 0, 0);
	m_mapImageRect[MAP_ICE_CANNON] = WRect(51.0f, 257.0f, 0, 0);
	m_mapImageRect[MAP_WHITE_WIZ] = WRect(275.0f, 210.0f, 0, 0);
	m_mapImageRect[MAP_SHINING_SAND] = WRect(490.0f, 293.0f, 0, 0);
	m_mapImageRect[MAP_PINK_WIND] = WRect(206.0f, 353.0f, 0, 0);
	m_mapImageRect[MAP_DEEP_INFERNO] = WRect(354.0f, 263.0f, 0, 0);
	m_mapImageRect[MAP_ICE_SPA] = WRect(94.0f, 205.0f, 0, 0);
	m_mapImageRect[MAP_LOST_SEAWAY] = WRect(440.0f, 375.0f, 0, 0);
	m_mapImageRect[MAP_EASTERN_VALLEY] = WRect(359.0f, 349.0f, 0, 0);
	m_mapImageRect[MAP_WIZ_CITY] = WRect(0.0f, 0.0f, 0, 0);
	m_mapImage[MAP_BLUE_LAGOON].m_pImage = g_pFresh->GetBitmap("map_bluelagoon_on");
	m_mapImage[MAP_BLUE_LAGOON].m_pImageOver = g_pFresh->GetBitmap("map_bluelagoon_off");
	m_mapImage[MAP_BLUE_WATER].m_pImage = g_pFresh->GetBitmap("map_bluewater_on");
	m_mapImage[MAP_BLUE_WATER].m_pImageOver = g_pFresh->GetBitmap("map_bluewater_off");
	m_mapImage[MAP_SEPIA_WIND].m_pImage = g_pFresh->GetBitmap("map_sepiawind_on");
	m_mapImage[MAP_SEPIA_WIND].m_pImageOver = g_pFresh->GetBitmap("map_sepiawind_off");
	m_mapImage[MAP_WIND_HILL].m_pImage = g_pFresh->GetBitmap("map_windhill_on");
	m_mapImage[MAP_WIND_HILL].m_pImageOver = g_pFresh->GetBitmap("map_windhill_off");
	m_mapImage[MAP_WIZ_WIZ].m_pImage = g_pFresh->GetBitmap("map_wizwiz_on");
	m_mapImage[MAP_WIZ_WIZ].m_pImageOver = g_pFresh->GetBitmap("map_wizwiz_off");
	m_mapImage[MAP_WEST_WIZ].m_pImage = g_pFresh->GetBitmap("map_westwiz_on");
	m_mapImage[MAP_WEST_WIZ].m_pImageOver = g_pFresh->GetBitmap("map_westwiz_off");
	m_mapImage[MAP_BLUE_MOON].m_pImage = g_pFresh->GetBitmap("map_bluemoon_on");
	m_mapImage[MAP_BLUE_MOON].m_pImageOver = g_pFresh->GetBitmap("map_bluemoon_off");
	m_mapImage[MAP_SILVIA_CANNON].m_pImage = g_pFresh->GetBitmap("map_silviacannon_on");
	m_mapImage[MAP_SILVIA_CANNON].m_pImageOver = g_pFresh->GetBitmap("map_silviacannon_off");
	m_mapImage[MAP_ICE_CANNON].m_pImage = g_pFresh->GetBitmap("map_icecannon_on");
	m_mapImage[MAP_ICE_CANNON].m_pImageOver = g_pFresh->GetBitmap("map_icecannon_off");
	m_mapImage[MAP_WHITE_WIZ].m_pImage = g_pFresh->GetBitmap("map_whitewiz_on");
	m_mapImage[MAP_WHITE_WIZ].m_pImageOver = g_pFresh->GetBitmap("map_whitewiz_off");
	m_mapImage[MAP_SHINING_SAND].m_pImage = g_pFresh->GetBitmap("map_shining_on");
	m_mapImage[MAP_SHINING_SAND].m_pImageOver = g_pFresh->GetBitmap("map_shining_off");
	m_mapImage[MAP_PINK_WIND].m_pImage = g_pFresh->GetBitmap("map_pinkwind_on");
	m_mapImage[MAP_PINK_WIND].m_pImageOver = g_pFresh->GetBitmap("map_pinkwind_off");
	m_mapImage[MAP_DEEP_INFERNO].m_pImage = g_pFresh->GetBitmap("map_deepinferno_on");
	m_mapImage[MAP_DEEP_INFERNO].m_pImageOver = g_pFresh->GetBitmap("map_deepinferno_off");
	m_mapImage[MAP_ICE_SPA].m_pImage = g_pFresh->GetBitmap("map_icespa_on");
	m_mapImage[MAP_ICE_SPA].m_pImageOver = g_pFresh->GetBitmap("map_icespa_off");
	m_mapImage[MAP_LOST_SEAWAY].m_pImage = g_pFresh->GetBitmap("map_lostseaway_on");
	m_mapImage[MAP_LOST_SEAWAY].m_pImageOver = g_pFresh->GetBitmap("map_lostseaway_off");
	m_mapImage[MAP_EASTERN_VALLEY].m_pImage = g_pFresh->GetBitmap("map_esternvalley_on");
	m_mapImage[MAP_EASTERN_VALLEY].m_pImageOver = g_pFresh->GetBitmap("map_esternvalley_off");
	m_mapImage[MAP_WIZ_CITY].m_pImage = g_pFresh->GetBitmap("map_lostseaway_on");
	m_mapImage[MAP_WIZ_CITY].m_pImageOver = g_pFresh->GetBitmap("map_lostseaway_off");
	m_stampRect[MAP_BLUE_LAGOON] = WRect(293.0f, 418.0f, 0, 0);
	m_stampRect[MAP_BLUE_WATER] = WRect(200.0f, 427.0f, 0, 0);
	m_stampRect[CMapTypeAdapter::GetMapIndex(MAP_SEPIA_WIND)] = WRect(159.0f, 316.0f, 0, 0);
	m_stampRect[MAP_WIND_HILL] = WRect(217.0f, 283.0f, 0, 0);
	m_stampRect[MAP_WIZ_WIZ] = WRect(230.0f, 229.0f, 0, 0);
	m_stampRect[MAP_WEST_WIZ] = WRect(175.0f, 241.0f, 0, 0);
	m_stampRect[MAP_BLUE_MOON] = WRect(120.0f, 393.0f, 0, 0);
	m_stampRect[MAP_SILVIA_CANNON] = WRect(242.0f, 488.0f, 0, 0);
	m_stampRect[MAP_ICE_CANNON] = WRect(65.0f, 266.0f, 0, 0);
	m_stampRect[MAP_WHITE_WIZ] = WRect(281.0f, 215.0f, 0, 0);
	m_stampRect[MAP_SHINING_SAND] = WRect(502.0f, 319.0f, 0, 0);
	m_stampRect[MAP_PINK_WIND] = WRect(212.0f, 366.0f, 0, 0);
	m_stampRect[MAP_DEEP_INFERNO] = WRect(368.0f, 288.0f, 0, 0);
	m_stampRect[MAP_ICE_SPA] = WRect(105.0f, 221.0f, 0, 0);
	m_stampRect[MAP_LOST_SEAWAY] = WRect(450.0f, 385.0f, 0, 0);
	m_stampRect[MAP_EASTERN_VALLEY] = WRect(359.0f, 349.0f, 0, 0);
	m_stampRect[MAP_WIZ_CITY] = WRect(0.0f, 385.0f, 0, 0);

	m_pStampImage = g_pFresh->GetBitmap("wte_mark");

	return true;
}

// worldtoureventdlg.cpp:216
void FrWorldTourEventDlg::OnProc(const float dt) {
	m_time += dt;
}

// worldtoureventdlg.cpp:221
void FrWorldTourEventDlg::DrawMapImage(void) {


	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	if (pDevice) {

		WRect rect = m_rect;

		for (int i = 0; i < MAP_SLOT_COUNT; i++) {








			const Bitmap* pImage;
			if (m_mapFlagVector[i] & m_mapFlag) {

				pImage = m_mapImage[i].m_pImage;

			} else {

				pImage = m_mapImage[i].m_pImageOver;
			}

			if (pImage) {





				pDevice->DrawTexture(pImage, WRect(rect.x + m_mapImageRect[i].x, rect.y + m_mapImageRect[i].y, pImage->Width(), pImage->Height()), 0xffffffff, 0);
			}

			if (m_mapFlagVector[i] & m_mapFlag) {


				if (m_pStampImage) {





					pDevice->DrawTexture(m_pStampImage, WRect(rect.x + m_stampRect[i].x, rect.y + m_stampRect[i].y, m_pStampImage->Width(), m_pStampImage->Height()), 0xffffffff, 0);
				}
			}
		}
	}
}

// worldtoureventdlg.cpp:270
void FrWorldTourEventDlg::OnMapIconOwnerDraw(void) {
	DrawMapImage();

	if (m_mapFlag == 0xffff) {

		const Bitmap* pStamp = g_pFresh->GetBitmap("stamp_good_big");
		if (pStamp) {

			WRect rect(m_rect.x + 217.0f, m_rect.y + 122.0f, pStamp->Width(), pStamp->Height());


			g_pFresh->GetManager()->GetGDI()->DrawTexture(pStamp, rect, 0xffffffff, 0);
		}
	}
}

// worldtoureventdlg.cpp:288
void FrWorldTourEventDlg::OnInitGiftImageLeftTop(intptr_t param) {
	m_pGiftImageLeftTop = DYNAMIC_CAST(FrArea, param);
	m_pGiftImageLeftTop->SetVisible(false);
}

// worldtoureventdlg.cpp:294
void FrWorldTourEventDlg::OnInitGiftImageRightTop(intptr_t param) {
	m_pGiftImageRightTop = DYNAMIC_CAST(FrArea, param);
	m_pGiftImageRightTop->SetVisible(false);
}

// worldtoureventdlg.cpp:300
void FrWorldTourEventDlg::OnInitGiftImageLeftBottom(intptr_t param) {
	m_pGiftImageLeftBottom = DYNAMIC_CAST(FrArea, param);
	m_pGiftImageLeftBottom->SetVisible(false);
}

// worldtoureventdlg.cpp:306
void FrWorldTourEventDlg::OnInitGiftImageRightBottom(intptr_t param) {
	m_pGiftImageRightBottom = DYNAMIC_CAST(FrArea, param);
	m_pGiftImageRightBottom->SetVisible(false);
}

// worldtoureventdlg.cpp:312
void FrWorldTourEventDlg::OnInitDescButton(intptr_t param) {
	m_pDescButton = DYNAMIC_CAST(FrButton, param);


	m_pDescButton->SetPushDelay(0.0f);
}

// worldtoureventdlg.cpp:320
void FrWorldTourEventDlg::OnLBDownDescButton(void) {

	if (m_pDescDlg == 0) {

		m_pDescDlg = CreateForm<FrWorldTourEventDescDlg>(g_pFresh->GetManager(), this, "world_tour_event_desc_dlg", 0);

		if (m_pDescDlg) {

			WRect rect = m_pDescButton->GetRect();

			m_pDescDlg->SetIconRect(rect);
			m_pDescDlg->Open((FRESH_RESULT_CALLBACK)&FrWorldTourEventDlg::OnDescDlgResult, 0x11);
		}
	}
}

// worldtoureventdlg.cpp:350
void FrWorldTourEventDlg::OnInitCourseButton(intptr_t param) {
	m_pCourseButton = DYNAMIC_CAST(FrButton, param);
}

// worldtoureventdlg.cpp:355
void FrWorldTourEventDlg::OnLBDownCourseButton(void) {

	if (m_pDescDlg == 0) {

		if (m_pGiftImageLeftTop) m_pGiftImageLeftTop->SetVisible(false);
		if (m_pGiftImageRightTop) m_pGiftImageRightTop->SetVisible(false);
		if (m_pGiftImageLeftBottom) m_pGiftImageLeftBottom->SetVisible(false);
		if (m_pGiftImageRightBottom) m_pGiftImageRightBottom->SetVisible(false);
	}
}

// worldtoureventdlg.cpp:367
void FrWorldTourEventDlg::OnInitGiftButtonCover(intptr_t param) {
	m_pGiftButtonCover = DYNAMIC_CAST(FrArea, param);
	if (m_pGiftButtonCover) {

		m_pGiftButtonCover->SetVisible(false);
	}
}

// worldtoureventdlg.cpp:376
void FrWorldTourEventDlg::OnInitGiftButton(intptr_t param) {
	m_pGiftButton = DYNAMIC_CAST(FrButton, param);


	m_pGiftButton->SetPushDelay(0.0f);


	CWorldTourEvent* pEvent = (CWorldTourEvent*)WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x1e);
	if (pEvent) {



		if (!pEvent->GetCanGift()) {

			m_pGiftButton->Enable(false);
			m_pGiftButton->SetVisible(false);
		}
	}

	if (m_pGiftButton && m_pGiftButtonCover) {

		if (!m_pGiftButton->IsEnabled() || !m_pGiftButton->IsVisible()) {

			m_pGiftButtonCover->Enable(false);
			m_pGiftButtonCover->SetVisible(false);
		}
	}
}

// worldtoureventdlg.cpp:412
void FrWorldTourEventDlg::OnLBDownGiftButton(void) {
	if (m_pGiftDlg == 0) {

		m_pGiftDlg = CreateForm<FrWorldTourEventGiftDlg>(g_pFresh->GetManager(), this, "world_tour_event_gift_dlg", 0);

		if (m_pGiftDlg) {

			m_pGiftButtonCover->Enable(true);
			m_pGiftButtonCover->SetVisible(true);

			WRect rect = m_pGiftButton->GetRect();

			m_pGiftDlg->SetIconRect(rect);
			m_pGiftDlg->Open((FRESH_RESULT_CALLBACK)&FrWorldTourEventDlg::OnGiftDlgResult, 0x11);
		}
	}
}

// worldtoureventdlg.cpp:432
bool FrWorldTourEventDlg::OnDescDlgResult(int result, FrForm* pForm) {
	m_pDescDlg = 0;
	return true;
}

// worldtoureventdlg.cpp:438
bool FrWorldTourEventDlg::OnGiftDlgResult(int result, FrForm* pForm) {
	if (result == FrOK) {



		CWorldTourEvent* pEvent = (CWorldTourEvent*)WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x1e);
		if (pEvent) {


			pEvent->SetCanGift(false);
			pEvent->SetGotGift(true);
		}


		m_pGiftButton->Enable(false);
		m_pGiftButton->SetVisible(false);
		m_pGiftButtonCover->Enable(false);
		m_pGiftButtonCover->SetVisible(false);

		FrForm* pNotify = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		WRect rect = pNotify->GetRect();
		pNotify->SetRect(WRect(rect.x, rect.y, rect.w, rect.h));
		pNotify->SetTimeLimit(12.0f);



		// "- Finished the Pangya island tour and received a swimsuit.\n\n - Automatically entered into the prize event"
		pNotify->SetMessage("-\xc6\xce\xbe\xdf\xbc\xb6 \xc5\xf5\xbe\xee\xb8\xa6 \xb8\xb6\xc4\xa1\xb0\xed \xbc\xf6\xbf\xb5\xba\xb9\xc0\xbb \xc1\xf6\xb1\xde\xb9\xde\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.\n\n -\\c0xffff0000\\c\xb0\xe6\xc7\xb0 \xc0\xcc\xba\xa5\xc6\xae\\c0xff000000\\c\xbf\xa1 \xc0\xda\xb5\xbf\xc0\xb8\xb7\xce \xc0\xc0\xb8\xf0\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9", false);

		pNotify->Open((FRESH_RESULT_CALLBACK)0, 0);
	}

	m_pGiftDlg = 0;

	return true;
}

// worldtoureventdlg.cpp:475 (run-time type and factory registration of FrWorldTourEventDescDlg)
const WRTTI FrWorldTourEventDescDlg::m_RTTI("FrWorldTourEventDescDlg", &FrForm::m_RTTI);

IObject* FrWorldTourEventDescDlgMakeInstance(void) {
	return new FrWorldTourEventDescDlg;
}

struct __sFrWorldTourEventDescDlg {
	__sFrWorldTourEventDescDlg(void) {
		ObjectFactory().AddObjectFunctor(FrWorldTourEventDescDlgMakeInstance, "FrWorldTourEventDescDlg");
	}
};
static __sFrWorldTourEventDescDlg s_registerFrWorldTourEventDescDlg;

// worldtoureventdlg.cpp:477
const sFRESH_MSGMAP* FrWorldTourEventDescDlg::GetMessageMap(void) const {
	return &FrWorldTourEventDescDlg::_MsgMap;
}

sFRESH_MSGMAP FrWorldTourEventDescDlg::_MsgMap = { &FrForm::_MsgMap, &FrWorldTourEventDescDlg::_MsgEntries[0] };

sFRESH_ENTRY FrWorldTourEventDescDlg::_MsgEntries[] = {
	// worldtoureventdlg.cpp:479
	{ "desc_view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrWorldTourEventDescDlg::OnInitDescView },

	// worldtoureventdlg.cpp:481
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// worldtoureventdlg.cpp:484
FrWorldTourEventDescDlg::FrWorldTourEventDescDlg(void) {
	m_pDescView = 0;
}

// worldtoureventdlg.cpp:489
FrWorldTourEventDescDlg::~FrWorldTourEventDescDlg(void) {
}

// worldtoureventdlg.cpp:493
void FrWorldTourEventDescDlg::OnInitDescView(intptr_t param) {
	m_pDescView = DYNAMIC_CAST(FrViewer, param);

	if (m_pDescView) {

		m_pDescView->ShowScrollBar(false);
		m_pDescView->Open("wte_explanation.jpg");
	}
}

// worldtoureventdlg.cpp:505 (run-time type and factory registration of FrWorldTourEventGiftDlg)
const WRTTI FrWorldTourEventGiftDlg::m_RTTI("FrWorldTourEventGiftDlg", &FrForm::m_RTTI);

IObject* FrWorldTourEventGiftDlgMakeInstance(void) {
	return new FrWorldTourEventGiftDlg;
}

struct __sFrWorldTourEventGiftDlg {
	__sFrWorldTourEventGiftDlg(void) {
		ObjectFactory().AddObjectFunctor(FrWorldTourEventGiftDlgMakeInstance, "FrWorldTourEventGiftDlg");
	}
};
static __sFrWorldTourEventGiftDlg s_registerFrWorldTourEventGiftDlg;

// worldtoureventdlg.cpp:507
const sFRESH_MSGMAP* FrWorldTourEventGiftDlg::GetMessageMap(void) const {
	return &FrWorldTourEventGiftDlg::_MsgMap;
}

sFRESH_MSGMAP FrWorldTourEventGiftDlg::_MsgMap = { &FrForm::_MsgMap, &FrWorldTourEventGiftDlg::_MsgEntries[0] };

sFRESH_ENTRY FrWorldTourEventGiftDlg::_MsgEntries[] = {
	// worldtoureventdlg.cpp:509
	{ "yes_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrWorldTourEventGiftDlg::OnInitYesButton },
	// worldtoureventdlg.cpp:510
	{ "yes_button", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrWorldTourEventGiftDlg::OnLBDownYesButton },

	// worldtoureventdlg.cpp:512
	{ "no_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrWorldTourEventGiftDlg::OnInitNoButton },
	// worldtoureventdlg.cpp:513
	{ "no_button", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrWorldTourEventGiftDlg::OnLBDownNoButton },

	// worldtoureventdlg.cpp:515
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// worldtoureventdlg.cpp:518
FrWorldTourEventGiftDlg::FrWorldTourEventGiftDlg(void) {
	m_pYesButton = 0;
	m_pNoButton = 0;
}

// worldtoureventdlg.cpp:524
FrWorldTourEventGiftDlg::~FrWorldTourEventGiftDlg(void) {
}

// worldtoureventdlg.cpp:529
void FrWorldTourEventGiftDlg::OnInitYesButton(intptr_t param) {
	m_pYesButton = DYNAMIC_CAST(FrButton, param);
}

// worldtoureventdlg.cpp:534
void FrWorldTourEventGiftDlg::OnLBDownYesButton(void) {
	WSendPacket packet((enumClientPacket)0xa5);
	packet.Encode4(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid);
	packet.Send(SEND_TO_DEFAULT);
	Close(FrOK, true);
}

// worldtoureventdlg.cpp:542
void FrWorldTourEventGiftDlg::OnInitNoButton(intptr_t param) {
	m_pNoButton = DYNAMIC_CAST(FrButton, param);
}

// worldtoureventdlg.cpp:547
void FrWorldTourEventGiftDlg::OnLBDownNoButton(void) {
	Close(FrCANCEL, true);
}
