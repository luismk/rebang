// Original: sMascotInfo's original header is not known; it lives here so that
// SharedDoc.h (the equipped mascot of a player) and MascotInfoDlg.h share it.
#ifndef PG_CLIENT_PROJECTG_MASCOTINFO_H
#define PG_CLIENT_PROJECTG_MASCOTINFO_H

#include <windows.h>
#include "..\..\defines.h"
#include "..\..\padding.h"

// A mascot owned by the player, as the server sends it (0x3f bytes, copied
// whole by FrMascotInfoDlg::SetMascotInfo). The original header of this type
// is not known yet; only the members this unit uses are named.
#pragma pack(push, 1)
struct sMascotInfo {
	unsigned long id;           // 0x0 (item id sent back in the message-change packets)
	unsigned long typeId;       // 0x4 (IFF type id, CItemManager::FindMascot)
	UNKNOWN(0x8, 0xd);
	char message[0x1e];         // 0xd (text shown on the mascot)
	unsigned short remainHours; // 0x2b (remaining time in hours, split into days and hours by OnInit)
	UNKNOWN(0x2d, 0x2e);
	_SYSTEMTIME endTime;        // 0x2e (expiry; Net_EquipMascot unequips an expired non-PC-bang mascot)
	unsigned char pcBang;       // 0x3e (PC-bang mascot: changing its message uses packet 0x97)
};
#pragma pack(pop)

#endif // PG_CLIENT_PROJECTG_MASCOTINFO_H
