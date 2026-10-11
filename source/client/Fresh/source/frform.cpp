// Original: d:\build\custom\temp\code\client\fresh\source\frform.cpp
#include "frform.h"
#include "ObjectFactory.h"
#include "fredit.h"
#include "frdesktop.h"
#include "frframe.h"
#include "frstatic.h"
#include "frwndmanager.h"
#include "../client/ProjectG/SoundManager.h"
#include "../client/Wangreal/include/wview.h"

// <windows.h> maps GetForm to GetFormA; FrElementDoc::GetForm is declared before it.
#undef GetForm

bool FrForm::ms_bHasTail = true;
bool FrForm::ms_bExclusive = false;
WPoint FrForm::ms_oldPos;

// frform.cpp:22 (run-time type and factory registration of FrForm)
const WRTTI FrForm::m_RTTI("FrForm", &FrWnd::m_RTTI);

IObject* FrFormMakeInstance(void) {
	return new FrForm;
}

struct __sFrForm {
	__sFrForm(void) {
		ObjectFactory().AddObjectFunctor(FrFormMakeInstance, "FrForm");
	}
};
static __sFrForm s_registerFrForm;

// frform.cpp:28
const sFRESH_MSGMAP* FrForm::GetMessageMap(void) const {
	return &FrForm::_MsgMap;
}

sFRESH_MSGMAP FrForm::_MsgMap = { &FrCmdTarget::_MsgMap, &FrForm::_MsgEntries[0] };

sFRESH_ENTRY FrForm::_MsgEntries[] = {
	// frform.cpp:30
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrForm::OnFreshOkay },
	// frform.cpp:31
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrForm::OnFreshCancel },
	// frform.cpp:32
	{ "yes", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrForm::OnFreshOkay },
	// frform.cpp:33
	{ "no", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrForm::OnFreshCancel },

	// frform.cpp:35
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frform.cpp:39
void FrForm::OnFreshCancel(void) {
	OnCancel();
}

// frform.cpp:44
void FrForm::OnFreshOkay(void) {
	OnOK();
}

// frform.cpp:53
FrForm::FrForm(void)
	: m_canDrag(true), m_bCaptionDown(false), m_bResizeBtnDown(false), m_pBaseFrm(0), m_pMsgStatic(0),
	  m_pMsgEdit(0), m_timeLimit(-1.0f) {
	m_flag = 0;
	m_defaultRetCode = FrNONE;
}

// frform.cpp:61
FrForm::~FrForm(void) {
}

// frform.cpp:65
void FrForm::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	m_pFnResult = 0;

	FrElementDoc* pDoc = pManager->GetDocument();
	FrElementForm* pElement = pDoc->GetForm(item.m_resource);
	if (pElement == 0) {
		return;
	}

	std::map<std::string, std::string>& param = item.m_param;

	if (param.find("drag") != param.end()) {
		int tf;
		sscanf(param["drag"].c_str(), "%d", &tf);
		EnableDrag(tf != 0);
	}

	if (param.find("modal") != param.end() || (param.find("modal") != param.end() && param["modal"] == "false")) {
		m_flag |= 4;   // modeless
	}

	WRect rect((float)item.m_rect.left, (float)item.m_rect.top, (float)pElement->m_bgSize.w, (float)pElement->m_bgSize.h);
	m_pCmdDest = m_pOwner;
	Create("Form", item.m_name.c_str(), pManager, 0x21, rect, pParent);

	FrWnd* pWnd = pManager->DoCreate(pElement->m_bgFrame, pManager, this, m_pOwner);
	m_pBaseFrm = DYNAMIC_CAST(FrFrame, pWnd);
	SetCaption(pElement->m_caption.c_str());
	SetDesc(pElement->m_desc.c_str());

	std::list<FrGuiItem*>::iterator it;
	for (it = pElement->m_guiList.begin(); it != pElement->m_guiList.end(); ++it) {
		FrWnd* pChild = pManager->DoCreate(**it, pManager, this, m_pOwner);
		switch ((*it)->m_type) {
		case GI_STATIC:
		case GI_EDIT:
			if ((*it)->m_name == "message") {
				SetMessageControlByType((*it)->m_type, pChild);
			}
			break;
		}
	}

	SetViewFocus(false);
}

// frform.cpp:127
void FrForm::SetMessageControlByType(enumGuiType type, FrWnd* pChild) {
	if (type == GI_STATIC) {
		m_pMsgStatic = DYNAMIC_CAST(FrStatic, pChild);
	} else if (type == GI_EDIT) {
		m_pMsgEdit = DYNAMIC_CAST(FrEdit, pChild);
	}
}

// frform.cpp:192
FrForm* FrForm::_Init(FrWndManager* pManager, FrCmdTarget* pCmdDest, const char* lpszTemplateID, FrWnd* pParent) {
	m_pFnResult = 0;

	FrElementDoc* pDoc = pManager->GetDocument();

	FrElementForm* pForm = pDoc->GetForm(lpszTemplateID);   // implicit std::string temp (see CreateLayout)
	if (pForm == 0) {
		return 0;
	}

	pManager->SetKeyFocus(0, true);

	WSize s((float)pForm->m_bgSize.w, (float)pForm->m_bgSize.h);
	WPoint pos = pManager->GetCreatePosition(s);
	WRect rect(pos, s);

	SetOwner(this);
	m_pCmdDest = pCmdDest;

	if (pParent) {
		Create("Form", lpszTemplateID, pManager, 0x21, rect, pParent);
	} else {
		Create("Form", lpszTemplateID, pManager, 0x21, rect, pManager->GetDesktop());
	}

	FrWnd* pWnd = pManager->DoCreate(pForm->m_bgFrame, pManager, this, this);
	m_pBaseFrm = DYNAMIC_CAST(FrFrame, pWnd);
	const char* caption = pForm->m_caption.c_str();
	SetCaption(caption);
	SetDesc(pForm->m_desc.c_str());

	for (std::list<FrGuiItem*>::iterator it = pForm->m_guiList.begin(); it != pForm->m_guiList.end(); ++it) {
		FrWnd* pChild = pManager->DoCreate(**it, pManager, this, this);

		if ((*it)->m_name == "message") {
			SetMessageControlByType((*it)->m_type, pChild);
		}
	}

	return this;
}

// frform.cpp:290
bool FrForm::Open(FRESH_RESULT_CALLBACK pfnResult, unsigned long flag) {
	if (!m_pWndManager->IsValidWindow(this)) {
		return false;
	}

	m_pFnResult = pfnResult;
	m_flag = flag;

	if (ms_bExclusive) {
		SetVisible(false);
	}
	SetViewFocus(!IsFixed() && !ms_bExclusive);
	OnInit();

	if (!(m_flag & 0x100)) {
		g_audio->PlaySfx("ui_dialog_open", 0, 0, 0, 0, 0.5f, 200.0f);
	}
	if (m_flag & 0x200) {
		ms_bExclusive = true;
	}

	if (!(flag & 0x4)) {
		SetFadeout(false);
	}

	return true;
}

// frform.cpp:319
bool FrForm::Open(FRESH_RESULT_CALLBACK pfnResult, const WPoint& pt, unsigned long flag) {
	MoveWindow(pt);
	Open(pfnResult, flag);

	SetFadeout(false);

	return true;
}

// frform.cpp:330
bool FrForm::Close(bool bDestroy) {
	return Close(m_defaultRetCode, bDestroy);
}

// frform.cpp:335
bool FrForm::Close(eFormRet ret, bool bDestroy) {
	m_pWndManager->ResetKey();

	m_retCode = ret;
	if (m_pFnResult) {
		if (!(m_pCmdDest->*m_pFnResult)(ret, this)) {
			return false;
		}
	}

	if (!(m_flag & 0x10)) {
		SetIconRect(m_rect);
	}

	if (!(m_flag & 0x100)) {
		g_audio->PlaySfx("ui_dialog_close", 0, 0, 0, 0, 0.5f, 200.0f);
	}
	if (m_flag & 0x200) {
		ms_bExclusive = false;
	}

	return FrWnd::Close(ms_bHasTail ? bDestroy : false);
}

// frform.cpp:359
void FrForm::SetResultCallback(FRESH_RESULT_CALLBACK pfnResult) {
	m_pFnResult = pfnResult;
}

// frform.cpp:364
void FrForm::SetCaption(const char* caption) {
	if (m_pBaseFrm) {
		m_pBaseFrm->SetCaption(caption);
	}
}

// frform.cpp:370
void FrForm::SetDesc(const char* desc) {
	if (m_pBaseFrm) {
		m_pBaseFrm->SetDesc(desc);
	}
}

// frform.cpp:376
void FrForm::SetMessage(const char* message, bool b) {
	if (m_pMsgStatic) {
		m_pMsgStatic->SetCaption(message);
	}
	if (m_pMsgEdit) {
		m_pMsgEdit->ClearLine();
		m_pMsgEdit->AddText(message, b, true);
	}
}

// frform.cpp:388
void FrForm::EnableDrag(bool bEnable) {
	m_canDrag = bEnable;
}

// frform.cpp:393
bool FrForm::OnInit(void) {
	if (ms_bHasTail && !(m_flag & 0x8)) {
		if (!(m_flag & 0x10)) {
			SetIconRect(m_rect);
		}
		m_nFlags.Enable(0x100);
	}

	if (m_flag & 0x20) {
		m_defaultRetCode = FrNONE;
	} else if (m_flag & 0x40) {
		m_defaultRetCode = FrOK;
	} else if (m_flag & 0x80) {
		m_defaultRetCode = FrCANCEL;
	}

	return true;
}

// frform.cpp:412
void FrForm::SetIconRect(const WRect& rect) {
	m_iconRect.x = rect.x + rect.w * 0.5f;
	m_iconRect.y = rect.y + rect.h * 0.5f;
	m_iconRect.w = rect.w * 0.0f;
	m_iconRect.h = rect.h * 0.0f;
}

// frform.cpp:423
void FrForm::OnOK(void) {
	if (!(m_flag & 0x100)) {
		g_audio->PlaySfx("ui_button_ok_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}

	Close(FrOK, true);
}

// frform.cpp:431
void FrForm::OnCancel(void) {
	if (!(m_flag & 0x100)) {
		g_audio->PlaySfx("ui_button_cancel_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}

	Close(FrCANCEL, true);
}

// frform.cpp:439
void FrForm::OnDraw(void) {
	FrWnd::OnDraw();
}

// frform.cpp:444
void FrForm::OnProc(const float deltaTime) {
	FrWnd::OnProc(deltaTime);

	if (m_timeLimit > 0.0f) {
		m_dt += deltaTime;
		if (m_dt > m_timeLimit) {
			OnCancel();
		}
	}
}

// frform.cpp:457
bool FrForm::IsTitleBarArea(const WPoint& pos) {
	WRect topArea(m_rect.x, m_rect.y, m_rect.w, 20.0f);

	return pos.x >= topArea.x && pos.x <= topArea.x + topArea.w && pos.y >= topArea.y
		&& pos.y <= topArea.y + topArea.h ? true : false;
}

// frform.cpp:465
bool FrForm::IsResizeBtnArea(const WPoint& pt) {
	return false;
}

// frform.cpp:481
void FrForm::OnSetCursor(bool b, const WPoint& pt) {
	if (!IsViewFocused()) {
		return;
	}

	if (IsTitleBarArea(pt) && m_canDrag) {
		SetCursor(1);
	}
}

// frform.cpp:490
void FrForm::OnMouseMove(const WPoint& mousePos) {
	if (GetCapture() == this) {
		WPoint delta = mousePos - ms_oldPos;
		delta.x = (float)(int)delta.x;
		delta.y = (float)(int)delta.y;

		if (m_canDrag && m_bCaptionDown) {
			WRect dr(m_rect.TopLeft() + delta, m_rect.Size());

			m_pWndManager->ConfineRect(dr);

			FrWnd::MoveWindow(WPoint(dr.x, dr.y));
		}

		if (m_canDrag && m_bResizeBtnDown) {
			WSize ds(delta.x, delta.y);
			WRect dr(m_rect.TopLeft(), m_rect.Size() + ds);

			m_pWndManager->ConfineRect(dr);

			SetRect(dr);
		}

		ms_oldPos = mousePos;
	}
}

// frform.cpp:525
bool FrForm::OnLButtonDown(const WPoint& pt) {
	if (IsViewFocused()) {
		if (IsTitleBarArea(pt)) {
			m_bCaptionDown = true;
			ms_oldPos = pt;
			SetCapture();
		}

		if (IsResizeBtnArea(pt)) {
			m_bResizeBtnDown = true;
			ms_oldPos = pt;
			SetCapture();
		}
	}

	FindNextTopFocus(false);

	return false;
}

// frform.cpp:549
bool FrForm::OnLButtonUp(const WPoint& pt) {
	ReleaseCapture();

	m_bCaptionDown = false;
	m_bResizeBtnDown = false;

	return false;
}

// frform.cpp:559
void FrForm::SetRect(const WRect& rect) {
	FrWnd::SetRect(rect);

	if (m_pBaseFrm) {
		m_pBaseFrm->SetRect(rect);
	}
}

// frform.cpp:567
void FrForm::SetClientRect(const WRect& rect) {
	FrWnd::SetClientRect(rect);

	if (m_pBaseFrm) {
		m_pBaseFrm->SetClientRect(rect);
	}
}

// frform.cpp:585
void FrForm::Adjust(WRect& rtRect) {
	float fWidth = g_view->GetWidth();
	float fHeight = g_view->GetHeight();

	if (rtRect.x + rtRect.w > fWidth) {
		rtRect.x = fWidth - rtRect.w;
	}

	// A named bottom edge makes cl load h before adding y, as the original does.
	float fBottom = rtRect.y + rtRect.h;
	if (fBottom > fHeight) {
		rtRect.y = fHeight - rtRect.h;
	}
}

// frform.cpp:611
void FrForm::SetFrameCaptionFocus(bool bFocus) {
	if (m_pBaseFrm) {
		m_pBaseFrm->SetCaptionFocus(bFocus);
	}
}
