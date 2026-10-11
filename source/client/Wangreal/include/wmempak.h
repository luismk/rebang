// Original: d:\build\custom\temp\code\client\wangreal\include\wmempak.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WMEMPAK_H
#define PG_CLIENT_WANGREAL_INCLUDE_WMEMPAK_H

#include <stdlib.h>
#include "..\..\defines.h"

// A block allocator of T: blocks of blknum items, each block's allocation
// is one char[] holding a listinfo header and the items. Layout from the
// PDB (WMemPak<EmptyRect>).
template <class T>
class WMemPak {
	LAYOUT_CHECK_FRIEND;
public:
	WMemPak(int count = 8) {
		blknum = count;
		pos = count;
		prealloc = NULL;
		idle = NULL;
	}
	~WMemPak() {
		Clear();
		while (prealloc) {
			listinfo* block = prealloc;
			prealloc = prealloc->next;
			delete[](char*)block;
		}
	}
	void Clear() {
		if (prealloc) {
			listinfo* block = prealloc;
			if (block->next) {
				do {
					block = block->next;
				} while (block->next);
			}
			block->next = idle;
		} else {
			prealloc = idle;
		}
		idle = NULL;
		pos = blknum;
	}
	T* Alloc() {
		if (pos >= blknum) {
			listinfo* block = prealloc;
			prealloc = prealloc ? prealloc->next : NULL;
			if (block) {
				block->next = idle;
				idle = block;
			}
			if (!prealloc) {
				prealloc = (listinfo*)new char[sizeof(listinfo) + (blknum - 1) * sizeof(T)];
				prealloc->next = NULL;
			}
			pos = 0;
		}
		return &prealloc->item[pos++];
	}
	void Free(T* item);

private:
	struct listinfo {
		listinfo* next;
		T item[1];
	};
	listinfo* prealloc;     // 0x0
	listinfo* idle;         // 0x4
	int blknum;             // 0x8
	int pos;                // 0xC
};

#endif // PG_CLIENT_WANGREAL_INCLUDE_WMEMPAK_H
