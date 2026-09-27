#pragma once

typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned char uchar;

enum wUnitMode
{
	W_UNIT_XPOS = 0,
	W_UNIT_YPOS = 1,
	W_UNIT_WIDTH = 2,
	W_UNIT_HEIGHT = 3
};

struct _WPOINT
{
	float x, y;
};
struct _WRECT
{
	float x, y, w, h;
};

class WPoint : public _WPOINT
{
public:
	WPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	WPoint() { }
	WPoint operator+(WPoint& p) const { return WPoint(x + p.x, y + p.y); }
};

class WRect : public _WRECT
{
public:
	WRect(const _WRECT& rect)
	{
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;
	}
	WRect(const WRect& rect)
	{
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;
	}
	float Right() const { return w + x; }
	float Bottom() const { return h + y; }
	bool IsInRect(const WPoint& point)
	{
		if (point.x >= x && point.x <= x + w && point.y >= y &&
			point.y <= y + h)
			return true;
		return false;
	}

	__forceinline WRect() { }

	template <class Width, class Height>
	WRect(float x, float y, Width width, Height height)
	{
		this->x = x;
		this->y = y;
		this->w = width;
		this->h = height;
	}
};

template <>
inline WRect::WRect(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->w = width;
	this->h = height;
}
