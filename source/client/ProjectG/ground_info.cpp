// Original: d:\build\custom\temp\code\client\projectg\ground_info.cpp
#include "ground_info.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "ClientSetting.h"
#include "TaskManager.h"
#include "golfball.h"
#include "club.h"
#include "../client/Wangreal/include/wpuppet.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wview.h"

#include <algorithm>

extern WMatrix g_camera;

// ground_info.cpp:19
CBase::CBase(void) : m_pCurBall(0), m_unknown58(0), m_lightDir(0.0f, 0.0f, 0.0f), m_color68(0), m_color6C(0), m_color70(0xFFFFFF), m_hConditionLine(0), m_hConditionFont(0) {
	float barY = COption::Instance()->gGetBar_Y();

	m_screenX = 58.0f;
	m_screenY = barY + 4.0f;

	Load();

	m_hConditionLine = g_resrcmng->LoadTexture("{condition_line.png", 0, 0, 0);
	m_hConditionFont = g_resrcmng->LoadTexture("[font_conditoin.jpg", 0, 0, 0);

	m_spin.Reset();
	m_unknownC0 = 0;
	m_angle = 0;

	m_uiMatrix.Reset();
	m_uiMatrix.pivot = WVector(0.0f, 0.0f, -1.8f) * (g_view->GetWidth() * 0.0015625f);

	m_unknown58 = 2;

	m_color6C = 0x808080;
	m_color68 = 0xFFFFFF;
	m_lightDir = WVector(-1.0f, -1.0f, 1.0f).Normalize();

	m_normal.Reset();
	m_matrix90.Reset();

	m_pSlots[0] = m_slot0;
	m_pSlots[1] = m_slot1;
	m_pSlots[2] = m_slot2;
	m_pSlots[3] = m_slot3;
	m_slotCount = 0;

	m_powerLoss = 0;
	m_lieIconIndex = 12;

	m_fxBox.Init(g_resrcmng);
	m_fxBox.SetCamera(&g_camera);

	// ground_info.cpp:57 stores 33.333f into m_fxBox.m_frameTime (CFxBox
	// 0x4), protected in FxBox.h.
}

// ground_info.cpp:61
CBase::~CBase(void) {
	for (std::list<sBall*>::iterator it = m_balls.begin(); it != m_balls.end(); ++it) {
		if (*it) {
			if (g_resrcmng != NULL && (*it)->m_pPuppet != NULL) { g_resrcmng->Release((*it)->m_pPuppet, false); (*it)->m_pPuppet = NULL; }
			delete *it;
		}
	}

	m_balls.clear();

	if (m_hConditionLine) {
		g_resrcmng->Release(m_hConditionLine);
		m_hConditionLine = 0;
	}

	if (m_hConditionFont) {
		g_resrcmng->Release(m_hConditionFont);
		m_hConditionFont = 0;
	}

	m_fxBox.ClearAll(false);
}

// ground_info.cpp:90
void CBase::Load(void) {
	for (std::list<sBall*>::iterator it = m_balls.begin(); it != m_balls.end(); ++it) {
		if (*it) {
			if (g_resrcmng != NULL && (*it)->m_pPuppet != NULL) { g_resrcmng->Release((*it)->m_pPuppet, false); (*it)->m_pPuppet = NULL; }
			delete *it;
		}
	}

	m_balls.clear();

	m_fxBox.ClearAll(false);

	for (unsigned char i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		if (PLAYER(i)->m_state != 3) {
			AddBall(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[i].m_ballId);
		}
	}
}

// ground_info.cpp:115
void CBase::ResetUiPosition(void) {
	float barY = COption::Instance()->gGetBar_Y();

	m_screenY = barY + 4.0f;
	m_fxBox.SetScreenCenter(m_screenX, m_screenY);

	m_uiMatrix.pivot = WVector(0.0f, 0.0f, -1.8f) * (g_view->GetWidth() * 0.0015625f);
}

// ground_info.cpp:126
void CBase::Process(float elapsed) {
	if (GOLFDOC()->m_gameState >= 12) {
		if (GolfBall().m_preDataIndex < 0) {
			AfxGetTask()->GetActor("Spin") << MsgObject(0, 0x161, (intptr_t)&m_spin.x, (intptr_t)&m_spin.y, 0, 0, 0);
			m_spin *= elapsed * 12.566371f;

			if (GolfClub().GetType() != 3) {
				m_matrix90 = m_matrix90 * (RotMat(m_spin.x, 1) * RotMat(m_spin.y, 0));
			}
		} else {
			m_matrix90 = RotMat(GolfBall().m_rotation[0], 1) * RotMat(GolfBall().m_rotation[1], 0);
		}

		m_unknownC0 += elapsed * 0.31415927f;

		elapsed *= 1000.0f;
		CFxBox* pFxBox = &m_fxBox;
		pFxBox->ProcessAttacher(elapsed);
		m_fxBox.Process(elapsed);
	}
}

// ground_info.cpp:303
WMatrix CBase::GetBaseMatrix(void) {
	WMatrix m;

	m.ya = m_normal; m.xa = WCrossProduct(m.ya, WVector::UNIT_POS_Z);
	m.za = WCrossProduct(m.xa, m.ya);

	m.xa.Normalize();
	m.za.Normalize();
	m.pivot.Reset();

	m = RotMat(m_unknownC0 * -2.0f, 1) * m * RotMat(m_angle * -1.0f, 1);

	return m;
}

// ground_info.cpp:321
CBase::sBall* CBase::ChangeBall(unsigned long typeId) {
	sBall* pBall = AddBall(typeId);

	m_fxBox.ClearAll(false);

	if (pBall) {
		m_pCurBall = pBall;
		AttachFx();
	}

	return pBall;
}

// ground_info.cpp:365
CBase::sBall* CBase::FindBall(unsigned long typeId) {
	std::list<sBall*>::iterator it = std::find_if(m_balls.begin(), m_balls.end(), sFindBall(typeId));

	if (it != m_balls.end()) {
		return *it;
	}
	return 0;
}

// ground_info.cpp:377
void CBase::ChangeTurn(void) {
	ChangeBall(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId);
}
