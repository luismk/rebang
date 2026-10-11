// Original: d:\build\custom\temp\code\client\fresh\source\frwndmanager.cpp
#include <string.h>

#include "FrCursor.h"
#include "frdesktop.h"
#include "frarea.h"
#include "frbutton.h"
#include "frcombobox.h"
#include "frcontextmenuctrl.h"
#include "fredit.h"
#include "frelement.h"
#include "fremoticon.h"
#include "frform.h"
#include "frframe.h"
#include "frgaugebar.h"
#include "frgraphicinterface.h"
#include "frgroupbox.h"
#include "frlistbox.h"
#include "frmacroitem.h"
#include "frstatic.h"
#include "frtabbutton.h"
#include "frtextbutton.h"
#include "frtooltip.h"
#include "frviewer.h"
#include "frwnd.h"
#include "frwndmanager.h"
#include "../client/ProjectG/ChatMsg.h"
#include "../client/ProjectG/InputManager.h"
#include "../client/Wangreal/include/wview.h"

// The arguments DoCreate hands to the per-type creators (PDB, size 0x10).
struct sFrCreateSub {
	FrGuiItem* pItem;         // 0x0
	FrWndManager* pManager;   // 0x4
	FrWnd* pParent;           // 0x8
	FrCmdTarget* pOwner;      // 0xc
};

CHECK_SIZE(sFrCreateSub, 0x10);
CHECK_FIELD(sFrCreateSub, pItem, 0x0);
CHECK_FIELD(sFrCreateSub, pManager, 0x4);
CHECK_FIELD(sFrCreateSub, pParent, 0x8);
CHECK_FIELD(sFrCreateSub, pOwner, 0xc);

// frwndmanager.cpp:36
bool FocusWndCompare(const void* d1, const void* d2) {
	const FrWnd* pWnd1 = (const FrWnd*)d1;
	const FrWnd* pWnd2 = (const FrWnd*)d2;
	if (pWnd1 == 0) {
		return false;
	}
	if (pWnd2 == 0) {
		return true;
	}

	if (pWnd1->GetParent() == pWnd2) {
		return false;
	}
	if (pWnd2->GetParent() == pWnd1) {
		return true;
	}

	return pWnd1->IsTopFocus() < pWnd2->IsTopFocus();
}

// frwndmanager.cpp:69
FrWndManager::FrWndManager(FrElementDoc* pDoc)
	: m_pDoc(pDoc), m_pDesktop(0), m_pCursor(0) {
	m_pCaptured = 0;
	m_pDevice = 0;
	m_pEmoticon = 0;
	m_istate.im = 0;
	m_istate.keyFocused = 0;
	m_istate.wheelFocused = 0;
	m_pFocusedEdit = 0;
	m_caretPos = WPoint(0.0f, 0.0f);
	m_hidePrivacy = false;
}

// frwndmanager.cpp:81
FrWndManager::~FrWndManager(void) {
	if (m_pDesktop) { delete m_pDesktop; m_pDesktop = 0; }
	if (m_pDevice) { delete m_pDevice; m_pDevice = 0; }
	if (m_pCursor) { delete m_pCursor; m_pCursor = 0; }
	if (m_pEmoticon) { delete m_pEmoticon; m_pEmoticon = 0; }

	if (m_exclusiveKey) {
		if (WSingleton<CChatMsg>::Instance()) {
			WSingleton<CChatMsg>::Instance()->SetActive(false, true, true);
		}
	}
}

// frwndmanager.cpp:95
void FrWndManager::CreateToolTip(void) {
	FrWnd::CreateToolTip(this);
}

// frwndmanager.cpp:100
void FrWndManager::DestroyToolTip(void) {
	FrWnd::DestroyToolTip();
}

// frwndmanager.cpp:105
bool FrWndManager::Init(const char* wallPaper, bool exclusiveKey) {
	m_exclusiveKey = exclusiveKey;
	m_cursorIndex = 0;

	memset(&m_istate, 0, sizeof(m_istate));
	CheckSystemStatus();

	if (m_pDevice == 0) {
		m_pDevice = new FrGraphicInterface(g_view);
	}
	m_pDevice->Reset();

	if (m_pDesktop) { delete m_pDesktop; m_pDesktop = 0; }
	m_pDesktop = new FrDesktop;
	m_pDesktop->m_pWndManager = this;
	m_pDesktop->Init(g_view->GetWidth(), g_view->GetHeight());
	m_pDesktop->SetWallPaper(wallPaper, true);

	if (m_pCursor == 0) {
		m_pCursor = new FrCursor;
	}
	m_pCursor->Init();

	if (m_pEmoticon == 0) {
		m_pEmoticon = new FrEmoticon(this);
	}
	m_pEmoticon->Init();

	if (m_exclusiveKey) {
		WSingleton<CChatMsg>::Instance()->SetActive(true, true, true);
	}

	return true;
}

// frwndmanager.cpp:146
void FrWndManager::RemoveFade(void) {
	std::list<FrWnd*>::reverse_iterator it = m_pDesktop->m_childList.rbegin();
	for (; it != m_pDesktop->m_childList.rend(); ++it) {
		(*it)->m_fadeTime = -1.0f;
	}
}

// frwndmanager.cpp:155
void FrWndManager::Display(bool drawDesktop) {
	m_pFocusedEdit = 0;

	m_pDesktop->OnDisplay(true, true, drawDesktop);

	for (std::list<FrWnd*>::iterator it = m_topmostList.begin(); it != m_topmostList.end(); ++it) {
		FrWnd* pWnd = *it;

		if (pWnd->m_pParentWnd) {
			if (!pWnd->m_pParentWnd->IsVisible()) {
				continue;
			}
		}

		pWnd->OnDisplay(false, true, true);
	}

	if (FrWnd::IsToolTipInstantiated()) {
		FrWnd::ToolTip()->OnDisplay();
	}
}

// frwndmanager.cpp:195
void FrWndManager::ResetTopFocus(void) {
	if (m_topmostList.empty()) {
		return;
	}

	for (std::list<FrWnd*>::iterator it = m_topmostList.begin(); it != m_topmostList.end(); ++it) {
		FrWnd* pWnd = *it;

		if (pWnd) {
			pWnd->m_nFlags.Disable(0x200);

			if (strcmp(pWnd->m_wndName.c_str(), "messenger_chat") == 0) {
				if (!pWnd->m_nFlags.GetFlag(0x20)) {
					FrForm* pForm = DYNAMIC_CAST(FrForm, pWnd);
					if (pForm) {
						pForm->SetFrameCaptionFocus(false);
					}

					if (!pWnd->m_nFlags.GetFlag(0x400)) {
						pWnd->SetAlpha2ToChild(0.7f);
					}
				}
			}
		}
	}
}

// frwndmanager.cpp:232
void FrWndManager::CheckSystemStatus(void) {
	m_istate.wheelDelta = g_input->GetMouseDelta().z;
	m_istate.mouse = 0;
	m_istate.mousePos = WPoint(g_input->GetMousePoint().x, g_input->GetMousePoint().y);
	if (m_istate.mousePos != m_istate.oldMousePos) {
		m_istate.oldMousePos = m_istate.mousePos;
		m_istate.mouse = 2;
	}

	switch (g_input->GetButton(LEFT_BUTTON)) {
	case 1: m_istate.mouse |= 4; break;
	case 2: m_istate.mouse |= 8; break;
	}

	switch (g_input->GetButton(RIGHT_BUTTON)) {
	case 1: m_istate.mouse |= 0x10; break;
	case 2: m_istate.mouse |= 0x20; break;
	}

	m_istate.hoverChecked = false;
	m_istate.im = WSingleton<CChatMsg>::Instance();
	if (m_exclusiveKey && !m_istate.im->IsActive()) {
		m_istate.im->SetActive(true, false, false);
	}
}

// frwndmanager.cpp:273
void FrWndManager::Process(const float deltaTime) {
	if (m_state == OPENING && m_pDevice) {
		m_fadeoutAlpha += 0.02f;
		if (m_fadeoutAlpha > 1.0f) {
			m_fadeoutAlpha = 1.0f;
			m_state = OPENED;
		}
		m_pDevice->SetAlpha(m_fadeoutAlpha);
	}

	int cursor = m_pCursor->GetCursor();

	if (m_exclusiveKey) {
		m_cursorIndex = 0;
	} else {
		m_cursorIndex = cursor;
	}

	CheckSystemStatus();
	FrWnd* pOldKeyFocused = m_istate.keyFocused;
	ProcessHotKey();

	m_istate.wheelFocused = 0;

	bool bFocus = false;
	if (FrWnd::IsToolTipInstantiated()) {
		FrWnd::ToolTip()->OnProcess(deltaTime);
	}

	FrWnd* pTopWnd = 0;
	for (std::list<FrWnd*>::reverse_iterator it = m_topmostList.rbegin(); it != m_topmostList.rend(); ++it) {
		FrWnd* pWnd = *it;
		pWnd->OnProcess(deltaTime, m_istate, false, false);

		if (pWnd->IsVisible()) {
			WRect rc;
			pWnd->GetClientRect(rc);
			if (m_istate.mousePos.x >= rc.x && rc.x + rc.w >= m_istate.mousePos.x
				&& m_istate.mousePos.y >= rc.y && rc.y + rc.h >= m_istate.mousePos.y) {
				bFocus = true;
				pTopWnd = pWnd;
			}
		}
	}

	if (pTopWnd && !pTopWnd->m_nFlags.GetFlag(0x20) && pTopWnd->m_nFlags.GetFlag(0x1)) {
		if (FrWnd::IsToolTipInstantiated()) {
			FrWnd::ToolTip()->SetToolTipText(pTopWnd->GetToolTipText());
			FrWnd::ToolTip()->SetFrameStyle(pTopWnd->GetToolTipFrameStyle());
			if (pTopWnd->IsFixedToolTip()) {
				FrWnd::ToolTip()->Move(pTopWnd->GetToolTipWndPos());
			}
		}
	}

	ProcessKey(pOldKeyFocused);
	m_pDesktop->OnProcess(deltaTime, m_istate, true, bFocus);

	if (m_cursorIndex != cursor) {
		m_pCursor->SetCursor((FrCursor::eCursor)m_cursorIndex);
	}
}

// frwndmanager.cpp:345
void FrWndManager::CloseWindow(FrWnd* pWnd, bool bFade) {
	m_pDesktop->CloseChild(pWnd, bFade, false);
}

// frwndmanager.cpp:350
void FrWndManager::CloseForm(bool bFade) {
	m_pDesktop->CloseChildForm(bFade, false);
}

// frwndmanager.cpp:355
void FrWndManager::ProcessHotKey(void) {
	if (g_input->GetDown("TAB", true)
		&& !g_input->Get("LALT", true)
		&& !g_input->Get("RALT", true)) {
		MoveKeyFocusToNext(true);
	}

	if (g_input->GetDown("ESCAPE", true) == 1) {
		FrForm* pForm = DYNAMIC_CAST(FrForm, m_pDesktop->FindViewFocused(true));

		if (pForm && pForm->GetFlag(0x1) && pForm->m_nFlags.GetFlag(0x80) && !pForm->m_nFlags.GetFlag(0x40)) {
			pForm->OnCancel();
			g_input->ExclusiveGetDownUseDone("ESCAPE");
		}
	}

	if (g_input->GetDown("ENTER", true) == 1 || g_input->GetDown("PADENTER", true) == 1) {
		FrForm* pForm = DYNAMIC_CAST(FrForm, m_pDesktop->FindViewFocused(true));

		if (pForm && pForm->GetFlag(0x2) && pForm->m_nFlags.GetFlag(0x80)) {
			pForm->OnOK();
			g_input->ExclusiveGetDownUseDone("ENTER");
			g_input->ExclusiveGetDownUseDone("PADENTER");
		}
	}
}

// frwndmanager.cpp:411
void FrWndManager::ProcessKey(FrWnd* pOldKeyFocused) {
	m_istate.im->ProcessMacro(0.01f);

	if (g_input->GetButton(LEFT_BUTTON) == 1)
		m_topmostList.sort(FocusWndCompare);

	if ((m_istate.keyFocused && (!pOldKeyFocused || pOldKeyFocused != m_istate.keyFocused))
		|| (!m_istate.keyFocused && MoveKeyFocusToNext(false))) {
		const char* text = m_istate.keyFocused->OnSelectText(0);

		if (text) {
			m_istate.keyFocused->SetWindowTextA(text);
			m_istate.im->Reset();
			m_istate.im->SetChatText(text, false);
		} else {
			std::string str;
			m_istate.keyFocused->GetWindowTextA(str);
			m_istate.im->Reset();
			m_istate.im->SetChatText(str.c_str(), false);
		}
	}
}

// frwndmanager.cpp:442
void FrWndManager::CloseLayout(void) {
	m_pDesktop->SendCmdToOwnerTarget(FRCMD_DESTROY, 0, 0);

	m_pDesktop->CloseChild(0, false, true);
	m_pDesktop->SetOwner(0);
	m_layoutID = "";
}

// frwndmanager.cpp:452
bool FrWndManager::CreateLayout(const char* layout, FrCmdTarget* owner, bool firstTime) {
	FrElementLayout* pElement = m_pDoc->GetLayout(layout);
	if (pElement == 0) {
		return false;
	}

	m_layoutID = layout;
	m_pDesktop->m_pOwner = owner;
	m_pDesktop->ReleaseCapture();
	m_pDesktop->SetViewFocus(true);

	m_istate.keyFocused = 0;
	m_pDesktop->SendCmdToOwnerTarget(FRCMD_INIT, 0, 0);

	std::list<FrGuiItem*>::iterator end = pElement->m_guiList.end();
	std::list<FrGuiItem*>::iterator it = pElement->m_guiList.begin();

	for (; it != end; ++it) {
		DoCreate(**it, this, m_pDesktop, owner);
	}

	m_pDesktop->SetAniBg(pElement->m_aniBg);

	m_pDesktop->SendCmdToOwnerTarget(FRCMD_FINISH, 0, 0);

	if (!firstTime) {
		m_state = OPENING;
		m_fadeoutAlpha = 0.0f;
	}

	return true;
}

// frwndmanager.cpp:668
template <class T>
T* DoCreateItem(const sFrCreateSub& m) {
	T* pItem = new T;
	pItem->SetOwner(m.pOwner);
	pItem->Init(*m.pItem, m.pManager, m.pParent);
	return pItem;
}

// frwndmanager.cpp:676
void DoCreateMaroItem(const sFrCreateSub& m) {
	FrMacroItem item;
	item.SetOwner(m.pOwner);
	item.Init(*m.pItem, m.pManager, m.pParent);
}

// frwndmanager.cpp:690
FrWnd* FrWndManager::DoCreate(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent, FrCmdTarget* pOwner) {
	FrWnd* pWnd = 0;
	sFrCreateSub m;
	m.pItem = &item;
	m.pManager = this;
	m.pOwner = pOwner;
	m.pParent = pParent;

	switch (item.m_type) {
	case GI_FORM:           pWnd = DoCreateItem<FrForm>(m); break;
	case 8:  /* GI_FRAME */          pWnd = DoCreateItem<FrFrame>(m); break;
	case GI_BUTTON:         pWnd = DoCreateItem<FrButton>(m); break;
	case GI_STATIC:         pWnd = DoCreateItem<FrStatic>(m); break;
	case GI_TEXTBUTTON:     pWnd = DoCreateItem<FrTextButton>(m); break;
	case 11: /* GI_AREA */           pWnd = DoCreateItem<FrArea>(m); break;
	case 12: /* GI_LISTBOX */        pWnd = DoCreateItem<FrListBox>(m); break;
	case 4:  /* GI_EDIT */           pWnd = DoCreateItem<FrEdit>(m); break;
	case 5:  /* GI_COMBOBOX */       pWnd = DoCreateItem<FrComboBox>(m); break;
	case 13: /* GI_GAUGEBAR */       pWnd = DoCreateItem<FrGaugeBar>(m); break;
	case 14: /* GI_GAUGEBAREX */     pWnd = DoCreateItem<FrGaugeBarEx>(m); break;
	case 15: /* GI_GAUGEBARIMAGE */  pWnd = DoCreateItem<FrGaugeBarImage>(m); break;
	case 16: /* GI_VIEWER */         pWnd = DoCreateItem<FrViewer>(m); break;
	case GI_CONTEXTMENU:    pWnd = DoCreateItem<FrContextMenuCtrl>(m); break;
	case 18: /* GI_TABBUTTON */      pWnd = DoCreateItem<FrTabButton>(m); break;
	case 19: /* GI_GROUPBOX */       pWnd = DoCreateItem<FrGroupBox>(m); break;
	case 6:  /* GI_COMBOCTLEX */     pWnd = DoCreateItem<FrComboCtlEx>(m); break;
	case 20: /* GI_MACROITEM */      pWnd = 0; DoCreateMaroItem(m); break;
	}

	if (pWnd) {
		pWnd->SendCmdToOwnerTarget(FRCMD_INIT, (intptr_t)pWnd, 0);
	}

	return pWnd;
}

// frwndmanager.cpp:736
FrElementDoc* FrWndManager::GetDocument(void) const {
	return m_pDoc;
}

// frwndmanager.cpp:741
const Bitmap* FrWndManager::GetBitmap(const char* resource, const char* id) const {
	if (m_pDoc) {
		return m_pDoc->GetBitmap(id ? std::string(id) : std::string(""));
	}

	return 0;
}

// frwndmanager.cpp:751
bool FrWndManager::IsValidWindow(FrWnd* pWnd) {
	if (m_pDesktop == pWnd) {
		return true;
	}

	return m_pDesktop->FindChild(pWnd) != 0;
}

// frwndmanager.cpp:759
bool FrWndManager::AddTopmostWindow(FrWnd* pWnd) {
	for (std::list<FrWnd*>::iterator it = m_topmostList.begin(); it != m_topmostList.end(); ++it) {
		if (*it == pWnd) {
			return false;
		}
	}

	m_topmostList.push_back(pWnd);
	return true;
}

// frwndmanager.cpp:771
bool FrWndManager::DeleteTopmostWindow(FrWnd* pWnd) {
	for (std::list<FrWnd*>::iterator it = m_topmostList.begin(); it != m_topmostList.end(); ++it) {
		if (*it == pWnd) {
			m_topmostList.erase(it);
			return true;
		}
	}

	return false;
}

// frwndmanager.cpp:785
FrWnd* FrWndManager::GetModalForm(void) {
	std::list<FrWnd*>::reverse_iterator it = m_pDesktop->m_childList.rbegin();
	for (; it != m_pDesktop->m_childList.rend(); ++it) {
		FrForm* pForm = DYNAMIC_CAST(FrForm, *it);
		if (pForm && !pForm->GetFlag(0x4)
			&& (!pForm->m_nFlags.GetFlag(0x100) || !pForm->m_nFlags.GetFlag(0x40))
			&& !pForm->m_nFlags.GetFlag(0x20) && *it != m_pDesktop) {
			return *it;
		}
	}

	return 0;
}

// frwndmanager.cpp:814
FrWnd* FrWndManager::HandOverViewFocus(void) {
	FrWnd* pModal = GetModalForm();

	if (pModal) {
		pModal->m_nFlags.Enable(0x8);
		return 0;
	}

	std::list<FrWnd*>::reverse_iterator it = m_pDesktop->m_childList.rbegin();
	for (; it != m_pDesktop->m_childList.rend(); ++it) {
		FrForm* pForm = DYNAMIC_CAST(FrForm, *it);
		if (pForm && pForm->IsVisible() && pForm->m_dwStyle.GetFlag(0x80)) {
			pForm->m_nFlags.Enable(0x8);

			if (pForm->IsFixed()) {
				break;
			}

			return pForm;
		}
	}

	m_pDesktop->m_nFlags.Enable(0x8);
	return m_pDesktop;
}

// frwndmanager.cpp:845
bool FrWndManager::SetKeyFocus(FrWnd* pWnd, bool resetPrevImeData) {
	if (!CanGetKeyFocus(pWnd)) {
		return false;
	}

	if (m_istate.keyFocused) {
		m_istate.keyFocused->m_nFlags.Disable(0x10);
		m_istate.keyFocused->SendCmdToOwnerTarget(FRCMD_LOSTKEYFOCUS, (intptr_t)&m_istate, 0);
	}

	if (pWnd) {
		pWnd->EnableKeyFocus(m_istate);

		if (resetPrevImeData) {
			if (m_exclusiveKey) {
				WSingleton<CChatMsg>::Instance()->Reset();

				std::string text;
				pWnd->GetWindowTextA(text);
				WSingleton<CChatMsg>::Instance()->SetChatText(text.c_str(), false);
			}
		}
	}

	m_istate.keyFocused = pWnd;
	return true;
}

// frwndmanager.cpp:886
bool FrWndManager::CanGetKeyFocus(FrWnd* pWnd) {
	if (pWnd == 0 || (pWnd->GetStyle().GetFlag(0x80) && pWnd->IsVisible() && !pWnd->GetStyle().GetFlag(0x2))) {
		return true;
	}

	return false;
}

// frwndmanager.cpp:899
bool FrWndManager::ResetKeyFocus(FrWnd* pWnd) {
	if (m_istate.keyFocused == pWnd) {
		m_istate.keyFocused = 0;
		pWnd->m_nFlags.Disable(0x10);
		pWnd->SendCmdToOwnerTarget(FRCMD_LOSTKEYFOCUS, (intptr_t)&m_istate, 0);
		return true;
	}

	for (std::list<FrWnd*>::iterator it = pWnd->m_childList.begin(); it != pWnd->m_childList.end(); ++it) {
		if (ResetKeyFocus(*it) == true) {
			return true;
		}
	}

	return false;
}

// frwndmanager.cpp:918
bool FrWndManager::MoveKeyFocusToNext(bool resetPrevImeData) {
	FrWnd* pParent;

	if (m_istate.keyFocused) {
		pParent = m_istate.keyFocused->GetParent();
		if (pParent == 0) {
			return false;
		}
	} else {
		pParent = m_pDesktop->FindViewFocused(true);
		if (pParent == 0) {
			pParent = m_pDesktop;
		}
	}

	m_topmostList.sort(FocusWndCompare);

	std::list<FrWnd*>::iterator itFocused;
	for (itFocused = pParent->m_childList.begin(); itFocused != pParent->m_childList.end(); ++itFocused) {
		if (m_istate.keyFocused == *itFocused) {
			++itFocused;
			break;
		}
	}

	std::list<FrWnd*>::iterator it;
	for (it = itFocused; it != pParent->m_childList.end(); ++it) {
		if (SetKeyFocus(*it, resetPrevImeData)) {
			return true;
		}
	}

	for (it = pParent->m_childList.begin(); it != itFocused; ++it) {
		if (SetKeyFocus(*it, resetPrevImeData)) {
			return true;
		}
	}

	return false;
}

// frwndmanager.cpp:990
void FrWndManager::SetWheelFocus(FrScrollBar* pScrBar) {
	m_istate.wheelFocused = pScrBar;
}

// frwndmanager.cpp:995
void FrWndManager::MoveCursor(FrWnd* pParent, const char* name) {
	FrWnd* pWnd = pParent->FindChildByName(name);
	if (pWnd) {
		WRect rect = pWnd->GetRect();
		m_pCursor->MoveCursor(WPoint(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f));
	}
}

// frwndmanager.cpp:1005
bool FrWndManager::SetCapture(FrWnd* pWnd) {
	if (m_pCaptured == 0) {
		m_pCaptured = pWnd;
		return true;
	}

	return false;
}

// frwndmanager.cpp:1016
bool FrWndManager::ReleaseCapture(FrWnd* pWnd) {
	if (m_pCaptured == pWnd) {
		m_pCaptured = 0;
		return true;
	}

	return false;
}

// frwndmanager.cpp:1027
bool FrWndManager::HasEscKeyWindow(void) {
	std::list<FrWnd*>::reverse_iterator it = m_pDesktop->m_childList.rbegin();
	for (; it != m_pDesktop->m_childList.rend(); ++it) {
		FrForm* pForm = DYNAMIC_CAST(FrForm, *it);
		if (pForm && pForm->GetFlag(0x1)) {
			return true;
		}
	}

	return false;
}

// frwndmanager.cpp:1040
bool FrWndManager::HasValidWindow(void) {
	std::list<FrWnd*>::reverse_iterator it = m_pDesktop->m_childList.rbegin();
	for (; it != m_pDesktop->m_childList.rend(); ++it) {
		FrForm* pForm = DYNAMIC_CAST(FrForm, *it);
		if (pForm && !pForm->IsFixed()) {
			return true;
		}
	}

	return false;
}

// frwndmanager.cpp:1055
WPoint FrWndManager::GetCreatePosition(const WSize& rectSize) {
	const WRect& rc = m_pDesktop->GetRect();

	int x = (int)((rc.w - rectSize.w + 1.0f) * 0.5f);
	int y = (int)((rc.h - rectSize.h + 1.0f) * 0.5f);

	WRect rect(WPoint(rc.x + x, rc.y + y), rectSize);
	ConfineRect(rect);

	return WPoint(rect.x, rect.y);
}

// frwndmanager.cpp:1072
void FrWndManager::ConfineRect(WRect& dr) {
	const WRect& rc = m_pDesktop->GetRect();
	if (dr.x < rc.x) dr.x = rc.x;
	if (dr.y < rc.y) dr.y = rc.y;
	if (dr.x > rc.x + rc.w - 100.0f) dr.x = rc.x + rc.w - 100.0f;
	if (dr.y > rc.Bottom() - 30.0f) dr.y = rc.Bottom() - 30.0f;
}

// frwndmanager.cpp:1082
void FrWndManager::SetExclusiveKey(bool set) {
	m_exclusiveKey = set;

	if (WSingleton<CChatMsg>::Instance()) {
		WSingleton<CChatMsg>::Instance()->SetActive(set, true, false);
	}
}

// frwndmanager.cpp:1098
float FrWndManager::PrintText(const WPoint& pos, unsigned long align, const char* text, float limit, unsigned long emoDiffuse) {
	if (!m_pDevice)
		return 0.0f;

	if (m_pEmoticon) {
		if (limit > 0.0f && m_pEmoticon->GetTextWidth(text) > limit)
			return m_pEmoticon->PrintText(pos, align, m_istate.im->MakeShortID(text, limit), emoDiffuse);
		return m_pEmoticon->PrintText(pos, align, text, emoDiffuse);
	}

	if (limit > 0.0f && m_pDevice->GetTextExtend(text) > limit)
		return m_pDevice->PrintText(pos, align, m_istate.im->MakeShortID(text, limit), 0);
	return m_pDevice->PrintText(pos, align, text, 0);
}

// frwndmanager.cpp:1119
float FrWndManager::PrintText11(const WPoint& pos, unsigned long align, const char* text, float limit, unsigned long emoDiffuse) {
	if (!m_pDevice)
		return 0.0f;

	if (m_pEmoticon) {
		if (limit > 0.0f && m_pEmoticon->GetTextWidth11(text) > limit)
			return m_pEmoticon->PrintText11(pos, align, m_istate.im->MakeShortID(text, limit), emoDiffuse);
		return m_pEmoticon->PrintText11(pos, align, text, emoDiffuse);
	}

	if (limit > 0.0f && m_pDevice->GetTextExtend11(text) > limit)
		return m_pDevice->PrintText11(pos, align, m_istate.im->MakeShortID(text, limit), 0);
	return m_pDevice->PrintText11(pos, align, text, 0);
}

// frwndmanager.cpp:1140
float FrWndManager::GetTextWidth(const char* text) {
	if (m_pDevice == 0) {
		return 0.0f;
	}

	if (m_pEmoticon) {
		return m_pEmoticon->GetTextWidth(text);
	}

	return m_pDevice->GetTextExtend(text);
}

// frwndmanager.cpp:1151
void FrWndManager::IME_ShowCandWindow(FrEdit* pFocusedEdit, WPoint& caretPos) {
	m_pFocusedEdit = pFocusedEdit;
	m_caretPos = caretPos;
}
