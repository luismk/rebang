// Original: d:\build\custom\temp\code\client\wangreal\source\bitmap.cpp
#include <stdio.h>
#include <new>
#include <math.h>

#include "../include/bitmap.h"

// bitmap.cpp:18
Bitmap::Bitmap(void) {
	bi = 0;
	vram = 0;
	m_lock = false;
}

// bitmap.cpp:25
Bitmap::Bitmap(int w, int h, int bpp) {
	bi = 0;
	vram = 0;
	m_lock = false;
	Create(w, h, bpp);
}

// bitmap.cpp:33
Bitmap::~Bitmap(void) {
	if (!m_lock && bi) {
		delete[] bi;
	}
}

// bitmap.cpp:39
Bitmap& Bitmap::operator=(Bitmap& bitmap) {
	if (bi == 0 || Width() != bitmap.Width() || Height() != bitmap.Height() || bi->bmiHeader.biBitCount != bitmap.bi->bmiHeader.biBitCount) {
		Create(bitmap.Width(), bitmap.Height(), bitmap.BitsPerPixel());
	}
	bitmap.Update();

	if (bi->bmiHeader.biBitCount <= 8) {
		memcpy(bi->bmiColors, bitmap.bi->bmiColors, sizeof(RGBQUAD) * (bi->bmiHeader.biClrUsed ? bi->bmiHeader.biClrUsed : 256));
	}
	memcpy(vram, bitmap.vram, Size());

	return *this;
}

// bitmap.cpp:57
void Bitmap::GetPixel(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const {
	if (x >= 0 && y >= 0 && x < bi->bmiHeader.biWidth && y < bi->bmiHeader.biHeight) {
		if (bi) {
			if (bi->bmiHeader.biBitCount == 32) {
				a = vram[pitch * y + x * 4 + 3];
				r = vram[pitch * y + x * 4 + 2];
				g = vram[pitch * y + x * 4 + 1];
				b = vram[pitch * y + x * 4];
			} else if (bi->bmiHeader.biBitCount == 24) {
				a = 255;
				r = vram[pitch * y + x * 3 + 2];
				g = vram[pitch * y + x * 3 + 1];
				b = vram[pitch * y + x * 3];
			}
		}
	}
}

// bitmap.cpp:85
void Bitmap::GetPixel(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b) const {
	if (x < 0 || y < 0 || x >= bi->bmiHeader.biWidth || y >= bi->bmiHeader.biHeight) {
		return;
	}

	if (bi == 0) {
		return;
	}
	if (bi->bmiHeader.biBitCount == 8) {
		unsigned char index = (vram + pitch * y)[x];
		r = bi->bmiColors[index].rgbRed;
		g = bi->bmiColors[index].rgbGreen;
		b = bi->bmiColors[index].rgbBlue;
	}
	if (bi->bmiHeader.biBitCount == 24) {
		r = (vram + pitch * y)[x * 3 + 2];
		g = (vram + pitch * y)[x * 3 + 1];
		b = (vram + pitch * y)[x * 3];
	}
	if (bi->bmiHeader.biBitCount == 32) {
		r = (vram + pitch * y)[x * 4 + 2];
		g = (vram + pitch * y)[x * 4 + 1];
		b = (vram + pitch * y)[x * 4];
	}
}

// bitmap.cpp:121
void Bitmap::SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	if (x < 0 || y < 0 || x >= bi->bmiHeader.biWidth || y >= bi->bmiHeader.biHeight) {
		return;
	}

	if (bi == 0) {
		return;
	}
	if (bi->bmiHeader.biBitCount == 8) {
		unsigned char index = (vram + pitch * y)[x];
		bi->bmiColors[index].rgbRed = r;
		bi->bmiColors[index].rgbGreen = g;
		bi->bmiColors[index].rgbBlue = b;
	} else if (bi->bmiHeader.biBitCount == 24) {
		unsigned char* p = &vram[pitch * y + x * 3];
		*p++ = b;
		*p++ = g;
		*p = r;
	} else if (bi->bmiHeader.biBitCount == 32) {
		unsigned char* p = &vram[pitch * y + x * 4];
		*p++ = b;
		*p++ = g;
		*p++ = r;
		*p = a;
	}
}

// bitmap.cpp:162
unsigned char Bitmap::GetAlpha(int x, int y) {
	if (x < 0 || y < 0 || x >= bi->bmiHeader.biWidth || y >= bi->bmiHeader.biHeight) {
		return 255;
	}

	if (bi == 0) {
		return 255;
	}
	if (bi->bmiHeader.biBitCount == 32) {
		return vram[pitch * y + x * 4 + 3];
	}
	return 255;
}

// bitmap.cpp:183
void Bitmap::SetAlpha(int x, int y, unsigned char a) {
	if (x < 0 || y < 0 || x >= bi->bmiHeader.biWidth || y >= bi->bmiHeader.biHeight) {
		return;
	}

	if (bi == 0) {
		return;
	}
	if (bi->bmiHeader.biBitCount == 32) {
		vram[pitch * y + x * 4 + 3] = a;
	}
}

// bitmap.cpp:201
void Bitmap::SetPalette(unsigned char* pal) {
	for (int i = 0; i < 256; i++, pal += 3) {
		bi->bmiColors[i].rgbBlue = pal[2];
		bi->bmiColors[i].rgbGreen = pal[1];
		bi->bmiColors[i].rgbRed = pal[0];
	}
}

// bitmap.cpp:212
void Bitmap::SetGrayPalette(unsigned char* pal) {
	int i;

	if (pal) {
		for (i = 0; i < 256; i++) {
			bi->bmiColors[i].rgbBlue = (pal[2] + i < 255) ? pal[2] + i : 255;
			bi->bmiColors[i].rgbGreen = (pal[1] + i < 255) ? pal[1] + i : 255;
			bi->bmiColors[i].rgbRed = (pal[0] + i < 255) ? pal[0] + i : 255;
		}
	} else {
		for (i = 0; i < 256; i++) {
			bi->bmiColors[i].rgbBlue = bi->bmiColors[i].rgbGreen = bi->bmiColors[i].rgbRed = i;
		}
	}
}

// bitmap.cpp:232
void Bitmap::SetBITMAPINFO(tagBITMAPINFO* _bi, unsigned char* _vram) {
	bi = _bi;
	vram = _vram;
	m_lock = true;
	pitch = ((bi->bmiHeader.biBitCount * bi->bmiHeader.biWidth) / 8 + 3) & ~3;
}

// bitmap.cpp:240
void Bitmap::Create(int w, int h, int bpp) {
	if (!m_lock && bi) {
		delete[] bi;
	}
	pitch = ((bpp / 8) * w + 3) & ~3;
	int headerSize = sizeof(BITMAPINFOHEADER) + (bpp > 8 ? 0 : 256 * sizeof(RGBQUAD));

	bi = (tagBITMAPINFO*)new unsigned char[headerSize + pitch * h];
	m_lock = false;

	memset(bi, 0, headerSize);

	bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi->bmiHeader.biPlanes = 1;
	bi->bmiHeader.biCompression = BI_RGB;
	bi->bmiHeader.biSizeImage = 0;
	bi->bmiHeader.biClrUsed = 0;
	bi->bmiHeader.biWidth = w;
	bi->bmiHeader.biHeight = h;
	bi->bmiHeader.biBitCount = bpp;

	vram = VMem(bi);
}

// bitmap.cpp:268
void Bitmap::Save(const char* filename) const {
	struct bmp_info {
		long size;          // 0x0
		long width;         // 0x4
		long height;        // 0x8
		short planes;       // 0xc
		short bitcount;     // 0xe
		long compression;   // 0x10
		long sizeimage;     // 0x14
		long xpelspermeter; // 0x18
		long ypelspermeter; // 0x1c
		long clrused;       // 0x20
		long clrimportant;  // 0x24
	};
	bmp_info info = { 40, 0, 0, 1, 24, 0, 0, 0, 0, 0, 0 };
	int y;
	int x;

#pragma pack(push, 1)
	struct bmp_head {
		char signature[2]; // 0x0
		long size;         // 0x2
		short reserved1;   // 0x6
		short reserved2;   // 0x8
		long offbits;      // 0xa
	};
#pragma pack(pop)
	bmp_head header = { { 'B', 'M' }, sizeof(bmp_head), 0, 0, sizeof(bmp_head) + sizeof(bmp_info) };

	FILE* fp = fopen(filename, "wb");
	if (fp) {
		info.width = bi->bmiHeader.biWidth;
		info.height = bi->bmiHeader.biHeight;
		fwrite(&header, 1, sizeof(header), fp);
		fwrite(&info, 1, sizeof(info), fp);

		for (y = info.height - 1; y >= 0; y--) {
			for (x = 0; x < info.width; x++) {
				const unsigned char* p = &vram[pitch * y + x * 3];
				fputc(p[0], fp);
				fputc(p[1], fp);
				fputc(p[2], fp);
			}
			int pad = ((info.width * 3 + 3) & ~3) - info.width * 3;
			for (x = 0; x < pad; x++) {
				fputc(0, fp);
			}
		}
		fclose(fp);
	}
}

// bitmap.cpp:322
void Bitmap::PaintStretch(HWND__* hwnd, tagRECT* rect) {
	HDC dc = GetDC(hwnd);

	StretchDIBits(dc, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, 0, 0, bi->bmiHeader.biWidth, abs(bi->bmiHeader.biHeight), vram, bi, DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(hwnd, dc);
}

// bitmap.cpp:332
void Bitmap::Paint(HWND__* hwnd, tagPOINT* p) {
	int w;
	int h;
	HDC dc = GetDC(hwnd);

	w = bi->bmiHeader.biWidth;
	h = bi->bmiHeader.biHeight;
	if (p) {
		SetDIBitsToDevice(dc, p->x, p->y, w, h, 0, h, h, h, vram, bi, DIB_RGB_COLORS);
	} else {
		SetDIBitsToDevice(dc, 0, 0, w, h, 0, h, h, h, vram, bi, DIB_RGB_COLORS);
	}
	ReleaseDC(hwnd, dc);
}
