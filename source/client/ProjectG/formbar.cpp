// Original: d:\build\custom\temp\code\client\projectg\formbar.cpp
#include "formbar.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"

// formbar.cpp:13 (run-time type and factory registration of FrFormBar)
const WRTTI FrFormBar::m_RTTI("FrFormBar", &FrForm::m_RTTI);

IObject* FrFormBarMakeInstance(void) {
	return new FrFormBar;
}

struct __sFrFormBar {
	__sFrFormBar(void) {
		ObjectFactory().AddObjectFunctor(FrFormBarMakeInstance, "FrFormBar");
	}
};
static __sFrFormBar s_registerFrFormBar;

// formbar.cpp:14
const sFRESH_MSGMAP* FrFormBar::GetMessageMap(void) const {
	return &FrFormBar::_MsgMap;
}

sFRESH_MSGMAP FrFormBar::_MsgMap = { &FrForm::_MsgMap, &FrFormBar::_MsgEntries[0] };

sFRESH_ENTRY FrFormBar::_MsgEntries[] = {
	// formbar.cpp:16
	{ "gauge_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFormBar::OnInitGaugeArea },
	// formbar.cpp:17
	{ "gauge_area", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrFormBar::OnDrawGaugeArea },
	// formbar.cpp:18
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFormBar::OnInitBtnOK },
	// formbar.cpp:19
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrFormBar::OnDownBtnOK },
	// formbar.cpp:20
	{ "no", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFormBar::OnInitBtnNO },
	// formbar.cpp:21
	{ "no", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrFormBar::OnDownBtnNO },
	// formbar.cpp:22
	{ "msg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFormBar::OnInitEdMsg },

	// formbar.cpp:24
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// formbar.cpp:30
FrFormBar::FrFormBar(void)
	: m_pGaugeArea(0), m_pGaugeImg(0), m_pFrameImg(0), m_min(0.0f), m_max(100.0f), m_pos(0.0f),
	  m_pivot(1.0f, 1.0f), m_pBtnOK(0), m_pBtnNO(0), m_pEdMsg(0), m_autoCloseTime(0.0f), m_autoCloseTimer(0.0f) {
	m_pBtnNO = 0;
	m_pBtnOK = 0;
}

// formbar.cpp:36
FrFormBar::~FrFormBar(void) {
}

// formbar.cpp:50
void FrFormBar::Attach(const WRect& rect, const char* name) {
	const Bitmap* pBitmap = g_pFresh->GetBitmap(name);

	if (pBitmap) {




		sATTACHRES res;
		res.pBitmap = pBitmap;
		res.rect = rect;
		m_attachList.push_back(res);
	}
}

// formbar.cpp:63
void FrFormBar::SetFixedWnd(void) {
	m_dwStyle.Enable(0x100);
	EnableDrag(false);
}

// formbar.cpp:69
bool FrFormBar::Open(FRESH_RESULT_CALLBACK pfnResult, unsigned long flag) {
	Init();
	return FrForm::Open(pfnResult, flag);
}

// formbar.cpp:75
void FrFormBar::SetGaugeSize(const WSize& size) {
	WRect rect;
	rect.x = m_rect.x;
	rect.y = m_rect.y;
	rect.w = size.w;
	rect.h = size.h;
	SetRect(rect);

	ReCalcRects();
}

// formbar.cpp:85
void FrFormBar::MoveBar(const WPoint& pt) {
	m_barPos.x = pt.x;
	m_barPos.y = pt.y;
}

// formbar.cpp:90
WPoint FrFormBar::GetRelativeBarPos(void) {
	return m_barPos;
}

// formbar.cpp:95
void FrFormBar::Init(void) {
	MakeDefault();
	m_defaultRect = m_rect;
}

// formbar.cpp:101
void FrFormBar::SetFrameImg(const char* name) {
	m_pFrameImg = g_pFresh->GetBitmap(name);
}

// formbar.cpp:107
void FrFormBar::SetGaugeImg(const char* name) {
	m_pGaugeImg = g_pFresh->GetBitmap(name);
}

// formbar.cpp:113
void FrFormBar::SetRange(int min, int max) {
	m_min = (float)min;
	m_max = (float)max;
}

// formbar.cpp:119
void FrFormBar::SetPos(float pos) {
	m_pos = pos;
}

// formbar.cpp:124
void FrFormBar::SetGaugeFrameSize(const WSize& size) {
	WSize wndSize(m_rect.w, m_rect.h);

	if (wndSize.w > size.w) {
		m_frameSize.w = size.w;
	}
	else {
		m_frameSize.w = wndSize.w - 2.0f;
	}

	if (wndSize.h > size.h) {
		m_frameSize.h = size.h;
	}
	else {
		m_frameSize.h = wndSize.h - 2.0f;
	}

	ReCalcRects();
}

// formbar.cpp:145
void FrFormBar::SetGaugePivot(const WPoint& pivot) {
	m_pivot = pivot;
	ReCalcRects();
}

// formbar.cpp:151
void FrFormBar::OnProc(const float dt) {
	FrForm::OnProc(dt);

	if (m_autoCloseTime > 0.0f) {

		if (m_autoCloseTimer > 0.0f && (m_autoCloseTimer -= dt) < 0.0f) m_autoCloseTimer = 0.0f;

		m_pos = m_autoCloseTimer / m_autoCloseTime * m_max;

		if (m_autoCloseTimer == 0.0f) {


			if (m_pBtnNO->IsVisible()) {
				FrForm::Close(FrNO, true);
			}
			else {
				FrForm::Close(FrCANCEL, true);
			}
		}
	}
}

// formbar.cpp:173
void FrFormBar::OnInitEdMsg(intptr_t param) {
	m_pEdMsg = DYNAMIC_CAST(FrEdit, param);
	if (m_pEdMsg) {
		m_pEdMsg->SetVisible(false);
	}
}

// formbar.cpp:181
void FrFormBar::OnInitBtnOK(intptr_t param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	if (pBtn) {


		pBtn->SetVisible(false);
		m_pBtnOK = pBtn;
	}
}

// formbar.cpp:192
void FrFormBar::OnDownBtnOK(void) {
	FrForm::Close(FrOK, true);
}

// formbar.cpp:198
void FrFormBar::OnInitBtnNO(intptr_t param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	if (pBtn) {


		pBtn->SetVisible(false);
		m_pBtnNO = pBtn;
	}
}

// formbar.cpp:209
void FrFormBar::OnDownBtnNO(void) {

	FrForm::Close(FrNO, true);
}

// formbar.cpp:217
void FrFormBar::OnInitGaugeArea(intptr_t param) {
	m_pGaugeArea = DYNAMIC_CAST(FrArea, param);
}

// formbar.cpp:224
void FrFormBar::OnDrawGaugeArea(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0) {
		return;
	}



	ReCalcRects();


	for (std::list<sATTACHRES>::iterator it = m_attachList.begin(); it != m_attachList.end(); ++it) {


		WRect src(0.0f, 0.0f, (*it).pBitmap->Width(), (*it).pBitmap->Height());



		WRect dest(m_rect.x + (*it).rect.x, m_rect.y + (*it).rect.y, (*it).rect.w, (*it).rect.h);


		pGDI->DrawTexture((*it).pBitmap, src, dest, 0xFFFFFFFF, 0);
	}

	float pos = m_pos + m_min;
	WRect gauge = m_gaugeRect;

	if (m_max != m_min && pos < m_max) {
		gauge.w = pos / (m_max - m_min) * gauge.w;
	}

	pGDI->DrawTexture(m_pGaugeImg, gauge, 0xFFFFFFFF, 0);


	pGDI->DrawTexture(m_pFrameImg, m_frameRect, 0xFFFFFFFF, 0);
}

// formbar.cpp:264
void FrFormBar::EnableButton(bool enable) {
	m_pBtnOK->SetVisible(enable);
	m_pBtnOK->Enable(enable);

	m_pBtnNO->SetVisible(enable);
	m_pBtnNO->Enable(enable);
}

// formbar.cpp:273
void FrFormBar::SetDesc(const char* desc) {
	if (m_pEdMsg) {

		m_pEdMsg->SetVisible(true);
		m_pEdMsg->SetLine(1, desc, 0, false, 0);
	}
}

// formbar.cpp:282
void FrFormBar::MakeDefault(void) {
	m_pFrameImg = g_pFresh->GetBitmap("formbar_frame");
	m_pGaugeImg = g_pFresh->GetBitmap("formbar_gauge_n");



	SetGaugeFrameSize(WSize(228.0f, 18.0f));
	SetGaugePivot(WPoint(1.0f, 1.0f));

	WSize wndSize(m_rect.w, m_rect.h);
	m_barPos.x = (wndSize.w - m_frameSize.w) * 0.5f;
	m_barPos.y = (wndSize.h - m_frameSize.h) * 0.8f;
}

// formbar.cpp:297
void FrFormBar::ReCalcRects(void) {
	WPoint pt(m_rect.x, m_rect.y);

	m_frameRect.x = pt.x + m_barPos.x;
	m_frameRect.y = pt.y + m_barPos.y;
	m_frameRect.w = m_frameSize.w;
	m_frameRect.h = m_frameSize.h;

	m_gaugeRect.x = m_frameRect.x + m_pivot.x;
	m_gaugeRect.y = m_frameRect.y + m_pivot.y;
	m_gaugeRect.w = m_frameRect.w - (m_pivot.x + m_pivot.x);
	m_gaugeRect.h = m_frameRect.h - (m_pivot.y + m_pivot.y);
}

// formbar.cpp:320
void FrFormBar::SetAutoCloseTimer(float time) {
	m_autoCloseTimer = time;
	m_autoCloseTime = time;

	m_pos = m_max;
}
