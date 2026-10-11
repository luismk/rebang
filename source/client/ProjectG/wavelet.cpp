// Original: d:\build\custom\temp\code\client\projectg\wavelet.cpp
#include "wavelet.h"
#include "wvideo.h"
#include "wresrcmng.h"
#include "wview.h"
#include "quadtree.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include <stdio.h>
#include <string.h>

WTVertex CWavelet::vtr[4];
WTVertex* CWavelet::vtx[5] = { &vtr[0], &vtr[1], &vtr[3], &vtr[2], NULL };

// wavelet.cpp:16
CWavelet::CWavelet(void) : m_texture(0), m_range(16.0f) {
	vtr[0].tv = vtr[2].tv = 0;
	vtr[1].tv = vtr[3].tv = 1.0f;
}

// wavelet.cpp:22
CWavelet::~CWavelet(void) {
	m_wavelets.clear();
	m_layers.clear();

	if (g_resrcmng && m_texture) { g_resrcmng->Release(m_texture); m_texture = 0; }
}

// wavelet.cpp:30
void CWavelet::SetLayer(int count) {
	m_layers.reserve(count);

	for (int i = 0; i < count; i++) {
		sLayer layer;
		layer.m_alpha = 0;
		layer.m_speed = 38.4f;
		layer.m_offset = 0;
		layer.m_state = 2;
		layer.m_delay = 0;
		layer.m_time = 0;

		m_layers.push_back(layer);
	}

	m_layers[0].m_state = 0;
}

// wavelet.cpp:56
void CWavelet::ReadAttribute(const char* attribute) {
	int id;

	strchr(attribute, '_');

	if (strchr(attribute, '_') && sscanf(strchr(attribute, '_') + 1, "%d", &id)) {
		m_texture = g_resrcmng->LoadTexture(GOLFDOC()->m_waveInfo.GetTexture(id), 0, 0, 0);
		m_range = GOLFDOC()->m_waveInfo.GetWidth(id);
		m_freq = GOLFDOC()->m_waveInfo.GetFreq(id);

		m_fadeInTime = m_freq * 0.08f;
		m_fadeOutTime = m_freq * 0.72f;
		m_restartDelay = m_freq * 0.2f;
		for (int i = 0; i < 4; i++) {
			vtr[i].diffuse = 0xFFFFFF;
		}

		m_deceleration = m_range / (m_fadeInTime * 4.0f - m_fadeInTime * m_fadeInTime * 0.5f);
	}
}

// wavelet.cpp:85
void CWavelet::SetWavelet(const std::vector<WVector>& points, const char* attribute, int layer) {
	ReadAttribute(attribute);

	m_wavelets.clear();

	m_wavelets.reserve(points.size());

	float height = GetPVS().GetGroundPoint(points[1], true, NULL, true).y;

	for (unsigned int i = 0; i < points.size(); i++) {
		sWavelet wavelet;

		wavelet.pos[0] = points[i];
		wavelet.pos[0].y = height;

		if (i == points.size() - 1) {
			wavelet.dir = m_wavelets[i - 1].dir;
		} else {
			wavelet.dir = points[i + 1] - points[i];

			wavelet.dir = WCrossProduct(WVector(wavelet.dir.x, 0, wavelet.dir.z), WVector::UNIT_POS_Y).Normalize();
		}

		wavelet.pos[1] = wavelet.pos[0] + wavelet.dir * m_range;

		m_wavelets.push_back(wavelet);
	}

	for (int k = 0; k < 2; k++) {
		m_wavelets[0].length[k] = 0;

		for (unsigned int j = 1; j < m_wavelets.size(); j++) {
			m_wavelets[j].length[k] = m_wavelets[j - 1].length[k] + (m_wavelets[j].pos[k] - m_wavelets[j - 1].pos[k]).Magnitude() / m_range;
		}
	}

	SetLayer(layer);
}

// wavelet.cpp:138
void CWavelet::Process(float elapsed) {
	for (unsigned char i = 0; i < m_layers.size(); i++) {
		m_layers[i].m_time += elapsed;

		switch (m_layers[i].m_state) {
		case 0:
			m_layers[i].m_alpha += elapsed * (255.0f / m_fadeInTime);
			m_layers[i].m_speed -= elapsed * m_deceleration;
			m_layers[i].m_offset += elapsed * m_layers[i].m_speed;

			if (m_layers[i].m_alpha > 255.0f) {
				m_layers[i].m_alpha = 255.0f;
				m_layers[i].m_state = 1;
				m_layers[(i + 1) % m_layers.size()].m_delay = m_restartDelay;
			}
			break;
		case 1:
			m_layers[i].m_alpha -= elapsed * (255.0f / m_fadeOutTime);
			m_layers[i].m_speed -= elapsed * m_deceleration;
			m_layers[i].m_offset += elapsed * m_layers[i].m_speed;

			if (m_layers[i].m_alpha < 0.0f) {
				m_layers[i].m_state = 2;
			}
			break;
		case 2:
			if (m_layers[i].m_delay > 0.0f) {
				m_layers[i].m_delay -= elapsed;
				if (m_layers[i].m_delay < 0.0f) {
					m_layers[i].m_state = 0;
					m_layers[i].m_alpha = 0;
					m_layers[i].m_time = 0;
					m_layers[i].m_offset = 0;
					m_layers[i].m_speed = m_deceleration * 4.0f;
				}
			}
			break;
		}
	}
}

// wavelet.cpp:186
void CWavelet::Render(void) {
	for (unsigned char i = 0; i < m_layers.size(); i++) {
		for (unsigned char j = 0; j < m_wavelets.size() - 1; j++) {
			for (unsigned char k = 0; k < 4; k++) {
				WVector pos;
				unsigned char index = ((k >> 1) & 1) + j;
				bool edge = index == 0 || index == m_wavelets.size() - 1;

				WVector dir = m_wavelets[index].dir * (((k & 1) ? 0.1f : (edge ? 0.5f : 1.0f)) * m_layers[i].m_offset);
				pos = GetPVS().GetGroundPoint(m_wavelets[index].pos[k & 1] - dir, true, NULL, true);

				vtr[k].SetPosition(pos);

				vtr[k].tu = m_wavelets[index].length[k & 1];

				if (edge) {
					vtr[k].diffuse = 0xFFFFFF;
				} else {
					vtr[k].diffuse = ((int)m_layers[i].m_alpha << 24) | 0xFFFFFF;
				}
			}

			g_view->DrawPolygonFan(vtx, (m_texture & 0x7FF) | 0x28C00000, 0x400, false);
		}
	}
}
