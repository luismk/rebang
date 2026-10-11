// Original: d:\build\custom\temp\code\client\projectg\spin.h
#ifndef PG_CLIENT_PROJECTG_SPIN_H
#define PG_CLIENT_PROJECTG_SPIN_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"

class IObject;

IObject* CSpinMakeInstance(void);

// The spin (impact point) marker of the shot gauge: a point moved inside an
// ellipse around a base position, with an automatic point that loops around
// the ellipse's edge.
class CSpin : public IActor {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;

	virtual ~CSpin(void) {}   // spin.h:9

	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // spin.h:11

	virtual void OnPreLoadInit(void);
	virtual void OnInit(void);
	virtual void OnProcess(float elapsed);
	virtual void HandleMsg(const MsgObject& msg);

protected:
	friend IObject* CSpinMakeInstance(void);   // the factory creates it
	CSpin(void);

	void Reset(void);
	void GetNextDirection(void);
	void ProcessLoopHitPoint(float elapsed, float &x, float &y, float &unused);
	void ProcessAutoPoint(float elapsed);
	void ConvertToWorld(float x, float y);
	bool IsPointInAutoPoint(void);
	bool IsInclude(float x, float y);

	float m_worldX;             // 0x18, the point on the screen
	float m_worldY;             // 0x1C
	float m_spinX;              // 0x20, the point relative to the base
	float m_spinY;              // 0x24
	float m_radiusX;            // 0x28, the ellipse's radii
	float m_radiusY;            // 0x2C
	float m_baseX;              // 0x30, the ellipse's centre on the screen
	float m_baseY;              // 0x34
	float m_angle;              // 0x38, the point's angle on the ellipse's edge while the keys move it
	PADDING(0x3C, 0x40);
	unsigned long m_color;      // 0x40, the point's color
	float m_autoWorldX;         // 0x44, the automatic point on the screen
	float m_autoWorldY;         // 0x48
	float m_autoX;              // 0x4C, the automatic point relative to the base
	float m_autoY;              // 0x50
	bool m_bActive;             // 0x54
	bool m_bAutoPoint;          // 0x55, the automatic point moves
	bool m_bFlag56;             // 0x56
	bool m_bToggle57;           // 0x57
	int m_direction;            // 0x58, the automatic point's direction (1..5)
	int m_nextDirection;        // 0x5C
	int m_loopMode;             // 0x60, how GetNextDirection picks the next one
	bool m_bFlag64;             // 0x64
	bool m_bFlag65;             // 0x65
	PADDING(0x66, 0x68);
};

CHECK_SIZE(CSpin, 0x68);
BEGIN_LAYOUT_CHECK(CSpin)
	CHECK_FIELD(CSpin, m_worldX, 0x18);
	CHECK_FIELD(CSpin, m_radiusX, 0x28);
	CHECK_FIELD(CSpin, m_baseX, 0x30);
	CHECK_FIELD(CSpin, m_angle, 0x38);
	CHECK_FIELD(CSpin, m_color, 0x40);
	CHECK_FIELD(CSpin, m_autoWorldX, 0x44);
	CHECK_FIELD(CSpin, m_autoX, 0x4C);
	CHECK_FIELD(CSpin, m_bActive, 0x54);
	CHECK_FIELD(CSpin, m_bToggle57, 0x57);
	CHECK_FIELD(CSpin, m_direction, 0x58);
	CHECK_FIELD(CSpin, m_loopMode, 0x60);
	CHECK_FIELD(CSpin, m_bFlag65, 0x65);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SPIN_H
