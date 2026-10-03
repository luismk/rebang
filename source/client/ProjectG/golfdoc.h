#pragma once

class WPolySoup;

struct sPlayerData
{
	int state;
	unsigned char index;
	// TODO: incomplete
	unsigned char unused5[0x4d3];
	unsigned char flag;
	unsigned char team;
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

	sHoleData& GetHoleData() { return m_pHoleData[m_currentHole - 1]; }
	sHoleData& GetHoleData(unsigned char hole) { return m_pHoleData[hole - 1]; }

	// TODO: incomplete
	unsigned char m_unused4[4];
	WPolySoup* m_pPolySoup;
	unsigned char m_unusedc[0x6c];
	int m_tutorialMode;
	unsigned char m_unused7c[0x49];
	unsigned char m_playerNum;
	unsigned char m_unusedc6[2];
	sHoleData* m_pHoleData;
	unsigned char m_unusedcc[0x9c];
	unsigned char m_currentHole;
	unsigned char m_unused169[0xc];
	unsigned char m_currentPlayer;
	unsigned char m_unused176[8];
	unsigned char m_maxGrade;
};
