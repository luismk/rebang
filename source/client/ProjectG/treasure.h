#pragma once
#include "singleton.h"
#include "../../shared/globalgamedefine.h"
#include <vector>

class CTHunter : public WSingleton<CTHunter>
{
public:
	virtual ~CTHunter();
	sTreasureHunt GetGiftInfo(unsigned char index);
	void ReAdjustRepository();
	unsigned char GetRepositorySize()
	{
		return (unsigned char)m_vecRepository.size();
	}

private:
	unsigned char m_unused28[0x10];
	std::vector<sTreasureHunt> m_vecRepository;
};
