// Original: d:\build\custom\temp\code\client\projectg\eventprizedlg.cpp
#include "EventPrizeDlg.h"
#include "ObjectFactory.h"
#include "commonutil.h"
#include "frbutton.h"
#include "frviewer.h"

// eventprizedlg.cpp:16 (run-time type and factory registration of FrEventPrizeDlg)
const WRTTI FrEventPrizeDlg::m_RTTI("FrEventPrizeDlg", &FrForm::m_RTTI);

IObject* FrEventPrizeDlgMakeInstance(void) {
	return new FrEventPrizeDlg;
}

struct __sFrEventPrizeDlg {
	__sFrEventPrizeDlg(void) {
		ObjectFactory().AddObjectFunctor(FrEventPrizeDlgMakeInstance, "FrEventPrizeDlg");
	}
};
static __sFrEventPrizeDlg s_registerFrEventPrizeDlg;

// eventprizedlg.cpp:18
const sFRESH_MSGMAP* FrEventPrizeDlg::GetMessageMap(void) const {
	return &FrEventPrizeDlg::_MsgMap;
}

sFRESH_MSGMAP FrEventPrizeDlg::_MsgMap = { &FrForm::_MsgMap, &FrEventPrizeDlg::_MsgEntries[0] };

sFRESH_ENTRY FrEventPrizeDlg::_MsgEntries[] = {
	// eventprizedlg.cpp:20
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEventPrizeDlg::OnCancelInit },
	// eventprizedlg.cpp:21
	{ "view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEventPrizeDlg::OnViewInit },
	// eventprizedlg.cpp:22
	{ "view", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrEventPrizeDlg::OnViewBtnUp },

	// eventprizedlg.cpp:24
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// eventprizedlg.cpp:27
bool FrEventPrizeDlg::OnInit(void) {
	if (m_viewBtn) {
		m_viewBtn->Open(MakeStr("event%02d.jpg", m_event));

		// Grow the viewer to the image (at most 600x400) and the form with it.
		int dw = m_viewBtn->GetSrcWidth();
		if (dw > 600)
			dw = 600;
		dw -= (int)m_viewBtn->GetViewWidth();

		int dh = m_viewBtn->GetSrcHeight();
		if (dh > 400)
			dh = 400;
		dh -= (int)m_viewBtn->GetViewHeight();

		WRect rect = m_viewBtn->GetRect();
		rect.w += dw;
		rect.h += dh;
		m_viewBtn->SetRect(rect);

		if (m_cancelBtn) {
			rect = m_cancelBtn->GetRect();
			rect.x += dw;
			m_cancelBtn->MoveWindow(WPoint(rect.x, rect.y));
		}

		rect = m_rect;
		rect.x -= dw >> 1;
		rect.y -= dh >> 1;
		rect.w += dw;
		rect.h += dh;
		SetRect(rect);

		FrWnd* prev = FindChildByName("prev");
		if (prev)
			prev->SetVisible(false);

		FrWnd* next = FindChildByName("next");
		if (next)
			next->SetVisible(false);
	}

	return FrForm::OnInit();
}

// eventprizedlg.cpp:82
void FrEventPrizeDlg::OnViewInit(intptr_t param) {
	m_viewBtn = DYNAMIC_CAST(FrViewer, param);
}

// eventprizedlg.cpp:89
void FrEventPrizeDlg::OnViewBtnUp(void) {
	Close(true);
}

// eventprizedlg.cpp:94
void FrEventPrizeDlg::OnCancelInit(intptr_t param) {
	m_cancelBtn = DYNAMIC_CAST(FrButton, param);
}
