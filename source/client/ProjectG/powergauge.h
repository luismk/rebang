// Original: d:\build\custom\temp\code\client\projectg\powergauge.h
#ifndef PG_CLIENT_PROJECTG_POWERGAUGE_H
#define PG_CLIENT_PROJECTG_POWERGAUGE_H

#include <list>
#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"

// The size of the PangYa zone of the power gauge: a base plus what the local
// player's equipment, cards and the wind add (GetPangYaArea).
class CPowerGauge : public WSingleton<CPowerGauge> {
	LAYOUT_CHECK_FRIEND;
public:
	CPowerGauge(void);
	virtual ~CPowerGauge(void);

	void SetDebugPangYaArea(float area);
	void SetDebugAirNightLuckyPangYaArea(float area);
	float GetPangYaArea(void);

private:
	bool Init(void);
	float GetAddLuckyPangYaArea(void);
	float GetAddElfEarArea(void);
	float GetAddAirnightLuckyArea(void);
	float GetAddWingArea(void);
	float GetAddCardArea(void);
	float GetLuckyNecklaceArea(void);
	bool IsEquipSpecialAuxPart(unsigned long typeId);
	float GetAddAuxPartArea(void);

	std::list<unsigned long> m_luckyPangYaItems;   // 0x28, item typeids that widen the zone (filled by Init)
	float m_debugPangYaArea;                        // 0x34
	float m_debugAirNightLuckyPangYaArea;           // 0x38
};
CHECK_SIZE(CPowerGauge, 0x3C);
BEGIN_LAYOUT_CHECK(CPowerGauge)
	CHECK_FIELD(CPowerGauge, m_luckyPangYaItems, 0x28);
	CHECK_FIELD(CPowerGauge, m_debugPangYaArea, 0x34);
	CHECK_FIELD(CPowerGauge, m_debugAirNightLuckyPangYaArea, 0x38);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_POWERGAUGE_H
