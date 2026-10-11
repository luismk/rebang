// Original: d:\build\custom\temp\code\client\projectg\shadow.h
#ifndef PG_CLIENT_PROJECTG_SHADOW_H
#define PG_CLIENT_PROJECTG_SHADOW_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "wscene.h"

#include <float.h>
#include <string.h>
#include <list>

class WPuppet;
class WRecvView;
class WResourceManager;
class WShadowView;
class WView;
struct sShadowRecver;

// How the shadow is rendered (CShadow::CShadow: 0 builds the software
// views, 1 and 2 the hardware ones; CShadowManager passes m_hwCaps).
enum eShadowMode {
	SHADOW_MODE_SOFTWARE = 0,
	SHADOW_MODE_HARDWARE = 1,
	SHADOW_MODE_HARDWARE2 = 2
};

// A shadow renderer. Only the members the units built so far use are
// transcribed; the size is unknown.
class CShadow {
public:
	// Only the values the code tests are named.
	enum ShadowType {
		SHADOW_TYPE_NONE = 0,
		SHADOW_TYPE_PROJECTION = 1
	};

	CShadow(WResourceManager *resrcmng, eShadowMode mode);
	virtual ~CShadow();

	void SetShadowType(ShadowType type);
	void RenderCaster(WView *view, const LightSet &light, const WSphere &bsphere, std::list<WPuppet *> &pets, sShadowRecver *recver, bool update);
	void RenderRecver(sShadowRecver *recver);

protected:
	bool Setup(WView *view, const LightSet &light, const WSphere &bsphere, sShadowRecver *recver);
	bool CheckFrustum(WView *view, const WMatrix &cam, float fov, float nearDist, float farDist);
	void RenderProjectionShadow(WView *view, const LightSet &light, const WSphere &bsphere, std::list<WPuppet *> &pets, sShadowRecver *recver, bool update);

public:
	WShadowView *m_shadowView; // 0x4
	WRecvView *m_recvView;     // 0x8
	ShadowType m_shadowType;   // 0xC
};
CHECK_FIELD(CShadow, m_shadowView, 0x4);
CHECK_FIELD(CShadow, m_recvView, 0x8);
CHECK_FIELD(CShadow, m_shadowType, 0xC);

extern const char *const g_theCasterName;

// A shadow-casting object registered with the shadow manager. Only the
// members the units built so far use are transcribed; the size is unknown.
class CShadowCaster {
public:
	CShadowCaster(const char *name, int state, WResourceManager *resrcmng, eShadowMode mode);
	~CShadowCaster();

	const char *GetName() const;
	const char *GetLightName() const;
	void Reset();
	int GetState() const;
	void SetState_New(int state);
	void ClearCaster();
	void ClearCaster_New();
	void AddPet(WPuppet *pet);
	void AddPet_New(WPuppet *pet);
	void DeletePet(WPuppet *pet);
	void ReplacePets(WPuppet *oldPet, WPuppet *newPet, bool show);
	void UpdateBSphere(WView *view, float minAngle, float minDist);
	void RenderCaster(WView *view, sShadowRecver *recver);
	void RenderRecver(sShadowRecver *recver);
	void ShowCaster(bool show);
	void SetLight(const char *lightName, LightSet &light);

	char m_name[0x20];      // 0x0
	char m_lightName[0x20]; // 0x20
	LightSet m_light;       // 0x40
	std::list<WPuppet *> m_pets; // 0x5C
	WSphere m_bsphere;      // 0x68: bounding sphere of the pets (radius at least 8)
	CShadow *m_shadow;      // 0x78
	int m_state;            // 0x7C: state bit flags (1 = light changed, 2 = no pet, 0x20 = caster hidden, 0x40 = keep a small bounding sphere, 0x80 = rendered once)
};
CHECK_SIZE(CShadowCaster, 0x80);
CHECK_FIELD(CShadowCaster, m_name, 0x0);
CHECK_FIELD(CShadowCaster, m_lightName, 0x20);
CHECK_FIELD(CShadowCaster, m_light, 0x40);
CHECK_FIELD(CShadowCaster, m_pets, 0x5C);
CHECK_FIELD(CShadowCaster, m_bsphere, 0x68);
CHECK_FIELD(CShadowCaster, m_shadow, 0x78);
CHECK_FIELD(CShadowCaster, m_state, 0x7C);

// A shadow receiver registered with the shadow manager. Only the members the
// units built so far use are transcribed; the size is unknown.
struct sShadowRecver {
	// shadow.h:32
	sShadowRecver() : m_pet(0), m_dist(FLT_MAX) {
		memset(m_name, 0, sizeof(m_name));
	}

	// shadow.h:37
	void ClearRecver() {
		m_pet = 0;
		m_dist = FLT_MAX;
	}

	WPuppet *m_pet;      // 0x0
	char m_name[0x20];   // 0x4
	float m_dist;        // 0x24, FLT_MAX when cleared
};
CHECK_FIELD(sShadowRecver, m_pet, 0x0);
CHECK_FIELD(sShadowRecver, m_name, 0x4);
CHECK_FIELD(sShadowRecver, m_dist, 0x24);

// The shadow manager singleton (size 0x5C, the `new` of
// CGolfBg::OnPreLoadInit). Only the methods the units built so far call are
// declared.
class CShadowManager : public WSingleton<CShadowManager> {
	LAYOUT_CHECK_FRIEND;
public:
	CShadowManager(WResourceManager *resrcmng, bool enable);
	virtual ~CShadowManager();

	CShadowCaster *RegisterCaster_New(const char *name, int size, const char *lightName, LightSet &light);
	void AddRecvPet_New(const char *name, WPuppet *pet, float dist);
	CShadowCaster *FindCaster(const char *name);
	void AddCastPet_New(const char *name, bool clear, WPuppet *pet);
	sShadowRecver *RegisterRecver_New(const char *name);
	bool IsEnabled() const;
	void Enable(bool enable);
	void SetActive(bool active);
	void UpdateLight(const char *lightName, LightSet &light);
	void Reset();
	void SetMinViewInfo(float minAngle, float minDist, bool use);
	void Reset_New();
	void Flush(WView *view);

protected:
	void UpdateBVolume(WView *view);
	void ClearCasters();
	void ClearRecvers();
	void ClearCasters_New();
	void ClearRecvers_New();
	sShadowRecver *FindCommonRecver(const char *name);

public:
	bool m_enabled;                        // 0x28
	int m_hwCaps;                          // 0x2C, WX_VDEV_GET_CAPS result is non-zero
	bool m_active;                         // 0x30
	WResourceManager *m_resrcmng;          // 0x34
	std::list<CShadowCaster *> m_casters;  // 0x38
	std::list<sShadowRecver *> m_recvers;  // 0x44
	float m_minViewAngle;                  // 0x50, at least 0.001 (SetMinViewInfo)
	float m_minViewDist;                   // 0x54, at least 0.001 (SetMinViewInfo)
	bool m_useMinView;                     // 0x58
};
CHECK_SIZE(CShadowManager, 0x5C);
BEGIN_LAYOUT_CHECK(CShadowManager)
	CHECK_FIELD(CShadowManager, m_enabled, 0x28);
	CHECK_FIELD(CShadowManager, m_hwCaps, 0x2C);
	CHECK_FIELD(CShadowManager, m_active, 0x30);
	CHECK_FIELD(CShadowManager, m_resrcmng, 0x34);
	CHECK_FIELD(CShadowManager, m_casters, 0x38);
	CHECK_FIELD(CShadowManager, m_recvers, 0x44);
	CHECK_FIELD(CShadowManager, m_minViewAngle, 0x50);
	CHECK_FIELD(CShadowManager, m_minViewDist, 0x54);
	CHECK_FIELD(CShadowManager, m_useMinView, 0x58);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SHADOW_H
