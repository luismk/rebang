// Original: d:\build\custom\temp\code\client\projectg\talkbox.h
#ifndef PG_CLIENT_PROJECTG_TALKBOX_H
#define PG_CLIENT_PROJECTG_TALKBOX_H

#include <list>

#include "..\..\defines.h"
#include "..\..\padding.h"

class WView;
class WFont;
class WOverlay;

// A speech balloon. Its members are transcribed only as far as the functions
// decompiled so far need them.
class CTalkBox {
	LAYOUT_CHECK_FRIEND;
public:
	CTalkBox(void);
	~CTalkBox(void);

	void Process(float fDelta);
	void Render(WView* pView, float x, float y, int align);
	void SetText(const char* text, float showTime);

	bool IsActive(void);                    // talkbox.h:15
	void SetTransparency(float alpha);      // talkbox.h:19

	// One line of the balloon (size 0x2C from SetText's local and Render).
	struct sLine {
		char text[32];          // 0x0
		float width;            // 0x20
		unsigned long color;    // 0x24
		int lineBreak;          // 0x28, the next line starts below this one
	};

protected:
	WFont* m_pFont;             // 0x0
	WOverlay* m_pOverlay;       // 0x4
	std::list<sLine> m_lines;   // 0x8
	int m_lineCount;            // 0x14
	float m_transparency;       // 0x18
	float m_remainTime;         // 0x1c
	float m_boxX;               // 0x20
	float m_boxY;               // 0x24
	char m_text[0x200];         // 0x28
	unsigned char m_maxLineLength; // 0x228
	PADDING(0x229, 0x22C);
};

CHECK_SIZE(CTalkBox::sLine, 0x2C);
CHECK_SIZE(CTalkBox, 0x22C);
BEGIN_LAYOUT_CHECK(CTalkBox)
	CHECK_FIELD(CTalkBox, m_pFont, 0x0);
	CHECK_FIELD(CTalkBox, m_pOverlay, 0x4);
	CHECK_FIELD(CTalkBox, m_lines, 0x8);
	CHECK_FIELD(CTalkBox, m_lineCount, 0x14);
	CHECK_FIELD(CTalkBox, m_transparency, 0x18);
	CHECK_FIELD(CTalkBox, m_remainTime, 0x1c);
	CHECK_FIELD(CTalkBox, m_boxX, 0x20);
	CHECK_FIELD(CTalkBox, m_boxY, 0x24);
	CHECK_FIELD(CTalkBox, m_text, 0x28);
	CHECK_FIELD(CTalkBox, m_maxLineLength, 0x228);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_TALKBOX_H
