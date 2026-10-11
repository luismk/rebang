// Original: d:\build\custom\temp\code\client\fresh\include\frgraphicinterface.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRGRAPHICINTERFACE_H
#define PG_CLIENT_FRESH_INCLUDE_FRGRAPHICINTERFACE_H

#include "..\..\defines.h"
#include "../client/Wangreal/include/wtypes.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wview.h"

class Bitmap;
class FrTEXT;
class TexCacheManager;
class WFont;

// Fresh's text drawing enums (the PDB's FrGDI::eTextAlign and
// FrGDI::eTextStyleFlags; no UDT record for FrGDI itself, hence a namespace).
namespace FrGDI {
	enum eTextAlign {
		ALIGN_LEFT = 0,
		ALIGN_CENTER = 1,
		ALIGN_RIGHT = 2
	};
	enum eTextStyleFlags {
		NONE = 0,
		THICK = 1,
		OUTLINE = 2,
		SHADOW = 4,
		ITALIC = 8,
		ANTIALIASED = 16
	};
}

// Fresh's drawing front end over the Wangreal view: textures, text, lines and
// boxes. Layout, access and methods from the PDB (size 0x2c, vfptr at 0). The
// bodies written here are the inline ones whose COMDATs carry
// frgraphicinterface.h line marks (lines 96-171).
class FrGraphicInterface {
	LAYOUT_CHECK_FRIEND;
public:
	FrGraphicInterface(WView* pView);
	virtual ~FrGraphicInterface(void);

	void RefreshTexCache(const Bitmap& bitmap);
	void InvalidateCache(const Bitmap& bitmap);
	void DrawTexture(const Bitmap* pBitmap, const WRect& src, const WRect& dest, unsigned long diffuse, int type) const;
	void DrawTexture(const Bitmap* pBitmap, const WRect& dest, unsigned long diffuse, int type) const;
	void UpdateTextureCacheInfo(const Bitmap* pBitmap);
	void RefreshTextureCacheInfo(const Bitmap* pBitmap);
	void SetClippingArea(WRect* clip);
	void SetClippingArea11(WRect* clip);
	void SetAlpha(float alpha);
	float GetAlpha(void);
	void SetTextColor(unsigned long color, unsigned long outlineColor);
	void SetTextStyle(unsigned long textStyle);
	float __cdecl Print(const WPoint& pos, unsigned long align, const char* format, ...);
	float __cdecl Print11(const WPoint& pos, unsigned long align, const char* format, ...);
	void PrintText(const WPoint& pos, unsigned long align, const FrTEXT& text);
	void PrintText(const WPoint& pos, const FrTEXT& text);
	float PrintText(const WPoint& pos, unsigned long align, const char* text, Bitmap* pBitmap);
	void PrintText11(const WPoint& pos, unsigned long align, const FrTEXT& text);
	void PrintText11(const WPoint& pos, const FrTEXT& text);
	float PrintText11(const WPoint& pos, unsigned long align, const char* text, Bitmap* pBitmap);
	float GetTextExtend(const char* lpszText);
	float GetTextExtend11(const char* lpszText);
	int GetFontHeight(void);
	int GetFontHeight11(void);
	int SetSpace(int space);
	int SetSpace11(int space);
	void SetScale(float scale);
	void SetScale11(float scale);
	void Reset(void);

	unsigned long GetTextColor(void) const { return m_textColor; }                 // frgraphicinterface.h:96
	unsigned long GetTextOutlineColor(void) const { return m_textOutlineColor; }   // frgraphicinterface.h:97

	// frgraphicinterface.h:137
	void Line(const WPoint& a, const WPoint& b, unsigned long colorA, unsigned long colorB, unsigned long type) const {
		g_view->DrawLine2D(a, b, colorA, colorB, type);
	}
	// frgraphicinterface.h:142
	void Box(const _WRECT& dest, unsigned long diffuse, unsigned long type, float depth) const {
		WOverlay::DrawBox(g_view, WRect(dest), type, diffuse, depth);
	}
	// frgraphicinterface.h:147
	void RainbowBox(const _WRECT& dest, unsigned long* const aDiffuse, unsigned long type, float depth) const {
		WOverlay::DrawRainbowBox(g_view, WRect(dest), aDiffuse, type, depth);
	}
	// frgraphicinterface.h:152
	void LineBox(const _WRECT& dest, unsigned long diffuse, unsigned long type, float depth) const {
		WOverlay::DrawLineBox(g_view, WRect(dest), type, diffuse);
	}
	// frgraphicinterface.h:164
	float GetViewWidth(void) {
		return m_pView ? m_pView->GetWidth() : 0.0f;
	}
	// frgraphicinterface.h:169
	float GetViewHeight(void) {
		return m_pView ? m_pView->GetHeight() : 0.0f;
	}

protected:
	WView* m_pView;                       // 0x4
	TexCacheManager* m_pCacheManager;     // 0x8
	float m_alpha;                        // 0xc
	unsigned long m_textColor;            // 0x10
	unsigned long m_textOutlineColor;     // 0x14
	unsigned long m_textStyle;            // 0x18
	WFont* m_pFont12;                     // 0x1c
	WFont* m_pFont11;                     // 0x20
	int m_maxTexCache;                    // 0x24
	int m_cacheTexSize;                   // 0x28
};

CHECK_SIZE(FrGraphicInterface, 0x2c);
BEGIN_LAYOUT_CHECK(FrGraphicInterface)
	CHECK_FIELD(FrGraphicInterface, m_pView, 0x4);
	CHECK_FIELD(FrGraphicInterface, m_pCacheManager, 0x8);
	CHECK_FIELD(FrGraphicInterface, m_alpha, 0xc);
	CHECK_FIELD(FrGraphicInterface, m_textColor, 0x10);
	CHECK_FIELD(FrGraphicInterface, m_textOutlineColor, 0x14);
	CHECK_FIELD(FrGraphicInterface, m_textStyle, 0x18);
	CHECK_FIELD(FrGraphicInterface, m_pFont12, 0x1c);
	CHECK_FIELD(FrGraphicInterface, m_pFont11, 0x20);
	CHECK_FIELD(FrGraphicInterface, m_maxTexCache, 0x24);
	CHECK_FIELD(FrGraphicInterface, m_cacheTexSize, 0x28);
END_LAYOUT_CHECK

// frgraphicinterface.h:159; scales the alpha byte of a colour.
inline unsigned long FrALPHA(unsigned long color, float alpha) {
	return ((unsigned long)((color >> 24) * alpha) << 24) | (color & 0x00ffffff);
}

#endif // PG_CLIENT_FRESH_INCLUDE_FRGRAPHICINTERFACE_H
