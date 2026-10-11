// Original: d:\build\custom\temp\code\client\projectg\golfplayer.h
#ifndef PG_CLIENT_PROJECTG_GOLFPLAYER_H
#define PG_CLIENT_PROJECTG_GOLFPLAYER_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"
#include "wmath.h"
#include "wscene.h"
#include "ExtendedEffect.h"

#include <list>
#include <utility>
#include <vector>

class CFxSequence;
class CFxSpray;
class CPetFrame;
class CPartTidList;
class CTailEffect;
class MsgObject;
class WBone;
class WPuppet;
struct sTexturePart;
struct w_motion_data;

class IObject;

IObject* CGolfPlayerMakeInstance(void);

// The player actor of a golf round. Its members are transcribed only as far
// as the functions of this unit decompiled so far need them.
class CGolfPlayer : public IActor {
	LAYOUT_CHECK_FRIEND;
public:
	// golfplayer.h:17
	virtual ~CGolfPlayer(void) {}
	// golfplayer.h:19
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	static const WRTTI m_RTTI;

	struct Object {
		Object(void);
	};

	// One club type's grip setup of one player, read by RegisterClub from
	// the club's motion data.
	struct sClub {
		sClub(void);

		WVector m_ballPos;      // 0x0, x is the motion's "ball_dist"
		float m_gripPos;        // 0xC, the motion's "grip_pos"
	};

	// A spray effect a bounding-box script opened, closed by its id.
	struct sCondBBSpray {
		unsigned long m_fxId;   // 0x0
		CFxSpray *m_pSpray;     // 0x4
	};

	// A sequence effect a bounding-box script opened, closed by its id.
	struct sCondBBSequence {
		unsigned long m_fxId;           // 0x0
		CFxSequence *m_pSequence;       // 0x4
	};

	// The meaning of most members is not known yet; the constructor gives
	// their types and initial values.
	struct sPetInfo {
		sPetInfo(void);     // golfplayer.h:139
		~sPetInfo(void);    // golfplayer.h:146

		float m_motionTime;     // 0x00, the current time within the motion (SetMotion)
		w_motion_data *m_pMotion;   // 0x04, the motion SetMotion found
		float m_unk08[3][4];    // 0x08, each row starts as (1, 0, 0, 0)
		bool m_unk38;           // 0x38
		bool m_unk39;           // 0x39
		UNKNOWN(0x3A, 0x5C);
		unsigned int m_unk5C;   // 0x5C
		int m_unk60;            // 0x60
		float m_shotTimes[16];  // 0x64, per motion of data_00a804a8[6..21]: seconds from the motion's start to its "*shot" frame (CheckShotScript)
		sTexturePart *m_pTexPart;   // 0xA4, the pet's pending texture part (SetPendingTexPart)
		int m_unkA8;            // 0xA8
		float m_scale;          // 0xAC, starts at 1
		float m_unkB0;          // 0xB0, m_scale's rate per second (OnProcess)
	};

	virtual void OnPreLoadInit(void);   // golfplayer.cpp:227
	virtual void OnLoad(void);
	virtual void OnInit(void);          // golfplayer.cpp:467
	virtual void OnReload(void);        // golfplayer.cpp:340
	virtual void OnProcess(float dt);
	virtual void OnDisplay(void);
	virtual void OnDestroy(void);       // golfplayer.cpp:113
	virtual void HandleMsg(const MsgObject &msg);

protected:
	friend IObject* CGolfPlayerMakeInstance(void);

	CGolfPlayer(void);
	WMatrix GetHeadMatrix(void);        // golfplayer.cpp:1016
	WMatrix GetFootMatrix(int foot);    // golfplayer.cpp:1040
	void SearchGround(void);
	void SetLight(void);                // golfplayer.cpp:3939
	void SetLight(int index);           // golfplayer.cpp:3945
	void RegisterClub(int);
	void CheckShotScript(void);
	void ParseScript_ShowVis(int, bool, const char *);
	void PlayShotSfx(int);
	bool Decide_PowerShot(void);
	void RefreshAllUccClothes(void);
	void SetPendingTexPart(int, char *);
	void ParseBoundingBoxScript(int, bool, bool, bool);
	void CloseBoundingBoxFx(int, unsigned long);
	void ParsePuppetScript(char *, int, bool);
	void ScriptProcess(w_motion_data *, float, float, int, bool, bool);
	void ScoreEvent(void);
	void SetMotion(const char *, float, int);
	void SetSwingType(void);            // golfplayer.cpp:2808
	void Shot_PowerShot(void);
	void Cancel_PowerShot(void);        // golfplayer.cpp:4119
	void PowerShotInput(void);
	bool ShowAbilityParts(CPartTidList *);
	void InputProcess(float dt);    // golfplayer.cpp:484
	bool SearchGround(const WVector &pos, float angle);
	unsigned char HasStandGround(const WVector &pos);    // golfplayer.cpp:3925
	bool IsAnimFxRunning(int slot, unsigned long fxId);   // golfplayer.cpp:4256
	void ResetComboState(void);     // golfplayer.cpp:3958
	void StartComboState(void);     // golfplayer.cpp:3965
	void EndComboState(void);       // golfplayer.cpp:3972

private:
	int GetAbilityPartsSize(void);                                  // golfplayer.h:233
	void AddAbilityParts(unsigned long tid, unsigned long value);   // golfplayer.cpp:4308

	std::vector<sCondBBSpray> m_condBBSprays[4];         // 0x18, per slot
	std::vector<sCondBBSequence> m_condBBSequences[4];   // 0x58, per slot
	std::vector<unsigned long> m_animFx[4];   // 0x98, running animation effects per slot
	std::vector<unsigned long> m_unkD8;   // 0xD8, element type not known yet (only constructed and destroyed so far)
	std::vector<unsigned long> m_unkE8;   // 0xE8, element type not known yet (only constructed and destroyed so far)
	int m_unkF8;                    // 0xF8, starts at 0
	WPuppet *m_pClubPuppet;         // 0xFC, handed to CSceneManager::SetPuppet (HandleMsg)
	PADDING(0x100, 0x104);          // members not transcribed yet
	WMatrix m_displayMatrix;        // 0x104, reset by OnInit, composed by OnDisplay
	PADDING(0x134, 0x140);          // members not transcribed yet
	float m_unk140;                 // 0x140, starts at pi
	CPetFrame *m_pPetFrames;        // 0x144, array
	sPetInfo *m_pPetInfos;          // 0x148, array
	float m_ballDist;               // 0x14C, the current club's ball distance (SetSwingType)
	// A tail end: a bone and the point in its space (OnProcess feeds CTailEffect::PutSpot).
	struct sTailSpot {
		WBone *m_pBone;         // 0x0
		struct sPoint {
			UNKNOWN(0x0, 0x18);
			WVector m_pos;      // 0x18
		} *m_pPoint;            // 0x4
	};
	sTailSpot *m_pTailSpots[2];     // 0x150
	CTailEffect *m_pTailEffect;     // 0x158
	bool m_unk15C;                  // 0x15C
	bool m_unk15D;                  // 0x15D, starts true
	PADDING(0x15E, 0x15F);
	char m_swingTypeName[0x10];     // 0x15F, set by SetSwingType
	bool m_unk16F;                  // 0x16F
	PADDING(0x170, 0x18F);          // members not transcribed yet
	bool m_unk18F;                  // 0x18F, a swing motion is pending (OnProcess)
	bool m_unk190;                  // 0x190
	PADDING(0x191, 0x194);
	sClub m_clubs[4][4];            // 0x194, per player and club type

	float m_footAngle;              // 0x294, turns the foot matrices (GetFootMatrix)
	WBone *m_pFootBones[2];         // 0x298
	bool m_unk2A0;                  // 0x2A0, toggled by ALT on a power putter (OnProcess)
	PADDING(0x2A1, 0x2A4);
	LightSet m_lightSet;            // 0x2A4, the player's light (SetLight, OnInit)
	PADDING(0x2C0, 0x2F0);          // members not transcribed yet
	bool m_unk2F0;                  // 0x2F0, starts false
	PADDING(0x2F1, 0x2F4);
	float m_unk2F4;                 // 0x2F4, compared with 9.6 by OnProcess
	std::list<sTexturePart *> m_pendingTexParts;   // 0x2F8
	bool m_unk304;                  // 0x304, sends message 0x10E once (OnProcess)
	PADDING(0x305, 0x30C);
	float m_headAngle;              // 0x30C, turned by InputProcess within +-pi/3
	WBone *m_pHeadBone;             // 0x310
	bool m_comboRunning;            // 0x314
	PADDING(0x315, 0x318);
	float m_comboTime;              // 0x318
	int m_comboCount;               // 0x31C
	int m_unk320;                   // 0x320, starts at 0
	unsigned long m_powerShotKeyTime;   // 0x324, timeGetTime when the ALT key went up (PowerShotInput)
	bool m_powerShotOn;             // 0x328, a power shot is set (Shot_PowerShot), cleared by Cancel_PowerShot
	bool m_unk329;                  // 0x329, starts false
	PADDING(0x32A, 0x32C);
	int m_powerShotKeyState;        // 0x32C, PowerShotInput's ALT key state: 0 idle, 1 first press, 2 second press
	bool m_unk330;                  // 0x330, starts false
	PADDING(0x331, 0x334);
	std::vector<std::pair<unsigned long, unsigned long> > m_abilityParts;   // 0x334
	CExtendedEffect m_extendedEffect;   // 0x344
};

CHECK_SIZE(CGolfPlayer::sCondBBSpray, 0x8);
CHECK_SIZE(CGolfPlayer::sCondBBSequence, 0x8);
CHECK_SIZE(CGolfPlayer::sClub, 0x10);
CHECK_FIELD(CGolfPlayer::sClub, m_gripPos, 0xC);
CHECK_SIZE(CGolfPlayer::sPetInfo, 0xB4);
CHECK_FIELD(CGolfPlayer::sPetInfo, m_unk08, 0x08);
CHECK_FIELD(CGolfPlayer::sPetInfo, m_unk38, 0x38);
CHECK_FIELD(CGolfPlayer::sPetInfo, m_unk5C, 0x5C);
CHECK_FIELD(CGolfPlayer::sPetInfo, m_shotTimes, 0x64);
CHECK_FIELD(CGolfPlayer::sPetInfo, m_scale, 0xAC);
CHECK_FIELD(CGolfPlayer::sPetInfo, m_unkB0, 0xB0);

BEGIN_LAYOUT_CHECK(CGolfPlayer)
	CHECK_FIELD(CGolfPlayer, m_condBBSprays, 0x18);
	CHECK_FIELD(CGolfPlayer, m_condBBSequences, 0x58);
	CHECK_FIELD(CGolfPlayer, m_animFx, 0x98);
	CHECK_FIELD(CGolfPlayer, m_unkD8, 0xD8);
	CHECK_FIELD(CGolfPlayer, m_unkE8, 0xE8);
	CHECK_FIELD(CGolfPlayer, m_unkF8, 0xF8);
	CHECK_FIELD(CGolfPlayer, m_pClubPuppet, 0xFC);
	CHECK_FIELD(CGolfPlayer, m_displayMatrix, 0x104);
	CHECK_FIELD(CGolfPlayer, m_unk15D, 0x15D);
	CHECK_FIELD(CGolfPlayer, m_unk2F0, 0x2F0);
	CHECK_FIELD(CGolfPlayer, m_unk320, 0x320);
	CHECK_FIELD(CGolfPlayer, m_unk329, 0x329);
	CHECK_FIELD(CGolfPlayer, m_unk330, 0x330);
	CHECK_FIELD(CGolfPlayer, m_unk140, 0x140);
	CHECK_FIELD(CGolfPlayer, m_pPetFrames, 0x144);
	CHECK_FIELD(CGolfPlayer, m_unk15C, 0x15C);
	CHECK_FIELD(CGolfPlayer, m_swingTypeName, 0x15F);
	CHECK_FIELD(CGolfPlayer, m_unk16F, 0x16F);
	CHECK_FIELD(CGolfPlayer, m_pTailSpots, 0x150);
	CHECK_FIELD(CGolfPlayer, m_unk18F, 0x18F);
	CHECK_FIELD(CGolfPlayer, m_unk2A0, 0x2A0);
	CHECK_FIELD(CGolfPlayer, m_unk2F4, 0x2F4);
	CHECK_FIELD(CGolfPlayer, m_unk304, 0x304);
	CHECK_FIELD(CGolfPlayer, m_ballDist, 0x14C);
	CHECK_FIELD(CGolfPlayer, m_clubs, 0x194);
	CHECK_FIELD(CGolfPlayer, m_footAngle, 0x294);
	CHECK_FIELD(CGolfPlayer, m_pFootBones, 0x298);
	CHECK_FIELD(CGolfPlayer, m_lightSet, 0x2A4);
	CHECK_FIELD(CGolfPlayer, m_pPetInfos, 0x148);
	CHECK_FIELD(CGolfPlayer, m_pTailEffect, 0x158);
	CHECK_FIELD(CGolfPlayer, m_pendingTexParts, 0x2F8);
	CHECK_FIELD(CGolfPlayer, m_headAngle, 0x30C);
	CHECK_FIELD(CGolfPlayer, m_pHeadBone, 0x310);
	CHECK_FIELD(CGolfPlayer, m_comboRunning, 0x314);
	CHECK_FIELD(CGolfPlayer, m_comboTime, 0x318);
	CHECK_FIELD(CGolfPlayer, m_comboCount, 0x31C);
	CHECK_FIELD(CGolfPlayer, m_powerShotKeyTime, 0x324);
	CHECK_FIELD(CGolfPlayer, m_powerShotOn, 0x328);
	CHECK_FIELD(CGolfPlayer, m_powerShotKeyState, 0x32C);
	CHECK_FIELD(CGolfPlayer, m_abilityParts, 0x334);
	CHECK_FIELD(CGolfPlayer, m_extendedEffect, 0x344);
END_LAYOUT_CHECK
CHECK_SIZE(CGolfPlayer, 0x360);

#endif
