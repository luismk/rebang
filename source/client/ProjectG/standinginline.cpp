// Original: d:\build\custom\temp\code\client\projectg\standinginline.cpp
#include "StandingInLine.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frviewer.h"

// Labels of the list's numbers 1-13 (zero-initialized, no symbol).
static const char* data_00af6508[14];

// standinginline.cpp:22 (run-time type and factory registration of CStandingInLine)
const WRTTI CStandingInLine::m_RTTI("CStandingInLine", &FrForm::m_RTTI);

IObject* CStandingInLineMakeInstance(void) {
	return new CStandingInLine;
}

struct __sCStandingInLine {
	__sCStandingInLine(void) {
		ObjectFactory().AddObjectFunctor(CStandingInLineMakeInstance, "CStandingInLine");
	}
};
static __sCStandingInLine s_registerCStandingInLine;

// standinginline.cpp:23
const sFRESH_MSGMAP* CStandingInLine::GetMessageMap(void) const {
	return &CStandingInLine::_MsgMap;
}

sFRESH_MSGMAP CStandingInLine::_MsgMap = { &FrForm::_MsgMap, &CStandingInLine::_MsgEntries[0] };

sFRESH_ENTRY CStandingInLine::_MsgEntries[] = {
	// standinginline.cpp:26
	{ "Viewer_Total", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CStandingInLine::OnView_TotalInit },
	// standinginline.cpp:27
	{ "Viewer_Total", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CStandingInLine::OnView_TotalOwnerDraw },
	// standinginline.cpp:28
	{ "Viewer_Total", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CStandingInLine::OnView_TotalButtonUp },

	// standinginline.cpp:30
	{ "List_BtnArray", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CStandingInLine::OnArrayListInit },
	// standinginline.cpp:31
	{ "List_BtnArray", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CStandingInLine::OnArrayListOwnerDraw },
	// standinginline.cpp:32
	{ "List_BtnArray", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CStandingInLine::OnArrayListBtnDown },
	// standinginline.cpp:33
	{ "List_BtnArray", FRCMD_RBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CStandingInLine::OnArrayListRBtnDown },

	// standinginline.cpp:36
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// standinginline.cpp:40
CStandingInLine::CStandingInLine(void) {
	m_pViewTotal = 0;
	m_pArrayList = 0;
	m_selectNum = 0;
}

// standinginline.cpp:47
CStandingInLine::~CStandingInLine(void) {
}

// standinginline.cpp:52
void CStandingInLine::OnView_TotalInit(intptr_t param) {
	m_pViewTotal = DYNAMIC_CAST(FrViewer, param);

	if (m_pViewTotal) {


		WRect rect = m_pViewTotal->GetRect();

		m_pViewTotal->SetRect(WRect(rect.x, rect.y, rect.w + 91.0f, rect.h));
	}
}

// standinginline.cpp:64
void CStandingInLine::OnView_TotalOwnerDraw(int param) {
	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	pDevice->SetTextColor(0xff1f1f1f, 0xffffffff);

	const WRect& rect = m_pViewTotal->GetRect();
	WPoint pos(rect.x + 30.0f, rect.y);



	pDevice->Print(pos, 0, "\xc0\xfc \xc3\xbc");
}

// standinginline.cpp:76
void CStandingInLine::OnView_TotalButtonUp(void) {
	m_selectNum = 0;
	FrForm::OnOK();
}

// standinginline.cpp:82
void CStandingInLine::OnArrayListInit(intptr_t param) {
	m_pArrayList = DYNAMIC_CAST(FrListBox, param);

	if (m_pArrayList) {


		m_pArrayList->UseRightButton(true);

		for (int i = 1; i < 14; i++) {

			m_pArrayList->AddItem((void*)i);
		}
	}
}

// standinginline.cpp:96
void CStandingInLine::OnArrayListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem) {
		int num = (intptr_t)pItem->pData;
		if (num) {
			FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
			pDevice->SetTextColor(0xff1f1f1f, 0xffffffff);
			pDevice->Print(pItem->pos, 0, "%s", data_00af6508[num]);
		}
	}
}

// standinginline.cpp:112
void CStandingInLine::OnArrayListBtnDown(void) {
	FrListItem* pItem = m_pArrayList->GetItemUnderCursor();
	if (!pItem) {
		return;
	}

	int num = (intptr_t)pItem->pData;
	if (!num) {
		return;
	}

	if (num > 13) {
		return;
	}

	m_selectNum = num;

	FrForm::OnOK();
}

// standinginline.cpp:134
void CStandingInLine::OnArrayListRBtnDown(void) {
	FrForm::OnCancel();
}
