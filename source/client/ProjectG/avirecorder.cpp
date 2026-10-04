#include "avirecorder.h"
#include "audioin.h"
#include <queue>

namespace ntg
{
	class VideoIn;
	class Writer;

	class Compressor
	{
	public:
		Compressor();
		~Compressor();

		bool init(const tagBITMAPINFO* format, Writer* writer, int quality);
		void cleanup();
		VideoFrame* get_frame();
		void queue(VideoFrame* frame);
		unsigned long get_fcc_handler() const
		{
			return m_vcomp.get_fcc_handler();
		}
		const tagBITMAPINFO* get_comp_format() { return &m_comp_format; }

	protected:
		void thread_func();
		static unsigned long __stdcall static_thread_func(void* param);

		static const int NUM_VIDEOFRAMES = 30;

		Writer* m_writer;
		tagBITMAPINFO m_format;
		VideoFrame m_frames[NUM_VIDEOFRAMES];
		std::queue<VideoFrame*> m_queue;
		tagBITMAPINFO m_comp_format;
		VideoCompressor m_vcomp;
		CRITICAL_SECTION m_cs;
		void* m_thread;
		unsigned long m_thread_id;
	};

	class Writer
	{
	public:
		struct VideoStream
		{
			unsigned char* data;
			int size;
			int frames;
		};

		Writer();
		~Writer();

		bool init(const char* filename, const tagBITMAPINFOHEADER* bih,
			int quality, int fps);
		void cleanup();
		void set_comp_format(const tagBITMAPINFO* format);
		unsigned long get_thread_id() const { return m_thread_id; }
		void set_fcc_handler(unsigned long fcc) { m_avi.set_fcc_handler(fcc); }
		void queue_video(unsigned char* data, int size, int frames);

	protected:
		void thread_func();
		static unsigned long __stdcall static_thread_func(void* param);

		std::queue<VideoStream> m_video;
		AviWriter m_avi;
		CRITICAL_SECTION m_cs;
		void* m_thread;
		unsigned long m_thread_id;
	};

	Compressor::Compressor()
		: m_writer(NULL), m_thread(INVALID_HANDLE_VALUE)
	{
		memset(&m_format, 0, sizeof(m_format));
	}

	Compressor::~Compressor()
	{
	}

	bool Compressor::init(const BITMAPINFO* format, Writer* writer, int quality)
	{
		m_writer = writer;
		m_format = *format;
		m_vcomp.init(format, quality);
		m_vcomp.get_format(&m_comp_format);
		for (int i = 0; i < NUM_VIDEOFRAMES; ++i)
			m_frames[i].m_data =
				new unsigned char[format->bmiHeader.biSizeImage];
		InitializeCriticalSection(&m_cs);
		m_thread =
			CreateThread(NULL, 0, static_thread_func, this, 0, &m_thread_id);
		return true;
	}

	void Compressor::cleanup()
	{
		if (m_thread)
		{
			PostThreadMessage(m_thread_id, WM_QUIT, 0, 0);
			WaitForSingleObject(m_thread, INFINITE);
			m_thread = INVALID_HANDLE_VALUE;
		}
		m_vcomp.stop();
		m_vcomp.cleanup();
		DeleteCriticalSection(&m_cs);
	}

	VideoFrame* Compressor::get_frame()
	{
		VideoFrame* frame = NULL;
		EnterCriticalSection(&m_cs);
		for (int i = 0; i < NUM_VIDEOFRAMES; ++i)
		{
			if (!m_frames[i].m_used)
			{
				m_frames[i].m_used = 1;
				frame = &m_frames[i];
				break;
			}
		}
		LeaveCriticalSection(&m_cs);
		return frame;
	}

	void Compressor::queue(VideoFrame* frame)
	{
		EnterCriticalSection(&m_cs);
		m_queue.push(frame);
		LeaveCriticalSection(&m_cs);
		PostThreadMessage(m_thread_id, WM_USER, 0, 0);
	}

	void Compressor::thread_func()
	{
		m_vcomp.start();
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			if (msg.message == WM_USER)
			{
				EnterCriticalSection(&m_cs);
				VideoFrame* frame = m_queue.front();
				m_queue.pop();
				LeaveCriticalSection(&m_cs);
				int size;
				unsigned char* data =
					(unsigned char*)m_vcomp.compress(frame->m_data, &size);
				m_writer->queue_video(data, size, frame->m_frames);
				EnterCriticalSection(&m_cs);
				frame->m_used = 0;
				LeaveCriticalSection(&m_cs);
			}
		}
		m_vcomp.stop();
	}

	DWORD WINAPI Compressor::static_thread_func(void* param)
	{
		((Compressor*)param)->thread_func();
		return 0;
	}

	Writer::Writer()
	{
	}

	Writer::~Writer()
	{
	}

	bool Writer::init(const char* filename, const BITMAPINFOHEADER* bih,
		int quality, int fps)
	{
		InitializeCriticalSection(&m_cs);
		m_avi.open(filename, bih, quality, fps);
		m_thread =
			CreateThread(NULL, 0, static_thread_func, this, 0, &m_thread_id);
		return true;
	}

	void Writer::cleanup()
	{
		if (m_thread)
		{
			PostThreadMessage(m_thread_id, WM_USER + 2, 0, 0);
			WaitForSingleObject(m_thread, INFINITE);
			m_thread = INVALID_HANDLE_VALUE;
		}
		m_avi.close();
		DeleteCriticalSection(&m_cs);
	}

	void Writer::set_comp_format(const BITMAPINFO* format)
	{
		m_avi.set_comp_format(format);
	}

	void Writer::queue_video(unsigned char* data, int size, int frames)
	{
		VideoStream stream;
		stream.data = new unsigned char[size];
		memcpy(stream.data, data, size);
		stream.size = size;
		stream.frames = frames;
		EnterCriticalSection(&m_cs);
		m_video.push(stream);
		LeaveCriticalSection(&m_cs);
		PostThreadMessage(m_thread_id, WM_USER, 0, 0);
	}

	void Writer::thread_func()
	{
		bool stopping = false;
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			if (msg.message == WM_USER)
			{
				EnterCriticalSection(&m_cs);
				VideoStream stream = m_video.front();
				m_video.pop();
				LeaveCriticalSection(&m_cs);
				m_avi.write_video(stream.frames, stream.data, stream.size);
				delete[] stream.data;
			}
			else if (msg.message == WM_USER + 1)
			{
				if (stopping)
				{
					m_avi.write_audio_sync((void*)msg.wParam, msg.lParam);
					return;
				}
				m_avi.write_audio((void*)msg.wParam, msg.lParam);
			}
			else if (msg.message == WM_USER + 2)
				stopping = true;
		}
	}

	DWORD WINAPI Writer::static_thread_func(void* param)
	{
		((Writer*)param)->thread_func();
		return 0;
	}

	AviRecorder::AviRecorder()
		: m_hwnd(NULL),
		  m_video_in(NULL),
		  m_audio_in(NULL),
		  m_compressor(NULL),
		  m_writer(NULL),
		  m_fps(0),
		  m_scale(1),
		  m_recording(false),
		  m_first_frame(false),
		  m_last_time(0)
	{
	}

	AviRecorder::~AviRecorder()
	{
		if (m_compressor)
		{
			delete m_compressor;
			m_compressor = NULL;
		}
		if (m_video_in)
		{
			delete m_video_in;
			m_video_in = NULL;
		}
		if (m_audio_in)
		{
			delete m_audio_in;
			m_audio_in = NULL;
		}
	}

	bool AviRecorder::init(void* hwnd)
	{
		m_hwnd = hwnd;
		return true;
	}

	void AviRecorder::cleanup()
	{
	}

	bool AviRecorder::start(const char* filename, int quality, int scale,
		int fps)
	{
		quality = quality < 0 ? 0 : quality;
		if (quality > 100)
			quality = 100;
		m_scale = scale;
		m_fps = fps;
		m_video_in->init(m_hwnd);
		BITMAPINFO format;
		memset(&format, 0, sizeof(format));
		format.bmiHeader = *m_video_in->get_format();
		int width = format.bmiHeader.biWidth / m_scale;
		int height = format.bmiHeader.biHeight / m_scale;
		int stride = width * 4 + (width * 4) % 4;
		format.bmiHeader.biWidth = width;
		format.bmiHeader.biHeight = height;
		format.bmiHeader.biSizeImage = stride * height;
		m_writer = new Writer;
		m_writer->init(filename, &format.bmiHeader, quality, m_fps);
		m_audio_in = new AudioIn;
		m_audio_in->init();
		m_audio_in->start(m_writer->get_thread_id());
		m_compressor = new Compressor;
		m_compressor->init(&format, m_writer, quality);
		m_writer->set_fcc_handler(m_compressor->get_fcc_handler());
		m_writer->set_comp_format(m_compressor->get_comp_format());
		m_recording = true;
		m_first_frame = true;
		return true;
	}

	void AviRecorder::stop()
	{
		if (m_recording)
		{
			m_recording = false;
			m_video_in->cleanup();
			m_compressor->cleanup();
			m_writer->cleanup();
			m_audio_in->stop();
			m_audio_in->cleanup();
			if (m_compressor)
			{
				delete m_compressor;
				m_compressor = NULL;
			}
			if (m_writer)
			{
				delete m_writer;
				m_writer = NULL;
			}
			if (m_audio_in)
			{
				delete m_audio_in;
				m_audio_in = NULL;
			}
			if (m_video_in)
			{
				delete m_video_in;
				m_video_in = NULL;
			}
		}
	}

	void AviRecorder::record()
	{
		if (!m_recording)
			return;
		if (m_first_frame)
		{
			m_last_time = timeGetTime();
			m_first_frame = false;
		}
		DWORD now = timeGetTime();
		DWORD elapsed = now - m_last_time;
		DWORD interval = 1000 / m_fps;
		if (elapsed < interval)
		{
			Sleep(1);
			return;
		}
		int frames = elapsed / interval;
		m_last_time = now - elapsed % interval;
		m_video_in->capture();
		VideoFrame* frame;
		while (!(frame = m_compressor->get_frame()))
			Sleep(1);
		BITMAPINFOHEADER format = *m_video_in->get_format();
		VideoFrame* source = m_video_in->get_frame();
		if (!source)
		{
			stop();
			return;
		}
		if (m_scale == 1)
		{
			int stride = format.biSizeImage / format.biHeight;
			for (int y = 0; y < format.biHeight; ++y)
				memcpy(frame->m_data + y * stride,
					source->m_data + (format.biHeight - y - 1) * stride,
					stride);
		}
		else
		{
			int width = format.biWidth / m_scale;
			int height = format.biHeight / m_scale;
			int stride = width * 4 + (width * 4) % 4;
			int source_stride = format.biSizeImage / format.biHeight;
			unsigned char* data = source->m_data;
			unsigned char* output = frame->m_data;
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					DWORD pixel =
						*(DWORD*)(data + (y * source_stride + x * 4) * m_scale);
					unsigned char* dest =
						output + (height - y - 1) * stride + x * 4;
					*dest++ = (unsigned char)pixel;
					*dest++ = (unsigned char)(pixel >> 8);
					*dest = (unsigned char)(pixel >> 16);
				}
			}
		}
		frame->m_frames = frames;
		m_compressor->queue(frame);
	}

	void AviRecorder::unit_test()
	{
		m_video_in->capture();
		VideoFrame* frame;
		while (!(frame = m_compressor->get_frame()))
			;
		BITMAPINFOHEADER format = *m_video_in->get_format();
		int width = format.biWidth / m_scale;
		int height = format.biHeight / m_scale;
		int stride = width * 4 + (width * 4) % 4;
		int source_stride = format.biSizeImage / format.biHeight;
		unsigned char* data = m_video_in->get_frame()->m_data;
		unsigned char* output = frame->m_data;
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				DWORD pixel =
					*(DWORD*)(data + (y * source_stride + x * 4) * m_scale);
				unsigned char* dest =
					output + (height - y - 1) * stride + x * 4;
				*dest++ = (unsigned char)pixel;
				*dest++ = (unsigned char)(pixel >> 8);
				*dest = (unsigned char)(pixel >> 16);
			}
		}
		frame->m_frames = 1;
		m_compressor->queue(frame);
	}
}
