// Original: d:\build\custom\temp\code\client\projectg\securitykeymanager.h
#ifndef PG_CLIENT_PROJECTG_SECURITYKEYMANAGER_H
#define PG_CLIENT_PROJECTG_SECURITYKEYMANAGER_H

#include <list>
#include <string>

#include "..\..\defines.h"

// Content a security key is requested for (only the value the UCC drawing
// dialog passes is known).
enum eSKContent {
	SK_CONTENT_UCC = 1
};

// One security key the server created for a content's serial (list element
// of CSecurityKeyManager; layout from SetKey and GetKey).
struct sSKItem {
	eSKContent content;      // 0x00
	unsigned long serial;    // 0x04
	std::string key;         // 0x08
};
CHECK_SIZE(sSKItem, 0x24);
CHECK_FIELD(sSKItem, content, 0x00);
CHECK_FIELD(sSKItem, serial, 0x04);
CHECK_FIELD(sSKItem, key, 0x08);

// Requests and checks the security keys of the player's contents; the keys
// the server returns are kept per content and serial.
class CSecurityKeyManager {
	LAYOUT_CHECK_FRIEND;

public:
	CSecurityKeyManager(void);
	~CSecurityKeyManager(void);

	void SetKey(eSKContent content, unsigned long serial, std::string key);
	std::string GetKey(eSKContent content, unsigned long serial);
	void ReqCreateKey(eSKContent content, unsigned long serial);
	void ResCreateKey(eSKContent content, unsigned long serial, std::string key, unsigned char result);
	void ReqCheckKey(eSKContent content, unsigned long serial, std::string key);
	void ResCheckKey(eSKContent content, unsigned long serial, std::string key, unsigned char result);

private:
	sSKItem* GetItem(eSKContent content, unsigned long serial);

	std::list<sSKItem> m_items;  // 0x00
};
CHECK_SIZE(CSecurityKeyManager, 0xC);
BEGIN_LAYOUT_CHECK(CSecurityKeyManager)
	CHECK_FIELD(CSecurityKeyManager, m_items, 0x00);
END_LAYOUT_CHECK

CSecurityKeyManager* SecurityKeyManager(void);

#endif // PG_CLIENT_PROJECTG_SECURITYKEYMANAGER_H
