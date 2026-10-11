// Original: d:\build\custom\temp\code\client\fresh\texturecache\tcachemanager.h
#ifndef PG_CLIENT_FRESH_TEXTURECACHE_TCACHEMANAGER_H
#define PG_CLIENT_FRESH_TEXTURECACHE_TCACHEMANAGER_H

#include <map>

#include "..\..\defines.h"
#include "../client/Fresh/texturecache/rectcache.h"

class Bitmap;

// One bitmap's place in the cache: its rectangle, the cache texture holding
// it and the bitmap. Layout from the PDB (size 0x1c, base RectInfo at 0).
struct sTexCacheInfo : public RectCache::RectInfo {
	// tcachemanager.h:22
	void Set(int index, const Bitmap& bitmap, int handle) {
		idx = index;
		pBitmap = &bitmap;
		texHandle = handle;
	}

	int idx;                    // 0x10
	const Bitmap* pBitmap;      // 0x14
	int texHandle;              // 0x18
};

CHECK_SIZE(sTexCacheInfo, 0x1c);
CHECK_FIELD(sTexCacheInfo, idx, 0x10);
CHECK_FIELD(sTexCacheInfo, pBitmap, 0x14);
CHECK_FIELD(sTexCacheInfo, texHandle, 0x18);

// Packs bitmaps into a set of cache textures. Layout, access and methods from
// the PDB (size 0x48, vfptr at 0); bodies in tcachemanager.cpp.
class TexCacheManager {
	LAYOUT_CHECK_FRIEND;
	enum {
		MAX_INFOLIST = 256
	};
	typedef std::map<const Bitmap*, sTexCacheInfo> CACHE_INFO_MAP;

public:
	TexCacheManager(void);
	virtual ~TexCacheManager(void);

	bool Init(int maxCache, int texWidth, int texHeight, int blockWidth, int blockHeight);
	void ClearAll(void);

	// tcachemanager.h:72
	const sTexCacheInfo* Draw(const Bitmap& bitmap) {
		const sTexCacheInfo* pInfo = Get(bitmap);
		if (pInfo) {
			return pInfo;
		}

		return Add(bitmap);
	}

	void UpdateTextureCacheInfo(const sTexCacheInfo& info);
	void RefreshTexCache(const Bitmap& bitmap);
	void InvalidateCache(const Bitmap& bitmap);
	int GetCacheTexture(int index);

protected:
	void ReleaseAllTextures(void);
	bool CreateTexture(unsigned long format);
	void FillTexture(const sTexCacheInfo& info);
	const sTexCacheInfo* Get(const Bitmap& bitmap) const;
	const sTexCacheInfo* Add(const Bitmap& bitmap);
	void Clear(int index);

	int* m_aTexture;                // 0x4
	RectCache m_Cache;              // 0x8
	int m_nMaxCache;                // 0x2c
	int m_CurCacheIdx;              // 0x30
	unsigned long m_texWidth;       // 0x34
	unsigned long m_texHeight;      // 0x38
	CACHE_INFO_MAP m_infoMap;       // 0x3c
};

CHECK_SIZE(TexCacheManager, 0x48);
BEGIN_LAYOUT_CHECK(TexCacheManager)
	CHECK_FIELD(TexCacheManager, m_aTexture, 0x4);
	CHECK_FIELD(TexCacheManager, m_Cache, 0x8);
	CHECK_FIELD(TexCacheManager, m_nMaxCache, 0x2c);
	CHECK_FIELD(TexCacheManager, m_CurCacheIdx, 0x30);
	CHECK_FIELD(TexCacheManager, m_texWidth, 0x34);
	CHECK_FIELD(TexCacheManager, m_texHeight, 0x38);
	CHECK_FIELD(TexCacheManager, m_infoMap, 0x3c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_TEXTURECACHE_TCACHEMANAGER_H
