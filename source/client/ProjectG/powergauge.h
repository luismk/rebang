#pragma once

#include <list>

namespace
{
	struct sEarPartsPos
	{
		unsigned char pos;
	};
}

class CPowerGauge : public WSingleton<CPowerGauge>
{
public:
	CPowerGauge();
	virtual ~CPowerGauge();

	void SetDebugPangYaArea(float area);
	void SetDebugAirNightLuckyPangYaArea(float area);
	float GetPangYaArea();

private:
	bool Init();
	float GetAddLuckyPangYaArea();
	float GetAddElfEarArea();
	float GetAddAirnightLuckyArea();
	float GetAddWingArea();
	float GetAddCardArea();
	float GetLuckyNecklaceArea();
	bool IsEquipSpecialAuxPart(unsigned long typeId);
	float GetAddAuxPartArea();

	std::list<unsigned long> m_luckyItemList;
	float m_debugPangYaArea;
	float m_debugAirNightLuckyPangYaArea;
};
