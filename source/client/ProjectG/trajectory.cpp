// Original: d:\build\custom\temp\code\client\projectg\trajectory.cpp
#include "trajectory.h"
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include "wresrcmng.h"
#include "ObjectFactory.h"
#include "golfball.h"
#include "quadtree.h"
#include "wview.h"
#include "../Wangreal/include/wpuppet.h"

extern WMatrix g_camera;

// The original's min/max compile as `a > b ? b : a` / `a < b ? b : a`
// (wutil's Min/Max test with <= and >=); their header is unknown, as for
// river.cpp and camerawork.cpp.
template <class T> T MinOf(T a, T b) { return a > b ? b : a; }
template <class T> T MaxOf(T a, T b) { return a < b ? b : a; }

// Fade-out threshold of CComet::Process (a data constant in the original,
// not a literal).
static const float s_fAlphaEpsilon = 1e-5f;

// trajectory.cpp:16 (run-time type and factory registration of CTrajectory)
const WRTTI CTrajectory::m_RTTI("CTrajectory", &IActor::m_RTTI);

const WRTTI* CTrajectory::GetRTTI(void) const { return &m_RTTI; }

IObject* CTrajectoryMakeInstance(void) {
	return new CTrajectory;
}

struct __impCTrajectory {
	__impCTrajectory(void) {
		ObjectFactory().AddObjectFunctor(CTrajectoryMakeInstance, "CTrajectory");
		ActorFactory().AddActorClass("CTrajectory");
	}
};
static __impCTrajectory s_impCTrajectory;

// trajectory.cpp:21
CTrajectory::CTrajectory(void) : m_pComet(NULL), m_pDoughnut(NULL), m_pPuttLine(NULL), m_pBallTrack(NULL), m_pSkidMark(NULL), m_pBallWindTrack(NULL) {
	for (int i = 0; i < 2; i++) m_pActiveTrace[i] = NULL;

	m_pCurPuttLine = NULL;

	for (int j = 0; j < 32; j++) {
		m_pEffectTrace[j] = NULL;
	}

	m_pCobraSkidMark = NULL;
	m_pExplodeMark = NULL;
	AddSkipList(0x8C342);

	m_priority = 1;
}

// trajectory.cpp:39
void CTrajectory::OnDestroy(void) {
	if (m_pComet != NULL) { delete m_pComet; m_pComet = NULL; }
	if (m_pDoughnut != NULL) { delete m_pDoughnut; m_pDoughnut = NULL; }
	if (m_pBallTrack != NULL) { delete m_pBallTrack; m_pBallTrack = NULL; }
	if (m_pSkidMark != NULL) { delete m_pSkidMark; m_pSkidMark = NULL; }
	if (m_pBallWindTrack != NULL) { delete m_pBallWindTrack; m_pBallWindTrack = NULL; }
	if (m_pPuttLine != NULL) { delete m_pPuttLine; m_pPuttLine = NULL; }

	for (int i = 0; i < 32; i++) {
		if (m_pEffectTrace[i] != NULL) { delete m_pEffectTrace[i]; m_pEffectTrace[i] = NULL; }
	}

	if (m_pCobraSkidMark != NULL) { delete m_pCobraSkidMark; m_pCobraSkidMark = NULL; }
	if (m_pExplodeMark != NULL) { delete m_pExplodeMark; m_pExplodeMark = NULL; }
}

// trajectory.cpp:57
void CTrajectory::OnPreLoadInit(void) {
	CSceneManager::Instance()->RegisterElement((eObjType)6, 0, this, "\xb1\xcb\xc0\xfb", 0, 0);
}

// trajectory.cpp:62
void CTrajectory::OnLoad(void) {
	m_pComet = new CComet;
	m_pDoughnut = new CDoughnut;
	m_pBallTrack = new CBallTrack;
	m_pSkidMark = new CSkidMark;
	m_pBallWindTrack = new CBallWindTrack;
	m_pPuttLine = new CPuttLine;

	m_pCobraSkidMark = new ntSkidMark_Cobra;
	m_pExplodeMark = new ntExplodeMark;
}

// trajectory.cpp:75
void CTrajectory::OnProcess(float fElapsed) {
	if (GolfBall().m_preDataIndex < 0) {
		return;
	}

	for (int i = 0; i < 2; i++) {
		if (m_pActiveTrace[i] != NULL) {
			m_pActiveTrace[i]->Process(fElapsed);
		}
	}
}

// trajectory.cpp:87
void CTrajectory::Display(void) {
	for (int i = 0; i < 32; i++) {
		if (m_pEffectTrace[i] != NULL) {
			m_pEffectTrace[i]->Display();
		}
	}

	if (!Skipped() && GolfBall().m_preDataIndex >= 0) {
		for (int i = 0; i < 2; i++) {
			if (m_pActiveTrace[i] != NULL) {
				m_pActiveTrace[i]->Display();
			}
		}

		m_pExplodeMark->Display();
	}
}

// trajectory.cpp:107
void CTrajectory::HandleMsg(const MsgObject &msg) {
	CBallTrace **ppSlot = NULL;

	if (msg.m_msg != 0x18C) {
		for (int i = 0; i < 2; i++) {
			if (m_pActiveTrace[i] == NULL) {
				ppSlot = &m_pActiveTrace[i];
				break;
			}
		}

		if (ppSlot == NULL) {
			return;
		}
	}

	switch (msg.m_msg) {
	case 0x158:
		if (m_pComet != NULL) {
			*ppSlot = m_pComet;
			m_pComet->Init();

			if (CSceneManager::Instance()->GetType("\xb1\xcb\xc0\xfb") != 6) {
				CSceneManager::Instance()->SetType("\xb1\xcb\xc0\xfb", (eObjType)6);
			}
		}
		break;

	case 0x15C:
		if (m_pDoughnut != NULL) {
			*ppSlot = m_pDoughnut;
			m_pDoughnut->Init();

			if (CSceneManager::Instance()->GetType("\xb1\xcb\xc0\xfb") != 6) {
				CSceneManager::Instance()->SetType("\xb1\xcb\xc0\xfb", (eObjType)6);
			}
		}
		break;

	case 0x159:
		if (m_pBallTrack != NULL) {
			*ppSlot = m_pBallTrack;
			m_pBallTrack->Init();

			if (CSceneManager::Instance()->GetType("\xb1\xcb\xc0\xfb") != 6) {
				CSceneManager::Instance()->SetType("\xb1\xcb\xc0\xfb", (eObjType)6);
			}
		}
		break;

	case 0x15A:
		if (m_pSkidMark != NULL) {
			m_pSkidMark->SetFrame(GolfBall().m_eventPreDataIndex[6], GolfBall().m_eventPreDataIndex[6] + 75);
			*ppSlot = m_pSkidMark;
			m_pSkidMark->Init();

			if (CSceneManager::Instance()->GetType("\xb1\xcb\xc0\xfb") != 1) {
				CSceneManager::Instance()->SetType("\xb1\xcb\xc0\xfb", (eObjType)1);
			}
		}
		break;

	case 0x15B:
		if (m_pBallWindTrack != NULL) {
			*ppSlot = m_pBallWindTrack;
			m_pBallWindTrack->Init();

			if (CSceneManager::Instance()->GetType("\xb1\xcb\xc0\xfb") != 6) {
				CSceneManager::Instance()->SetType("\xb1\xcb\xc0\xfb", (eObjType)6);
			}
		}
		break;

	case 0x15D:
		if (m_pPuttLine != NULL) {
			m_pCurPuttLine = new CPuttLine;

			for (int i = 0; i < 32; i++) {
				if (m_pEffectTrace[i] == NULL) {
					m_pEffectTrace[i] = m_pCurPuttLine;
					break;
				}
			}

			*ppSlot = m_pCurPuttLine;
			m_pCurPuttLine->Init();

			if (CSceneManager::Instance()->GetType("\xb1\xcb\xc0\xfb") != 1) {
				CSceneManager::Instance()->SetType("\xb1\xcb\xc0\xfb", (eObjType)1);
			}
		}
		break;

	case 0x15E:
		if (m_pCobraSkidMark != NULL) {
			m_pCobraSkidMark->SetFrame(0, GolfBall().m_eventPreDataIndex[9]);
			*ppSlot = m_pCobraSkidMark;
			m_pCobraSkidMark->Init();

			if (CSceneManager::Instance()->GetType("\xb1\xcb\xc0\xfb") != 1) {
				CSceneManager::Instance()->SetType("\xb1\xcb\xc0\xfb", (eObjType)1);
			}
		}
		break;

	case 0x15F:
		m_pExplodeMark->Init(GolfBall().m_pos);
		break;

	case 0x18C:
		{
			WPuppet *pBall = CSceneManager::Instance()->GetPuppet("\xb0\xf8");
			if (pBall != NULL && pBall->GetRootBone() != NULL) {
				pBall->SetAlpha((unsigned char)0xFF);
			}

			m_pActiveTrace[0] = NULL;
			m_pActiveTrace[1] = NULL;

			if (m_pExplodeMark != NULL) {
				m_pExplodeMark->Uninit();
			}

			if (m_pCurPuttLine != NULL) {
				if (!m_pCurPuttLine->IsKeep()) {
					for (int i = 0; i < 32; i++) {
						if (m_pEffectTrace[i] == m_pCurPuttLine) {
							if (m_pEffectTrace[i] != NULL) { delete m_pEffectTrace[i]; m_pEffectTrace[i] = NULL; }
						}
					}
				} else {
					m_pCurPuttLine->RemoveUnkeptAlpha();
				}
			}

			int nFree;
			for (nFree = 0; nFree < 32; nFree++) {
				if (m_pEffectTrace[nFree] == NULL) {
					break;
				}
			}

			if (nFree == 32) {
				int nMaxAge = 0;
				int nOldest;

				for (int i = 0; i < 32; i++) {
					if (m_pEffectTrace[i] != NULL && m_pEffectTrace[i]->HowOldAreYou() > nMaxAge) {
						nMaxAge = m_pEffectTrace[i]->HowOldAreYou();
						nOldest = i;
					}
				}

				if (m_pEffectTrace[nOldest] != NULL) { delete m_pEffectTrace[nOldest]; m_pEffectTrace[nOldest] = NULL; }
			}
		}
		break;

	case 0x160:
		{
			WPuppet *pBall = CSceneManager::Instance()->GetPuppet("\xb0\xf8");
			if (pBall != NULL && pBall->GetRootBone() != NULL) {
				pBall->SetAlpha((unsigned char)0xFF);
			}

			m_pCurPuttLine = NULL;

			for (int i = 0; i < 32; i++) {
				if (m_pEffectTrace[i] != NULL) { delete m_pEffectTrace[i]; m_pEffectTrace[i] = NULL; }
			}
		}
		break;
	}
}

// trajectory.h:70
void CBallTrace::Generate(void) {
}

// trajectory.h:71
void CBallTrace::BlowAway(float fDist, int nType) {
}

// trajectory.cpp:293
CBallTrace::CBallTrace(void) : m_nState(0), m_hTexture(0), m_bActive(false) {
	m_pVertex[0] = &m_vertex[0];
	m_pVertex[1] = &m_vertex[1];
	m_pVertex[2] = &m_vertex[3];
	m_pVertex[3] = &m_vertex[2];
	m_nVertex = 0;
}

// trajectory.cpp:311
CBallTrack::CBallTrack(void) : m_pPoint(NULL), m_bRainbow(false), m_pPointEx(NULL), m_hRainbowTexture(0) {
	m_hTexture = g_resrcmng->LoadTexture("[swingtale01.jpg", 0, 0, NULL);
}

// trajectory.cpp:316
CBallTrack::~CBallTrack(void) {
	if (m_hTexture != 0) {
		g_resrcmng->Release(m_hTexture);
		m_hTexture = 0;
	}
	if (m_pPoint != NULL) { delete[] m_pPoint; m_pPoint = NULL; }
	if (g_resrcmng != NULL && m_hRainbowTexture != 0) { g_resrcmng->Release(m_hRainbowTexture); m_hRainbowTexture = 0; }
	if (m_pPoint != NULL) { delete[] m_pPoint; m_pPoint = NULL; }
	if (m_pPointEx != NULL) { delete[] m_pPointEx; m_pPointEx = NULL; }
}

// trajectory.cpp:334
// Partial: lines 335-337 and the two allocations. The element counts are
// GolfBall()'s int at 0x64 plus one, which golfball.h does not declare yet;
// the constant stands in for it until it does.
void CBallTrack::Init(void) {
	if (m_pPoint != NULL) {
		delete[] m_pPoint;
		m_pPoint = NULL;
	}
	if (m_pPointEx != NULL) {
		delete[] m_pPointEx;
		m_pPointEx = NULL;
	}
	m_pPointEx = new sPointEx[4];
	m_pPoint = new sPoint[4];
}

// trajectory.cpp:500
void CBallTrack::Display(void) {
	if (!m_bActive) {
		return;
	}

	int hTexture = m_hTexture;

	int nExtra = m_nLastFrame < GolfBall().m_preDataCount ? 1 : 0;

	if (m_bRainbow) {
		hTexture = m_hRainbowTexture;
	}

	for (int i = 1; i < m_nPoint + nExtra; i++) {
		for (int j = 0; j < 4; j++) {
			int nIndex = (j & 2) ? i : i - 1;

			m_vertex[j].diffuse = ((int)(m_pPoint[nIndex].m_fAlpha * 255.0f) << 24) | 0xFFFFFF;

			m_vertex[j].SetPosition((j & 1) ? m_pPoint[nIndex].m_vSide : m_pPoint[nIndex].m_vPos);
		}

		g_view->DrawPolygonFan(m_pVertex, (hTexture & 0x7FF) | 0xC00000, 0x400, false);
	}
}

// trajectory.cpp:546
void CBallTrack::UpdateRainbowEffect(float fElapsed) {
	for (int i = 0; i < m_nPoint; i++) {
		if (m_pPointEx[i].m_bActive == true) {
			m_pPointEx[i].m_fProgress += fElapsed * 5.0f;
		}

		if (m_pPointEx[i].m_fProgress > 1.0f) {
			m_pPoint[i].m_vPos = m_pPointEx[i].m_vStart + m_pPointEx[i].m_vDir * m_pPointEx[i].m_fProgress;
			m_pPoint[i].m_vSide = m_pPointEx[i].m_vStart - m_pPointEx[i].m_vDir * m_pPointEx[i].m_fProgress;
		}
	}
}

// trajectory.cpp:564
CSkidMark::CSkidMark(void) {
	m_pPoint = NULL;
	m_nStartFrame = -1;
	m_nEndFrame = -1;
	m_nPoint = 0;
	m_nCurPoint = 0;
	m_hTexture = g_resrcmng->LoadTexture("~skidmark01.jpg", 0, 0, NULL);
}

// trajectory.cpp:573
CSkidMark::~CSkidMark(void) {
	if (m_hTexture != 0) {
		g_resrcmng->Release(m_hTexture);
		m_hTexture = 0;
	}

	if (m_pPoint != NULL) {
		delete[] m_pPoint;
		m_pPoint = NULL;
	}
	m_nPoint = 0;
	m_nCurPoint = 0;
}

// trajectory.cpp:586
void CSkidMark::SetFrame(int nStart, int nEnd) {
	int n = GolfBall().m_preDataIndex; m_nStartFrame = nStart < n ? n : nStart;
	n = GolfBall().m_preDataCount; m_nEndFrame = nEnd > n ? n : nEnd;
}

// trajectory.cpp:592
unsigned char CSkidMark::GetPointAlpha(int nIndex, int nFrame) {
	if (nIndex > 0 && nFrame > 0) {
		WVector cur = GolfBall().PreData(nFrame)->m_pos;
		WVector prev = GolfBall().PreData(nFrame - 1)->m_pos;

		WVector delta;
		delta.x = cur.x - prev.x;
		delta.y = cur.y - prev.y;
		delta.z = cur.z - prev.z;
		if (delta.Magnitude() < 0.01f) {
			int nAlpha = (int)(m_pPoint[nIndex - 1].m_alpha * 1.1);
			return nAlpha < 255 ? nAlpha : 255;
		}
	}
	return (int)((float)(m_nPoint - nIndex) / m_nPoint * 192.0f);
}

// trajectory.cpp:608
void CSkidMark::Init(void) {
	if (m_nStartFrame < GolfBall().m_preDataIndex) {
		m_nStartFrame = GolfBall().m_preDataIndex;
	}
	if (m_nEndFrame < 0 || m_nEndFrame > GolfBall().m_preDataCount) {
		m_nEndFrame = GolfBall().m_preDataCount;
	}

	m_nPoint = m_nEndFrame - m_nStartFrame + 1;
	if (m_pPoint != NULL) { delete[] m_pPoint; m_pPoint = NULL; }
	if (m_nPoint > 0) {
		m_pPoint = new sPoint[m_nPoint];
	}

	m_nCurPoint = 0; m_nLastFrame = m_nStartFrame - 1;

	for (int i = 0; i < 4; i++) {
		m_vertex[i].tu = (i & 2) ? 1.0f : 0.0f;
		m_vertex[i].tv = (i & 1) ? 0.0f : 1.0f;
	}

	m_bActive = true;
}

// trajectory.cpp:639
void CSkidMark::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	int nFrame = GolfBall().m_preDataIndex;
	if (nFrame < m_nStartFrame) {
		return;
	}

	WVector vSide = WCrossProduct(WVector::UNIT_POS_Y, GolfBall().PreData(nFrame)->m_velocity);
	vSide.Normalize();
	vSide *= GolfBall().m_radius * 1.2f;

	for (int i = m_nLastFrame + 1; i <= nFrame; i++) {
		if (m_nCurPoint < m_nPoint) {
			WVector vPos = GolfBall().PreData(i)->m_pos;
			vPos.y = GetPVS().GetGroundPoint(vPos, true, NULL, true).y;

			m_pPoint[m_nCurPoint].m_vLeft = vPos + vSide;
			m_pPoint[m_nCurPoint].m_vRight = vPos - vSide;
			m_pPoint[m_nCurPoint].m_alpha = GetPointAlpha(m_nCurPoint, i);
			m_nCurPoint++;
		}
	}

	m_nLastFrame = nFrame;
	if (nFrame < m_nEndFrame) {
		if (m_nCurPoint < m_nPoint) {
			WVector vPos = GolfBall().m_pos;
			vPos.y = GetPVS().GetGroundPoint(vPos, true, NULL, true).y;

			m_pPoint[m_nCurPoint].m_vLeft = vPos + vSide;
			m_pPoint[m_nCurPoint].m_vRight = vPos - vSide;
			m_pPoint[m_nCurPoint].m_alpha = GetPointAlpha(m_nCurPoint, m_nLastFrame);
		}
	}
}

// trajectory.cpp:684
void CSkidMark::Display(void) {
	if (!m_bActive) {
		return;
	}

	int nNum = MinOf(m_nPoint, m_nLastFrame < m_nEndFrame ? m_nCurPoint + 1 : m_nCurPoint);

	for (int i = 1; i < nNum; i++) {
		unsigned long color = 0xFF000000 | (m_pPoint[i].m_alpha << 16) | (m_pPoint[i].m_alpha << 8) | m_pPoint[i].m_alpha;

		for (int j = 0; j < 4; j++) {
			m_vertex[j].diffuse = color;
			int nIndex = (j & 2) ? i : i - 1;

			m_vertex[j].SetPosition((j & 1) ? m_pPoint[nIndex].m_vRight : m_pPoint[nIndex].m_vLeft);
		}

		g_view->DrawPolygonFan(m_pVertex, (m_hTexture & 0x7FF) | 0x21B00000, 0x400, false);
	}
}

// trajectory.cpp:723
CBallWindTrack::CBallWindTrack(void) : m_pLineEffect(NULL) {
	m_vLastEyePos.Reset();
	m_vEyeMove.Reset();

	m_pLineEffect = new sLineEffect[MAX_LINE_EFFECT];
}

// trajectory.cpp:731
CBallWindTrack::~CBallWindTrack(void) {
	if (m_pLineEffect != NULL) { delete[] m_pLineEffect; m_pLineEffect = NULL; }
}

// trajectory.cpp:736
void CBallWindTrack::Init(void) {
	m_ballTrack.Init();

	m_bActive = true;

	for (int i = 0; i < MAX_LINE_EFFECT; i++) {
		m_pLineEffect[i].m_bActive = false;
	}
}

// trajectory.cpp:809
void CBallWindTrack::Display(void) {
	m_ballTrack.Display();

	if (!m_bActive) {
		return;
	}

	if (m_fLineAlpha > 0.0f && m_vEyeMove.Magnitude() != 0.0f) {
		for (int i = 0; i < MAX_LINE_EFFECT; i++) {
			if (m_pLineEffect[i].m_bActive) {
				g_view->DrawLine(m_pLineEffect[i].m_vStart, ((int)(m_pLineEffect[i].m_nAlpha * m_fLineAlpha) << 24) | 0xFFFFFF, m_pLineEffect[i].m_vEnd, 0, 0x20C00000);
			}
		}
	}
}

// trajectory.cpp:841
CComet::CComet(void) {
	m_hTailTexture = g_resrcmng->LoadTexture("~ball_straight.jpg", 0, 0, NULL);
	m_fInterval = 0.1f;
	m_nCount = 20;

	for (int i = 0; i < 4; i++) {
		m_vertex[i].diffuse = 0xFFFFFFFF;
	}
}

// trajectory.cpp:854
CComet::~CComet(void) {
	if (m_hTailTexture != 0) {
		g_resrcmng->Release(m_hTailTexture);
		m_hTailTexture = 0;
	}
}

// trajectory.cpp:863
void CComet::Init(void) {
	m_fNumPoint = 1.0f; m_bHeadPoint = 0;
	m_nHead = 0;
	m_fAlpha = 255.0f;
	m_fTimer = 0.0f;
	m_bActive = true;

	WVector vSide = WCrossProduct(GolfBall().m_velocity, g_camera.za);
	float fRadius = GolfBall().m_radius; vSide = vSide.Normalize() * 2.0f * fRadius;

	m_point[0].m_vLeft = GolfBall().m_pos + vSide;
	m_point[0].m_vRight = GolfBall().m_pos - vSide;
}

// trajectory.cpp:879
void CComet::Generate(void) {
	WVector vSide = WCrossProduct(GolfBall().m_velocity, g_camera.za);
	float fRadius = GolfBall().m_radius; vSide = vSide.Normalize() * 2.0f * fRadius;

	m_nHead = (m_nHead + 1) % m_nCount;

	m_point[m_nHead].m_vLeft = GolfBall().m_pos + vSide;
	m_point[m_nHead].m_vRight = GolfBall().m_pos - vSide;

	if ((int)m_fNumPoint < m_nCount) {
		m_fNumPoint += 1.0f;
	}

	m_bHeadPoint = 0;
}

// trajectory.cpp:896
void CComet::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	if (GolfBall().m_velocity.y < 0.0f) {
		float fNumPoint = m_fNumPoint - 0.5f; m_fNumPoint = fNumPoint > 0.0f ? fNumPoint : 0.0f;
		m_fTimer += MinOf(fElapsed, fElapsed * m_fAlpha * 0.005f);
		m_fAlpha = MaxOf(0.0f, m_fAlpha - fElapsed * 150.0f);

		if (WAbsBySign(m_fAlpha) < s_fAlphaEpsilon) {
			m_bActive = false;
			m_fAlpha = 0.0f;
		}
	} else {
		m_fTimer += fElapsed;
	}

	if (m_fTimer > m_fInterval) {
		m_fTimer -= m_fInterval;
		Generate();
	} else {
		WVector vSide = WCrossProduct(GolfBall().m_velocity, g_camera.za);
		vSide.Normalize();
		vSide *= GolfBall().m_radius * 2.0f;

		m_vHeadLeft = GolfBall().m_pos + vSide;
		m_vHeadRight = GolfBall().m_pos - vSide;

		m_bHeadPoint = 1;
	}
}

// trajectory.cpp:1017
CDoughnut::CDoughnut(void) {
	m_nCount = 10;
	m_pParticle = new sParticle[m_nCount];
	m_hTexture = g_resrcmng->LoadTexture("~ball_circle01.jpg", 0, 0, NULL);

	m_fInterval = 0.1f;
}

// trajectory.cpp:1027
CDoughnut::~CDoughnut(void) {
	if (m_pParticle != NULL) {
		delete[] m_pParticle;
		m_pParticle = NULL;
	}
	if (m_hTexture != 0) {
		g_resrcmng->Release(m_hTexture);
		m_hTexture = 0;
	}
}

// trajectory.cpp:1038
void CDoughnut::Init(void) {
	for (int i = 0; i < m_nCount; i++) {
		m_pParticle[i].m_bActive = false;
	}
	m_fTimer = 0.0f;
	m_nHead = 0;
	m_bActive = true;
}

// trajectory.cpp:1048
void CDoughnut::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	if (GolfBall().m_eventPreDataIndex[0] > 0) {
		m_bActive = false;
		return;
	}

	for (int i = 0; i < m_nCount; i++) {
		if (m_pParticle[i].m_bActive) {
			m_pParticle[i].m_fAlpha -= fElapsed * 255.0f;
			m_pParticle[i].m_fScale -= fElapsed * 0.8f;

			if (m_pParticle[i].m_fAlpha < 0.0f) {
				m_pParticle[i].m_bActive = false;
			}
		}
	}

	m_fTimer += fElapsed;

	if (m_fTimer > m_fInterval) {
		m_fTimer -= m_fInterval;
		m_pParticle[m_nHead].m_bActive = true;
		m_pParticle[m_nHead].m_fAlpha = 255.0f;
		m_pParticle[m_nHead].m_fScale = 1.0f;

		m_pParticle[m_nHead].m_matrix = MakeCameraMatrix(GolfBall().m_pos, GolfBall().m_pos + GolfBall().m_velocity);
		m_nHead = (m_nHead + 1) % m_nCount;
	}
}

// trajectory.cpp:1085
void CDoughnut::Display(void) {
	if (!m_bActive) {
		return;
	}

	for (int i = 0; i < m_nCount; i++) {
		if (m_pParticle[i].m_bActive) {
			for (int j = 0; j < 4; j++) {
				WVector vPos = m_pParticle[i].m_matrix.pivot;
				vPos += (m_pParticle[i].m_matrix.xa * ((j & 2) ? 1.0f : -1.0f) + m_pParticle[i].m_matrix.ya * ((j & 1) ? 1.0f : -1.0f)) * (m_pParticle[i].m_fScale * 0.5f);

				m_vertex[j].SetPosition(vPos); m_vertex[j].diffuse = ((int)m_pParticle[i].m_fAlpha << 24) + 0xFFFFFF;
				m_vertex[j].tu = (j & 2) ? 1.0f : 0.0f;
				m_vertex[j].tv = (j & 1) ? 0.0f : 1.0f;
			}

			g_view->DrawPolygonFan(m_pVertex, (m_hTexture & 0x7FF) | 0xC00000, 0x400, false);
		}
	}
}

// trajectory.cpp:1124
CPuttLine::CPuttLine(void) : m_pPoint(NULL) {
}

// trajectory.cpp:1128
CPuttLine::~CPuttLine(void) {
	if (m_pPoint != NULL) {
		delete[] m_pPoint;
		m_pPoint = NULL;
	}
}

// trajectory.cpp:1220
void CPuttLine::Display(void) {
	if (!m_bActive || m_pPoint == NULL) {
		return;
	}

	for (int i = 1; i <= m_nLastPoint; i++) {
		for (int j = 0; j < 4; j++) {
			int nIndex = (j & 2) ? i : i - 1;

			m_vertex[j].SetPosition((j & 1) ? m_pPoint[nIndex].m_vRight : m_pPoint[nIndex].m_vLeft);

			if (m_pPoint[nIndex].m_nAlpha == 0x100)
				m_vertex[j].diffuse = 0x80E6C882;
			else
				m_vertex[j].diffuse = (m_pPoint[nIndex].m_nAlpha << 24) | 0xFFFFFF;
		}

		g_view->DrawPolygonFan(m_pVertex, 0x20300000, 0x400, false);
	}
}

// trajectory.cpp:1252
bool CPuttLine::IsKeep(void) {
	int i;
	for (i = 0; i <= m_nLastPoint; i++) {
		if (m_pPoint[i].m_nAlpha == 0x100) {
			break;
		}
	}
	return i != m_nLastPoint ? true : false;
}

// trajectory.cpp:1268
int CPuttLine::HowOldAreYou(void) {
	return timeGetTime() - m_dwStartTime;
}

// trajectory.cpp:1274
void CPuttLine::RemoveUnkeptAlpha(void) {
	for (int i = 0; i <= m_nLastPoint; i++) {
		if (m_pPoint[i].m_nAlpha != 0x100) {
			m_pPoint[i].m_nAlpha = 0;
		}
	}
}

// trajectory.cpp:1287
ntSkidMark_Cobra::ntSkidMark_Cobra(void) {
	m_pPoint = NULL;
	m_nStartFrame = -1;
	m_nEndFrame = -1;
	m_nPoint = 0;
	m_nCurPoint = 0;
	m_hTexture = g_resrcmng->LoadTexture("[_cobra_ef.jpg", 0, 0, NULL);
}

// trajectory.cpp:1299
ntSkidMark_Cobra::~ntSkidMark_Cobra(void) {
	if (m_pPoint != NULL) {
		delete[] m_pPoint;
		m_pPoint = NULL;
	}

	if (m_hTexture != 0) {
		g_resrcmng->Release(m_hTexture);
		m_hTexture = 0;
	}
	m_nPoint = 0;
	m_nCurPoint = 0;
}

// trajectory.cpp:1314
void ntSkidMark_Cobra::Init(void) {
	if (m_nStartFrame < GolfBall().m_preDataIndex) {
		m_nStartFrame = GolfBall().m_preDataIndex;
	}
	if (m_nEndFrame < 0 || m_nEndFrame > GolfBall().m_eventPreDataIndex[9]) {
		m_nEndFrame = GolfBall().m_preDataCount;
	}

	m_nPoint = m_nEndFrame - m_nStartFrame + 1;
	if (m_pPoint != NULL) { delete[] m_pPoint; m_pPoint = NULL; }
	if (m_nPoint > 0) {
		m_pPoint = new sPoint[m_nPoint];
	}

	m_nCurPoint = 0; m_nLastFrame = m_nStartFrame - 1;

	for (int i = 0; i < 4; i++) {
		m_vertex[i].tu = (i & 2) ? 1.0f : 0.0f;
		m_vertex[i].tv = (i & 1) ? 0.0f : 1.0f;
	}

	m_bActive = true;
}

// trajectory.cpp:1347
void ntSkidMark_Cobra::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	int nFrame = GolfBall().m_preDataIndex < GolfBall().m_eventPreDataIndex[9] ? GolfBall().m_preDataIndex : GolfBall().m_eventPreDataIndex[9];

	if (nFrame < m_nStartFrame) {
		return;
	}

	WVector vSide = WCrossProduct(WVector(0.0f, 1.0f, 0.0f), GolfBall().PreData(nFrame)->m_velocity);
	vSide.Normalize();
	vSide *= GolfBall().m_radius * 10.0f;

	for (int i = m_nLastFrame + 1; i <= nFrame; i++) {
		if (m_nCurPoint < m_nPoint) {
			WVector vPos = GolfBall().PreData(i)->m_pos;
			vPos.y = GetPVS().GetGroundPoint(vPos, true, NULL, true).y;

			m_pPoint[m_nCurPoint].m_vLeft = vPos + vSide;
			m_pPoint[m_nCurPoint].m_vRight = vPos - vSide;
			m_nCurPoint++;
		}
	}

	m_nLastFrame = nFrame;
	if (nFrame < m_nEndFrame) {
		if (m_nCurPoint < m_nPoint) {
			WVector vPos = GolfBall().m_pos;
			vPos.y = GetPVS().GetGroundPoint(vPos, true, NULL, true).y;

			m_pPoint[m_nCurPoint].m_vLeft = vPos + vSide;
			m_pPoint[m_nCurPoint].m_vRight = vPos - vSide;
		}
	}
}

// trajectory.cpp:1390
void ntSkidMark_Cobra::Display(void) {
	if (!m_bActive) {
		return;
	}

	int nNum = MinOf(m_nPoint, m_nLastFrame < m_nEndFrame ? m_nCurPoint + 1 : m_nCurPoint);

	for (int i = 1; i < nNum; i++) {
		for (int j = 0; j < 4; j++) {
			m_vertex[j].diffuse = 0xFFFFFFFF;
			int nIndex = (j & 2) ? i : i - 1;

			m_vertex[j].SetPosition((j & 1) ? m_pPoint[nIndex].m_vRight : m_pPoint[nIndex].m_vLeft);
		}

		g_view->DrawPolygonFan(m_pVertex, (m_hTexture & 0x7FF) | 0x20200000, 0, false);
	}
}

// trajectory.cpp:1425
void ntSkidMark_Cobra::SetFrame(int nStart, int nEnd) {
	int n = GolfBall().m_preDataIndex; m_nStartFrame = nStart < n ? n : nStart;
	n = GolfBall().m_preDataCount; m_nEndFrame = nEnd > n ? n : nEnd;
}

// trajectory.cpp:1436
ntExplodeMark::ntExplodeMark(void) : m_bActive(false) {
	m_hTexture = g_resrcmng->LoadTexture("[_boom.jpg", 0, 0, NULL);
}

// trajectory.cpp:1445
ntExplodeMark::~ntExplodeMark(void) {
	if (m_hTexture != 0) {
		g_resrcmng->Release(m_hTexture);
		m_hTexture = 0;
	}
}

// trajectory.cpp:1455
void ntExplodeMark::Init(const WVector &pos) {
	m_bActive = true;
	memcpy(&m_vPos, &pos, sizeof(WVector));
}

// trajectory.cpp:1463
void ntExplodeMark::Uninit(void) {
	m_bActive = false;
}

// trajectory.cpp:1469
void ntExplodeMark::Process(void) {
}

// trajectory.cpp:1476
void ntExplodeMark::Display(void) {
	if (m_bActive) {
		GetPVS().DrawDecal(m_hTexture, GetPVS().GetGroundPoint(m_vPos, false, 0, false), WVector(15.0f, 0.0f, 0.0f), WVector(0.0f, 0.0f, 15.0f), 1.0f, 1.0f, W_POLY_BLEND_NORMAL, false);
	}
}
