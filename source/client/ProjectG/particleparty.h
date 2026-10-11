// Original: d:\build\custom\temp\code\client\projectg\particleparty.h
#ifndef PG_CLIENT_PROJECTG_PARTICLEPARTY_H
#define PG_CLIENT_PROJECTG_PARTICLEPARTY_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"
#include "../client/Wangreal/include/w3danispr.h"
#include "../client/Wangreal/include/w3dspr.h"

class IActor;
class CPetBody;

// One of the four 0x28-byte buffers a CParticle points into (a vertex:
// texture coordinates at 0x14/0x18, set by CPowerShot).
struct sParticleBuffer {
	WVector m_vPos;     // 0x0
	PADDING(0xC, 0x10);     // members not transcribed yet
	unsigned long m_nColor;     // 0x10, ARGB
	float m_fU;     // 0x14
	float m_fV;     // 0x18
	PADDING(0x1C, 0x28);     // members not transcribed yet
};
CHECK_SIZE(sParticleBuffer, 0x28);
CHECK_FIELD(sParticleBuffer, m_vPos, 0x0);
CHECK_FIELD(sParticleBuffer, m_nColor, 0x10);
CHECK_FIELD(sParticleBuffer, m_fU, 0x14);
CHECK_FIELD(sParticleBuffer, m_fV, 0x18);

class CParticle {
	LAYOUT_CHECK_FRIEND;

public:
	CParticle(void);     // particleparty.cpp:21
	virtual ~CParticle(void);     // particleparty.cpp:30
	virtual void Process(float fElapsed) = 0;
	virtual void Display(void) = 0;
	virtual void DisplayShadow(void);     // particleparty.h:16
	virtual void MakeParticle(void) = 0;
	int GetTexHandle(void) { return m_texHandle; }     // particleparty.h:19
	void LoadTexture(int handle) { m_texHandle = handle; }     // particleparty.h:20
	virtual void SetActive(bool bActive) { m_bActive = bActive; }     // particleparty.h:21
	void SetCenter(WVector const& vCenter);     // particleparty.h:22
	bool IsActive(void) { return m_bActive; }     // particleparty.h:23
	virtual void SetNormal(WVector const& vNormal);     // particleparty.h:25

protected:
	virtual void Generate(void);     // particleparty.h:28
	virtual void BlowAway(float fPower, int nIndex);     // particleparty.h:29

protected:
	int m_nCount;     // 0x4, number of live elements (CRipple::Init)

public:
	WVector m_vCenter;     // 0x8, CRipple writes its sub-particle's

protected:
	int m_texHandle;     // 0x14, WResourceManager texture handle
	bool m_bActive;     // 0x18
	W3dAniSpr* m_pAniSpr;     // 0x1C
	W3dSpr* m_pSpr;     // 0x20

protected:
	float m_fGenTime;     // 0x24, accumulated time, compared with m_fRate
	float m_fRate;     // 0x28, set to 0.01 by CStarDust

private:
	PADDING(0x2C, 0x30);     // members not transcribed yet

protected:
	sParticleBuffer* m_pBuffer0;     // 0x30
	sParticleBuffer* m_pBuffer1;     // 0x34
	sParticleBuffer* m_pBuffer2;     // 0x38
	sParticleBuffer* m_pBuffer3;     // 0x3C
	int m_nBufferState;     // 0x40
	sParticleBuffer m_buffers[4];     // 0x44 (m_pBuffer2 -> [3], m_pBuffer3 -> [2])
};
CHECK_SIZE(CParticle, 0xE4);
BEGIN_LAYOUT_CHECK(CParticle)
	CHECK_FIELD(CParticle, m_nCount, 0x4);
	CHECK_FIELD(CParticle, m_vCenter, 0x8);
	CHECK_FIELD(CParticle, m_texHandle, 0x14);
	CHECK_FIELD(CParticle, m_fRate, 0x28);
	CHECK_FIELD(CParticle, m_bActive, 0x18);
	CHECK_FIELD(CParticle, m_pAniSpr, 0x1C);
	CHECK_FIELD(CParticle, m_pSpr, 0x20);
	CHECK_FIELD(CParticle, m_pBuffer0, 0x30);
	CHECK_FIELD(CParticle, m_pBuffer3, 0x3C);
	CHECK_FIELD(CParticle, m_nBufferState, 0x40);
	CHECK_FIELD(CParticle, m_buffers, 0x44);
END_LAYOUT_CHECK

class CShotParticle : public CParticle {
public:
	struct sPiece {
		bool m_bAlive;     // 0x0
		WVector m_vPos;     // 0x4
		WVector m_vVelocity;     // 0x10
		float m_fLife;     // 0x1C, seconds left
		float m_fLength;     // 0x20, length of the drawn streak
		float m_fSlowTime;     // 0x24, below this life the piece slows down
	};

	CShotParticle(void);     // particleparty.cpp:41
	virtual ~CShotParticle(void);     // particleparty.cpp:47
	virtual void Process(float fElapsed);
	virtual void Display(void);
	virtual void MakeParticle(void);

private:
	sPiece* m_pPieces;     // 0xE4
	int m_nPieces;     // 0xE8
};
CHECK_SIZE(CShotParticle::sPiece, 0x28);
CHECK_FIELD(CShotParticle::sPiece, m_vPos, 0x4);
CHECK_FIELD(CShotParticle::sPiece, m_fSlowTime, 0x24);

class CDust : public CParticle {
public:
	struct sParticle {
		WVector m_vPos;     // 0x0
		float m_fAngle;     // 0xC
		float m_fStartSize;     // 0x10
		float m_fEndSize;     // 0x14
	};

	CDust(int nCount);     // particleparty.cpp:798
	virtual ~CDust(void);     // particleparty.cpp:804
	virtual void Process(float fElapsed);
	virtual void Display(void);
	virtual void MakeParticle(void);

private:
	sParticle* m_pParticles;     // 0xE4
	int m_nParticles;     // 0xE8
	float m_fDuration;     // 0xEC
	float m_fRemain;     // 0xF0, 1 -> 0 over m_fDuration
};
CHECK_SIZE(CDust::sParticle, 0x18);
CHECK_FIELD(CDust::sParticle, m_fEndSize, 0x14);

class CGrassParticle : public CParticle {
public:
	struct sGrass {
		int m_nFrame;     // 0x0, sprite number passed to Render
		float m_fAlpha;     // 0x4
		float m_fAngle;     // 0x8
		float m_fAngularSpeed;     // 0xC, added to m_fAngle per second
		WVector m_vPos;     // 0x10
		WVector m_vVelocity;     // 0x1C
		bool m_bAlive;     // 0x28
		PADDING(0x29, 0x2C);
		float m_fFadeTime;     // 0x2C, seconds m_fAlpha takes from 255 to 0
	};

	CGrassParticle(int nCount, WVector const& vCenter);     // particleparty.cpp:870
	virtual ~CGrassParticle(void);     // particleparty.cpp:877
	virtual void Process(float fElapsed);
	virtual void Display(void);
	virtual void MakeParticle(void);

private:
	sGrass* m_pGrass;     // 0xE4
	int m_nGrass;     // 0xE8
	WVector m_vOrigin;     // 0xEC
};
CHECK_SIZE(CGrassParticle::sGrass, 0x30);
CHECK_FIELD(CGrassParticle::sGrass, m_fAlpha, 0x4);
CHECK_FIELD(CGrassParticle::sGrass, m_vPos, 0x10);
CHECK_FIELD(CGrassParticle::sGrass, m_bAlive, 0x28);
CHECK_FIELD(CGrassParticle::sGrass, m_vVelocity, 0x1C);
CHECK_FIELD(CGrassParticle::sGrass, m_fFadeTime, 0x2C);

class CBunkerShotParticle : public CParticle {
public:
	struct sSmoke {
		WVector m_vPos;     // 0x0
		WVector m_vVelocity;     // 0xC
	};

	struct sParticle {
		bool m_bAlive;     // 0x0
		WVector m_vPos;     // 0x4
		WVector m_vVelocity;     // 0x10
		float m_fLength;     // 0x1C, length of the drawn streak
		float m_fLife;     // 0x20, seconds left
		float m_fMaxLife;     // 0x24, the streak shortens below this
	};

	CBunkerShotParticle(int nSmoke, int nParticle);     // particleparty.cpp:970
	virtual ~CBunkerShotParticle(void);     // particleparty.cpp:986
	virtual void MakeParticle(void);
	virtual void Process(float fElapsed);
	virtual void Display(void);

protected:
	virtual void Generate(void);

private:
	sSmoke* m_pSmokes;     // 0xE4
	sParticle* m_pParticles;     // 0xE8
	int m_nSmokes;     // 0xEC
	int m_nParticles;     // 0xF0
	float m_fDuration;     // 0xF4
	float m_fRemain;     // 0xF8, 1 -> 0 over m_fDuration
	unsigned long m_dwColor;     // 0xFC
};
CHECK_SIZE(CBunkerShotParticle::sSmoke, 0x18);
CHECK_SIZE(CBunkerShotParticle::sParticle, 0x28);
CHECK_FIELD(CBunkerShotParticle::sParticle, m_vPos, 0x4);
CHECK_FIELD(CBunkerShotParticle::sParticle, m_vVelocity, 0x10);
CHECK_FIELD(CBunkerShotParticle::sParticle, m_fLength, 0x1C);
CHECK_FIELD(CBunkerShotParticle::sParticle, m_fLife, 0x20);
CHECK_FIELD(CBunkerShotParticle::sParticle, m_fMaxLife, 0x24);

class CPowerShot : public CParticle {
	LAYOUT_CHECK_FRIEND;

public:
	CPowerShot(void);     // particleparty.cpp:128
	virtual ~CPowerShot(void);     // particleparty.cpp:142
	virtual void Process(float fElapsed);     // particleparty.cpp:157
	virtual void Display(void);
	virtual void MakeParticle(void);     // particleparty.cpp:146

private:
	float m_fSize;     // 0xE4
	float m_fWidth;     // 0xE8
	int m_nSegments;     // 0xEC
	int m_nAlpha;     // 0xF0, 255 -> 0 over m_fLife
	float m_fLife;     // 0xF4
	float m_fTime;     // 0xF8
	float m_fSpeed;     // 0xFC
	float m_fDistance;     // 0x100
};
BEGIN_LAYOUT_CHECK(CPowerShot)
	CHECK_FIELD(CPowerShot, m_fSize, 0xE4);
	CHECK_FIELD(CPowerShot, m_nSegments, 0xEC);
	CHECK_FIELD(CPowerShot, m_nAlpha, 0xF0);
	CHECK_FIELD(CPowerShot, m_fLife, 0xF4);
	CHECK_FIELD(CPowerShot, m_fDistance, 0x100);
END_LAYOUT_CHECK

class CLightColumn : public CParticle {
	LAYOUT_CHECK_FRIEND;

public:
	CLightColumn(void);     // particleparty.cpp:456
	virtual ~CLightColumn(void);     // particleparty.cpp:462
	virtual void Process(float fElapsed);
	virtual void Display(void);
	virtual void MakeParticle(void);     // particleparty.cpp:671
	void FadeIn(void);
	void FadeOut(void);

private:
	int m_nFade;     // 0xE4
	float m_fHeight;     // 0xE8
	float m_fDistance;     // 0xEC, camera distance to the cup, scaled, at least the ball's radius
	unsigned char m_byAlpha;     // 0xF0
	unsigned char m_byTargetAlpha;     // 0xF1
	PADDING(0xF2, 0xF4);
	int m_hEffectTex[5];     // 0xF4, hole_ef_nomal_10..14
	int m_hScreenTex;     // 0x108, hole_scr01
	float m_fFrame;     // 0x10C, decreased by 2 per second
};
BEGIN_LAYOUT_CHECK(CLightColumn)
	CHECK_FIELD(CLightColumn, m_nFade, 0xE4);
	CHECK_FIELD(CLightColumn, m_fHeight, 0xE8);
	CHECK_FIELD(CLightColumn, m_byAlpha, 0xF0);
	CHECK_FIELD(CLightColumn, m_byTargetAlpha, 0xF1);
	CHECK_FIELD(CLightColumn, m_hEffectTex, 0xF4);
	CHECK_FIELD(CLightColumn, m_hScreenTex, 0x108);
	CHECK_FIELD(CLightColumn, m_fFrame, 0x10C);
	CHECK_FIELD(CLightColumn, m_fDistance, 0xEC);
END_LAYOUT_CHECK

class CBunkerParticle : public CParticle {
	LAYOUT_CHECK_FRIEND;

public:
	CBunkerParticle(void);     // particleparty.cpp:1126
	virtual ~CBunkerParticle(void);     // particleparty.cpp:1130
	virtual void Process(float fElapsed);     // particleparty.cpp:1134
	virtual void Display(void);
	virtual void MakeParticle(void);

private:
	int m_nAlpha;     // 0xE4
	float m_fRemain;     // 0xE8
	PADDING(0xEC, 0xF0);     // members not transcribed yet
	float m_fSize;     // 0xF0
	float m_fDuration;     // 0xF4
};
BEGIN_LAYOUT_CHECK(CBunkerParticle)
	CHECK_FIELD(CBunkerParticle, m_nAlpha, 0xE4);
	CHECK_FIELD(CBunkerParticle, m_fSize, 0xF0);
	CHECK_FIELD(CBunkerParticle, m_fRemain, 0xE8);
	CHECK_FIELD(CBunkerParticle, m_fDuration, 0xF4);
END_LAYOUT_CHECK

class CFragment : public CParticle {
	LAYOUT_CHECK_FRIEND;

public:
	struct sFrag {
		WVector m_vPos;     // 0x0
		WVector m_vVelocity;     // 0xC
	};

	CFragment(void);     // particleparty.cpp:1180
	virtual ~CFragment(void);     // particleparty.cpp:1186
	virtual void Process(float fElapsed);
	virtual void Display(void);
	virtual void MakeParticle(void);
	virtual void SetNormal(WVector const& vNormal);

private:
	sFrag* m_pFrags;     // 0xE4
	int m_nFrags;     // 0xE8
	PADDING(0xEC, 0xF4);     // members not transcribed yet
	float m_fAlpha;     // 0xF4
	WVector m_vNormal;     // 0xF8, surface normal the fragments fly away from
};
CHECK_SIZE(CFragment::sFrag, 0x18);
BEGIN_LAYOUT_CHECK(CFragment)
	CHECK_FIELD(CFragment, m_pFrags, 0xE4);
	CHECK_FIELD(CFragment, m_nFrags, 0xE8);
	CHECK_FIELD(CFragment, m_fAlpha, 0xF4);
	CHECK_FIELD(CFragment, m_vNormal, 0xF8);
END_LAYOUT_CHECK

class CRipple : public CParticle {
	LAYOUT_CHECK_FRIEND;

public:
	struct sRipple {
		float m_fRadius;     // 0x0
		float m_fPrevX;     // 0x4
		float m_fX;     // 0x8
		float m_fZ;     // 0xC
		float m_fSize;     // 0x10
		bool m_bAlive;     // 0x14
		float m_fDelay;     // 0x18
		float m_fWait;     // 0x1C
	};

	CRipple(void);
	virtual ~CRipple(void);     // particleparty.cpp:1531
	void Init(void);     // particleparty.cpp:1537
	virtual void Process(float fElapsed);
	virtual void Display(void);
	virtual void MakeParticle(void);     // particleparty.cpp:1550

protected:
	void AddRipple(float fSize, float fDelay, float fX, float fZ);     // particleparty.cpp:1563

private:
	sRipple* m_pRipples;     // 0xE4
	int m_nRipples;     // 0xE8
	CParticle* m_pSubParticle;     // 0xEC, centred on the ripple and made with it
};
CHECK_SIZE(CRipple::sRipple, 0x20);
BEGIN_LAYOUT_CHECK(CRipple)
	CHECK_FIELD(CRipple, m_pRipples, 0xE4);
	CHECK_FIELD(CRipple, m_nRipples, 0xE8);
	CHECK_FIELD(CRipple, m_pSubParticle, 0xEC);
END_LAYOUT_CHECK

class CStarDust : public CParticle {
	LAYOUT_CHECK_FRIEND;

public:
	struct sParticle {
		bool m_bAlive;     // 0x0
		float m_fRadius;     // 0x4
		WVector m_vPos;     // 0x8
		WVector m_vAxisX;     // 0x14, cos(m_fAngle) axis of the orbit
		WVector m_vAxisY;     // 0x20, sin(m_fAngle) axis of the orbit
		PADDING(0x2C, 0x38);     // members not transcribed yet
		WVector m_vOrigin;     // 0x38
		float m_fAngle;     // 0x44
		float m_fAngularSpeed;     // 0x48
		float m_fLife;     // 0x4C
	};

	CStarDust(void);     // particleparty.cpp:2068
	virtual ~CStarDust(void);     // particleparty.cpp:2075
	virtual void Process(float fElapsed);
	virtual void Display(void);
	virtual void MakeParticle(void);

protected:
	virtual void Generate(void);
	virtual void BlowAway(float fPower, int nIndex);

private:
	sParticle* m_pParticles;     // 0xE4
	int m_nParticles;     // 0xE8
	int m_nGenerated;     // 0xEC, next particle Generate fills
	float m_fFade;     // 0xF0, decreased by Process
	float m_fAlpha;     // 0xF4
};
CHECK_SIZE(CStarDust::sParticle, 0x50);
CHECK_FIELD(CStarDust::sParticle, m_vPos, 0x8);
CHECK_FIELD(CStarDust::sParticle, m_vOrigin, 0x38);
CHECK_FIELD(CStarDust::sParticle, m_fLife, 0x4C);
BEGIN_LAYOUT_CHECK(CStarDust)
	CHECK_FIELD(CStarDust, m_pParticles, 0xE4);
	CHECK_FIELD(CStarDust, m_nParticles, 0xE8);
END_LAYOUT_CHECK

class CEnergyCenter : public CParticle {
	LAYOUT_CHECK_FRIEND;

public:
	struct sSpark {
		bool m_bAlive;     // 0x0
		WVector m_vPos;     // 0x4
		WVector m_vDir;     // 0x10, unit vector towards m_vOrigin
		float m_fAlpha;     // 0x1C
	};

	CEnergyCenter(int nCount, WVector const& vCenter, float fRadius);
	virtual ~CEnergyCenter(void);     // particleparty.cpp:1910
	virtual void Process(float fElapsed);
	virtual void Display(void);
	virtual void MakeParticle(void);     // particleparty.cpp:1916

protected:
	void Generate(float fElapsed);

private:
	sSpark* m_pSparks;     // 0xE4
	float m_fRadius;     // 0xE8
	int m_nGenerated;     // 0xEC
	int m_nSparks;     // 0xF0
	WVector m_vOrigin;     // 0xF4
	float m_fLife;     // 0x100, 1 at construction, decreased by Process
	float m_fTime;     // 0x104
	float m_fAlpha;     // 0x108
	float m_fMaxAlpha;     // 0x10C
	IActor* m_pGolfBg;     // 0x110, the current task's "GolfBg" actor
};
CHECK_SIZE(CEnergyCenter::sSpark, 0x20);
CHECK_FIELD(CEnergyCenter::sSpark, m_vPos, 0x4);
CHECK_FIELD(CEnergyCenter::sSpark, m_vDir, 0x10);
CHECK_FIELD(CEnergyCenter::sSpark, m_fAlpha, 0x1C);
BEGIN_LAYOUT_CHECK(CEnergyCenter)
	CHECK_FIELD(CEnergyCenter, m_fRadius, 0xE8);
	CHECK_FIELD(CEnergyCenter, m_vOrigin, 0xF4);
	CHECK_FIELD(CEnergyCenter, m_fMaxAlpha, 0x10C);
	CHECK_FIELD(CEnergyCenter, m_pGolfBg, 0x110);
	CHECK_FIELD(CEnergyCenter, m_pSparks, 0xE4);
	CHECK_FIELD(CEnergyCenter, m_nGenerated, 0xEC);
	CHECK_FIELD(CEnergyCenter, m_nSparks, 0xF0);
END_LAYOUT_CHECK

class CStarDust2 : public CParticle {
	LAYOUT_CHECK_FRIEND;

public:
	struct sParticle {
		bool m_bAlive;     // 0x0
		float m_fRadius;     // 0x4
		WVector m_vPos;     // 0x8
		WVector m_vAxisX;     // 0x14
		WVector m_vAxisY;     // 0x20
		PADDING(0x2C, 0x38);     // members not transcribed yet
		WVector m_vOrigin;     // 0x38
		float m_fAngle;     // 0x44
		float m_fAngularSpeed;     // 0x48
		float m_fLife;     // 0x4C, also the sprite's alpha
	};

	CStarDust2(void);     // particleparty.cpp:2237
	virtual ~CStarDust2(void);     // particleparty.cpp:2244
	virtual void Process(float fElapsed);
	virtual void Display(void);
	virtual void MakeParticle(void);

protected:
	virtual void Generate(void);

private:
	sParticle* m_pParticles;     // 0xE4
	int m_nParticles;     // 0xE8
	int m_nGenerated;     // 0xEC
	float m_fFade;     // 0xF0
	float m_fAlpha;     // 0xF4
};
CHECK_SIZE(CStarDust2::sParticle, 0x50);
CHECK_FIELD(CStarDust2::sParticle, m_vPos, 0x8);
CHECK_FIELD(CStarDust2::sParticle, m_fLife, 0x4C);
BEGIN_LAYOUT_CHECK(CStarDust2)
	CHECK_FIELD(CStarDust2, m_pParticles, 0xE4);
	CHECK_FIELD(CStarDust2, m_nParticles, 0xE8);
END_LAYOUT_CHECK

class CDroplet : public CParticle {
	LAYOUT_CHECK_FRIEND;

public:
	struct sDroplet {
		WVector m_vPos;     // 0x0
		WVector m_vVelocity;     // 0xC
		float m_fSpeed;     // 0x18, random 10..40 (MakeParticle)
	};

	CDroplet(void);     // particleparty.cpp:1368
	virtual ~CDroplet(void);     // particleparty.cpp:1379
	virtual void Process(float fElapsed);
	virtual void Display(void);     // particleparty.cpp:1415
	virtual void MakeParticle(void);

private:
	sDroplet* m_pDroplets;     // 0xE4
	int m_nDroplets;     // 0xE8
	float m_fDuration;     // 0xEC
	float m_fAlpha;     // 0xF0, 255 -> 0 over m_fDuration
};
CHECK_SIZE(CDroplet::sDroplet, 0x1C);
BEGIN_LAYOUT_CHECK(CDroplet)
	CHECK_FIELD(CDroplet, m_pDroplets, 0xE4);
	CHECK_FIELD(CDroplet, m_nDroplets, 0xE8);
	CHECK_FIELD(CDroplet, m_fAlpha, 0xF0);
END_LAYOUT_CHECK

// Total size not established yet: the members after CParticle (0xE4..0x10C at
// least) are not transcribed, so there is no CHECK_SIZE.
class CCoin : public CParticle {
	LAYOUT_CHECK_FRIEND;

public:
	struct sCoin {
		sCoin(void) : m_pBody(NULL) {}     // particleparty.h:519

		CPetBody* m_pBody;     // 0x0
		PADDING(0x4, 0x18);     // members not transcribed yet
	};

	struct sSpark {
		sSpark(void) {}

		PADDING(0x0, 0x4);     // members not transcribed yet
		WVector m_vPos;     // 0x4
		PADDING(0x10, 0x1C);     // members not transcribed yet
		float m_fAlpha;     // 0x1C
		PADDING(0x20, 0x28);     // members not transcribed yet
	};

	CCoin(int nCount, WVector const& vCenter);     // particleparty.cpp:1649
	virtual ~CCoin(void);     // particleparty.cpp:1660
	virtual void MakeParticle(void);     // particleparty.cpp:1673
	virtual void Process(float fElapsed);
	virtual void Display(void);     // particleparty.cpp:1855
	virtual void DisplayShadow(void);     // particleparty.cpp:1876
	virtual void SetActive(bool bActive);     // particleparty.cpp:1757

protected:
	bool ShouldDisplay(void);     // particleparty.cpp:1752
	virtual void Generate(void);

private:
	sCoin* m_pCoins;     // 0xE4
	sSpark* m_pSparks;     // 0xE8
	int m_nCoins;     // 0xEC
	int m_nSparks;     // 0xF0
	WVector m_vOrigin;     // 0xF4
	float m_fDuration;     // 0x100, 3 seconds (MakeParticle)
	float m_fRemain;     // 0x104, 1 -> 0 over m_fDuration
	int m_nDropSounds;     // 0x108, "pang_coin_drop" plays at most 4 times
};
CHECK_SIZE(CCoin::sCoin, 0x18);
CHECK_SIZE(CCoin::sSpark, 0x28);
CHECK_FIELD(CCoin::sSpark, m_vPos, 0x4);
CHECK_FIELD(CCoin::sSpark, m_fAlpha, 0x1C);
BEGIN_LAYOUT_CHECK(CCoin)
	CHECK_FIELD(CCoin, m_pCoins, 0xE4);
	CHECK_FIELD(CCoin, m_pSparks, 0xE8);
	CHECK_FIELD(CCoin, m_nCoins, 0xEC);
	CHECK_FIELD(CCoin, m_nSparks, 0xF0);
	CHECK_FIELD(CCoin, m_vOrigin, 0xF4);
	CHECK_FIELD(CCoin, m_fDuration, 0x100);
	CHECK_FIELD(CCoin, m_fRemain, 0x104);
	CHECK_FIELD(CCoin, m_nDropSounds, 0x108);
END_LAYOUT_CHECK

#endif
