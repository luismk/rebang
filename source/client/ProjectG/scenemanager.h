// Original: d:\build\custom\temp\code\client\projectg\scenemanager.h
#ifndef PG_CLIENT_PROJECTG_SCENEMANAGER_H
#define PG_CLIENT_PROJECTG_SCENEMANAGER_H

#include <string.h>
#include <list>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "commonutil.h"
#include "wlist.h"
#include "wmath.h"

class WPuppet;
class Waabb;
class CSea;
class CWeatherType;
class CMarker;
class WFont;
class WView;
class CBeach;
class CWavelet;
class CPool;
class ntRiver;
struct WTVertex;

// Kind of a registered scene element; the enumerators are not known yet.
enum eObjType {
};

class CRenderFuncPtr {
public:
	// scenemanager.h:54
	CRenderFuncPtr(void) {}
	// scenemanager.h:55
	~CRenderFuncPtr(void) {}
	// scenemanager.h:58
	void DisplayAll(void) {
		Display();
	}
	virtual void DisplayShadow(void);     // scenemanager.h:67
	virtual void Display(void) = 0;       // vtable slot 1 (_purecall)

protected:
	virtual void DebugDisplay(void);      // scenemanager.h:71
};
CHECK_SIZE(CRenderFuncPtr, 0x4);

class WTextureImposterView;

// One registered scene element (`std::list<SceneElement>` of CSceneManager).
struct SceneElement {
	eObjType m_type;                       // 0x00
	WPuppet* m_pPuppet;                    // 0x04
	CRenderFuncPtr* m_pRender;             // 0x08
	char m_name[0x80];                     // 0x0C
	unsigned long m_flags;                 // 0x8C
	unsigned char m_alpha;                 // 0x90
	bool m_visible;                        // 0x91
	WTextureImposterView* m_pImposter;     // 0x94
	Waabb* m_pBox;                         // 0x98

	// scenemanager.h:78
	SceneElement(void) {
		m_type = (eObjType)2; m_pPuppet = 0;
		m_pRender = 0;
		m_name[0] = 0;
		m_pImposter = 0;
		m_pBox = 0;
		m_flags = 0;
		m_alpha = 0xFF;
		m_visible = true;
	}
	// scenemanager.h:91
	~SceneElement(void) {}
	// scenemanager.h:106
	bool operator==(const char* name) const {
		if (m_name[0] == 0 || name == 0) {
			return false;
		}
		return _strcmpi(m_name, name) == 0;
	}
};
CHECK_SIZE(SceneElement, 0x9C);
CHECK_FIELD(SceneElement, m_flags, 0x8C);
CHECK_FIELD(SceneElement, m_pBox, 0x98);

// A custom-rendered element.
struct CustomElement {
	eObjType m_type;                       // 0x00
	CRenderFuncPtr* m_pRender;             // 0x04

	// scenemanager.h:116
	CustomElement(eObjType type, CRenderFuncPtr* pRender) {
		m_type = type;
		m_pRender = pRender;
	}
};
CHECK_SIZE(CustomElement, 0x8);

// Sort predicate of the scene elements: ascending type.
struct RenderOrder {
	bool operator()(const SceneElement& a, const SceneElement& b);
};

// An element drawn after the opaque ones, sorted by Greater (scenemanager.cpp).
struct TransparentElement {
	WPuppet* m_pPuppet;                    // 0x00
	Waabb* m_pBox;                         // 0x04
	unsigned long m_texture;               // 0x08, the imposter's first render-target texture
	int m_numPolys;                        // 0x0C, face count of the puppet
};
CHECK_SIZE(TransparentElement, 0x10);

// RegisterElement's arguments packed in one structure.
struct SceneElemRegister {
	eObjType m_type;                       // 0x00
	WPuppet* m_pPuppet;                    // 0x04
	CRenderFuncPtr* m_pRender;             // 0x08
	const char* m_name;                    // 0x0C
	unsigned long m_flags;                 // 0x10
	Waabb* m_pBox;                         // 0x14
};
CHECK_SIZE(SceneElemRegister, 0x18);

// The scene's element registry. Only the members this project's code uses so
// far are declared.
class CSceneManager : public CStackableSingleton<CSceneManager> {
	LAYOUT_CHECK_FRIEND;
public:
	// A drop position marker of DropTest (std::list<sDrop>).
	struct sDrop {
		WVector m_pos;                     // 0x00
		unsigned long m_color;             // 0x0C
	};

	CSceneManager(void);
	virtual ~CSceneManager(void);
	void Load(void);
	void SetReflection(bool reflection);
	void Prerender(WView* pView);
	void UpdateSea(void);
	void UpdateWind(void);
	void RegisterElement(eObjType type, WPuppet* pPuppet, CRenderFuncPtr* pRender, const char* name, unsigned long flags, Waabb* pBox);
	void RegisterElement(SceneElemRegister& reg);
	void ReleaseElement(void);
	void UpdateBright(void);
	void UpdateFog(bool load);
	void UpdateFog(const char* name);
	void SetLobbyWeather(bool weather);
	WPuppet* GetPuppet(const char* name);
	eObjType GetType(const char* name);
	void SetAlpha(const char* name, unsigned char alpha);
	void SetVisible(const char* name, bool visible);
	void SetPuppet(const char* name, WPuppet* pPuppet);
	void SetType(const char* name, eObjType type);
	void DeleteElement(const char* name);
	void SetLOD(bool lod, bool apply);
	void DropTest(void);
	void Clear(void);
	void Process(float elapsed);
	void DrawDebugLine(WView* pView);
	void Render(WView* pView, unsigned long mask);
	void SetWeather(unsigned char weather, bool lobby);
	// scenemanager.h:232
	bool ToogleNoOverlapCamera(void) {
		return m_noOverlapCamera = !m_noOverlapCamera;
	}

protected:
	unsigned char OverlapCamera(WView* pView, const SceneElement& elem);
	void Render_BuildImposter(WView* pView);
	void Render_RenderWaterStuff(WView* pView);
	void Render_RenderDropPosition(WView* pView);
	int Render_RenderElement(WView* pView, unsigned long mask);
	void Render_RenderBlendGroup(WView* pView, std::list<TransparentElement>& elements, int first, int last);
	void Render_RenderOBMarker(WView* pView);
	void Render_RenderSea(WView* pView);
	void Render_RenderSky(WView* pView);
	void Render_RenderWeather(WView* pView);
	void Render_RenderWeatherFullScreenOverlay(WView* pView);
	void Render_RenderEffect(WView* pView);
	void Render_RenderBillboardGroup(WView* pView);
	void Render_RenderCustom(WView* pView);
	void Render_RenderNearGroup(WView* pView);
	void DrawImposter(WView* pView, const TransparentElement& elem);
	void DrawPuppet(WView* pView, WPuppet* pPuppet, Waabb* pBox);
	void RenderShadow(WView* pView, unsigned long mask);
	void Render_BaseAndShadow(WView* pView, unsigned long mask);

private:
	static WTVertex vtr[4];
	static WTVertex* vtx[5];

	WList<CBeach*> m_beachList;            // 0x28
	WList<CWavelet*> m_waveletList;        // 0x48
	WList<CPool*> m_poolList;              // 0x68
	WList<ntRiver*> m_riverList;           // 0x88
	CSea* m_pSea;                          // 0xA8
	CMarker* m_pMarker;                    // 0xAC
public:
	CWeatherType* m_pWeather;              // 0xB0, read directly by CWind::SetWind
private:
	CWeatherType* m_pLobbyWeather;         // 0xB4, set by SetLobbyWeather, drawn after m_pWeather
	bool m_useFog;                         // 0xB8, UpdateFog: CGolfDoc::m_gameMode bit 0x200 clear
	bool m_useImposter;                    // 0xB9, elements get a texture imposter
	bool m_skipTransparent;                // 0xBA, no billboard flush and no near group drawn
	UNKNOWN(0xBB, 0xBC);
	std::list<SceneElement> m_elements;    // 0xBC
	std::list<CustomElement> m_customElements; // 0xC8, drawn by Render_RenderCustom
	std::list<CustomElement> m_effectElements; // 0xD4, drawn by Render_RenderEffect
	std::list<TransparentElement> m_transparentElements; // 0xE0, drawn by Render_RenderBillboardGroup
	std::list<TransparentElement> m_blendElements; // 0xEC, filled by Render_RenderElement, drawn by Render_RenderBlendGroup
	std::list<TransparentElement> m_nearElements; // 0xF8, sorted far to near by Render_RenderNearGroup
	bool m_dirty;                          // 0x104, set when the element list changes
	UNKNOWN(0x105, 0x108);
	std::list<sDrop> m_drops;              // 0x108, filled by DropTest, drawn by Render_RenderDropPosition
	UNKNOWN(0x114, 0x115);
	bool m_drawElementBoxes;               // 0x115, DrawDebugLine draws every element's bounding box
	PADDING(0x116, 0x118);
	WFont* m_pFont;                        // 0x118, CChatMsg's masked font, taken by the constructor
	float m_fogStart;                      // 0x11C, scaled by UpdateBright
	float m_fogEnd;                        // 0x120
	unsigned long m_fogColor;              // 0x124
	bool m_overlapCamera;                  // 0x128, OverlapCamera fades elements between the camera and the target
	bool m_noOverlapCamera;                // 0x129, OverlapCamera keeps each element's own alpha (ToogleNoOverlapCamera)
	UNKNOWN(0x12A, 0x12C);
	unsigned long m_debugDraw;             // 0x12C, DrawDebugLine: bit 0 AABB, bit 1 OBBs, bit 2 sphere
};
BEGIN_LAYOUT_CHECK(CSceneManager)
	CHECK_FIELD(CSceneManager, m_beachList, 0x28);
	CHECK_FIELD(CSceneManager, m_waveletList, 0x48);
	CHECK_FIELD(CSceneManager, m_poolList, 0x68);
	CHECK_FIELD(CSceneManager, m_riverList, 0x88);
	CHECK_FIELD(CSceneManager, m_pSea, 0xA8);
	CHECK_FIELD(CSceneManager, m_pMarker, 0xAC);
	CHECK_FIELD(CSceneManager, m_pWeather, 0xB0);
	CHECK_FIELD(CSceneManager, m_pLobbyWeather, 0xB4);
	CHECK_FIELD(CSceneManager, m_useFog, 0xB8);
	CHECK_FIELD(CSceneManager, m_useImposter, 0xB9);
	CHECK_FIELD(CSceneManager, m_skipTransparent, 0xBA);
	CHECK_FIELD(CSceneManager, m_elements, 0xBC);
	CHECK_FIELD(CSceneManager, m_customElements, 0xC8);
	CHECK_FIELD(CSceneManager, m_effectElements, 0xD4);
	CHECK_FIELD(CSceneManager, m_transparentElements, 0xE0);
	CHECK_FIELD(CSceneManager, m_blendElements, 0xEC);
	CHECK_FIELD(CSceneManager, m_nearElements, 0xF8);
	CHECK_FIELD(CSceneManager, m_dirty, 0x104);
	CHECK_FIELD(CSceneManager, m_drops, 0x108);
	CHECK_FIELD(CSceneManager, m_drawElementBoxes, 0x115);
	CHECK_FIELD(CSceneManager, m_pFont, 0x118);
	CHECK_FIELD(CSceneManager, m_fogStart, 0x11C);
	CHECK_FIELD(CSceneManager, m_fogEnd, 0x120);
	CHECK_FIELD(CSceneManager, m_fogColor, 0x124);
	CHECK_FIELD(CSceneManager, m_overlapCamera, 0x128);
	CHECK_FIELD(CSceneManager, m_noOverlapCamera, 0x129);
	CHECK_FIELD(CSceneManager, m_debugDraw, 0x12C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SCENEMANAGER_H
