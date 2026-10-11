// Original: d:\build\custom\temp\code\client\projectg\simpleoverlay.cpp
#include "SimpleOverlay.h"
#include "../client/Wangreal/include/wtypes.h"
#include "../client/Wangreal/include/wmath.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wview.h"

// simpleoverlay.cpp:15
CSimpleUI::CSimpleUI(const char* name)
	: m_pOverlay(NULL), m_rectList(8, 8) {
	m_pOverlay = g_resrcmng->GetOverlay(name, 0);
	if (m_pOverlay != NULL) {

		m_width = (float)m_pOverlay->GetWidth();
		m_height = (float)m_pOverlay->GetHeight();
	}

	m_angle = 0.0f;
	m_color = 0xffffffff;
}

// simpleoverlay.cpp:70
CSimpleUI::~CSimpleUI(void) {
	if (g_resrcmng != NULL && m_pOverlay != NULL) { g_resrcmng->Release(m_pOverlay); m_pOverlay = NULL; }

	DeleteAllItem();
}

// simpleoverlay.cpp:83
void CSimpleUI::Add(const char* name, const WRect& rect, bool b) {
	WRect* piece;

	piece = new WRect;
	piece->x = rect.x / m_width; piece->y = rect.y / m_height; piece->w = rect.w / m_width; piece->h = rect.h / m_height;

	m_rectList.AddItem(piece, name, b);
}

// simpleoverlay.cpp:98
void CSimpleUI::Render(const char* name, float x, float y, float alpha, int i) {
	if ((m_color & 0xff000000) == 0 || name == NULL) return;

	WRect* piece = m_rectList.Find(name);

	if (piece == NULL) return;

	WRect dst;

	dst.w = piece->w * m_width * alpha;

	switch (i & 6) {
	case 0:

		dst.x = x;
		break;

	case 4:
		dst.x = x - dst.w;
		break;

	case 2:
		dst.x = x - dst.w * 0.5f;
		break;
	}

	dst.h = piece->h * m_height * alpha;
	switch (i & 0x60) {
	case 0:

		dst.y = y;
		break;

	case 0x40:
		dst.y = y - dst.h;
		break;

	case 0x20:
		dst.y = y - dst.h * 0.5f;
		break;
	}

	if (WAbsBySign(m_angle) < WMATH_EPSILON) {

		if (m_pOverlay != NULL)
			m_pOverlay->Render(g_view, *piece, dst, 0, m_color, 0, 0);
	}
	else {

		if (m_pOverlay != NULL)
			m_pOverlay->RenderWithAxis(g_view, *piece, dst, dst.x + dst.w * 0.5f, dst.y + dst.h * 0.5f, m_angle, 0, m_color, 0);
	}
}

// simpleoverlay.cpp:166
void CSimpleUI::DeleteAllItem(void) {
	for (WRect* piece = m_rectList.Start(); piece != NULL; piece = m_rectList.Next()) {
		if (piece != NULL) {

			m_rectList -= piece;
			delete piece;
		}
	}

	m_rectList.Reset();
}
