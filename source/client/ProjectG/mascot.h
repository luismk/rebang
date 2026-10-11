// Original: d:\build\custom\temp\code\client\projectg\mascot.h
#ifndef PG_CLIENT_PROJECTG_MASCOT_H
#define PG_CLIENT_PROJECTG_MASCOT_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"
#include "scenemanager.h"
#include "npc.h"

class IObject;
class WPuppet;

namespace IFF_STRUCT {
struct sMascot;
}

struct sMascotInfo;

IObject *CMascotMakeInstance(void);

// The bird carrying a player's mascot (`new` of 0x154 in CMascot::OnLoad).
// Only the members the functions decompiled so far use are declared.
class CCameraBird : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	CCameraBird(const char *name, unsigned char playerIndex);   // mascot.cpp:263

	virtual void Display(void);
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

	void ChangePet(const char *name);

	// Accessors for CMascot::HandleMsg (the members are CCreature's).
	void SetActive(bool active) { m_active = active; }
	WPuppet *GetPet(void) const { return m_pPet; }
	void SetAddTex(int handle) { m_addTex = handle; }

protected:
	unsigned char m_playerIndex;    // 0x120, the player the bird follows
	PADDING(0x121, 0x124);
	WMatrix m_playerMatrix;         // 0x124, filled by the "Player" actor (message 0x124, CheckTarget)
};

BEGIN_LAYOUT_CHECK(CCameraBird)
	CHECK_FIELD(CCameraBird, m_playerIndex, 0x120);
	CHECK_FIELD(CCameraBird, m_playerMatrix, 0x124);
	CHECK_SIZE(CCameraBird, 0x154);
END_LAYOUT_CHECK

// The mascots following the players of a round (IActor at 0x0,
// CRenderFuncPtr at 0x18; `new` of 0x80 in CMascotMakeInstance). Only the
// members the functions decompiled so far use are declared.
class CMascot : public IActor, public CRenderFuncPtr {
	LAYOUT_CHECK_FRIEND;
public:
	// mascot.h:13
	virtual ~CMascot(void) {}

	static const WRTTI m_RTTI;
	// mascot.h:15
	virtual const WRTTI *GetRTTI(void) const { return &m_RTTI; }

	virtual void OnPreLoadInit(void);
	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnReload(void);
	virtual void OnProcess(float dt);
	virtual void OnDestroy(void);
	// CRenderFuncPtr's Display slot (not declared in scenemanager.h yet).
	virtual void Display(void);

	static int ChangeMascotMsgInTex(const char *msg, const IFF_STRUCT::sMascot *pMascot, WPuppet *pPet, const char *suffix);

	virtual void HandleMsg(const MsgObject &msg);

protected:
	// One player's mascot, 0x18 bytes.
	struct sMascot {
		// mascot.h:36
		sMascot(void) : m_pBird(0), m_pIffMascot(0), m_pPlayer(0) {}

		CCreature *m_pBird;                 // 0x00, a CCameraBird
		UNKNOWN(0x4, 0x10);
		const IFF_STRUCT::sMascot *m_pIffMascot; // 0x10
		const sMascotInfo *m_pPlayer;       // 0x14, the equipped mascot in the shared document (CSharedDoc room player 0)
	};

	// mascot.h:43
	CMascot(void) {}

	friend IObject *CMascotMakeInstance(void);

	sMascot m_mascots[4];       // 0x1C
	int m_unk7C;                // 0x7C
};

BEGIN_LAYOUT_CHECK(CMascot)
	CHECK_SIZE(CMascot::sMascot, 0x18);
	CHECK_FIELD(CMascot::sMascot, m_pIffMascot, 0x10);
	CHECK_FIELD(CMascot::sMascot, m_pPlayer, 0x14);
	CHECK_FIELD(CMascot, m_mascots, 0x1C);
	CHECK_FIELD(CMascot, m_unk7C, 0x7C);
	CHECK_SIZE(CMascot, 0x80);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MASCOT_H
