// Original: d:\build\custom\temp\code\client\wangreal\include\woverlay.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WOVERLAY_H
#define PG_CLIENT_WANGREAL_INCLUDE_WOVERLAY_H

#include <windows.h>

#include "..\..\defines.h"
#include "wresource.h"
#include "wvideo.h"
#include "wtypes.h"

class Bitmap;
class WView;

enum wUnitMode {
	W_UNIT_XPOS = 0,
	W_UNIT_YPOS = 1,
	W_UNIT_WIDTH = 2,
	W_UNIT_HEIGHT = 3
};

// A 2D textured quad drawn over the scene (PDB layout, size 0x58). The static
// Draw* helpers draw untextured primitives in screen space.
class WOverlay : public WResource {
	LAYOUT_CHECK_FRIEND;
public:
	WOverlay(void);
	virtual ~WOverlay(void);

	virtual int Load(const char* filename, unsigned long type);
	int Load(const char* filename, Bitmap* bitmap, unsigned long type);
	virtual void Render(WView* view, const _WRECT& dst, const _WRECT& src, int flags, unsigned long color, float z, unsigned char alpha);
	virtual void SetCoordMode(int mode) { m_coordMode = mode; }   // woverlay.h:81

	int GetWidth(void) { return m_texWidth; }     // woverlay.h:70
	int GetHeight(void) { return m_texHeight; }   // woverlay.h:71
	void RenderWithAxis(WView* view, const _WRECT& dst, const _WRECT& src, float ax, float ay, float angle, int flags, unsigned long color, unsigned char alpha);
	void ArcClipRender(WView* view, const _WRECT& dst, const _WRECT& src, float start, float end, int flags, unsigned long color);
	void RenderWithShear(WView* view, const _WRECT& dst, const _WRECT& src, float shear, int flags, unsigned long color);
	void SetClippingArea(WView* view, const WRect* rect);
	void SetClippingArea(const WRect* rect);

	static void DrawLine(WView* view, const _WPOINT& a, const _WPOINT& b, unsigned long colorA, unsigned long colorB, int flags);
	static void DrawLine(WView* view, const _WPOINT& a, const _WPOINT& b, int flags, unsigned long color);
	static WRect ConvertRect(WView* view, const WRect& rect, int mode);
	static int CrossRect(const WRect& a, const WRect& b, WRect* out);
	static void DrawLineBox(WView* view, const WRect& rect, int flags, unsigned long color);
	static void DrawBox(WView* view, const WRect& rect, int flags, unsigned long color, float z);
	static void DrawRainbowBox(WView* view, const WRect& rect, unsigned long* const colors, int flags, float z);
	static void DrawPicture(WView* view, const WRect& rect, int texhandle, int flags, unsigned long color);
	static void DrawFrameOverlay1(WView* view, WOverlay* overlay, float x, float y, float w, float h, const tagRECT& border, unsigned long color);
	static void DrawFrameOverlay9(WView* view, WOverlay** const overlays, const tagRECT& rect, unsigned long color);

protected:
	void DrawTexture(WView* view, int texhandle, const _WRECT& dst, const _WRECT& src, float z, int flags, unsigned long color);
	void DrawTexture(WView* view, int texhandle, const _WRECT& dst, const _WRECT& src, int flags, unsigned long color, float z, unsigned char alpha, bool clip);
	void DrawTextureWithAxis(WView* view, int texhandle, const _WRECT& dst, const _WRECT& src, float ax, float ay, float angle, int flags, unsigned long color, unsigned char alpha);
	void DrawArcClipTexture(WView* view, int texhandle, const _WRECT& dst, const _WRECT& src, float start, float end, int flags, unsigned long color);
	float GetUnit(WView* view, float value, wUnitMode mode);
	int GetTexhandle(void) { return m_texHandle; }   // woverlay.h:109
	int GetCoordMode(void) { return m_coordMode; }   // woverlay.h:110
	void clip_2D_left(WView* view, WTVertex* in, WTVertex* out, int* num, int flags);
	void clip_2D_right(WView* view, WTVertex* in, WTVertex* out, int* num, int flags);
	void clip_2D_top(WView* view, WTVertex* in, WTVertex* out, int* num, int flags);
	void clip_2D_bottom(WView* view, WTVertex* in, WTVertex* out, int* num, int flags);
	int GetSection(WTVertex* vtx, float x, float y);
	void ConvertSourceRectByTextureSize(_WRECT& out, const _WRECT& src) const;

	int m_texHandle;                // 0x2c
	unsigned int m_texWidth;        // 0x30
	unsigned int m_texHeight;       // 0x34
	unsigned int m_devTexWidth;     // 0x38
	unsigned int m_devTexHeight;    // 0x3c

private:
	static WTVertex m_vtx[4];
	static WTVertex* m_vl[5];
	static WTVertex* m_vtxList[9];

	WRect m_clipArea;               // 0x40
	bool m_clipFlag;                // 0x50
	int m_coordMode;                // 0x54
};
CHECK_SIZE(WOverlay, 0x58);
BEGIN_LAYOUT_CHECK(WOverlay)
	CHECK_FIELD(WOverlay, m_texHandle, 0x2c);
	CHECK_FIELD(WOverlay, m_texWidth, 0x30);
	CHECK_FIELD(WOverlay, m_texHeight, 0x34);
	CHECK_FIELD(WOverlay, m_devTexWidth, 0x38);
	CHECK_FIELD(WOverlay, m_devTexHeight, 0x3c);
	CHECK_FIELD(WOverlay, m_clipArea, 0x40);
	CHECK_FIELD(WOverlay, m_clipFlag, 0x50);
	CHECK_FIELD(WOverlay, m_coordMode, 0x54);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WOVERLAY_H
