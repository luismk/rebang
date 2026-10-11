// Original: d:\build\custom\temp\code\client\projectg\shareddoc.h
#ifndef PG_CLIENT_PROJECTG_SHAREDDOC_H
#define PG_CLIENT_PROJECTG_SHAREDDOC_H

#include "MascotInfo.h"
#include <windows.h>

#include <list>
#include <map>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "../shared/globalgamedefine.h"
#include "../shared/globalnetworkdefine.h"
#include "../shared/ItemManager.h"
#include "../shared/Localize.h"
#include "../shared/classdefine.h"
#include "globalgamedefine.h"
#include "parttidlist.h"
#include "hatManager.h"
#include "wmath.h"
#include "buddymanager.h"

class CGolfDoc;
class CExhibition;
class FrComboBox;
class WVector;
class CPeriodContents;
struct sPlayerData;
struct sFurniture_List;  // realmyroomdata.h
struct sGiftInfo;        // panggiftdlg.h
struct sGameServerInfo;  // messengerdlg.h

namespace _guild {
class CGuildActor;
}

// The personal shop's state (m_stateTrade). Only the values the code
// tests are named; 1 is not seen yet.
enum eStateTrade {
	STATE_TRADE_NONE = 0,  // no shop: SendTradeCloseShop does nothing
	STATE_TRADE_READY = 2, // the shop is set up: SendTradeOpenShop may open it
	STATE_TRADE_OPEN = 3,  // the own shop is open (FrTradeDlg::OnCloseBnUp closes it)
	STATE_TRADE_EDIT = 4   // the shop's items are being edited (CheckTradeItem)
};

// The replay mode of the current game (GetPlayingMode); values not known yet.
enum eReplayModeType {
};

// Who a chat line can be addressed to, as bit flags (InitReservedChatList
// adds the "to team" entry for the first bit and the "to guild" entry for
// the second).
enum eReservedChatPartner {
	RESERVED_CHAT_PARTNER_TEAM = 0x1,
	RESERVED_CHAT_PARTNER_GUILD = 0x2,
};

#pragma pack(push, 1)
// The local player's medals per course (CSharedDoc 0x5DA), 0x4E bytes:
// gold, silver and bronze counts for each of the 13 courses
// (CheckGoldMedal/CheckSilverMedal/CheckBronzeMedal sum one column).
struct sTrophyStatistics {
	unsigned short m_medals[13][3]; // 0x0, [course][gold, silver, bronze]
};

// One course's record of a player, 0x2B bytes (the 0x14-entry arrays of
// sUserInfo at 0x2B4/0x610 and of sUserInfoTime at 0xFF5).
struct sMapStatistics {
	sMapStatistics(void) {
		m_course = 0xFF;
	}

	unsigned char m_course;         // 0x0, the course index; 0xFF: empty
	PADDING(0x1, 0x1D);
	char m_bestScore;               // 0x1D, the best score; 0x7F when there is no record
	PADDING(0x1E, 0x2B);
};

// A player's full information, 0xB92 bytes, byte-packed. CSharedDoc keeps
// the room's players in an array of four (0x12D0, the constructor calls
// ??0sUserInfo@@QAE@XZ for each, 0xB92 apart; SetIndex copies the local
// player's 0x10D-byte user info over the first one) and each sUserInfoTime
// holds one at 0x4. The local player's members of CSharedDoc from 0x3E0 on
// (level 0x539, pang 0x53A, character 0x62C, club 0x630, equipped items
// 0x638) follow the same layout. The constructor (0x40fa70, emitted by
// shareddoc.cpp, no line marks) fills two arrays of 0x14 0x2B-byte records
// at 0x2B4 and 0x610 and zeroes the character. Only the members the
// transcribed callers use are named.
struct sUserInfo {
	sUserInfo(void);

	unsigned short m_slot;          // 0x0, callers set it to 0xFFFF (meaning not traced)
	// 0x2-0x10F: a copy of the player's sPangYaUserInfo (0x10D bytes, SetIndex
	// copies it as one; the members below mirror its layout).
	char m_nickname[0x16];          // 0x2, the player's name (FrPlayerDlg::SetState shows it); sPangYaUserInfo::id
	char m_name[0x16];              // 0x18, the second name (sPangYaUserInfo::nickname; FrPlayerDlg::SetName, the ghost converters)
	char m_guildName[0x15];         // 0x2E, sPangYaUserInfo::guildName
	char m_guildMark[0xC];          // 0x43, sPangYaUserInfo::guildMark
	PADDING(0x4F, 0x53);
	unsigned long m_identity;       // 0x53, sPangYaUserInfo::identity
	PADDING(0x57, 0x5B);
	unsigned long m_guid;           // 0x5B, sPangYaUserInfo::guid
	PADDING(0x5F, 0x6B);
	unsigned long m_guildUID;       // 0x6B, sPangYaUserInfo::guildUID (the login reply requests its RSS)
	PADDING(0x6F, 0x10B);
	unsigned long m_uid;            // 0x10B, the player's UID (sPangYaUserInfo + 0x109)
	// 0x10F-0x1FA: the player's sPangYaUserStatistics (0xEB bytes, byte-packed;
	// the type is not defined yet, the ghost converters copy it as one).
	unsigned long m_statShots;      // 0x10F, the statistics' first dword (CTask::OnPacketCommon copies the block from here)
	PADDING(0x113, 0x155);
	unsigned long m_value155;       // 0x155
	unsigned char m_level;          // 0x159, the grade of the skins/match rules derives from it
	__int64 m_pang;                 // 0x15A
	PADDING(0x162, 0x1FA);
	sTrophyStatistics m_trophy;     // 0x1FA, the player's medals per course
	// 0x248-0x2B4: a 0x6C-byte block (0x1B dwords, like CSharedDoc::m_realEquip)
	// callers copy as one.
	unsigned long m_equipHead;      // 0x248, first dword of the block
	unsigned long m_characterId;    // 0x24C, the equipped character (CSharedDoc::m_myCharacterId)
	unsigned long m_clubId;         // 0x250, the equipped club set (CSharedDoc::m_myClubId)
	unsigned long m_ballId;         // 0x254, the equipped ball's type id (0x14xxxxxx; CGallery::HandleMsg)
	unsigned long m_equipItem[10];  // 0x258, type ids of the equipped (consumable) items (CGolfRule::CanCancelShot searches the first player's)
	PADDING(0x280, 0x2AC);
	unsigned long m_skinTypeId;     // 0x2AC, the equipped skin's type id (HeadIcon SetLevel)
	PADDING(0x2B0, 0x2B4);          // the block's end
	sMapStatistics m_mapStatistics[20];  // 0x2B4, by course (the constructor marks them empty)
	sMapStatistics m_mapStatistics2[20]; // 0x610, likewise
	sCharacterInfo m_characterInfo; // 0x96C, the equipped character (RequestUccInfo; zeroed by the constructor)
	PADDING(0xB28, 0xB2C);
	unsigned long m_caddieTypeId;   // 0xB2C, the equipped caddie's type id (CGolfCaddie::OnInit; the last argument of CGolfRule::AddPlayer)
	unsigned long m_cadItemTypeId;  // 0xB30, the caddie's equipped item type id (CGolfCaddie::sFindCaddie)
	// 0xB28-0xB41: the equipped caddie's record, 0xB41: the equipped club set's
	// (id, type id, five stat shorts), 0xB53: the equipped mascot's (sMascotInfo,
	// 0x3F bytes; type id at 0xB57).
	PADDING(0xB34, 0xB41);
	unsigned long m_clubSetId;      // 0xB41, the equipped club set's item id (FrPlayerDlg::SetClubControl)
	unsigned long m_clubSetTypeId;  // 0xB45, the equipped club set's type id (0x10xxxxxx; CPowerGauge::GetAddAirnightLuckyArea, CSoundItemManager)
	short m_clubStat[5];            // 0xB49, the club set's upgrade level of each stat (sItemInfo::m_clubStatLevel)
	sMascotInfo m_mascot;           // 0xB53, the equipped mascot (0x3F bytes; CMascot::HandleMsg message 0x4E fills it)
};

// A player's information with the time it was received (CSharedDoc keeps
// them in a std::map keyed by UID at 0x5CB4; FrUserInfoForm::SetInfo shows
// one). 0x13FE bytes, byte-packed (the map's pair copy is `rep movsd` 0x4FF +
// `movsw`). The implicit constructor (0x4116a0) calls sUserInfo's and fills
// two more arrays of 0x14 0x2B-byte records at 0xC99 and 0xFF5 and the
// members from 0x13AF on.
struct sUserInfoTime {
	unsigned long m_time;           // 0x0
	sUserInfo m_userInfo;           // 0x4
	PADDING(0xB96, 0xB9A);
	char *m_pArray1;                // 0xB9A, heap array (ClearUserInfoTimeMap deletes it)
	PADDING(0xB9E, 0xBA2);
	char *m_pArray2;                // 0xBA2, heap array
	PADDING(0xBA6, 0x13A3);
	char *m_pArray3;                // 0x13A3, heap array
	PADDING(0x13A7, 0x13AB);
	char *m_pArray4;                // 0x13AB, heap array
	PADDING(0x13AF, 0x13FE);
};

// The real header of sTrophyStatistics and sMapStatistics is not known
// (shared code: CheckGoldMedal and CheckCourseMaster of sharedtitledoc.inl
// take them); they live above until it is found.
CHECK_SIZE(sTrophyStatistics, 0x4E);
CHECK_SIZE(sMapStatistics, 0x2B);
CHECK_FIELD(sMapStatistics, m_course, 0x0);
CHECK_FIELD(sMapStatistics, m_bestScore, 0x1D);

// A player slot of the room (CSharedDoc::m_roomSlotMap, keyed by UID). The
// original header and the size are not known yet (GolfTask emits its
// constructor; UccLibrary reads an sCharacterInfo at 0xD0). Only the members
// the transcribed callers use are named.
struct sRoomSlot {
	PADDING(0x0, 0x4);
	CExhibition *m_pPet;            // 0x4, the slot's pet exhibition (CLobbyMain::HidePet deletes it)
	PADDING(0x8, 0x10);
	unsigned long m_partTypeIds[24]; // 0x10, the equipped parts' type ids (NetResourceManager::ClearGarbage)
	unsigned long m_partItemIds[24]; // 0x70, the equipped parts' item ids
	sCharacterInfo m_characterInfo; // 0xD0, the player's character (RequestUccInfo(sRoomSlot&))
};
CHECK_FIELD(sRoomSlot, m_pPet, 0x4);
CHECK_FIELD(sRoomSlot, m_characterInfo, 0xD0);
CHECK_FIELD(sRoomSlot, m_partTypeIds, 0x10);
CHECK_FIELD(sRoomSlot, m_partItemIds, 0x70);

// An entry of the room's player list (CSharedDoc::m_roomPlayerList), as
// SetPlayer of the rule classes walks it; 0x152 bytes (AvatarUserList).
struct sRoomPlayer {
	unsigned long m_uid;            // 0x0, 0xFFFFFFFF for an empty slot
	char m_nickname[0x16];          // 0x4
	char m_guildName[0x15];         // 0x1A, copied into sRivalData::m_guildName (GolfRuleTimeattack SetPlayer)
	unsigned char m_connection;     // 0x2F, the lobby's connection sort key (ConnectionUserCompare)
	unsigned long m_identity;       // 0x30, sPangYaUserInfo::identity flags
	unsigned long m_titleTypeId;    // 0x34, the equipped title's type id
	PADDING(0x38, 0x54);
	unsigned char m_attribute;      // 0x54, the low two bits go to sPlayerData; bits 5-7: character type (AvatarUserList)
	unsigned char m_mannerFlag;     // 0x55, bit 0: manner player
	unsigned char m_level;          // 0x56
	unsigned char m_angelFlag;      // 0x57, bit 0: angel wing
	PADDING(0x58, 0x59);
	unsigned long m_guildUID;       // 0x59
	char m_emblemName[0xC];         // 0x5D, the guild emblem's name
	unsigned long m_ruleUID;        // 0x69, passed to CGolfRule::AddPlayer
	PADDING(0x6D, 0x152);
};

// An entry of the replay file list (CSharedDoc::m_replayFileList), as
// FrReplayModeDlg::OnOkBtnUp searches it by file name. The size is not
// known yet.
struct sReplayFileInfo {
	PADDING(0x0, 0xBC);
	char m_fileName[0x24];          // 0xBC, compared (_stricmp) with CSharedDoc::m_fileName; length not known
};

// One player's line of the game's result list (CSharedDoc::m_gameResultList),
// 0x190 bytes; FrFinalRankDlg lists and sorts them, GUILDMATCHUP returns one.
// The name is the PDB's (std::vector<sRivalData>, sRivalData::operator=,
// emitted by shareddoc.cpp, copies it member by member: the gaps at 0x37,
// 0x92, 0x142, 0x155 and 0x15D it skips are natural alignment). Only the
// members the transcribed functions read are named. sGameResultInfo is the
// name the callers transcribed first.
struct sRivalData {
	unsigned long m_id;             // 0x0, the player's id (FrRankForm::OnRankRBtnUp hands it to CUserInfo::SetInfo)
	unsigned long m_uid;            // 0x4
	char m_nickname[0x16];          // 0x8
	char m_guildName[0x15];         // 0x1E, the player's guild (FrResultTeamDlg::OnInit names the winning guild)
	unsigned char m_team;           // 0x33, 0: red, 1: blue (FrResultDlg::FindWinner sums the team results by it)
	unsigned short m_level;         // 0x34, the player's level (FrDemonProcessRank)
	unsigned char m_state;          // 0x36, 2: finished, 3: skipped
	PADDING(0x37, 0x38);
	int m_holeScore[18];            // 0x38, one per hole (operator= copies 0x12 dwords; CLobbyTask::OnPacket 0x6B)
	unsigned char m_holeShots[18];  // 0x80, one per hole (FrScoreDlg GetGuildVersus/GetMyCurHoleInGuildMatch)
	PADDING(0x92, 0x98);
	__int64 m_holePang[18];         // 0x98, one per hole (operator= copies them as dword pairs; CLobbyTask::OnPacket 0x6B)
	__int64 m_pang;                 // 0x128, third sort key (FinalRankCompare)
	__int64 m_extraPang;            // 0x130, added to m_pang for the result's bonus (FrResultDlg::OnBonusInit)
	int m_score;                    // 0x138, second sort key
	unsigned char m_totalShots;     // 0x13C, the shots so far (CLobbyTask::OnPacket 0x6B)
	unsigned char m_hole;           // 0x13D
	unsigned char m_rank;           // 0x13E, 0x10 and above are not listed
	unsigned char m_value13F;       // 0x13F
	unsigned char m_sortGroup;      // 0x140, first sort key (FinalRankCompare)
	unsigned char m_finishOrder;    // 0x141, the lobby's finish sort key (FinishUserCompare)
	PADDING(0x142, 0x144);
	WVector m_ballPos;              // 0x144, the player's ball position (CSpot::UpdateRivals copies it)
	PADDING(0x150, 0x158);          // int 0x150, byte 0x154, both set to 1 by SetPlayer (operator=)
	unsigned long m_titleTypeId;    // 0x158, the equipped title's type id (FrDemonProcessRank::GetTitleBitmap)
	unsigned char m_characterType;  // 0x15C, sRoomPlayer::m_attribute >> 5 (GolfRuleTimeattack SetPlayer)
	PADDING(0x15D, 0x160);
	int m_hasEmblem;                // 0x160, the player's guild UID (GolfRuleTimeattack SetPlayer; rename pending, FinalRankDlg/ResultDlg test it)
	char m_emblemName[0xC];         // 0x164 (operator= copies 0xC bytes)
	unsigned long m_uniteGuildIndex; // 0x170, second sort key of the unite guild result, higher first (UniteGuildIndexCompare)
	unsigned int m_approachDistanceCopy; // 0x174, CLobbyTask::OnPacket 0x6C stores the approach distance here and at 0x178
	unsigned int m_approachDistance; // 0x178, second sort key of the approach ranking, lower first (ApproachRankUserCompare)
	int m_approachPoint;            // 0x17C, third sort key of the approach ranking, higher first
	unsigned long m_identity;       // 0x180, sPangYaUserInfo::identity flags (GolfRuleTimeattack SetPlayer, FrScoreDlg::OnGolferListOwnerDraw)
	PADDING(0x184, 0x190);          // dword 0x184, bytes 0x188, 0x189: sRoomPlayer 0xC7, 0xCB, 0xCC (GolfRuleTimeattack SetPlayer)
};

// shareddoc.h:211, finds a rival by UID (std::find_if).
struct sFindRival {
	// shareddoc.h:212
	sFindRival(unsigned long uid) : m_uid(uid) {}

	// shareddoc.h:214
	bool operator()(sRivalData &rival) {
		return rival.m_uid == m_uid;
	}

	unsigned long m_uid;            // 0x0
};
CHECK_SIZE(sFindRival, 0x4);
CHECK_FIELD(sFindRival, m_uid, 0x0);

// The two guilds of a guild match (CSharedDoc 0x49F3, 0x4A bytes;
// FrResultTeamDlg::OnInit copies it as one into its own). The original name
// is not known.
struct sGuildMatchInfo {
	PADDING(0x0, 0x8);
	char m_guildName[2][0x15];      // 0x8, the red and blue guild's names (FrResultTeamDlg::OnRedOwnerDraw/OnBlueOwnerDraw)
	char m_guildMark[2][0xC];       // 0x32, the red and blue guild's emblem names (NetResourceManager::GetEmblemByName)
};
CHECK_SIZE(sGuildMatchInfo, 0x4A);
CHECK_FIELD(sGuildMatchInfo, m_guildName, 0x8);

// A player of the award ceremony (CSharedDoc 0x456E and 0x459E, six each,
// 8 bytes; FrAwardDlg looks them up by UID). The original name is not known.
struct sAwardPlayer {
	unsigned long m_uid;            // 0x0
	PADDING(0x4, 0x8);
};
CHECK_SIZE(sAwardPlayer, 0x8);

// The exp of a caddie level (CSharedDoc 0x4FF8, three of them, 8 bytes;
// FrCaddieInfoDlg::OnInit reads the first dword by the caddie's level). The
// original name is not known.
struct sCaddieLevelExp {
	unsigned long m_exp;            // 0x0
	unsigned long m_value4;         // 0x4, set by LoadLevelTable (0x208, 0x4B0, 0x7F8)
};
CHECK_SIZE(sCaddieLevelExp, 0x8);

// A player level of the level table (CSharedDoc 0x4DC0, 71 of them, 8 bytes;
// LoadLevelTable fills it from the exe's table). The original name is not known.
struct sLevelExp {
	unsigned long m_exp;            // 0x0, exp of this level
	unsigned long m_totalExp;       // 0x4, exp of all the levels up to and including this one
};
CHECK_SIZE(sLevelExp, 0x8);
CHECK_FIELD(sAwardPlayer, m_uid, 0x0);

CHECK_SIZE(sRivalData, 0x190);
CHECK_FIELD(sRivalData, m_level, 0x34);
CHECK_FIELD(sRivalData, m_state, 0x36);
CHECK_FIELD(sRivalData, m_holeShots, 0x80);
CHECK_FIELD(sRivalData, m_pang, 0x128);
CHECK_FIELD(sRivalData, m_extraPang, 0x130);
CHECK_FIELD(sRivalData, m_titleTypeId, 0x158);
CHECK_FIELD(sRivalData, m_hasEmblem, 0x160);
CHECK_FIELD(sRivalData, m_uniteGuildIndex, 0x170);
CHECK_FIELD(sRivalData, m_identity, 0x180);
CHECK_FIELD(sRivalData, m_characterType, 0x15C);
CHECK_FIELD(sGuildMatchInfo, m_guildMark, 0x32);

// One team's sums of the finished team game (CSharedDoc::m_teamResult,
// FrResultDlg::FindWinner clears and fills them). The original name is not
// known.
struct sTeamResult {
	int m_score;                    // 0x0, the sum of the members' m_rank (shown when m_roomType is 5)
	PADDING(0x4, 0x8);
	__int64 m_pang;                 // 0x8, the sum of the members' m_pang
};
CHECK_SIZE(sTeamResult, 0x10);
CHECK_FIELD(sTeamResult, m_score, 0x0);
CHECK_FIELD(sTeamResult, m_pang, 0x8);
#pragma pack(pop)

// An item of the offline shop's sale list (CSharedDoc::m_maketList), 0x5E
// bytes (list<sSaleItem>::_Buynode copies 0x17 dwords and a word). The
// original header and the members are not known yet; FrListBox items point
// at it (COfflineTradeDlg::SetListItem).
struct sSaleItem {
	PADDING(0x0, 0x5E);
};
CHECK_SIZE(sSaleItem, 0x5E);

// An owned card stack (CSharedDoc::m_myCardList), 0x3A bytes
// (list<sCards>::_Node copies 0xE dwords and a word). The name is the PDB's
// (CCardSlot::GetCards, sWarpItem::sWarpItem(const sCards&)); the original
// header is not known (no inline of it carries line info).
#pragma pack(push, 1)
struct sCards {
	unsigned long m_guid;           // 0x0, matched by _FindByGuid / _FindByUid
	unsigned long m_typeId;         // 0x4
	UNKNOWN(0x8, 0x14);
	unsigned long m_count;          // 0x14, the cards in the stack (traded by CTransferableItem)
	UNKNOWN(0x18, 0x3A);
};
#pragma pack(pop)
CHECK_SIZE(sCards, 0x3A);
CHECK_FIELD(sCards, m_guid, 0x0);
CHECK_FIELD(sCards, m_typeId, 0x4);
CHECK_FIELD(sCards, m_count, 0x14);

// A line of the chat view (CSharedDoc::m_chatLineList). The name is the
// PDB's (ChatMsg emits its implicit constructors); the original header is
// not known.
struct sChatLine {
	std::string m_text;             // 0x0
	unsigned long m_color;          // 0x1C
};
CHECK_SIZE(sChatLine, 0x20);
CHECK_FIELD(sChatLine, m_text, 0x0);
CHECK_FIELD(sChatLine, m_color, 0x1C);

typedef sRivalData sGameResultInfo;
CHECK_SIZE(sGameResultInfo, 0x190);
CHECK_FIELD(sGameResultInfo, m_team, 0x33);
CHECK_FIELD(sGameResultInfo, m_id, 0x0);
CHECK_FIELD(sGameResultInfo, m_uid, 0x4);
CHECK_FIELD(sGameResultInfo, m_nickname, 0x8);
CHECK_FIELD(sGameResultInfo, m_state, 0x36);
CHECK_FIELD(sGameResultInfo, m_pang, 0x128);
CHECK_FIELD(sGameResultInfo, m_score, 0x138);
CHECK_FIELD(sGameResultInfo, m_hole, 0x13D);
CHECK_FIELD(sGameResultInfo, m_rank, 0x13E);
CHECK_FIELD(sGameResultInfo, m_value13F, 0x13F);
CHECK_FIELD(sGameResultInfo, m_sortGroup, 0x140);
CHECK_FIELD(sGameResultInfo, m_ballPos, 0x144);
CHECK_FIELD(sGameResultInfo, m_hasEmblem, 0x160);
CHECK_FIELD(sGameResultInfo, m_emblemName, 0x164);
CHECK_FIELD(sGameResultInfo, m_uniteGuildIndex, 0x170);
CHECK_FIELD(sGameResultInfo, m_approachDistanceCopy, 0x174);
CHECK_FIELD(sGameResultInfo, m_holeScore, 0x38);
CHECK_FIELD(sGameResultInfo, m_holePang, 0x98);
CHECK_FIELD(sGameResultInfo, m_totalShots, 0x13C);
CHECK_FIELD(sGameResultInfo, m_approachDistance, 0x178);
CHECK_FIELD(sGameResultInfo, m_approachPoint, 0x17C);
CHECK_FIELD(sReplayFileInfo, m_fileName, 0xBC);
CHECK_SIZE(sUserInfo, 0xB92);
CHECK_FIELD(sUserInfo, m_slot, 0x0);
CHECK_FIELD(sUserInfo, m_skinTypeId, 0x2AC);
CHECK_FIELD(sUserInfo, m_nickname, 0x2);
CHECK_FIELD(sUserInfo, m_name, 0x18);
CHECK_FIELD(sUserInfo, m_guildName, 0x2E);
CHECK_FIELD(sUserInfo, m_guildMark, 0x43);
CHECK_FIELD(sUserInfo, m_identity, 0x53);
CHECK_FIELD(sUserInfo, m_guid, 0x5B);
CHECK_FIELD(sUserInfo, m_uid, 0x10B);
CHECK_FIELD(sUserInfo, m_trophy, 0x1FA);
CHECK_FIELD(sUserInfo, m_value155, 0x155);
CHECK_FIELD(sUserInfo, m_level, 0x159);
CHECK_FIELD(sUserInfo, m_pang, 0x15A);
CHECK_FIELD(sUserInfo, m_equipHead, 0x248);
CHECK_FIELD(sUserInfo, m_characterId, 0x24C);
CHECK_FIELD(sUserInfo, m_clubId, 0x250);
CHECK_FIELD(sUserInfo, m_ballId, 0x254);
CHECK_FIELD(sUserInfo, m_equipItem, 0x258);
CHECK_FIELD(sUserInfo, m_mapStatistics, 0x2B4);
CHECK_FIELD(sUserInfo, m_mapStatistics2, 0x610);
CHECK_FIELD(sUserInfo, m_characterInfo, 0x96C);
CHECK_FIELD(sUserInfo, m_caddieTypeId, 0xB2C);
CHECK_FIELD(sUserInfo, m_cadItemTypeId, 0xB30);
CHECK_FIELD(sUserInfo, m_clubSetId, 0xB41);
CHECK_FIELD(sUserInfo, m_clubSetTypeId, 0xB45);
CHECK_FIELD(sUserInfo, m_clubStat, 0xB49);
CHECK_FIELD(sUserInfo, m_statShots, 0x10F);
CHECK_FIELD(sUserInfo, m_guildUID, 0x6B);

// The login reply's player information (globalgamedefine.h:1699 in the
// original; it lives here, after sUserInfo, until that moves): an sUserInfo
// and a 0x400-byte block the server sends when local content 0x70 is on
// (CTask::OnPacketCommon 0x42 copies it to CSharedDoc::m_userInfoExtBlock).
// The constructor is emitted by taskmanager.cpp.
#pragma pack(push, 1)
struct sUserInfoExt : sUserInfo {
	// globalgamedefine.h:1699
	sUserInfoExt(void) {
		memset(&m_characterInfo, 0, sizeof(m_characterInfo));
		// 0xB28 (0x19 bytes) and 0xB41 (0x12 bytes): the caddie's and the club
		// set's records, not named yet; zeroed by the original.
		memset(&m_mascot, 0, sizeof(m_mascot));
		memset(m_nickname, 0, 0x10D);
		memset(&m_statShots, 0, 0xEB);
		memset(&m_trophy, 0, sizeof(m_trophy));
		memset(&m_equipHead, 0, 0x6C);
		memset(m_mapStatistics, 0, sizeof(m_mapStatistics));
		memset(m_extBlock, 0, sizeof(m_extBlock));
	}

	unsigned char m_extBlock[0x400]; // 0xB92
};
#pragma pack(pop)
CHECK_SIZE(sUserInfoExt, 0xF92);
CHECK_FIELD(sUserInfoExt, m_extBlock, 0xB92);
CHECK_SIZE(sUserInfoTime, 0x13FE);
CHECK_FIELD(sUserInfoTime, m_time, 0x0);
CHECK_FIELD(sUserInfoTime, m_userInfo, 0x4);
CHECK_FIELD(sUserInfoTime, m_userInfo.m_characterInfo, 0x970);
CHECK_FIELD(sUserInfoTime, m_userInfo.m_characterInfo.auxParts, 0x9E0);
CHECK_FIELD(sRoomPlayer, m_uid, 0x0);
CHECK_FIELD(sRoomPlayer, m_nickname, 0x4);
CHECK_FIELD(sRoomPlayer, m_attribute, 0x54);
CHECK_FIELD(sRoomPlayer, m_ruleUID, 0x69);
CHECK_FIELD(sRoomPlayer, m_identity, 0x30);
CHECK_FIELD(sRoomPlayer, m_titleTypeId, 0x34);
CHECK_FIELD(sRoomPlayer, m_level, 0x56);
CHECK_FIELD(sRoomPlayer, m_guildUID, 0x59);
CHECK_FIELD(sRoomPlayer, m_emblemName, 0x5D);
CHECK_FIELD(sRoomPlayer, m_guildName, 0x1A);
CHECK_FIELD(sRoomPlayer, m_connection, 0x2F);
CHECK_SIZE(sRoomPlayer, 0x152);

// A received note, element of CSharedDoc's note list (m_noteList; its node
// is 0x78 bytes, hence sizeof 0x70). FrMailBoxDlg lists them.
struct sNoteInfo {
	unsigned long id;        // 0x00, sent back in the read receipt (FrMailBoxDlg::OnNoteDlgResult)
	PADDING(0x04, 0x06);
	char nick[0x16];         // 0x06, the sender ('@' first: an operator)
	char text[0x40];         // 0x1C
	char date[0x10];         // 0x5C
	bool read;               // 0x6C, drawn grey once read
	PADDING(0x6D, 0x70);
};
CHECK_SIZE(sNoteInfo, 0x70);
CHECK_FIELD(sNoteInfo, id, 0x00);
CHECK_FIELD(sNoteInfo, nick, 0x06);
CHECK_FIELD(sNoteInfo, text, 0x1C);
CHECK_FIELD(sNoteInfo, date, 0x5C);
CHECK_FIELD(sNoteInfo, read, 0x6C);

// One entry of CSharedDoc::m_gameTypeNames (8 bytes: the stride of
// FrDtRoomDlg::OnGameDescInit's index).
struct sGameTypeName {
	const char *name;            // 0x0, the room name SetGameType copies into m_roomName
	unsigned char maxPlayers;    // 0x4, 4 for stroke/match, 2 for the ladder, 30 for lobbies and tourneys (InitGameType)
	unsigned char defaultPlayers; // 0x5, 2 for stroke/match, 10 for lobbies and tourneys (InitGameType)
	unsigned char defaultHoles;  // 0x6, 3 for stroke, 9 for the ladder and tourneys, 18 for the last two types (InitGameType)
	UNKNOWN(0x7, 0x8);
};
CHECK_FIELD(sGameTypeName, name, 0x0);
CHECK_FIELD(sGameTypeName, maxPlayers, 0x4);
CHECK_FIELD(sGameTypeName, defaultPlayers, 0x5);
CHECK_FIELD(sGameTypeName, defaultHoles, 0x6);
CHECK_SIZE(sGameTypeName, 0x8);

// A map event (the element of CSharedDoc::m_mapEventList; its list node is
// 0x30 bytes, hence sizeof 0x28). The members are not known yet.
struct sMapEventInfo {
	UNKNOWN(0x0, 0x28);
};
CHECK_SIZE(sMapEventInfo, 0x28);

struct sCardStack;
struct sRoomUserInfo;    // detailedroominfodlg.h
struct sItemAttachCard;  // carddlg.h

// The element types below are known only by their size (the constructor's
// list and map nodes); their members are not transcribed yet.
#pragma pack(push, 1)
// A guild match flag (CSharedDoc::m_guildMatchFlags; list node 0x38 bytes).
struct sGuildMatchFlagInfo {
	UNKNOWN(0x0, 0x30);
};
// A guild match pairing the server sends (CLobbyTask::OnPacket 0xBD,
// CSharedDoc::m_guildMatchups).
struct sGuildMatchup {
	bool m_kind;               // 0x0, copied to CSharedDoc::m_flag5020
	unsigned long m_guidA;     // 0x1, a player of the match
	unsigned long m_guidB;     // 0x5, his opponent
};
// A guild member (CSharedDoc::m_guildMembers, keyed by int; tree node 0x54 bytes).
struct GuildMemberInfo_t {
	UNKNOWN(0x0, 0x40);
};
// A quest of the player (CSharedDoc::m_questList; list node 0x18 bytes),
// not IFF_STRUCT::sQuest.
struct sQuest {
	unsigned long m_typeId;      // 0x0, the quest's IFF type id (CLobbyTask::OnPacket 0xCD)
	int m_state;                 // 0x4, a byte the server sends with it
	IFF_STRUCT::sQuest *m_iff;   // 0x8, CItemManager::FindQuest(m_typeId)
	bool m_flagC;                // 0xC, false when received
	UNKNOWN(0xD, 0x10);
};
// A burning special card (CSharedDoc::m_burningSpCards; list node 0x1C bytes).
struct sBurnningSpCard {
	UNKNOWN(0x0, 0x14);
};
// Stand-in for CGolfRule::sShotData (golfrule.h, 0x88 bytes: list node 0x90),
// the element of CSharedDoc::m_shotDataList: CGolfRule cannot be included here.
struct sShotDataStorage {
	UNKNOWN(0x0, 0x88);
};
// The game settings of the room FrDtRoomDlg describes (OnGameDescInit),
// decoded as one 12-byte block (CSharedDoc::m_gameDesc).
struct sRoomGameDesc {
	unsigned char m_descHoleCount;  // 0x0, number of holes
	unsigned long m_descGameTime;   // 0x1, time limit in milliseconds
	unsigned char m_descCourse;     // 0x5, course index, 0xFF: random
	unsigned char m_descGameType;   // 0x6, game type (index of CSharedDoc::m_gameTypeNames)
	unsigned char m_descMode;       // 0x7, hole order mode
	unsigned long m_descMatchId;    // 0x8, the match (CItemManager::FindMatch)
};
#pragma pack(pop)
CHECK_SIZE(sGuildMatchFlagInfo, 0x30);
CHECK_SIZE(sGuildMatchup, 0x9);
CHECK_FIELD(sGuildMatchup, m_guidA, 0x1);
CHECK_FIELD(sGuildMatchup, m_guidB, 0x5);
CHECK_SIZE(GuildMemberInfo_t, 0x40);
CHECK_SIZE(sBurnningSpCard, 0x14);
CHECK_SIZE(sQuest, 0x10);
CHECK_FIELD(sQuest, m_state, 0x4);
CHECK_FIELD(sQuest, m_iff, 0x8);
CHECK_FIELD(sQuest, m_flagC, 0xC);
CHECK_SIZE(sShotDataStorage, 0x88);
CHECK_SIZE(sRoomGameDesc, 0xC);
CHECK_FIELD(sRoomGameDesc, m_descHoleCount, 0x0);
CHECK_FIELD(sRoomGameDesc, m_descGameTime, 0x1);
CHECK_FIELD(sRoomGameDesc, m_descCourse, 0x5);
CHECK_FIELD(sRoomGameDesc, m_descGameType, 0x6);
CHECK_FIELD(sRoomGameDesc, m_descMode, 0x7);
CHECK_FIELD(sRoomGameDesc, m_descMatchId, 0x8);

// The client's shared document (the WSingleton<CSharedDoc> instance). Only
// the members the transcribed functions touch are named. The byte-packed
// sPangYaUserInfo at 0x3E2 puts the following members at odd offsets, hence
// the packing. Size 0x5D30 (CProjectG's constructor allocates it).
#pragma pack(push, 1)
class CSharedDoc : public WSingleton<CSharedDoc> {
	LAYOUT_CHECK_FRIEND;
	friend class CProjectG; // WinProc reconnects through the server list
	friend class CGolfDoc;  // its constructor registers itself as m_pGolfDoc
	friend class CSky;      // CSky::RenderBillboardCloud reads m_pGolfDoc
	friend class FrServerDlg; // ConnectToServer stores the chosen server id
	friend class CLobbyTask;  // OnPacket stores the login result in the protected user info
	friend class CTask;       // OnPacketCommon reads and fills the protected login state
protected:
	void ClearGameMode(void);
	void ClearStatistics(void);
	void ClearCommonVars(void);
	void LoadLevelTable(void);
	void InitGameType(void);
	void ClearLobbyStateVars(void);
	void ClearServerVars(void);

public:
	CSharedDoc(void);
	virtual ~CSharedDoc(void);

	int GetBuffValue(int target, int type);
	bool AddWhisperPartner(FrComboBox* combo, const char* name);
	void ClearCurrentChannel(bool notify);
	void ClearRealEquip(void);
	unsigned long *GetMyAuxPartTidList(void);
	CPartTidList &GetMyPartTidList(void);
	void ClearMyGuildInfo(void);
	void ClearUserInfoTimeMap(unsigned long uid);
	void ClearAll(void);
	void ClearInventory(void);
	void ClearGameVars(void);
	void ClearRivalVars(bool);
	void InitTradeData(void);
	void ClearOldSeasonStat(void);
	void ClearJapanVars(void);
	void ClearUcc(void);
	void LogGstUpdateHole(unsigned long uid, unsigned char hole, int score, __int64 pang);
	int IsControlServerService(int service);
	void SetTradeUID(unsigned long uid);
	unsigned long GetTradeUID(void);               // shareddoc.h:568, inline below (Avatar_MainUI's copy)
	unsigned short GetSecurityKey(void);
	void LoadItemDb(void);
	void AddChar(sCharacterInfo *pChar);
	void AddCaddie(sCaddieInfo *pCaddie);
	void AddMascot(sMascotInfo *pMascot);
	void AddMyPartsList(const sItemInfo &item);
	void AddMyItem(const sItemInfo &item);
	bool InitRecentWhisperList(FrComboBox *pComboBox);
	bool InitReservedChatList(FrComboBox *pComboBox, eReservedChatPartner partner);
	bool UpdateChatTarget(FrComboBox *pComboBox);
	const char *GetPrevChatTarget(const char *target);
	const char *GetNextChatTarget(const char *target);
	std::string GetLoginAuthKey(void);
	void SetLoginAuthKey(std::string key);
	void SetSecurityKey(unsigned short key);
	void SetFileName(char *const fileName);
	void InitMapEvent(void);
	bool IsMapEventActive(int map);
	unsigned long GetMapEventPangRate(int map);
	unsigned long GetMapEventExpRate(int map);
	bool CanUseRookieChannelMap(unsigned int map);
	bool CanCompound(unsigned long typeId);
	void SetIdentity(int identity);
	void AccumulateIdentity(int identity);
	_SYSTEMTIME &GetServerTime(void);
	void SetServerTime(_SYSTEMTIME &time);
	void SetCurEquipInfo(sUserEquip *equip);
	void BuildMyPartTidList(void);
	void SetInitHaveHalloweenItem(void);
	void SetDirectMoveRoomInfo(unsigned char server, unsigned short room, unsigned long uid);
	bool GetDirectMoveRoomInfo(unsigned char *server, unsigned short *room, unsigned long *uid);
	void ClearDirectMoveRoomInfo(void);
	CPeriodContents *GetPeriodDoc(void);
	void SetPeriodDoc(void);
	void LoadChatFilter(void);
	void InitGolfDoc(void);                         // 0x413ed0
	void LoadVisGroup(const char *fileName);        // 0x420a60
	sItemInfo *FindMyItemByGuid(unsigned long guid);
	bool IsOverlapPart(unsigned long typeId);
	sCardStack *GetCardInfoToUID(unsigned long uid);
	sCardStack *GetCardInfoToTID(unsigned long typeId);
	sMascotInfo *GetMyMascotInfo(unsigned long id);
	sMascotInfo *GetMyEquippedMascotInfo(void);
	int GetNumAddItemSlotByMascot(unsigned char player);
	int GetNumAddItemSlotByMascot(void);
	bool DecreaseMyItem(std::list<sItemInfo>::iterator &it);
	bool IsEquipParts(sCharacterInfo &characterInfo, unsigned long typeId, unsigned long partsId);
	__int64 RemainOwnCash(unsigned long buyType, __int64 price);
	void SetIndex(unsigned long uid);
	bool IsCurrentMissionBlind(void);
	int FindUserInfoTimeUID(unsigned long uid, std::map<unsigned long, sUserInfoTime>::iterator &it);
	int FindUserInfoTimeGUID(unsigned long guid, std::map<unsigned long, sUserInfoTime>::iterator &it);
	int FindUserInfo(unsigned long key, std::map<unsigned long, sBriefUserInfo>::iterator &it);
	int FindUserInfoUID(unsigned long uid, std::map<unsigned long, sBriefUserInfo>::iterator &it);
	int InsertUserInfoTime(unsigned long uid, sUserInfoTime &info);

	// shareddoc.inl:389 (inline; SharedDoc emitted the copy the linker kept,
	// not transcribed yet).
	unsigned char GetIndex(unsigned long uid);
	bool AreWePlayingTogether(unsigned long uid);
	const char *GetDefaultTitle(unsigned char gameType);
	std::map<unsigned char, sMapStatistics> &GetMyPastMapStat(unsigned char course);

	// shareddoc.h:505 (inline; GuildActor emitted the copy the linker kept)
	const sPangYaUserInfo &GetPangyaUserInfo(void) {
		return m_userInfo;
	}

	// Defined in the class body in the original (shareddoc.h line marks);
	// this unit emits them, so they are defined out of line in shareddoc.cpp.
	void HaveAngelWing(bool have);
	eStateTrade GetStateTrade(void);                // shareddoc.h:559, inline below
	std::list<sTradeItem> *GetListTradeItem(void);  // shareddoc.h:564, inline below
	std::list<sStall> *GetListStallItem(void);
	sStall *GetStallItem(void);
	void SetTradeMode(unsigned char mode);
	unsigned char GetTradeMode(void);               // shareddoc.h:609, inline below (OfflineTradeDlg's copy)
	std::list<sSaleItem> *GetMaketList(void);       // shareddoc.h:583, inline below (OfflineTradeDlg's copy)
	void SetHaveHalloweenItem(bool have);
	void SetWearHalloweenItem(bool wear);
	void SetUserInfoMod(int mod);                   // shareddoc.h:1144, inline below
	int GetUserInfoMod(void);

	// Bongdari shop play counts (shareddoc.h:550-554, inline below).
	short GetBsUsableTimes(void);
	short GetBsUsableBonusTimes(void);
	short GetBsRemainedBonusTimes(void);

	// Emitted by taskmain.cpp, defined out of line there.
	void SetRmrMode(unsigned char mode);            // shareddoc.h:616
	unsigned char GetRmrMode(void);                 // shareddoc.h:617
	unsigned long GetFlagBlock(void);               // shareddoc.h:620
	bool IsHaveHalloweenItem(void);                 // shareddoc.h:1011
	eReplayModeType GetPlayingMode(void);           // shareddoc.h:1024
	std::list<std::string> *GetVisGroup(const char *name); // 0x41a6c0, out of line in shareddoc.cpp (CGolfPlayer::ParseScript_ShowVis)
	void SetPlayingMode(eReplayModeType mode);      // shareddoc.h:1025
	int GetNowGoWithMode(void);                     // shareddoc.h:1137
	int GetGoWithFriendUID(void);                   // shareddoc.h:1138
	void SetGoWithModeStart(int friendUID);          // shareddoc.h:1132, inline below (MessengerDlg's copy)
	void SetGoWithModeEnd(void);                    // shareddoc.h:1136, inline below (MessengerDlg's copy)
	void SetGoWithMode(int mode);                   // shareddoc.h:1140, inline below (LobbyMain's copy)

	// Not present in the transcribed header earlier; some callers expect
	// an out-of-line helper to log 30s scores. Provide the declaration so
	// an implementation unit can emit a stub to satisfy the linker.
	void Log30sScore(void);

	// Emitted by taskmanager.cpp, defined out of line there.
	void SetBsUsableTimes(short times);             // shareddoc.h:551
	void SetBsUsableBonusTimes(short times);        // shareddoc.h:553
	void SetBsRemainedBonusTimes(short times);      // shareddoc.h:555
	void SetFlagBlock(unsigned long flagBlock);     // shareddoc.h:622

	// Emitted by lobbytask.cpp, defined out of line there.
	void SetTimeBlock(int timeBlock);               // shareddoc.h:623
	unsigned long GetFriendGameSvrUID(void);        // shareddoc.h:1058
	void SetForceNicknameChange(unsigned char force); // shareddoc.h:1074

	// Emitted by golfrule.cpp, defined out of line there.
	void SetReplayState(int state);                 // shareddoc.h:1018
	int GetReplayState(void);                       // shareddoc.h:1019
	char *GetFileName(void);                        // shareddoc.h:1021
	int GetReplayPlayType(void);                    // shareddoc.h:1022
	void SetReplayPlayType(int type);               // shareddoc.h:1023
	void SetGoStopMode(bool goStop);                // shareddoc.h:1150
	bool GetGoStopMode(void);                       // shareddoc.h:1151

	// Emitted by fontinfo.cpp, defined out of line there.
	unsigned long GetFlagAbilityItem(void);         // shareddoc.h:642
	unsigned char GetNumBonusPangItem(void);        // shareddoc.h:644
	bool IsWearHalloweenItem(void);                 // shareddoc.h:1012

	// Emitted by tradedlg.cpp, defined out of line there.
	const char *GetTradeTitle(void); // shareddoc.h:570
	int GetCountVisitor(void);      // shareddoc.h:572
	__int64 GetTradeIncome(void);   // shareddoc.h:590

	// Emitted by tradehandler.cpp, defined out of line there.
	void SetStateTrade(eStateTrade state); // shareddoc.h:560
	void AddTradeIncome(__int64 income);   // shareddoc.h:589

	bool SendTradeOpenShop(void);
	bool SendTradeTitle(std::string title);          // 0x40c990
	bool SendTradeCloseShop(void);
	bool SendTradeEditShop(void);                   // 0x40c820
	bool SendTradeEnterShop(unsigned long uid);     // 0x40c890
	void DeleteCardToPartsAttach(unsigned long cardId); // 0x4195a0
	float GetAuxPartProperty(unsigned char team, unsigned char property); // 0x4142d0
	bool ConfirmInsertItem(unsigned long typeId, unsigned long count) const; // 0x4103d0
	bool FindEquipPart(unsigned char team, unsigned long typeId); // 0x414470
	float GetComboGaugeLimit(unsigned char team);   // 0x414540
	bool CheckUnderLevelClubSet(unsigned long typeId); // 0x410b70
	void RefreshAllCookie(void);                    // 0x410be0
	bool DeleteMyItem(std::list<sItemInfo>::iterator &it); // 0x419dc0
	void AddFurniture(const sFurniture_List &furniture); // 0x41c720
	bool UTIL_SendChatMessage(FrComboBox *pComboBox, const char *message, bool whisper); // 0x41ce70
	void SortAllMyItemList(void);                   // 0x4200b0

	// shareddoc.inl's free functions read these.
	friend void SetHoles(unsigned char holes);
	friend void SetWeather(unsigned char weather);
	friend CItemManager *ItemManager(void);
	friend unsigned long MyUID(void);
	friend __int64 MyBonusCash(void);
	friend int IsGuildGame(void);
	friend char *MyNick(void);
	friend char *MyId(void);
	friend unsigned char GetPlayerNum(void);
	friend sPlayerData *PLAYER(unsigned char index);
	friend unsigned char GetCurMap(void);
	friend bool SetCurMap(unsigned char map);
	friend int OfflinePlay(void);
	friend __int64 MyOutsideCash(void);
	friend int GalleryMode(void);
	friend unsigned long MyGuid(bool galleryGuid);
	friend unsigned char GetHoles(void);
	friend unsigned char GetStartHole(void);
	friend unsigned char GetEndHole(void);
	friend unsigned char GetWeather(void);
	friend unsigned long GetShotTimeLimit(void);
	friend void SetGameOver(bool gameOver);
	friend bool IsGameOver(void);
	friend unsigned long GetGameTimeLimit(void);
	friend unsigned char MyIndex(void);
	friend int IsObserver(unsigned long identity);
	friend int IsAdministrator(unsigned long identity);
	friend CGolfDoc *GOLFDOC(void);

	// TurnOn3DBackground / TurnOff3DBackground write the weather and the
	// 3D background switch directly.
	friend class CTaskMain;

	// CGMToolkit::IsAdministrator reads m_userInfo.identity directly.
	friend class CGMToolkit;

	// CClock::HandleMsg tests bit 1 of m_userInfo.identity directly.
	friend class CClock;

	// _guild::CGuildActor::ChangeGuildName copies the new name into
	// m_userInfo.guildName directly.
	friend class _guild::CGuildActor;

	// FrAwardDlg's handlers read the local player's and the room's state
	// directly.
	friend class FrAwardDlg;

	// FrPrivateTrade::OnInit reads the local player's nickname directly.
	friend class FrPrivateTrade;

	// CGolfBg::OnLoad/DoReLoad read the room's course state directly.
	friend class CGolfBg;
	// CLobbyMain::OnToppage_Finish tests and clears m_flag50F6.
	friend class CLobbyMain;

protected:

public:
	std::string m_loginServerAddress;                // 0x28, the login server's dotted address (GameUnit::Connect hands it to SetMoveLoginServer; MSNUnit::OnConnect)
	std::string m_gameServerAddress;                 // 0x44, the game server's dotted address (GameUnit::Connect, inet_addr)

protected:
	std::string m_string60;                          // 0x60 (only constructed so far)

public:
	unsigned long m_gameServerPort;                  // 0x7C, the game server's port, host order (GameUnit::Connect, htons)

protected:
	unsigned long m_value80;                         // 0x80, cleared by ClearCommonVars
	unsigned long m_value84;                         // 0x84, likewise
	unsigned long m_value88;                         // 0x88, likewise

public:
	unsigned long m_loginConnectValue;               // 0x8C, a dword LoginUnit::OnConnect decodes from the login server's reply
	unsigned char m_value90;                         // 0x90, cleared by ClearGameVars
	PADDING(0x91, 0x94);
	unsigned long m_value94;                         // 0x94, cleared by ClearCommonVars
	std::string m_rankServerAddress;                 // 0x98, the rank server's dotted address (RankUnit::Connect, inet_addr)
	unsigned short m_rankServerPort;                 // 0xB4, the rank server's port, host order (RankUnit::Connect, htons)

protected:
	PADDING(0xB6, 0xC8);
	unsigned long m_loginTime;                       // 0xC8, timeGetTime() when the login server accepted the login (CLobbyTask::OnPacket)

public:
	sRoomGameDesc m_gameDesc;                        // 0xCC, the room FrDtRoomDlg describes (CLobbyTask::OnPacket 0x84)
	// The room name of each game type (SetGameType copies the entry into
	// m_roomName; FrDtRoomDlg::OnGameDescInit shows it).
	sGameTypeName m_gameTypeNames[15];               // 0xD8, indexed by game type (m_roomType)

protected:
	PADDING(0x150, 0x158);
	_SYSTEMTIME m_serverTime;                        // 0x158
	std::string m_string168;                         // 0x168 (only constructed so far)
	unsigned long m_japanState;                      // 0x184, cleared by ClearJapanVars
	std::string m_string188;                         // 0x188 (only constructed so far)

public:
	unsigned long m_billingType;                     // 0x1A4, the billing service: 2 gametree, 4 hangame (FrGiftDlg/FrCheckoutDlg::OnChargeResult)

protected:
	std::string m_commandLineAccount;                // 0x1A8, the first command-line argument after the exe path, quotes stripped (ClearJapanVars)
	unsigned char m_japanFlag;                       // 0x1C4, cleared by ClearJapanVars
	PADDING(0x1C5, 0x1C8);
	unsigned long m_japanParam1;                     // 0x1C8, likewise
	unsigned long m_japanParam2;                     // 0x1CC, likewise
	std::list<unsigned long> m_list1D0;              // 0x1D0 (only constructed so far)
	std::list<unsigned short> m_list1DC;             // 0x1DC (only constructed so far)
	CItemManager m_itemManager;                      // 0x1E8, constructed by CSharedDoc's constructor (ItemManager)
public:
	unsigned short m_mySlot;                         // 0x3E0, the local player's slot in the room (sUserInfo::m_slot), 0xFFFF outside one (CTaskMain::SendRoomExitPacket)
	sPangYaUserInfo m_userInfo;                      // 0x3E2, the local player's (GetPangyaUserInfo)

public:
	char m_loginId[0x16];                            // 0x46B, a web login whose nickname still equals it must pick one (CLobbyMain::NeedsChangeNick)
	PADDING(0x481, 0x4EB);

public:
	unsigned long m_myUID;                           // 0x4EB, the local player's UID (MSNUnit::OnConnect)
	// The local player's statistics (sPangYaUserStatistics at 0x4EF, flat
	// until the sMyInfo restructure; names after the Titles_Client checks).
	unsigned long m_myStatShots;                     // 0x4EF, statistics +0x00
	unsigned long m_myStatPuttAttempts;              // 0x4F3, statistics +0x04 (CheckPuttinPercent's attempts)
	PADDING(0x4F7, 0x503);
	unsigned long m_myStatPangyas;                   // 0x503, statistics +0x14
	PADDING(0x507, 0x513);
	unsigned long m_myStatDrives;                    // 0x513, statistics +0x24 (CheckFairawayPercent's drives)
	unsigned long m_myStatPar3Drives;                // 0x517, statistics +0x28 (CheckFairawayPercent's par 3s)
	unsigned short m_myStatHoleInOnes;               // 0x51B, statistics +0x2C
	PADDING(0x51D, 0x51F);
	unsigned long m_myStatFairways;                  // 0x51F, statistics +0x30 (CheckFairawayPercent's fairways)
	PADDING(0x523, 0x529);
	unsigned long m_myStatPuttsSunk;                 // 0x529, statistics +0x3A (CheckPuttinPercent's successes)
	PADDING(0x52D, 0x535);
	unsigned long m_myExp;                           // 0x535, the local player's experience (CFamilyTask::OnPacket)
	unsigned char m_myLevel;                         // 0x539, the local player's level
	__int64 m_myPang;                                // 0x53A, the local player's pang (FrPangGiftDlg, "%I64d")

protected:
	PADDING(0x542, 0x57C);

public:
	unsigned long m_myStatGamesPlayed;               // 0x57C, statistics +0x8D (CheckNoMannerPercent's games)

protected:
	PADDING(0x580, 0x5A8);

public:
	unsigned long m_myStatNoMannerGames;             // 0x5A8, statistics +0xB9 (CheckNoMannerPercent's count)

protected:
	PADDING(0x5AC, 0x5C6);

public:
	unsigned long m_myStatHolesPlayed;               // 0x5C6, the local player's holes played (ValentineEventDlg)

protected:
	PADDING(0x5CA, 0x5DA);

public:
	sTrophyStatistics m_myTrophy;                    // 0x5DA, the local player's medals (Title25-27, CheckGoldMedal)

public:
	unsigned long m_myCaddieId;                      // 0x628, the local player's equipped caddie (sUserInfo::m_equipHead; Net_EquipCaddie, cCaddieTalkTeeShot::FindMsg)
	unsigned long m_myCharacterId;                   // 0x62C, the local player's equipped character
	unsigned long m_myClubId;                        // 0x630, the local player's equipped club set

	unsigned long m_myBallId;                        // 0x634, the local player's equipped ball (sUserInfo::m_ballId; Net_EquipGear)
	unsigned long m_equipItem[10];                   // 0x638, type ids of the equipped (consumable) items

protected:
	PADDING(0x660, 0x678);

public:
	unsigned long m_equipSkin[6];                    // 0x678, type ids of the equipped skins (CTaskMain::HandleMsgCommon)

public:
	unsigned long m_myMascotId;                      // 0x690, the local player's equipped mascot (GetMyEquippedMascotInfo)

protected:
	sMapStatistics m_myMapStatistics[20];            // 0x694, by course (the constructor marks them empty)
	sMapStatistics m_myMapStatistics2[20];           // 0x9F0, likewise
	unsigned char m_userInfoExtBlock[0x400];         // 0xD4C, sUserInfoExt::m_extBlock of the login reply (CTask::OnPacketCommon 0x42)
	std::map<unsigned int, std::string> m_stringMap; // 0x114C (only constructed so far)

public:
	std::map<unsigned int, sCharacterInfo> m_characterMap; // 0x1158, the characters the player owns, keyed by id

protected:

public:
	std::map<unsigned int, sCaddieInfo> m_caddieMap; // 0x1164, the caddies the player owns, keyed by id

public:
	std::map<unsigned int, sMascotInfo> m_myMascotMap; // 0x1170, the mascots the player owns, keyed by id

public:
	std::list<sItemInfo> m_myPartsList;             // 0x117C, AddMyPartsList appends to it

protected:

public:
	std::map<unsigned int, sItemInfo> m_myItemMap;   // 0x1188

public:
	std::list<sItemInfo> m_myGearList;               // 0x1194, sorted by SortAllMyItemList; FrPlayerDlg cycles balls/clubs through it (typeId +4)

public:
	std::list<sItemInfo> m_itemInfoList;            // 0x11A0, AddMyItem appends to it (elements read by type id +4 and the short at +0xC)

protected:

public:
	std::list<sReplayFileInfo> m_replayFileList;     // 0x11AC, the saved replays (FrReplayModeDlg deletes from it)

public:
	// Two more owned-item lists (SortAllMyItemList sorts them, UccLibrary's
	// GetMyItemInfo searches them); their item classes are not identified.
	std::list<sItemInfo> m_myExtraItemList;          // 0x11B8
	std::list<sItemInfo> m_myExtraItemList2;         // 0x11C4
	std::list<sGiftInfo> m_giftList;                 // 0x11D0, the received gifts (CTask::_ReFreshMyRoomGift)

protected:
	std::list<sGiftInfo> m_giftList2;                // 0x11DC (only constructed so far)
	std::list<sMailInfoBrief> m_mailList;            // 0x11E8 (only constructed so far)

public:
	std::list<sItemInfo> m_mySkinList;              // 0x11F4, the owned items of type class 14 (skins; FrRecycleDlg::IsHaveItem reads the type id +4)

protected:
	std::list<sSpecialTrophy> m_mySpecialTrophies;   // 0x1200 (only constructed so far)
	std::list<sGuildTrophy> m_myGuildTrophies;       // 0x120C (only constructed so far)

public:
	// Two more owned-item lists, searched last by UccLibrary's GetMyItemInfo;
	// CTransferableItem trades the first by count (m_count, +0xC). Their item
	// classes are not identified.
	std::list<sItemInfo> m_myExtraItemList3;         // 0x1218 (PrivateTradeDlg finds by guid)
	std::list<sItemInfo> m_myExtraItemList4;         // 0x1224

public:
	std::list<sFurniture_List> m_furnitureList;      // 0x1230, the my-room furniture (AddFurniture appends; element flag byte +0x1A)

public:
	std::list<sCards> m_myCardList;                  // 0x123C, the owned cards (CTransferableItem, CCardBook, PrivateTradeDlg)

public:
	// The period cards in use, by kind (sSCardAvilityPeriodInfo::kind; [0] is
	// what CItemBuff::UpdateBuffDlg shows). CCardManager::CalcCardPeriodAndStatus
	// walks the four lists.
	std::list<sSCardAvilityPeriodInfo> m_cardPeriodList[4]; // 0x1248

protected:
	std::list<sQuest> m_questList;                   // 0x1278 (only constructed so far)
	PADDING(0x1284, 0x1288);

public:
	__int64 m_myCash;                                // 0x1288, the local player's cash (FrOnelineReqChkOutDlg::OnYesInit)

protected:
	__int64 m_outsideCash;                           // 0x1290
	__int64 m_myBonusCash;                           // 0x1298 (MyBonusCash)

public:
	std::list<sItemInfo> m_myItemList;               // 0x12A0
	std::list<sItemInfo> m_myAuxItemList;            // 0x12AC, searched for UCC aztecs

public:
	std::map<unsigned long, sRoomSlot> m_roomSlotMap; // 0x12B8, the room's player slots by UID (CLobbyMain::HidePet deletes a slot's pet exhibition)

public:
	std::map<unsigned long, unsigned char> m_indexMap; // 0x12C4, a player's index by UID (SetIndex, GetIndex)

public:
	sUserInfo m_roomPlayers[4];                      // 0x12D0, indexed by player number

protected:
	sPangYaUserStatistics m_gameStatistics[4];      // 0x4118, by player number; ClearGameVars zeroes them and sets exp to -1

public:
	std::vector<unsigned long> m_gainedItemList[4];  // 0x44C4, type ids of the items won in the game (FrUniteResultDlg lists [0])

public:
	std::vector<sGameResultInfo> m_gameResultList;   // 0x4504, the finished game's results (FrFinalRankDlg)

protected:
	std::vector<unsigned long> m_vector4514;         // 0x4514 (only constructed so far; element type not known)
	std::vector<sGuildMatchup> m_vector4524;         // 0x4524, the guild match pairings (CLobbyTask::OnPacket 0xBD)
	std::list<sGuildMatchFlagInfo> m_guildMatchFlags; // 0x4534 (only constructed so far)
	unsigned char m_block4540[0x1C];                 // 0x4540, a block the server sends whole (CLobbyTask::OnPacket 0xBE, then CUserInfo::Refresh)

public:
	bool m_gameOver;                                 // 0x455C

protected:
	PADDING(0x455D, 0x4560);

public:
	int m_gameMode;                                  // 0x4560
	unsigned char m_gameKind;                        // 0x4564, 4: no ghost recording (CGhostRecorder::IsSaveAvailable, FamilyMain, ClientSetting, TrainingDlg)

protected:
	PADDING(0x4565, 0x4568);
	unsigned long m_matchTypeId;                     // 0x4568, the match's type id (CItemManager::FindMatch; FrResultTeamDlg::OnMatchInit)
	unsigned char m_value456C;                       // 0x456C, cleared by ClearGameVars

public:
	unsigned char m_winningTeam;                     // 0x456D, the team game's winner: 0 red, 1 blue, 2 draw (FrResultDlg)

protected:
	sAwardPlayer m_awardPlayers[6];                  // 0x456E, the award ceremony's players (FrAwardDlg)
	sAwardPlayer m_winnerPlayers[6];                 // 0x459E, the winners (FrAwardDlg)

public:
	unsigned char m_holePin[18];                     // 0x45CE, the pin position (0-2) of each played hole (TrainingDlg draws it with rand() % 3)

protected:
	unsigned char m_holeCourse[18];                  // 0x45E0, course map of each played hole (CGolfBg::OnLoad/DoReLoad)

public:
	unsigned char m_holeOrder[18];                   // 0x45F2, course hole of each played hole

protected:
	PADDING(0x4604, 0x4608);

public:
	unsigned long m_holeRandom[18];                  // 0x4608, the per-hole random table (GolfRuleApproach::GetRandomStartPos)
	unsigned long m_gameStartTime;                   // 0x4650, g_CurrentTime when the game started (CClock::OnProcess)

	float m_pangRate;                                // 0x4654, the game's pang multiplier: bonus = (m_pang + m_extraPang) * m_pangPercent * m_pangRate / 100 (FrResultDlg::OnBonusInit)
	unsigned int m_pangPercent;                      // 0x4658, see m_pangRate

protected:
	PADDING(0x465C, 0x4660);
	unsigned char m_curMap;                          // 0x4660, +0x80 for a special course
	PADDING(0x4661, 0x4664);

public:
	IFF_STRUCT::sCourse *m_pCurCourse;               // 0x4664, the item manager's record of m_curMap (SetCurMap)
	unsigned char m_roomType;                        // 0x4668 (6 guild, 7 shows the shot board)
	char m_roomName[0x104];                          // 0x4669, the game type's room name (SetGameType)
	unsigned char m_holes;                           // 0x476D

protected:
	unsigned char m_weather;                         // 0x476E
	PADDING(0x476F, 0x4770);

public:
	unsigned long m_shotTimeLimit;                   // 0x4770, the rule classes' SetPlayer clears it offline

public:
	unsigned long m_gameTimeLimit;                   // 0x4774

public:
	unsigned char m_courseHoleOn[18];                // 0x4778, per course hole (index course hole - 1): nonzero when the hole is played (FrScoreDlg::OnHoleListOwnerDraw/GetMyCurHoleInGuildMatch)

protected:
	PADDING(0x478A, 0x4790);

public:
	sTeamResult m_teamResult[2];                     // 0x4790, red and blue (FrResultDlg::FindWinner)
	short m_teamPoint[2];                            // 0x47B0, red and blue, shown instead of m_teamResult's score when m_roomType is not 5

protected:
	unsigned long m_value47B4[2];                    // 0x47B4, cleared by ClearGameVars
	PADDING(0x47BC, 0x47C0);

public:
	__int64 m_quitGamePang;                          // 0x47C0, the player's game pang when he leaves the game early, else 0 (CGolfRule::ReturnToLobbyDlgResult)

protected:
	PADDING(0x47C8, 0x47CC);
	unsigned long m_value47CC;                       // 0x47CC (cleared by the constructor)
	bool m_flag47D0;                                 // 0x47D0 (cleared by the constructor)
	PADDING(0x47D1, 0x47D4);

public:
	unsigned long m_myRoomOwnerUID;                  // 0x47D4, set to MyUID() by CTaskMain::OnUnderBar_ItemStorageUp before MakeRealMyRoom

public:
	unsigned long m_realMyRoomUID;                   // 0x47D8, the uid of the My Room being entered (CTaskMain::HandleMsgCommon)

protected:
	PADDING(0x47DC, 0x47DE);

public:
	bool m_authSetting;                              // 0x47DE, copied into CRealMyRoom_AuthorityDlg::m_bAuthSetting
	char m_authPassword[0x64];                       // 0x47DF, the room's password (CRealMyRoom_AuthorityDlg::OnAuth_PasswordInit); real length not known, at most up to 0x4843
	unsigned char m_value4843;                       // 0x4843, cleared by ClearGameVars
	unsigned char m_value4844;                       // 0x4844, cleared by ClearGameVars
	sPangYaUserStatistics m_oldSeasonStatistics;     // 0x4845, the previous season's statistics (ClearOldSeasonStat)
	sTrophyStatistics m_oldSeasonTrophy;             // 0x4930, the previous season's medals (ClearOldSeasonStat)
	PADDING(0x497E, 0x4980);
	std::list<sSpecialTrophy> m_specialTrophies;     // 0x4980 (constructor)
	std::list<sGuildTrophy> m_guildTrophies;         // 0x498C (constructor)

	// The room the player is in: m_holeCount (CLobbyMain::ReLoadHoleItem
	// compares it with 18), m_gameType (6: guild lobby, FrUserListDlg disables
	// the invite button), m_roomNumber (0xFFFF: none, in the lobby;
	// CGMToolkit::CommandDestroy), the guilds of a guild match from m_guildUID
	// on (an sGuildMatchInfo), m_roomMode (14 shows the blank score panels).
	sRoomInfo m_roomInfo;                            // 0x4998

protected:
	PADDING(0x4A4A, 0x4A4C);

public:
	std::map<unsigned long, sBriefUserInfo> m_userMap; // 0x4A4C, the lobby's players by UID (FrUserListDlg)

public:
	std::list<sRoomInfo> m_roomList;                 // 0x4A58, read by CIntrusion::IsIntrusionGame

public:
	std::list<sChannelInfo> m_channelList;           // 0x4A64, the game server's channels (CProjectG::OnGameServerResult clears it)

public:
	std::list<sRoomPlayer> m_roomPlayerList;         // 0x4A70, walked by the rule classes' SetPlayer

public:
	std::list<sRoomPlayer> m_roomPlayerList2;        // 0x4A7C, a second list of the room's players (CLobbyTask::OnPacket 0xAC)

public:
	std::list<sGameServerInfo> m_gameServerList;     // 0x4A88, the game servers (FrServerDlg::BuildServerList, CProjectG::OnGameServerResult)

protected:
	std::vector<unsigned long> m_vector4A94;         // 0x4A94 (only constructed so far; element type not known)
	std::list<sRoomUserInfo> m_roomUserList;         // 0x4AA4 (only constructed so far)

public:
	const char* m_lobbyMessage;                      // 0x4AB0, reset to "" by CLobbyMain's constructor (role not known)
	int m_lobbyStateValue;                           // 0x4AB4, zeroed by ClearLobbyStateVars (role not known)

public:
	sChannelInfo m_currentChannel;                   // 0x4AB8, its flags are read by FrMakeRoomDlg and FrReplayModeDlg

	char m_serverName[0x0D];                         // 0x4B05, the connected game server's name (FrServerDlg::TurnOnChannels); length runs to the next member
	sChannelInfo m_serverChannel;                    // 0x4B12, flags tested by _util::IsAvatarChatServer

protected:
	PADDING(0x4B5F, 0x4B64);
	std::list<unsigned long> m_list4B64;             // 0x4B64 (only constructed so far)
	std::map<int, GuildMemberInfo_t> m_guildMembers; // 0x4B70 (only constructed so far)

public:
	bool m_paranNickChanged;                         // 0x4B7C, tested by LobbyMain OnParanNickChangeResult
	bool m_noticeShown;                              // 0x4B7D, set by CTaskMain::OnUnderBar_NoticeUp
	bool m_3DBackgroundOff;                          // 0x4B7E, TurnOff3DBackground
	bool m_lobbyStateFlag;                           // 0x4B7F, cleared by ClearLobbyStateVars (role not known)
	bool m_lobbyStateFlag2;                          // 0x4B80, cleared by ClearLobbyStateVars (role not known)

protected:
	sChannelInfo m_selectedChannel;                  // 0x4B81, the channel chosen in FrServerDlg::OnChannelListBtnDown
	PADDING(0x4BCE, 0x4BD0);
	std::map<unsigned long, sFriend> m_friendMap;    // 0x4BD0 (only constructed so far)
	sFriend m_myFriendInfo;                          // 0x4BDC (only constructed so far)
	PADDING(0x4C69, 0x4C6C);

public:
	ChatManager m_chatManager;                       // 0x4C6C, chat filters, flood control, chat commands

protected:
	PADDING(0x4C90, 0x4CA4);

public:
	std::list<sChatLine> m_chatLineList;             // 0x4CA4, the chat view's lines (FrChatFuncDlg::OnChatViewInit)
	std::list<std::string> m_chatHistory;            // 0x4CB0, the chat lines already sent (ChatManager::RecordMyChatHistory keeps 20)

public:
	std::list<std::string> m_ignoreList;             // 0x4CBC, nicknames FrFriendInfoDlg::OnIgnoreBtnUp adds/removes

protected:
	PADDING(0x4CC8, 0x4CCC);

public:
	std::list<std::string> m_recentWhisperList;      // 0x4CCC, the nicknames offered as whisper targets (InitRecentWhisperList)

public:
	std::list<std::string> m_list4CD8;               // 0x4CD8, the chat targets (all, team, guild; CLobbyMain::ProcessDialogKeyboard cycles them with Get{Prev,Next}ChatTarget)

protected:
	int m_value4CE4;                                 // 0x4CE4, 8 at construction
	std::string m_string4CE8;                        // 0x4CE8 (only constructed so far)
	unsigned long m_value4D04;                       // 0x4D04, cleared by ClearCommonVars

public:
	std::list<sNoteInfo> m_noteList;                 // 0x4D08, the received notes (FrMailBoxDlg)

protected:
	std::string m_string4D14;                        // 0x4D14 (only constructed so far)

public:
	std::string m_latestVersion;                     // 0x4D30, the client version the server announced (CLobbyMain::LatestVersion compares it with the own one)

protected:
	std::string m_string4D4C;                        // 0x4D4C, "" at construction
	int m_controlServerService;                      // 0x4D68
	int m_offlinePlay;                               // 0x4D6C
	unsigned long m_value4D70;                       // 0x4D70, cleared by ClearCommonVars
	unsigned long m_value4D74;                       // 0x4D74, likewise

public:
	unsigned long m_serverProperty;                  // 0x4D78, eGameServerProperty bits (S5::UTIL::IsEventServerControl)

protected:
	unsigned char m_value4D7C;                       // 0x4D7C, cleared by ClearGameVars

public:
	bool m_logoutPending;                            // 0x4D7D, cleared by FrLogoutDlg::OnLogoutBtnUp when set

protected:
	unsigned char m_value4D7E;                       // 0x4D7E, 0xFF after ClearCommonVars

public:
	bool m_newMapEvent;                              // 0x4D7F, CTaskManager sets it with the flags (ClearCommonVars clears both)
	int m_newMapEventFlags;                          // 0x4D80

protected:
	bool m_flag4D84;                                 // 0x4D84, cleared by ClearCommonVars
	PADDING(0x4D85, 0x4D88);
	int m_value4D88;                                 // 0x4D88, -1 after ClearCommonVars

public:
	unsigned long m_tutorialData[3];                 // 0x4D8C, written by CTutorialTask::OnPacket (one element, or all 12 bytes by DecodeBuffer)

protected:
	unsigned long m_value4D98;                       // 0x4D98, 15 after ClearCommonVars
	std::string m_string4D9C;                        // 0x4D9C, "" after ClearCommonVars
	int m_value4DB8;                                 // 0x4DB8, -1 after ClearCommonVars

public:
	unsigned char m_tutorialFinish;                  // 0x4DBC, read by TutorialMain OnTutorialMainFinish

protected:
	unsigned char m_value4DBD;                       // 0x4DBD, cleared by ClearCommonVars
	unsigned char m_value4DBE;                       // 0x4DBE, likewise
	PADDING(0x4DBF, 0x4DC0);

public:
	sLevelExp m_levelExp[71];                        // 0x4DC0, filled by LoadLevelTable

public:
	sCaddieLevelExp m_caddieLevelExp[3];             // 0x4FF8, per caddie level below 3 (FrCaddieInfoDlg::OnInit)

public:
	unsigned long m_trophyUID[2];                    // 0x5010, by team, a guild match player: the guild trophy shows when his team won (FrResultTeamDlg::OnStageInit/OnInit)

protected:
	unsigned long m_value5018;                       // 0x5018, cleared by ClearCommonVars
	int m_matchGameCount;                            // 0x501C, the match's game count ("%d" in FrResultTeamDlg::OnMatchInit)
	bool m_flag5020;                                 // 0x5020, cleared by ClearCommonVars
	PADDING(0x5021, 0x5024);
	std::string m_string5024;                        // 0x5024 (only constructed so far)
	CGolfDoc *m_pGolfDoc;                            // 0x5040
	unsigned long m_value5044;                       // 0x5044, cleared by ClearCommonVars
	unsigned long m_value5048;                       // 0x5048, likewise
	unsigned long m_value504C;                       // 0x504C, likewise
	int m_value5050;                                 // 0x5050, 1 after ClearCommonVars
	unsigned char m_value5054;                       // 0x5054, cleared by ClearCommonVars
	unsigned char m_value5055;                       // 0x5055, likewise
	unsigned char m_value5056;                       // 0x5056, likewise
	PADDING(0x5057, 0x5058);
	unsigned short m_value5058;                      // 0x5058, likewise
	unsigned char m_value505A;                       // 0x505A, likewise
	PADDING(0x505B, 0x505C);

public:
	// The top page's point event (CLobbyMain::OnToppage_PointEventDown sets
	// the state to 2 and clears the flag when it requests the event, packet 0x95).
	int m_pointEventState;                           // 0x505C
	unsigned long m_value5060;                       // 0x5060, a dword of the login reply (CTask::OnPacketCommon 0x42)
	int m_value5064;                                 // 0x5064, cleared by ClearCommonVars
	bool m_pointEventFlag;                           // 0x5068

protected:
	PADDING(0x5069, 0x506C);
	unsigned long m_value506C;                       // 0x506C, cleared by ClearCommonVars

public:
	unsigned long m_realEquip[0x1B];                 // 0x5070

protected:
	std::list<sMapEventInfo> m_mapEventList;       // 0x50DC, cleared by ClearAll where local content 0x39 is on
	std::list<sShotDataStorage> m_shotDataList;      // 0x50E8, std::list<CGolfRule::sShotData> in the original; cleared by ClearAll where local content 0x4F is on
	bool m_flag50F4;                                 // 0x50F4, cleared by ClearCommonVars

public:
	bool m_storageLock;                              // 0x50F5, copied into CRealMyRoom_AuthorityDlg::m_bStorageLock

protected:
	bool m_flag50F6;                                 // 0x50F6, cleared by ClearCommonVars
	PADDING(0x50F7, 0x50F8);

public:
	CPartTidList m_myPartTidList;                    // 0x50F8, GetMyPartTidList

protected:
	unsigned long m_myAuxPartTidList[5];             // 0x51C0
	bool m_haveAngelWing;                            // 0x51D4
	PADDING(0x51D5, 0x51D8);
	std::string m_loginAuthKey;                      // 0x51D8, GetLoginAuthKey
	unsigned short m_securityKey;                    // 0x51F4
	PADDING(0x51F6, 0x51F8);
	unsigned long m_flagAbilityItem;                 // 0x51F8
	unsigned char m_numBonusPangItem;                // 0x51FC
	PADDING(0x51FD, 0x5200);
	eStateTrade m_stateTrade;                        // 0x5200
	std::list<sTradeItem> m_listTradeItem;           // 0x5204
	std::list<sItemAttachCard> m_itemAttachCardList; // 0x5210 (only constructed so far)
	unsigned long m_tradeUID;                        // 0x521C
	std::string m_tradeTitle;                        // 0x5220, the personal shop's title
	int m_countVisitor;                              // 0x523C
	unsigned char m_tradeMode;                       // 0x5240
	sStall m_stallItem;                              // 0x5241
	std::list<sStall> m_listStallItem;               // 0x5B68
	std::list<sSaleItem> m_maketList;                // 0x5B74, GetMaketList
	unsigned char m_rmrMode;                         // 0x5B80
	PADDING(0x5B81, 0x5B88);
	__int64 m_tradeIncome;                           // 0x5B88
	bool m_haveHalloweenItem;                        // 0x5B90
	bool m_wearHalloweenItem;                        // 0x5B91
	PADDING(0x5B92, 0x5B94);

public:
	int m_replayState;                               // 0x5B94, 14 once FrReplayModeDlg starts a replay
	int m_replayPlayType;                            // 0x5B98, read and written directly by FrReplayDlg

public:
	// Read directly by the rule classes (no getter call in the original).
	eReplayModeType m_playingMode;                   // 0x5B9C
	char m_fileName[0x20];                           // 0x5BA0, the replay file LoadShot opens
	int m_replayValue5BC0;                           // 0x5BC0, cleared by the constructor
	// Set by FrReplayModeDlg::OnOkBtnUp when a replay starts.
	int m_replayStartA;                              // 0x5BC4, cleared
	unsigned long m_replayChannelFlags;              // 0x5BC8, the current channel's flags
	int m_replayStartB;                              // 0x5BCC, cleared

protected:
	bool m_flag5BD0;                                 // 0x5BD0, cleared by the constructor
	PADDING(0x5BD1, 0x5BD4);
	int m_value5BD4;                                 // 0x5BD4, 6385 at construction
	unsigned long m_serverTimeTick;                  // 0x5BD8, GetTickCount() when SetServerTime received m_serverTime
	bool m_flag5BDC;                                 // 0x5BDC, cleared by ClearCommonVars
	PADDING(0x5BDD, 0x5BE0);
	CPeriodContents *m_pPeriodDoc;                   // 0x5BE0
	int m_nowGoWithMode;                             // 0x5BE4
	int m_goWithFriendUID;                           // 0x5BE8
	sRoomInfo m_roomInfo2;                           // 0x5BEC (only constructed so far)
	unsigned char m_directMoveServer;                // 0x5C9E
	PADDING(0x5C9F, 0x5CA0);
	unsigned short m_directMoveRoom;                 // 0x5CA0
	PADDING(0x5CA2, 0x5CA4);
	unsigned long m_directMoveUID;                   // 0x5CA4
	unsigned long m_friendGameSvrUID;                // 0x5CA8
	unsigned long m_value5CAC;                       // 0x5CAC, zeroed by CLobbyTask::OnPacket 0x47 (entering a room) when IsLocalContent(0x54); role not known
	PADDING(0x5CB0, 0x5CB4);

public:
	std::map<unsigned long, sUserInfoTime> m_userInfoTimeMap; // 0x5CB4, the players' information by UID

protected:
	int m_userInfoMod;                               // 0x5CC0

public:
	unsigned char m_forceNicknameChange;             // 0x5CC4, tested by CLobbyMain

protected:
	PADDING(0x5CC5, 0x5CC8);
	std::map<unsigned char, sMapStatistics> m_myPastMapStat[4]; // 0x5CC8 (GetMyPastMapStat)
	std::map<std::string, std::list<std::string> > m_visGroups; // 0x5CF8 (GetVisGroup)
	bool m_goStopMode;                               // 0x5D04
	PADDING(0x5D05, 0x5D08);
public:
	unsigned long m_value5D08;                       // 0x5D08, cleared by ClearCommonVars; 2 makes CLobbyMain::ProcessLobbyUI open the treasure approach gift (room type 10)
protected:
	bool m_flag5D0C;                                 // 0x5D0C, cleared by the constructor
	PADDING(0x5D0D, 0x5D10);

public:
	std::list<sCardStack> m_cardList;                // 0x5D10, the card stacks the player owns (GetCardInfoToUID / GetCardInfoToTID)

protected:
	std::list<sBurnningSpCard> m_burningSpCards;     // 0x5D1C (only constructed so far)
public:
	bool m_loginDlgCreated;                          // 0x5D28, set by FrLoginDlg's constructor
	PADDING(0x5D29, 0x5D30);
};
#pragma pack(pop)

CHECK_SIZE(CSharedDoc, 0x5D30);
BEGIN_LAYOUT_CHECK(CSharedDoc)
	CHECK_FIELD(CSharedDoc, m_myMapStatistics, 0x694);
	CHECK_FIELD(CSharedDoc, m_equipSkin, 0x678);
	CHECK_FIELD(CSharedDoc, m_realMyRoomUID, 0x47D8);
	CHECK_FIELD(CSharedDoc, m_stringMap, 0x114C);
	CHECK_FIELD(CSharedDoc, m_userInfoExtBlock, 0xD4C);
	CHECK_FIELD(CSharedDoc, m_value5060, 0x5060);
	CHECK_FIELD(CSharedDoc, m_questList, 0x1278);
	CHECK_FIELD(CSharedDoc, m_gainedItemList, 0x44C4);
	CHECK_FIELD(CSharedDoc, m_guildMatchFlags, 0x4534);
	CHECK_FIELD(CSharedDoc, m_roomInfo, 0x4998);
	CHECK_FIELD(CSharedDoc, m_roomUserList, 0x4AA4);
	CHECK_FIELD(CSharedDoc, m_guildMembers, 0x4B70);
	CHECK_FIELD(CSharedDoc, m_myFriendInfo, 0x4BDC);
	CHECK_FIELD(CSharedDoc, m_string4CE8, 0x4CE8);
	CHECK_FIELD(CSharedDoc, m_string4D9C, 0x4D9C);
	CHECK_FIELD(CSharedDoc, m_string5024, 0x5024);
	CHECK_FIELD(CSharedDoc, m_shotDataList, 0x50E8);
	CHECK_FIELD(CSharedDoc, m_itemAttachCardList, 0x5210);
	CHECK_FIELD(CSharedDoc, m_value5BD4, 0x5BD4);
	CHECK_FIELD(CSharedDoc, m_roomInfo2, 0x5BEC);
	CHECK_FIELD(CSharedDoc, m_myPastMapStat, 0x5CC8);
	CHECK_FIELD(CSharedDoc, m_visGroups, 0x5CF8);
	CHECK_FIELD(CSharedDoc, m_burningSpCards, 0x5D1C);
	CHECK_FIELD(CSharedDoc, m_loginServerAddress, 0x28);
	CHECK_FIELD(CSharedDoc, m_gameServerAddress, 0x44);
	CHECK_FIELD(CSharedDoc, m_gameServerPort, 0x7C);
	CHECK_FIELD(CSharedDoc, m_loginConnectValue, 0x8C);
	CHECK_FIELD(CSharedDoc, m_rankServerAddress, 0x98);
	CHECK_FIELD(CSharedDoc, m_rankServerPort, 0xB4);
	CHECK_FIELD(CSharedDoc, m_loginTime, 0xC8);
	CHECK_FIELD(CSharedDoc, m_serverTime, 0x158);
	CHECK_FIELD(CSharedDoc, m_itemManager, 0x1E8);
	CHECK_FIELD(CSharedDoc, m_cardPeriodList, 0x1248);
	CHECK_FIELD(CSharedDoc, m_myCash, 0x1288);
	CHECK_FIELD(CSharedDoc, m_myBonusCash, 0x1298);
	CHECK_FIELD(CSharedDoc, m_pCurCourse, 0x4664);
	CHECK_FIELD(CSharedDoc, m_authPassword, 0x47DF);
	CHECK_FIELD(CSharedDoc, m_value90, 0x90);
	CHECK_FIELD(CSharedDoc, m_gameStatistics, 0x4118);
	CHECK_FIELD(CSharedDoc, m_value456C, 0x456C);
	CHECK_FIELD(CSharedDoc, m_value47B4, 0x47B4);
	CHECK_FIELD(CSharedDoc, m_value4843, 0x4843);
	CHECK_FIELD(CSharedDoc, m_value4844, 0x4844);
	CHECK_FIELD(CSharedDoc, m_oldSeasonStatistics, 0x4845);
	CHECK_FIELD(CSharedDoc, m_oldSeasonTrophy, 0x4930);
	CHECK_FIELD(CSharedDoc, m_value4D7C, 0x4D7C);
	CHECK_FIELD(CSharedDoc, m_userInfo, 0x3E2);
	CHECK_FIELD(CSharedDoc, m_userInfo.nickname, 0x3F8);
	CHECK_FIELD(CSharedDoc, m_userInfo.guildName, 0x40E);
	CHECK_FIELD(CSharedDoc, m_userInfo.guildMark, 0x423);
	CHECK_FIELD(CSharedDoc, m_userInfo.identity, 0x433);
	CHECK_FIELD(CSharedDoc, m_userInfo.galleryGuid, 0x437);
	CHECK_FIELD(CSharedDoc, m_userInfo.guid, 0x43B);
	CHECK_FIELD(CSharedDoc, m_userInfo.guildUID, 0x44B);
	CHECK_FIELD(CSharedDoc, m_userInfo.guildPosition, 0x44F);
	CHECK_FIELD(CSharedDoc, m_userInfo.flagBlock, 0x45F);
	CHECK_FIELD(CSharedDoc, m_loginId, 0x46B);
	CHECK_FIELD(CSharedDoc, m_myUID, 0x4EB);
	CHECK_FIELD(CSharedDoc, m_myStatShots, 0x4EF);
	CHECK_FIELD(CSharedDoc, m_myStatPuttAttempts, 0x4F3);
	CHECK_FIELD(CSharedDoc, m_myStatPangyas, 0x503);
	CHECK_FIELD(CSharedDoc, m_myStatDrives, 0x513);
	CHECK_FIELD(CSharedDoc, m_myStatPar3Drives, 0x517);
	CHECK_FIELD(CSharedDoc, m_myStatHoleInOnes, 0x51B);
	CHECK_FIELD(CSharedDoc, m_myStatFairways, 0x51F);
	CHECK_FIELD(CSharedDoc, m_myStatPuttsSunk, 0x529);
	CHECK_FIELD(CSharedDoc, m_myExp, 0x535);
	CHECK_FIELD(CSharedDoc, m_myStatGamesPlayed, 0x57C);
	CHECK_FIELD(CSharedDoc, m_myStatNoMannerGames, 0x5A8);
	CHECK_FIELD(CSharedDoc, m_myTrophy, 0x5DA);
	CHECK_FIELD(CSharedDoc, m_myLevel, 0x539);
	CHECK_FIELD(CSharedDoc, m_roomName, 0x4669);
	CHECK_FIELD(CSharedDoc, m_paranNickChanged, 0x4B7C);
	CHECK_FIELD(CSharedDoc, m_tutorialFinish, 0x4DBC);
	CHECK_FIELD(CSharedDoc, m_value80, 0x80);
	CHECK_FIELD(CSharedDoc, m_value84, 0x84);
	CHECK_FIELD(CSharedDoc, m_value88, 0x88);
	CHECK_FIELD(CSharedDoc, m_value94, 0x94);
	CHECK_FIELD(CSharedDoc, m_value4D04, 0x4D04);
	CHECK_FIELD(CSharedDoc, m_value4D70, 0x4D70);
	CHECK_FIELD(CSharedDoc, m_value4D74, 0x4D74);
	CHECK_FIELD(CSharedDoc, m_value4D7E, 0x4D7E);
	CHECK_FIELD(CSharedDoc, m_flag4D84, 0x4D84);
	CHECK_FIELD(CSharedDoc, m_value4D88, 0x4D88);
	CHECK_FIELD(CSharedDoc, m_value4D98, 0x4D98);
	CHECK_FIELD(CSharedDoc, m_value4DB8, 0x4DB8);
	CHECK_FIELD(CSharedDoc, m_value4DBD, 0x4DBD);
	CHECK_FIELD(CSharedDoc, m_value4DBE, 0x4DBE);
	CHECK_FIELD(CSharedDoc, m_value5018, 0x5018);
	CHECK_FIELD(CSharedDoc, m_flag5020, 0x5020);
	CHECK_FIELD(CSharedDoc, m_value5044, 0x5044);
	CHECK_FIELD(CSharedDoc, m_value5048, 0x5048);
	CHECK_FIELD(CSharedDoc, m_value504C, 0x504C);
	CHECK_FIELD(CSharedDoc, m_value5050, 0x5050);
	CHECK_FIELD(CSharedDoc, m_value5054, 0x5054);
	CHECK_FIELD(CSharedDoc, m_value5055, 0x5055);
	CHECK_FIELD(CSharedDoc, m_value5056, 0x5056);
	CHECK_FIELD(CSharedDoc, m_value5058, 0x5058);
	CHECK_FIELD(CSharedDoc, m_value505A, 0x505A);
	CHECK_FIELD(CSharedDoc, m_value5064, 0x5064);
	CHECK_FIELD(CSharedDoc, m_value506C, 0x506C);
	CHECK_FIELD(CSharedDoc, m_flag50F4, 0x50F4);
	CHECK_FIELD(CSharedDoc, m_flag50F6, 0x50F6);
	CHECK_FIELD(CSharedDoc, m_flag5BDC, 0x5BDC);
	CHECK_FIELD(CSharedDoc, m_value5D08, 0x5D08);
	CHECK_FIELD(CSharedDoc, m_myPang, 0x53A);
	CHECK_FIELD(CSharedDoc, m_myCharacterId, 0x62C);
	CHECK_FIELD(CSharedDoc, m_mySlot, 0x3E0);
	CHECK_FIELD(CSharedDoc, m_myCaddieId, 0x628);
	CHECK_FIELD(CSharedDoc, m_myBallId, 0x634);
	CHECK_FIELD(CSharedDoc, m_value5CAC, 0x5CAC);
	CHECK_FIELD(CSharedDoc, m_matchTypeId, 0x4568);
	CHECK_FIELD(CSharedDoc, m_quitGamePang, 0x47C0);
	CHECK_FIELD(CSharedDoc, m_matchGameCount, 0x501C);
	CHECK_FIELD(CSharedDoc, m_myClubId, 0x630);
	CHECK_FIELD(CSharedDoc, m_equipItem, 0x638);
	CHECK_FIELD(CSharedDoc, m_caddieMap, 0x1164);
	CHECK_FIELD(CSharedDoc, m_myMascotId, 0x690);
	CHECK_FIELD(CSharedDoc, m_myMascotMap, 0x1170);
	CHECK_FIELD(CSharedDoc, m_cardList, 0x5D10);
	CHECK_FIELD(CSharedDoc, m_myItemMap, 0x1188);
	CHECK_FIELD(CSharedDoc, m_mySkinList, 0x11F4);
	CHECK_FIELD(CSharedDoc, m_outsideCash, 0x1290);
	CHECK_FIELD(CSharedDoc, m_myItemList, 0x12A0);
	CHECK_FIELD(CSharedDoc, m_myAuxItemList, 0x12AC);
	CHECK_FIELD(CSharedDoc, m_userInfo.timeBlock, 0x463);
	CHECK_FIELD(CSharedDoc, m_friendGameSvrUID, 0x5CA8);
	CHECK_FIELD(CSharedDoc, m_forceNicknameChange, 0x5CC4);
	CHECK_FIELD(CSharedDoc, m_roomPlayers, 0x12D0);
	CHECK_FIELD(CSharedDoc, m_characterMap, 0x1158);
	CHECK_FIELD(CSharedDoc, m_roomSlotMap, 0x12B8);
	CHECK_FIELD(CSharedDoc, m_indexMap, 0x12C4);
	CHECK_FIELD(CSharedDoc, m_userInfoTimeMap, 0x5CB4);
	CHECK_FIELD(CSharedDoc, m_userMap, 0x4A4C);
	CHECK_FIELD(CSharedDoc, m_roomList, 0x4A58);
	CHECK_FIELD(CSharedDoc, m_channelList, 0x4A64);
	CHECK_FIELD(CSharedDoc, m_roomPlayerList, 0x4A70);
	CHECK_FIELD(CSharedDoc, m_gameResultList, 0x4504);
	CHECK_FIELD(CSharedDoc, m_block4540, 0x4540);
	CHECK_FIELD(CSharedDoc, m_gameOver, 0x455C);
	CHECK_FIELD(CSharedDoc, m_gameMode, 0x4560);
	CHECK_FIELD(CSharedDoc, m_holeOrder, 0x45F2);
	CHECK_FIELD(CSharedDoc, m_holePin, 0x45CE);
	CHECK_FIELD(CSharedDoc, m_awardPlayers, 0x456E);
	CHECK_FIELD(CSharedDoc, m_winnerPlayers, 0x459E);
	CHECK_FIELD(CSharedDoc, m_holeCourse, 0x45E0);
	CHECK_FIELD(CSharedDoc, m_levelExp, 0x4DC0);
	CHECK_FIELD(CSharedDoc, m_caddieLevelExp, 0x4FF8);
	CHECK_FIELD(CSharedDoc, m_pangRate, 0x4654);
	CHECK_FIELD(CSharedDoc, m_pangPercent, 0x4658);
	CHECK_FIELD(CSharedDoc, m_specialTrophies, 0x4980);
	CHECK_FIELD(CSharedDoc, m_guildTrophies, 0x498C);
	CHECK_FIELD(CSharedDoc, m_courseHoleOn, 0x4778);
	CHECK_FIELD(CSharedDoc, m_gameTypeNames, 0xD8);
	CHECK_FIELD(CSharedDoc, m_gameDesc, 0xCC);
	CHECK_FIELD(CSharedDoc, m_japanState, 0x184);
	CHECK_FIELD(CSharedDoc, m_billingType, 0x1A4);
	CHECK_FIELD(CSharedDoc, m_commandLineAccount, 0x1A8);
	CHECK_FIELD(CSharedDoc, m_japanFlag, 0x1C4);
	CHECK_FIELD(CSharedDoc, m_japanParam1, 0x1C8);
	CHECK_FIELD(CSharedDoc, m_japanParam2, 0x1CC);
	CHECK_FIELD(CSharedDoc, m_gameStartTime, 0x4650);
	CHECK_FIELD(CSharedDoc, m_holeRandom, 0x4608);
	CHECK_FIELD(CSharedDoc, m_myStatHolesPlayed, 0x5C6);
	CHECK_FIELD(CSharedDoc, m_curMap, 0x4660);
	CHECK_FIELD(CSharedDoc, m_roomType, 0x4668);
	CHECK_FIELD(CSharedDoc, m_holes, 0x476D);
	CHECK_FIELD(CSharedDoc, m_weather, 0x476E);
	CHECK_FIELD(CSharedDoc, m_shotTimeLimit, 0x4770);
	CHECK_FIELD(CSharedDoc, m_gameTimeLimit, 0x4774);
	CHECK_FIELD(CSharedDoc, m_gameKind, 0x4564);
	CHECK_FIELD(CSharedDoc, m_winningTeam, 0x456D);
	CHECK_FIELD(CSharedDoc, m_teamResult, 0x4790);
	CHECK_FIELD(CSharedDoc, m_teamPoint, 0x47B0);
	CHECK_FIELD(CSharedDoc, m_myRoomOwnerUID, 0x47D4);
	CHECK_FIELD(CSharedDoc, m_serverName, 0x4B05);
	CHECK_FIELD(CSharedDoc, m_selectedChannel, 0x4B81);
	CHECK_FIELD(CSharedDoc, m_serverChannel, 0x4B12);
	CHECK_FIELD(CSharedDoc, m_noticeShown, 0x4B7D);
	CHECK_FIELD(CSharedDoc, m_serverProperty, 0x4D78);
	CHECK_FIELD(CSharedDoc, m_logoutPending, 0x4D7D);
	CHECK_FIELD(CSharedDoc, m_serverTimeTick, 0x5BD8);
	CHECK_FIELD(CSharedDoc, m_currentChannel, 0x4AB8);
	CHECK_FIELD(CSharedDoc, m_3DBackgroundOff, 0x4B7E);
	CHECK_FIELD(CSharedDoc, m_ignoreList, 0x4CBC);
	CHECK_FIELD(CSharedDoc, m_chatManager, 0x4C6C);
	CHECK_FIELD(CSharedDoc, m_chatLineList, 0x4CA4);
	CHECK_FIELD(CSharedDoc, m_chatHistory, 0x4CB0);
	CHECK_FIELD(CSharedDoc, m_itemInfoList, 0x11A0);
	CHECK_FIELD(CSharedDoc, m_pointEventState, 0x505C);
	CHECK_FIELD(CSharedDoc, m_pointEventFlag, 0x5068);
	CHECK_FIELD(CSharedDoc, m_myGearList, 0x1194);
	CHECK_FIELD(CSharedDoc, m_myExtraItemList, 0x11B8);
	CHECK_FIELD(CSharedDoc, m_myExtraItemList2, 0x11C4);
	CHECK_FIELD(CSharedDoc, m_giftList, 0x11D0);
	CHECK_FIELD(CSharedDoc, m_myExtraItemList3, 0x1218);
	CHECK_FIELD(CSharedDoc, m_myExtraItemList4, 0x1224);
	CHECK_FIELD(CSharedDoc, m_furnitureList, 0x1230);
	CHECK_FIELD(CSharedDoc, m_myCardList, 0x123C);
	CHECK_FIELD(CSharedDoc, m_gameServerList, 0x4A88);
	CHECK_FIELD(CSharedDoc, m_newMapEvent, 0x4D7F);
	CHECK_FIELD(CSharedDoc, m_newMapEventFlags, 0x4D80);
	CHECK_FIELD(CSharedDoc, m_myPartTidList, 0x50F8);
	CHECK_FIELD(CSharedDoc, m_recentWhisperList, 0x4CCC);
	CHECK_FIELD(CSharedDoc, m_loginAuthKey, 0x51D8);
	CHECK_FIELD(CSharedDoc, m_myPartsList, 0x117C);
	CHECK_FIELD(CSharedDoc, m_noteList, 0x4D08);
	CHECK_FIELD(CSharedDoc, m_latestVersion, 0x4D30);
	CHECK_FIELD(CSharedDoc, m_controlServerService, 0x4D68);
	CHECK_FIELD(CSharedDoc, m_offlinePlay, 0x4D6C);
	CHECK_FIELD(CSharedDoc, m_tutorialData, 0x4D8C);
	CHECK_FIELD(CSharedDoc, m_trophyUID, 0x5010);
	CHECK_FIELD(CSharedDoc, m_pGolfDoc, 0x5040);
	CHECK_FIELD(CSharedDoc, m_realEquip, 0x5070);
	CHECK_FIELD(CSharedDoc, m_mapEventList, 0x50DC);
	CHECK_FIELD(CSharedDoc, m_myAuxPartTidList, 0x51C0);
	CHECK_FIELD(CSharedDoc, m_haveAngelWing, 0x51D4);
	CHECK_FIELD(CSharedDoc, m_securityKey, 0x51F4);
	CHECK_FIELD(CSharedDoc, m_flagAbilityItem, 0x51F8);
	CHECK_FIELD(CSharedDoc, m_numBonusPangItem, 0x51FC);
	CHECK_FIELD(CSharedDoc, m_stateTrade, 0x5200);
	CHECK_FIELD(CSharedDoc, m_listTradeItem, 0x5204);
	CHECK_FIELD(CSharedDoc, m_tradeUID, 0x521C);
	CHECK_FIELD(CSharedDoc, m_tradeTitle, 0x5220);
	CHECK_FIELD(CSharedDoc, m_countVisitor, 0x523C);
	CHECK_FIELD(CSharedDoc, m_tradeMode, 0x5240);
	CHECK_FIELD(CSharedDoc, m_maketList, 0x5B74);
	CHECK_FIELD(CSharedDoc, m_stallItem, 0x5241);
	CHECK_FIELD(CSharedDoc, m_listStallItem, 0x5B68);
	CHECK_FIELD(CSharedDoc, m_rmrMode, 0x5B80);
	CHECK_FIELD(CSharedDoc, m_tradeIncome, 0x5B88);
	CHECK_FIELD(CSharedDoc, m_haveHalloweenItem, 0x5B90);
	CHECK_FIELD(CSharedDoc, m_wearHalloweenItem, 0x5B91);
	CHECK_FIELD(CSharedDoc, m_replayState, 0x5B94);
	CHECK_FIELD(CSharedDoc, m_replayPlayType, 0x5B98);
	CHECK_FIELD(CSharedDoc, m_playingMode, 0x5B9C);
	CHECK_FIELD(CSharedDoc, m_fileName, 0x5BA0);
	CHECK_FIELD(CSharedDoc, m_replayStartA, 0x5BC4);
	CHECK_FIELD(CSharedDoc, m_replayChannelFlags, 0x5BC8);
	CHECK_FIELD(CSharedDoc, m_replayStartB, 0x5BCC);
	CHECK_FIELD(CSharedDoc, m_replayFileList, 0x11AC);
	CHECK_FIELD(CSharedDoc, m_authSetting, 0x47DE);
	CHECK_FIELD(CSharedDoc, m_storageLock, 0x50F5);
	CHECK_FIELD(CSharedDoc, m_pPeriodDoc, 0x5BE0);
	CHECK_FIELD(CSharedDoc, m_nowGoWithMode, 0x5BE4);
	CHECK_FIELD(CSharedDoc, m_goWithFriendUID, 0x5BE8);
	CHECK_FIELD(CSharedDoc, m_directMoveServer, 0x5C9E);
	CHECK_FIELD(CSharedDoc, m_directMoveRoom, 0x5CA0);
	CHECK_FIELD(CSharedDoc, m_directMoveUID, 0x5CA4);
	CHECK_FIELD(CSharedDoc, m_userInfoMod, 0x5CC0);
	CHECK_FIELD(CSharedDoc, m_goStopMode, 0x5D04);
	CHECK_FIELD(CSharedDoc, m_loginDlgCreated, 0x5D28);
END_LAYOUT_CHECK

// shareddoc.h:559/564/1144: defined in the class body in the original. SharedDoc
// emitted the copies the linker kept: shareddoc.cpp defines SHAREDDOC_INLINE
// before its first include and defines them out of line itself.
#ifndef SHAREDDOC_INLINE
inline eStateTrade CSharedDoc::GetStateTrade(void) {
	return m_stateTrade;
}

inline std::list<sTradeItem> *CSharedDoc::GetListTradeItem(void) {
	return &m_listTradeItem;
}

// shareddoc.h:574
inline std::list<sStall> *CSharedDoc::GetListStallItem(void) {
	return &m_listStallItem;
}

// shareddoc.h:575
inline sStall *CSharedDoc::GetStallItem(void) {
	return &m_stallItem;
}

// shareddoc.h:1144
inline void CSharedDoc::SetUserInfoMod(int mod) {
	m_userInfoMod = mod;
}
#endif

// shareddoc.h: defined in the class body in the original. OfflineTradeDlg
// emitted the copies the linker kept: OfflineTradeDlg.cpp defines
// OFFLINETRADEDLG_SHAREDDOC_INLINE empty before its first include.
#ifndef OFFLINETRADEDLG_SHAREDDOC_INLINE
#define OFFLINETRADEDLG_SHAREDDOC_INLINE inline
#endif

// shareddoc.h:609
OFFLINETRADEDLG_SHAREDDOC_INLINE unsigned char CSharedDoc::GetTradeMode(void) {
	return m_tradeMode;
}

// shareddoc.h:583
OFFLINETRADEDLG_SHAREDDOC_INLINE std::list<sSaleItem> *CSharedDoc::GetMaketList(void) {
	return &m_maketList;
}

// Avatar_MainUI emitted the copy the linker kept: Avatar_MainUI.cpp defines
// AVATAR_MAINUI_SHAREDDOC_INLINE empty before its first include.
#ifndef AVATAR_MAINUI_SHAREDDOC_INLINE
#define AVATAR_MAINUI_SHAREDDOC_INLINE inline
#endif

// shareddoc.h:568
AVATAR_MAINUI_SHAREDDOC_INLINE unsigned long CSharedDoc::GetTradeUID(void) {
	return m_tradeUID;
}

// shareddoc.h:550-554: defined in the class body in the original.
// BongdariShopDlg emitted the copies the linker kept: BongdariShopDlg.cpp
// defines BONGDARISHOPDLG_SHAREDDOC_INLINE empty before its first include.
#ifndef BONGDARISHOPDLG_SHAREDDOC_INLINE
#define BONGDARISHOPDLG_SHAREDDOC_INLINE inline
#endif

// shareddoc.h:550
BONGDARISHOPDLG_SHAREDDOC_INLINE short CSharedDoc::GetBsUsableTimes(void) {
	return m_userInfo.bsUsableTimes;
}

// shareddoc.h:552
BONGDARISHOPDLG_SHAREDDOC_INLINE short CSharedDoc::GetBsUsableBonusTimes(void) {
	return m_userInfo.bsUsableBonusTimes;
}

// shareddoc.h:554
BONGDARISHOPDLG_SHAREDDOC_INLINE short CSharedDoc::GetBsRemainedBonusTimes(void) {
	return m_userInfo.bsRemainedBonusTimes;
}

// shareddoc.h:1132-1136: defined in the class body in the original.
// MessengerDlg emitted the copies the linker kept: MessengerDlg.cpp defines
// MESSENGERDLG_SHAREDDOC_INLINE empty before its first include.
#ifndef MESSENGERDLG_SHAREDDOC_INLINE
#define MESSENGERDLG_SHAREDDOC_INLINE inline
#endif

// shareddoc.h:1132
MESSENGERDLG_SHAREDDOC_INLINE void CSharedDoc::SetGoWithModeStart(int friendUID) {
	m_goWithFriendUID = friendUID;
}

// shareddoc.h:1136
MESSENGERDLG_SHAREDDOC_INLINE void CSharedDoc::SetGoWithModeEnd(void) {
	m_nowGoWithMode = 0;
	m_goWithFriendUID = -1;
}

// shareddoc.inl: global accessors of the shared document.
void SetHoles(unsigned char holes);
void SetWeather(unsigned char weather);
int IsGuildGame(void);
char *MyNick(void);
unsigned char GetPlayerNum(void);
WVector GetTeeShotPos(void);
struct sCameraData &GameCamera(void);
unsigned char GetCurMap(void);
bool SetCurMap(unsigned char map);
bool IsCurMapSnow(void);
int OnlinePlay(void);
__int64 MyOutsideCash(void);
int GalleryMode(void);
unsigned long MyGuid(bool galleryGuid);
float CalcPowerBarPosition(void);               // shareddoc.cpp:5077 area, 0x414cb0
void PerformPowerBarSetting(void);
WVector GetHoleCupPos(void);                    // shareddoc.inl:368 (SharedDoc's copy 0x40fc90)
unsigned char GetHoleIndex(unsigned char hole);
int IsMassGame(void);
int IsMassGame(unsigned char gameType);
unsigned char MyIndex(void);
bool IsMyTurn(bool);
bool IsOthersTurn(bool);
unsigned char GetHoles(void);
unsigned char GetStartHole(void);
unsigned char GetEndHole(void);
void SetGameOver(bool gameOver);
bool IsGameOver(void);
int IsObserver(unsigned long identity);
int IsAdministrator(unsigned long identity);
bool IsGameType(unsigned char type);

// shareddoc.cpp:2009, SortAllMyItemList's order: by type id (0x4073f0)
bool CompareItemInfo(const sItemInfo &a, const sItemInfo &b);

// Inline in the original (shareddoc.inl, expanded by their callers; GolfRule
// emitted the copies the linker kept unless noted). SharedDoc.cpp defines
// SHAREDDOC_INLINE empty before its first include, so its object emits the
// out-of-line copies the original's did.
#ifndef SHAREDDOC_INLINE
#define SHAREDDOC_INLINE inline
#endif

// shareddoc.inl:437. TaskMain emitted the copy the linker kept; TaskMain.cpp
// (TASKMAIN_SHAREDDOC_INLINE) defines it out of line itself.
#ifndef TASKMAIN_SHAREDDOC_INLINE
inline unsigned long MyGuid(bool galleryGuid) {
	if (WSingleton<CSharedDoc>::Instance()->m_userInfo.IsIdentity(2) && galleryGuid)
		return WSingleton<CSharedDoc>::Instance()->m_userInfo.galleryGuid;

	return WSingleton<CSharedDoc>::Instance()->m_userInfo.guid;
}
#endif

// TaskMain.cpp defines TASKMAIN_SHAREDDOC_INLINE empty before its first
// include, so its object emits the out-of-line copies of OfflinePlay and
// GalleryMode, as the original's did.
#ifndef TASKMAIN_SHAREDDOC_INLINE
#define TASKMAIN_SHAREDDOC_INLINE inline
#endif

// shareddoc.inl:139 (TaskMain emitted the copy the linker kept)
TASKMAIN_SHAREDDOC_INLINE int OfflinePlay(void) {
	return WSingleton<CSharedDoc>::Instance()->m_offlinePlay;
}

// shareddoc.inl:184 (SharedDoc emitted the copy the linker kept)
SHAREDDOC_INLINE void SetGameType(unsigned char type) {
	WSingleton<CSharedDoc>::Instance()->m_roomType = type;
	if (type < 15)
		strcpy(WSingleton<CSharedDoc>::Instance()->m_roomName, WSingleton<CSharedDoc>::Instance()->m_gameTypeNames[type].name);
}

// shareddoc.inl:288
inline unsigned char GetWeather(void) {
	return WSingleton<CSharedDoc>::Instance()->m_weather;
}

// shareddoc.inl:323
inline unsigned long GetShotTimeLimit(void) {
	return WSingleton<CSharedDoc>::Instance()->m_shotTimeLimit;
}

// shareddoc.inl:328 (SharedDoc emitted the copy the linker kept)
SHAREDDOC_INLINE CItemManager *ItemManager(void) {
	return &WSingleton<CSharedDoc>::Instance()->m_itemManager;
}

// The courses in the order the lobby offers them (FirstMap takes the first
// one that is not a special course, LastMap the last; 0x7F marks an unused
// slot). A file-local table of the original (0xa1ce08, no symbol).
static const unsigned char s_courseOrder[18] = {
	0x13, 0x10, 0x0F, 0x0E, 0x0D, 0x0B, 0x08, 0x0A, 0x00,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x09, 0x7F,
};

// LobbyMain.cpp defines LOBBYMAIN_SHAREDDOC_INLINE empty before its first
// include, so its object emits the out-of-line copies of FirstMap and
// LastMap, as the original's did.
#ifndef LOBBYMAIN_SHAREDDOC_INLINE
#define LOBBYMAIN_SHAREDDOC_INLINE inline
#endif

// shareddoc.inl:59
LOBBYMAIN_SHAREDDOC_INLINE unsigned char FirstMap(void) {
	if (!(WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800))
		return 0x13;

	if (IsLocalContent((localContentType_t)0x3A)) {
		if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800)
			return 0x0B;
	}

	int i;
	for (i = 0; i < 18; i++) {
		IFF_STRUCT::sCourse *course = ItemManager()->FindCourse(0x28000000 | s_courseOrder[i]);

		if ((course->courseFlags & 0x0F) < 3)
			break;
	}

	return s_courseOrder[i];
}

// shareddoc.h:1140
LOBBYMAIN_SHAREDDOC_INLINE void CSharedDoc::SetGoWithMode(int mode) {
	m_nowGoWithMode = mode;
}

// shareddoc.inl:83
LOBBYMAIN_SHAREDDOC_INLINE unsigned char LastMap(bool skipUnused) {
	if (!(WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800))
		return skipUnused ? 0x09 : 0x7F;

	int i;
	for (i = 17; i >= 0; i--) {
		if (skipUnused && s_courseOrder[i] == 0x7F)
			continue;

		IFF_STRUCT::sCourse *course = ItemManager()->FindCourse(0x28000000 | s_courseOrder[i]);

		if ((course->courseFlags & 0x0F) < 3)
			break;
	}

	return s_courseOrder[i];
}

// shareddoc.inl:7, the course before map in the selection order (map when
// none is selectable).
LOBBYMAIN_SHAREDDOC_INLINE unsigned char PrevMap(unsigned char map) {
	int index = 17;
	for (int i = 0; i < 18; i++) {
		if (s_courseOrder[i] == map) {
			index = i;
			break;
		}
	}

	for (int prev = index - 1; prev >= 0; prev--) {
		if (!IsLocalContent((localContentType_t)0x3A)
			|| !(WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800)
			|| WSingleton<CSharedDoc>::Instance()->CanUseRookieChannelMap(s_courseOrder[prev]))
			return s_courseOrder[prev];
	}

	return map;
}

// shareddoc.inl:33, the course after map in the selection order (map when
// none is selectable).
LOBBYMAIN_SHAREDDOC_INLINE unsigned char NextMap(unsigned char map, bool) {
	int index = 0;
	for (int i = 0; i < 18; i++) {
		if (s_courseOrder[i] == map) {
			index = i;
			break;
		}
	}

	for (int next = index + 1; next < 18; next++) {
		if (!IsLocalContent((localContentType_t)0x3A)
			|| !(WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800)
			|| WSingleton<CSharedDoc>::Instance()->CanUseRookieChannelMap(s_courseOrder[next]))
			return s_courseOrder[next];
	}

	return map;
}

// shareddoc.inl:356
inline CGolfDoc *GOLFDOC(void) {
	return WSingleton<CSharedDoc>::Instance()->m_pGolfDoc;
}

// shareddoc.inl:431
inline unsigned long GetGameTimeLimit(void) {
	return WSingleton<CSharedDoc>::Instance()->m_gameTimeLimit;
}

// shareddoc.inl:446 (SharedDoc emitted the copy the linker kept)
SHAREDDOC_INLINE unsigned long MyUID(void) {
	return WSingleton<CSharedDoc>::Instance()->m_myUID;
}

// GolfRule.cpp defines GOLFRULE_SHAREDDOC_INLINE empty before its first
// include, so its object emits the out-of-line copy of GUILDMATCHUP
// (0x452e80), as the original's did.
#ifndef GOLFRULE_SHAREDDOC_INLINE
#define GOLFRULE_SHAREDDOC_INLINE inline
#endif

// shareddoc.inl:254, the 1-based play order of a course hole, 19 when it is
// not played.
GOLFRULE_SHAREDDOC_INLINE unsigned char GetHoleIndex(unsigned char hole) {
	for (unsigned char i = 0; i < 18; i++) {
		if (WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] == hole)
			return i + 1;
	}

	return 19;
}

// shareddoc.inl:351, the result line of the player whose UID the team's
// trophy goes to in a guild match.
GOLFRULE_SHAREDDOC_INLINE sRivalData *GUILDMATCHUP(unsigned char team) {
	return &WSingleton<CSharedDoc>::Instance()->m_gameResultList[WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_trophyUID[team])];
}

// ClientSetting.cpp defines CLIENTSETTING_SHAREDDOC_INLINE empty before its
// first include, so its object emits the out-of-line copies of SetHoleType,
// SetShotTimeLimit, SetGameTimeLimit and MyId, as the original's did.
#ifndef CLIENTSETTING_SHAREDDOC_INLINE
#define CLIENTSETTING_SHAREDDOC_INLINE inline
#endif

// shareddoc.inl:244
CLIENTSETTING_SHAREDDOC_INLINE void SetHoleType(unsigned char type) {
	WSingleton<CSharedDoc>::Instance()->m_gameKind = type;
}

// shareddoc.inl:318
CLIENTSETTING_SHAREDDOC_INLINE void SetShotTimeLimit(unsigned long limit) {
	WSingleton<CSharedDoc>::Instance()->m_shotTimeLimit = limit;
}

// shareddoc.inl:426
CLIENTSETTING_SHAREDDOC_INLINE void SetGameTimeLimit(unsigned long limit) {
	WSingleton<CSharedDoc>::Instance()->m_gameTimeLimit = limit;
}

// shareddoc.inl:487
CLIENTSETTING_SHAREDDOC_INLINE char *MyId(void) {
	return WSingleton<CSharedDoc>::Instance()->m_userInfo.id;
}

// shareddoc.inl:519 (OnelineReqDlg emitted the copy the linker kept)
inline __int64 MyBonusCash(void) {
	return WSingleton<CSharedDoc>::Instance()->m_myBonusCash;
}

// shareddoc.inl:527 (TaskMain emitted the copy the linker kept). Callers test
// its int result, not the identity bit: `if (GalleryMode())` compiles to
// mov/shr/test, `!GalleryMode()` to movzx/and 1/sete.
TASKMAIN_SHAREDDOC_INLINE int GalleryMode(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_userInfo.IsIdentity(2))
		return 1;

	return 0;
}

#endif // PG_CLIENT_PROJECTG_SHAREDDOC_H

// shareddoc.inl's inlines that need CGolfDoc complete: defined once both
// headers are in (GolfDoc.h includes this header again at its end), without
// SharedDoc.h pulling GolfDoc.h (and its dynamic initializer) into every
// includer. SharedDoc.cpp defines SHAREDDOC_INLINE empty before its first
// include, so its object emits the out-of-line copy of PLAYER (0x4137a0), as
// the original's did.
#if defined(PG_CLIENT_PROJECTG_GOLFDOC_H) && !defined(PG_CLIENT_PROJECTG_SHAREDDOC_GOLFDOC_INLINES)
#define PG_CLIENT_PROJECTG_SHAREDDOC_GOLFDOC_INLINES

#ifndef SHAREDDOC_INLINE
#define SHAREDDOC_INLINE inline
#endif

// shareddoc.inl:340
SHAREDDOC_INLINE sPlayerData *PLAYER(unsigned char index) {
	CGolfDoc *golfDoc = WSingleton<CSharedDoc>::Instance()->m_pGolfDoc;
	return golfDoc->GetPlayer(index);
}

// shareddoc.inl:345 (GolfRule emitted the copy the linker kept)
inline sTeamData *TEAM(unsigned char index) {
	return GOLFDOC()->GetTeam(index);
}

#endif
