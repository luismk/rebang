// Original: d:\build\custom\temp\code\client\wangreal\include\wresrccache.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WRESRCCACHE_H
#define PG_CLIENT_WANGREAL_INCLUDE_WRESRCCACHE_H

#include "..\..\defines.h"
#include "wlist.h"
#include "wmemblock.h"

// A named cache of resources: each origin (a loaded resource, by name) and
// the resources cloned from it (PDB layout, size 0x40).
template <class T>
class WResrcCache {
	LAYOUT_CHECK_FRIEND;
public:
	struct w_origin {
		T resource;         // 0x0
		int count;          // 0x4
		int checkflag;      // 0x8
		char name[1];       // 0xc
	};
	struct w_cache {
		w_origin* origin;   // 0x0
		T resource;         // 0x4
	};

	WResrcCache(void) : resrcList(32, 32), originList(16, 32) {}           // wresrccache.h:28


	~WResrcCache(void) {                                                   // wresrccache.h:32



		w_origin* origin = originList.Start();
		while (origin) {

			g_mem.Free(origin); origin = originList.Next();
		}




		w_cache* cache = resrcList.Start();
		while (cache) {

			g_mem.Free(cache); cache = resrcList.Next();
		}




	}
	T FindResource(const char* name);
	T FirstOrigin(void);
	T NextOrigin(void);
	T FirstResource(T resource);
	T FirstResource(const char* name);
	T FirstResource(void);
	T NextResource(T resource);
	T NextResource(const char* name);
	T NextResource(void);
	T FindOrigin(T resource);
	T FindOrigin(const char* name);
	bool IsOrigin(T resource);
	const char* GetOriginName(T resource);
	const char* GetName(T resource);
	char* FindIdleOrigin(char* first, int count);
	void ClearOrigin(const char* name);
	bool AddResource(const char* name, T resource);
	bool DeleteResource(T resource);

private:
	void AddOrigin(const char* name, T resource);

	WList<w_cache*> resrcList;      // 0x0
	WList<w_origin*> originList;    // 0x20
};

// wresrccache.h:159
template <class T>
T WResrcCache<T>::FindOrigin(const char* name) {
	w_origin* origin = originList.Find(name);
	return origin ? origin->resource : NULL;
}

// wresrccache.h:241
template <class T>
void WResrcCache<T>::ClearOrigin(const char* name) {
	w_origin* origin = originList.Find(name);

	originList -= origin;

	g_mem.Free(origin);
}

#endif // PG_CLIENT_WANGREAL_INCLUDE_WRESRCCACHE_H
