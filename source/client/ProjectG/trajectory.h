// Original: d:\build\custom\temp\code\client\projectg\trajectory.h
#ifndef PG_CLIENT_PROJECTG_TRAJECTORY_H
#define PG_CLIENT_PROJECTG_TRAJECTORY_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"
#include "scenemanager.h"
#include "../Wangreal/include/wmath.h"
#include "../Wangreal/include/wvideo.h"

class IObject;
class CBallTrace;
class CComet;
class CDoughnut;
class CPuttLine;
class CBallTrack;
class CSkidMark;
class CBallWindTrack;
class ntSkidMark_Cobra;
class ntExplodeMark;

IObject* CTrajectoryMakeInstance(void);

// The trajectory actor (operator new size 0xC8 in CTrajectoryMakeInstance).
class CTrajectory : public IActor, public CRenderFuncPtr {
	friend IObject* CTrajectoryMakeInstance(void);
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const;
	// trajectory.h:27
	virtual ~CTrajectory(void) {}
	virtual void OnPreLoadInit(void);
	virtual void OnLoad(void);
	virtual void OnProcess(float);
	virtual void OnDestroy(void);
	virtual void Display(void);
	virtual void HandleMsg(const MsgObject &msg);

protected:
	CTrajectory(void);

private:
	CBallTrace *m_pActiveTrace[2];      // 0x1C (processed while the ball flies; not owned)
	CComet *m_pComet;                   // 0x24
	CDoughnut *m_pDoughnut;             // 0x28
	CPuttLine *m_pPuttLine;             // 0x2C
	CBallTrack *m_pBallTrack;           // 0x30
	CSkidMark *m_pSkidMark;             // 0x34
	CBallWindTrack *m_pBallWindTrack;   // 0x38
	CPuttLine *m_pEffectTrace[32];      // 0x3C (kept putt lines: displayed every frame, deleted by OnDestroy)
	CPuttLine *m_pCurPuttLine;          // 0xBC (putt line being drawn, also in m_pEffectTrace)
	ntSkidMark_Cobra *m_pCobraSkidMark; // 0xC0
	ntExplodeMark *m_pExplodeMark;      // 0xC4
	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(CTrajectory, 0xC8);
BEGIN_LAYOUT_CHECK(CTrajectory)
	CHECK_FIELD(CTrajectory, m_pActiveTrace, 0x1C);
	CHECK_FIELD(CTrajectory, m_pComet, 0x24);
	CHECK_FIELD(CTrajectory, m_pBallWindTrack, 0x38);
	CHECK_FIELD(CTrajectory, m_pEffectTrace, 0x3C);
	CHECK_FIELD(CTrajectory, m_pCurPuttLine, 0xBC);
	CHECK_FIELD(CTrajectory, m_pCobraSkidMark, 0xC0);
	CHECK_FIELD(CTrajectory, m_pExplodeMark, 0xC4);
END_LAYOUT_CHECK

class CBallTrace {
	LAYOUT_CHECK_FRIEND;
public:
	CBallTrace(void);
	virtual ~CBallTrace(void) {}     // trajectory.h:63
	virtual void Init(void) = 0;
	virtual void Process(float fElapsed) = 0;
	virtual void Display(void) = 0;

protected:
	virtual void Generate(void);     // trajectory.h:70
	virtual void BlowAway(float fDist, int nType);     // trajectory.h:71

	int m_nState;                // 0x4 (zeroed by the constructor)
	int m_hTexture;              // 0x8 (g_resrcmng texture handle)
	int m_nCount;                // 0xC (element count of the derived trail)
	int m_nHead;                 // 0x10 (reset by Init)
	float m_fTimer;              // 0x14 (time since the last Generate, reset by Init)
	float m_fInterval;           // 0x18
	bool m_bActive;              // 0x1C
	WTVertex *m_pVertex[4];      // 0x20 (-> m_vertex[0], [1], [3], [2]: a fan for DrawPolygonFan)
	int m_nVertex;               // 0x30
	WTVertex m_vertex[4];        // 0x34 (quad corners; CSkidMark::Init sets the UVs)
};
CHECK_SIZE(CBallTrace, 0xD4);
BEGIN_LAYOUT_CHECK(CBallTrace)
	CHECK_FIELD(CBallTrace, m_hTexture, 0x8);
	CHECK_FIELD(CBallTrace, m_nCount, 0xC);
	CHECK_FIELD(CBallTrace, m_nHead, 0x10);
	CHECK_FIELD(CBallTrace, m_fTimer, 0x14);
	CHECK_FIELD(CBallTrace, m_fInterval, 0x18);
	CHECK_FIELD(CBallTrace, m_bActive, 0x1C);
	CHECK_FIELD(CBallTrace, m_pVertex, 0x20);
	CHECK_FIELD(CBallTrace, m_nVertex, 0x30);
	CHECK_FIELD(CBallTrace, m_vertex, 0x34);
END_LAYOUT_CHECK

class CBallTrack : public CBallTrace {
	LAYOUT_CHECK_FRIEND;
public:
	// Trail point.
	struct sPoint {
		WVector m_vPos;      // 0x0
		WVector m_vSide;     // 0xC
		float m_fAlpha;      // 0x18

		// trajectory.h:106
		sPoint() { m_fAlpha = 1.0f; }
	};

	// Rainbow segment: the implicit default constructor (WVector members)
	// is the out-of-line ??0sPointEx.
	struct sPointEx {
		bool m_bActive;      // 0x0
		float m_fProgress;   // 0x4
		WVector m_vStart;    // 0x8
		WVector m_vDir;      // 0x14
	};

	CBallTrack(void);
	virtual ~CBallTrack(void);
	virtual void Init(void);     // trajectory.cpp:334
	virtual void Process(float fElapsed);
	virtual void Display(void);

	sPoint *m_pPoint;        // 0xD4 (allocated in Init)
	bool m_bRainbow;         // 0xD8
	sPointEx *m_pPointEx;    // 0xDC (allocated in Init)
	int m_hRainbowTexture;   // 0xE0 (released by the destructor)
	int m_nPoint;            // 0xE4 (points of m_pPoint in use, 1 after Init)

private:
	void UpdateRainbowEffect(float fElapsed);

	PADDING(0xE8, 0xF8);     // members not transcribed yet
	int m_nLastFrame;        // 0xF8 (last PreData index added to the trail, 0 after Init)
};
CHECK_SIZE(CBallTrack, 0xFC);
CHECK_SIZE(CBallTrack::sPoint, 0x1C);
CHECK_FIELD(CBallTrack::sPoint, m_vPos, 0x0);
CHECK_FIELD(CBallTrack::sPoint, m_vSide, 0xC);
CHECK_FIELD(CBallTrack::sPoint, m_fAlpha, 0x18);
CHECK_FIELD(CBallTrack, m_pPoint, 0xD4);
CHECK_FIELD(CBallTrack, m_bRainbow, 0xD8);
CHECK_FIELD(CBallTrack, m_pPointEx, 0xDC);
CHECK_FIELD(CBallTrack, m_hRainbowTexture, 0xE0);
CHECK_FIELD(CBallTrack, m_nPoint, 0xE4);
BEGIN_LAYOUT_CHECK(CBallTrack)
	CHECK_FIELD(CBallTrack, m_nLastFrame, 0xF8);
END_LAYOUT_CHECK
CHECK_SIZE(CBallTrack::sPointEx, 0x20);
CHECK_FIELD(CBallTrack::sPointEx, m_bActive, 0x0);
CHECK_FIELD(CBallTrack::sPointEx, m_fProgress, 0x4);
CHECK_FIELD(CBallTrack::sPointEx, m_vStart, 0x8);
CHECK_FIELD(CBallTrack::sPointEx, m_vDir, 0x14);

// Skid mark of the rolling ball (operator new size 0xEC in CTrajectory::OnLoad).
class CSkidMark : public CBallTrace {
	LAYOUT_CHECK_FRIEND;
public:
	// Mark point on the ground.
	struct sPoint {
		WVector m_vLeft;         // 0x0
		WVector m_vRight;        // 0xC
		unsigned char m_alpha;   // 0x18 (GetPointAlpha)
		sPoint() {}
	};

	CSkidMark(void);
	virtual ~CSkidMark(void);
	virtual void Init(void);
	virtual void Process(float fElapsed);
	virtual void Display(void);
	void SetFrame(int nStart, int nEnd);

protected:
	unsigned char GetPointAlpha(int, int);

private:
	sPoint *m_pPoint;        // 0xD4 (m_nPoint elements, allocated in Init)
	int m_nPoint;            // 0xD8 (m_nEndFrame - m_nStartFrame + 1)
	int m_nCurPoint;         // 0xDC (reset by Init)
	int m_nStartFrame;       // 0xE0 (PreData index, -1 = unset)
	int m_nEndFrame;         // 0xE4 (PreData index, -1 = unset)
	int m_nLastFrame;        // 0xE8 (m_nStartFrame - 1 after Init)
};
CHECK_SIZE(CSkidMark, 0xEC);
CHECK_SIZE(CSkidMark::sPoint, 0x1C);
CHECK_FIELD(CSkidMark::sPoint, m_vRight, 0xC);
CHECK_FIELD(CSkidMark::sPoint, m_alpha, 0x18);
BEGIN_LAYOUT_CHECK(CSkidMark)
	CHECK_FIELD(CSkidMark, m_pPoint, 0xD4);
	CHECK_FIELD(CSkidMark, m_nPoint, 0xD8);
	CHECK_FIELD(CSkidMark, m_nCurPoint, 0xDC);
	CHECK_FIELD(CSkidMark, m_nStartFrame, 0xE0);
	CHECK_FIELD(CSkidMark, m_nEndFrame, 0xE4);
	CHECK_FIELD(CSkidMark, m_nLastFrame, 0xE8);
END_LAYOUT_CHECK

// Cobra shot skid mark (operator new size 0xEC in CTrajectory::OnLoad).
class ntSkidMark_Cobra : public CBallTrace {
	LAYOUT_CHECK_FRIEND;
public:
	// Mark point on the ground.
	struct sPoint {
		WVector m_vLeft;     // 0x0
		WVector m_vRight;    // 0xC
		sPoint() {}
	};

	ntSkidMark_Cobra(void);
	virtual ~ntSkidMark_Cobra(void);
	virtual void Init(void);
	virtual void Process(float fElapsed);
	virtual void Display(void);
	void SetFrame(int nStart, int nEnd);

private:
	sPoint *m_pPoint;        // 0xD4
	int m_nPoint;            // 0xD8
	int m_nCurPoint;         // 0xDC
	int m_nStartFrame;       // 0xE0 (PreData index, -1 = unset)
	int m_nEndFrame;         // 0xE4 (PreData index, -1 = unset)
	int m_nLastFrame;        // 0xE8
};
CHECK_SIZE(ntSkidMark_Cobra, 0xEC);
CHECK_SIZE(ntSkidMark_Cobra::sPoint, 0x18);
BEGIN_LAYOUT_CHECK(ntSkidMark_Cobra)
	CHECK_FIELD(ntSkidMark_Cobra, m_pPoint, 0xD4);
	CHECK_FIELD(ntSkidMark_Cobra, m_nStartFrame, 0xE0);
	CHECK_FIELD(ntSkidMark_Cobra, m_nLastFrame, 0xE8);
END_LAYOUT_CHECK

// Wind lines along the ball track (operator new size 0x1F0 in CTrajectory::OnLoad).
class CBallWindTrack : public CBallTrace {
	LAYOUT_CHECK_FRIEND;
public:
	enum { MAX_LINE_EFFECT = 256 };

	struct sLineEffect {
		WVector m_vStart;    // 0x0
		WVector m_vEnd;      // 0xC
		int m_nAlpha;        // 0x18 (scaled by m_fLineAlpha in Display)
		PADDING(0x1C, 0x20); // members not transcribed yet
		bool m_bActive;      // 0x20
		PADDING(0x21, 0x24); // members not transcribed yet
		sLineEffect() {}
	};

	CBallWindTrack(void);
	virtual ~CBallWindTrack(void);
	virtual void Init(void);
	virtual void Process(float fElapsed);
	virtual void Display(void);

private:
	sLineEffect *m_pLineEffect;  // 0xD4 (MAX_LINE_EFFECT elements)
	CBallTrack m_ballTrack;      // 0xD8
	float m_fLineAlpha;          // 0x1D4 (line alpha scale, from the eye movement length)
	WVector m_vLastEyePos;       // 0x1D8 (eye position of the previous Process)
	WVector m_vEyeMove;          // 0x1E4 (eye position minus m_vLastEyePos)
};
CHECK_SIZE(CBallWindTrack, 0x1F0);
CHECK_SIZE(CBallWindTrack::sLineEffect, 0x24);
CHECK_FIELD(CBallWindTrack::sLineEffect, m_vEnd, 0xC);
CHECK_FIELD(CBallWindTrack::sLineEffect, m_nAlpha, 0x18);
CHECK_FIELD(CBallWindTrack::sLineEffect, m_bActive, 0x20);
BEGIN_LAYOUT_CHECK(CBallWindTrack)
	CHECK_FIELD(CBallWindTrack, m_pLineEffect, 0xD4);
	CHECK_FIELD(CBallWindTrack, m_ballTrack, 0xD8);
	CHECK_FIELD(CBallWindTrack, m_fLineAlpha, 0x1D4);
	CHECK_FIELD(CBallWindTrack, m_vLastEyePos, 0x1D8);
	CHECK_FIELD(CBallWindTrack, m_vEyeMove, 0x1E4);
END_LAYOUT_CHECK

// Putting guide line: a CBallTrace that ages its points.
class CPuttLine : public CBallTrace {
	LAYOUT_CHECK_FRIEND;
public:
	// Guide point: m_nAlpha 0x100 marks a kept point.
	struct sPoint {
		int m_nAlpha;        // 0x0
		WVector m_vLeft;     // 0x4
		WVector m_vRight;    // 0x10
	};

	CPuttLine(void);
	virtual ~CPuttLine(void);
	virtual void Init(void);
	virtual void Process(float fElapsed);
	virtual void Display(void);
	bool IsKeep(void);
	int HowOldAreYou(void);
	void RemoveUnkeptAlpha(void);

private:
	sPoint *m_pPoint;            // 0xD4 (delete[] in the destructor)
	int m_nLastPoint;            // 0xD8 (last valid index of m_pPoint)
	unsigned long m_dwStartTime; // 0xDC (timeGetTime())
};
CHECK_SIZE(CPuttLine, 0xE0);
CHECK_SIZE(CPuttLine::sPoint, 0x1C);
CHECK_FIELD(CPuttLine::sPoint, m_nAlpha, 0x0);
CHECK_FIELD(CPuttLine::sPoint, m_vLeft, 0x4);
CHECK_FIELD(CPuttLine::sPoint, m_vRight, 0x10);
BEGIN_LAYOUT_CHECK(CPuttLine)
	CHECK_FIELD(CPuttLine, m_pPoint, 0xD4);
	CHECK_FIELD(CPuttLine, m_nLastPoint, 0xD8);
	CHECK_FIELD(CPuttLine, m_dwStartTime, 0xDC);
END_LAYOUT_CHECK

// Comet trail (operator new size 0x2E0 in CTrajectory::OnLoad).
class CComet : public CBallTrace {
	LAYOUT_CHECK_FRIEND;
public:
	CComet(void);
	virtual ~CComet(void);
	virtual void Init(void);
	virtual void Process(float fElapsed);
	virtual void Display(void);

	// Trail point: the ball position plus and minus the side vector.
	struct sPoint {
		WVector m_vLeft;     // 0x0
		WVector m_vRight;    // 0xC
	};

	enum { MAX_POINT = 20 };

protected:
	virtual void Generate(void);

private:
	sPoint m_point[MAX_POINT];   // 0xD4 (ring buffer, m_nHead is the newest)
	float m_fNumPoint;       // 0x2B4 (points drawn: grows by one per Generate up to m_nCount)
	PADDING(0x2B8, 0x2BC);   // members not transcribed yet
	float m_fAlpha;          // 0x2BC (255 after Init, fades once the ball falls)
	int m_hTailTexture;      // 0x2C0 (released by the destructor)
	int m_bHeadPoint;        // 0x2C4 (m_vHeadLeft/m_vHeadRight valid, set by Process, cleared by Generate)
	WVector m_vHeadLeft;     // 0x2C8 (ball position plus the side vector, between two Generates)
	WVector m_vHeadRight;    // 0x2D4 (ball position minus the side vector)
};
CHECK_SIZE(CComet, 0x2E0);
CHECK_SIZE(CComet::sPoint, 0x18);
BEGIN_LAYOUT_CHECK(CComet)
	CHECK_FIELD(CComet, m_point, 0xD4);
	CHECK_FIELD(CComet, m_fNumPoint, 0x2B4);
	CHECK_FIELD(CComet, m_fAlpha, 0x2BC);
	CHECK_FIELD(CComet, m_bHeadPoint, 0x2C4);
	CHECK_FIELD(CComet, m_vHeadLeft, 0x2C8);
	CHECK_FIELD(CComet, m_vHeadRight, 0x2D4);
	CHECK_FIELD(CComet, m_hTailTexture, 0x2C0);
END_LAYOUT_CHECK

// Rings around the ball (operator new size 0xD8 in CTrajectory::OnLoad).
class CDoughnut : public CBallTrace {
	LAYOUT_CHECK_FRIEND;
public:
	// Ring: the implicit default constructor (WMatrix member) is the
	// out-of-line ??0sParticle.
	struct sParticle {
		float m_fScale;      // 0x0 (1 when generated, shrinks with time)
		float m_fAlpha;      // 0x4 (255 when generated, fades with time)
		bool m_bActive;      // 0x8
		WMatrix m_matrix;    // 0xC (camera matrix along the ball's velocity)
	};

	CDoughnut(void);
	virtual ~CDoughnut(void);
	virtual void Init(void);
	virtual void Process(float fElapsed);
	virtual void Display(void);

private:
	sParticle *m_pParticle;  // 0xD4 (m_nCount elements)
};
CHECK_SIZE(CDoughnut, 0xD8);
CHECK_SIZE(CDoughnut::sParticle, 0x3C);
CHECK_FIELD(CDoughnut::sParticle, m_fAlpha, 0x4);
CHECK_FIELD(CDoughnut::sParticle, m_bActive, 0x8);
CHECK_FIELD(CDoughnut::sParticle, m_matrix, 0xC);
BEGIN_LAYOUT_CHECK(CDoughnut)
	CHECK_FIELD(CDoughnut, m_pParticle, 0xD4);
END_LAYOUT_CHECK

// Explosion mark left where the ball lands.
class ntExplodeMark {
	LAYOUT_CHECK_FRIEND;
public:
	ntExplodeMark(void);
	~ntExplodeMark(void);
	void Init(const WVector &pos);
	void Uninit(void);
	void Process(void);
	void Display(void);

private:
	bool m_bActive;      // 0x0
	int m_hTexture;      // 0x4 ("[_boom.jpg" of g_resrcmng)
	WVector m_vPos;      // 0x8
};
CHECK_SIZE(ntExplodeMark, 0x14);
BEGIN_LAYOUT_CHECK(ntExplodeMark)
	CHECK_FIELD(ntExplodeMark, m_bActive, 0x0);
	CHECK_FIELD(ntExplodeMark, m_hTexture, 0x4);
	CHECK_FIELD(ntExplodeMark, m_vPos, 0x8);
END_LAYOUT_CHECK

#endif
