// Original: d:\build\custom\temp\code\client\fresh\source\frviewer.cpp
#include <stdio.h>
#include <stdlib.h>

#include "frviewer.h"
#include "frelement.h"
#include "frgraphicinterface.h"
#include "frscrollbar.h"
#include "frwndmanager.h"
#include "ObjectFactory.h"
#include "../client/ProjectG/background.h"
#include "../client/Wangreal/include/wresrcmng.h"

class cFile;

// westpak.cpp (client/Wangreal/source/westpak); westpak.h does not declare it.
void __cdecl CloseCFile(cFile* hdl);

// frviewer.cpp:16 (run-time type and factory registration of FrViewer)
const WRTTI FrViewer::m_RTTI("FrViewer", &FrWnd::m_RTTI);

IObject* FrViewerMakeInstance(void) {
	return new FrViewer;
}

struct __sFrViewer {
	__sFrViewer(void) {
		ObjectFactory().AddObjectFunctor(FrViewerMakeInstance, "FrViewer");
	}
};
static __sFrViewer s_registerFrViewer;

// frviewer.cpp:19
FrViewer::FrViewer(void) {
	m_pView = 0;
	m_fileName = "";
	m_bgColor = 0xffffffff;
	m_mouseEvent = true;
}

// frviewer.cpp:28
FrViewer::~FrViewer(void) {
	if (m_pView) { delete m_pView; m_pView = 0; }
}

// frviewer.cpp:34
void FrViewer::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	pManager->GetDocument();
	std::map<std::string, std::string>& param = item.m_param;
	std::string fileName("");
	bool useWheel = false;

	if (param.find("bgcolor") != param.end())
		sscanf(param["bgcolor"].c_str(), "%x", &m_bgColor);

	if (param.find("image") != param.end())
		fileName = param["image"];

	if (param.find("use_wheel") != param.end())
		useWheel = param["use_wheel"] == "true";

	if (param.find("mouse_event") != param.end())
		m_mouseEvent = atoi(param["mouse_event"].c_str()) != 0;

	m_pItem = &item;

	WRect rect(item.m_rect.left, item.m_rect.top,
		(short)(item.m_rect.right - item.m_rect.left),
		(short)(item.m_rect.bottom - item.m_rect.top));
	unsigned long style = (m_mouseEvent ? 0 : 0x40) | 0x1;
	Create(item.m_caption.c_str(), item.m_name.c_str(), pManager, style, rect, pParent);

	int rowCapacity = (int)(rect.h * 0.05f);

	if (useWheel) {
		if (m_pScrBar == 0) {
			m_pScrBar = new FrScrollBar;
			m_pScrBar->Init(pManager, this, 0, rowCapacity, 1, false);
			m_pScrBar->FollowBottom(false);
			m_pScrBar->SetGuideVisible(true);
		}
	}

	if (fileName.length()) {
		if (!Open(fileName.c_str()))
			fileName = "";
	}
}

// frviewer.cpp:103
bool FrViewer::Open(const char* fileName) {
	if (m_fileName != fileName) {
		cFile* pFile = g_resrcmng->GetCFile(fileName, 0);
		if (!pFile)
			return false;

		CloseCFile(pFile);

		m_fileName = fileName;

		if (m_pView) { delete m_pView; m_pView = 0; }
		m_pView = new CBackGround(fileName, false);

		if (m_pScrBar) {
			m_pScrBar->ClearItem();

			int h = (int)(m_pView->GetHeight() * 0.05f);
			for (int i = 0; i != h; ++i)
				m_pScrBar->AddItem();
		}
	}

	return true;
}

// frviewer.cpp:134
void FrViewer::OnDraw(void) {
	FrWnd::OnDraw();

	if (m_pView) {
		WRect dst = m_rect;
		WPoint src(0.0f, 0.0f);

		if (m_pScrBar)
			src.y = m_pScrBar->GetCurTopRow() * 20.0f;

		if (IsEnabled())
			m_pView->Draw(&src, &dst, FrALPHA(m_bgColor, m_wndAlpha));
		else
			m_pView->Draw(&src, &dst, FrALPHA(0x50B0B0B0, m_wndAlpha));
	}

	SendCmdToOwnerTarget(FRCMD_OWNERDRAW, 0, 0);
}

// frviewer.cpp:158
void FrViewer::OnResize(void) {
	if (m_pView && m_pScrBar) {
		m_pScrBar->Resize(
			(int)(m_rect.h * 0.05f), 1, false);

		m_pScrBar->ClearItem();

		int h = (int)(m_pView->GetHeight() * 0.05f);
		for (int i = 0; i != h; ++i)
			m_pScrBar->AddItem();
	}
}

// frviewer.cpp:173
void FrViewer::SetMouseEvent(bool mouseEvent) {
	m_mouseEvent = mouseEvent;

	if (mouseEvent)
		m_dwStyle.Disable(0x40);
	else
		m_dwStyle.Enable(0x40);
}

// frviewer.cpp:183
void FrViewer::OnSetCursor(bool bInClient, const WPoint& mousePos) {
	if (m_wndName == "over_char_sel") {
		if (bInClient && m_mouseEvent)
			SetCursor(1);
	}

	if (bInClient && m_mouseEvent)
		SetCursor(1);
}

// frviewer.cpp:199
bool FrViewer::OnLButtonDown(const WPoint& pt) {
	SendCmdToOwnerTarget(FRCMD_LBUTTONDOWN, 0, 0);
	return false;
}

// frviewer.cpp:205
bool FrViewer::OnLButtonUp(const WPoint& pt) {
	SendCmdToOwnerTarget(FRCMD_LBUTTONUP, 0, 0);
	return false;
}

// frviewer.cpp:211
bool FrViewer::OnRButtonDown(const WPoint& pt) {
	SendCmdToOwnerTarget(FRCMD_RBUTTONDOWN, 0, 0);
	return false;
}

// frviewer.cpp:217
bool FrViewer::OnRButtonUp(const WPoint& pt) {
	SendCmdToOwnerTarget(FRCMD_RBUTTONUP, 0, 0);
	return false;
}

// frviewer.cpp:223
void FrViewer::OnDblClick(const WPoint& pt) {
	SendCmdToOwnerTarget(FRCMD_DBLCLICK, 0, 0);
}
