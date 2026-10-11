// Original: d:\build\custom\temp\code\client\fresh\texturecache\tcachemanager.cpp
#include <string.h>

#include "../client/Fresh/texturecache/tcachemanager.h"
#include "../client/Wangreal/include/bitmap.h"
#include "../client/Wangreal/include/wresrcmng.h"

// tcachemanager.cpp:15
TexCacheManager::TexCacheManager(void)
	: m_aTexture(NULL), m_nMaxCache(0), m_CurCacheIdx(0), m_texWidth(0), m_texHeight(0) {
}

// tcachemanager.cpp:19
TexCacheManager::~TexCacheManager(void) {
	ClearAll();

	ReleaseAllTextures();
}

// tcachemanager.cpp:26
bool TexCacheManager::Init(int nMaxCache, int w, int h, int bw, int bh) {
	m_nMaxCache = nMaxCache;
	m_CurCacheIdx = 0;
	m_texWidth = w;
	m_texHeight = h;
	m_Cache.Init(w, h, bw, bh);

	ReleaseAllTextures();

	m_aTexture = new int[nMaxCache];
	memset(m_aTexture, 0, nMaxCache * sizeof(int));

	return true;
}

// tcachemanager.cpp:49
void TexCacheManager::ClearAll(void) {
	m_Cache.Clear();
	m_CurCacheIdx = 0;
	m_infoMap.clear();
}

// tcachemanager.cpp:56
void TexCacheManager::Clear(int idx) {
	m_Cache.Clear();

	CACHE_INFO_MAP::iterator it = m_infoMap.begin();
	while (it != m_infoMap.end()) {
		if (it->second.idx == idx) {
			CACHE_INFO_MAP::iterator itErase = it++;
			m_infoMap.erase(itErase);
		} else {
			++it;
		}
	}
}

// tcachemanager.cpp:76
const sTexCacheInfo* TexCacheManager::Get(const Bitmap& bitmap) const {
	CACHE_INFO_MAP::const_iterator it = m_infoMap.find(&bitmap);
	if (it != m_infoMap.end()) {
		return &it->second;
	}

	return NULL;
}

// tcachemanager.cpp:89
void TexCacheManager::UpdateTextureCacheInfo(const sTexCacheInfo& info) {
	FillTexture(info);
}

// tcachemanager.cpp:94
int TexCacheManager::GetCacheTexture(int index) {
	return m_aTexture[index];
}

// tcachemanager.cpp:99
void TexCacheManager::ReleaseAllTextures(void) {
	if (m_aTexture) {
		for (int i = 0; i < m_nMaxCache; i++) {
			if (g_resrcmng && m_aTexture[i]) { g_resrcmng->Release(m_aTexture[i]); m_aTexture[i] = 0; }
		}
		if (m_aTexture) { delete[] m_aTexture; m_aTexture = NULL; }
	}
}

// tcachemanager.cpp:112
bool TexCacheManager::CreateTexture(unsigned long texIdx) {
	Bitmap temp(m_texWidth, m_texHeight, 32);
	memset(temp.vram, 0, temp.Size());

	m_aTexture[texIdx] = g_resrcmng->UploadTexture(NULL, &temp, 0x80000000, NULL);

	return m_aTexture[texIdx] > 0;
}

// tcachemanager.cpp:126
void TexCacheManager::FillTexture(const sTexCacheInfo& info) {
	tagRECT r;
	r.left = info.rcPixel.left;
	r.top = info.rcPixel.top;
	r.right = info.rcPixel.right;
	r.bottom = info.rcPixel.bottom;

	if (info.pBitmap->BitsPerPixel() == 32) {
		g_resrcmng->FixTexture(info.texHandle, const_cast<Bitmap*>(info.pBitmap), 0, &r);
	}
}

// tcachemanager.cpp:149
const sTexCacheInfo* TexCacheManager::Add(const Bitmap& bitmap) {
	sTexCacheInfo& info = m_infoMap[&bitmap];

	if (m_Cache.Add(info, bitmap.Width(), bitmap.Height()) == true) {
		if (m_aTexture[m_CurCacheIdx] == 0) {
			CreateTexture(m_CurCacheIdx);
		}
		info.Set(m_CurCacheIdx, bitmap, m_aTexture[m_CurCacheIdx]);
		FillTexture(info);
		return &info;
	}

	m_CurCacheIdx = (m_CurCacheIdx + 1) % m_nMaxCache;
	Clear(m_CurCacheIdx);

	if (m_Cache.Add(info, bitmap.Width(), bitmap.Height()) == true) {
		if (m_aTexture[m_CurCacheIdx] == 0) {
			CreateTexture(m_CurCacheIdx);
		}
		info.Set(m_CurCacheIdx, bitmap, m_aTexture[m_CurCacheIdx]);
		FillTexture(info);
		return &info;
	}

	CACHE_INFO_MAP::iterator it = m_infoMap.find(&bitmap);
	if (it != m_infoMap.end()) {
		m_infoMap.erase(it);
	}

	return NULL;
}

// tcachemanager.cpp:191
void TexCacheManager::RefreshTexCache(const Bitmap& bitmap) {
	const sTexCacheInfo* pInfo = Get(bitmap);
	if (pInfo) {
		UpdateTextureCacheInfo(*pInfo);
	}
}

// tcachemanager.cpp:200
void TexCacheManager::InvalidateCache(const Bitmap& bitmap) {
	CACHE_INFO_MAP::iterator it = m_infoMap.find(&bitmap);
	if (it != m_infoMap.end()) {
		if (it->second.idx == m_CurCacheIdx) {
			Clear(it->second.idx);
		} else {
			m_infoMap.erase(it);
		}
	}
}
