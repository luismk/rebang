// Original: d:\build\custom\temp\code\client\projectg\scenemanager.cpp
#include "scenemanager.h"

#include <algorithm>
#include <math.h>

#include "sea.h"
#include "weather.h"
#include "wresrcmng.h"
#include "wview.h"
#include "wutil.h"
#include "wpuppet.h"
#include "wbone.h"
#include "obmarker.h"
#include "GolfDoc.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "pool.h"
#include "river.h"
#include "wavelet.h"
#include "beach.h"
#include "westpak.h"
#include "../shared/classdefine.h"
#include "../shared/Localize.h"
#include "Shadow.h"
#include "ClientSetting.h"
#include "polysoup.h"
#include "sky.h"
#include "AutoTarget.h"
#include "golfball.h"
#include "quadtree.h"
#include "wfont.h"
#include "ChatMsg.h"
#include "snow.h"
#include "lobbybg.h"
#include "Petal.h"

void __cdecl CloseCFile(cFile* file);

extern int g_nDrawModel;
extern int g_nDrawPoly;
extern int g_nModel;
extern LightSet g_lightset;

WTVertex CSceneManager::vtr[4];
WTVertex* CSceneManager::vtx[5] = { &CSceneManager::vtr[0], &CSceneManager::vtr[1], &CSceneManager::vtr[3], &CSceneManager::vtr[2], 0 };

// wmath.inl (emitted out of line in SharedDoc; not declared in wmath.h yet).
float WVectorLen(const WVector& v);

// Wangreal render-to-texture types (wtextureview.h, wvideo.h); only what this
// unit uses is declared.
struct WRenderToTextureSizeInfo {
	bool m_isAbsolute;                     // 0x0
	float m_width;                         // 0x4
	float m_height;                        // 0x8

	static const WRenderToTextureSizeInfo SIZE_ABS;
};
CHECK_SIZE(WRenderToTextureSizeInfo, 0xC);

struct WRenderToTextureParam {
	struct DepthSurfInfo {
		enum SurfaceUsage {
		};
	};
};

class WTextureView : public WView {
public:
	struct TextureParam {
		struct RtTexInfo {
			unsigned long m_texStyle;              // 0x00
			WRenderToTextureSizeInfo m_sizeInfo;   // 0x04
			bool m_needToClear;                    // 0x10
			unsigned long m_clearClr;              // 0x14

			RtTexInfo(void);
			// wtextureview.h:35
			void Set(unsigned long texStyle, const WRenderToTextureSizeInfo& sizeInfo, bool needToClear, unsigned long clearClr) {
				m_texStyle = texStyle;
				m_sizeInfo = sizeInfo;
				m_needToClear = needToClear;
				m_clearClr = clearClr;
			}
		};
		struct DepthSurfInfo {
			WRenderToTextureParam::DepthSurfInfo::SurfaceUsage m_surfUsage;  // 0x00
			WRenderToTextureSizeInfo m_sizeInfo;   // 0x04
			bool m_needToClear;                    // 0x10
			float m_clearZ;                        // 0x14

			DepthSurfInfo(void);
			void Set(WRenderToTextureParam::DepthSurfInfo::SurfaceUsage surfUsage, const WRenderToTextureSizeInfo& sizeInfo, bool needToClear, float clearZ) {
				m_surfUsage = surfUsage;
				m_sizeInfo = sizeInfo;
				m_needToClear = needToClear;
				m_clearZ = clearZ;
			}
		};

		unsigned int m_numRts;                 // 0x00
		RtTexInfo m_rtTexInfo[2];              // 0x04
		DepthSurfInfo m_depthSurfInfo;         // 0x34

		TextureParam(void);
	};

	virtual ~WTextureView(void);
	void SetTextureParam(const TextureParam& param);
	void Begin(void);
	void End(void);

protected:
	TextureParam m_texParam;               // 0x1b1c
	UNKNOWN(0x1b68, 0x1b90);               // WRenderToTextureParam m_r2tParam
	bool m_r2tBegun;                       // 0x1b90
};
CHECK_SIZE(WTextureView::TextureParam, 0x4C);
CHECK_SIZE(WTextureView, 0x1B94);

class WTextureImposterView : public WTextureView {
public:
	WTextureImposterView(WPuppet* target);
	virtual ~WTextureImposterView(void);
	bool NeedToUpdate(const WMatrix& camera);
	// First dword of m_r2tParam (0x1b68), the render-target texture handle.
	unsigned long GetFirstTexture(void) const { return *(const unsigned long*)((const char*)this + 0x1b68); }

private:
	const WPuppet* m_target;               // 0x1b94
	float m_tolerance;                     // 0x1b98
};
CHECK_SIZE(WTextureImposterView, 0x1B9C);

// scenemanager.cpp:60
CSceneManager::CSceneManager(void)
	: m_beachList(16, 16), m_waveletList(16, 16), m_poolList(16, 16), m_riverList(16, 16),
	  m_pSea(NULL), m_pMarker(NULL), m_pWeather(NULL), m_pLobbyWeather(NULL),
	  m_useFog(true), m_useImposter(false), m_skipTransparent(false),
	  m_drawElementBoxes(false), m_pFont(NULL), m_overlapCamera(true), m_noOverlapCamera(false), m_debugDraw(0) {
	m_pMarker = new CMarker;

	// The device tells whether it can draw transparent elements.
	int caps;
	g_resrcmng->VideoReference()->Command(WX_VDEV_GET_CAPS, 1, (intptr_t)&caps);

	if (caps)
		m_skipTransparent = true;

	m_useImposter = COption::Instance()->vIsLodEnabled();
	m_pFont = CChatMsg::Instance()->GetMaskedFont();
}

// scenemanager.cpp:100
void CSceneManager::RegisterElement(eObjType type, WPuppet* pPuppet, CRenderFuncPtr* pRender, const char* name, unsigned long flags, Waabb* pBox) {
	SceneElement elem;
	elem.m_type = type;
	elem.m_pPuppet = pPuppet;
	elem.m_pRender = pRender;
	elem.m_pBox = pBox;
	if (name) {
		strcpy(elem.m_name, name);
	} else {
		elem.m_name[0] = 0;
	}

	switch (type) {
	case 1:
	case 5:
	case 6:
		flags |= 0xB;
		break;
	case 3:
		flags |= 6;
		break;
	case 4:
		flags |= 7;
		break;
	default:
		break;
	}
	elem.m_flags = flags;

	if (m_useImposter && (flags & 2) == 0) {
		WTextureImposterView* pView = new WTextureImposterView(pPuppet);
		elem.m_pImposter = pView;
		pView->SetResourceManager(g_resrcmng);

		WTextureView::TextureParam param;
		param.m_numRts = 1;
		param.m_rtTexInfo[0].Set(0x12000, WRenderToTextureSizeInfo::SIZE_ABS, true, 0);
		param.m_depthSurfInfo.Set((WRenderToTextureParam::DepthSurfInfo::SurfaceUsage)2, WRenderToTextureSizeInfo::SIZE_ABS, true, 1.0f);
		pView->SetTextureParam(param);
	}

	m_elements.push_back(elem);
	m_dirty = true;
}

// scenemanager.cpp:163
void CSceneManager::RegisterElement(SceneElemRegister& reg) {
	RegisterElement(reg.m_type, reg.m_pPuppet, reg.m_pRender, reg.m_name, reg.m_flags, reg.m_pBox);
}

// scenemanager.cpp:168
void CSceneManager::ReleaseElement(void) {
	std::list<SceneElement>::iterator it = m_elements.begin();
	while (it != m_elements.end()) {
		if (it->m_flags & 8) {
			++it;
			continue;
		}

		if (it->m_pImposter) {
			delete it->m_pImposter;
			it->m_pImposter = 0;
		}

		it = m_elements.erase(it);
	}
}

// scenemanager.cpp:195
void CSceneManager::DeleteElement(const char* name) {
	if (IsLocalContent((localContentType_t)0x56)) {
		std::list<SceneElement>::iterator it = m_elements.begin();
		while (it != m_elements.end()) {
			if (strcmp(it->m_name, name) == 0) {
				if (it->m_pImposter) { delete it->m_pImposter; it->m_pImposter = 0; }
				it = m_elements.erase(it);
				continue;
			}
			++it;
		}
	}
}

// scenemanager.cpp:219
void CSceneManager::SetLOD(bool lod, bool apply) {
	m_useImposter = lod && COption::Instance()->vIsLodEnabled();

	if (!apply) {
		return;
	}

	if (m_useImposter) {
		for (std::list<SceneElement>::iterator it = m_elements.begin(); it != m_elements.end(); ++it) {
			if (it->m_flags & 2) {
				continue;
			}
			if (it->m_pImposter) {
				continue;
			}
			if (!it->m_pPuppet) {
				continue;
			}
			it->m_pImposter = new WTextureImposterView(it->m_pPuppet);
			it->m_pImposter->SetResourceManager(g_resrcmng);

			WTextureView::TextureParam param;
			param.m_numRts = 1;
			param.m_rtTexInfo[0].Set(0x12000, WRenderToTextureSizeInfo::SIZE_ABS, true, 0);
			param.m_depthSurfInfo.Set((WRenderToTextureParam::DepthSurfInfo::SurfaceUsage)2, WRenderToTextureSizeInfo::SIZE_ABS, true, 1.0f);

			it->m_pImposter->SetTextureParam(param);
		}
	} else {
		for (std::list<SceneElement>::iterator it = m_elements.begin(); it != m_elements.end(); ++it) {
			if (it->m_pImposter) { delete it->m_pImposter; it->m_pImposter = 0; }
		}
	}
}

// scenemanager.cpp:276
void CSceneManager::SetReflection(bool reflection) {
	for (CPool* pPool = m_poolList.Start(); pPool; pPool = m_poolList.Next()) {
		pPool->SetReflection(reflection);
	}
}

// scenemanager.cpp:282
void CSceneManager::UpdateSea(void) {
	if (m_pSea) {
		m_pSea->LoadTexture();
	}
}

// scenemanager.cpp:288
void CSceneManager::UpdateWind(void) {
	if (m_pWeather) {
		m_pWeather->UpdateWind();
	}
}

// scenemanager.cpp:294
void CSceneManager::UpdateBright(void) {
	g_view->SetFogState(m_fogStart * 3.2f, m_fogEnd * 3.2f, m_fogColor); m_useFog = !(GOLFDOC()->GameMode() & 0x200);
}

// scenemanager.cpp:299
void CSceneManager::UpdateFog(bool load) {
	if (load) {
		cFile* file = g_resrcmng->GetCFile(MakeStr("%s_fog.txt", CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0), -1);

		if (file) {
			int r, g, b;

			file->Scan("%d %d %d\n", &r, &g, &b);
			m_fogColor = ModifyColor((((unsigned char)r << 8 | (unsigned char)g) << 8) | (unsigned char)b, GOLFDOC()->m_fogBrightness);

			file->Scan("%f %f\n", &m_fogStart, &m_fogEnd);

			CloseCFile(file);
		} else {
			m_fogColor = 0xFFFFFF;
			m_fogStart = 100.0f;
			m_fogEnd = 400.0f;
		}
	}

	g_view->SetFogState(m_fogStart * 3.2f, m_fogEnd * 3.2f, m_fogColor);
	m_useFog = !(GOLFDOC()->GameMode() & 0x200);
}

// scenemanager.cpp:328
void CSceneManager::UpdateFog(const char* name) {
	cFile* file = g_resrcmng->GetCFile(MakeStr("%s_fog.txt", name), -1);

	if (file) {
		int r, g, b;

		file->Scan("%d %d %d\n", &r, &g, &b);
		m_fogColor = ModifyColor((unsigned char)b | ((unsigned char)g << 8) | ((unsigned char)r << 16), GOLFDOC()->m_fogBrightness);

		file->Scan("%f %f\n", &m_fogStart, &m_fogEnd);

		CloseCFile(file);
	} else {
		m_fogColor = 0xFFFFFF;
		m_fogStart = 100.0f;
		m_fogEnd = 400.0f;
	}

	g_view->SetFogState(m_fogStart * 3.2f, m_fogEnd * 3.2f, m_fogColor);
	m_useFog = true;
}

// scenemanager.cpp:354
WPuppet* CSceneManager::GetPuppet(const char* name) {
	if (name == 0) {
		return 0;
	}
	std::list<SceneElement>::iterator it = std::find(m_elements.begin(), m_elements.end(), name);
	if (it != m_elements.end()) {
		return it->m_pPuppet;
	}
	return 0;
}

// scenemanager.cpp:366
eObjType CSceneManager::GetType(const char* name) {
	if (name == 0) {
		return (eObjType)7;
	}
	std::list<SceneElement>::iterator it = std::find(m_elements.begin(), m_elements.end(), name);
	if (it != m_elements.end()) {
		return it->m_type;
	}
	return (eObjType)7;
}

// scenemanager.cpp:378
void CSceneManager::SetAlpha(const char* name, unsigned char alpha) {
	if (name != 0) {
		std::list<SceneElement>::iterator it = std::find(m_elements.begin(), m_elements.end(), name);
		if (it != m_elements.end()) {
			it->m_alpha = alpha;
		}
	}
}

// scenemanager.cpp:388
void CSceneManager::SetVisible(const char* name, bool visible) {
	if (name != 0) {
		std::list<SceneElement>::iterator it = std::find(m_elements.begin(), m_elements.end(), name);
		if (it != m_elements.end()) {
			it->m_visible = visible;
		}
	}
}

// scenemanager.cpp:398
void CSceneManager::SetPuppet(const char* name, WPuppet* pPuppet) {
	if (name != 0) {
		std::list<SceneElement>::iterator it = std::find(m_elements.begin(), m_elements.end(), name);
		if (it != m_elements.end()) {
			it->m_pPuppet = pPuppet;
		}
	}
}

// scenemanager.cpp:408
void CSceneManager::SetType(const char* name, eObjType type) {
	if (name != 0) {
		std::list<SceneElement>::iterator it = std::find(m_elements.begin(), m_elements.end(), name);
		if (it != m_elements.end()) {
			it->m_type = type;
			m_dirty = true;
		}
	}
}

// scenemanager.cpp:421
void CSceneManager::Process(float elapsed) {
	if (GOLFDOC()->m_pSky) {
		GOLFDOC()->m_pSky->Process(elapsed);
	}

	if (m_pWeather) {
		m_pWeather->Process(elapsed);
	}

	if (m_pLobbyWeather) {
		m_pLobbyWeather->Process(elapsed);
	}

	if (m_pSea) {
		m_pSea->Process(elapsed);
	}

	CPool* pPool;
	for (pPool = m_poolList.Start(); pPool; pPool = m_poolList.Next()) {
		pPool->Process(elapsed);
	}

	CBeach* pBeach;
	for (pBeach = m_beachList.Start(); pBeach; pBeach = m_beachList.Next()) {
		pBeach->Process(elapsed);
	}

	ntRiver* pRiver;
	for (pRiver = m_riverList.Start(); pRiver; pRiver = m_riverList.Next()) {
		pRiver->Process(elapsed);
	}

	CWavelet* pWavelet;
	for (pWavelet = m_waveletList.Start(); pWavelet; pWavelet = m_waveletList.Next()) {
		pWavelet->Process(elapsed);
	}
}

// scenemanager.cpp:450
void CSceneManager::DropTest(void) {
	WPolySoup::sModel* pModel = GOLFDOC()->m_pPolySoup->FindModel(0);

	Waabb box = pModel->m_aabb;
	int countX = 1 - (int)((box.max.x - box.min.x) / -16.0f);
	int countZ = 1 - (int)((box.max.z - box.min.z) / -16.0f);
	m_drops.clear();

	WVector pos = box.min;
	for (int z = 0; z < countZ; z++) {
		for (int x = 0; x < countX; x++) {
			sDrop drop;
			pos.x += 16.0f;

			switch (GOLFDOC()->m_pAutoTarget->CalcDropPos(pos)) {
			case 0: {
				drop.m_pos = GolfBall().m_pos;
				drop.m_color = 0xFFFF0000;
				m_drops.push_back(drop);
				break;
			}
			case 2: {
				drop.m_pos = GolfBall().m_pos;
				drop.m_color = 0xFF0000FF;
				m_drops.push_back(drop);
				break;
			}
			}
		}
		pos.z += 16.0f; pos.x = box.min.x;
	}

	GolfBall().m_pos = PLAYER(GOLFDOC()->m_myPlayerIndex)->m_ballPos;
}

// scenemanager.cpp:503
bool RenderOrder::operator()(const SceneElement& a, const SceneElement& b) {
	return a.m_type < b.m_type;
}

// scenemanager.cpp:509
void CSceneManager::Prerender(WView* pView) {
	bool useImposter = m_useImposter;
	bool noOverlapCamera = m_noOverlapCamera;
	bool overlapCamera = m_overlapCamera;

	m_useImposter = false;
	m_noOverlapCamera = false;
	m_overlapCamera = false;

	for (CPool* pPool = m_poolList.Start(); pPool; pPool = m_poolList.Next()) {
		pPool->Prerender(pView);
	}

	m_useImposter = useImposter;
	m_noOverlapCamera = noOverlapCamera;
	m_overlapCamera = overlapCamera;
}

// scenemanager.cpp:527
void CSceneManager::Render(WView* pView, unsigned long mask) {
	pView->SetFogEnable(m_useFog && GOLFDOC()->GameMode() != 0x200000);

	g_nDrawPoly = 0;
	g_nDrawModel = 0;
	g_nModel = GOLFDOC()->m_pPolySoup->m_numObjects;

	m_customElements.clear();
	m_effectElements.clear();
	m_nearElements.clear();
	m_blendElements.clear();
	m_transparentElements.clear();

	if (m_dirty) {
		m_elements.sort(RenderOrder());
		m_dirty = false;
	}

	if (mask & 0x80) {
		if (GOLFDOC()->m_pSky) {
			GOLFDOC()->m_pSky->Render(pView);
		}
	}

	unsigned long imposterBits = mask & 0xC;
	if (imposterBits) {
		Render_BuildImposter(pView);
	}

	if (mask & 1) {
		Render_BaseAndShadow(pView, mask);
	}

	int numBlend = Render_RenderElement(pView, mask);

	if (mask & 0x40) {
		Render_RenderEffect(pView);
	}

	if (mask & 0x100) {
		if (m_pMarker) {
			m_pMarker->Display();
		}
	}

	unsigned long water = mask & 0x200;
	if (water) {
		if (m_pSea) {
			m_pSea->Render();
		}
	}

	if (water) {
		Render_RenderWaterStuff(pView);
	}

	pView->Flush(1);

	if (imposterBits) {
		Render_RenderBillboardGroup(pView);
		Render_RenderBlendGroup(pView, m_blendElements, 0, numBlend);
	}

	unsigned long weather = mask & 0x400;
	if (weather) {
		Render_RenderWeather(pView);
	}

	if (mask & 0x20) {
		Render_RenderCustom(pView);
	}

	if (imposterBits) {
		Render_RenderNearGroup(pView);
	}

	if (weather) {
		Render_RenderWeatherFullScreenOverlay(pView);
	}

	if (mask & 0x800) {
		Render_RenderDropPosition(pView);
	}

	pView->Flush(0);

	DrawDebugLine(pView);
}

// scenemanager.cpp:603
void CSceneManager::Render_RenderSky(WView* pView) {
	if (GOLFDOC()->m_pSky) {
		GOLFDOC()->m_pSky->Render(pView);
	}
}

// scenemanager.cpp:609
void CSceneManager::Render_BuildImposter(WView* pView) {
	if (!m_useImposter) {
		return;
	}

	WxBatchState state;

	for (std::list<SceneElement>::iterator it = m_elements.begin(); it != m_elements.end(); ++it) {
		if (&(*it) == 0) {
			continue;
		}
		if (!it->m_visible) {
			continue;
		}
		if (it->m_type == 5) {
			continue;
		}
		if (it->m_type == 6) {
			continue;
		}
		if (it->m_pRender) {
			continue;
		}
		WPuppet* pPuppet = it->m_pPuppet;
		if (pPuppet == 0) {
			continue;
		}
		if (it->m_type != 2) {
			continue;
		}
		if (pPuppet->GetRootBone() == 0) {
			continue;
		}
		if (!pView->InFrustum(pPuppet->GetBoundSphere()) || !pView->InFrustum(pPuppet->GetBoundBox())) {
			continue;
		}
		WTextureImposterView* pImposter = it->m_pImposter;
		if (pImposter == 0) {
			continue;
		}

		const WMatrix& camera = pView->GetCamera();
		WVector dist;
		dist = camera.pivot - pPuppet->GetBoundSphere().pos;
		dist.y = 0.0f;

		if (dist.Magnitude() <= 800.0f) {
			continue;
		}
		if (!pImposter->NeedToUpdate(camera)) {
			continue;
		}

		it->m_pImposter->Begin();
		it->m_pPuppet->SetAlpha(0xFF);

		if (!m_skipTransparent) {
			it->m_pPuppet->Render(it->m_pImposter, true, it->m_pPuppet->GetMatrix().xa.Magnitude(), false, false, false);
		} else {
			it->m_pImposter->xSetLight(0, it->m_pPuppet->GetRootBone()->GetLight());
			it->m_pPuppet->Transform(it->m_pImposter, WVectorLen(it->m_pPuppet->GetMatrix().xa), 2);
			it->m_pPuppet->GetRootBone()->xRender(it->m_pImposter, &state, false, 0, 0);
		}

		it->m_pImposter->Flush(0);
		it->m_pImposter->End();
	}
}

// scenemanager.cpp:698
void CSceneManager::Render_BaseAndShadow(WView* pView, unsigned long mask) {
	for (std::list<SceneElement>::iterator it = m_elements.begin(); it != m_elements.end(); ++it) {
		SceneElement* pElem = &(*it);
		if (pElem == 0) {
			continue;
		}
		if (!pElem->m_visible) {
			continue;
		}
		if ((mask & (1 << pElem->m_type)) == 0) {
			continue;
		}
		if (pElem->m_type != 0) {
			continue;
		}

		WPuppet* pPuppet = pElem->m_pPuppet;
		if (pPuppet == 0) {
			return;
		}
		if (pPuppet->GetRootBone() == 0) {
			return;
		}
		if (!pView->InFrustum(pPuppet->GetBoundSphere()) || !pView->InFrustum(pPuppet->GetBoundBox())) {
			return;
		}

		g_nDrawModel++;
		g_nDrawPoly += pElem->m_pPuppet->GetFaceNum();

		DrawPuppet(pView, pElem->m_pPuppet, pElem->m_pBox);
		pView->Flush(1);

		if (CShadowManager::Instance()) {
			RenderShadow(pView, mask);
		}
		return;
	}
}

// scenemanager.cpp:905
void CSceneManager::Render_RenderEffect(WView* pView) {
	for (std::list<CustomElement>::iterator it = m_effectElements.begin(); it != m_effectElements.end(); ++it) {
		if (it->m_pRender) {
			it->m_pRender->Display();
		}
	}
}

// scenemanager.cpp:917
void CSceneManager::Render_RenderOBMarker(WView* pView) {
	if (m_pMarker) {
		m_pMarker->Display();
	}
}

// scenemanager.cpp:742
int CSceneManager::Render_RenderElement(WView* pView, unsigned long mask) {
	int numBlend = 0;

	for (std::list<SceneElement>::iterator it = m_elements.begin(); it != m_elements.end(); ++it) {
		SceneElement* pElem = &(*it);
		if (pElem == 0) {
			continue;
		}
		if (!pElem->m_visible) {
			continue;
		}
		if ((mask & (1 << pElem->m_type)) == 0) {
			continue;
		}
		if (pElem->m_type == 0) {
			continue;
		}

		if (pElem->m_type == 5) {
			CustomElement custom((eObjType)pElem->m_type, pElem->m_pRender);
			m_customElements.push_back(custom);
			continue;
		}
		if (pElem->m_type == 6 && (mask & 0x40)) {
			CustomElement custom((eObjType)pElem->m_type, pElem->m_pRender);
			m_effectElements.push_back(custom);
			continue;
		}

		if (pElem->m_pRender) {
			pElem->m_pRender->Display();
			continue;
		}

		WPuppet* pPuppet = pElem->m_pPuppet;
		if (pPuppet == 0) {
			continue;
		}
		if (pPuppet->GetRootBone() == 0) {
			continue;
		}

		if (pElem->m_type == 2) {
			if (!pView->InFrustum(pPuppet->GetBoundSphere()) || !pView->InFrustum(pPuppet->GetBoundBox())) {
				continue;
			}
		} else {
			if (!pView->InFrustum(pPuppet->GetBoundSphere())) {
				continue;
			}
		}

		if (m_useImposter && pElem->m_pImposter) {
			WVector dist;
			dist = pView->camera.pivot - pElem->m_pPuppet->GetBoundSphere().pos;
			dist.y = 0.0f;
			float distance = dist.Magnitude();

			if (distance < 960.0f) {
				g_nDrawModel++;
				g_nDrawPoly += pElem->m_pPuppet->GetFaceNum();
			}

			if (distance > 800.0f) {
				float t = Between(0.0f, (960.0f - distance) * 0.00625f, 1.0f);
				pElem->m_alpha = (unsigned char)(t * 255.0f);

				if (distance > 960.0f) {
					TransparentElement te;
					te.m_pPuppet = pElem->m_pPuppet;
					te.m_pBox = pElem->m_pBox;
					te.m_texture = pElem->m_pImposter->GetFirstTexture();
					te.m_numPolys = 0;
					m_transparentElements.push_back(te);
				} else {
					TransparentElement te;
					te.m_pPuppet = pElem->m_pPuppet;
					te.m_pBox = pElem->m_pBox;
					te.m_texture = pElem->m_pImposter->GetFirstTexture();
					te.m_numPolys = pElem->m_pPuppet->GetFaceNum();
					m_blendElements.push_back(te);
					numBlend += pElem->m_pPuppet->GetFaceNum();
				}
			} else {
				pElem->m_alpha = OverlapCamera(pView, *pElem);
				if (pElem->m_alpha < 0xFF) {
					TransparentElement te;
					te.m_pPuppet = pElem->m_pPuppet;
					te.m_pBox = pElem->m_pBox;
					te.m_texture = 0;
					te.m_numPolys = 0;
					m_nearElements.push_back(te);
				}
			}
		} else {
			if (pElem->m_type == 2 || pElem->m_type == 3) {
				g_nDrawModel++;
				g_nDrawPoly += pElem->m_pPuppet->GetFaceNum();
			}

			if (pElem->m_type == 4) {
				pElem->m_alpha = pElem->m_pPuppet->GetRootBone()->GetAlpha();
			} else if (pElem->m_pPuppet) {
				WVector dist;
				dist = pView->camera.pivot - pElem->m_pPuppet->GetBoundSphere().pos;
				dist.y = 0.0f;
				if (dist.Magnitude() > 800.0f) {
					pElem->m_alpha = 0xFF;
				} else {
					pElem->m_alpha = OverlapCamera(pView, *pElem);
				}
			} else {
				pElem->m_alpha = 0xFF;
			}

			if (pElem->m_alpha < 0xFF) {
				TransparentElement te;
				te.m_pPuppet = pElem->m_pPuppet;
				te.m_pBox = pElem->m_pBox;
				te.m_texture = 0;
				te.m_numPolys = 0;
				m_nearElements.push_back(te);
			}
		}

		if (pElem->m_alpha != pElem->m_pPuppet->GetRootBone()->GetAlpha()) {
			pElem->m_pPuppet->SetAlpha(pElem->m_alpha);
		}

		if (!m_skipTransparent) {
			if (pElem->m_alpha == 0xFF) {
				DrawPuppet(pView, pElem->m_pPuppet, pElem->m_pBox);
			}
		} else if (pElem->m_type != 2) {
			DrawPuppet(pView, pElem->m_pPuppet, pElem->m_pBox);
		}
	}

	return numBlend;
}

// scenemanager.cpp:924
void CSceneManager::Render_RenderSea(WView* pView) {
	if (m_pSea) {
		m_pSea->Render();
	}
}

// scenemanager.cpp:931
void CSceneManager::Render_RenderWaterStuff(WView* pView) {
	CBeach* pBeach;
	for (pBeach = m_beachList.Start(); pBeach; pBeach = m_beachList.Next()) {
		pBeach->Render();
	}

	ntRiver* pRiver;
	for (pRiver = m_riverList.Start(); pRiver; pRiver = m_riverList.Next()) {
		pRiver->Render();
	}

	CPool* pPool;
	for (pPool = m_poolList.Start(); pPool; pPool = m_poolList.Next()) {
		pPool->Render(pView);
	}

	CWavelet* pWavelet;
	for (pWavelet = m_waveletList.Start(); pWavelet; pWavelet = m_waveletList.Next()) {
		pWavelet->Render();
	}
}

// Sort predicate of the transparent elements: farthest along the view
// direction first.
struct Greater {
	WVector m_dir;                         // 0x00

	Greater(const WVector& dir);
	bool operator()(const TransparentElement& a, const TransparentElement& b);
};

// scenemanager.cpp:951
unsigned char CSceneManager::OverlapCamera(WView* pView, const SceneElement& elem) {
	if (m_noOverlapCamera) {
		return elem.m_alpha;
	}
	if ((elem.m_flags & 1) || !m_overlapCamera) {
		return 0xFF;
	}

	Waabb box = elem.m_pPuppet->GetBoundBox();
	float dist = 0.0f;

	for (int i = 0; i < 3; i++) {
		float d;
		if (pView->camera.pivot.p[i] < box.min.p[i]) {
			d = pView->camera.pivot.p[i] - box.min.p[i];
		} else if (pView->camera.pivot.p[i] > box.max.p[i]) {
			d = pView->camera.pivot.p[i] - box.max.p[i];
		} else {
			continue;
		}
		dist += d * d;
	}

	float radius = (box.max - box.min).Magnitude() * 0.5f; radius = radius > 96.0f ? 96.0f : radius;

	if (dist > radius * radius) {
		return 0xFF;
	}

	float ratio = sqrtf(dist) / radius; if (ratio < 0.25f) ratio = 0.25f; else if (ratio > 1.0f) ratio = 1.0f;
	return (unsigned char)(ratio * 255.0f);
}

// scenemanager.cpp:986
Greater::Greater(const WVector& dir) : m_dir(dir) {
}

// scenemanager.cpp:989
bool Greater::operator()(const TransparentElement& a, const TransparentElement& b) {
	const WVector& pa = a.m_pPuppet->GetBoundSphere().pos;
	const WVector& pb = b.m_pPuppet->GetBoundSphere().pos;
	return pa.x * m_dir.x + pa.y * m_dir.y + pa.z * m_dir.z > pb.x * m_dir.x + pb.y * m_dir.y + pb.z * m_dir.z;
}

// scenemanager.cpp:997
void CSceneManager::Render_RenderBillboardGroup(WView* pView) {
	if (m_transparentElements.size()) {
		for (std::list<TransparentElement>::iterator it = m_transparentElements.begin(); it != m_transparentElements.end(); ++it) {
			DrawImposter(pView, *it);
		}
		if (!m_skipTransparent) {
			pView->Flush(0);
		}
	}
}

// scenemanager.cpp:1009
void CSceneManager::Render_RenderBlendGroup(WView* pView, std::list<TransparentElement>& elements, int first, int last) {
	if (elements.size() == 0) {
		return;
	}

	if (m_skipTransparent) {
		for (std::list<TransparentElement>::iterator it = elements.begin(); it != elements.end(); ++it) {
			DrawImposter(pView, *it);
		}
		return;
	}

	if (elements.size() == 1 || last < 0x400) {
		for (std::list<TransparentElement>::iterator it = elements.begin(); it != elements.end(); ++it) {
			DrawPuppet(pView, it->m_pPuppet, it->m_pBox);
			DrawImposter(pView, *it);
		}
		pView->Flush(0);
		return;
	}

	// Split the elements in two groups along an alternating camera axis and
	// draw each group on its own.
	std::list<TransparentElement> groupA;
	std::list<TransparentElement> groupB;
	bool split = false;

	WVector axis;
	if (first & 1) {
		axis = pView->camera.za;
	} else {
		axis = pView->camera.xa;
	}

	elements.sort(Greater(axis));

	float prevExtent = -1.0f;
	int countA = 0;
	int countB = 0;
	std::list<TransparentElement>::iterator it = elements.begin();
	for (; it != elements.end(); ++it) {
		std::list<TransparentElement>::iterator next = it;
		++next;
		if (next == elements.end()) {
			break;
		}

		if (!split) {
			groupA.push_back(*it);
			countA += it->m_numPolys;

			if (countA < last / 3) {
				continue;
			}
			if (countA > last * 2 / 3) {
				split = true;
				continue;
			}

			float proj = WDotProduct(axis, it->m_pPuppet->GetBoundSphere().pos - next->m_pPuppet->GetBoundSphere().pos);
			float extent;
			if (prevExtent <= 0.0f) {
				const Waabb& box = it->m_pPuppet->GetBoundBox();
				extent = WVectorLen(WVector(box.max.x - box.min.x, 0.0f, box.max.z - box.min.z));
			} else {
				extent = prevExtent;
			}
			const Waabb& nextBox = next->m_pPuppet->GetBoundBox();
			prevExtent = WVectorLen(WVector(nextBox.max.x - nextBox.min.x, 0.0f, nextBox.max.z - nextBox.min.z));

			if (extent + prevExtent < Abs(proj)) {
				split = true;
			}
		} else {
			groupB.push_back(*it);
			countB += it->m_numPolys;
		}
	}

	groupB.push_back(*it);
	countB += it->m_numPolys;

	Render_RenderBlendGroup(pView, groupA, first + 1, countA);
	Render_RenderBlendGroup(pView, groupB, first + 1, countB);
}

// scenemanager.cpp:1102
void CSceneManager::Render_RenderWeather(WView* pView) {
	if (AfxGetTask()->GetActor("LobbyBg") == 0) {
		if (m_pWeather) {
			m_pWeather->Display();
		}
	}

	if (m_pLobbyWeather) {
		m_pLobbyWeather->Display();
	}
}

// scenemanager.cpp:1119
void CSceneManager::Render_RenderCustom(WView* pView) {
	for (std::list<CustomElement>::iterator it = m_customElements.begin(); it != m_customElements.end(); ++it) {
		if (it->m_pRender) {
			it->m_pRender->Display();
		}
	}
}

// scenemanager.cpp:1130
void CSceneManager::Render_RenderNearGroup(WView* pView) {
	if (m_nearElements.size() && !m_skipTransparent) {
		m_nearElements.sort(Greater(pView->camera.za));

		for (std::list<TransparentElement>::iterator it = m_nearElements.begin(); it != m_nearElements.end(); ++it) {
			DrawPuppet(pView, it->m_pPuppet, it->m_pBox);
			pView->Flush(2);
		}
	}
}

// scenemanager.cpp:1144
void CSceneManager::Render_RenderWeatherFullScreenOverlay(WView* pView) {
	if (AfxGetTask()->GetActor("LobbyBg") == 0) {
		if (m_pWeather) {
			m_pWeather->DisplayFullScreenOverlay();
		}
	}

	if (m_pLobbyWeather) {
		m_pLobbyWeather->DisplayFullScreenOverlay();
	}
}

// scenemanager.cpp:1161
// Debug aid of the drop test: crosses at every drop position, then, while a
// drop is pending, a flat ring around the hole that shrinks toward the ball
// (a fan of 128 triangles in the first colour, then a black outline ring).
void CSceneManager::Render_RenderDropPosition(WView* pView) {
	if (GOLFDOC()->GameMode() & 0x200) {
		for (std::list<sDrop>::iterator it = m_drops.begin(); it != m_drops.end(); ++it) {
			WVector axis = pView->camera.xa + pView->camera.ya;
			pView->DrawLine(it->m_pos + axis * 4.0f, it->m_color, it->m_pos - axis * 4.0f, it->m_color, 0x300000);

			WVector axis2 = pView->camera.xa - pView->camera.ya;
			pView->DrawLine(it->m_pos + axis2 * 4.0f, it->m_color, it->m_pos - axis2 * 4.0f, it->m_color, 0x300000);
		}

		if (GOLFDOC()->m_dropPending && GOLFDOC()->m_setFlag159) {
			WVector cup = GOLFDOC()->GetHoleData().m_cupPos;

			WTVertex fan[3];
			WTVertex* polygon[4];
			polygon[0] = &fan[0];
			polygon[1] = &fan[1];
			polygon[2] = &fan[2];
			polygon[3] = 0;

			fan[0].diffuse = 0x66000000;
			fan[1].diffuse = 0x66000000;
			fan[2].diffuse = 0x66000000;

			float dx = GolfBall().m_pos.x - cup.x;
			float dz = GolfBall().m_pos.z - cup.z;
			float dist = sqrtf(dx * dx + dz * dz);

			cup = GOLFDOC()->GetHoleData().m_cupPos;
			fan[2].x = cup.x;
			fan[2].y = cup.y;
			fan[2].z = cup.z;

			vtr[0].diffuse = 0xFF000000;
			vtr[1].diffuse = 0xFF000000;
			vtr[2].diffuse = 0xFF000000;
			vtr[3].diffuse = 0xFF000000;

			float radius = dist - 25.6f;
			for (int i = 0; i < 128; i++) {
				for (int k = 0; k < 2; k++) {
					float angle = (float)((k + i) % 128) * (3.14159265f / 64.0f);
					WVector c = GOLFDOC()->GetHoleData().m_cupPos;
					WVector off(cosf(angle) * radius, 0.0f * radius, sinf(angle) * radius);
					fan[k].x = c.x + off.x;
					fan[k].y = c.y + off.y;
					fan[k].z = c.z + off.z;
				}
				pView->DrawPolygonFan(polygon, 0x300000, 0x400, false);

				for (int j = 0; j < 4; j++) {
					float r = radius;
					if (j & 2) {
						r += 1.0f;
					}
					float angle = (float)(((j & 1) + i) % 128) * (3.14159265f / 64.0f);
					WVector c = GOLFDOC()->GetHoleData().m_cupPos;
					WVector off(cosf(angle) * r, 0.0f * r, sinf(angle) * r);
					vtr[j].x = c.x + off.x;
					vtr[j].y = c.y + off.y;
					vtr[j].z = c.z + off.z;
				}
				pView->DrawPolygonFan(vtx, 0x300000, 0x400, false);
			}
		}
	}
}

// scenemanager.cpp:1266
void CSceneManager::DrawPuppet(WView* pView, WPuppet* pPuppet, Waabb* pBox) {
	pPuppet->Render(pView, true, pPuppet->GetMatrix().xa.Magnitude(), pView->InFrustumSafe(pPuppet->GetBoundBox()), false, false);

	if (m_debugDraw & 1) {
		pView->DrawAABB(pPuppet->GetBoundBox(), 0xFFFF0000, false, 0);
	}
	if ((m_debugDraw >> 1) & 1) {
		w_bound_box* box = pPuppet->m_bbList.Find("char0");
		if (box) {
			int index = 0;
			do {
				Wobb obb(box->bone->GetMatrix());
				obb *= (box->info->aabb.max - box->info->aabb.min) * 0.5f + WVector::ONE * 1.1f;
				obb.center = box->bone->Transform(box->info->spherePivot);
				pView->DrawOBB(obb, 0xFF00FF00);
				++index;
				box = pPuppet->m_bbList.Find(MakeStr("char%d", index));
			} while (box);
		} else if (pBox) {
			pView->DrawAABB(*pBox, 0xFF00FF00, false, 0);
		}
	}
	if ((m_debugDraw >> 2) & 1) {
		pView->DrawSphere(pPuppet->GetBoundSphere(), 0xFFFFFF00, 0x18, 0);
	}
}

// scenemanager.cpp:1305
void CSceneManager::RenderShadow(WView* pView, unsigned long mask) {
	int caster = 0;
	std::list<SceneElement>::iterator it;

	for (it = m_elements.begin(); it != m_elements.end(); ++it) {
		if (&(*it) == 0) {
			continue;
		}
		if (!it->m_visible) {
			continue;
		}
		if ((mask & (1 << it->m_type)) == 0) {
			continue;
		}
		if (it->m_type != 0) {
			continue;
		}

		const Waabb& box = it->m_pPuppet->GetBoundBox();

		CShadowManager::Instance()->RegisterRecver_New("bg");
		CShadowManager::Instance()->AddRecvPet_New("bg", it->m_pPuppet, box.min.y);

		for (it = m_elements.begin(); it != m_elements.end(); ++it) {
			if (&(*it) == 0) {
				continue;
			}
			if (!it->m_visible) {
				continue;
			}
			if ((mask & (1 << it->m_type)) == 0) {
				continue;
			}
			if (it->m_type == 0) {
				continue;
			}
			if ((it->m_flags & 0x30) == 0) {
				continue;
			}

			if (it->m_pRender) {
				it->m_pRender->DisplayShadow();
				continue;
			}
			if (it->m_pPuppet == 0 || it->m_pPuppet->GetRootBone() == 0) {
				continue;
			}

			CShadowManager::Instance()->RegisterCaster_New(MakeStr("caster%d", caster), (~it->m_flags & 0x20) >> 1, "sun", g_lightset);
			CShadowManager::Instance()->AddCastPet_New(MakeStr("caster%d", caster), false, it->m_pPuppet);
			caster++;
		}

		CShadowManager::Instance()->Flush(pView);

		CShadowManager::Instance()->Reset_New();
		return;
	}
}

// scenemanager.cpp:1409
void CSceneManager::Clear(void) {
	if (m_pSea) {
		delete m_pSea;
		m_pSea = NULL;
	}
	for (CPool* pool = m_poolList.Start(); pool; pool = m_poolList.Next()) {
		delete pool;
	}
	m_poolList.Reset();
	for (CBeach* beach = m_beachList.Start(); beach; beach = m_beachList.Next()) {
		delete beach;
	}
	m_beachList.Reset();
	for (CWavelet* wavelet = m_waveletList.Start(); wavelet; wavelet = m_waveletList.Next()) {
		delete wavelet;
	}
	m_waveletList.Reset();
	for (ntRiver* river = m_riverList.Start(); river; river = m_riverList.Next()) {
		delete river;
	}
	m_riverList.Reset();
	if (m_pMarker) {
		m_pMarker->Reset();
	}
}

// scenemanager.cpp:1422
// Builds the course's water and markers from the polygons and areas of the
// course geometry.
void CSceneManager::Load(void) {
	Clear();

	for (std::vector<CNodeContainer>::const_iterator it = GOLFDOC()->m_pPolySoup->m_boundAreas.begin(); it != GOLFDOC()->m_pPolySoup->m_boundAreas.end(); ++it) {
		if (it->GetType() == 10) {
			m_pSea = new CSea;
			m_pSea->SetArea(it->GetList());
		} else if (it->GetType() == 5) {
			CBeach* pBeach = new CBeach;
			pBeach->SetWavelet(it->GetList(), it->GetName().c_str(), 3);

			m_beachList.AddItem(pBeach);
		} else if (it->GetType() == 7) {
			ntRiver* pRiver = new CRiver;
			pRiver->Setup(it->GetList(), it->GetName().c_str());

			m_riverList.AddItem(pRiver);
		} else if (it->GetType() == 8) {
			ntRiver* pRiver = new CRiver_ex;
			pRiver->Setup(it->GetList(), it->GetName().c_str());

			m_riverList += pRiver;
		} else if (it->GetType() == 13) {
			ntRiver* pRiver = new ntRiver_Lava;
			pRiver->Setup(it->GetList(), it->GetName().c_str());

			m_riverList += pRiver;
		} else if (it->GetType() == 11) {
			CWavelet* pWavelet = new CWavelet;
			pWavelet->SetWavelet(it->GetList(), it->GetName().c_str(), 2);

			m_waveletList += pWavelet;
		} else if (it->GetType() == 3 || it->GetType() == 2) {
			m_pMarker->AddList(it->GetList());
		}
	}

	for (int i = 0; i < GOLFDOC()->m_pPolySoup->GetAreaNum(); i++) {
		WPolySoup::sArea* pArea = GOLFDOC()->m_pPolySoup->GetArea(i);

		if (pArea->m_type == 2) {
			CPool* pPool = CPool::CreatePoolByMapType((eMapType)GetCurMap());
			pPool->SetArea(pArea->m_aabb, pArea->m_name);

			m_poolList.AddItem(pPool);
		}
	}

	m_pMarker->GetReady();
}

// scenemanager.cpp:1523
// Switches the shadows and the course's shadow map on or off, each only when
// the option allows it. Not referenced in this object, and the PDB has no
// public for it.
void __stdcall EnableShadows(bool shadow, bool shadowmap) {
	if (COption::Instance()->vIsShadowEnabled()) {
		CShadowManager::Instance()->Enable(shadow);
	}
	if (COption::Instance()->vIsShadowmapEnabled()) {
		GOLFDOC()->m_pPolySoup->EnableShadowmap(shadowmap);
	}
}

// scenemanager.cpp:1690
// Rebuilds the lobby's weather for the lobby background map: drops both
// weather objects, then (when the particle option allows it) creates the
// effect of the map and its gravity.
void CSceneManager::SetLobbyWeather(bool weather) {
	EnableShadows(true, true);

	delete m_pWeather;
	m_pWeather = NULL;
	delete m_pLobbyWeather;
	m_pLobbyWeather = NULL;

	if (m_pLobbyWeather) {
		return;
	}

	int count = 0;
	switch (COption::Instance()->vGetParticleLevel()) {
	case 0:
		count = 900;
		break;
	case 1:
		count = 500;
		break;
	case 2:
		count = 200;
		break;
	}

	float gravity = 0.0f;
	switch (LOBBYBG_MAP) {
	case 8:
	case 9:
	case 14:
		m_pLobbyWeather = new ntIceSnow(count * 4, "GlowGraySmall.jpg", 0);
		gravity = 0.08f;
		break;
	case 11:
		m_pLobbyWeather = new ntImplFlutter<sPetalNameHolder>(count, "", 0);
		break;
	case 13:
		m_pLobbyWeather = new ntImplFlutter<sSilicaParticleNameHolder>(count, "", 0);
		break;
	case 19:
		if (count > 200) {
			m_pLobbyWeather = new CSnow(count / 10, "bubble.jpg", 0, 128, 128);
		}
		gravity = 0.025f;
		break;
	}

	if (m_pLobbyWeather) {
		m_pLobbyWeather->SetGravity(gravity);
	}
}

// scenemanager.cpp:1759
// Debug overlay: the course's model box drawn as a flat quad at the lowest
// point (bit 4 of m_debugDraw) and 192 above it (bit 5), and, when
// m_drawElementBoxes is set, the name of every nearby or off-screen element
// at its projected bounding-box top.
void CSceneManager::DrawDebugLine(WView* pView) {
	if ((m_debugDraw >> 4) & 1) {
		Waabb& box = GOLFDOC()->m_pPolySoup->FindModel(0)->m_aabb;
		for (int j = 0; j < 4; j++) {
			float x = (j & 1) ? box.max.x : box.min.x;
			float z = (j & 2) ? box.max.z : box.min.z;
			vtr[j].x = x;
			vtr[j].y = GetPVS().m_lowest;
			vtr[j].z = z;
			vtr[j].diffuse = 0x808080FF;
		}
		pView->DrawPolygonFan(vtx, 0, 0, false);
	}

	if ((m_debugDraw >> 5) & 1) {
		Waabb& box = GOLFDOC()->m_pPolySoup->FindModel(0)->m_aabb;
		for (int j = 0; j < 4; j++) {
			float x = (j & 1) ? box.max.x : box.min.x;
			float z = (j & 2) ? box.max.z : box.min.z;
			vtr[j].x = x;
			vtr[j].y = GetPVS().m_lowest + 192.0f;
			vtr[j].z = z;
			vtr[j].diffuse = 0x808080FF;
		}
		pView->DrawPolygonFan(vtx, 0, 0, false);
	}

	if (m_drawElementBoxes) {
		for (std::list<SceneElement>::iterator it = m_elements.begin(); it != m_elements.end(); ++it) {
			WPuppet* pPuppet = it->m_pPuppet;
			if (pPuppet == 0) {
				continue;
			}

			const Waabb& bound = pPuppet->GetBoundBox();
			WVector top((bound.min.x + bound.max.x) * 0.5f, (bound.max.y + bound.min.y) * 0.5f, (bound.min.z + bound.max.z) * 0.5f);
			top.y = bound.max.y;

			WVector delta = top - pView->camera.pivot;
			float dist = sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
			if (dist > 192.0f && pView->InFrustum(pPuppet->GetBoundBox())) {
				continue;
			}

			WVector screen = pView->Projection(top);
			if (0.0f < screen.z && screen.z < 1.0f) {
				m_pFont->Print(pView, screen.x - m_pFont->GetTextWidth(pView, it->m_name) * 0.5f, screen.y, it->m_name, 0, 0xFFFFFFFF, 0);
			}
		}
		m_pFont->Flush(pView);
	}
}
