// Original: d:\build\custom\temp\code\shared\uselzo.h
#ifndef PG_SHARED_USELZO_H
#define PG_SHARED_USELZO_H

#include "..\defines.h"

// Compresses a buffer with miniLZO 2.02 (or stores it as is when the library
// failed to initialise) into an owned buffer headed by a 4-byte header.
class CCompressBuffer {
public:
	CCompressBuffer(void);    // uselzo.cpp:20
	~CCompressBuffer(void);   // uselzo.cpp:25

	void Init(void);                                     // uselzo.cpp:34
	int Compress(unsigned char *pSrc, int nSize);        // uselzo.cpp:48
	int Decompress(unsigned char *pSrc, int nSize);      // uselzo.cpp:137
	unsigned char *GetNewBuffer(void);                   // uselzo.cpp:262
	int GetNewSize(void);                                // uselzo.cpp:274

private:
	LAYOUT_CHECK_FRIEND;

	int Max(int a, int b) {   // uselzo.h:72
		return a > b ? a : b;
	}

	unsigned char *m_pNewBuffer;   // 0x0, new[]'d, header + data
	bool m_bUsed;                  // 0x4, set once Compress/Decompress ran
	int m_nLzoInit;                // 0x8, lzo_init()'s result (LZO_E_OK = 0)
	int m_nNewSize;                // 0xC, bytes in m_pNewBuffer
};

BEGIN_LAYOUT_CHECK(CCompressBuffer)
	CHECK_SIZE(CCompressBuffer, 0x10);
	CHECK_FIELD(CCompressBuffer, m_pNewBuffer, 0x0);
	CHECK_FIELD(CCompressBuffer, m_bUsed, 0x4);
	CHECK_FIELD(CCompressBuffer, m_nLzoInit, 0x8);
	CHECK_FIELD(CCompressBuffer, m_nNewSize, 0xC);
END_LAYOUT_CHECK

#endif // PG_SHARED_USELZO_H
