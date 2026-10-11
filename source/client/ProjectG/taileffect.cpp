// Original: d:\build\custom\temp\code\client\projectg\taileffect.cpp
#include "scenemanager.h"
#include "TailEffect.h"
#include "wresrcmng.h"
#include "wview.h"
#include "wvideo.h"

extern int index;
extern "C" int data_af23f0;

// scenemanager.h:67
void CRenderFuncPtr::DisplayShadow(void) {
}

// scenemanager.h:71
void CRenderFuncPtr::DebugDisplay(void) {
}

// taileffect.cpp:17
CTailEffect::CTailEffect(const std::string &name) {
	m_texture = 0;
	m_head = 0;
	m_tail = 0;
	m_clearedFlag = false;
	m_clearedCount = 0;
	Init(name);
}

// taileffect.cpp:27
CTailEffect::~CTailEffect(void) {
	for (std::map<std::string, int>::iterator it = m_textures.begin(); it != m_textures.end(); ++it) {
		if (it->second != 0) {
			g_resrcmng->Release(it->second);
		}
	}

	m_textures.clear();
}

// taileffect.cpp:40
void CTailEffect::Init(const std::string &name) {
	m_maxEdges = 30;
	m_life = 0.3f;
	m_edges.reserve(m_maxEdges);
	m_spots.reserve(m_maxEdges);

	ChangeTexture(std::string(name.c_str()));
}

// taileffect.cpp:61
void CTailEffect::Reset(void) {
	for (int i = 0; i < m_maxEdges; ++i) {
		m_edges[i].bUse = false;
	}

	m_edges.clear();

	m_edges.reserve(m_maxEdges);

	m_tail = 0;
	m_head = 0;
	index = 0;
}

// taileffect.cpp:82
void CTailEffect::PutSpot(const WVector &spot1, const WVector &spot2) {
	if (WisEqual(m_lastSpot1, spot1, 0.00001f) && WisEqual(m_lastSpot2, spot2, 0.00001f)) {
		data_af23f0++;
		return;
	}

	if (index >= m_maxEdges) {
		return;
	}

	TAILEDGE edge;
	edge.bUse = true;
	edge.fLife = m_life;
	edge.v1 = spot1;
	edge.v2 = spot2;
	m_spots[index++] = edge;

	m_lastSpot1 = spot1;
	m_lastSpot2 = spot2;

	m_tail = 0;
	m_head = 0;

	if (index > 3) {
		for (int i = 0; i < index - 3; i++) {
			for (int j = 0; j < 5; j++) {
				float t = j * 0.2f;
				float t2 = t * t;
				float t3 = t2 * t;
				float b0 = -t3 + 3 * t2 - 3 * t + 1;
				float b1 = 3 * t3 - 6 * t2 + 0 * t + 4;
				float b2 = -3 * t3 + 3 * t2 + 3 * t + 1;
				float b3 = t3 + 0 * t2 + 0 * t;
				WVector v1;
				v1.x = (b3 * m_spots[i + 3].v1.x + b2 * m_spots[i + 2].v1.x + b1 * m_spots[i + 1].v1.x + b0 * m_spots[i].v1.x) / 6;
				v1.y = (b3 * m_spots[i + 3].v1.y + b2 * m_spots[i + 2].v1.y + b1 * m_spots[i + 1].v1.y + b0 * m_spots[i].v1.y) / 6;
				v1.z = (b3 * m_spots[i + 3].v1.z + b2 * m_spots[i + 2].v1.z + b1 * m_spots[i + 1].v1.z + b0 * m_spots[i].v1.z) / 6;
				WVector v2;
				v2.x = (b3 * m_spots[i + 3].v2.x + b2 * m_spots[i + 2].v2.x + b1 * m_spots[i + 1].v2.x + b0 * m_spots[i].v2.x) / 6;
				v2.y = (b3 * m_spots[i + 3].v2.y + b2 * m_spots[i + 2].v2.y + b1 * m_spots[i + 1].v2.y + b0 * m_spots[i].v2.y) / 6;
				v2.z = (b3 * m_spots[i + 3].v2.z + b2 * m_spots[i + 2].v2.z + b1 * m_spots[i + 1].v2.z + b0 * m_spots[i].v2.z) / 6;
				edge.bUse = true;
				edge.fLife = m_life;
				edge.v1 = v1;
				edge.v2 = v2;
				m_edges[m_head] = edge;
				m_head = (m_head + 1) % m_maxEdges;
				if (m_head == m_tail) {
					m_tail = (m_tail + 1) % m_maxEdges;
				}
			}
		}
	}
}

// taileffect.cpp:159
void CTailEffect::Process(float dt) {
	int tail = m_tail, length = GetLength();
	if (length == 0) {
		return;
	}

	for (int i = 0; i < length; ++i) {
		int at = (tail + i) % m_maxEdges;
		if (m_edges[at].bUse) {
			m_edges[at].fLife -= dt;
			if (m_edges[at].fLife <= 0.0f) {
				m_tail = (m_tail + 1) % m_maxEdges;
			}
		}
	}

	if (GetLength() == 0) {
		Reset();
	}
}

// taileffect.cpp:184
void CTailEffect::Display(void) {
	int length = GetLength();
	if (length == 0) {
		return;
	}

	// one quad per pair of consecutive edges, fading in from the tail
	WTVertex v[4];
	WTVertex* vl[4];
	vl[0] = &v[0];
	vl[1] = &v[1];
	vl[2] = &v[3];
	vl[3] = &v[2];

	for (int i = 0; i < length - 1; ++i) {
		int at = (m_tail + i) % m_maxEdges;
		float alpha = i / (float)length;
		unsigned long color = ((int)(255.0f * alpha) << 24) | 0xffffff;

		for (int j = 0; j < 4; ++j) {
			int edge = (j & 2) ? (at + 1) % m_maxEdges : at;
			WVector pos = (j & 1) ? m_edges[edge].v2 : m_edges[edge].v1;

			v[j].SetPosition(pos);
			v[j].tu = (j & 2) ? (i + 1) / (float)length : alpha;
			v[j].tv = (j & 1) ? 0.0f : 1.0f;

			v[j].diffuse = color;
		}

		g_view->DrawPolygonFan(vl, (m_texture & 0x7ff) | 0x20C00000, 0x400, false);
	}
}

// taileffect.cpp:250
void CTailEffect::ChangeTexture(const std::string &name) {
	std::map<std::string, int>::iterator it = m_textures.find(name);

	if (it != m_textures.end()) {
		m_texture = it->second;
	} else {
		m_texture = g_resrcmng->LoadTexture(name.c_str(), 0, 0, 0);
		m_textures[name] = m_texture;
	}
}
