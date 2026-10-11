// Original: d:\build\custom\temp\code\client\fresh\source\frdesktop.cpp
#include <stdio.h>
#include <string.h>

#include "frdesktop.h"
#include "frelement.h"
#include "frgraphicinterface.h"
#include "frscrollbar.h"
#include "ObjectFactory.h"
#include "../client/ProjectG/background.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wview.h"

// frdesktop.cpp:42 (run-time type and factory registration of FrDesktop)
const WRTTI FrDesktop::m_RTTI("FrDesktop", &FrWnd::m_RTTI);

IObject* FrDesktopMakeInstance(void) {
	return new FrDesktop;
}

struct __sFrDesktop {
	__sFrDesktop(void) {
		ObjectFactory().AddObjectFunctor(FrDesktopMakeInstance, "FrDesktop");
	}
};
static __sFrDesktop s_registerFrDesktop;

// frdesktop.cpp:50
FrDesktop::FrDesktop(void)
	: m_pBackGround(0)
	, m_bShow(false) {
	m_bgColor = 0xffffffff;
	m_bLocked = false;
	m_pAniBg = 0;
	m_pScrBar = new FrScrollBar;
}

// frdesktop.cpp:69
FrDesktop::~FrDesktop(void) {
	if (m_pScrBar) { delete m_pScrBar; m_pScrBar = 0; }
	if (m_pBackGround) { delete m_pBackGround; m_pBackGround = 0; }
	if (g_resrcmng && m_pAniBg) { g_resrcmng->Release(m_pAniBg); m_pAniBg = 0; }
}

// frdesktop.cpp:76
void FrDesktop::Init(float width, float height) {
	Create("Desktop", "Desktop", m_pWndManager, 1, WRect(0.0f, 0.0f, width, height), 0);
	SetWheelEvent(false);
}

// frdesktop.cpp:82
void FrDesktop::SetScroll(float du, float dv) {
	if (m_pBackGround)
		m_pBackGround->SetDUV(du, dv);
}

// frdesktop.cpp:88
void FrDesktop::OnProc(const float deltaTime) {
	if (m_pBackGround && m_bShow) {
		m_pBackGround->Process(deltaTime);
	}

	if (m_pAniBg && m_bShow) {
		float dx = m_velX * deltaTime;
		float dy = m_velY * deltaTime;

		m_aniBgSrcX += dx;
		m_aniBgSrcY += dy;

		if (m_aniBgSrcX + m_aniBgDest.w >= m_pAniBg->GetWidth())
			m_aniBgSrcX = dx;
		else if (m_aniBgSrcX < 0)
			m_aniBgSrcX += m_pAniBg->GetWidth() - m_aniBgDest.w;

		if (m_aniBgSrcY + m_aniBgDest.h >= m_pAniBg->GetHeight())
			m_aniBgSrcY = dy;
		else if (m_aniBgSrcY < 0)
			m_aniBgSrcY += m_pAniBg->GetHeight() - m_aniBgDest.h;
	}
}

// frdesktop.cpp:127
void FrDesktop::OnDraw(void) {
	FrWnd::OnDraw();

	if (m_pBackGround && m_bShow) {
		m_pBackGround->Draw(m_bgColor);
	}

	if (m_pAniBg && m_bShow) {
		WRect src;

		float width = (float)m_pAniBg->GetWidth();
		float height = (float)m_pAniBg->GetHeight();

		src.x = (1.0f / width) * m_aniBgSrcX;
		src.y = (1.0f / height) * m_aniBgSrcY;
		src.w = (1.0f / width) * m_aniBgDest.w;
		src.h = (1.0f / height) * m_aniBgDest.h;
		m_pAniBg->Render(g_view, src, m_aniBgDest, 0, 0xffffffff, 0.0f, 0);
	}
}

// frdesktop.cpp:154
void FrDesktop::SetWallPaper(const char* pszFilename, bool bShow) {
	if (m_bLocked)
		return;

	if (!pszFilename) {
		m_bgFilename = "";
		m_bShow = false;
		if (m_pBackGround) { delete m_pBackGround; m_pBackGround = 0; }
		return;
	}

	if (stricmp(pszFilename, m_bgFilename.c_str()) != 0) {
		if (m_pBackGround) { delete m_pBackGround; m_pBackGround = 0; }
		m_pBackGround = new CBackGround(pszFilename, true);
		m_bgFilename = pszFilename;
		m_bShow = bShow;
	} else {
		m_bShow = bShow;
	}
}

// frdesktop.cpp:189
void FrDesktop::SetWallPaperFromBitmap(const Bitmap* bitmap, bool bShow) {
	if (bitmap) {
		if (m_pBackGround) { delete m_pBackGround; m_pBackGround = 0; }
		m_pBackGround = new CBackGround(true, bitmap);
		m_bgFilename = "";
	}

	m_bShow = bShow;
}

// frdesktop.cpp:207
void FrDesktop::ResetRect(void) {
	m_rect.w = GDI()->GetViewWidth();
	m_rect.h = GDI()->GetViewHeight();

	if (m_pBackGround)
		m_pBackGround->ResetScreenSize();
}

// frdesktop.cpp:216
bool FrDesktop::IsWallPaperVisible(void) {
	return m_bShow && m_pBackGround && m_bgColor > 0;
}

// frdesktop.cpp:224
void FrDesktop::SetAniBg(FrGuiItem& aniBg) {
	if (g_resrcmng && m_pAniBg) {
		g_resrcmng->Release(m_pAniBg);
		m_pAniBg = 0;
	}
	m_velX = m_velY = 0;
	m_aniBgSrcX = m_aniBgSrcY = 0;

	_RectangleSHORT& rc = aniBg.m_rect;
	m_aniBgDest = WRect(rc.left, rc.top, rc.Width(), rc.Height());

	std::map<std::string, std::string>& param = aniBg.m_param;
	if (param.find("img") != param.end()) {
		m_pAniBg = g_resrcmng->GetOverlay(param["img"].c_str(), 0);
	}

	if (param.find("vel") != param.end() && !param["vel"].empty()) {
		sscanf(param["vel"].c_str(), "%d %d", &m_velX, &m_velY);
	}
}
