// Original: d:\build\custom\temp\code\client\projectg\particleparty.cpp
#include "particleparty.h"
#include "ClientSetting.h"
#include "golfball.h"
#include "GolfDoc.h"
#include "SharedDoc.h"
#include "../client/Wangreal/include/wutil.h"
#include "commonutil.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wview.h"
#include "../client/Wangreal/include/wpuppet.h"
#include "SoundManager.h"
#include "TaskManager.h"
#include "Shadow.h"
#include "petbody.h"
#include "quadtree.h"
#include <math.h>
#include <stdio.h>

extern LightSet g_lightset;
extern WVector data_00b7bfac;     // the camera's position

// clientsetting.h:346
int COption::vGetParticleLevel(void) const {
	return m_option.m_particleLevel;
}

// particleparty.cpp:21
CParticle::CParticle(void) : m_texHandle(0), m_bActive(false), m_pAniSpr(NULL), m_pSpr(NULL) {
	m_pBuffer0 = &m_buffers[0];
	m_pBuffer1 = &m_buffers[1];
	m_pBuffer2 = &m_buffers[3];
	m_pBuffer3 = &m_buffers[2];
	m_nBufferState = 0;
}

// particleparty.cpp:30
CParticle::~CParticle(void) {
	if (m_pSpr) {
		delete m_pSpr;
		m_pSpr = NULL;
	}
	if (m_pAniSpr) {
		delete m_pAniSpr;
		m_pAniSpr = NULL;
	}
}

// particleparty.cpp:41
CShotParticle::CShotParticle(void) {
	m_nPieces = 100;
	m_pPieces = new sPiece[m_nPieces];
}

// particleparty.cpp:47
CShotParticle::~CShotParticle(void) {
	if (m_pPieces) {
		delete[] m_pPieces;
		m_pPieces = NULL;
	}
}

// particleparty.cpp:52
void CShotParticle::MakeParticle(void) {
	CGolfBall& ball = GolfBall();
	if (ball.m_preDataCount >= 3) {
		const CGolfBall::sExpected* pExpected = &ball.m_preData[3];
		if (pExpected) {
			for (int i = 0; i < m_nPieces; i++) {
				m_pPieces[i].m_bAlive = true;
				m_pPieces[i].m_vPos = ball.m_pos;
				m_pPieces[i].m_vVelocity = pExpected->m_velocity * 0.1f
					+ WVector((float)(rand() % 41) - 20.0f, (float)(rand() % 41) - 20.0f, (float)(rand() % 41) - 20.0f);

				m_pPieces[i].m_fLength = 0.7f;
				m_pPieces[i].m_fLife = (float)(rand() % 10) * 0.05f + 0.5f;
				m_pPieces[i].m_fSlowTime = m_pPieces[i].m_fLife * 0.6f;
			}

			m_nCount = m_nPieces;
			m_bActive = true;
		}
	}
}

// particleparty.cpp:74
void CShotParticle::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	for (int i = 0; i < m_nPieces; i++) {
		sPiece* pPiece = &m_pPieces[i];

		if (pPiece->m_bAlive) {
			if (pPiece->m_fLife < pPiece->m_fSlowTime) {
				pPiece->m_vVelocity *= pPiece->m_fLife / pPiece->m_fSlowTime;
				pPiece->m_fLength *= 0.9f;
			}

			pPiece->m_vPos += pPiece->m_vVelocity * fElapsed;

			pPiece->m_fLife -= fElapsed;
			if (pPiece->m_fLife < 0.0f) {
				pPiece->m_bAlive = false;
				m_nCount--;
			}
		}
	}

	if (m_nCount == 0) {
		m_bActive = false;
	}
}

// particleparty.cpp:107
void CShotParticle::Display(void) {
	if (!m_bActive) {
		return;
	}

	for (int i = 0; i < m_nPieces; i++) {
		if (m_pPieces[i].m_bAlive) {
			WVector vDir = m_pPieces[i].m_vVelocity * -1.0f;
			vDir.Normalize();
			vDir *= m_pPieces[i].m_fLength;

			g_view->DrawLine(m_pPieces[i].m_vPos, 0xFFFFFFFF, m_pPieces[i].m_vPos + vDir, 0xFFFFFF, 0);
		}
	}
}

// particleparty.cpp:128
CPowerShot::CPowerShot(void) {
	m_nSegments = 16;

	m_fSize = 2.0f;
	m_fWidth = 0.8f;

	for (int i = 0; i < 4; i++) {
		m_buffers[i].m_fU = (i & 2) ? 1.0f : 0.0f;
		m_buffers[i].m_fV = (i & 1) ? 1.0f : 0.0f;
	}
}

// particleparty.cpp:142
CPowerShot::~CPowerShot(void) {
}

// particleparty.cpp:146
void CPowerShot::MakeParticle(void) {
	m_fDistance = 0.0f;
	m_fTime = 0.0f;
	m_fLife = 0.8f;
	m_fSpeed = 20.0f;
	m_vCenter = GolfBall().m_pos;
	m_vCenter.y += 1.6f;
	m_bActive = true;
}

// particleparty.cpp:157
void CPowerShot::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	m_fDistance += fElapsed * m_fSpeed;

	m_fTime += fElapsed;
	m_nAlpha = (int)((1.0f - m_fTime / m_fLife) * 255.0f);

	if (m_fTime > m_fLife) {
		m_bActive = false;
	}
}

// particleparty.cpp:171
void CPowerShot::Display(void) {
	if (!m_bActive) {
		return;
	}

	float fStep = 6.2831855f / (float)m_nSegments;
	unsigned long nColor = ((unsigned long)m_nAlpha << 24) | 0xFFFFFF;

	m_buffers[0].m_nColor = nColor;
	m_buffers[1].m_nColor = nColor;
	m_buffers[2].m_nColor = nColor;
	m_buffers[3].m_nColor = nColor;

	for (int i = 0; i < m_nSegments; i++) {
		float fAngle = (float)i * fStep;

		for (int j = 0; j < 4; j++) {
			float fSide = (j & 2) ? 1.0f : 0.0f;
			float fRing = (j & 1) ? 0.0f : 1.0f;

			float fCos = cosf(fAngle + fSide * fStep);
			float fSin = sinf(fAngle + fSide * fStep);
			float fRadius = (m_fSize - fRing * m_fWidth) * m_fDistance;

			WVector vOffset(fCos, 0.0f, fSin);
			m_buffers[j].m_vPos = m_vCenter + vOffset * fRadius;
		}

		g_view->DrawPolygonFan((WTVertex**)&m_pBuffer0, (m_texHandle & 0x7FF) | 0xC00000, 0x400, false);
	}
}

// particleparty.cpp:456
CLightColumn::CLightColumn(void) : m_hScreenTex(0) {
	m_hEffectTex[0] = 0; m_hEffectTex[1] = 0; m_hEffectTex[2] = 0; m_hEffectTex[3] = 0; m_hEffectTex[4] = 0;
}

// particleparty.cpp:462
CLightColumn::~CLightColumn(void) {
	for (int i = 0; i < 5; i++) {
		if (g_resrcmng && m_hEffectTex[i]) { g_resrcmng->Release(m_hEffectTex[i]); m_hEffectTex[i] = 0; }
	}
	if (g_resrcmng && m_hScreenTex) { g_resrcmng->Release(m_hScreenTex); m_hScreenTex = 0; }
}

// particleparty.cpp:470
void CLightColumn::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	WVector vHole = GetHoleCupPos();
	WVector vToCamera(data_00b7bfac.x - vHole.x, 0.0f, data_00b7bfac.z - vHole.z);

	float fDistance = sqrtf(vToCamera.z * vToCamera.z + vToCamera.x * vToCamera.x) * 0.01f;
	float fMinimum = GolfBall().m_radius * 10.0f;
	m_fDistance = (fDistance < fMinimum) ? fMinimum : fDistance;

	if (IsLocalContent((localContentType_t)0x3A)
		&& (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800)
		&& WSingleton<CSharedDoc>::Instance()->m_roomType != 0xB
		&& WSingleton<CSharedDoc>::Instance()->m_roomType != 0xC) {
		m_fDistance = Max(GolfBall().m_radius * 20.0f, vToCamera.Magnitude() * 0.01f);
	}

	m_fFrame -= fElapsed * 2.0f;

	if (m_nFade == 1) {
		m_byAlpha = (unsigned char)Min(m_byAlpha + 20, 255);
		m_byTargetAlpha = (unsigned char)Min(m_byTargetAlpha + 20, 255);

		if (m_byAlpha == 0xFF && m_byTargetAlpha == 0x50) {
			m_nFade = 0;
		}
	} else if (m_nFade == 2) {
		m_byAlpha = (unsigned char)Max(m_byAlpha - 20, 0);
		m_byTargetAlpha = (unsigned char)Max(m_byTargetAlpha - 20, 0);

		if (m_byAlpha == 0 && m_byTargetAlpha == 0) {
			m_nFade = 0;
		}
	}
}

// particleparty.cpp:671
void CLightColumn::MakeParticle(void) {
	for (int i = 0; i < 5; i++) {
		m_hEffectTex[i] = g_resrcmng->LoadTexture(MakeStr("[hole_ef_nomal_%d.jpg", i + 10), 0, 0, NULL);
	}
	m_hScreenTex = g_resrcmng->LoadTexture("hole_scr01.jpg", 0, 0, NULL);

	m_fFrame = 0.0f;

	m_nFade = 0;
	m_fHeight = 1600.0f; m_byTargetAlpha = 0xFF;
	m_byAlpha = 0xFF;
	m_bActive = true;
}

// particleparty.cpp:798
CDust::CDust(int nCount) : m_pParticles(NULL) {
	m_nParticles = nCount;
	m_pParticles = new sParticle[m_nParticles];
}

// particleparty.cpp:804
CDust::~CDust(void) {
	if (m_pParticles) {
		delete[] m_pParticles;
		m_pParticles = NULL;
	}
}

// particleparty.cpp:809
void CDust::MakeParticle(void) {
	if (m_pSpr) { delete m_pSpr; m_pSpr = NULL; }
	m_pSpr = new W3dSpr;
	m_pSpr->SetResourceManager(g_resrcmng);
	m_pSpr->AttachTexture(m_texHandle);

	float fSpeed = GolfBall().m_velocity.Magnitude() * 0.5f;

	for (int i = 0; i < m_nParticles; i++) {
		m_pParticles[i].m_vPos = m_vCenter;
		m_pParticles[i].m_fAngle = ((float)(rand() % 128) / 128.0f - 0.5f) * 1.5707964f;
		m_pParticles[i].m_fStartSize = 1.0f > fSpeed ? fSpeed : 1.0f;
		m_pParticles[i].m_fEndSize = 1.0f < fSpeed ? fSpeed : 1.0f;
	}

	m_fDuration = m_fRemain = 1.0f;
	m_bActive = true;
}

// particleparty.cpp:834
void CDust::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	m_fRemain -= fElapsed / m_fDuration;
	if (m_fRemain < 0.0f) {
		m_fRemain = 1.0f;
		m_bActive = false;
	}
}

// particleparty.cpp:847
void CDust::Display(void) {
	if (!m_bActive) {
		return;
	}

	for (int i = 0; i < m_nParticles; i++) {
		int nAlpha = (int)(m_fRemain * 128.0f);
		float fSize = (m_pParticles[i].m_fEndSize - m_pParticles[i].m_fStartSize) * (1.0f - m_fRemain) + m_pParticles[i].m_fStartSize;

		m_pSpr->SetPos(m_pParticles[i].m_vPos);
		m_pSpr->SetRect(fSize, fSize, fSize * 0.5f, fSize * 0.5f);
		m_pSpr->SetColor((nAlpha << 24) + 0xFFFFFF);
		m_pSpr->Render(g_view, m_pParticles[i].m_fAngle * m_fRemain, 0x20400000);
	}
}

// particleparty.cpp:870
CGrassParticle::CGrassParticle(int nCount, WVector const& vCenter)
	: m_nGrass(nCount), m_vOrigin(vCenter) {
	m_pGrass = new sGrass[m_nGrass];
}

// particleparty.cpp:877
CGrassParticle::~CGrassParticle(void) {
	if (m_pGrass) {
		delete[] m_pGrass;
		m_pGrass = NULL;
	}
}

// particleparty.cpp:882
void CGrassParticle::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	bool bActive = false;
	for (int i = 0; i < m_nGrass; i++) {
		sGrass* pGrass = &m_pGrass[i];

		if (pGrass->m_bAlive) {
			pGrass->m_fAlpha -= fElapsed / pGrass->m_fFadeTime * 255.0f;
			pGrass->m_vVelocity.y -= fElapsed * 34.295296f;
			pGrass->m_vPos += pGrass->m_vVelocity * fElapsed;
			pGrass->m_fAngle += fElapsed * pGrass->m_fAngularSpeed;

			if (pGrass->m_fAlpha < 0.0f || pGrass->m_vPos.y < GetPVS().GetGroundPoint(pGrass->m_vPos, false, NULL, false).y) {
				pGrass->m_bAlive = false;
			} else {
				bActive = true;
			}
		}
	}

	m_bActive = bActive;
}

// particleparty.cpp:970
CBunkerShotParticle::CBunkerShotParticle(int nSmoke, int nParticle)
	: m_pSmokes(NULL), m_pParticles(NULL), m_nSmokes(nSmoke), m_nParticles(nParticle), m_dwColor(0xF8E9AB) {
	if (m_nSmokes > 0) {
		m_pSmokes = new sSmoke[m_nSmokes];
	}
	if (m_nParticles > 0) {
		m_pParticles = new sParticle[m_nParticles];
	}
}

// particleparty.cpp:986
CBunkerShotParticle::~CBunkerShotParticle(void) {
	if (m_pSmokes) {
		delete[] m_pSmokes;
		m_pSmokes = NULL;
	}
	if (m_pParticles) {
		delete[] m_pParticles;
		m_pParticles = NULL;
	}
	m_nSmokes = 0;
	m_nParticles = 0;
}

// particleparty.cpp:911
void CGrassParticle::Display(void) {
	if (!m_bActive) {
		return;
	}

	for (int i = 0; i < m_nGrass; i++) {
		sGrass* pGrass = &m_pGrass[i];

		if (pGrass->m_bAlive) {
			int nColor = ((int)pGrass->m_fAlpha << 24) | 0xFFFFFF;

			m_pAniSpr->SetPos(pGrass->m_vPos);
			m_pAniSpr->SetColor(nColor);
			m_pAniSpr->Render(g_view, pGrass->m_fAngle, 0x20400000, pGrass->m_nFrame);
		}
	}
}

// particleparty.cpp:934
void CGrassParticle::MakeParticle(void) {
	if (m_pAniSpr) { delete m_pAniSpr; m_pAniSpr = NULL; }
	m_pAniSpr = new W3dAniSpr;
	m_pAniSpr->SetResourceManager(g_resrcmng);
	m_pAniSpr->LoadTexture(m_texHandle, 64.0f, 64.0f);

	m_pAniSpr->SetRect(0.5f, 0.5f, 0.25f, 0.25f);

	for (int i = 0; i < m_nGrass; i++) {
		sGrass* pGrass = &m_pGrass[i];

		pGrass->m_vPos = GolfBall().m_pos;
		pGrass->m_vPos.y -= GolfBall().m_radius;
		pGrass->m_bAlive = true;
		pGrass->m_fAngle = (float)(rand() % 1023) * 0.0061359233f;
		pGrass->m_fAngularSpeed = ((float)(rand() % 1023) * 0.0009765625f - 0.5f) * 12.566371f;

		pGrass->m_vVelocity.x = (float)(rand() % 1025 - 512) * (m_vOrigin.x * 0.2f + 3.0f) * 0.001953125f + m_vOrigin.x;
		pGrass->m_vVelocity.y = (float)(rand() % 1025 - 512) * (m_vOrigin.y * 0.2f + 3.0f) * 0.001953125f + m_vOrigin.y;
		pGrass->m_vVelocity.z = (float)(rand() % 1025 - 512) * (m_vOrigin.z * 0.2f + 3.0f) * 0.001953125f + m_vOrigin.z;

		pGrass->m_nFrame = rand() % 4;
		pGrass->m_fFadeTime = 1.5f;
		pGrass->m_fAlpha = 255.0f;
	}

	m_bActive = true;
}

// particleparty.cpp:1010
void CBunkerShotParticle::Generate(void) {
	CGolfBall& ball = GolfBall();
	if (ball.m_preDataCount >= 3) {
		const CGolfBall::sExpected* pExpected = &ball.m_preData[3];
		if (pExpected) {
			for (int i = 0; i < m_nSmokes; i++) {
				m_pSmokes[i].m_vPos = ball.m_pos;
				m_pSmokes[i].m_vVelocity = pExpected->m_velocity * 0.5f
					+ WVector((float)(rand() % 11) - 5.0f, (float)(rand() % 11) - 5.0f, (float)(rand() % 11) - 5.0f) * 2.5f;
			}

			for (int i = 0; i < m_nParticles; i++) {
				m_pParticles[i].m_bAlive = true;
				m_pParticles[i].m_vPos = ball.m_pos;
				m_pParticles[i].m_vVelocity = pExpected->m_velocity * 0.6f
					+ WVector((float)(rand() % 41) - 20.0f, (float)(rand() % 41) - 20.0f, (float)(rand() % 41) - 20.0f);
				m_pParticles[i].m_fLength = 0.3f;
				m_pParticles[i].m_fLife = (float)(rand() % 10) * 0.05f + 1.0f;
				m_pParticles[i].m_fMaxLife = m_pParticles[i].m_fLife * 0.5f;
			}
		}
	}
}

// particleparty.cpp:995
void CBunkerShotParticle::MakeParticle(void) {
	if (m_pSpr) { delete m_pSpr; m_pSpr = NULL; }
	m_pSpr = new W3dSpr;
	m_pSpr->SetResourceManager(g_resrcmng);
	m_pSpr->AttachTexture(m_texHandle);

	Generate();

	m_fRemain = m_fDuration = 1.0f; g_audio->PlaySfx("\xBA\xA1\xC4\xBF\xB8\xF0\xB7\xA1\xB3\xAF\xB8\xB2", NULL, 0, NULL, NULL, 0.5f, 200.0f);

	m_bActive = true;
}

// particleparty.cpp:1039
void CBunkerShotParticle::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	m_fRemain -= fElapsed / m_fDuration;

	if (m_fRemain < 0.0f) {
		m_bActive = false;
		return;
	}

	for (int i = 0; i < m_nSmokes; i++) {
		m_pSmokes[i].m_vVelocity.y -= fElapsed * 120.033536f;
		m_pSmokes[i].m_vPos += m_pSmokes[i].m_vVelocity * fElapsed;
	}

	for (int j = 0; j < m_nParticles; j++) {
		sParticle* pParticle = &m_pParticles[j];

		if (pParticle->m_bAlive) {
			pParticle->m_vVelocity.x *= 0.99f;
			pParticle->m_vVelocity.y -= fElapsed * 102.885888f;
			pParticle->m_vVelocity.z *= 0.99f;
			pParticle->m_vPos += pParticle->m_vVelocity * fElapsed;
			pParticle->m_fLife -= fElapsed;

			if (pParticle->m_fLife < 0.0f) {
				pParticle->m_bAlive = false;
				m_nParticles--;
			}
		}
	}
}

// particleparty.cpp:1084
void CBunkerShotParticle::Display(void) {
	if (!m_bActive) {
		return;
	}

	for (int i = 0; i < m_nSmokes; i++) {
		sSmoke* pSmoke = &m_pSmokes[i];
		float fSize = (1.0f - m_fRemain) * 40.0f;

		m_pSpr->SetRect(fSize, fSize, fSize * 0.5f, 0.0f);
		m_pSpr->SetColor(((int)(m_fRemain * 255.0f) << 24) | m_dwColor);
		m_pSpr->SetPos(pSmoke->m_vPos);
		m_pSpr->Render(g_view, 0x20400000, W3dSpr::CAMERA_X_ALIGN);
	}

	for (int j = 0; j < m_nParticles; j++) {
		sParticle* pParticle = &m_pParticles[j];

		if (pParticle->m_bAlive) {
			WVector vDir = pParticle->m_vVelocity * -1.0f;
			vDir.Normalize();
			vDir *= pParticle->m_fLength;

			if (pParticle->m_fLength > 0.1f) {
				pParticle->m_fLength *= 0.95f;
			}

			if (pParticle->m_fLife < pParticle->m_fMaxLife) {
				vDir *= pParticle->m_fLife / pParticle->m_fMaxLife;
			}

			g_view->DrawLine(pParticle->m_vPos, 0xFFA08000, pParticle->m_vPos + vDir, 0xA08000, 0);
		}
	}
}

// particleparty.cpp:1126
CBunkerParticle::CBunkerParticle(void) {
}

// particleparty.cpp:1130
CBunkerParticle::~CBunkerParticle(void) {
}

// particleparty.cpp:1134
void CBunkerParticle::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	m_fRemain -= fElapsed / m_fDuration;

	if (m_fRemain < 0.0f) {
		m_bActive = false;
	}
}

// particleparty.cpp:1146
void CBunkerParticle::Display(void) {
	if (!m_bActive) {
		return;
	}

	m_fSize = (1.0f - m_fRemain) * 20.0f;
	m_pSpr->SetRect(m_fSize, m_fSize, m_fSize * 0.5f, 0.0f);

	m_nAlpha = (int)(m_fRemain * 255.0f);
	m_pSpr->SetColor((m_nAlpha << 24) | 0xFFFFFF);

	m_pSpr->Render(g_view, 0x20400000, W3dSpr::CAMERA_X_ALIGN);
}

// particleparty.cpp:1160
void CBunkerParticle::MakeParticle(void) {
	if (m_pSpr) { delete m_pSpr; m_pSpr = NULL; }
	m_pSpr = new W3dSpr;
	m_pSpr->SetResourceManager(g_resrcmng);
	m_pSpr->AttachTexture(m_texHandle);

	m_fSize = 0.0f;
	m_fRemain = 1.0f;
	m_fDuration = 0.7f;
	m_pSpr->SetPos(GolfBall().m_pos);

	g_audio->PlaySfx("\xBA\xA1\xC4\xBF\xB8\xF0\xB7\xA1\xB3\xAF\xB8\xB2", NULL, 0, NULL, NULL, 0.5f, 200.0f);

	m_bActive = true;
}

// particleparty.cpp:1180
CFragment::CFragment(void) : m_pFrags(NULL), m_nFrags(30) {
	m_pFrags = new sFrag[m_nFrags];
}

// particleparty.cpp:1186
CFragment::~CFragment(void) {
	if (m_pFrags) {
		delete[] m_pFrags;
		m_pFrags = NULL;
	}
}

// particleparty.cpp:1191
void CFragment::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	m_fAlpha -= fElapsed * 96.0f;

	for (int i = 0; i < m_nFrags; i++) {
		m_pFrags[i].m_vVelocity.y -= fElapsed * 17.147648f;
		m_pFrags[i].m_vPos += m_pFrags[i].m_vVelocity * fElapsed;
	}

	if (m_fAlpha < 0.0f) {
		m_bActive = false;
	}
}

// particleparty.cpp:1208
void CFragment::Display(void) {
	if (!m_bActive) {
		return;
	}

	m_pAniSpr->SetColor(((int)m_fAlpha << 24) | 0xFFFFFF);

	for (int i = 0; i < m_nFrags; i++) {
		m_pAniSpr->SetPos(m_pFrags[i].m_vPos);
		m_pAniSpr->Render(g_view, 0.0f, 0xC00000, 7);
	}
}

// particleparty.cpp:1222
void CFragment::MakeParticle(void) {
	if (m_pAniSpr) { delete m_pAniSpr; m_pAniSpr = NULL; }
	m_pAniSpr = new W3dAniSpr;
	m_pAniSpr->SetResourceManager(g_resrcmng);
	m_pAniSpr->LoadTexture(m_texHandle, 32.0f, 32.0f);

	WVector vRight(1.0f, 0.0f, 0.0f);
	WVector vUp(0.0f, 1.0f, 0.0f);
	WVector vForward(0.0f, 0.0f, 1.0f);

	if (!(Abs(m_vNormal.y - 1.0f) < 0.00001f)) {
		vUp = m_vNormal;
		vRight = WCrossProduct(vUp, WVector::UNIT_POS_Y);
		vRight.Normalize();
		vForward = WCrossProduct(vRight, vUp);
		vForward.Normalize();
	}

	for (int i = 0; i < m_nFrags; i++) {
		float fRatio = (float)(rand() % 1023) * 0.0009765625f;

		m_pFrags[i].m_vPos = m_vCenter;

		float fAngle = (float)(rand() % 1023) * 0.0009765625f;
		WVector vDir(WVector::UNIT_POS_Z.x * fAngle, WVector::UNIT_POS_Z.y * fAngle, WVector::UNIT_POS_Z.z * fAngle);
		m_pFrags[i].m_vVelocity = vDir * RotMat((float)(rand() % 1023) * 0.0061359233f, 1);
		m_pFrags[i].m_vVelocity.y = (1.0f - fRatio) * 2.0f + fRatio * 5.0f;

		float fSpeed = Min(GolfBall().m_velocity.Magnitude() * 0.1f, 2.0f);
		float vx = m_pFrags[i].m_vVelocity.x * fSpeed;
		float vy = m_pFrags[i].m_vVelocity.y * fSpeed;
		float vz = m_pFrags[i].m_vVelocity.z * fSpeed;

		m_pFrags[i].m_vVelocity.x = vRight.z * vx + vUp.z * vy + vForward.z * vz;
		m_pFrags[i].m_vVelocity.y = vRight.y * vx + vUp.y * vy + vForward.y * vz;
		m_pFrags[i].m_vVelocity.z = vRight.x * vx + vUp.x * vy + vForward.x * vz;
	}

	m_fAlpha = 255.0f;
	m_bActive = true;

	float fSize = Max(Min(GolfBall().m_velocity.Magnitude() * 0.01f, 0.5f), 0.2f);
	m_pAniSpr->SetRect(fSize, fSize, fSize * 0.5f, fSize * 0.5f);
}

// particleparty.cpp:1368
CDroplet::CDroplet(void) {
	m_nDroplets = 100;
	m_pDroplets = new sDroplet[m_nDroplets];
	m_pSpr = g_resrcmng->Get3DSpr("w-drop.dds", 0);

	m_pSpr->SetRect(0.2f, 0.2f, 0.1f, 0.1f);
}

// particleparty.cpp:1379
CDroplet::~CDroplet(void) {
	if (m_pSpr) {
		g_resrcmng->Release(m_pSpr);
		m_pSpr = NULL;
	}

	if (m_pDroplets) { delete[] m_pDroplets; m_pDroplets = NULL; }
}

// particleparty.cpp:1390
void CDroplet::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	m_fAlpha -= fElapsed / m_fDuration * 255.0f;
	if (m_fAlpha < 0.0f) {
		m_bActive = false;
		return;
	}

	if (m_fDuration < 0.0f) {
		m_bActive = false;
		return;
	}

	for (int i = 0; i < m_nDroplets; i++) {
		m_pDroplets[i].m_vVelocity.y -= fElapsed * 34.295296f;
		m_pDroplets[i].m_vPos += m_pDroplets[i].m_vVelocity * fElapsed;
	}
}

// particleparty.cpp:1415
void CDroplet::Display(void) {
	if (!m_bActive) {
		return;
	}

	int nColor = ((int)m_fAlpha << 24) | 0xFFFFFF;

	for (int i = 0; i < m_nDroplets; i++) {
		m_pSpr->SetPos(m_pDroplets[i].m_vPos);
		m_pSpr->SetColor(nColor);
		m_pSpr->Render(g_view, 0.0f, 0x20400000);
	}
}

// particleparty.cpp:1430
void CDroplet::MakeParticle(void) {
	for (int i = 0; i < m_nDroplets; i++) {
		m_pDroplets[i].m_vPos = GolfBall().m_pos;
		m_pDroplets[i].m_vVelocity = WVector::UNIT_POS_Z * ((float)(rand() % 1023) * 0.005859375f) * RotMat((float)(rand() % 1023) * 0.0061359233f, 1);
		float t = (float)(rand() % 1023) * 0.0009765625f;
		m_pDroplets[i].m_fSpeed = (1.0f - t) * 10.0f + t * 40.0f;
	}

	m_fAlpha = 255.0f;
	m_fDuration = 1.5f;
	m_bActive = true;
}

// particleparty.cpp:1515
CRipple::CRipple(void) : m_pRipples(NULL), m_nRipples(0) {
	m_pRipples = new sRipple[5];

	AddRipple(15.0f, 0.01f, 255.0f, 255.0f);
	AddRipple(17.0f, 0.2f, 255.0f, 255.0f);
	AddRipple(19.0f, 0.5f, 128.0f, 128.0f);

	m_pSubParticle = new CDroplet;

	Init();
}

// particleparty.cpp:1531
CRipple::~CRipple(void) {
	if (m_pRipples) {
		delete[] m_pRipples;
		m_pRipples = NULL;
	}
	if (m_pSubParticle) {
		delete m_pSubParticle;
		m_pSubParticle = NULL;
	}
}

// particleparty.cpp:1537
void CRipple::Init(void) {
	for (int i = 0; i < m_nRipples; i++) {
		m_pRipples[i].m_bAlive = false;
		m_pRipples[i].m_fRadius = 0.0f;
		m_pRipples[i].m_fPrevX = m_pRipples[i].m_fX;
		m_pRipples[i].m_fWait = 0.0f;
	}

	m_nCount = m_nRipples;
}

// particleparty.cpp:1550
void CRipple::MakeParticle(void) {
	Init();

	for (int i = 0; i < m_nRipples; i++) {
		m_pRipples[i].m_fWait = m_pRipples[i].m_fDelay;
	}

	m_bActive = true;
	m_pSubParticle->m_vCenter = m_vCenter;
	m_pSubParticle->MakeParticle();
}

// particleparty.cpp:1563
void CRipple::AddRipple(float fSize, float fDelay, float fX, float fZ) {
	m_pRipples[m_nRipples].m_fSize = fSize;
	m_pRipples[m_nRipples].m_fDelay = fDelay;
	m_pRipples[m_nRipples].m_fX = fX;
	m_pRipples[m_nRipples].m_fZ = fZ;
	m_nRipples++;
}

// particleparty.cpp:1572
void CRipple::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	for (int i = 0; i < m_nRipples; i++) {
		if (m_pRipples[i].m_fWait > 0.0f) {
			m_pRipples[i].m_fWait -= fElapsed;
			if (m_pRipples[i].m_fWait < 0.0f) {
				m_pRipples[i].m_bAlive = true;
			}
		}

		if (m_pRipples[i].m_bAlive) {
			m_pRipples[i].m_fRadius += fElapsed * m_pRipples[i].m_fSize;
			m_pRipples[i].m_fPrevX -= fElapsed * m_pRipples[i].m_fZ;

			if (m_pRipples[i].m_fPrevX < 0.0f) {
				m_pRipples[i].m_bAlive = false;
				m_nCount--;

				if (m_nCount == 0) {
					m_bActive = false;
				}
			}
		}
	}

	m_pSubParticle->Process(fElapsed);
}

// particleparty.h:16
void CParticle::DisplayShadow(void) {
}

// particleparty.h:25
void CParticle::SetNormal(WVector const& vNormal) {
}

// particleparty.h:28
void CParticle::Generate(void) {
}

// particleparty.h:29
void CParticle::BlowAway(float fPower, int nIndex) {
}

// particleparty.h:378
void CFragment::SetNormal(WVector const& vNormal) {
	m_vNormal = vNormal;
}

// particleparty.cpp:1607
void CRipple::Display(void) {
	if (!m_bActive) {
		return;
	}

	for (int i = 0; i < m_nRipples; i++) {
		if (m_pRipples[i].m_bAlive) {
			for (int j = 0; j < 4; j++) {
				WVector vPos = m_vCenter;
				vPos.x += ((j & 2) ? 1.0f : -1.0f) * m_pRipples[i].m_fRadius * 0.5f;
				vPos.z += ((j & 1) ? 1.0f : -1.0f) * m_pRipples[i].m_fRadius * 0.5f;
				m_buffers[j].m_vPos = vPos;
				m_buffers[j].m_fU = (j & 2) ? 1.0f : 0.0f;
				m_buffers[j].m_fV = (j & 1) ? 0.0f : 1.0f;
				m_buffers[j].m_nColor = ((int)m_pRipples[i].m_fPrevX << 24) | 0xFFFFFF;
			}

			g_view->DrawPolygonFan((WTVertex**)&m_pBuffer0, (m_texHandle & 0x7FF) | 0x20800000, 0x400, false);
		}
	}

	m_pSubParticle->Display();
}

// particleparty.cpp:1649
CCoin::CCoin(int nCount, WVector const& vCenter) {
	m_vOrigin = vCenter;
	m_nCoins = nCount;
	m_pCoins = new sCoin[nCount];
	m_nSparks = 50;
	m_pSparks = new sSpark[50];
}

// particleparty.cpp:1660
CCoin::~CCoin(void) {
	for (int i = 0; i < m_nCoins; i++) {
		if (m_pCoins[i].m_pBody) {
			delete m_pCoins[i].m_pBody;
			m_pCoins[i].m_pBody = NULL;
		}
	}
	if (m_pCoins) { delete[] m_pCoins; m_pCoins = NULL; }
	if (m_pSparks) { delete[] m_pSparks; m_pSparks = NULL; }
	m_nCoins = m_nSparks = 0;
}

// particleparty.cpp:1673
void CCoin::MakeParticle(void) {
	m_fRemain = 1.0f;
	m_fDuration = 3.0f;

	if (m_pAniSpr) { delete m_pAniSpr; m_pAniSpr = NULL; }
	m_pAniSpr = new W3dAniSpr;
	m_pAniSpr->SetResourceManager(g_resrcmng);
	m_pAniSpr->LoadTexture(m_texHandle, 32.0f, 32.0f);
	m_pAniSpr->SetRect(0.3f, 0.3f, 0.15f, 0.15f);

	m_nDropSounds = 0;
	Generate();

	SetActive(true);
}

// particleparty.cpp:1752
bool CCoin::ShouldDisplay(void) {
	return m_bActive;
}

// particleparty.cpp:1757
void CCoin::SetActive(bool bActive) {
	m_bActive = bActive;
}

// particleparty.cpp:1897
CEnergyCenter::CEnergyCenter(int nCount, WVector const& vCenter, float fRadius) {
	m_fRadius = fRadius; m_nSparks = nCount;
	m_vOrigin = vCenter;

	m_fLife = 1.0f; m_fTime = 0.0f; m_fAlpha = 255.0f;
	m_fMaxAlpha = 255.0f;
	m_pGolfBg = AfxGetTask()->GetActor("GolfBg");
	m_pSparks = new sSpark[m_nSparks];
}

// particleparty.cpp:1855
void CCoin::Display(void) {
	if (!m_bActive) {
		return;
	}

	for (int i = 0; i < m_nCoins; i++) {
		m_pCoins[i].m_pBody->Display();
	}

	for (int j = 0; j < m_nSparks; j++) {
		sSpark* pSpark = &m_pSparks[j];

		m_pAniSpr->SetColor(((int)pSpark->m_fAlpha << 24) | 0xFFFFFF);
		m_pAniSpr->SetPos(pSpark->m_vPos);
		m_pAniSpr->Render(g_view, 0.0f, 0x20C00000, 5);
	}
}

// particleparty.cpp:1876
void CCoin::DisplayShadow(void) {
	if (!m_bActive) {
		return;
	}

	char szName[32];

	sprintf(szName, "coin%x", this);

	CShadowManager::Instance()->RegisterCaster_New(szName, 16, "sun", g_lightset);

	for (int i = 0; i < m_nCoins; i++) {
		m_pCoins[i].m_pBody->DisplayShadow(szName);
	}
}

// particleparty.cpp:1910
CEnergyCenter::~CEnergyCenter(void) {
	if (m_pSparks) {
		delete[] m_pSparks;
		m_pSparks = NULL;
	}
	m_nSparks = 0;
}

// particleparty.cpp:1916
void CEnergyCenter::MakeParticle(void) {
	m_nGenerated = 0;
	m_bActive = true;

	if (m_pAniSpr) {
		delete m_pAniSpr;
		m_pAniSpr = NULL;
	}
	m_pAniSpr = new W3dAniSpr;
	m_pAniSpr->SetResourceManager(g_resrcmng);
	m_pAniSpr->LoadTexture(m_texHandle, 32.0f, 32.0f);
}

// particleparty.cpp:1927
void CEnergyCenter::Generate(float fElapsed) {
	for (int i = 0; m_nGenerated < m_nSparks && i < fElapsed * 35.0f; i++) {
		sSpark* pSpark = &m_pSparks[m_nGenerated];

		pSpark->m_vPos = WVector((float)(rand() % 31 - 15) * 0.05f + 8.0f, (float)(rand() % 31 - 15) * 0.05f + 8.0f, 0.0f)
			* RotMat((float)m_nSparks * 6.2831855f / (float)(rand() % m_nSparks + 1), 1) + m_vOrigin;
		pSpark->m_vDir = m_vOrigin - pSpark->m_vPos;
		pSpark->m_vDir = pSpark->m_vDir.Normalize();

		pSpark->m_fAlpha = 0.0f;
		pSpark->m_bAlive = true;
		m_nGenerated++;
	}
}

// particleparty.cpp:1942
void CEnergyCenter::Process(float fElapsed) {
	if (!m_bActive) {
		return;
	}

	Generate(fElapsed);

	bool bAllArrived = true;

	for (int i = 0; i < m_nGenerated; i++) {
		sSpark* pSpark = &m_pSparks[i];

		if (pSpark->m_bAlive) {
			pSpark->m_fAlpha = Min(fElapsed * 1000.0f + pSpark->m_fAlpha, 255.0f);

			pSpark->m_vPos += pSpark->m_vDir * 20.0f * fElapsed;

			WVector vRel = pSpark->m_vPos - m_vOrigin;
			pSpark->m_vPos = vRel * RotMat(m_fRadius, 1) + m_vOrigin;

			if (pSpark->m_vPos.y * pSpark->m_vDir.y >= m_vOrigin.y * pSpark->m_vDir.y) {
				m_fLife += 255.0f / (float)m_nSparks;
				pSpark->m_bAlive = false;
			} else {
				bAllArrived = false;
			}

			pSpark->m_vDir = m_vOrigin - pSpark->m_vPos;
			pSpark->m_vDir = pSpark->m_vDir.Normalize();
		}
	}

	m_fTime -= fElapsed * 1.5707964f;

	if (bAllArrived) {
		m_fLife -= fElapsed * 400.0f;
		if (m_fLife < 0.0f) {
			m_bActive = false;
		}
	} else {
		m_fAlpha = Max(m_fAlpha - fElapsed * 250.0f, 160.0f);
	}
}

// particleparty.cpp:2013
void CEnergyCenter::Display(void) {
	if (!m_bActive) {
		return;
	}

	if (GOLFDOC()->m_day && COption::Instance()->GetOption()->m_particleLevel < 1) {
		if (!WisEqual(m_fAlpha, m_fMaxAlpha, 0.00001f)) {
			m_fMaxAlpha = m_fAlpha;

			unsigned char byAlpha = (unsigned char)float2int(m_fAlpha);
			unsigned long dwColor = (((0xFFFFFF00 | byAlpha) << 8 | byAlpha) << 8) | byAlpha;

			GetPVS().SetBaseVtxColor(dwColor);
			GetPVS().SetObjVtxColor(dwColor);

			if (m_pGolfBg) {
				m_pGolfBg << MsgObject(0, 0x13F, dwColor, 0, 0, 0, 0);
			}
		}
	}

	m_pAniSpr->SetColor(((int)m_fLife << 24) | 0xFFFFFF);
	m_pAniSpr->SetPos(m_vOrigin);
	m_pAniSpr->SetRect(1.0f, 1.0f, 0.5f, 0.5f);
	m_pAniSpr->Render(g_view, m_fTime, 0xF00000, 4);

	for (int i = 0; i < m_nGenerated; i++) {
		sSpark* pSpark = &m_pSparks[i];

		if (pSpark->m_bAlive) {
			m_pAniSpr->SetColor(((int)pSpark->m_fAlpha << 24) | 0xFFFFFF);
			m_pAniSpr->SetPos(pSpark->m_vPos);
			m_pAniSpr->SetRect(0.5f, 0.5f, 0.25f, 0.25f);
			m_pAniSpr->Render(g_view, 0.0f, 0x20C00000, 6);
		}
	}
}

// particleparty.cpp:2068
CStarDust::CStarDust(void) {
	m_fRate = 0.01f;
	m_nParticles = 400;
	m_pParticles = new sParticle[m_nParticles];
}

// particleparty.cpp:2075
CStarDust::~CStarDust(void) {
	if (m_pParticles) {
		delete[] m_pParticles;
		m_pParticles = NULL;
	}
	m_nParticles = 0;
}

// particleparty.cpp:2081
void CStarDust::MakeParticle(void) {
	if (m_pAniSpr) { delete m_pAniSpr; m_pAniSpr = NULL; }
	m_pAniSpr = new W3dAniSpr;
	m_pAniSpr->SetResourceManager(g_resrcmng);
	m_pAniSpr->LoadTexture(m_texHandle, 32.0f, 32.0f);

	m_nGenerated = 0; m_fAlpha = 255.0f; m_fGenTime = 0.0f; m_fFade = 0.0f; m_bActive = true; for (int i = 0; i < m_nParticles; i++) {
		m_pParticles[i].m_bAlive = false;
	}
}

extern WVector data_00b7bfa0;

// particleparty.cpp:2100
void CStarDust::Generate(void) {
	WVector vSide = WCrossProduct(data_00b7bfa0, GolfBall().m_pos);
	float fRadius = GolfBall().m_radius;
	vSide = vSide.Normalize() * 2.0f * fRadius;

	sParticle* p = &m_pParticles[m_nGenerated];

	p->m_bAlive = true;
	p->m_fRadius = GolfBall().m_radius;
	p->m_fAngularSpeed = (float)(rand() % 1023) * 0.0015339808f + 1.5707964f;
	p->m_vAxisX = WVector((float)(rand() % 1024 - 512) * 0.001953125f, (float)(rand() % 1024 - 512) * 0.001953125f, (float)(rand() % 1024 - 512) * 0.001953125f) * 2.0f + data_00b7bfa0;
	p->m_vAxisY = WVector((float)(rand() % 1024 - 512) * 0.001953125f, (float)(rand() % 1024 - 512) * 0.001953125f, (float)(rand() % 1024 - 512) * 0.001953125f) * 2.0f + vSide * (1.0f / vSide.Magnitude());
	p->m_vOrigin = GolfBall().m_pos;
	p->m_fLife = 1.0f;
	p->m_fAngle = (float)(rand() % 1023) * 0.0061359233f;
}

// particleparty.cpp:2118
void CStarDust::BlowAway(float fPower, int nIndex) {
	sParticle* p = &m_pParticles[nIndex];

	if (p->m_bAlive) {
		float fSpeed = fPower * 2.0f;

		p->m_fAngularSpeed -= fSpeed; p->m_fAngle -= p->m_fAngularSpeed * fPower; p->m_vPos = (p->m_vAxisX * cosf(p->m_fAngle) + p->m_vAxisY * sinf(p->m_fAngle)) * p->m_fRadius + p->m_vOrigin;
		p->m_fRadius += fSpeed;
		p->m_fLife -= fPower * 0.3f;

		if (p->m_fLife < 0.0f) {
			p->m_bAlive = false;
		}
	}
}

// particleparty.cpp:2135
void CStarDust::Process(float fElapsed) {
	if (m_bActive) {
		WPuppet* pPuppet;
		int nCount;

		if (GolfBall().m_pos.y < -1.0f) {
			m_fGenTime += Max(fElapsed * m_fAlpha * 0.005f, fElapsed);
			m_fAlpha = Max(m_fAlpha - fElapsed * 127.0f, 0.0f);

			pPuppet = CSceneManager::Instance()->GetPuppet("°ø");
			if (pPuppet && pPuppet->GetRootBone()) {
				pPuppet->SetAlpha(255 - (unsigned char)(int)m_fAlpha);
			}

			if (Abs(m_fAlpha) < 0.00001f) {
				m_bActive = false;
				m_fAlpha = 0.0f;
				if (pPuppet && pPuppet->GetRootBone()) {
					pPuppet->SetAlpha(255);
				}
			}
		} else {
			m_fGenTime += fElapsed;
		}

		for (int i = 0; i < m_nParticles; i++) {
			BlowAway(fElapsed, i);
		}

		if (m_fGenTime > m_fRate) {
			if (fElapsed > m_fRate) {
				nCount = (int)(m_fGenTime / m_fRate);
			} else {
				nCount = 1;
			}

			m_fGenTime -= nCount * m_fRate;

			while (nCount > 0) {
				for (int k = 2; k != 0; k--) {
					Generate();

					if (fElapsed > m_fRate) {
						BlowAway(nCount * m_fRate + m_fGenTime, m_nGenerated);
					}

					m_nGenerated = (m_nGenerated + 1) % m_nParticles;
				}

				nCount--;
			}
		}

		m_fFade -= fElapsed * 18.78f;
	}
}

// particleparty.cpp:2203
void CStarDust::Display(void) {
	if (!m_bActive) {
		return;
	}

	int nColor = ((int)m_fAlpha << 24) | 0xFFFFFF;

	m_pAniSpr->SetRect(1.0f, 1.0f, 0.5f, 0.5f);
	m_pAniSpr->SetColor(nColor);
	m_pAniSpr->SetPos(GolfBall().m_pos);
	m_pAniSpr->Render(g_view, m_fFade, 0x20C00000, 4);

	for (int i = 0; i < m_nParticles; i++) {
		sParticle* p = &m_pParticles[i];

		if (p->m_bAlive) {
			float fSize = p->m_fLife * 0.6f;

			m_pAniSpr->SetPos(p->m_vPos);
			m_pAniSpr->SetRect(fSize, fSize, fSize * 0.5f, fSize * 0.5f);
			m_pAniSpr->SetColor(nColor);
			m_pAniSpr->Render(g_view, 0.0f, 0x20C00000, 5);
		}
	}
}

// particleparty.cpp:2237
CStarDust2::CStarDust2(void) {
	m_fRate = 0.1f;
	m_nParticles = 60;
	m_pParticles = new sParticle[m_nParticles];
}

// particleparty.cpp:2244
CStarDust2::~CStarDust2(void) {
	if (m_pParticles) {
		delete[] m_pParticles;
		m_pParticles = NULL;
	}
	m_nParticles = 0;
}

// particleparty.cpp:2250
void CStarDust2::MakeParticle(void) {
	if (m_pAniSpr) { delete m_pAniSpr; m_pAniSpr = NULL; }
	m_pAniSpr = new W3dAniSpr;
	m_pAniSpr->SetResourceManager(g_resrcmng);
	m_pAniSpr->LoadTexture(m_texHandle, 32.0f, 32.0f);

	m_nGenerated = 0; m_fAlpha = 255.0f; m_fGenTime = 0.0f; m_fFade = 0.0f; m_bActive = true; for (int i = 0; i < m_nParticles; i++) {
		m_pParticles[i].m_bAlive = false;
	}
}

// particleparty.cpp:2269
void CStarDust2::Generate(void) {
	WVector vSide = WCrossProduct(data_00b7bfa0, GolfBall().m_pos);
	float fRadius = GolfBall().m_radius;
	vSide = vSide.Normalize() * 2.0f * fRadius;

	for (int i = 0; i < 4; i++) {
		sParticle* p = &m_pParticles[m_nGenerated];
		m_nGenerated = (m_nGenerated + 1) % m_nParticles;

		p->m_bAlive = true;
		p->m_fRadius = GolfBall().m_radius * 4.0f;
		p->m_fAngularSpeed = (float)(rand() % 1023) * 0.0015339808f + 1.5707964f;
		p->m_vAxisX = WVector((float)(rand() % 1024 - 512) * 0.001953125f, (float)(rand() % 1024 - 512) * 0.001953125f, (float)(rand() % 1024 - 512) * 0.001953125f) * 2.0f + data_00b7bfa0;
		p->m_vAxisY = WVector((float)(rand() % 1024 - 512) * 0.001953125f, (float)(rand() % 1024 - 512) * 0.001953125f, (float)(rand() % 1024 - 512) * 0.001953125f) * 2.0f + vSide * (1.0f / vSide.Magnitude());
		p->m_vOrigin = GolfBall().m_pos;
		p->m_fAngle = (float)(rand() % 1023) * 0.0061359233f;
		p->m_fLife = 255.0f;
	}
}

// particleparty.cpp:2291
void CStarDust2::Process(float fElapsed) {
	if (m_bActive) {
		if (GolfBall().m_pos.y < 0.0f) {
			m_fGenTime += Max(fElapsed * m_fAlpha * 0.005f, fElapsed);
			m_fAlpha = Max(m_fAlpha - fElapsed * 150.0f, 0.0f);

			CSceneManager::Instance()->GetPuppet("€Ï")->SetAlpha(255 - (unsigned char)(int)m_fAlpha);

			if (Abs(m_fAlpha) < 0.00001f) {
				m_bActive = false;
				m_fAlpha = 0.0f;
				CSceneManager::Instance()->GetPuppet("€Ï")->SetAlpha(255);
			}
		} else {
			m_fGenTime += fElapsed;
		}

		if (m_fGenTime > m_fRate) {
			m_fGenTime -= m_fRate;
			Generate();
		}

		m_fFade -= fElapsed * 18.78f;

		for (int i = 0; i < m_nParticles; i++) {
			sParticle* p = &m_pParticles[i];

			if (p->m_bAlive) {
				float fSpeed = fElapsed * 2.0f;

				p->m_fAngularSpeed -= fSpeed; p->m_fAngle -= p->m_fAngularSpeed * fElapsed; p->m_vPos = (p->m_vAxisX * cosf(p->m_fAngle) + p->m_vAxisY * sinf(p->m_fAngle)) * p->m_fRadius + p->m_vOrigin;
				p->m_fRadius += fSpeed;
				p->m_fLife = Max(p->m_fLife - fElapsed * 100.0f, 0.0f);

				if (GolfBall().m_radius * 0.5f > p->m_fRadius) {
					p->m_bAlive = false;
				}
			}
		}
	}
}

// particleparty.cpp:2350
void CStarDust2::Display(void) {
	if (!m_bActive) {
		return;
	}

	int nColor = ((int)m_fAlpha << 24) | 0xFFFFFF;

	m_pAniSpr->SetColor(nColor);
	m_pAniSpr->SetPos(GolfBall().m_pos);
	m_pAniSpr->SetRect(2.0f, 2.0f, 1.0f, 1.0f);
	m_pAniSpr->Render(g_view, m_fFade, 0x20F00000, 5);

	for (int i = 0; i < m_nParticles; i++) {
		sParticle* p = &m_pParticles[i];

		if (p->m_bAlive) {
			m_pAniSpr->SetPos(p->m_vPos);
			m_pAniSpr->SetRect(0.5f, 0.5f, 0.25f, 0.25f);
			m_pAniSpr->SetColor((((int)p->m_fLife << 24) | 0xFFFFFF) & nColor);
			m_pAniSpr->Render(g_view, 0.0f, 0xF00000, 6);
		}
	}
}
