// Original: d:\build\custom\temp\code\client\projectg\videoin.h
#ifndef PG_CLIENT_PROJECTG_VIDEOIN_H
#define PG_CLIENT_PROJECTG_VIDEOIN_H

#include <windows.h>
#include "..\..\defines.h"

namespace ntg {

// One captured frame. Layout from its constructor and destructor
// (videoin.h:22-32) and from Compressor (avirecorder.cpp).
class VideoFrame {
	LAYOUT_CHECK_FRIEND;
public:
	VideoFrame(void) {
		m_pData = 0;
		m_bInUse = 0;
	}

	virtual ~VideoFrame(void) {
		if (m_pData)
			delete[] m_pData;
	}

	BYTE *m_pData;     // 0x04, the frame's pixels
	int m_nFrameCount; // 0x08, number of frame periods the frame lasts
	int m_bInUse;      // 0x0C, set while the compressor holds the frame
};

// A video source (the capture of a window). Interface from its calls in
// AviRecorder (avirecorder.cpp); no implementation is named by the PDB.
class VideoIn {
public:
	virtual ~VideoIn(void) {}
	virtual bool start(void *pWindow) = 0;                 // slot 1
	virtual void stop(void) = 0;                           // slot 2
	virtual void capture(void) = 0;                        // slot 3
	virtual VideoFrame *get_frame(void) = 0;               // slot 4
	virtual BITMAPINFOHEADER const *get_format(void) = 0;  // slot 5
};

} // namespace ntg

CHECK_SIZE(ntg::VideoFrame, 0x10);
BEGIN_LAYOUT_CHECK(ntg::VideoFrame)
	CHECK_FIELD(ntg::VideoFrame, m_pData, 0x04);
	CHECK_FIELD(ntg::VideoFrame, m_nFrameCount, 0x08);
	CHECK_FIELD(ntg::VideoFrame, m_bInUse, 0x0C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_VIDEOIN_H
