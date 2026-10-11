// Original: d:\build\custom\temp\code\client\projectg\titles_client.cpp
#include "Titles_Client.h"
#include "SharedDoc.h"
#include "../shared/TitleManager.h"
#include "../shared/SharedTitleDoc.inl"


// titles_client.cpp:48
class Title21 : public Title {
public: virtual bool CanEquip() {
	if (WSingleton<CSharedDoc>::Instance()->m_myLevel < 6)
	{
		return false;
	}

	if (CheckPangyaPercent(WSingleton<CSharedDoc>::Instance()->m_myStatPangyas, WSingleton<CSharedDoc>::Instance()->m_myStatShots, 70.0f))
	{
		return true;
	}
	return false;
} };

// titles_client.cpp:63
class Title22 : public Title {
public: virtual bool CanEquip() {
	if (WSingleton<CSharedDoc>::Instance()->m_myLevel < 6)
	{
		return false;
	}

	if (CheckFairawayPercent(WSingleton<CSharedDoc>::Instance()->m_myStatFairways, WSingleton<CSharedDoc>::Instance()->m_myStatDrives, WSingleton<CSharedDoc>::Instance()->m_myStatPar3Drives, 70.0f))
	{
		return true;
	}
	return false;
} };
// titles_client.cpp:77
class Title23 : public Title {
public: virtual bool CanEquip() {
	if (WSingleton<CSharedDoc>::Instance()->m_myLevel < 6)
	{
		return false;
	}

	if (CheckPuttinPercent(WSingleton<CSharedDoc>::Instance()->m_myStatPuttsSunk, WSingleton<CSharedDoc>::Instance()->m_myStatPuttAttempts, 80.0f))
	{
		return true;
	}
	return false;
} };

// titles_client.cpp:92
class Title24 : public Title { public: virtual bool CanEquip(); };
































// titles_client.cpp:126
class Title25 : public Title {
public: virtual bool CanEquip() {
	if (CheckGoldMedal(WSingleton<CSharedDoc>::Instance()->m_myTrophy, 10))
	{
		return true;
	}
	return false;
} };

// titles_client.cpp:136
class Title26 : public Title {
public: virtual bool CanEquip() {
	if (CheckSilverMedal(WSingleton<CSharedDoc>::Instance()->m_myTrophy, 10))
	{
		return true;
	}
	return false;
} };

// titles_client.cpp:146
class Title27 : public Title {
public: virtual bool CanEquip() {
	if (CheckBronzeMedal(WSingleton<CSharedDoc>::Instance()->m_myTrophy, 10))
	{
		return true;
	}
	return false;
} };

// titles_client.cpp:156
class Title28 : public Title {
public: virtual bool CanEquip() {
	if (WSingleton<CSharedDoc>::Instance()->m_myLevel < 6)
	{
		return false;
	}

	if (CheckNoMannerPercent(WSingleton<CSharedDoc>::Instance()->m_myStatNoMannerGames, WSingleton<CSharedDoc>::Instance()->m_myStatGamesPlayed, 3.0f))
	{
		return true;
	}
	return false;
} };




























































































































































































































































// titles_client.cpp:422
class Title357 : public Title {
public: virtual bool CanEquip() {
	if (WSingleton<CSharedDoc>::Instance()->m_myPang < 1000000)
	{
		return false;
	}
	return true;
} };
// titles_client.cpp:431
class Title358 : public Title {
public: virtual bool CanEquip() {
	if (WSingleton<CSharedDoc>::Instance()->m_myPang < 3000000)
	{
		return false;
	}
	return true;
} };
// titles_client.cpp:440
class Title359 : public Title {
public: virtual bool CanEquip() {
	if (WSingleton<CSharedDoc>::Instance()->m_myPang < 5000000)
	{
		return false;
	}
	return true;
} };
// titles_client.cpp:449
class Title360 : public Title {
public: virtual bool CanEquip() {
	if (CheckHoleinCount(WSingleton<CSharedDoc>::Instance()->m_myStatHoleInOnes, 300))
	{
		return true;
	}
	return false;
} };
// titles_client.cpp:458
class Title361 : public Title {
public: virtual bool CanEquip() {
	if (CheckHoleinCount(WSingleton<CSharedDoc>::Instance()->m_myStatHoleInOnes, 500))
	{
		return true;
	}
	return false;
} };
// The TitleNN classes are defined before RegisterTitles here (it news them);
// the original line info puts RegisterTitles at line 8, ahead of them.
// titles_client.cpp:8
void RegisterTitles(TitleManager *manager) {
	manager->Register(TITLE_21, new Title21);
	manager->Register(TITLE_22, new Title22);
	manager->Register(TITLE_23, new Title23);
	manager->Register(TITLE_24, new Title24);
	manager->Register(TITLE_25, new Title25);
	manager->Register(TITLE_26, new Title26);
	manager->Register(TITLE_27, new Title27);
	manager->Register(TITLE_28, new Title28);
	manager->Register(TITLE_357, new Title357);
	manager->Register(TITLE_358, new Title358);
	manager->Register(TITLE_359, new Title359);
	manager->Register(TITLE_360, new Title360);
	manager->Register(TITLE_361, new Title361);
}
