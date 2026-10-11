// Original: d:\build\custom\temp\code\client\wangreal\source\freetype-2.2.1\freetype2.cpp
#include <new>
#include <string.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "client/Wangreal/source/freetype-2.2.1/freetype2.h"
#include "westpak.h"

// westpak.cpp; westpak.h does not declare it.
cFile* __cdecl GetCFile(const char* filename, int len, int pakonly);

// freetype2.cpp:21
CFreeType2::CFreeType2(void) {
	m_pFileBuffer = NULL;
	m_LastLetter = 0;
	m_Face = NULL;
	m_Library = NULL;
}

// freetype2.cpp:30
CFreeType2::~CFreeType2(void) {
	if (m_Face)
		FT_Done_Face(m_Face);

	if (m_Library)
		FT_Done_FreeType(m_Library);

	if (m_pFileBuffer)
		delete m_pFileBuffer;
}

// freetype2.cpp:43
bool CFreeType2::Load(const char* fname, int fntsize, int fontindex, bool preloadall) {
	if (FT_Init_FreeType(&m_Library))
		return false;

	m_FontSize = fntsize;
	if (preloadall == true) {
		cFile* file = GetCFile(fname, 0x8000, 0);
		if (file == NULL)
			return false;
		int size = file->Length();
		m_pFileBuffer = new unsigned char[size];

		file->Read(m_pFileBuffer, size);
		CloseCFile(file);

		if (FT_New_Memory_Face(m_Library, m_pFileBuffer, size, fontindex, &m_Face))
			return false;
	} else {
		if (FT_New_Face(m_Library, fname, fontindex, &m_Face))
			return false;
	}

	return FT_Set_Pixel_Sizes(m_Face, 0, fntsize) == 0 ? true : false;
}

// freetype2.cpp:110
bool CFreeType2::ChangFontSize(int fontsize) {
	if (m_Face && m_FontSize != fontsize) {
		if (FT_Set_Pixel_Sizes(m_Face, 0, fontsize) == 0) {
			m_FontSize = fontsize;
			return true;
		}
	}
	return false;
}

// freetype2.cpp:124
bool CFreeType2::Render(unsigned short code, int* w, int* h) {
	if (m_LastLetter != code) {
		if (FT_Get_Char_Index(m_Face, code) == 0)
			return false;

		if (FT_Load_Char(m_Face, code, FT_LOAD_RENDER | FT_LOAD_NO_HINTING))
			return false;
	}

	*w = m_Face->glyph->advance.x >> 6;
	*h = m_Face->glyph->bitmap.rows - m_Face->glyph->bitmap_top + m_FontSize;

	m_LastLetter = code;
	return true;
}

// freetype2.cpp:147
void CFreeType2::Write24(unsigned char* img24, int pitch) {
	FT_GlyphSlot slot = m_Face->glyph;

	switch (slot->bitmap.pixel_mode) {
	case FT_PIXEL_MODE_GRAY:
		for (int y = 0; y < slot->bitmap.rows; y++) {
			for (int x = 0; x < slot->bitmap.width; x++) {
				memset(img24 + (y - slot->bitmap_top + m_FontSize) * pitch + (slot->bitmap_left + x) * 3, slot->bitmap.buffer[slot->bitmap.width * y + x], 3);
			}
		}
		break;
	case FT_PIXEL_MODE_MONO: {
		int off;
		int v;
		for (v = 0, off = 0; v < slot->bitmap.rows; v++, off = (off + 7) & ~7) {
			for (int x = 0; x < slot->bitmap.width; x++, off++) {
				if (slot->bitmap.buffer[off >> 3] & (0x80 >> (off & 7)))
					memset(img24 + (m_FontSize - slot->bitmap_top + v) * pitch + (slot->bitmap_left + x) * 3, 0xff, 3);
			}
		}
		break;
	}
	}
}

// freetype2.cpp:183
void CFreeType2::Write32a(unsigned char* img32, int pitch) {
	FT_GlyphSlot slot = m_Face->glyph;

	switch (slot->bitmap.pixel_mode) {
	case FT_PIXEL_MODE_GRAY:
		for (int y = 0; y < slot->bitmap.rows; y++) {
			for (int x = 0; x < slot->bitmap.width; x++) {
				img32[(m_FontSize - slot->bitmap_top + y) * pitch + (slot->bitmap_left + x) * 4 + 3] = slot->bitmap.buffer[slot->bitmap.width * y + x];
			}
		}
		break;
	case FT_PIXEL_MODE_MONO: {
		int off;
		int v;
		for (v = 0, off = 0; v < slot->bitmap.rows; v++, off = (off + 7) & ~7) {
			for (int x = 0; x < slot->bitmap.width; x++, off++) {
				if (slot->bitmap.buffer[off >> 3] & (0x80 >> (off & 7)))
					img32[(m_FontSize - slot->bitmap_top + v) * pitch + (slot->bitmap_left + x) * 4 + 3] = 0xff;
				else
					img32[(m_FontSize - slot->bitmap_top + v) * pitch + (slot->bitmap_left + x) * 4 + 3] = 0;
			}
		}
		break;
	}
	}
}
