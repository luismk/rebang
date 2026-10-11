// Original: d:\build\custom\temp\code\client\projectg\onelineboard.h
#ifndef PG_CLIENT_PROJECTG_ONELINEBOARD_H
#define PG_CLIENT_PROJECTG_ONELINEBOARD_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "../client/Wangreal/include/wtypes.h"

#include <list>
#include <string>

class Bitmap;

// The one-line message board of the lobby's over bar. Only the members this
// project's code uses so far are declared.
class COneLineBoard : public WSingleton<COneLineBoard> {
	LAYOUT_CHECK_FRIEND;

public:
	// One scrolling message (size 0x28, the `new` of AddOnelineMsg).
	struct sOnelineMsg {
		std::string text;       // 0x0
		float width;            // 0x1C
		float x;                // 0x20
		unsigned long time;     // 0x24
	};

	COneLineBoard(void);
	virtual ~COneLineBoard(void);

	void Clear(void);
	void Process(float elapsed);
	void AddOnelineMsg(const std::string &name, const std::string &message);
	void LoadBg(void);
	void FadeOut(void);
	void FadeIn(void);
	float GetFadeFactor(void);

	void SetRect(const WRect &rect);           // onelineboard.h:14
	void SetMessage(const char *message);      // onelineboard.h:15
	std::string &GetMessageA(void);            // onelineboard.h:16, GetMessage renamed by <windows.h>
	void SetWaitMsgNum(int num);               // onelineboard.h:17
	void SetWaitMsgTime(int time);             // onelineboard.h:18
	void Display(void);
	void ShowBg(bool show);                    // onelineboard.h:22

protected:
	void DrawBg(void);

private:
	typedef std::list<sOnelineMsg*> OnelineMsgList;

	bool m_showBg;                                  // 0x28
	int m_fadeState;                                // 0x2C, 0 none, 1 fading out, 2 fading in
	float m_fadeFactor;                             // 0x30
	bool m_visible;                                 // 0x34
	OnelineMsgList::iterator m_iter;                // 0x38
	std::reverse_iterator<OnelineMsgList::iterator> m_riter; // 0x3C
	OnelineMsgList m_msgList;                       // 0x40
	WRect m_rect;                                   // 0x4C
	std::string m_message;                          // 0x5C
	int m_waitMsgNum;                               // 0x78
	int m_waitMsgTime;                              // 0x7C
	UNKNOWN(0x80, 0x84);
	const Bitmap *m_bg;                             // 0x84
};
CHECK_SIZE(COneLineBoard::sOnelineMsg, 0x28);
BEGIN_LAYOUT_CHECK(COneLineBoard)
	CHECK_FIELD(COneLineBoard, m_showBg, 0x28);
	CHECK_FIELD(COneLineBoard, m_fadeState, 0x2C);
	CHECK_FIELD(COneLineBoard, m_fadeFactor, 0x30);
	CHECK_FIELD(COneLineBoard, m_visible, 0x34);
	CHECK_FIELD(COneLineBoard, m_iter, 0x38);
	CHECK_FIELD(COneLineBoard, m_riter, 0x3C);
	CHECK_FIELD(COneLineBoard, m_msgList, 0x40);
	CHECK_FIELD(COneLineBoard, m_rect, 0x4C);
	CHECK_FIELD(COneLineBoard, m_message, 0x5C);
	CHECK_FIELD(COneLineBoard, m_waitMsgNum, 0x78);
	CHECK_FIELD(COneLineBoard, m_waitMsgTime, 0x7C);
	CHECK_FIELD(COneLineBoard, m_bg, 0x84);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_ONELINEBOARD_H
