// Original: d:\build\custom\temp\code\client\projectg\aviwriter.h
#ifndef PG_CLIENT_PROJECTG_AVIWRITER_H
#define PG_CLIENT_PROJECTG_AVIWRITER_H

#include <windows.h>
#include <vfw.h>
#include <vector>
#include "..\..\defines.h"

namespace ntg {

// Writes an AVI file (aviwriter.cpp). Layout from the constructor
// (aviwriter.cpp:27-31), open and the chunk writers.
class AviWriter {
	LAYOUT_CHECK_FRIEND;
public:
	AviWriter(void);
	virtual ~AviWriter(void);

	bool open(char const *pszFileName, BITMAPINFOHEADER const *pFormat, int nQuality, int nFps);
	void close(void);
	void set_comp_format(BITMAPINFO const *pFormat);
	void set_fcc_handler(unsigned long fccHandler) { m_fccHandler = fccHandler; }
	void write_video(int nFrameCount, void *pData, int nSize);
	void write_audio(void *pData, int nSize);
	void write_audio_sync(void *pData, int nSize);

protected:
	void write_indexes(void);
	void write_header(void);
	void reserve_idxcache(void);
	void index(MMCKINFO *pChunk, unsigned long dwFlags);

private:
	BITMAPINFOHEADER m_format;           // 0x04, the input frame format
	int m_nQuality;                      // 0x2C
	int m_nFps;                          // 0x30
	unsigned long m_fccHandler;          // 0x34
	BITMAPINFO m_compFormat;             // 0x38, the compressed frame format
	WAVEFORMAT m_waveFormat;             // 0x64
	int m_nVideoFrames;                  // 0x74, '00dc' chunks written
	int m_nMaxVideoSize;                 // 0x78, largest '00dc' chunk
	int m_nAudioChunks;                  // 0x7C, '01wb' chunks written
	int m_nMaxAudioSize;                 // 0x80, largest '01wb' chunk
	HMMIO m_hmmio;                       // 0x84
	std::vector<AVIINDEXENTRY> m_index;  // 0x88
};

} // namespace ntg

CHECK_SIZE(ntg::AviWriter, 0x98);
BEGIN_LAYOUT_CHECK(ntg::AviWriter)
	CHECK_FIELD(ntg::AviWriter, m_format, 0x04);
	CHECK_FIELD(ntg::AviWriter, m_nQuality, 0x2C);
	CHECK_FIELD(ntg::AviWriter, m_nFps, 0x30);
	CHECK_FIELD(ntg::AviWriter, m_fccHandler, 0x34);
	CHECK_FIELD(ntg::AviWriter, m_compFormat, 0x38);
	CHECK_FIELD(ntg::AviWriter, m_waveFormat, 0x64);
	CHECK_FIELD(ntg::AviWriter, m_nVideoFrames, 0x74);
	CHECK_FIELD(ntg::AviWriter, m_nMaxVideoSize, 0x78);
	CHECK_FIELD(ntg::AviWriter, m_nAudioChunks, 0x7C);
	CHECK_FIELD(ntg::AviWriter, m_nMaxAudioSize, 0x80);
	CHECK_FIELD(ntg::AviWriter, m_hmmio, 0x84);
	CHECK_FIELD(ntg::AviWriter, m_index, 0x88);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_AVIWRITER_H
