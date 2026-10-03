#pragma once

#include <bitset>
#include "encrypttypes.h"
#include "parttidlist.h"
#include <map>

class WPolySoup;
class CSky;

#pragma pack(push, 1)
struct sWinningPrize
{
	unsigned long holeInOne;
	unsigned long albatross;
	unsigned long eagle;
	unsigned long birdie;
	unsigned long par;
	unsigned long bogey;
};

struct sPrizeInfo
{
	unsigned long typeId;
	unsigned char unknown[2];
	unsigned short quantity;
	int type;
};
#pragma pack(pop)

struct sPlayerData
{
	int state;
	unsigned char index;
	WVector startPos;
	WVector pos;
	WCrypticValue<unsigned char> stroke[18];
	WCrypticValue<unsigned char> putt[18];
	WCrypticValue<int> pang[18];
	WCrypticValue<int> skinsPang[18];
	unsigned long color;
	unsigned long uid;
	unsigned long oid;
	unsigned char itemNum;
	unsigned long item;
	unsigned char teeOrder;
	WCrypticValue<float> gauge;
	unsigned short unusedWord;
	unsigned char bunker;
	unsigned char timeout;
	unsigned char giveUp;
	unsigned long shotFlag;
	std::bitset<32> event;
	unsigned char flag;
	unsigned char team;
	unsigned char teammate;
	unsigned long shotTime;
	unsigned char order : 4;
	unsigned char rank : 4;
	unsigned char totalStroke;
	int score;
	WCrypticValue<int> totalPang;
	WCrypticValue<int> bonusPang;
	unsigned long holeNum;
	WCrypticValue<int> power;
	WCrypticValue<int> accuracy;
	WCrypticValue<int> control;
	WCrypticValue<int> spin;
	WCrypticValue<int> curve;
	unsigned char chatBlock;
	sWinningPrize prize;
	unsigned char extPrizeNum;
	sPrizeInfo extPrize[128];
	unsigned char holeInPose;
	unsigned char holeInCollision;
	__int64 skinsTotalPang;
	__int64 skinsSumPang;
	__int64 skinsHolePang;
	unsigned char level;
};

struct sWave
{
	char texture[32];
	float width;
	float freq;
};

class CWaveInfo
{
public:
	void Load(const char* filename);

private:
	std::map<int, sWave> m_wave;
};

struct sRiver
{
	float scrollVel;
	float tileHead;
	float tileSide;
	float depth;
};

class CRiverInfo
{
public:
	void Load(const char* filename);

private:
	std::map<int, sRiver> m_river;
};

struct sHoleData
{
	WVector tee;
	WVector pin;
	unsigned char par;
	unsigned char playerNum;
	unsigned char rank[4];
	unsigned char result;
	unsigned char carry;
	__int64 pang;
	__int64 basePang;
	unsigned long checkSum;
	unsigned long reserved;
};

class CGolfDoc
{
public:
	CGolfDoc();
	virtual ~CGolfDoc();

	sPlayerData* GetPlayer(unsigned char index);
	void MakePlayer(unsigned char num);
	void RefreshPlayerNum();
	void SetPlayerLevel(unsigned char index);
	CPartTidList& GetPartTidList(unsigned char index);
	void ApproachDataAllClear();

	sHoleData& GetHoleData() { return m_pHoleData[m_currentHole - 1]; }
	sHoleData& GetHoleData(unsigned char hole) { return m_pHoleData[hole - 1]; }

	// TODO: incomplete
	unsigned char m_unused4[4];
	WPolySoup* m_pPolySoup;
	CSky* m_pSky;
	unsigned char m_unused10[0x30];
	CWaveInfo m_waveInfo;
	CRiverInfo m_riverInfo;
	unsigned char m_unused58[0x20];
	int m_tutorialMode;
	unsigned char m_unused7c[0x49];
	unsigned char m_playerNum;
	unsigned char m_unusedc6[2];
	sHoleData* m_pHoleData;
	unsigned char m_unusedcc[0x7c];
	unsigned long m_gameMode;
	unsigned char m_unused14c[1];
	bool m_bPause;
	unsigned char m_unused14e[0x1a];
	unsigned char m_currentHole;
	unsigned char m_unused169[3];
	unsigned long m_remainTime;
	unsigned long m_shotTime;
	unsigned char m_shotPhase;
	unsigned char m_currentPlayer;
	unsigned char m_unused176[8];
	unsigned char m_maxGrade;
	bool m_bStopDlg;
	bool m_bNoStat;
};
