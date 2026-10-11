// Original: d:\build\custom\temp\code\client\projectg\titlefontex.cpp
#include "TitleFontEx.h"
#include "../client/Wangreal/include/wfont.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wview.h"
#include <string.h>

// Texture file of each font, one page each (data_a7fa38: one array, the
// fonts take the address of their entry).
static char* s_fontFiles[] = {
	"[font_wind.jpg",
	"pot_numbers.tga",
	"blue_wind_s.tga",
	"blue_wind_b.tga",
	"[font_hole_skins_blue.jpg",
	"[font_bar.jpg"
};

// titlefontex.cpp:18
WTitleFontEx::WTitleFontEx(void) : m_pFont(NULL), m_color(0xffffffff) {
}

// titlefontex.cpp:23
WTitleFontEx::~WTitleFontEx(void) {
	if (g_resrcmng && m_pFont) { g_resrcmng->Release(m_pFont); m_pFont = NULL; }
}

// titlefontex.cpp:28
float WTitleFontEx::Print(float x, float y, const char* text) const {
	return m_pFont->Print(g_view, x, y, text, 0, m_color, NULL);
}

// titlefontex.cpp:33
void WTitleFontEx::Print(float x, float y, const char* text, eTITLEFONTALINE align, float space, float scale) const {
	if (text == NULL) {
		return;
	}

	const char* p = text;
	char* charSet = m_pFont->GetFontInfo()->pCharSet;
	float oldScale = m_pFont->GetScale();
	m_pFont->SetScale(scale);

	float offset = 0.0f;
	switch (align) {
	case TITLEFONT_ALIGN_CENTER:
		for (; *p; ) {
			char c = *p++;
			const char* pos = strchr(charSet, c);
			offset += m_pFont->GetCharWidth(g_view, pos - charSet) + space * scale;
		}
		p = text;
		offset -= space;
		offset *= 0.5f;
		break;
	case TITLEFONT_ALIGN_RIGHT:
		for (; *p; ) {
			char c = *p++;
			const char* pos = strchr(charSet, c);
			offset += m_pFont->GetCharWidth(g_view, pos - charSet) + space * scale;
		}
		p = text;
		offset -= space;
		break;
	}

	x -= offset;

	while (*p) {
		char c = *p++;
		const char* pos = strchr(charSet, c);
		float width = m_pFont->PutChar(g_view, x, y, pos - charSet, 0, m_color);
		x += width + space;
	}

	m_pFont->SetScale(oldScale);
}

// titlefontex.cpp:91
float WTitleFontEx::GetFontH(void) const { return (float)m_pFont->GetFontInfo()->fonth; }
// titlefontex.cpp:92
float WTitleFontEx::GetFontW(void) const { return (float)m_pFont->GetFontInfo()->fontw; }

// titlefontex.cpp:95
int WTitleFontEx::GetCharIndex(char c) {
	tagWTITLEFONT* info = m_pFont->GetFontInfo();
	int len = strlen(info->pCharSet);
	for (int i = 0; i < len; i++) {
		if (info->pCharSet[i] == c) {
			return i;
		}
	}

	return 0;
}

// titlefontex.cpp:108
float WTitleFontEx::GetStringWidth(const char* text) {
	if (text == NULL) {
		return 0.0f;
	}

	float width = 0.0f;
	int len = strlen(text);
	for (int i = 0; i < len; i++) {
		width += m_pFont->GetCharWidth(g_view, GetCharIndex(text[i]));
	}

	return width;
}

// titlefontex.cpp:122
CWindFont::CWindFont(void) {
	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));

	info.filename = &s_fontFiles[0];
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:,x";

	m_pFont = g_resrcmng->GetTitleFont();
	m_pFont->Create(&info);
}

// titlefontex.cpp:140
COpenTournamentFont::COpenTournamentFont(void) {
	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));

	info.filename = &s_fontFiles[1];
	info.fontw = 45;
	info.fonth = 43;
	info.numPages = 1;
	info.texw = 225;
	info.texh = 86;
	info.pCharSet = "1234567890";

	m_pFont = g_resrcmng->GetTitleFont();
	m_pFont->Create(&info);
}

// titlefontex.cpp:158
CBlueWindFontSmall::CBlueWindFontSmall(void) {
	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));

	info.filename = &s_fontFiles[2];
	info.fontw = 18;
	info.fonth = 21;
	info.numPages = 1;
	info.texw = 108;
	info.texh = 42;
	info.pCharSet = "0123456789~";

	m_pFont = g_resrcmng->GetTitleFont();
	m_pFont->Create(&info);
}

// titlefontex.cpp:176
CBlueWindFontBig::CBlueWindFontBig(void) {
	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));

	info.filename = &s_fontFiles[3];
	info.fontw = 29;
	info.fonth = 33;
	info.numPages = 1;
	info.texw = 174;
	info.texh = 66;
	info.pCharSet = "0123456789+-";

	m_pFont = g_resrcmng->GetTitleFont();
	m_pFont->Create(&info);
}

// titlefontex.cpp:194
CBlueHoleSkinFont::CBlueHoleSkinFont(void) {
	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));

	info.filename = &s_fontFiles[4];
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 32;
	info.pCharSet = "1234567890+,";

	m_pFont = g_resrcmng->GetTitleFont();
	m_pFont->Create(&info);
}

// titlefontex.cpp:213
CBarFont::CBarFont(void) {
	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));

	info.filename = &s_fontFiles[5];
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 32;
	info.pCharSet = "1234567890ym%-.";

	m_pFont = g_resrcmng->GetTitleFont();
	m_pFont->Create(&info);
}
