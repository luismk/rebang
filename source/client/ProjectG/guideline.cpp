// Original: d:\build\custom\temp\code\client\projectg\guideline.cpp
#include "guideline.h"
#include "ObjectFactory.h"
#include "fx.h"
#include "fxsequence.h"
#include "wresrcmng.h"
#include "wview.h"
#include "wutil.h"
#include "SharedDoc.h"
#include "golfdoc.h"
#include "golfball.h"
#include "club.h"
#include "CapturedBg.h"
#include "QuadTree.h"

// guideline.cpp:18
IObject* CGuideLineMakeInstance(void) {
	return new CGuideLine;
}

// guideline.cpp:18 (run-time type and factory registration of CGuideLine)
const WRTTI CGuideLine::m_RTTI("CGuideLine", &IActor::m_RTTI);

struct __impCGuideLine {
	__impCGuideLine(void) {
		ObjectFactory().AddObjectFunctor(CGuideLineMakeInstance, "CGuideLine");
		ActorFactory().AddActorClass("CGuideLine");
	}
};
static __impCGuideLine s_impCGuideLine;

// guideline.cpp:25
CGuideLine::CGuideLine(void) : m_texture(0) {
	AddSkipList(0x85806);
}

// guideline.cpp:30
void CGuideLine::OnPreLoadInit(void) {
	m_pVertex[0] = &m_vertex[0];
	m_pVertex[1] = &m_vertex[1];
	m_pVertex[2] = &m_vertex[3];
	m_pVertex[3] = &m_vertex[2];
	m_bufferIndex = 0;

	for (int i = 0; i < 80; i++) {
		m_road[i].m_pFx = NULL;
	}

	// "\xB0\xA1\xC0\xCC\xB5\xE5\xB6\xF3\xC0\xCE" is "guide line" in Korean (CP949)
	CSceneManager::Instance()->RegisterElement((eObjType)1, NULL, this, "\xB0\xA1\xC0\xCC\xB5\xE5\xB6\xF3\xC0\xCE", 0, NULL);

	m_texture = g_resrcmng->LoadTexture("~Track.jpg", 0, 0, NULL);
}

// guideline.cpp:46
void CGuideLine::OnLoad(void) {
}

// guideline.cpp:50
void CGuideLine::OnInit(void) {
	m_bHidden = m_bFading = false;
	m_bActive = false;
	m_segmentTime = 0;
	m_segmentCount = 0;
	m_bDirty = false;
}

// guideline.cpp:60
void CGuideLine::OnDestroy(void) {
	if (g_resrcmng && m_texture) { g_resrcmng->Release(m_texture); m_texture = 0; }
}

// guideline.cpp:63
CGuideLine::sRoad::sRoad(void) {
}

// guideline.cpp:65
void CGuideLine::OnProcess(float elapsed) {
	if (GOLFDOC()->m_gameState < 15 || GolfBall().m_groundType != 2 || !m_bActive || WSingleton<CCapturedBg>::Instance()->IsCaptured()) {
		return;
	}

	if (m_bHidden && !m_bFading) {
		CalcSegment();
	}

	if (m_segmentTime > 0.0f) {
		m_segmentTime -= elapsed;
		if (m_segmentTime < 0.0f) {
			SetActive(false);
		}
	}

	m_bHidden = m_bFading;
	m_bFading = false;
	if (m_alpha > 0.0f && m_alpha < 255.0f) {
		m_alpha = Min(m_alpha + elapsed * 255.0f, 255.0f);
		for (int i = 0; i < 4; i++) {
			m_vertex[i].diffuse = ((int)m_alpha << 24) | 0xFFFFFF;
		}
	}
}

// guideline.cpp:93
void CGuideLine::HandleMsg(const MsgObject& msg) {
	switch (msg.m_msg) {
	case 0x1a0:
		SetActive(false);
		break;
	case 0x1a2:
		SetActive(msg.m_param1 != 0);
		break;
	case 0x1a1:
		if (m_bActive) {
			if (!m_bHidden) {
				for (int i = 0; i < 80; i++) {
					if (m_road[i].m_pFx == NULL) {
						break;
					}

					CFx::Instance()->CloseSequence(m_road[i].m_pFx, false);
					m_road[i].m_pFx = NULL;
				}
			}

			m_bFading = true;
		}
		break;
	}
}

// guideline.cpp:125
void CGuideLine::SetActive(bool bActive) {
	if (m_bActive) {
		for (int i = 0; i < 80; i++) {
			if (m_road[i].m_pFx == NULL) {
				break;
			}

			CFx::Instance()->CloseSequence(m_road[i].m_pFx, false);
			m_road[i].m_pFx = NULL;
		}
	}

	m_bActive = bActive;

	if (m_bActive) {
		m_bHidden = false;
		m_bFading = false;
		m_bDirty = true;
		CalcSegment();

		m_segmentTime = m_segmentCount * 0.05f + 5.0f;
	}
}

// guideline.cpp:153
void CGuideLine::Display(void) {
	if (Skipped() || GOLFDOC()->m_gameState < 15 || GolfBall().m_groundType != 2 || m_bHidden || !m_bActive || WSingleton<CCapturedBg>::Instance()->IsCaptured()) {
		return;
	}

	for (int i = 0; i < m_segmentCount - 1; i++) {
		m_vertex[0].SetPosition(m_road[i].m_left);
		m_vertex[0].tu = 0;
		m_vertex[0].tv = i * 0.1f;
		m_vertex[1].SetPosition(m_road[i].m_right);
		m_vertex[1].tu = 1.0f;
		m_vertex[1].tv = i * 0.1f;
		m_vertex[2].SetPosition(m_road[i + 1].m_left);
		m_vertex[2].tu = 0;
		m_vertex[2].tv = (i + 1.0f) * 0.1f;
		m_vertex[3].SetPosition(m_road[i + 1].m_right);
		m_vertex[3].tu = 1.0f;
		m_vertex[3].tv = (i + 1.0f) * 0.1f;
		g_view->DrawPolygonFan(m_pVertex, (m_texture & 0x7FF) | 0x20B00000, 0x400, false);
	}
}

// guideline.cpp:181
void CGuideLine::CalcSegment(void) {
	if (GolfBall().m_groundType != 2) {
		return;
	}

	GolfBall().Reset(GolfBall().m_pos);

	sPlayerData *player = PLAYER(GOLFDOC()->m_myPlayerIndex); WVector toCup = GOLFDOC()->GetHoleData().m_cupPos; toCup -= player->m_ballPos;

	float dist = sqrtf(toCup.x * toCup.x + toCup.z * toCup.z);
	float power = sqrtf(dist / (GolfClub().GetRange(NULL) * 3.2f)) * GolfClub().GetPower();

	float yaw;
	SendMsg(this, "Player", 0x110, (intptr_t)&yaw, 0, 0, 0);

	GolfBall().m_velocity = power * RotMat(yaw, 1).za;
	GolfBall().m_state = 3;
	GolfBall().m_eventPreDataIndex[7] = 0;

	m_alpha = 1.0f;

	WVector dir, toNext, toBall;
	m_segmentCount = 0;
	SetPoint(GOLFDOC()->GetCameraData().m_matrix.za * 1.6f);
	while (m_segmentCount < 80 && GolfBall().m_state == 3) {
		CGolfBall &ball = GolfBall(); CGolfBall::sExpected *pData = ball.PreDataToWrite(++ball.m_preDataCount);
		GetPVS().BallProcess(0.02f);
		pData->m_pos = GolfBall().m_pos;

		dir = GolfBall().m_pos - m_road[m_segmentCount - 1].m_point;
		dir.y = 0;
		dir.Normalize();
		dir *= 1.6f;

		toNext = (m_road[m_segmentCount - 1].m_point + dir) - GolfBall().m_pos;

		toBall = m_road[m_segmentCount - 1].m_point - GolfBall().m_pos;

		while (m_segmentCount < 80 && toBall.x * toNext.x + toBall.z * toNext.z > 0.0f) {
			SetPoint(dir);

			toNext = (m_road[m_segmentCount - 1].m_point + dir) - GolfBall().m_pos;

			toBall = m_road[m_segmentCount - 1].m_point - GolfBall().m_pos;
		}
	}

	GolfBall().Reset(m_road[0].m_point);
	GolfBall().m_groundType = 2;

	m_bDirty = false;
}

// guideline.cpp:243
void CGuideLine::SetPoint(const WVector& dir) {
	if (m_segmentCount == 0) {
		m_road[m_segmentCount].m_point = GolfBall().m_pos;
	} else {
		float radius = GolfBall().m_radius;
		WVector ground = GetPVS().GetGroundPoint(m_road[m_segmentCount - 1].m_point + dir, false, NULL, false);
		m_road[m_segmentCount].m_point = WVector(ground.x, radius + ground.y, ground.z);
	}

	WVector side = WCrossProduct(dir, WVector::UNIT_POS_Y).Normalize();
	side *= GolfBall().m_radius;

	m_road[m_segmentCount].m_left = m_road[m_segmentCount].m_point + side;
	m_road[m_segmentCount].m_right = m_road[m_segmentCount].m_point - side;
	m_road[m_segmentCount].m_left.y -= GolfBall().m_radius;
	m_road[m_segmentCount].m_right.y -= GolfBall().m_radius;
	m_road[m_segmentCount].m_pFx = CFx::Instance()->OpenSequence("TrackItem.seq", false);
	m_road[m_segmentCount].m_pFx->Pos() = m_road[m_segmentCount].m_point;
	if (m_bDirty) {
		m_road[m_segmentCount].m_pFx->SetDelay(m_segmentCount * 50.0f);
	}

	m_segmentCount++;
}

// fxsequence.inl:63
void CFxSequence::SetDelay(float delay) {
	m_delay = delay;
	if (m_delay > 0.0f) {
		if (m_bActive) {
			m_bActive = false;
		}
	}
}
