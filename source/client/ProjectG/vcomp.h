// Original: d:\build\custom\temp\code\client\projectg\vcomp.h
#ifndef PG_CLIENT_PROJECTG_VCOMP_H
#define PG_CLIENT_PROJECTG_VCOMP_H

#include <windows.h>
#include <vfw.h>
#include "..\..\defines.h"

namespace ntg {

// Compresses video frames through the Video Compression Manager (vcomp.cpp).
class VideoCompressor {
	LAYOUT_CHECK_FRIEND;
public:
	VideoCompressor(void);
	virtual ~VideoCompressor(void);

	bool init(BITMAPINFO const *pInFormat, int nQuality);
	void cleanup(void);
	bool start(void);
	void stop(void);
	void *compress(void *pBits, int *pSize);
	bool get_format(BITMAPINFO *pOutFormat);
	// vcomp.h:33
	unsigned long get_fcc_handler(void) const { return m_cv.fccHandler; }

private:
	COMPVARS m_cv;      // 0x04
	BITMAPINFO m_bmiIn; // 0x44, the input frame format
};

} // namespace ntg

CHECK_SIZE(ntg::VideoCompressor, 0x70);
BEGIN_LAYOUT_CHECK(ntg::VideoCompressor)
	CHECK_FIELD(ntg::VideoCompressor, m_cv, 0x04);
	CHECK_FIELD(ntg::VideoCompressor, m_bmiIn, 0x44);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_VCOMP_H
