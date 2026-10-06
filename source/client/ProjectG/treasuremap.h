#pragma once

class CTMapInfo
{
public:
	CTMapInfo();
	CTMapInfo(const CTMapInfo& rhs);
	~CTMapInfo();

	CTMapInfo& operator=(const CTMapInfo& rhs);

	unsigned char GetTMapIndex() const;
	void SetTMapIndex(unsigned char index);
	unsigned char GetTMark() const;
	void SetTMark(unsigned char mark);
	unsigned long GetGauge() const;
	void SetGauge(unsigned long gauge);

	unsigned char m_TMapIndex;
	unsigned char m_TMark;
	unsigned long m_gauge;
};

class THunterFinder
{
public:
	THunterFinder(unsigned char index)
		: m_index(index)
	{
	}

	bool operator()(const CTMapInfo* pInfo) const
	{
		return pInfo->m_TMapIndex == m_index;
	}

	unsigned char m_index;
};
