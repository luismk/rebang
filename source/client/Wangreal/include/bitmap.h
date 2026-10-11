// Original: d:\build\custom\temp\code\client\wangreal\include\bitmap.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_BITMAP_H
#define PG_CLIENT_WANGREAL_INCLUDE_BITMAP_H

#include <windows.h>

#include "..\..\defines.h"

// A DIB in system memory (PDB layout, size 0x14). The bodies live in
// bitmap.cpp (Wangreal) except the inline accessors.
class Bitmap {
	LAYOUT_CHECK_FRIEND;
public:
	Bitmap(void);
	Bitmap(int width, int height, int bpp);
	virtual ~Bitmap(void);
	virtual void Update(void) {}                                                     // bitmap.h:64

	Bitmap& operator=(Bitmap& src);
	void Create(int width, int height, int bpp);
	void SetBITMAPINFO(tagBITMAPINFO* info, unsigned char* bits);
	void SetPalette(unsigned char* palette);
	void SetGrayPalette(unsigned char* palette);
	void GetPixel(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const;
	void GetPixel(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b) const;
	void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned char GetAlpha(int x, int y);
	void SetAlpha(int x, int y, unsigned char a);
	unsigned char* GetVram(int y) { return vram + pitch * y; }                       // bitmap.h:72
	void Save(const char* filename) const;
	void PaintStretch(HWND__* hwnd, tagRECT* rect);
	void Paint(HWND__* hwnd, tagPOINT* pt);
	unsigned int Width(void) const { return bi->bmiHeader.biWidth; }                 // bitmap.h:80
	unsigned int Height(void) const { return bi->bmiHeader.biHeight; }               // bitmap.h:81
	unsigned int BitsPerPixel(void) const { return bi->bmiHeader.biBitCount; }       // bitmap.h:82
	unsigned int Size(void) const { return bi->bmiHeader.biHeight * pitch; }         // bitmap.h:83

	tagBITMAPINFO* bi;       // 0x4
	int pitch;               // 0x8
	unsigned char* vram;     // 0xc

private:
	unsigned char* VMem(tagBITMAPINFO* info) {                                        // bitmap.h:93
		return (unsigned char*)info + sizeof(BITMAPINFOHEADER) + (info->bmiHeader.biBitCount > 8 ? 0 : (info->bmiHeader.biClrUsed ? info->bmiHeader.biClrUsed : 256) * sizeof(RGBQUAD));
	}

	bool m_lock;             // 0x10
};

CHECK_SIZE(Bitmap, 0x14);
BEGIN_LAYOUT_CHECK(Bitmap)
	CHECK_FIELD(Bitmap, bi, 0x4);
	CHECK_FIELD(Bitmap, pitch, 0x8);
	CHECK_FIELD(Bitmap, vram, 0xc);
	CHECK_FIELD(Bitmap, m_lock, 0x10);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_BITMAP_H
