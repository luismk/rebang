// Original: d:\build\custom\temp\code\client\projectg\fallingspot.cpp
// actor.h names IActor in MsgObject before declaring it.
class IActor;

#include "fallingspot.h"

#include <new>
#include <math.h>

#include "ObjectFactory.h"
#include "3dfont.h"
#include "golfball.h"
#include "wview.h"
#include "wresrcmng.h"
#include "woverlay.h"
#include "fontinfo.h"
#include "SimpleOverlay.h"
#include "club.h"
#include "quadtree.h"
#include "SharedDoc.h"

extern WMatrix g_camera;

float Random(float min, float max, int seed);

// fallingspot.cpp:28 (run-time type and factory registration of CSpot)
const WRTTI CSpot::m_RTTI("CSpot", &IActor::m_RTTI);

IObject* CSpotMakeInstance(void) {
	return new CSpot;
}

struct __impCSpot {
	__impCSpot(void) {
		ObjectFactory().AddObjectFunctor(CSpotMakeInstance, "CSpot");
		ActorFactory().AddActorClass("CSpot");
	}
};
static __impCSpot s_impCSpot;

// fallingspot.cpp:43
CSpot::CSpot(void) : m_pFont1c(NULL), m_pFont20(NULL), m_pRangeInfo(NULL), m_hTexture9c(0), m_hTexturea0(0), m_hTexturea4(0), m_hTexturea8(0), m_pOverlayc4(NULL), m_pOverlayc8(NULL), m_pSimpleUI(NULL), m_hTextured4(0), m_pRivals(NULL), m_unknown19c(0) {
	AddSkipList(0x1400);

	m_pVertices[0] = &m_vertices[0];
	m_pVertices[1] = &m_vertices[1];
	m_pVertices[3] = &m_vertices[2];

	m_unknowne8 = 0;
	m_pOverlaybc = NULL;
	m_pOverlayc0 = NULL;
	m_priority = 1;
	m_pVertices[2] = &m_vertices[3];
}

// fallingspot.cpp:59
void CSpot::OnPreLoadInit(void) {
	m_bRegistered = true;
	CSceneManager::Instance()->RegisterElement((eObjType)5, 0, this, "\xb3\xab\xc7\xcf\xc1\xf6\xc1\xa1", 0, 0);
}

// fallingspot.cpp:66
void CSpot::OnDestroy(void) {
	if (g_resrcmng != NULL && m_pOverlayc4 != NULL) { g_resrcmng->Release(m_pOverlayc4); m_pOverlayc4 = NULL; }
	if (g_resrcmng != NULL && m_pOverlaybc != NULL) { g_resrcmng->Release(m_pOverlaybc); m_pOverlaybc = NULL; }
	if (g_resrcmng != NULL && m_pOverlayc0 != NULL) { g_resrcmng->Release(m_pOverlayc0); m_pOverlayc0 = NULL; }
	if (g_resrcmng != NULL && m_hTexture9c != 0) { g_resrcmng->Release(m_hTexture9c); m_hTexture9c = 0; }
	if (g_resrcmng != NULL && m_hTexturea0 != 0) { g_resrcmng->Release(m_hTexturea0); m_hTexturea0 = 0; }
	if (g_resrcmng != NULL && m_hTexturea4 != 0) { g_resrcmng->Release(m_hTexturea4); m_hTexturea4 = 0; }
	if (g_resrcmng != NULL && m_pOverlayc8 != NULL) { g_resrcmng->Release(m_pOverlayc8); m_pOverlayc8 = NULL; }
	if (g_resrcmng != NULL && m_hTextured4 != 0) { g_resrcmng->Release(m_hTextured4); m_hTextured4 = 0; }
	if (g_resrcmng != NULL && m_hTexturea8 != 0) { g_resrcmng->Release(m_hTexturea8); m_hTexturea8 = 0; }

	if (m_pSimpleUI != NULL) { delete m_pSimpleUI; m_pSimpleUI = NULL; }
	if (m_pFont20 != NULL) { delete m_pFont20; m_pFont20 = NULL; }
	if (m_pFont1c != NULL) { delete m_pFont1c; m_pFont1c = NULL; }
	if (m_pRangeInfo != NULL) { delete m_pRangeInfo; m_pRangeInfo = NULL; }

	if (m_pRivals != NULL) { delete[] m_pRivals; m_pRivals = NULL; }
}

// fallingspot.cpp:163
void CSpot::OnInit(void) {
	m_unknown78 = 0;
	m_angle = 0.0f;
	m_unknown7c = 0;
	m_spotPos.x = m_spotPos.y = m_spotPos.z = 0.0f;
	m_unknown68.x = m_unknown68.y = m_unknown68.z = 0.0f;
	m_unknown30.x = m_unknown30.y = m_unknown30.z = 0.0f;

	m_unknown80 = true;
	m_unknown18c = true;
	m_unknown4c = 1.0f;
	m_unknown84 = 2.0f;
	m_unknown88 = 12.0f;
	m_unknown94 = 20.0f;
	m_unknown98 = 20.0f;
	m_unknown74 = 0.0f;
	m_unknownac = 0;
	m_unknownd0 = 0;

	m_unknownb4 = 0;
	m_unknownb8 = 1.0f;

	m_unknown50.x = m_unknown50.y = m_unknown50.z = 0.0f;
	float angle = Random(-3.1415927f, 3.1415927f, -1);
	m_dir = Random(0.0f, 0.02f * (GolfClub().GetRange(0) * 3.2f), -1) * WVector(cosf(angle), 0.0f, sinf(angle));
}

// fallingspot.cpp:1082
bool CSpot::_ValidTopViewLineDraw(void) {
	WView* pView = g_view;
	if (pView->InFrustum(GolfBall().m_pos) || pView->InFrustum(m_spotPos)) {
		return true;
	}

	WVector cam = g_camera.pivot;
	WVector2D ball(GolfBall().m_pos.x, GolfBall().m_pos.z);

	WVector2D toCam(cam.x - ball.x, cam.z - ball.y);
	WVector2D toSpot(m_spotPos.x - ball.x, m_spotPos.z - ball.y);
	float camDist = sqrtf(toCam.y * toCam.y + toCam.x * toCam.x);
	if (sqrtf(toSpot.x * toSpot.x + toSpot.y * toSpot.y) > camDist) {
		return false;
	}

	return true;
}

// fallingspot.cpp:1240
void CSpot::Rotate(void) {
	m_unknown78 = 0;
	WVector pos = GolfClub().GetRange(0) * 3.2f * RotMat(m_angle, 1).za + GolfBall().m_pos;

	m_unknown68 = GetPVS().GetGroundPoint(pos, true, &m_unknown78, true);

	if (fabsf(m_unknown74) < 0.00001f) {
		m_spotPos = m_unknown68;
	}
}

// fallingspot.cpp:1356
void CSpot::ClipIndicator(int index) {
	if (m_vertices[index].sx < 0.5f) {
		float d = (0.5f - m_vertices[index].sx) / m_unknown94;
		m_vertices[index].sx = 0.5f; m_vertices[index].tu += d;
	} else if (g_view->GetWidth() - 0.5f < m_vertices[index].sx) {
		m_vertices[index].tu -= (m_vertices[index].sx - g_view->GetWidth() + 0.5f) / m_unknown94;
		m_vertices[index].sx = g_view->GetWidth() - 0.5f;
	}

	if (m_vertices[index].sy < 0.5f) {
		float d = (0.5f - m_vertices[index].sy) / m_unknown98;
		m_vertices[index].sy = 0.5f; m_vertices[index].tv += d;
	} else if (g_view->GetHeight() - 0.5f < m_vertices[index].sy) {
		m_vertices[index].tv -= (m_vertices[index].sy - g_view->GetHeight() + 0.5f) / m_unknown98;
		m_vertices[index].sy = g_view->GetHeight() - 0.5f;
	}
}

// fallingspot.cpp:1381
float CSpot::GetMovingRate(float t) {
	return (3.0f - 2.0f * t) * t * t;
}

// fallingspot.cpp:1387
bool CSpot::UpdateRivals(void) {
	if (m_pRivals) {
		delete[] m_pRivals;
		m_pRivals = NULL;
	}

	int count = CSharedDoc::Instance()->m_gameResultList.size();
	if (count > 0) {
		m_pRivals = new sRivalPos[count];

		for (unsigned char i = 0; i < count; i++) {
			m_pRivals[i].m_unknown0 = 0;

			if (CSharedDoc::Instance()->m_roomType == 10) {
				m_pRivals[i].m_unknown4 = 0;
			}

			m_pRivals[i].m_hole = CSharedDoc::Instance()->m_gameResultList[i].m_hole;
			m_pRivals[i].m_pos = CSharedDoc::Instance()->m_gameResultList[i].m_ballPos;
		}
	} else {
		return false;
	}

	return true;
}
