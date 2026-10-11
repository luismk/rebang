// Original: d:\build\custom\temp\code\client\projectg\treasure.h
#ifndef PG_CLIENT_PROJECTG_TREASURE_H
#define PG_CLIENT_PROJECTG_TREASURE_H

#include <vector>
#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "../shared/globalgamedefine.h"

// sTreasureHunt and sTreasureGift belong in shared/globalgamedefine.h (line
// info: globalgamedefine.h:479-536); kept here until that header can take
// them.

// One item of the treasure repository, 11 bytes (packed).
// ReAdjustRepository merges items by m_typeId and sums their m_count.
#pragma pack(push, 1)
struct sTreasureHunt {
	// globalgamedefine.h:479
	sTreasureHunt() : m_index(0), m_typeId(0), m_count(0), m_flag(0) {}

	// globalgamedefine.h:481
	sTreasureHunt(const sTreasureHunt& other) {
		m_index = other.m_index;
		m_typeId = other.m_typeId;
		m_count = other.m_count;
		m_flag = other.m_flag;
	}

	// globalgamedefine.h:489
	void operator=(const sTreasureHunt& other) {
		m_index = other.m_index;
		m_typeId = other.m_typeId;
		m_count = other.m_count;
		m_flag = other.m_flag;
	}

	unsigned long m_index;  // 0x0
	unsigned long m_typeId; // 0x4
	unsigned short m_count; // 0x8
	unsigned char m_flag;   // 0xa
};
#pragma pack(pop)
CHECK_SIZE(sTreasureHunt, 0xb);
CHECK_FIELD(sTreasureHunt, m_index, 0x0);
CHECK_FIELD(sTreasureHunt, m_typeId, 0x4);
CHECK_FIELD(sTreasureHunt, m_count, 0x8);
CHECK_FIELD(sTreasureHunt, m_flag, 0xa);

// One treasure gift, 21 bytes (packed). Member meanings not identified yet.
#pragma pack(push, 1)
struct sTreasureGift {
	// globalgamedefine.h:515
	sTreasureGift() { Reset(); }

	// globalgamedefine.h:518
	void operator=(const sTreasureGift& other) {
		m_value0 = other.m_value0;
		m_value4 = other.m_value4;
		m_value8 = other.m_value8;
		m_valueC = other.m_valueC;
		m_valueE = other.m_valueE;
		m_valueF = other.m_valueF;
		m_value13 = other.m_value13;
	}

	// globalgamedefine.h:528
	void Reset() {
		m_value0 = 0;
		m_value4 = 0;
		m_value8 = 0;
		m_valueC = 0;
		m_valueE = 0;
		m_valueF = 0;
		m_value13 = 0;
	}

	unsigned long m_value0;   // 0x0
	unsigned long m_value4;   // 0x4
	unsigned long m_value8;   // 0x8
	unsigned short m_valueC;  // 0xc
	unsigned char m_valueE;   // 0xe
	unsigned long m_valueF;   // 0xf
	unsigned short m_value13; // 0x13
};
#pragma pack(pop)
CHECK_SIZE(sTreasureGift, 0x15);

class CTMapInfo {
public:
	CTMapInfo();
	CTMapInfo(const CTMapInfo&);        // 0x602b00
	~CTMapInfo();
	unsigned char GetTMapIndex() const; // 0x601da0
	void SetTMapIndex(unsigned char);   // 0x601db0
	unsigned char GetTMark() const;     // 0x601dc0
	void SetTMark(unsigned char);       // 0x601dd0
	unsigned long GetGauge() const;     // 0x601de0
	void SetGauge(unsigned long);       // 0x601df0
	CTMapInfo& operator=(const CTMapInfo&); // 0x601e00

	unsigned char m_TMapIndex; // 0x0
	unsigned char m_TMark;     // 0x1
	PADDING(0x2, 0x4);
	unsigned long m_Gauge;     // 0x4
};
CHECK_SIZE(CTMapInfo, 0x8);
CHECK_FIELD(CTMapInfo, m_TMapIndex, 0x0);
CHECK_FIELD(CTMapInfo, m_TMark, 0x1);
CHECK_FIELD(CTMapInfo, m_Gauge, 0x4);

// Predicate matching a treasure map by its index (std::find_if).
struct THunterFinder {
	// treasure.h:45
	THunterFinder(unsigned char index) : m_index(index) {}

	// treasure.h:48
	bool operator()(const CTMapInfo* info) const {
		return info->m_TMapIndex == m_index;
	}

	unsigned char m_index; // 0x0
};
CHECK_SIZE(THunterFinder, 0x1);
CHECK_FIELD(THunterFinder, m_index, 0x0);

// The virtual destructor's vfptr is in the BaseObject padding of WSingleton.
class CTHunter : public WSingleton<CTHunter> {
public:
	CTHunter();                             // 0x605a50
	virtual ~CTHunter();                    // 0x605ac0
	void Initialize(int);                   // 0x606260
	void AddTreasure(unsigned char, unsigned long); // 0x605b70
	unsigned char FindTreasure(unsigned char);      // 0x603c70
	bool DeleteTreasure(unsigned char);             // 0x6048d0
	void SetTHunterGauge(unsigned char, unsigned long); // 0x603cb0
	unsigned long GetTHunterGauge(eMapType);        // 0x603cf0
	unsigned char GetTHunterMark(eMapType);         // 0x603d20
	unsigned char GetTreasureMapSize();             // 0x602b20
	unsigned char GetGiftSize() { return (unsigned char)m_GiftList.size(); }   // treasure.h:95
	void SetTreasurePoint(unsigned long);   // 0x601e40
	unsigned long GetTreasurePoint() const; // 0x601e60
	bool IsReturnLobby();                   // treasure.h:97
	void SetReturnLobby(bool);              // treasure.h:98
	bool IsReceiveGiftPacket();             // treasure.h:100
	void SetReceiveGiftPacket(bool);        // treasure.h:101
	bool IsUsedTikiReport();                // treasure.h:103
	void SetUsedTikiReport(bool);           // treasure.h:104
	void ResetTGift();                      // 0x605c00
	void DecideTMark();                     // 0x605c30
	sTreasureHunt GetGiftInfo(unsigned char); // 0x603d50
	void ReceiveRepository(std::vector<sTreasureHunt>&); // 0x6062a0
	void ReceiveAllGift(std::vector<sTreasureGift>&);    // 0x606430

protected:
	std::vector<CTMapInfo*> m_TMapList;       // 0x28
	std::vector<sTreasureHunt> m_Repository;  // 0x38
	std::vector<sTreasureGift> m_GiftList;    // 0x48
	unsigned long m_TreasurePoint; // 0x58
	unsigned char m_TreasureMapSize; // 0x5c
	bool m_bReturnLobby;           // 0x5d
	bool m_bReceiveGiftPacket;     // 0x5e
	bool m_usedTikiReport;         // 0x5f

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(CTHunter, 0x60);
BEGIN_LAYOUT_CHECK(CTHunter)
	CHECK_FIELD(CTHunter, m_TMapList, 0x28);
	CHECK_FIELD(CTHunter, m_Repository, 0x38);
	CHECK_FIELD(CTHunter, m_GiftList, 0x48);
	CHECK_FIELD(CTHunter, m_TreasurePoint, 0x58);
	CHECK_FIELD(CTHunter, m_TreasureMapSize, 0x5c);
	CHECK_FIELD(CTHunter, m_bReturnLobby, 0x5d);
	CHECK_FIELD(CTHunter, m_bReceiveGiftPacket, 0x5e);
	CHECK_FIELD(CTHunter, m_usedTikiReport, 0x5f);
END_LAYOUT_CHECK

#endif
