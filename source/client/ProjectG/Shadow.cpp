// Original: d:\build\custom\temp\code\client\projectg\shadow.cpp
#include "Shadow.h"
#include "wresrcmng.h"
#include "wvideo.h"
#include "wshadowview.h"
#include "wpuppet.h"
#include "westpak.h"

#include <float.h>
#include <math.h>
#include <string.h>
#include <set>
#include <string>

void __cdecl CloseCFile(cFile *file);

namespace {

// The pet names listed in club_noshadow.txt, one per line: they cast no
// shadow.
class DisableShadowPet {
public:
	static DisableShadowPet *Instance();

	bool Exist(const char *name) const;

private:
	DisableShadowPet();

	std::set<std::string> m_names; // 0x0
};

// shadow.cpp:36
DisableShadowPet *DisableShadowPet::Instance() {
	static DisableShadowPet _Singleton;
	return &_Singleton;
}

// shadow.cpp:41
bool DisableShadowPet::Exist(const char *name) const {
	if (name == 0) {
		return false;
	}

	std::string key(name);
	std::string::size_type pos = key.rfind('.');
	if (pos != std::string::npos) {
		key.erase(pos, key.length() - pos);
	}

	return m_names.end() != m_names.find(key);
}

// shadow.cpp:56
DisableShadowPet::DisableShadowPet() {
	cFile *file = g_resrcmng->GetCFile("club_noshadow.txt", 0xFFFF);
	if (file == 0) {
		return;
	}

	char *buf = new char[file->Length()];
	memset(buf, 0, file->Length());
	file->Read(buf, file->Length());

	std::string name;
	for (int i = 0; i < file->Length(); i++) {
		if (buf[i] == '\n' || buf[i] == '\r') {
			if (!name.empty()) {
				m_names.insert(name);
				name.clear();
			}
		} else {
			name += buf[i];
		}
	}

	if (!name.empty()) {
		m_names.insert(name);
	}

	delete[] buf;
	CloseCFile(file);
}

} // namespace

// shadow.cpp:88
CShadow::CShadow(WResourceManager *resrcmng, eShadowMode mode)
	: m_shadowView(0), m_recvView(0), m_shadowType(SHADOW_TYPE_PROJECTION) {
	switch (mode) {
	case SHADOW_MODE_SOFTWARE:
		m_shadowView = new WShadowViewSw(0x60);
		m_recvView = new WRecvViewSw;
		break;
	case SHADOW_MODE_HARDWARE:
		m_shadowView = new WShadowViewHw(0x60);
		m_recvView = new WRecvViewHw;
		break;
	default:
		m_shadowView = new WShadowViewHw(0x60);
		m_recvView = new WRecvViewHw;
		break;
	}

	m_shadowView->SetResourceManager(resrcmng);
	if (mode == SHADOW_MODE_SOFTWARE) {
		m_shadowView->SetTextureSize(0x80, 0x80);
	} else {
		int size[2];
		size[0] = 0x100;
		size[1] = 0x100;
		resrcmng->VideoReference()->Command(WX_VDEV_GET_CAPS, 3, (intptr_t)size);
		m_shadowView->SetTextureSize(size[0] > 0x800 ? 0x800 : size[0], size[1] > 0x800 ? 0x800 : size[1]);
	}
	m_recvView->SetResourceManager(resrcmng);
}

// shadow.cpp:129
CShadow::~CShadow() {
	if (m_shadowView) { delete m_shadowView; m_shadowView = 0; }
	if (m_recvView) { delete m_recvView; m_recvView = 0; }
}

// shadow.cpp:139
void CShadow::SetShadowType(ShadowType type) {
	m_shadowType = type;
}

// shadow.cpp:150
void CShadow::RenderCaster(WView *view, const LightSet &light, const WSphere &bsphere, std::list<WPuppet *> &pets, sShadowRecver *recver, bool update) {
	if (m_shadowType != SHADOW_TYPE_NONE) {
		switch (m_shadowType) {
		case SHADOW_TYPE_PROJECTION:
			RenderProjectionShadow(view, light, bsphere, pets, recver, update);
			break;
		}

		view->SetCamera(view->camera);
		view->SetClip(view->GetClipNearValue(), view->GetClipFarValue(), false);
		view->UpdateCamera();
	}
}

// shadow.cpp:217
void CShadow::RenderProjectionShadow(WView *view, const LightSet &light, const WSphere &bsphere, std::list<WPuppet *> &pets, sShadowRecver *recver, bool update) {
	if (!Setup(view, light, bsphere, recver)) {
		return;
	}

	if (!update) {
		return;
	}

	if (m_shadowView == 0) {
		return;
	}

	for (std::list<WPuppet *>::iterator it = pets.begin(); it != pets.end(); ++it) {
		WPuppet *pet = *it;
		if (pet) {
			if (!DisableShadowPet::Instance()->Exist(pet->GetPuppetName())) {
				m_shadowView->Render(pet, true, 1.0f, false, false);
			}
		}
	}

	m_shadowView->Render();
}

// shadow.cpp:359
bool CShadow::Setup(WView *view, const LightSet &light, const WSphere &bsphere, sShadowRecver *recver) {
	WMatrix cam;
	WVector eye;
	float dist;

	if (light.type == LightSet::W_LIGHT_DIRECTIONAL) {
		float t = bsphere.radius + 1000000.0f;
		eye.x = bsphere.pos.x - light.nearone.x * t;
		eye.y = bsphere.pos.y - light.nearone.y * t;
		eye.z = bsphere.pos.z - light.nearone.z * t;
		cam = MakeCameraMatrix(eye, bsphere.pos);
		dist = t;
	} else {
		cam = MakeCameraMatrix(light.nearone, bsphere.pos);
		WVector local = bsphere.pos * ~cam;
		dist = local.y * WVector::UNIT_POS_Z.y + local.z * WVector::UNIT_POS_Z.z + WVector::UNIT_POS_Z.x * local.x;
	}

	float nearDist = dist * 0.5f;
	float farDist = dist + bsphere.radius;
	float fov = (float)(asin(bsphere.radius / dist) * 2.0);

	if (nearDist > farDist) {
		return false;
	}

	if (!CheckFrustum(view, cam, fov, nearDist, farDist)) {
		return false;
	}

	m_recvView->SetRenderView(view);
	fov = fov * (4.0f / 3.0f);
	m_recvView->SetFOV(fov);
	m_recvView->SetCamera(cam);
	m_recvView->SetClip(nearDist, farDist, false);
	m_recvView->BeginScene();
	m_recvView->Clear(0, 6);
	m_recvView->CollectRecvTris(recver->m_pet, true, 1.0f, false, false);
	m_recvView->EndScene();
	if (!m_recvView->HasRecver()) {
		return false;
	}

	m_shadowView->SetFOV(fov);
	m_shadowView->SetCamera(cam);
	m_shadowView->SetClip(nearDist, farDist, false);
	m_shadowView->BeginScene();
	m_shadowView->Clear(0, 6);
	return true;
}

// shadow.cpp:443
void CShadow::RenderRecver(sShadowRecver *recver) {
	m_recvView->Render(m_shadowView, recver->m_pet);
}

// shadow.cpp:493
CShadowCaster::CShadowCaster(const char *name, int state, WResourceManager *resrcmng, eShadowMode mode) {
	strcpy(m_name, name);
	m_state = state | 2;

	m_shadow = new CShadow(resrcmng, mode);
}

// shadow.cpp:502
CShadowCaster::~CShadowCaster() {
	if (m_shadow) { delete m_shadow; m_shadow = 0; }
}

// shadow.cpp:508
const char *CShadowCaster::GetName() const {
	return m_name;
}

// shadow.cpp:513
const char *CShadowCaster::GetLightName() const {
	return m_lightName;
}

// shadow.cpp:518
void CShadowCaster::Reset() {
	m_state &= ~0xC;
}

// shadow.cpp:527
int CShadowCaster::GetState() const {
	return m_state;
}

// shadow.cpp:542
void CShadowCaster::SetState_New(int state) {
	m_state = state;
}

// shadow.cpp:556
void CShadowCaster::ClearCaster() {
	m_pets.clear();
	m_state |= 2;
}

// shadow.cpp:562
void CShadowCaster::ClearCaster_New() {
	m_pets.clear();

	m_state = 2;
}

// shadow.cpp:569
void CShadowCaster::SetLight(const char *lightName, LightSet &light) {
	if (lightName) {
		strcpy(m_lightName, lightName);
	}

	m_light = light;
	m_state |= 1;
}

// shadow.cpp:581
void CShadowCaster::AddPet(WPuppet *pet) {
}

// shadow.cpp:590
void CShadowCaster::AddPet_New(WPuppet *pet) {
	m_pets.push_back(pet);
	m_state &= ~2;
}

// shadow.cpp:597
void CShadowCaster::DeletePet(WPuppet *pet) {
	for (std::list<WPuppet *>::iterator it = m_pets.begin(); it != m_pets.end(); ++it) {
		if (*it == pet && _stricmp((*it)->GetPuppetName(), pet->GetPuppetName()) == 0) {
			m_pets.erase(it);

			if (m_pets.size() == 0) {
				m_state |= 2;
			}
			return;
		}
	}
}

// shadow.cpp:615
void CShadowCaster::ReplacePets(WPuppet *oldPet, WPuppet *newPet, bool show) {
	if (oldPet == 0) {
		if (m_pets.size() == 0) {
			if (newPet) {
				m_pets.push_back(newPet);
				m_state &= ~2;
			}
		} else if (m_pets.size() == 1) {
			std::list<WPuppet *>::iterator it = m_pets.begin();

			if (newPet) {
				*it = newPet;
			} else {
				m_pets.erase(it);
				m_state |= 2;
			}
		}
	} else {
		for (std::list<WPuppet *>::iterator it = m_pets.begin(); it != m_pets.end(); ++it) {
			if (*it == oldPet && _stricmp((*it)->GetPuppetName(), oldPet->GetPuppetName()) == 0) {
				if (newPet) {
					*it = newPet;
				} else {
					m_pets.erase(it);

					if (m_pets.size() == 0) {
						m_state |= 2;
					}
				}
				break;
			}
		}
	}

	m_state &= ~0x80;

	if (show) {
		m_state &= ~0x20;
	} else {
		m_state |= 0x20;
	}
}

// shadow.cpp:679
void CShadowCaster::UpdateBSphere(WView *view, float minAngle, float minDist) {
	if (m_state & 0x22) {
		return;
	}

	float ratio = view->GetRatio();
	float fov = view->GetFOV();
	if (fov < minAngle) {
		fov = minAngle;
	}
	float nearDist = view->GetClipNearValue();
	float farDist = nearDist + minDist;

	float tanY = (float)tan(fov * 0.5f);
	float tanX = (float)tan(ratio * fov * 0.5f);

	float nearX = tanX * nearDist;
	float nearY = tanY * nearDist;
	float farX = tanX * farDist;
	float farY = tanY * farDist;

	float farSq = farX * farX + farY * farY + farDist * farDist;
	float nearSq = nearX * nearX + nearY * nearY + nearDist * nearDist;
	float c = (farSq - nearSq) / (minDist * 2.0f);

	m_bsphere.pos.x = c * view->camera.za.x + view->camera.pivot.x;
	m_bsphere.pos.y = c * view->camera.za.y + view->camera.pivot.y;
	m_bsphere.pos.z = c * view->camera.za.z + view->camera.pivot.z;

	float g = farDist - c;
	m_bsphere.radius = sqrtf(g * g + farX * farX + farY * farY);
}

// shadow.cpp:840
void CShadowCaster::RenderCaster(WView *view, sShadowRecver *recver) {
	if (!(m_state & 0x40) && m_bsphere.radius < 8.0f) {
		m_bsphere.radius = 8.0f;
	}

	if ((m_state & 0x20) && (m_state & 0x80)) {
		m_shadow->RenderCaster(view, m_light, m_bsphere, m_pets, recver, false);
	} else {
		m_state |= 0x80;

		m_shadow->RenderCaster(view, m_light, m_bsphere, m_pets, recver, true);
	}
}

// shadow.cpp:865
void CShadowCaster::RenderRecver(sShadowRecver *recver) {
	m_shadow->RenderRecver(recver);
}

// shadow.cpp:870
void CShadowCaster::ShowCaster(bool show) {
	if (show) {
		m_state &= ~0x20;
	} else {
		m_state |= 0x20;
	}
}

// shadow.cpp:881
CShadowManager::CShadowManager(WResourceManager *resrcmng, bool enable)
	: m_enabled(enable), m_active(enable), m_resrcmng(resrcmng) {
	int caps;
	resrcmng->VideoReference()->Command(WX_VDEV_GET_CAPS, 1, (intptr_t)&caps);

	m_minViewAngle = 1.5707964f;
	m_minViewDist = 200.0f;
	m_useMinView = true;
	m_hwCaps = caps != 0;
}

// shadow.cpp:899
CShadowManager::~CShadowManager() {
	ClearCasters();
	ClearRecvers();
}

// shadow.cpp:905
void CShadowManager::ClearCasters() {
	for (std::list<CShadowCaster *>::iterator it = m_casters.begin(); it != m_casters.end(); ++it) {
		delete (*it);
	}

	m_casters.clear();
}

// shadow.cpp:915
void CShadowManager::ClearRecvers() {
	for (std::list<sShadowRecver *>::iterator it = m_recvers.begin(); it != m_recvers.end(); ++it) {
		delete (*it);
		(*it) = 0;
	}

	m_recvers.clear();
}

// shadow.cpp:928
void CShadowManager::ClearCasters_New() {
	for (std::list<CShadowCaster *>::iterator it = m_casters.begin(); it != m_casters.end(); ++it) {
		(*it)->Reset();
		(*it)->ClearCaster_New();
	}
}

// shadow.cpp:939
void CShadowManager::ClearRecvers_New() {
	for (std::list<sShadowRecver *>::iterator it = m_recvers.begin(); it != m_recvers.end(); ++it) {
		(*it)->ClearRecver();
	}
}

// shadow.cpp:947
bool CShadowManager::IsEnabled() const {
	return m_enabled;
}

// shadow.cpp:952
void CShadowManager::Enable(bool enable) {
	m_enabled = enable;
}

// shadow.cpp:980
CShadowCaster *CShadowManager::RegisterCaster_New(const char *name, int size, const char *lightName, LightSet &light) {
	const char *casterName = g_theCasterName;

	CShadowCaster *caster = FindCaster(casterName);

	if (caster == 0) {
		caster = new CShadowCaster(casterName, 0, m_resrcmng, (eShadowMode)m_hwCaps);
		caster->SetLight(lightName, light);

		m_casters.push_back(caster);
	} else {
		caster->SetLight(lightName, light);
	}

	return caster;
}

// shadow.cpp:1028
void CShadowManager::AddCastPet_New(const char *name, bool clear, WPuppet *pet) {
	CShadowCaster *caster = FindCaster(name);

	if (caster) {
		if (clear) {
			caster->ClearCaster();
		}
		caster->AddPet_New(pet);
	}
}

// shadow.cpp:1094
CShadowCaster *CShadowManager::FindCaster(const char *name) {
	for (std::list<CShadowCaster *>::iterator it = m_casters.begin(); it != m_casters.end(); ++it) {
		if (strcmp((*it)->m_name, g_theCasterName) == 0) {
			return *it;
		}
	}

	return 0;
}

// shadow.cpp:1133
sShadowRecver *CShadowManager::RegisterRecver_New(const char *name) {
	sShadowRecver *recver = FindCommonRecver(name);

	if (recver == 0) {
		recver = new sShadowRecver;

		strcpy(recver->m_name, name);
		m_recvers.push_back(recver);
	}

	return recver;
}

// shadow.cpp:1170
void CShadowManager::AddRecvPet_New(const char *name, WPuppet *pet, float dist) {
	sShadowRecver *recver = FindCommonRecver(name);

	if (recver) {
		recver->m_pet = pet;
		recver->m_dist = dist;
	}
}

// shadow.cpp:1185
sShadowRecver *CShadowManager::FindCommonRecver(const char *name) {
	for (std::list<sShadowRecver *>::iterator it = m_recvers.begin(); it != m_recvers.end(); ++it) {
		if (strcmp((*it)->m_name, name) == 0) {
			return *it;
		}
	}

	return 0;
}

// shadow.cpp:1198
void CShadowManager::Flush(WView *view) {
	if (!m_enabled || !m_active) {
		return;
	}

	for (std::list<CShadowCaster *>::iterator it = m_casters.begin(); it != m_casters.end(); ++it) {
		(*it)->Reset();
	}

	UpdateBVolume(view);

	for (std::list<CShadowCaster *>::iterator cit = m_casters.begin(); cit != m_casters.end(); ++cit) {
		if (((*cit)->GetState() & 0x2E) == 0) {
			for (std::list<sShadowRecver *>::iterator rit = m_recvers.begin(); rit != m_recvers.end(); ++rit) {
				(*cit)->RenderCaster(view, *rit);
				(*cit)->RenderRecver(*rit);
			}
		}
	}

	view->UpdateCamera();
}

// shadow.cpp:1237
void CShadowManager::SetActive(bool active) {
	m_active = active;
}

// The original's max compiles as `a < b ? b : a` (header unknown, as in river.cpp).
template <class T> T MaxOf(T a, T b) { return a < b ? b : a; }

// shadow.cpp:1246
void CShadowManager::UpdateBVolume(WView *view) {
	float minDist;



	if (m_useMinView && WisEqual(view->camera.za, WVector::UNIT_NEG_Y, 1e-5f)) {



		if (m_recvers.size() > 0) {

			sShadowRecver *recver = m_recvers.front();

			minDist = MaxOf(view->camera.ym - recver->m_dist, m_minViewDist);

		} else
			minDist = m_minViewDist;

	} else
		minDist = m_minViewDist;




	for (std::list<CShadowCaster *>::iterator it = m_casters.begin(); it != m_casters.end(); ++it) {

		(*it)->UpdateBSphere(view, m_minViewAngle, minDist);
	}
}

// shadow.cpp:1305
void CShadowManager::UpdateLight(const char *lightName, LightSet &light) {
	for (std::list<CShadowCaster *>::iterator it = m_casters.begin(); it != m_casters.end(); ++it) {
		CShadowCaster *caster = *it;
		if (strcmp(caster->m_lightName, lightName) == 0) {
			caster->m_light = light;
			caster->m_state |= 1;
		}
	}
}

// shadow.cpp:1316
void CShadowManager::Reset() {
	ClearCasters();
	ClearRecvers();
}

// shadow.cpp:1322
void CShadowManager::Reset_New() {
	ClearCasters_New();
	ClearRecvers_New();
}

// shadow.cpp:1343
void CShadowManager::SetMinViewInfo(float minAngle, float minDist, bool use) {
	m_minViewAngle = minAngle < 0.001f ? 0.001f : minAngle;
	m_minViewDist = minDist < 0.001f ? 0.001f : minDist;
	m_useMinView = use;
}
