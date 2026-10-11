// Original: d:\build\custom\temp\code\client\projectg\powergauge.cpp
#include "PowerGauge.h"

#include <algorithm>

#include "GolfDoc.h"
#include "SharedDoc.h"
#include "cardsystem.h"
#include "club.h"
#include "wind.h"
#include "../shared/Localize.h"
#include "../shared/classdefine.h"

namespace {

// Which part slot holds the ears of each character (indexed by the
// character's typeid without its type bits).
struct sEarPartsPos {
	unsigned char slot;
};

sEarPartsPos aEarParts[] = {
	{ 0x11 }, { 0x11 }, { 0x13 }, { 0x11 }, { 0x14 },
	{ 0x15 }, { 0x10 }, { 0x13 }, { 0x11 }, { 0x11 },
};

// The zone sizes the equipment adds (.rdata 0xa06fa0).
const float kSmallArea = 1.0f;
const float kBaseArea = 2.0f;
const float kAirNightLuckyArea = 5.0f;
const float kHalfArea = 0.5f;

// Which part slot holds the wings of each character (.rdata 0xa06fb0).
const int kWingSlot[10] = {
	0xB, 0xC, 0xC, 0xE, 0xC, 0x10, 0x8, 0x11, 0xA, 0xA,
};

// The wing parts of each character that widen the zone (.rdata 0xa06fd8),
// -1 for an unused entry.
const unsigned long kWingParts[10][6] = {
	{ 0x8016805, 0x8016806, 0x8016803, 0x8016802, 0xFFFFFFFF, 0xFFFFFFFF },
	{ 0x8058805, 0x8058806, 0x8058803, 0x8058807, 0x8058808, 0x8058802 },
	{ 0x8098805, 0x8098806, 0x8098803, 0x8098802, 0xFFFFFFFF, 0xFFFFFFFF },
	{ 0x80DC805, 0x80DC806, 0x80DC803, 0x80DC807, 0x80DC808, 0x80DC802 },
	{ 0x8118805, 0x8118806, 0x8118803, 0x8118802, 0xFFFFFFFF, 0xFFFFFFFF },
	{ 0x8160805, 0x8160806, 0x8160803, 0x8160807, 0x8160808, 0x8160802 },
	{ 0x8190805, 0x8190806, 0x8190803, 0x8190807, 0x8190808, 0x8190802 },
	{ 0x81E2805, 0x81E2806, 0x81E2807, 0x81E2802, 0xFFFFFFFF, 0xFFFFFFFF },
	{ 0x8214805, 0x8214806, 0x8214807, 0x8214808, 0x8214809, 0x8214802 },
	{ 0x8254802, 0x8254803, 0x8254804, 0x8254805, 0x8254806, 0x8254807 },
};

// The local player's room entry.
inline sUserInfo &MyRoomPlayer(void) {
	return WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex];
}

} // namespace

// powergauge.cpp:102
bool CPowerGauge::Init(void) {
	m_luckyPangYaItems.push_back(0x1800000A);
	m_luckyPangYaItems.push_back(0x18000007);
	m_luckyPangYaItems.push_back(0x18000002);
	m_luckyPangYaItems.push_back(0x18000012);

	return true;
}

// powergauge.cpp:128
CPowerGauge::CPowerGauge(void) {
	m_debugPangYaArea = 1.0f;
	m_debugAirNightLuckyPangYaArea = 0.0f;
	Init();
}

// powergauge.cpp:136
CPowerGauge::~CPowerGauge(void) {
	m_luckyPangYaItems.clear();
}

// powergauge.cpp:141
void CPowerGauge::SetDebugPangYaArea(float area) {
	m_debugPangYaArea = area;
}

// powergauge.cpp:146
void CPowerGauge::SetDebugAirNightLuckyPangYaArea(float area) {
	m_debugAirNightLuckyPangYaArea = area;
}

// powergauge.cpp:151
float CPowerGauge::GetPangYaArea(void) {
	float area = GetAddLuckyPangYaArea() + kBaseArea;
	area += GetAddElfEarArea();
	area += GetAddWingArea();
	area += GetAddAirnightLuckyArea();

	float cardArea = GetAddCardArea();

	area = GetLuckyNecklaceArea() + (cardArea + area);

	area += GetAddAuxPartArea();

	return area;
}

// powergauge.cpp:195
float CPowerGauge::GetAddLuckyPangYaArea(void) {
	unsigned long typeId = PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId;
	std::list<unsigned long>::iterator it = std::find(m_luckyPangYaItems.begin(), m_luckyPangYaItems.end(), typeId);
	if (it != m_luckyPangYaItems.end()) {
		return kBaseArea;
	}

	return 0.0f;
}

// powergauge.cpp:206
float CPowerGauge::GetAddElfEarArea(void) {
	if (!IsLocalContent((localContentType_t)0x33)) {
		return 0.0f;
	}

	sCharacterInfo *pInfo = &MyRoomPlayer().m_characterInfo;

	unsigned long character = pInfo->typeId & 0x3FFFFFF;

	unsigned long partTypeId = pInfo->partTypeIds[aEarParts[character].slot];
	if (partTypeId == 0) {
		return 0.0f;
	}

	switch (partTypeId & 0x1FF) {
	case 0: {
		float intensity = Wind().GetGlobalIntensity();
		if (intensity < 4.0f) {
			break;
		} else if (intensity >= 4.0f && intensity <= 7.0f) {
			return kSmallArea;
		} else {
			return kBaseArea;
		}
	}
	case 1:
	case 2:
	case 3:
	case 4:
	case 7:
	case 12: {
		float intensity = Wind().GetGlobalIntensity();
		if (intensity >= 7.0f) {
			return kSmallArea;
		}
		break;
	}
	}

	return 0.0f;
}

// powergauge.cpp:323
float CPowerGauge::GetAddAirnightLuckyArea(void) {
	if (GolfClub().GetType() != 3) {
		if (MyRoomPlayer().m_clubSetTypeId == 0x10000012) {
			return kAirNightLuckyArea;
		}
	}

	return 0.0f;
}

// powergauge.cpp:348
float CPowerGauge::GetAddWingArea(void) {
	float area = 0.0f;

	sCharacterInfo *pInfo = &MyRoomPlayer().m_characterInfo;

	unsigned long character = pInfo->typeId & 0x3FFFFFF;
	unsigned long partTypeId = pInfo->partTypeIds[kWingSlot[character]];

	for (int i = 0; i < 6; i++) {
		if (partTypeId == kWingParts[character][i]) {
			return kHalfArea;
		}
	}

	return area;
}

// powergauge.cpp:371
float CPowerGauge::GetAddCardArea(void) {
	if (GolfClub().GetType() == 3) {
		return 0.0f;
	}

	float zone = WSingleton<CCardManager>::Instance()->GetTotalPangyaZone();
	return zone;
}

// powergauge.cpp:383
float CPowerGauge::GetLuckyNecklaceArea(void) {
	IFF_STRUCT::sItem *pItem = ItemManager()->FindItem(0x1A000006);
	unsigned char level = MyRoomPlayer().m_level;
	if (pItem && level <= (pItem->m_level & 0x7F)) {
		if (level < 11) {
			return kSmallArea;
		}
	}

	return 0.0f;
}

// powergauge.cpp:478
bool CPowerGauge::IsEquipSpecialAuxPart(unsigned long typeId) {
	sCharacterInfo *pInfo = &MyRoomPlayer().m_characterInfo;

	return pInfo->auxParts.tids[(typeId >> 21) & 0x1F] == typeId;
}

// powergauge.cpp:488
float CPowerGauge::GetAddAuxPartArea(void) {
	float area = 0.0f;
	if (IsEquipSpecialAuxPart(0x70010012)) {
		float intensity = Wind().GetGlobalIntensity();
		if (intensity > 0.0f && intensity < 4.0f) {
			return kHalfArea;
		}
	}

	return area;
}
