// Original: d:\build\custom\temp\code\client\wangreal\source\w3dspr.cpp
#include <math.h>
#include <stdio.h>

#include "../include/w3dspr.h"
#include "../include/wresrcmng.h"
#include "../include/wview.h"

// The four corners as (1 = w1/h1, 2 = w2/h2) pairs, in drawing order.
static int corner[4][2] = { { 1, 2 }, { 2, 2 }, { 2, 1 }, { 1, 1 } };

// Texture coordinates of the four corners, in texels of a 256 grid.
static int uv[4][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };

// w3dspr.cpp:20
W3dSpr::W3dSpr(void) : m_handle(0), m_texLoaded(false) {
	for (int i = 0; i < 4; i++) {
		vl[i] = &vtx[i];
		vl[i]->tu = (uv[i][0] * 255.0f + 0.5f) / 256.0f;
		vl[i]->tv = (uv[i][1] * 255.0f + 0.5f) / 256.0f;

		vl[i]->diffuse = 0xffffffff;
	}
	vl[4] = NULL;
}

// w3dspr.cpp:35
W3dSpr::~W3dSpr(void) {
	if (m_texLoaded) {
		if (GetResrcManager() && m_handle) { GetResrcManager()->Release(m_handle); m_handle = 0; }
	}
}

// w3dspr.cpp:47
void W3dSpr::SetPos(const WVector& vec) {
	pos = vec;
}

// w3dspr.cpp:52
void W3dSpr::Render(WView* view, int type, w_spr_align align) {
	if (!m_handle)
		return;

	WVector ya;
	WVector za;
	WVector xa;
	za = pos - view->camera.pivot;
	xa = WCrossProduct(WVector::UNIT_POS_Y, za);
	za.Normalize();
	xa.Normalize();
	switch (align) {
	case CAMERA_XY_ALIGN:
		ya = WCrossProduct(za, xa);
		ya.Normalize();
		break;
	case CAMERA_X_ALIGN:
		ya = WVector::UNIT_POS_Y;
		break;
	}

	vl[0]->SetPosition(pos + xa * w1 + ya * h2);
	vl[1]->SetPosition(pos + xa * w2 + ya * h2);
	vl[2]->SetPosition(pos + xa * w2 + ya * h1);
	vl[3]->SetPosition(pos + xa * w1 + ya * h1);
	view->DrawPolygonFan(vl, (m_handle & 0x7ff) | type, 0x400, false);
}

// w3dspr.cpp:96
void W3dSpr::Render(WView* view, float angle, int type) {
	if (!m_handle)
		return;

	WVector ya;
	WVector xa;
	{
		WVector za;
		za = pos - view->camera.pivot;
		xa = WCrossProduct(WVector::UNIT_POS_Y, za);
		ya = WCrossProduct(za, xa);
		za.Normalize();
		xa.Normalize();
		ya.Normalize();
	}

	for (int i = 0; i < 4; ++i) {
		float x;
		float y;
		Rotate(corner[i][0] == 1 ? w1 : w2, corner[i][1] == 1 ? h1 : h2, x, y, angle);
		vl[i]->SetPosition(pos + xa * x + ya * y);
	}
	view->DrawPolygonFan(vl, (m_handle & 0x7ff) | type, 0x400, false);
}

// w3dspr.cpp:132
void W3dSpr::Rotate(float x, float y, float& tx, float& ty, float angle) {
	float cos = cosf(angle), sin = sinf(angle);
	tx = cos * x - sin * y;
	ty = sin * x + cos * y;
}

// w3dspr.cpp:139
void W3dSpr::SetRect(float width, float height, float pivot_x, float pivot_y) {
	w1 = -pivot_x;
	w2 = width - pivot_x;
	h1 = -pivot_y;
	h2 = height - pivot_y;
}

// w3dspr.cpp:147
void W3dSpr::SetColor(const int color) {
	for (int i = 0; i < 4; i++) vl[i]->diffuse = color;
}

// w3dspr.cpp:153
int W3dSpr::LoadSprite(const char* filename, int type) {
	m_handle = GetResrcManager()->LoadTexture(filename, type, 0, 0);
	char buf[260];
	if (m_handle) {
		m_texLoaded = true;
		sprintf(buf, "W3dSpr:%s", filename);
		SetLeakHint(buf);
		return 0;
	}
	return 1;
}

// w3dspr.cpp:173
int W3dSpr::AttachTexture(int texHandle) {
	m_handle = texHandle;
	return 1;
}
