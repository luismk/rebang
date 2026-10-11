// Original: d:\build\custom\temp\code\client\projectg\frquickmovedlg.cpp
#include "FrQuickMoveDlg.h"
#include "ObjectFactory.h"
#include "TaskManager.h"
#include "commonutil.h"
#include "frbutton.h"
#include "frviewer.h"
#include <string.h>

// Notice image of each page (index = m_page; 0 is unused).
static char data_00ab4410[5][64] = {
	"DUMMY",
	"user_notice_1.jpg",
	"user_notice_2.jpg",
	"user_notice_3.jpg",
	"user_notice_4.jpg"
};

// Move button image prefix of each page ("DUMMY": the page hides the button).
static char data_00ab4550[5][64] = {
	"DUMMY",
	"btn_notice_go_",
	"DUMMY",
	"DUMMY",
	"btn_notice_go_selfdesign_"
};

// frquickmovedlg.cpp:36 (run-time type and factory registration of FrQuickMoveDlg)
const WRTTI FrQuickMoveDlg::m_RTTI("FrQuickMoveDlg", &FrForm::m_RTTI);

IObject* FrQuickMoveDlgMakeInstance(void) {
	return new FrQuickMoveDlg;
}

struct __sFrQuickMoveDlg {
	__sFrQuickMoveDlg(void) {
		ObjectFactory().AddObjectFunctor(FrQuickMoveDlgMakeInstance, "FrQuickMoveDlg");
	}
};
static __sFrQuickMoveDlg s_registerFrQuickMoveDlg;

// frquickmovedlg.cpp:38
const sFRESH_MSGMAP* FrQuickMoveDlg::GetMessageMap(void) const {
	return &FrQuickMoveDlg::_MsgMap;
}

sFRESH_MSGMAP FrQuickMoveDlg::_MsgMap = { &FrForm::_MsgMap, &FrQuickMoveDlg::_MsgEntries[0] };

sFRESH_ENTRY FrQuickMoveDlg::_MsgEntries[] = {
	// frquickmovedlg.cpp:40
	{ "desc_view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickMoveDlg::OnInitDescViewer },
	// frquickmovedlg.cpp:41
	{ "desc_view", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrQuickMoveDlg::OnLBDownViewer },

	// frquickmovedlg.cpp:43
	{ "button_prev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickMoveDlg::OnInitBtnPrev },
	// frquickmovedlg.cpp:44
	{ "button_next", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickMoveDlg::OnInitBtnNext },
	// frquickmovedlg.cpp:45
	{ "button_move", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrQuickMoveDlg::OnInitBtnMove },

	// frquickmovedlg.cpp:47
	{ "button_prev", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrQuickMoveDlg::OnLBDownBtnPrev },
	// frquickmovedlg.cpp:48
	{ "button_next", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrQuickMoveDlg::OnLBDownBtnNext },
	// frquickmovedlg.cpp:49
	{ "button_move", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrQuickMoveDlg::OnLBDownBtnMove },


	// frquickmovedlg.cpp:52
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frquickmovedlg.cpp:57
FrQuickMoveDlg::FrQuickMoveDlg(void) {
	m_pDescViewer = 0;
	memset(m_descImage, 0, sizeof(m_descImage));

	m_page = 1;
}

// frquickmovedlg.cpp:65
FrQuickMoveDlg::~FrQuickMoveDlg(void) {
	m_page = 1;
}

// frquickmovedlg.cpp:70
void FrQuickMoveDlg::SetDescImage(const char* fileName) {
	if (m_pDescViewer) {

		memset(m_descImage, 0, sizeof(m_descImage));
		strcpy(m_descImage, fileName);
		m_pDescViewer->ShowScrollBar(true);
		m_pDescViewer->Open(m_descImage);
	}
}

// frquickmovedlg.cpp:82
void FrQuickMoveDlg::OnInitDescViewer(intptr_t param) {
	m_pDescViewer = DYNAMIC_CAST(FrViewer, param);


	if (m_pDescViewer) {

		SetDescImage(data_00ab4410[m_page]);
	}
}

// frquickmovedlg.cpp:93
void FrQuickMoveDlg::OnLBDownViewer(void) {
	Close(true);
}

// frquickmovedlg.cpp:98
void FrQuickMoveDlg::OnInitBtnPrev(intptr_t param) {
	m_pBtnPrev = DYNAMIC_CAST(FrButton, param);

	if (m_pBtnPrev) {

		m_pBtnPrev->SetPushDelay(0.1f);
	}
}

// frquickmovedlg.cpp:108
void FrQuickMoveDlg::OnInitBtnNext(intptr_t param) {
	m_pBtnNext = DYNAMIC_CAST(FrButton, param);

	if (m_pBtnNext) {

		m_pBtnNext->SetPushDelay(0.1f);
	}
}

// frquickmovedlg.cpp:118
void FrQuickMoveDlg::OnInitBtnMove(intptr_t param) {
	m_pBtnMove = DYNAMIC_CAST(FrButton, param);

	if (m_pBtnMove) {

		m_pBtnMove->SetPushDelay(0.1f);
	}
}

// frquickmovedlg.cpp:128
void FrQuickMoveDlg::OnLBDownBtnPrev(void) {
	if (m_page > 1) {

		m_page--;
	}


	SetDescImage(data_00ab4410[m_page]);

	if (m_page == 3 || m_page == 2) {

		m_pBtnMove->SetVisible(false);
	}
	else {

		m_pBtnMove->SetVisible(true);
		m_pBtnMove->SetButtonImg(MakeStr("%sn", data_00ab4550[m_page]), FrButton::NORMAL);
		m_pBtnMove->SetButtonImg(MakeStr("%so", data_00ab4550[m_page]), FrButton::OVER);
		m_pBtnMove->SetButtonImg(MakeStr("%sd", data_00ab4550[m_page]), FrButton::PRESSED);
	}
}

// frquickmovedlg.cpp:150
void FrQuickMoveDlg::OnLBDownBtnNext(void) {


	if (m_page < 4) {

		m_page++;
	}


	SetDescImage(data_00ab4410[m_page]);

	if (m_page == 3 || m_page == 2) {

		m_pBtnMove->SetVisible(false);
	}
	else {

		m_pBtnMove->SetVisible(true);
		m_pBtnMove->SetButtonImg(MakeStr("%sn", data_00ab4550[m_page]), FrButton::NORMAL);
		m_pBtnMove->SetButtonImg(MakeStr("%so", data_00ab4550[m_page]), FrButton::OVER);
		m_pBtnMove->SetButtonImg(MakeStr("%sd", data_00ab4550[m_page]), FrButton::PRESSED);
	}
}

// frquickmovedlg.cpp:174
void FrQuickMoveDlg::OnLBDownBtnMove(void) {
	switch (m_page) {
	case 1:
		CTaskManager::Instance()->ChangeTask("CShopTask", "", false);
		// PostMsg's params are int (?PostMsg@CTaskManager@@QAEHPBVIActor@@PBDHHHHK@Z):
		// the original passes the string as param1.
		CTaskManager::Instance()->PostMsg(NULL, "Shop", 0, (intptr_t)"SHOPMAIN", 0, 0, 0);
		CTaskManager::Instance()->PostMsg(NULL, "Shop", 0xC6, 4, 0, 0, 0);
		break;
	case 4:
		CTaskManager::Instance()->ChangeTask("CShopTask", "", false);
		CTaskManager::Instance()->PostMsg(NULL, "Shop", 0, (intptr_t)"SHOPMAIN", 0, 0, 0);
		CTaskManager::Instance()->PostMsg(NULL, "Shop", 0xC6, 3, 0, 0, 0);
		break;
	}
	Close(true);
}
