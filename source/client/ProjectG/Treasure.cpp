// Original: d:\build\custom\temp\code\client\projectg\treasure.cpp

#include <algorithm>
#include "Treasure.h"

// treasure.cpp:14
CTMapInfo::CTMapInfo() : m_TMapIndex(0), m_TMark(2), m_Gauge(0) {
}

// treasure.cpp:19
CTMapInfo::CTMapInfo(const CTMapInfo& other) {
	m_TMapIndex = other.m_TMapIndex;
	m_TMark = other.m_TMark;
	m_Gauge = other.m_Gauge;
}

// treasure.cpp:26
CTMapInfo::~CTMapInfo() {
}

// treasure.cpp:31
unsigned char CTMapInfo::GetTMapIndex() const {
	return m_TMapIndex;
}

// treasure.cpp:36
void CTMapInfo::SetTMapIndex(unsigned char index) {
	m_TMapIndex = index;
}

// treasure.cpp:41
unsigned char CTMapInfo::GetTMark() const {
	return m_TMark;
}

// treasure.cpp:46
void CTMapInfo::SetTMark(unsigned char mark) {
	m_TMark = mark;
}

// treasure.cpp:51
unsigned long CTMapInfo::GetGauge() const {
	return m_Gauge;
}

// treasure.cpp:56
void CTMapInfo::SetGauge(unsigned long gauge) {
	m_Gauge = gauge;
}

// treasure.cpp:61
CTMapInfo& CTMapInfo::operator=(const CTMapInfo& other) {
	m_TMapIndex = other.m_TMapIndex;
	m_TMark = other.m_TMark;
	m_Gauge = other.m_Gauge;

	return *this;
}

// treasure.cpp:78
CTHunter::CTHunter() {
	m_TreasurePoint = 0;
	m_bReturnLobby = false;
	m_bReceiveGiftPacket = false;
	m_usedTikiReport = false;
	m_TreasureMapSize = 24;
}

// treasure.cpp:83
CTHunter::~CTHunter() {
	std::vector<CTMapInfo*>::iterator it;
	for (it = m_TMapList.begin(); it != m_TMapList.end(); ++it) {
		if (*it) {
			delete *it;
			*it = 0;
		}
	}
	m_TMapList.~vector();
}

// treasure.cpp:93
void CTHunter::Initialize(int count) {
	for (int i = 0; i < count; i++) {
		AddTreasure(i, 0);
	}

	m_GiftList.reserve(m_TreasureMapSize);
}

// treasure.cpp:108
void CTHunter::AddTreasure(unsigned char index, unsigned long gauge) {
	CTMapInfo* info = new CTMapInfo;

	info->SetTMapIndex(index);
	info->SetGauge(gauge);
	m_TMapList.push_back(info);
}

// treasure.cpp:118
bool CTHunter::DeleteTreasure(unsigned char index) {
	std::vector<CTMapInfo*>::iterator it = m_TMapList.begin();
	while (it != m_TMapList.end()) {
		if (index == (*it)->GetTMapIndex()) {
			delete *it;
			m_TMapList.erase(it);
			return true;
		}
		++it;
	}

	return false;
}

// treasure.cpp:138
unsigned char CTHunter::FindTreasure(unsigned char index) {
	std::vector<CTMapInfo*>::iterator it = std::find_if(m_TMapList.begin(), m_TMapList.end(), THunterFinder(index));

	return (*it)->GetTMapIndex();
}

// treasure.cpp:145
void CTHunter::SetTHunterGauge(unsigned char index, unsigned long gauge) {
	if (m_TMapList.size() > index) {
		m_TMapList[index]->SetGauge(gauge);
	}
}

// treasure.cpp:156
unsigned long CTHunter::GetTHunterGauge(eMapType type) {
	CMapTypeAdapter::eMapIndex index = CMapTypeAdapter::GetMapIndex(type);
	if (index == -1) {
		return 9999;
	}

	return m_TMapList[index]->GetGauge();
}

// treasure.cpp:165
unsigned char CTHunter::GetTHunterMark(eMapType type) {
	CMapTypeAdapter::eMapIndex index = CMapTypeAdapter::GetMapIndex(type);
	if (index == -1) {
		return 2;
	}

	return m_TMapList[index]->GetTMark();
}

// treasure.cpp:175
unsigned char CTHunter::GetTreasureMapSize() {
	return (unsigned char)m_TMapList.size();
}

// treasure.cpp:181
void CTHunter::SetTreasurePoint(unsigned long point) {
	if (point >= 1000) {
		m_TreasurePoint = 1000;
	} else {
		m_TreasurePoint = point;
	}
}

// treasure.cpp:192
unsigned long CTHunter::GetTreasurePoint() const {
	return m_TreasurePoint;
}

// treasure.cpp:197
void CTHunter::ResetTGift() {
	m_GiftList.~vector();
}

// treasure.cpp:202
void CTHunter::DecideTMark() {
	unsigned long gauge = m_TMapList[0]->GetGauge();
	int count = m_TMapList.size();
	{
		std::vector<unsigned char> maxList;

		for (int i = 1; i < count; i++) {
			unsigned long current = m_TMapList[i]->GetGauge();
			if ((current > gauge ? current : gauge) != 0) {
				gauge = current;
			}
		}

		for (int j = 0; j < count; j++) {
			if (m_TMapList[j]->GetGauge() == gauge) {
				maxList.push_back(j);
			}
		}

		for (unsigned int k = 0; k < maxList.size(); k++) {
			m_TMapList[maxList[k]]->SetTMark(1);
		}
	}

	gauge = m_TMapList[0]->GetGauge();
	{
		std::vector<unsigned char> minList;

		for (int i = 1; i < count; i++) {
			unsigned long current = m_TMapList[i]->GetGauge();
			if ((current < gauge ? current : gauge) != 0) {
				gauge = current;
			}
		}

		for (int j = 0; j < count; j++) {
			if (m_TMapList[j]->GetGauge() == gauge) {
				minList.push_back(j);
			}
		}

		for (unsigned int k = 0; k < minList.size(); k++) {
			m_TMapList[minList[k]]->SetTMark(0);
		}
	}
}

// treasure.cpp:254
void CTHunter::ReceiveAllGift(std::vector<sTreasureGift>& gifts) {
	int count = m_GiftList.size();
	if (count != 0) {
		m_GiftList.~vector();
	}

	m_GiftList.swap(gifts);
}

// treasure.cpp:403
void CTHunter::ReceiveRepository(std::vector<sTreasureHunt>& repository) {
	int count = m_Repository.size();
	if (count != 0) {
		m_Repository.~vector();
	}

	m_Repository.swap(repository);
}

// treasure.cpp:413
sTreasureHunt CTHunter::GetGiftInfo(unsigned char index) {
	return m_Repository[index];
}
