// Original: d:\build\custom\temp\code\client\fresh\source\frtabbutton.cpp
#include <stdio.h>

#include "frtabbutton.h"
#include "frelement.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "bitmap.h"

// frtabbutton.cpp:16 (run-time type and factory registration of FrTabButton)
const WRTTI FrTabButton::m_RTTI("FrTabButton", &FrWnd::m_RTTI);

IObject* FrTabButtonMakeInstance(void) {
	return new FrTabButton;
}

struct __sFrTabButton {
	__sFrTabButton(void) {
		ObjectFactory().AddObjectFunctor(FrTabButtonMakeInstance, "FrTabButton");
	}
};
static __sFrTabButton s_registerFrTabButton;

// frtabbutton.cpp:19
FrTabButton::FrTabButton(void) {
	m_sepImg = 0;
	m_sepWidth = 0.0f;
	m_selected = 0;
	m_underCursor = 0;
	m_pressed = 0;
	m_btnType = BT_NONE;
	m_pBtnBgImg[2] = 0;
	m_pBtnBgImg[1] = 0;
	m_pBtnBgImg[0] = 0;
	m_szPushSound = "ui_icon_click";
}

// frtabbutton.cpp:37
FrTabButton::~FrTabButton(void) {
	ClearTab();
}

// frtabbutton.cpp:43
void FrTabButton::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	FrElementDoc* pDoc = pManager->GetDocument();

	// frtabbutton.cpp:48
	std::map<std::string, std::string>& param = item.m_param;
	if (param.find("sepImg") != param.end()) {
		m_sepImg = pDoc->GetBitmap(param["sepImg"].c_str());
		if (m_sepImg)
			m_sepWidth = (float)m_sepImg->Width();
	}

	m_pBtnBgImg[0] = pDoc->GetBitmap(param["below_normal"]);
	m_pBtnBgImg[1] = pDoc->GetBitmap(param["below_over"]);
	m_pBtnBgImg[2] = pDoc->GetBitmap(param["below_selected"]);

	_RectangleSHORT r = item.m_rect;
	Create(item.m_caption.c_str(), item.m_name.c_str(), pManager, 1, WRect(r.left, r.top, r.Width(), r.Height()), pParent);

	ClearTab();
}

// frtabbutton.cpp:69
void FrTabButton::PreCreateWindow(FrWndManager* pManager, unsigned long dwStyle, const WRect& rect, FrWnd* pParentWnd) {
	FrWnd::PreCreateWindow(pManager, dwStyle, rect, pParentWnd);

	sFRESH_HANDLER chk;
	memset(&chk, 0, sizeof(chk));
	SendCmdToOwnerTarget(FRCMD_LBUTTONDOWN, 0, &chk);
	if (chk.pFn)
		m_btnType = BT_DOWN;

	memset(&chk, 0, sizeof(chk));
	SendCmdToOwnerTarget(FRCMD_LBUTTONUP, 0, &chk);
	if (chk.pFn)
		m_btnType = BT_UP;

	if (m_btnType == BT_NONE)
		m_dwStyle.Enable(0x40);
}

// frtabbutton.cpp:89
void FrTabButton::ClearTab(void) {
	m_rect.w = 0.0f;
	m_rect.h = 0.0f;

	for (TBUTTON_LIST::iterator it = m_tBtnList.begin(); it != m_tBtnList.end(); ++it) {
		if (*it) { delete *it; *it = 0; }
	}
	m_tBtnList.clear();

	m_pressed = 0;
	m_underCursor = 0;
	m_selected = 0;
}

// frtabbutton.cpp:104
void FrTabButton::AddTab(const char* imgName, unsigned long index, WSize* size) {
	TButtonItem* tab = new TButtonItem;

	FrElementDoc* pDoc = WndManager()->GetDocument();
	if (pDoc) {
		char* ext[3] = { "n", "o", "d" };
		char buff[32];
		for (int i = 0; i < 3; ++i) {
			sprintf(buff, "%s_%s", imgName, ext[i]);
			tab->bitmap[i] = pDoc->GetBitmap(buff);
		}
	}
	InitButtonItem(tab, index, size);
}

// frtabbutton.cpp:123
void FrTabButton::AddTab(const char** const imgNames, unsigned long index, WSize* size) {
	TButtonItem* tab = new TButtonItem;

	FrElementDoc* pDoc = WndManager()->GetDocument();
	if (pDoc) {
		for (int i = 0; i < 3; ++i)
			tab->bitmap[i] = pDoc->GetBitmap(imgNames[i]);
	}

	InitButtonItem(tab, index, size);
}

// frtabbutton.cpp:139
void FrTabButton::Select(unsigned long index) {
	for (TBUTTON_LIST::iterator it = m_tBtnList.begin(); it != m_tBtnList.end(); ++it) {
		if ((*it)->index == index) {
			Select(*it);
			return;
		}
	}
}

// frtabbutton.cpp:151
void FrTabButton::UnSelect(unsigned long index) {
	for (TBUTTON_LIST::iterator it = m_tBtnList.begin(); it != m_tBtnList.end(); ++it) {
		if ((*it)->index == index) {
			UnSelect(*it);
			return;
		}
	}
}

// frtabbutton.cpp:163
void FrTabButton::InitButtonItem(TButtonItem* tab, unsigned long index, WSize* size) {
	tab->index = index;
	tab->selected = false;
	tab->underCursor = false;

	tab->rect.x = m_rect.x + m_rect.w;
	if (!m_tBtnList.empty())
		tab->rect.x += m_sepWidth;

	tab->rect.y = m_rect.y;

	if (size) {
		tab->rect.w = size->w;
		tab->rect.h = size->h;
	} else {
		tab->rect.w = (float)tab->bitmap[0]->Width();
		tab->rect.h = (float)tab->bitmap[0]->Height();
	}

	m_rect.w += tab->rect.w;
	if (m_tBtnList.empty())
		m_rect.h = tab->rect.h;
	else
		m_rect.w += m_sepWidth;

	m_tBtnList.push_back(tab);
}

// frtabbutton.cpp:198
void FrTabButton::Select(TButtonItem* pItem) {
	if (m_selected)
		m_selected->selected = false;

	m_selected = pItem;

	if (m_selected)
		m_selected->selected = true;
}

// frtabbutton.cpp:210
void FrTabButton::UnSelect(TButtonItem* pItem) {
	if (pItem)
		pItem->selected = false;

	if (m_selected == pItem)
		m_selected = 0;
}

// frtabbutton.cpp:220
void FrTabButton::OnDraw(void) {
	FrWnd::OnDraw();

	SendCmdToOwnerTarget(FRCMD_OWNERDRAW, 0, 0);

	float x = 0.0f;
	for (TBUTTON_LIST::iterator it = m_tBtnList.begin(); it != m_tBtnList.end(); ++it) {
		int index = 0;
		if ((*it)->selected) {
			if ((*it)->bitmap[2])
				index = 2;
		} else if ((*it)->underCursor) {
			if ((*it)->bitmap[1])
				index = 1;
		}

		(*it)->rect.x = m_rect.x + x;
		(*it)->rect.y = m_rect.y;

		if (it != m_tBtnList.begin()) {
			if (m_sepImg)
				GDI()->DrawTexture(m_sepImg, WRect((*it)->rect.x - m_sepWidth, (*it)->rect.y, m_sepWidth, (float)m_sepImg->Height()),
					FrALPHA(0xffffffff, m_wndAlpha2 * m_wndAlpha), 0);
		}

		if (m_pBtnBgImg[index])
			GDI()->DrawTexture(m_pBtnBgImg[index], (*it)->rect, FrALPHA(0xffffffff, m_wndAlpha2 * m_wndAlpha), 0);

		GDI()->DrawTexture((*it)->bitmap[index], (*it)->rect, FrALPHA(0xffffffff, m_wndAlpha2 * m_wndAlpha), 0);

		x += (*it)->rect.w + m_sepWidth;

		SendCmdToOwnerTarget(FRCMD_OWNERDRAW, (intptr_t)*it, 0);
	}
}

// frtabbutton.cpp:266
void FrTabButton::OnProc(const float deltaTime) {
}

// frtabbutton.cpp:271
void FrTabButton::OnMouseMove(const WPoint& mousePos) {
	m_underCursor = 0;

	for (TBUTTON_LIST::iterator it = m_tBtnList.begin(); it != m_tBtnList.end(); ++it) {
		TButtonItem* tab = *it;

		if (mousePos.x >= tab->rect.x && tab->rect.x + tab->rect.w >= mousePos.x &&
			mousePos.y >= tab->rect.y && tab->rect.y + tab->rect.h >= mousePos.y) {
			m_underCursor = tab;
			tab->underCursor = true;
		} else {
			tab->underCursor = false;
		}
	}
}

// frtabbutton.cpp:292
void FrTabButton::OnSetCursor(bool bInClient, const WPoint& mousePos) {
	if (bInClient) {
		if (m_underCursor)
			SetCursor(1);
	} else {
		if (m_underCursor) {
			m_underCursor->underCursor = false;
			m_underCursor = 0;
		}
	}
}

// frtabbutton.cpp:311
bool FrTabButton::OnLButtonDown(const WPoint& mousePos) {
	OnMouseMove(mousePos);

	if (m_underCursor) {
		if (m_btnType == BT_DOWN)
			Select(m_underCursor);

		if (m_btnType == BT_UP)
			m_pressed = m_underCursor;

		PlayPushSound();

		SendCmdToOwnerTarget(FRCMD_LBUTTONDOWN, 0, 0);
	}
	return false;
}

// frtabbutton.cpp:330
bool FrTabButton::OnLButtonUp(const WPoint& mousePos) {
	if (m_btnType == BT_UP) {
		OnMouseMove(mousePos);
		if (m_underCursor == m_pressed) {
			Select(m_underCursor);
			SendCmdToOwnerTarget(FRCMD_LBUTTONUP, 0, 0);
		}

		m_pressed = 0;
	}
	return false;
}
