// Original: d:\build\custom\temp\code\client\fresh\source\frtooltip.cpp
#define FRTOOLTIP_WTYPES_INLINE
#include "frtooltip.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "../client/ProjectG/InputManager.h"
#include "../client/ProjectG/commonutil.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wutil.h"
#include "../client/Wangreal/include/wview.h"

// Named constants of frtooltip.cpp (the original's .rdata).
const float CURSOR_PIVOT = 20.0f;
const float TEXT_TOOLTIP_PIVOT_X = 8.0f;
// Line interval used by PrintText11 and tooltip layout.
// Define with external linkage so other translation units and prebuilt libs
// can reference the symbol. Place it at global scope and mark 'extern'
// so the constant has external linkage with a single definition here.
extern const float TEXT_TOOLTIP_LINE_INTERVAL = 2.0f;
// frtooltip.cpp:29 (keeps rc inside the view; nonzero when it moved)
int FixRectInView(WRect& rc) {
	WRect copy = rc;

	if (rc.x < 0.0f) {
		rc.x = 0.0f;
	}

	if (rc.y < 0.0f) {
		rc.y = 0.0f;
	}

	if (rc.w + rc.x > g_view->GetWidth()) {
		rc.x = g_view->GetWidth() - rc.w;
	}

	if (rc.y + rc.h > g_view->GetHeight()) {
		rc.y = g_view->GetHeight() - rc.h;
	}

	return copy.x != rc.x || copy.y != rc.y || copy.w != rc.w || copy.h != rc.h;

}
// (previously used anonymous namespace closed above)

// frtooltip.cpp:56
FrToolTip::FrToolTip(FrWndManager* pWndManager)
	: m_fAlpha(0.0f), m_pWndManager(pWndManager), m_textColor(0xff000000) {
	m_Frame.Init();
}

// frtooltip.cpp:61
FrToolTip::~FrToolTip(void) {
}

// frtooltip.cpp:66
void FrToolTip::Move(const WPoint& pt) {
	m_rcWnd.x = pt.x;
	m_rcWnd.y = pt.y;
}

// frtooltip.cpp:72
void FrToolTip::SetToolTipText(const std::string& text) {
	if (text == "") {
		return;
	}

	m_fAlpha = 220.0f;
	if (m_orgText == text) {
		return;
	}

	m_orgText = text;
	m_FrText = m_orgText;
	Prepare();
}

// frtooltip.cpp:86
void FrToolTip::SetFrameStyle(unsigned long style) {
	m_Frame.m_style = Between<unsigned long>(0, style, 3);
}

// frtooltip.cpp:92
void FrToolTip::OnProcess(float delta) {
	if (m_fAlpha <= 0.0f) {
		return;
	}

	if (m_fAlpha > 0.0f) {
		m_fAlpha -= delta * 660.0f;

		m_rcWnd.x = m_pWndManager->GetMousePos().x + CURSOR_PIVOT;
		m_rcWnd.y = m_pWndManager->GetMousePos().y + CURSOR_PIVOT;

		FixRectInView(m_rcWnd);
	}

	if (m_fAlpha <= 0.0f) {
		Reset();
	}
}

// frtooltip.cpp:112
void FrToolTip::OnDisplay(void) {
	if (m_fAlpha <= 0.0f) {
		return;
	}

	m_Frame.Render(m_rcWnd, ((unsigned long)(unsigned char)m_fAlpha << 24) | 0x00ffffff);

	WPoint pos(m_rcWnd.x + TEXT_TOOLTIP_PIVOT_X, m_rcWnd.y + 5.0f);
	FrGraphicInterface* pGI = m_pWndManager->GetGDI();
	pGI->SetTextColor(m_textColor, 0xffffffff);
	float prevAlpha = pGI->GetAlpha();
	pGI->SetAlpha(m_fAlpha * (1.0f / 255.0f));

	pGI->PrintText11(pos, m_FrText);

	pGI->SetAlpha(prevAlpha);
}

// frtooltip.cpp:140
void FrToolTip::Prepare(void) {
	m_fAlpha = 220.0f;
	m_rcWnd.x = g_input->GetMousePoint().x + CURSOR_PIVOT;
	m_rcWnd.y = g_input->GetMousePoint().y + CURSOR_PIVOT;
	FrGraphicInterface* pGI = m_pWndManager->GetGDI();
	unsigned int lineCount = m_FrText.GetLineSize();
	m_rcWnd.h = (float)pGI->GetFontHeight11() * lineCount + Max<int>(0, (int)lineCount - 1) * 2.0f + TEXT_TOOLTIP_PIVOT_X;

	m_rcWnd.w = m_FrText.GetWidthLong(pGI) + 16.0f;
}

// frtooltip.cpp:154
void FrToolTip::Reset(void) {
	m_fAlpha = 0.0f;
	m_rcWnd = WRect();
	m_orgText = "";
	m_FrText.Clear();
}

// frtooltip.cpp:165
FrToolTip::cToolTipFrame::cToolTipFrame(void) {
	m_style = 3;
}

// frtooltip.cpp:174
FrToolTip::cToolTipFrame::~cToolTipFrame(void) {
	for (std::vector<TOOLTIPFRM>::iterator it = m_Frames.begin(); it != m_Frames.end(); ++it) {
		for (int i = 0; i < FRMIDX_MAX; ++i) {
			if (g_resrcmng != 0 && (*it)[i] != 0) {
				g_resrcmng->Release((*it)[i]);
				(*it)[i] = 0;
			}
		}
		it->clear();
	}
	m_Frames.clear();
}

// frtooltip.cpp:189
void FrToolTip::cToolTipFrame::Init(void) {
	m_Frames.reserve(4);

	for (int iFrm = 0; iFrm < 4; ++iFrm) {
		TOOLTIPFRM Frame(FRMIDX_MAX, (WOverlay*)0);
		for (int i = 0; i < FRMIDX_MAX; ++i) {
			Frame.push_back(0);
			WOverlay* pOverlay = g_resrcmng->GetOverlay(MakeStr("tooltip_frm%02d_%02d.tga", iFrm, i), 0);
			if (pOverlay != 0) {
				Frame[i] = pOverlay;
			}
		}
		m_Frames.push_back(Frame);
	}
}

// frtooltip.cpp:214
void FrToolTip::cToolTipFrame::SetStyle(unsigned long style) {
	m_style = Between<unsigned long>(0, style, 3);
}

// frtooltip.cpp:221
void FrToolTip::cToolTipFrame::Render(const WRect& rc, unsigned long color) {
	float fw = rc.w - 16.0f;
	float fh = rc.h - 16.0f;
	TOOLTIPFRM& frm = m_Frames[m_style];
	WRect src(0.0f, 0.0f, 1.0f, 1.0f);

	if (frm[LT] != 0) {
		frm[LT]->Render(g_view, src, WRect(rc.x, rc.y, 8, 8), 0, color, 0, 0);
	}
	if (frm[LM] != 0) {
		frm[LM]->Render(g_view, src, WRect(rc.x, rc.y + TEXT_TOOLTIP_PIVOT_X, 8, fh), 0, color, 0, 0);
	}
	if (frm[LB] != 0) {
		frm[LB]->Render(g_view, src, WRect(rc.x, fh + rc.y + TEXT_TOOLTIP_PIVOT_X, 8, 8), 0, color, 0, 0);
	}
	if (frm[MT] != 0) {
		frm[MT]->Render(g_view, src, WRect(rc.x + TEXT_TOOLTIP_PIVOT_X, rc.y, fw, 8), 0, color, 0, 0);
	}
	if (frm[MM] != 0) {
		frm[MM]->Render(g_view, src, WRect(rc.x + TEXT_TOOLTIP_PIVOT_X, rc.y + TEXT_TOOLTIP_PIVOT_X, fw, fh), 0, color, 0, 0);
	}
	if (frm[MB] != 0) {
		frm[MB]->Render(g_view, src, WRect(rc.x + TEXT_TOOLTIP_PIVOT_X, fh + rc.y + TEXT_TOOLTIP_PIVOT_X, fw, 8), 0, color, 0, 0);
	}
	if (frm[RT] != 0) {
		frm[RT]->Render(g_view, src, WRect(fw + rc.x + TEXT_TOOLTIP_PIVOT_X, rc.y, 8, 8), 0, color, 0, 0);
	}
	if (frm[RM] != 0) {
		frm[RM]->Render(g_view, src, WRect(fw + rc.x + TEXT_TOOLTIP_PIVOT_X, rc.y + TEXT_TOOLTIP_PIVOT_X, 8, fh), 0, color, 0, 0);
	}
	if (frm[RB] != 0) {
		frm[RB]->Render(g_view, src, WRect(fw + rc.x + TEXT_TOOLTIP_PIVOT_X, fh + rc.y + TEXT_TOOLTIP_PIVOT_X, 8, 8), 0, color, 0, 0);
	}
}
