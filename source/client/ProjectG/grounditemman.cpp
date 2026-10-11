// Original: d:\build\custom\temp\code\client\projectg\grounditemman.cpp
// actor.h names IActor in MsgObject before declaring it.
class IActor;

#include "GroundItemMan.h"

#include <new>

#include "ObjectFactory.h"
#include "grounditem.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "wview.h"

unsigned int GroundItemMan::ms_initCnt;

// grounditemman.cpp:42 (run-time type and factory registration of GroundItemMan)
const WRTTI GroundItemMan::m_RTTI("GroundItemMan", &NetSyncActor::m_RTTI);

IObject* GroundItemManMakeInstance(void) {
	return new GroundItemMan;
}

struct __impGroundItemMan {
	__impGroundItemMan(void) {
		ObjectFactory().AddObjectFunctor(GroundItemManMakeInstance, "GroundItemMan");
		ActorFactory().AddActorClass("GroundItemMan");
	}
};
static __impGroundItemMan s_impGroundItemMan;

// grounditemman.cpp:47
GroundItemMan::GroundItemMan(void) {
	WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0xa3, m_gimmickContainer);
}

// grounditemman.cpp:53
GroundItemMan::~GroundItemMan(void) {
}

// grounditemman.cpp:58
void GroundItemMan::OnLoad(void) {
	if (!OnlinePlay()) {
		m_gimmickContainer->LoadFromScript("test_table");
	}

	m_playerStats = new PlayerStats[GOLFDOC()->GetPlayerNum()];
}

// grounditemman.cpp:68
void GroundItemMan::OnReload(void) {
}

// grounditemman.cpp:130
void GroundItemMan::OnDestroy(void) {
	delete[] m_playerStats;

	m_gimmickContainer->Release();

	OnHoleOut();

	NetSyncActor::OnDestroy();
}

// grounditemman.cpp:145
void GroundItemMan::HandleMsg(const MsgObject &msg) {
	NetSyncActor::HandleMsg(msg);

	switch (msg.m_msg) {
	case 0x277:
		*(std::vector<FieldItem::AcquiredItemInfo>**)msg.m_param1 = &m_gimmickContainer->GetAcquiredItemInfoArray();
		break;
	case 0x27c:
		*(int*)msg.m_param1 = m_groundItems.size();
		break;
	case 0x27b:
		EnableGroundItemAll(msg.m_param1 == 1);
		break;
	}
}

// grounditemman.cpp:179
void GroundItemMan::NSC_OnStep(bool active, bool replay, float elapsed, float syncTime, int frame) {
	std::list<GroundItem*>::iterator it = m_groundItems.begin();
	for (; it != m_groundItems.end(); ++it) {
		(*it)->NSC_OnStep(active, replay, elapsed, syncTime, frame);
	}
}

// grounditemman.cpp:191
void GroundItemMan::NSC_OnFirstNewShotFrame(float startTime, int frame) {
	bool jackpot = false;

	unsigned long chance = NSA_GetRandomGenerator().randInt(999999);

	if ((CSharedDoc::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId & 0x3FFFFFF) == 0) {
		if (chance < 20000) {
			jackpot = true;
		}
	} else {
		if (chance < 40000) {
			jackpot = true;
		}
	}

	GroundItem::EnableGreenItemJackpot(jackpot);

	PlayerStats &stats = m_playerStats[GOLFDOC()->m_myPlayerIndex];

	stats.m_shotCount[0] = 0;
	stats.m_shotCount[1] = 0;
	stats.m_shotCount[2] = 0;

	std::list<GroundItem*>::iterator it = m_groundItems.begin();

	for (; it != m_groundItems.end(); ++it) {
		GroundItem *item = *it;
		item->NSC_OnFirstNewShotFrame(startTime, frame);
	}
}

// grounditemman.cpp:236
void GroundItemMan::NSC_OnFirstReplayFrame(int frame) {
	std::list<GroundItem*>::iterator it = m_groundItems.begin();

	for (; it != m_groundItems.end(); ++it) {
		GroundItem *item = *it;
		item->NSC_OnFirstReplayFrame(frame);
	}
}

// grounditemman.cpp:249
void GroundItemMan::OnHoleOut(void) {
	std::list<GroundItem*>::iterator it = m_groundItems.begin();

	for (; it != m_groundItems.end(); ++it) {
		GroundItem *item = *it;
		item->Terminate();
		delete item;
	}

	m_groundItems.clear();

	CSceneManager::Instance()->DeleteElement(m_sceneElemName.c_str());
}

// grounditemman.cpp:268
void GroundItemMan::Display(void) {
}

// grounditemman.cpp:273
void GroundItemMan::DebugDisplay(void) {
	std::list<GroundItem*>::iterator it = m_groundItems.begin();

	for (; it != m_groundItems.end(); ++it) {
		GroundItem *item = *it;
		item->DrawBoundingVolume();
	}

	const ThSphere &ball = GroundItem::GetBallBoundingVolume();
	g_view->DrawSphere((const WVector &)ball.center, ball.radius, 0xffff0000, 10, 0);
}

// grounditemman.cpp:397
GimmickDispositionInformation *GroundItemMan::FindUnusedDisposeInfo(FieldItem::eDisposeTextureType textureType, std::vector<GimmickDispositionInformation> &infos) const {
	GimmickDispositionInformation *info = 0;
	switch (textureType) {
	case 1:
		info = m_gimmickContainer->FindDisposeInfo(GOLFDOC()->m_currentHole, (GIMMICK_ITEM_TYPE)0, GimmickDispositionInformation::FLAG_NONE);
		break;
	case 2:
		info = m_gimmickContainer->FindDisposeInfo(GOLFDOC()->m_currentHole, GIMMICK_ITEM_ANY, GimmickDispositionInformation::FLAG_NONE);
		break;
	}
	return info;
}

// grounditemman.cpp:424
GroundItem *GroundItemMan::CreateGroundItem(unsigned long id, GIMMICK_ITEM_TYPE itemType, FieldItem::eDisposeTextureType textureType, const Ut::Spatial &spatial) {
	GroundItem *item = 0;
	std::string puppetName;
	Ut::Spatial itemSpatial = spatial;

	switch (itemType) {
	case 0:
		puppetName = "coin.pet";

		itemSpatial.m_position.y += 0.1f;
		itemSpatial.m_scale = 10.0f;
		item = new GroundItem(this, id, itemType, textureType, puppetName.c_str(), itemSpatial);
		break;
	case 1:
		puppetName = "ct_event.pet";

		itemSpatial.m_position.y -= 3.0f;

		itemSpatial.m_scale = 14.0f;
		item = new GroundItem(this, id, itemType, textureType, puppetName.c_str(), itemSpatial);
		break;
	}

	return item;
}

// grounditemman.cpp:470
void GroundItemMan::EnableGroundItemAll(bool enable) {
	std::list<GroundItem*>::iterator it = m_groundItems.begin();

	for (; it != m_groundItems.end(); ++it) {
		(*it)->Enable(enable);
	}
}
