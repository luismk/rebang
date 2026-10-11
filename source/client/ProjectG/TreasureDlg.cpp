// Original: d:\build\custom\temp\code\client\projectg\treasuredlg.cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TreasureDlg.h"
#include "ObjectFactory.h"
#include "Treasure.h"
#include "frarea.h"
#include "frbutton.h"
#include "frcombobox.h"
#include "frdesktop.h"
#include "frlistbox.h"
#include "frgaugebar.h"
#include "frviewer.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "fresh.h"
#include "wview.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wfont.h"
#include "SimpleOverlay.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "commonutil.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "../shared/ItemManager.h"
#include "../shared/classdefine.h"
#include "../shared/globalgamedefine.h"
#include "Packet.h"
#include "TaskManager.h"
#include "frwndmanager.h"

unsigned char GetNextHole(void);   // s5/demonlanddlg.cpp

extern "C" unsigned long data_a94af8;   // grid line color of FrTreasureGift (0xffa2d7fd)

// treasuredlg.cpp:39
bool nsTHunter::IsWndSizeMinimum(void) {
	WRect rect = g_pFresh->GetDesktop()->GetRect();
	if (rect.w == 640.0f && rect.h == 480.0f) {
		return true;
	}
	return false;
}

// treasuredlg.cpp:49
unsigned long nsTHunter::CalcGauge(unsigned long value) {
	return value < 701 ? 0 : value - 700;
}

// treasuredlg.cpp:56
bool CalculateUVPos(WRect& rc, int width, int height, float cellW, float cellH, int index) {
	if ((float)width < cellW || (float)height < cellH)
		return false;

	int rows = (int)((float)height / cellH);
	int cols = (int)((float)width / cellW);

	if (cols * rows < index)
		return false;

	float invW = 1.0f / (float)width;
	int col = index / rows;
	int row = index % rows;

	rc.x = cellW * col * invW;
	float invH = 1.0f / (float)height;
	rc.y = cellH * row * invH;
	rc.w = invW * cellW;
	rc.h = invH * cellH;
	return true;
}

// treasuredlg.cpp:89 (run-time type and factory registration of FrNextHole)
const WRTTI FrNextHole::m_RTTI("FrNextHole", &FrForm::m_RTTI);

IObject* FrNextHoleMakeInstance(void) {
	return new FrNextHole;
}

struct __sFrNextHole {
	__sFrNextHole(void) {
		ObjectFactory().AddObjectFunctor(FrNextHoleMakeInstance, "FrNextHole");
	}
};
static __sFrNextHole s_registerFrNextHole;

// treasuredlg.cpp:91
const sFRESH_MSGMAP* FrNextHole::GetMessageMap(void) const {
	return &FrNextHole::_MsgMap;
}

sFRESH_MSGMAP FrNextHole::_MsgMap = { &FrForm::_MsgMap, &FrNextHole::_MsgEntries[0] };

// treasuredlg.cpp:92
sFRESH_ENTRY FrNextHole::_MsgEntries[] = {
	{ "nexthole", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNextHole::OnAreaNextHole_Init },
	{ "nexthole", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNextHole::OnAreaNextHole_OwnerDraw },

	{ "outframe1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNextHole::OnArea_OutFrame_Init },
	{ "outframe2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNextHole::OnArea_OutFrame_Init },
	{ "outframe3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNextHole::OnArea_OutFrame_Init },

	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// treasuredlg.cpp:103
FrNextHole::FrNextHole(void) {
	m_pOverlay[0] = 0;
	m_pOverlay[1] = 0;
	m_pNextHoleArea = 0;
	m_overlayIdx = 0;
	m_state = STATE_NONE;
	m_pOverlay[2] = 0;
}

// treasuredlg.cpp:114
FrNextHole::~FrNextHole(void) {
	if (g_resrcmng && m_pOverlay[1]) { g_resrcmng->Release(m_pOverlay[1]); m_pOverlay[1] = 0; }
	if (g_resrcmng && m_pOverlay[0]) { g_resrcmng->Release(m_pOverlay[0]); m_pOverlay[0] = 0; }

	if (g_resrcmng && m_pOverlay[2]) { g_resrcmng->Release(m_pOverlay[2]); m_pOverlay[2] = 0; }
}

// treasuredlg.cpp:122
void FrNextHole::OnAreaNextHole_Init(intptr_t param) {
	m_pNextHoleArea = DYNAMIC_CAST(FrArea, param);
}

// treasuredlg.cpp:127
bool FrNextHole::OnInit(void) {
	FrForm::OnInit();

	IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(((GetCurMap() > 0x7F && GetCurMap() != 0xFD) ? GetCurMap() - 0x80 : GetCurMap()) | 0x28000000);
	if (pCourse == 0)
		return false;

	m_pOverlay[0] = g_resrcmng->GetOverlay(MakeStr("%s_1.png", pCourse->m_name), 0);
	m_pOverlay[1] = g_resrcmng->GetOverlay(MakeStr("%s_2.png", pCourse->m_name), 0);

	m_pOverlay[2] = g_resrcmng->GetOverlay("next_chaos.tga", 0);

	if (m_pOverlay[0] != 0 || m_pOverlay[1] != 0) {

		m_overlayWidth = m_pOverlay[0]->GetWidth();
		m_overlayHeight = m_pOverlay[0]->GetHeight();
	}






	m_outFrames.reserve(3);


	MoveWindow(GetCalcPoint());

	return true;
}

// treasuredlg.cpp:157
void FrNextHole::SetNextHole(int hole) {
	if (m_pOverlay[0] == 0 || m_pOverlay[1] == 0)
		return;

	if (hole > 10 || hole == 19 || CSharedDoc::Instance()->m_holeOrder[CSharedDoc::Instance()->m_holes - 1] == GOLFDOC()->m_currentHole) {
		m_overlayIdx = 1;
		hole -= 10;
	} else {
		m_overlayIdx = 0;
	}

	if (hole == 19 || CSharedDoc::Instance()->m_holeOrder[CSharedDoc::Instance()->m_holes - 1] == GOLFDOC()->m_currentHole) {
		CalculateUVPos(m_drawRect, m_overlayWidth, m_overlayHeight, 140.0f, 93.0f, 8);
	} else {
		CalculateUVPos(m_drawRect, m_overlayWidth, m_overlayHeight, 140.0f, 93.0f, hole - 1);
	}
}

// treasuredlg.cpp:191
void FrNextHole::OnAreaNextHole_OwnerDraw(int param) {
	if (m_state == STATE_HIDE)
		return;

	WRect rect = m_pNextHoleArea->GetRect();

	if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomMode == 14) {
		m_overlayIdx = 2;
		m_drawRect.x = 0.0f;
		m_drawRect.y = 0.0f;
		m_drawRect.w = 1.0f;
		m_drawRect.h = 1.0f;
	}

	if (m_pOverlay[m_overlayIdx]) {
		m_pOverlay[m_overlayIdx]->Render(g_view, m_drawRect, WRect(rect.x, rect.y, 140.0f, 93.0f), 0, 0xffffffff, 0, 0);
	}
}

// treasuredlg.cpp:210
void FrNextHole::OnArea_OutFrame_Init(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea) {
		m_outFrames.push_back(pArea);
	}
}

// treasuredlg.cpp:223
void FrNextHole::SetState(eState state) {
	if (m_state == state) {
		if (IsEnabled()) {
			if (m_state == STATE_HIDE) {
				Enable(false);
				SetVisible(false);
			}
		}
		return;
	}

	m_state = state;
	if (state == STATE_HIDE) {
		Enable(false);
		SetVisible(false);
		return;
	}

	Enable(true);
	MoveWindow(GetCalcPoint());
	SetVisible(true);

	if (nsTHunter::IsWndSizeMinimum() == true)
		SetOutFrameVisible(false);
	else
		SetOutFrameVisible(true);

	SetNextHole(GetNextHole());
}

// treasuredlg.cpp:266
WPoint FrNextHole::GetCalcPoint(void) {
	WRect rect = m_rect;

	float offset = nsTHunter::IsWndSizeMinimum() == true ? 6.0f : -38.0f;

	WPoint pt;
	pt.x = (float)(int)((g_view->GetWidth() - rect.w) * 0.5f) + 210.0f;
	pt.y = (float)(int)(g_view->GetHeight() - rect.h + offset);

	return pt;
}

// treasuredlg.cpp:282
void FrNextHole::SetOutFrameVisible(bool bVisible) {
	int count = m_outFrames.size();
	for (int i = 0; i < count; ++i) {
		if (m_outFrames[i]) {
			m_outFrames[i]->SetVisible(bVisible);
		}
	}
}

// treasuredlg.cpp:296 (run-time type and factory registration of FrTreasureGauge)
const WRTTI FrTreasureGauge::m_RTTI("FrTreasureGauge", &FrForm::m_RTTI);

IObject* FrTreasureGaugeMakeInstance(void) {
	return new FrTreasureGauge;
}

struct __sFrTreasureGauge {
	__sFrTreasureGauge(void) {
		ObjectFactory().AddObjectFunctor(FrTreasureGaugeMakeInstance, "FrTreasureGauge");
	}
};
static __sFrTreasureGauge s_registerFrTreasureGauge;

// treasuredlg.cpp:298
const sFRESH_MSGMAP* FrTreasureGauge::GetMessageMap(void) const {
	return &FrTreasureGauge::_MsgMap;
}

// treasuredlg.cpp:311
FrTreasureGauge::FrTreasureGauge(void) {
	m_pGauge = 0;
	m_pArrowArea = 0;
	m_state = STATE_NONE;
}

// treasuredlg.cpp:318
FrTreasureGauge::~FrTreasureGauge(void) {
}

// treasuredlg.cpp:323
void FrTreasureGauge::OnImageBarInit(int param) {
	m_pGauge = DYNAMIC_CAST(FrGaugeBarImage, param);
	if (m_pGauge) {
		m_pGauge->SetRange(0, 1000, 0);
		m_pGauge->SetBarSpeed(250.0f);
	}
}

// treasuredlg.cpp:333
bool FrTreasureGauge::OnInit(void) {
	m_pArrowBitmap = g_pFresh->RegisterBitmap("th_gage_arrow");

	m_outFrames.reserve(3);

	MoveWindow(GetCalcPoint());

	return true;
}

// treasuredlg.cpp:349
void FrTreasureGauge::SetTPoint(void) {
	unsigned long point = CTHunter::Instance()->GetTreasurePoint();
	if (m_pGauge) {
		m_pGauge->SetDestPos(point, false);
	}
}

// treasuredlg.cpp:362
void FrTreasureGauge::SetState(eState state) {
	if (m_state != state) {
		m_state = state;

		if (state == (eState)1) {
			Enable(false);
			SetVisible(false);
		} else {
			Enable(true);
			MoveWindow(GetCalcPoint());
			SetVisible(true);

			if (nsTHunter::IsWndSizeMinimum() == true)
				SetOutFrameVisible(false);
			else
				SetOutFrameVisible(true);

			if (state == (eState)3) {
				unsigned long point = CTHunter::Instance()->GetTreasurePoint();
	if (m_pGauge) {
		m_pGauge->SetDestPos(point, false);
	}
			}
		}
	}
}

// treasuredlg.cpp:393
WPoint FrTreasureGauge::GetCalcPoint(void) {
	WRect rect = m_rect;

	float offset = nsTHunter::IsWndSizeMinimum() == true ? 14.0f : -30.0f;

	WPoint pt;
	pt.x = (float)(int)((g_view->GetWidth() - rect.w) * 0.5f) - 90.0f;
	pt.y = (float)(int)(g_view->GetHeight() - rect.h + offset);

	return pt;
}

// treasuredlg.cpp:408
void FrTreasureGauge::SetOutFrameVisible(bool bVisible) {
	int count = m_outFrames.size();
	for (int i = 0; i < count; ++i) {
		if (m_outFrames[i]) {
			m_outFrames[i]->SetVisible(bVisible);
		}
	}
}

// treasuredlg.cpp:418
void FrTreasureGauge::OnArea_Arrow_Init(int param) {
	m_pArrowArea = DYNAMIC_CAST(FrArea, param);
}

// treasuredlg.cpp:423
void FrTreasureGauge::OnArea_Arrow_OwnerDraw(int) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {



		static float pixelPerHeight = (float)m_pGauge->GetPixelPerHeight();
		if (m_pArrowBitmap) {

			WRect rc = m_pGauge->GetRect();
			pixelPerHeight = (float)m_pGauge->GetPixelPerHeight();

			pGDI->DrawTexture(m_pArrowBitmap, WRect(pixelPerHeight + rc.x - 7.0f, rc.y - 15.0f, m_pArrowBitmap->Width(), m_pArrowBitmap->Height()), 0xFFFFFFFF, 0);
		}
	}
}

// treasuredlg.cpp:440
void FrTreasureGauge::OnArea_OutFrame_Init(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea) {
		m_outFrames.push_back(pArea);
	}
}

// treasuredlg.cpp:453 (run-time type and factory registration of FrTreasureCourse)
const WRTTI FrTreasureCourse::m_RTTI("FrTreasureCourse", &FrForm::m_RTTI);

IObject* FrTreasureCourseMakeInstance(void) {
	return new FrTreasureCourse;
}

struct __sFrTreasureCourse {
	__sFrTreasureCourse(void) {
		ObjectFactory().AddObjectFunctor(FrTreasureCourseMakeInstance, "FrTreasureCourse");
	}
};
static __sFrTreasureCourse s_registerFrTreasureCourse;

// treasuredlg.cpp:455
const sFRESH_MSGMAP* FrTreasureCourse::GetMessageMap(void) const {
	return &FrTreasureCourse::_MsgMap;
}

// treasuredlg.cpp:561
FrTreasureCourse::FrTreasureCourse(void) {
	m_mapBtns.reserve(20);

	m_mapBtns.insert(m_mapBtns.begin(), 20, (FrButton*)0);

	for (int i = 0; i < 20; i++) {
		m_mapBitmaps[i] = 0;
		m_mapGauges[i] = 0;
	}

	m_randomMapBitmap = 0;
	m_unknown254 = 0;
	m_randomBtn = 0;
	m_pCloudOverlay = 0;
	m_pSelectMapArea = 0;
	m_pViewerF1 = 0;
	m_pViewerF2 = 0;
	m_curMapIdx = 0;
	m_unknown1ec = 0;
	m_unknown26c = 0;
	m_unknown270 = 0;
	m_mapBtnInitIdx = 0;
	m_unknown1e9 = 0;
	m_pImageBar = 0;
	m_pSmallBar = 0;
	m_openedMapIdx = -1;
	m_mapIndexChanged = true;
}

// treasuredlg.cpp:607
FrTreasureCourse::~FrTreasureCourse(void) {
	if (g_resrcmng && m_pCloudOverlay) { g_resrcmng->Release(m_pCloudOverlay); m_pCloudOverlay = 0; }
}

// treasuredlg.cpp:799
void FrTreasureCourse::OnProc(const float dt) {
	ProcCloud(dt);
}

// treasuredlg.cpp:804
void FrTreasureCourse::InitCloud(void) {
	int cloudCounts[2] = { 3, 4 };
	srand((unsigned int)time(NULL));

	int count = cloudCounts[rand() % 2];

	sCloud cloud;
	m_clouds.reserve(count);

	for (int i = 0; i < count; ++i) {
		m_clouds.push_back(cloud);
	}

	m_cloudCount = m_clouds.size();
	for (int j = 0; j < m_cloudCount; ++j) {
		ReInitCloud(j);
	}
}

// treasuredlg.cpp:888
void FrTreasureCourse::ProcCloud(float dt) {
	for (unsigned char i = 0; i < m_cloudCount; ++i) {
		CheckArea(i, dt);
		m_clouds[i].srcRect.x += dt * m_clouds[i].m_unknown10;
	}
}

// treasuredlg.cpp:902
void FrTreasureCourse::CheckArea(int cloudIdx, float dt) {
	WRect areaRect = m_pCloudArea->GetRect();
	WRect cloudRect = m_clouds[cloudIdx].srcRect;
	float areaRight = areaRect.x + areaRect.w;
	float cloudRight = cloudRect.x + cloudRect.w;

	if (areaRect.x <= cloudRect.x && areaRect.x + cloudRect.w * 2.0f > cloudRight) {
		if (!m_clouds[cloudIdx].m_unknown1d) { if (m_clouds[cloudIdx].alpha < 255) m_clouds[cloudIdx].alpha += 1; else m_clouds[cloudIdx].alpha = 255; }
	} else if (cloudRight > areaRight && cloudRight + areaRight > cloudRect.x) {
		if (m_clouds[cloudIdx].alpha > 0) m_clouds[cloudIdx].alpha -= 5; else m_clouds[cloudIdx].alpha = 0;
	}

	if (cloudRect.x > areaRight) {
		ReInitCloud(cloudIdx);
	}
}

// treasuredlg.cpp:928
void FrTreasureCourse::ShiftAlpha(int cloudIdx, float dt, eALPHA type) {
	if (type == ALPHA_IN && !m_clouds[cloudIdx].m_unknown1d) {
		if (m_clouds[cloudIdx].alpha < 255) {
			m_clouds[cloudIdx].alpha += 1;
		} else {
			m_clouds[cloudIdx].alpha = 255;
		}
	} else if (type == ALPHA_OUT) {
		if (m_clouds[cloudIdx].alpha > 0) {
			m_clouds[cloudIdx].alpha -= 5;
		} else {
			m_clouds[cloudIdx].alpha = 0;
		}
	}
}

// treasuredlg.cpp:957
void FrTreasureCourse::OnAreaSelectMapInit(int param) {
	m_pSelectMapArea = DYNAMIC_CAST(FrArea, param);
}

// treasuredlg.cpp:975
void FrTreasureCourse::OnBtnMap_ALL_Init(int param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	if (pBtn) {
		if (m_mapBtnInitIdx == 12 || m_mapBtnInitIdx == 17) {
			m_mapBtnInitIdx++;
		}

		if (m_mapBtnInitIdx == 18)
			m_mapBtnInitIdx = 19;

		m_mapBtns[m_mapBtnInitIdx++] = pBtn;

		int count = m_mapBtns.size();
		if (m_mapBtnInitIdx == count) {
			m_mapBtnInitIdx = 0;
		}
	}
}

// treasuredlg.cpp:1000
void FrTreasureCourse::OnBtnMap_Lagoon_Down(void) {
	SelectMapBtn(MAP_BLUE_LAGOON);
	Close(true);
}

// treasuredlg.cpp:1005
void FrTreasureCourse::OnBtnMap_Lagoon_OwnerDraw(int) {
	DrawMapBtn(MAP_BLUE_LAGOON);
}

// treasuredlg.cpp:1010
void FrTreasureCourse::OnBtnMap_Water_Down(void) {
	SelectMapBtn(MAP_BLUE_WATER);
	Close(true);
}

// treasuredlg.cpp:1015
void FrTreasureCourse::OnBtnMap_Water_OwnerDraw(int) {
	DrawMapBtn(MAP_BLUE_WATER);
}

// treasuredlg.cpp:1020
void FrTreasureCourse::OnBtnMap_Sepia_Down(void) {
	SelectMapBtn(MAP_SEPIA_WIND);
	Close(true);
}

// treasuredlg.cpp:1025
void FrTreasureCourse::OnBtnMap_Sepia_OwnerDraw(int) {
	DrawMapBtn(MAP_SEPIA_WIND);
}

// treasuredlg.cpp:1030
void FrTreasureCourse::OnBtnMap_Hill_Down(void) {
	SelectMapBtn(MAP_WIND_HILL);
	Close(true);
}

// treasuredlg.cpp:1035
void FrTreasureCourse::OnBtnMap_Hill_OwnerDraw(int) {
	DrawMapBtn(MAP_WIND_HILL);
}

// treasuredlg.cpp:1040
void FrTreasureCourse::OnBtnMap_WizWiz_Down(void) {
	SelectMapBtn(MAP_WIZ_WIZ);
	Close(true);
}

// treasuredlg.cpp:1045
void FrTreasureCourse::OnBtnMap_WizWiz_OwnerDraw(int) {
	DrawMapBtn(MAP_WIZ_WIZ);
}

// treasuredlg.cpp:1050
void FrTreasureCourse::OnBtnMap_West_Down(void) {
	SelectMapBtn(MAP_WEST_WIZ);
	Close(true);
}

// treasuredlg.cpp:1055
void FrTreasureCourse::OnBtnMap_West_OwnerDraw(int) {
	DrawMapBtn(MAP_WEST_WIZ);
}

// treasuredlg.cpp:1060
void FrTreasureCourse::OnBtnMap_Moon_Down(void) {
	SelectMapBtn(MAP_BLUE_MOON);
	Close(true);
}

// treasuredlg.cpp:1065
void FrTreasureCourse::OnBtnMap_Moon_OwnerDraw(int) {
	DrawMapBtn(MAP_BLUE_MOON);
}

// treasuredlg.cpp:1070
void FrTreasureCourse::OnBtnMap_Silvia_Down(void) {
	SelectMapBtn(MAP_SILVIA_CANNON);
	Close(true);
}

// treasuredlg.cpp:1075
void FrTreasureCourse::OnBtnMap_Silvia_OwnerDraw(int) {
	DrawMapBtn(MAP_SILVIA_CANNON);
}

// treasuredlg.cpp:1080
void FrTreasureCourse::OnBtnMap_Cannon_Down(void) {
	SelectMapBtn(MAP_ICE_CANNON);
	Close(true);
}

// treasuredlg.cpp:1085
void FrTreasureCourse::OnBtnMap_Cannon_OwnerDraw(int) {
	DrawMapBtn(MAP_ICE_CANNON);
}

// treasuredlg.cpp:1090
void FrTreasureCourse::OnBtnMap_White_Down(void) {
	SelectMapBtn(MAP_WHITE_WIZ);
	Close(true);
}

// treasuredlg.cpp:1095
void FrTreasureCourse::OnBtnMap_White_OwnerDraw(int) {
	DrawMapBtn(MAP_WHITE_WIZ);
}

// treasuredlg.cpp:1100
void FrTreasureCourse::OnBtnMap_Shining_Down(void) {
	SelectMapBtn(MAP_SHINING_SAND);
	Close(true);
}

// treasuredlg.cpp:1105
void FrTreasureCourse::OnBtnMap_Shining_OwnerDraw(int) {
	DrawMapBtn(MAP_SHINING_SAND);
}

// treasuredlg.cpp:1110
void FrTreasureCourse::OnBtnMap_Pink_Down(void) {
	SelectMapBtn(MAP_PINK_WIND);
	Close(true);
}

// treasuredlg.cpp:1115
void FrTreasureCourse::OnBtnMap_Pink_OwnerDraw(int) {
	DrawMapBtn(MAP_PINK_WIND);
}

// treasuredlg.cpp:1120
void FrTreasureCourse::OnBtnMap_Inferno_Down(void) {
	SelectMapBtn(MAP_DEEP_INFERNO);
	Close(true);
}

// treasuredlg.cpp:1125
void FrTreasureCourse::OnBtnMap_Inferno_OwnerDraw(int) {
	DrawMapBtn(MAP_DEEP_INFERNO);
}

// treasuredlg.cpp:1130
void FrTreasureCourse::OnBtnMap_IceSpa_Down(void) {
	SelectMapBtn(MAP_ICE_SPA);
	Close(true);
}

// treasuredlg.cpp:1135
void FrTreasureCourse::OnBtnMap_IceSpa_OwnerDraw(int) {
	DrawMapBtn(MAP_ICE_SPA);
}

// treasuredlg.cpp:1140
void FrTreasureCourse::OnBtnMap_SeaWay_Down(void) {
	SelectMapBtn(MAP_LOST_SEAWAY);
	Close(true);
}

// treasuredlg.cpp:1146
void FrTreasureCourse::OnBtnMap_SeaWay_OwnerDraw(int) {
	DrawMapBtn(MAP_LOST_SEAWAY);
}

// treasuredlg.cpp:1151
void FrTreasureCourse::OnBtnMap_Panda_Down(void) {
	SelectMapBtn(MAP_EASTERN_VALLEY);
	Close(true);
}

// treasuredlg.cpp:1157
void FrTreasureCourse::OnBtnMap_Panda_OwnerDraw(int) {
	DrawMapBtn(MAP_EASTERN_VALLEY);
}

// treasuredlg.cpp:1163
void FrTreasureCourse::OnBtnMap_Wizcity_Down(void) {
	SelectMapBtn(MAP_WIZ_CITY);
	Close(true);
}

// treasuredlg.cpp:1169
void FrTreasureCourse::OnBtnMap_Wizcity_OwnerDraw(int) {
	DrawMapBtn(MAP_WIZ_CITY);
}

// treasuredlg.cpp:1175
void FrTreasureCourse::OnBtnMap_Random_Init(int param) {
	m_randomBtn = DYNAMIC_CAST(FrButton, param);
}

// treasuredlg.cpp:1179
void FrTreasureCourse::OnBtnMap_Random_Down(void) {
	SelectMapBtn(0x7f);
	Close(true);
}

// treasuredlg.cpp:1185
void FrTreasureCourse::OnViewerF1(int param) {
	m_pViewerF1 = DYNAMIC_CAST(FrViewer, param);
	m_pViewerF1->SetMouseEvent(false);
	m_pViewerF1->Open("th_quick_frm1.tga");
}

// treasuredlg.cpp:1192
void FrTreasureCourse::OnViewerF2(int param) {
	m_pViewerF2 = DYNAMIC_CAST(FrViewer, param);
	m_pViewerF2->SetMouseEvent(false);
	m_pViewerF2->Open("th_quick_frm2.tga");
}

// treasuredlg.cpp:1199
void FrTreasureCourse::OnImageBar(int param) {
	m_pImageBar = DYNAMIC_CAST(FrGaugeBarImage, param);
	if (m_pImageBar) {
		m_pImageBar->SetRange(0, 300, 0);
		m_pImageBar->SetBarSpeed(200.0f);
		m_pImageBar->SetVisible(false);
	}
}

// treasuredlg.cpp:1210
void FrTreasureCourse::OnGauge_SmallBar(int param) {
	m_pSmallBar = DYNAMIC_CAST(FrGaugeBarImage, param);
	if (m_pSmallBar) {
		m_pSmallBar->SetRange(0, 300, 0);
		m_pSmallBar->SetBarSpeed(200.0f);
	}
}

// treasuredlg.cpp:1220
void FrTreasureCourse::OnGauge_SmallBar_OwnerDraw(int param) {
	FrGraphicInterface* pGI = g_pFresh->GetManager()->GetGDI();
	if (pGI == 0)
		return;

	const Bitmap* pBackSkin = m_pSmallBar->GetGaugeSkin(0);
	const Bitmap* pBarSkin = m_pSmallBar->GetGaugeSkin(1);
	if (pBackSkin == 0 || pBarSkin == 0)
		return;

	for (int i = 0; i < 20; ++i) {
		if (i == 12)
			continue;
		if (i == 17)
			continue;
		if (m_openedMapIdx == i)
			continue;
		m_pSmallBar->SetPos(nsTHunter::CalcGauge(m_mapGauges[i]));

		pGI->DrawTexture(pBackSkin, WRect(m_gaugePos[i].x, m_gaugePos[i].y, (float)pBackSkin->Width(), (float)pBackSkin->Height()), 0xffffffff, 0);
		pGI->DrawTexture(pBarSkin, WRect(m_gaugePos[i].x, m_gaugePos[i].y, (float)pBarSkin->Width(), (float)pBarSkin->Height()), 0xffffffff, 0);

		m_pSmallBar->ResetGauge();
	}
}

// treasuredlg.cpp:1243
void FrTreasureCourse::OnViewer_CurrMap_Init(int param) {
	m_pCurMapViewer = DYNAMIC_CAST(FrViewer, param);
}

// treasuredlg.cpp:1248
void FrTreasureCourse::OnArea_Explane_Init(int param) {
	m_pExplaneArea = DYNAMIC_CAST(FrArea, param);
}

// treasuredlg.cpp:1253
void FrTreasureCourse::OnArea_Explane_OwnerDraw(int param) {
	FrGraphicInterface* pGI = g_pFresh->GetManager()->GetGDI();
	if (pGI) {
		if (m_pExplaneArea && m_pExplaneBitmap) {
			WRect rect = m_pExplaneArea->GetRect();
			if (rect.IsInRect(g_input->GetMousePoint().x, g_input->GetMousePoint().y)) {
				pGI->DrawTexture(m_pExplaneBitmap, WRect(610.0f, 100.0f, m_pExplaneBitmap->Width(), m_pExplaneBitmap->Height()), 0xffffffff, 0);
			}
		}
	}
}

// treasuredlg.cpp:1266
void FrTreasureCourse::OnInit_Cloud_Area(int param) {
	m_pCloudArea = DYNAMIC_CAST(FrArea, param);
}

// treasuredlg.cpp:1271
void FrTreasureCourse::OnOwnerDraw_Cloud_Area(int param) {
	if (m_pCloudOverlay) {
		for (int i = 0; i < m_cloudCount; ++i) {
			m_pCloudOverlay->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), m_clouds[i].srcRect, 0, (m_clouds[i].alpha << 24) | 0xffffff, 0, 0);
		}
	}
}

// treasuredlg.cpp:1283
void FrTreasureCourse::OpenSelectedMap(unsigned char mapIdx) {
}

// treasuredlg.cpp:1359
void FrTreasureCourse::DrawCurrentMap(FrGraphicInterface* pGI, int mapIdx) {
	float y = m_rect.y;
	float x = m_rect.w - 5.0f;

	if (mapIdx != 0x7f) {
		pGI->DrawTexture(m_mapBitmaps[mapIdx], WRect(x, y + 47.0f, m_mapBitmaps[mapIdx]->Width(), m_mapBitmaps[mapIdx]->Height()), 0xffffffff, 0);
	} else {
		pGI->DrawTexture(m_randomMapBitmap, WRect(x, y + 47.0f, m_randomMapBitmap->Width(), m_randomMapBitmap->Height()), 0xffffffff, 0);
	}
}

// treasuredlg.cpp:1372
void FrTreasureCourse::SetMapGauge(void) {
	for (int i = 0; i < 20; ++i) {
		unsigned long gauge = WSingleton<CTHunter>::Instance()->GetTHunterGauge((eMapType)i);
		if (gauge != 9999) {
			m_mapGauges[i] = gauge;
		}
	}
}

// treasuredlg.cpp:1386
bool FrTreasureCourse::OpenGauge(WPoint& pt, int mapIdx) {
	if (mapIdx == -1) {
		return false;
	}
	if (m_openedMapIdx == mapIdx) {
		return false;
	}

	m_openedMapIdx = mapIdx;
	m_pImageBar->MoveWindow(pt);
	m_pImageBar->ResetGauge();
	m_pImageBar->SetRange(0, 300, 0);
	m_pImageBar->SetDestPos(m_mapGauges[mapIdx] < 701 ? 0 : m_mapGauges[mapIdx] - 700, false);
	m_pImageBar->SetVisible(true);

	return true;
}

// treasuredlg.cpp:1411
void FrTreasureCourse::OpenCurrentMap(int mapType) {
	if (m_openedMapIdx == mapType) {
		return;
	}

	IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(0x28000000 | (mapType > 127 && mapType != 253 ? mapType - 128 : mapType));
	if (pCourse == 0) {
		return;
	}

	if (mapType < 127) {
		m_pCurMapViewer->Open(MakeStr("mid_%s.tga", pCourse->m_icon));
	} else {
		m_pCurMapViewer->Open("map_random_small.tga");
	}
}

// treasuredlg.cpp:1434
void FrTreasureCourse::SelectMapBtn(unsigned char mapIdx) {
	IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(((mapIdx > 0x7F && mapIdx != 0xFD) ? mapIdx - 0x80 : mapIdx) | 0x28000000);
	if (pCourse == 0)
		return;

	if (m_curMapIdx == mapIdx)
		m_mapIndexChanged = false;
	else
		m_mapIndexChanged = true;

	if (m_mapIndexChanged) {
		m_pCurMapViewer->Open(MakeStr("mid_%s", pCourse->m_name));
	}

	if (m_mapIndexChanged && (!stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM") || !stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM_EXT") || !stricmp(g_pFresh->GetManager()->GetLayoutID(), "AVATARCHAT_MAIN"))) {
		WSendPacket packet((enumClientPacket)0x0A);
		packet.Encode2(0xFFFF);
		packet.Encode1(1);
		packet.Encode1(3);
		packet.Encode1((unsigned char)pCourse->m_typeId);
		packet.Send(SEND_TO_DEFAULT);

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));

		m_curMapIdx = CMapTypeAdapter::GetMapIndex((eMapType)(pCourse->m_typeId & 0x3FFFFFF));
	} else {
		m_curMapIdx = CMapTypeAdapter::GetMapIndex((eMapType)(pCourse->m_typeId & 0x3FFFFFF));
	}
}

// treasuredlg.cpp:1503
void FrTreasureCourse::SetSelectMap(unsigned char mapType) {
	m_curMapIdx = CMapTypeAdapter::GetMapIndex((eMapType)mapType);
	OpenCurrentMap(mapType);
}

// treasuredlg.cpp:1509
void FrTreasureCourse::SetCurMapIdx(unsigned char mapIdx) {
	m_curMapIdx = mapIdx;
}

// treasuredlg.cpp:1514
unsigned char FrTreasureCourse::GetCurMapIdx(void) {
	return m_curMapIdx;
}

// treasuredlg.cpp:1519
bool FrTreasureCourse::GetMapIndexChanged(void) const {
	return m_mapIndexChanged;
}

// treasuredlg.cpp:1524
void FrTreasureCourse::SetMapIndexChanged(bool changed) {
	m_mapIndexChanged = changed;
}

// treasuredlg.cpp:1534 (run-time type and factory registration of FrTreasureScore)
const WRTTI FrTreasureScore::m_RTTI("FrTreasureScore", &FrForm::m_RTTI);

IObject* FrTreasureScoreMakeInstance(void) {
	return new FrTreasureScore;
}

struct __sFrTreasureScore {
	__sFrTreasureScore(void) {
		ObjectFactory().AddObjectFunctor(FrTreasureScoreMakeInstance, "FrTreasureScore");
	}
};
static __sFrTreasureScore s_registerFrTreasureScore;

// treasuredlg.cpp:1536
const sFRESH_MSGMAP* FrTreasureScore::GetMessageMap(void) const {
	return &FrTreasureScore::_MsgMap;
}

sFRESH_MSGMAP FrTreasureScore::_MsgMap = { &FrForm::_MsgMap, &FrTreasureScore::_MsgEntries[0] };

// treasuredlg.cpp:1538
sFRESH_ENTRY FrTreasureScore::_MsgEntries[] = {
	{ "imagebar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTreasureScore::OnGBarImage_ImageBar_Init },
	{ "back", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTreasureScore::OnView_Backgound_Init },
	{ "team", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTreasureScore::OnArea_Team_Init },

	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// treasuredlg.cpp:1545
FrTreasureScore::FrTreasureScore(void) {
	m_pGauge = 0;
	m_pBackground = 0;
	m_pTeamArea = 0;
	m_pTeam0Bitmap = 0;
	m_pTeam1Bitmap = 0;
}

// treasuredlg.cpp:1554
FrTreasureScore::~FrTreasureScore(void) {
}

// treasuredlg.cpp:1559
bool FrTreasureScore::OnInit(void) {
	MoveWindow(GetCalcPoint());

	const char* mapName = WSingleton<CSharedDoc>::Instance()->m_pCurCourse ? WSingleton<CSharedDoc>::Instance()->m_pCurCourse->m_mapName : 0;

	char fileName[128];
	sprintf(fileName, "th_result_%s.tga", mapName);
	if (m_pBackground) {
		m_pBackground->Open(fileName);
	}

	m_pTeam0Bitmap = g_pFresh->RegisterBitmap("th_team_blue");
	m_pTeam1Bitmap = g_pFresh->RegisterBitmap("th_team_red");

	IsTeamMode();
	unsigned long point = CTHunter::Instance()->GetTreasurePoint(); if (m_pGauge) m_pGauge->SetDestPos(point, false);

	return true;
}

// treasuredlg.cpp:1581
void FrTreasureScore::SetTPoint(void) {
	unsigned long point = CTHunter::Instance()->GetTreasurePoint();

	if (m_pGauge) {
		m_pGauge->SetDestPos(point, false);
	}
}

// treasuredlg.cpp:1591
void FrTreasureScore::SetState(bool bState) {
	if (m_bState == bState) {
		return;
	}
	m_bState = bState;
	if (!bState) {
		SetVisible(false);
	} else {
		MoveWindow(GetCalcPoint());
		SetVisible(true);
	}
}

// treasuredlg.cpp:1608
WPoint FrTreasureScore::GetCalcPoint(void) {
	WRect rect = m_rect;

	WPoint pt;
	pt.x = (float)(int)((g_view->GetWidth() - rect.w) * 0.5f);
	pt.y = (float)(int)((g_view->GetHeight() - rect.h) * 0.5f);

	pt.y -= 20.0f;
	return pt;
}

// treasuredlg.cpp:1621
void FrTreasureScore::IsTeamMode(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_roomType != 1) {
		m_pTeamArea->SetVisible(false);
		return;
	}

	for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid == GOLFDOC()->GetPlayer(i)->m_uid) {
			const Bitmap* pBitmap = GOLFDOC()->GetPlayer(i)->m_attribute == 0 ? m_pTeam0Bitmap : m_pTeam1Bitmap;

			if (pBitmap) {
				m_pTeamArea->SetBgImg(pBitmap);
			}
			return;
		}
	}
}

// treasuredlg.cpp:1647
void FrTreasureScore::OnGBarImage_ImageBar_Init(intptr_t param) {
	m_pGauge = DYNAMIC_CAST(FrGaugeBarImage, param);
	if (m_pGauge) {
		m_pGauge->SetRange(0, 1000, 0);
		m_pGauge->SetBarSpeed(600.0f);
	}
}

// treasuredlg.cpp:1657
void FrTreasureScore::OnView_Backgound_Init(intptr_t param) {
	m_pBackground = DYNAMIC_CAST(FrViewer, param);

	if (m_pBackground) {
		m_pBackground->SetMouseEvent(false);
	}
}

// treasuredlg.cpp:1667
void FrTreasureScore::OnArea_Team_Init(intptr_t param) {
	m_pTeamArea = DYNAMIC_CAST(FrArea, param);
}

// treasuredlg.cpp:1678
CTFrame::CTFrame(void)
	: m_count(3) {
	m_frames.reserve(m_count);
}

// treasuredlg.cpp:1690
CTFrame::CTFrame(unsigned char count)
	: m_count(count) {
	m_frames.reserve(count);
}

// treasuredlg.cpp:1701
CTFrame::~CTFrame(void) {
	if (m_frames.size() > 0) m_frames.clear();
}

// treasuredlg.cpp:1706
void CTFrame::AddFrame(FrArea* pArea) {
	if (pArea) {
		m_frames.push_back(pArea);
	}
}

// treasuredlg.cpp:1712
bool CTFrame::SetFrameWidth(float width) {
	unsigned char start;
	unsigned char end;

	if (0.0f == width) {
		return false;
	}

	unsigned char size = (unsigned char)m_frames.size();

	if (size == 3) {
		start = 3; end = 6;
	} else if (size == 9) {
		start = 0; end = 3;
	}

	for (unsigned char i = start; i < end; ++i) {
		WRect rect = m_frames[i]->GetRect();
		rect.w += width;
		m_frames[i]->SetRect(rect);
	}

	return true;
}

// treasuredlg.cpp:1745
bool CTFrame::SetFrameHeight(float height) {
	unsigned char start;
	unsigned char mid;
	unsigned char end;

	if (0.0f == height) {
		return false;
	}

	unsigned char size = (unsigned char)m_frames.size();

	if (size == 6) {
		start = 0; mid = 3; end = 6;
	}

	for (unsigned char i = start; i < mid; ++i) {
		WRect rect = m_frames[i]->GetRect();
		rect.h += height;
		m_frames[i]->SetRect(rect);
	}

	for (unsigned char j = mid; j < end; ++j) {
		WRect rect = m_frames[j]->GetRect();
		rect.y += height;
		m_frames[j]->SetRect(rect);
	}

	return true;
}

// treasuredlg.cpp:1786 (run-time type and factory registration of FrTreasureGift)
const WRTTI FrTreasureGift::m_RTTI("FrTreasureGift", &FrForm::m_RTTI);

IObject* FrTreasureGiftMakeInstance(void) {
	return new FrTreasureGift;
}

struct __sFrTreasureGift {
	__sFrTreasureGift(void) {
		ObjectFactory().AddObjectFunctor(FrTreasureGiftMakeInstance, "FrTreasureGift");
	}
};
static __sFrTreasureGift s_registerFrTreasureGift;

// treasuredlg.cpp:1788
const sFRESH_MSGMAP* FrTreasureGift::GetMessageMap(void) const {
	return &FrTreasureGift::_MsgMap;
}

// treasuredlg.cpp:1818
FrTreasureGift::FrTreasureGift(void)
	: m_unknown150(8) {
	m_unknown151 = 1;
	m_bState = true;
}

// treasuredlg.cpp:1824
FrTreasureGift::~FrTreasureGift(void) {
}

// treasuredlg.cpp:1829
bool FrTreasureGift::OnInit(void) {
	m_boxOpener.OnInit();

	MoveWindow(GetCalcPoint());
	WSingleton<CTHunter>::Instance()->GetTreasurePoint();
	SetPointByFrameHeight();
	m_unknown151 = (unsigned char)(m_boxOpener.m_unknown64 / 8);

	return true;
}

// treasuredlg.cpp:1841
void FrTreasureGift::SetPointByFrameHeight(void) {
	float outHeight;
	float inHeight;

	int count = m_boxOpener.m_giftCount;

	if (count > 8 && count < 17) {
		outHeight = 65.0f;
		inHeight = 64.0f;
	} else if (count > 16 && count < 25) {
		outHeight = 129.0f;
		inHeight = 128.0f;
	} else {
		return;
	}

	m_frame0.SetFrameHeight(outHeight);
	m_frame1.SetFrameHeight(inHeight);
}

// treasuredlg.cpp:1865
WPoint FrTreasureGift::GetCalcPoint(void) {
	WRect rect = m_rect;

	WPoint pt;
	pt.x = (float)(int)((g_view->GetWidth() - rect.w) * 0.5f);
	pt.y = (float)(int)((g_view->GetHeight() - rect.h) * 0.5f);

	pt.y += 50.0f;
	return pt;
}

// treasuredlg.cpp:1878
void FrTreasureGift::SetState(bool bState) {
	if (m_bState == bState) {
		return;
	}
	m_bState = bState;
	if (!bState) {
		SetVisible(false);
	} else {
		MoveWindow(GetCalcPoint());
		SetVisible(true);
	}
}

// treasuredlg.cpp:1896
void FrTreasureGift::OnOwnerDraw_Gift_List(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem) {
		m_boxOpener.DrawGiftList(pItem);
	}
}

// treasuredlg.cpp:1906
void FrTreasureGift::OnProc(const float dt) {
	m_boxOpener.OnProc(dt);
}

// treasuredlg.cpp:1913
void FrTreasureGift::OnInit_RangeArea_Out(int param) {
	m_frame0.AddFrame(DYNAMIC_CAST(FrArea, param));
}

// treasuredlg.cpp:1918
void FrTreasureGift::OnInit_RangeArea_In(int param) {
	m_frame1.AddFrame(DYNAMIC_CAST(FrArea, param));
}

// treasuredlg.cpp:1923
void FrTreasureGift::OnInit_Gift_List(intptr_t param) {
	m_boxOpener.m_pGiftList = DYNAMIC_CAST(FrListBox, param);
}

// treasuredlg.cpp:1928
void FrTreasureGift::OnInit_GridArea(int param) {
	m_pGridArea = DYNAMIC_CAST(FrArea, param);
}

// treasuredlg.cpp:1933
void FrTreasureGift::OnOwnerDraw_GridArea(int param) {
	WRect rect = m_boxOpener.m_pGiftList->GetRect();

	// Horizontal lines of the gift grid.
	WOverlay::DrawBox(g_view, WRect(rect.x + 25.0f, rect.y + 30.0f, rect.w - 65.0f, 1.0f), 0, data_a94af8, 0.0f);

	if (m_unknown151 == 2) {
		WOverlay::DrawBox(g_view, WRect(rect.x + 25.0f, rect.y + 98.0f, rect.w - 65.0f, 1.0f), 0, data_a94af8, 0.0f);
	} else if (m_unknown151 == 3) {
		WOverlay::DrawBox(g_view, WRect(rect.x + 25.0f, rect.y + 166.0f, rect.w - 65.0f, 1.0f), 0, data_a94af8, 0.0f);
	}

	// Vertical lines.
	for (int i = 0; i < 8; ++i) {
		WOverlay::DrawBox(g_view, WRect(rect.x + i * 65.0f + 33.0f, rect.y + 35.0f, 1.0f, rect.h - 60.0f), 0, data_a94af8, 0.0f);
	}
}

// treasuredlg.cpp:1962
void FrTreasureGift::OnInit_Box_Area(int param) {
	m_boxOpener.m_pBoxArea = DYNAMIC_CAST(FrArea, param);
	if (m_boxOpener.m_pBoxArea) {
		WRect rect = m_boxOpener.m_pBoxArea->GetRect();

		rect.y = m_rect.y - 10.0f;
		m_boxOpener.m_pBoxArea->SetRect(rect);
	}
}

// treasuredlg.cpp:1975
void FrTreasureGift::OnOwnerDraw_Box_Area(int param) {
	m_boxOpener.DrawBoxArea();
}

// treasuredlg.cpp:1981 (run-time type and factory registration of FrTestDlg_combo)
const WRTTI FrTestDlg_combo::m_RTTI("FrTestDlg_combo", &FrForm::m_RTTI);

IObject* FrTestDlg_comboMakeInstance(void) {
	return new FrTestDlg_combo;
}

struct __sFrTestDlg_combo {
	__sFrTestDlg_combo(void) {
		ObjectFactory().AddObjectFunctor(FrTestDlg_comboMakeInstance, "FrTestDlg_combo");
	}
};
static __sFrTestDlg_combo s_registerFrTestDlg_combo;

// treasuredlg.cpp:1983
const sFRESH_MSGMAP* FrTestDlg_combo::GetMessageMap(void) const {
	return &FrTestDlg_combo::_MsgMap;
}

sFRESH_MSGMAP FrTestDlg_combo::_MsgMap = { &FrForm::_MsgMap, &FrTestDlg_combo::_MsgEntries[0] };

// treasuredlg.cpp:1985
sFRESH_ENTRY FrTestDlg_combo::_MsgEntries[] = {
	{ "mapcomboex", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTestDlg_combo::OnInit_combo },
	{ "mapcomboex", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrTestDlg_combo::ONLBTN_DOWN },

	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// treasuredlg.cpp:1991
FrTestDlg_combo::FrTestDlg_combo(void) {
	m_pCombo = 0;
}

// treasuredlg.cpp:1996
FrTestDlg_combo::~FrTestDlg_combo(void) {
}

// treasuredlg.cpp:2001
void FrTestDlg_combo::OnInit_combo(intptr_t param) {
	m_pCombo = DYNAMIC_CAST(FrComboCtlEx, param);
	if (m_pCombo) {
		m_pCombo->AddTexItem("hole_front");
		m_pCombo->AddTexItem("hole_back");
		m_pCombo->AddTexItem("hole_random");
		m_pCombo->AddTexItem("hole_shuffle");
		m_pCombo->SelectItem(0);
	}
}

// treasuredlg.cpp:2016
bool FrTestDlg_combo::OnInit(void) {
	return true;
}

// treasuredlg.cpp:2021
void FrTestDlg_combo::ONLBTN_DOWN(void) {
}

// treasuredlg.cpp:2030
CBoxOpener::CBoxOpener(void) {
	m_unknown54 = 0;
	m_openGiftIdx = 0;
	m_giftCount = 0;
	m_unknown64 = 0;

	m_pGiftList = 0;
	m_unknown48 = 0;
	m_nextSeq = SEQ_NONE;
	m_curSeq = SEQ_NONE;
	m_unknown74 = 0;
	m_seqDuration = 2000;

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	char* filename = "[font_wind.jpg";
	info.filename = &filename;
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:";

	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);
}

// treasuredlg.cpp:2060
CBoxOpener::~CBoxOpener(void) {
	if (m_pSimpleUI) { delete m_pSimpleUI; m_pSimpleUI = 0; }
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }

	m_unknown74 = 0;
}

// treasuredlg.cpp:2163
void CBoxOpener::OnProc(float dt) {
	m_dt = dt;
	if (m_nextSeq != SEQ_NONE) {
		ChangeSequence(m_nextSeq);
	}

	if (m_pProcFunc != NULL) {
		(this->*m_pProcFunc)();
	}
}

// treasuredlg.cpp:2178
void CBoxOpener::ChangeSequence(eSeqProc seq) {
	switch (seq) {
	case SEQ_INIT:
		m_seqStartTime = timeGetTime();
		m_pProcFunc = &CBoxOpener::ProcInit;
		WSingleton<CTHunter>::Instance()->SetReturnLobby(false);
		break;
	case SEQ_CREATE:
		m_seqStartTime = timeGetTime();
		m_seqDuration = 250;
		m_pProcFunc = &CBoxOpener::ProcCreate;
		m_unknown48 = 3;
		break;
	case SEQ_MYITEM_OPEN:
		m_seqStartTime = timeGetTime();
		m_pProcFunc = &CBoxOpener::ProcMyItemOpen;
		m_unknown48 = 4;
		for (unsigned char i = 0; i < m_giftCount; ++i) {
			GetGiftItem(i);
		}
		m_openGiftIdx = 0;
		m_seqDuration = 200;
		break;
	case SEQ_COMPLETE:
		m_seqStartTime = timeGetTime();
		m_pProcFunc = &CBoxOpener::ProcComplete;
		m_seqDuration = 1500;
		break;
	case SEQ_UNKNOWN5:
		break;
	case SEQ_END:
		m_seqStartTime = timeGetTime();
		m_pProcFunc = &CBoxOpener::ProcEnd;
		m_seqDuration = 3000;
		break;
	case SEQ_CLOSE:
		WSingleton<CTHunter>::Instance()->SetReturnLobby(true);
		break;
	}

	m_curSeq = seq;
	m_nextSeq = SEQ_NONE;
}

// treasuredlg.cpp:2266
void CBoxOpener::ProcInit(void) {
	if (timeGetTime() - m_seqStartTime > 1000) {
		for (unsigned char i = 0; i < m_pGiftList->GetCurrentItemSize(true); ++i) {
			GetGiftItem(i)->m_unknown10 = 1;
		}
		m_nextSeq = SEQ_CREATE;
	}
}

// treasuredlg.cpp:2283
void CBoxOpener::ProcCreate(void) {
	if (timeGetTime() - m_seqStartTime > m_seqDuration) {
		if (m_openGiftIdx < m_giftCount) {
			sTGift* pGift = GetGiftItem(m_openGiftIdx++);
			if (pGift && pGift->m_unknown8) {
				pGift->m_unknownf = 1;
				pGift->m_unknown10 = 3;
				g_audio->PlaySfx("play_fail_item", 0, 0, 0, 0, 0.5f, 200.0f);
			}
		} else if (m_openGiftIdx < m_unknown64) {
			sTGift* pGift = GetGiftItem(m_openGiftIdx++);
			pGift->m_unknown10 = 2;
		} else {
			m_openGiftIdx++;
		}

		m_seqStartTime = timeGetTime();

		if (m_openGiftIdx == m_giftCount) {
			m_seqDuration = 50;
		}
		if (m_openGiftIdx == m_unknown64) {
			m_nextSeq = SEQ_MYITEM_OPEN;
		}
	}
}

// treasuredlg.cpp:2324
void CBoxOpener::ProcMyItemOpen(void) {
	if (m_openGiftIdx < m_giftCount && timeGetTime() - m_seqStartTime > 250) {
		if (m_openGiftIdx < m_giftCount) {
			sTGift* pGift = GetGiftItem(m_openGiftIdx++);
			if (pGift && pGift->m_unknown8) {
				pGift->m_unknown10 = 4;
				pGift->m_unknown4 = 0;
				g_audio->PlaySfx("play_fail_item", 0, 0, 0, 0, 0.5f, 200.0f);
			} else {
				m_openGiftIdx++;
			}
			m_seqStartTime = timeGetTime();
		}
	}

	if (m_openGiftIdx == m_giftCount && timeGetTime() - m_seqStartTime > m_seqDuration) {
		m_nextSeq = SEQ_COMPLETE;
	}
}

// treasuredlg.cpp:2355
void CBoxOpener::ProcComplete(void) {
	if (timeGetTime() - m_seqStartTime > m_seqDuration && m_curSeq == SEQ_COMPLETE) {
		m_nextSeq = SEQ_CLOSE;
	}
}

// treasuredlg.cpp:2364
void CBoxOpener::ProcBoxNone(void) {
}

// treasuredlg.cpp:2369
void CBoxOpener::ProcEnd(void) {
	if (timeGetTime() - m_seqStartTime > m_seqDuration && m_curSeq == SEQ_END) {
		m_nextSeq = SEQ_CLOSE;
	}
}

// treasuredlg.cpp:2381
CBoxOpener::sTGift* CBoxOpener::GetGiftItem(int index) {
	FrListItem* pItem = m_pGiftList->GetItem(index);
	if (pItem == NULL) {
		return NULL;
	}

	return (sTGift*)pItem->pData;
}

// treasuredlg.cpp:2565
void CBoxOpener::GetItemOffset(WPoint& pt, unsigned long typeId, bool bOpened) {
	if (bOpened == true) {
		switch (typeId >> 26) {
		case 6:
			// Both coupon kinds use the same offset in the original.
			if (ItemManager()->GetCouponKind(typeId) == 0)
				pt = WPoint(0.0f, 0.0f);
			else
				pt = WPoint(0.0f, 0.0f);
			break;
		case 0x1c:
			pt = WPoint(-10.0f, -20.0f);
			break;
		case 0x1f:
			pt = WPoint(0.0f, -10.0f);
			break;
		}
	} else {
		switch (typeId >> 26) {
		case 6:
			if (ItemManager()->GetCouponKind(typeId) == 0)
				pt = WPoint(40.0f, 33.0f);
			else
				pt = WPoint(40.0f, 33.0f);
			break;
		case 0x1c:
			pt = WPoint(48.0f, 50.0f);
			break;
		case 0x1f:
			pt = WPoint(45.0f, 43.0f);
			break;
		}
	}
}
