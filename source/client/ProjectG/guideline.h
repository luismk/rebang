// Original: d:\build\custom\temp\code\client\projectg\guideline.h
#ifndef PG_CLIENT_PROJECTG_GUIDELINE_H
#define PG_CLIENT_PROJECTG_GUIDELINE_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"
#include "scenemanager.h"
#include "wmath.h"
#include "wvideo.h"

class CFxSequence;
class IObject;

IObject* CGuideLineMakeInstance(void);

// The guide line actor (IActor at 0x0, CRenderFuncPtr at 0x18, seen in the
// constructor). Members are transcribed as far as the decompiled functions
// need them.
class CGuideLine : public IActor, public CRenderFuncPtr {
	LAYOUT_CHECK_FRIEND;
	friend IObject* CGuideLineMakeInstance(void);
public:
	virtual ~CGuideLine(void) {}   // guideline.h:14
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // guideline.h:16

	// One segment of the line, 0x28 bytes.
	class sRoad {
	public:
		sRoad(void);

		WVector m_left;         // 0x0, left edge of the segment
		WVector m_right;        // 0xc, right edge of the segment
		WVector m_point;        // 0x18, centre point on the ground
		CFxSequence* m_pFx;     // 0x24
	};

	virtual void OnPreLoadInit(void);
	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnProcess(float elapsed);
	virtual void OnDestroy(void);
	virtual void HandleMsg(const MsgObject& msg);
	virtual void Display(void);

protected:
	CGuideLine(void);

	void SetPoint(const WVector& dir);
	void CalcSegment(void);
	void SetActive(bool bActive);

	bool m_bHidden;             // 0x1c
	bool m_bFading;             // 0x1d
	UNKNOWN(0x1e, 0x20);
	int m_segmentCount;         // 0x20
	float m_alpha;              // 0x24, 0..255, the line fades in
	bool m_bActive;             // 0x28
	UNKNOWN(0x29, 0x2c);
	float m_segmentTime;        // 0x2c
	bool m_bDirty;              // 0x30
	UNKNOWN(0x31, 0x34);
	sRoad m_road[80];           // 0x34
	int m_texture;              // 0xcb4
	WTVertex m_vertex[4];       // 0xcb8, one segment's quad
	WTVertex* m_pVertex[4];     // 0xd58, fan order into m_vertex
	int m_bufferIndex;          // 0xd68
};

CHECK_SIZE(CGuideLine::sRoad, 0x28);
CHECK_FIELD(CGuideLine::sRoad, m_left, 0x0);
CHECK_FIELD(CGuideLine::sRoad, m_right, 0xc);
CHECK_FIELD(CGuideLine::sRoad, m_point, 0x18);
CHECK_FIELD(CGuideLine::sRoad, m_pFx, 0x24);
CHECK_SIZE(CGuideLine, 0xD6C);
BEGIN_LAYOUT_CHECK(CGuideLine)
	CHECK_FIELD(CGuideLine, m_bHidden, 0x1c);
	CHECK_FIELD(CGuideLine, m_bFading, 0x1d);
	CHECK_FIELD(CGuideLine, m_segmentCount, 0x20);
	CHECK_FIELD(CGuideLine, m_alpha, 0x24);
	CHECK_FIELD(CGuideLine, m_bActive, 0x28);
	CHECK_FIELD(CGuideLine, m_segmentTime, 0x2c);
	CHECK_FIELD(CGuideLine, m_bDirty, 0x30);
	CHECK_FIELD(CGuideLine, m_road, 0x34);
	CHECK_FIELD(CGuideLine, m_texture, 0xcb4);
	CHECK_FIELD(CGuideLine, m_vertex, 0xcb8);
	CHECK_FIELD(CGuideLine, m_pVertex, 0xd58);
	CHECK_FIELD(CGuideLine, m_bufferIndex, 0xd68);
END_LAYOUT_CHECK

#endif
