// Original: d:\build\custom\temp\code\client\projectg\weather.h
#ifndef PG_CLIENT_PROJECTG_WEATHER_H
#define PG_CLIENT_PROJECTG_WEATHER_H

#include "..\..\defines.h"
#include "wmath.h"
class W3dAniSpr;

class CWeatherType {
public:
	CWeatherType(void) { m_gravity = 1.0f; }
	virtual ~CWeatherType(void) {}

	virtual void Process(float fDelta) = 0;
	virtual void Display(void) = 0;
	virtual void DisplayFullScreenOverlay(void) {}
	virtual void UpdateWind(void) {}
	virtual void Reset(int nType, float fValue) {}

	void SetGravity(float fGravity) { m_gravity = fGravity; }

protected:
	LAYOUT_CHECK_FRIEND;
	float m_gravity; // 0x4
};
CHECK_SIZE(CWeatherType, 0x8);
BEGIN_LAYOUT_CHECK(CWeatherType)
	CHECK_FIELD(CWeatherType, m_gravity, 0x4);
END_LAYOUT_CHECK

class CCloud : public CWeatherType {
public:
	CCloud(void);
	virtual ~CCloud(void);

	virtual void Process(float fDelta);
	virtual void Display(void);
	virtual void DisplayFullScreenOverlay(void);
};
CHECK_SIZE(CCloud, 0x8);

// Falling ice or snow flakes: m_count sprites of m_spr scattered in m_area
// around the camera.
class ntIceSnow : public CWeatherType {
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

	ntIceSnow(int count, const char* name, int spriteIndex);
	virtual ~ntIceSnow(void);

	virtual void Process(float fDelta);
	virtual void Display(void);
	virtual void DisplayFullScreenOverlay(void);
	virtual void Reset(int nType, float fValue);

private:
	float m_size;               // 0x8, Reset's fValue
	Waabb m_area;               // 0xc
	sFrag* m_frags;             // 0x24
	W3dAniSpr* m_spr;           // 0x28
	int m_count;                // 0x2c
	int m_spriteIndex;          // 0x30, < 0 picks a random sprite per flake
};
CHECK_SIZE(ntIceSnow::sFrag, 0x34);
CHECK_FIELD(ntIceSnow::sFrag, pos, 0x0);
CHECK_FIELD(ntIceSnow::sFrag, center, 0xc);
CHECK_FIELD(ntIceSnow::sFrag, spriteIndex, 0x18);
CHECK_FIELD(ntIceSnow::sFrag, offset, 0x1c);
CHECK_FIELD(ntIceSnow::sFrag, yaw, 0x28);
CHECK_FIELD(ntIceSnow::sFrag, pitch, 0x2c);
CHECK_FIELD(ntIceSnow::sFrag, visible, 0x30);
CHECK_SIZE(ntIceSnow, 0x34);
BEGIN_LAYOUT_CHECK(ntIceSnow)
	CHECK_FIELD(ntIceSnow, m_size, 0x8);
	CHECK_FIELD(ntIceSnow, m_area, 0xc);
	CHECK_FIELD(ntIceSnow, m_frags, 0x24);
	CHECK_FIELD(ntIceSnow, m_spr, 0x28);
	CHECK_FIELD(ntIceSnow, m_count, 0x2c);
	CHECK_FIELD(ntIceSnow, m_spriteIndex, 0x30);
END_LAYOUT_CHECK

#endif
