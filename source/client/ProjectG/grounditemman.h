// Original: d:\build\custom\temp\code\client\projectg\grounditemman.h
#ifndef PG_CLIENT_PROJECTG_GROUNDITEMMAN_H
#define PG_CLIENT_PROJECTG_GROUNDITEMMAN_H

#include <string.h>
#include <list>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "netsyncactor.h"
#include "scenemanager.h"
#include "contentsdoc.h"
#include "polysoup.h"

class GroundItem;
class MsgObject;
class CPangInfo;

namespace Ut {
struct Spatial;
}

// The actor that places the course items (GroundItem) of the current hole,
// steps them with the network-synchronised time and draws them. Size 0xa34
// (GroundItemManMakeInstance's new).
class GroundItemMan : public NetSyncActor, public CRenderFuncPtr {
	LAYOUT_CHECK_FRIEND;
	friend class GroundItem;
	friend class CPangInfo;
public:
	// Per-player counters (0x18 bytes, one per player of the game, OnLoad).
	// NSC_OnFirstNewShotFrame clears the first three of the local player.
	struct PlayerStats {
		// grounditemman.h:39
		PlayerStats(void) { memset(this, 0, sizeof(PlayerStats)); }
		// grounditemman.h:41
		void Reset(void) {
			m_shotCount[0] = 0;
			m_shotCount[1] = 0;
			m_shotCount[2] = 0;
			m_totalCount[0] = 0;
			m_totalCount[1] = 0;
			m_totalCount[2] = 0;
		}

		int m_shotCount[3];     // 0x0, reset at each new shot
		int m_totalCount[3];    // 0xc
	};

	static const WRTTI m_RTTI;
	// grounditemman.h:24
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	GroundItemMan(void);
	virtual ~GroundItemMan(void);

	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnReload(void);
	virtual void OnDestroy(void);
	virtual void HandleMsg(const MsgObject &msg);
	virtual void OnHoleOut(void);

	virtual void NSC_OnStep(bool active, bool replay, float elapsed, float syncTime, int frame);
	virtual void NSC_OnFirstNewShotFrame(float startTime, int frame);
	virtual void NSC_OnFirstReplayFrame(int frame);

	virtual void Display(void);
	virtual void DebugDisplay(void);

protected:
	virtual int DisposeGroundItem(FieldItem::eDisposeTextureType textureType, const std::vector<WPolySoup::sTriangle*> *triangles, std::vector<GimmickDispositionInformation> &infos);
	virtual GimmickDispositionInformation *FindUnusedDisposeInfo(FieldItem::eDisposeTextureType textureType, std::vector<GimmickDispositionInformation> &infos) const;
	virtual GroundItem *CreateGroundItem(unsigned long id, GIMMICK_ITEM_TYPE itemType, FieldItem::eDisposeTextureType textureType, const Ut::Spatial &spatial);

	void EnableGroundItemAll(bool enable);

private:
	static unsigned int ms_initCnt;

protected:
	std::list<GroundItem*> m_groundItems;           // 0xa04
	CGimmickContainer *m_gimmickContainer;          // 0xa10
	std::string m_sceneElemName;                    // 0xa14
	PlayerStats *m_playerStats;                     // 0xa30, one per player
};

CHECK_SIZE(GroundItemMan::PlayerStats, 0x18);
CHECK_SIZE(GroundItemMan, 0xa34);
BEGIN_LAYOUT_CHECK(GroundItemMan)
	CHECK_FIELD(GroundItemMan, m_groundItems, 0xa04);
	CHECK_FIELD(GroundItemMan, m_gimmickContainer, 0xa10);
	CHECK_FIELD(GroundItemMan, m_sceneElemName, 0xa14);
	CHECK_FIELD(GroundItemMan, m_playerStats, 0xa30);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GROUNDITEMMAN_H
