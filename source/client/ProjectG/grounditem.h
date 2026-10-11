// Original: d:\build\custom\temp\code\client\projectg\grounditem.h
#ifndef PG_CLIENT_PROJECTG_GROUNDITEM_H
#define PG_CLIENT_PROJECTG_GROUNDITEM_H

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "ObjectFactory.h"
#include "netsyncactor.h"
#include "thirties/thgeometry.h"
#include "contentsdoc.h"
#include "wmath.h"
#include "util.h"

class GroundItemMan;
class CGimmickContainer;

class WPuppet;

// An item lying on the course, stepped by its GroundItemMan. Vtable
// (??_7GroundItem@@6BIObject@@@): GetRTTI, the destructor, Initialize,
// Terminate, DrawBoundingVolume, then the GetBoundingSphere overloads
// (the WVector one first), OnChangeState, TestIntersection and
// TestIntersection2. Size 0xbc (GroundItemMakeInstance's new).
class GroundItem : public IObject, public NetSyncable {
	LAYOUT_CHECK_FRIEND;
public:
	enum eGroundItemState {
		GIS_IDLE = 0,
		GIS_ACTIVE = 1,
		GIS_REMOVED = 2,
		GIS_NONE = 3        // before the first Initialize
	};

	// What Update does this step, chosen by NSC_OnStep from the item type
	// and the texture the item is placed on.
	enum eGoundItemUpdateType {
		GIUT_ITEM_TYPE_0 = 0,
		GIUT_ITEM_TYPE_1 = 1,
		GIUT_DISPOSE_TEXTURE_1 = 2,
		GIUT_NONE = 3
	};

	// The ball's sphere for a frame, rebuilt only when the frame changes
	// (m_frame -1: none).
	struct BallInfo {
		// grounditem.h:127
		BallInfo(void) : m_frame(-1) { m_sphere.center = gaV3::MAX; }
		// grounditem.h:129
		void Reset(void) { m_frame = -1; m_sphere.center = gaV3::MAX; }

		const ThSphere &MakeBallBoundingVolume(int frame);

		int m_frame;        // 0x0
		ThSphere m_sphere;  // 0x4
	};

	static const WRTTI m_RTTI;
	// grounditem.h:52
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	GroundItem(void);
	GroundItem(GroundItemMan *owner, unsigned long id, GIMMICK_ITEM_TYPE itemType, FieldItem::eDisposeTextureType disposeTextureType, const char *puppetName, const Ut::Spatial &spatial);
	virtual ~GroundItem(void);
	virtual void Initialize(void);
	virtual void Terminate(void);
	virtual void DrawBoundingVolume(void);
	virtual void GetBoundingSphere(gaV3 &center, float &radius) const;
	virtual void GetBoundingSphere(WVector &center, float &radius) const;

	virtual void NSC_OnStep(bool active, bool replay, float elapsed, float syncTime, int frame);
	virtual void NSC_OnFirstNewShotFrame(float startTime, int frame);
	virtual void NSC_OnFirstReplayFrame(int frame);

	void Enable(bool enable);

	// grounditem.h:188
	bool NeedToUpdate(void) const {
		return m_enabled && m_state != GIS_REMOVED;
	}

	static const ThSphere &GetBallBoundingVolume(void);
	static void EnableGreenItemJackpot(bool enable);

protected:
	virtual void OnChangeState(int frame);
	virtual bool TestIntersection(int frame);
	virtual bool TestIntersection2(int frame, float time);

	void Transform(void);
	void Update(eGoundItemUpdateType type, bool active, bool replay, float elapsed, float syncTime, int frame);
	eGroundItemState ChangeState(eGroundItemState state, int frame);

	static BallInfo ms_ballInfo;
	static unsigned int ms_initCnt;
	static bool ms_greenItemJackpot;

	GroundItemMan *m_owner;             // 0x8
	unsigned long m_id;                 // 0xc
	GIMMICK_ITEM_TYPE m_itemType;       // 0x10
	eGroundItemState m_state;           // 0x14
	bool m_enabled;                     // 0x18
	PADDING(0x19, 0x1c);
	int m_idleFrame;                    // 0x1c, -1 until set
	int m_startFrame;                   // 0x20, frame of the change to GIS_ACTIVE
	int m_endFrame;                     // 0x24, frame of the change to GIS_REMOVED
	FieldItem::eDisposeTextureType m_disposeTextureType;   // 0x28
	std::string m_puppetName;           // 0x2c, the resource GetPuppet loads
	WPuppet *m_puppet;                  // 0x48
	std::string m_sceneElemName;        // 0x4c, the CSceneManager element name
	Ut::Spatial m_spatial;              // 0x68, the placement m_worldMat is built from
	WMatrix m_worldMat;                 // 0x88, the bones' root for Transform
	CGimmickContainer *m_gimmickContainer;  // 0xb8, content 0xa3 (crash sequences)
};

CHECK_SIZE(GroundItem::BallInfo, 0x14);
CHECK_FIELD(GroundItem::BallInfo, m_sphere, 0x4);
CHECK_SIZE(GroundItem, 0xbc);
BEGIN_LAYOUT_CHECK(GroundItem)
	CHECK_FIELD(GroundItem, m_owner, 0x8);
	CHECK_FIELD(GroundItem, m_id, 0xc);
	CHECK_FIELD(GroundItem, m_itemType, 0x10);
	CHECK_FIELD(GroundItem, m_state, 0x14);
	CHECK_FIELD(GroundItem, m_enabled, 0x18);
	CHECK_FIELD(GroundItem, m_idleFrame, 0x1c);
	CHECK_FIELD(GroundItem, m_startFrame, 0x20);
	CHECK_FIELD(GroundItem, m_endFrame, 0x24);
	CHECK_FIELD(GroundItem, m_disposeTextureType, 0x28);
	CHECK_FIELD(GroundItem, m_puppetName, 0x2c);
	CHECK_FIELD(GroundItem, m_puppet, 0x48);
	CHECK_FIELD(GroundItem, m_sceneElemName, 0x4c);
	CHECK_FIELD(GroundItem, m_spatial, 0x68);
	CHECK_FIELD(GroundItem, m_worldMat, 0x88);
	CHECK_FIELD(GroundItem, m_gimmickContainer, 0xb8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GROUNDITEM_H
