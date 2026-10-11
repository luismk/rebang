// Original: d:\build\custom\temp\code\client\projectg\snow.h
#ifndef PG_CLIENT_PROJECTG_SNOW_H
#define PG_CLIENT_PROJECTG_SNOW_H

#include "weather.h"

// Falling snow flakes: m_count sprites of m_spr scattered in m_area around
// the camera.
class CSnow : public CWeatherType {
	LAYOUT_CHECK_FRIEND;
public:
	struct sFrag {
		WVector pos;            // 0x0, rendered position
		WVector center;         // 0xc
		int spriteIndex;        // 0x18
		WVector offset;         // 0x1c
		float yaw;              // 0x28, random 0..2pi
		float pitch;            // 0x2c, random 0..pi
		bool visible;           // 0x30
	};

	CSnow(int count, const char* name, int spriteIndex, int width, int height);
	virtual ~CSnow(void);

	virtual void Process(float fDelta);
	virtual void Display(void);
	virtual void DisplayFullScreenOverlay(void);
	virtual void Reset(int nType, float fValue);

	void TurnOffAdditiveBlend(void);

private:
	float m_size;               // 0x8, Reset's fValue
	Waabb m_area;               // 0xc
	sFrag* m_frags;             // 0x24
	W3dAniSpr* m_spr;           // 0x28
	int m_count;                // 0x2c
	int m_spriteIndex;          // 0x30, < 0 picks a random sprite per flake
	bool m_additiveBlendOff;    // 0x34, Display renders without additive blending
};
CHECK_SIZE(CSnow::sFrag, 0x34);
CHECK_FIELD(CSnow::sFrag, pos, 0x0);
CHECK_FIELD(CSnow::sFrag, center, 0xc);
CHECK_FIELD(CSnow::sFrag, spriteIndex, 0x18);
CHECK_FIELD(CSnow::sFrag, offset, 0x1c);
CHECK_FIELD(CSnow::sFrag, yaw, 0x28);
CHECK_FIELD(CSnow::sFrag, pitch, 0x2c);
CHECK_FIELD(CSnow::sFrag, visible, 0x30);
CHECK_SIZE(CSnow, 0x38);
BEGIN_LAYOUT_CHECK(CSnow)
	CHECK_FIELD(CSnow, m_size, 0x8);
	CHECK_FIELD(CSnow, m_area, 0xc);
	CHECK_FIELD(CSnow, m_frags, 0x24);
	CHECK_FIELD(CSnow, m_spr, 0x28);
	CHECK_FIELD(CSnow, m_count, 0x2c);
	CHECK_FIELD(CSnow, m_spriteIndex, 0x30);
	CHECK_FIELD(CSnow, m_additiveBlendOff, 0x34);
END_LAYOUT_CHECK

#endif
