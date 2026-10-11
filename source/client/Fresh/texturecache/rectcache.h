// Original: d:\build\custom\temp\code\client\fresh\texturecache\rectcache.h
#ifndef PG_CLIENT_FRESH_TEXTURECACHE_RECTCACHE_H
#define PG_CLIENT_FRESH_TEXTURECACHE_RECTCACHE_H

#include "..\..\defines.h"
#include "../shared/coord.h"

// Allocator of rectangles inside a texture, on a grid of blocks. Layout,
// access and methods from the PDB (size 0x24, vfptr at 0); bodies in
// rectcache.cpp.
class RectCache {
	LAYOUT_CHECK_FRIEND;
public:
	struct RectInfo {
		_RectangleSHORT rcPixel;    // 0x0
		_RectangleSHORT rcBlock;    // 0x8
	};
	typedef const RectInfo* LPRECTINFO;

protected:
	enum eCRRes {
		eCRRes_Empty = 0,
		eCRRes_RectOnX = 1,
		eCRRes_RectOnY = 2,
		eCRRes_Error = 3
	};

public:
	RectCache(void);
	virtual ~RectCache(void);

	void Init(int width, int height, int blockWidth, int blockHeight);
	void Clear(void);
	int Width(void) const;
	int Height(void) const;
	bool Add(RectInfo& info, int width, int height);

protected:
	eCRRes CheckRect(int xInBlock, int yInBlock, int wInBlock, int hInBlock, LPRECTINFO& pResInfo, LPRECTINFO& pMinHeightInfo);
	int SkipX(int xInBlock, int yInBlock, int xDest, LPRECTINFO& pMinHeightInfo);
	void FillRect(const RectInfo& info);
	int PixelToBlock_W(int w) const;
	int PixelToBlock_H(int h) const;
	int BlockToPixel_X(int x) const;
	int BlockToPixel_Y(int y) const;

	int m_Width;                    // 0x4
	int m_Height;                   // 0x8
	int m_BlockWidth;               // 0xc
	int m_BlockHeight;              // 0x10
	int m_BlockCountW;              // 0x14
	int m_BlockCountH;              // 0x18
	int m_EmptyArea;                // 0x1c
	const RectInfo** m_ppTable;     // 0x20
};

CHECK_SIZE(RectCache::RectInfo, 0x10);
CHECK_FIELD(RectCache::RectInfo, rcPixel, 0x0);
CHECK_FIELD(RectCache::RectInfo, rcBlock, 0x8);
CHECK_SIZE(RectCache, 0x24);
BEGIN_LAYOUT_CHECK(RectCache)
	CHECK_FIELD(RectCache, m_Width, 0x4);
	CHECK_FIELD(RectCache, m_Height, 0x8);
	CHECK_FIELD(RectCache, m_BlockWidth, 0xc);
	CHECK_FIELD(RectCache, m_BlockHeight, 0x10);
	CHECK_FIELD(RectCache, m_BlockCountW, 0x14);
	CHECK_FIELD(RectCache, m_BlockCountH, 0x18);
	CHECK_FIELD(RectCache, m_EmptyArea, 0x1c);
	CHECK_FIELD(RectCache, m_ppTable, 0x20);
END_LAYOUT_CHECK

// RectCache.cpp defines RECTCACHE_RECTCACHE_INLINE empty before its includes:
// the original holds the out-of-line copy of these header methods in
// RectCache's object; every other unit inlines them.
#ifndef RECTCACHE_RECTCACHE_INLINE
#define RECTCACHE_RECTCACHE_INLINE inline
#endif

// rectcache.h:69
RECTCACHE_RECTCACHE_INLINE int RectCache::PixelToBlock_W(int w) const {
	return (w + m_BlockWidth - 1) / m_BlockWidth;
}

// rectcache.h:75
RECTCACHE_RECTCACHE_INLINE int RectCache::PixelToBlock_H(int h) const {
	return (h + m_BlockHeight - 1) / m_BlockHeight;
}

// rectcache.h:81
RECTCACHE_RECTCACHE_INLINE int RectCache::BlockToPixel_X(int x) const {
	return x * m_BlockWidth;
}

// rectcache.h:87
RECTCACHE_RECTCACHE_INLINE int RectCache::BlockToPixel_Y(int y) const {
	return y * m_BlockHeight;
}

// rectcache.h:93
RECTCACHE_RECTCACHE_INLINE RectCache::eCRRes RectCache::CheckRect(int xInBlock, int yInBlock, int wInBlock, int hInBlock, LPRECTINFO& pResInfo, LPRECTINFO& pMinHeightInfo) {
	int line = m_BlockCountW * yInBlock;

	for (int x = xInBlock; x < xInBlock + wInBlock; ++x) {
		LPRECTINFO pInfo = m_ppTable[line + x];
		if (pInfo) {
			if (pInfo->rcBlock.left < 0 || pInfo->rcBlock.left > m_BlockCountW
				|| pInfo->rcBlock.right < 0 || pInfo->rcBlock.right > m_BlockCountW
				|| pInfo->rcBlock.top < 0 || pInfo->rcBlock.top > m_BlockCountH
				|| pInfo->rcBlock.bottom < 0 || pInfo->rcBlock.bottom > m_BlockCountH) {
				pResInfo = pInfo;
				return eCRRes_Error;
			}

			if (!pMinHeightInfo || pInfo->rcBlock.bottom < pMinHeightInfo->rcBlock.bottom) {
				pMinHeightInfo = pInfo;
			}

			pResInfo = pInfo;
			return eCRRes_RectOnX;
		}
	}

	line += m_BlockCountW;
	for (int y = 1; y < hInBlock; ++y, line += m_BlockCountW) {
		LPRECTINFO pInfo = m_ppTable[line + xInBlock];
		if (pInfo) {
			pResInfo = pInfo;
			return eCRRes_RectOnY;
		}
	}

	line = yInBlock * m_BlockCountW + m_BlockCountW;
	for (int y = 1; y < hInBlock; ++y, line += m_BlockCountW) {
		LPRECTINFO pInfo = m_ppTable[line + xInBlock + wInBlock - 1];
		if (pInfo) {
			if (!pMinHeightInfo || pInfo->rcBlock.bottom < pMinHeightInfo->rcBlock.bottom) {
				pMinHeightInfo = pInfo;
			}

			pResInfo = pInfo;
			return eCRRes_RectOnX;
		}
	}

	pResInfo = NULL;
	return eCRRes_Empty;
}

// rectcache.h:153
RECTCACHE_RECTCACHE_INLINE int RectCache::SkipX(int xInBlock, int yInBlock, int xDest, LPRECTINFO& pMinHeightInfo) {
	const int line = m_BlockCountW * yInBlock;
	int x;

	for (x = xInBlock; x < xDest; ) {
		LPRECTINFO pInfo = m_ppTable[x + line];
		if (pInfo) {
			if (!pMinHeightInfo || pInfo->rcBlock.bottom < pMinHeightInfo->rcBlock.bottom) {
				pMinHeightInfo = pInfo;
			}

			x = pInfo->rcBlock.right;
		} else {
			++x;
		}
	}

	return x;
}

// rectcache.h:179
RECTCACHE_RECTCACHE_INLINE void RectCache::FillRect(const RectInfo& info) {
	int line = info.rcBlock.top * m_BlockCountW;

	for (int x = info.rcBlock.left; x < info.rcBlock.right; ++x) {
		m_ppTable[line + x] = &info;
	}

	int height = info.rcBlock.Height();

	line += m_BlockCountW;
	for (int y = 1; y < height; ++y, line += m_BlockCountW) {
		m_ppTable[line + info.rcBlock.left] = &info;
	}
}

#endif // PG_CLIENT_FRESH_TEXTURECACHE_RECTCACHE_H
