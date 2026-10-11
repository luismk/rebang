// Original: d:\build\custom\temp\code\client\projectg\buddymanager.cpp
#include <windows.h>
#include <string.h>
#include <string>

#include "buddymanager.h"
#include "ChatMsg.h"
#include "IMessage.h"
#include "LobbyTask.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "commonutil.h"
#include "frwnd.h"

// Tick counts of BuddyManager::Process (file statics, no public names).
static unsigned long s_lastBuddyListCheck; // 0xb79530
static unsigned long s_lastBuddyQuery;     // 0xb79534
static unsigned long s_currentTick;        // 0xb79538

// buddymanager.cpp:33
void ProcMsg(sockaddr_in &addr, const char *msg, int len) {
	BuddyMGR->ProcCallBackMsg(addr, msg, len);
}

// buddymanager.cpp:38
void ProcErr(const char *err) {
}

// buddymanager.cpp:44
BuddyManager::BuddyManager(void) {
	m_bInit = false;
	BuddyMGR = this;
}

// buddymanager.cpp:51
BuddyManager::~BuddyManager(void) {
	UnRegisterDNS();
}

// buddymanager.cpp:61
bool BuddyManager::Init(void) {
	return m_bInit;
}

// buddymanager.cpp:73
void BuddyManager::Close(void) {
}

// buddymanager.cpp:92
void BuddyManager::UnRegisterDNS(void) {
	ISendMsg msg(6);
	msg.EncodeStr(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id);
	msg.MakePacketComplete();
}

// buddymanager.cpp:115
void BuddyManager::CheckBuddyList(void) {
}

// buddymanager.cpp:120
void BuddyManager::QueryBuddy(const char *nick) {
	ISendMsg msg(2);
	msg.EncodeStr(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id);
	msg.EncodeStr(nick);
	msg.MakePacketComplete();
}

// buddymanager.cpp:131
void BuddyManager::ClearBuddy(void) {
	m_buddyList.clear();
}

// buddymanager.cpp:137
void BuddyManager::SendDNS(const char *msg, int len) {
}

// buddymanager.cpp:150
void BuddyManager::AddBuddy(sFriend *buddy) {
	if (buddy) {
		WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringHack(buddy->group, false);
		m_buddyList[buddy->uid] = *buddy;
	}
}

// buddymanager.cpp:192
void BuddyManager::DeleteBuddy(unsigned long id) {
	m_buddyList.erase(id);
}

// buddymanager.cpp:198
void BuddyManager::SendChat(const char *nick, const char *msg) {
	ISendMsg packet(4);
	packet.EncodeStr(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id);
	packet.EncodeStr(nick);
	packet.EncodeStr(msg);
	packet.MakePacketComplete();
}

// buddymanager.cpp:211
int BuddyManager::GetBuddySize(void) {
	return m_buddyList.size();
}

// buddymanager.cpp:216
std::map<unsigned long, sFriend> *BuddyManager::GetBuddyList(void) {
	return &m_buddyList;
}

// buddymanager.cpp:222
void BuddyManager::UpdateBuddyInfo(sFriend *buddy) {
	sFriend *pFriend = GetBuddy(buddy->uid);
	if (pFriend) {
		pFriend->channel = buddy->channel;
		pFriend->status = buddy->status;
	}
	else {
		WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringHack(buddy->group, false);
		m_buddyList[buddy->uid] = *buddy;
	}
}

// buddymanager.cpp:280
void BuddyManager::BuddyLogOn(unsigned long id) {
	sFriend *pFriend = GetBuddy(id);
	if (pFriend) {
		pFriend->flags |= 2;
		if (pFriend->flags & 8) {
			// "Friend (%s) has logged on to PangYa."
			DisplayString(MakeStr("\xC4\xA3\xB1\xB8 (%s) \xB4\xD4\xC0\xCC \xC6\xCE\xBE\xDF\xBF\xA1 \xC1\xA2\xBC\xD3 \xC7\xCF\xBC\xCC\xBD\xC0\xB4\xCF\xB4\xD9.", pFriend));
		}
	}
}

// buddymanager.cpp:295
void BuddyManager::BuddyLogOut(unsigned long id) {
	sFriend *pFriend = GetBuddy(id);
	if (pFriend) {
		if ((pFriend->flags & 2) && (pFriend->flags & 8)) {
			// "Friend (%s) has left PangYa"
			DisplayString(MakeStr("\xC4\xA3\xB1\xB8 (%s) \xB4\xD4\xC0\xCC \xC6\xCE\xBE\xDF\xB8\xA6 \xC1\xBE\xB7\xE1 \xC7\xCF\xBC\xCC\xBD\xC0\xB4\xCF\xB4\xD9", pFriend));
		}
		pFriend->flags &= ~2;
	}
}

// buddymanager.cpp:330
sFriend *BuddyManager::GetBuddyByNick(const char *nick) {
	for (std::map<unsigned long, sFriend>::iterator it = m_buddyList.begin(); it != m_buddyList.end(); ++it) {
		if (stricmp(it->second.nick, nick) == 0) {
			return &it->second;
		}
	}
	return 0;
}

// buddymanager.cpp:347
sFriend *BuddyManager::GetBuddy(unsigned long id) {
	for (std::map<unsigned long, sFriend>::iterator it = m_buddyList.begin(); it != m_buddyList.end(); ++it) {
		if (it->second.uid == id) {
			return &it->second;
		}
	}
	return 0;
}

// buddymanager.cpp:365
void BuddyManager::SetRecentPlayer(void) {
}

// buddymanager.cpp:391
void BuddyManager::Process(void) {
	s_currentTick = GetTickCount();

	if (m_bInit) {
		if (s_currentTick - s_lastBuddyQuery > 60000) {
			s_lastBuddyQuery = s_currentTick;
		}

		if (s_currentTick - s_lastBuddyListCheck > 60000) {
			s_lastBuddyListCheck = s_currentTick;
		}
	}
}

// buddymanager.cpp:423
void BuddyManager::DisplayString(const char *text) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CLobbyTask::m_RTTI)) {
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 3, (intptr_t)text, 0xFFFF7878, 0, 0, 0));
	}
	else {
		WSingleton<CChatMsg>::Instance()->AddChatMsg(text, 0xFFFF0000, false, false);
	}
}

// buddymanager.cpp:431
void BuddyManager::ProcCallBackMsg(sockaddr_in &addr, const char *msg, int len) {
	IRecvMsg packet;
	packet.SetRcvPacket(msg, len);

	packet.Decode2();
	unsigned short type = packet.Decode2();

	switch (type) {
	case 4: {
		std::string nick = packet.DecodeStr();
		std::string chat = packet.DecodeStr();

		std::string text = nick;
		text += " : ";
		text += chat;

		DisplayString(text.c_str());
		break;
	}
	case 2: {
		sFriend buddy;
		packet.DecodeBuffer(&buddy, sizeof(sFriend));
		UpdateBuddyInfo(&buddy);
		break;
	}
	case 7: {
		std::string nick = packet.DecodeStr();
		break;
	}
	case 8: {
		std::string nick = packet.DecodeStr();
		break;
	}
	case 9: {
		std::string nick = packet.DecodeStr();
		break;
	}
	}
}

// buddymanager.cpp:496
void BuddyManager::ProcCallBackErr(const char *err) {
}
