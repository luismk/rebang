// Original: d:\build\custom\temp\code\client\projectg\overlaycursor.h
#ifndef PG_CLIENT_PROJECTG_OVERLAYCURSOR_H
#define PG_CLIENT_PROJECTG_OVERLAYCURSOR_H

#include <map>

#include "..\..\defines.h"
#include "..\..\padding.h"

class FrWnd;
class WOverlay;

// A cursor registered with COverlayCursor::Add (size 0x10; global, as the
// map's decorated names show).
struct sOCItem {
	sOCItem(void)      // overlaycursor.h:13
	{
		type = 0;
		pOverlay = 0;
		hotSpotX = 0;
		hotSpotY = 0;
	}

	int type;          // 0x0, the COverlayCursor::eCursorType it was added for
	WOverlay* pOverlay;  // 0x4
	int hotSpotX;      // 0x8
	int hotSpotY;      // 0xc
};

CHECK_SIZE(sOCItem, 0x10);

// Cursor image drawn over a window (the UCC canvas), chosen among the items
// registered with Add. Size 0x14 (the constructor initialises every member).
class COverlayCursor {
	LAYOUT_CHECK_FRIEND;
public:
	// Cursor kind; only the "no cursor" value is recovered (Render falls
	// back to the system cursor on it).
	enum eCursorType {
		CURSOR_NONE = 0
	};

	COverlayCursor(void);
	~COverlayCursor(void);

	void Render(void);
	void Add(eCursorType type, const char* fileName, int hotSpotX, int hotSpotY);
	void SetRect(FrWnd** ppWnd);              // overlaycursor.h:44
	void SetCursor(eCursorType type);         // overlaycursor.h:47

private:
	std::map<int, sOCItem> m_items;  // 0x0, the registered cursors by type
	FrWnd** m_ppWnd;             // 0xc, the window the cursor is drawn over
	eCursorType m_cursorType;    // 0x10
};

CHECK_SIZE(COverlayCursor, 0x14);
BEGIN_LAYOUT_CHECK(COverlayCursor)
	CHECK_FIELD(COverlayCursor, m_items, 0x0);
	CHECK_FIELD(COverlayCursor, m_ppWnd, 0xc);
	CHECK_FIELD(COverlayCursor, m_cursorType, 0x10);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_OVERLAYCURSOR_H
