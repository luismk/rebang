// Original: d:\build\custom\temp\code\client\projectg\golfball.h
#ifndef PG_CLIENT_PROJECTG_GOLFBALL_H
#define PG_CLIENT_PROJECTG_GOLFBALL_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"
#include "encrypttypes.h"

// The golf ball. Its members are transcribed only as far as the functions
// decompiled so far need them.
class CGolfBall {
	LAYOUT_CHECK_FRIEND;
public:
	CGolfBall(void);
	~CGolfBall(void);

	void ClearBuffer(void);
	void AddCrypticValue(void);
	void Reset(WVector pos);

	void DbgTraceOutput(const char *label, float value);
	void DbgTraceToFile(const char *label, const WVector &vector);

	// The water the ball is over (CQuadTree::GetGroundType's output); the
	// enumerators are not transcribed yet.
	enum eWater {
	};

	// One predicted point of the ball's flight.
	struct sExpected {
		WVector m_pos;          // 0x0
		WVector m_velocity;     // 0xC, read by CFxShotImpact
		UNKNOWN(0x18, 0x20);
		char m_sound[0x40];     // 0x20, the collision's sound name (CQuadTree::CommitResponse copies sColResponse::m_sound)
		UNKNOWN(0x60, 0x64);
		WVector m_contactVector;        // 0x64, CQuadTree::CommitResponse copies sColResponse::m_contactVector
		UNKNOWN(0x70, 0x74);
	};

	float GetMass(void) const;          // golfball.h:54
	// golfball.h:55
	float GetRadius(void) const { return m_radius; }
	int IsHoleIn(void);                 // golfball.h:56
	void SetHoleIn(int holeIn);         // golfball.h:57
	float GetPowerFactor(void) const;   // golfball.h:61
	void AddVectorSlideCount(void);     // golfball.h:190
	int GetVectorSlideCount(void);      // golfball.h:191
	void ResetVectorSlideCount(void);   // golfball.h:192

	const sExpected* PreData(int index) const;   // golfball.inl:8
	sExpected* PreDataToWrite(int index);        // golfball.inl:25 (out-of-line copy in GolfRule)

	WVector m_pos;          // 0x0, the ball position
	WVector m_velocity;     // 0xC
	int m_state;            // 0x18, 0 = at rest
	PADDING(0x1C, 0x28);
	int m_groundArea;       // 0x28, CQuadTree::GetGroundPoint's int* output (CGolfBg::SetStartPoint)
	float m_rotation[2];    // 0x2C, the ball model's rotation angles (CBase: RotMat axis 1, axis 0); [1] advanced by CQuadTree::SpinProcess
	float m_lieSlope;       // 0x34, copied with m_lieAngle by CRival::SetResult
	float m_lieAngle;       // 0x38, CClub::GetLie scales it by pi/45
	int m_shotCounters[2];  // 0x3C, zeroed by Reset (meaning not known yet)
	unsigned long m_powerFlags;     // 0x44, bits 0x50 raise GetPowerFactor (golfball.h:61) to 1.3
	unsigned long m_flags;  // 0x48, bit 0 reverses the power-spin effect (CFxPowerSpin::Process)
	WVector m_spinImpulse;  // 0x4C, added to m_velocity when the spin takes hold (CQuadTree::SpinProcess)
	bool m_landed;          // 0x58, tested by CRival::SetResult
	unsigned char m_groundType;     // 0x59, CQuadTree::GetGroundType of the ball's position (CBase::SetNormal); Reset sets 1
	PADDING(0x5A, 0x5C);
	int m_lie;              // 0x5C, the ground the ball rests on (CRival::SetResult: 2 and 3 grade alike, 1 otherwise)
	int m_shotStage;        // 0x60, zeroed by Reset (golfball.cpp:71)
	int m_preDataCount;     // 0x64, last valid index of PreData, negative when none
	int m_preDataIndex;     // 0x68, the PreData index of the ball's current point (CFxPowerSpin::Process)
	int m_eventPreDataIndex[15];    // 0x6C, -1 = none (Reset), like m_preDataIndex; [6] = spin start, [7] = spin applied (SpinProcess)
	PADDING(0xA8, 0xAC);
	int m_lastEventPreDataIndex;    // 0xAC, -1 = none (Reset)
	unsigned char m_bounceCount;    // 0xB0, CRival::SetResult gives no result above 4
	PADDING(0xB1, 0xB2);
	short m_firstContactId;         // 0xB2, -1 = none (Reset)
	int m_firstContactIndex;        // 0xB4, -1 = none (Reset)
	short m_lastContactId;          // 0xB8, -1 = none (Reset)
	PADDING(0xBA, 0xBC);
	int m_lastContactIndex;         // 0xBC, -1 = none (Reset)
	int m_vectorSlideCount; // 0xC0, ResetVectorSlideCount (golfball.h:192)
	float m_mass;           // 0xC4, GetMass (golfball.h:54)
	float m_radius;         // 0xC8
	sExpected m_preData[1500];   // 0xCC, PreData(0..1499)
	sExpected* m_pPreDataOverflow;   // 0x2A87C, PreData(1500..)
	unsigned char m_preDataOverflowBlocks;  // 0x2A880, blocks allocated in m_pPreDataOverflow (PreDataToWrite)
	PADDING(0x2A881, 0x2A884);
	WCrypticValue<int> m_holeIn;    // 0x2A884, IsHoleIn/SetHoleIn (golfball.h:56-57), watched by HackingManager
};
CHECK_SIZE(CGolfBall::sExpected, 0x74);
CHECK_FIELD(CGolfBall::sExpected, m_velocity, 0xC);
CHECK_FIELD(CGolfBall::sExpected, m_sound, 0x20);
CHECK_FIELD(CGolfBall::sExpected, m_contactVector, 0x64);

BEGIN_LAYOUT_CHECK(CGolfBall)
	CHECK_FIELD(CGolfBall, m_pos, 0x0);
	CHECK_FIELD(CGolfBall, m_velocity, 0xC);
	CHECK_FIELD(CGolfBall, m_state, 0x18);
	CHECK_FIELD(CGolfBall, m_groundArea, 0x28);
	CHECK_FIELD(CGolfBall, m_lieSlope, 0x34);
	CHECK_FIELD(CGolfBall, m_lieAngle, 0x38);
	CHECK_FIELD(CGolfBall, m_landed, 0x58);
	CHECK_FIELD(CGolfBall, m_lie, 0x5C);
	CHECK_FIELD(CGolfBall, m_bounceCount, 0xB0);
	CHECK_FIELD(CGolfBall, m_preDataCount, 0x64);
	CHECK_FIELD(CGolfBall, m_flags, 0x48);
	CHECK_FIELD(CGolfBall, m_preDataIndex, 0x68);
	CHECK_FIELD(CGolfBall, m_radius, 0xC8);
	CHECK_FIELD(CGolfBall, m_preData, 0xCC);
	CHECK_FIELD(CGolfBall, m_pPreDataOverflow, 0x2A87C);
	CHECK_FIELD(CGolfBall, m_rotation, 0x2C);
	CHECK_FIELD(CGolfBall, m_spinImpulse, 0x4C);
	CHECK_FIELD(CGolfBall, m_shotCounters, 0x3C);
	CHECK_FIELD(CGolfBall, m_powerFlags, 0x44);
	CHECK_FIELD(CGolfBall, m_groundType, 0x59);
	CHECK_FIELD(CGolfBall, m_shotStage, 0x60);
	CHECK_FIELD(CGolfBall, m_eventPreDataIndex, 0x6C);
	CHECK_FIELD(CGolfBall, m_lastEventPreDataIndex, 0xAC);
	CHECK_FIELD(CGolfBall, m_firstContactId, 0xB2);
	CHECK_FIELD(CGolfBall, m_firstContactIndex, 0xB4);
	CHECK_FIELD(CGolfBall, m_lastContactId, 0xB8);
	CHECK_FIELD(CGolfBall, m_lastContactIndex, 0xBC);
	CHECK_FIELD(CGolfBall, m_vectorSlideCount, 0xC0);
	CHECK_FIELD(CGolfBall, m_mass, 0xC4);
	CHECK_FIELD(CGolfBall, m_preDataOverflowBlocks, 0x2A880);
	CHECK_FIELD(CGolfBall, m_holeIn, 0x2A884);
END_LAYOUT_CHECK

// GolfRule.cpp and QuadTree.cpp define GOLFRULE_GOLFBALL_INLINE and
// QUADTREE_GOLFBALL_INLINE empty before their includes: the original holds the
// out-of-line copies of these inline members in those objects; every other
// unit inlines them.
#ifndef GOLFRULE_GOLFBALL_INLINE
#define GOLFRULE_GOLFBALL_INLINE inline
#endif
#ifndef QUADTREE_GOLFBALL_INLINE
#define QUADTREE_GOLFBALL_INLINE inline
#endif

// golfball.h:54
QUADTREE_GOLFBALL_INLINE float CGolfBall::GetMass(void) const {
	return m_mass;
}

// golfball.h:56
GOLFRULE_GOLFBALL_INLINE int CGolfBall::IsHoleIn(void) {
	return m_holeIn == 1 ? true : false;
}

// golfball.h:57
GOLFRULE_GOLFBALL_INLINE void CGolfBall::SetHoleIn(int holeIn) {
	m_holeIn = holeIn;
}

// golfball.h:61
GOLFRULE_GOLFBALL_INLINE float CGolfBall::GetPowerFactor(void) const {
	return !(m_powerFlags & 0x50) ? 1.0f : 1.3f;
}

// golfball.h:190
QUADTREE_GOLFBALL_INLINE void CGolfBall::AddVectorSlideCount(void) {
	m_vectorSlideCount++;
}

// golfball.h:191
QUADTREE_GOLFBALL_INLINE int CGolfBall::GetVectorSlideCount(void) {
	return m_vectorSlideCount;
}

// golfball.h:192
GOLFRULE_GOLFBALL_INLINE void CGolfBall::ResetVectorSlideCount(void) {
	m_vectorSlideCount = 0;
}

// The ball singleton (a function-local static, golfball.inl).
CGolfBall &GolfBall(void);

#include "golfball.inl"

#endif // PG_CLIENT_PROJECTG_GOLFBALL_H
