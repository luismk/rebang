// Original: d:\build\custom\temp\code\client\projectg\vcomp.cpp
#include "vcomp.h"

#include <string.h>

namespace ntg {

// vcomp.cpp:32
VideoCompressor::VideoCompressor(void) {
	memset(&m_cv, 0, sizeof(m_cv));
	memset(&m_bmiIn, 0, sizeof(m_bmiIn));
}

// vcomp.cpp:38
VideoCompressor::~VideoCompressor(void) {
}

// vcomp.cpp:42
bool VideoCompressor::init(BITMAPINFO const *pInFormat, int nQuality) {
	m_bmiIn = *pInFormat;

	memset(&m_cv, 0, sizeof(m_cv));

	m_cv.cbSize = sizeof(m_cv);
	m_cv.dwFlags = ICMF_COMPVARS_VALID;
	m_cv.fccType = ICTYPE_VIDEO;
	m_cv.fccHandler = mmioFOURCC('m', 's', 'v', 'c');
	m_cv.lQ = nQuality * 100;

	m_cv.hic = ICOpen(ICTYPE_VIDEO, mmioFOURCC('m', 's', 'v', 'c'), ICMODE_FASTCOMPRESS);

	return m_cv.hic != 0 ? true : false;
}

// vcomp.cpp:66
void VideoCompressor::cleanup(void) {
	ICCompressorFree(&m_cv);

	if (m_cv.hic) {
		ICClose(m_cv.hic);
		m_cv.hic = 0;
	}
}

// vcomp.cpp:76
bool VideoCompressor::start(void) {
	return ICSeqCompressFrameStart(&m_cv, &m_bmiIn) == TRUE;
}

// vcomp.cpp:81
void VideoCompressor::stop(void) {
	ICSeqCompressFrameEnd(&m_cv);
}

// vcomp.cpp:86
void *VideoCompressor::compress(void *pBits, int *pSize) {
	BOOL bKey;
	LONG lSize;

	void *pResult = ICSeqCompressFrame(&m_cv, 0, pBits, &bKey, &lSize);

	if (pResult) {
		*pSize = lSize;
	}

	return pResult;
}

// vcomp.cpp:102
bool VideoCompressor::get_format(BITMAPINFO *pOutFormat) {
	return ICCompressGetFormat(m_cv.hic, &m_bmiIn, pOutFormat) == ICERR_OK;
}

} // namespace ntg
