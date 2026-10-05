#pragma once

#include <string>
#include <list>
#include "../../shared/globalgamedefine.h"
#include "../../shared/classdefine.h"
ILFILL2
#include "../../shared/itemmanager.h"
#include "hatmanager.h"

struct sGameTypeInfo
{
	const char* name;
	unsigned char maxPlayer;
	unsigned char minPlayer;
	unsigned char holes;
};

struct sGolfGame
{
	unsigned char map;
	IFF_STRUCT::sCourse* pCourse;
	unsigned char gameType;
	char gameTypeName[260];
	unsigned char holes;
	unsigned char weather;
	unsigned long shotTimeLimit;
	unsigned long gameTimeLimit;

	sGolfGame()
		: map(0),
		  pCourse(NULL),
		  gameType(0),
		  weather(0),
		  shotTimeLimit(0),
		  gameTimeLimit(0)
	{
		holes = 18;
		memset(gameTypeName, 0, sizeof(gameTypeName));
	}
};

enum eReplayModeType
{
};

struct sRivalData
{
	unsigned long uid;
	unsigned long oid;
	char nickname[22];
	char guildName[21];
	unsigned char team;
	unsigned short level;
	unsigned char state;
	int holeScore[18];
	unsigned char holeStroke[18];
	__int64 holePang[18];
	__int64 totalPang;
	__int64 totalBonusPang;
	int totalScore;
	unsigned char totalStroke;
	unsigned char hole;
	unsigned char rank;
	unsigned char order;
	unsigned char quitOrder;
	unsigned char finishOrder;
	WVector ballPos;
	unsigned long observerState;
	unsigned char observerRank;
	unsigned long titleTypeId;
	unsigned char flag;
	unsigned long guildUID;
	char guildMark[12];
	unsigned long guildPoint;
	unsigned long approachDistance;
	unsigned long approachResultDistance;
	int approachTime;
	unsigned long capability;
	unsigned long mascotTypeId;
	unsigned char pangMastery;
	unsigned char pangNitro;
};

struct sRecordedItemInfo
{
	sItemInfo itemInfo;
	unsigned long course;
	SYSTEMTIME date;
	char fileName[32];
	unsigned char bValid;
};

class CGolfDoc;

struct sChatLine
{
	std::string text;
	unsigned long color;
};

class CSharedDoc : public WSingleton<CSharedDoc>
{
public:
	CSharedDoc();
	virtual ~CSharedDoc();

	int FindUserInfoTimeUID(unsigned long uid,
		std::map<unsigned long, sUserInfoTime>::iterator& it);
	std::map<unsigned char, sMapStatistics>& GetMyPastMapStat(
		unsigned char season);
	std::string GetLoginAuthKey();
	int IsControlServerService(int service);
	void ClearUserInfoTimeMap(unsigned long uid);
	void SetIndex(unsigned long uid);
	unsigned char GetIndex(unsigned long guid);
	float GetWtPepPangyaComboGauge(unsigned char player);
	float GetComboGaugeLimit(unsigned char team);
	SYSTEMTIME& GetServerTime();
	int CollapseItemSlot(unsigned char slot);

	unsigned long GetMapEventPangRate(int course);
	bool CanCompound(unsigned long uid);
	bool CanUseRookieChannelMap(unsigned int course);
	__int64 RemainOwnCash(unsigned long provider, __int64 cookie);

	char* GetFileName() { return m_fileName; }
	const std::list<sItemInfo>& GetConstMyItemList() { return m_myItemList; }
	eReplayModeType GetPlayingMode() { return m_playingMode; }

	std::string m_password;
	std::string m_gameServerAddr;
	std::string m_unusedString60;
	unsigned long m_gameServerPort;
	unsigned long m_unused80;
	unsigned long m_gameServerUID;
	unsigned long m_unused88;
	unsigned long m_loginServerUID;
	bool m_bSentHoleStat;
	unsigned long m_needParentAgree;
	std::string m_rankServerAddr;
	unsigned long m_rankServerPort;
	unsigned long m_unusedB8[4];
	unsigned long m_lobbyLoginTime;
	sRoomDetail m_roomDetail;
	sGameTypeInfo m_gameTypeInfo[15];
	unsigned char m_unused150[8];
	SYSTEMTIME m_serverTime;
	std::string m_unusedString168;
	unsigned long m_unused184;
	std::string m_unusedString188;
	int m_provType;
	std::string m_commandLineID;
	unsigned char m_unused1c4;
	int m_gachaTicket[2];
	std::list<unsigned long> m_refreshGuidList;
	std::list<unsigned short> m_refreshCountList;
	CItemManager m_itemManager;
	sMyInfo m_myInfo;
	unsigned char m_unusedd4c[0x40c];
	std::map<unsigned int, sCharacterInfo> m_charMap;
	std::map<unsigned int, sCaddieInfo> m_caddieMap;
	unsigned char m_unused1170[0x18];
	std::map<unsigned int, sItemInfo> m_clubSetMap;
	std::list<sItemInfo> m_ballList;
	std::list<sItemInfo> m_myItemList;
	std::list<sRecordedItemInfo> m_recordedItemList;
	unsigned char m_unused11b8[0x90];
	std::list<sSCardAvilityPeriodInfo> m_cardAbilityList[4];
	unsigned char m_unused1278[0x10];
	__int64 m_cookie;
	__int64 m_cash;
	__int64 m_bonusCash;
	unsigned char m_unused12a0[0x30];
	sUserInfo m_userInfo[4];
	unsigned char m_unused4118[0x3ec];
	std::vector<sRivalData> m_rivalList;
	unsigned char m_unused4514[0x48];
	bool m_bGameOver;
	unsigned char m_unused455d[3];
	int m_gameMode;
	unsigned char m_holeType;
	unsigned char m_unused4565[0x69];
	unsigned char m_courseMap[18];
	unsigned char m_unused45e0[0x12];
	unsigned char m_holeOrder[19];
	unsigned char m_unused4605[3];
	unsigned long m_holeRandom[18];
	unsigned long m_approachStartTime;
	unsigned char m_unused4654[0xc];
	sGolfGame m_golfGame;
	unsigned char m_unused4778[0x60];
	sRealMyRoomAuthority m_rmrAuthority;
	unsigned char m_unused4843[0x209];
	std::map<unsigned long, sBriefUserInfo> m_briefUserInfoMap;
	unsigned char m_unused4a58[0x18];
	std::list<sSlotInfo> m_slotList;
	unsigned char m_unused4a7c[0x3c];
	sChannelInfo m_curChannel;
	unsigned char m_unused4b05[0x79];
	bool m_bBackgroundVideo;
	unsigned char m_unused4b7f[0x51];
	std::map<unsigned long, sFriend> m_friendMap;
	unsigned char m_unused4bdc[0x90];
	ChatManager m_chatManager;
	std::list<sChatLine> m_chatLineList;
	unsigned char m_unused4cb0[0xc];
	std::list<std::string> m_ignoreList;
	unsigned char m_unused4cc8[0x40];
	std::list<sNoteInfo> m_noteList;
	unsigned char m_unused4d14[0x58];
	int m_offlinePlay;
	unsigned char m_unused4d70[0xd];
	bool m_bTutorialResume;
	unsigned char m_unused4d7e[2];
	unsigned long m_newMapEventMask;
	unsigned char m_unused4d84[0x8];
	unsigned long m_tutorialComplete[3];
	unsigned char m_unused4d98[0x28];
	struct
	{
		unsigned long exp, totalExp;
	} m_levelTable[71];
	struct
	{
		unsigned long exp, pang;
	} m_bonusPangTable[3];
	unsigned char m_unused5010[0x30];
	CGolfDoc* m_pGolfDoc;
	unsigned char m_unused5044[0xb1];
	bool m_bItemStorageLock;
	unsigned char m_unused50f6[0xa9e];
	int m_replayState;
	int m_replayPlayType;
	eReplayModeType m_playingMode;
	char m_fileName[32];
	unsigned long m_replayPosition;
	unsigned long m_replayDuration;
	unsigned long m_replayTick;
	int m_replayShotIndex;
	// TODO: this struct definition is incomplete
};

int OnlinePlay();
bool SetCurMap(unsigned char map);

ILFILL3

#include "shareddoc.inl"
