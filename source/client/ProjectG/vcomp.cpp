#include <windows.h>
#include <vfw.h>
#include "vcomp.h"

namespace ntg
{

	VideoCompressor::VideoCompressor()
	{
		memset(&m_compvars, 0, sizeof(m_compvars));
		memset(&m_format, 0, sizeof(m_format));
	}

	VideoCompressor::~VideoCompressor()
	{
	}

	bool VideoCompressor::init(const BITMAPINFO* format, int quality)
	{
		memcpy(&m_format, format, sizeof(m_format));

		memset(&m_compvars, 0, sizeof(m_compvars));
		m_compvars.cbSize = sizeof(m_compvars);
		m_compvars.dwFlags = ICMF_COMPVARS_VALID;
		m_compvars.fccType = ICTYPE_VIDEO;
		m_compvars.fccHandler = mmioFOURCC('m', 's', 'v', 'c');
		m_compvars.lQ = quality * 100;

		m_compvars.hic = ICOpen(m_compvars.fccType, m_compvars.fccHandler,
			ICMODE_FASTCOMPRESS);

		if (m_compvars.hic == NULL)
			return false;

		return true;
	}

	void VideoCompressor::cleanup()
	{
		ICCompressorFree(&m_compvars);

		if (m_compvars.hic)
		{
			ICClose(m_compvars.hic);
			m_compvars.hic = NULL;
		}
	}

	bool VideoCompressor::start()
	{
		return ICSeqCompressFrameStart(&m_compvars, &m_format) == TRUE;
	}

	void VideoCompressor::stop()
	{
		ICSeqCompressFrameEnd(&m_compvars);
	}

	void* VideoCompressor::compress(void* data, int* size)
	{
		BOOL key;
		LONG compressed_size;
		void* result;

		result =
			ICSeqCompressFrame(&m_compvars, 0, data, &key, &compressed_size);

		if (result)
			*size = compressed_size;

		return result;
	}

	bool VideoCompressor::get_format(BITMAPINFO* format)
	{
		return ICCompressGetFormat(m_compvars.hic, &m_format, format) ==
			ICERR_OK;
	}

}
