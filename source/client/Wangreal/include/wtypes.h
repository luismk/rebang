// Original: d:\build\custom\temp\code\client\wangreal\include\wtypes.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WTYPES_H
#define PG_CLIENT_WANGREAL_INCLUDE_WTYPES_H

// The name clashes with the Platform SDK's wtypes.h (pulled in by
// <windows.h>): include this one by its path from src/,
// #include "wtypes.h".

#include "..\..\defines.h"

// Inline bodies the original emits out of line in exactly one unit. The
// default is inline; the unit that carries the out-of-line copy defines the
// macro empty before its first include (docs/tough_problems/011).
#ifndef FRWND_WTYPES_INLINE
#define FRWND_WTYPES_INLINE inline
#endif
#ifndef FRTOOLTIP_WTYPES_INLINE
#define FRTOOLTIP_WTYPES_INLINE inline
#endif

// Layouts and signatures from the PDB. The inline bodies written here are the
// ones verified against the original COMDATs (wtypes.h line marks); the other
// members are declared only until a unit needs their code.

typedef struct _WPOINT {
	float x;   // 0x0
	float y;   // 0x4
} WPOINT;
CHECK_SIZE(_WPOINT, 0x8);

typedef struct _WSIZE {
	float w;   // 0x0
	float h;   // 0x4
} WSIZE;
CHECK_SIZE(_WSIZE, 0x8);

typedef struct _WRECT {
	float x;   // 0x0
	float y;   // 0x4
	float w;   // 0x8
	float h;   // 0xc
} WRECT;
CHECK_SIZE(_WRECT, 0x10);

class WPoint : public _WPOINT {
public:
	WPoint(void) {}                                        // wtypes.h:68
	WPoint(float px, float py) { x = px; y = py; }         // wtypes.h:69
	WPoint(WPoint& pt) { x = pt.x; y = pt.y; }             // wtypes.h:70
	WPoint(WPoint* pPt);

	void Offset(float dx, float dy);
	void Offset(WPoint& pt);
	int operator==(WPoint& pt) const;
	int operator!=(WPoint& pt) const { return x != pt.x || y != pt.y; }   // wtypes.h:77
	void operator+=(WPoint& pt);
	void operator-=(WPoint& pt);
	WPoint operator+(WPoint& pt) const { return WPoint(x + pt.x, y + pt.y); }   // wtypes.h:82
	WPoint operator-(WPoint& pt) const { return WPoint(x - pt.x, y - pt.y); }   // wtypes.h:83
	WPoint operator-(void) const;
};
CHECK_SIZE(WPoint, 0x8);

// wtypes.h:76
FRWND_WTYPES_INLINE int WPoint::operator==(WPoint& pt) const {
	return (x == pt.x && y == pt.y);
}

class WSize : public _WSIZE {
public:
	WSize(void) {}                                         // wtypes.h:90
	WSize(float sw, float sh) { w = sw; h = sh; }          // wtypes.h:91
	WSize(WSize& size);
	WSize(WSize* pSize);

	WSize operator+(WSize& size) const { return WSize(w + size.w, h + size.h); }   // wtypes.h:101
};
CHECK_SIZE(WSize, 0x8);

class WRect : public _WRECT {
public:
	WRect(void) {}                                         // wtypes.h:110
	template <class TW, class TH>
	WRect(float rx, float ry, TW rw, TH rh) {              // wtypes.h:118
		x = rx;
		y = ry;
		w = (float)rw;
		h = (float)rh;
	}
	template <class TW, class TH>
	WRect(const WPoint& pt, TW rw, TH rh) {                // wtypes.h:121
		x = pt.x;
		y = pt.y;
		w = (float)rw;
		h = (float)rh;
	}
	WRect(const WRect& rc) { x = rc.x; y = rc.y; w = rc.w; h = rc.h; }     // wtypes.h:123
	WRect(const _WRECT& rc) { x = rc.x; y = rc.y; w = rc.w; h = rc.h; }    // wtypes.h:124
	WRect(const WPoint& pt, const WSize& size) {           // wtypes.h:125
		x = pt.x;
		y = pt.y;
		w = size.w;
		h = size.h;
	}

	int operator==(WRect& rc) const;
	int operator!=(WRect& rc) const;
	void operator+=(WSize& size);
	void operator-=(WSize& size);
	WRect operator+(WRect& rc) const { return WRect(x + rc.x, y + rc.y, w + rc.w, h + rc.h); }   // wtypes.h:141
	WRect operator+(WPoint& pt) const { return WRect(x + pt.x, y + pt.y, w, h); }   // wtypes.h:143
	WRect operator-(WPoint& pt) const { return WRect(x - pt.x, y - pt.y, w, h); }   // wtypes.h:144
	WRect operator-(WRect& rc) const;
	// wtypes.h:146-149
	WPoint TopLeft(void) const
	{
		return WPoint(x, y);
	}
	// wtypes.h:151-154
	WSize Size(void) const
	{
		return WSize(w, h);
	}
	float Right(void) const;
	// wtypes.h:162-164
	float Bottom(void) const
	{
		return h + y;
	}
	bool IsInRect(const WPoint& pt);
	bool IsInRect(float px, float py);
};
CHECK_SIZE(WRect, 0x10);

// wtypes.h:138
FRTOOLTIP_WTYPES_INLINE int WRect::operator!=(WRect& rc) const {
	return !(x == rc.x && y == rc.y && w == rc.w && h == rc.h);
}

#endif // PG_CLIENT_WANGREAL_INCLUDE_WTYPES_H
