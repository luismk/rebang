// Original: d:\build\custom\temp\code\client\fresh\include\fredit.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FREDIT_H
#define PG_CLIENT_FRESH_INCLUDE_FREDIT_H

#include <list>
#include <string>
#include "..\..\defines.h" 
#include "frwnd.h"

class FrGuiItem;

// One line of an edit window (PDB layout, size 0x28). The constructor is the
// inline one of fredit.h:27-29; the destructor is compiler-generated.
struct FrLine {
	FrLine(void) {                     // fredit.h:27
		bIncludeEnterLine = false;
	}

	std::string text;                  // 0x0
	bool marginAlign;                  // 0x1c
	unsigned long animStart;           // 0x20
	bool bIncludeEnterLine;            // 0x24
};

CHECK_SIZE(FrLine, 0x28);
CHECK_FIELD(FrLine, text, 0x0);
CHECK_FIELD(FrLine, marginAlign, 0x1c);
CHECK_FIELD(FrLine, animStart, 0x20);
CHECK_FIELD(FrLine, bIncludeEnterLine, 0x24);

// A text edit window, single- or multi-line. Layout, access and methods from
// the PDB (size 0x1b4, base FrWnd, no new virtual). The bodies written here
// are the inline ones whose COMDATs carry fredit.h line marks (lines 37-89).
class FrEdit : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	enum eEditStyle {
		ES_EMOTICON = 1,
		ES_READ_ONLY = 2,
		ES_NO_EMOTICON = 4
	};

	enum eAlign {
		leftAlign = 0,
		centerAlign = 1,
		rightAlign = 2
	};

	enum eEditTextType {
		ETT_FRONT = 0,
		ETT_COMP = 1,
		ETT_END = 2,
		MAX_ETT = 3
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // fredit.h:37

	FrEdit(void);
	virtual ~FrEdit(void);

	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	void SetReadOnly(bool readOnly);
	const char* AddLine(const char* text, unsigned long color, bool marginAlign);
	const char* SetLine(int line, const char* text, unsigned long color, bool marginAlign, unsigned long AnimStart);
	const char* GetLine(int line, bool makeNewLine);
	void ForcedEdit(const char* text, unsigned long color);
	int GetLineNum(void);
	void ClearLine(void);
	void DeleteFirstLine(void);
	void DeleteEndLine(void);
	void AddText(const char* text, bool spaceAlign, bool marginAlign);
	void SetCharLimit(int limit, bool getMinValue);
	void SetWidthLimit(float limit) { m_widthLimit = limit; }                      // fredit.h:68
	const char* GetEditText_Front(void) { return m_editText[ETT_FRONT].c_str(); }   // fredit.h:69
	const char* GetEditText_Comp(void) { return m_editText[ETT_COMP].c_str(); }     // fredit.h:70
	const char* GetEditText_End(void) { return m_editText[ETT_END].c_str(); }       // fredit.h:71
	void SetFontColor(unsigned long color) { m_fontColor = color; }                // fredit.h:72
	float GetWidthLimit(void) const { return m_widthLimit; }                       // fredit.h:73
	int GetLineHeight(void) const { return m_lineHeight; }                         // fredit.h:74
	unsigned long GetAnimTime(int line);
	// Never emitted as a COMDAT; the body is inferred from its member.
	void SetAutoLine(bool autoLine) { m_AutoLine = autoLine; }
	int GetSelectLine(void) { return m_selectedLine; }                             // fredit.h:78
	// Never emitted as a COMDAT; the body is inferred from its member.
	void SetSelectLine(int line) { m_selectedLine = line; }
	void EnableEditStyle(eEditStyle style) { m_editProperty.Enable(style); }       // fredit.h:87
	void DisableEditStyle(eEditStyle style) { m_editProperty.Disable(style); }     // fredit.h:88
	bool IsEditStyle(eEditStyle style) { return m_editProperty.GetFlag(style); }   // fredit.h:89
	bool IsEmoticonStyle(void);

protected:
	virtual void OnProc(const float deltaTime);
	virtual void OnDraw(void);
	virtual void OnResize(void);
	virtual void OnMouseMove(const WPoint& mousePos);
	virtual bool OnLButtonUp(const WPoint& mousePos);
	virtual bool OnLButtonDown(const WPoint& mousePos);
	virtual const char* OnSelectText(const FrInputState* istate);
	virtual void EnableKeyFocus(FrInputState& istate);
	virtual void OnKeyFocus(CChatMsg* im);

	const char* InsertLine(int lineIndex, const char* text, unsigned long color, bool marginAlign);
	int DeleteLine(int lineIndex);
	void CursorInputProcess(void);
	void AutoCutNextLineProcess(void);
	void LimitTextMultiLine(void);
	int GetWholeTextLength(bool bIncludeReturnLine);
	bool IsCursorEndPosition(void);
	bool IsStringCompress(void);
	bool PullNextLine(void);
	void LimitText(std::string& text);
	void RemoveColornLastSpace(const char* src, std::string& dest);
	void MakePassword(const char* src, std::string& dest);

	FrGuiItem* m_pItem;                 // 0xd4
	int m_lines;                        // 0xd8
	int m_lineHeight;                   // 0xdc
	int m_leftMargin;                   // 0xe0
	int m_topMargin;                    // 0xe4
	unsigned long m_font;               // 0xe8
	unsigned long m_fontColor;          // 0xec
	unsigned long m_fontColor2;         // 0xf0
	unsigned long m_bgColor;            // 0xf4
	unsigned long m_borderColor;        // 0xf8
	bool m_multiLine;                   // 0xfc
	int m_charLimit;                    // 0x100
	float m_widthLimit;                 // 0x104
	bool m_password;                    // 0x108
	bool m_bCaretMove;                  // 0x109
	WFlags m_editProperty;              // 0x10c
	bool m_focusOff;                    // 0x110
	bool m_bSymmetry;                   // 0x111
	unsigned long m_dwCaretColor;       // 0x114
	unsigned long m_dwCaretColor2;      // 0x118
	bool m_bScrollUpdate;               // 0x11c
	int m_OldCaretPos;                  // 0x120
	eAlign m_align;                     // 0x124
	bool m_vCenterAlign;                // 0x128
	std::string m_editText[MAX_ETT];    // 0x12c
	std::string m_oldEditText;          // 0x180
	int m_selectedLine;                 // 0x19c
	std::list<FrLine*> m_lineList;      // 0x1a0
	float m_caret;                      // 0x1ac
	char m_caretDelta;                  // 0x1b0
	bool m_AutoLine;                    // 0x1b1
};

CHECK_SIZE(FrEdit, 0x1b4);
BEGIN_LAYOUT_CHECK(FrEdit)
	CHECK_FIELD(FrEdit, m_pItem, 0xd4);
	CHECK_FIELD(FrEdit, m_lines, 0xd8);
	CHECK_FIELD(FrEdit, m_lineHeight, 0xdc);
	CHECK_FIELD(FrEdit, m_leftMargin, 0xe0);
	CHECK_FIELD(FrEdit, m_topMargin, 0xe4);
	CHECK_FIELD(FrEdit, m_font, 0xe8);
	CHECK_FIELD(FrEdit, m_fontColor, 0xec);
	CHECK_FIELD(FrEdit, m_fontColor2, 0xf0);
	CHECK_FIELD(FrEdit, m_bgColor, 0xf4);
	CHECK_FIELD(FrEdit, m_borderColor, 0xf8);
	CHECK_FIELD(FrEdit, m_multiLine, 0xfc);
	CHECK_FIELD(FrEdit, m_charLimit, 0x100);
	CHECK_FIELD(FrEdit, m_widthLimit, 0x104);
	CHECK_FIELD(FrEdit, m_password, 0x108);
	CHECK_FIELD(FrEdit, m_bCaretMove, 0x109);
	CHECK_FIELD(FrEdit, m_editProperty, 0x10c);
	CHECK_FIELD(FrEdit, m_focusOff, 0x110);
	CHECK_FIELD(FrEdit, m_bSymmetry, 0x111);
	CHECK_FIELD(FrEdit, m_dwCaretColor, 0x114);
	CHECK_FIELD(FrEdit, m_dwCaretColor2, 0x118);
	CHECK_FIELD(FrEdit, m_bScrollUpdate, 0x11c);
	CHECK_FIELD(FrEdit, m_OldCaretPos, 0x120);
	CHECK_FIELD(FrEdit, m_align, 0x124);
	CHECK_FIELD(FrEdit, m_vCenterAlign, 0x128);
	CHECK_FIELD(FrEdit, m_editText, 0x12c);
	CHECK_FIELD(FrEdit, m_oldEditText, 0x180);
	CHECK_FIELD(FrEdit, m_selectedLine, 0x19c);
	CHECK_FIELD(FrEdit, m_lineList, 0x1a0);
	CHECK_FIELD(FrEdit, m_caret, 0x1ac);
	CHECK_FIELD(FrEdit, m_caretDelta, 0x1b0);
	CHECK_FIELD(FrEdit, m_AutoLine, 0x1b1);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FREDIT_H
