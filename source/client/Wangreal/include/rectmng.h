// Original: d:\build\custom\temp\code\client\wangreal\include\rectmng.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_RECTMNG_H
#define PG_CLIENT_WANGREAL_INCLUDE_RECTMNG_H

#include "wmempak.h"

// PDB layout, size 0x18.
struct EmptyRect {
	int x1;             // 0x0
	int y1;             // 0x4
	int x2;             // 0x8
	int y2;             // 0xC
	int extent;         // 0x10
	EmptyRect* next;    // 0x14
};
CHECK_SIZE(EmptyRect, 0x18);
CHECK_FIELD(EmptyRect, x1, 0x0);
CHECK_FIELD(EmptyRect, y1, 0x4);
CHECK_FIELD(EmptyRect, x2, 0x8);
CHECK_FIELD(EmptyRect, y2, 0xC);
CHECK_FIELD(EmptyRect, extent, 0x10);
CHECK_FIELD(EmptyRect, next, 0x14);

// PDB layout, size 0x14.
class WRectManager {
	LAYOUT_CHECK_FRIEND;
public:
	WRectManager(int width, int height) {
		Clear(width, height);
	}
	WRectManager() {
	}
	void Clear(int width, int height) {
		mem.Clear();
		list = mem.Alloc();
		list->x1 = list->y1 = 0;
		list->x2 = width;
		list->y2 = height;
		list->next = NULL;
	}
	int Alloc(int width, int height, int& x, int& y) {
		EmptyRect* rect = Find(width, height);
		if (!rect) {
			return 1;
		}
		Divide(rect, width, height, x, y);
		return 0;
	}

private:
	void Divide(EmptyRect* rect, int width, int height, int& x, int& y) {
		EmptyRect* extra;
		x = rect->x1;
		y = rect->y1;
		if (rect->x2 - rect->x1 >= rect->y2 - rect->y1) {
			if (rect->y2 - rect->y1 > height) {
				extra = mem.Alloc();
				extra->x1 = rect->x1;
				extra->y1 = rect->y1 + height;
				extra->x2 = rect->x1 + width;
				extra->y2 = rect->y2;
				extra->extent = (extra->y2 - extra->y1) * (extra->x2 - extra->x1);
				extra->next = rect->next;
				rect->next = extra;
			}
			rect->x1 += width;
			rect->extent = (rect->y2 - rect->y1) * (rect->x2 - rect->x1);
		} else {
			if (rect->x2 - rect->x1 > width) {
				extra = mem.Alloc();
				extra->x1 = rect->x1 + width;
				extra->y1 = rect->y1;
				extra->x2 = rect->x2;
				extra->y2 = rect->y1 + height;
				extra->extent = (extra->y2 - extra->y1) * (extra->x2 - extra->x1);
				extra->next = rect->next;
				rect->next = extra;
			}
			rect->y1 += height;
			rect->extent = (rect->x2 - rect->x1) * (rect->y2 - rect->y1);
		}
	}
	EmptyRect* Find(int width, int height) {
		EmptyRect* found = NULL;
		int extent;
		for (EmptyRect* rect = list; rect; rect = rect->next) {
			if (rect->x2 - rect->x1 >= width && rect->y2 - rect->y1 >= height &&
				(!found || extent > rect->extent)) {
				found = rect;
				extent = (rect->x2 - rect->x1) * (rect->y2 - rect->y1);
			}
		}
		return found;
	}
	WMemPak<EmptyRect> mem;     // 0x0
	EmptyRect* list;            // 0x10
};
CHECK_SIZE(WRectManager, 0x14);
BEGIN_LAYOUT_CHECK(WRectManager)
	CHECK_FIELD(WRectManager, mem, 0x0);
	CHECK_FIELD(WRectManager, list, 0x10);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_RECTMNG_H
