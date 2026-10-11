// Original: d:\build\custom\temp\code\client\projectg\cardsystem.h
#ifndef PG_CLIENT_PROJECTG_CARDSYSTEM_H
#define PG_CLIENT_PROJECTG_CARDSYSTEM_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "../shared/globalgamedefine.h"

#include <list>

class FrSPCardBuffDlg;

// One card ability slot of a player (CCardManager keeps 13 per player).
// Only the value is read so far.
struct sCardAvility {
	int kind;      // 0x0, role not identified yet (cleared by Init)
	int value;     // 0x4
	int extra;     // 0x8, role not identified yet (cleared by Init)
};
CHECK_SIZE(sCardAvility, 0xC);

// A stack of cards the player owns (0xE bytes: std::list's node constructor
// copies three dwords and a word). CSharedDoc keeps them in a std::list
// (GetCardInfoToUID / GetCardInfoToTID). The original header of this type is
// not known yet; only the members used so far are named.
#pragma pack(push, 1)
struct sCardStack {
	unsigned long uid;      // 0x0
	unsigned long typeId;   // 0x4
	UNKNOWN(0x8, 0xC);
	unsigned char valid;    // 0xC, the lookups only accept a stack with 1 here
	UNKNOWN(0xD, 0xE);
};
#pragma pack(pop)
CHECK_SIZE(sCardStack, 0xE);

// Card collection state. Only the members this project's code uses so far
// are declared.
class CCardManager : public WSingleton<CCardManager> {
	LAYOUT_CHECK_FRIEND;

public:
	CCardManager(void);                          // cardsystem.cpp:23
	virtual ~CCardManager(void);                 // cardsystem.cpp:35

	void Init(void);                             // cardsystem.cpp:40
	void CalcCardPeriodAndStatus(void);          // cardsystem.cpp:80
	int GetTotalRange(bool withPowerRange);      // cardsystem.cpp:276
	float GetTotalPangyaZone(void);              // cardsystem.cpp:290
	int GetPowerRangeDown(void);                 // cardsystem.cpp:304
	int GetCardPeriodStatus(int slot);           // cardsystem.cpp:310
	int GetCardStatusSlot(int slot);             // cardsystem.cpp:319
	float GetCardPeriodComboGauge(void);         // cardsystem.cpp:328
	int GetCardPeriodSlot(void);                 // cardsystem.cpp:333
	float GetCardPeriodPangRate(void);           // cardsystem.cpp:338
	int GetCardSuccesssProbUp(void);             // cardsystem.cpp:343
	float GetCardRangeUp(void);                  // cardsystem.cpp:349
	__int64 GetCardBounceBonusUp(void);          // cardsystem.cpp:355
	float GetCardPowerRangeUp(void);             // cardsystem.cpp:361
	int GetCardComboGaugeUp(void);               // cardsystem.cpp:367
	float GetCardPangyaZoneUp(void);             // cardsystem.cpp:373
	unsigned char GetWindPowerDown(void);        // cardsystem.cpp:379
	float GetSpecialCardPangyaZoneUp(void);      // cardsystem.cpp:386
	float GetSpecialClearBonusProb(unsigned char type);   // cardsystem.cpp:390
	float GetCaddieCardPangYaZoneUpWind1(void);  // cardsystem.cpp:400
	void UpdateCardBuffDlg(void);                // cardsystem.cpp:407
	void InitCardBuffDlgPosition(void);          // cardsystem.cpp:415
	void ResetSpecialBuffTick(void);             // cardsystem.cpp:421
	void SetPlayerIndex(unsigned char index);    // cardsystem.cpp:427
	void SetOffLinePlayerIndex(unsigned char index);   // cardsystem.cpp:443
	int SetSpecialCardAvility(const sSCardAvilityPeriodInfo& info, int index);
	void SetCharacterCardAvility(const sSCardAvilityPeriodInfo& info, int index);
	void SetCaddieCardAvility(const sSCardAvilityPeriodInfo& info, int index);

	unsigned char GetWindColor(void);            // cardsystem.h:63
	void SetCardBuffDlg(FrSPCardBuffDlg *dlg);   // cardsystem.h:106
	void SetSPCardAlarm(bool alarm);             // cardsystem.h:107
	bool ShowSPCardAlarm(void);                  // cardsystem.h:108
	std::list<unsigned long> &GetActiveCardList(void);   // cardsystem.h:115

protected:
	virtual void OnProc(float dt);               // cardsystem.cpp:74

private:
	unsigned char m_playerIndexMap[5];           // 0x28, slot -> player index (SetPlayerIndex)
	sCharacterInfo m_characterInfo[5];           // 0x2D, the character of each player (SetCharacterCardAvility)
	PADDING(0x8D9, 0x8DC);
	int m_cardPeriodStatus[5][5];                // 0x8DC, [player][slot]
	int m_cardPeriodSlot[5];                     // 0x940
	float m_cardPeriodComboGauge[5];             // 0x954
	float m_cardPeriodPangRate[5];               // 0x968
	float m_specialCardPangyaZoneUp[5];          // 0x97C
	float m_specialClearBonusProb[5];            // 0x990
	unsigned char m_specialClearBonusType;       // 0x9A4, 20 = none
	PADDING(0x9A5, 0x9A8);
	int m_cardStatusSlot[5][5];                  // 0x9A8, [player][slot]
	int m_powerRangeDown[5];                     // 0xA0C
	unsigned char m_windColor;                   // 0xA20
	PADDING(0xA21, 0xA24);
	sCardAvility m_cardAvility[5][13];           // 0xA24, [player][ability]
	UNKNOWN(0xD30, 0xD34);
	unsigned char m_playerIndex;                 // 0xD34
	PADDING(0xD35, 0xD38);
	std::list<unsigned long> m_activeCardList;   // 0xD38
	FrSPCardBuffDlg *m_cardBuffDlg;    // 0xD44
	bool m_spCardAlarm;                // 0xD48
};
BEGIN_LAYOUT_CHECK(CCardManager)
	CHECK_FIELD(CCardManager, m_playerIndexMap, 0x28);
	CHECK_FIELD(CCardManager, m_characterInfo, 0x2D);
	CHECK_FIELD(CCardManager, m_cardPeriodStatus, 0x8DC);
	CHECK_FIELD(CCardManager, m_cardPeriodSlot, 0x940);
	CHECK_FIELD(CCardManager, m_cardPeriodComboGauge, 0x954);
	CHECK_FIELD(CCardManager, m_cardPeriodPangRate, 0x968);
	CHECK_FIELD(CCardManager, m_specialCardPangyaZoneUp, 0x97C);
	CHECK_FIELD(CCardManager, m_specialClearBonusProb, 0x990);
	CHECK_FIELD(CCardManager, m_specialClearBonusType, 0x9A4);
	CHECK_FIELD(CCardManager, m_cardStatusSlot, 0x9A8);
	CHECK_FIELD(CCardManager, m_powerRangeDown, 0xA0C);
	CHECK_FIELD(CCardManager, m_cardAvility, 0xA24);
	CHECK_FIELD(CCardManager, m_playerIndex, 0xD34);
	CHECK_FIELD(CCardManager, m_activeCardList, 0xD38);
	CHECK_FIELD(CCardManager, m_windColor, 0xA20);
	CHECK_FIELD(CCardManager, m_cardBuffDlg, 0xD44);
	CHECK_FIELD(CCardManager, m_spCardAlarm, 0xD48);
END_LAYOUT_CHECK

// TaskMain.cpp defines TASKMAIN_CARDSYSTEM_INLINE empty before its includes:
// the original holds the out-of-line copy of these inline accessors in
// TaskMain's object; every other unit inlines them.
#ifndef TASKMAIN_CARDSYSTEM_INLINE
#define TASKMAIN_CARDSYSTEM_INLINE inline
#endif

// cardsystem.h:106
TASKMAIN_CARDSYSTEM_INLINE void CCardManager::SetCardBuffDlg(FrSPCardBuffDlg *dlg) {
	m_cardBuffDlg = dlg;
}

// cardsystem.h:107
TASKMAIN_CARDSYSTEM_INLINE void CCardManager::SetSPCardAlarm(bool alarm) {
	m_spCardAlarm = alarm;
}

// cardsystem.h:108
TASKMAIN_CARDSYSTEM_INLINE bool CCardManager::ShowSPCardAlarm(void) {
	return m_spCardAlarm;
}

#endif // PG_CLIENT_PROJECTG_CARDSYSTEM_H
