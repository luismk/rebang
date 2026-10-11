// Original: d:\build\custom\temp\code\client\projectg\rival.h
#ifndef PG_CLIENT_PROJECTG_RIVAL_H
#define PG_CLIENT_PROJECTG_RIVAL_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "wmath.h"

class CGolfDoc;

// The computer rival of a practice round: it plays its shot from the bar of
// the golf document and keeps the result of the ball.
class CRival : public WSingleton<CRival> {
	LAYOUT_CHECK_FRIEND;
public:
	CRival(void);
	virtual ~CRival(void);

	// rival.h:16
	WVector GetPos(void) { return m_pos; }

	void Shot(void);
	void SetResult(void);
	void SetVariable(CGolfDoc *pGolfDoc);

private:
	WVector GetGreenSlope(void);

	WVector m_pos;          // 0x28, the rival's ball position (GetPos)
	WVector m_expectedPos;  // 0x34, the ball's expected landing point (SetResult, CGolfBall::PreData)
	int m_state;            // 0x40, zeroed by the constructor
	float m_aimAngle;       // 0x44, the shot's yaw from CalcDeltaAngle against UNIT_POS_Z (SetVariable)
	float m_powerRate;      // 0x48, scales the bar power of the rival's shot (Shot)
	int m_clubIndex;        // 0x4C, the club the rival plays, at most 12 (SetVariable, CClub::SetClub)
	int m_result;           // 0x50, how the shot ended (SetResult: 0 none, 1 collision, 2..4 by the ball's lie)
	float m_lieSlope;       // 0x54, CGolfBall::m_lieSlope copied by SetResult
	float m_lieAngle;       // 0x58, CGolfBall::m_lieAngle copied by SetResult
};

CHECK_SIZE(CRival, 0x5C);
BEGIN_LAYOUT_CHECK(CRival)
	CHECK_FIELD(CRival, m_pos, 0x28);
	CHECK_FIELD(CRival, m_expectedPos, 0x34);
	CHECK_FIELD(CRival, m_state, 0x40);
	CHECK_FIELD(CRival, m_aimAngle, 0x44);
	CHECK_FIELD(CRival, m_powerRate, 0x48);
	CHECK_FIELD(CRival, m_clubIndex, 0x4C);
	CHECK_FIELD(CRival, m_result, 0x50);
	CHECK_FIELD(CRival, m_lieSlope, 0x54);
	CHECK_FIELD(CRival, m_lieAngle, 0x58);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_RIVAL_H
