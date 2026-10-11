// Original: d:\build\custom\temp\code\client\projectg\buddymanager.h
#ifndef PG_CLIENT_PROJECTG_BUDDYMANAGER_H
#define PG_CLIENT_PROJECTG_BUDDYMANAGER_H

#include <map>
#include <string.h>

#include "..\..\defines.h"
#include "singleton.h"

struct sockaddr_in;

// A buddy entry, 0x8D bytes, byte-aligned (the map node's _Color sits at
// 0xA0; FrFriendDlg reads the uid at 0x21). The original declares it in
// globalgamedefine.h:3543 (constructor and Clear emitted by SharedDoc).
#pragma pack(push, 1)
// The block at sFriend+0x3D (0x4B bytes): a member whose inline constructor
// runs before sFriend's own body (FrMessengerDlg's sFriend array inlines it:
// word, two dwords and a byte set to -1, then 0x40 bytes cleared). Clear()
// resets it to the same values. Meaning unconfirmed.
struct sFriendExtra {
	sFriendExtra(void) : code(0xFFFF), id1(0xFFFFFFFF), id2(0xFFFFFFFF), index(0xFF) {
		memset(data, 0, sizeof(data));
	}

	unsigned short code;      // 0x0 (sFriend+0x3D), -1
	unsigned long id1;        // 0x2 (sFriend+0x3F), -1
	unsigned long id2;        // 0x6 (sFriend+0x43), -1
	unsigned char index;      // 0xA (sFriend+0x47), -1
	char data[0x40];          // 0xB (sFriend+0x48), cleared
};
CHECK_SIZE(sFriendExtra, 0x4B);

struct sFriend {
	// globalgamedefine.h:3543
	sFriend(void) {
		Clear();
	}

	void Clear(void);         // globalgamedefine.h:3549, emitted by SharedDoc

	char nick[0x16];          // 0x0, shown by FrFriendDlg, passed to CUserInfo::SetInfo
	char group[0xB];          // 0x16, "Friend" for a plain friend (FrFriendDlg::OnFriendListOwnerDraw)
	unsigned long uid;        // 0x21, -1: none
	unsigned long value25;    // 0x25, -1 after Clear (meaning not known)
	unsigned long value29;    // 0x29, 0 after Clear (meaning not known)
	unsigned long value2D;    // 0x2D, -1 after Clear (meaning not known)
	char data31[0xC];         // 0x31, cleared by Clear (meaning not known)
	sFriendExtra extra;       // 0x3D
	unsigned char status;     // 0x88, 5 after Clear; refreshed by UpdateBuddyInfo (meaning unconfirmed)
	unsigned char channel;    // 0x89, -1 after Clear; refreshed by UpdateBuddyInfo (meaning unconfirmed)
	unsigned char value8A;    // 0x8A, 0 after Clear (meaning not known)
	unsigned char flags;      // 0x8B: bit 0 female icon, bit 1 logged on, bit 3 log-on notice, bits 1-2 drawn opaque when both set, listed when 0x0C has any
	unsigned char listFlags;  // 0x8C, 0 after Clear: bits 0 and 1 counted by FrMessengerDlg::SetCount (meaning unconfirmed)
};
#pragma pack(pop)
CHECK_SIZE(sFriend, 0x8D);
CHECK_FIELD(sFriend, nick, 0x0);
CHECK_FIELD(sFriend, group, 0x16);
CHECK_FIELD(sFriend, uid, 0x21);
CHECK_FIELD(sFriend, extra, 0x3D);
CHECK_FIELD(sFriend, status, 0x88);
CHECK_FIELD(sFriend, channel, 0x89);
CHECK_FIELD(sFriend, flags, 0x8B);
CHECK_FIELD(sFriend, listFlags, 0x8C);

// Buddy list manager (WSingleton<BuddyManager>, 0x3C bytes).
class BuddyManager : public WSingleton<BuddyManager> {
public:
	BuddyManager(void);
	virtual ~BuddyManager(void);

	bool Init(void);
	void Close(void);
	void UnRegisterDNS(void);
	void CheckBuddyList(void);
	void QueryBuddy(const char *nick);
	void ClearBuddy(void);
	void SendDNS(const char *msg, int len);
	void DeleteBuddy(unsigned long id);
	void SendChat(const char *nick, const char *msg);
	int GetBuddySize(void);
	std::map<unsigned long, sFriend> *GetBuddyList(void);
	sFriend *GetBuddyByNick(const char *nick);
	sFriend *GetBuddy(unsigned long id);
	void BuddyLogOn(unsigned long id);
	void BuddyLogOut(unsigned long id);
	void AddBuddy(sFriend *buddy);
	void UpdateBuddyInfo(sFriend *buddy);
	void SetRecentPlayer(void);
	void Process(void);
	void DisplayString(const char *text);
	void ProcCallBackMsg(sockaddr_in &addr, const char *msg, int len);
	void ProcCallBackErr(const char *err);

	// buddymanager.h:118
	int IsInit(void) {
		return m_bInit;
	}

	UNKNOWN(0x28, 0x2C);
	std::map<unsigned long, sFriend> m_buddyList; // 0x2C
	bool m_bInit;                                 // 0x38
};
CHECK_FIELD(BuddyManager, m_buddyList, 0x2C);
CHECK_FIELD(BuddyManager, m_bInit, 0x38);
CHECK_SIZE(BuddyManager, 0x3C);

extern BuddyManager *BuddyMGR;

BuddyManager *Buddy(void);             // buddymanager.h:130

#endif // PG_CLIENT_PROJECTG_BUDDYMANAGER_H
