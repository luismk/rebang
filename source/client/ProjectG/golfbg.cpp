// Original: d:\build\custom\temp\code\client\projectg\golfbg.cpp
#include "golfbg.h"
#include "autotarget.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "scenemanager.h"
#include "quadtree.h"
#include "ClientSetting.h"
#include "GolfDoc.h"
#include "Shadow.h"
#include "sky.h"
#include "wresrcmng.h"
#include "../shared/classdefine.h"
#include "../shared/Localize.h"
#include "polysoup.h"
#include "golfball.h"

extern LightSet g_lightset;

// golfbg.cpp:20
IObject* CGolfBgMakeInstance(void) {
	return new CGolfBg;
}

// golfbg.cpp:20 (run-time type and factory registration of CGolfBg)
const WRTTI CGolfBg::m_RTTI("CGolfBg", &IActor::m_RTTI);

struct __impCGolfBg {
	__impCGolfBg(void) {
		ObjectFactory().AddObjectFunctor(CGolfBgMakeInstance, "CGolfBg");
		ActorFactory().AddActorClass("CGolfBg");
	}
};
static __impCGolfBg s_impCGolfBg;

// golfbg.cpp:23
CGolfBg::CGolfBg(void) {
	m_priority = 1;

	m_renderWeather = 0;
}

// golfbg.cpp:31
void CGolfBg::OnDestroy(void) {
	if (CShadowManager::Instance()) {
		delete CShadowManager::Instance();
	}
}

// golfbg.cpp:150
void CGolfBg::DoInitAfterLoad(void) {
	GOLFDOC()->m_pPolySoup->MakeDataStructure();
	SendMsg(this, "Camera", 0xEF, 0, 0, 0, 0);

	WPolySoup::sPoint *point;
	if (CSharedDoc::Instance()->m_pCurCourse && (CSharedDoc::Instance()->m_pCurCourse->courseFlags >> 4) == 1) {
		point = GOLFDOC()->m_pPolySoup->FindPoint("\xB1\xD7\xB8\xAE\xB5\xE5");
	} else {
		point = GOLFDOC()->m_pPolySoup->FindPoint("\xB1\xD7\xB8\xAE\xB5\xE5" "2");
		if (!point)
			point = GOLFDOC()->m_pPolySoup->FindPoint("\xB1\xD7\xB8\xAE\xB5\xE5");
	}
	SetGridCenter(point->m_pos);

	SetStartPoint(GOLFDOC()->GetHoleData().m_teePos);
	SetHolePoint(GOLFDOC()->GetHoleData().m_cupPos);

	SendMsg(this, "GolfBg", 0x142, GetWeather(), 0, 0, 0);

	if (GOLFDOC()->m_pSky) {
		GOLFDOC()->m_pSky->GenerateStars(-1);
		GOLFDOC()->m_pSky->GenerateLensFlare();
		GOLFDOC()->m_pSky->GenerateMoons();
	}

	CShadowManager::Instance()->UpdateLight("sun", g_lightset);

	if (!OnlinePlay()) {
		float radius = GolfBall().m_radius; WVector tee = GOLFDOC()->GetHoleData().m_teePos; GolfBall().Reset(WVector(tee.x, tee.y + radius, tee.z));
	}

	CSceneManager::Instance()->Load();

	GOLFDOC()->m_pAutoTarget->Reset();

	for (std::vector<WPolySoup::sBoundArea>::iterator it = GOLFDOC()->m_pPolySoup->m_boundAreas.begin(); it != GOLFDOC()->m_pPolySoup->m_boundAreas.end(); ++it) {
		if (it->m_type == 4) {
			WVector cup = GOLFDOC()->GetHoleData().m_cupPos;
			GOLFDOC()->m_pAutoTarget->SetList(it->m_points, cup);
		}
	}

	GOLFDOC()->m_pPolySoup->EndLoad(true);
}

// golfbg.cpp:208
void CGolfBg::OnPreLoadInit(void) {
	new CShadowManager(g_resrcmng, COption::Instance()->vIsShadowEnabled());
}

// golfbg.cpp:280
void CGolfBg::OnInit(void) {
	CSceneManager::Instance()->UpdateFog(true);
}

// golfbg.cpp:500
void CGolfBg::SetStartPoint(const WVector& point) {
	GOLFDOC()->GetHoleData().m_teePos = GetPVS().GetGroundPoint(point, false, &GolfBall().m_groundArea, false);
}

// golfbg.cpp:505
void CGolfBg::SetHolePoint(const WVector& point) {
	GOLFDOC()->GetHoleData().m_cupPos = GetPVS().GetGroundPoint(point, false, 0, false);
}

// golfbg.cpp:510
void CGolfBg::SetGridCenter(WVector& center) {
	center = GetPVS().GetGroundPoint(center, false, 0, false);

	SendMsg(this, "Grid", 0x74, (intptr_t)&center, 0, 0, 0);
	SendMsg(this, "GolfRule", 0x74, (intptr_t)&center, 0, 0, 0);
}

// golfbg.cpp:518
void CGolfBg::SetSkyColor(unsigned long color) {
	if (GOLFDOC()->m_pSky) {
		GOLFDOC()->m_pSky->SetColor(color);
	}
}

// golfbg.cpp:524
void CGolfBg::SetLowestPoint(void) {
	SendMsg(this, "Camera", 0xEF, 0, 0, 0, 0);
}

// golfbg.cpp:529
const char* CGolfBg::GetSkyBoxName(void) {
	if ((CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0) == 0) {
		return "blue";
	}

	bool halloween = IsLocalContent((localContentType_t)0x9A);

	if (!halloween) {
		return CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0;
	}
	return "Halloween";
}

// golfbg.cpp:549
void CGolfBg::SetRenderWeather(unsigned char weather) {
	m_renderWeather = weather;

	if (IsLocalContent((localContentType_t)0x9A)) {
		if (CSharedDoc::Instance()->m_roomType < 0xB) {
			if (m_renderWeather <= 0) {
				m_renderWeather = 1;
			}

			if (GOLFDOC()->m_pPolySoup) {
				GOLFDOC()->m_pPolySoup->ChangePointType(2, 3);
			}
		}
	}
}
