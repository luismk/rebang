// Original: d:\build\custom\temp\code\client\projectg\titlefontex.h
#ifndef PG_CLIENT_PROJECTG_TITLEFONTEX_H
#define PG_CLIENT_PROJECTG_TITLEFONTEX_H

#include "..\..\defines.h"

class WTitleFont;

// Horizontal alignment of WTitleFontEx::Print (names invented: /Zd).
enum eTITLEFONTALINE {
	TITLEFONT_ALIGN_LEFT = 0,
	TITLEFONT_ALIGN_CENTER = 1,
	TITLEFONT_ALIGN_RIGHT = 2
};

// A title font of the resource manager with its colour (size 0xc). The
// subclasses only describe which font texture to create.
class WTitleFontEx {
	LAYOUT_CHECK_FRIEND;
public:
	virtual ~WTitleFontEx(void);

	float Print(float x, float y, const char* text) const;
	void Print(float x, float y, const char* text, eTITLEFONTALINE align, float space, float scale) const;
	float GetFontH(void) const;
	float GetFontW(void) const;
	float GetStringWidth(const char* text);
	// titlefontex.h:33
	void SetColor(unsigned long color) { m_color = color; }

protected:
	WTitleFontEx(void);

	int GetCharIndex(char c);

	WTitleFont* m_pFont;        // 0x4
	unsigned long m_color;      // 0x8
};
CHECK_SIZE(WTitleFontEx, 0xc);
BEGIN_LAYOUT_CHECK(WTitleFontEx)
	CHECK_FIELD(WTitleFontEx, m_pFont, 0x4);
	CHECK_FIELD(WTitleFontEx, m_color, 0x8);
END_LAYOUT_CHECK

class CWindFont : public WTitleFontEx {
public:
	CWindFont(void);
};
CHECK_SIZE(CWindFont, 0xc);

class COpenTournamentFont : public WTitleFontEx {
public:
	COpenTournamentFont(void);
};
CHECK_SIZE(COpenTournamentFont, 0xc);

class CBlueWindFontSmall : public WTitleFontEx {
public:
	CBlueWindFontSmall(void);
};
CHECK_SIZE(CBlueWindFontSmall, 0xc);

class CBlueWindFontBig : public WTitleFontEx {
public:
	CBlueWindFontBig(void);
};
CHECK_SIZE(CBlueWindFontBig, 0xc);

class CBlueHoleSkinFont : public WTitleFontEx {
public:
	CBlueHoleSkinFont(void);
};
CHECK_SIZE(CBlueHoleSkinFont, 0xc);

class CBarFont : public WTitleFontEx {
public:
	CBarFont(void);
};
CHECK_SIZE(CBarFont, 0xc);

#endif // PG_CLIENT_PROJECTG_TITLEFONTEX_H
