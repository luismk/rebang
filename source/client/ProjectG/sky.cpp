// Original: d:\build\custom\temp\code\client\projectg\sky.cpp
#include "sky.h"
#include "commonutil.h"
#include "FxBox.h"
#include "fx.h"
#include "fxspray.h"
#include "fxsequence.h"
#include "GolfDoc.h"
#include "LensFlare.h"
#include "polysoup.h"
#include "SharedDoc.h"
#include "wresrcmng.h"
#include "wview.h"
#include "wind.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

float Random(float min, float max, int seed);  // commonutil.cpp

// The batch flags of a sky texture: the handle in the low 11 bits, then the
// render flags of the sky layers (their Wangreal names are not in the PDB).
#define SKY_TEX_FLAG(tex) (((tex) & 0x7FF) | 0x22300000)

extern WMatrix g_camera;

// sky.cpp:3
sCloud::sCloud(void) {
}

// sky.cpp:94
CSky::CSky(void) : m_lensFlare(0), m_fxBox(0), m_pvsFxBox(0) {
	m_cloudTex = 0;

	m_cloudCount = 0;
	m_cloudGenCount = 0;
	m_cloudInterval = 0;

	m_cloudTimer = 0;

	m_rotateSpeed = 0;

	m_loadParam = 0; m_cloudRadius = 1120.0f; m_cloudFadeRange = 1600.0f; m_cloudHeight = 416.0f; m_skyColor = 0xffffffff;
	m_texColumns = 2;
	m_texRows = 2;

	memset(m_farTex, 0, sizeof(m_farTex));
	memset(m_nearTex, 0, sizeof(m_nearTex));
	memset(m_upTex, 0, sizeof(m_upTex));
	memset(m_dnTex, 0, sizeof(m_dnTex));

	m_loaded = 0;

	m_upAndBottomAngle = 0;
	m_nearCylinderAngle = 0;
}

// sky.cpp:126
CSky::~CSky(void) {
	Clear();
}

// sky.cpp:173
void CSky::LoadTexByWeather(const char* name, const char* layer) {
	char path[128] = "";
	char altPath[256] = "";
	int tex;
	int i;

	sprintf(path, "%s_%s.jpg", name, layer);
	tex = g_resrcmng->LoadTexture(path, 0x80000000, 0, 0);
	if (!tex) {
		sprintf(altPath, "[%s", path);
		tex = g_resrcmng->LoadTexture(altPath, 0x80000000, 0, 0);
	}
	if (tex) {
		if (strcmp(layer, "far") == 0) {
			m_farTex[0] = tex;
		} else if (strcmp(layer, "near") == 0) {
			m_nearTex[0] = tex;
		} else if (strcmp(layer, "up") == 0) {
			m_upTex[0] = tex;
		} else if (strcmp(layer, "dn") == 0) {
			m_dnTex[0] = tex;
		}
	}

	for (i = tex != 0; i < 5; i++) {
		sprintf(path, "%s_%s_%d.jpg", name, layer, i);
		tex = g_resrcmng->LoadTexture(path, 0x80000000, 0, 0);
		if (strcmp(layer, "far") == 0) {
			m_farTex[i] = tex;
		} else if (strcmp(layer, "near") == 0) {
			m_nearTex[i] = tex;
		} else if (strcmp(layer, "up") == 0) {
			m_upTex[i] = tex;
			return;
		} else if (strcmp(layer, "dn") == 0) {
			m_dnTex[i] = tex;
			return;
		}
	}
}

// sky.cpp:257
int CSky::IsThereTexHandle(const char* name, int index) {
	int* handle;
	if (strcmp(name, "far") == 0) {
		handle = &m_farTex[index];
	} else if (strcmp(name, "near") == 0) {
		handle = &m_nearTex[index];
	} else if (strcmp(name, "up") == 0) {
		if (index == 0) {
			handle = &m_upTex[0];
		} else {
			handle = &m_upTex[1];
		}
	} else if (strcmp(name, "dn") == 0) {
		if (index == 0) {
			handle = &m_dnTex[0];
		} else {
			handle = &m_dnTex[1];
		}
	} else {
		return -1;
	}
	return *handle;
}

// sky.cpp:298
void CSky::LoadSky(const char* name, float param) {
	if (m_loaded) {
		Clear();
	}

	// texture divisions of each layer: far, near, up, down
	int divs[4];
	divs[0] = 2; divs[1] = 2; divs[2] = 1; divs[3] = 1;
	m_loaded = 1;
	LoadTexByWeather(name, "far");

	int tex = IsThereTexHandle("far", 0);
	if (tex) {
		divs[0] = 1;
		m_batch[0].xiFlag0 = SKY_TEX_FLAG(tex);
	} else {
		m_batch[0].xiFlag0 = 0;
	}

	LoadTexByWeather(name, "near");
	tex = IsThereTexHandle("near", 0);
	if (tex) {
		divs[1] = 1;
		m_batch[1].xiFlag0 = SKY_TEX_FLAG(tex);
	} else {
		m_batch[1].xiFlag0 = 0;
	}

	LoadTexByWeather(name, "up");
	tex = IsThereTexHandle("up", 0);
	if (tex) {
		m_batch[2].xiFlag0 = SKY_TEX_FLAG(tex);
	} else {
		m_batch[2].xiFlag0 = 0;
	}

	LoadTexByWeather(name, "dn");
	tex = IsThereTexHandle("dn", 0);
	if (tex) {
		m_batch[3].xiFlag0 = SKY_TEX_FLAG(tex);
	} else {
		m_batch[3].xiFlag0 = 0;
	}

	m_loadParam = param; m_texColumns = divs[0] < 1 ? 1 : divs[0];
	m_texRows = divs[1] < 1 ? 1 : divs[1];

	float turn[2];
	turn[0] = param / divs[0];
	WVideoDev* video = g_resrcmng->VideoReference();
	turn[1] = param / divs[1];

	for (int layer = 0; layer < 4; layer++) {
		WxBatchState& batch = m_batch[layer];
		if (!(batch.xiFlag0 & 0x7FF)) {
			continue;
		}

		batch.xiFlag1 = 5;
		unsigned long fvf = video->xDetermineFVF(batch.xiFlag0, 5, 0);
		unsigned long usage = video->xDetermineBufferUsage(fvf) | 8;
		int stride = video->VertexSize(fvf);

		int numVerts;
		int numIdxs;
		int bandBytes;
		int rows;
		int cols;
		float radius;
		float ys[3];
		float vs[3];
		if (layer <= 1) {
			// cylinder: three bands of (divs + 32) vertices
			numVerts = (divs[layer] + 32) * 3;
			bandBytes = (divs[layer] + 32) * stride;
			numIdxs = divs[layer] * 4 + 130;
			radius = layer ? 400.0f : 500.0f;

			rows = divs[layer];
			cols = 32 / divs[layer] + 1;
			ys[0] = radius * 0.785398185f;
			ys[1] = radius * 0.628318548f;
			ys[2] = radius * -0.628318548f;
			vs[0] = 0.0f;
			vs[1] = 0.1f;
			vs[2] = 1.0f;
		} else {
			// up or down plane: one quad
			batch.xiFlag1 |= 0x400;
			numVerts = numIdxs = 4;
			bandBytes = stride * 2;
			ys[0] = (layer == 2 ? 1.0f : -1.0f) * 196.349548f;

			rows = divs[layer];
			cols = 2 / divs[layer];
			vs[0] = 0.0f;
			vs[1] = 1.0f;
		}

		int vb = video->xCreateVertexBuffer(numVerts, fvf, usage);
		int ib = video->xCreateIndexBuffer(numIdxs, usage);

		unsigned long color = ModifyColor(m_skyColor, GOLFDOC()->m_fogBrightness);
		if (layer) {
			color |= 0xFF000000;
		}
		unsigned long colors[3];
		colors[0] = color;
		colors[1] = colors[2] = color | 0xFF000000;

		unsigned char* vtx = video->xLockVertexBuffer(vb, 0, 0);
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < cols; col++) {
				if (layer <= 1) {
					int step = (cols - 1) * row + col;

					int base = (row * cols + col) * stride;
					float angle = (step * 0.03125f - turn[layer]) * 6.28318548f;
					float x = (float)(cos(angle) * radius);
					float z = (float)(sin(angle) * radius);
					float u = 1.0f - (rows * col) * 0.03125f;
					int off = base;
					*(float*)(vtx + off) = x; off += 4;
					*(float*)(vtx + off) = ys[0]; off += 4;
					*(float*)(vtx + off) = z; off += 4;
					*(unsigned int*)(vtx + off) = colors[0]; off += 4;
					*(float*)(vtx + off) = u; off += 4;
					*(float*)(vtx + off) = vs[0];

					off = base + bandBytes;
					*(float*)(vtx + off) = x; off += 4;
					*(float*)(vtx + off) = ys[1]; off += 4;
					*(float*)(vtx + off) = z; off += 4;
					*(unsigned int*)(vtx + off) = colors[1]; off += 4;
					*(float*)(vtx + off) = u; off += 4;
					*(float*)(vtx + off) = vs[1];

					off = base + bandBytes * 2;
					*(float*)(vtx + off) = x; off += 4;
					*(float*)(vtx + off) = ys[2]; off += 4;
					*(float*)(vtx + off) = z; off += 4;
					*(unsigned int*)(vtx + off) = colors[2]; off += 4;
					*(float*)(vtx + off) = u; off += 4;
					*(float*)(vtx + off) = vs[2];
				} else {
					float u = 0.0f;
					if (col % 2 == 0) {
						u = 1.0f;
					}

					int base = (row * cols + col) * stride;
					for (int j = 0; j < 2; j++) {
						int off = base + bandBytes * j;
						float x = (col ? 1.0f : -1.0f) * 500.0f;
						float z = j ? -500.0f : 500.0f;
						*(float*)(vtx + off) = x; off += 4;
						*(float*)(vtx + off) = ys[0]; off += 4;
						*(float*)(vtx + off) = z; off += 4;
						*(unsigned int*)(vtx + off) = colors[0]; off += 4;
						*(float*)(vtx + off) = u; off += 4;
						*(float*)(vtx + off) = vs[j];
					}
				}
			}
		}
		video->xUnlockVertexBuffer(vb);

		unsigned short* idx = (unsigned short*)video->xLockIndexBuffer(ib, 0, 0);
		int count;
		if (layer <= 1) {
			// a strip per band pair, joined by a degenerate triangle
			count = 0;
			for (int band = 0; band < 2; band++) {
				for (int row = 0; row < rows; row++) {
					for (int col = 0; col < cols; col++) {
						unsigned short top = (unsigned short)(band * (divs[layer] + 32) + row * cols + col);
						idx[count] = top;
						idx[count + 1] = (unsigned short)(top + divs[layer] + 32);
						count += 2;
					}
				}
				if (band < 1) {
					idx[count] = idx[count - 1];
					idx[count + 1] = (unsigned short)(divs[layer] + 32);
					count += 2;
				}
			}
		} else {
			idx[0] = 0;
			idx[1] = 2; idx[2] = 1; idx[3] = 3;
			count = 4;
		}
		video->xUnlockIndexBuffer(ib);

		batch.xnmTransfs = 0;
		batch.xiBaseVtxIdx = 0; batch.xnVtxs = numVerts;
		batch.xiBaseIdxIdx = 0; batch.xnIdxs = count;
		batch.xiFlag1 |= ((vb << 8) | (ib & 0xFF)) << 14;
	}
}

// sky.cpp:588
void CSky::SetSkyTex(int texhandle, unsigned char layer, unsigned char batch) {
	if (texhandle) {
		m_batch[batch].xiFlag0 = SKY_TEX_FLAG(texhandle);
	} else {
		m_batch[batch].xiFlag0 = 0;
	}
}

// sky.cpp:619
void CSky::ChangeWeatherTex(unsigned char index) {
	int farTex = IsThereTexHandle("far", index);
	int nearTex = IsThereTexHandle("near", index);
	if (farTex == 0 && nearTex == 0) {
		return;
	}
	if (farTex) { m_batch[0].xiFlag0 = SKY_TEX_FLAG(farTex); } else { m_batch[0].xiFlag0 = 0; }
	if (nearTex) { m_batch[1].xiFlag0 = SKY_TEX_FLAG(nearTex); } else { m_batch[1].xiFlag0 = 0; }

	int upTex = IsThereTexHandle("up", index);
	if (upTex) { m_batch[2].xiFlag0 = SKY_TEX_FLAG(upTex); } else { m_batch[2].xiFlag0 = 0; }

	int dnTex = IsThereTexHandle("dn", index);
	if (dnTex) { m_batch[3].xiFlag0 = SKY_TEX_FLAG(dnTex); } else { m_batch[3].xiFlag0 = 0; }
}

// sky.cpp:641
void CSky::ClearClouds(void) {
	if (g_resrcmng && m_cloudTex) { g_resrcmng->Release(m_cloudTex); m_cloudTex = 0; }

	m_cloudCount = 0;
	m_cloudGenCount = 0;
}

// sky.cpp:648
void CSky::RegisterCloud(char* name) {
	ClearClouds();
	m_cloudTex = g_resrcmng->LoadTexture(name, 0, 0, 0);
}

// sky.cpp:654
void CSky::ShowClouds(bool show) {
	if (show == true) {
		for (int i = 0; i < m_cloudCount; i++) {
			m_clouds[i].m_hide = rand() % 2 == 0;
		}
	} else {
		for (int i = 0; i < m_cloudCount; i++) {
			m_clouds[i].m_hide = true;
		}
	}
}

// sky.cpp:668
void CSky::GenerateClouds(int count, float density) {
	if (count > 50) {
		count = 50;
	}

	m_cloudCount = count;
	if (density < 1.0f) {
		m_cloudInterval = 10.0f;
	} else {
		m_cloudInterval = 9.0f / density + 1.0f;
	}

	for (int i = 0; i < count; i++) {
		m_clouds[i].m_size = Random(1.0f, 2.0f, -1) * 256.0f;
		m_clouds[i].m_texIndex = rand() % 4;
		m_clouds[i].m_rotSpeed = Random(-0.1f, 0.1f, -1);
		m_clouds[i].m_flip = rand() % 2 != 0;

		if (rand() % 2 == 0) {
			m_clouds[i].m_pos = WVector(Random(-1.0f, 1.0f, -1), 0.0f, Random(-1.0f, 1.0f, -1)) * m_cloudRadius;
			m_clouds[i].m_pos.y = m_cloudHeight;
			m_clouds[i].m_hide = true;
			m_cloudGenCount++;
		} else {
			m_clouds[i].m_pos.y = m_cloudHeight;
			m_clouds[i].m_hide = false;
		}
	}
}

// sky.cpp:703
void CSky::GenerateStars(int count) {
	if (m_fxBox) {
		delete m_fxBox;
		m_fxBox = 0;
	}

	int map = count;
	if (map == -1) {
		map = GetCurMap();
	}
	if (map > 0x7F && map != 0xFD) {
		map -= 0x80;
	}

	IFF_STRUCT::sCourse* course = ItemManager()->FindCourse(0x28000000 | map);
	if (!course || !course->m_skyEffectFile[0]) {
		return;
	}

	const char* name = course->m_skyEffectFile;
	if (strlen(name) < 5) {
		return;
	}

	// sky.cpp:720
	const char* ext = strrchr(name, '.');
	if (!ext || strlen(ext) < 4) {
		return;
	}

	if (!m_fxBox) {
		m_fxBox = new CPvsFxBox;
		m_fxBox->Init(g_resrcmng);
		m_fxBox->SetCamera(&g_camera);
		m_fxBox->m_frameTime = 33.333f;
	}

	// sky.cpp:735
	if (stricmp(ext + 1, "spr") == 0) {
		CFxSpray* pSpray = m_fxBox->LoadSpray(name, NULL);
		if (!pSpray) {
			return;
		}

		Attacher attacher;
		attacher.m_pOffset[0] = &g_view->camera.pivot;

		pSpray->m_pAttacher = &attacher;
		pSpray->m_fAttachLimitA = attacher.m_fLimitA;
		pSpray->m_fAttachLimitB = attacher.m_fLimitB;
		memcpy(pSpray->m_pMatrix, attacher.m_pMatrix, sizeof(pSpray->m_pMatrix));
		memcpy(pSpray->m_pOffset, attacher.m_pOffset, sizeof(pSpray->m_pOffset));
		m_fxBox->AddSpray(pSpray);
	} else if (stricmp(ext + 1, "seq") == 0) {
		// sky.cpp:759
		CFxSequence* pSequence = m_fxBox->LoadSequence(name, true, NULL);
		if (!pSequence) {
			return;
		}

		Attacher attacher;
		attacher.m_pOffset[0] = &g_view->camera.pivot;

		pSequence->m_pAttacher = &attacher;
		pSequence->m_fDistLimitA = attacher.m_fLimitA;
		pSequence->m_fDistLimitB = attacher.m_fLimitB;
		m_fxBox->AddSequence(pSequence);

		m_fxBox->Process(m_fxBox->m_frameTime);

		std::map<CFxSpray*, CFxSpray*>::const_iterator it = pSequence->m_sprays.begin();
		for (; it != pSequence->m_sprays.end(); ++it) {
			CFxSpray* pSpray = it->second;
			pSpray->m_pAttacher = &attacher;
			pSpray->m_fAttachLimitA = attacher.m_fLimitA;
			pSpray->m_fAttachLimitB = attacher.m_fLimitB;
			memcpy(pSpray->m_pMatrix, attacher.m_pMatrix, sizeof(pSpray->m_pMatrix));
			memcpy(pSpray->m_pOffset, attacher.m_pOffset, sizeof(pSpray->m_pOffset));
			m_fxBox->AddSpray(pSpray);
		}

		m_fxBox->DeleteSequence(pSequence, false);
	}
}

// sky.cpp:799
void CSky::GenerateMoons(void) {
	if (m_pvsFxBox) {
		delete m_pvsFxBox;
		m_pvsFxBox = 0;
	}

	WPolySoup* soup = GOLFDOC()->m_pPolySoup;
	int count = soup->m_numPoints;
	WPolySoup::sPoint* point = soup->m_pPoints;

	for (int i = 0; i < count; i++, point++) {
		// a moon
		if (point->m_type != 3) {
			continue;
		}

		const char* name = point->m_desc.c_str();
		const char* ext = strrchr(name, '.');
		if (!ext || strlen(ext) < 4) {
			continue;
		}

		if (!m_pvsFxBox) {
			m_pvsFxBox = new CPvsFxBox;
			m_pvsFxBox->Init(g_resrcmng);
			m_pvsFxBox->SetCamera(&g_camera);
			m_pvsFxBox->m_frameTime = 33.333f;
		}

		// sky.cpp:829
		if (stricmp(ext + 1, "spr") == 0) {
			CFxSpray* pSpray = m_pvsFxBox->LoadSpray(name, NULL);
			if (!pSpray) {
				continue;
			}

			Attacher attacher;
			attacher.m_pOffset[0] = &g_view->camera.pivot;

			pSpray->m_pAttacher = &attacher;
			pSpray->m_fAttachLimitA = attacher.m_fLimitA;
			pSpray->m_fAttachLimitB = attacher.m_fLimitB;
			WVector dir = point->m_pos;
			dir.Normalize();
			pSpray->m_spawnDir.x = dir.x * 50.0f;
			pSpray->m_spawnDir.y = dir.y * 50.0f;
			pSpray->m_spawnDir.z = dir.z * 50.0f;
			memcpy(pSpray->m_pMatrix, attacher.m_pMatrix, sizeof(pSpray->m_pMatrix));
			memcpy(pSpray->m_pOffset, attacher.m_pOffset, sizeof(pSpray->m_pOffset));
			m_pvsFxBox->AddSpray(pSpray);
		} else if (stricmp(ext + 1, "seq") == 0) {
			// sky.cpp:854
			CFxSequence* pSequence = m_pvsFxBox->LoadSequence(name, true, NULL);
			if (!pSequence) {
				continue;
			}

			Attacher attacher;
			attacher.m_pOffset[0] = &g_view->camera.pivot;

			pSequence->m_pAttacher = &attacher;
			pSequence->m_fDistLimitA = attacher.m_fLimitA;
			pSequence->m_fDistLimitB = attacher.m_fLimitB;
			m_pvsFxBox->AddSequence(pSequence);

			m_pvsFxBox->Process(m_pvsFxBox->m_frameTime);

			// sky.cpp:875
			std::map<CFxSpray*, CFxSpray*>::const_iterator it = pSequence->m_sprays.begin();
			for (; it != pSequence->m_sprays.end(); ++it) {
				CFxSpray* pSpray = it->second;
				pSpray->m_pAttacher = &attacher;
				pSpray->m_fAttachLimitA = attacher.m_fLimitA;
				pSpray->m_fAttachLimitB = attacher.m_fLimitB;
				WVector dir = point->m_pos;
				dir.Normalize();
				pSpray->m_spawnDir.x = dir.x * 50.0f + pSpray->m_pos.x;
				pSpray->m_spawnDir.y = dir.y * 50.0f + pSpray->m_pos.y;
				pSpray->m_spawnDir.z = dir.z * 50.0f + pSpray->m_pos.z;
				memcpy(pSpray->m_pMatrix, attacher.m_pMatrix, sizeof(pSpray->m_pMatrix));
				memcpy(pSpray->m_pOffset, attacher.m_pOffset, sizeof(pSpray->m_pOffset));
				m_pvsFxBox->AddSpray(pSpray);
			}

			m_pvsFxBox->DeleteSequence(pSequence, false);
		}
	}
}

// sky.cpp:897
void CSky::GenerateLensFlare(void) {
	if (m_lensFlare) { delete m_lensFlare; m_lensFlare = 0; }

	int count = GOLFDOC()->m_pPolySoup->m_numPoints;
	WPolySoup::sPoint* point = GOLFDOC()->m_pPolySoup->m_pPoints;

	for (int i = 0; i < count; i++, point++) {
		// the sun
		if (point && point->m_type == 2) {
			if (!m_lensFlare) {
				m_lensFlare = new CLensFlare;
			}
			m_lensFlare->Init(point->m_pos);
			return;
		}
	}
}

// sky.cpp:926
void CSky::ClearSky(void) {
	switch (GetCurMap()) {
	case 5:
	case 6:
		m_fxBox->Clear();
		m_pvsFxBox->Clear();
		if (m_fxBox) {
			delete m_fxBox;
			m_fxBox = 0;
		}
		if (m_pvsFxBox) {
			delete m_pvsFxBox;
			m_pvsFxBox = 0;
		}
		break;
	case 7:
		m_pvsFxBox->Clear();
		if (m_pvsFxBox) {
			delete m_pvsFxBox;
			m_pvsFxBox = 0;
		}
		break;
	}
}

// sky.cpp:949
void CSky::Process(float elapsed) {
	m_cloudTimer += elapsed;
	if (m_cloudTimer > m_cloudInterval) {
		m_cloudTimer -= m_cloudInterval;

		if (m_cloudGenCount < m_cloudCount) {
			int i = 0;
			for (; i < m_cloudCount; i++) {
				if (!m_clouds[i].m_hide) {
					break;
				}
			}
			if (i < m_cloudCount) {
				float r = Random(-1.0f, 1.0f, -1);
				WVector spawn = WVector(m_cloudRadius, 0.0f, -1.0f) * r;
				m_clouds[i].m_pos = spawn * RotMat(Wind().GetGlobalDirection(), 1);
				m_clouds[i].m_pos.y = m_cloudHeight;
				m_clouds[i].m_hide = true;
				m_clouds[i].m_flip = rand() % 2 != 0;
				m_cloudGenCount++;
			}
		}
	}

	// sky.cpp:977
	for (int i = 0; i < m_cloudCount; i++) {
		sCloud& cloud = m_clouds[i];
		if (!cloud.m_hide) {
			continue;
		}

		// The drift: faster for the clouds that turn faster.
		float speed = (cloud.m_rotSpeed + 1.0f) * 3.0f;
		WVector wind = Wind().GetGlobalWind();
		cloud.m_pos.x += wind.x * speed * elapsed;
		cloud.m_pos.y += wind.y * speed * elapsed;
		cloud.m_pos.z += wind.z * speed * elapsed;

		// The alpha fades out toward the edge of the camera's view range.
		float dx = cloud.m_pos.x - g_camera.pivot.x;
		float dz = cloud.m_pos.z - g_camera.pivot.z;
		float t = 1.0f - sqrtf(dx * dx + dz * dz) / m_cloudFadeRange;
		if (t < 0.0f) {
			t = 0.0f;
		}
		if (t > 0.5f) {
			cloud.m_alpha = 255.0f;
		} else {
			cloud.m_alpha = t * 2.0f * 255.0f;
		}

		// And toward the course's origin.
		WVector diff = cloud.m_pos - GOLFDOC()->m_pPolySoup->m_pBgModel->m_basePos;
		float dist = WVectorLen(diff);
		t = 1.0f - dist / m_cloudRadius;
		if (t < 0.0f) {
			t = 0.0f;
		}
		if (t < 0.1f) {
			cloud.m_alpha *= t * 10.0f;
		}

		if (dist > (float)sqrt(2.0) * m_cloudRadius) {
			cloud.m_hide = false;
			m_cloudGenCount--;
		}
	}

	// sky.cpp:1010
	if (m_fxBox) {
		m_fxBox->Process(elapsed * 1000.0f);
	}
	if (m_pvsFxBox) {
		m_pvsFxBox->Process(elapsed * 1000.0f);
	}
	if (m_lensFlare) {
		m_lensFlare->Process(elapsed);
	}

	// sky.cpp:1023
	// The near cylinder turns with the wind seen along the camera's view.
	WVector wind = Wind().GetGlobalWind();
	WMatrix rot;
	g_camera.GetRotMatrix(&rot);
	WVector view;
	view.x = (rot.yx + rot.zx) * 0.0f - rot.xx * 1.0f + rot.pivot.x;
	view.y = (rot.yy + rot.zy) * 0.0f - rot.xy * 1.0f + rot.pivot.y;
	view.z = (rot.yz + rot.zz) * 0.0f - rot.xz * 1.0f + rot.pivot.z;
	view.Normalize();
	wind.Normalize();
	float along = wind.x * view.x + wind.y * view.y + wind.z * view.z;

	m_nearCylinderAngle += WVectorLen(Wind().GetGlobalWind()) * along * elapsed * 0.005f;
	if (fabsf(m_nearCylinderAngle) >= 360.0f) {
		m_nearCylinderAngle = 0.0f;
	}

	// sky.cpp:1036
	m_upAndBottomAngle += elapsed * 0.01f;
	if (m_upAndBottomAngle >= 360.0f) {
		m_upAndBottomAngle = 0.0f;
	}
}

// sky.cpp:1045
void CSky::Render(WView* view) {
	float oldNear = view->GetClipNearValue();
	float oldFar = view->GetClipFarValue();

	view->SetClip(1.0f, CalcClipFarByView(view), false);
	view->UpdateCamera();

	m_viewPos = view->camera.pivot;
	m_viewPos.y += m_rotateSpeed;

	RenderUpAndBottom(view);

	unsigned long gameMode = GOLFDOC()->GameMode();
	if (gameMode != 0x200 || view->GetReflective()) {
		RenderFarCylinder(view);

		if (m_fxBox) {
			std::map<CFxSpray*, CFxSpray*>::iterator it = m_fxBox->m_sprays.begin();
			for (; it != m_fxBox->m_sprays.end(); ++it) {
				it->second->m_pos = g_view->camera.pivot;
			}
			m_fxBox->Display(g_view);
			g_view->Flush(0);
		}

		if (m_pvsFxBox) {
			std::map<CFxSpray*, CFxSpray*>::iterator it = m_pvsFxBox->m_sprays.begin();
			for (; it != m_pvsFxBox->m_sprays.end(); ++it) {
				CFxSpray* spray = it->second;
				spray->m_pos = spray->m_spawnDir + g_view->camera.pivot;
			}
			m_pvsFxBox->Display(g_view);
			g_view->Flush(0);
		}

		if (m_lensFlare && !GetWeather()) {
			m_lensFlare->Render(view);
		}

		RenderNearCylinder(view);

		gameMode = GOLFDOC()->GameMode();
		if (gameMode != 0x200000) {
			if (gameMode & 0x200) {
				RenderFlatCloud(view);
			} else {
				RenderBillboardCloud(view);
			}
		}
	}

	view->SetClip(oldNear, oldFar, false);
	view->UpdateCamera();
}

// sky.cpp:1115
void CSky::RenderUpAndBottom(WView* view) {
	if (m_batch[2].xiFlag0 & 0x7FF) {
		WMatrix mat;
		mat.Reset(); mat.Rotate(m_upAndBottomAngle, 1);

		mat *= CalcScaleByView(view);
		mat.pivot = m_viewPos;

		m_batch[2].xmW = mat;

		view->xDrawIndexedTriangles(m_batch[2]);
	}

	if (m_batch[3].xiFlag0 & 0x7FF) {
		WMatrix mat;
		mat.Reset(); mat *= CalcScaleByView(view);
		mat.pivot = m_viewPos;

		m_batch[3].xmW = mat;

		view->xDrawIndexedTriangles(m_batch[3]);
	}
}

// sky.cpp:1173
void CSky::RenderBillboardCloud(WView* view) {
	WTVertex verts[4];
	WTVertex* vl[4];
	vl[0] = &verts[0];
	vl[1] = &verts[1];
	vl[2] = &verts[3];
	vl[3] = &verts[2];

	for (int i = 0; i < m_cloudCount; i++) {
		sCloud& cloud = m_clouds[i];
		if (cloud.m_hide) {
			continue;
		}
		for (int k = 0; k < 4; k++) {
			float sx = (k & 1) ? 0.25f : -0.25f;
			float sy = (k & 2) ? 0.5f : -0.5f;
			WVector d;
			d.x = sy * view->camera.xa.x + sx * view->camera.ya.x + view->camera.za.x * 0.0f;
			d.y = sy * view->camera.xa.y + sx * view->camera.ya.y + view->camera.za.y * 0.0f;
			d.z = sy * view->camera.xa.z + sx * view->camera.ya.z + view->camera.za.z * 0.0f;
			verts[k].x = d.x * cloud.m_size + cloud.m_pos.x;
			verts[k].y = d.y * cloud.m_size + cloud.m_pos.y;
			verts[k].z = d.z * cloud.m_size + cloud.m_pos.z;
			if (cloud.m_flip) {
				verts[k].tu = (k & 2) ? 0.0f : 1.0f;
			} else {
				verts[k].tu = (k & 2) ? 1.0f : 0.0f;
			}
			verts[k].tv = (float)(((~k) & 1) + cloud.m_texIndex) * 0.25f;
			verts[k].diffuse = ModifyColor(m_skyColor, WSingleton<CSharedDoc>::Instance()->m_pGolfDoc->m_fogBrightness)
				| ((unsigned long)(unsigned char)(int)cloud.m_alpha << 24);
		}
		view->DrawPolygonFan(vl, (m_cloudTex & 0x7FF) | 0x20300000, 0x400, false);
	}
}

// sky.cpp:1156
void CSky::RenderFarCylinder(WView* view) {
	if (!(m_batch[0].xiFlag0 & 0x7FF)) {
		return;
	}

	WMatrix mat;
	mat.Reset(); mat *= CalcScaleByView(view);
	mat.pivot = m_viewPos;

	m_batch[0].xmW = mat;

	view->xDrawIndexedTriangles(m_batch[0]);
}

// sky.cpp:1217
void CSky::RenderFlatCloud(WView* view) {
	WTVertex verts[4];
	WTVertex* vl[4];
	vl[0] = &verts[0];
	vl[1] = &verts[1];
	vl[2] = &verts[3];
	vl[3] = &verts[2];

	for (int i = 0; i < m_cloudCount; i++) {
		sCloud& cloud = m_clouds[i];
		if (cloud.m_hide) {
			continue;
		}
		for (int k = 0; k < 4; k++) {
			float sx = (k & 1) ? 0.25f : -0.25f;
			float sy = (k & 2) ? 0.5f : -0.5f;
			verts[k].x = sy * cloud.m_size + cloud.m_pos.x;
			verts[k].y = cloud.m_size * 0.0f + cloud.m_pos.y;
			verts[k].z = sx * cloud.m_size + cloud.m_pos.z;
			if (cloud.m_flip) {
				verts[k].tu = (k & 2) ? 0.0f : 1.0f;
			} else {
				verts[k].tu = (k & 2) ? 1.0f : 0.0f;
			}
			verts[k].tv = (float)(((~k) & 1) + cloud.m_texIndex) * 0.25f;
			verts[k].diffuse = ModifyColor(m_skyColor, WSingleton<CSharedDoc>::Instance()->m_pGolfDoc->m_fogBrightness)
				| 0xFF000000;
		}
		view->DrawPolygonFan(vl, (m_cloudTex & 0x7FF) | 0x20300000, 0x400, false);
	}
}

// sky.cpp:1261
void CSky::RenderNearCylinder(WView* view) {
	if (!(m_batch[1].xiFlag0 & 0x7FF)) {
		return;
	}

	WMatrix mat;
	mat.Reset(); mat.Rotate(m_nearCylinderAngle, 1);

	mat *= CalcScaleByView(view);
	mat.pivot = m_viewPos;

	m_batch[1].xmW = mat;

	view->xDrawIndexedTriangles(m_batch[1]);
}

// sky.cpp:1281
void CSky::UpdateBright(void) {
}

// sky.cpp:1285
float CSky::CalcScaleByView(WView* view) const {
	if (view->GetProjectionMode() == WView::PARALLEL) {
		float scale = view->GetScale() * 2.0f; return scale * 0.002f;
	}
	return 1.0f;
}

// sky.cpp:1293
float CSky::CalcClipFarByView(WView* view) const {
	if (view->GetProjectionMode() == WView::PARALLEL) {
		return CalcScaleByView(view) * 1000.0f;
	}
	return view->GetClipFarValue();
}
