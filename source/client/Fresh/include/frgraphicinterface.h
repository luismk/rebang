#pragma once
#include "../../Wangreal/include/wtypes.h"
#include "../../Wangreal/include/woverlay.h"

class Bitmap;
class WView;

extern WView* g_view;

class FrGraphicInterface
{
public:
	void Box(const _WRECT& rc, unsigned long color, unsigned long z = 0,
		float alpha = 0.0f) const
	{
		WOverlay::DrawBox(g_view, rc, z, color, alpha);
	}
	void LineBox(const _WRECT& rc, unsigned long color, unsigned long z = 0,
		float alpha = 0.0f) const
	{
		WOverlay::DrawLineBox(g_view, rc, z, color);
	}
	void DrawTexture(const Bitmap* bitmap, const WRect& dst,
		unsigned long color, int flags) const;
	void DrawTexture(const Bitmap* bitmap, const WRect& src, const WRect& dst,
		unsigned long color, int flags) const;
	void SetTextColor(unsigned long color, unsigned long outlineColor);
	void SetTextStyle(unsigned long style);
	float Print(const WPoint& point, unsigned long style, const char* format,
		...);
};

inline unsigned long FrALPHA(unsigned long color, float alpha)
{
	return ((int)((color >> 24) * alpha) << 24) | (color & 0x00ffffff);
}
