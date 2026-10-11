// Original: d:\build\custom\temp\code\client\projectg\helpmsg.h
#ifndef PG_CLIENT_PROJECTG_HELPMSG_H
#define PG_CLIENT_PROJECTG_HELPMSG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "../client/Wangreal/include/wtypes.h"

class WOverlay;
class WFont;
struct sItemInfo;

// The in-game help balloon: a title and a coloured text drawn above the
// power bar, chosen from a table of 29 help entries (title, text, key) or set
// from an item or the emoticon commands.
class CHelpMsg : public WSingleton<CHelpMsg> {
public:
	CHelpMsg(void);
	virtual ~CHelpMsg(void);

	void Process(float elapsed);
	void Display(void);
	void ResetUiPosition(void);
	void SetHelpMsg(int index, char *text);
	void SetItemHelpMsg(unsigned long typeId);
	void SetEmoticonHelpMsg(char **commands);
	void OnChangeTurn(void);
	void CheckReplayAvailable(void);

private:
	int GetLine(int *width, int *length, unsigned long *color, char *text, bool continued);
	void CalcWindowSize(char *title, char *text);

	LAYOUT_CHECK_FRIEND;

	WOverlay *m_pOverlay;               // 0x28, "[help.jpg"
	char *(*m_helpTexts)[3];            // 0x2C, 29 entries of three strings each
	int m_helpIndex;                    // 0x30, -1 none, 15 the emoticon commands
	WFont *m_pFont;                     // 0x34, the chat window's masked font
	WRect m_texRect[3];                 // 0x38, help.jpg's frame, title and text parts (texture coordinates)
	WRect m_frameRect;                  // 0x68
	WRect m_titleRect;                  // 0x78
	WRect m_textRect;                   // 0x88
	unsigned long m_itemTypeId;         // 0x98, SetItemHelpMsg's item
	char m_title[0x20];                 // 0x9C
	char m_text[0x100];                 // 0xBC
	char m_keyText[0x10];               // 0x1BC
	char m_itemText[0x40];              // 0x1CC
	float m_showTime;                   // 0x20C, seconds a timed help (SetHelpMsg resets it) has been shown
	bool m_itemChanged;                 // 0x210
	sItemInfo *m_pReplayItem;           // 0x214, the replay item (type 0x1A00004F) when owned
};
CHECK_SIZE(CHelpMsg, 0x218);
BEGIN_LAYOUT_CHECK(CHelpMsg)
	CHECK_FIELD(CHelpMsg, m_pOverlay, 0x28);
	CHECK_FIELD(CHelpMsg, m_helpTexts, 0x2C);
	CHECK_FIELD(CHelpMsg, m_helpIndex, 0x30);
	CHECK_FIELD(CHelpMsg, m_pFont, 0x34);
	CHECK_FIELD(CHelpMsg, m_texRect, 0x38);
	CHECK_FIELD(CHelpMsg, m_frameRect, 0x68);
	CHECK_FIELD(CHelpMsg, m_titleRect, 0x78);
	CHECK_FIELD(CHelpMsg, m_textRect, 0x88);
	CHECK_FIELD(CHelpMsg, m_itemTypeId, 0x98);
	CHECK_FIELD(CHelpMsg, m_title, 0x9C);
	CHECK_FIELD(CHelpMsg, m_text, 0xBC);
	CHECK_FIELD(CHelpMsg, m_keyText, 0x1BC);
	CHECK_FIELD(CHelpMsg, m_itemText, 0x1CC);
	CHECK_FIELD(CHelpMsg, m_showTime, 0x20C);
	CHECK_FIELD(CHelpMsg, m_itemChanged, 0x210);
	CHECK_FIELD(CHelpMsg, m_pReplayItem, 0x214);
END_LAYOUT_CHECK

#endif
