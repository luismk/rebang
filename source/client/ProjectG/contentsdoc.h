// Original: d:\build\custom\temp\code\client\projectg\contentsdoc.h
#ifndef PG_CLIENT_PROJECTG_CONTENTSDOC_H
#define PG_CLIENT_PROJECTG_CONTENTSDOC_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "icontentsdoc.h"
#include "../shared/globalgamedefine.h"

#include <windows.h>

#include <list>
#include <string>
#include <map>
#include <vector>

#include <string.h>

class WReceivedPacket;

namespace FieldItem {
	// One item a player picked up on the course (0x10 bytes from the vector
	// growth; layout not known yet).
	struct AcquiredItemInfo {
		UNKNOWN(0x0, 0x10);
	};

	// Texture kind a course item is placed on (names invented).
	enum eDisposeTextureType {
		DISPOSE_TEXTURE_UNKNOWN = 0,
		DISPOSE_TEXTURE_GREEN_B = 1,    // a "ct_green_b" texture
		DISPOSE_TEXTURE_BOOSTER = 2     // "ct_booster.dds"
	};
}

CHECK_SIZE(FieldItem::AcquiredItemInfo, 0x10);

// Kind of a course item (names invented from the script keys
// CGimmickContainer::LoadFromScript reads the counts with).
enum GIMMICK_ITEM_TYPE {
	GIMMICK_ITEM_ANY = -1,          // matches every kind
	GIMMICK_ITEM_PANG_COIN = 0,     // "gimmick_pang_count"
	GIMMICK_ITEM_RANDOM_BOX = 1     // "gimmick_randombox_count"
};

// Placement of one course item on a hole (0x14 bytes, read raw from the
// packet by CGimmickContainer::LoadFromPacket).
struct GimmickDispositionInformation {
	// State of the item (values not known yet).
	enum eFlag {
		FLAG_NONE = 0,
		FLAG_ANY = 2    // matches every state in a search (name invented)
	};

	GimmickDispositionInformation(void) {
		itemType = GIMMICK_ITEM_ANY;
		value4 = 0;
		flag = FLAG_NONE;
		value0C = 20;
		hole = 0;
	}

	GIMMICK_ITEM_TYPE itemType;     // 0x0
	int value4;                     // 0x4
	eFlag flag;                     // 0x8
	int value0C;                    // 0xC: 20 by default
	unsigned char hole;             // 0x10: the hole it is placed on
};

BEGIN_LAYOUT_CHECK(GimmickDispositionInformation)
	CHECK_SIZE(GimmickDispositionInformation, 0x14);
	CHECK_FIELD(GimmickDispositionInformation, itemType, 0x0);
	CHECK_FIELD(GimmickDispositionInformation, flag, 0x8);
	CHECK_FIELD(GimmickDispositionInformation, hole, 0x10);
END_LAYOUT_CHECK

// The container of the course items ("gimmicks") placed on the holes.
class CGimmickContainer : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	virtual bool Initialize(void);
	virtual bool Release(void);

	// contentsdoc.h:507
	int GetRandomSeed(void) { return m_randomSeed; }
	// contentsdoc.h:509
	float GetMinDistBtwItems(void) const { return m_minDistBtwItems; }
	// contentsdoc.h:516
	std::vector<FieldItem::AcquiredItemInfo> &GetAcquiredItemInfoArray(void) { return m_acquiredItems; }

	std::vector<GimmickDispositionInformation> &GetGimmickPosition(unsigned char hole);
	void ResetGimmikFlag(unsigned char hole, GimmickDispositionInformation::eFlag flag);
	GimmickDispositionInformation *FindDisposeInfo(unsigned char hole, GIMMICK_ITEM_TYPE itemType, GimmickDispositionInformation::eFlag flag);
	int GetNumDisposeInfo(unsigned char hole, GIMMICK_ITEM_TYPE itemType, GimmickDispositionInformation::eFlag flag);
	void LoadFromPacket(WReceivedPacket &packet);
	bool LoadFromScript(const char *fileName);
	bool NeedToMakeTriPtArray(const char *textureName, FieldItem::eDisposeTextureType *textureType) const;
	void AddItemAcquired(const FieldItem::AcquiredItemInfo &info);

protected:
	void AddGimmickPosition(unsigned char hole, const std::vector<GimmickDispositionInformation> &positions);

private:
	std::map<unsigned char, std::vector<GimmickDispositionInformation> > m_gimmickPositions;  // 0x4: per hole
	int m_randomSeed;                                   // 0x10
	float m_minDistBtwItems;                            // 0x14
	std::vector<FieldItem::AcquiredItemInfo> m_acquiredItems;   // 0x18
	std::string m_magicCarpetBoundingSeq;               // 0x28
	std::string m_pangCoinCrashSeq;                     // 0x44
	std::string m_randomBoxCrashSeq;                    // 0x60
	std::string m_seq7C;                                // 0x7C: not set by Initialize
};

BEGIN_LAYOUT_CHECK(CGimmickContainer)
	CHECK_SIZE(CGimmickContainer, 0x98);
	CHECK_FIELD(CGimmickContainer, m_gimmickPositions, 0x4);
	CHECK_FIELD(CGimmickContainer, m_randomSeed, 0x10);
	CHECK_FIELD(CGimmickContainer, m_minDistBtwItems, 0x14);
	CHECK_FIELD(CGimmickContainer, m_acquiredItems, 0x18);
	CHECK_FIELD(CGimmickContainer, m_magicCarpetBoundingSeq, 0x28);
	CHECK_FIELD(CGimmickContainer, m_pangCoinCrashSeq, 0x44);
	CHECK_FIELD(CGimmickContainer, m_randomBoxCrashSeq, 0x60);
	CHECK_FIELD(CGimmickContainer, m_seq7C, 0x7C);
END_LAYOUT_CHECK

// The settings of the vector-slide test (console-tunable). Only the members
// the decompiled units use are transcribed.
class CVectorSlideTestContainer : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	virtual bool Initialize(void);
	virtual bool Release(void);

	bool IsNewTypeVectorSlide(void);        // contentsdoc.h:472
	int GetLimitSpeed(void);                // contentsdoc.h:473
	int GetBoundHeight(void);               // contentsdoc.h:474
	int GetLimitVectorSlideCount(void);     // contentsdoc.h:475

private:
	bool m_newTypeVectorSlide;      // 0x4
	int m_limitSpeed;               // 0x8
	int m_boundHeight;              // 0xC
	int m_limitVectorSlideCount;    // 0x10
};

BEGIN_LAYOUT_CHECK(CVectorSlideTestContainer)
	CHECK_FIELD(CVectorSlideTestContainer, m_newTypeVectorSlide, 0x4);
	CHECK_FIELD(CVectorSlideTestContainer, m_limitSpeed, 0x8);
	CHECK_FIELD(CVectorSlideTestContainer, m_boundHeight, 0xC);
	CHECK_FIELD(CVectorSlideTestContainer, m_limitVectorSlideCount, 0x10);
END_LAYOUT_CHECK

// The gift-box event content: the box the player opened.
class CGiftBoxEvent : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	void SetOpenedBoxTID(unsigned long openedBoxTID);
	unsigned long GetOpenedBoxTID(void);

	virtual bool Initialize(void);
	virtual bool Release(void);

private:
	unsigned long m_openedBoxTID;   // 0x4
};

BEGIN_LAYOUT_CHECK(CGiftBoxEvent)
	CHECK_SIZE(CGiftBoxEvent, 0x8);
	CHECK_FIELD(CGiftBoxEvent, m_openedBoxTID, 0x4);
END_LAYOUT_CHECK

// The world-tour event content: the maps played and the gift state.
class CWorldTourEvent : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	void SetMapFlag(unsigned long mapFlag);
	void SetGotGift(bool gotGift);
	void SetCanGift(bool canGift);
	unsigned long GetMapFlag(void) const;
	bool GetGotGift(void) const;
	bool GetCanGift(void) const;

	virtual bool Initialize(void);
	virtual bool Release(void);

private:
	unsigned long m_mapFlag;    // 0x4
	bool m_gotGift;             // 0x8
	bool m_canGift;             // 0x9
};

BEGIN_LAYOUT_CHECK(CWorldTourEvent)
	CHECK_SIZE(CWorldTourEvent, 0xC);
	CHECK_FIELD(CWorldTourEvent, m_mapFlag, 0x4);
	CHECK_FIELD(CWorldTourEvent, m_gotGift, 0x8);
	CHECK_FIELD(CWorldTourEvent, m_canGift, 0x9);
END_LAYOUT_CHECK

// The Halloween event content: the maps played, the gifts and the event
// state.
class CHalloweenEvent : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	void SetMapFlag(unsigned long mapFlag);
	void SetGiftCnt(unsigned long giftCnt);
	void SetShowGiftDlg(bool showGiftDlg);
	void SetEventState(bool eventState);
	unsigned long GetMapFlag(void) const;
	unsigned long GetGiftCnt(void) const;
	bool GetShowGiftDlg(void) const;
	bool GetEventState(void) const;

	virtual bool Initialize(void);
	virtual bool Release(void);

private:
	unsigned long m_mapFlag;    // 0x4
	unsigned long m_giftCnt;    // 0x8
	bool m_showGiftDlg;         // 0xC
	bool m_eventState;          // 0xD
};

BEGIN_LAYOUT_CHECK(CHalloweenEvent)
	CHECK_SIZE(CHalloweenEvent, 0x10);
	CHECK_FIELD(CHalloweenEvent, m_mapFlag, 0x4);
	CHECK_FIELD(CHalloweenEvent, m_giftCnt, 0x8);
	CHECK_FIELD(CHalloweenEvent, m_showGiftDlg, 0xC);
	CHECK_FIELD(CHalloweenEvent, m_eventState, 0xD);
END_LAYOUT_CHECK

// The escape-beginner event content: one flag.
class CEscapeBeginnerEvent : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	void SetFlag(bool flag);
	bool GetFlag(void) const;

	virtual bool Initialize(void);
	virtual bool Release(void);

private:
	bool m_flag;    // 0x4
};

BEGIN_LAYOUT_CHECK(CEscapeBeginnerEvent)
	CHECK_SIZE(CEscapeBeginnerEvent, 0x8);
	CHECK_FIELD(CEscapeBeginnerEvent, m_flag, 0x4);
END_LAYOUT_CHECK

// The passing-title check content: one flag.
class CPassingTitleChk : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	void SetFlag(bool flag);
	bool GetFlag(void) const;

	virtual bool Initialize(void);
	virtual bool Release(void);

private:
	bool m_flag;    // 0x4
};

BEGIN_LAYOUT_CHECK(CPassingTitleChk)
	CHECK_SIZE(CPassingTitleChk, 0x8);
	CHECK_FIELD(CPassingTitleChk, m_flag, 0x4);
END_LAYOUT_CHECK

// The Christmas event content: one flag (set by default).
class CChristmasEvent : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	void SetFlag(bool flag);
	bool GetFlag(void) const;

	virtual bool Initialize(void);
	virtual bool Release(void);

private:
	bool m_flag;    // 0x4
};

BEGIN_LAYOUT_CHECK(CChristmasEvent)
	CHECK_SIZE(CChristmasEvent, 0x8);
	CHECK_FIELD(CChristmasEvent, m_flag, 0x4);
END_LAYOUT_CHECK

// The ticket exchange content: one flag (set by default).
class CTicketExchange : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	void SetFlag(bool flag);
	bool GetFlag(void) const;

	virtual bool Initialize(void);
	virtual bool Release(void);

private:
	bool m_flag;    // 0x4
};

BEGIN_LAYOUT_CHECK(CTicketExchange)
	CHECK_SIZE(CTicketExchange, 0x8);
	CHECK_FIELD(CTicketExchange, m_flag, 0x4);
END_LAYOUT_CHECK

// The Christmas sock event content: one flag (set by default).
class CChristmasSockEvent : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	void SetFlag(bool flag);
	bool GetFlag(void) const;

	virtual bool Initialize(void);
	virtual bool Release(void);

private:
	bool m_flag;    // 0x4
};

BEGIN_LAYOUT_CHECK(CChristmasSockEvent)
	CHECK_SIZE(CChristmasSockEvent, 0x8);
	CHECK_FIELD(CChristmasSockEvent, m_flag, 0x4);
END_LAYOUT_CHECK

#pragma pack(push, 1)
namespace GlobalEnum {
	// The state of one mission of the mission event (5 bytes, packed).
	// globalgamedefine.h:367 declares it in the original; transcribed here
	// until that shared header carries it.
	struct sMissionInfo {
		// globalgamedefine.h:367
		sMissionInfo(void) {
			condition = 0;
			complete = false;
		}

		int condition;      // 0x0: the mission's progress bits
		bool complete;      // 0x4
	};
}
#pragma pack(pop)

BEGIN_LAYOUT_CHECK(GlobalEnum::sMissionInfo)
	CHECK_SIZE(GlobalEnum::sMissionInfo, 0x5);
	CHECK_FIELD(GlobalEnum::sMissionInfo, condition, 0x0);
	CHECK_FIELD(GlobalEnum::sMissionInfo, complete, 0x4);
END_LAYOUT_CHECK

// The mission event content. Only the members the decompiled units use are
// declared.
class CMissionEvent : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	virtual bool Initialize(void);
	virtual bool Release(void);

	void SetEventFlag(unsigned int eventFlag);
	unsigned int GetEventFlag(void) const;

	// contentsdoc.h:163
	void SetDayGiftFlag(bool dayGiftFlag) { m_dayGiftFlag = dayGiftFlag; }
	// contentsdoc.h:164
	bool GetDayGiftFlag(void) const { return m_dayGiftFlag; }
	// contentsdoc.h:165
	void SetTermGiftFlag(bool termGiftFlag) { m_termGiftFlag = termGiftFlag; }
	// contentsdoc.h:166
	bool GetTermGiftFlag(void) const { return m_termGiftFlag; }
	// contentsdoc.h:168
	int GetCondition(int index) { return m_missionInfo[index].condition; }
	// contentsdoc.h:169
	const int GetMapCount(void) const { return 15; }

	bool IsCompleteMission(int index);
	bool IsCompleteAllDayMission(void);
	bool IsCompleteAllTermMission(void);
	bool IsCompleteCourse(int course);
	void DecodeMissionInfo(WReceivedPacket &packet);

private:
	// contentsdoc.h:173
	void ClearMissionInfo(void) { m_missionInfo.clear(); }

	int GetMissionField(int field) const;

	unsigned int m_eventFlag;                   // 0x4
	bool m_dayGiftFlag;                         // 0x8
	bool m_termGiftFlag;                        // 0x9
	std::vector<GlobalEnum::sMissionInfo> m_missionInfo;    // 0xC: 2 day missions, 5 term missions, then the course flags
};

BEGIN_LAYOUT_CHECK(CMissionEvent)
	CHECK_FIELD(CMissionEvent, m_eventFlag, 0x4);
	CHECK_FIELD(CMissionEvent, m_dayGiftFlag, 0x8);
	CHECK_FIELD(CMissionEvent, m_termGiftFlag, 0x9);
	CHECK_FIELD(CMissionEvent, m_missionInfo, 0xC);
END_LAYOUT_CHECK

// The rank of a player in his guild (only 1-3 are members; names
// invented).
enum GUILD_CLASS_IDX {
	GUILD_CLASS_NONE = 0,
	GUILD_CLASS_MASTER = 1      // the rank handed over by the master delegation
};


// One member of a guild member page (0x54 bytes from the list node
// allocation; layout partly known).
struct GUILD_USER_LIST {
	UNKNOWN(0x0, 0x4);
	unsigned long uid;          // 0x4: the member's user id
	GUILD_CLASS_IDX m_classIdx; // 0x8: the member's rank
	char text[0x2E];            // 0xC: the guild talk, passed through ExchangeQuotation
	char nickname[0x16];        // 0x3A: shown by the user info window
	UNKNOWN(0x50, 0x54);
};

// One guild of a guild search page (0xC4 bytes, packed). globalgamedefine.h
// declares it in the original (its constructor's line info); transcribed
// here until that shared header carries it.
#pragma pack(push, 1)
struct GUILD_LIST {
	// globalgamedefine.h:184
	GUILD_LIST(void) : guildNumber(0), data19(0), data1D(0), data21(0), data9A(0), data9E(-1) {
		memset(name, 0, sizeof(name));
		memset(dataB8, 0, sizeof(dataB8));
		memset(intro, 0, sizeof(intro));
		memset(dataA2, 0, sizeof(dataA2));
	}

	int guildNumber;            // 0x0: the guild a join request goes to
	char name[0x15];            // 0x4: drawn by the search list
	int data19;                 // 0x19: the members below are zeroed by the constructor (meaning not yet known)
	int data1D;                 // 0x1D
	int data21;                 // 0x21
	UNKNOWN(0x25, 0x35);        // not initialised by the constructor
	char intro[0x65];           // 0x35: the guild introduction shown on selection
	int data9A;                 // 0x9A
	int data9E;                 // 0x9E: -1 by default
	char dataA2[0x16];          // 0xA2
	char dataB8[0xC];           // 0xB8
};
#pragma pack(pop)

// One entry of the guild history (0x34 bytes from the list node allocation,
// packed). CGuildMainForm::RefreshMyGuildHistory prints the year, month and
// day of the date and the message of the state.
#pragma pack(push, 1)
struct GUILD_HISTORY {
	UNKNOWN(0x0, 0x1D);
	int state;                  // 0x1D: a GUILD_STATE_IDX (guildformcontroller.h, not included here)
	_SYSTEMTIME date;           // 0x21: wYear 0x21, wMonth 0x23, wDay 0x27 read
	UNKNOWN(0x31, 0x34);
};
#pragma pack(pop)

CHECK_SIZE(GUILD_USER_LIST, 0x54);
CHECK_SIZE(GUILD_LIST, 0xC4);
BEGIN_LAYOUT_CHECK(GUILD_LIST)
	CHECK_FIELD(GUILD_LIST, guildNumber, 0x0);
	CHECK_FIELD(GUILD_LIST, name, 0x4);
	CHECK_FIELD(GUILD_LIST, data19, 0x19);
	CHECK_FIELD(GUILD_LIST, intro, 0x35);
	CHECK_FIELD(GUILD_LIST, data9A, 0x9A);
	CHECK_FIELD(GUILD_LIST, data9E, 0x9E);
	CHECK_FIELD(GUILD_LIST, dataA2, 0xA2);
	CHECK_FIELD(GUILD_LIST, dataB8, 0xB8);
END_LAYOUT_CHECK
CHECK_SIZE(GUILD_HISTORY, 0x34);
BEGIN_LAYOUT_CHECK(GUILD_HISTORY)
	CHECK_FIELD(GUILD_HISTORY, state, 0x1D);
	CHECK_FIELD(GUILD_HISTORY, date, 0x21);
END_LAYOUT_CHECK
BEGIN_LAYOUT_CHECK(GUILD_USER_LIST)
	CHECK_FIELD(GUILD_USER_LIST, uid, 0x4);
	CHECK_FIELD(GUILD_USER_LIST, m_classIdx, 0x8);
	CHECK_FIELD(GUILD_USER_LIST, text, 0xC);
	CHECK_FIELD(GUILD_USER_LIST, nickname, 0x3A);
END_LAYOUT_CHECK

namespace _guild {

// The guild content: the player's guild, its member and search pages, its
// history and the guild messages.
class CGuildInfo : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	virtual bool Initialize(void);
	virtual bool Release(void);

	void SetMyGuildInfo(GUILD_INFO &info);
	void SetMyGuildUserInfo(GUILD_USER_INFO &info);
	void SetGuildEmblem(GUILD_USER_INFO *pInfo);
	void SetGuildName(GUILD_INFO *pInfo);
	void SetGuildIntroduce(const std::string &introduce);
	void SetGuildNotice(const std::string &notice);
	const GUILD_INFO &GetMyGuildInfo(void) const;
	// contentsdoc.h:363
	const std::list<GUILD_HISTORY> &GetHistoryList(void) const { return m_historyList; }
	bool IsGuildMember(void) const;
	bool IsGuildMember(GUILD_CLASS_IDX classIndex);
	bool IsManager(void) const;
	bool IsManager(GUILD_CLASS_IDX classIndex);
	const char *GuildMsg(int index);
	const std::list<GUILD_USER_LIST> *GetMemberList(int page);
	const std::list<GUILD_LIST> *GetSearchList(int page);
	int GetMaxMemberPage(void);
	int GetMaxSearchPage(void);
	void ClearHistoryList(void);
	void ClearMemberList(int page);
	void ClearSearchList(int page);
	void ClearMemberPage(void);
	void ClearSearchPage(void);
	void SetMemberPage(int page, std::list<GUILD_USER_LIST> *pList);
	void SetHistoryList(std::list<GUILD_HISTORY> *pList);
	void SetSearchList(int page, std::list<GUILD_LIST> *pList);

	// contentsdoc.h:362
	const GUILD_CLASS_IDX GetClassIndex(void) const { return (GUILD_CLASS_IDX)m_myGuildInfo.classIndex; }
	// contentsdoc.h:365
	int GetGuildNumber(void) const { return m_myGuildInfo.guildNumber; }
	// contentsdoc.h:367
	int GetAllMemberCount(void) { return m_allMemberCount; }
	// contentsdoc.h:368
	void SetAllMemberCount(int allMemberCount) { m_allMemberCount = allMemberCount; }
	// contentsdoc.h:370
	int GetAllGuildCount(void) { return m_allGuildCount; }
	// contentsdoc.h:371
	void SetAllGuildCount(int allGuildCount) { m_allGuildCount = allGuildCount; }

	void ClearAllData(void);

private:
	GUILD_INFO m_myGuildInfo;                                       // 0x4
	std::list<GUILD_HISTORY> m_historyList;                         // 0x130
	std::map<int, const char *> m_guildMsgs;                        // 0x13C
	std::map<int, std::list<GUILD_USER_LIST> > m_memberPages;       // 0x148
	std::map<int, std::list<GUILD_LIST> > m_searchPages;            // 0x154
	int m_allMemberCount;                                           // 0x160
	int m_allGuildCount;                                            // 0x164
};

} // namespace _guild

CHECK_SIZE(_guild::CGuildInfo, 0x168);
BEGIN_LAYOUT_CHECK(_guild::CGuildInfo)
	CHECK_FIELD(_guild::CGuildInfo, m_myGuildInfo, 0x4);
	CHECK_FIELD(_guild::CGuildInfo, m_historyList, 0x130);
	CHECK_FIELD(_guild::CGuildInfo, m_guildMsgs, 0x13C);
	CHECK_FIELD(_guild::CGuildInfo, m_memberPages, 0x148);
	CHECK_FIELD(_guild::CGuildInfo, m_searchPages, 0x154);
	CHECK_FIELD(_guild::CGuildInfo, m_allMemberCount, 0x160);
	CHECK_FIELD(_guild::CGuildInfo, m_allGuildCount, 0x164);
END_LAYOUT_CHECK

// The table of the contents usable over a period of dates. Only the members
// the decompiled units use are declared.
class CPeriodContents : public IContentsDataContainer {
	LAYOUT_CHECK_FRIEND;
public:
	// The period of one content and whether it is usable now.
	struct sPeriodUnit {
		_SYSTEMTIME begin;  // 0x0
		_SYSTEMTIME end;    // 0x10: not set when noEnd
		bool noEnd;         // 0x20: usable from begin on, without an end
		bool usable;        // 0x21
	};

	virtual bool Initialize(void);
	virtual bool Release(void);

	void AddUsableContents(int type, const _SYSTEMTIME *begin, const _SYSTEMTIME *end, bool noEnd);
	bool IsUsableContents(int type);
	void Set_PreSettingContentsDate(void);
	void CheckAllContents(void);
	bool CheckContent(int type);

private:
	void IsUsable(sPeriodUnit &unit);

	std::map<int, sPeriodUnit> m_contents;  // 0x4
};

BEGIN_LAYOUT_CHECK(CPeriodContents::sPeriodUnit)
	CHECK_SIZE(CPeriodContents::sPeriodUnit, 0x22);
	CHECK_FIELD(CPeriodContents::sPeriodUnit, begin, 0x0);
	CHECK_FIELD(CPeriodContents::sPeriodUnit, end, 0x10);
	CHECK_FIELD(CPeriodContents::sPeriodUnit, noEnd, 0x20);
	CHECK_FIELD(CPeriodContents::sPeriodUnit, usable, 0x21);
END_LAYOUT_CHECK

BEGIN_LAYOUT_CHECK(CPeriodContents)
	CHECK_FIELD(CPeriodContents, m_contents, 0x4);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CONTENTSDOC_H
