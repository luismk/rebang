// Original: d:\build\custom\temp\code\client\fresh\source\frwnd.cpp
#define FRWND_WTYPES_INLINE
#include <string.h>

#include <algorithm>

#include "frwnd.h"
#include "frwndmanager.h"
#include "frscrollbar.h"
#include "frtooltip.h"
#include "frform.h"
#include "frdesktop.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wview.h"
#include "../client/ProjectG/SoundManager.h"

// Wangreal float helper (out of line in ClientSetting); its header is not
// transcribed yet.
int WisZero(const float& f, float eps);

// frwnd.cpp:18 (run-time type and factory registration of FrWnd)
const WRTTI FrWnd::m_RTTI("FrWnd", &IObject::m_RTTI);

IObject* FrWndMakeInstance(void) {
	return new FrWnd;
}

struct __sFrWnd {
	__sFrWnd(void) {
		ObjectFactory().AddObjectFunctor(FrWndMakeInstance, "FrWnd");
	}
};
static __sFrWnd s_registerFrWnd;

FrToolTip* FrWnd::m_pToolTip = 0;

// frwnd.cpp:30
FrWnd::FrWnd(void)
	: m_nFlags(0),
	  m_pParentWnd(0),
	  m_pOwner(0),
	  m_dwStyle(0),
	  m_wndAlpha(1.0f),
	  m_wndAlpha2(1.0f),
	  m_pWndManager(0),
	  m_pScrBar(0),
	  m_fadeTime(0.0f),
	  m_szPushSound(0),
	  m_dblClickTimeout(-100.0f),
	  m_dblClicked(false),
	  m_hoverOn(false),
	  m_hoverTime(0.0f),
	  m_accHoverTime(0.0f),
	  m_pToolTipData(0) {
}

// frwnd.cpp:35
FrWnd::~FrWnd(void) {
	if (FrWndManager* pManager = m_pWndManager) {
		pManager->DeleteTopmostWindow(this);
		pManager->ReleaseCapture(this);
	}

	DestroyChild();
	if (m_pToolTipData) {
		delete m_pToolTipData;
		m_pToolTipData = 0;
	}
}

// frwnd.cpp:47
void FrWnd::CreateToolTip(FrWndManager* pWndManager) {
	if (!m_pToolTip) {
		m_pToolTip = new FrToolTip(pWndManager);
	}
}

// frwnd.cpp:53
FrToolTip* FrWnd::ToolTip(void) {
	return m_pToolTip;
}

// frwnd.cpp:59
void FrWnd::DestroyToolTip(void) {
	if (m_pToolTip) {
		delete m_pToolTip;
	}
	m_pToolTip = 0;
}

// frwnd.cpp:67
void FrWnd::RemoveWindow(FrWnd* pWnd) {
	std::list<FrWnd*>::iterator it = std::find(m_childList.begin(), m_childList.end(), pWnd);
	if (it != m_childList.end()) {
		m_childList.erase(it);
	}
}

// frwnd.cpp:74
bool FrWnd::Close(bool bDestroy) {
	if (!m_nFlags.GetFlag(0x20)) {
		if (bDestroy) {
			m_nFlags.Enable(0x140);
		} else {
			m_nFlags.Disable(0x100);
			m_nFlags.Enable(0x20);
		}

		if (m_pWndManager) {
			m_pWndManager->ResetKeyFocus(this);
		}
	}

	if (m_dwStyle.GetFlag(0x10)) {
		SetTopmost(false);
	}
	return true;
}

// frwnd.cpp:100
void FrWnd::DestroyChild(void) {
	std::list<FrWnd*>::iterator it = m_childList.begin();
	while (it != m_childList.end()) {
		delete *it;
		it = m_childList.erase(it);
	}
}

// frwnd.cpp:114
bool FrWnd::Create(const char* lpszWindowText, const char* lpszWindowName, FrWndManager* pManager, unsigned long dwStyle,
	const WRect& rect, FrWnd* pParentWnd) {
	if (lpszWindowText) {
		m_wndText = lpszWindowText;
	}
	if (lpszWindowName) {
		m_wndName = lpszWindowName;
	}

	PreCreateWindow(pManager, dwStyle, rect, pParentWnd);

	m_dwStyle.Enable(0x800);

	m_nRefID = pManager->RefIndex()++;

	if (m_dwStyle.GetFlag(0x10)) {
		pManager->AddTopmostWindow(this);
	}

	if (pParentWnd) {
		pParentWnd->m_childList.push_back(this);
		m_rect.x += pParentWnd->m_rect.x;
		m_rect.y += pParentWnd->m_rect.y;
	}

	m_dblClickTimeout = -100.0f;
	m_dblClicked = false;

	return true;
}

// frwnd.cpp:152
void FrWnd::PreCreateWindow(FrWndManager* pWndManager, unsigned long style, const WRect& rect, FrWnd* pParent) {
	m_pWndManager = pWndManager; m_dwStyle = style;
	m_rect = rect;
	m_pParentWnd = pParent;
}

// frwnd.cpp:160
bool FrWnd::SendCmdToOwnerTarget(eFrCmd cmd, intptr_t param, sFRESH_HANDLER* pHandler) {
	if (m_nFlags.GetFlag(0x20)) {
		return false;
	}
	return SendCmdToOwnerTarget(m_pOwner, cmd, param, pHandler);
}

// frwnd.cpp:168
bool FrWnd::SendCmdToOwnerTarget(FrCmdTarget* pTarget, eFrCmd cmd, intptr_t param, sFRESH_HANDLER* pHandler) {
	if (pTarget) {
		std::string strName("");
		if (this == m_pWndManager->GetDesktop()) {
			strName = m_pWndManager->GetLayoutID();
		} else if (this == pTarget) {
			strName = ".";
		} else {
			std::string strPath;
			strName = m_wndName;

			for (FrWnd* pParent = m_pParentWnd;; pParent = pParent->m_pParentWnd) {
				if (pParent == NULL || pParent == WndManager()->GetDesktop()) {
					strPath = m_pWndManager->GetLayoutID();
					strPath += ".";
					strPath += strName;
					strName = strPath;
					break;
				}

				if (pParent == pTarget) {
					break;
				}

				strPath = pParent->m_wndName;
				strPath += ".";
				strPath += strName;
				strName = strPath;
			}
		}

		return pTarget->OnFreshMsg(strName.c_str(), cmd, param, pHandler);
	}
	return true;
}

// frwnd.cpp:220
FrGraphicInterface* FrWnd::GDI(void) const {
	return m_pWndManager->GetGDI();
}

// frwnd.cpp:225
FrEmoticon* FrWnd::Emo(void) const {
	return m_pWndManager->GetEmoticon();
}

// frwnd.cpp:230
FrWnd* FrWnd::GetParent(void) const {
	return m_pParentWnd;
}

// frwnd.cpp:235
FrWnd* FrWnd::FindChild(const FrWnd* pWnd) {
	for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
		FrWnd* pChild = *it;
		if (pChild == pWnd) {
			return *it;
		}
		if (pChild) {
			FrWnd* pFound = pChild->FindChild(pWnd);
			if (pFound) {
				return pFound;
			}
		}
	}
	return 0;
}

// frwnd.cpp:255
FrWnd* FrWnd::FindChild(const char* name) {
	for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
		if ((*it)->m_wndText == name) {
			return *it;
		}
	}
	return 0;
}

// frwnd.cpp:268
FrWnd* FrWnd::FindChildByName(const char* name) {
	for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
		if ((*it)->m_wndName == name) {
			return *it;
		}
	}
	return 0;
}

// frwnd.cpp:281
FrWnd* FrWnd::FindChildByStyle(unsigned long style) {
	for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
		if ((*it)->m_dwStyle.GetFlag(style)) {
			return *it;
		}
	}
	return 0;
}

// frwnd.cpp:294
FrWnd* FrWnd::FindChildForm(const char* name) {
	for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
		if ((__rtti_obj = *it) != 0 && ((IObject*)*it)->IsKindOf(&FrForm::m_RTTI) && (*it)->m_wndName == name) {
			return *it;
		}
	}
	return 0;
}

// frwnd.cpp:306
void FrWnd::EnumerateChildWindow(ENUM_CHILD_WINDOW_CALLBACK pfn, void* pParam) {
	for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
		FrWnd* pWnd = *it;
		if (!pfn(pWnd, pParam)) {
			break;
		}
	}
}

// frwnd.cpp:316
void FrWnd::SetOwner(FrCmdTarget* pOwner) {
	m_pOwner = pOwner;
}

// frwnd.cpp:321
bool FrWnd::IsChild(const FrWnd* pWnd) const {
	FrWnd* pParent = m_pParentWnd;
	while (pParent) {
		if (pParent == pWnd) {
			return true;
		}
		pParent = pParent->m_pParentWnd;
	}
	return false;
}

// frwnd.cpp:335
void FrWnd::CloseChild(FrWnd* pChild, bool bDestroy, bool bAll) {
	if (pChild) {
		std::list<FrWnd*>::iterator it = std::find(m_childList.begin(), m_childList.end(), pChild);
		if (it != m_childList.end()) {
			if (bAll || !pChild->IsFixed()) {
				(*it)->Close(bDestroy);
			}
		}
	} else {
		std::list<FrWnd*>::iterator it = m_childList.begin();
		while (it != m_childList.end()) {
			if ((!bAll && (*it)->IsFixed())
				|| (*it)->m_nFlags.GetFlag(0x20)) {
				it++;
				continue;
			}
			(*it)->Close(bDestroy);
			it++;
		}
	}
}

// frwnd.cpp:371
void FrWnd::CloseChildForm(bool bDestroy, bool bAll) {
	for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
		if ((!bAll && (*it)->IsFixed())
			|| (*it)->m_nFlags.GetFlag(0x20)) {
			continue;
		}
		if ((__rtti_obj = *it) != 0 && ((IObject*)*it)->IsKindOf(&FrForm::m_RTTI)) {
			(*it)->Close(bDestroy);
		}
	}
}

// frwnd.cpp:394
void FrWnd::GetClientRect(WRect& rect) const {
	rect = m_rect;
	ScreenToClient(rect);
}

// frwnd.cpp:400
void FrWnd::SetClientRect(const WRect& rect) {
	WRect rc = rect;
	ClientToScreen(rc);
	SetRect(rc);
}

// frwnd.cpp:407
void FrWnd::ClientToScreen(WRect& rect) const {
	for (FrWnd* pParent = m_pParentWnd; pParent; pParent = pParent->m_pParentWnd) {
		rect.x += pParent->m_rect.x;
		rect.y += pParent->m_rect.y;
	}
}

// frwnd.cpp:417
void FrWnd::ClientToScreen(WPoint& pt) const {
	for (FrWnd* pParent = m_pParentWnd; pParent; pParent = pParent->m_pParentWnd) {
		pt.x += pParent->m_rect.x;
		pt.y += pParent->m_rect.y;
	}
}

// frwnd.cpp:427
void FrWnd::ScreenToClient(WRect& rect) const {
	for (FrWnd* pParent = m_pParentWnd; pParent; pParent = pParent->m_pParentWnd) {
		rect.x -= pParent->m_rect.x;
		rect.y -= pParent->m_rect.y;
	}
}

// frwnd.cpp:437
void FrWnd::ScreenToClient(WPoint& pt) const {
	for (FrWnd* pParent = m_pParentWnd; pParent; pParent = pParent->m_pParentWnd) {
		pt.x -= pParent->m_rect.x;
		pt.y -= pParent->m_rect.y;
	}
}

// frwnd.cpp:447
void FrWnd::SetWindowTextA(const char* text) {
	if (text) {
		m_wndText = text;
	}
}

// frwnd.cpp:453
void FrWnd::GetWindowTextA(char* text, unsigned int size) const {
	strcpy(text, m_wndText.c_str());
}

// frwnd.cpp:459
void FrWnd::GetWindowTextA(std::string& text) const {
	text = m_wndText;
}

// frwnd.cpp:464
unsigned int FrWnd::GetWindowTextLengthA(void) const {
	return m_wndText.length();
}

// frwnd.cpp:469
void FrWnd::SetWindowName(const char* name) {
	if (name) {
		m_wndName = name;
	}
}

// frwnd.cpp:475
void FrWnd::GetWindowName(char* name, unsigned int size) const {
	strcpy(name, m_wndName.c_str());
}

// frwnd.cpp:481
void FrWnd::GetWindowName(std::string& name) const {
	name = m_wndName;
}

// frwnd.cpp:486
unsigned int FrWnd::GetWindowNameLength(void) const {
	return m_wndName.length();
}

// frwnd.cpp:491
void FrWnd::SetToolTipText(const std::string& text) {
	m_wndToolTip = text;
	EnableToolTip(true);

	if (!m_pToolTipData) {
		m_pToolTipData = new sToolTipData;
		m_pToolTipData->style = 0;
		m_pToolTipData->bFixWnd = 0;
		m_pToolTipData->posFixWnd = WPoint(0.0f, 0.0f);
	}
}

// frwnd.cpp:505
void FrWnd::SetFixTooltipWnd(const WPoint& pt) {
	if (m_pToolTipData) {
		m_pToolTipData->bFixWnd = 1;
		m_pToolTipData->posFixWnd = pt;
	}
}

// frwnd.cpp:514
void FrWnd::SetUnFixToolTipWnd(void) {
	if (m_pToolTipData) {
		m_pToolTipData->bFixWnd = 0;
	}
}

// frwnd.cpp:520
int FrWnd::IsFixedToolTip(void) const {
	if (m_pToolTipData) {
		return m_pToolTipData->bFixWnd;
	}
	return 0;
}

// frwnd.cpp:528
const WPoint& FrWnd::GetToolTipWndPos(void) const {
	if (m_pToolTipData && m_pToolTipData->bFixWnd) {
		return m_pToolTipData->posFixWnd;
	}
	return m_pWndManager->GetMousePos();
}

// frwnd.cpp:536
void FrWnd::SetToolTipFrameStyle(unsigned long style) {
	if (m_pToolTipData) {
		m_pToolTipData->style = style;
	}
}

// frwnd.cpp:542
unsigned long FrWnd::GetToolTipFrameStyle(void) const {
	if (m_pToolTipData) {
		return m_pToolTipData->style;
	}
	return 0;
}

// frwnd.cpp:550
void FrWnd::OnDisplay(bool bSkipTopmost, bool bChildren, bool bDraw) {
	if (bSkipTopmost && m_dwStyle.GetFlag(0x10)) {
		return;
	}
	if (!m_nFlags.GetFlag(0x80)) {
		return;
	}
	if (!IsVisible() && !m_nFlags.GetFlag(0x400)) {
		return;
	}
	if (m_nFlags.GetFlag(0x20)) {
		return;
	}
	if (m_nFlags.GetFlag(0x100)) {
		DoFadeDisplay();
		return;
	}

	if (bDraw) {
		OnDraw();
	}
	if (bChildren) {
		for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
			FrWnd* pChild = *it;
			if (m_nFlags.GetFlag(0x100)) {
				pChild->m_wndAlpha = m_wndAlpha;
			}
			pChild->OnDisplay(true, true, true);
		}
	}
}

// frwnd.cpp:592
void FrWnd::OnDraw(void) {
}

// frwnd.cpp:615
void FrWnd::DoFadeProcess(const float dt, bool bElement) {
	if (bElement) {
		if (m_nFlags.GetFlag(0x800)) {
			m_fadeTime -= dt * 5.0f;

			if (m_fadeTime < 0.0f) {
				m_fadeTime = 0.0f;

				m_nFlags.Disable(0x400);
				if (m_nFlags.GetFlag(0x2000)) {
					m_nFlags.Disable(0x2000);
					m_dwStyle.Disable(0x1);
				} else {
					Close(false);
				}
			}
		} else {
			if (m_nFlags.GetFlag(0x2000)) {
				m_dwStyle.Enable(0x1);
			}

			m_fadeTime += dt * 5.0f;

			if (m_fadeTime > 0.9f) {
				m_fadeTime = 1.0f;

				m_nFlags.Disable(0x400);
				if (m_nFlags.GetFlag(0x2000)) {
					m_nFlags.Disable(0x2000);
				}
			}
		}
		SetAlpha2ToChild(m_fadeTime);
	} else {
		if (m_nFlags.GetFlag(0x40)) {
			m_fadeTime -= dt * 5.0f;
			if (m_fadeTime < 0.0f) {
				m_fadeTime = 0.0f;
				if (m_nFlags.GetFlag(0x2000)) {
					m_nFlags.Disable(0x2000);
					m_dwStyle.Disable(0x1);
				} else {
					m_nFlags.Enable(0x20);
				}
			}
		} else {
			if (m_nFlags.GetFlag(0x2000)) {
				m_dwStyle.Enable(0x1);
			}

			m_fadeTime += dt * 5.0f;
			if (m_fadeTime > 0.9f) {
				m_fadeTime = 1.0f;
				m_nFlags.Disable(0x100);
				if (m_nFlags.GetFlag(0x2000)) {
					m_nFlags.Disable(0x2000);
				}
			}
		}
	}
}

// frwnd.cpp:709
void FrWnd::DoFadeDisplay(void) {
	float inv = 1.0f - m_fadeTime;

	WOverlay::DrawLineBox(g_view, WRect(inv * m_iconRect.x + m_rect.x * m_fadeTime, inv * m_iconRect.y + m_rect.y * m_fadeTime,
		inv * m_iconRect.w + m_rect.w * m_fadeTime, inv * m_iconRect.h + m_rect.h * m_fadeTime), 0, 0xffffffff);
}

// frwnd.cpp:720
void FrWnd::CheckHover(float dt, bool bIn, bool& bHover) {
	if (bIn) {
		if (!bHover) {
			bHover = true;

			if (!m_hoverOn) {
				m_accHoverTime += dt;
				if (m_accHoverTime > m_hoverTime) {
					m_accHoverTime = 0.0f; m_hoverOn = true; SendCmdToOwnerTarget(FRCMD_HOVERON, (intptr_t)this, 0);
				}
			}
		}
	} else {
		m_accHoverTime = 0.0f;
		if (m_hoverOn) {
			m_hoverOn = false;
			SendCmdToOwnerTarget(FRCMD_HOVEROFF, (intptr_t)this, 0);
		}
	}
}

// frwnd.cpp:765
void FrWnd::OnProcess(const float deltaTime, FrInputState& istate, bool checkTopmost, bool bUnableMode) {
	if (checkTopmost && m_dwStyle.GetFlag(0x10)) {
		return;
	}

	if (!m_nFlags.GetFlag(0x80) && !m_nFlags.GetFlag(0x20)) {
		m_nFlags.Enable(0x80);
		return;
	}

	if (!IsVisible() && !m_nFlags.GetFlag(0x400)) {
		return;
	}

	if (m_nFlags.GetFlag(0x100)) {
		DoFadeProcess(deltaTime, false);
		return;
	}

	std::list<FrWnd*>::iterator it = m_childList.end();
	for (--it; it != m_childList.end(); --it) {
		FrWnd* pChild = *it;

		if (pChild->m_nFlags.GetFlag(0x20)) {
			if (pChild->m_dwStyle.GetFlag(0x10)) {
				m_pWndManager->DeleteTopmostWindow(pChild);
			}
			pChild->m_pWndManager->ResetKeyFocus(pChild);
			bool viewFocused = pChild->m_nFlags.GetFlag(0x8);

			delete pChild;
			it = m_childList.erase(it);

			if (viewFocused) {
				m_pWndManager->HandOverViewFocus();
			}
		} else {
			pChild->OnProcess(deltaTime, istate, true, bUnableMode);
		}
	}

	if (m_nFlags.GetFlag(0x400)) {
		DoFadeProcess(deltaTime, true);
		return;
	}

	if (!m_nFlags.GetFlag(0x80)) {
		return;
	}

	OnProc(deltaTime);

	if (m_pWndManager->GetWheelFocus() == 0) {
		FrScrollBar* pScrBar = DYNAMIC_CAST(FrScrollBar, this);
		if (!pScrBar) {
			pScrBar = m_pScrBar;
		}
		if (pScrBar && !pScrBar->m_dwStyle.GetFlag(0x800) && pScrBar->IsBarVisible()) {
			bool bIn;
			if (pScrBar->m_pParentWnd) {
				bIn = pScrBar->m_pParentWnd->m_rect.IsInRect(istate.mousePos);
			} else {
				bIn = m_rect.IsInRect(istate.mousePos);
			}

			if (bIn) {
				m_pWndManager->SetWheelFocus(pScrBar);
				if (!WisZero(istate.wheelDelta, 1e-5f)) {
					OnWheel(istate);
				}
			}
		} else if (IsVisible()) {
			if ((__rtti_obj = this) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&FrForm::m_RTTI)) {
				if (m_rect.IsInRect(istate.mousePos)) {
					m_pWndManager->SetWheelFocus(m_pWndManager->GetDesktop()->GetScrollBar());
				}
			}
		}
	}

	if (!m_dwStyle.GetFlag(0x10) && bUnableMode == true && m_pWndManager && m_pWndManager->GetDesktop()) {
		return;
	}

	if (IsEnableToolTip() && m_rect.IsInRect(istate.mousePos)) {
		m_pToolTip->SetToolTipText(m_wndToolTip);
		m_pToolTip->SetFrameStyle(m_pToolTipData ? m_pToolTipData->style : 0);
		if (m_pToolTipData && m_pToolTipData->bFixWnd) {
			m_pToolTip->Move(m_pToolTipData->posFixWnd);
		}
	}

	if (m_dwStyle.GetFlag(0x2)) {
		return;
	}
	if (m_pWndManager && m_pWndManager->GetDesktop() && m_pWndManager->GetDesktop()->GetStyle().GetFlag(0x2)) {
		return;
	}

	if (m_dblClickTimeout > -0.1f) {
		m_dblClickTimeout -= deltaTime;
		if (m_dblClickTimeout < 0.0f) {
			m_dblClickTimeout = -100.0f;
		}
	}

	if (istate.keyFocused == this && istate.im) {
		OnKeyFocus(istate.im);
	}

	if (!IsViewFocused()) {
		return;
	}

	if (m_dwStyle.GetFlag(0x40)) {
		return;
	}

	FrWnd* pCaptured = m_pWndManager->GetCapture();
	if (pCaptured && pCaptured != this) {
		if (!m_pWndManager->IsValidWindow(pCaptured)) {
			m_pWndManager->ReleaseCapture(pCaptured);
		}
		return;
	}

	bool bInClient = m_rect.IsInRect(istate.mousePos);
	OnSetCursor(bInClient, istate.mousePos);

	if (m_dwStyle.GetFlag(0x400)) {
		CheckHover(deltaTime, bInClient, istate.hoverChecked);
	}

	if (!pCaptured && !bInClient) {
		return;
	}

	if (istate.mouse & 0x2) {
		OnMouseMove(istate.mousePos);
	}

	if (istate.mouse & 0x4) {
		if (m_dblClickTimeout > -0.1f && m_dblClickPos == istate.mousePos) {
			m_dblClicked = true;
			istate.mouse &= ~0x4;
		} else if (!OnLButtonDown(istate.mousePos)) {
			istate.mouse &= ~0x4;

			if (!m_dwStyle.GetFlag(0x200)) {
				m_dblClickTimeout = 0.5f;
				m_dblClickPos = istate.mousePos;
			}

			if (istate.keyFocused != this) {
				FrWnd* pDesktop = (FrWnd*)m_pWndManager->GetDesktop();
				pDesktop->m_pWndManager->ResetKeyFocus(pDesktop);
				SetKeyFocus(true);
			} else if (m_dwStyle.GetFlag(0x80)) {
				OnSelectText(&istate);
			}
		}
	}

	if (istate.mouse & 0x8) {
		if (m_dblClicked) {
			OnDblClick(istate.mousePos);
			istate.mouse &= ~0x8;
			m_dblClickTimeout = -100.0f;
			m_dblClicked = false;
		} else if (!OnLButtonUp(istate.mousePos)) {
			istate.mouse &= ~0x8;
		}
	}

	if (istate.mouse & 0x10) {
		if (!OnRButtonDown(istate.mousePos)) {
			istate.mouse &= ~0x10;
		}
	}

	if (istate.mouse & 0x20) {
		if (!OnRButtonUp(istate.mousePos)) {
			istate.mouse &= ~0x20;
		}
	}
}

// frwnd.cpp:1030
void FrWnd::SetVisible(bool bVisible) {
	if (!bVisible && m_pWndManager) {
		m_pWndManager->ReleaseCapture(this);
	}
	m_dwStyle.Turn(0x1, bVisible);
}

// frwnd.cpp:1038
void FrWnd::SetTopmost(bool bTopmost) {
	if (bTopmost) {
		m_dwStyle.Enable(0x10);
		m_pWndManager->AddTopmostWindow(this);
	} else {
		m_dwStyle.Disable(0x10);
		m_pWndManager->DeleteTopmostWindow(this);
		for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
			(*it)->SetTopmost(false);
		}
	}
}

// frwnd.cpp:1055
void FrWnd::EnableHover(bool bEnable, float time) {
	m_dwStyle.Turn(0x400, bEnable);
	m_hoverTime = time;
	m_accHoverTime = 0.0f;
}

// frwnd.cpp:1062
void FrWnd::SetWheelEvent(bool bEnable) {
	if (!bEnable) {
		m_dwStyle.Enable(0x800);
	} else {
		m_dwStyle.Disable(0x800);
	}
	if (m_pScrBar) {
		m_pScrBar->SetWheelEvent(bEnable);
	}
}

// frwnd.cpp:1070
void FrWnd::SetViewFocus(bool bReset) {
	FrWnd* pModal = m_pWndManager->GetModalForm();
	if (pModal && pModal != this) {
		return;
	}

	if (bReset) {
		m_pWndManager->GetDesktop()->ResetViewFocus(0);
	}

	for (FrWnd* pWnd = this; pWnd; pWnd = pWnd->m_pParentWnd) {
		if (!pWnd->m_pParentWnd || ((__rtti_obj = pWnd) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&FrForm::m_RTTI))) {
			pWnd->m_nFlags.Enable(0x8);
			return;
		}
	}
}

// frwnd.cpp:1094
void FrWnd::ResetViewFocus(FrWnd* pWnd) {
	if (pWnd == this) {
		return;
	}
	for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
		(*it)->ResetViewFocus(pWnd);
		(*it)->m_pWndManager->ReleaseCapture(*it);
	}
	m_nFlags.Disable(0x8);
}

// frwnd.cpp:1108
FrWnd* FrWnd::FindViewFocused(bool bEnabledOnly) {
	for (std::list<FrWnd*>::reverse_iterator it = m_childList.rbegin(); it != m_childList.rend(); ++it) {
		FrWnd* pWnd = *it;

		if (pWnd->m_dwStyle.GetFlag(0x10)) {
			if (pWnd->m_nFlags.GetFlag(0x200)) {
				return *it;
			}
		} else if (pWnd->m_nFlags.GetFlag(0x8)) {
			if (!bEnabledOnly || (pWnd->IsEnabled() && pWnd->IsVisible())) {
				return *it;
			}
		}
	}
	return 0;
}

// frwnd.cpp:1136
bool FrWnd::IsViewFocused(void) const {
	const FrWnd* pWnd = this;
	while (pWnd) {
		if (pWnd->m_nFlags.GetFlag(0x8) || pWnd->m_dwStyle.GetFlag(0x100)) {
			return true;
		}
		if ((__rtti_obj = (void*)pWnd) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&FrForm::m_RTTI)) {
			break;
		}
		pWnd = pWnd->m_pParentWnd;
	}
	return false;
}

// frwnd.cpp:1151
bool FrWnd::IsTopFocus(void) const {
	if (m_nFlags.GetFlag(0x200) && m_dwStyle.GetFlag(0x10)) {
		return true;
	}
	return false;
}

// frwnd.cpp:1162
bool FrWnd::SetKeyFocus(bool bFocus) {
	return WndManager()->SetKeyFocus(this, bFocus);
}

// frwnd.cpp:1167
bool FrWnd::ResetKeyFocus(void) {
	return WndManager()->ResetKeyFocus(this);
}

// frwnd.cpp:1172
void FrWnd::MoveWindow(const WPoint& pt) {
	WPoint offset = pt - m_rect.TopLeft();
	m_rect.x = pt.x;
	m_rect.y = pt.y;

	for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
		if ((__rtti_obj = *it) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&FrForm::m_RTTI)) {
			continue;
		}
		WPoint childPt = (*it)->m_rect.TopLeft();
		(*it)->MoveWindow(childPt + offset);
	}
}

// frwnd.cpp:1192
void FrWnd::SetRect(const WRect& rect) {
	MoveWindow(WPoint(rect.x, rect.y));
	m_rect.w = rect.w;
	m_rect.h = rect.h;
	OnResize();
	for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
		(*it)->OnResize();
	}
}

// frwnd.cpp:1208
bool FrWnd::SetCapture(void) {
	return WndManager()->SetCapture(this);
}

// frwnd.cpp:1213
FrWnd* FrWnd::GetCapture(void) {
	return m_pWndManager->GetCapture();
}

// frwnd.cpp:1218
bool FrWnd::ReleaseCapture(void) {
	return WndManager()->ReleaseCapture(this);
}

// frwnd.cpp:1223
void FrWnd::SetCursor(int cursor) {
	m_pWndManager->SetCursor(cursor);
}

// frwnd.cpp:1228
int FrWnd::GetCursor(void) const {
	return m_pWndManager->GetCursor();
}

// frwnd.cpp:1233
void FrWnd::MoveCursor(const char* name) {
	FrWnd* pModal = m_pWndManager->GetModalForm();

	if (!pModal || this == pModal) {
		m_pWndManager->MoveCursor(this, name);
	}
}

// frwnd.cpp:1243
void FrWnd::SetPushSound(const char* sound) {
	m_szPushSound = sound;
}

// frwnd.cpp:1248
bool FrWnd::PlayPushSound(void) {
	if (m_szPushSound) {
		g_audio->PlaySfx(m_szPushSound, 0, 0, 0, 0, 0.5f, 200.0f);
		return true;
	}
	return false;
}

// frwnd.cpp:1259
void FrWnd::SetWheelFocus(void) {
	FrScrollBar* pScrBar = DYNAMIC_CAST(FrScrollBar, this);
	if (pScrBar) {
		m_pWndManager->SetWheelFocus(pScrBar);
	} else {
		m_pWndManager->SetWheelFocus(m_pScrBar);
	}
}

// frwnd.cpp:1268
void FrWnd::AddChild(FrWnd* pChild) {
	m_childList.push_back(pChild);
}

// frwnd.cpp:1286
void FrWnd::SetElementFadeOut(bool bFadeout) {
	if (!m_nFlags.GetFlag(0x400)) {
		m_nFlags.Enable(0x2400);

		if (bFadeout) {
			m_nFlags.Enable(0x800);
			m_fadeTime = 1.0f;
		} else {
			m_nFlags.Disable(0x800);
			m_fadeTime = 0.0f;
		}
	}
}

// frwnd.cpp:1318
void FrWnd::SetFadeout(bool bFadeout) {
	if (!m_nFlags.GetFlag(0x400)) {
		m_nFlags.Enable(0x400);

		if (bFadeout) {
			m_nFlags.Enable(0x800);
			m_fadeTime = 1.0f;
		} else {
			m_nFlags.Disable(0x800);
			m_fadeTime = 0.0f;
		}
	}
}

// frwnd.cpp:1347
void FrWnd::SetAlpha2ToChild(float alpha) {
	m_wndAlpha2 = alpha;

	for (std::list<FrWnd*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it) {
		(*it)->SetAlpha2ToChild(m_wndAlpha2);
	}
}

// frwnd.cpp:1374
void FrWnd::FindNextTopFocus(bool bForce) {
	if (m_dwStyle.GetFlag(0x10)) {
		m_pWndManager->ResetTopFocus();

		m_nFlags.Enable(0x200);
		FrForm* pForm = DYNAMIC_CAST(FrForm, this);

		if (!m_nFlags.GetFlag(0x400) || bForce) {
			if (pForm) {
				pForm->SetFrameCaptionFocus(true);
			}
			SetAlpha2ToChild(1.0f);
		}
	}
}
