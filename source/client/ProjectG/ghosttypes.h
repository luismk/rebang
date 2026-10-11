// Original: d:\build\custom\temp\code\client\projectg\ghosttypes.h
#ifndef PG_CLIENT_PROJECTG_GHOSTTYPES_H
#define PG_CLIENT_PROJECTG_GHOSTTYPES_H

#include <string.h>

#include "..\..\defines.h"
#include "..\..\padding.h"

namespace GHOST {

// The recorded actions (the index of the action in GHOST_SHOT_DATA's array).
// Only the names the ghost text dump prints are known.
enum eGHOSTACTMSG {
	CHANGE_WIND = 0,
	CHANGE_WEATHER = 1,
	CHANGE_DIRECTION = 2,
	CHANGE_CLUB = 4,
	USE_ITEM = 5,
	BALL_DROP = 7,
	TIME_OUT = 8,
	GAME_TIME_OVER = 11,
	SAVE_SHOT_DATA = 12
};

} // namespace GHOST

// One entry of the ghost game list (std::list<GHOST_GAME_BRIEF> nodes are
// 0x24 bytes: two links and a 0x1c-byte value copied with rep movsd). The
// six ints at 0x0..0x14 are the numbers of the ghost file name
// "%d_%d_%d_%d_%d_%d.GST" (MakeGhostGameFileName,
// GetGhostGameBriefFromFileName); the first one is the recording user's id.
struct GHOST_GAME_BRIEF {
	unsigned long m_userId;  // 0x00, returned by GetGhostGameBriefFromFileName
	int m_gameId;            // 0x04
	int m_fileNameParts[4];  // 0x08..0x14, the other numbers of the file name
	int m_hasData;           // 0x18 (FrGhostGameMapDlg::Init: non-zero = the map's ghost is playable)

	bool operator==(int gameId) const;   // ghosttypes.h:275
};

CHECK_SIZE(GHOST_GAME_BRIEF, 0x1c);
CHECK_FIELD(GHOST_GAME_BRIEF, m_userId, 0x00);
CHECK_FIELD(GHOST_GAME_BRIEF, m_gameId, 0x04);
CHECK_FIELD(GHOST_GAME_BRIEF, m_fileNameParts, 0x08);
CHECK_FIELD(GHOST_GAME_BRIEF, m_hasData, 0x18);

// A card in effect, as recorded in a ghost file (converted from/to
// sSCardAvilityPeriodInfo by conv_legacycard_to_ghostcard and
// conv_ghostcard_to_legacycard).
struct GHOST_CARD_DATA {
	unsigned long typeId;      // 0x00, sSCardAvilityPeriodInfo::typeId
	unsigned long id;          // 0x04, sSCardAvilityPeriodInfo::id
	unsigned long partsTypeId; // 0x08
	unsigned long partsId;     // 0x0C
	int effect;                // 0x10
	int effectValue;           // 0x14
	int slot;                  // 0x18
	int kind;                  // 0x1C
};

CHECK_SIZE(GHOST_CARD_DATA, 0x20);
CHECK_FIELD(GHOST_CARD_DATA, typeId, 0x00);
CHECK_FIELD(GHOST_CARD_DATA, id, 0x04);
CHECK_FIELD(GHOST_CARD_DATA, partsTypeId, 0x08);
CHECK_FIELD(GHOST_CARD_DATA, partsId, 0x0C);
CHECK_FIELD(GHOST_CARD_DATA, effect, 0x10);
CHECK_FIELD(GHOST_CARD_DATA, effectValue, 0x14);
CHECK_FIELD(GHOST_CARD_DATA, slot, 0x18);
CHECK_FIELD(GHOST_CARD_DATA, kind, 0x1C);

// The aligned copy of sShotProtocol (packed, 0x2E bytes, GolfDoc.h) stored in
// a ghost file; conv_ghostshot_to_shotprotocol and conv_shotprotocol_to_ghostshot
// (ghostmanager.cpp:241, 258) copy it member by member.
struct GHOST_SHOT_PROTOCOL {
	float power;                    // 0x00
	float accuracy;                 // 0x04
	float lieSlope;                 // 0x08
	float lieAngle;                 // 0x0C
	unsigned char accuracyGrade;    // 0x10
	unsigned long powerFlags;       // 0x14
	unsigned long frame;            // 0x18
	float angle;                    // 0x1C
	int shotElapsed;                // 0x20
	float barStartPosition;         // 0x24
	unsigned char club;             // 0x28
	float randomPower;              // 0x2C
	unsigned long spot;             // 0x30
};

CHECK_SIZE(GHOST_SHOT_PROTOCOL, 0x34);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, power, 0x00);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, accuracy, 0x04);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, lieSlope, 0x08);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, lieAngle, 0x0C);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, accuracyGrade, 0x10);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, powerFlags, 0x14);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, frame, 0x18);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, angle, 0x1C);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, shotElapsed, 0x20);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, barStartPosition, 0x24);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, club, 0x28);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, randomPower, 0x2C);
CHECK_FIELD(GHOST_SHOT_PROTOCOL, spot, 0x30);

// The recording player's information stored in a ghost file (the aligned
// copy of sUserInfo's members a ghost replay needs). conv_userinfo_to_ghostdata
// and conv_ghostdata_to_userinfo (ghostmanager.cpp:144, 94) copy it member by
// member; conv_ghostdata_to_slotinfo (ghostmanager.cpp:194) fills an
// sSlotInfo from it. The strings are char[0x20] here and shorter in
// sUserInfo/sSlotInfo: py_strcpy copies N - 1 bytes of the source into a
// zeroed char[N]. Bytes whose meaning no caller shows stay UNKNOWN; their
// comments name where the conversions copy them from.
struct GHOST_USER_DATA {
	unsigned long characterTypeId;  // 0x000, sUserInfo::m_characterInfo.typeId
	unsigned char hairColor;        // 0x004, sUserInfo::m_characterInfo.hairColor
	unsigned long partTypeIds[24];  // 0x008, sUserInfo::m_characterInfo.partTypeIds (0x18 dwords)
	unsigned long auxPartTids[5];   // 0x068, sUserInfo::m_characterInfo.auxParts
	UNKNOWN(0x07C, 0x081);          // sCharacterInfo::unknown6A (5 bytes, copied as one structure)
	UNKNOWN(0x081, 0x159);          // sCharacterInfo::unknown84 (0xD8 bytes, copied with 0x36 dwords)
	UNKNOWN(0x159, 0x15C);
	unsigned long clubSetTypeId;    // 0x15C, sUserInfo::m_clubSetTypeId
	UNKNOWN(0x160, 0x16C);          // a 10-byte block (sUserInfo 0xB49, the club set's stats), copied as one structure
	unsigned long ballTypeId;       // 0x16C, sUserInfo::m_ballId
	unsigned long caddieTypeId;     // 0x170, sUserInfo::m_caddieTypeId
	unsigned long cadItemTypeId;    // 0x174, sUserInfo::m_cadItemTypeId
	UNKNOWN(0x178, 0x17C);          // a byte (sUserInfo 0xB34)
	unsigned long mascotTypeId;     // 0x17C, sUserInfo 0xB57 (the equipped mascot's type id), sSlotInfo::m_valueC7
	UNKNOWN(0x180, 0x1A0);          // a char[0x20] (sUserInfo char[0x1E] at 0xB60, in the mascot's record)
	unsigned long equipItem[10];    // 0x1A0, sUserInfo::m_equipItem
	char name[0x20];                // 0x1C8, sUserInfo::m_name, sSlotInfo::m_nickname
	char nickname[0x20];            // 0x1E8, sUserInfo::m_nickname
	char guildName[0x20];           // 0x208, sUserInfo::m_guildName, sSlotInfo::m_guildName
	unsigned long identity;         // 0x228, sUserInfo::m_identity, sSlotInfo::m_identity
	char guildMark[0x20];           // 0x22C, sUserInfo::m_guildMark, sSlotInfo::m_guildMark
	// bits 1, 3, 4 and 5 of sUserInfo's byte at 0x73 (sPangYaUserInfo::eventFlags)
	unsigned char eventFlag1;       // 0x24C, sSlotInfo 0x54 bits 5-7
	unsigned char eventFlag3;       // 0x24D, sSlotInfo 0x57 bit 0
	unsigned char eventFlag4;       // 0x24E, sSlotInfo 0x57 bit 1
	unsigned char eventFlag5;       // 0x24F
	unsigned char level;            // 0x250, sUserInfo::m_level (sSlotInfo 0x56)
	unsigned long uid;              // 0x254, sUserInfo 0x10B; the first dword of CGhostPlayer::SendAwardResult's packet (0xBA)
};

CHECK_SIZE(GHOST_USER_DATA, 0x258);
CHECK_FIELD(GHOST_USER_DATA, characterTypeId, 0x000);
CHECK_FIELD(GHOST_USER_DATA, hairColor, 0x004);
CHECK_FIELD(GHOST_USER_DATA, partTypeIds, 0x008);
CHECK_FIELD(GHOST_USER_DATA, auxPartTids, 0x068);
CHECK_FIELD(GHOST_USER_DATA, ballTypeId, 0x16C);
CHECK_FIELD(GHOST_USER_DATA, caddieTypeId, 0x170);
CHECK_FIELD(GHOST_USER_DATA, cadItemTypeId, 0x174);
CHECK_FIELD(GHOST_USER_DATA, clubSetTypeId, 0x15C);
CHECK_FIELD(GHOST_USER_DATA, mascotTypeId, 0x17C);
CHECK_FIELD(GHOST_USER_DATA, name, 0x1C8);
CHECK_FIELD(GHOST_USER_DATA, guildName, 0x208);
CHECK_FIELD(GHOST_USER_DATA, identity, 0x228);
CHECK_FIELD(GHOST_USER_DATA, guildMark, 0x22C);
CHECK_FIELD(GHOST_USER_DATA, eventFlag1, 0x24C);
CHECK_FIELD(GHOST_USER_DATA, eventFlag3, 0x24D);
CHECK_FIELD(GHOST_USER_DATA, eventFlag4, 0x24E);
CHECK_FIELD(GHOST_USER_DATA, eventFlag5, 0x24F);
CHECK_FIELD(GHOST_USER_DATA, equipItem, 0x1A0);
CHECK_FIELD(GHOST_USER_DATA, nickname, 0x1E8);
CHECK_FIELD(GHOST_USER_DATA, level, 0x250);
CHECK_FIELD(GHOST_USER_DATA, uid, 0x254);

// Copies a string into a fixed char array: zeroes the array, then copies its
// length minus one bytes of the source (the GhostManager conversions; the
// out-of-line copies are py_strcpy<0x1E>, <0x16>, <0x15>, <0xC> and <0x20>).
// ghosttypes.h:291
template <int N>
void py_strcpy(char (&dst)[N], const char *src) {
	memset(dst, 0, N);
	memcpy(dst, src, N - 1);
}

#endif // PG_CLIENT_PROJECTG_GHOSTTYPES_H
