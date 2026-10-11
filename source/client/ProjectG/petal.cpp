// Original: d:\build\custom\temp\code\client\projectg\petal.cpp
// The original holds the out-of-line copies of these header inlines.
#define PETAL_PETBODY_INLINE
#define PETAL_WPUPPET_INLINE
#include "Petal.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "petbody.h"
#include "SharedDoc.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wview.h"
#include "../client/Wangreal/include/wpuppet.h"
#include "commonutil.h"
#include "wind.h"

// TEMP: the dot product has no out-of-line copy in the binary to place it in wmath.inl (as in IceSnow.cpp)
inline float operator%(const WVector& a, const WVector& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

float Random(float min, float max, int seed);

extern WMatrix g_camera;

// petal.cpp:37
ntFlutter::ntFlutter(int count, const std::string& name, int unused) : m_frags(0), m_name(name) {
	memset(m_pets, 0, sizeof(m_pets));

	m_area.min = WVector(-112.0f, -78.4f, -112.0f);
	m_area.max = WVector(112.0f, 33.6f, 112.0f);

	Reset(count, 1.0f);
}

// petal.cpp:48
ntFlutter::~ntFlutter(void) {
	ReleasePets();

	if (m_frags) { delete[] m_frags; m_frags = 0; }
}

// petal.cpp:56
void ntFlutter::Reset(int nType, float fValue) {
	m_count = nType;

	ReleasePets();
	if (m_frags) { delete[] m_frags; m_frags = 0; }

	m_frags = new sFrag[m_count];

	m_size = fValue;

	char buf[260];
	sprintf(buf, "*pet %s", MakeStr(m_name.c_str(), rand() % 2 + 1));

	int count = m_count > 10 ? 10 : m_count;
	for (int i = 0; i < count; i++) {
		m_pets[i] = new CPetBody(buf, false);
		w_motion_data* md = m_pets[i]->GetPet()->GetFirstMotionData();
		if (md)
			m_pets[i]->SetMotion(md->name, false, 0.0f, false);
		m_pets[i]->Process(Random(0.0f, 2.0f, -1));
		m_pets[i]->ActiveGlobalLight(true);
	}

	for (int i = 0; i < m_count; i++) {
		m_frags[i].yaw = Random(0.0f, 6.2831855f, -1);
		m_frags[i].pitch = Random(0.0f, 3.1415927f, -1);

		for (int j = 0; j < 3; j++) m_frags[i].center.p[j] = g_camera.pivot.p[j] + Random(-1.0f, 1.0f, -1) * 112.0f;

		m_frags[i].pos = m_frags[i].center + m_frags[i].offset;
	}
}

// petal.cpp:97
void ntFlutter::Process(float fDelta, float fTime) {
	Waabb area = m_area;
	area.min += g_camera.pivot;
	area.max += g_camera.pivot;

	WVector size = m_area.max - m_area.min;

	for (int i = 0; i < m_count; i++) {
		m_frags[i].center += fDelta * Wind().GetWind(m_frags[i].center);
		m_frags[i].center.y -= fDelta * fTime * 34.295296f;

		m_frags[i].pitch += fDelta * 1.5707964f;
		float s = sin(m_frags[i].pitch);

		m_frags[i].offset = s * (WVector(cos(m_frags[i].yaw), 0.0f, sin(m_frags[i].yaw)) * 4.0f);
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

	for (int i = 0; i < 10; i++) {
		if (m_pets[i])
			m_pets[i]->Process(fDelta);
	}
}

// petal.cpp:134
void ntFlutter::Display(void) {
	for (int i = 0; i < m_count; i++) {
		if (m_frags[i].visible) {
			m_pets[i % 10]->SetPos(m_frags[i].pos);
			m_pets[i % 10]->SetLight();
			m_pets[i % 10]->Display();
		}
	}
}

// petal.cpp:151
void ntFlutter::DisplayFullScreenOverlay(void) {
	if (GetWeather())
		WOverlay::DrawBox(g_view, WRect(0.0f, 0.0f, g_view->GetWidth(), g_view->GetHeight()), 0, 0x4C3C3C3C, 0.001f);
}

// petal.cpp:158
void ntFlutter::ReleasePets(void) {
	for (int i = 0; i < 10; i++) {
		if (m_pets[i]) { delete m_pets[i]; m_pets[i] = 0; }
	}
}
