// Original: d:\build\custom\temp\code\client\projectg\ucccolorpicker.cpp
#include <windows.h>

#include "ucccolorpicker.h"
#include "UccLibrary.h"
#include "bitmap.h"
#include "wresrcmng.h"
#include "frarea.h"

// ucccolorpicker.cpp:13
CUccColorPicker::CUccColorPicker(void) {
	m_red = m_green = m_blue = 0;
}

// ucccolorpicker.cpp:17
CUccColorPicker::~CUccColorPicker(void) {
	DestroyVariables();
}

// ucccolorpicker.cpp:22
void CUccColorPicker::DestroyVariables(void) {
	m_red = 0;
	m_green = 0;
	m_blue = 0;
}

// ucccolorpicker.cpp:31
void CUccColorPicker::Initialize(FrArea* pArea) {
	CUccBase::Initialize(pArea, NULL, NULL);

	GenerateColorMap();
}

// ucccolorpicker.cpp:38
void CUccColorPicker::GetColor(const WPoint& point, unsigned char& red, unsigned char& green, unsigned char& blue) {
	WPoint pos(m_pArea->GetRect().x, m_pArea->GetRect().y);

	GetColor((int)(point.x - pos.x), (int)(point.y - pos.y), red, green, blue);
}

// ucccolorpicker.cpp:45
void CUccColorPicker::GetColor(int x, int y, unsigned char& red, unsigned char& green, unsigned char& blue) {
	if (x >= 0 && y >= 0 && (unsigned int)x < m_pTexture32->Width() && (unsigned int)y < m_pTexture32->Height()) {
		m_pTexture32->GetPixel(x, y, red, green, blue);
		m_red = red;
		m_green = green;
		m_blue = blue;
	}
}

// ucccolorpicker.cpp:60
void CUccColorPicker::GenerateColorMap(void) {
	ClearTexCache(m_pTexture32);
	delete m_pTexture32;
	m_pTexture32 = NULL;
	m_pTexture32 = g_resrcmng->LoadBitmap("ucc_color_map.tga", 0, false);
	if (m_pTexture32 != NULL) {
		RefreshTexCache(m_pTexture32);
	}
}

// ucccolorpicker.cpp:78
CUccColorBrightness::CUccColorBrightness(void) {
}

// ucccolorpicker.cpp:82
CUccColorBrightness::~CUccColorBrightness(void) {
}

// ucccolorpicker.cpp:86
void CUccColorBrightness::Initialize(FrArea* pArea) {
	CUccBase::Initialize(pArea, NULL, NULL);

	int width = (int)m_pArea->GetRect().w, height = (int)m_pArea->GetRect().h;

	ClearTexCache(m_pTexture32); delete m_pTexture32; m_pTexture32 = NULL;
	m_pTexture32 = new Bitmap(width, height, 32);
}

// ucccolorpicker.cpp:99
void CUccColorBrightness::GenerateBrightnessMap(unsigned char red, unsigned char green, unsigned char blue) {
	unsigned char hue, lightness, saturation;
	RGBToHLS(red, green, blue, hue, lightness, saturation);
	lightness = 128;
	HLSToRGB(hue, lightness, saturation, red, green, blue);

	const WRect& rect = m_pArea->GetRect();
	int width = (int)rect.w, height = (int)rect.h;

	// upper half: from white down to the colour
	int y = 0;
	int lastY = 0;

	int half = height / 2;
	int rowsPerStep = 1; if (half / 11 >= 1) rowsPerStep = half / 11;
	double stepRows = rowsPerStep;

	double r = 255.0, g = 255.0, b = 255.0;
	double dr = (255.0 - red) * (-1.0 / 11.0);
	double dg = (255.0 - green) * (-1.0 / 11.0);
	double db = (255.0 - blue) * (-1.0 / 11.0);

	do {
		for (int i = 0; i < stepRows; ++i, ++y) {
			if (y > half) {
				break;
			}

			for (int x = 0; x < width; ++x) {
				m_pTexture32->SetPixel(x, y, (unsigned char)r, (unsigned char)g, (unsigned char)b, 0xff);
				lastY = y;
			}
		}

		r += dr; g += dg; b += db;
	} while (y <= half);

	// lower half: from the colour down to black
	r = red; g = green; b = blue;
	dr = r * (-1.0 / 11.0);
	dg = g * (-1.0 / 11.0);
	db = b * (-1.0 / 11.0);

	y = lastY;

	do {
		for (int i = 0; i < stepRows; ++i, ++y) {
			if (y >= height) {
				break;
			}

			for (int x = 0; x < width; ++x) {
				m_pTexture32->SetPixel(x, y, (unsigned char)r, (unsigned char)g, (unsigned char)b, 0xff);
			}
		}

		r += dr; g += dg; b += db;
	} while (y < height);

	RefreshTexCache(m_pTexture32);

	m_pArea->SetBgImg(m_pTexture32);
}

// ucccolorpicker.h:18
void CUccColorPicker::Process(void) {
}

// ucccolorpicker.h:20
void CUccColorPicker::MouseEventHandler(eButton button, int state, const WPoint &point) {
}

// ucccolorpicker.h:27
void CUccColorPicker::Refresh2D(void) {
}

// ucccolorpicker.h:28
void CUccColorPicker::Refresh3D(void) {
}

// ucccolorpicker.h:48
void CUccColorBrightness::GenerateColorMap(void) {
}
