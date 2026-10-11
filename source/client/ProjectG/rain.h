// Original: d:\build\custom\temp\code\client\projectg\rain.h
#ifndef PG_CLIENT_PROJECTG_RAIN_H
#define PG_CLIENT_PROJECTG_RAIN_H

#include "..\..\defines.h"
#include "weather.h"
#include "wmath.h"

// Rain drops: 500 fragments scattered in m_area around the camera, falling
// along m_wind.
class CRain : public CWeatherType {
	LAYOUT_CHECK_FRIEND;
public:
	struct sFrag {
		WVector pos;            // 0x0
		bool visible;           // 0xc
	};

	CRain(void);
	virtual ~CRain(void);

	virtual void Process(float fDelta);
	virtual void Display(void);
	virtual void DisplayFullScreenOverlay(void);
	virtual void UpdateWind(void);

private:
	WVector m_area;             // 0x8
	float m_height;             // 0x14
	WVector m_wind;             // 0x18
	sFrag* m_frags;             // 0x24
};
CHECK_SIZE(CRain::sFrag, 0x10);
CHECK_FIELD(CRain::sFrag, pos, 0x0);
CHECK_FIELD(CRain::sFrag, visible, 0xc);
CHECK_SIZE(CRain, 0x28);
BEGIN_LAYOUT_CHECK(CRain)
	CHECK_FIELD(CRain, m_area, 0x8);
	CHECK_FIELD(CRain, m_height, 0x14);
	CHECK_FIELD(CRain, m_wind, 0x18);
	CHECK_FIELD(CRain, m_frags, 0x24);
END_LAYOUT_CHECK

#endif
