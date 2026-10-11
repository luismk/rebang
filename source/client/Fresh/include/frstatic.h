// Original: d:\build\custom\temp\code\client\fresh\include\frstatic.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRSTATIC_H
#define PG_CLIENT_FRESH_INCLUDE_FRSTATIC_H

#include <string>

#include "..\..\defines.h"
#include "frwnd.h"

class FrGuiItem;

// A static text window. Layout, access and methods from the PDB (size 0x108,
// base FrWnd, no new virtual). The bodies written here are the inline ones
// whose COMDATs carry frstatic.h line marks (lines 16-25).
class FrStatic : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frstatic.h:16

	FrStatic(void);
	virtual ~FrStatic(void);

	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	void SetCaption(const char* caption) { m_caption = caption; }   // frstatic.h:23
	void SetCaption11(const char* caption) {                        // frstatic.h:24
		m_caption = caption;
		m_smallFont = true;
	}
	const char* GetCaption(void) { return m_caption.c_str(); }      // frstatic.h:25
	void SetClippingArea(WRect* clip);
	void SetTextStyle(unsigned long style);
	void SetTextColor(unsigned long color, unsigned long outline_color);
	virtual void Enable(bool enable);

protected:
	virtual void OnDraw(void);

	std::string m_caption;              // 0xd4
	unsigned long m_color;              // 0xf0
	unsigned long m_outlineColor;       // 0xf4
	unsigned long m_style;              // 0xf8
	unsigned long m_align;              // 0xfc
	WRect* m_clip;                      // 0x100
	bool m_smallFont;                   // 0x104
};

CHECK_SIZE(FrStatic, 0x108);
BEGIN_LAYOUT_CHECK(FrStatic)
	CHECK_FIELD(FrStatic, m_caption, 0xd4);
	CHECK_FIELD(FrStatic, m_color, 0xf0);
	CHECK_FIELD(FrStatic, m_outlineColor, 0xf4);
	CHECK_FIELD(FrStatic, m_style, 0xf8);
	CHECK_FIELD(FrStatic, m_align, 0xfc);
	CHECK_FIELD(FrStatic, m_clip, 0x100);
	CHECK_FIELD(FrStatic, m_smallFont, 0x104);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRSTATIC_H
