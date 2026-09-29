#pragma once

#pragma pack(push, 1)

struct sRoomDetail
{
	unsigned char holes;
	unsigned long gameTime;
	unsigned char map;
	unsigned char gameType;
	unsigned char holeOrder;
	unsigned long matchTypeId;
};

struct sPangYaUserInfo
{
	char id[22];
	char nickname[22];
	char guildName[21];
	char guildMark[12];
	unsigned long schoolID;
	unsigned long capability;
	unsigned long gmOid;
	unsigned long oid;
	unsigned char reserved[12];
	unsigned long guildUID;
	unsigned long guildPang;
	unsigned char flag;
	unsigned char flag2;
	short bsUsableTimes;
	short bsUsableBonusTimes;
	short bsRemainedBonusTimes;
	unsigned long pointEvent;
	unsigned long flagBlock;
	int timeBlock;
	unsigned long changeNick;
	char globalID[128];
	unsigned long uid;

	bool IsIdentity(unsigned long identity)
	{
		return (capability & identity) ? true : false;
	}
};

struct sPangYaUserStatistics
{
	unsigned long stroke;
	unsigned long putt;
	unsigned long playTime;
	unsigned long shotTime;
	float longestDrive;
	unsigned long pangya;
	unsigned long timeout;
	unsigned long ob;
	unsigned long totalDistance;
	unsigned long hole;
	unsigned long unfinishedHole;
	unsigned short holeInOne;
	unsigned short bunker;
	unsigned long fairway;
	unsigned short albatross;
	unsigned long manner;
	unsigned long puttIn;
	float longestPutt;
	float longestChipIn;
	unsigned long exp;
	unsigned char level;
	__int64 pang;
	int totalScore;
	char bestScore[5];
	unsigned char eventFlag;
	__int64 bestPang[5];
	__int64 bestPangTotal;
	unsigned long gameCount;
	unsigned long teamHole;
	unsigned long teamWin;
	unsigned long teamGame;
	unsigned long ladderPoint;
	unsigned long ladderHole;
	unsigned long ladderWin;
	unsigned long ladderLose;
	unsigned long ladderDraw;
	unsigned long comboCount;
	unsigned long maxComboCount;
	unsigned long quitCount;
	__int64 pangBattlePang;
	unsigned long pangBattleWin;
	unsigned long pangBattleLose;
	unsigned long pangBattleAllIn;
	unsigned long pangBattleRunHole;
	unsigned short eventValue;
	unsigned long holeEventCount;
	unsigned long holeEventSerial;
	unsigned long seasonGameCount;
	unsigned char reserved[8];
};

struct sTrophyStatistics
{
	unsigned short trophy[13][3];
};

struct sUserEquip
{
	unsigned long caddieId;
	unsigned long characterId;
	unsigned long clubSetId;
	unsigned long ballTypeId;
	unsigned long itemSlot[10];
	unsigned long skinId[6];
	unsigned long skinTypeId[6];
	unsigned long mascotId;
};

struct sMapStatistics
{
	unsigned char course;
	int stroke;
	int putt;
	int hole;
	int fairway;
	int holeIn;
	int puttIn;
	int totalScore;
	char bestScore;
	__int64 bestPang;
	unsigned long characterTypeId;
	unsigned char eventScore;

	sMapStatistics() { course = 0xff; }
};

struct sMyInfo
{
	unsigned short roomNumber;
	sPangYaUserInfo userInfo;
	sPangYaUserStatistics statistics;
	sTrophyStatistics trophy;
	sUserEquip equip;
	sMapStatistics mapStatistics[20];
	sMapStatistics classicMapStatistics[20];
};

#pragma pack(pop)
