// Original: d:\build\custom\temp\code\client\wangreal\include\wfont.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WFONT_H
#define PG_CLIENT_WANGREAL_INCLUDE_WFONT_H

#include "..\..\defines.h"
#include "wlist.h"
#include "woverlay.h"

class Bitmap;
class WView;

// Base of the bitmap fonts: an overlay with a colour set and the overlays it
// cached (PDB layout, size 0xb0).
class WFont : public WOverlay {
	LAYOUT_CHECK_FRIEND;
public:
	enum eFontStyle {
		NORMAL = 0,
		MASKED = 1,
		BOLD = 2,
		SHADOW = 3,
		OUTLINE = 4
	};

	WFont(void);
	virtual ~WFont(void);

	// wfont.h:11
	virtual WFont* MakeClone(void) { return NULL; }
	// wfont.h:13
	void SetColorSet(int index, unsigned long color) { m_colorset[index] = color; }
	// wfont.h:14
	void SetScale(float scale) { m_scale = scale; }
	// wfont.h:15
	void SetSpace(int space) { m_space = space; }
	// wfont.h:16
	virtual void SetCoordMode(int mode) { WOverlay::SetCoordMode(mode | 0x400); }
	virtual float GetTextWidth(WView* view, const char* text);
	// wfont.h:27
	void SetMode(eFontStyle type) { m_eType = type; }
	// wfont.h:29
	int GetSpace(void) const { return m_space; }
	// wfont.h:30
	float GetScale(void) const { return m_scale; }
	// wfont.h:33
	virtual void SetFixedWidth(bool fixed) {}
	// wfont.h:34
	virtual void SetFontWidth(int width) {}
	// wfont.h:35
	virtual int GetFontHeight(void) { return 0; }
	virtual void Flush(WView* view);
	// wfont.h:37
	virtual void Reset(void) {}

	float Print(WView* view, float x, float y, const char* text, int type, unsigned long diffuse, Bitmap* bmp);

protected:
	virtual float PrintInside(WView* view, float x, float y, const char* text, int len, unsigned long color, Bitmap* bitmap) = 0;
	virtual float GetTextWidthInside(WView* view, const char* text) = 0;

	float PrintOut(WView* view, float x, float y, const char* text, int len, unsigned long color, bool useOverlay, Bitmap* bitmap);
	float GetFontScale(WView* view);

	eFontStyle m_eType;                 // 0x58
	int m_space;                        // 0x5c

private:
	void ResetOverlay(void);

	float m_scale;                      // 0x60
	int m_colorset[10];                 // 0x64
	bool m_useOverlay;                  // 0x8c
	WList<WOverlay*> m_overlayList;     // 0x90
};
CHECK_SIZE(WFont, 0xb0);
BEGIN_LAYOUT_CHECK(WFont)
	CHECK_FIELD(WFont, m_eType, 0x58);
	CHECK_FIELD(WFont, m_space, 0x5c);
	CHECK_FIELD(WFont, m_scale, 0x60);
	CHECK_FIELD(WFont, m_colorset, 0x64);
	CHECK_FIELD(WFont, m_useOverlay, 0x8c);
	CHECK_FIELD(WFont, m_overlayList, 0x90);
END_LAYOUT_CHECK

// Font drawn from a texture loaded as an overlay (PDB layout: size 0xb0,
// base WFont, no members of its own).
class WTexFont : public WFont {
public:
	WTexFont(const char* filename);
	virtual ~WTexFont(void);

	int Load(const char* filename, int flag);

protected:
	virtual float PrintInside(WView* view, float x, float y, const char* text, int len, unsigned long color, Bitmap* bitmap);
	virtual float GetTextWidthInside(WView* view, const char* text);
};
CHECK_SIZE(WTexFont, 0xb0);

// Description of a bitmap title font (PDB layout).
struct tagWTITLEFONT {
	char** filename;    // 0x0
	int numPages;       // 0x4
	int fontw;          // 0x8
	int fonth;          // 0xc
	int texw;           // 0x10
	int texh;           // 0x14
	int flag;           // 0x18
	char* pCharSet;     // 0x1c
};

CHECK_SIZE(tagWTITLEFONT, 0x20);
CHECK_FIELD(tagWTITLEFONT, filename, 0x0);
CHECK_FIELD(tagWTITLEFONT, numPages, 0x4);
CHECK_FIELD(tagWTITLEFONT, fontw, 0x8);
CHECK_FIELD(tagWTITLEFONT, fonth, 0xc);
CHECK_FIELD(tagWTITLEFONT, texw, 0x10);
CHECK_FIELD(tagWTITLEFONT, texh, 0x14);
CHECK_FIELD(tagWTITLEFONT, flag, 0x18);
CHECK_FIELD(tagWTITLEFONT, pCharSet, 0x1c);

// Texture font drawn from a tagWTITLEFONT: one texture page per file, a
// width per character (PDB layout, size 0xec).
class WTitleFont : public WFont {
	LAYOUT_CHECK_FRIEND;
public:
	WTitleFont(void);
	WTitleFont(tagWTITLEFONT* info);
	virtual ~WTitleFont(void);

	void Create(tagWTITLEFONT* info);
	void Erase(void);
	// wfont.h:97
	tagWTITLEFONT* GetFontInfo(void) { return &m_info; }
	float PutChar(WView* view, float x, float y, const int nChar, int type, unsigned long diffuse);
	float GetCharWidthA(WView* view, const int nChar);

protected:
	virtual float PrintInside(WView* view, float x, float y, const char* text, int len, unsigned long color, Bitmap* bitmap);
	virtual float GetTextWidthInside(WView* view, const char* text);

private:
	tagWTITLEFONT m_info;           // 0xb0
	int* m_iTexIndex;               // 0xd0
	float* m_fWidthIndex;           // 0xd4
	char* m_pCharSet;               // 0xd8
	int m_numCharSet;               // 0xdc
	int m_numPerPage;               // 0xe0
	int m_numWidth;                 // 0xe4
	int m_numTex;                   // 0xe8
};
CHECK_SIZE(WTitleFont, 0xec);
BEGIN_LAYOUT_CHECK(WTitleFont)
	CHECK_FIELD(WTitleFont, m_info, 0xb0);
	CHECK_FIELD(WTitleFont, m_iTexIndex, 0xd0);
	CHECK_FIELD(WTitleFont, m_fWidthIndex, 0xd4);
	CHECK_FIELD(WTitleFont, m_pCharSet, 0xd8);
	CHECK_FIELD(WTitleFont, m_numCharSet, 0xdc);
	CHECK_FIELD(WTitleFont, m_numPerPage, 0xe0);
	CHECK_FIELD(WTitleFont, m_numWidth, 0xe4);
	CHECK_FIELD(WTitleFont, m_numTex, 0xe8);
END_LAYOUT_CHECK

// Font read from a .fnt bitmap set, its glyphs cached into eight bitmaps
// (PDB layout, size 0x3a0).
class WFntFont : public WFont {
	LAYOUT_CHECK_FRIEND;
public:
	WFntFont(unsigned char* ptr);
	virtual ~WFntFont(void);

	virtual WFont* MakeClone(void);
	int Load(const char* filename);
	virtual void Flush(WView* view);
	void SetFullEnglish(bool flag);
	// wfont.h:147
	virtual void SetFixedWidth(bool fixed) { m_bFixedWidth = fixed; }

private:
	virtual float PrintInside(WView* view, float x, float y, const char* text, int len, unsigned long color, Bitmap* bitmap);
	virtual float GetTextWidthInside(WView* view, const char* text);
	void Release(void);
	int GetFontIdx(unsigned short code);
	void FlushFont(void);

protected:
	// wfont.h:158
	int GetCacheNum(int idx) { return idx / 16; }
	int PutChar(WView* view, unsigned short code, float x, float y, unsigned long type, unsigned long diffuse, int bGetWidth);
	int GetFont(unsigned short code, unsigned char* ptr, int width);
	int AddCache(unsigned short code);

	bool m_bFullEnglish;                    // 0xb0
	bool m_bFixedWidth;                     // 0xb1
	unsigned char* m_font;                  // 0xb4
	unsigned char m_midbuff[256];           // 0xb8
	Bitmap* m_bitmap[8];                    // 0x1b8
	int m_cachedHandle[8];                  // 0x1d8
	int m_updateFrame[8];                   // 0x1f8
	unsigned short m_cached[128];           // 0x218
	unsigned char m_cachedFontWidth[128];   // 0x318
	int m_cacheLen;                         // 0x398
	bool m_clone;                           // 0x39c
};
CHECK_SIZE(WFntFont, 0x3a0);
BEGIN_LAYOUT_CHECK(WFntFont)
	CHECK_FIELD(WFntFont, m_bFullEnglish, 0xb0);
	CHECK_FIELD(WFntFont, m_bFixedWidth, 0xb1);
	CHECK_FIELD(WFntFont, m_font, 0xb4);
	CHECK_FIELD(WFntFont, m_midbuff, 0xb8);
	CHECK_FIELD(WFntFont, m_bitmap, 0x1b8);
	CHECK_FIELD(WFntFont, m_cachedHandle, 0x1d8);
	CHECK_FIELD(WFntFont, m_updateFrame, 0x1f8);
	CHECK_FIELD(WFntFont, m_cached, 0x218);
	CHECK_FIELD(WFntFont, m_cachedFontWidth, 0x318);
	CHECK_FIELD(WFntFont, m_cacheLen, 0x398);
	CHECK_FIELD(WFntFont, m_clone, 0x39c);
END_LAYOUT_CHECK

// Font whose glyphs are written on demand into texture pages; the texts
// printed in a frame are queued in m_flush_list (PDB layout, size 0xb0c).
class WFixedFont : public WFont {
	LAYOUT_CHECK_FRIEND;
public:
	struct w_fixedtext {
		int px;         // 0x0
		int py;         // 0x4
		int width;      // 0x8
		char msg[1];    // 0xc
	};
	struct w_flush_area {
		w_fixedtext* tex;       // 0x0
		float x;                // 0x4
		float y;                // 0x8
		unsigned long diffuse;  // 0xc
		int type;               // 0x10
	};
	struct w_temp_pair {
		w_fixedtext* tex;       // 0x0
		w_fixedtext* newone;    // 0x4
		char msg[1];            // 0x8
	};

	WFixedFont(int w, int h, int alias, unsigned char* ptr, int maxTexture);
	WFixedFont(int maxTexture);
	virtual ~WFixedFont(void);

	virtual WFont* MakeClone(void);
	int Load(const char* filename);
	virtual void Flush(WView* view);
	void Clear(bool flag);
	void SetFullEnglish(bool flag);
	// wfont.h:204
	virtual void SetFixedWidth(bool fixed) { m_bFixedWidth = fixed; }
	void Update(void);
	virtual void Reset(void);
	// wfont.h:209
	virtual int GetFontHeight(void) { return m_iFontHeight; }

protected:
	virtual float PrintInside(WView* view, float x, float y, const char* text, int len, unsigned long color, Bitmap* bitmap);
	virtual float GetTextWidthInside(WView* view, const char* text);

private:
	void Init(int maxTexture);
	unsigned char* GetFontAddr(unsigned short code, int* width);
	int GetTexture(const char* text);
	w_fixedtext* GetNewPart(const char* text);
	void AddOutArea(w_fixedtext* tex, float x, float y, int type, unsigned long diffuse);
	int WriteToBitmap(int width, unsigned char* mid);
	void Write2Buffer(unsigned char* font, unsigned char* buff, int len);
	void ReArrange(void);
	void PrintInTex(w_fixedtext* f, float x, float y, Bitmap* bmp, int type, unsigned long diffuse);
	void CreateBitmap(int i);

	WList<w_fixedtext*> m_textList;         // 0xb0
	w_flush_area m_flush_list[128];         // 0xd0
	int m_flush_num;                        // 0xad0
	bool m_update_flag;                     // 0xad4
	bool m_bFullEnglish;                    // 0xad5
	bool m_bFixedWidth;                     // 0xad6
	int m_bAntialiased;                     // 0xad8
	int m_nMaxTexture;                      // 0xadc
	int* m_texIndex;                        // 0xae0
	Bitmap** m_bitmap;                      // 0xae4
	bool* m_update;                         // 0xae8
	unsigned char* m_font;                  // 0xaec
	static unsigned char m_mid[65536];
	int m_iFontWidth;                       // 0xaf0
	int m_iFontHeight;                      // 0xaf4
	int m_nMaxWidth;                        // 0xaf8
	int m_nowX;                             // 0xafc
	int m_nowY;                             // 0xb00
	int m_bytePerChar;                      // 0xb04
	bool m_clone;                           // 0xb08
};
CHECK_SIZE(WFixedFont::w_fixedtext, 0x10);
CHECK_SIZE(WFixedFont::w_flush_area, 0x14);
CHECK_SIZE(WFixedFont::w_temp_pair, 0xc);
CHECK_SIZE(WFixedFont, 0xb0c);
BEGIN_LAYOUT_CHECK(WFixedFont)
	CHECK_FIELD(WFixedFont, m_textList, 0xb0);
	CHECK_FIELD(WFixedFont, m_flush_list, 0xd0);
	CHECK_FIELD(WFixedFont, m_flush_num, 0xad0);
	CHECK_FIELD(WFixedFont, m_update_flag, 0xad4);
	CHECK_FIELD(WFixedFont, m_bFullEnglish, 0xad5);
	CHECK_FIELD(WFixedFont, m_bFixedWidth, 0xad6);
	CHECK_FIELD(WFixedFont, m_bAntialiased, 0xad8);
	CHECK_FIELD(WFixedFont, m_nMaxTexture, 0xadc);
	CHECK_FIELD(WFixedFont, m_texIndex, 0xae0);
	CHECK_FIELD(WFixedFont, m_bitmap, 0xae4);
	CHECK_FIELD(WFixedFont, m_update, 0xae8);
	CHECK_FIELD(WFixedFont, m_font, 0xaec);
	CHECK_FIELD(WFixedFont, m_iFontWidth, 0xaf0);
	CHECK_FIELD(WFixedFont, m_iFontHeight, 0xaf4);
	CHECK_FIELD(WFixedFont, m_nMaxWidth, 0xaf8);
	CHECK_FIELD(WFixedFont, m_nowX, 0xafc);
	CHECK_FIELD(WFixedFont, m_nowY, 0xb00);
	CHECK_FIELD(WFixedFont, m_bytePerChar, 0xb04);
	CHECK_FIELD(WFixedFont, m_clone, 0xb08);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WFONT_H
