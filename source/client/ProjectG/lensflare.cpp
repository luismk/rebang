// Original: d:\build\custom\temp\code\client\projectg\lensflare.cpp
#include "LensFlare.h"
#include "SharedDoc.h"
#include "w3dspr.h"
#include "woverlay.h"
#include "wresrcmng.h"
#include "wview.h"

#include <stdlib.h>
#include <string.h>

#define PI 3.1415927f

CLensFlare::w_flare_set CLensFlare::ms_lenzTable[7] = {
	{ 0, 0.5f, 0.03f, 0xFF0B0B0F },
	{ 0, 0.4f, 0.05f, 0xFF0B0B0F },
	{ 0, 0.2f, 0.032f, 0xFF1D1008 },
	{ 0, -0.47f, 0.03f, 0xFF110901 },
	{ 0, -0.65f, 0.06f, 0xFF050F0C },
	{ 0, -1.0f, 0.2f, 0xFF110E01 },
	{ 1, -1.3f, 0.4f, 0xFF191919 },
};

static const char* s_overlayNames[2] = {
	"~lensflare01.bmp",
	"~lensflare02.bmp",
};

static const char* s_spriteNames[4] = {
	"~glaring_yellowish.bmp",
	"~sun.bmp",
	"~lensflare03.bmp",
	"~lensflare04.bmp",
};

// lensflare.cpp:40
CLensFlare::CLensFlare(void) {
	memset(m_overlays, 0, sizeof(m_overlays));
	memset(m_sprites, 0, sizeof(m_sprites));
}

// lensflare.cpp:47
CLensFlare::~CLensFlare(void) {
	int i;

	for (i = 0; i < 2; i++) {
		if (g_resrcmng && m_overlays[i]) { g_resrcmng->Release(m_overlays[i]); m_overlays[i] = 0; }
	}

	for (i = 0; i < 4; i++) {
		if (g_resrcmng && m_sprites[i]) { g_resrcmng->Release(m_sprites[i]); m_sprites[i] = 0; }
	}
}

// lensflare.cpp:62
void CLensFlare::Init(const WVector& sunDir) {
	float scale[4];

	scale[0] = 8.0f;
	scale[1] = 1.5f;
	scale[2] = 1.5f;
	scale[3] = 1.0f;

	unsigned int i;

	for (i = 0; i < sizeof(s_overlayNames) / sizeof(s_overlayNames[0]); i++) {
		m_overlays[i] = g_resrcmng->GetOverlay(s_overlayNames[i], 0);
		m_overlays[i]->SetCoordMode(0x2200);
	}

	for (i = 0; i < sizeof(s_spriteNames) / sizeof(s_spriteNames[0]); i++) {
		m_sprites[i] = g_resrcmng->Get3DSpr(s_spriteNames[i], 0);
		int tex = g_resrcmng->LoadTexture(s_spriteNames[i], 0, 0, 0);
		float width = g_resrcmng->GetTextureWidth(tex) * scale[i];
		float height = g_resrcmng->GetTextureHeight(tex) * scale[i];

		m_spriteSize[i].width = width;
		m_spriteSize[i].height = height;

		if (g_resrcmng && tex) g_resrcmng->Release(tex);
	}

	SetSunDirection(sunDir);
	m_angle = 0.0f;
}

// lensflare.cpp:101
void CLensFlare::SetSunDirection(const WVector& sunDir) {
	m_sunDir = sunDir;
	m_sunDir.Normalize();

	m_alpha = WAbsBySign(m_sunDir.y) * 0.8f + 0.2f;
	m_sprites[2]->SetColor(((int)(m_alpha * 255.0f) << 24) | 0xFFFFFF);
	m_sprites[3]->SetColor(((int)(m_alpha * 255.0f) << 24) | 0xFFFFFF);
}

// lensflare.cpp:111
void CLensFlare::Process(float elapsed) {
	m_angle += elapsed * PI;

	if (m_angle > 120.0f * PI)
		m_angle -= 120.0f * PI;
}

// lensflare.cpp:119
void CLensFlare::Render(WView* view) {
	const WMatrix& camera = view->GetCamera();
	if (camera.za.x * m_sunDir.x + (camera.za.y * m_sunDir.y + camera.za.z * m_sunDir.z) <= 0.0f)
		return;

	WVector pos;
	pos = m_sunDir * 1000.0f + view->GetCamera().pivot;

	WTVertex vtx;
	view->Projection2(&vtx, pos);

	if (vtx.sx < view->GetWidth() * -1.4f || vtx.sx > view->GetWidth() * 2.4f
		|| vtx.sy < view->GetHeight() * -1.4f || vtx.sy > view->GetHeight() * 2.4f)
		return;

	float clipNear = view->GetClipNearValue();
	float clipFar = view->GetClipFarValue();
	view->SetClip(clipNear, 2000.0f, false);
	view->UpdateCamera();

	if (GetWeather()) {
		if (vtx.sx > view->GetWidth() * -0.4f && vtx.sx < view->GetWidth() * 1.4f
			&& vtx.sy > view->GetHeight() * -0.4f && vtx.sy < view->GetHeight() * 1.4f) {
			m_sprites[1]->SetColor(0xFF505050);
			m_sprites[1]->SetPos(pos);
			m_sprites[1]->SetRect(m_spriteSize[1].width, m_spriteSize[1].height, m_spriteSize[1].width * 0.5f, m_spriteSize[1].height * 0.5f);
			m_sprites[1]->Render(view, 0x20800000, W3dSpr::CAMERA_XY_ALIGN);
		}

		view->SetClip(clipNear, clipFar, false);
		view->UpdateCamera();
		return;
	}

	m_sprites[0]->SetPos(pos);
	float height = m_spriteSize[0].height * 2.0f; float width = m_spriteSize[0].width * 2.0f; m_sprites[0]->SetRect(width, height, width * 0.5f, height * 0.5f);
	m_sprites[0]->Render(view, 0x20B00000, W3dSpr::CAMERA_XY_ALIGN);

	if (vtx.sx < view->GetWidth() * -0.4f || vtx.sx > view->GetWidth() * 1.4f
		|| vtx.sy < view->GetHeight() * -0.4f || vtx.sy > view->GetHeight() * 1.4f) {
		view->SetClip(clipNear, clipFar, false);
		view->UpdateCamera();
		return;
	}

	m_sprites[1]->SetPos(pos);
	m_sprites[1]->SetRect(m_spriteSize[1].width, m_spriteSize[1].height, m_spriteSize[1].width * 0.5f, m_spriteSize[1].height * 0.5f);
	m_sprites[1]->Render(view, 0x20800000, W3dSpr::CAMERA_XY_ALIGN);

	m_sprites[2]->SetPos(pos);
	height = m_spriteSize[1].height * 2.0f; width = m_spriteSize[1].width * 2.0f; m_sprites[2]->SetRect(width, height, width * 0.5f, height * 0.5f);
	m_sprites[2]->Render(view, 0x20800000, W3dSpr::CAMERA_XY_ALIGN);

	if (vtx.sx < m_spriteSize[1].width * -0.7f || vtx.sx > m_spriteSize[1].width * 0.7f + view->GetWidth()
		|| vtx.sy < m_spriteSize[1].height * -0.7f || vtx.sy > m_spriteSize[1].height * 0.7f + view->GetHeight()) {
		view->SetClip(clipNear, clipFar, false);
		view->UpdateCamera();
		return;
	}

	float angle[5];
	angle[0] = 1.41371667f + m_angle * 0.05f;
	angle[1] = 0.785398185f - m_angle * 0.05f;
	angle[2] = 3.92699099f + m_angle * 0.0333333351f;
	angle[3] = 2.98451304f - m_angle * 0.0333333351f;
	angle[4] = m_angle * 0.0166666675f;

	m_sprites[3]->SetPos(pos);

	for (int i = 0; i < 5; i++) {
		int r = rand() % 800;

		float size = ((r - 400) * 0.001f + 1.0f) * 2.0f;
		m_sprites[3]->SetRect(m_spriteSize[2].width * size, m_spriteSize[2].height * size, m_spriteSize[2].width * size * 0.5f, m_spriteSize[2].height * size * 0.5f);
		m_sprites[3]->Render(view, angle[i], 0x20800000);
	}

	WRect src(0.0f, 0.0f, 1.0f, 1.0f);
	WRect dst;

	float dx = vtx.sx / view->GetWidth() - 0.5f;
	float dy = vtx.sy / view->GetHeight() - 0.5f;
	float unit = view->GetWidth() * 0.0015625f;

	for (int j = 0; j < 7; j++) {
		WOverlay* overlay = m_overlays[ms_lenzTable[j].overlay];
		int r = rand() % 500;

		dst.w = (r * 0.0001f + 1.0f) * ms_lenzTable[j].scale * view->GetWidth() * unit;
		dst.h = dst.w;
		dst.x = (dx * ms_lenzTable[j].position + 0.5f) * view->GetWidth() - dst.w * 0.5f;
		dst.y = (dy * ms_lenzTable[j].position + 0.5f) * view->GetHeight() - dst.w * 0.5f;
		overlay->Render(view, src, dst, 0xC00000, ms_lenzTable[j].color, 0.0f, 0);
	}

	view->SetClip(clipNear, clipFar, false);
	view->UpdateCamera();
}
