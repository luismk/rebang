// Original: d:\build\custom\temp\code\client\projectg\texturepart.h
#ifndef PG_CLIENT_PROJECTG_TEXTUREPART_H
#define PG_CLIENT_PROJECTG_TEXTUREPART_H

#include <windows.h>
#include <string.h>

#include "..\..\defines.h"
#include "bitmap.h"

// A texture swapped into one part of a pet (the size 0x94 is the `new` of
// CGolfPlayer::SetPendingTexPart). Bodies from the line marks of PetBody.
struct sTexturePart {
	// texturepart.h:20
	sTexturePart(void) {
		memset(m_partName, 0, sizeof(m_partName));
		memset(m_texName, 0, sizeof(m_texName));
		memset(&m_rect, 0, sizeof(m_rect));
		m_pBitmap = NULL;
	}
	// texturepart.h:29
	~sTexturePart(void) {
		if (m_pBitmap) { delete m_pBitmap; m_pBitmap = NULL; }
	}

	char m_partName[0x40];   // 0x00
	char m_texName[0x40];    // 0x40
	Bitmap *m_pBitmap;       // 0x80
	RECT m_rect;             // 0x84
};

CHECK_SIZE(sTexturePart, 0x94);
CHECK_FIELD(sTexturePart, m_texName, 0x40);
CHECK_FIELD(sTexturePart, m_pBitmap, 0x80);
CHECK_FIELD(sTexturePart, m_rect, 0x84);

// Predicate of the std::find_if over a list of texture parts: matches the
// texture name, case-insensitively.
struct sFindTexPart {
	// texturepart.h:42
	sFindTexPart(const char *name) : m_name(name) {}
	// texturepart.h:45
	bool operator()(const sTexturePart *pPart) const {
		return strcmpi(pPart->m_texName, m_name) == 0;
	}

	const char *m_name;     // 0x0
};

CHECK_SIZE(sFindTexPart, 0x4);

#endif // PG_CLIENT_PROJECTG_TEXTUREPART_H
