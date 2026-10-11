// Original: d:\build\custom\temp\code\shared\uselzo.cpp
#include "uselzo.h"

#include <new>
#include <stdio.h>
#include <string.h>

#include "minilzo/minilzo.h"

// lzo_init()'s result, shared by every CCompressBuffer: 1 until the first
// one initialises the library.
static int s_nLzoInit = 1;

// miniLZO's testmini.c sizes: the work memory is declared OUT_LEN long.
#define IN_LEN      (128 * 1024L)
#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)

#define HEAP_ALLOC(var, size) \
	lzo_align_t __LZO_MMODEL var[((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t)]

// uselzo.cpp:20
CCompressBuffer::CCompressBuffer(void) {
	Init();
}

// uselzo.cpp:25
CCompressBuffer::~CCompressBuffer(void) {
	if (m_pNewBuffer) {
		delete[] m_pNewBuffer;
		m_pNewBuffer = NULL;
	}
}

// uselzo.cpp:34
void CCompressBuffer::Init(void) {
	if (s_nLzoInit == 1) {
		s_nLzoInit = lzo_init();
	}
	m_nLzoInit = s_nLzoInit;
	m_bUsed = false;
	m_nNewSize = 0;
	m_pNewBuffer = NULL;
}

// uselzo.cpp:48
int CCompressBuffer::Compress(unsigned char *pSrc, int nSize) {
	if (m_bUsed) {
		m_nNewSize = 0;
		return 1;
	}

	if (nSize == 0) {
		return 3;
	}

	if (pSrc == NULL) {
		return 2;
	}

	m_bUsed = true;
	if (m_pNewBuffer) {
		delete[] m_pNewBuffer;
		m_pNewBuffer = NULL;
	}

	m_pNewBuffer = new unsigned char[Max(0x1000, nSize * 2)];
	memset(m_pNewBuffer, 0, Max(0x1000, nSize * 2));

	// Header: [0] stored raw (1) or LZO (0), [1..3] the size in base 255.
	m_pNewBuffer[3] = nSize % 255;
	int nRest = (nSize - m_pNewBuffer[3]) / 255; m_pNewBuffer[2] = nRest % 255; m_pNewBuffer[1] = ((nRest - m_pNewBuffer[2]) / 255) % 255;

	if (m_nLzoInit) {
		m_pNewBuffer[0] = 1;
		for (int i = 0; i < nSize; i++) {
			m_pNewBuffer[i + 4] = pSrc[i];
		}
		m_nNewSize = nSize + 4;
		return 4;
	}

	m_pNewBuffer[0] = 0;

	char szMsg[128];
	lzo_uint nOutLen;
	HEAP_ALLOC(wrkmem, OUT_LEN);
	int nResult = lzo1x_1_compress(pSrc, nSize, m_pNewBuffer + 4, &nOutLen, wrkmem);
	if (nResult != LZO_E_OK) {
		sprintf(szMsg, "::COMPRESS -> The result should be zero! (res : %d)", nResult);
	}

	m_nNewSize = nOutLen + 4;
	return 0;
}

// uselzo.cpp:137
int CCompressBuffer::Decompress(unsigned char *pSrc, int nSize) {
	if (m_bUsed) {
		m_nNewSize = 0;
		return 1;
	}

	if (pSrc == NULL) {
		return 2;
	}

	if (nSize == 0) {
		return 3;
	}

	int nOrgSize = pSrc[1] * 65025 + pSrc[2] * 255 + pSrc[3];

	m_bUsed = true;
	if (m_pNewBuffer) {
		delete[] m_pNewBuffer;
		m_pNewBuffer = NULL;
	}

	m_pNewBuffer = new unsigned char[Max(nOrgSize + 10, 0x1000)];
	memset(m_pNewBuffer, 0, Max(nOrgSize + 10, 0x1000));

	if (pSrc[0] == 1) {
		for (int i = 0; i < nSize - 4; i++) {
			m_pNewBuffer[i] = pSrc[i + 4];
		}
		if (nOrgSize == nSize - 4) {
			m_nNewSize = nSize - 4;
		} else {
			m_nNewSize = 0;
			return 5;
		}
		return 0;
	} else if (pSrc[0] == 0) {
		if (m_nLzoInit) {
			m_nNewSize = 0;
			return 7;
		}

		char szMsg[128];
		lzo_uint nOutLen;
		int nResult = lzo1x_decompress(pSrc + 4, nSize - 4, m_pNewBuffer, &nOutLen, NULL);
		if (nResult != LZO_E_OK) {
			sprintf(szMsg, "::DECOMPRESS -> The result should be zero! (res : %d)", nResult);
			return 8;
		}

		if (nOrgSize == nOutLen) {
			m_nNewSize = nOutLen;
		} else {
			m_nNewSize = 0;
			return 6;
		}
		return 0;
	}

	return 9;
}

// uselzo.cpp:262
unsigned char *CCompressBuffer::GetNewBuffer(void) {
	if (m_nNewSize == 0) {
		return NULL;
	}
	return m_pNewBuffer;
}

// uselzo.cpp:274
int CCompressBuffer::GetNewSize(void) {
	return m_nNewSize;
}
