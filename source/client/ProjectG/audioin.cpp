#include <windows.h>
#include <mmsystem.h>
#include "audioin.h"

namespace ntg
{

	AudioIn::AudioIn()
		: m_hwavein(NULL),
		  m_buffer_size(0),
		  m_target_thread_id(0),
		  m_thread(INVALID_HANDLE_VALUE),
		  m_thread_id(0)
	{
		memset(&m_format, 0, sizeof(m_format));
		memset(m_header, 0, sizeof(m_header));
	}

	AudioIn::~AudioIn()
	{
	}

	bool AudioIn::init()
	{
		memset(&m_format, 0, sizeof(m_format));
		m_format.wFormatTag = WAVE_FORMAT_PCM;
		m_format.nChannels = 2;
		m_format.nSamplesPerSec = 44100;
		m_format.wBitsPerSample = 16;
		m_format.nBlockAlign = m_format.nChannels * m_format.wBitsPerSample / 8;
		m_format.nAvgBytesPerSec =
			m_format.nSamplesPerSec * m_format.nBlockAlign;

		m_buffer_size = m_format.nAvgBytesPerSec / 2;

		return true;
	}

	void AudioIn::cleanup()
	{
	}

	bool AudioIn::start(DWORD thread_id)
	{
		m_target_thread_id = thread_id;

		m_thread =
			CreateThread(NULL, 0, static_thread_func, this, 0, &m_thread_id);

		waveInOpen(&m_hwavein, WAVE_MAPPER, &m_format, m_thread_id, 0,
			CALLBACK_THREAD);

		for (int i = 0; i < 4; i++)
		{
			memset(&m_header[i], 0, sizeof(WAVEHDR));
			m_header[i].dwBufferLength = m_buffer_size;
			m_header[i].lpData = (LPSTR)VirtualAlloc(NULL,
				m_header[i].dwBufferLength, MEM_COMMIT, PAGE_READWRITE);

			waveInPrepareHeader(m_hwavein, &m_header[i], sizeof(WAVEHDR));

			waveInAddBuffer(m_hwavein, &m_header[i], sizeof(WAVEHDR));
		}

		waveInStart(m_hwavein);

		return true;
	}

	void AudioIn::stop()
	{
		PostThreadMessage(m_thread_id, WM_QUIT, 0, 0);

		waveInStop(m_hwavein);

		for (int i = 0; i < 4; i++)
		{
			waveInUnprepareHeader(m_hwavein, &m_header[i], sizeof(WAVEHDR));

			if (m_header[i].lpData)
			{
				VirtualFree(m_header[i].lpData, 0, MEM_RELEASE);
				m_header[i].lpData = NULL;
			}
		}

		if (m_hwavein)
		{
			waveInClose(m_hwavein);
			m_hwavein = NULL;
		}

		if (WaitForSingleObject(m_thread, INFINITE) == WAIT_OBJECT_0)
			m_thread = INVALID_HANDLE_VALUE;
	}

	void AudioIn::thread_func()
	{
		MSG msg;

		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			if (msg.message == MM_WIM_DATA)
			{
				WAVEHDR* hdr = (WAVEHDR*)msg.lParam;
				if (hdr->dwBytesRecorded)
				{
					PostThreadMessage(m_target_thread_id, WM_USER + 1,
						(WPARAM)hdr->lpData, hdr->dwBytesRecorded);
					waveInAddBuffer(m_hwavein, hdr, sizeof(WAVEHDR));
				}
			}
		}
	}

	DWORD WINAPI AudioIn::static_thread_func(LPVOID param)
	{
		AudioIn* self = (AudioIn*)param;
		self->thread_func();
		return 0;
	}

}
