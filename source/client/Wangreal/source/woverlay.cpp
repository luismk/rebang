// Original: d:\build\custom\temp\code\client\wangreal\source\woverlay.cpp
#include <stdio.h>
#include <math.h>
#include <algorithm>

#include "../include/woverlay.h"
#include "../include/wview.h"
#include "../include/wresrcmng.h"
#include "../include/wutil.h"

// Static vertex tables (m_vl at 0xAC1AB8 is initialised, the others live in .bss).
WTVertex WOverlay::m_vtx[4];
WTVertex* WOverlay::m_vl[5] = { &m_vtx[0], &m_vtx[1], &m_vtx[3], &m_vtx[2], 0 };
WTVertex* WOverlay::m_vtxList[9];

// woverlay.cpp:30
WOverlay::WOverlay(void) {
	m_texHandle = 0;
	m_texWidth = 0;
	m_texHeight = 0;
	m_devTexWidth = 0;
	m_devTexHeight = 0;
	m_clipFlag = false;
	m_coordMode = 0x2200;
}

// woverlay.cpp:42
WOverlay::~WOverlay(void) {
	if (m_texHandle) {
		GetResrcManager()->Release(m_texHandle);
	}
}

// woverlay.cpp:48
int WOverlay::Load(const char* filename, unsigned long flag) {
	char buf[260];

	if (m_texHandle) {
		GetResrcManager()->Release(m_texHandle);
	}

	m_texHandle = GetResrcManager()->LoadTexture(filename, flag | 0x80000, 0, 0);

	if (m_texHandle) {
		m_texWidth = GetResrcManager()->GetTextureWidth(m_texHandle);
		m_texHeight = GetResrcManager()->GetTextureHeight(m_texHandle);
		m_devTexWidth = GetResrcManager()->VideoReference()->GetTextureWidth(m_texHandle);
		m_devTexHeight = GetResrcManager()->VideoReference()->GetTextureHeight(m_texHandle);

		sprintf(buf, "WOverlay:%s", filename);
		SetLeakHint(buf);
	}

	return !m_texHandle;
}

// woverlay.cpp:71
int WOverlay::Load(const char* filename, Bitmap* bitmap, unsigned long flag) {
	char buf[260];

	if (m_texHandle) {
		GetResrcManager()->Release(m_texHandle);
	}

	m_texHandle = GetResrcManager()->UploadTexture(filename, bitmap, flag | 0x80000, 0);

	if (m_texHandle) {
		m_texWidth = GetResrcManager()->GetTextureWidth(m_texHandle);
		m_texHeight = GetResrcManager()->GetTextureHeight(m_texHandle);
		m_devTexWidth = GetResrcManager()->VideoReference()->GetTextureWidth(m_texHandle);
		m_devTexHeight = GetResrcManager()->VideoReference()->GetTextureHeight(m_texHandle);

		sprintf(buf, "WOverlay:%s", filename);
		SetLeakHint(buf);
	}

	return !m_texHandle;
}

// woverlay.cpp:94
int WOverlay::CrossRect(const WRect& r1, const WRect& r2, WRect* out) {
	float x = Max(r1.x, r2.x);
	out->x = x;
	float y = Max(r1.y, r2.y);
	out->y = y;

	float right = Min(r1.w + r1.x, r2.w + r2.x);
	out->w = right - x;

	float bottom = Min(r1.h + r1.y, r2.h + r2.y);
	out->h = bottom - y;

	if (out->w < 0.0f || out->h < 0.0f)
		return 1;
	return 0;
}

// woverlay.cpp:104
void WOverlay::SetClippingArea(const WRect* rect) {
	if (!rect) {
		m_clipFlag = false;
	} else {
		m_clipFlag = true;
		m_clipArea = *rect;
	}
}

// woverlay.cpp:116
void WOverlay::SetClippingArea(WView* view, const WRect* rect) {
	WRect out;

	if (!rect && !view) {
		m_clipFlag = false;
		return;
	}

	m_clipFlag = true;
	if (rect) {
		m_clipArea = *rect;
	}
	if (view) {
		if (rect) {
			CrossRect(m_clipArea, WRect(0.0f, 0.0f, view->GetWidth(), view->GetHeight()), &out);
			m_clipArea = out;
		} else {
			m_clipArea = WRect(0.0f, 0.0f, view->GetWidth(), view->GetHeight());
		}
	}
}

// woverlay.cpp:147
void WOverlay::Render(WView* view, const _WRECT& src, const _WRECT& dest, int type, unsigned long diffuse, float angle, unsigned char flipflag) {
	_WRECT _src;
	ConvertSourceRectByTextureSize(_src, src);

	DrawTexture(view, m_texHandle, _src, ConvertRect(view, WRect(dest), m_coordMode), type, diffuse, angle, flipflag, true);
}

// woverlay.cpp:156
void WOverlay::RenderWithAxis(WView* view, const _WRECT& src, const _WRECT& dest, float axis_x, float axis_y, float angle, int type, unsigned long diffuse, unsigned char flipflag) {
	_WRECT _src;
	ConvertSourceRectByTextureSize(_src, src);

	DrawTextureWithAxis(view, m_texHandle, _src, ConvertRect(view, WRect(dest), m_coordMode), axis_x, axis_y, angle, type, diffuse, flipflag);
}

// woverlay.cpp:165
void WOverlay::ArcClipRender(WView* view, const _WRECT& src, const _WRECT& dest, float startang, float endang, int type, unsigned long diffuse) {
	_WRECT _src;
	ConvertSourceRectByTextureSize(_src, src);

	DrawArcClipTexture(view, m_texHandle, _src, ConvertRect(view, WRect(dest), m_coordMode), startang, endang, type, diffuse);
}

// woverlay.cpp:174
void WOverlay::RenderWithShear(WView* view, const _WRECT& src, const _WRECT& dest, float shear, int type, unsigned long diffuse) {
	_WRECT _src;
	ConvertSourceRectByTextureSize(_src, src);

	DrawTexture(view, m_texHandle, _src, ConvertRect(view, WRect(dest), m_coordMode), shear, type, diffuse);
}

// woverlay.cpp:183
void WOverlay::DrawTexture(WView* view, int texHandle, const _WRECT& src, const _WRECT& dest, float shear, int type, unsigned long diffuse) {
	for (int i = 0; i < 4; i++) {
		float x = (i & 1) ? dest.w + dest.x : dest.x;
		float offset = (i & 2) ? shear : 0.0f;
		m_vtx[i].sx = x - 0.5f + offset;
		float y = (i & 2) ? dest.h + dest.y : dest.y;
		m_vtx[i].sy = y - 0.5f;
		m_vtx[i].sz = 0.001f;
		m_vtx[i].rhw = 1.0f;
		m_vtx[i].diffuse = diffuse;
		m_vtx[i].tu = (i & 1) ? src.w + src.x : src.x;
		m_vtx[i].tv = (i & 2) ? src.h + src.y : src.y;
	}

	view->DrawPolygonFan(m_vl, (texHandle & 0x7ff) | type | 0x20300000, 0, true);
}

// woverlay.cpp:207
// Draws src of the texture into dest, snapped to whole pixels in pixel mode or
// scaled by the view's projection, clipped to the screen and the clip area
// when clipping is set, mirrored by flipflag and rotated by angle.
void WOverlay::DrawTexture(WView* view, int texHandle, const _WRECT& src, const _WRECT& dest, int type, unsigned long diffuse, float angle, unsigned char flipflag, bool clipping) {
	WRect _src(src);
	WRect _dest;
	WRect _clipArea;

	if (m_coordMode & 0x400) {
		_dest.x = floor(dest.x);
		_dest.y = floor(dest.y);
		_dest.w = dest.w;
		_dest.h = dest.h;
		if (m_clipFlag) {
			_clipArea.x = floor(m_clipArea.x);
			_clipArea.y = floor(m_clipArea.y);
			_clipArea.w = m_clipArea.w;
			_clipArea.h = m_clipArea.h;
		}
	} else {
		_dest = WRect(dest);
		bool clipFlag = m_clipFlag;
		if (clipFlag)
			_clipArea = m_clipArea;
		if (view->xGetProjScale() > 1.0f) {
			view->xConvScreenRectByProjScale(_dest);
			if (clipFlag)
				view->xConvScreenRectByProjScale(_clipArea);
		}
	}
	if (clipping == true) {
		WRect out;
		if (CrossRect(_dest, WRect(0.0f, 0.0f, view->GetWidth(), view->GetHeight()), &out) != 0)
			return;
		if (m_clipFlag && CrossRect(_dest, _clipArea, &out) != 0)
			return;
		if (_dest.w != out.w || _dest.h != out.h) {
			WRect src_cliped;
			float ratio;
			if (_dest.w != out.w) {
				float delta;
				ratio = src.w / _dest.w;
				delta = out.x - _dest.x;
				src_cliped.x = delta * ratio + src.x;
				src_cliped.w = (((out.x + out.w) - (_dest.w + _dest.x)) - delta) * ratio + src.w;
			} else {
				src_cliped.x = src.x;
				src_cliped.w = src.w;
			}
			if (_dest.h != out.h) {
				float delta;
				ratio = src.h / _dest.h;
				delta = out.y - _dest.y;
				src_cliped.y = delta * ratio + src.y;
				src_cliped.h = (((out.y + out.h) - (_dest.h + _dest.y)) - delta) * ratio + src.h;
			} else {
				src_cliped.y = src.y;
				src_cliped.h = src.h;
			}
			_src.x = src_cliped.x;
			_src.y = src_cliped.y;
			_src.w = src_cliped.w;
			_src.h = src_cliped.h;
			_dest = out;
		}
	}
	float sx, sy, ex, ey;
	if (m_coordMode & 0x400) {
		sx = _dest.x - 0.5f;
		ex = sx + _dest.w;
		sy = _dest.y - 0.5f;
		ey = sy + _dest.h;
	}
	for (int i = 0; i < 4; ++i) {
		if (m_coordMode & 0x400) {
			float px;
			if (i & 1)
				px = ex;
			else
				px = sx;
			m_vtx[i].sx = px;
			float py;
			if (i & 2)
				py = ey;
			else
				py = sy;
			m_vtx[i].sy = py;
		} else {
			float px;
			if (i & 1)
				px = _dest.w + _dest.x;
			else
				px = _dest.x;
			m_vtx[i].sx = px - 0.5f;
			float py;
			if (i & 2)
				py = _dest.h + _dest.y;
			else
				py = _dest.y;
			m_vtx[i].sy = py - 0.5f;
		}
		m_vtx[i].sz = 0.001f;
		m_vtx[i].rhw = 1.0f;
		m_vtx[i].diffuse = diffuse;
		float u;
		if ((unsigned int)(i & 1) == (~(unsigned int)flipflag & 1))
			u = _src.w + _src.x;
		else
			u = _src.x;
		m_vtx[i].tu = u;
		float v;
		if ((((unsigned int)i >> 1) & 1) == (~((unsigned int)flipflag >> 1) & 1))
			v = _src.h + _src.y;
		else
			v = _src.y;
		m_vtx[i].tv = v;
	}
	float scale = WAbsBySign(angle);
	if (!(scale < WMATH_EPSILON)) {
		float cos = ::cos(angle);
		float sin = ::sin(angle);
		float px = _dest.w * 0.5f + _dest.x;
		float py = _dest.h * 0.5f + _dest.y;
		for (int j = 0; j < 4; ++j) {
			float tx = m_vtx[j].sx - px;
			float ty = m_vtx[j].sy - py;
			m_vtx[j].sx = (tx * cos - ty * sin) + px;
			m_vtx[j].sy = ty * cos + tx * sin + py;
		}
	}
	view->DrawPolygonFan(m_vl, (texHandle & 0x7ff) | type | 0x20300000, 0, true);
}

// woverlay.cpp:345
// Draws src of the texture into dest rotated by angle around (axis_x, axis_y),
// clipped against the screen edges; a negligible angle draws it unrotated.
void WOverlay::DrawTextureWithAxis(WView* view, int texHandle, const _WRECT& src, const _WRECT& dest, float axis_x, float axis_y, float angle, int type, unsigned long diffuse, unsigned char flipflag) {
	if (WAbsBySign(angle) < WMATH_EPSILON) {
		DrawTexture(view, texHandle, src, dest, type, diffuse, 0.0f, flipflag, true);
		return;
	}

	WTVertex out_vtx[8];
	WTVertex in_vtx[8];
	int out_len;

	for (int i = 0; i < 4; ++i) {
		if (m_coordMode & 0x400) {
			m_vtx[i].sx = floorf((i & 1) ? dest.w + dest.x : dest.x) - 0.5f;
			m_vtx[i].sy = floorf((i & 2) ? dest.h + dest.y : dest.y) - 0.5f;
		} else {
			m_vtx[i].sx = ((i & 1) ? dest.w + dest.x : dest.x) - 0.5f;
			m_vtx[i].sy = ((i & 2) ? dest.h + dest.y : dest.y) - 0.5f;
		}
		m_vtx[i].sz = 0.001f;
		m_vtx[i].rhw = 1.0f;
		m_vtx[i].diffuse = diffuse;
		float u;
		if ((unsigned int)(i & 1) == (~(unsigned int)flipflag & 1))
			u = src.w + src.x;
		else
			u = src.x;
		m_vtx[i].tu = u;
		float v;
		if ((((unsigned int)i >> 1) & 1) == (~((unsigned int)flipflag >> 1) & 1))
			v = src.h + src.y;
		else
			v = src.y;
		m_vtx[i].tv = v;
	}

	float cos = ::cos(angle);
	float sin = ::sin(angle);
	int code = 0;
	for (int k = 0; k < 4; ++k) {
		float tx = m_vtx[k].sx - axis_x;
		float ty = m_vtx[k].sy - axis_y;
		m_vtx[k].sx = (tx * cos - ty * sin) + axis_x;
		m_vtx[k].sy = ty * cos + tx * sin + axis_y;
		if (m_vtx[k].sx < 0.0f)
			code |= 1;
		if (m_vtx[k].sx > view->GetWidth())
			code |= 2;
		if (m_vtx[k].sy < 0.0f)
			code |= 4;
		if (m_vtx[k].sy > view->GetHeight())
			code |= 8;
	}

	in_vtx[0] = m_vtx[0];
	in_vtx[1] = m_vtx[1];
	in_vtx[2] = m_vtx[3];
	out_len = 4;
	in_vtx[3] = m_vtx[2];

	if (code & 1) {
		clip_2D_left(view, out_vtx, in_vtx, &out_len, out_len);
		memcpy(in_vtx, out_vtx, sizeof(WTVertex) * out_len);
	}
	if (code & 2) {
		clip_2D_right(view, out_vtx, in_vtx, &out_len, out_len);
		memcpy(in_vtx, out_vtx, sizeof(WTVertex) * out_len);
	}
	if (code & 4) {
		clip_2D_top(view, out_vtx, in_vtx, &out_len, out_len);
		memcpy(in_vtx, out_vtx, sizeof(WTVertex) * out_len);
	}
	if (code & 8) {
		clip_2D_bottom(view, out_vtx, in_vtx, &out_len, out_len);
		memcpy(in_vtx, out_vtx, sizeof(WTVertex) * out_len);
	}

	if (code != 0) {
		if (out_len != 0) {
			int m;
			for (m = 0; m < out_len; ++m) {
				m_vtxList[m] = &in_vtx[m];
				m_vtxList[m]->rhw = 1.0f;
				m_vtxList[m]->diffuse = diffuse;
				m_vtxList[m]->sz = 0.001f;
			}
			m_vtxList[m] = 0;
			view->DrawPolygonFan(m_vtxList, (texHandle & 0x7ff) | type | 0x20300000, 0, true);
		}
	} else {
		view->DrawPolygonFan(m_vl, (texHandle & 0x7ff) | type | 0x20300000, 0x400, true);
	}
}

// woverlay.cpp:518
// Draws the part of the texture swept from startang to endang around the
// center of dest, as a fan from the center through the box corners in between.
void WOverlay::DrawArcClipTexture(WView* view, int texHandle, const _WRECT& src, const _WRECT& dest, float startang, float endang, int type, unsigned long diffuse) {
	WTVertex e_vtx;
	WTVertex s_vtx;
	WTVertex c_vtx;

	for (int i = 0; i < 4; ++i) {
		m_vtx[i].sz = 0.001f;
		m_vtx[i].rhw = 1.0f;
		m_vtx[i].diffuse = diffuse;
		m_vtx[i].tu = (i & 1) ? src.x : src.x + src.w;
		m_vtx[i].tv = (i & 2) ? src.y + src.h : src.y;
		float x = (i & 1) ? dest.x : dest.w + dest.x;
		m_vtx[i].sx = x - 0.5f;
		float y = (i & 2) ? dest.h + dest.y : dest.y;
		m_vtx[i].sy = y - 0.5f;
	}
	m_clipArea.x = dest.w * 0.5f + dest.x;
	m_clipArea.y = dest.h * 0.5f + dest.y;
	m_clipArea.w = dest.w * 0.5f;
	m_clipArea.h = dest.h * 0.5f;
	float ratio = dest.h / dest.w;
	float snap_ang = (atanf(ratio) * 3.1415927f) / 3.1415927f;
	s_vtx.sz = 0.001f;
	s_vtx.rhw = 1.0f;
	s_vtx.diffuse = diffuse;
	e_vtx.sz = 0.001f;
	e_vtx.rhw = 1.0f;
	e_vtx.diffuse = diffuse;
	c_vtx.sz = 0.001f;
	c_vtx.rhw = 1.0f;
	c_vtx.diffuse = diffuse;
	c_vtx.tu = 0.5f;
	c_vtx.tv = 0.5f;
	c_vtx.sx = dest.w * 0.5f + dest.x;
	c_vtx.sy = dest.h * 0.5f + dest.y;
	int s = GetSection(&s_vtx, snap_ang, startang);
	int e = GetSection(&e_vtx, snap_ang, endang);
	m_vtxList[0] = &c_vtx;
	m_vtxList[1] = &s_vtx;
	int n = 0;
	if (startang < endang) {
		for (; n < (e - s) / 2; ++n)
			m_vtxList[n + 2] = m_vl[s / 2 + n];
	} else {
		for (; n < (e - s + 8) / 2; ++n)
			m_vtxList[n + 2] = m_vl[(s / 2 + n) % 4];
	}

	m_vtxList[n + 2] = &e_vtx;
	m_vtxList[n + 3] = 0;
	view->DrawPolygonFan(m_vtxList, (texHandle & 0x7ff) | type | 0x20300000, 0x800, true);
}

// woverlay.cpp:587
// The point where the ray from the clip area's center at angle ang leaves the
// area, with its texture coordinate; snapang is the angle of the upper right
// corner. Returns the section (0..8) it falls in, -1 past the last one.
int WOverlay::GetSection(WTVertex* out, float snapang, float ang) {
	if (ang < snapang) {
		out->sx = m_clipArea.x + m_clipArea.w;
		out->sy = m_clipArea.y - tanf(ang) * m_clipArea.w;
		out->tu = 1.0f;
		out->tv = 0.5f - tanf(ang) * m_clipArea.w * 0.5f / m_clipArea.h;
		return 0;
	}

	if (ang == snapang) {
		out->sx = m_clipArea.x + m_clipArea.w;
		out->sy = m_clipArea.y - m_clipArea.h;
		out->tu = 1.0f;
		out->tv = 0.0f;
		return 1;
	}

	if (ang > snapang && 3.1415927f - snapang > ang) {
		ang -= 1.5707964f;
		out->sx = m_clipArea.x - tanf(ang) * m_clipArea.h;
		out->sy = m_clipArea.y - m_clipArea.h;
		out->tu = 0.5f - tanf(ang) * m_clipArea.h * 0.5f / m_clipArea.w;
		out->tv = 0.0f;
		return 2;
	}

	float limit = 3.1415927f - snapang;
	if (ang == limit) {
		out->sx = m_clipArea.x - m_clipArea.w;
		out->sy = m_clipArea.y - m_clipArea.h;
		out->tu = 0.0f;
		out->tv = 0.0f;
		return 3;
	}

	if (ang > limit && snapang + 3.1415927f > ang) {
		out->sx = m_clipArea.x - m_clipArea.w;
		out->sy = m_clipArea.y + tanf(ang) * m_clipArea.w;
		out->tu = 0.0f;
		out->tv = 0.5f + tanf(ang) * m_clipArea.w * 0.5f / m_clipArea.h;
		return 4;
	}

	limit = snapang + 3.1415927f;
	if (ang == limit) {
		out->sx = m_clipArea.x - m_clipArea.w;
		out->sy = m_clipArea.y + m_clipArea.h;
		out->tu = 0.0f;
		out->tv = 1.0f;
		return 5;
	}

	if (ang > limit && 6.2831855f - snapang > ang) {
		ang -= 4.712389f;
		out->sx = m_clipArea.x + tanf(ang) * m_clipArea.h;
		out->sy = m_clipArea.y + m_clipArea.h;
		out->tu = 0.5f + tanf(ang) * m_clipArea.h * 0.5f / m_clipArea.w;
		out->tv = 1.0f;
		return 6;
	}

	snapang = 6.2831855f - snapang;
	if (ang == snapang) {
		out->sx = m_clipArea.x + m_clipArea.w;
		out->sy = m_clipArea.y + m_clipArea.h;
		out->tu = 1.0f;
		out->tv = 1.0f;
		return 7;
	}

	if (ang > snapang) {
		out->sx = m_clipArea.x + m_clipArea.w;
		out->sy = m_clipArea.y - tanf(ang) * m_clipArea.w;
		out->tu = 1.0f;
		out->tv = 0.5f - tanf(ang) * m_clipArea.w * 0.5f / m_clipArea.h;
		return 8;
	}

	return -1;
}

// woverlay.cpp:674
void WOverlay::ConvertSourceRectByTextureSize(_WRECT& newRc, const _WRECT& rc) const {
	if (m_texWidth != m_devTexWidth && m_devTexWidth > 0) {
		newRc.x = (float)m_texWidth * rc.x / (float)m_devTexWidth;
		newRc.w = (float)m_texWidth * rc.w / (float)m_devTexWidth;
	} else {
		newRc.x = rc.x;
		newRc.w = rc.w;
	}

	if (m_texHeight != m_devTexHeight && m_devTexHeight > 0) {
		newRc.y = (float)m_texHeight * rc.y / (float)m_devTexHeight;
		newRc.h = (float)m_texHeight * rc.h / (float)m_devTexHeight;
	} else {
		newRc.y = rc.y;
		newRc.h = rc.h;
	}
}

// woverlay.cpp:699
void WOverlay::clip_2D_left(WView* view, WTVertex* out, WTVertex* in, int* outlen, int inlen) {
	*outlen = 0;
	if (inlen <= 0)
		return;

	WTVertex* cur = in;
	int next = 1;
	for (int i = inlen; i > 0; --i, ++cur, ++next) {
		if (cur->sx > 0.0f) {
			out[*outlen].sx = cur->sx;
			out[*outlen].sy = cur->sy;

			out[*outlen].tu = cur->tu;
			out[*outlen].tv = cur->tv;
			*outlen = *outlen + 1;
		}

		WTVertex* nv = in + next % inlen;
		if (cur->sx * nv->sx < 0.0f) {
			float d0 = Abs(cur->sx);
			float d1 = Abs(nv->sx);
			float d2 = Abs(cur->sx);
			float t = d0 / (d2 + d1);
			out[*outlen].sx = 0.0f;
			out[*outlen].sy = (nv->sy - cur->sy) * t + cur->sy;

			out[*outlen].tu = (nv->tu - cur->tu) * t + cur->tu;
			out[*outlen].tv = (nv->tv - cur->tv) * t + cur->tv;
			*outlen = *outlen + 1;
		}
	}
}

// woverlay.cpp:731
void WOverlay::clip_2D_right(WView* view, WTVertex* out, WTVertex* in, int* outlen, int inlen) {
	*outlen = 0;
	if (inlen <= 0)
		return;

	WTVertex* cur = in;
	int next = 1;
	for (int i = inlen; i > 0; --i, ++cur, ++next) {
		if (cur->sx < view->GetWidth()) {
			out[*outlen].sx = cur->sx;
			out[*outlen].sy = cur->sy;

			out[*outlen].tu = cur->tu;
			out[*outlen].tv = cur->tv;
			*outlen = *outlen + 1;
		}

		if ((float)((cur->sx - view->GetWidth())) * (in[next % inlen].sx - view->GetWidth()) < 0.0f) {
			WTVertex* nv = &in[next % inlen];
			float d0 = Abs(cur->sx - view->GetWidth());
			float d1 = Abs(nv->sx - view->GetWidth());
			float d2 = Abs(cur->sx - view->GetWidth());
			float t = d0 / (d2 + d1);
			out[*outlen].sx = view->GetWidth();
			out[*outlen].sy = (nv->sy - cur->sy) * t + cur->sy;

			out[*outlen].tu = (nv->tu - cur->tu) * t + cur->tu;
			out[*outlen].tv = (nv->tv - cur->tv) * t + cur->tv;
			*outlen = *outlen + 1;
		}
	}
}

// woverlay.cpp:763
void WOverlay::clip_2D_top(WView* view, WTVertex* out, WTVertex* in, int* outlen, int inlen) {
	*outlen = 0;
	if (inlen <= 0)
		return;

	WTVertex* cur = in;
	int next = 1;
	for (int i = inlen; i > 0; --i, ++cur, ++next) {
		if (cur->sy > 0.0f) {
			out[*outlen].sx = cur->sx;
			out[*outlen].sy = cur->sy;

			out[*outlen].tu = cur->tu;
			out[*outlen].tv = cur->tv;
			*outlen = *outlen + 1;
		}

		if (cur->sy * in[next % inlen].sy < 0.0f) {
			WTVertex* nv = in + next % inlen;
			float d0 = Abs(cur->sy);
			float d1 = Abs(nv->sy);
			float d2 = Abs(cur->sy);
			float t = d0 / (d2 + d1);
			out[*outlen].sx = (nv->sx - cur->sx) * t + cur->sx;
			out[*outlen].sy = 0.0f;

			out[*outlen].tu = (nv->tu - cur->tu) * t + cur->tu;
			out[*outlen].tv = (nv->tv - cur->tv) * t + cur->tv;
			*outlen = *outlen + 1;
		}
	}
}

// woverlay.cpp:795
void WOverlay::clip_2D_bottom(WView* view, WTVertex* out, WTVertex* in, int* outlen, int inlen) {
	*outlen = 0;
	if (inlen <= 0)
		return;

	WTVertex* cur = in;
	int next = 1;
	for (int i = inlen; i > 0; --i, ++cur, ++next) {
		if (cur->sy < view->GetHeight()) {
			out[*outlen].sx = cur->sx;
			out[*outlen].sy = cur->sy;

			out[*outlen].tu = cur->tu;
			out[*outlen].tv = cur->tv;
			*outlen = *outlen + 1;
		}

		if ((cur->sy - view->GetHeight()) * (in[next % inlen].sy - view->GetHeight()) < 0.0f) {
			WTVertex* nv = &in[next % inlen];
			float d0 = Abs(cur->sy - view->GetHeight());
			float d1 = Abs(nv->sy - view->GetHeight());
			float d2 = Abs(cur->sy - view->GetHeight());
			float t = d0 / (d2 + d1);
			out[*outlen].sx = (nv->sx - cur->sx) * t + cur->sx;
			out[*outlen].sy = view->GetHeight();

			out[*outlen].tu = (nv->tu - cur->tu) * t + cur->tu;
			out[*outlen].tv = (nv->tv - cur->tv) * t + cur->tv;
			*outlen = *outlen + 1;
		}
	}
}

// woverlay.cpp:827
void WOverlay::DrawLine(WView* view, const _WPOINT& p1, const _WPOINT& p2, int type, unsigned long diffuse) {
	WTVertex vtx[2];
	WTVertex* vl[3];

	vl[0] = &vtx[0];

	vtx[0].sx = p1.x;
	vtx[0].sy = p1.y;
	vtx[0].sz = 0.001f;
	vtx[0].rhw = 1.0f;
	vtx[0].diffuse = diffuse;

	vl[1] = &vtx[1];
	vtx[1].sx = p2.x;
	vtx[1].sy = p2.y;
	vtx[1].sz = 0.001f;
	vtx[1].rhw = 1.0f;
	vtx[1].diffuse = diffuse;

	vl[2] = 0;

	view->DrawPolygonFan(vl, type | 0x4000000, 0, true);
}

// woverlay.cpp:856
void WOverlay::DrawLine(WView* view, const _WPOINT& p1, const _WPOINT& p2, unsigned long diffuse1, unsigned long diffuse2, int type) {
	WTVertex vtx[2];
	WTVertex* vl[3];

	vl[0] = &vtx[0];

	vtx[0].sx = p1.x;
	vtx[0].sy = p1.y;
	vtx[0].sz = 0.001f;
	vtx[0].rhw = 1.0f;
	vtx[0].diffuse = diffuse1;

	vl[1] = &vtx[1];
	vtx[1].sx = p2.x;
	vtx[1].sy = p2.y;
	vtx[1].sz = 0.001f;
	vtx[1].rhw = 1.0f;
	vtx[1].diffuse = diffuse2;

	vl[2] = 0;

	view->DrawPolygonFan(vl, type | 0x4000000, 0, true);
}

// woverlay.cpp:893
void WOverlay::DrawBox(WView* view, const WRect& rect, int type, unsigned long diffuse, float depth) {
	float px = rect.x;
	m_vtx[0].sx = px - 0.5f;
	float py = rect.y;
	m_vtx[0].sz = depth;
	m_vtx[0].diffuse = diffuse;
	m_vtx[0].rhw = 1.0f;
	m_vtx[0].sy = py - 0.5f;

	px = rect.w + rect.x;
	m_vtx[1].sx = px - 0.5f;
	py = rect.y;
	m_vtx[1].sz = depth;
	m_vtx[1].diffuse = diffuse;
	m_vtx[1].rhw = 1.0f;
	m_vtx[1].sy = py - 0.5f;

	px = rect.x;
	m_vtx[2].sx = px - 0.5f;
	py = rect.h + rect.y;
	m_vtx[2].sz = depth;
	m_vtx[2].diffuse = diffuse;
	m_vtx[2].rhw = 1.0f;
	m_vtx[2].sy = py - 0.5f;

	px = rect.w + rect.x;
	m_vtx[3].sx = px - 0.5f;
	py = rect.h + rect.y;
	m_vtx[3].sz = depth;
	m_vtx[3].diffuse = diffuse;
	m_vtx[3].rhw = 1.0f;
	m_vtx[3].sy = py - 0.5f;

	if (type & 0x7ff) {
		for (int i = 0; i < 4; ++i) {
			float u;
			if (i & 1)
				u = 0.0f;
			else
				u = 1.0f;
			m_vtx[i].tu = u;
			float v;
			if (i & 2)
				v = 0.0f;
			else
				v = 1.0f;
			m_vtx[i].tv = v;
		}
	}

	view->DrawPolygonFan(m_vl, type | 0x20300000, 0, true);
}

// woverlay.cpp:920
void WOverlay::DrawRainbowBox(WView* view, const WRect& rect, unsigned long* const aDiffuse, int type, float depth) {
	for (int i = 0; i < 4; ++i) {
		float px;
		if (i & 1)
			px = rect.w + rect.x;
		else
			px = rect.x;
		m_vtx[i].sx = px - 0.5f;

		float py;
		if (i & 2)
			py = rect.h + rect.y;
		else
			py = rect.y;
		m_vtx[i].sz = depth;
		m_vtx[i].rhw = 1.0f;
		m_vtx[i].sy = py - 0.5f;
		m_vtx[i].diffuse = aDiffuse[i];
	}

	if (type & 0x7ff) {
		for (int i = 0; i < 4; ++i) {
			float u;
			if (i & 1)
				u = 0.0f;
			else
				u = 1.0f;
			m_vtx[i].tu = u;
			float v;
			if (i & 2)
				v = 0.0f;
			else
				v = 1.0f;
			m_vtx[i].tv = v;
		}
	}

	view->DrawPolygonFan(m_vl, type | 0x20300000, 0, true);
}

// woverlay.cpp:947
void WOverlay::DrawLineBox(WView* view, const WRect& rect, int type, unsigned long diffuse) {
	WRect r(rect);
	WPoint lt(r.x, r.y);
	WPoint rt(r.x + r.w, r.y);
	WPoint lb(r.x, r.Bottom());
	WPoint rb(r.x + r.w, r.Bottom());
	DrawLine(view, lt, rt, type | 0x300000, diffuse);
	DrawLine(view, rt, rb, type | 0x300000, diffuse);
	DrawLine(view, rb, lb, type | 0x300000, diffuse);
	DrawLine(view, lb, lt, type | 0x300000, diffuse);
}

// woverlay.cpp:958
float WOverlay::GetUnit(WView* view, float unit, wUnitMode mode) {
	switch (mode) {
	case W_UNIT_XPOS:
		if (m_coordMode & 0x100) {
			switch (m_coordMode & 6) {
			case 0:
				return view->GetWidth() / 640.0f * unit;
			case 2:
				return (unit + 320.0f) * (view->GetWidth() / 640.0f);
			case 4:
				return (unit + 640.0f) * (view->GetWidth() / 640.0f);
			}
		}
		break;
	case W_UNIT_YPOS:
		if (m_coordMode & 0x100) {
			switch (m_coordMode & 0x60) {
			case 0:
				return view->GetHeight() / 480.0f * unit;
			case 0x20:
				return (unit + 240.0f) * (view->GetHeight() / 480.0f);
			case 0x40:
				return (unit + 480.0f) * (view->GetHeight() / 480.0f);
			}
		}
		break;
	case W_UNIT_WIDTH:
		if (m_coordMode & 0x1000) {
			return view->GetWidth() / 640.0f * unit;
		}
		break;
	case W_UNIT_HEIGHT:
		if (m_coordMode & 0x1000) {
			return view->GetHeight() / 480.0f * unit;
		}
		break;
	}

	return unit;
}

// woverlay.cpp:1009
WRect WOverlay::ConvertRect(WView* view, const WRect& rect, int coor_flag) {
	float pivot_x;
	if (!(coor_flag & 6)) {
		pivot_x = 0.0f;
	} else {
		pivot_x = ((coor_flag & 2) ? 0.5f : 1.0f) * view->GetWidth();
	}

	float pivot_y;
	if (!(coor_flag & 0x60)) {
		pivot_y = 0.0f;
	} else {
		pivot_y = ((coor_flag & 0x20) ? 0.5f : 1.0f) * view->GetHeight();
	}

	WRect ret;
	if (coor_flag & 0x100) {
		ret.x = view->GetWidth() * rect.x / 640.0f + pivot_x;
		ret.y = view->GetHeight() * rect.y / 480.0f + pivot_y;
	} else {
		ret.x = pivot_x + rect.x;
		ret.y = pivot_y + rect.y;
	}

	if (coor_flag & 0x1000) {
		ret.w = view->GetWidth() * rect.w / 640.0f;
		ret.h = view->GetHeight() * rect.h / 480.0f;
	} else {
		ret.w = rect.w;
		ret.h = rect.h;
	}

	return ret;
}

// woverlay.cpp:1040
void WOverlay::DrawPicture(WView* view, const WRect& rect, int texhandle, int type, unsigned long diffuse) {
	for (int i = 0; i < 4; ++i) {
		float px;
		if (i & 1)
			px = rect.w + rect.x;
		else
			px = rect.x;
		m_vtx[i].sx = px - 0.5f;

		float py;
		if (i & 2)
			py = rect.h + rect.y;
		else
			py = rect.y;
		m_vtx[i].sy = py - 0.5f;

		float u;
		if (i & 1)
			u = 1.0f;
		else
			u = 0.0f;
		m_vtx[i].tu = u;

		float v;
		if (i & 2)
			v = 1.0f;
		else
			v = 0.0f;
		m_vtx[i].tv = v;

		m_vtx[i].diffuse = diffuse;
		m_vtx[i].sz = 0.001f;
		m_vtx[i].rhw = 1.0f;
	}
	view->DrawPolygonFan(m_vl, (texhandle & 0x7ff) | type | 0x20000000, 0, true);
}

// woverlay.cpp:1060
// Draws a nine-piece frame over rect from the single texture of frame, cut
// by the border sizes: the corners at their size, the rest stretched.
void WOverlay::DrawFrameOverlay1(WView* view, WOverlay* frame, float sizeL, float sizeT, float sizeR, float sizeB, const tagRECT& rect, unsigned long color) {
	float sizeLR = sizeL + sizeR;
	float sizeTB = sizeT + sizeB;
	float xoffset = 0.5f / frame->GetWidth();
	float yoffset = 0.5f / frame->GetHeight();
	float inv_w = 1.0f / frame->GetWidth();
	float inv_h = 1.0f / frame->GetHeight();
	WRect offsetRect(xoffset, yoffset, -xoffset, -yoffset);
	float x = (float)rect.left;
	float y = (float)rect.top;
	float w = (float)rect.right;
	float h = (float)rect.bottom;
	WRect frameRect[9];
	frameRect[0] = WRect(0.0f, 0.0f, sizeL, sizeT);
	frameRect[1] = WRect(sizeL, 0.0f, (float)frame->GetWidth() - sizeLR, sizeT);
	frameRect[2] = WRect((float)frame->GetWidth() - sizeR, 0.0f, sizeR, sizeT);
	frameRect[3] = WRect(0.0f, sizeB, sizeL, (float)frame->GetHeight() - sizeTB);
	frameRect[4] = WRect(sizeL, sizeB, (float)frame->GetWidth() - sizeLR, (float)frame->GetHeight() - sizeTB);
	frameRect[5] = WRect((float)frame->GetWidth() - sizeR, sizeB, sizeR, (float)frame->GetHeight() - sizeTB);
	frameRect[6] = WRect(0.0f, (float)frame->GetHeight() - sizeB, sizeL, sizeB);
	frameRect[7] = WRect(sizeL, (float)frame->GetHeight() - sizeB, (float)frame->GetWidth() - sizeLR, sizeB);
	frameRect[8] = WRect((float)frame->GetWidth() - sizeR, (float)frame->GetHeight() - sizeB, sizeR, sizeB);

	WRect srcRect[9];
	for (int i = 0; i < 9; ++i) {
		srcRect[i].x = frameRect[i].x * inv_w + offsetRect.x;
		srcRect[i].y = frameRect[i].y * inv_h + offsetRect.y;
		srcRect[i].w = frameRect[i].w * inv_w + offsetRect.w;
		srcRect[i].h = frameRect[i].h * inv_h + offsetRect.h;
	}

	frame->Render(view, srcRect[0], WRect(x, y, sizeL, sizeT), 0x2000000, color, 0.0f, 0);
	frame->Render(view, srcRect[1], WRect(x + sizeL, y, w - sizeLR, sizeT), 0x2000000, color, 0.0f, 0);
	frame->Render(view, srcRect[2], WRect(x + w - sizeR, y, sizeR, sizeT), 0x2000000, color, 0.0f, 0);
	frame->Render(view, srcRect[3], WRect(x, y + sizeT, sizeL, h - sizeTB), 0x2000000, color, 0.0f, 0);
	frame->Render(view, srcRect[4], WRect(x + sizeL, y + sizeT, w - sizeLR, h - sizeTB), 0x2000000, color, 0.0f, 0);
	frame->Render(view, srcRect[5], WRect(x + w - sizeR, y + sizeT, sizeR, h - sizeTB), 0x2000000, color, 0.0f, 0);
	frame->Render(view, srcRect[6], WRect(x, y + h - sizeB, sizeL, sizeB), 0x2000000, color, 0.0f, 0);
	frame->Render(view, srcRect[7], WRect(x + sizeL, y + h - sizeB, w - sizeLR, sizeB), 0x2000000, color, 0.0f, 0);
	frame->Render(view, srcRect[8], WRect(x + w - sizeR, y + h - sizeB, sizeR, sizeB), 0x2000000, color, 0.0f, 0);
}

// woverlay.cpp:1111
// Draws a nine-piece frame over rect: the corners at their texture size,
// the edges and the center stretched in between.
void WOverlay::DrawFrameOverlay9(WView* view, WOverlay** const frames, const tagRECT& rect, unsigned long color) {
	float sizeL = (float)frames[3]->GetWidth();
	float sizeR = (float)frames[5]->GetWidth();
	float sizeT = (float)frames[1]->GetHeight();
	float sizeB = (float)frames[7]->GetHeight();
	float sizeLR = sizeL + sizeR;
	float sizeTB = sizeT + sizeB;
	float x = (float)rect.left;
	float y = (float)rect.top;
	float w = (float)rect.right;
	float h = (float)rect.bottom;
	WRect srcRect[9];

	for (int i = 0; i < 9; i++)
		srcRect[i] = WRect(0.0f, 0.0f, 1.0f, 1.0f);

	frames[0]->Render(view, srcRect[0], WRect(x, y, sizeL, sizeT), 0x2080000, color, 0.0f, 0);
	frames[1]->Render(view, srcRect[1], WRect(x + sizeL, y, w - sizeLR, sizeT), 0x2080000, color, 0.0f, 0);
	frames[2]->Render(view, srcRect[2], WRect(x + w - sizeR, y, sizeR, sizeT), 0x2080000, color, 0.0f, 0);

	frames[3]->Render(view, srcRect[3], WRect(x, y + sizeT, sizeL, h - sizeTB), 0x2080000, color, 0.0f, 0);
	frames[4]->Render(view, srcRect[4], WRect(x + sizeL, y + sizeT, w - sizeLR, h - sizeTB), 0x2080000, color, 0.0f, 0);
	frames[5]->Render(view, srcRect[5], WRect(x + w - sizeR, y + sizeT, sizeR, h - sizeTB), 0x2080000, color, 0.0f, 0);

	frames[6]->Render(view, srcRect[6], WRect(x, y + h - sizeB, sizeL, sizeB), 0x2080000, color, 0.0f, 0);
	frames[7]->Render(view, srcRect[7], WRect(x + sizeL, y + h - sizeB, w - sizeLR, sizeB), 0x2080000, color, 0.0f, 0);
	frames[8]->Render(view, srcRect[8], WRect(x + w - sizeR, y + h - sizeB, sizeR, sizeB), 0x2080000, color, 0.0f, 0);
}
