// Original: d:\build\custom\temp\code\client\fresh\include\frcursor.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRCURSOR_H
#define PG_CLIENT_FRESH_INCLUDE_FRCURSOR_H

#include "..\..\..\defines.h"

class WPoint;

class FrCursor {
public:
	enum eCursor {
		UNSELECT = 0,
		NORMAL = 0,
		ROLL_OVER = 1,
		SELECT = 2,
		WRONG = 3,
		UP = 4,
		DOWN = 5,
		ZOOM = 6,
		INFO = 7,
	};

	FrCursor();
	virtual ~FrCursor();    // vtable slot 0 (??_G)

	void Init();
	void SetCursor(eCursor cursor);
	int GetCursor();
	void MoveCursor(const WPoint& pt);
	void ShowCursor(bool show);

protected:
	int m_cursorIndex;      // 0x4
	bool m_bShow;           // 0x8

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(FrCursor, 0xc);
BEGIN_LAYOUT_CHECK(FrCursor)
	CHECK_FIELD(FrCursor, m_cursorIndex, 0x4);
	CHECK_FIELD(FrCursor, m_bShow, 0x8);
END_LAYOUT_CHECK

#endif
