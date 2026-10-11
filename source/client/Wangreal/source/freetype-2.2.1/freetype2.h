// Original: d:\build\custom\temp\code\client\wangreal\source\freetype-2.2.1\freetype2.h
#ifndef PG_CLIENT_WANGREAL_SOURCE_FREETYPE_2_2_1_FREETYPE2_H
#define PG_CLIENT_WANGREAL_SOURCE_FREETYPE_2_2_1_FREETYPE2_H

#include "..\..\..\defines.h"

struct FT_LibraryRec_;
struct FT_FaceRec_;

// A FreeType 2 face rendering one glyph at a time (PDB layout, size 0x14).
class CFreeType2 {
	LAYOUT_CHECK_FRIEND;
public:
	CFreeType2(void);
	~CFreeType2(void);

	bool Load(const char* fname, int fntsize, int fontindex, bool preloadall);
	bool ChangFontSize(int fontsize);
	bool Render(unsigned short code, int* w, int* h);
	void Write24(unsigned char* img24, int pitch);
	void Write32a(unsigned char* img32, int pitch);

private:
	FT_LibraryRec_* m_Library;      // 0x0
	FT_FaceRec_* m_Face;            // 0x4
	unsigned char* m_pFileBuffer;   // 0x8
	int m_FontSize;                 // 0xc
	unsigned short m_LastLetter;    // 0x10
};
CHECK_SIZE(CFreeType2, 0x14);
BEGIN_LAYOUT_CHECK(CFreeType2)
	CHECK_FIELD(CFreeType2, m_Library, 0x0);
	CHECK_FIELD(CFreeType2, m_Face, 0x4);
	CHECK_FIELD(CFreeType2, m_pFileBuffer, 0x8);
	CHECK_FIELD(CFreeType2, m_FontSize, 0xc);
	CHECK_FIELD(CFreeType2, m_LastLetter, 0x10);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_SOURCE_FREETYPE_2_2_1_FREETYPE2_H
