// Original: d:\build\custom\temp\code\client\projectg\icesnow.cpp
#include "weather.h"
#include "wind.h"
#include "w3danispr.h"
#include "wresrcmng.h"
#include "wview.h"
#include "woverlay.h"
#include "SharedDoc.h"
#include <stdlib.h>
#include <math.h>

// TEMP: the dot product has no out-of-line copy in the binary to place it in wmath.inl
inline float operator%(const WVector& a, const WVector& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

float Random(float min, float max, int seed);

extern WMatrix g_camera;

// icesnow.cpp:24
ntIceSnow::ntIceSnow(int count, const char* name, int spriteIndex) : m_frags(0), m_spr(0) {
	m_spr = g_resrcmng->Get3DAniSpr(name, 0, 8.0f, 8.0f);

	m_spriteIndex = spriteIndex;

	m_area.min = WVector(-160.0f, -112.0f, -160.0f);
	m_area.max = WVector(160.0f, 48.0f, 160.0f);

	Reset(count, 1.0f);
}

// icesnow.cpp:38
ntIceSnow::~ntIceSnow(void) {
	if (m_frags) { delete[] m_frags; m_frags = 0; }
	if (g_resrcmng && m_spr) { g_resrcmng->Release(m_spr); m_spr = 0; }
}

// icesnow.cpp:45
void ntIceSnow::Reset(int nType, float fValue) {
	m_count = nType;

	if (m_frags) { delete[] m_frags; m_frags = 0; }
	m_frags = new sFrag[m_count];

	m_size = fValue;
	m_spr->SetRect(m_size, m_size, m_size * 0.5f, m_size * 0.5f);

	for (int i = 0; i < m_count; i++) {
		m_frags[i].spriteIndex = m_spriteIndex >= 0 ? m_spriteIndex : rand() % m_spr->GetSpriteNum();
		m_frags[i].yaw = Random(0.0f, 6.2831855f, -1);
		m_frags[i].pitch = Random(0.0f, 3.1415927f, -1);

		for (int j = 0; j < 3; j++) m_frags[i].center.p[j] = g_camera.pivot.p[j] + Random(-1.0f, 1.0f, -1) * 160.0f;

		m_frags[i].pos = WVector(m_frags[i].offset.x + m_frags[i].center.x, m_frags[i].offset.y + m_frags[i].center.y, m_frags[i].offset.z + m_frags[i].center.z);
	}
}

// icesnow.cpp:71
void ntIceSnow::Process(float fDelta) {
	WSphere sphere;

	Waabb area = m_area;
	area.min += g_camera.pivot;
	area.max += g_camera.pivot;

	WVector size = m_area.max - m_area.min;

	for (int i = 0; i < m_count; i++) {
		m_frags[i].center += fDelta * Wind().GetWind(m_frags[i].center);
		m_frags[i].center.y -= fDelta * m_gravity * 34.295296f;

		m_frags[i].pitch += fDelta * 1.5707964f;
		float s = sin(m_frags[i].pitch);

		m_frags[i].offset = s * WVector(cos(m_frags[i].yaw) * 4.0f, 0.0f, sin(m_frags[i].yaw) * 4.0f);
		for (int j = 0; j < 3; j++) {
			if (m_frags[i].center.p[j] > area.max.p[j]) {
				m_frags[i].center.p[j] -= ceil((m_frags[i].center.p[j] - area.max.p[j]) / size.p[j]) * size.p[j];
			} else if (m_frags[i].center.p[j] < area.min.p[j]) {
				m_frags[i].center.p[j] += ceil((area.min.p[j] - m_frags[i].center.p[j]) / size.p[j]) * size.p[j];
			}
		}

		m_frags[i].pos = m_frags[i].center + m_frags[i].offset;
		m_frags[i].visible = (m_frags[i].pos - g_camera.pivot) % g_camera.za > 0.0f;
	}
}

// icesnow.cpp:102
void ntIceSnow::Display(void) {
	for (int i = 0; i < m_count; i++) {
		if (m_frags[i].visible) {
			m_spr->SetPos(m_frags[i].pos);
			m_spr->Render(g_view, 0.0f, 0x20800000, m_frags[i].spriteIndex);
		}
	}
}

// icesnow.cpp:117
void ntIceSnow::DisplayFullScreenOverlay(void) {
	if (GetWeather()) {
		WOverlay::DrawBox(g_view, WRect(0, 0, g_view->GetWidth(), g_view->GetHeight()), 0, 0x4C3C3C3C, 0.001f);
	}
}
