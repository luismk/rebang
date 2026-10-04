#include "aviwriter.h"

namespace ntg
{
	AviWriter::AviWriter()
		: m_quality(0),
		  m_fps(0),
		  m_fcc_handler(0),
		  m_video_frames(0),
		  m_video_max_size(0),
		  m_audio_chunks(0),
		  m_audio_max_size(0),
		  m_hmmio(NULL)
	{
		memset(&m_bih, 0, sizeof(m_bih));
		memset(&m_comp_format, 0, sizeof(m_comp_format));
		memset(&m_wave_format, 0, sizeof(m_wave_format));
	}

	AviWriter::~AviWriter()
	{
	}

	bool AviWriter::open(const char* filename, const BITMAPINFOHEADER* bih,
		int quality, int fps)
	{
		m_hmmio = mmioOpen((char*)filename, NULL, MMIO_CREATE | MMIO_WRITE);
		if (!m_hmmio)
			return false;
		mmioSeek(m_hmmio, 2048, SEEK_SET);
		m_bih = *bih;
		m_quality = quality;
		m_fps = fps;
		m_video_frames = 0;
		m_video_max_size = 0;
		m_audio_chunks = 0;
		m_audio_max_size = 0;
		reserve_idxcache();
		return true;
	}

	void AviWriter::close()
	{
		if (m_hmmio)
		{
			write_header();
			m_index.clear();
			mmioFlush(m_hmmio, 0);
			mmioClose(m_hmmio, 0);
			m_hmmio = NULL;
		}
	}

	void AviWriter::set_comp_format(const BITMAPINFO* format)
	{
		m_comp_format = *format;
	}

	void AviWriter::write_header()
	{
		MMCKINFO riff, movi, hdrl, avih, strl, header_chunk, format_chunk, junk;
		LONG pos = mmioSeek(m_hmmio, 0, SEEK_CUR);
		mmioSeek(m_hmmio, 0, SEEK_SET);

		memset(&riff, 0, sizeof(riff));
		riff.fccType = mmioFOURCC('A', 'V', 'I', ' ');
		mmioCreateChunk(m_hmmio, &riff, MMIO_CREATERIFF);
		memset(&hdrl, 0, sizeof(hdrl));
		hdrl.fccType = mmioFOURCC('h', 'd', 'r', 'l');
		mmioCreateChunk(m_hmmio, &hdrl, MMIO_CREATELIST);
		memset(&avih, 0, sizeof(avih));
		avih.ckid = mmioFOURCC('a', 'v', 'i', 'h');
		avih.cksize = sizeof(MainAVIHeader);
		mmioCreateChunk(m_hmmio, &avih, 0);
		MainAVIHeader header;
		memset(&header, 0, sizeof(header));
		header.dwMicroSecPerFrame = MulDiv(1000000, 1, m_fps);
		header.dwMaxBytesPerSec = 0;
		header.dwPaddingGranularity = 0;
		header.dwFlags = AVIF_HASINDEX;
		header.dwTotalFrames = m_video_frames;
		header.dwStreams = 2;
		header.dwSuggestedBufferSize = m_video_max_size;
		header.dwWidth = m_bih.biWidth;
		header.dwHeight = m_bih.biHeight;
		mmioWrite(m_hmmio, (char*)&header, sizeof(header));
		mmioAscend(m_hmmio, &avih, 0);

		memset(&strl, 0, sizeof(strl));
		strl.fccType = mmioFOURCC('s', 't', 'r', 'l');
		mmioCreateChunk(m_hmmio, &strl, MMIO_CREATELIST);
		memset(&header_chunk, 0, sizeof(header_chunk));
		header_chunk.ckid = mmioFOURCC('s', 't', 'r', 'h');
		header_chunk.cksize = 56;
		mmioCreateChunk(m_hmmio, &header_chunk, 0);
		AVIStreamHeader stream;
		memset(&stream, 0, sizeof(stream));
		stream.fccType = streamtypeVIDEO;
		stream.fccHandler = m_fcc_handler;
		stream.dwScale = 1;
		stream.dwRate = m_fps;
		stream.dwLength = 1;
		stream.dwSuggestedBufferSize = m_video_max_size;
		stream.dwQuality = m_quality * 100;
		short rect[4] = { 0, 0, (short)m_bih.biWidth, (short)m_bih.biHeight };
		mmioWrite(m_hmmio, (char*)&stream, 48);
		mmioWrite(m_hmmio, (char*)rect, 8);
		mmioAscend(m_hmmio, &header_chunk, 0);
		memset(&format_chunk, 0, sizeof(format_chunk));
		format_chunk.ckid = mmioFOURCC('s', 't', 'r', 'f');
		format_chunk.cksize = sizeof(BITMAPINFOHEADER);
		mmioCreateChunk(m_hmmio, &format_chunk, 0);
		mmioWrite(m_hmmio, (char*)&m_comp_format, sizeof(BITMAPINFOHEADER));
		mmioAscend(m_hmmio, &format_chunk, 0);
		mmioAscend(m_hmmio, &strl, 0);
		mmioAscend(m_hmmio, &hdrl, 0);

		memset(&strl, 0, sizeof(strl));
		strl.fccType = mmioFOURCC('s', 't', 'r', 'l');
		mmioCreateChunk(m_hmmio, &strl, MMIO_CREATELIST);
		memset(&header_chunk, 0, sizeof(header_chunk));
		header_chunk.ckid = mmioFOURCC('s', 't', 'r', 'h');
		header_chunk.cksize = 56;
		mmioCreateChunk(m_hmmio, &header_chunk, 0);
		memset(&stream, 0, sizeof(stream));
		stream.fccType = streamtypeAUDIO;
		stream.dwScale = 4;
		stream.dwRate = 176400;
		stream.dwLength = m_audio_chunks * 22050;
		stream.dwSampleSize = 4;
		memset(rect, 0, sizeof(rect));
		mmioWrite(m_hmmio, (char*)&stream, 48);
		mmioWrite(m_hmmio, (char*)rect, 8);
		mmioAscend(m_hmmio, &header_chunk, 0);
		memset(&format_chunk, 0, sizeof(format_chunk));
		format_chunk.ckid = mmioFOURCC('s', 't', 'r', 'f');
		format_chunk.cksize = 16;
		mmioCreateChunk(m_hmmio, &format_chunk, 0);
		memset(&m_wave_format, 0, sizeof(m_wave_format));
		m_wave_format.wFormatTag = WAVE_FORMAT_PCM;
		m_wave_format.nChannels = 2;
		m_wave_format.nSamplesPerSec = 44100;
		m_wave_format.nAvgBytesPerSec = 176400;
		m_wave_format.nBlockAlign = 4;
		mmioWrite(m_hmmio, (char*)&m_wave_format, sizeof(m_wave_format));
		int bits = format_chunk.cksize;
		mmioWrite(m_hmmio, (char*)&bits, 2);
		mmioAscend(m_hmmio, &format_chunk, 0);
		mmioAscend(m_hmmio, &strl, 0);
		mmioAscend(m_hmmio, &hdrl, 0);

		memset(&junk, 0, sizeof(junk));
		junk.ckid = mmioFOURCC('J', 'U', 'N', 'K');
		mmioCreateChunk(m_hmmio, &junk, 0);
		mmioSeek(m_hmmio, 2048 - 12, SEEK_SET);
		mmioAscend(m_hmmio, &junk, 0);
		memset(&movi, 0, sizeof(movi));
		movi.fccType = mmioFOURCC('m', 'o', 'v', 'i');
		mmioCreateChunk(m_hmmio, &movi, MMIO_CREATELIST);
		mmioSeek(m_hmmio, pos, SEEK_SET);
		mmioAscend(m_hmmio, &movi, 0);
		write_indexes();
		mmioAscend(m_hmmio, &riff, 0);
	}

	void AviWriter::write_indexes()
	{
		if (!m_index.empty())
		{
			MMCKINFO ck;
			memset(&ck, 0, sizeof(ck));
			ck.ckid = mmioFOURCC('i', 'd', 'x', '1');
			ck.cksize = m_index.size() * sizeof(AVIINDEXENTRY);
			mmioCreateChunk(m_hmmio, &ck, 0);
			mmioWrite(m_hmmio, (char*)&m_index[0], ck.cksize);
			mmioAscend(m_hmmio, &ck, 0);
		}
	}

	void AviWriter::reserve_idxcache()
	{
		if (m_index.size() == m_index.capacity())
			m_index.reserve(m_index.size() + m_fps * 60);
	}

	void AviWriter::index(MMCKINFO* ck, DWORD flags)
	{
		AVIINDEXENTRY entry;
		entry.ckid = ck->ckid;
		entry.dwChunkLength = ck->cksize;
		entry.dwChunkOffset = ck->dwDataOffset - 8;
		entry.dwFlags = flags;
		reserve_idxcache();
		m_index.push_back(entry);
	}

	void AviWriter::write_video(int frames, void* data, int size)
	{
		for (int i = 0; i < frames; ++i)
		{
			MMCKINFO ck;
			memset(&ck, 0, sizeof(ck));
			ck.ckid = mmioFOURCC('0', '0', 'd', 'c');
			ck.cksize = size;
			mmioCreateChunk(m_hmmio, &ck, 0);
			mmioWrite(m_hmmio, (char*)data, size);
			mmioAscend(m_hmmio, &ck, 0);
			++m_video_frames;
			if (m_video_max_size < size)
				m_video_max_size = size;
			index(&ck, AVIIF_KEYFRAME);
		}
	}

	void AviWriter::write_audio(void* data, int size)
	{
		MMCKINFO ck;
		memset(&ck, 0, sizeof(ck));
		ck.ckid = mmioFOURCC('0', '1', 'w', 'b');
		ck.cksize = size;
		mmioCreateChunk(m_hmmio, &ck, 0);
		mmioWrite(m_hmmio, (char*)data, size);
		mmioAscend(m_hmmio, &ck, 0);
		++m_audio_chunks;
		if (m_audio_max_size < size)
			m_audio_max_size = size;
		index(&ck, 0);
	}

	void AviWriter::write_audio_sync(void* data, int size)
	{
		int video_time = (1000 / m_fps) * m_video_frames;
		int audio_time = m_audio_chunks * 500;
		if (video_time > audio_time)
			write_audio(data, (video_time - audio_time) * 176400 / 1000);
	}
}
