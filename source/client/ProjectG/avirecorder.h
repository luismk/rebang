// Original: d:\build\custom\temp\code\client\projectg\avirecorder.h
#ifndef PG_CLIENT_PROJECTG_AVIRECORDER_H
#define PG_CLIENT_PROJECTG_AVIRECORDER_H

#include "..\..\defines.h"

namespace ntg {

class AudioIn;
class Compressor;
class VideoIn;
class Writer;

// Records the game window into an AVI file. Layout from the constructor
// (avirecorder.cpp:344) and stop (avirecorder.cpp:414).
class AviRecorder {
	LAYOUT_CHECK_FRIEND;
public:
	AviRecorder(void);
	virtual ~AviRecorder(void);

	bool init(void *pWindow);
	void cleanup(void);
	bool start(char const *pszFileName, int nQuality, int nScale, int nFps);
	void stop(void);
	void record(void);
	void unit_test(void);

private:
	void *m_pWindow;          // 0x04, set by init
	VideoIn *m_pVideoSource;  // 0x08
	AudioIn *m_pAudioIn;      // 0x0C
	Compressor *m_pCompressor; // 0x10
	Writer *m_pWriter;        // 0x14
	int m_nFps;               // 0x18
	int m_nScale;             // 0x1C, divisor of the captured size
	bool m_bRecording;        // 0x20, tested and cleared by stop
	bool m_bFirstFrame;       // 0x21, set by start, cleared by the first record
	unsigned long m_dwLastTime; // 0x24, timeGetTime of the last recorded frame
};

} // namespace ntg

CHECK_SIZE(ntg::AviRecorder, 0x28);
BEGIN_LAYOUT_CHECK(ntg::AviRecorder)
	CHECK_FIELD(ntg::AviRecorder, m_pWindow, 0x04);
	CHECK_FIELD(ntg::AviRecorder, m_pVideoSource, 0x08);
	CHECK_FIELD(ntg::AviRecorder, m_pAudioIn, 0x0C);
	CHECK_FIELD(ntg::AviRecorder, m_pCompressor, 0x10);
	CHECK_FIELD(ntg::AviRecorder, m_pWriter, 0x14);
	CHECK_FIELD(ntg::AviRecorder, m_nFps, 0x18);
	CHECK_FIELD(ntg::AviRecorder, m_nScale, 0x1C);
	CHECK_FIELD(ntg::AviRecorder, m_bRecording, 0x20);
	CHECK_FIELD(ntg::AviRecorder, m_bFirstFrame, 0x21);
	CHECK_FIELD(ntg::AviRecorder, m_dwLastTime, 0x24);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_AVIRECORDER_H
