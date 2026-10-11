// Original: d:\build\custom\temp\code\client\projectg\ball.h
#ifndef PG_CLIENT_PROJECTG_BALL_H
#define PG_CLIENT_PROJECTG_BALL_H

#include <list>
#include <string.h>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "scenemanager.h"
#include "FxBox.h"
#include "../client/Wangreal/include/wmath.h"
#include "../client/Wangreal/include/wscene.h"
#include "../client/Wangreal/include/wvideo.h"
#include "../shared/classdefine.h"

class CFxSequence;
class CFxSpray;
class WPuppet;

// The ball actor: draws the balls of the players (a list of sBall, one per
// ball type id) and keeps the effects attached to them. Its size (0x158) is
// the `new` of CBallMakeInstance.
class CBall : public IActor, public CRenderFuncPtr, public CFxKeeper {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	// One loaded ball (0x28 bytes, `new` of AddBall).
	struct sBall {
		// ball.h:46
		sBall(void) {
			m_pData = 0;
			m_pPuppet = 0;
			memset(m_pSpray, 0, sizeof(m_pSpray));
			memset(m_pSequence, 0, sizeof(m_pSequence));
		}

		IFF_STRUCT::sBall* m_pData;   // 0x0, the item record (CItemManager::FindBall)
		WPuppet* m_pPuppet;           // 0x4, "<name>.pet" of g_resrcmng
		CFxSpray* m_pSpray[4];        // 0x8, attached sprays (CFx::CloseSpray)
		CFxSequence* m_pSequence[4];  // 0x18, attached sequences (CFx::CloseSequence)
	};

	// find_if predicate: the ball of a type id.
	struct sFindBall {
		// ball.h:57
		sFindBall(unsigned long typeId) {
			m_typeId = typeId;
		}
		// ball.h:60
		bool operator()(sBall* pBall) {
			return pBall->m_pData->m_typeId == m_typeId;
		}

		unsigned long m_typeId;       // 0x0
	};

protected:
	CBall(void);
	friend IObject* CBallMakeInstance(void);   // the factory creates it

public:
	// ball.h:13
	virtual ~CBall(void) {}
	virtual void OnPreLoadInit(void);
	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnReload(void);
	virtual void OnProcess(float elapsed);
	virtual void OnDisplay(void);
	virtual void OnDestroy(void);
	virtual void Display(void);
	virtual void EmptyFx(void* pFx);

protected:
	virtual void HandleMsg(const MsgObject& msg);
	float GetDownDelta(void);
	void TransformBall(void);
	sBall* FindBall(unsigned long typeId);
	sBall* AddBall(unsigned long typeId);
	void AttachFx(int index);
	void DetachFx(int index);

	std::list<sBall*> m_balls;    // 0x24
	WPuppet* m_pPuppet;           // 0x30
	int m_hShadowTexture;         // 0x34, "ball_shadow.jpg" texture handle of g_resrcmng
	WMatrix m_matrix;             // 0x38
	WVector m_axis;               // 0x68, WVector::UNIT_POS_X at OnPreLoadInit
	bool m_bRefreshLight;         // 0x74, UpdateLightSource on the next display
	UNKNOWN(0x75, 0x78);
	float m_sinkFactor;           // 0x78, 1 - CQuadTree::GetMinPowerOnGround of the landing point
	int m_sinkStartIndex;         // 0x7c, PreData index from which the ball sinks, -1 for none
	LightSet m_light;             // 0x80
	bool m_bVisible;              // 0x9c
	UNKNOWN(0x9d, 0xa0);
	int m_lastPreDataIndex;       // 0xa0, CGolfBall::m_preDataIndex last processed, -1 for none
	WTVertex* m_shadowPolygon[5]; // 0xa4, the shadow quad, NULL-terminated
	WTVertex m_shadowVertices[4]; // 0xb8
};

CHECK_SIZE(CBall, 0x158);
BEGIN_LAYOUT_CHECK(CBall)
	CHECK_FIELD(CBall, m_balls, 0x24);
	CHECK_FIELD(CBall, m_pPuppet, 0x30);
	CHECK_FIELD(CBall, m_hShadowTexture, 0x34);
	CHECK_FIELD(CBall, m_matrix, 0x38);
	CHECK_FIELD(CBall, m_axis, 0x68);
	CHECK_FIELD(CBall, m_bRefreshLight, 0x74);
	CHECK_FIELD(CBall, m_sinkFactor, 0x78);
	CHECK_FIELD(CBall, m_sinkStartIndex, 0x7c);
	CHECK_FIELD(CBall, m_light, 0x80);
	CHECK_FIELD(CBall, m_bVisible, 0x9c);
	CHECK_FIELD(CBall, m_lastPreDataIndex, 0xa0);
	CHECK_FIELD(CBall, m_shadowPolygon, 0xa4);
	CHECK_FIELD(CBall, m_shadowVertices, 0xb8);
END_LAYOUT_CHECK
CHECK_SIZE(CBall::sBall, 0x28);
CHECK_SIZE(CBall::sFindBall, 0x4);

#endif // PG_CLIENT_PROJECTG_BALL_H
