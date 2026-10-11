// Original: d:\build\custom\temp\code\client\projectg\grounditem.cpp
#include "grounditem.h"
#include "GroundItemMan.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "golfball.h"
#include "IContentsDoc.h"
#include "scenemanager.h"
#include "wpuppet.h"
#include "wresrcmng.h"
#include "wscene.h"
#include "wview.h"

// ?g_lightset@@3ULightSet@@A; its header (wscene.h) does not declare it yet.
extern LightSet g_lightset;

// grounditem.cpp:23 (run-time type and factory registration of GroundItem)
const WRTTI GroundItem::m_RTTI("GroundItem", &IObject::m_RTTI);

IObject* GroundItemMakeInstance(void) {
	return new GroundItem;
}

struct __sGroundItem {
	__sGroundItem(void) {
		ObjectFactory().AddObjectFunctor(GroundItemMakeInstance, "GroundItem");
	}
};
static __sGroundItem s_registerGroundItem;

// grounditem.cpp:26
GroundItem::BallInfo GroundItem::ms_ballInfo;

unsigned int GroundItem::ms_initCnt;
bool GroundItem::ms_greenItemJackpot;

// grounditem.cpp:31
const ThSphere &GroundItem::GetBallBoundingVolume(void) {
	return ms_ballInfo.m_sphere;
}

// grounditem.cpp:37
void GroundItem::EnableGreenItemJackpot(bool enable) {
	ms_greenItemJackpot = enable;
}

// grounditem.cpp:42
// Works on ms_ballInfo whatever the object (the original addresses the
// static directly).
const ThSphere &GroundItem::BallInfo::MakeBallBoundingVolume(int frame) {
	if (frame == ms_ballInfo.m_frame) {
		return ms_ballInfo.m_sphere;
	}

	WVector prevPos = GolfBall().PreData(frame - 1)->m_pos;
	WVector curPos = GolfBall().PreData(frame)->m_pos;

	WVector sum = prevPos + curPos; WVector dir = curPos - prevPos;

	ms_ballInfo.m_frame = frame;

	float ballRadius = GolfBall().GetRadius(); ms_ballInfo.m_sphere.center.x = sum.x * 0.5f; ms_ballInfo.m_sphere.center.y = sum.y * 0.5f; ms_ballInfo.m_sphere.center.z = sum.z * 0.5f;

	ms_ballInfo.m_sphere.radius = dir.Magnitude() + ballRadius;

	return ms_ballInfo.m_sphere;
}

// grounditem.cpp:73
GroundItem::GroundItem(void) {
}

// grounditem.cpp:88
GroundItem::GroundItem(GroundItemMan *owner, unsigned long id, GIMMICK_ITEM_TYPE itemType, FieldItem::eDisposeTextureType disposeTextureType, const char *puppetName, const Ut::Spatial &spatial)
	: m_owner(owner), m_id(id), m_itemType(itemType), m_state(GIS_NONE), m_enabled(true), m_disposeTextureType(disposeTextureType), m_puppetName(puppetName), m_puppet(0), m_spatial(spatial) {
	WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0xa3, m_gimmickContainer);

	m_idleFrame = m_startFrame = m_endFrame = -1;
}

// grounditem.cpp:101
GroundItem::~GroundItem(void) {
}

// grounditem.cpp:106
void GroundItem::Initialize(void) {
	m_puppet = g_resrcmng->GetPuppet(m_puppetName.c_str(), false, true, false);

	Ut::MakeObjectName(m_sceneElemName, "SceneElemName", *this, ms_initCnt++);

	CSceneManager::Instance()->RegisterElement((eObjType)4, m_puppet, 0, m_sceneElemName.c_str(), 0x18, 0);

	m_state = GIS_IDLE; m_spatial.ToMatrix(m_worldMat);
	Transform();
}

// grounditem.cpp:136
void GroundItem::Terminate(void) {
	CSceneManager::Instance()->DeleteElement(m_sceneElemName.c_str());
	if (g_resrcmng && m_puppet) { g_resrcmng->Release(m_puppet, false); m_puppet = 0; }
	m_puppet = 0;
}

// grounditem.cpp:143
void GroundItem::Transform(void) {
	m_puppet->UpdateLightSource(&g_lightset, false, 0);

	m_puppet->ApplyBones(0, m_worldMat);

	m_puppet->Transform(0, 1.0f, 0);

	m_puppet->UpdateBound(true, false);
}

// grounditem.cpp:157
void GroundItem::NSC_OnStep(bool active, bool replay, float elapsed, float syncTime, int frame) {
	if (frame == -1) {
		ms_ballInfo.Reset();
	}

	if (!NeedToUpdate()) {
		return;
	}

	eGoundItemUpdateType type = GIUT_NONE;

	switch (m_itemType) {
	case 0:
		type = m_disposeTextureType == 1 ? GIUT_DISPOSE_TEXTURE_1 : GIUT_ITEM_TYPE_0;
		break;

	case 1:
		type = m_disposeTextureType == 1 ? GIUT_DISPOSE_TEXTURE_1 : GIUT_ITEM_TYPE_1;
		break;
	}

	Update(type, active, replay, elapsed, syncTime, frame);
}

// grounditem.cpp:198
void GroundItem::NSC_OnFirstNewShotFrame(float startTime, int frame) {
	m_startFrame = m_endFrame = -1;

	if (m_state == GIS_ACTIVE) {
		ChangeState(GIS_IDLE, frame);
	}
}

// grounditem.cpp:211
void GroundItem::NSC_OnFirstReplayFrame(int frame) {
	if (m_state == GIS_ACTIVE) {
		if (frame <= m_startFrame) {
			ChangeState(GIS_IDLE, frame);
		}
	}

	if (m_state == GIS_REMOVED) {
		if (frame <= m_endFrame) {
			ChangeState(GIS_IDLE, frame);

			GroundItemMan::PlayerStats &stats = m_owner->m_playerStats[GOLFDOC()->m_myPlayerIndex];
			stats.m_shotCount[m_itemType]--;
		}
	}
}

// grounditem.cpp:241
void GroundItem::DrawBoundingVolume(void) {
	if (NeedToUpdate()) {
		WVector center;
		float radius;
		GetBoundingSphere(center, radius);
		g_view->DrawSphere(center, radius, 0xffff0000, 10, 0);

		g_view->DrawAABB(m_puppet->GetBoundBox(), 0xff00ff00, false, 0);
	}
}

// grounditem.cpp:261
void GroundItem::Enable(bool enable) {
	m_enabled = enable;

	CSceneManager::Instance()->SetVisible(m_sceneElemName.c_str(), NeedToUpdate());
}

// grounditem.cpp:274
void GroundItem::GetBoundingSphere(gaV3 &center, float &radius) const {
	const WSphere &sphere = m_puppet->GetBoundSphere();
	center = reinterpret_cast<const gaV3 &>(sphere.pos);
	radius = sphere.radius * 0.7f;
}

// grounditem.cpp:281
void GroundItem::GetBoundingSphere(WVector &center, float &radius) const {
	const WSphere &sphere = m_puppet->GetBoundSphere();
	center = sphere.pos;
	radius = sphere.radius * 0.7f;
}

// grounditem.cpp:288
GroundItem::eGroundItemState GroundItem::ChangeState(eGroundItemState state, int frame) {
	eGroundItemState prevState = m_state;
	m_state = state;
	OnChangeState(frame);
	return prevState;
}

// grounditem.cpp:383
bool GroundItem::TestIntersection(int frame) {
	const ThSphere &ball = ms_ballInfo.MakeBallBoundingVolume(frame);

	ThSphere item;
	GetBoundingSphere(item.center, item.radius);

	float dx = ball.center.x - item.center.x;
	float dy = ball.center.y - item.center.y;
	float dz = ball.center.z - item.center.z;
	float sumRadius = item.radius + ball.radius;

	if (dx * dx + dy * dy + dz * dz <= sumRadius * sumRadius) {
		return true;
	}

	return false;
}

// grounditem.cpp:410
bool GroundItem::TestIntersection2(int frame, float ballRadius) {
	ThSphere ball;
	ball.center = ms_ballInfo.MakeBallBoundingVolume(frame).center;

	ThSphere item;
	GetBoundingSphere(item.center, item.radius);

	float dx = ball.center.x - item.center.x;
	float dy = ball.center.y - item.center.y;
	float dz = ball.center.z - item.center.z;
	float sumRadius = item.radius + ballRadius;

	if (dx * dx + dy * dy + dz * dz <= sumRadius * sumRadius) {
		return true;
	}

	return false;
}
