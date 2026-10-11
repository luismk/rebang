// Original: d:\build\custom\temp\code\client\fresh\include\frtextbutton.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRTEXTBUTTON_H
#define PG_CLIENT_FRESH_INCLUDE_FRTEXTBUTTON_H

#include <list>
#include <string>

#include "..\..\defines.h"
#include "frwnd.h"

class FrGuiItem;

// A row of text buttons built from a separated caption. Layout, access and
// methods from the PDB (size 0x130, base FrWnd, no new virtual).
class FrTextButton : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	// One button of the row (PDB layout, size 0x28).
	struct TButtonItem {
		std::string text;                // 0x0
		float left;                      // 0x1c
		float right;                     // 0x20
		bool underCursor;                // 0x24
		bool selected;                   // 0x25
	};

	typedef std::list<TButtonItem> TBUTTON_LIST;

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frtextbutton.h:19

	FrTextButton(void);
	virtual ~FrTextButton(void);

	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	void SetCaption(const char* caption);
	const char* GetCaption(void);
	void SetClippingArea(WRect* clip);
	const char* GetUnderCursor(void);
	const char* GetSelected(void);
	void Select(int index);
	void Unselect(int index);

protected:
	void Select(TBUTTON_LIST::iterator it);
	void Unselect(TBUTTON_LIST::iterator it);
	virtual void OnDraw(void);
	virtual void OnProc(const float deltaTime);
	virtual void OnMouseMove(const WPoint& mousePos);
	virtual bool OnLButtonUp(const WPoint& mousePos);
	virtual void OnSetCursor(bool bInClient, const WPoint& mousePos);

	std::string m_caption;               // 0xd4
	unsigned long m_color;               // 0xf0
	unsigned long m_outlineColor;        // 0xf4
	unsigned long m_style;               // 0xf8
	unsigned long m_selColor;            // 0xfc
	unsigned long m_selOutlineColor;     // 0x100
	unsigned long m_selStyle;            // 0x104
	bool m_center;                       // 0x108
	WRect* m_clip;                       // 0x10c
	float m_hlAlpha;                     // 0x110
	char m_hlAlphaD;                     // 0x114
	float m_sepWidth;                    // 0x118
	TBUTTON_LIST m_tBtnList;             // 0x11c
	TBUTTON_LIST::iterator m_underCursor;  // 0x128
	TBUTTON_LIST::iterator m_selected;   // 0x12c
};

CHECK_SIZE(FrTextButton::TButtonItem, 0x28);
CHECK_FIELD(FrTextButton::TButtonItem, text, 0x0);
CHECK_FIELD(FrTextButton::TButtonItem, left, 0x1c);
CHECK_FIELD(FrTextButton::TButtonItem, right, 0x20);
CHECK_FIELD(FrTextButton::TButtonItem, underCursor, 0x24);
CHECK_FIELD(FrTextButton::TButtonItem, selected, 0x25);

CHECK_SIZE(FrTextButton, 0x130);
BEGIN_LAYOUT_CHECK(FrTextButton)
	CHECK_FIELD(FrTextButton, m_caption, 0xd4);
	CHECK_FIELD(FrTextButton, m_color, 0xf0);
	CHECK_FIELD(FrTextButton, m_outlineColor, 0xf4);
	CHECK_FIELD(FrTextButton, m_style, 0xf8);
	CHECK_FIELD(FrTextButton, m_selColor, 0xfc);
	CHECK_FIELD(FrTextButton, m_selOutlineColor, 0x100);
	CHECK_FIELD(FrTextButton, m_selStyle, 0x104);
	CHECK_FIELD(FrTextButton, m_center, 0x108);
	CHECK_FIELD(FrTextButton, m_clip, 0x10c);
	CHECK_FIELD(FrTextButton, m_hlAlpha, 0x110);
	CHECK_FIELD(FrTextButton, m_hlAlphaD, 0x114);
	CHECK_FIELD(FrTextButton, m_sepWidth, 0x118);
	CHECK_FIELD(FrTextButton, m_tBtnList, 0x11c);
	CHECK_FIELD(FrTextButton, m_underCursor, 0x128);
	CHECK_FIELD(FrTextButton, m_selected, 0x12c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRTEXTBUTTON_H
