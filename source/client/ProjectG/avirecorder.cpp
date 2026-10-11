// Original: d:\build\custom\temp\code\client\projectg\avirecorder.cpp
#include <windows.h>
#include <string.h>
#include <queue>
#include "avirecorder.h"
#include "audioin.h"
#include "aviwriter.h"
#include "vcomp.h"
#include "videoin.h"

namespace ntg {

class Writer;

// Compresses the captured frames on its own thread and hands them to the
// writer. Layout from the constructor (avirecorder.cpp:122).
class Compressor {
	LAYOUT_CHECK_FRIEND;
public:
	Compressor(void);
	~Compressor(void);

	bool init(BITMAPINFO const *pFormat, Writer *pWriter, int nQuality);
	void cleanup(void);
	VideoFrame *get_frame(void);
	void queue(VideoFrame *pFrame);
	// avirecorder.cpp:48
	unsigned long get_fcc_handler(void) const { return m_compressor.get_fcc_handler(); }
	// avirecorder.cpp:49
	BITMAPINFO const *get_comp_format(void) { return &m_compFormat; }

protected:
	void thread_func(void);
	static unsigned long __stdcall static_thread_func(void *pParam);

private:
	Writer *m_pWriter;                    // 0x000
	BITMAPINFO m_format;                  // 0x004, the captured frame format
	VideoFrame m_frames[30];              // 0x030
	std::queue<VideoFrame *> m_queue;     // 0x210, frames waiting for compression
	BITMAPINFO m_compFormat;              // 0x224, the compressed format
	VideoCompressor m_compressor;         // 0x250
	CRITICAL_SECTION m_cs;                // 0x2C0, guards m_frames and m_queue
	HANDLE m_hThread;                     // 0x2D8
	DWORD m_dwThreadId;                   // 0x2DC
};

// Writes the compressed frames and the recorded sound to the AVI file on its
// own thread. Layout from the constructor (avirecorder.cpp:235).
class Writer {
	LAYOUT_CHECK_FRIEND;
public:
	struct VideoStream {
		BYTE *pData;      // 0x00
		int nSize;        // 0x04
		int nFrameCount;  // 0x08
	};

	Writer(void);
	~Writer(void);

	bool init(char const *pszFileName, BITMAPINFOHEADER const *pFormat, int nQuality, int nFps);
	void cleanup(void);
	void set_comp_format(BITMAPINFO const *pFormat);
	void queue_video(BYTE *pData, int nSize, int nFrameCount);
	// avirecorder.cpp:89
	unsigned long get_thread_id(void) const { return m_dwThreadId; }
	// avirecorder.cpp:91
	void set_fcc_handler(unsigned long fccHandler) { m_writer.set_fcc_handler(fccHandler); }

protected:
	void thread_func(void);
	static unsigned long __stdcall static_thread_func(void *pParam);

private:
	std::queue<VideoStream> m_queue; // 0x00, compressed frames waiting to be written
	AviWriter m_writer;              // 0x14
	CRITICAL_SECTION m_cs;           // 0xAC, guards m_queue
	HANDLE m_hThread;                // 0xC4
	DWORD m_dwThreadId;              // 0xC8
};

// avirecorder.cpp:122
Compressor::Compressor(void)
	: m_pWriter(0), m_hThread(INVALID_HANDLE_VALUE) {
	memset(&m_format, 0, sizeof(m_format));
}

// avirecorder.cpp:127
Compressor::~Compressor(void) {
}

// avirecorder.cpp:131
bool Compressor::init(BITMAPINFO const *pFormat, Writer *pWriter, int nQuality) {
	m_pWriter = pWriter;

	m_format = *pFormat;
	m_compressor.init(pFormat, nQuality);

	m_compressor.get_format(&m_compFormat);

	for (int i = 0; i < 30; i++)
		m_frames[i].m_pData = new BYTE[pFormat->bmiHeader.biSizeImage];

	InitializeCriticalSection(&m_cs);

	m_hThread = CreateThread(0, 0, static_thread_func, this, 0, &m_dwThreadId);

	return true;
}

// avirecorder.cpp:155
void Compressor::cleanup(void) {
	if (m_hThread) {
		PostThreadMessage(m_dwThreadId, WM_QUIT, 0, 0);
		WaitForSingleObject(m_hThread, INFINITE);
		m_hThread = INVALID_HANDLE_VALUE;
	}

	m_compressor.stop();
	m_compressor.cleanup();

	DeleteCriticalSection(&m_cs);
}

// avirecorder.cpp:169
VideoFrame *Compressor::get_frame(void) {
	VideoFrame *pFrame = 0;

	EnterCriticalSection(&m_cs);
	for (int i = 0; i < 30; i++) {
		if (m_frames[i].m_bInUse == 0) {
			m_frames[i].m_bInUse = 1;
			pFrame = &m_frames[i];
			break;
		}
	}
	LeaveCriticalSection(&m_cs);

	return pFrame;
}

// avirecorder.cpp:186
void Compressor::queue(VideoFrame *pFrame) {
	EnterCriticalSection(&m_cs);
	m_queue.push(pFrame);
	LeaveCriticalSection(&m_cs);

	PostThreadMessage(m_dwThreadId, WM_USER, 0, 0);
}

// avirecorder.cpp:195
void Compressor::thread_func(void) {
	m_compressor.start();

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0) > 0) {
		if (msg.message == WM_USER) {
			VideoFrame *pFrame;

			EnterCriticalSection(&m_cs);
			pFrame = m_queue.front();
			m_queue.pop();
			LeaveCriticalSection(&m_cs);

			int nSize;

			void *pData = m_compressor.compress(pFrame->m_pData, &nSize);

			m_pWriter->queue_video((BYTE *)pData, nSize, pFrame->m_nFrameCount);

			EnterCriticalSection(&m_cs);
			pFrame->m_bInUse = 0;
			LeaveCriticalSection(&m_cs);
		}
	}

	m_compressor.stop();
}

// avirecorder.cpp:226
unsigned long __stdcall Compressor::static_thread_func(void *pParam) {
	((Compressor *)pParam)->thread_func();
	return 0;
}

// avirecorder.cpp:235
Writer::Writer(void) {
}

// avirecorder.cpp:239
Writer::~Writer(void) {
}

// avirecorder.cpp:243
bool Writer::init(char const *pszFileName, BITMAPINFOHEADER const *pFormat, int nQuality, int nFps) {
	InitializeCriticalSection(&m_cs);

	m_writer.open(pszFileName, pFormat, nQuality, nFps);

	m_hThread = CreateThread(0, 0, static_thread_func, this, 0, &m_dwThreadId);

	return true;
}

// avirecorder.cpp:254
void Writer::cleanup(void) {
	if (m_hThread) {
		PostThreadMessage(m_dwThreadId, WM_USER + 2, 0, 0);
		WaitForSingleObject(m_hThread, INFINITE);
		m_hThread = INVALID_HANDLE_VALUE;
	}

	m_writer.close();

	DeleteCriticalSection(&m_cs);
}

// avirecorder.cpp:267
void Writer::set_comp_format(BITMAPINFO const *pFormat) {
	m_writer.set_comp_format(pFormat);
}

// avirecorder.cpp:272
void Writer::queue_video(BYTE *pData, int nSize, int nFrameCount) {
	VideoStream stream;
	stream.pData = new BYTE[nSize];
	memcpy(stream.pData, pData, nSize);
	stream.nSize = nSize;
	stream.nFrameCount = nFrameCount;

	EnterCriticalSection(&m_cs);
	m_queue.push(stream);
	LeaveCriticalSection(&m_cs);

	PostThreadMessage(m_dwThreadId, WM_USER, 0, 0);
}

// avirecorder.cpp:287
void Writer::thread_func(void) {
	MSG msg;
	bool bStopping = false;

	while (GetMessage(&msg, 0, 0, 0) > 0) {
		if (msg.message == WM_USER) {
			VideoStream stream;

			EnterCriticalSection(&m_cs);
			stream = m_queue.front();
			m_queue.pop();
			LeaveCriticalSection(&m_cs);

			m_writer.write_video(stream.nFrameCount, stream.pData, stream.nSize);

			delete[] stream.pData;
		} else if (msg.message == WM_USER + 1) {
			if (bStopping) {
				m_writer.write_audio_sync((void *)msg.wParam, (int)msg.lParam);
				return;
			}

			m_writer.write_audio((void *)msg.wParam, (int)msg.lParam);
		} else if (msg.message == WM_USER + 2) {
			bStopping = true;
		}
	}
}

// avirecorder.cpp:321
unsigned long __stdcall Writer::static_thread_func(void *pParam) {
	((Writer *)pParam)->thread_func();
	return 0;
}

// avirecorder.cpp:344
AviRecorder::AviRecorder(void)
	: m_pWindow(0), m_pVideoSource(0), m_pAudioIn(0), m_pCompressor(0), m_pWriter(0),
	  m_nFps(0), m_nScale(1), m_bRecording(false), m_bFirstFrame(false),
	  m_dwLastTime(0) {
}

// avirecorder.cpp:348
AviRecorder::~AviRecorder(void) {
	if (m_pCompressor) { delete m_pCompressor; m_pCompressor = 0; }
	if (m_pVideoSource) { delete m_pVideoSource; m_pVideoSource = 0; }
	if (m_pAudioIn) { delete m_pAudioIn; m_pAudioIn = 0; }
}

// avirecorder.cpp:355
bool AviRecorder::init(void *pWindow) {
	m_pWindow = pWindow;
	return true;
}

// avirecorder.cpp:361
void AviRecorder::cleanup(void) {
}

// avirecorder.cpp:365
bool AviRecorder::start(char const *pszFileName, int nQuality, int nScale, int nFps) {
	nQuality = nQuality < 0 ? 0 : nQuality;
	if (nQuality > 100) nQuality = 100;

	m_nScale = nScale;
	m_nFps = nFps;

	m_pVideoSource->start(m_pWindow);

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader = *m_pVideoSource->get_format();
	bmi.bmiHeader.biWidth /= m_nScale;
	bmi.bmiHeader.biHeight /= m_nScale;
	int nPad = bmi.bmiHeader.biWidth * 4 % 4;
	int nPitch = bmi.bmiHeader.biWidth * 4 + nPad;
	bmi.bmiHeader.biSizeImage = nPitch * bmi.bmiHeader.biHeight;

	m_pWriter = new Writer;
	m_pWriter->init(pszFileName, &bmi.bmiHeader, nQuality, m_nFps);

	m_pAudioIn = new AudioIn;
	m_pAudioIn->init();
	m_pAudioIn->start(m_pWriter->get_thread_id());

	m_pCompressor = new Compressor;
	m_pCompressor->init(&bmi, m_pWriter, nQuality);

	m_pWriter->set_fcc_handler(m_pCompressor->get_fcc_handler());
	m_pWriter->set_comp_format(m_pCompressor->get_comp_format());

	m_bRecording = true;
	m_bFirstFrame = true;
	return true;
}

// avirecorder.cpp:414
void AviRecorder::stop(void) {
	if (!m_bRecording)
		return;

	m_bRecording = false;
	m_pVideoSource->stop();
	m_pCompressor->cleanup();
	m_pWriter->cleanup();
	m_pAudioIn->stop();
	m_pAudioIn->cleanup();

	if (m_pCompressor) { delete m_pCompressor; m_pCompressor = 0; }
	if (m_pWriter) { delete m_pWriter; m_pWriter = 0; }
	if (m_pAudioIn) { delete m_pAudioIn; m_pAudioIn = 0; }
	if (m_pVideoSource) { delete m_pVideoSource; m_pVideoSource = 0; }
}

// avirecorder.cpp:441
void AviRecorder::record(void) {
	if (!m_bRecording)
		return;

	if (m_bFirstFrame) {
		m_dwLastTime = timeGetTime();
		m_bFirstFrame = false;
	}

	DWORD dwNow = timeGetTime();

	DWORD dwElapsed = dwNow - m_dwLastTime;
	DWORD dwInterval = 1000 / m_nFps;

	if (dwElapsed < dwInterval) {
		Sleep(1);
		return;
	}

	int nFrameCount = dwElapsed / dwInterval;
	m_dwLastTime = dwNow - dwElapsed % dwInterval;

	m_pVideoSource->capture();

	VideoFrame *pFrame = m_pCompressor->get_frame();
	while (pFrame == 0) {
		Sleep(1);
		pFrame = m_pCompressor->get_frame();
	}

	BITMAPINFOHEADER bih = *m_pVideoSource->get_format();

	VideoFrame *pSource = m_pVideoSource->get_frame();
	if (pSource == 0) {
		stop();
		return;
	}

	if (m_nScale == 1) {
		int nStride = bih.biSizeImage / bih.biHeight;

		for (int y = 0; y < bih.biHeight; y++)
			memcpy(pFrame->m_pData + y * nStride, pSource->m_pData + (bih.biHeight - 1 - y) * nStride, nStride);
	} else {
		int nWidth = bih.biWidth / m_nScale;
		int nHeight = bih.biHeight / m_nScale;
		int nPad = nWidth * 4 % 4;
		int nPitch = nWidth * 4 + nPad;

		int nStride = bih.biSizeImage / bih.biHeight;

		BYTE *pSrc = pSource->m_pData;
		BYTE *pDest = pFrame->m_pData;

		for (int y = 0; y < nHeight; y++) {
			for (int x = 0; x < nWidth; x++) {
				DWORD dwColor = *(DWORD *)(pSrc + (y * nStride + x * 4) * m_nScale);
				pDest[(nHeight - 1 - y) * nPitch + x * 4 + 0] = (BYTE)dwColor;
				pDest[(nHeight - 1 - y) * nPitch + x * 4 + 1] = (BYTE)(dwColor >> 8);
				pDest[(nHeight - 1 - y) * nPitch + x * 4 + 2] = (BYTE)(dwColor >> 16);
			}
		}
	}

	pFrame->m_nFrameCount = nFrameCount;
	m_pCompressor->queue(pFrame);
}

// avirecorder.cpp:528
void AviRecorder::unit_test(void) {
	m_pVideoSource->capture();

	VideoFrame *pFrame;
	do {
		pFrame = m_pCompressor->get_frame();
	} while (pFrame == 0);

	BITMAPINFOHEADER bih = *m_pVideoSource->get_format();

	int nWidth = bih.biWidth / m_nScale;
	int nHeight = bih.biHeight / m_nScale;
	int nPad = nWidth * 4 % 4;
	int nPitch = nWidth * 4 + nPad;

	int nStride = bih.biSizeImage / bih.biHeight;

	BYTE *pSource = m_pVideoSource->get_frame()->m_pData;
	BYTE *pDest = pFrame->m_pData;

	for (int y = 0; y < nHeight; y++) {
		for (int x = 0; x < nWidth; x++) {
			DWORD dwColor = *(DWORD *)(pSource + (y * nStride + x * 4) * m_nScale);
			pDest[(nHeight - 1 - y) * nPitch + x * 4 + 0] = (BYTE)dwColor;
			pDest[(nHeight - 1 - y) * nPitch + x * 4 + 1] = (BYTE)(dwColor >> 8);
			pDest[(nHeight - 1 - y) * nPitch + x * 4 + 2] = (BYTE)(dwColor >> 16);
		}
	}

	pFrame->m_nFrameCount = 1;

	m_pCompressor->queue(pFrame);
}

} // namespace ntg

CHECK_SIZE(ntg::Compressor, 0x2E0);
BEGIN_LAYOUT_CHECK(ntg::Compressor)
	CHECK_FIELD(ntg::Compressor, m_pWriter, 0x000);
	CHECK_FIELD(ntg::Compressor, m_format, 0x004);
	CHECK_FIELD(ntg::Compressor, m_frames, 0x030);
	CHECK_FIELD(ntg::Compressor, m_queue, 0x210);
	CHECK_FIELD(ntg::Compressor, m_compFormat, 0x224);
	CHECK_FIELD(ntg::Compressor, m_compressor, 0x250);
	CHECK_FIELD(ntg::Compressor, m_cs, 0x2C0);
	CHECK_FIELD(ntg::Compressor, m_hThread, 0x2D8);
	CHECK_FIELD(ntg::Compressor, m_dwThreadId, 0x2DC);
END_LAYOUT_CHECK

CHECK_SIZE(ntg::Writer, 0xCC);
BEGIN_LAYOUT_CHECK(ntg::Writer)
	CHECK_FIELD(ntg::Writer, m_queue, 0x00);
	CHECK_FIELD(ntg::Writer, m_writer, 0x14);
	CHECK_FIELD(ntg::Writer, m_cs, 0xAC);
	CHECK_FIELD(ntg::Writer, m_hThread, 0xC4);
	CHECK_FIELD(ntg::Writer, m_dwThreadId, 0xC8);
END_LAYOUT_CHECK
