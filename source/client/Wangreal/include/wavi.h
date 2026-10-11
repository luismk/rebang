// Original: d:\build\custom\temp\code\client\wangreal\include\wavi.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WAVI_H
#define PG_CLIENT_WANGREAL_INCLUDE_WAVI_H

#include <windows.h>
#include <vfw.h>

#include "..\..\defines.h"

class Bitmap;

// Result of WAVIEncoder::Close (PDB, size 0x8).
struct sAviResult {
	int totalFrames;    // 0x0
	long fileSize;      // 0x4
};
CHECK_SIZE(sAviResult, 0x8);
CHECK_FIELD(sAviResult, totalFrames, 0x0);
CHECK_FIELD(sAviResult, fileSize, 0x4);

// Writes frames (Bitmaps) to an AVI file through Video for Windows, either
// one at a time or batched through a history of m_hisNum frames
// (PDB layout, size 0x130).
class WAVIEncoder {
	LAYOUT_CHECK_FRIEND;
public:
	WAVIEncoder();
	virtual ~WAVIEncoder();

	bool Open(char* filename, int w, int h, int fps, HWND__* hwnd);
	void Write(Bitmap* bitmap);
	sAviResult Close();
	void SetFrameBuffer(int page);
	bool IsOpened() { return m_bOpened; }

protected:
	void WriteAVI(Bitmap* bitmap);
	void Flush();
	void ScaleBitmap(Bitmap* image, Bitmap* bitmap, int w, int h, int px, int py, bool flip);
	Bitmap* ScaleBitmap(Bitmap* bitmap, int w, int h, int px, int py, bool flip);
	void CopyBitmap(Bitmap* image, Bitmap* bitmap, bool flip);
	void Release();

	IAVIFile* m_pfile;              // 0x4
	IAVIStream* m_ps;               // 0x8
	IAVIStream* m_psCompressed;     // 0xc
	int m_cur_frame;                // 0x10
	int m_width;                    // 0x14
	int m_height;                   // 0x18
	Bitmap** m_history;             // 0x1c
	int m_hisNum;                   // 0x20
	int m_cur;                      // 0x24
	bool m_bOpened;                 // 0x28
	char m_filename[260];           // 0x29
};
CHECK_SIZE(WAVIEncoder, 0x130);
BEGIN_LAYOUT_CHECK(WAVIEncoder)
	CHECK_FIELD(WAVIEncoder, m_pfile, 0x4);
	CHECK_FIELD(WAVIEncoder, m_ps, 0x8);
	CHECK_FIELD(WAVIEncoder, m_psCompressed, 0xc);
	CHECK_FIELD(WAVIEncoder, m_cur_frame, 0x10);
	CHECK_FIELD(WAVIEncoder, m_width, 0x14);
	CHECK_FIELD(WAVIEncoder, m_height, 0x18);
	CHECK_FIELD(WAVIEncoder, m_history, 0x1c);
	CHECK_FIELD(WAVIEncoder, m_hisNum, 0x20);
	CHECK_FIELD(WAVIEncoder, m_cur, 0x24);
	CHECK_FIELD(WAVIEncoder, m_bOpened, 0x28);
	CHECK_FIELD(WAVIEncoder, m_filename, 0x29);
END_LAYOUT_CHECK

#endif
