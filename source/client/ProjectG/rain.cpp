// Original: d:\build\custom\temp\code\client\projectg\rain.cpp
#include "rain.h"
#include <new>
#include <stdlib.h>
#include <math.h>
#include "woverlay.h"
#include "wview.h"
#include "wind.h"

extern WMatrix g_camera;

// rain.cpp:22
CRain::CRain(void) {
	m_frags = new sFrag[500];
	m_area = WVector(256.0f, 128.0f, 256.0f);
	m_height = 128.0f;

	for (int i = 0; i < 500; i++) {
		m_frags[i].visible = true;

		for (int j = 0; j < 3; j++) {
			m_frags[i].pos.p[j] = ((rand() % 1023) * (1.0f / 1024.0f) - 0.5f) * m_area.p[j] + g_camera.pivot.p[j];
		}
	}

	UpdateWind();
}

// rain.cpp:40
void CRain::UpdateWind(void) {
	m_wind = Wind().GetGlobalWind() - WVector(0.0f, 20.0f, 0.0f);
	m_wind.Normalize();
	m_wind *= 10.0f;
}

// rain.cpp:47
CRain::~CRain(void) {
	if (m_frags) { delete[] m_frags; m_frags = 0; }
}

// rain.cpp:52
void CRain::Process(float fDelta) {
	Waabb area;
	area.min = g_camera.pivot - m_area * 0.5f;
	area.max = m_area * 0.5f + g_camera.pivot;

	for (int i = 0; i < 500; i++) {
		m_frags[i].pos += m_wind * 10.0f * fDelta;
		m_frags[i].visible = (m_frags[i].pos - g_camera.pivot).Magnitude() < m_height || (m_frags[i].pos + m_wind - g_camera.pivot).Magnitude() < m_height ? true : false;

		for (int j = 0; j < 3; j++) {
			if (m_frags[i].pos.p[j] > area.max.p[j]) {
				m_frags[i].pos.p[j] -= (floor((m_frags[i].pos.p[j] - area.max.p[j]) / m_area.p[j]) + 1.0f) * m_area.p[j];
			} else if (m_frags[i].pos.p[j] < area.min.p[j]) {
				m_frags[i].pos.p[j] += (floor((area.min.p[j] - m_frags[i].pos.p[j]) / m_area.p[j]) + 1.0f) * m_area.p[j];
			}
		}
	}
}

// rain.cpp:77
void CRain::Display(void) {
	WSphere sphere;
	sphere.radius = m_wind.Magnitude() * 0.5f;

	for (int i = 0; i < 500; i++) {
		if (m_frags[i].visible) {
			sphere.pos = m_wind * 0.5f + m_frags[i].pos;

			if (g_view->InFrustum(sphere)) {
				g_view->DrawLine(m_frags[i].pos, 0xA0E0E0E0, m_frags[i].pos + m_wind, 0xA0E0E0E0, 0);
			}
		}
	}
}

// rain.cpp:96
void CRain::DisplayFullScreenOverlay(void) {
	WOverlay::DrawBox(g_view, WRect(0, 0, g_view->GetWidth(), g_view->GetHeight()), 0, 0x60303030, 0.001f);
}
