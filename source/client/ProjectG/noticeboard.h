// Original: d:\build\custom\temp\code\client\projectg\noticeboard.h
#ifndef PG_CLIENT_PROJECTG_NOTICEBOARD_H
#define PG_CLIENT_PROJECTG_NOTICEBOARD_H

#include <list>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"

class WFont;
class WOverlay;

// The scrolling system notice board shown at the top of the screen. Layout
// reconstructed from the code (/Zd unit).
class CNoticeBoard : public WSingleton<CNoticeBoard> {
	LAYOUT_CHECK_FRIEND;
public:
	// One queued notice.
	struct sNotice {
		std::string m_text;             // 0x0
		int m_repeatCount;              // 0x1c, remaining passes, decremented by Process
		float m_width;                  // 0x20, text width
		unsigned long m_color;          // 0x24
	};

	CNoticeBoard(void);
	virtual ~CNoticeBoard(void);

	bool GetColornText(const char* pszText, std::string& text, unsigned long& color);
	void Process(float elapsed);
	void Display(void);
	void AddNotice(const char* text, int repeatCount, bool immediately);

protected:
	WFont* m_pFont;                                 // 0x28, CChatMsg's masked font
	WOverlay* m_pOverlayLeft;                       // 0x2c, "notice_l.tga"
	WOverlay* m_pOverlayMid;                        // 0x30, "notice_m.tga"
	WOverlay* m_pOverlayRight;                      // 0x34, "notice_r.tga"
	std::list<sNotice*> m_noticeList;               // 0x38
	std::list<sNotice*>::iterator m_current;        // 0x44
	float m_posX;                                   // 0x48
	float m_posY;                                   // 0x4c
};

CHECK_SIZE(CNoticeBoard::sNotice, 0x28);
CHECK_FIELD(CNoticeBoard::sNotice, m_repeatCount, 0x1C);
CHECK_FIELD(CNoticeBoard::sNotice, m_width, 0x20);
CHECK_FIELD(CNoticeBoard::sNotice, m_color, 0x24);
BEGIN_LAYOUT_CHECK(CNoticeBoard)
	CHECK_FIELD(CNoticeBoard, m_pFont, 0x28);
	CHECK_FIELD(CNoticeBoard, m_pOverlayLeft, 0x2C);
	CHECK_FIELD(CNoticeBoard, m_pOverlayMid, 0x30);
	CHECK_FIELD(CNoticeBoard, m_pOverlayRight, 0x34);
	CHECK_FIELD(CNoticeBoard, m_noticeList, 0x38);
	CHECK_FIELD(CNoticeBoard, m_current, 0x44);
	CHECK_FIELD(CNoticeBoard, m_posX, 0x48);
	CHECK_FIELD(CNoticeBoard, m_posY, 0x4C);
END_LAYOUT_CHECK
CHECK_SIZE(CNoticeBoard, 0x50);

#endif
