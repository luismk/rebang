// Original: d:\build\custom\temp\code\client\fresh\texturecache\rectcache.cpp
#include "../client/Fresh/texturecache/rectcache.h"

// rectcache.cpp:14
RectCache::RectCache(void) {
	m_Width = m_Height = m_BlockWidth = m_BlockHeight = 0;
	m_BlockCountW = 0;
	m_BlockCountH = 0;

	m_ppTable = NULL;
	m_EmptyArea = 0;
}

// rectcache.cpp:24
RectCache::~RectCache(void) {
	if (m_ppTable != NULL) {
		delete[] m_ppTable;
		m_ppTable = NULL;
	}
}

// rectcache.cpp:29
void RectCache::Init(int width, int height, int blockWidth, int blockHeight) {
	m_Width = width;
	m_Height = height;
	m_BlockWidth = blockWidth;
	m_BlockHeight = blockHeight;

	m_BlockCountW = width / blockWidth;
	m_BlockCountH = height / blockHeight;

	if (m_ppTable != NULL) {
		delete[] m_ppTable;
		m_ppTable = NULL;
	}

	m_ppTable = new const RectInfo*[m_BlockCountW * m_BlockCountH];
	Clear();
}

// rectcache.cpp:46
void RectCache::Clear(void) {
	for (int i = 0; i < m_BlockCountW * m_BlockCountH; i++) {
		m_ppTable[i] = NULL;
	}

	m_EmptyArea = m_BlockCountW * m_BlockCountH;
}

// rectcache.cpp:54
bool RectCache::Add(RectInfo& info, int width, int height) {
	const int wInBlock = PixelToBlock_W(width);
	const int hInBlock = PixelToBlock_H(height);

	const int blockArea = wInBlock * hInBlock;
	if (blockArea > m_EmptyArea) {
		return false;
	}

	const int xMax = m_BlockCountW - wInBlock + 1;
	const int yMax = m_BlockCountH - hInBlock + 1;

	for (int y = 0; y < yMax; ) {
		int x = 0;
		LPRECTINFO pMinHeightInfo = NULL;
		while (x < xMax) {
			LPRECTINFO pInfo = NULL;
			eCRRes res = CheckRect(x, y, wInBlock, hInBlock, pInfo, pMinHeightInfo);
			switch (res) {
			case eCRRes_RectOnX:
				if (x >= pInfo->rcBlock.right) {
					return false;
				}

				x = pInfo->rcBlock.right;
				break;

			case eCRRes_RectOnY:
				x = SkipX(x + wInBlock, y, pInfo->rcBlock.right < xMax ? pInfo->rcBlock.right : xMax, pMinHeightInfo);
				break;

			case eCRRes_Empty:
				info.rcBlock.left = x;
				info.rcBlock.top = y;
				info.rcBlock.right = x + wInBlock;
				info.rcBlock.bottom = y + hInBlock;

				info.rcPixel.left = BlockToPixel_X(x);
				info.rcPixel.top = BlockToPixel_Y(y);
				info.rcPixel.right = info.rcPixel.left + width;
				info.rcPixel.bottom = info.rcPixel.top + height;

				FillRect(info);

				m_EmptyArea -= blockArea;

				return true;

			case eCRRes_Error:
				return false;
			}
		}

		if (pMinHeightInfo) {
			if (pMinHeightInfo->rcBlock.bottom > y) {
				y = pMinHeightInfo->rcBlock.bottom;
			} else {
				++y;
			}
		} else {
			++y;
		}
	}

	return false;
}
