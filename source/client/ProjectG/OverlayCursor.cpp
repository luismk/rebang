// Original: d:\build\custom\temp\code\client\projectg\overlaycursor.cpp
#include "OverlayCursor.h"
#include "fresh.h"
#include "frwnd.h"
#include "frwndmanager.h"
#include "mousecursor.h"
#include "woverlay.h"
#include "wresrcmng.h"
#include "wview.h"
#include <string.h>

// overlaycursor.cpp:11
COverlayCursor::COverlayCursor(void) {
	m_cursorType = CURSOR_NONE;
	m_ppWnd = 0;
}

// overlaycursor.cpp:17
COverlayCursor::~COverlayCursor(void) {
	std::map<int, sOCItem>::iterator it;
	for (it = m_items.begin(); it != m_items.end(); ++it) {
		WOverlay* pOverlay = it->second.pOverlay;
		if (g_resrcmng && pOverlay) {
			g_resrcmng->Release(pOverlay);
		}
	}
}

// overlaycursor.cpp:30
void COverlayCursor::Add(eCursorType type, const char* fileName, int hotSpotX, int hotSpotY) {
	if (!fileName) {
		return;
	}
	if (!strlen(fileName)) {
		return;
	}
	WOverlay* pOverlay = g_resrcmng->GetOverlay(fileName, 0);
	if (!pOverlay) {
		return;
	}
	sOCItem item;
	item.type = type;
	item.pOverlay = pOverlay;
	item.hotSpotX = hotSpotX;
	item.hotSpotY = hotSpotY;
	m_items.insert(std::map<int, sOCItem>::value_type(type, item));
}

// overlaycursor.cpp:52
void COverlayCursor::Render(void) {
	const WPoint& pt = g_pFresh->GetManager()->GetMousePos();
	if (m_cursorType == CURSOR_NONE) {
		CMouseCursor::Instance()->ForceMove(pt.x, pt.y, -1.0f);
		CMouseCursor::Instance()->Toggle(true);
		return;
	}
	std::map<int, sOCItem>::iterator it = m_items.find(m_cursorType);
	if (it == m_items.end()) {
		CMouseCursor::Instance()->Toggle(true);
		return;
	}
	if (m_ppWnd && *m_ppWnd) {
		WRect rc = (*m_ppWnd)->GetRect();
		if (!(rc.x <= pt.x && pt.x <= rc.x + rc.w && rc.y <= pt.y && rc.y + rc.h >= pt.y)) {
			CMouseCursor::Instance()->ForceMove(pt.x, pt.y, -1.0f);
			CMouseCursor::Instance()->Toggle(true);
			return;
		}
	}
	bool focused = (*m_ppWnd)->IsViewFocused();
	if (!focused) {
		CMouseCursor::Instance()->Toggle(true);
		return;
	}
	CMouseCursor::Instance()->Toggle(false);
	WOverlay* pOverlay = it->second.pOverlay;
	WRect dst(pt.x - it->second.hotSpotX, pt.y - it->second.hotSpotY, (float)pOverlay->GetWidth(), (float)pOverlay->GetHeight());
	pOverlay->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), dst, 0, 0xffffffff, 0.0f, 0);
}
