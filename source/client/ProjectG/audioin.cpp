// Original: d:\build\custom\temp\code\client\projectg\audioin.cpp
#include <windows.h>
#include <string.h>
#include "audioin.h"

namespace ntg {

// audioin.cpp:22
AudioIn::AudioIn()
	: m_hWaveIn(0), m_bufferSize(0), m_clientThreadId(0), m_hThread(INVALID_HANDLE_VALUE), m_threadId(0) {
	memset(&m_format, 0, sizeof(m_format));
	memset(m_headers, 0, sizeof(m_headers));
}

// audioin.cpp:28
AudioIn::~AudioIn() {
}

// audioin.cpp:32
bool AudioIn::init() {
	memset(&m_format, 0, sizeof(m_format));
	m_format.wFormatTag = WAVE_FORMAT_PCM;
	m_format.nChannels = 2;
	m_format.nSamplesPerSec = 44100;
	m_format.wBitsPerSample = 16;
	m_format.nBlockAlign = 4;
	m_format.nAvgBytesPerSec = 176400;

	m_bufferSize = 88200;

	return true;
}

// audioin.cpp:47
void AudioIn::cleanup() {
}

// audioin.cpp:51
bool AudioIn::start(unsigned long clientThreadId) {
	int i;

	m_clientThreadId = clientThreadId;

	m_hThread = CreateThread(0, 0, static_thread_func, this, 0, &m_threadId);

	waveInOpen(&m_hWaveIn, WAVE_MAPPER, &m_format, m_threadId, 0, CALLBACK_THREAD);

	for (i = 0; i < 4; i++) {
		memset(&m_headers[i], 0, sizeof(WAVEHDR));
		m_headers[i].dwBufferLength = m_bufferSize;
		m_headers[i].lpData = (LPSTR)VirtualAlloc(0, m_bufferSize, MEM_COMMIT, PAGE_READWRITE);

		waveInPrepareHeader(m_hWaveIn, &m_headers[i], sizeof(WAVEHDR));

		waveInAddBuffer(m_hWaveIn, &m_headers[i], sizeof(WAVEHDR));
	}

	waveInStart(m_hWaveIn);

	return true;
}

// audioin.cpp:80
void AudioIn::stop() {
	PostThreadMessage(m_threadId, WM_QUIT, 0, 0);

	waveInStop(m_hWaveIn);

	for (int i = 0; i < 4; i++) {
		waveInUnprepareHeader(m_hWaveIn, &m_headers[i], sizeof(WAVEHDR));

		if (m_headers[i].lpData) {
			VirtualFree(m_headers[i].lpData, 0, MEM_RELEASE);
			m_headers[i].lpData = 0;
		}
	}

	if (m_hWaveIn) {
		waveInClose(m_hWaveIn);
		m_hWaveIn = 0;
	}

	if (WaitForSingleObject(m_hThread, INFINITE) == WAIT_OBJECT_0)
		m_hThread = INVALID_HANDLE_VALUE;
}

// audioin.cpp:104
void AudioIn::thread_func() {
	MSG msg;

	while (GetMessage(&msg, 0, 0, 0) > 0) {
		if (msg.message == MM_WIM_DATA) {
			WAVEHDR *pHeader = (WAVEHDR *)msg.lParam;
			if (pHeader->dwBytesRecorded) {
				PostThreadMessage(m_clientThreadId, WM_USER + 1, (WPARAM)pHeader->lpData, pHeader->dwBytesRecorded);
				waveInAddBuffer(m_hWaveIn, pHeader, sizeof(WAVEHDR));
			}
		}
	}
}

// audioin.cpp:120
unsigned long __stdcall AudioIn::static_thread_func(void *pParam) {
	((AudioIn *)pParam)->thread_func();
	return 0;
}

} // namespace ntg
