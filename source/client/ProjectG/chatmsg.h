// Original: d:\build\custom\temp\code\client\projectg\chatmsg.h
#ifndef PG_CLIENT_PROJECTG_CHATMSG_H
#define PG_CLIENT_PROJECTG_CHATMSG_H

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "../client/Wangreal/include/wtypes.h"

class WFont;
class WOverlay;
struct tagCANDIDATELIST;

// The IME composition state GetCompositionString hands out: pointers into
// CChatMsg's buffers and two of its counters.
struct sCompStr {
	char *pCompStr;                     // 0x0, CChatMsg::m_compText
	char *pCompAttr;                    // 0x4, CChatMsg::m_compAttr
	char *pCompClause;                  // 0x8, CChatMsg::m_compClause
	int compCursor;                     // 0xC, CChatMsg::m_compCursor
	int compDeltaStart;                 // 0x10, CChatMsg::m_compDeltaStart
};
CHECK_SIZE(sCompStr, 0x14);

// The chat window singleton. Its layout is partly transcribed: only the
// members this project's code uses so far are declared.
class CChatMsg : public WSingleton<CChatMsg> {
public:
	CChatMsg(void);
	virtual ~CChatMsg(void);

	void Init(void);
	void Process(float elapsed);
	void SetActive(bool, bool, bool);
	void Open(bool, bool);
	void Close(bool, bool);
	void Reset(void);
	void AddChatMsg(const std::string& msg, unsigned long color, bool, bool);
	void SetChatText(const char* text, bool b);
	bool HasBeenPrinted(void);
	void ResetUiPosition(void);
	void ReplaceBackupUiAlpha(void);
	void PrintInScreen(void);
	void DrawChatWindow(void);
	unsigned long ConvertToUIColor(unsigned long color);
	int GetCurCaretPos(void) { return m_cursorPos; }                        // chatmsg.h:57
	void ResetCurCaretPos(void) { m_cursorPos = 0; m_cursorX = 0; }         // chatmsg.h:63
	void CurCaretMoveRight(void) { OnMoveRight(); }                         // chatmsg.h:65
	int GetNumBytes(void) { return m_inputLength; }                         // chatmsg.h:66
	void GetChatText(std::string& front, std::string& comp, std::string& end);
	void GetCompositionString(sCompStr *compStr);
	std::string GetIncomingChatMsg(void) const;
	bool IsOpened(void);                // chatmsg.h:75
	bool IsClosed(void);                // chatmsg.h:76
	bool IsOpenning(void) { return m_openState == 2 || m_openState == 1; }  // chatmsg.h:77
	int GetConsolKeyCode(void) const { return m_consolKeyCode; }            // chatmsg.h:91
	bool IsActive(void) const;          // chatmsg.h:92
	WFont *GetMaskedFont(void) const;   // chatmsg.h:94
	void EnableCaretMove(bool enable) { m_caretMoveEnabled = enable; }      // chatmsg.h:99
	void SetBufLen(int len) { m_bufLen = len; }                             // chatmsg.h:100
	void SetBufWidth(float width) { m_bufWidth = width; }                   // chatmsg.h:101
	void Toggle(bool on);               // chatmsg.h:102

	void ToggleInsertMode(void);
	void PrintInChatWindow(void);
	void PrintOnChatRoom(void);
	bool IME_IsCandWinOpened(void);
	bool IME_IsCompDone(void);
	bool IME_IsImeOpened(void);
	bool IME_IsCompInProgress(void);
	bool IME_IsAlphaNumericMode(void);
	void IME_SetAlphaNumericMode(void);
	void IME_SetOpenStatus(bool open);
	tagCANDIDATELIST *const IME_GetCandList(void);

	void RemoveColor(const std::string& src, std::string& dst);
	void ClearHistory(void);
	const char* MakeShortID(const char* id, float width);
	void SetAlwaysActive(void);
	void GetChatText_JP(std::string& front, std::string& comp, std::string& end, int);
	void ProcessWindow(float elapsed);
	void ProcessMacro(float elapsed);
	int GetOpenState(void) { return m_openState; }

protected:
	void InitFont(void);
	void ProcessResultString(void);
	void ProcessResultString_JP(void);
	void ProcessCompString(void);
	void ProcessCompString_JP(void);
	void ProcessEditKey(float elapsed);
	void WriteCharacter(int ch);
	void OnMoveLeft(void);
	void OnMoveRight(void);
	void OnMoveEnd(void);
	void OnMoveHome(void);
	void DeleteCharacter(void);
	void DeleteSelected(void);
	void OnDelete(void);
	void OnBackSpace(void);
	void CopySelected(void);
	void PasteCopied(void);
	void InsertWCharacter(int ch);

	bool m_printed;                     // 0x28, HasBeenPrinted's flag
	bool m_caretMoveEnabled;            // 0x29, EnableCaretMove; gates OnMoveLeft/Right
	PADDING(0x2A, 0x2C);
	int m_bufLen;                       // 0x2C, SetBufLen: input byte limit
	float m_bufWidth;                   // 0x30, SetBufWidth: input pixel width limit
	WFont *m_pMaskedFont;               // 0x34
	WOverlay *m_pOverlays[3];           // 0x38, the window's pieces, drawn by DrawChatWindow
	_WRECT m_overlayDst[3];             // 0x44, their screen rectangles
	_WRECT m_overlaySrc[3];             // 0x74, their texture rectangles
	float m_openSpeed;                  // 0xA4, the opening slide's speed (ProcessWindow), 3000 at Init
	bool m_active;                      // 0xA8
	bool m_insertMode;                  // 0xA9, toggled by ToggleInsertMode
	bool m_editFlag;                    // 0xAA, cleared by Reset
	bool m_caretVisible;                // 0xAB, the caret's blink phase
	float m_caretBlinkTime;             // 0xAC, time in the blink phase
	int m_openState;                    // 0xB0, 0 closed, 1-2 opening, 3 opened
	int m_compChar;                     // 0xB4, the IME's composing character (0: none)
	int m_editState;                    // 0xB8, cleared by Reset
	int m_consolKeyCode;                // 0xBC
	char m_inputText[0x400];            // 0xC0, the input line
	char m_compText[0x400];             // 0x4C0, the IME composition string
	// Per input byte, kept parallel to m_inputText by every insertion and deletion.
	char m_inputTextAttr[0x400];        // 0x8C0
	// Per input character, its byte count minus one (0 single-byte, 1 double-byte).
	unsigned char m_charExtraBytes[0x400]; // 0xCC0
	// The input line: OnMoveEnd copies 0x10C0 -> 0x10C4 and 0x10C8 -> 0x10CC,
	// OnMoveHome zeroes 0x10C4 and 0x10CC (names inferred from that).
	int m_inputLength;                  // 0x10C0
	int m_cursorPos;                    // 0x10C4
	int m_inputWidth;                   // 0x10C8
	int m_cursorX;                      // 0x10CC
	// The selection's two ends, in either order (CopySelected copies from the smaller).
	int m_selBeginPos;                  // 0x10D0, byte index
	int m_selEndPos;                    // 0x10D4, byte index
	int m_selectedLength;               // 0x10D8, nonzero while a selection exists
	int m_selBeginX;                    // 0x10DC, character index of m_selBeginPos
	int m_selEndX;                      // 0x10E0, character index of m_selEndPos
	int m_selectedWidth;                // 0x10E4, the selection's character count
	// The clipboard CopySelected fills and PasteCopied inserts.
	char m_copyText[0x400];             // 0x10E8
	char m_copyTextAttr[0x400];         // 0x14E8
	unsigned char m_copyCharExtraBytes[0x400]; // 0x18E8
	int m_copyLength;                   // 0x1CE8, bytes in m_copyText
	int m_copyWidth;                    // 0x1CEC, characters in m_copyText
	unsigned long m_lastPrintTime;      // 0x1CF0, g_CurrentTime of PrintInScreen's last notice
	unsigned char m_backupUiAlpha;      // 0x1CF4, ReplaceBackupUiAlpha
	PADDING(0x1CF5, 0x1CF6);
	bool m_toggle;                      // 0x1CF6
	PADDING(0x1CF7, 0x1CF8);
	std::string m_chatFacialUser;       // 0x1CF8, SetChatFacialUser (chatmsg.h:62)
	// IME composition details, g_ime->GetState(3, -3 .. -6) (ProcessCompString_JP).
	char m_compAttr[0x400];             // 0x1D14
	char m_compClause[0x400];           // 0x2114
	PADDING(0x2514, 0x2518);
	int m_compCursor;                   // 0x2518
	int m_compDeltaStart;               // 0x251C

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(CChatMsg)
	CHECK_FIELD(CChatMsg, m_printed, 0x28);
	CHECK_FIELD(CChatMsg, m_caretMoveEnabled, 0x29);
	CHECK_FIELD(CChatMsg, m_bufLen, 0x2C);
	CHECK_FIELD(CChatMsg, m_bufWidth, 0x30);
	CHECK_FIELD(CChatMsg, m_pMaskedFont, 0x34);
	CHECK_FIELD(CChatMsg, m_pOverlays, 0x38);
	CHECK_FIELD(CChatMsg, m_overlayDst, 0x44);
	CHECK_FIELD(CChatMsg, m_overlaySrc, 0x74);
	CHECK_FIELD(CChatMsg, m_openSpeed, 0xA4);
	CHECK_FIELD(CChatMsg, m_active, 0xA8);
	CHECK_FIELD(CChatMsg, m_insertMode, 0xA9);
	CHECK_FIELD(CChatMsg, m_editFlag, 0xAA);
	CHECK_FIELD(CChatMsg, m_caretVisible, 0xAB);
	CHECK_FIELD(CChatMsg, m_caretBlinkTime, 0xAC);
	CHECK_FIELD(CChatMsg, m_openState, 0xB0);
	CHECK_FIELD(CChatMsg, m_compChar, 0xB4);
	CHECK_FIELD(CChatMsg, m_editState, 0xB8);
	CHECK_FIELD(CChatMsg, m_consolKeyCode, 0xBC);
	CHECK_FIELD(CChatMsg, m_inputText, 0xC0);
	CHECK_FIELD(CChatMsg, m_compText, 0x4C0);
	CHECK_FIELD(CChatMsg, m_inputTextAttr, 0x8C0);
	CHECK_FIELD(CChatMsg, m_charExtraBytes, 0xCC0);
	CHECK_FIELD(CChatMsg, m_inputLength, 0x10C0);
	CHECK_FIELD(CChatMsg, m_cursorPos, 0x10C4);
	CHECK_FIELD(CChatMsg, m_inputWidth, 0x10C8);
	CHECK_FIELD(CChatMsg, m_cursorX, 0x10CC);
	CHECK_FIELD(CChatMsg, m_selBeginPos, 0x10D0);
	CHECK_FIELD(CChatMsg, m_selEndPos, 0x10D4);
	CHECK_FIELD(CChatMsg, m_selectedLength, 0x10D8);
	CHECK_FIELD(CChatMsg, m_selBeginX, 0x10DC);
	CHECK_FIELD(CChatMsg, m_selEndX, 0x10E0);
	CHECK_FIELD(CChatMsg, m_selectedWidth, 0x10E4);
	CHECK_FIELD(CChatMsg, m_copyText, 0x10E8);
	CHECK_FIELD(CChatMsg, m_copyTextAttr, 0x14E8);
	CHECK_FIELD(CChatMsg, m_copyCharExtraBytes, 0x18E8);
	CHECK_FIELD(CChatMsg, m_copyLength, 0x1CE8);
	CHECK_FIELD(CChatMsg, m_copyWidth, 0x1CEC);
	CHECK_FIELD(CChatMsg, m_lastPrintTime, 0x1CF0);
	CHECK_FIELD(CChatMsg, m_backupUiAlpha, 0x1CF4);
	CHECK_FIELD(CChatMsg, m_toggle, 0x1CF6);
	CHECK_FIELD(CChatMsg, m_chatFacialUser, 0x1CF8);
	CHECK_FIELD(CChatMsg, m_compAttr, 0x1D14);
	CHECK_FIELD(CChatMsg, m_compClause, 0x2114);
	CHECK_FIELD(CChatMsg, m_compCursor, 0x2518);
	CHECK_FIELD(CChatMsg, m_compDeltaStart, 0x251C);
END_LAYOUT_CHECK

// Defined empty, before its first include, by GolfRule.cpp: the original kept
// the out-of-line copy of these inline accessors in GolfRule's object; every
// other unit inlines them.
#ifndef GOLFRULE_CHATMSG_INLINE
#define GOLFRULE_CHATMSG_INLINE inline
#endif

// chatmsg.h:75
GOLFRULE_CHATMSG_INLINE bool CChatMsg::IsOpened(void) {
	return m_openState == 3;
}

// chatmsg.h:76
GOLFRULE_CHATMSG_INLINE bool CChatMsg::IsClosed(void) {
	return m_openState == 0;
}

// chatmsg.h:92
GOLFRULE_CHATMSG_INLINE bool CChatMsg::IsActive(void) const {
	return m_active;
}

// chatmsg.h:94
GOLFRULE_CHATMSG_INLINE WFont *CChatMsg::GetMaskedFont(void) const {
	return m_pMaskedFont;
}

// chatmsg.h:102
GOLFRULE_CHATMSG_INLINE void CChatMsg::Toggle(bool on) {
	m_toggle = on;
}

#endif // PG_CLIENT_PROJECTG_CHATMSG_H
