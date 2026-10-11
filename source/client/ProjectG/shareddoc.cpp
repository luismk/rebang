// Original: d:\build\custom\temp\code\client\projectg\shareddoc.cpp
// The precompiled header already supplies ShareDoc's inline definitions.
#include "stdafx.h"	// use the project's precompiled header so header changes are visible
#include "SharedDoc.h"
#include "globalgamedefine.h"
#include "GolfDoc.h"
#include "icontentsdoc.h"
#include "contentsdoc.h"
#include "cardsystem.h"
#include "Packet.h"
#include "RealMyRoomData.h"
#include "MascotInfoDlg.h"
#include "../shared/s5/SharedUtilities.h"
#include "buddymanager.h"
#include "DetailedRoomInfoDlg.h"
#include "CardDlg.h"
#include "PangGiftDlg.h"
#include "messengerdlg.h"
#include "UccManager.h"
#include "commonutil.h"
#include "golfball.h"
#include "NetworkSystem.h"
#include "TaskManager.h"
#include "LobbyTask.h"
#include "GolfTask.h"
#include "Exhibition.h"
#include "ShopTask.h"
#include "frwnd.h"
#include "frcombobox.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SharedDoc.inl"
#if !defined(PG_CLIENT_PROJECTG_GOLFDOC_H)
// Ensure GolfDoc.h's inlines (GOLFDOC helper) are available for the
// inline functions emitted from shareddoc.inl. GolfDoc.h includes
// shareddoc.h at its end, so include it here if not already present.
#include "GolfDoc.h"
#endif
#include "../client/Wangreal/include/westpak.h"
#include "../client/Wangreal/include/wresrcmng.h"

void __fastcall LineParse(const char *line, std::vector<std::string> &tokens);

// The player level table of the exe, 71 records of 5 dwords (the first is the level's exp).
extern "C" const unsigned long data_9e6468[71][5];

// Progressive stubs for unnamed data and small helpers referenced by many
// units. These are minimal placeholders to satisfy the linker during the
// iterative porting process and should be replaced with accurate data.

// A small float used by login/dialog animations.
extern "C" float data_b7c970 = 0.0f;

// Wangreal math unnamed .rdata table used by several modules. Provide a
// small placeholder array with reasonable defaults.
extern "C" const float data_9eb420[5] = { 3.14159265f, 1.57079633f, 0.0174532925f, 0.0f, 0.0f };

/* FirstMap/PrevMap/NextMap and CSharedDoc::Log30sScore are provided
   out-of-line by other translation units or as inline in headers; do not
   redefine them here to avoid duplicate definitions. Only unnamed data
   placeholders remain in this file. */

// Minimal stub for CSharedDoc::Log30sScore declared in shareddoc.h. The
// original function likely logged or updated score data every 30 seconds;
// for now implement a no-op to satisfy the linker. Replace with correct
// behavior when available.
void CSharedDoc::Log30sScore(void) {
	// no-op placeholder
}


// Some unnamed string literals referenced by many units are provided in the
// original exe's .rdata. Provide minimal placeholders for those string
// symbols expected by the linker (empty const char arrays). These should be
// replaced with the original values when available.
extern "C" const char _C_data_placeholder_1[] = "";
extern "C" const char _C_data_placeholder_2[] = "";
extern "C" const char _C_data_placeholder_3[] = "";


// The room titles GetDefaultTitle picks from at random, by game kind. The
// original initializes them dynamically (sub_00989900 to sub_009899f0, one
// per array): each array's first title is a constant, which makes the
// initializer non-constant (docs/tough_problems/010). Battle and approach
// share their first title.
static const char *const s_titleVs = "\xc6\xce\xbe\xdf~ \xc6\xce\xbe\xdf!"; // 팡야~ 팡야!
static const char *const s_titleMass = "\xc8\xa6\xc0\xce\xbf\xf8\xc0\xbb \xb3\xbb \xc7\xb0\xbe\xc8\xbf\xa1"; // 홀인원을 내 품안에
static const char *const s_titleOneRound = "\xbd\xac\xbe\xee\xb0\xa1\xb4\xc2 \xc7\xd1 \xc6\xc7!"; // 쉬어가는 한 판!
static const char *const s_titleChat = "\xbc\xd2\xb3\xe2, \xbc\xd2\xb3\xe0\xb8\xa6 \xb8\xb8\xb3\xaa\xb4\xd9"; // 소년, 소녀를 만나다

// shareddoc.cpp:52
const char *defaultTitles_vs[] = {
	s_titleVs,
	"\xb3\xca\xb6\xfb \xb3\xaa\xb6\xfb \xc6\xce\xbe\xdf~", // 너랑 나랑 팡야~
	"\xb9\xd9\xb6\xf7 \xc1\xc1\xc0\xba \xb9\xe6~", // 바람 좋은 방~
	"\xc6\xce\xbe\xdf\xbf\xcd \xc7\xd4\xb2\xb2\xb6\xf3\xb8\xe9~!", // 팡야와 함께라면~!
	"\xbf\xb9\xbb\xdb \xbd\xba\xc0\xae, \xb8\xda\xc1\xf8 \xbc\xa6!", // 예쁜 스윙, 멋진 샷!
	"\xb3\xaf\xbe\xc6\xb6\xf3 \xbe\xc6\xc1\xee\xc5\xd8~", // 날아라 아즈텍~
	"\xc0\xfa \xc7\xcf\xb4\xc3\xc0\xc7 \xc6\xce\xc0\xcc \xb5\xc7\xbe\xee~", // 저 하늘의 팡이 되어~
	"\xc6\xce\xbe\xdf\xb0\xa1 \xc3\xd6\xb0\xed!", // 팡야가 최고!
	"\xb4\xd9\xc7\xd4\xb2\xb2 \xc6\xae\xb7\xb9\xc0\xfa \xc7\xe5\xc6\xae!", // 다함께 트레저 헌트!
};

// shareddoc.cpp:70
const char *defaultTitles_mass[] = {
	s_titleMass,
	"\xc6\xce\xbe\xdf\xb4\xeb\xc8\xb8\xb7\xce \xb8\xf0\xc0\xcc\xbc\xbc\xbf\xe4~", // 팡야대회로 모이세요~
	"\xb8\xf0\xb5\xce\xb8\xf0\xb5\xce \xc6\xce\xbe\xdf!", // 모두모두 팡야!
	"\xb1\xe2\xb7\xcf \xb0\xbb\xbd\xc5\xc0\xc7 \xb1\xd7\xb3\xaf\xb1\xee\xc1\xf6!", // 기록 갱신의 그날까지!
	"1\xb5\xee \xb1\xe2\xb4\xd9\xb7\xc1~", // 1등 기다려~
	"\xc6\xae\xb7\xce\xc7\xc7\xb4\xc2 \xb3\xbb\xb2\xa8~", // 트로피는 내꺼~
};

// shareddoc.cpp:82
const char *defaultTitles_battle[] = {
	s_titleOneRound,
	"\xc7\xd1\xb9\xe6\xbd\xc2\xba\xce! \xc6\xce\xb9\xe8\xc6\xb2~", // 한방승부! 팡배틀~
	"\xb4\xeb\xb9\xda \xb2\xde, \xbd\xf1\xbe\xc6\xc1\xf6\xb4\xc2 \xc6\xce", // 대박 꿈, 쏟아지는 팡
	"\xbb\xf5\xb7\xce\xbf\xee \xbd\xc3\xc1\xf0 \xc6\xce\xb9\xe8\xc6\xb2 \xc7\xd1 \xc6\xc7!", // 새로운 시즌 팡배틀 한 판!
};

// shareddoc.cpp:91
const char *defaultTitles_approach[] = {
	s_titleOneRound,
	"\xc7\xd1\xb9\xe6\xbd\xc2\xba\xce! \xbe\xee\xc7\xc1\xb7\xce\xc4\xa1~", // 한방승부! 어프로치~
	"\xbd\xc3\xc1\xf0 NEW \xbe\xee\xc7\xc1\xb7\xce\xc4\xa1", // 시즌 NEW 어프로치
};

// shareddoc.cpp:100
const char *defaultTitles_chat[] = {
	s_titleChat,
	"\xbc\xf6\xb4\xd9\xc0\xef\xc0\xcc, \xbb\xf5\xc4\xa7\xb6\xbc\xb1\xe2", // 수다쟁이, 새침떼기
	"\xc0\xce\xbf\xac\xc0\xcc \xb8\xf0\xc0\xcc\xb4\xc2 \xb0\xf7", // 인연이 모이는 곳
	"\xc6\xce\xbe\xdf! \xc1\xf1\xb0\xc5\xbf\xee \xb8\xb8\xb3\xb2~", // 팡야! 즐거운 만남~
	"\xc6\xce\xbe\xdf\xbf\xf9\xb5\xe5\xbf\xcd \xc7\xd4\xb2\xb2!", // 팡야월드와 함께!
	"\xc6\xce\xbe\xdf\xc0\xce\xb5\xe9 \xb4\xd9 \xb8\xf0\xbf\xa9\xb6\xf3~!", // 팡야인들 다 모여라~!
	"\xc4\xf0 \xb0\xa1\xc0\xcc! \xb3\xaa\xc0\xcc\xbd\xba \xb0\xc9!", // 쿨 가이! 나이스 걸!
};

// shareddoc.h:534
void CSharedDoc::HaveAngelWing(bool have) {
	m_haveAngelWing = have;
}

// shareddoc.h:608
void CSharedDoc::SetTradeMode(unsigned char mode) {
	m_tradeMode = mode;
}

// shareddoc.h:1009
void CSharedDoc::SetHaveHalloweenItem(bool have) {
	m_haveHalloweenItem = have;
}

// shareddoc.h:1010
void CSharedDoc::SetWearHalloweenItem(bool wear) {
	m_wearHalloweenItem = wear;
}

// globalgamedefine.h:3549
void sFriend::Clear(void) {
	memset(nick, 0, sizeof(nick));
	memset(group, 0, sizeof(group));
	uid = 0xFFFFFFFF;

	value25 = 0xFFFFFFFF;
	value29 = 0;
	value2D = 0xFFFFFFFF;

	memset(data31, 0, sizeof(data31));

	memset(extra.data, 0, sizeof(extra.data));
	extra.index = 0xFF;
	extra.code = 0xFFFF;
	extra.id1 = 0xFFFFFFFF;
	extra.id2 = 0xFFFFFFFF;

	status = 5;
	channel = 0xFF;
	value8A = 0;
	flags = 0;
	listFlags = 0;
}

// globalnetworkdefine.h:2481
sTradeItem::sTradeItem(void) {
	index = 0;
	typeId = 0;
	guid = 0;
	count = 0;
	kind = 0;
	flag = 0;
	price = 0;
	period = 0;
	memset(stats, 0, sizeof(stats));
	statFlag = 0;

	memset(uccIdx, 0, sizeof(uccIdx));
	uccSeq = 0;
	uccStatus = 0;

	memset(uccData, 0, sizeof(uccData));
	memset(uccName, 0, sizeof(uccName));
	memset(uccCopierNick, 0, sizeof(uccCopierNick));
}

// globalnetworkdefine.h:2837
sStall::sStall(void) {
	AllClear();
}

// shareddoc.cpp:123
CSharedDoc::CSharedDoc(void)
	: m_curMap(0), m_pCurCourse(NULL), m_roomType(0), m_roomName(), m_holes(18), m_weather(0),
	  m_shotTimeLimit(0), m_gameTimeLimit(0), m_value4CE4(8), m_string4D4C(""), m_serverProperty(0),
	  m_pGolfDoc(NULL), m_loginDlgCreated(true) {
	// The original also clears 0x4B39 (inside m_serverChannel as transcribed).
	m_serverName[0] = 0;

	ClearAll();

	m_value47CC = 0;
	m_flag47D0 = false;

	memset(m_fileName, 0, sizeof(m_fileName));

	m_replayState = 0;
	m_replayValue5BC0 = 0;
	m_replayStartA = 0;
	m_replayChannelFlags = 0;
	m_replayPlayType = -1;
	m_playingMode = (eReplayModeType)1;
	m_replayStartB = 0;
	m_flag5BD0 = false;

	m_value5BD4 = 6385;
	m_rmrMode = 0;

	m_serverTimeTick = GetTickCount();

	m_directMoveUID = 0xFFFFFFFF;
	m_myCash = 0;
	m_flag5D0C = false;
	m_goStopMode = false;
	m_outsideCash = 0;
	m_myBonusCash = 0;
	m_directMoveServer = 0xFF;
	m_directMoveRoom = 0xFFFF;
}

// shareddoc.cpp (no line marks in the original); defined after the
// CSharedDoc constructor, which calls it rather than inlining it.
sUserInfo::sUserInfo(void)
	: m_characterInfo() {
}

// shareddoc.cpp:271
CSharedDoc::~CSharedDoc(void) {
	for (std::map<unsigned long, sRoomSlot>::iterator it = WSingleton<CSharedDoc>::Instance()->m_roomSlotMap.begin(); it != m_roomSlotMap.end(); ++it) {
		if (it->second.m_pPet != 0) {
			delete it->second.m_pPet;
			it->second.m_pPet = 0;
		}
	}
	m_roomSlotMap.clear();

	ClearRivalVars(false);
	UccManager()->Clear();
}

// shareddoc.cpp:289
void CSharedDoc::ClearAll(void) {
	ClearCommonVars();
	ClearInventory();
	ClearGameVars();
	ClearRivalVars(false);
	InitGameType();
	ClearLobbyStateVars();
	InitTradeData();

	ClearOldSeasonStat();
	ClearServerVars();
	ClearJapanVars();
	memset(m_realEquip, 0, sizeof(m_realEquip));

	if (IsLocalContent((localContentType_t)0x57)) {
		ClearUcc();
	}

	if (IsLocalContent((localContentType_t)0x39)) {
		m_mapEventList.clear();
	}
	// shareddoc.cpp:313-314: m_shotDataList.clear() where local content 0x4F
	// is on; the std::list<CGolfRule::sShotData> member needs CGolfRule's
	// definition in this header (not transcribed yet).

	m_userInfoMod = 0;
}

// shareddoc.cpp:321
void CSharedDoc::ClearUcc(void) {
	m_myItemList.clear();
	m_myAuxItemList.clear();
	UccManager()->Clear();
}

// shareddoc.cpp:329
void CSharedDoc::ClearJapanVars(void) {
	m_japanState = 0;
	m_billingType = 0xFFFFFFFF;

	m_japanFlag = 0;
	m_japanParam1 = 0;
	m_japanParam2 = 0;

	char commandLine[MAX_PATH] = { 0 };

	// The command line: the exe path, the account, the login server address.
	strcpy(commandLine, GetCommandLineA());

	char args[3][MAX_PATH];
	for (int i = 0; i < 3; i++) {
		args[i][0] = 0;
	}

	char *rest;
	if (commandLine[0] == '"') {
		rest = GetStr(commandLine, args[0]);
	} else {
		sscanf(commandLine, "%s", args[0]);
		rest = commandLine + strlen(args[0]);
	}

	sscanf(rest, "%s %s", args[1], args[2]);

	char unquoted[MAX_PATH];
	if (args[1][0] != 0) {
		int len = strlen(args[1]);
		if (args[1][0] == '"' && args[1][len - 1] == '"') {
			strncpy(unquoted, args[1] + 1, len - 2);
			unquoted[len - 2] = 0;
			strcpy(args[1], unquoted);
			WSingleton<CSharedDoc>::Instance()->m_commandLineAccount = unquoted;
		} else {
			WSingleton<CSharedDoc>::Instance()->m_commandLineAccount = args[1];
		}
	}

	int len = strlen(args[2]);
	if (args[2][0] == '"' && args[2][len - 1] == '"') {
		strncpy(unquoted, args[2] + 1, len - 2);
		unquoted[len - 2] = 0;
		strcpy(args[2], unquoted);
		WSingleton<CSharedDoc>::Instance()->m_loginServerAddress = unquoted;
	} else {
		WSingleton<CSharedDoc>::Instance()->m_loginServerAddress = args[2];
	}
}

// shareddoc.cpp:418
void CSharedDoc::ClearCommonVars(void) {
	m_loginConnectValue = 0;
	m_gameOver = false;
	m_gameKind = 0;
	m_gameMode = 0;
	m_matchTypeId = 0;
	m_value456C = 0;
	m_winningTeam = 2;
	m_pangRate = 1.0f;
	m_pangPercent = 100;
	m_offlinePlay = 0;
	m_quitGamePang = 0;
	m_value4D7E = 0xFF;
	m_value4D70 = 0;
	m_value4D74 = 0;

	m_newMapEvent = false;
	m_newMapEventFlags = 0;
	m_flag4D84 = false;
	m_value4D88 = -1;

	memset(m_tutorialData, 0, sizeof(m_tutorialData));
	m_value4D98 = 15;
	m_gameServerPort = 0;
	m_value84 = 0;
	m_value80 = 0;
	m_value88 = 0;

	LoadLevelTable();

	// The hole order runs 1 to 18 and ends with 19 in the byte after the array.
	unsigned char i;
	for (i = 0; i < 18; i++) {
		m_holeRandom[i] = rand();
		m_holePin[i] = (unsigned char)(m_holeRandom[i] % 3);
		m_holeOrder[i] = i + 1;
	}
	m_holeOrder[i] = 19;

	m_noteList.clear();

	memset(&m_serverTime, 0, sizeof(m_serverTime));

	m_value4D04 = 0;

	m_logoutPending = false;
	memset(m_myAuxPartTidList, 0, sizeof(m_myAuxPartTidList));
	m_value5018 = 0;
	m_matchGameCount = 0;
	m_flag5020 = false;
	m_haveHalloweenItem = false;
	m_wearHalloweenItem = false;
	m_string4D9C = "";

	m_value4DB8 = -1;

	m_tutorialFinish = 0;
	m_value4DBD = 0;
	m_value4DBE = 0;
	m_friendMap.clear();
	memset(&m_myFriendInfo, 0, sizeof(m_myFriendInfo));
	m_value94 = 0;
	m_controlServerService = 0;
	m_serverProperty = 0;
	m_flagAbilityItem = 0;
	m_numBonusPangItem = 0;
	m_value5044 = 0;
	m_value5048 = 0;
	m_value504C = 0;
	m_value5050 = 1;
	m_value5055 = 0;
	m_value5056 = 0;
	m_value5058 = 0;
	m_value5054 = 0;
	m_value505A = 0;
	m_value5064 = 0;
	m_pointEventState = 0;
	m_pointEventFlag = false;
	m_value506C = 0;
	IsLocalContent((localContentType_t)0x39);
	m_value5D08 = 0;
	m_flag50F4 = false;
	m_flag50F6 = false;
	m_flag5BDC = false;
	m_myRoomOwnerUID = 0xFFFFFFFF;
	if (IsLocalContent((localContentType_t)0x81)) {
		m_forceNicknameChange = 0;
	}
	m_pPeriodDoc = NULL;
}

// shareddoc.cpp:560
void CSharedDoc::ClearInventory(void) {
	// The local player's block from 0x3E0 to 0xD4C (one struct in the original).
	memset(&m_mySlot, 0, 0xD4C - 0x3E0); m_myCash = 0;

	m_outsideCash = 0;
	m_myBonusCash = 0;

	m_stringMap.clear();
	m_characterMap.clear();
	m_caddieMap.clear();
	m_myMascotMap.clear();
	m_myPartsList.clear();
	m_myItemMap.clear();
	m_myGearList.clear();
	m_itemInfoList.clear();
	m_replayFileList.clear();
	m_myExtraItemList.clear();
	m_myExtraItemList2.clear();
	m_giftList.clear();
	m_mySkinList.clear();
	m_mySpecialTrophies.clear();
	m_myGuildTrophies.clear();
	m_myExtraItemList3.clear();
	m_myExtraItemList4.clear();
	m_questList.clear();
	m_furnitureList.clear();
	m_list1D0.clear();
	m_list1DC.clear();

	ClearUserInfoTimeMap(0xFFFFFFFF);
}

// shareddoc.cpp:595
void CSharedDoc::ClearGameMode(void) {
	m_gameMode = 0;
}

// shareddoc.cpp:601
void CSharedDoc::ClearUserInfoTimeMap(unsigned long uid) {
	if (uid == 0xFFFFFFFF) {
		std::map<unsigned long, sUserInfoTime>::iterator it = m_userInfoTimeMap.begin();
		for (; it != m_userInfoTimeMap.end(); ++it) {
			sUserInfoTime &info = (*it).second;
			if (info.m_pArray1 != NULL) {
				delete[] info.m_pArray1;
				info.m_pArray1 = NULL;
			}
			if (info.m_pArray2 != NULL) {
				delete[] info.m_pArray2;
				info.m_pArray2 = NULL;
			}
			if (info.m_pArray3 != NULL) {
				delete[] info.m_pArray3;
				info.m_pArray3 = NULL;
			}
			if (info.m_pArray4 != NULL) {
				delete[] info.m_pArray4;
				info.m_pArray4 = NULL;
			}
		}
		m_userInfoTimeMap.clear();
	} else {
		std::map<unsigned long, sUserInfoTime>::iterator it = m_userInfoTimeMap.find(uid);
		if (it != m_userInfoTimeMap.end()) {
			sUserInfoTime &info = (*it).second;
			if (info.m_pArray1 != NULL) {
				delete[] info.m_pArray1;
				info.m_pArray1 = NULL;
			}
			if (info.m_pArray2 != NULL) {
				delete[] info.m_pArray2;
				info.m_pArray2 = NULL;
			}
			if (info.m_pArray3 != NULL) {
				delete[] info.m_pArray3;
				info.m_pArray3 = NULL;
			}
			if (info.m_pArray4 != NULL) {
				delete[] info.m_pArray4;
				info.m_pArray4 = NULL;
			}
			m_userInfoTimeMap.erase(it);
		}
	}
}

// shareddoc.cpp:635
void CSharedDoc::ClearGameVars(void) {
	m_value90 = 0;
	m_value4843 = 0;
	m_value4844 = 0;
	memset(m_gameStatistics, 0, sizeof(m_gameStatistics));
	memset(m_teamResult, 0, sizeof(m_teamResult));
	memset(m_teamPoint, 0, sizeof(m_teamPoint));

	memset(m_value47B4, 0, sizeof(m_value47B4));

	for (int i = 3; i >= 0; i--) {
		m_gameStatistics[i].exp = 0xFFFFFFFF;
	}
	m_value4D7C = 0;
	m_value456C = 0;
	m_winningTeam = 2;
	m_indexMap.clear();
}

// shareddoc.cpp:657
void CSharedDoc::ClearRivalVars(bool keepPlayers) {
	if (!keepPlayers)
		memset(m_roomPlayers, 0, sizeof(m_roomPlayers));

	memset(m_trophyUID, 0, sizeof(m_trophyUID));

	m_gameResultList.clear();
	m_vector4524.clear();
	m_guildMatchFlags.clear();
}

// shareddoc.cpp:670
int CSharedDoc::GetBuffValue(int target, int type) {
	if (type != 0x14 && type != 0x10)
		return -1;

	return 0;
}

// shareddoc.cpp:692
void CSharedDoc::ClearOldSeasonStat(void) {
	memset(&m_oldSeasonStatistics, 0, sizeof(m_oldSeasonStatistics));
	memset(&m_oldSeasonTrophy, 0, sizeof(m_oldSeasonTrophy));

	m_specialTrophies.clear();
	m_guildTrophies.clear();
}

// shareddoc.cpp:701
void CSharedDoc::ClearStatistics(void) {
}

// shareddoc.cpp:705
void CSharedDoc::ClearCurrentChannel(bool notify) {
	memset(&m_currentChannel, 0, sizeof(m_currentChannel));
	m_currentChannel.id = 0xFF;
}

// shareddoc.cpp:711
void CSharedDoc::ClearServerVars(void) {
	memset(&m_currentChannel, 0, sizeof(m_currentChannel)); m_currentChannel.id = 0xFF;
	memset(m_serverName, 0, 0x5C); // the server's name and channel, one 0x5C-byte block up to 0x4B61
	m_gameServerList.clear();
	m_channelList.clear();
}

// shareddoc.cpp:719
void CSharedDoc::ClearLobbyStateVars(void) {
	m_lobbyMessage = "";
	m_paranNickChanged = false;
	m_noticeShown = true;
	m_lobbyStateValue = 0;
	m_3DBackgroundOff = false;
	m_lobbyStateFlag = false;
	m_lobbyStateFlag2 = false;

	memset(&m_roomInfo, 0, sizeof(m_roomInfo));

	sRoomInfo roomInfo;
	m_roomInfo = roomInfo;

	m_userMap.clear();
	m_roomList.clear();
	m_roomPlayerList.clear();
	m_roomPlayerList2.clear();
	m_roomUserList.clear();
	m_guildMembers.clear();
}

// shareddoc.cpp:742
void CSharedDoc::ClearRealEquip(void) {
	memset(m_realEquip, 0, sizeof(m_realEquip));
}

// shareddoc.cpp:747
// The game types' room names (cp949), player limits and hole counts.
void CSharedDoc::InitGameType(void) {
	// "스트로크" (stroke)
	m_gameTypeNames[0].maxPlayers = 4;
	m_gameTypeNames[0].defaultPlayers = 2;
	m_gameTypeNames[0].defaultHoles = 3;
	m_gameTypeNames[0].name = "\xBD\xBA\xC6\xAE\xB7\xCE\xC5\xA9";

	// "매치" (match)
	m_gameTypeNames[1].maxPlayers = 4;
	m_gameTypeNames[1].defaultPlayers = 2;
	m_gameTypeNames[1].defaultHoles = 6;
	m_gameTypeNames[1].name = "\xB8\xC5\xC4\xA1";

	// "개인 래더" (ladder)
	m_gameTypeNames[3].maxPlayers = 2;
	m_gameTypeNames[3].defaultPlayers = 2;
	m_gameTypeNames[3].defaultHoles = 9;
	m_gameTypeNames[3].name = "\xB0\xB3\xC0\xCE \xB7\xA1\xB4\xF5";

	// "30인 대회" (30-player tourney)
	m_gameTypeNames[4].maxPlayers = 30;
	m_gameTypeNames[4].defaultPlayers = 10;
	m_gameTypeNames[4].defaultHoles = 9;
	m_gameTypeNames[4].name = "30\xC0\xCE \xB4\xEB\xC8\xB8";

	// "30인 대회 팀전" (30-player team tourney)
	m_gameTypeNames[5].maxPlayers = 30;
	m_gameTypeNames[5].defaultPlayers = 10;
	m_gameTypeNames[5].defaultHoles = 9;
	m_gameTypeNames[5].name = "30\xC0\xCE \xB4\xEB\xC8\xB8 \xC6\xC0\xC0\xFC";

	// "길드 대전" (guild battle)
	m_gameTypeNames[6].maxPlayers = 30;
	m_gameTypeNames[6].defaultPlayers = 10;
	m_gameTypeNames[6].defaultHoles = 9;
	m_gameTypeNames[6].name = "\xB1\xE6\xB5\xE5 \xB4\xEB\xC0\xFC";

	// "공개 마이룸" (open my room)
	m_gameTypeNames[8].maxPlayers = 30;
	m_gameTypeNames[8].defaultPlayers = 1;
	m_gameTypeNames[8].defaultHoles = 1;
	m_gameTypeNames[8].name = "\xB0\xF8\xB0\xB3 \xB8\xB6\xC0\xCC\xB7\xEB";

	// "팡배틀" (pang battle)
	m_gameTypeNames[7].maxPlayers = 4;
	m_gameTypeNames[7].defaultPlayers = 2;
	m_gameTypeNames[7].defaultHoles = 6;
	m_gameTypeNames[7].name = "\xC6\xCE\xB9\xE8\xC6\xB2";

	// "어프로치" (approach)
	m_gameTypeNames[9].maxPlayers = 30;
	m_gameTypeNames[9].defaultPlayers = 10;
	m_gameTypeNames[9].defaultHoles = 1;
	m_gameTypeNames[9].name = "\xBE\xEE\xC7\xC1\xB7\xCE\xC4\xA1";

	m_gameTypeNames[10].maxPlayers = 30;
	m_gameTypeNames[10].defaultPlayers = 6;
	m_gameTypeNames[10].defaultHoles = 3;
	m_gameTypeNames[10].name = "\xBE\xEE\xC7\xC1\xB7\xCE\xC4\xA1";

	// "대화방" (chat room)
	m_gameTypeNames[2].maxPlayers = 30;
	m_gameTypeNames[2].defaultPlayers = 10;
	m_gameTypeNames[2].defaultHoles = 1;
	m_gameTypeNames[2].name = "\xB4\xEB\xC8\xAD\xB9\xE6";

	// "처음 시작하기" (first steps)
	m_gameTypeNames[11].maxPlayers = 1;
	m_gameTypeNames[11].defaultPlayers = 1;
	m_gameTypeNames[11].defaultHoles = 1;
	m_gameTypeNames[11].name = "\xC3\xB3\xC0\xBD \xBD\xC3\xC0\xDB\xC7\xCF\xB1\xE2";

	// "캐디와 배우기" (learn with the caddie)
	m_gameTypeNames[12].maxPlayers = 1;
	m_gameTypeNames[12].defaultPlayers = 1;
	m_gameTypeNames[12].defaultHoles = 1;
	m_gameTypeNames[12].name = "\xC4\xB3\xB5\xF0\xBF\xCD \xB9\xE8\xBF\xEC\xB1\xE2";

	// "스트로크" (stroke)
	m_gameTypeNames[13].maxPlayers = 0xFF;
	m_gameTypeNames[13].defaultPlayers = 1;
	m_gameTypeNames[13].defaultHoles = 18;
	m_gameTypeNames[13].name = "\xBD\xBA\xC6\xAE\xB7\xCE\xC5\xA9";

	// "불가사의한 혼돈" (mysterious chaos)
	m_gameTypeNames[14].maxPlayers = 30;
	m_gameTypeNames[14].defaultPlayers = 30;
	m_gameTypeNames[14].defaultHoles = 18;
	m_gameTypeNames[14].name = "\xBA\xD2\xB0\xA1\xBB\xE7\xC0\xC7\xC7\xD1 \xC8\xA5\xB5\xB7";
}

// shareddoc.cpp:827
void CSharedDoc::LoadLevelTable(void) {
	for (int i = 0; i < 71; i++) {
		m_levelExp[i].m_exp = data_9e6468[i][0];
		m_levelExp[i].m_totalExp = m_levelExp[i].m_exp + (i == 0 ? 0 : m_levelExp[i - 1].m_totalExp);
	}

	m_caddieLevelExp[0].m_exp = 0x208;
	m_caddieLevelExp[0].m_value4 = 0x208;
	m_caddieLevelExp[1].m_exp = 0x2A8;
	m_caddieLevelExp[1].m_value4 = 0x4B0;
	m_caddieLevelExp[2].m_exp = 0x348;
	m_caddieLevelExp[2].m_value4 = 0x7F8;
}

// shareddoc.cpp:843
void CSharedDoc::LoadItemDb(void) {
	m_itemManager.Load();
	CPartTidList::SetItemManager(&m_itemManager);
}

// shareddoc.cpp:850
void CSharedDoc::InitGolfDoc(void) {
	IFF_STRUCT::sCourse *course = WSingleton<CSharedDoc>::Instance()->m_itemManager.FindCourse(0x28000000);
	if (course != 0) {
		WSingleton<CSharedDoc>::Instance()->m_curMap = 0;
		WSingleton<CSharedDoc>::Instance()->m_pCurCourse = course;
	}
	WSingleton<CSharedDoc>::Instance()->m_roomType = 0;
	strcpy(WSingleton<CSharedDoc>::Instance()->m_roomName, WSingleton<CSharedDoc>::Instance()->m_gameTypeNames[0].name);
	WSingleton<CSharedDoc>::Instance()->m_gameKind = 0;
	WSingleton<CSharedDoc>::Instance()->m_holes = 18;
	WSingleton<CSharedDoc>::Instance()->m_weather = 0;
	WSingleton<CSharedDoc>::Instance()->m_shotTimeLimit = 40000;
	WSingleton<CSharedDoc>::Instance()->m_gameTimeLimit = 0;
}

// shareddoc.cpp:861
void CSharedDoc::LoadChatFilter(void) {
	m_chatManager.Load();
}

// shareddoc.cpp:866, whether the player with the UID is in the current game
// (the golf doc's players) or in its results (the result kinds).
bool CSharedDoc::AreWePlayingTogether(unsigned long uid) {
	switch (m_roomType) {
	case 0:
	case 1:
	case 3:
	case 7:
		if (GOLFDOC() == 0)
			break;

		for (unsigned char i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
			if (GOLFDOC()->GetPlayer(i)->m_state == 3)
				continue;

			if (GOLFDOC()->GetPlayer(i)->m_uid == uid)
				return true;
		}
		break;

	case 4:
	case 5:
	case 6:
	case 9:
	case 10:
		for (std::vector<sGameResultInfo>::iterator it = m_gameResultList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++it) {
			if (it->m_uid == uid)
				return it->m_state != 3;
		}
		break;
	}

	return false;
}

// shareddoc.cpp:1021, whether the player holds every material of the compound
// recipe in the needed count.
bool CSharedDoc::CanCompound(unsigned long typeId) {
	IFF_STRUCT::sQuest *pQuest = m_itemManager.FindQuest(typeId);
	if (pQuest == 0) {
		return false;
	}

	for (int i = 0; i < 5; i++) {
		unsigned long materialId = pQuest->m_materialIds[i];
		if (materialId == 0) {
			continue;
		}

		std::list<sItemInfo>::iterator it;
		for (it = m_myExtraItemList4.begin(); it != m_myExtraItemList4.end(); ++it) {
			if (it->m_typeId == materialId) {
				if (it->m_count < pQuest->m_materialCounts[i]) {
					return false;
				}
				break;
			}
		}

		if (it == m_myExtraItemList4.end()) {
			return false;
		}
	}

	return true;
}

// shareddoc.cpp:1054, the sum (property 0) or product (properties 1-3, in
// percent) of what the aux parts a player wears add to a property.
float CSharedDoc::GetAuxPartProperty(unsigned char team, unsigned char property) {
	if (m_pGolfDoc != 0 && m_pGolfDoc->GetPlayerNum() <= team) {
		if (property == 0) {
			return 0.0f;
		}

		return 1.0f;
	}

	float value = (property == 0) ? 0.0f : 1.0f;
	for (int i = 0; i < 5; i++) {
		unsigned long typeId = m_roomPlayers[team].m_characterInfo.auxParts.tids[i];
		if (typeId == 0) {
			continue;
		}

		IFF_STRUCT::sAuxPart *pAuxPart = m_itemManager.FindAuxPart(typeId);
		if (pAuxPart == 0) {
			continue;
		}

		switch (property) {
		case 0:
			value += (float)pAuxPart->m_propertyAdd;
			break;
		case 1:
			if (pAuxPart->m_propertyRate[0] > 0) {
				value = (float)pAuxPart->m_propertyRate[0] * value * 0.01f;
			}
			break;
		case 2:
			if (pAuxPart->m_propertyRate[1] > 0) {
				value = (float)pAuxPart->m_propertyRate[1] * value * 0.01f;
			}
			break;
		case 3:
			if (pAuxPart->m_propertyRate[2] > 0) {
				value = (float)pAuxPart->m_propertyRate[2] * value * 0.01f;
			}
			break;
		case 4:
			if ((pAuxPart->m_typeId & 0xFFFF) == 9) {
				value = 9.0f;
			} else if (IsLocalContent((localContentType_t)0x4B) && (pAuxPart->m_typeId & 0xFFFF) == 0x0B) {
				value = 11.0f;
			}
			break;
		}
	}

	return value;
}

// shareddoc.cpp:1167, whether the player of a team (every player of the team
// in a team room) wears the part.
bool CSharedDoc::FindEquipPart(unsigned char team, unsigned long typeId) {
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1) {
		for (unsigned char i = 0; i < m_pGolfDoc->GetPlayerNum(); i++) {
			if (m_pGolfDoc->GetPlayer(i)->m_attribute == team) {
				for (int j = 0; j < 24; j++) {
					if (m_roomPlayers[i].m_characterInfo.partTypeIds[j] == typeId) {
						return true;
					}
				}
			}
		}

		return false;
	}

	for (int j = 0; j < 24; j++) {
		if (m_roomPlayers[team].m_characterInfo.partTypeIds[j] == typeId) {
			return true;
		}
	}

	return false;
}

// shareddoc.cpp:1212, the combo gauge's limit for a team: 99, 33 more when one
// of its players wears one of the parts below.
float CSharedDoc::GetComboGaugeLimit(unsigned char team) {
	static unsigned long comboParts[] = {
		0x08008816, 0x08008817,
		0x08046822, 0x08046823,
		0x08088816, 0x08088817,
		0x080C8027, 0x080C8028,
		0x08106019, 0x0810601A,
		0x08148025, 0x08148026,
		0x0818601D, 0x0818601E,
		0x081C801A, 0x081C801B,
		0x08208026, 0x08208027,
		0x08248026, 0x08248027,
		0x08008818, 0x08008819,
		0x08046828, 0x08046829,
		0x0808881B, 0x0808881C,
		0x080C802E, 0x080C802F,
		0x0810601C, 0x0810601D,
		0x0814802D, 0x0814802E,
		0x08186027, 0x08186028,
		0x081C801C, 0x081C801D,
		0x0820802F, 0x08208030,
		0x0824802B, 0x0824802C,
	};

	float bonus = 0.0f;
	for (unsigned int i = 0; i < 40; i++) {
		if (FindEquipPart(team, comboParts[i])) {
			bonus = 33.0f;
			break;
		}
	}

	return bonus + 99.0f;
}

// shareddoc.cpp:1249, adds an owned character; a part combination the
// character cannot wear is replaced by the default parts.
void CSharedDoc::AddChar(sCharacterInfo *pChar) {
	CPartTidList tidList;

	if (!tidList.SetTids(pChar, 0xFF) || !tidList.IsComboValid()) {
		tidList.SetDefaultTids();
		memcpy(pChar->partTypeIds, tidList.m_partTids, sizeof(pChar->partTypeIds));
	}

	m_characterMap.insert(std::map<unsigned int, sCharacterInfo>::value_type(pChar->id, *pChar));
}

// shareddoc.cpp:1277
void CSharedDoc::AddCaddie(sCaddieInfo *pCaddie) {
	m_caddieMap.insert(std::map<unsigned int, sCaddieInfo>::value_type(pCaddie->id, *pCaddie));
}

// shareddoc.cpp:1282, adds an owned mascot or updates the one with its id.
void CSharedDoc::AddMascot(sMascotInfo *pMascot) {
	std::map<unsigned int, sMascotInfo>::iterator it = m_myMascotMap.find(pMascot->id);

	if (it != m_myMascotMap.end()) {
		it->second = *pMascot;
	} else {
		m_myMascotMap.insert(std::map<unsigned int, sMascotInfo>::value_type(pMascot->id, *pMascot));
	}
}

// shareddoc.cpp:1297
sCardStack *CSharedDoc::GetCardInfoToUID(unsigned long uid) {
	std::list<sCardStack>::iterator it;
	for (it = WSingleton<CSharedDoc>::Instance()->m_cardList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_cardList.end(); ++it) {
		sCardStack *pCard = &*it;
		if (uid == pCard->uid && pCard->valid == 1) {
			return pCard;
		}
	}
	return 0;
}

// shareddoc.cpp:1312
sCardStack *CSharedDoc::GetCardInfoToTID(unsigned long typeId) {
	std::list<sCardStack>::iterator it;
	for (it = WSingleton<CSharedDoc>::Instance()->m_cardList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_cardList.end(); ++it) {
		sCardStack *pCard = &*it;
		if (typeId == pCard->typeId && pCard->valid == 1) {
			return pCard;
		}
	}
	return 0;
}

// shareddoc.cpp:1349
void CSharedDoc::DeleteCardToPartsAttach(unsigned long cardId) {
	std::list<sSCardAvilityPeriodInfo> &cards = WSingleton<CSharedDoc>::Instance()->m_cardPeriodList[0];
	for (std::list<sSCardAvilityPeriodInfo>::iterator it = cards.begin(); it != cards.end(); ++it) {
		if (it->partsId == cardId) {
			m_cardPeriodList[0].erase(it);
			break;
		}
	}
}

// shareddoc.cpp:1440
void CSharedDoc::AddFurniture(const sFurniture_List &furniture) {
	m_furnitureList.push_back(furniture);
}

// shareddoc.cpp:1445
void CSharedDoc::AddMyPartsList(const sItemInfo &item) {
	m_myPartsList.push_back(item);
}

// shareddoc.cpp:1459
sMascotInfo *CSharedDoc::GetMyMascotInfo(unsigned long id) {
	if (id) {
		std::map<unsigned int, sMascotInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myMascotMap.find(id);
		if (it != WSingleton<CSharedDoc>::Instance()->m_myMascotMap.end()) {
			return &it->second;
		}
	}
	return 0;
}

// shareddoc.cpp:1474
sMascotInfo *CSharedDoc::GetMyEquippedMascotInfo(void) {
	if (m_myMascotId) {
		std::map<unsigned int, sMascotInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myMascotMap.find(m_myMascotId);
		if (it != WSingleton<CSharedDoc>::Instance()->m_myMascotMap.end()) {
			return &it->second;
		}
	}
	return 0;
}

// shareddoc.cpp:1610, takes the local player's equipment; a caddie or mascot
// the player does not own is replaced (the first owned caddie) or cleared.
void CSharedDoc::SetCurEquipInfo(sUserEquip *equip) {
	memcpy(&m_myCaddieId, equip, sizeof(sUserEquip));
	BuildMyPartTidList();

	if (m_myCaddieId != 0) {
		if (m_myCaddieId != 0xFFFFFFFF) {
			std::map<unsigned int, sCaddieInfo>::iterator it = m_caddieMap.find(m_myCaddieId);

			if (it == m_caddieMap.end()) {
				if (m_caddieMap.size() > 0) {
					m_myCaddieId = m_caddieMap.begin()->first;
				}
			}
		}
	}

	if (m_myMascotId != 0) {
		if (m_myMascotId != 0xFFFFFFFF) {
			std::map<unsigned int, sMascotInfo>::iterator it = m_myMascotMap.find(m_myMascotId);

			if (it == m_myMascotMap.end()) {
				m_myMascotId = 0;
			}
		}
	}
}

// shareddoc.cpp:1787, builds the local player's part list from the equipped
// character; with no owned character of that id, from the default parts of
// character typeid 0x4000000 | (eventFlags bit 1).
void CSharedDoc::BuildMyPartTidList(void) {
	sCharacterInfo defaultChar = sCharacterInfo();
	sCharacterInfo *pChar;
	std::map<unsigned int, sCharacterInfo>::iterator it = m_characterMap.find(m_myCharacterId);

	if (it == m_characterMap.end()) {
		memset(&defaultChar, 0, sizeof(defaultChar));
		defaultChar.typeId = 0x4000000 | (WSingleton<CSharedDoc>::Instance()->m_userInfo.eventFlags >> 1 & 1) % 2;
		WSingleton<CSharedDoc>::Instance()->m_itemManager.GetDefCombo(defaultChar.typeId, defaultChar.partTypeIds);
		pChar = &defaultChar;
	} else {
		pChar = &it->second;
	}

	if (!m_myPartTidList.SetTids(pChar, WSingleton<CSharedDoc>::Instance()->m_myLevel)) {
		m_myPartTidList.SetDefaultTids();
	}

	if (!m_myPartTidList.IsComboValid()) {
		m_myPartTidList.SetDefaultTids();
	}

	memcpy(m_myAuxPartTidList, &pChar->auxParts, sizeof(m_myAuxPartTidList));
}

// shareddoc.cpp:1972
CPartTidList &CSharedDoc::GetMyPartTidList(void) {
	return m_myPartTidList;
}

// shareddoc.cpp:1976
unsigned long *CSharedDoc::GetMyAuxPartTidList(void) {
	return m_myAuxPartTidList;
}

// shareddoc.cpp:1981
void CSharedDoc::SetIndex(unsigned long uid) {
	if (m_indexMap.find(uid) == m_indexMap.end()) {
		unsigned char index = (unsigned char)m_indexMap.size();

		if (OnlinePlay()) {
			if (IsMassGame()) {
				if (uid == m_userInfo.guid)
					memcpy(m_roomPlayers[0].m_nickname, &m_userInfo, sizeof(m_userInfo));
			}
		}

		m_indexMap[uid] = index;
	}
}

// shareddoc.cpp:2009
bool CompareItemInfo(const sItemInfo &a, const sItemInfo &b) {
	return a.m_typeId < b.m_typeId;
}

// shareddoc.cpp:2014
void CSharedDoc::SortAllMyItemList(void) {
	m_myPartsList.sort(CompareItemInfo);
	m_myGearList.sort(CompareItemInfo);
	m_itemInfoList.sort(CompareItemInfo);
	m_myExtraItemList.sort(CompareItemInfo);
	m_myExtraItemList2.sort(CompareItemInfo);
}

// shareddoc.cpp:2024, whether the player can receive count more of the item:
// a stackable item (type class 5 or 6) holds at most 20000.
bool CSharedDoc::ConfirmInsertItem(unsigned long typeId, unsigned long count) const {
	unsigned long typeClass = typeId >> 26;

	if (typeClass >= 5 && typeClass <= 6 && typeId != 0x1A000010) {
		std::list<sItemInfo>::const_iterator it;
		for (it = m_itemInfoList.begin(); it != m_itemInfoList.end(); ++it) {
			sItemInfo item = *it;
			if (item.m_typeId == typeId) {
				sItemInfo found = *it;
				return found.m_count + count <= 20000;
			}
		}

		return true;
	}

	return true;
}

// shareddoc.cpp:2085
void CSharedDoc::AddMyItem(const sItemInfo &item) {
	WSingleton<CSharedDoc>::Instance()->m_itemInfoList.push_back(item);
}

// shareddoc.cpp:2092
bool CSharedDoc::DecreaseMyItem(std::list<sItemInfo>::iterator &it) {
	sItemInfo *pItem = &(*it);
	pItem->m_count--;
	if (pItem->m_count <= 0)
		m_itemInfoList.erase(it);

	return true;
}

// shareddoc.cpp:2117
bool CSharedDoc::DeleteMyItem(std::list<sItemInfo>::iterator &it) {
	if (it == m_itemInfoList.end()) {
		return false;
	}

	m_itemInfoList.erase(it);
	return true;
}

// shareddoc.cpp:2128
sItemInfo *CSharedDoc::FindMyItemByGuid(unsigned long guid) {
	for (std::list<sItemInfo>::iterator it = m_itemInfoList.begin(); it != m_itemInfoList.end(); ++it) {
		if ((*it).m_serial == guid)
			return &*it;
	}

	return NULL;
}

// shareddoc.cpp:2146
void CSharedDoc::ClearMyGuildInfo(void) {
	m_userInfo.guildUID = 0;
	m_userInfo.guildPosition = 0;
	memset(m_userInfo.guildName, 0, sizeof(m_userInfo.guildName));
	memset(m_userInfo.guildMark, 0, sizeof(m_userInfo.guildMark));
}

// shareddoc.cpp:2312
const char *CSharedDoc::GetDefaultTitle(unsigned char gameType) {
	const char **titles;
	int index;

	switch (gameType) {
	case 0:
	case 1:
		index = rand() % 9;
		titles = defaultTitles_vs;
		break;

	case 4:
	case 5:
	case 6:
	case 0xE:
		index = rand() % 6;
		titles = defaultTitles_mass;
		break;

	case 7:
		index = rand() % 4;
		titles = defaultTitles_battle;
		break;

	case 0xA:
		index = rand() % 3;
		titles = defaultTitles_approach;
		break;

	case 2:
	case 8:
		index = rand() % 7;
		titles = defaultTitles_chat;
		break;

	default:
		return "No Title!";
	}

	return titles[index];
}

// shareddoc.cpp:2632
void CSharedDoc::LogGstUpdateHole(unsigned long uid, unsigned char hole, int score, __int64 pang) {
}

// shareddoc.cpp:2893
bool CSharedDoc::InitRecentWhisperList(FrComboBox *pComboBox) {
	m_recentWhisperList.clear();
	return UpdateChatTarget(pComboBox);
}

// shareddoc.cpp:2899, the chat target names are "to team", "to all" and "to
// guild" (cp949).
bool CSharedDoc::InitReservedChatList(FrComboBox *pComboBox, eReservedChatPartner partner) {
	if (pComboBox == 0) {
		return false;
	}

	m_list4CD8.clear();
	if (m_string4CE8.size() > 0) {
		pComboBox->SetLine(1, m_string4CE8.c_str(), 0, false, 0);
	}

	if (partner & RESERVED_CHAT_PARTNER_TEAM) {
		m_list4CD8.push_back(std::string("ÆÀ¿¡°Ô"));
	} else if (strcmpi(m_string4CE8.c_str(), "ÆÀ¿¡°Ô") == 0) {
		m_string4CE8 = "¸ðµÎ¿¡°Ô";
		pComboBox->SetLine(1, "¸ðµÎ¿¡°Ô", 0, false, 0);
	}

	if (partner & RESERVED_CHAT_PARTNER_GUILD) {
		m_list4CD8.push_back(std::string("¸ðµÎ¿¡°Ô"));
	}

	if (WSingleton<CSharedDoc>::Instance()->m_userInfo.guildUID != 0) {
		m_list4CD8.push_back(std::string("±æµå¿¡°Ô"));
	}

	if (strlen(pComboBox->GetLine(1, false)) == 0) {
		pComboBox->SetLine(1, "¸ðµÎ¿¡°Ô", 0, false, 0);
	}

	return UpdateChatTarget(pComboBox);
}

// shareddoc.cpp:3024, refills the combo box with the recent whispers, then
// the reserved partners.
bool CSharedDoc::UpdateChatTarget(FrComboBox *pComboBox) {
	if (pComboBox == 0) {
		return false;
	}

	pComboBox->ClearList();

	for (std::list<std::string>::iterator it = m_recentWhisperList.begin(); it != m_recentWhisperList.end(); ++it) {
		pComboBox->AddString((*it).c_str());
	}

	for (std::list<std::string>::iterator it = m_list4CD8.begin(); it != m_list4CD8.end(); ++it) {
		pComboBox->AddString((*it).c_str());
	}

	return true;
}

// shareddoc.cpp:3085, the chat target before target: the reserved partners
// follow the recent whispers, and the first whisper wraps to the last
// reserved partner.
const char *CSharedDoc::GetPrevChatTarget(const char *target) {
	std::list<std::string>::iterator it = std::find(m_list4CD8.begin(), m_list4CD8.end(), target);

	if (it != m_list4CD8.end()) {
		if (it != m_list4CD8.begin()) {
			--it;
			return it->c_str();
		}

		if (m_recentWhisperList.size() > 0) {
			it = m_recentWhisperList.end();
			--it;
			return it->c_str();
		}
	} else if (m_recentWhisperList.size() > 0) {
		it = std::find(m_recentWhisperList.begin(), m_recentWhisperList.end(), target);

		if (it != m_recentWhisperList.end()) {
			if (it != m_recentWhisperList.begin()) {
				--it;
			}
			return it->c_str();
		}
	}

	it = m_list4CD8.end();
	--it;
	return it->c_str();
}

// shareddoc.cpp:3122, the chat target after target ("" when none follows).
const char *CSharedDoc::GetNextChatTarget(const char *target) {
	std::list<std::string>::iterator it = std::find(m_recentWhisperList.begin(), m_recentWhisperList.end(), target);

	if (it != m_recentWhisperList.end()) {
		++it;
		if (it != m_recentWhisperList.end()) {
			return it->c_str();
		}

		if (m_list4CD8.size() > 0) {
			return m_list4CD8.begin()->c_str();
		}
	} else if (m_list4CD8.size() > 0) {
		it = std::find(m_list4CD8.begin(), m_list4CD8.end(), target);

		if (it != m_list4CD8.end()) {
			++it;
			if (it != m_list4CD8.end()) {
				return it->c_str();
			}
		}
	}

	return "";
}

// shareddoc.cpp:3175
void CSharedDoc::SetInitHaveHalloweenItem(void) {
	m_haveHalloweenItem = false;
	for (std::list<sItemInfo>::iterator it = m_myPartsList.begin(); it != m_myPartsList.end(); ++it) {
		if (((*it).m_typeId & 0x1800) == 0x1800) {
			m_haveHalloweenItem = true;
			break;
		}
	}
}

// shareddoc.cpp:3225
int CSharedDoc::IsControlServerService(int service) {
	return m_controlServerService & service;
}

// shareddoc.cpp:3254
void CSharedDoc::InitTradeData(void) {
	m_stateTrade = STATE_TRADE_NONE;
	m_listTradeItem.clear();


	m_tradeUID = 0xFFFFFFFF; m_tradeTitle = "";
	m_countVisitor = 0;
	m_tradeIncome = 0;


	m_tradeMode = 0;
}

// shareddoc.cpp:3269
void CSharedDoc::SetTradeUID(unsigned long uid) {
	if (uid != 0xFFFFFFFF) {
		m_tradeUID = uid;
	}
}

// shareddoc.cpp:3453
bool CSharedDoc::SendTradeOpenShop(void) {
	if (m_stateTrade == STATE_TRADE_READY && m_listTradeItem.size() > 0) {
		WSendPacket packet((enumClientPacket)0x74);
		packet.Send(SEND_TO_DEFAULT);

		return true;
	}

	return false;
}

// shareddoc.cpp:3465
bool CSharedDoc::SendTradeCloseShop(void) {
	if (m_stateTrade == STATE_TRADE_NONE) return false;

	WSendPacket packet((enumClientPacket)0x75);
	packet.Send(SEND_TO_DEFAULT);

	return true;
}

// shareddoc.cpp:3476
bool CSharedDoc::SendTradeEditShop(void) {
	WSendPacket packet((enumClientPacket)0x76);
	packet.Send(SEND_TO_DEFAULT);

	return true;
}

// shareddoc.cpp:3511
bool CSharedDoc::SendTradeTitle(std::string title) {
	if (title.length() <= 0) return false;

	WSendPacket packet((enumClientPacket)0x79);
	packet.EncodeStr(title);
	packet.Send(SEND_TO_DEFAULT);

	return true;
}

// shareddoc.cpp:3748
bool CSharedDoc::IsOverlapPart(unsigned long typeId) {
	for (std::list<sItemInfo>::iterator it = m_myPartsList.begin(); it != m_myPartsList.end(); ++it) {
		if ((*it).m_typeId == typeId)
			return true;
	}

	return false;
}

// shareddoc.cpp:3763
int OnlinePlay(void) {
	if (!WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT))
		return 0;

	if (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&CLobbyTask::m_RTTI) : false)
		|| ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&CShopTask::m_RTTI) : false))
		return 1;

	if (WSingleton<CSharedDoc>::Instance()->m_gameMode != 0
		|| WSingleton<CSharedDoc>::Instance()->m_roomType == 0x0B || WSingleton<CSharedDoc>::Instance()->m_roomType == 0x0C)
		return 0;

	return 1;
}

// shareddoc.cpp:3777
unsigned char GetCurMap(void) {
	unsigned char map = WSingleton<CSharedDoc>::Instance()->m_curMap;
	return (map > 0x7F && map != 0xFD) ? map - 0x80 : map;
}

// shareddoc.cpp:3796
bool SetCurMap(unsigned char map) {
	unsigned long id = (map > 0x7F && map != 0xFD) ? map - 0x80 : map;
	IFF_STRUCT::sCourse *course = WSingleton<CSharedDoc>::Instance()->m_itemManager.FindCourse(id | 0x28000000);

	if (!course)
		return false;
	WSingleton<CSharedDoc>::Instance()->m_curMap = map;
	WSingleton<CSharedDoc>::Instance()->m_pCurCourse = course;
	return true;
}

// shareddoc.cpp:3782
bool IsCurMapSnow(void) {
	unsigned char map = GetCurMap();

	if (map == 8)
		return true;
	else if (map == 14)
		return true;
	else if (map == 17)
		return true;

	return false;
}

// shareddoc.cpp:3810
std::string CSharedDoc::GetLoginAuthKey(void) {
	return WSingleton<CSharedDoc>::Instance()->m_loginAuthKey;
}

// shareddoc.cpp:3814
unsigned short CSharedDoc::GetSecurityKey(void) {
	return Instance()->m_securityKey;
}

// shareddoc.cpp:3819
void CSharedDoc::SetLoginAuthKey(std::string key) {
	WSingleton<CSharedDoc>::Instance()->m_loginAuthKey = key;
}

// shareddoc.cpp:3824
void CSharedDoc::SetSecurityKey(unsigned short key) {
	Instance()->m_securityKey = key;
}

// shareddoc.cpp:3996, reads the visibility groups: each line of the file is a
// group name followed by the names of its members.
void CSharedDoc::LoadVisGroup(const char *fileName) {
	cFile *file = g_resrcmng->GetCFile(fileName, 0xFFFF);
	if (file == 0) {
		return;
	}

	std::vector<std::string> tokens;
	int length = file->Length();
	char *data = new char[length + 1];
	file->Read(data, length);
	CloseCFile(file);

	data[length] = 0;
	char *cursor = data;
	for (;;) {
		while (*cursor == '\n' || *cursor == '\r') {
			cursor++;
		}
		if (*cursor == 0) {
			break;
		}

		char line[0x100];
		char *dest = line;
		while (*cursor != '\n' && *cursor != '\r' && *cursor != 0) {
			*dest++ = *cursor++;
		}
		*dest = 0;

		LineParse(line, tokens);
		if (tokens.size() > 1) {
			std::list<std::string> &group = m_visGroups[tokens[0]];
			for (size_t i = 1; i < tokens.size(); i++) {
				group.push_back(tokens[i]);
			}
		}
	}
	delete[] data;
}

// shareddoc.cpp:4048
void CSharedDoc::SetFileName(char *const fileName) {
	strcpy(m_fileName, fileName);
}

// shareddoc.cpp:4118
std::list<std::string> *CSharedDoc::GetVisGroup(const char *name) {
	std::map<std::string, std::list<std::string> >::iterator it = m_visGroups.find(std::string(name));
	if (it == m_visGroups.end()) {
		return 0;
	}

	return &it->second;
}

// shareddoc.cpp:4129, false when the club set needs a level above the
// local player's.
bool CSharedDoc::CheckUnderLevelClubSet(unsigned long typeId) {
	bool result = true;

	IFF_STRUCT::sClubSet *pClubSet = WSingleton<CSharedDoc>::Instance()->m_itemManager.FindClubSet(typeId);

	if (pClubSet->m_level & 0x80) {
		if ((pClubSet->m_level & 0x7F) < m_myLevel)
			return false;
	}

	return result;
}

// shareddoc.cpp:4154
void CSharedDoc::InitMapEvent(void) {
}

// shareddoc.cpp:4173
bool CSharedDoc::IsMapEventActive(int map) {
	return false;
}

// shareddoc.cpp:4190
unsigned long CSharedDoc::GetMapEventPangRate(int map) {
	return 100;
}

// shareddoc.cpp:4204
unsigned long CSharedDoc::GetMapEventExpRate(int map) {
	return 100;
}

// shareddoc.cpp:4245
void CSharedDoc::SetIdentity(int identity) {
	m_userInfo.identity = identity;
}

// shareddoc.cpp:4250
void CSharedDoc::AccumulateIdentity(int identity) {
	m_userInfo.identity |= identity;
}

// shareddoc.cpp:4256
__int64 CSharedDoc::RemainOwnCash(unsigned long buyType, __int64 price) {
	__int64 remain = WSingleton<CSharedDoc>::Instance()->m_myCash;
	switch (buyType) {
	case 2:
	case 4:
		remain -= price;
		break;
	}

	return remain;
}

// shareddoc.cpp:4301
void CSharedDoc::RefreshAllCookie(void) {
	WSingleton<CSharedDoc>::Instance()->m_myCash = S5::ExchangeOwnCashToCookie(WSingleton<CSharedDoc>::Instance()->m_outsideCash, WSingleton<CSharedDoc>::Instance()->m_myBonusCash, 100);
}

// shareddoc.cpp:4307
bool CSharedDoc::CanUseRookieChannelMap(unsigned int map) {
	bool canUse = true;
	IFF_STRUCT::sCourse *pCourse = WSingleton<CSharedDoc>::Instance()->m_itemManager.FindCourse(((map > 0x7F && map != 0xFD) ? map - 0x80 : map) | 0x28000000);
	if (pCourse) {
		if ((pCourse->courseFlags & 0xF) > 3) {
			return false;
		}
	}

	if (map == 8 || map == 1 || map == 9 || map == 0xE || map == 6)
		canUse = false;

	return canUse;
}

// shareddoc.cpp:4330
void CSharedDoc::SetServerTime(_SYSTEMTIME &time) {
	WSingleton<CSharedDoc>::Instance()->m_serverTime = time;

	if (IsLocalContent((localContentType_t)0x51)) {
		if (WSingleton<CCardManager>::IsInstantiated())
			WSingleton<CCardManager>::Instance()->ResetSpecialBuffTick();
	}

	WSingleton<CSharedDoc>::Instance()->m_serverTimeTick = GetTickCount();
}

// shareddoc.cpp:4344
_SYSTEMTIME &CSharedDoc::GetServerTime(void) {
	return Instance()->m_serverTime;
}

// shareddoc.cpp:4350
void CSharedDoc::SetDirectMoveRoomInfo(unsigned char server, unsigned short room, unsigned long uid) {
	m_directMoveServer = server;
	m_directMoveRoom = room;
	m_directMoveUID = uid;
}

// shareddoc.cpp:4357
bool CSharedDoc::GetDirectMoveRoomInfo(unsigned char *server, unsigned short *room, unsigned long *uid) {
	if (m_directMoveUID == 0xFFFFFFFF || m_directMoveRoom == 0xFFFF)
		return false;

	*server = m_directMoveServer;
	*room = m_directMoveRoom;
	*uid = m_directMoveUID;

	return true;
}

// shareddoc.cpp:4370
void CSharedDoc::ClearDirectMoveRoomInfo(void) {
	m_directMoveServer = 0xFF;
	m_directMoveRoom = 0xFFFF;
	m_directMoveUID = 0xFFFFFFFF;
}

// shareddoc.cpp:4377
int CSharedDoc::FindUserInfoTimeUID(unsigned long uid, std::map<unsigned long, sUserInfoTime>::iterator &it) {
	it = m_userInfoTimeMap.find(uid);
	return it == m_userInfoTimeMap.end() ? FALSE : TRUE;
}

// shareddoc.cpp:4386, the map has no GUID key: a linear search.
int CSharedDoc::FindUserInfoTimeGUID(unsigned long guid, std::map<unsigned long, sUserInfoTime>::iterator &it) {
	if (m_userInfoTimeMap.size() > 0) {
		for (std::map<unsigned long, sUserInfoTime>::iterator i = m_userInfoTimeMap.begin(); i != m_userInfoTimeMap.end(); ++i) {
			if (i->second.m_userInfo.m_guid == guid) {
				it = i;
				return TRUE;
			}
		}
	}

	it = m_userInfoTimeMap.end();
	return FALSE;
}

// shareddoc.cpp:4405
int CSharedDoc::FindUserInfo(unsigned long key, std::map<unsigned long, sBriefUserInfo>::iterator &it) {
	int count = m_userMap.size();

	if (count) {
		std::map<unsigned long, sBriefUserInfo>::iterator found = m_userMap.find(key);
		if (found != m_userMap.end()) {
			it = found;
			return TRUE;
		}
	}

	return FALSE;
}

// shareddoc.cpp:4423
int CSharedDoc::FindUserInfoUID(unsigned long uid, std::map<unsigned long, sBriefUserInfo>::iterator &it) {
	if (m_userMap.size() == 0)
		return FALSE;

	std::map<unsigned long, sBriefUserInfo>::iterator i = m_userMap.begin();
	for (; i != m_userMap.end(); ++i) {
		if (uid == (*i).second.id) {
			it = i;
			return TRUE;
		}
	}

	return FALSE;
}

// shareddoc.cpp:4466
int CSharedDoc::InsertUserInfoTime(unsigned long uid, sUserInfoTime &info) {
	m_userInfoTimeMap[uid] = info;
	return TRUE;
}

// shareddoc.cpp:4488
CPeriodContents *CSharedDoc::GetPeriodDoc(void) {
	return m_pPeriodDoc;
}

// shareddoc.cpp:4499
void CSharedDoc::SetPeriodDoc(void) {
	m_pPeriodDoc = NULL;
	WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x92, m_pPeriodDoc);
}

// The approach missions played blind (IsCurrentMissionBlind).
static const int s_blindMissions[] = { 4, 0x15 };

// shareddoc.cpp:4506
bool CSharedDoc::IsCurrentMissionBlind(void) {
	bool blind = false;
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
		for (unsigned int i = 0; i < sizeof(s_blindMissions) / sizeof(s_blindMissions[0]); i++) {
			if (s_blindMissions[i] == GOLFDOC()->GetApproachMissionType())
				return true;
		}
	}

	return blind;
}

// shareddoc.cpp:4565
bool CSharedDoc::IsEquipParts(sCharacterInfo &characterInfo, unsigned long itemId, unsigned long typeId) {
	if ((typeId >> 26) == 2) {
		for (int i = 0; i < 24; i++) {
			if (characterInfo.partTypeIds[i] != 0) {
				if (itemId == characterInfo.partIds[i])
					return true;
			}
		}
	} else if ((typeId >> 26) == 0x1C) {
		for (int i = 0; i < 5; i++) {
			if (characterInfo.auxParts.tids[i] != 0) {
				if (typeId == characterInfo.auxParts.tids[i])
					return true;
			}
		}
	}

	return false;
}

// shareddoc.cpp:5076
void PerformPowerBarSetting(void) {
	float position = CalcPowerBarPosition();

	IActor *pScreen = AfxGetTask()->GetActor("Screen");
	if (pScreen != 0) {
		pScreen << MsgObject(0, 0xA1, (intptr_t)&position, 0, 0, 0, 0);
	}
}

// golfball.inl:2 (the ball singleton; the original's out-of-line copy of this
// inline function sits in SharedDoc's object). Defined last: its guarded
// static takes a slot of cl's _$E counter, and defining it earlier renumbers
// the dynamic initializers paired in symbol_mappings.json.
CGolfBall &GolfBall(void) {
	static CGolfBall me;
	return me;
}
