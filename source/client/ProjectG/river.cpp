// Original: d:\build\custom\temp\code\client\projectg\river.cpp
#include "river.h"
#include "wresrcmng.h"
#include "wview.h"
#include "wvideo.h"
#include "quadtree.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "commonutil.h"
#include "slicestr.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "wutil.h"

float Random(float min, float max, int seed);

// The original's min/max helper (compare shape `a < b ? b : a`, not wutil.h's
// Max); its header is unknown, as for camerawork.cpp's MinOf.
template <class T> T MinOf(T a, T b) { return a > b ? b : a; }
template <class T> T MaxOf(T a, T b) { return a < b ? b : a; }

WTVertex CRiver::vtr[4];
WTVertex* CRiver::vtx[5] = { &vtr[0], &vtr[1], &vtr[3], &vtr[2], NULL };
WTVertex ntRiver_Lava::vtr[4];
WTVertex* ntRiver_Lava::vtx[5] = { &vtr[0], &vtr[1], &vtr[3], &vtr[2], NULL };
WTVertex CRiver_ex::vtr[4];
WTVertex* CRiver_ex::vtx[5] = { &vtr[0], &vtr[1], &vtr[3], &vtr[2], NULL };

// river.cpp:29
CRiver::CRiver(void) {
	for (int i = 0; i < 2; i++) {
		m_layers[i].offset = 0.0f;
		m_layers[i].speed = 0.0f;
		m_layers[i].texture = 0;
		m_layers[i].tileHead = 0.0f;
		m_layers[i].tileSide = 0.0f;
		m_layers[i].depth = 0.0f;
		m_layers[i].enabled = false;
	}
}

// river.cpp:43
CRiver::~CRiver(void) {
	for (int i = 0; i < 2; i++) {
		if (g_resrcmng != NULL && m_layers[i].texture != 0) { g_resrcmng->Release(m_layers[i].texture); m_layers[i].texture = 0; }
	}
}

// river.cpp:49
void CRiver::ReadAttribute(const char* name) {
	int index[2] = { -1, -1 };

	if (strstr(name, "_")) {
		m_layers[0].enabled = sscanf(strchr(name, '_') + 1, "%d", &index[0]) != 0;

		if (sscanf(strrchr(name, '_') + 1, "%d", &index[1]) && index[1] != index[0]) {
			m_layers[1].enabled = true;
		} else {
			m_layers[1].enabled = false;
		}

		for (int i = 0; i < 2; i++) {
			if (m_layers[i].enabled) {
				m_layers[i].offset = Random(0.0f, 1.0f, -1);
				m_layers[i].speed = GOLFDOC()->m_riverInfo.GetScrollVelocity(index[i]);
				m_layers[i].tileHead = GOLFDOC()->m_riverInfo.GetHeadTile(index[i]);
				m_layers[i].tileSide = GOLFDOC()->m_riverInfo.GetSideTile(index[i]);
				m_layers[i].depth = GOLFDOC()->m_riverInfo.GetDepth(index[i]) * 3.2f;
				m_layers[i].texture = g_resrcmng->LoadTexture(MakeStr("[%s_river_%s.jpg", WSingleton<CSharedDoc>::Instance()->m_pCurCourse ? WSingleton<CSharedDoc>::Instance()->m_pCurCourse->m_mapName : 0, i == 0 ? "up" : "down"), 0, 0, 0);
			}
		}
	} else if (strstr(name, "*d")) {
		float depth = 0.0f;
		int count = sscanf(strchr(name, 'd') + 1, "%f", &depth);
		if (count == EOF) {
			depth = 1.0f;
		}

		for (int i = 0; i < 2; i++) {
			m_layers[i].offset = Random(0.0f, 1.0f, -1);
			m_layers[i].speed = Random(1.0f, 3.0f, -1);
			m_layers[i].tileHead = 1.0f;
			m_layers[i].tileSide = 1.0f;
			m_layers[i].depth = depth;
			m_layers[i].texture = g_resrcmng->LoadTexture(MakeStr("[%s_river_%s.jpg", WSingleton<CSharedDoc>::Instance()->m_pCurCourse ? WSingleton<CSharedDoc>::Instance()->m_pCurCourse->m_mapName : 0, i == 0 ? "up" : "down"), 0, 0, 0);
			m_layers[i].enabled = true;
		}
	} else {
		for (int i = 0; i < 2; i++) {
			m_layers[i].offset = Random(0.0f, 1.0f, -1);
			m_layers[i].speed = Random(1.0f, 3.0f, -1);
			m_layers[i].tileHead = 1.0f;
			m_layers[i].tileSide = 1.0f;

			char* file;
			if (GetCurMap() == 11) {
				m_layers[i].depth = 10.56f;
				file = MakeStr("[pink_river_%s.jpg", i == 0 ? "up" : "down");
			} else if (GetCurMap() == 15) {
				m_layers[i].depth = 3.2f;
				file = MakeStr("[gs_river_%s.jpg", i == 0 ? "up" : "down");
			} else {
				m_layers[i].depth = 3.2f;
				file = MakeStr("[wind_river_%s.jpg", i == 0 ? "up" : "down");
			}
			m_layers[i].texture = g_resrcmng->LoadTexture(file, 0, 0, 0);
			m_layers[i].enabled = true;
		}
	}
}

// river.cpp:136
void CRiver::Setup(const std::vector<WVector>& points, const char* name) {
	ReadAttribute(name);

	Waabb aabb;
	WVector dir = points[1] - points[0];
	dir = WCrossProduct(WVector(dir.x, 0.0f, dir.z), WVector::UNIT_POS_Y).Normalize();

	m_width = 0.0f;
	float height = GetPVS().GetLowestGround(points[1]) + MaxOf(m_layers[0].depth, m_layers[1].depth);

	// Widen the strip until the ground rises above the surface.
	do {
		m_width += 3.2f;
	} while (GetPVS().GetLowestGround(dir * m_width + points[1]) <= height);
	m_width += 3.2f;

	m_rivers.reserve(50);

	unsigned int n = 0;
	for (unsigned int i = 0; i < points.size() - 1; i++) {
		dir = points[i + 1] - points[i];
		dir = WCrossProduct(WVector(dir.x, 0.0f, dir.z), WVector::UNIT_POS_Y).Normalize();
		dir *= m_width;

		// One segment per 1.6 units of height difference.
		float count = MaxOf(1.0f, (float)ceil(Abs(GetPVS().GetLowestGround(points[i + 1]) - GetPVS().GetLowestGround(points[i])) * 0.625f));

		float step = 1.0f / count;
		float ground = 0.0f;
		for (float t = 0.0f; t < 1.0f; t += step) {
			WVector pos = points[i] * (1.0f - t) + points[i + 1] * t;
			float last = ground;
			ground = GetPVS().GetLowestGround(pos);
			if (last + 1.2f < ground) continue;

			sRiver river;
			for (int j = 0; j < 2; j++) {
				river.bank[j] = dir * ((j & 1) ? 1.0f : -1.0f) + pos;
				river.bank[j].y = ground + m_layers[0].depth;
				if (n == 0) {
					river.texV[j] = 0.0f;
				} else {
					WVector d = river.bank[j] - m_rivers[n - 1].bank[j];
					river.texV[j] = d.Magnitude() / m_width * m_layers[j].tileHead + m_rivers[n - 1].texV[j];
				}
			}

			if (n > 0) {
				const sRiver& prev = m_rivers[n - 1];
				aabb.max.x = MaxOf(MaxOf(river.bank[0].x, river.bank[1].x), MaxOf(prev.bank[0].x, prev.bank[1].x));
				aabb.max.y = MinOf(river.bank[0].y, prev.bank[0].y);
				aabb.max.z = MaxOf(MaxOf(river.bank[0].z, river.bank[1].z), MaxOf(prev.bank[0].z, prev.bank[1].z));

				aabb.min.x = MinOf(MinOf(river.bank[0].x, river.bank[1].x), MinOf(prev.bank[0].x, prev.bank[1].x));
				aabb.min.y = aabb.max.y - MaxOf(m_layers[0].depth, m_layers[1].depth);
				aabb.min.z = MinOf(MinOf(river.bank[0].z, river.bank[1].z), MinOf(prev.bank[0].z, prev.bank[1].z));

				GetPVS().AddWater(aabb, 2);
			}

			m_rivers.push_back(river);

			n++;
		}
	}
}

// river.cpp:234
void CRiver::Process(float fDelta) {
	for (int i = 0; i < 2; i++) {
		if (m_layers[i].enabled) {
			m_layers[i].offset -= fDelta * m_layers[i].speed;
		}
	}
}

// river.cpp:244
void CRiver::Render(void) {
	for (int i = 0; i < 2; i++) {
		if (m_layers[i].enabled) {
			for (unsigned char j = 0; j < 4; j++) {
				vtr[j].tu = (j & 1) ? 0.0f : m_layers[i].tileSide;
			}
			for (unsigned char k = 0; k < m_rivers.size() - 1; k++) {
				for (unsigned char j = 0; j < 4; j++) {
					unsigned char n = k + ((j >> 1) & 1);

					const WVector& p = m_rivers[n].bank[j & 1];
					vtr[j].x = p.x; vtr[j].y = p.y; vtr[j].z = p.z;

					vtr[j].tv = m_rivers[n].texV[i] + m_layers[i].offset;

					if (n == 0 || n == m_rivers.size() - 1) {
						vtr[j].diffuse = 0x00FFFFFF;
					} else {
						vtr[j].diffuse = 0xFFFFFFFF;
					}
				}
				g_view->DrawPolygonFan(vtx, (m_layers[i].texture & 0x7FF) | 0x28C00000, 0x400, false);
			}
		}
	}
}

// river.cpp:303
ntRiver_Lava::ntRiver_Lava(void) {
	for (int i = 0; i < 2; i++) {
		m_layers[i].offset = 0.0f;
		m_layers[i].speed = 0.0f;
		m_layers[i].texture = 0;
		m_layers[i].tileHead = 0.0f;
		m_layers[i].tileSide = 0.0f;
		m_layers[i].depth = 0.0f;
		m_layers[i].enabled = false;
	}
}

// river.cpp:317
ntRiver_Lava::~ntRiver_Lava(void) {
	for (int i = 0; i < 2; i++) {
		if (g_resrcmng != NULL && m_layers[i].texture != 0) { g_resrcmng->Release(m_layers[i].texture); m_layers[i].texture = 0; }
	}
}

// river.cpp:323
void ntRiver_Lava::Setup(const std::vector<WVector>& points, const char* name) {
	ReadAttribute(name);

	WVector dir;
	float height;
	float widths[256] = { 0.0f };
	Waabb aabb;
	unsigned int i;
	for (i = 0; i < points.size() - 1; i++) {
		dir = points[i + 1] - points[i];
		dir = WCrossProduct(WVector(dir.x, 0.0f, dir.z), WVector::UNIT_POS_Y).Normalize();

		height = GetPVS().GetLowestGround(points[i]) + MaxOf(m_layers[0].depth, m_layers[1].depth);

		// Widen the strip until the ground rises above the surface.
		do {
			if (GetPVS().GetLowestGround(dir * (widths[i] += 6.4f) + points[i]) > height) {
				widths[i] += 3.2f;
				break;
			}
		} while (widths[i] <= 160.0f);
	}

	m_rivers.reserve(50);

	unsigned int n = 0;
	for (i = 0; i < points.size() - 1; i++) {
		dir = points[i + 1] - points[i];
		dir = WCrossProduct(WVector(dir.x, 0.0f, dir.z), WVector::UNIT_POS_Y).Normalize();
		dir *= widths[n];

		sRiver river;
		WVector pos = points[i];
		float ground = GetPVS().GetLowestGround(points[i]);

		for (int j = 0; j < 2; j++) {
			river.bank[j] = dir * ((j & 1) ? 1.0f : -1.0f) + pos;
			river.bank[j].y = ground + m_layers[0].depth;
			if (n == 0) {
				river.texV[j] = 0.0f;
			} else {
				WVector d = river.bank[j] - m_rivers[n - 1].bank[j];
				river.texV[j] = d.Magnitude() / widths[n] * m_layers[j].tileHead + m_rivers[n - 1].texV[j];
			}
		}

		if (n > 0) {
			const sRiver& prev = m_rivers[n - 1];
			aabb.max.x = MaxOf(MaxOf(river.bank[0].x, river.bank[1].x), MaxOf(prev.bank[0].x, prev.bank[1].x));
			aabb.max.y = MinOf(river.bank[0].y, prev.bank[0].y);
			aabb.max.z = MaxOf(MaxOf(river.bank[0].z, river.bank[1].z), MaxOf(prev.bank[0].z, prev.bank[1].z));

			aabb.min.x = MinOf(MinOf(river.bank[0].x, river.bank[1].x), MinOf(prev.bank[0].x, prev.bank[1].x));
			aabb.min.y = aabb.max.y - MaxOf(m_layers[0].depth, m_layers[1].depth);
			aabb.min.z = MinOf(MinOf(river.bank[0].z, river.bank[1].z), MinOf(prev.bank[0].z, prev.bank[1].z));

			GetPVS().AddWater(aabb, 2);
		}

		m_rivers.push_back(river);

		n++;
	}
}

// river.cpp:420
void ntRiver_Lava::Process(float fDelta) {
	for (int i = 0; i < 2; i++) {
		if (m_layers[i].enabled) {
			m_layers[i].offset -= fDelta * m_layers[i].speed;
		}
	}
}

// river.cpp:429
void ntRiver_Lava::Render(void) {
	for (int i = 0; i < 2; i++) {
		if (m_layers[i].enabled) {
			for (unsigned char j = 0; j < 4; j++) {
				vtr[j].tu = (j & 1) ? 0.0f : 1.0f;
			}
			for (unsigned char k = 0; k < m_rivers.size() - 1; k++) {
				for (unsigned char j = 0; j < 4; j++) {
					unsigned char n = k + ((j >> 1) & 1);

					const WVector& p = m_rivers[n].bank[j & 1];
					vtr[j].x = p.x; vtr[j].y = p.y; vtr[j].z = p.z;

					vtr[j].tv = m_rivers[n].texV[i] + m_layers[i].offset;

					if (n == 0 || n == m_rivers.size() - 1) {
						vtr[j].diffuse = 0x00FFFFFF;
					} else {
						vtr[j].diffuse = 0xFFFFFFFF;
					}
				}
				g_view->DrawPolygonFan(vtx, (m_layers[i].texture & 0x7FF) | 0x28C00000, 0x400, false);
			}
		}
	}
}

// river.cpp:490
void ntRiver_Lava::ReadAttribute(const char* name) {
	int index[2];
	index[0] = 0;
	index[1] = 1;

	m_layers[0].enabled = true;
	m_layers[1].enabled = true;

	for (int i = 0; i < 2; i++) {
		m_layers[i].offset = Random(0.0f, 1.0f, -1);
		m_layers[i].speed = GOLFDOC()->m_riverInfo.GetScrollVelocity(index[i]);
		m_layers[i].tileHead = GOLFDOC()->m_riverInfo.GetHeadTile(index[i]);
		m_layers[i].tileSide = GOLFDOC()->m_riverInfo.GetSideTile(index[i]);
		m_layers[i].depth = GOLFDOC()->m_riverInfo.GetDepth(index[i]) * 3.2f;
		m_layers[i].texture = g_resrcmng->LoadTexture(MakeStr("[hell_river_%s.jpg", i == 0 ? "up" : "down"), 0, 0, 0);
	}

	if (strstr(name, "*d")) {
		float depth = 0.0f;
		int count = sscanf(strchr(name, 'd') + 1, "%f", &depth);
		if (count == EOF) {
			depth = 1.0f;
		}

		for (int i = 0; i < 2; i++) {
			m_layers[i].depth = depth;
		}
	}
}

// river.cpp:541
CRiver_ex::CRiver_ex(void) {
	for (int i = 0; i < 2; i++) {
		m_layers[i].offset = 0.0f;
		m_layers[i].speed = 0.0f;
		m_layers[i].texture = 0;
		m_layers[i].tileHead = 0.0f;
		m_layers[i].tileSide = 0.0f;
		m_layers[i].depth = 0.0f;
		m_layers[i].enabled = false;
	}
}

// river.cpp:555
CRiver_ex::~CRiver_ex(void) {
	for (int i = 0; i < 2; i++) {
		if (g_resrcmng != NULL && m_layers[i].texture != 0) { g_resrcmng->Release(m_layers[i].texture); m_layers[i].texture = 0; }
	}
}

// river.cpp:561
void CRiver_ex::ReadAttribute(const char* name) {
	int index[2] = { -1, -1 };

	if (strstr(name, "_")) {
		m_layers[0].enabled = sscanf(strchr(name, '_') + 1, "%d", &index[0]) != 0;

		if (sscanf(strrchr(name, '_') + 1, "%d", &index[1]) && index[1] != index[0]) {
			m_layers[1].enabled = true;
		} else {
			m_layers[1].enabled = false;
		}

		for (int i = 0; i < 2; i++) {
			if (m_layers[i].enabled) {
				m_layers[i].offset = Random(0.0f, 1.0f, -1);
				m_layers[i].speed = GOLFDOC()->m_riverInfo.GetScrollVelocity(index[i]);
				m_layers[i].tileHead = GOLFDOC()->m_riverInfo.GetHeadTile(index[i]);
				m_layers[i].tileSide = GOLFDOC()->m_riverInfo.GetSideTile(index[i]);
				m_layers[i].depth = GOLFDOC()->m_riverInfo.GetDepth(index[i]) * 3.2f;
				m_layers[i].texture = g_resrcmng->LoadTexture(MakeStr("[%s_river_%s.jpg", WSingleton<CSharedDoc>::Instance()->m_pCurCourse ? WSingleton<CSharedDoc>::Instance()->m_pCurCourse->m_mapName : 0, i == 0 ? "up" : "down"), 0, 0, 0);
			}
		}
	} else if (strstr(name, "*d")) {
		float depth = 0.0f;
		int count = sscanf(strchr(name, 'd') + 1, "%f", &depth);
		if (count == EOF) {
			depth = 1.0f;
		}

		float speed = 3.0f;
		if (strstr(name, "*v")) {
			WSliceStr slice(name);
			if (!slice.Get("*v", "%f", &speed)) {
				speed = 1.0f;
			}
		}

		for (int i = 0; i < 2; i++) {
			m_layers[i].offset = Random(0.0f, 1.0f, -1);
			m_layers[i].speed = speed;
			m_layers[i].tileHead = 1.0f;
			m_layers[i].tileSide = 1.0f;
			m_layers[i].depth = depth;
			m_layers[i].texture = g_resrcmng->LoadTexture(MakeStr("[%s_river_%s.jpg", WSingleton<CSharedDoc>::Instance()->m_pCurCourse ? WSingleton<CSharedDoc>::Instance()->m_pCurCourse->m_mapName : 0, i == 0 ? "up" : "down"), 0, 0, 0);
			m_layers[i].enabled = true;
		}
	} else {
		float speed = 3.0f;
		if (strstr(name, "*v")) {
			WSliceStr slice(name);
			if (!slice.Get("*v", "%f", &speed)) {
				speed = 1.0f;
			}
		}

		for (int i = 0; i < 2; i++) {
			m_layers[i].offset = Random(0.0f, 1.0f, -1);
			m_layers[i].speed = speed;
			m_layers[i].tileHead = 1.0f;
			m_layers[i].tileSide = 1.0f;

			char* file;
			if (GetCurMap() == 11) {
				m_layers[i].depth = 10.56f;
				file = MakeStr("[pink_river_%s.jpg", i == 0 ? "up" : "down");
			} else if (GetCurMap() == 15) {
				m_layers[i].depth = 3.2f;
				file = MakeStr("[gs_river_%s.jpg", i == 0 ? "up" : "down");
			} else if (GetCurMap() == 16) {
				m_layers[i].depth = 3.2f;
				file = MakeStr("[panda_river_%s.jpg", i == 0 ? "up" : "down");
			} else {
				m_layers[i].depth = 3.2f;
				file = MakeStr("[wind_river_%s.jpg", i == 0 ? "up" : "down");
			}
			m_layers[i].texture = g_resrcmng->LoadTexture(file, 0, 0, 0);
			m_layers[i].enabled = true;
		}
	}
}

// river.cpp:671
void CRiver_ex::Setup(const std::vector<WVector>& points, const char* name) {
	if (points.size() < 3) return;

	ReadAttribute(name);

	Waabb aabb;
	m_rivers.reserve(50);

	unsigned int n = 0;
	for (unsigned int i = 0; i < points.size() - 1; i += 2) {
		sRiver river;
		memset(&river, 0, sizeof(river));

		for (unsigned int j = 0; j < 2; j++) {
			river.bank[j] = points[i + j];
			river.texV[j] = 0.0f;
		}

		if (n > 0) {
			WVector d = river.bank[1] - river.bank[0];
			float width = d.Magnitude();
			if (width != 0.0f) {
				WVector a = river.bank[0] - m_rivers[n - 1].bank[0];
				WVector b = river.bank[1] - m_rivers[n - 1].bank[1];
				float inv = 1.0f / width;
				river.texV[0] = a.Magnitude() * inv * m_layers[0].tileHead + m_rivers[n - 1].texV[0];
				river.texV[1] = b.Magnitude() * inv * m_layers[1].tileHead + m_rivers[n - 1].texV[1];
			}

			aabb.max.x = MaxOf(MaxOf(river.bank[0].x, river.bank[1].x), MaxOf(m_rivers[n - 1].bank[0].x, m_rivers[n - 1].bank[1].x));
			aabb.max.y = MaxOf(MaxOf(river.bank[0].y, river.bank[1].y), MaxOf(m_rivers[n - 1].bank[0].y, m_rivers[n - 1].bank[1].y));
			aabb.max.z = MaxOf(MaxOf(river.bank[0].z, river.bank[1].z), MaxOf(m_rivers[n - 1].bank[0].z, m_rivers[n - 1].bank[1].z));

			aabb.min.x = MinOf(MinOf(river.bank[0].x, river.bank[1].x), MinOf(m_rivers[n - 1].bank[0].x, m_rivers[n - 1].bank[1].x));
			aabb.min.y = MinOf(MinOf(river.bank[0].y, river.bank[1].y), MinOf(m_rivers[n - 1].bank[0].y, m_rivers[n - 1].bank[1].y));
			aabb.min.z = MinOf(MinOf(river.bank[0].z, river.bank[1].z), MinOf(m_rivers[n - 1].bank[0].z, m_rivers[n - 1].bank[1].z));

			float height = aabb.max.y - aabb.min.y;

			if (fabs(height) > 8.0f) {
				// A steep segment: register it as four water boxes.
				WVector step = d * 0.25f;
				WVector cur = river.bank[0];
				WVector last = m_rivers[n - 1].bank[0];
				WVector lastStep = (m_rivers[n - 1].bank[1] - m_rivers[n - 1].bank[0]) * 0.25f;

				for (int k = 0; k < 4; k++) {
					WVector cur0 = cur; cur = cur + step;
					WVector last0 = last; last = last + lastStep;

					Waabb box;
					box.max.x = MaxOf(MaxOf(cur0.x, cur.x), MaxOf(last0.x, last.x));
					box.max.y = MaxOf(MaxOf(cur0.y, cur.y), MaxOf(last0.y, last.y));
					box.max.z = MaxOf(MaxOf(cur0.z, cur.z), MaxOf(last0.z, last.z));

					box.min.x = MinOf(MinOf(cur0.x, cur.x), MinOf(last0.x, last.x));
					box.min.y = MinOf(MinOf(cur0.y, cur.y), MinOf(last0.y, last.y));
					box.min.z = MinOf(MinOf(cur0.z, cur.z), MinOf(last0.z, last.z));

					GetPVS().AddWater(box, 2);
				}
			} else {
				GetPVS().AddWater(aabb, 2);
			}
		}

		m_rivers.push_back(river);

		n++;
	}
}

// river.cpp:784
void CRiver_ex::Process(float fDelta) {
	for (int i = 0; i < 2; i++) {
		if (m_layers[i].enabled) {
			m_layers[i].offset -= fDelta * m_layers[i].speed;
		}
	}
}

// river.cpp:794
void CRiver_ex::Render(void) {
	unsigned int count = m_rivers.size();
	if (count < 3) return;

	for (int i = 0; i < 2; i++) {
		if (m_layers[i].enabled) {
			for (unsigned char j = 0; j < 4; j++) {
				vtr[j].tu = (j & 2) ? 1.0f : 0.0f;
			}
			for (unsigned char k = 0; k < count - 1; k++) {
				for (unsigned int j = 0; j < 4; j++) {
					unsigned char n = k + (j & 1);

					const WVector& p = m_rivers[n].bank[(j >> 1) & 1];
					vtr[j].x = p.x; vtr[j].y = p.y; vtr[j].z = p.z;
					vtr[j].tv = m_rivers[n].texV[i] + m_layers[i].offset;

					if ((k < 1 && !(j & 1)) || (k > count - 3 && (j & 1))) {
						vtr[j].diffuse = 0x00FFFFFF;
					} else {
						vtr[j].diffuse = 0xFFFFFFFF;
					}
				}
				g_view->DrawPolygonFan(vtx, (m_layers[i].texture & 0x7FF) | 0x28C00000, 0x400, false);
			}
		}
	}
}
