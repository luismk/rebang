// Original: d:\build\custom\temp\code\client\wangreal\source\freetype-2.2.1\textout.h
#ifndef PG_CLIENT_WANGREAL_SOURCE_FREETYPE_2_2_1_TEXTOUT_H
#define PG_CLIENT_WANGREAL_SOURCE_FREETYPE_2_2_1_TEXTOUT_H

#include <vector>

#include "defines.h"
#include "client/Wangreal/include/bitmap.h"
#include "client/Wangreal/include/wfont.h"
#include "client/Wangreal/source/freetype-2.2.1/list.h"

class CFreeType2;
class CRenderer;

// WFont drawing TrueType text through FreeType: each text is rendered once
// into 256x256 work textures and drawn from its cached image set (PDB
// layout, size 0x11c).
class CTextOut : public WFont {
	LAYOUT_CHECK_FRIEND;
public:
	// Where a piece of a text sits in a work texture.
	struct _IMAGESET {
		int idx;            // 0x0
		int x;              // 0x4
		int y;              // 0x8
		int w;              // 0xc
		int h;              // 0x10
		_IMAGESET* next;    // 0x14
	};
	// A rendered text; the string follows the header.
	struct _TEXTSET {
		bool valid;                     // 0x0
		int width;                      // 0x4
		int fntsize;                    // 0x8
		unsigned long lastrendertick;   // 0xc
		_IMAGESET* ptr;                 // 0x10
		char text[1];                   // 0x14
	};
	// A text queued for the next Flush.
	struct _DISPLAYLIST {
		_TEXTSET* set;          // 0x0
		float x;                // 0x4
		float y;                // 0x8
		unsigned long rs;       // 0xc
		unsigned long diffuse;  // 0x10
	};

	CTextOut(CRenderer* renderer);
	virtual ~CTextOut(void);

	bool Load(const char* fname, int size, int fontindex, bool preload);
	void Clear(void);
	void SetFontSize(int fntsize);
	int GetWidth(const char* str);
	int Print(float x, float y, const char* str, unsigned long rs, unsigned long diffuse);
	virtual float PrintInside(WView* view, float x, float y, const char* text, int type, unsigned long diffuse, Bitmap* bmp);
	virtual float GetTextWidthInside(WView* view, const char* pText);
	virtual void Flush(WView* view);

private:
	_TEXTSET* GetTextSet(const char* text, int fontsize);
	void UpdateText(_TEXTSET* set);
	void UpdateTexture(void);
	void UseNextTexture(void);
	virtual void Reset(void);

	list::multimap_str<_TEXTSET*> m_TextSet;        // 0xb0
	Bitmap m_WorkImage;                             // 0xbc
	int m_WorkTextureCnt;                           // 0xd0
	int m_WorkTextureNum;                           // 0xd4
	int m_BeginWorkTextureCnt;                      // 0xd8
	int m_WorkX;                                    // 0xdc
	int m_WorkY[2];                                 // 0xe0
	bool m_bUpdateTexture;                          // 0xe8
	bool m_bResetTexture;                           // 0xe9
	unsigned long m_RecentCheckTime;                // 0xec
	std::vector<int> m_TextList;                    // 0xf0
	std::vector<_DISPLAYLIST> m_DisplayList;        // 0x100
	CFreeType2* m_pFontSet;                         // 0x110
	CRenderer* m_pRenderer;                         // 0x114
	int m_iFontSize;                                // 0x118
};
CHECK_SIZE(CTextOut::_IMAGESET, 0x18);
CHECK_SIZE(CTextOut::_TEXTSET, 0x18);
CHECK_SIZE(CTextOut::_DISPLAYLIST, 0x14);
CHECK_SIZE(CTextOut, 0x11c);
BEGIN_LAYOUT_CHECK(CTextOut)
	CHECK_FIELD(CTextOut, m_TextSet, 0xb0);
	CHECK_FIELD(CTextOut, m_WorkImage, 0xbc);
	CHECK_FIELD(CTextOut, m_WorkTextureCnt, 0xd0);
	CHECK_FIELD(CTextOut, m_WorkTextureNum, 0xd4);
	CHECK_FIELD(CTextOut, m_BeginWorkTextureCnt, 0xd8);
	CHECK_FIELD(CTextOut, m_WorkX, 0xdc);
	CHECK_FIELD(CTextOut, m_WorkY, 0xe0);
	CHECK_FIELD(CTextOut, m_bUpdateTexture, 0xe8);
	CHECK_FIELD(CTextOut, m_bResetTexture, 0xe9);
	CHECK_FIELD(CTextOut, m_RecentCheckTime, 0xec);
	CHECK_FIELD(CTextOut, m_TextList, 0xf0);
	CHECK_FIELD(CTextOut, m_DisplayList, 0x100);
	CHECK_FIELD(CTextOut, m_pFontSet, 0x110);
	CHECK_FIELD(CTextOut, m_pRenderer, 0x114);
	CHECK_FIELD(CTextOut, m_iFontSize, 0x118);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_SOURCE_FREETYPE_2_2_1_TEXTOUT_H
