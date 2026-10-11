// Original: d:\build\custom\temp\code\shared\globalgamedefine.h
#ifndef PG_SHARED_GLOBALGAMEDEFINE_H
#define PG_SHARED_GLOBALGAMEDEFINE_H

#include <windows.h>

#include "..\..\defines.h"
#include "..\..\padding.h"

#include <list>

#include <string.h>

// Function object for std::for_each over containers of owned pointers.
struct Delete_Object {
	// globalgamedefine.h:307
	template <class T> void operator()(T ptr) const {
		delete ptr;
	}
};

// Function object for std::for_each over maps of owned pointers: deletes
// each element's second and clears it.
struct Delete_SecondObject {
	// globalgamedefine.h:326
	template <class T> void operator()(T& p) const {
		delete p.second;
		p.second = 0;
	}
};

// Level-up notification handed to the level-up reward dialog
// (FrLevelupItemForm::SetData); 3 bytes, cleared by its constructor.
struct sLevelUpDone {
	sLevelUpDone(void) {   // globalgamedefine.h:1749
		m_hasReward = 0;
		m_level = 0;
		m_rewardReceived = 0;
	}

	unsigned char m_hasReward;        // 0x0
	unsigned char m_level;            // 0x1
	unsigned char m_rewardReceived;   // 0x2
};

CHECK_SIZE(sLevelUpDone, 0x3);
CHECK_FIELD(sLevelUpDone, m_hasReward, 0x0);
CHECK_FIELD(sLevelUpDone, m_level, 0x1);
CHECK_FIELD(sLevelUpDone, m_rewardReceived, 0x2);

// Where the player is, sent to the messenger server when a channel is
// entered (0x4B bytes, byte-packed). Constructor out of line
// (globalgamedefine.h:1799, emitted by the SharedDoc unit).
#pragma pack(push, 1)
struct sUserPosition {
	// Keep the constructor out-of-line, matching the original ABI and ensuring
	// there is one stable definition for every translation unit that uses it.
	sUserPosition(void);

	unsigned short m_roomNumber;      // 0x00 (0xFFFF: none)
	unsigned long m_value2;           // 0x02 (-1)
	unsigned long m_userValue;        // 0x06 (-1; ConnectToServer's id, CSharedDoc::m_value84)
	unsigned char m_channelId;        // 0x0A (0xFF)
	char m_channelName[0x40];         // 0x0B
};
#pragma pack(pop)
CHECK_SIZE(sUserPosition, 0x4B);
CHECK_FIELD(sUserPosition, m_roomNumber, 0x00);
CHECK_FIELD(sUserPosition, m_value2, 0x02);
CHECK_FIELD(sUserPosition, m_userValue, 0x06);
CHECK_FIELD(sUserPosition, m_channelId, 0x0A);
CHECK_FIELD(sUserPosition, m_channelName, 0x0B);

// A game server channel, as listed in the server dialog (0x4D bytes,
// byte-packed). Looked up by id (operator==, globalgamedefine.h:3377).
#pragma pack(push, 1)
struct sChannelInfo {
	bool operator==(const unsigned char &id) const;

	char name[0x40];              // 0x00
	unsigned short maxUsers;      // 0x40
	unsigned short curUsers;      // 0x42
	unsigned char id;             // 0x44
	unsigned long flags;          // 0x45, 0x80: tournament rooms (game type 3), 0x4000: game type 7 (FrMakeRoomDlg)
	PADDING(0x49, 0x4D);
};
#pragma pack(pop)
CHECK_SIZE(sChannelInfo, 0x4D);
CHECK_FIELD(sChannelInfo, flags, 0x45);
CHECK_FIELD(sChannelInfo, name, 0x00);
CHECK_FIELD(sChannelInfo, maxUsers, 0x40);
CHECK_FIELD(sChannelInfo, curUsers, 0x42);
CHECK_FIELD(sChannelInfo, id, 0x44);

// An item of the player's inventory (0xA8 bytes; constructor
// globalgamedefine.h:1950). Only the members the transcribed functions read
// are named.
struct sItemInfo {
	sItemInfo(void);
	bool operator==(const unsigned long &typeId) const; // 0x43fbe0 (emitted in GolfRule)

	unsigned long m_serial;       // 0x00
	unsigned long m_typeId;       // 0x04
	unsigned long m_reserved08;   // 0x08, cleared by the constructor, otherwise unused so far
	union {
		short m_count;            // 0x0C, how many of a stackable item the player holds (TransferableItem)
		short m_clubStatLevel[5]; // 0x0C, the club's upgrade level of each stat (FrDowngradeDlg)
	};
	unsigned char m_reserved16;   // 0x16, cleared by the constructor, otherwise unused so far
	struct {
		unsigned char flag0 : 1;      // bit 0: cleared by the constructor
		unsigned char expired : 1;    // bit 1: the item's period is over
		unsigned char flags23 : 2;    // bits 2-3: cleared by the constructor
		unsigned char kind : 3;       // bits 4-6: 6 = rental (S5::RENTALPARTS::CollectRentalItem)
		unsigned char flag7 : 1;      // bit 7: kept by the constructor
	} m_stateBits;                // 0x17
	unsigned long m_reserved18[4]; // 0x18, cleared by the constructor, otherwise unused so far
	struct {
		unsigned char flags01 : 2;    // bits 0-1: cleared by the constructor
		unsigned char flags27 : 6;    // bits 2-7: kept by the constructor
	} m_flags28;                  // 0x28
	char m_uccName[0x29];         // 0x29, the UCC's name (CTask::OnPacketCommon 0x71 copies it into sUccClothes)
	char m_uccKey[9];             // 0x52
	unsigned char m_uccStatus;    // 0x5B, the UCC (self-designed item) state (TransferableItem)
	unsigned short m_uccSeq;      // 0x5C, the UCC's sequence number (TransferableItem)
	char m_reserved5E[0x16];      // 0x5E, cleared by the constructor, otherwise unused so far
	int m_cardSlots[12];          // 0x74, the type ids of the cards slotted into the item
	short m_charCardSlotCount;    // 0xA4, the character-card slots the part opens (sPart 0x1F8; FrCardRemoveDlg)
	short m_caddieCardSlotCount;  // 0xA6, the caddie-card slots the part opens (sPart 0x1FA; FrCardRemoveDlg)
};
CHECK_SIZE(sItemInfo, 0xA8);
CHECK_FIELD(sItemInfo, m_reserved08, 0x08);
CHECK_FIELD(sItemInfo, m_reserved18, 0x18);
CHECK_FIELD(sItemInfo, m_flags28, 0x28);
CHECK_FIELD(sItemInfo, m_uccName, 0x29);
CHECK_FIELD(sItemInfo, m_reserved5E, 0x5E);
CHECK_FIELD(sItemInfo, m_serial, 0x00);
CHECK_FIELD(sItemInfo, m_typeId, 0x04);
CHECK_FIELD(sItemInfo, m_count, 0x0C);
CHECK_FIELD(sItemInfo, m_clubStatLevel, 0x0C);
CHECK_FIELD(sItemInfo, m_stateBits, 0x17);
CHECK_FIELD(sItemInfo, m_uccKey, 0x52);
CHECK_FIELD(sItemInfo, m_uccStatus, 0x5B);
CHECK_FIELD(sItemInfo, m_uccSeq, 0x5C);
CHECK_FIELD(sItemInfo, m_cardSlots, 0x74);
CHECK_FIELD(sItemInfo, m_charCardSlotCount, 0xA4);
CHECK_FIELD(sItemInfo, m_caddieCardSlotCount, 0xA6);

// globalgamedefine.h:1950
inline sItemInfo::sItemInfo(void) {
	m_stateBits.flag0 = 0;
	m_stateBits.expired = 0;
	m_stateBits.flags23 = 0;
	m_stateBits.kind = 0;
	m_flags28.flags01 = 0;
	m_serial = 0;
	m_typeId = 0;
	m_reserved08 = 0;
	m_reserved16 = 0;
	for (int i = 0; i < 5; i++) {
		m_clubStatLevel[i] = 0;
	}
	for (int i = 0; i < 4; i++) {
		m_reserved18[i] = 0;
	}
	memset(m_uccName, 0, sizeof(m_uccName));
	memset(m_uccKey, 0, sizeof(m_uccKey));
	memset(m_reserved5E, 0, sizeof(m_reserved5E));
	for (int i = 0; i < 12; i++) {
		m_cardSlots[i] = 0;
	}
	m_charCardSlotCount = 0;
	m_caddieCardSlotCount = 0;
	m_uccStatus = 0;
	m_uccSeq = 0;
}

// A caddie the player owns (CSharedDoc keeps them in a std::map keyed by
// id); 0x19 bytes, byte-packed (FrCaddieInfoDlg::GetCaddieInfo copies it with
// `rep movsd` x6 + `movsb`). Only the fields the transcribed functions use
// are named.
#pragma pack(push, 1)
struct sCaddieInfo {
	unsigned int id;            // 0x0 (key of CSharedDoc's caddie map)
	unsigned int typeId;        // 0x4 (sCaddie typeid, IFF_STRUCT)
	unsigned long cadItemTypeId; // 0x8 (the sCadItem typeid of the caddie's item)
	unsigned char level;        // 0xC
	unsigned long experience;   // 0xD
	unsigned char flags;        // 0x11 (bit 2: on vacation)
	unsigned short days;        // 0x12 (contract days left; EndContract sets 30)
	unsigned short hours;       // 0x14 (the caddie item's time left, in hours)
	UNKNOWN(0x16, 0x17);
	unsigned char contract;     // 0x17 (1: the sign-up button is disabled)
	unsigned char checkEquip;   // 0x18, nonzero: CTask::OnPacketCommon 0x6F re-checks the equipped caddie
};
#pragma pack(pop)
CHECK_SIZE(sCaddieInfo, 0x19);
CHECK_FIELD(sCaddieInfo, id, 0x0);
CHECK_FIELD(sCaddieInfo, typeId, 0x4);
CHECK_FIELD(sCaddieInfo, cadItemTypeId, 0x8);
CHECK_FIELD(sCaddieInfo, hours, 0x14);
CHECK_FIELD(sCaddieInfo, level, 0xC);
CHECK_FIELD(sCaddieInfo, experience, 0xD);
CHECK_FIELD(sCaddieInfo, flags, 0x11);
CHECK_FIELD(sCaddieInfo, days, 0x12);
CHECK_FIELD(sCaddieInfo, contract, 0x17);
CHECK_FIELD(sCaddieInfo, checkEquip, 0x18);

#pragma pack(push, 1)
// A guild member's view of his guild (0x119 bytes, packed: the copy in
// CGuildInfo::SetMyGuildUserInfo moves 0x46 dwords + 1 byte).
struct GUILD_USER_INFO {
	// globalgamedefine.h:137
	GUILD_USER_INFO(void) {
		guildNumber = 0;
		value19 = 0;
		value1D = 0;
		value21 = 0;
		classIndex = 0;
		valueFF = -1;
		memset(name, 0, sizeof(name));
		memset(emblem, 0, sizeof(emblem));
		memset(notice, 0, sizeof(notice));
		memset(introduce, 0, sizeof(introduce));
		memset(text103, 0, sizeof(text103));
	}

	int guildNumber;            // 0x0
	char name[0x15];            // 0x4
	int value19;                // 0x19
	int value1D;                // 0x1D
	int value21;                // 0x21
	char emblem[0xC];           // 0x25
	char notice[0x65];          // 0x31: SetGuildNotice
	char introduce[0x65];       // 0x96: SetGuildIntroduce
	int classIndex;             // 0xFB: a GUILD_CLASS_IDX
	int valueFF;                // 0xFF: -1 by default
	char text103[0x16];         // 0x103
};

// The player's guild (0x129 bytes, packed: CGuildInfo::SetMyGuildInfo moves
// 0x4A dwords + 1 byte).
struct GUILD_INFO : GUILD_USER_INFO {
	// globalgamedefine.h:151
	GUILD_INFO(void) {
	}

	char data119[0x10];         // 0x119: 16 bytes CGuildInfo::ClearAllData zeroes (meaning not yet known)
};
#pragma pack(pop)

BEGIN_LAYOUT_CHECK(GUILD_USER_INFO)
	CHECK_SIZE(GUILD_USER_INFO, 0x119);
	CHECK_FIELD(GUILD_USER_INFO, name, 0x4);
	CHECK_FIELD(GUILD_USER_INFO, emblem, 0x25);
	CHECK_FIELD(GUILD_USER_INFO, notice, 0x31);
	CHECK_FIELD(GUILD_USER_INFO, introduce, 0x96);
	CHECK_FIELD(GUILD_USER_INFO, classIndex, 0xFB);
	CHECK_FIELD(GUILD_USER_INFO, valueFF, 0xFF);
	CHECK_FIELD(GUILD_USER_INFO, text103, 0x103);
END_LAYOUT_CHECK
CHECK_SIZE(GUILD_INFO, 0x129);
CHECK_FIELD(GUILD_INFO, data119, 0x119);

// A guild member as the messenger server lists him (0x37 bytes, packed: CTask
// decodes 0x37 bytes per member in packets 0x38 and 0x39 and copies it into an
// sFriend).
#pragma pack(push, 1)
struct GUILD_USER_MSN_LIST {
	// globalgamedefine.h:230
	GUILD_USER_MSN_LIST(void) : uid(0xFFFFFFFF), value4(0), sex(0), value35(0), online(0) {
		// globalgamedefine.h:231
		memset(nick, 0, sizeof(nick));
		// globalgamedefine.h:232
		memset(name9, 0, sizeof(name9));
	}

	unsigned long uid;      // 0x0, sFriend::uid
	unsigned long value4;   // 0x4, copied to sFriend::value2D (meaning not known)
	unsigned char sex;      // 0x8, bit 0 copied to sFriend::flags bit 0 (female icon)
	char name9[0x16];       // 0x9, cleared (meaning not known)
	char nick[0x16];        // 0x1F, sFriend::nick
	unsigned char value35;  // 0x35, cleared (meaning not known)
	unsigned char online;   // 0x36, bit 0 copied to sFriend::flags bit 1 (logged on)
};
#pragma pack(pop)
CHECK_SIZE(GUILD_USER_MSN_LIST, 0x37);
CHECK_FIELD(GUILD_USER_MSN_LIST, value4, 0x4);
CHECK_FIELD(GUILD_USER_MSN_LIST, sex, 0x8);
CHECK_FIELD(GUILD_USER_MSN_LIST, name9, 0x9);
CHECK_FIELD(GUILD_USER_MSN_LIST, nick, 0x1F);
CHECK_FIELD(GUILD_USER_MSN_LIST, value35, 0x35);
CHECK_FIELD(GUILD_USER_MSN_LIST, online, 0x36);

// A character the player owns (CSharedDoc keeps them in a
// std::map<unsigned int, sCharacterInfo>; 0x1BC bytes). No constructor: the
// map's operator[] value-initialises it member by member (0x5a7980), which
// gives the member boundaries: the 4-byte store at 0x6A makes the struct
// byte-packed. The members at 0x6A, 0x70 and 0x84 are structures of their
// own: CFamilyMain::cFamily's constructor zeroes each through its own address
// (lea + stores), and without a class-type member cl zeroes the whole
// structure with one rep stosd. CPartTidList::SetTids reads the character,
// colours and parts.
#pragma pack(push, 1)
struct sCharacterInfo {
	unsigned long typeId;               // 0x00, the sChar typeid
	unsigned long id;                   // 0x04
	unsigned long partTypeIds[24];      // 0x08, the sPart typeid worn in each slot
	unsigned char hairColor;            // 0x68
	unsigned char shirtColor;           // 0x69, low nibble: the shirt colour
	struct {
		unsigned long value;            // +0x0
		unsigned char flag;             // +0x4
	} unknown6A;                        // 0x6A, 5 bytes
	unsigned char unknown6F;            // 0x6F
	struct {
		unsigned long tids[5];          // +0x0
	} auxParts;                         // 0x70, five ulong slots (the aux parts, by their count; meaning unconfirmed)
	struct {
		UNKNOWN(0x00, 0xD8);
	} unknown84;                        // 0x84, 0x36 dwords zeroed as one block
	unsigned long partIds[24];          // 0x15C, the item id worn in each slot
};
#pragma pack(pop)
CHECK_SIZE(sCharacterInfo, 0x1BC);
CHECK_FIELD(sCharacterInfo, typeId, 0x00);
CHECK_FIELD(sCharacterInfo, id, 0x04);
CHECK_FIELD(sCharacterInfo, partTypeIds, 0x08);
CHECK_FIELD(sCharacterInfo, hairColor, 0x68);
CHECK_FIELD(sCharacterInfo, shirtColor, 0x69);
CHECK_FIELD(sCharacterInfo, unknown6A, 0x6A);
CHECK_FIELD(sCharacterInfo, unknown6F, 0x6F);
CHECK_FIELD(sCharacterInfo, auxParts, 0x70);
CHECK_FIELD(sCharacterInfo, unknown84, 0x84);
CHECK_FIELD(sCharacterInfo, partIds, 0x15C);

// The items a player has equipped (0x6C bytes, byte-packed:
// CSharedDoc::SetCurEquipInfo copies it with 0x1B dwords; CFamilyMain::cFamily
// keeps one at 0x42D).
#pragma pack(push, 1)
struct sUserEquip {
	unsigned long caddieId;             // 0x00, 0 or -1: none (SetCurEquipInfo looks it up in the caddie map)
	UNKNOWN(0x04, 0x10);
	unsigned long itemSlots[10];        // 0x10, the equipped item slots (FrEquipDlg::GetMyItemSlot)
	UNKNOWN(0x38, 0x68);
	unsigned long mascotId;             // 0x68, 0 or -1: none (SetCurEquipInfo looks it up in the mascot map)
};
#pragma pack(pop)
CHECK_SIZE(sUserEquip, 0x6C);
CHECK_FIELD(sUserEquip, caddieId, 0x00);
CHECK_FIELD(sUserEquip, itemSlots, 0x10);
CHECK_FIELD(sUserEquip, mascotId, 0x68);

// One game of a player's match history (0x34 bytes; constructor
// globalgamedefine.h:1731). CMatchHistory keeps five (0x41 dwords).
// FrUserInfoForm::MatchHistoryUserBtnUp sends uid and opens the nickname.
struct sUserMatchHistory {
	// globalgamedefine.h:1731
	sUserMatchHistory(void) : sex(0), uid(0) {
		memset(nickname, 0, sizeof(nickname));
		memset(id, 0, sizeof(id));
	}

	unsigned long sex;            // 0x00 (name from the record's packet layout; no transcribed reader)
	char nickname[0x16];          // 0x04, given to CUserInfo::m_nick
	char id[0x16];                // 0x1A, the account id (same pair of arrays as sPangYaUserInfo)
	unsigned long uid;            // 0x30, sent by MatchHistoryUserBtnUp
};
CHECK_SIZE(sUserMatchHistory, 0x34);
CHECK_FIELD(sUserMatchHistory, sex, 0x00);
CHECK_FIELD(sUserMatchHistory, nickname, 0x04);
CHECK_FIELD(sUserMatchHistory, id, 0x1A);
CHECK_FIELD(sUserMatchHistory, uid, 0x30);

// A player's game statistics (0xEB bytes, byte-packed). sUserInfo holds one
// at 0x10F and CSharedDoc the local player's at 0x4EF (flattened there as
// m_myStat*, whose names these follow). IsNothingRecord/IsHideDisconnectRate
// read the level and the games played.
#pragma pack(push, 1)
struct sPangYaUserStatistics {
	unsigned long shots;            // 0x00
	unsigned long puttAttempts;     // 0x04
	UNKNOWN(0x08, 0x14);
	unsigned long pangyas;          // 0x14
	UNKNOWN(0x18, 0x24);
	unsigned long drives;           // 0x24
	unsigned long par3Drives;       // 0x28
	unsigned short holeInOnes;      // 0x2C
	UNKNOWN(0x2E, 0x30);
	unsigned long fairways;         // 0x30
	UNKNOWN(0x34, 0x3A);
	unsigned long puttsSunk;        // 0x3A
	UNKNOWN(0x3E, 0x46);
	unsigned long exp;              // 0x46
	unsigned char level;            // 0x4A
	__int64 pang;                   // 0x4B
	UNKNOWN(0x53, 0x57);
	signed char bestScore[6];       // 0x57, per course; 0x7F: none (FrUserInfoForm::GetBestScoreColor)
	__int64 maxPang[6];             // 0x5D, per course (FrUserInfoForm::GetMaxPangColor)
	unsigned long gamesPlayed;      // 0x8D
	UNKNOWN(0x91, 0xB9);
	unsigned long noMannerGames;    // 0xB9
	UNKNOWN(0xBD, 0xEB);
};
#pragma pack(pop)
CHECK_SIZE(sPangYaUserStatistics, 0xEB);
CHECK_FIELD(sPangYaUserStatistics, shots, 0x00);
CHECK_FIELD(sPangYaUserStatistics, puttAttempts, 0x04);
CHECK_FIELD(sPangYaUserStatistics, pangyas, 0x14);
CHECK_FIELD(sPangYaUserStatistics, drives, 0x24);
CHECK_FIELD(sPangYaUserStatistics, par3Drives, 0x28);
CHECK_FIELD(sPangYaUserStatistics, holeInOnes, 0x2C);
CHECK_FIELD(sPangYaUserStatistics, fairways, 0x30);
CHECK_FIELD(sPangYaUserStatistics, puttsSunk, 0x3A);
CHECK_FIELD(sPangYaUserStatistics, exp, 0x46);
CHECK_FIELD(sPangYaUserStatistics, level, 0x4A);
CHECK_FIELD(sPangYaUserStatistics, pang, 0x4B);
CHECK_FIELD(sPangYaUserStatistics, bestScore, 0x57);
CHECK_FIELD(sPangYaUserStatistics, maxPang, 0x5D);
CHECK_FIELD(sPangYaUserStatistics, gamesPlayed, 0x8D);
CHECK_FIELD(sPangYaUserStatistics, noMannerGames, 0xB9);

// An element of a player's special trophy list (0xC bytes: the list node
// copies three dwords). No transcribed code reads its members.
struct sSpecialTrophy {
	UNKNOWN(0x0, 0xC);
};
CHECK_SIZE(sSpecialTrophy, 0xC);

// An element of a player's guild trophy list (4 bytes: the list node copies
// one dword). No transcribed code reads it.
struct sGuildTrophy {
	UNKNOWN(0x0, 0x4);
};
CHECK_SIZE(sGuildTrophy, 0x4);

// A lobby player as the user list holds it (0xC4 bytes, byte-packed: the
// map node of CSharedDoc's user map copies 0x32 dwords of key and value).
#pragma pack(push, 1)
struct sBriefUserInfo {
	// globalgamedefine.h:1786
	bool IsIdentity(unsigned long identityFlag) {
		return (this->identity & identityFlag) ? true : false;
	}

	unsigned long id;             // 0x00, FrUserListDlg::m_selectedID
	unsigned long uid;            // 0x04, FrUserListDlg::m_selectedUID
	unsigned short roomNumber;    // 0x08, 0xFFFF: not in a room (invite enabled)
	char nick[0x16];              // 0x0A
	unsigned char level;          // 0x20
	union {
		unsigned long identity;   // 0x21, flags (IsIdentity; 2: gallery, 4: administrator)
		struct {
			unsigned long : 1;
			unsigned long gallery : 1;        // bit 1
			unsigned long administrator : 1;  // bit 2 (ntAvatarManager::AvatarByRayIntersect)
			unsigned long : 29;
		} identityBits;           // 0x21, the same flags as bits
	};
	unsigned long titleTypeId;    // 0x25, the equipped title's type id (CTask::OnPacketCommon 0x44 copies it to sRoomPlayer)
	unsigned long rankValue;      // 0x29, sorted instead of the level in channels with flag 0x80
	unsigned char unknownBit : 1; // 0x2D, bit 0
	unsigned char charType : 3;   // 0x2D, bits 1-3; even male, odd female (the sex sort)
	unsigned long guildUID;       // 0x2E, the guild sort key
	char guildMark[0xC];          // 0x32, the guild emblem (CGuildActor::ChangeGuildEmblem)
	unsigned char invisible : 1;  // 0x3E, bit 0: an administrator hidden from picking (AvatarByRayIntersect skips them)
	unsigned char lobbyFlags;     // 0x3F, bit 2: a hidden administrator stays in the lobby's user map (CTask::OnPacketCommon 0x44)
	PADDING(0x40, 0xC4);
};
#pragma pack(pop)
CHECK_SIZE(sBriefUserInfo, 0xC4);
CHECK_FIELD(sBriefUserInfo, id, 0x00);
CHECK_FIELD(sBriefUserInfo, uid, 0x04);
CHECK_FIELD(sBriefUserInfo, roomNumber, 0x08);
CHECK_FIELD(sBriefUserInfo, nick, 0x0A);
CHECK_FIELD(sBriefUserInfo, guildMark, 0x32);
CHECK_FIELD(sBriefUserInfo, level, 0x20);
CHECK_FIELD(sBriefUserInfo, identity, 0x21);
CHECK_FIELD(sBriefUserInfo, identityBits, 0x21);
CHECK_FIELD(sBriefUserInfo, rankValue, 0x29);
CHECK_FIELD(sBriefUserInfo, guildUID, 0x2E);
CHECK_FIELD(sBriefUserInfo, titleTypeId, 0x25);
CHECK_FIELD(sBriefUserInfo, lobbyFlags, 0x3F);

// An item attached to a mail (0x35 bytes, byte-packed: the list node's
// copy moves 0xD dwords + 1 byte; constructor globalgamedefine.h:4416).
// Filled from a sTradeItem by CPost_SendDlg::GetSendList.
#pragma pack(push, 1)
struct sMailIncludeItem {
	unsigned long guid;           // 0x00
	unsigned long typeId;         // 0x04
	unsigned char kind;           // 0x08
	unsigned long count;          // 0x09
	unsigned long flag;           // 0x0D, sTradeItem::flag
	UNKNOWN(0x11, 0x27);
	char uccKey[9];               // 0x27, the UCC's key (RequestUccInfo)
	UNKNOWN(0x30, 0x35);
};
#pragma pack(pop)
CHECK_SIZE(sMailIncludeItem, 0x35);
CHECK_FIELD(sMailIncludeItem, guid, 0x00);
CHECK_FIELD(sMailIncludeItem, typeId, 0x04);
CHECK_FIELD(sMailIncludeItem, kind, 0x08);
CHECK_FIELD(sMailIncludeItem, count, 0x09);
CHECK_FIELD(sMailIncludeItem, flag, 0x0D);
CHECK_FIELD(sMailIncludeItem, uccKey, 0x27);

// One line of the mailbox list (0xC4 bytes, byte-packed: an int sits at
// 0x89; the std::list node holds 0xCC).
#pragma pack(push, 1)
struct sMailInfoBrief {
	unsigned long m_mailIndex;   // 0x00, the letter's index sent to the server (delete_letter_check)
	UNKNOWN(0x04, 0x88);
	bool m_read;                 // 0x88, set once the letter is opened (CPost_ReciveDlg::OnPostMessageResult)
	int m_hasItem;               // 0x89, nonzero: items are attached (delete-all skips the letter; name inferred)
	UNKNOWN(0x8D, 0xC4);
};
#pragma pack(pop)
CHECK_SIZE(sMailInfoBrief, 0xC4);
CHECK_FIELD(sMailInfoBrief, m_mailIndex, 0x00);
CHECK_FIELD(sMailInfoBrief, m_read, 0x88);
CHECK_FIELD(sMailInfoBrief, m_hasItem, 0x89);

// One page of the mailbox (CTransferableItem::SetMailInfoPage). The three
// counters are copied as they are; their names are inferred.
struct sMailBoxListForPage {
	std::list<sMailInfoBrief> mailList;   // 0x00
	int page;                             // 0x0C
	int totalPage;                        // 0x10
	int totalMail;                        // 0x14
};
CHECK_SIZE(sMailBoxListForPage, 0x18);
CHECK_FIELD(sMailBoxListForPage, mailList, 0x00);
CHECK_FIELD(sMailBoxListForPage, page, 0x0C);
CHECK_FIELD(sMailBoxListForPage, totalPage, 0x10);
CHECK_FIELD(sMailBoxListForPage, totalMail, 0x14);

// A mail opened from the mailbox (0x368 bytes, byte-packed: the item list
// sits at 0x35B). The constructor (globalgamedefine.h:4443) is emitted by
// realmyroomtask.cpp; the member names are inferred from the sizes.
#pragma pack(push, 1)
struct sMailInfo {
	sMailInfo(void);

	int mailIndex;                          // 0x000, -1 when empty
	char senderNick[0x16];                  // 0x004
	char date[0x20];                        // 0x01A
	char message[0x320];                    // 0x03A
	unsigned char isRead;                   // 0x35A
	std::list<sMailIncludeItem> itemList;   // 0x35B
	unsigned char state;                    // 0x367, not cleared by the constructor
};
#pragma pack(pop)
CHECK_SIZE(sMailInfo, 0x368);
CHECK_FIELD(sMailInfo, mailIndex, 0x000);
CHECK_FIELD(sMailInfo, senderNick, 0x004);
CHECK_FIELD(sMailInfo, date, 0x01A);
CHECK_FIELD(sMailInfo, message, 0x03A);
CHECK_FIELD(sMailInfo, isRead, 0x35A);
CHECK_FIELD(sMailInfo, itemList, 0x35B);
CHECK_FIELD(sMailInfo, state, 0x367);

// A player's public profile, byte-packed (CSharedDoc keeps the local player's
// at 0x3E2, GetPangyaUserInfo). Only the members the transcribed functions
// read are named; the size is not known yet.
#pragma pack(push, 1)
struct sPangYaUserInfo {
	// globalgamedefine.h:1354 (inline; emitted by taskmain.cpp)
	bool IsIdentity(unsigned long identityFlag) {
		return (this->identity & identityFlag) ? true : false;
	}

	char id[0x16];                // 0x00
	char nickname[0x16];          // 0x16
	char guildName[0x15];         // 0x2C
	char guildMark[0xC];          // 0x41
	UNKNOWN(0x4D, 0x51);
	union {
		int identity;             // 0x51, flags (2: gallery, 4: administrator)
		struct {
			unsigned long : 1;
			unsigned long gallery : 1;        // bit 1 (CClock::HandleMsg)
			unsigned long administrator : 1;  // bit 2 (CGMToolkit::IsAdministrator)
			unsigned long : 29;
		} identityBits;           // 0x51, the same flags as bits
	};
	unsigned long galleryGuid;    // 0x55, MyGuid in gallery mode
	unsigned long guid;           // 0x59, MyGuid
	UNKNOWN(0x5D, 0x69);
	unsigned long guildUID;       // 0x69
	unsigned long guildPosition;  // 0x6D
	unsigned char eventFlags;     // 0x71, bit 1 (mask 2): the recycle event is on (FrRecycleDlg::SetEventMessage)
	UNKNOWN(0x72, 0x73);
	short bsUsableTimes;          // 0x73, Bongdari shop: plays usable (CSharedDoc::GetBsUsableTimes)
	short bsUsableBonusTimes;     // 0x75, Bongdari shop: bonus plays usable (CSharedDoc::GetBsUsableBonusTimes)
	short bsRemainedBonusTimes;   // 0x77, Bongdari shop: bonus plays remaining (CSharedDoc::GetBsRemainedBonusTimes)
	unsigned long pointEventPoints; // 0x79, the point event's points (FrPointEventDlg::OnProc)
	unsigned long flagBlock;      // 0x7D
	int timeBlock;                // 0x81, CSharedDoc::SetTimeBlock
	int nameByEdit;               // 0x85, 1: mail is addressed by the name typed in the receiver edit
};
#pragma pack(pop)
CHECK_FIELD(sPangYaUserInfo, id, 0x00);
CHECK_FIELD(sPangYaUserInfo, nickname, 0x16);
CHECK_FIELD(sPangYaUserInfo, guildName, 0x2C);
CHECK_FIELD(sPangYaUserInfo, guildMark, 0x41);
CHECK_FIELD(sPangYaUserInfo, identity, 0x51);
CHECK_FIELD(sPangYaUserInfo, identityBits, 0x51);
CHECK_FIELD(sPangYaUserInfo, galleryGuid, 0x55);
CHECK_FIELD(sPangYaUserInfo, guid, 0x59);
CHECK_FIELD(sPangYaUserInfo, guildUID, 0x69);
CHECK_FIELD(sPangYaUserInfo, guildPosition, 0x6D);
CHECK_FIELD(sPangYaUserInfo, eventFlags, 0x71);
CHECK_FIELD(sPangYaUserInfo, bsUsableTimes, 0x73);
CHECK_FIELD(sPangYaUserInfo, bsUsableBonusTimes, 0x75);
CHECK_FIELD(sPangYaUserInfo, bsRemainedBonusTimes, 0x77);
CHECK_FIELD(sPangYaUserInfo, pointEventPoints, 0x79);
CHECK_FIELD(sPangYaUserInfo, flagBlock, 0x7D);
CHECK_FIELD(sPangYaUserInfo, timeBlock, 0x81);
CHECK_FIELD(sPangYaUserInfo, nameByEdit, 0x85);

// A course of the game (the map id the server sends).
enum eMapType {
	MAP_BLUE_LAGOON = 0,
	MAP_BLUE_WATER = 1,
	MAP_SEPIA_WIND = 2,
	MAP_WIND_HILL = 3,
	MAP_WIZ_WIZ = 4,
	MAP_WEST_WIZ = 5,
	MAP_BLUE_MOON = 6,
	MAP_SILVIA_CANNON = 7,
	MAP_ICE_CANNON = 8,
	MAP_WHITE_WIZ = 9,
	MAP_SHINING_SAND = 10,
	MAP_PINK_WIND = 11,
	MAP_DEEP_INFERNO = 13,
	MAP_ICE_SPA = 14,
	MAP_LOST_SEAWAY = 15,
	MAP_EASTERN_VALLEY = 16,
	MAP_ICE_INFERNO = 18,
	MAP_WIZ_CITY = 19
};

// Conversions between a course id, its slot in per-course tables and its bit
// in a course mask.
class CMapTypeAdapter {
public:
	enum eMapIndex {
		MAP_INDEX_BLUE_LAGOON = 0
	};

	enum eMapFlag {
		MAP_FLAG_NONE = 0,
		MAP_FLAG_BLUE_LAGOON = 0x1,
		MAP_FLAG_BLUE_WATER = 0x2,
		MAP_FLAG_SEPIA_WIND = 0x4,
		MAP_FLAG_WIND_HILL = 0x8,
		MAP_FLAG_WIZ_WIZ = 0x10,
		MAP_FLAG_WEST_WIZ = 0x20,
		MAP_FLAG_BLUE_MOON = 0x40,
		MAP_FLAG_SILVIA_CANNON = 0x80,
		MAP_FLAG_ICE_CANNON = 0x100,
		MAP_FLAG_WHITE_WIZ = 0x200,
		MAP_FLAG_SHINING_SAND = 0x400,
		MAP_FLAG_PINK_WIND = 0x800,
		MAP_FLAG_DEEP_INFERNO = 0x1000,
		MAP_FLAG_ICE_SPA = 0x2000,
		MAP_FLAG_LOST_SEAWAY = 0x4000,
		MAP_FLAG_EASTERN_VALLEY = 0x8000,
		MAP_FLAG_WIZ_CITY = 0x40000
	};

	// globalgamedefine.h:4574
	static eMapFlag GetMapFlag(eMapType type) {
		eMapFlag flag;

		switch (type) {
		case MAP_BLUE_LAGOON:
			return MAP_FLAG_BLUE_LAGOON;
		case MAP_BLUE_WATER:
			return MAP_FLAG_BLUE_WATER;
		case MAP_SEPIA_WIND:
			return MAP_FLAG_SEPIA_WIND;
		case MAP_WIND_HILL:
			return MAP_FLAG_WIND_HILL;
		case MAP_WIZ_WIZ:
			return MAP_FLAG_WIZ_WIZ;
		case MAP_WEST_WIZ:
			return MAP_FLAG_WEST_WIZ;
		case MAP_BLUE_MOON:
			return MAP_FLAG_BLUE_MOON;
		case MAP_SILVIA_CANNON:
			return MAP_FLAG_SILVIA_CANNON;
		case MAP_ICE_CANNON:
			return MAP_FLAG_ICE_CANNON;
		case MAP_WHITE_WIZ:
			return MAP_FLAG_WHITE_WIZ;
		case MAP_SHINING_SAND:
			return MAP_FLAG_SHINING_SAND;
		case MAP_PINK_WIND:
			return MAP_FLAG_PINK_WIND;
		case MAP_DEEP_INFERNO:
			return MAP_FLAG_DEEP_INFERNO;
		case MAP_ICE_SPA:
			return MAP_FLAG_ICE_SPA;
		case MAP_LOST_SEAWAY:
			return MAP_FLAG_LOST_SEAWAY;
		case MAP_EASTERN_VALLEY:
			return MAP_FLAG_EASTERN_VALLEY;
		case MAP_WIZ_CITY:
			flag = MAP_FLAG_WIZ_CITY;
			break;
		}

		return flag;
	}

	static eMapIndex GetMapIndex(eMapType type);   // globalgamedefine.h:4683
};

// Animation style of a cut-in overlay (values 3..7 seen: scaling 3/4,
// alpha 5/6, rotate 7; the others are not named yet).
enum eCUTINANISTLYE {
	CUTIN_ANI_STYLE_NONE = 0
};

// Animation parameters of one cut-in overlay, 0xB8 bytes. Only the members
// the decompiled code touches are transcribed.
struct CUTIN_ANI_STYPEINFO {
	// globalgamedefine.h:5453
	CUTIN_ANI_STYPEINFO(void) {
		Reset();
	}

	void Reset(void);

	int m_posType;              // 0x00, the overlay's position type (0..5)
	eCUTINANISTLYE m_style0;    // 0x04, the style of overlay 0 (CCutin::PrepareCutIn)
	eCUTINANISTLYE m_style1;    // 0x08
	eCUTINANISTLYE m_style2;    // 0x0C
	eCUTINANISTLYE m_style3;    // 0x10
	eCUTINANISTLYE m_style;     // 0x14
	UNKNOWN(0x18, 0xB8);
};

CHECK_SIZE(CUTIN_ANI_STYPEINFO, 0xB8);
CHECK_FIELD(CUTIN_ANI_STYPEINFO, m_posType, 0x00);
CHECK_FIELD(CUTIN_ANI_STYPEINFO, m_style0, 0x04);
CHECK_FIELD(CUTIN_ANI_STYPEINFO, m_style1, 0x08);
CHECK_FIELD(CUTIN_ANI_STYPEINFO, m_style2, 0x0C);
CHECK_FIELD(CUTIN_ANI_STYPEINFO, m_style3, 0x10);
CHECK_FIELD(CUTIN_ANI_STYPEINFO, m_style, 0x14);

// A period (time-limited) card in use: the special card buffs and the
// character / caddie cards with an ability period (CCardManager, CItemBuff,
// FrSPCardBuffDlg). 0x41 bytes, byte-packed: the list node copies 0x41
// bytes and sAbilityFinder::operator(), taking it by value, pops 0x44.
// Received whole by CItemBuff::ProcessPacket (DecodeBuffer, 0x41).
#pragma pack(push, 1)
struct sSCardAvilityPeriodInfo {
	unsigned long id;               // 0x00
	unsigned long typeId;           // 0x04, the card's type id
	unsigned long partsTypeId;      // 0x08, the character / caddie type id it is put on (0: none)
	unsigned long partsId;          // 0x0C, the character / caddie id
	int effect;                     // 0x10
	int effectValue;                // 0x14
	int slot;                       // 0x18
	SYSTEMTIME startTime;           // 0x1C
	SYSTEMTIME endTime;             // 0x2C
	int kind;                       // 0x3C, 0 character card, 1 caddie card, 2 special card
	unsigned char use;              // 0x40
};
#pragma pack(pop)
CHECK_SIZE(sSCardAvilityPeriodInfo, 0x41);
CHECK_FIELD(sSCardAvilityPeriodInfo, typeId, 0x04);
CHECK_FIELD(sSCardAvilityPeriodInfo, partsTypeId, 0x08);
CHECK_FIELD(sSCardAvilityPeriodInfo, partsId, 0x0C);
CHECK_FIELD(sSCardAvilityPeriodInfo, slot, 0x18);
CHECK_FIELD(sSCardAvilityPeriodInfo, startTime, 0x1C);
CHECK_FIELD(sSCardAvilityPeriodInfo, endTime, 0x2C);
CHECK_FIELD(sSCardAvilityPeriodInfo, kind, 0x3C);
CHECK_FIELD(sSCardAvilityPeriodInfo, use, 0x40);

#endif // PG_SHARED_GLOBALGAMEDEFINE_H
