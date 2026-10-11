// Original: d:\build\custom\temp\code\client\projectg\ball.cpp
#include "ball.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "golfball.h"
#include "../client/Wangreal/include/wpuppet.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "scenemanager.h"
#include "fx.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "../client/Wangreal/include/wutil.h"
#include "../client/Wangreal/include/wview.h"
#include "commonutil.h"
#include "QuadTree.h"
#include "ProjectG.h"

#include <algorithm>

extern WMatrix g_camera;

// Nameless WVector the balls take as the light position (0xAA5278).
extern WVector data_00aa5278;

// The ball's light colours (diffuse 0xAA5284, ambient 0xAA5288) and the ratio
// applied to the ambient one.
extern unsigned long data_00aa5284;
extern unsigned long data_00aa5288;
extern float BALL_AMBIENT_RATIO;

// ball.cpp:20 (run-time type and factory registration of CBall)
const WRTTI CBall::m_RTTI("CBall", &IActor::m_RTTI);

IObject* CBallMakeInstance(void) {
	return new CBall;
}

struct __impCBall {
	__impCBall(void) {
		ObjectFactory().AddObjectFunctor(CBallMakeInstance, "CBall");
		ActorFactory().AddActorClass("CBall");
	}
};
static __impCBall s_registerCBall;

// ball.cpp:27
CBall::CBall(void) : m_pPuppet(0), m_hShadowTexture(0) {
	m_bVisible = true;
	m_priority = 1;
	m_lastPreDataIndex = -1;
}

// ball.cpp:32
void CBall::OnPreLoadInit(void) {
	m_axis = WVector::UNIT_POS_X;

	for (int i = 0; i < 4; i++) {
		m_shadowVertices[i].diffuse = 0x80ffffff;
		m_shadowVertices[i].tu = (i & 2) ? 1.0f : 0.0f;
		m_shadowVertices[i].tv = (i & 1) ? 0.0f : 1.0f;
	}

	m_shadowPolygon[0] = &m_shadowVertices[0];
	m_shadowPolygon[1] = &m_shadowVertices[1];
	m_shadowPolygon[2] = &m_shadowVertices[2];
	m_shadowPolygon[3] = &m_shadowVertices[3];
	m_shadowPolygon[4] = NULL;
	m_light.type = LightSet::W_LIGHT_DIRECTIONAL;
	m_hShadowTexture = g_resrcmng->LoadTexture("[ball_shadow.jpg", 0, 0, NULL);

	CSceneManager::Instance()->RegisterElement((eObjType)4, NULL, NULL, "\xB0\xF8", 8, NULL);

	CSceneManager::Instance()->RegisterElement((eObjType)1, NULL, this, "\xB0\xF8\xB1\xD7\xB8\xB2\xC0\xDA", 0, NULL);
}

// ball.cpp:62
void CBall::OnDestroy(void) {
	for (std::list<sBall*>::iterator it = m_balls.begin(); it != m_balls.end(); ++it) {
		if (*it) {
			if (g_resrcmng != NULL && (*it)->m_pPuppet != NULL) { g_resrcmng->Release((*it)->m_pPuppet, false); (*it)->m_pPuppet = NULL; }
			if (*it != NULL) { delete *it; *it = NULL; }
		}
	}

	m_balls.clear();

	if (g_resrcmng != NULL && m_hShadowTexture != 0) { g_resrcmng->Release(m_hShadowTexture); m_hShadowTexture = 0; }
}

// ball.cpp:105
void CBall::OnReload(void) {
	for (std::list<sBall*>::iterator it = m_balls.begin(); it != m_balls.end(); ++it) {
		if (*it) {
			if (g_resrcmng != NULL && (*it)->m_pPuppet != NULL) { g_resrcmng->Release((*it)->m_pPuppet, false); (*it)->m_pPuppet = NULL; }
			if (*it != NULL) { delete *it; *it = NULL; }
		}
	}

	m_balls.clear();

	CSceneManager::Instance()->SetPuppet("\xB0\xF8", NULL);

	OnLoad();
}

// ball.cpp:124
void CBall::OnInit(void) {
	m_matrix.Reset();
	m_sinkFactor = 0;
	m_sinkStartIndex = -1;
	m_light.nearone = data_00aa5278;
}

// ball.cpp:132
CBall::sBall* CBall::AddBall(unsigned long typeId) {
	sBall* pBall = FindBall(typeId);

	if (pBall)
		return pBall;

	IFF_STRUCT::sBall* pData = ItemManager()->FindBall(typeId);

	if (pData) {
		pBall = new sBall;
		pBall->m_pData = pData;
		char name[40];
		sprintf(name, "%s.pet", pData->m_model);
		pBall->m_pPuppet = g_resrcmng->GetPuppet(name, false, true, false);

		m_balls.push_back(pBall);
	}

	return pBall;
}

// ball.cpp:161
CBall::sBall* CBall::FindBall(unsigned long typeId) {
	std::list<sBall*>::iterator it = std::find_if(m_balls.begin(), m_balls.end(), sFindBall(typeId));

	if (it != m_balls.end()) {
		return *it;
	}
	return 0;
}

// ball.cpp:173
float CBall::GetDownDelta(void) {
	if (m_sinkStartIndex == GolfBall().m_preDataCount)
		return 1.0f;

	float delta = (float)(GolfBall().m_preDataIndex - m_sinkStartIndex);
	return (0.0f < delta ? delta : 0.0f) / (float)(GolfBall().m_preDataCount - m_sinkStartIndex);
}

// ball.cpp:301
void CBall::TransformBall(void) {
	int preDataIndex = GolfBall().m_preDataIndex;
	float scale;

	if (preDataIndex < 0) {
		scale = 1.0f;
	} else {
		// The ball's apparent height on screen: keep it between 0.1 and 1.5.
		WVector bottom;
		WVector top;
		bottom = g_view->Projection(GolfBall().m_pos - g_camera.ya * GolfBall().m_radius);
		top = g_view->Projection(GolfBall().m_pos + GolfBall().m_radius * g_camera.ya);
		scale = 1.5f / Between(0.1f, Abs(bottom.y - top.y), 1.5f);
	}

	if (m_pPuppet) {
		m_pPuppet->ApplyBones(NULL, m_matrix * scale);
		m_pPuppet->Transform(NULL, 1.0f, 0);
		m_pPuppet->UpdateBSphere();
	}
}

// ball.cpp:337
void CBall::OnDisplay(void) {
	if (!m_bVisible) {
		return;
	}

	TransformBall();

	if (m_pPuppet && !m_pPuppet->GetAlpha(0)) {
		return;
	}

	if (GolfBall().m_state == 0 && !m_bRefreshLight) {
		return;
	}

	m_bRefreshLight = false;
	if (m_pPuppet) {
		m_pPuppet->UpdateLightSource(&m_light, false, 0);
	}
}

// Closes the spray and the sequence of one effect slot of a ball.
static void CloseBallFx(CBall::sBall* pBall, int slot, bool flag) {
	if (pBall == 0) {
		return;
	}
	if (pBall->m_pSpray[slot]) {
		WSingleton<CFx>::Instance()->CloseSpray(pBall->m_pSpray[slot]);
		pBall->m_pSpray[slot] = 0;
	}
	if (pBall->m_pSequence[slot]) {
		WSingleton<CFx>::Instance()->CloseSequence(pBall->m_pSequence[slot], flag);
		pBall->m_pSequence[slot] = 0;
	}
}

// ball.cpp:402
void CBall::HandleMsg(const MsgObject& msg) {
	if ((unsigned long)msg.m_reserved > g_CurrentTime) {
		StoreDelayedMsg(msg);
		return;
	}

	sBall* pBall;

	switch (msg.m_msg) {
	case 0x163:
		m_bVisible = true;
		WSingleton<CSceneManager>::Instance()->SetVisible("공", true);
		break;
	case 0x164:
		m_bVisible = false;
		WSingleton<CSceneManager>::Instance()->SetVisible("공", false);
		break;
	case 0x1EB:
		m_bRefreshLight = true;
		m_light.diffuse = ModifyColor(data_00aa5284, GetPVS().GetShadowIntensity(GolfBall().m_pos));
		m_light.ambient = ModifyColor(data_00aa5288, BALL_AMBIENT_RATIO);
		break;
	case 0x1ED:
		switch (GetPVS().GetGroundType(GolfBall().m_pos, true, 0)) {
		case 3:
		case 4:
		case 5:
			m_sinkFactor = 1.0f - GetPVS().GetMinPowerOnGround(GolfBall().m_pos);
			break;
		case 7:
		case 8:
			break;
		default:
			m_sinkFactor = 0.0f;
			break;
		}
		break;
	case 0x1EC:
		switch (GetPVS().GetGroundType(GolfBall().PreData(GolfBall().m_preDataCount)->m_pos, true, 0)) {
		case 3:
		case 4:
		case 5:
			m_sinkFactor = 1.0f - GetPVS().GetMinPowerOnGround(GolfBall().PreData(GolfBall().m_preDataCount)->m_pos);
			m_sinkStartIndex = GolfBall().m_preDataCount - 100;
			if (m_sinkStartIndex < GolfBall().m_eventPreDataIndex[5]) {
				m_sinkStartIndex = GolfBall().m_eventPreDataIndex[5];
			}
			break;
		case 7:
		case 8:
			break;
		default:
			m_sinkFactor = 0.0f;
			m_sinkStartIndex = -1;
			break;
		}
		break;
	case 0x4C:
		pBall = FindBall(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId);
		CloseBallFx(pBall, 0, true);
		pBall = FindBall(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId);
		CloseBallFx(pBall, 1, false);
		pBall = AddBall(msg.m_param1);
		if (pBall) {
			WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId = msg.m_param1;
		m_pPuppet = pBall->m_pPuppet;
		if (m_pPuppet) {
			m_pPuppet->UpdateLightSource(&m_light, false, 0);
		}
		WSingleton<CSceneManager>::Instance()->SetPuppet("공", m_pPuppet);
		AttachFx(0);
		AttachFx(1);
		}
		break;
	case 0x4D:
		pBall = FindBall(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId);
		CloseBallFx(pBall, 0, true);
		pBall = FindBall(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId);
		CloseBallFx(pBall, 1, false);
		pBall = AddBall(msg.m_param1);
		if (pBall) {
			WSingleton<CSharedDoc>::Instance()->m_roomPlayers[msg.m_param2].m_ballId = msg.m_param1;
		m_pPuppet = pBall->m_pPuppet;
		if (m_pPuppet) {
			m_pPuppet->UpdateLightSource(&m_light, false, 0);
		}
		WSingleton<CSceneManager>::Instance()->SetPuppet("공", m_pPuppet);
		AttachFx(0);
		AttachFx(1);
		}
		break;
	case 0x1A0:
		pBall = FindBall(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId);
		CloseBallFx(pBall, 0, true);
		pBall = FindBall(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId);
		CloseBallFx(pBall, 1, false);
		m_lastPreDataIndex = -1;
		pBall = FindBall(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId);
		if (pBall) {
		m_pPuppet = pBall->m_pPuppet;
		if (m_pPuppet) {
			m_pPuppet->UpdateLightSource(&m_light, false, 0);
		}
		WSingleton<CSceneManager>::Instance()->SetPuppet("공", m_pPuppet);
		AttachFx(0);
		AttachFx(1);
		}
		break;
	case 0x1EE:
		pBall = FindBall(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId);
		if (pBall) {
			AttachFx(0);
			AttachFx(1);
			pBall->m_pSpray[2] = 0;
			pBall->m_pSpray[3] = 0;
			pBall->m_pSequence[2] = 0;
			pBall->m_pSequence[3] = 0;
		}
		break;
	case 0x1EF:
		AttachFx(2);
		AttachFx(3);
		break;
	}
}

// ball.cpp:656
void CBall::DetachFx(int index) {
	if (index != 0 && index != 1 && index != 3 && index != 5) {
		return;
	}

	sBall* pBall = FindBall(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId);

	if (pBall == 0) {
		return;
	}

	int slot;
	switch (index) {
	case 0:
		slot = 0;
		break;
	case 1:
		slot = 1;
		break;
	case 3:
		slot = 2;
		break;
	case 5:
		slot = 3;
		break;
	default:
		return;
	}

	if (pBall->m_pSpray[slot]) {
		WSingleton<CFx>::Instance()->CloseSpray(pBall->m_pSpray[slot]);
		pBall->m_pSpray[slot] = 0;
	}

	if (pBall->m_pSequence[slot]) {
		if (slot == 0) {
			WSingleton<CFx>::Instance()->CloseSequence(pBall->m_pSequence[0], true);
		} else {
			WSingleton<CFx>::Instance()->CloseSequence(pBall->m_pSequence[slot], false);
		}
		pBall->m_pSequence[slot] = 0;
	}
}

// ball.cpp:715
void CBall::EmptyFx(void* pFx) {
	if (pFx == 0) {
		return;
	}

	for (std::list<sBall*>::iterator it = m_balls.begin(); it != m_balls.end(); ++it) {
		sBall* pBall = *it;

		if (pBall == 0) {
			continue;
		}

		for (int i = 0; i < 4; i++) {
			if (pBall->m_pSpray[i] == pFx) {
				pBall->m_pSpray[i] = 0;
			}
			if (pBall->m_pSequence[i] == pFx) {
				pBall->m_pSequence[i] = 0;
			}
		}
	}
}
