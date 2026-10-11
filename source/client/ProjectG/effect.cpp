// Original: d:\build\custom\temp\code\client\projectg\effect.cpp
#include "effect.h"

#include "wresrcmng.h"
#include "wview.h"
#include "wvideo.h"

// effect.cpp:16
CRing::CRing(void) {
	m_pOuter = NULL; m_pInner = NULL; m_hTexture = 0; m_hTexture = g_resrcmng->LoadTexture("[ring.jpg", 0, 0, NULL);
	Init();
}

// effect.cpp:22
CRing::CRing(const char* texname) {
	m_pOuter = NULL; m_pInner = NULL; m_hTexture = 0; m_hTexture = g_resrcmng->LoadTexture(texname, 0, 0, NULL);
	Init();
}

// effect.cpp:28
CRing::~CRing(void) {
	if (m_pOuter) { delete[] m_pOuter; m_pOuter = NULL; }
	if (m_pInner) { delete[] m_pInner; m_pInner = NULL; }

	if (m_hTexture) {
		g_resrcmng->Release(m_hTexture);
		m_hTexture = 0;
	}
}

// effect.cpp:40
void CRing::Init(void) {
	m_count = 16;
	m_pOuter = new WVector[m_count];
	m_pInner = new WVector[m_count];

	m_bActive = false;

	m_center.x = m_center.y = m_center.z = 0.0f;
	m_radius = 3.0f;
	m_width = 0.6f;

	m_angle = 0.0f;
	m_innerRatio = 0.98f;
}

// effect.cpp:57
void CRing::SetCenter(const WVector& center) {
	m_center = center;
}

// effect.cpp:62
void CRing::Process(float elapsed) {
	if (!m_bActive)
		return;

	m_radius = 2.0f;
	m_angle += elapsed * 2.5132742f;

	for (int i = 0; i < m_count; i++) {
		m_pOuter[i] = m_radius * RotMat(i * 6.2831855f / m_count + m_angle, 1).xa + m_center;
		m_pInner[i] = m_innerRatio * m_radius * RotMat(i * 6.2831855f / m_count - m_angle, 1).xa + m_center;
	}
}

// effect.cpp:78
void CRing::Display(void) {
	if (!m_bActive)
		return;

	WTVertex v[4];
	WTVertex* vl[5];

	for (int i = 0; i < m_count; i++) {
		for (int j = 0; j < 4; j++) {
			WVector p = m_pOuter[(j & 2) ? (i + 1) % m_count : i];
			float s = (j & 1) ? -1.0f : 1.0f;
			p.y += s * m_width * 0.5f; v[j].SetPosition(p); v[j].diffuse = 0x32FFFFFF; v[j].tu = (float)((j & 2) ? i + 1 : i) / m_count;
			v[j].tv = (j & 1) ? 0.0f : 1.0f;
		}

		vl[0] = &v[0];
		vl[1] = &v[1];
		vl[2] = &v[3];
		vl[3] = &v[2];
		vl[4] = NULL;

		g_view->DrawPolygonFan(vl, (m_hTexture & 0x7FF) | 0xC00000, 0x400, false);
	}

	for (int i = 0; i < m_count; i++) {
		float width = m_width * m_innerRatio;
		for (int j = 0; j < 4; j++) {
			WVector p = m_pInner[(j & 2) ? (i + 1) % m_count : i];
			float s = (j & 1) ? -1.0f : 1.0f;
			p.y += s * width * 0.5f; v[j].SetPosition(p); v[j].diffuse = 0x50FFFFFF; v[j].tu = (float)((j & 2) ? i + 1 : i) / m_count;
			v[j].tv = (j & 1) ? 0.0f : 1.0f;
		}

		vl[0] = &v[0];
		vl[1] = &v[1];
		vl[2] = &v[3];
		vl[3] = &v[2];
		vl[4] = NULL;

		g_view->DrawPolygonFan(vl, (m_hTexture & 0x7FF) | 0xC00000, 0x400, false);
	}
}
