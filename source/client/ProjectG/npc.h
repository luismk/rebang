// Original: d:\build\custom\temp\code\client\projectg\npc.h
#ifndef PG_CLIENT_PROJECTG_NPC_H
#define PG_CLIENT_PROJECTG_NPC_H

#include <list>
#include <map>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"
#include "scenemanager.h"
#include "wmath.h"

class WPuppet;
struct w_motion_data;
class Waabb;
class CCreature;
class MsgObject;
class IObject;

IObject *CNPCMakeInstance(void);

// The actor owning the scene's creatures (IActor at 0x0). Only the members
// the functions decompiled so far use are declared; the size is not known yet.
class CNPC : public IActor {
	LAYOUT_CHECK_FRIEND;
public:
	// npc.h:14
	virtual ~CNPC(void) {}

	static const WRTTI m_RTTI;
	// npc.h:16
	virtual const WRTTI *GetRTTI(void) const { return &m_RTTI; }

	virtual void OnInit(void);
	virtual void OnProcess(float dt);
	virtual void OnDestroy(void);

protected:
	// npc.h:70
	CNPC(void) {}

	friend IObject *CNPCMakeInstance(void);

	virtual void HandleMsg(const MsgObject &msg);

	void Destroy(void);
	void SetNPC(const std::vector<WVector> &points, const char *name);
	void SetNPC(const Waabb &area, char *name);
	void AddCreature(CCreature *creature, const Waabb &area, unsigned long flags, int type);

	std::list<CCreature *> m_creatures; // 0x18
};

BEGIN_LAYOUT_CHECK(CNPC)
	CHECK_FIELD(CNPC, m_creatures, 0x18);
	CHECK_SIZE(CNPC, 0x24);
END_LAYOUT_CHECK

// A scripted puppet moving in the scene (birds, mascots). Vtable (from
// ??_7CCreature@@6B@): CRenderFuncPtr's DisplayShadow, Display and
// DebugDisplay, then the destructor, Process, SetArea (vector), SetArea
// (Waabb), PlayEvent and CheckTarget. scenemanager.h does not declare
// CRenderFuncPtr's Display slot yet, so Display is declared first here to keep
// the destructor and Process in slots 3 and 4. Only the members the units
// built so far use are declared; 0x120 bytes.
class CCreature : public CRenderFuncPtr {
	LAYOUT_CHECK_FRIEND;
	friend class CNPC;
public:
	virtual void Display(void);
	virtual ~CCreature(void);
	virtual void Process(float dt);
	virtual void SetArea(const std::vector<WVector> &points, float radius, unsigned long flags);
	virtual void SetArea(const Waabb &area, unsigned long flags);
	virtual void PlayEvent(void);
	virtual void CheckTarget(float dt);

	// npc.h:97
	void SetActive(bool active) { m_active = active; }
	// npc.h:108
	WPuppet *GetPet(void) { return m_pPet; }
	// npc.h:109
	void SetAddTex(int addTex) { m_addTex = addTex; }

	void DrawDebugLine(void);
	void SetMotion(const char *name, float time);
	void ScriptProcess(const w_motion_data *motion, float prevTime, float time);
	void ParsePuppetScript(const char *script);
	void Record(void);
	void Replay(void);

protected:
	// An alpha-fade entry read from the puppet's "*opa" frame data.
	struct sAlpha {
		float m_value;          // 0x0, the opacity (0..255)
		float m_time;           // 0x4, the frame time in seconds
	};

	CCreature(void);            // npc.h:129

	void Init(const char *name, unsigned long flags);

	unsigned long m_flags;      // 0x4
	Waabb m_area;               // 0x8, set by SetArea; FindTarget picks a point inside
	WPuppet *m_pPet;            // 0x20
	int m_addTex;               // 0x24
	WMatrix m_matrix;           // 0x28, the puppet's transform
	float m_motionTime;         // 0x58, set by SetMotion, advanced by Process
	int m_npcType;              // 0x5C, the "*type" of the area that made the creature (CNPC::SetNPC)
	const w_motion_data *m_pMotion; // 0x60, the motion set by SetMotion
	WVector m_pos;              // 0x64
	float m_turnRate;           // 0x70, scales the heading change towards the target
	float m_unk74;              // 0x74, 0.5 for the camera birds (the speed of the creatures)
	float m_motionParam;        // 0x78, saved by Record and restored by Replay
	float m_unk7C;              // 0x7C, pi/4 for the gulls and parrots, 0 for the butterflies
	float m_rollRate;           // 0x80, how fast m_motionParam moves towards +/-m_unk7C
	float m_unk84;              // 0x84, 48 for the gulls and parrots, 16 for the butterflies
	int m_gridX;                // 0x88, cells along x (SetArea)
	int m_gridY;                // 0x8C, cells along y
	int m_gridZ;                // 0x90, cells along z
	float m_cellX;              // 0x94, cell size along x
	float m_cellY;              // 0x98, cell size along y
	float m_cellZ;              // 0x9C, cell size along z
	int m_gridCount;            // 0xA0, entries of m_pBuffer
	unsigned char *m_pBuffer;   // 0xA4, freed by the destructor
	unsigned char m_alpha;      // 0xA8, passed to WPuppet::SetAlpha
	bool m_active;              // 0xA9
	UNKNOWN(0xAA, 0xAC);
	float m_unkAC;              // 0xAC, set by the moles, dolphins and crabs
	std::list<sAlpha> m_alphaList; // 0xB0
	std::vector<WVector> m_nodes; // 0xBC
	unsigned char m_nodeIndex;  // 0xCC, the m_nodes entry being flown to
	UNKNOWN(0xCD, 0xD0);
	std::vector<WVector> m_recordedPath; // 0xD0, the positions saved by Record
	unsigned char m_replayIndex;  // 0xE0, the m_recordedPath entry Replay flies to next (Replay sets 1)
	WMatrix m_recordedMatrix;   // 0xE4, m_matrix saved by Record
	float m_recordedMotionTime; // 0x114, m_motionTime saved by Record
	float m_recordedMotionParam; // 0x118, m_motionParam saved by Record
	bool m_recordedActive;      // 0x11C, m_active saved by Record
	bool m_recorded;            // 0x11D, set by Record, cleared by Replay
	UNKNOWN(0x11E, 0x120);
};

BEGIN_LAYOUT_CHECK(CCreature)
	CHECK_FIELD(CCreature, m_flags, 0x4);
	CHECK_FIELD(CCreature, m_area, 0x8);
	CHECK_FIELD(CCreature, m_nodes, 0xBC);
	CHECK_FIELD(CCreature, m_pPet, 0x20);
	CHECK_FIELD(CCreature, m_addTex, 0x24);
	CHECK_FIELD(CCreature, m_unk74, 0x74);
	CHECK_FIELD(CCreature, m_unk7C, 0x7C);
	CHECK_FIELD(CCreature, m_unk84, 0x84);
	CHECK_FIELD(CCreature, m_gridX, 0x88);
	CHECK_FIELD(CCreature, m_gridY, 0x8C);
	CHECK_FIELD(CCreature, m_gridZ, 0x90);
	CHECK_FIELD(CCreature, m_cellX, 0x94);
	CHECK_FIELD(CCreature, m_cellY, 0x98);
	CHECK_FIELD(CCreature, m_cellZ, 0x9C);
	CHECK_FIELD(CCreature, m_gridCount, 0xA0);
	CHECK_FIELD(CCreature, m_pBuffer, 0xA4);
	CHECK_FIELD(CCreature, m_alpha, 0xA8);
	CHECK_FIELD(CCreature, m_active, 0xA9);
	CHECK_FIELD(CCreature, m_unkAC, 0xAC);
	CHECK_FIELD(CCreature, m_alphaList, 0xB0);
	CHECK_FIELD(CCreature, m_matrix, 0x28);
	CHECK_FIELD(CCreature, m_motionTime, 0x58);
	CHECK_FIELD(CCreature, m_npcType, 0x5C);
	CHECK_FIELD(CCreature, m_pMotion, 0x60);
	CHECK_FIELD(CCreature, m_pos, 0x64);
	CHECK_FIELD(CCreature, m_motionParam, 0x78);
	CHECK_FIELD(CCreature, m_turnRate, 0x70);
	CHECK_FIELD(CCreature, m_rollRate, 0x80);
	CHECK_FIELD(CCreature, m_nodeIndex, 0xCC);
	CHECK_FIELD(CCreature, m_recordedPath, 0xD0);
	CHECK_FIELD(CCreature, m_replayIndex, 0xE0);
	CHECK_FIELD(CCreature, m_recordedMatrix, 0xE4);
	CHECK_FIELD(CCreature, m_recordedMotionTime, 0x114);
	CHECK_FIELD(CCreature, m_recordedMotionParam, 0x118);
	CHECK_FIELD(CCreature, m_recordedActive, 0x11C);
	CHECK_FIELD(CCreature, m_recorded, 0x11D);
	CHECK_SIZE(CCreature, 0x120);
END_LAYOUT_CHECK

// The creatures of npc.cpp. Only the members the functions decompiled so far
// use are declared.
class CGull : public CCreature {
public:
	CGull(const char *name);
	virtual ~CGull(void);
};

class CButterfly : public CCreature {
public:
	CButterfly(const char *name);
	virtual ~CButterfly(void);
};

class CSanta : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	CSanta(const char *name);
	virtual ~CSanta(void);
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

protected:
	float m_climbRate;          // 0x120, height change towards the target node
	int m_state;                // 0x124, 2 at start, 1 while turning, 0 when levelled
};

BEGIN_LAYOUT_CHECK(CSanta)
	CHECK_FIELD(CSanta, m_climbRate, 0x120);
	CHECK_FIELD(CSanta, m_state, 0x124);
END_LAYOUT_CHECK

class CFighter : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	CFighter(const char *name);
	virtual ~CFighter(void);
	virtual void CheckTarget(float dt);

protected:
	float m_climbRate;          // 0x120, height change towards the target node
	int m_state;                // 0x124, 2 at start, 1 while turning, 0 when levelled
};

BEGIN_LAYOUT_CHECK(CFighter)
	CHECK_FIELD(CFighter, m_climbRate, 0x120);
	CHECK_FIELD(CFighter, m_state, 0x124);
END_LAYOUT_CHECK

class CUfo : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	CUfo(const char *name);
	virtual ~CUfo(void);
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

protected:
	float m_climbRate;          // 0x120, height change towards the target node
	int m_state;                // 0x124, 2 at start, 1 while turning, 0 when levelled
};

BEGIN_LAYOUT_CHECK(CUfo)
	CHECK_FIELD(CUfo, m_climbRate, 0x120);
	CHECK_FIELD(CUfo, m_state, 0x124);
END_LAYOUT_CHECK

class ntPenguin : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	ntPenguin(const char *name);
	virtual ~ntPenguin(void);
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

protected:
	float m_climbRate;          // 0x120, height change towards the target node
	int m_state;                // 0x124, 2 at start, 1 while turning, 0 when levelled
};

BEGIN_LAYOUT_CHECK(ntPenguin)
	CHECK_FIELD(ntPenguin, m_climbRate, 0x120);
	CHECK_FIELD(ntPenguin, m_state, 0x124);
END_LAYOUT_CHECK

class ntPenguin_Sailor : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	ntPenguin_Sailor(const char *name);
	virtual ~ntPenguin_Sailor(void);
	virtual void Display(void);
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

protected:
	float m_scale;              // 0x120, random in [0.7, 1], scales the matrix in Display
	bool m_act0Motion;          // 0x124, true while NPC_Act0 plays
};

BEGIN_LAYOUT_CHECK(ntPenguin_Sailor)
	CHECK_FIELD(ntPenguin_Sailor, m_scale, 0x120);
	CHECK_FIELD(ntPenguin_Sailor, m_act0Motion, 0x124);
END_LAYOUT_CHECK

class ntNpc_BlackSparrow : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	ntNpc_BlackSparrow(const char *name);
	virtual ~ntNpc_BlackSparrow(void);
	virtual void Display(void);
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

protected:
	bool m_defaultMotion;       // 0x120, true while NPC_Default plays
};

BEGIN_LAYOUT_CHECK(ntNpc_BlackSparrow)
	CHECK_FIELD(ntNpc_BlackSparrow, m_defaultMotion, 0x120);
END_LAYOUT_CHECK

class ntNpc_PolaPenguin1 : public CCreature {
public:
	ntNpc_PolaPenguin1(const char *name, float param);
	virtual ~ntNpc_PolaPenguin1(void);
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

protected:
	void FindTarget(void);
};

class ntNpc_PolaPenguin2 : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	ntNpc_PolaPenguin2(const char *name);
	virtual ~ntNpc_PolaPenguin2(void);
	virtual void Display(void);
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

protected:
	void FindTarget(void);
	bool IsSlidable(void);

	bool m_unk120;              // 0x120, set by the constructor
};

BEGIN_LAYOUT_CHECK(ntNpc_PolaPenguin2)
	CHECK_FIELD(ntNpc_PolaPenguin2, m_unk120, 0x120);
END_LAYOUT_CHECK

// A group of ducks drawn from one puppet with a matrix per duck.
class CDuck : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	CDuck(const char *name, int count);
	// npc.h:210
	virtual ~CDuck(void) {
		if (m_pMatrices) {
			delete[] m_pMatrices;
			m_pMatrices = 0;
		}
	}
	virtual void Display(void);
	virtual void Process(float dt);
	virtual void SetArea(const Waabb &area, unsigned long flags);

protected:
	int m_count;                // 0x120, the number of ducks (0 draws the puppet once)
	WMatrix *m_pMatrices;       // 0x124, one transform per duck
};

BEGIN_LAYOUT_CHECK(CDuck)
	CHECK_FIELD(CDuck, m_count, 0x120);
	CHECK_FIELD(CDuck, m_pMatrices, 0x124);
	CHECK_SIZE(CDuck, 0x128);
END_LAYOUT_CHECK

class CMole : public CCreature {
public:
	CMole(const char *name);
	// npc.h:225
	virtual ~CMole(void) {}
	virtual void PlayEvent(void);
};

class CDolphin : public CCreature {
public:
	CDolphin(const char *name);
	// npc.h:234
	virtual ~CDolphin(void) {}
	virtual void PlayEvent(void);
};

class CCrab : public CCreature {
public:
	CCrab(const char *name);
	// npc.h:243
	virtual ~CCrab(void) {}
	virtual void PlayEvent(void);
};

// A flock of sparrows drawn from one puppet with a matrix per bird.
class CSparrow : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	CSparrow(const char *name, int count);
	// npc.h:252
	virtual ~CSparrow(void) {
		if (m_pMatrices) {
			delete[] m_pMatrices;
			m_pMatrices = 0;
		}
	}
	virtual void Display(void);
	virtual void PlayEvent(void);

protected:
	int m_maxCount;             // 0x120, the size of m_pMatrices
	int m_count;                // 0x124, the birds drawn by Display
	WMatrix *m_pMatrices;       // 0x128, one transform per bird
};

BEGIN_LAYOUT_CHECK(CSparrow)
	CHECK_FIELD(CSparrow, m_maxCount, 0x120);
	CHECK_FIELD(CSparrow, m_count, 0x124);
	CHECK_FIELD(CSparrow, m_pMatrices, 0x128);
END_LAYOUT_CHECK

// The camera creature of the online game, shown only to its player.
class COnGameNet : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	COnGameNet(const char *name, unsigned char playerIndex);
	virtual ~COnGameNet(void);
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

protected:
	unsigned char m_playerIndex;    // 0x120
	UNKNOWN(0x121, 0x124);
	WMatrix m_playerMatrix;         // 0x124, filled by the "Player" actor (message 0x124, CheckTarget)
	int m_hTexture;                 // 0x154, camera.jpg
};

BEGIN_LAYOUT_CHECK(COnGameNet)
	CHECK_FIELD(COnGameNet, m_playerIndex, 0x120);
	CHECK_FIELD(COnGameNet, m_playerMatrix, 0x124);
	CHECK_FIELD(COnGameNet, m_hTexture, 0x154);
END_LAYOUT_CHECK

class CNPC_BIRD : public CCreature {
public:
	CNPC_BIRD(const char *name, float speed);
	virtual ~CNPC_BIRD(void);
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

protected:
	void FindTarget(void);
};

class CNPC_PARROT : public CCreature {
public:
	CNPC_PARROT(const char *name);
	virtual ~CNPC_PARROT(void);
};

class CNPC_TORTOISE : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	CNPC_TORTOISE(const char *name, float speed);
	virtual ~CNPC_TORTOISE(void);
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

protected:
	float m_climbRate;          // 0x120, height change towards the target node
	int m_state;                // 0x124, 2 at start
};

BEGIN_LAYOUT_CHECK(CNPC_TORTOISE)
	CHECK_FIELD(CNPC_TORTOISE, m_climbRate, 0x120);
	CHECK_FIELD(CNPC_TORTOISE, m_state, 0x124);
END_LAYOUT_CHECK

// A Catmull-Rom spline through a list of points (0x14 bytes).
class ntCatmullRom {
	LAYOUT_CHECK_FRIEND;
public:
	ntCatmullRom(void);
	~ntCatmullRom(void);

	bool Init(const WVector *points, const float *times, int count);
	void Clean(void);
	WVector Evaluate(float t);
	WVector Velocity(float t);
	WVector Acceleration(float t);
	float ArcLength(float t0, float t1);
	float FindParameterByDistance(float t, float distance);

protected:
	float SegmentArcLength(int segment, float t0, float t1);

	WVector *m_pPoints;     // 0x0
	float *m_pTimes;        // 0x4
	float *m_pLengths;      // 0x8
	float m_totalLength;    // 0xC, divides the time range in FindParameterByDistance
	int m_count;            // 0x10, number of points
};

BEGIN_LAYOUT_CHECK(ntCatmullRom)
	CHECK_FIELD(ntCatmullRom, m_pPoints, 0x0);
	CHECK_FIELD(ntCatmullRom, m_pTimes, 0x4);
	CHECK_FIELD(ntCatmullRom, m_pLengths, 0x8);
	CHECK_FIELD(ntCatmullRom, m_totalLength, 0xC);
	CHECK_FIELD(ntCatmullRom, m_count, 0x10);
	CHECK_SIZE(ntCatmullRom, 0x14);
END_LAYOUT_CHECK

class CNPC_TRSHIP : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	CNPC_TRSHIP(const char *name, float speed);
	virtual ~CNPC_TRSHIP(void);
	virtual void Display(void);
	virtual void Process(float dt);
	virtual void SetArea(const std::vector<WVector> &points, float radius, unsigned long flags);
	virtual void CheckTarget(float dt);

protected:
	float m_distance;           // 0x120, the spline parameter (CheckTarget evaluates m_spline there)
	float m_speed;              // 0x124, the constructor's speed, 1 when it lies within [-1, 1]
	bool m_hidden;              // 0x128, not displayed while set
	UNKNOWN(0x129, 0x12C);
	ntCatmullRom m_spline;      // 0x12C
};

BEGIN_LAYOUT_CHECK(CNPC_TRSHIP)
	CHECK_FIELD(CNPC_TRSHIP, m_distance, 0x120);
	CHECK_FIELD(CNPC_TRSHIP, m_speed, 0x124);
	CHECK_FIELD(CNPC_TRSHIP, m_hidden, 0x128);
	CHECK_FIELD(CNPC_TRSHIP, m_spline, 0x12C);
END_LAYOUT_CHECK

class ntPenguin_Duck : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	ntPenguin_Duck(const char *name, int count);
	virtual ~ntPenguin_Duck(void);
	virtual void Display(void);
	virtual void Process(float dt);
	virtual void SetArea(const Waabb &area, unsigned long flags);

protected:
	bool m_playingAct0;         // 0x120, set while NPC_Act0 plays (Display draws only then)
	UNKNOWN(0x121, 0x124);
	int m_count;                // 0x124, the number of ducks
	WMatrix *m_pMatrices;       // 0x128, one transform per duck
	float *m_pScales;           // 0x12C, one random scale per duck (1 for the first)
};

BEGIN_LAYOUT_CHECK(ntPenguin_Duck)
	CHECK_FIELD(ntPenguin_Duck, m_playingAct0, 0x120);
	CHECK_FIELD(ntPenguin_Duck, m_count, 0x124);
	CHECK_FIELD(ntPenguin_Duck, m_pMatrices, 0x128);
	CHECK_FIELD(ntPenguin_Duck, m_pScales, 0x12C);
END_LAYOUT_CHECK

// Creatures flying along a spline (ntNpc_Dragon, ntNpc_PolaShip).
class ntNpc_Dragon : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	ntNpc_Dragon(const char *name, float speed);
	virtual ~ntNpc_Dragon(void);
	virtual void Display(void);
	virtual void Process(float dt);
	virtual void SetArea(const std::vector<WVector> &points, float radius, unsigned long flags);
	virtual void CheckTarget(float dt);

protected:
	UNKNOWN(0x120, 0x124);
	bool m_hidden;              // 0x124, not displayed while set
	float m_speed;              // 0x128
	float m_distance;           // 0x12C
	ntCatmullRom m_spline;      // 0x130
};

BEGIN_LAYOUT_CHECK(ntNpc_Dragon)
	CHECK_FIELD(ntNpc_Dragon, m_hidden, 0x124);
	CHECK_FIELD(ntNpc_Dragon, m_speed, 0x128);
	CHECK_FIELD(ntNpc_Dragon, m_distance, 0x12C);
	CHECK_FIELD(ntNpc_Dragon, m_spline, 0x130);
END_LAYOUT_CHECK

class ntNpc_PolaShip : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	ntNpc_PolaShip(const char *name, float speed);
	virtual ~ntNpc_PolaShip(void);
	virtual void Display(void);
	virtual void Process(float dt);
	virtual void SetArea(const std::vector<WVector> &points, float radius, unsigned long flags);
	virtual void CheckTarget(float dt);

protected:
	UNKNOWN(0x120, 0x124);
	bool m_hidden;              // 0x124, not displayed while set
	float m_speed;              // 0x128
	float m_distance;           // 0x12C
	ntCatmullRom m_spline;      // 0x130
};

BEGIN_LAYOUT_CHECK(ntNpc_PolaShip)
	CHECK_FIELD(ntNpc_PolaShip, m_hidden, 0x124);
	CHECK_FIELD(ntNpc_PolaShip, m_spline, 0x130);
END_LAYOUT_CHECK

class CDisposable : public CCreature {
public:
	CDisposable(const char *name);
	// npc.h:625
	virtual ~CDisposable(void) {}
	virtual void PlayEvent(void);
};

// A creature walking along a spline through nodes.
class ntNpc_NodeWalker : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	ntNpc_NodeWalker(const char *name, float speed);
	virtual ~ntNpc_NodeWalker(void);
	virtual void Process(float dt);
	virtual void SetArea(const std::vector<WVector> &points, float radius, unsigned long flags);
	virtual void CheckTarget(float dt);

protected:
	void MakeSpline(const std::vector<WVector> &points, float &length, ntCatmullRom &spline);

	float m_splineLength;       // 0x120
	float m_distance;           // 0x124, travelled along the spline
	ntCatmullRom m_spline;      // 0x128
};

BEGIN_LAYOUT_CHECK(ntNpc_NodeWalker)
	CHECK_FIELD(ntNpc_NodeWalker, m_splineLength, 0x120);
	CHECK_FIELD(ntNpc_NodeWalker, m_distance, 0x124);
	CHECK_FIELD(ntNpc_NodeWalker, m_spline, 0x128);
END_LAYOUT_CHECK

class ntNpc_WizCityNode : public ntNpc_NodeWalker {
	LAYOUT_CHECK_FRIEND;
public:
	ntNpc_WizCityNode(const char *name, float speed);
	virtual void Display(void);

	// npc.h:725
	void SetScale(float scale) { m_scale = scale; }

protected:
	float m_scale;              // 0x13C
};

BEGIN_LAYOUT_CHECK(ntNpc_WizCityNode)
	CHECK_FIELD(ntNpc_WizCityNode, m_scale, 0x13C);
END_LAYOUT_CHECK

// A creature walking inside an area, choosing its next motion from weighted
// lists. The destructor is pure (its vtable slot is _purecall) but defined.
class ntNpc_AreaWalker : public CCreature {
	LAYOUT_CHECK_FRIEND;
public:
	struct MotionMap {
		int m_threshold;        // 0x0, chosen when rand() % 100 is below it
		std::string m_motion;   // 0x4
	};

	ntNpc_AreaWalker(const char *name);
	virtual ~ntNpc_AreaWalker(void) = 0;
	virtual void Process(float dt);
	virtual void CheckTarget(float dt);

protected:
	virtual void ConstructNextMotion(void) = 0;
	void FindTarget(void);
	void SelectNextMotion(void);
	void AddWalkingMotion(const char *name);

	std::vector<std::string> m_walkingMotions;  // 0x120
	bool m_walking;                             // 0x130, CheckTarget moves only while set
	std::map<std::string, std::list<MotionMap> > m_motionMaps; // 0x134
};

BEGIN_LAYOUT_CHECK(ntNpc_AreaWalker)
	CHECK_FIELD(ntNpc_AreaWalker, m_walkingMotions, 0x120);
	CHECK_FIELD(ntNpc_AreaWalker, m_walking, 0x130);
	CHECK_FIELD(ntNpc_AreaWalker, m_motionMaps, 0x134);
END_LAYOUT_CHECK

class ntNpc_WizCityArea : public ntNpc_AreaWalker {
	LAYOUT_CHECK_FRIEND;
public:
	ntNpc_WizCityArea(const char *name);
	virtual void Display(void);

	// npc.h:739
	void SetScale(float scale) { m_scale = scale; }

protected:
	virtual void ConstructNextMotion(void);

	float m_scale;              // 0x140
};

BEGIN_LAYOUT_CHECK(ntNpc_WizCityArea)
	CHECK_FIELD(ntNpc_WizCityArea, m_scale, 0x140);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_NPC_H

