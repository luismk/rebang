// Original: d:\build\custom\temp\code\client\projectg\aviwriter.cpp
#include <windows.h>
#include <string.h>
#include "aviwriter.h"

namespace ntg {

// aviwriter.cpp:27
AviWriter::AviWriter(void)
	: m_nQuality(0), m_nFps(0), m_fccHandler(0), m_nVideoFrames(0), m_nMaxVideoSize(0),
	  m_nAudioChunks(0), m_nMaxAudioSize(0), m_hmmio(0) {
	memset(&m_format, 0, sizeof(m_format));
	memset(&m_compFormat, 0, sizeof(m_compFormat));
	memset(&m_waveFormat, 0, sizeof(m_waveFormat));
}

// aviwriter.cpp:34
AviWriter::~AviWriter(void) {
}

// aviwriter.cpp:38
bool AviWriter::open(char const *pszFileName, BITMAPINFOHEADER const *pFormat, int nQuality, int nFps) {
	m_hmmio = mmioOpen((LPSTR)pszFileName, 0, MMIO_CREATE | MMIO_WRITE);
	if (!m_hmmio)
		return false;

	mmioSeek(m_hmmio, 0x800, SEEK_SET);

	m_format = *pFormat;
	m_nQuality = nQuality;
	m_nFps = nFps;
	m_nVideoFrames = 0;
	m_nMaxVideoSize = 0;
	m_nAudioChunks = 0;
	m_nMaxAudioSize = 0;

	reserve_idxcache();

	return true;
}

// aviwriter.cpp:59
void AviWriter::close(void) {
	if (m_hmmio) {
		write_header();

		m_index.clear();

		mmioFlush(m_hmmio, 0);
		mmioClose(m_hmmio, 0);
		m_hmmio = 0;
	}
}

// aviwriter.cpp:73
void AviWriter::set_comp_format(BITMAPINFO const *pFormat) {
	m_compFormat = *pFormat;
}

// aviwriter.cpp:78
void AviWriter::write_video(int nFrameCount, void *pData, int nSize) {
	for (int i = 0; i < nFrameCount; i++) {
		MMCKINFO ck;
		memset(&ck, 0, sizeof(ck));
		ck.ckid = mmioFOURCC('0', '0', 'd', 'c');
		ck.cksize = nSize;
		mmioCreateChunk(m_hmmio, &ck, 0);
		mmioWrite(m_hmmio, (char *)pData, nSize);
		mmioAscend(m_hmmio, &ck, 0);

		m_nVideoFrames++;

		if (m_nMaxVideoSize < nSize)
			m_nMaxVideoSize = nSize;

		index(&ck, AVIIF_KEYFRAME);
	}
}

// aviwriter.cpp:99
void AviWriter::write_audio(void *pData, int nSize) {
	MMCKINFO ck;

	memset(&ck, 0, sizeof(ck));

	ck.ckid = mmioFOURCC('0', '1', 'w', 'b');
	ck.cksize = nSize;
	mmioCreateChunk(m_hmmio, &ck, 0);
	mmioWrite(m_hmmio, (char *)pData, nSize);
	mmioAscend(m_hmmio, &ck, 0);

	m_nAudioChunks++;

	if (m_nMaxAudioSize < nSize)
		m_nMaxAudioSize = nSize;

	index(&ck, 0);
}

// aviwriter.cpp:120
void AviWriter::write_audio_sync(void *pData, int nSize) {
	int nVideoTime = 1000 / m_nFps * m_nVideoFrames;
	int nAudioTime = m_nAudioChunks * 500;

	if (nVideoTime > nAudioTime) {
		write_audio(pData, (nVideoTime - nAudioTime) * 176400 / 1000);
	}
}

// aviwriter.cpp:136
void AviWriter::write_header(void) {
	MMCKINFO ckRiff;
	MMCKINFO ckHdrl;
	MMCKINFO ckAvih;
	MMCKINFO ckStrl;
	MMCKINFO ckStrh;
	MMCKINFO ckStrf;
	MMCKINFO ckJunk;
	MMCKINFO ckMovi;
	MainAVIHeader avih;
	AVIStreamHeader strh;
	short rcFrame[4];
	DWORD dwBitsPerSample;
	LONG lMoviEnd;

	lMoviEnd = mmioSeek(m_hmmio, 0, SEEK_CUR);
	mmioSeek(m_hmmio, 0, SEEK_SET);

	memset(&ckRiff, 0, sizeof(ckRiff));
	ckRiff.fccType = mmioFOURCC('A', 'V', 'I', ' ');
	mmioCreateChunk(m_hmmio, &ckRiff, MMIO_CREATERIFF);

	memset(&ckHdrl, 0, sizeof(ckHdrl));
	ckHdrl.fccType = mmioFOURCC('h', 'd', 'r', 'l');
	mmioCreateChunk(m_hmmio, &ckHdrl, MMIO_CREATELIST);

	memset(&ckAvih, 0, sizeof(ckAvih));
	ckAvih.ckid = mmioFOURCC('a', 'v', 'i', 'h');
	ckAvih.cksize = sizeof(MainAVIHeader);
	mmioCreateChunk(m_hmmio, &ckAvih, 0);

	memset(&avih, 0, sizeof(avih));
	avih.dwMicroSecPerFrame = MulDiv(1000000, 1, m_nFps);
	avih.dwMaxBytesPerSec = 0;
	avih.dwPaddingGranularity = 0;
	avih.dwFlags = AVIF_HASINDEX;
	avih.dwTotalFrames = m_nVideoFrames;
	avih.dwStreams = 2;
	avih.dwSuggestedBufferSize = m_nMaxVideoSize;
	avih.dwWidth = m_format.biWidth;
	avih.dwHeight = m_format.biHeight;
	mmioWrite(m_hmmio, (char *)&avih, sizeof(avih));
	mmioAscend(m_hmmio, &ckAvih, 0);

	memset(&ckStrl, 0, sizeof(ckStrl));
	ckStrl.fccType = mmioFOURCC('s', 't', 'r', 'l');
	mmioCreateChunk(m_hmmio, &ckStrl, MMIO_CREATELIST);

	memset(&ckStrh, 0, sizeof(ckStrh));
	ckStrh.ckid = mmioFOURCC('s', 't', 'r', 'h');
	ckStrh.cksize = sizeof(AVIStreamHeader) - sizeof(RECT) + sizeof(rcFrame);
	mmioCreateChunk(m_hmmio, &ckStrh, 0);

	memset(&strh, 0, sizeof(strh));
	strh.fccType = mmioFOURCC('v', 'i', 'd', 's');
	strh.fccHandler = m_fccHandler;
	strh.dwScale = 1;
	strh.dwRate = m_nFps;
	strh.dwLength = 1;
	strh.dwSuggestedBufferSize = m_nMaxVideoSize;
	strh.dwQuality = m_nQuality * 100;
	rcFrame[0] = 0;
	rcFrame[1] = 0;
	rcFrame[2] = (short)m_format.biWidth;
	rcFrame[3] = (short)m_format.biHeight;
	mmioWrite(m_hmmio, (char *)&strh, sizeof(strh) - sizeof(RECT));
	mmioWrite(m_hmmio, (char *)rcFrame, sizeof(rcFrame));
	mmioAscend(m_hmmio, &ckStrh, 0);

	memset(&ckStrf, 0, sizeof(ckStrf));
	ckStrf.ckid = mmioFOURCC('s', 't', 'r', 'f');
	ckStrf.cksize = sizeof(BITMAPINFOHEADER);
	mmioCreateChunk(m_hmmio, &ckStrf, 0);
	mmioWrite(m_hmmio, (char *)&m_compFormat, sizeof(BITMAPINFOHEADER));
	mmioAscend(m_hmmio, &ckStrf, 0);

	mmioAscend(m_hmmio, &ckStrl, 0);
	mmioAscend(m_hmmio, &ckHdrl, 0);

	memset(&ckStrl, 0, sizeof(ckStrl));
	ckStrl.fccType = mmioFOURCC('s', 't', 'r', 'l');
	mmioCreateChunk(m_hmmio, &ckStrl, MMIO_CREATELIST);

	memset(&ckStrh, 0, sizeof(ckStrh));
	ckStrh.ckid = mmioFOURCC('s', 't', 'r', 'h');
	ckStrh.cksize = sizeof(AVIStreamHeader) - sizeof(RECT) + sizeof(rcFrame);
	mmioCreateChunk(m_hmmio, &ckStrh, 0);

	memset(&strh, 0, sizeof(strh));
	strh.fccType = mmioFOURCC('a', 'u', 'd', 's');
	strh.dwScale = 4;
	strh.dwRate = 176400;
	strh.dwLength = m_nAudioChunks * 22050;
	strh.dwSampleSize = 4;
	memset(rcFrame, 0, sizeof(rcFrame));
	mmioWrite(m_hmmio, (char *)&strh, sizeof(strh) - sizeof(RECT));
	mmioWrite(m_hmmio, (char *)rcFrame, sizeof(rcFrame));
	mmioAscend(m_hmmio, &ckStrh, 0);

	memset(&ckStrf, 0, sizeof(ckStrf));
	ckStrf.ckid = mmioFOURCC('s', 't', 'r', 'f');
	ckStrf.cksize = sizeof(WAVEFORMAT) + sizeof(WORD);
	mmioCreateChunk(m_hmmio, &ckStrf, 0);

	memset(&m_waveFormat, 0, sizeof(m_waveFormat));
	m_waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	m_waveFormat.nChannels = 2;
	m_waveFormat.nSamplesPerSec = 44100;
	m_waveFormat.nAvgBytesPerSec = 176400;
	m_waveFormat.nBlockAlign = 4;
	mmioWrite(m_hmmio, (char *)&m_waveFormat, sizeof(m_waveFormat));
	dwBitsPerSample = ckStrf.cksize;
	mmioWrite(m_hmmio, (char *)&dwBitsPerSample, sizeof(WORD));
	mmioAscend(m_hmmio, &ckStrf, 0);

	mmioAscend(m_hmmio, &ckStrl, 0);
	mmioAscend(m_hmmio, &ckHdrl, 0);

	memset(&ckJunk, 0, sizeof(ckJunk));
	ckJunk.ckid = mmioFOURCC('J', 'U', 'N', 'K');
	mmioCreateChunk(m_hmmio, &ckJunk, 0);
	mmioSeek(m_hmmio, 0x7F4, SEEK_SET);
	mmioAscend(m_hmmio, &ckJunk, 0);

	memset(&ckMovi, 0, sizeof(ckMovi));
	ckMovi.fccType = mmioFOURCC('m', 'o', 'v', 'i');
	mmioCreateChunk(m_hmmio, &ckMovi, MMIO_CREATELIST);
	mmioSeek(m_hmmio, lMoviEnd, SEEK_SET);
	mmioAscend(m_hmmio, &ckMovi, 0);

	write_indexes();

	mmioAscend(m_hmmio, &ckRiff, 0);
}

// aviwriter.cpp:295
void AviWriter::reserve_idxcache(void) {
	if (m_index.capacity() == m_index.size()) {
		m_index.reserve(m_index.size() + m_nFps * 60);
	}
}

// aviwriter.cpp:307
void AviWriter::index(MMCKINFO *pChunk, unsigned long dwFlags) {
	AVIINDEXENTRY entry;

	entry.ckid = pChunk->ckid;
	entry.dwFlags = dwFlags;
	entry.dwChunkOffset = pChunk->dwDataOffset - 8;
	entry.dwChunkLength = pChunk->cksize;

	reserve_idxcache();
	m_index.push_back(entry);
}

// aviwriter.cpp:320
void AviWriter::write_indexes(void) {
	if (m_index.size()) {
		MMCKINFO ck;

		memset(&ck, 0, sizeof(ck));
		ck.ckid = mmioFOURCC('i', 'd', 'x', '1');
		ck.cksize = m_index.size() * sizeof(AVIINDEXENTRY);
		mmioCreateChunk(m_hmmio, &ck, 0);
		mmioWrite(m_hmmio, (char *)&m_index[0], ck.cksize);
		mmioAscend(m_hmmio, &ck, 0);
	}
}

} // namespace ntg
