#pragma once
#include "rectcache.h"
#include <map>

class Bitmap;

struct sTexCacheInfo : public RectCache::RectInfo
{
	int idx;
	const Bitmap* pBitmap;
	int texHandle;
	void Set(int index, const Bitmap& bitmap, int handle);
};

class TexCacheManager
{
public:
	TexCacheManager();
	virtual ~TexCacheManager();
	bool Init(int maxCache, int width, int height, int blockWidth,
		int blockHeight);
	void ClearAll();
	const sTexCacheInfo* Draw(const Bitmap& bitmap)
	{
		const sTexCacheInfo* info = Get(bitmap);
		if (info)
			return info;
		return Add(bitmap);
	}
	void UpdateTextureCacheInfo(const sTexCacheInfo& info);
	void RefreshTexCache(const Bitmap& bitmap);
	void InvalidateCache(const Bitmap& bitmap);
	int GetCacheTexture(int index);

protected:
	void ReleaseAllTextures();
	bool CreateTexture(unsigned long index);
	void FillTexture(const sTexCacheInfo& info);
	const sTexCacheInfo* Get(const Bitmap& bitmap) const;
	const sTexCacheInfo* Add(const Bitmap& bitmap);
	void Clear(int index);
	int* m_aTexture;
	RectCache m_Cache;
	int m_nMaxCache;
	int m_CurCacheIdx;
	unsigned long m_texWidth;
	unsigned long m_texHeight;
	std::map<const Bitmap*, sTexCacheInfo> m_infoMap;
};
