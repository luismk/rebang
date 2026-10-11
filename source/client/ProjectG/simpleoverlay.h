// Original: d:\build\custom\temp\code\client\projectg\simpleoverlay.h
#ifndef PG_CLIENT_PROJECTG_SIMPLEOVERLAY_H
#define PG_CLIENT_PROJECTG_SIMPLEOVERLAY_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "../client/Wangreal/include/wlist.h"

class WOverlay;
class WRect;

// Overlay drawn in named pieces (simpleoverlay.cpp): each piece is a source
// rectangle of the overlay, stored normalised by the texture size.
class CSimpleUI {
	LAYOUT_CHECK_FRIEND;
public:
	CSimpleUI(const char* name);
	~CSimpleUI(void);

	void Add(const char* name, const WRect& rect, bool b);
	void Render(const char* name, float x, float y, float alpha, int i);
	void DeleteAllItem(void);

	// simpleoverlay.h:14 (the original's copy is in GolfRule)
	void SetColor(int color) { m_color = color; }
	// simpleoverlay.h:15 (the original's copy is in gui_control)
	void SetAngle(float angle) { m_angle = angle; }

private:
	WOverlay* m_pOverlay;          // 0x0
	float m_width;                 // 0x4, texture width of m_pOverlay
	float m_height;                // 0x8, texture height of m_pOverlay
	unsigned long m_color;         // 0xC
	float m_angle;                 // 0x10, rotation for RenderWithAxis
	WList<WRect*> m_rectList;      // 0x14, the pieces by name
};

BEGIN_LAYOUT_CHECK(CSimpleUI)
	CHECK_SIZE(CSimpleUI, 0x34);
	CHECK_FIELD(CSimpleUI, m_pOverlay, 0x0);
	CHECK_FIELD(CSimpleUI, m_width, 0x4);
	CHECK_FIELD(CSimpleUI, m_height, 0x8);
	CHECK_FIELD(CSimpleUI, m_color, 0xC);
	CHECK_FIELD(CSimpleUI, m_angle, 0x10);
	CHECK_FIELD(CSimpleUI, m_rectList, 0x14);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SIMPLEOVERLAY_H
