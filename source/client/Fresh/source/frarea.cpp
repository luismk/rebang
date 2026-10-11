// Original: d:\build\custom\temp\code\client\fresh\source\frarea.cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "frarea.h"
#include "frelement.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "ObjectFactory.h"
#include "../client/Wangreal/include/bitmap.h"

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

// frarea.cpp:15 (run-time type and factory registration of FrArea)
const WRTTI FrArea::m_RTTI("FrArea", &FrWnd::m_RTTI);

IObject* FrAreaMakeInstance(void) {
	return new FrArea;
}

struct __sFrArea {
	__sFrArea(void) {
		ObjectFactory().AddObjectFunctor(FrAreaMakeInstance, "FrArea");
	}
};
static __sFrArea s_registerFrArea;

// frarea.cpp:18
FrArea::FrArea(void)
	: m_pBitmap(0) {
	m_center = false;
	m_stretch = false;
	m_mouseEvent = false;
	m_color = 0xffffffff;
	m_imgName = "";

	m_isClipping = false;
	m_rcClipArea = WRect(0.0f, 0.0f, 0, 0);
}

// frarea.cpp:32
FrArea::~FrArea(void) {
}

// frarea.cpp:36
void FrArea::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	FrElementDoc* pDoc = pManager->GetDocument();

	bool bVisible = true;

	std::map<std::string, std::string>& param = item.m_param;

	if (param.find("bgimg") != param.end()) {
		m_imgName = param["bgimg"];
		m_pBitmap = pDoc->GetBitmap(m_imgName);
	}

	if (param.find("center") != param.end())
		m_center = atoi(param["center"].c_str()) != 0;

	if (param.find("stretch") != param.end())
		m_stretch = atoi(param["stretch"].c_str()) != 0;

	if (param.find("mouse_event") != param.end())
		m_mouseEvent = atoi(param["mouse_event"].c_str()) != 0;

	if (param.find("color") != param.end())
		sscanf(param["color"].c_str(), "%x", &m_color);

	if (param.find("visible") != param.end())
		bVisible = atoi(param["visible"].c_str()) == 1;

	m_pItem = &item;

	WRect rect((float)item.m_rect.left, (float)item.m_rect.top,
		(short)(item.m_rect.right - item.m_rect.left),
		(short)(item.m_rect.bottom - item.m_rect.top));
	unsigned long style = (m_mouseEvent ? 0 : 0x40) | (bVisible ? 1 : 0);
	Create(item.m_caption.c_str(), item.m_name.c_str(), pManager, style, rect, pParent);

	if (param.find("enable_clip") != param.end())
		m_isClipping = atoi(param["enable_clip"].c_str()) == 1;

	m_rcClipArea = m_rect;
	if (param.find("clip_area") != param.end()) {
		int v[4];
		sscanf(param["clip_area"].c_str(), "%d %d %d %d", &v[0], &v[1], &v[2], &v[3]);

		m_rcClipArea.x = (float)v[0];
		m_rcClipArea.y = (float)v[1];
		m_rcClipArea.w = (float)(v[2] - v[0]);
		m_rcClipArea.h = (float)(v[3] - v[1]);
	}
}

// frarea.cpp:93
bool FrArea::SetBgImg(const char* name) {
	if (name == 0) {
		m_pBitmap = 0;
		return true;
	}

	if (strcmpi(m_imgName.c_str(), name) == 0)
		return true;

	m_imgName = name;

	FrElementDoc* pDoc = WndManager()->GetDocument();
	if (pDoc) {
		m_pBitmap = pDoc->GetBitmap(name);
		if (m_pBitmap)
			return true;
	}

	return false;
}

// frarea.cpp:117
bool FrArea::SetBgImg(const Bitmap* pBitmap) {
	if (pBitmap) {
		m_pBitmap = pBitmap;
		return true;
	}

	return false;
}

// frarea.cpp:130
void FrArea::OnDraw(void) {
	FrWnd::OnDraw();

	WRect src, dst;
	if (ClippingArea(src, dst)) {
		GDI()->DrawTexture(m_pBitmap, src, dst, FrALPHA(m_color, m_wndAlpha2 * m_wndAlpha), 0);
	}

	SendCmdToOwnerTarget(FRCMD_OWNERDRAW, 0, 0);
}

// frarea.cpp:148
void FrArea::OnSetCursor(bool b, const WPoint& pt) {
	if (b && m_mouseEvent)
		SetCursor(1);
}

// frarea.cpp:154
void FrArea::SetMouseEvent(bool mouseEvent) {
	m_mouseEvent = mouseEvent;

	if (m_mouseEvent)
		m_dwStyle.Disable(0x40);
	else
		m_dwStyle.Enable(0x40);
}

// frarea.cpp:164
bool FrArea::OnLButtonDown(const WPoint& pt) {
	SendCmdToOwnerTarget(FRCMD_LBUTTONDOWN, 0, 0);
	return false;
}

// frarea.cpp:170
bool FrArea::OnLButtonUp(const WPoint& pt) {
	SendCmdToOwnerTarget(FRCMD_LBUTTONUP, 0, 0);
	return false;
}

// frarea.cpp:177
bool FrArea::ClippingArea(WRect& src, WRect& dst) {
	if (!m_pBitmap)
		return false;

	WRect re_src(0, 0, m_pBitmap->Width(), m_pBitmap->Height());
	WRect re_dst(m_rect.x, m_rect.y, m_rect.w, m_rect.h);

	if (m_center) {
		re_dst.w = (float)m_pBitmap->Width();
		re_dst.h = (float)m_pBitmap->Height();
		re_dst.x += (int)(((m_rect.w - re_dst.w) + 1) * 0.5f);
		re_dst.y += (int)(((m_rect.h - re_dst.h) + 1) * 0.5f);
	} else if (!m_stretch) {
		re_dst.w = (float)m_pBitmap->Width();
		re_dst.h = (float)m_pBitmap->Height();
	}

	dst = re_dst;
	src = re_src;

	if (!m_isClipping)
		return true;

	if (m_rcClipArea.x <= dst.x &&
		(m_rcClipArea.x + m_rcClipArea.w) >= (dst.x + dst.w) &&
		m_rcClipArea.y <= dst.y &&
		(m_rcClipArea.y + m_rcClipArea.h) >= (dst.y + dst.h))
		return true;

	re_dst.x = max(dst.x, m_rcClipArea.x);
	re_dst.y = max(dst.y, m_rcClipArea.y);
	re_dst.w =
		max(0, min(dst.x + dst.w, m_rcClipArea.x + m_rcClipArea.w) - re_dst.x);
	re_dst.h =
		max(0, min(dst.y + dst.h, m_rcClipArea.y + m_rcClipArea.h) - re_dst.y);

	re_src.x = max(0, (m_rcClipArea.x - dst.x) / dst.w) * re_src.w;
	re_src.y = max(0, (m_rcClipArea.y - dst.y) / dst.h) * re_src.h;
	re_src.w = min(1, re_dst.w / dst.w) * re_src.w;
	re_src.h = min(1, re_dst.h / dst.h) * re_src.h;

	dst = re_dst;
	src = re_src;

	return true;
}
