// Original: d:\build\custom\temp\code\client\projectg\frimagenoticedlg.cpp
#include "FrImageNoticeDlg.h"
#include "ObjectFactory.h"
#include "frbutton.h"
#include "frarea.h"
#include "wview.h"

// frimagenoticedlg.cpp:14 (run-time type and factory registration of FrImageNoticeDlg)
const WRTTI FrImageNoticeDlg::m_RTTI("FrImageNoticeDlg", &FrForm::m_RTTI);

IObject* FrImageNoticeDlgMakeInstance(void) {
	return new FrImageNoticeDlg;
}

struct __sFrImageNoticeDlg {
	__sFrImageNoticeDlg(void) {
		ObjectFactory().AddObjectFunctor(FrImageNoticeDlgMakeInstance, "FrImageNoticeDlg");
	}
};
static __sFrImageNoticeDlg s_registerFrImageNoticeDlg;

// frimagenoticedlg.cpp:16
const sFRESH_MSGMAP* FrImageNoticeDlg::GetMessageMap(void) const {
	return &FrImageNoticeDlg::_MsgMap;
}

sFRESH_MSGMAP FrImageNoticeDlg::_MsgMap = { &FrForm::_MsgMap, &FrImageNoticeDlg::_MsgEntries[0] };

sFRESH_ENTRY FrImageNoticeDlg::_MsgEntries[] = {
	// frimagenoticedlg.cpp:18
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrImageNoticeDlg::OnInitCloseButton },
	// frimagenoticedlg.cpp:19
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrImageNoticeDlg::OnInitOkButton },
	// frimagenoticedlg.cpp:20
	{ "ok", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrImageNoticeDlg::OnLBDownOkButton },
	// frimagenoticedlg.cpp:21
	{ "lefttop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrImageNoticeDlg::OnInitLeftTop },
	// frimagenoticedlg.cpp:22
	{ "centertop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrImageNoticeDlg::OnInitCenterTop },
	// frimagenoticedlg.cpp:23
	{ "righttop", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrImageNoticeDlg::OnInitRightTop },
	// frimagenoticedlg.cpp:24
	{ "leftmiddle", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrImageNoticeDlg::OnInitLeftMiddle },
	// frimagenoticedlg.cpp:25
	{ "centermiddle", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrImageNoticeDlg::OnInitCenterMiddle },
	// frimagenoticedlg.cpp:26
	{ "rightmiddle", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrImageNoticeDlg::OnInitRightMiddle },
	// frimagenoticedlg.cpp:27
	{ "leftbottom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrImageNoticeDlg::OnInitLeftBottom },
	// frimagenoticedlg.cpp:28
	{ "centerbottom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrImageNoticeDlg::OnInitCenterBottom },
	// frimagenoticedlg.cpp:29
	{ "rightbottom", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrImageNoticeDlg::OnInitRightBottom },

	// frimagenoticedlg.cpp:31
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frimagenoticedlg.cpp:38
FrImageNoticeDlg::FrImageNoticeDlg(void) {
	m_pCloseButton = 0;
	m_pOkButton = 0;
}

// frimagenoticedlg.cpp:52
FrImageNoticeDlg::~FrImageNoticeDlg(void) {
}

// frimagenoticedlg.cpp:56
void FrImageNoticeDlg::OnInitCloseButton(intptr_t param) {
	m_pCloseButton = DYNAMIC_CAST(FrButton, param);
}

// frimagenoticedlg.cpp:62
void FrImageNoticeDlg::OnInitOkButton(intptr_t param) {
	m_pOkButton = DYNAMIC_CAST(FrButton, param);
	if (m_pOkButton) {
		m_pOkButton->SetVisible(false);
		m_pOkButton->Enable(false);
	}
}

// frimagenoticedlg.cpp:73
void FrImageNoticeDlg::OnLBDownOkButton(void) {
	Close(FrOK, true);
}

// frimagenoticedlg.cpp:78
void FrImageNoticeDlg::OnInitLeftTop(intptr_t param) {
	m_pArea[0][0] = DYNAMIC_CAST(FrArea, param);
}

// frimagenoticedlg.cpp:84
void FrImageNoticeDlg::OnInitCenterTop(intptr_t param) {
	m_pArea[0][1] = DYNAMIC_CAST(FrArea, param);
}

// frimagenoticedlg.cpp:90
void FrImageNoticeDlg::OnInitRightTop(intptr_t param) {
	m_pArea[0][2] = DYNAMIC_CAST(FrArea, param);
}

// frimagenoticedlg.cpp:97
void FrImageNoticeDlg::OnInitLeftMiddle(intptr_t param) {
	m_pArea[1][0] = DYNAMIC_CAST(FrArea, param);
}

// frimagenoticedlg.cpp:103
void FrImageNoticeDlg::OnInitCenterMiddle(intptr_t param) {
	m_pArea[1][1] = DYNAMIC_CAST(FrArea, param);
}

// frimagenoticedlg.cpp:109
void FrImageNoticeDlg::OnInitRightMiddle(intptr_t param) {
	m_pArea[1][2] = DYNAMIC_CAST(FrArea, param);
}

// frimagenoticedlg.cpp:115
void FrImageNoticeDlg::OnInitLeftBottom(intptr_t param) {
	m_pArea[2][0] = DYNAMIC_CAST(FrArea, param);
}

// frimagenoticedlg.cpp:121
void FrImageNoticeDlg::OnInitCenterBottom(intptr_t param) {
	m_pArea[2][1] = DYNAMIC_CAST(FrArea, param);
}

// frimagenoticedlg.cpp:127
void FrImageNoticeDlg::OnInitRightBottom(intptr_t param) {
	m_pArea[2][2] = DYNAMIC_CAST(FrArea, param);
}

// frimagenoticedlg.cpp:133
bool FrImageNoticeDlg::SetOkButton(WRect& rect, bool bRelative) {
	bool bRet = false;
	if (m_pOkButton) {
		if (bRelative) {
			WRect rc = m_pOkButton->GetRect();
			rect = rc + rect;
		}

		m_pOkButton->SetRect(rect);

		m_pOkButton->Enable(true);
		m_pOkButton->SetVisible(true);

		bRet = true;
	}
	return bRet;
}

// frimagenoticedlg.cpp:153
bool FrImageNoticeDlg::SetCloseButton(WRect& rect, bool bRelative) {
	bool bRet = false;
	if (m_pCloseButton) {
		if (bRelative) {
			WRect rc = m_pCloseButton->GetRect();
			rect = rc + rect;
		}

		m_pCloseButton->SetRect(rect);
		bRet = true;
	}
	return bRet;
}

// frimagenoticedlg.cpp:169
void FrImageNoticeDlg::SetDlgSize(WRect& rect) {
	if (rect.x <= -1.0f) {
		rect.x = (float)((int)g_view->GetWidth() / 2 - (int)rect.w / 2);
	}

	if (rect.y <= -1.0f) {
		rect.y = (float)((int)g_view->GetHeight() / 2 - (int)rect.h / 2);
	}

	SetRect(rect);

	if (m_pCloseButton) {
		WRect rc = m_pCloseButton->GetRect();
		rc.x = m_rect.x + rect.w - 53.0f;
		m_pCloseButton->SetRect(rc);
	}
}

// frimagenoticedlg.cpp:193
bool FrImageNoticeDlg::SetBackGroundImage(int x, int y, WRect& rect, const char* name, bool bRelative) {
	bool bRet = false;
	FrArea*& pArea = m_pArea[y][x];
	if (pArea) {
		if (bRelative) {
			WRect rc = GetRect();
			rect = rc + rect;
			rect.y += 23.0f;
			rect.h += 23.0f;
		}

		pArea->SetRect(rect);
		pArea->SetBgImg(name);
		bRet = true;
	}
	return bRet;
}
