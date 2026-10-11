// Original: d:\build\custom\temp\code\client\projectg\caddiebonus.cpp
#include "CaddieBonus.h"
#include "BonusTicket.h"
#include "GolfDoc.h"
#include "SharedDoc.h"

// caddiebonus.cpp:12
CCaddieBonus::CCaddieBonus(void) {
	RegisterBonus(CADDIE_W_BIGBONGDARI, "W_BIGBONGDARI");
	RegisterBonus(CADDIE_R_BIGBONGDARI, "R_BIGBONGDARI");
}

// caddiebonus.cpp:22
void CCaddieBonus::SetTicket(void) {
	switch (WSingleton<CSharedDoc>::Instance()->m_roomType) {
	case 4:
	case 5:
	case 6:
	case 9:
	case 10:
		CBonusTicket::Instance().SetTicketFromClient("W_BIGBONGDARI", 4, 50, 30, 15, 5);
		CBonusTicket::Instance().SetTicketFromClient("R_BIGBONGDARI", 4, 49, 20, 30, 1);
		break;
	}
}

// caddiebonus.cpp:38
__int64 CCaddieBonus::GetBonusPang(eCaddie caddie, __int64 pang) {
	if (!IsHaveCaddie(caddie)) {
		return 0;
	}

	__int64 bonus = 0;
	std::map<int, std::string>::iterator it = m_bonusTables.find(caddie);
	if (it == m_bonusTables.end()) {
		return 0;
	}

	int ticket = CBonusTicket::Instance().GetRandomTicket(it->second);
	switch (caddie) {
	case CADDIE_W_BIGBONGDARI:
		bonus = GetWBigPontaBonus(ticket);
		break;
	case CADDIE_R_BIGBONGDARI:
		bonus = GetRBigPontaBonus(ticket, pang);
		break;
	}
	return bonus;
}

// caddiebonus.cpp:72
CCaddieBonus::~CCaddieBonus(void) {
	Release();
}

// caddiebonus.cpp:81
bool CCaddieBonus::IsHaveCaddie(eCaddie caddie) {
	return (caddie | 0x1C000000) == WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_caddieTypeId ? true : false;
}

// caddiebonus.cpp:93
void CCaddieBonus::Release(void) {
	std::map<int, std::string>::iterator it = m_bonusTables.begin();
	while (it != m_bonusTables.end()) {
		std::string name = it->second;
		if (name != "") {
			CBonusTicket::Instance().DeleteTable(it->second);
		}
		++it;
	}
	m_bonusTables.clear();
}

// caddiebonus.cpp:106
void CCaddieBonus::RegisterBonus(int caddie, std::string name) {
	m_bonusTables[caddie] = name;
}

// caddiebonus.cpp:117
__int64 CCaddieBonus::GetWBigPontaBonus(int ticket) {
	__int64 bonus = 0;
	switch (ticket) {
	case 0: bonus = 1; break;
	case 1: bonus = 5; break;
	case 2: bonus = 15; break;
	case 3: bonus = 50; break;
	}
	return bonus;
}

// caddiebonus.cpp:135
__int64 CCaddieBonus::GetRBigPontaBonus(int ticket, __int64 pang) {
	__int64 bonus = 0;
	switch (ticket) {
	case 0: bonus = pang * 1 / 100; break;
	case 1: bonus = pang * 10 / 100; break;
	case 2: bonus = pang * 20 / 100; break;
	case 3: bonus = pang * 50 / 100; break;
	}
	if (bonus < 1) {
		bonus = 1;
	}
	return bonus;
}
