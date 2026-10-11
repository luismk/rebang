// Original: d:\build\custom\temp\code\client\projectg\securitykeymanager.cpp
#include "SecurityKeyManager.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "actor.h"

// securitykeymanager.cpp:9
CSecurityKeyManager* SecurityKeyManager(void) {
	static CSecurityKeyManager s_securityKeyManager;
	return &s_securityKeyManager;
}

// securitykeymanager.cpp:15
CSecurityKeyManager::CSecurityKeyManager(void) {
}

// securitykeymanager.cpp:19
CSecurityKeyManager::~CSecurityKeyManager(void) {
}

// securitykeymanager.cpp:23
void CSecurityKeyManager::SetKey(eSKContent content, unsigned long serial, std::string key) {
	sSKItem* pItem = GetItem(content, serial);
	if (pItem != NULL) {
		pItem->key = key;
		return;
	}

	sSKItem item;
	item.content = content;
	item.serial = serial;
	item.key = key;

	m_items.push_back(item);
}

// securitykeymanager.cpp:43
std::string CSecurityKeyManager::GetKey(eSKContent content, unsigned long serial) {
	sSKItem* pItem = GetItem(content, serial);
	if (pItem == NULL) {
		return "";
	}
	return pItem->key;
}

// securitykeymanager.cpp:52
sSKItem* CSecurityKeyManager::GetItem(eSKContent content, unsigned long serial) {
	for (std::list<sSKItem>::iterator it = m_items.begin(); it != m_items.end(); ++it) {
		sSKItem* pItem = &(*it);
		if (pItem->content == content && pItem->serial == serial) {
			return pItem;
		}
	}
	return NULL;
}

// securitykeymanager.cpp:68
void CSecurityKeyManager::ReqCreateKey(eSKContent content, unsigned long serial) {
	WSendPacket packet(0xC1);
	packet.Encode1(0);
	packet.Encode4(MyUID());
	packet.Encode1(content);
	packet.Encode4(serial);
	packet.Send(SEND_TO_DEFAULT);
}

// securitykeymanager.cpp:78
void CSecurityKeyManager::ResCreateKey(eSKContent content, unsigned long serial, std::string key, unsigned char result) {
	if (result == 1) {
		SetKey(content, serial, key);
	}

	switch (content) {
	case SK_CONTENT_UCC:
		if (result == 1) {
			AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x212, 0, 0, 0, 0, 0);
		}
		break;
	}
}

// securitykeymanager.cpp:110
void CSecurityKeyManager::ReqCheckKey(eSKContent content, unsigned long serial, std::string key) {
	WSendPacket packet(0xC1);
	packet.Encode1(1);
	packet.Encode4(MyUID());
	packet.Encode1(content);
	packet.Encode4(serial);
	packet.EncodeStr(key);
	packet.Send(SEND_TO_DEFAULT);
}

// securitykeymanager.cpp:121
void CSecurityKeyManager::ResCheckKey(eSKContent content, unsigned long serial, std::string key, unsigned char result) {
}
