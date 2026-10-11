// Original: d:\build\custom\temp\code\client\projectg\uccbrush.cpp
#include "UccBrush.h"
#include "UccLibrary.h"
#include "wresrcmng.h"
#include <stdio.h>

// uccbrush.cpp:7
sUccCurrentBrush* UccCurrentBrush(void) {
	static sUccCurrentBrush s_currentBrush;
	return &s_currentBrush;
}

// uccbrush.cpp:13
CUccBrush* UccBrushList(eBrushType type) {
	static CUccBrush s_brushList[4];
	static bool s_initialized = false;
	if (!s_initialized) {
		s_initialized = true;
		for (int i = 0; i < 4; i++) {
			s_brushList[i].Initialize((eBrushType)i);
		}
	}
	return &s_brushList[type];
}

// uccbrush.cpp:29
CUccBrush::CUccBrush(void) {
}

// uccbrush.cpp:33
CUccBrush::~CUccBrush(void) {
	Clear();
}

// uccbrush.cpp:38
void CUccBrush::Initialize(eBrushType type) {
	Clear();
	m_type = type;

	int index = 0;
	for (;;) {
		char fileName[256];

		switch (m_type) {
		case 0:
			sprintf(fileName, "ucc_brush%02d.tga", index++);
			break;
		case 1:
			sprintf(fileName, "ucc_pen%02d.tga", index++);
			break;
		case 2:
			sprintf(fileName, "ucc_eraser%02d.tga", index++);
			break;
		case 3:
			sprintf(fileName, "ucc_pattern%02d.tga", index++);
			break;
		}

		Bitmap* pBrush = LoadBrushFromFile(fileName); if (pBrush == NULL) break;

		AddBrush(pBrush);
		ClearTexCache(pBrush); delete pBrush;
	}
}

// uccbrush.cpp:86
void CUccBrush::Clear(void) {
	for (unsigned int i = 0; i < m_brushes.size(); i++) {
		ClearTexCache(m_brushes[i]); if (m_brushes[i] != NULL) { delete m_brushes[i]; } m_brushes[i] = NULL;
	}
	m_brushes.clear();
}

// uccbrush.cpp:95
Bitmap* CUccBrush::GetBrush(int index) {
	if (index < m_brushes.size()) {
		return m_brushes[index];
	}
	return NULL;
}

// uccbrush.cpp:107
void CUccBrush::SetBrush(int index, Bitmap* pBrush) {
	if (index < m_brushes.size()) {
		ClearTexCache(m_brushes[index]); if (m_brushes[index] != NULL) { delete m_brushes[index]; } m_brushes[index] = NULL;
		Bitmap* pNew = new Bitmap;
		*pNew = *pBrush;
		m_brushes[index] = pNew;
	}
}

// uccbrush.cpp:118
Bitmap* CUccBrush::LoadBrushFromFile(const char* fileName) {
	Bitmap* pBrush = g_resrcmng->LoadBitmap(fileName, 0, false);
	if (pBrush != NULL && pBrush->BitsPerPixel() == 32) {
		return pBrush;
	}
	return NULL;
}

// uccbrush.cpp:132
void CUccBrush::AddBrush(Bitmap* pBrush) {
	if (pBrush == NULL) {
		return;
	}

	Bitmap* pNew = new Bitmap;
	if (pBrush->BitsPerPixel() == 32) {
		*pNew = *pBrush;
		m_brushes.push_back(pNew);
	} else if (pBrush->BitsPerPixel() == 8) {
		pNew->Create(pBrush->Width(), pBrush->Height(), 32);
		for (unsigned int y = 0; y < pBrush->Height(); y++) {
			for (unsigned int x = 0; x < pBrush->Width(); x++) {
				unsigned char r, g, b;
				pBrush->GetPixel(x, y, r, g, b);
				if (r == 0 && g == 0 && b == 0) {
					pNew->SetPixel(x, y, 0, 0, 0, 0xff);
				} else {
					pNew->SetPixel(x, y, 0xff, 0xff, 0xff, 0);
				}
			}
		}
		m_brushes.push_back(pNew);
	}
}

// uccbrush.cpp:172
void CUccBrush::RemoveBrush(int index) {
	if (index < m_brushes.size()) {
		ClearTexCache(m_brushes[index]); if (m_brushes[index] != NULL) { delete m_brushes[index]; } m_brushes[index] = NULL;

		for (unsigned int i = index; i < m_brushes.size() - 1; i++) {
			m_brushes[i] = m_brushes[i + 1];
		}

		m_brushes[m_brushes.size() - 1] = NULL;

		m_brushes.resize(m_brushes.size() - 1);
	}
}

// uccbrush.cpp:191
sUccCurrentBrush::sUccCurrentBrush(void)
	: m_brushType((eBrushType)0), m_brushIndex(0), m_r(0), m_g(0), m_b(0) {
	SetBrush((eBrushType)0, 0, 0, 0, 0);
}

// uccbrush.cpp:201
void sUccCurrentBrush::SetBrush(eBrushType type, int index) {
	SetBrush(type, index, m_r, m_g, m_b);
}

// uccbrush.cpp:206
void sUccCurrentBrush::SetBrush(unsigned char r, unsigned char g, unsigned char b) {
	SetBrush(m_brushType, m_brushIndex, r, g, b);
}

// uccbrush.cpp:211
void sUccCurrentBrush::SetBrush(eBrushType type, int index, unsigned char r, unsigned char g, unsigned char b) {
	Bitmap* pSrc = UccBrushList(type)->GetBrush(index);
	if (pSrc == NULL) {
		return;
	}

	if (type == 2) {
		r = 0xff;
		g = 0xff;
		b = 0xff;
	}

	m_brush.Create(0x18, 0x18, 0x20);

	for (int y = 0; y < 0x18; y++) {
		for (int x = 0; x < 0x18; x++) {
			unsigned char srcR, srcG, srcB, srcA;
			pSrc->GetPixel(x, y, srcR, srcG, srcB, srcA);
			if (srcR == 0 && srcG == 0 && srcB == 0) {
				m_brush.SetPixel(x, y, r, g, b, srcA);
			} else {
				m_brush.SetPixel(x, y, 0, 0, 0, 0);
			}
		}
	}

	if (type != 2) {
		m_brushType = type;
		m_brushIndex = index;
		m_r = r; m_g = g;
		m_b = b;
	}
}

// uccbrush.cpp:256
void sUccCurrentBrush::SetBrush(eBrushType type, Bitmap* pBrush) {
	if (pBrush != NULL) {
		m_brushType = type;
		m_brush = *pBrush;
	}
}

// uccbrush.cpp:265
void sUccCurrentBrush::GetBrushColor(unsigned char& r, unsigned char& g, unsigned char& b) {
	r = m_r;
	g = m_g;
	b = m_b;
}
