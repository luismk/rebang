#ifndef PG_CLIENT_PROJECTG_AUDIOIN_H
#define PG_CLIENT_PROJECTG_AUDIOIN_H

#include <windows.h>
#include "..\..\defines.h"

namespace ntg {

// Records the wave input on a thread of its own and posts each filled buffer
// to a client thread. Layout from the constructor (audioin.cpp:22-24),
// init and start.
class AudioIn {
	LAYOUT_CHECK_FRIEND;
public:
	AudioIn();
	virtual ~AudioIn();

	bool init();
	void cleanup();
	bool start(unsigned long);
	void stop();

protected:
	void thread_func();
	static unsigned long __stdcall static_thread_func(void *);

	HWAVEIN m_hWaveIn;              // 0x04
	WAVEFORMATEX m_format;          // 0x08
	WAVEHDR m_headers[4];           // 0x1A
	unsigned long m_bufferSize;     // 0x9C, bytes per buffer
	unsigned long m_clientThreadId; // 0xA0, receives WM_USER + 1 per buffer
	HANDLE m_hThread;               // 0xA4
	unsigned long m_threadId;       // 0xA8
};

} // namespace ntg

CHECK_SIZE(ntg::AudioIn, 0xAC);
BEGIN_LAYOUT_CHECK(ntg::AudioIn)
	CHECK_FIELD(ntg::AudioIn, m_hWaveIn, 0x04);
	CHECK_FIELD(ntg::AudioIn, m_format, 0x08);
	CHECK_FIELD(ntg::AudioIn, m_headers, 0x1A);
	CHECK_FIELD(ntg::AudioIn, m_bufferSize, 0x9C);
	CHECK_FIELD(ntg::AudioIn, m_clientThreadId, 0xA0);
	CHECK_FIELD(ntg::AudioIn, m_hThread, 0xA4);
	CHECK_FIELD(ntg::AudioIn, m_threadId, 0xA8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_AUDIOIN_H
