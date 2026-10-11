// Original: d:\build\custom\temp\code\client\wangreal\include\wmemblock.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WMEMBLOCK_H
#define PG_CLIENT_WANGREAL_INCLUDE_WMEMBLOCK_H
#include <stdlib.h>
#include <string.h>
#include "..\..\defines.h"
// Small-block allocator: m_level pools, pool i holding m_num slots of
// (4 << i) bytes, each pool with a bitmask of its used slots. Requests
// larger than the biggest slot, or that find their pool full, fall back to
// malloc. Layout from the PDB.
class WMemFillBlock {
	LAYOUT_CHECK_FRIEND;
public:
	// wmemblock.h:23
	WMemFillBlock(int num, int level) {
		m_num = (num + 31) & ~31;
		m_level = level;
		m_size = ((1 << level) - 1) * m_num * 4;
		m_ptr = (unsigned char *)malloc(m_size + m_num / 8 * level);
		int i = 0;
		int offset = 0;
		for (; i < m_level; ++i) {
			m_mem[i] = m_ptr + offset;
			offset += m_num << (i + 2);
		}
		for (int i = 0; i < m_level; ++i) {
			m_mask[i] = (unsigned long *)(m_ptr + offset);
			memset(m_mask[i], 0, m_num / 8);
			offset += m_num / 8;
		}
		for (int i = 0; i < m_level; ++i) {
			m_lastidx[i] = 0;
		}
	}
	// wmemblock.h:81
	~WMemFillBlock() {
		free(m_ptr);
	}
	// wmemblock.h:120
	void *Alloc(int size) {
#if PG_ABI32
		if (size <= (1 << (m_level + 1))) {
			for (int i = 0; i < m_level; i++) {
				if (size <= (1 << (i + 2))) {
					int start = m_lastidx[i];
					for (int j = 0; j < m_num; j += 32) {
						if (j + start >= m_num) {
							start -= m_num;
						}
						if (m_mask[i][(j + start) >> 5] != 0xFFFFFFFF) {
							int bit = FindSlot(m_mask[i][(j + start) >> 5]);
							m_mask[i][(j + start) >> 5] |= 1 << bit;
							m_lastidx[i] = j + start;
							return m_mem[i] + ((j + bit + start) << (i + 2));
						}
					}
					break;
				}
			}
		}
#endif
		return malloc(size);
	}
	// wmemblock.h:190
	void Free(void *mem) {
#if PG_ABI32
		int offset = (unsigned char *)mem - m_ptr;
		if (offset < 0 || offset >= m_size) {
			free(mem);
			return;
		}
		int i = 0, base = 0;
		for (; i < m_level; i++) {
			int blockSize = m_num << (i + 2);
			if (offset - base < blockSize) {
				int slot = (offset - base) >> (i + 2);
				m_mask[i][slot >> 5] &= ~(1 << (slot & 31));
				return;
			}
			base += blockSize;
		}
#else
		free(mem);
#endif
	}
private:
	// wmemblock.h:231
	int FindSlot(unsigned long mask) {
		for (int i = 0; i < 32; i++) {
			if (!(mask & (1 << i))) return i;
		}
		return -1;
	}
	unsigned char *m_mem[6];     // 0x0, the first slot of each pool
	unsigned long *m_mask[6];    // 0x18, the used-slot bits of each pool
	unsigned char *m_ptr;        // 0x30, the one malloc'd block
	int m_lastidx[6];            // 0x34, the slot each pool's next search starts at
	int m_num;                   // 0x4C, slots per pool (a multiple of 32)
	int m_size;                  // 0x50, the bytes of all pools
	int m_level;                 // 0x54, the pool count
};
CHECK_SIZE(WMemFillBlock, 0x58);
BEGIN_LAYOUT_CHECK(WMemFillBlock)
	CHECK_FIELD(WMemFillBlock, m_mem, 0x0);
	CHECK_FIELD(WMemFillBlock, m_mask, 0x18);
	CHECK_FIELD(WMemFillBlock, m_ptr, 0x30);
	CHECK_FIELD(WMemFillBlock, m_lastidx, 0x34);
	CHECK_FIELD(WMemFillBlock, m_num, 0x4C);
	CHECK_FIELD(WMemFillBlock, m_size, 0x50);
	CHECK_FIELD(WMemFillBlock, m_level, 0x54);
END_LAYOUT_CHECK
// The allocator of the WList nodes and hash tables.
extern WMemFillBlock g_mem;
// The allocator of the WList nodes, hash tables and keycode copies
// (wlist.h). Wangreal's own objects use g_mem; ProjectG's stdafx.h defined
// WLIST_ALLOC / WLIST_FREE as malloc / free first (its WList instances call
// __imp__malloc). The stand-in of that stdafx.h, toolchain-shim/pch/
// projectg.h, forced into every ProjectG and shared unit, does not define
// them, so its include guard selects malloc here.
#ifndef WLIST_ALLOC
#ifdef PG_TOOLCHAIN_SHIM_PCH_PROJECTG_H
#define WLIST_ALLOC(size) malloc(size)
#define WLIST_FREE(ptr) free(ptr)
#else
#define WLIST_ALLOC(size) g_mem.Alloc(size)
#define WLIST_FREE(ptr) g_mem.Free(ptr)
#endif
#endif
#endif // PG_CLIENT_WANGREAL_INCLUDE_WMEMBLOCK_H