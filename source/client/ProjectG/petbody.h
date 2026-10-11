// Original: d:\build\custom\temp\code\client\projectg\petbody.h
#ifndef PG_CLIENT_PROJECTG_PETBODY_H
#define PG_CLIENT_PROJECTG_PETBODY_H

#include <list>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wflag.h"
#include "wlist.h"
#include "wmath.h"
#include "wscene.h"

class IActor;
class WPuppet;
struct sTexturePart;
struct w_motion_data;

// The animated body of a pet or caddie. Only the methods the units built so
// far call are declared; the members are partly transcribed.
class CPetBody {
	LAYOUT_CHECK_FRIEND;
public:
	// A local light and where it is (size 0x28).
	struct sLightSet {
		LightSet light;     // 0x0
		WVector pos;        // 0x1c
	};

	// Which matrix GetMatrix returns. The original names are not known.
	enum eBody {
		BODY_HEAD = 0,          // the head bone, turned by the look angle
		BODY_PUPPET = 1,        // the puppet's matrix
		BODY_PUPPET_AT_POS = 2  // the puppet's matrix moved to m_pos
	};

	// A script callback registered by name (size 0xc).
	struct ScriptHandle {
		const char *name;   // 0x0
		IActor *actor;      // 0x4
		int id;             // 0x8
	};

	// Vtable: ~CPetBody 0x0, Display 0x4, DisplayShadow 0x8 (the caddie
	// calls [vtable+8]), Process 0xc (ntFlutter calls [vtable+0xc]).
	CPetBody(const char *script, bool arg);
	virtual ~CPetBody(void);
	virtual void Display(void);
	virtual void DisplayShadow(char *name);
	virtual void Process(float delta);

	void ActiveGlobalLight(bool active);

	void InitScript(const char *script, bool arg);
	void Reset(void);
	void SetPos(const WVector &pos);
	void MultRotate(WMatrix *rot);
	void TakeOff(WVector dir);
	void SetScale(float scale);
	void Update(void);
	void SetRotate(const WVector &rotation);
	void SetRotate_Lean(const WVector &rotation);
	void SetRotate(float angle);
	void SetRotateSlope(float angle, WVector normal);
	void Rotate(float angle);
	WMatrix GetMatrix(eBody body);
	void AddScriptHandle(const char *name, IActor *actor, int id);
	void SetPendingTexPart(char *animName, char *texName);
	void SetCustomGround(char *groundName);
	void AddLocalLight(const LightSet &light, WVector pos);
	void ChangeTexture(char *name, int index);
	void LookUp(WVector *target, bool immediate);
	float GetAnimationLength(const char *motionName);
	void SetNextMotion(const char *motionName, bool loop);
	WPuppet *GetPet(void);
	void SetLight(void);
	void SetMotion(const char *name, bool loop, float time, bool applyDelta);
	// petbody.h:59
	WVector GetDirection(void) const { return m_rotMatrix.za * -1.0f; }

protected:
	void Init(void);
	void Animation(float delta);
	float Move(float delta);
	void ParsePuppetScript(const char *script);
	void FlightProcess(float delta);
	void LookUpProcess(float delta);
	void ScriptProcess(const w_motion_data *motion, float prevTime, float time);
	void SetPivot(const char *boneName, WVector offset);
	void LoadPuppet(const char *name, bool optimizeBone, bool arg3);
	void SetShadowType(int type);
	bool ShouldDisplay(void);
	sTexturePart *FindTexturePart(char *animName, char *texName);

	WPuppet *m_pPet;                // 0x4
	int m_shadowType;               // 0x8
	const w_motion_data *m_pMotion; // 0xc
	float m_motionTime;             // 0x10, the frame GetBones samples
	WMatrix m_rotMatrix;            // 0x14, rotation, pivot = m_pos
	WMatrix m_matrix;               // 0x44, m_rotMatrix scaled (or leaned)
	WVector m_rotation;             // 0x74, Euler angles of m_rotMatrix
	WVector m_pos;                  // 0x80
	WVector m_moveDelta;            // 0x8c, the motion's delta this frame, added to m_pos
	float m_scale;                  // 0x98
	int m_flightState;              // 0x9c, 0 after TakeOff (Init sets 2)
	bool m_bFlying;                 // 0xa0, role unconfirmed (set by the script, tested by FlightProcess)
	PADDING(0xa1, 0xa4);
	WVector m_takeOffDir;           // 0xa4
	WVector m_takeOffPos;           // 0xb0, m_pos when TakeOff was called
	float m_takeOffHeight;          // 0xbc, role unconfirmed (reset to 0)
	float m_takeOffSpeed;           // 0xc0, role unconfirmed (set to 50)
	float m_takeOffAngle;           // 0xc4, role unconfirmed (set to -pi/4)
	WFlags m_flags;                 // 0xc8, 0x1: Move accumulated a motion delta, 0x2: the motion loops,
	                                //       0x4: SetMotion ran, 0x10: matrix changed, 0x2000: hidden
	WList<ScriptHandle *> m_scriptHandles;  // 0xcc
	std::vector<sLightSet> m_localLights;   // 0xec
	bool m_bUseLocalLights;         // 0xfc, Display applies m_localLights
	bool m_bUseLight;               // 0xfd, Display hands m_light to the puppet
	PADDING(0xfe, 0x100);
	LightSet m_light;               // 0x100
	std::string m_customGround;     // 0x11c
	WVector m_accumDelta;           // 0x138, the motion deltas Move accumulated (body frame)
	float m_aniSpeed;               // 0x144, role unconfirmed (reset to 1)
	float m_aniWeight;              // 0x148, role unconfirmed (reset to 1)
	char m_nextMotion[32];          // 0x14c
	bool m_bNextMotionLoop;         // 0x16c
	PADDING(0x16d, 0x170);
	std::list<sTexturePart *> m_pendingTexParts; // 0x170, searched by FindTexturePart
	WVector *m_lookTarget;          // 0x17c
	bool m_bLookImmediate;          // 0x180
	WVector2D m_lookAngle;          // 0x184, role unconfirmed
	WVector2D m_lookAngleTarget;    // 0x18c, role unconfirmed
};
BEGIN_LAYOUT_CHECK(CPetBody)
	CHECK_FIELD(CPetBody, m_pPet, 0x4);
	CHECK_FIELD(CPetBody, m_shadowType, 0x8);
	CHECK_FIELD(CPetBody, m_motionTime, 0x10);
	CHECK_FIELD(CPetBody, m_rotMatrix, 0x14);
	CHECK_FIELD(CPetBody, m_matrix, 0x44);
	CHECK_FIELD(CPetBody, m_rotation, 0x74);
	CHECK_FIELD(CPetBody, m_pMotion, 0xc);
	CHECK_FIELD(CPetBody, m_pos, 0x80);
	CHECK_FIELD(CPetBody, m_moveDelta, 0x8c);
	CHECK_FIELD(CPetBody, m_bFlying, 0xa0);
	CHECK_FIELD(CPetBody, m_bUseLocalLights, 0xfc);
	CHECK_FIELD(CPetBody, m_bUseLight, 0xfd);
	CHECK_FIELD(CPetBody, m_light, 0x100);
	CHECK_FIELD(CPetBody, m_scale, 0x98);
	CHECK_FIELD(CPetBody, m_flightState, 0x9c);
	CHECK_FIELD(CPetBody, m_takeOffDir, 0xa4);
	CHECK_FIELD(CPetBody, m_takeOffPos, 0xb0);
	CHECK_FIELD(CPetBody, m_takeOffHeight, 0xbc);
	CHECK_FIELD(CPetBody, m_takeOffSpeed, 0xc0);
	CHECK_FIELD(CPetBody, m_takeOffAngle, 0xc4);
	CHECK_FIELD(CPetBody, m_scriptHandles, 0xcc);
	CHECK_FIELD(CPetBody, m_localLights, 0xec);
	CHECK_FIELD(CPetBody, m_customGround, 0x11c);
	CHECK_FIELD(CPetBody, m_accumDelta, 0x138);
	CHECK_FIELD(CPetBody, m_nextMotion, 0x14c);
	CHECK_FIELD(CPetBody, m_bNextMotionLoop, 0x16c);
	CHECK_FIELD(CPetBody, m_pendingTexParts, 0x170);
	CHECK_FIELD(CPetBody, m_flags, 0xc8);
	CHECK_FIELD(CPetBody, m_aniSpeed, 0x144);
	CHECK_FIELD(CPetBody, m_aniWeight, 0x148);
	CHECK_FIELD(CPetBody, m_lookTarget, 0x17c);
	CHECK_FIELD(CPetBody, m_lookAngle, 0x184);
	CHECK_FIELD(CPetBody, m_lookAngleTarget, 0x18c);
	CHECK_FIELD(CPetBody, m_bLookImmediate, 0x180);
END_LAYOUT_CHECK
CHECK_SIZE(CPetBody::sLightSet, 0x28);
CHECK_SIZE(CPetBody::ScriptHandle, 0xc);

// Petal.cpp defines PETAL_PETBODY_INLINE empty before its includes: the
// original holds the out-of-line copy of this inline in Petal's object;
// every other unit inlines it.
#ifndef PETAL_PETBODY_INLINE
#define PETAL_PETBODY_INLINE inline
#endif

// petbody.h:51
PETAL_PETBODY_INLINE void CPetBody::ActiveGlobalLight(bool active) {
	m_bUseLight = active;
}

#endif // PG_CLIENT_PROJECTG_PETBODY_H
