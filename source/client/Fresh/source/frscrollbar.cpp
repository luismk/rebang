// Original: d:\build\custom\temp\code\client\fresh\source\frscrollbar.cpp
#include <math.h>

#include "frscrollbar.h"
#include "frelement.h"
#include "frwndmanager.h"
#include "ObjectFactory.h"
#include "../client/ProjectG/commonutil.h"
#include "../client/Wangreal/include/bitmap.h"
#include "frgraphicinterface.h"

// frscrollbar.cpp:15 (run-time type and factory registration of FrScrollBar)
const WRTTI FrScrollBar::m_RTTI("FrScrollBar", &FrWnd::m_RTTI);

IObject* FrScrollBarMakeInstance(void) {
	return new FrScrollBar;
}

struct __sFrScrollBar {
	__sFrScrollBar(void) {
		ObjectFactory().AddObjectFunctor(FrScrollBarMakeInstance, "FrScrollBar");
	}
};
static __sFrScrollBar s_registerFrScrollBar;

// frscrollbar.cpp:18
FrScrollBar::FrScrollBar(void) {
	for (int i = 0; i < 3; i++) {
		m_frag[i].n_img = 0;
		m_frag[i].o_img = 0;
		m_frames[i] = 0;
	}
}

// frscrollbar.cpp:27
FrScrollBar::~FrScrollBar(void) {
}

// frscrollbar.cpp:35
void FrScrollBar::Init(FrWndManager* pManager, FrWnd* parentWnd, int row, int rowCapacity, int colCapacity, bool atLeft) {
	m_dwStyle.Enable(1);
	m_pParentWnd = parentWnd;

	m_itemNum = 0;
	m_curTopRow = 0;
	m_followBottom = true;
	m_showGuide = false;
	m_mouseDown = false;

	FrElementDoc* pDoc = pManager->GetDocument();

	for (int i = 0; i < 3; i++) {
		m_frag[i].n_img = pDoc->GetBitmap(MakeStr("scroll_n_%d", i + 1));
		m_frag[i].o_img = pDoc->GetBitmap(MakeStr("scroll_o_%d", i + 1));

		m_frag[i].width = m_frag[i].n_img->Width();
		m_frag[i].height = m_frag[i].n_img->Height();

		m_frames[i] = pDoc->GetBitmap(MakeStr("scrollbg%d", i + 1));
	}

	Create("scrollbar", "scrollbar", pManager, 1, WRect(0.0f, 0.0f, 0, 0), parentWnd);
	m_dwStyle.Disable(0x800);

	Resize(rowCapacity, colCapacity, atLeft);
}

// frscrollbar.cpp:88
void FrScrollBar::Resize(int rowCapacity, int colCapacity, bool atLeft) {
	m_rowCapacity = rowCapacity;
	m_colCapacity = colCapacity;

	m_rect = m_pParentWnd->GetRect();

	m_atLeft = atLeft;

	if (!atLeft) {
		m_rect.x += m_rect.w - m_frag[0].width;
	}

	m_rect.w = (float)m_frag[0].width;

	SetBarHnY();
}

// frscrollbar.cpp:113
void FrScrollBar::SetCurTopRow(int row) {
	int lastRow = GetCurRowCount() - m_rowCapacity;
	lastRow = lastRow < 0 ? 0 : lastRow;
	m_curTopRow = (float)(row > lastRow ? lastRow : row);

	SetBarHnY();
}

// frscrollbar.cpp:120
void FrScrollBar::AddItem(void) {
	m_itemNum++;

	if (m_followBottom) {
		int lastRow = GetCurRowCount() - m_rowCapacity;
		m_curTopRow = (float)(lastRow < 0 ? 0 : lastRow);
	}
	SetBarHnY();
}

// frscrollbar.cpp:130
void FrScrollBar::DelItem(void) {
	if (m_itemNum > 0) {
		m_itemNum--;
		if (m_colCapacity && m_itemNum % m_colCapacity == 0 && m_curTopRow > 0.0f)
			m_curTopRow -= 1.0f;
	}

	SetBarHnY();
}

// frscrollbar.cpp:144
void FrScrollBar::ClearItem(void) {
	m_curTopRow = 0;
	m_itemNum = 0;
	SetBarHnY();
}

// frscrollbar.cpp:151
void FrScrollBar::ScrollToFirst(void) {
	m_curTopRow = 0;
	m_followBottom = false;
	SetBarHnY();
}

// frscrollbar.cpp:159
void FrScrollBar::ScrollToBottom(void) {
	m_followBottom = true;
	int lastRow = GetCurRowCount() - m_rowCapacity;
	m_curTopRow = (float)(lastRow < 0 ? 0 : lastRow);

	SetBarHnY();
}

// frscrollbar.cpp:169
void FrScrollBar::ScrollUp(int delta) {
	if (delta < 0)
		delta = m_rowCapacity / 2;

	if (m_curTopRow - delta > 0.0f)
		m_curTopRow = m_curTopRow - delta;
	else
		m_curTopRow = 0.0f;

	m_followBottom = false;
	SetBarHnY();
}

// frscrollbar.cpp:183
void FrScrollBar::ScrollDown(int delta) {
	if (delta < 0)
		delta = m_rowCapacity / 2;

	if (m_curTopRow + delta + m_rowCapacity < GetCurRowCount())
		m_curTopRow += delta;
	else {
		m_curTopRow = (float)(GetCurRowCount() - m_rowCapacity);
		m_followBottom = true;
	}

	SetBarHnY();
}

// frscrollbar.cpp:199
void FrScrollBar::SetBarHnY(void) {
	if (GetCurRowCount() > 0)
		m_frag[1].height = (int)((m_rect.h - m_frag[0].height - m_frag[2].height) * m_rowCapacity / GetCurRowCount());

	m_barH = (float)(m_frag[0].height + m_frag[1].height + m_frag[2].height);

	if (GetCurRowCount() - m_rowCapacity > 0) {
		float topRow = (float)floor(m_curTopRow);
		if (m_curTopRow - topRow > 0.5f)
			m_curTopRow = topRow + 1.0f;
		else
			m_curTopRow = topRow;

		m_barY = (float)(int)((m_rect.h - m_barH) * m_curTopRow / (GetCurRowCount() - m_rowCapacity) + 0.5f);
	}
}

// frscrollbar.cpp:221
void FrScrollBar::OnDraw(void) {
	if (m_rowCapacity == 0)
		return;

	FrWnd::OnDraw();

	unsigned long color = FrALPHA(0xffffffff, m_wndAlpha2 * m_wndAlpha);

	bool wheelFocused = m_pWndManager->GetWheelFocus() == this;

	if (m_rowCapacity < GetCurRowCount()) {
		WRect rc(m_rect.x + 2.0f, m_rect.y, m_frames[0]->Width(), m_frames[0]->Height());
		GDI()->DrawTexture(m_frames[0], rc, color, 0);

		rc.y += rc.h;
		rc.h = m_rect.h - m_frames[0]->Height() - m_frames[1]->Height();
		GDI()->DrawTexture(m_frames[1], rc, color, 0);

		rc.y += rc.h;
		rc.h = (float)m_frames[2]->Height();
		GDI()->DrawTexture(m_frames[2], rc, color, 0);

		WRect bar(m_rect.x + 3.0f, m_barY + m_rect.y, m_rect.w, m_frag[0].height);
		GDI()->DrawTexture(wheelFocused ? m_frag[0].o_img : m_frag[0].n_img, bar, color, 0);

		bar.y += bar.h;
		bar.h = (float)m_frag[1].height;
		GDI()->DrawTexture(wheelFocused ? m_frag[1].o_img : m_frag[1].n_img, bar, color, 0);

		bar.y += bar.h;
		bar.h = (float)m_frag[2].height;
		GDI()->DrawTexture(wheelFocused ? m_frag[2].o_img : m_frag[2].n_img, bar, color, 0);
	} else if (m_showGuide) {
		WRect bar(m_rect.x + 2.0f, m_rect.y, m_rect.w, m_frag[0].height);
		GDI()->DrawTexture(wheelFocused ? m_frag[0].o_img : m_frag[0].n_img, bar, color, 0);

		bar.y += bar.h;
		bar.h = m_rect.h - m_frag[0].height - m_frag[2].height;
		GDI()->DrawTexture(wheelFocused ? m_frag[1].o_img : m_frag[1].n_img, bar, color, 0);

		bar.y += bar.h;
		bar.h = (float)m_frag[2].height;
		GDI()->DrawTexture(wheelFocused ? m_frag[2].o_img : m_frag[2].n_img, bar, color, 0);
	}
}

// frscrollbar.cpp:287
void FrScrollBar::OnResize(void) {
	Resize(m_rowCapacity, m_colCapacity, m_atLeft);
}

// frscrollbar.cpp:292
void FrScrollBar::OnMouseMove(const WPoint& mousePos) {
	if (GetCapture() == this) {
		if (mousePos.y + m_barY - m_dragPrevY < 0.0f) {
			m_barY = 0.0f;
		} else if (mousePos.y + m_barY + m_barH - m_dragPrevY > m_rect.h) {
			m_barY = m_rect.h - m_barH;
		} else {
			m_barY += mousePos.y - m_dragPrevY;
			m_dragPrevY = mousePos.y;
		}

		int d = (int)(m_rect.h - m_barH);
		m_curTopRow = d ? (GetCurRowCount() - m_rowCapacity) * m_barY / d : 0.0f;
		m_followBottom = m_curTopRow < GetCurRowCount() - m_rowCapacity ? false : true;
		m_mouseDown = true;
	} else {
		m_mouseDown = false;
	}
}

// frscrollbar.cpp:321
bool FrScrollBar::OnLButtonDown(const WPoint& mousePos) {
	if (mousePos.y < m_rect.y + m_barY) {
		ScrollUp(-1);
	} else if (m_rect.y + m_barH + m_barY < mousePos.y) {
		ScrollDown(-1);
	} else {
		if (m_rowCapacity >= GetCurRowCount() || !SetCapture())
			return true;

		m_dragPrevY = mousePos.y;
	}

	return false;
}

// frscrollbar.cpp:344
bool FrScrollBar::OnLButtonUp(const WPoint& mousePos) {
	SetBarHnY();

	if (GetCapture() == this) {
		ReleaseCapture();
		return false;
	}

	return true;
}

// frscrollbar.cpp:357
void FrScrollBar::OnWheel(FrInputState& istate) {
	if (istate.wheelFocused == this) {
		if (istate.wheelDelta > 0.0f)
			ScrollUp((int)istate.wheelDelta / 119);
		else
			ScrollDown((int)istate.wheelDelta / -119);
	}
}
