#pragma once

#include <string>
#include <list>
#include "../../shared/globalgamedefine.h"
#include "../../shared/classdefine.h"
ILFILL2
#include "../../shared/itemmanager.h"

struct sGameTypeInfo
{
	const char* name;
	unsigned char maxPlayer;
	unsigned char minPlayer;
	unsigned char holes;
};

class CGolfDoc;

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
	unsigned char m_unusedd4c[0x584];
	sUserInfo m_userInfo[4];
	unsigned char m_unused4118[0xf28];
	CGolfDoc* m_pGolfDoc;
	// TODO: this struct definition is incomplete
};

int OnlinePlay();

ILFILL3

#include "shareddoc.inl"
