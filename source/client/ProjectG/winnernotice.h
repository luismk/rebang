// Original: d:\build\custom\temp\code\client\projectg\winnernotice.h
#ifndef PG_CLIENT_PROJECTG_WINNERNOTICE_H
#define PG_CLIENT_PROJECTG_WINNERNOTICE_H

#include <list>
#include <string>
#include <string.h>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"

class TiXmlDocument;
class CGDEventNotice;
class WFont;
class WOverlay;

// The scrolling notice that announces event winners (lottery, item draws)
// in the lobby. Layout reconstructed from the code (/Zd unit).
class CWinnerNotice : public WSingleton<CWinnerNotice> {
	LAYOUT_CHECK_FRIEND;
public:
	enum EventTimeFlag {
		EVENT_TIME_DAY = 0,             // "%d month %d day"
		EVENT_TIME_HOUR = 1             // "%02dh%02dm"
	};

	// One coloured piece of the notice text.
	struct sNoticeMsg {
		unsigned long m_color;          // 0x0, text colour, 0xFFFFFFFF when unused
		int m_style;                    // 0x4, FrGraphicInterface text style
		std::string m_text;             // 0x8

		// winnernotice.h:86
		void initdata(void) {
			m_color = 0xFFFFFFFF;
			m_style = 0;
			m_text = "";
		}
	};

	// One queued notice.
	struct sWinnerNotice {
		sNoticeMsg m_msg[10];           // 0x0
		std::string m_userName;         // 0x168
		std::string m_itemName;         // 0x184
		std::string m_eventNum;         // 0x1a0
		std::string m_dayTime;          // 0x1bc
		int m_repeatCount;                     // 0x1d8, remaining passes, decremented by Process
		float m_width;                  // 0x1dc, text width of all messages
		unsigned long m_startTime;      // 0x1e0, timeGetTime() when queued
		int m_value1e4;                 // 0x1e4
		int m_value1e8;                 // 0x1e8
		int m_value1ec;                 // 0x1ec
		int m_value1f0;                 // 0x1f0
		int m_msgCount;                 // 0x1f4, used entries of m_msg

		// winnernotice.h:112
		void clear_data(void) {
			memset(m_msg, 0, sizeof(m_msg));
			for (int i = 0; i < 10; i++) {
				m_msg[i].initdata();
			}
			m_userName = "";
			m_itemName = "";
			m_eventNum = "";
			m_repeatCount = 1;
			m_width = 0;
			m_startTime = 0;
			m_value1e4 = 0;
			m_value1e8 = 0;
			m_value1ec = 0;
			m_value1f0 = 0;
			m_msgCount = 0;
		}
	};

	CWinnerNotice(void);
	virtual ~CWinnerNotice(void);

	bool GetColornText(const char* pszText, std::string& text, unsigned long& color);
	void Process(float elapsed);
	float GetTextTailPoint(int count);
	void Display(void);
	void AddWinnerNotice(int repeatCount);
	void SetWinnerData(int type, std::string data);

protected:
	void ParseXmlData(TiXmlDocument& doc, const char* xml);
	void GetEventNumfromXml(TiXmlDocument& doc, std::string& out, bool next);
	void GetDaynTimefromXml(TiXmlDocument& doc, std::string& out, EventTimeFlag flag, bool next);
	void GetItemCountfromXml(TiXmlDocument& doc, std::string& out);
	void NickNameEdit(char* name, bool mask, std::string& out);
	const char* GetItemNamefromXml(TiXmlDocument& doc);
	void GetUserNamefromXml(TiXmlDocument& doc, std::string& out);
	void WinnerNotice_Init(void);
	void WinnerNotice_Clear(void);

	WFont* m_pFont;                                     // 0x28, CChatMsg's masked font
	WOverlay* m_pOverlayLeft;                           // 0x2c, "notice_l.tga"
	WOverlay* m_pOverlayMid;                            // 0x30, "notice_m.tga"
	WOverlay* m_pOverlayRight;                          // 0x34, "notice_r.tga"
	std::list<sWinnerNotice*> m_noticeList;             // 0x38
	std::list<sWinnerNotice*>::iterator m_current;      // 0x44
	float m_posX;                                       // 0x48
	float m_posY;                                       // 0x4c
	CGDEventNotice* m_pEventNotice;                     // 0x50, "WinnerEventNotice" texts
	sWinnerNotice m_notice;                             // 0x54
	int m_state;                                        // 0x24c
};

CHECK_SIZE(CWinnerNotice::sNoticeMsg, 0x24);
CHECK_SIZE(CWinnerNotice::sWinnerNotice, 0x1F8);
CHECK_FIELD(CWinnerNotice::sWinnerNotice, m_userName, 0x168);
CHECK_FIELD(CWinnerNotice::sWinnerNotice, m_dayTime, 0x1BC);
CHECK_FIELD(CWinnerNotice::sWinnerNotice, m_repeatCount, 0x1D8);
CHECK_FIELD(CWinnerNotice::sWinnerNotice, m_msgCount, 0x1F4);
BEGIN_LAYOUT_CHECK(CWinnerNotice)
	CHECK_FIELD(CWinnerNotice, m_pFont, 0x28);
	CHECK_FIELD(CWinnerNotice, m_pOverlayLeft, 0x2C);
	CHECK_FIELD(CWinnerNotice, m_pOverlayRight, 0x34);
	CHECK_FIELD(CWinnerNotice, m_noticeList, 0x38);
	CHECK_FIELD(CWinnerNotice, m_current, 0x44);
	CHECK_FIELD(CWinnerNotice, m_posX, 0x48);
	CHECK_FIELD(CWinnerNotice, m_pEventNotice, 0x50);
	CHECK_FIELD(CWinnerNotice, m_notice, 0x54);
	CHECK_FIELD(CWinnerNotice, m_state, 0x24C);
END_LAYOUT_CHECK
CHECK_SIZE(CWinnerNotice, 0x250);

#endif
