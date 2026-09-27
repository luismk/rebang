#pragma once
#include "../../Wangreal/include/wtypes.h"
#include "../../Wangreal/include/woverlay.h"

class Bitmap;
class TexCacheManager;
class WFont;
class WView;

extern WView* g_view;

class FrGraphicInterface
{
public:
	FrGraphicInterface(WView* view);
	virtual ~FrGraphicInterface();
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
	void SetClippingArea(WRect* rect);
	void SetAlpha(float alpha);
	float GetAlpha();
	int GetFontHeight();
	float PrintText(const WPoint& point, unsigned long style, const char* text,
		Bitmap* bitmap);
	float PrintText11(const WPoint& point, unsigned long style,
		const char* text, Bitmap* bitmap);
	float GetTextExtend11(const char* text);
	void Reset();
	void Line(const WPoint& start, const WPoint& end, unsigned long first,
		unsigned long second, unsigned long type = 0) const
	{
		g_view->DrawLine2D(start, end, first, second, type);
	}
	float GetTextExtend(const char* text);
	void SetTextColor(unsigned long color, unsigned long outlineColor);
	void SetTextStyle(unsigned long style);
	float Print(const WPoint& point, unsigned long style, const char* format,
		...);
	float GetViewWidth();
	float GetViewHeight();

protected:
	WView* m_pView;
	TexCacheManager* m_pCacheManager;
	float m_alpha;
	unsigned long m_textColor;
	unsigned long m_textOutlineColor;
	unsigned long m_textStyle;
	WFont* m_pFont12;
	WFont* m_pFont11;
	int m_maxTexCache;
	int m_cacheTexSize;
};

inline float FrGraphicInterface::GetViewWidth()
{
	return m_pView ? m_pView->GetWidth() : 0.0f;
}

inline float FrGraphicInterface::GetViewHeight()
{
	return m_pView ? m_pView->GetHeight() : 0.0f;
}

inline unsigned long FrALPHA(unsigned long color, float alpha)
{
	return ((int)((color >> 24) * alpha) << 24) | (color & 0x00ffffff);
}
