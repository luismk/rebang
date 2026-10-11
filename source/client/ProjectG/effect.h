// Original: d:\build\custom\temp\code\client\projectg\effect.h
#ifndef PG_CLIENT_PROJECTG_EFFECT_H
#define PG_CLIENT_PROJECTG_EFFECT_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"

// The golf caddie's ring effect: two concentric rings of m_count points
// around m_center, drawn as textured quads (effect.cpp).
class CRing {
	LAYOUT_CHECK_FRIEND;

public:
	CRing(void);
	CRing(const char* texname);
	~CRing(void);
	void SetActive(bool active) { m_bActive = active; } // effect.h:16
	void Init(void);
	void SetCenter(const WVector& center);
	void Display(void);
	void Process(float elapsed);

private:
	UNKNOWN(0x0, 0x4);
	int m_count;            // 0x4, points per ring
	WVector* m_pOuter;      // 0x8, new[m_count]
	WVector* m_pInner;      // 0xC, new[m_count]
	WVector m_center;       // 0x10
	float m_radius;         // 0x1C
	int m_hTexture;         // 0x20, g_resrcmng texture handle
	float m_angle;          // 0x24, rotation, advanced by Process
	float m_width;          // 0x28, band width of a ring
	float m_innerRatio;     // 0x2C, inner ring radius / width scale
	bool m_bActive;         // 0x30
};
CHECK_SIZE(CRing, 0x34);
BEGIN_LAYOUT_CHECK(CRing)
CHECK_FIELD(CRing, m_count, 0x4);
CHECK_FIELD(CRing, m_pOuter, 0x8);
CHECK_FIELD(CRing, m_pInner, 0xC);
CHECK_FIELD(CRing, m_center, 0x10);
CHECK_FIELD(CRing, m_radius, 0x1C);
CHECK_FIELD(CRing, m_hTexture, 0x20);
CHECK_FIELD(CRing, m_angle, 0x24);
CHECK_FIELD(CRing, m_width, 0x28);
CHECK_FIELD(CRing, m_innerRatio, 0x2C);
CHECK_FIELD(CRing, m_bActive, 0x30);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_EFFECT_H
