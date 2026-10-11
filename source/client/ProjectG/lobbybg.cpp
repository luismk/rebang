// Original: d:\build\custom\temp\code\client\projectg\lobbybg.cpp
#include "lobbybg.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "TaskManager.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "ClientSetting.h"
#include "scenemanager.h"
#include "Shadow.h"
#include "sky.h"
#include "wind.h"
#include "wresrcmng.h"
#include "wview.h"
#include "fx.h"
#include "commonutil.h"
#include "quadtree.h"

extern bool g_bQuit;
extern unsigned long g_CurrentTime;
extern LightSet g_lightset;

// lobbybg.cpp:41 (run-time type and factory registration of ntLobbyBg)
const WRTTI ntLobbyBg::m_RTTI("ntLobbyBg", &IActor::m_RTTI);

IObject* ntLobbyBgMakeInstance(void) {
	return new ntLobbyBg;
}

struct __impntLobbyBg {
	__impntLobbyBg(void) {
		ObjectFactory().AddObjectFunctor(ntLobbyBgMakeInstance, "ntLobbyBg");
		ActorFactory().AddActorClass("ntLobbyBg");
	}
};
static __impntLobbyBg s_impntLobbyBg;

// lobbybg.cpp:45
void ntLobbyBg::OnPreLoadInit(void) {
	new CShadowManager(g_resrcmng, COption::Instance()->vIsShadowEnabled());

	if (LOBBYBG_MAP == 13) {
		g_view->SetClip(0.1f, 26214.4f, true);
	}
}

// lobbybg.cpp:60
void ntLobbyBg::OnLoad(void) {
	if (CFx::Instance()) {
		CFx::Instance()->SetActive(false);
	}

	if (!GOLFDOC()->m_pPolySoup->Load(MakeStr("%s_rank.gbin", LOBBYBG_MAPNAME), false)) {
		g_bQuit = true;
		return;
	}

	GOLFDOC()->m_waveInfo.Load(MakeStr("%s_wave.txt", LOBBYBG_MAPNAME));
	GOLFDOC()->m_riverInfo.Load(MakeStr("%s_river.txt", LOBBYBG_MAPNAME));

	GOLFDOC()->m_pSky = new CSky;
	GOLFDOC()->m_pSky->LoadSky(LOBBYBG_MAPNAME, 0.0f);
	GOLFDOC()->m_pSky->RegisterCloud(MakeStr("%s_cloud.dds", LOBBYBG_MAPNAME));

	GOLFDOC()->m_pPolySoup->MakeDataStructure();

	if (GOLFDOC()->m_pSky) {
		GOLFDOC()->m_pSky->GenerateStars(LOBBYBG_MAP);
		GOLFDOC()->m_pSky->GenerateLensFlare();
		GOLFDOC()->m_pSky->GenerateMoons();
	}

	CShadowManager::Instance()->UpdateLight("sun", g_lightset);

	CSceneManager::Instance()->Load();

	GOLFDOC()->m_pPolySoup->EndLoad(false);

	if (CFx::Instance()) {
		CFx::Instance()->SetActive(true);
	}
}

// lobbybg.cpp:104
void ntLobbyBg::OnInit(void) {
	CSceneManager::Instance()->UpdateFog(LOBBYBG_MAPNAME);

	Wind().CheckSpecialWind();
}

// lobbybg.cpp:112
void ntLobbyBg::OnDestroy(void) {
	if (CShadowManager::Instance()) {
		delete CShadowManager::Instance();
	}
}

// lobbybg.cpp:118
void ntLobbyBg::HandleMsg(const MsgObject& msg) {
	if ((unsigned long)msg.m_reserved > g_CurrentTime) {
		StoreDelayedMsg(msg);
		return;
	}

	switch (msg.m_msg) {
	case 317:
		GOLFDOC()->m_pSky->GenerateClouds(50, Wind().GetGlobalIntensity());
		break;

	case 318:
		GOLFDOC()->m_pSky->LoadSky(LOBBYBG_MAPNAME, 0.0f);
		GOLFDOC()->m_pSky->RegisterCloud(MakeStr("%s_cloud.dds", LOBBYBG_MAPNAME));
		GOLFDOC()->m_pSky->GenerateStars(LOBBYBG_MAP);
		GOLFDOC()->m_pSky->GenerateMoons();
		GOLFDOC()->m_pSky->GenerateLensFlare();
		break;

	case 319:
		GOLFDOC()->m_pSky->SetColor(msg.m_param1);
		break;

	case 320:
		DoReLoad();
		break;

	case 280:
		GOLFDOC()->m_pPolySoup->SetRenderMode(msg.m_param1);
		break;

	case 321:
		if (GOLFDOC()->m_pPolySoup) {
			GOLFDOC()->m_pPolySoup->UpdateBgFxWind();
		}
		break;
	}
}

// lobbybg.cpp:162
ntLobbyBg::ntLobbyBg(void) {
	m_priority = 1;
}

// lobbybg.cpp:168
void ntLobbyBg::DoReLoad(void) {
	SendMsg(this, "Ambient", 500, 0, 0, 0, 0);

	OnLoad();

	CTaskManager *pTaskManager = CTaskManager::Instance();
	(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->Init(0);
}

// lobbybg.cpp:178
void ntLobbyBg::SetSkyColor(unsigned long color) {
	if (GOLFDOC()->m_pSky) {
		GOLFDOC()->m_pSky->SetColor(color);
	}
}

// lobbybg.cpp:275
void ntLobbyBg::OnProcess(float elapsed) {
	GetPVS().PreProcess(elapsed, false);
}
