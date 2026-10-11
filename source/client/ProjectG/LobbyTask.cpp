// Original: d:\build\custom\temp\code\client\projectg\lobbytask.cpp
#include "NetworkSystem.h"
#include <windows.h>
#include <mmsystem.h>

#include "guilddefine.h"
#include "ClientSetting.h"
#include "LobbyTask.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "fresh.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "contentsdoc.h"
#include "GolfDoc.h"
#include "singleton.h"
#include "user_info.h"
#include "logininfo.h"
#include "commonutil.h"
#include "../shared/Localize.h"
#include "DlgQuickStartSetting.h"
#include "messengerdlg.h"
#include "UccLibrary.h"
#include "Intrusion.h"
#include "MouseCursor.h"
#include "ProjectG.h"
#include "ChatMsg.h"
#include "DetailedRoomInfoDlg.h"

#include <algorithm>

class TicketInfo;

extern HWND g_hwnd;

void _GGC_SendUserID(const char *userId);
extern int g_iRecvPacketType;

// The class and actor names of the lobby's guild and gateway actors (pairs
// of adjacent pointers of unnamed data).
extern const char *data_00ab2c78;
extern const char *data_00ab2c7c;
extern const char *data_00ab2c84;
extern const char *data_00ab2c88;

// clientsetting.h:418
void COption::mSetMacroText(int index, const char* text) {
	strcpy(m_option.m_macroText[index], text);
}

// clientsetting.h:419
int COption::mGetSaveMacros(void) {
	return m_option.m_saveMacros;
}

// shareddoc.h:623
void CSharedDoc::SetTimeBlock(int timeBlock) {
	m_userInfo.timeBlock = timeBlock;
}

// shareddoc.h:1058
unsigned long CSharedDoc::GetFriendGameSvrUID(void) {
	return m_friendGameSvrUID;
}

// shareddoc.h:1074
void CSharedDoc::SetForceNicknameChange(unsigned char force) {
	m_forceNicknameChange = force;
}

// user_info.h:597
unsigned long CUserInfo::GetGuid(void) {
	return m_guid;
}

// lobbytask.cpp:52
void RecvRSS(int) {
}

// lobbytask.cpp:57 (run-time type and factory registration of CLobbyTask)
const WRTTI CLobbyTask::m_RTTI("CLobbyTask", &CTask::m_RTTI);

IObject* CLobbyTaskMakeInstance(void) {
	return new CLobbyTask;
}

struct __sCLobbyTask {
	__sCLobbyTask(void) {
		ObjectFactory().AddObjectFunctor(CLobbyTaskMakeInstance, "CLobbyTask");
	}
};
static __sCLobbyTask s_registerCLobbyTask;

// lobbytask.cpp:60
CLobbyTask::CLobbyTask(void) {
	m_unknownC = true;
	IActor::m_defaultSkipLayer = 0;
	IActor::m_pLayer = 0;
}

// lobbytask.cpp:68
CLobbyTask::~CLobbyTask(void) {
}

// lobbytask.cpp:76
void CLobbyTask::Init(const char *backGround) {
	CTask::Init(NULL);

	SendMessageA(g_hwnd, 0xADB4, 0, 0);
}

// lobbytask.cpp:84
void CLobbyTask::Register(void) {
	SetMainActor(AddActor("CLobbyMain", "Lobby", 0, ""));

	AddActor("ntLobbyBg", "LobbyBg", 0, "");
	GOLFDOC()->m_currentHole = 0x13;

	AddActor("CNPC", "NPC", 0, "");

	AddActor(data_00ab2c78, data_00ab2c7c, 0, "");

	AddActor(data_00ab2c84, data_00ab2c88, 0, GetRTTI()->GetName());
}

// lobbytask.cpp:118
void CLobbyTask::Load(void) {
	SetProgressBar(false);
	SetTip(false);

	CTask::Load();
}

// lobbytask.cpp:126
void CLobbyTask::Process(float dt) {
	g_pFresh->OnProcess(dt);
	CTask::Process(dt);
}

// lobbytask.cpp:143
void CLobbyTask::Display(void) {
	if (!WSingleton<CSharedDoc>::Instance()->m_3DBackgroundOff)
		CTask::Display();

	g_pFresh->OnDisplay(WSingleton<CSharedDoc>::Instance()->m_3DBackgroundOff != 0);
}

// lobbytask.cpp:151
// FIRST PASS: the switch dispatch and the small cases are written; the other
// cases carry the line range and calls of the original.
int CLobbyTask::OnPacket(WReceivedPacket &packet) {
	g_iRecvPacketType = packet.Decode2();

	switch (g_iRecvPacketType) {
	case 0xC2: {
		// Another player's change (by player guid), not the local player's own.
		unsigned long guid = packet.Decode4();
		unsigned char kind = packet.Decode1();

		if (guid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid) {
			break;
		}

		switch (kind) {
		case 0: {
			unsigned long value;
			packet.DecodeBuffer(&value, sizeof(value));
			GetActor("Lobby") << MsgObject(0, 0x5B, guid, 0, (intptr_t)&value, 0, 0);
			break;
		}
		case 1: {
			std::string text;
			packet.DecodeStr(text);
			GetActor("Lobby") << MsgObject(0, 0x5B, guid, 1, (intptr_t)text.c_str(), 0, 0);
			break;
		}
		case 2:
		case 3:
			GetActor("Lobby") << MsgObject(0, 0x5B, guid, kind, 0, 0, 0);
			break;
		}
		break;
	}
	case 0x11B: {
		// The tutorial's progress: one element, or all three (index 3).
		unsigned char index = packet.Decode1();
		packet.Decode1();

		if (index == 3) {
			packet.DecodeBuffer(WSingleton<CSharedDoc>::Instance()->m_tutorialData, sizeof(WSingleton<CSharedDoc>::Instance()->m_tutorialData));
		} else if (index < 3) {
			WSingleton<CSharedDoc>::Instance()->m_tutorialData[index] = packet.Decode4();
		}
		break;
	}
	case 0xB0: {
		// The received notes.
		WSingleton<CSharedDoc>::Instance()->m_noteList.clear();

		unsigned char count = packet.Decode1();
		for (int i = 0; i < count; i++) {
			sNoteInfo note;
			packet.DecodeBuffer(&note, 0x6D);

			WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringHack(note.text, false);
			WSingleton<CSharedDoc>::Instance()->m_noteList.push_back(note);
		}
		break;
	}
	case 0x82: {
		// A whisper: bit 7 of the first byte marks a GM, the rest 0 for the
		// echo of the player's own whisper.
		unsigned char flags = packet.Decode1();
		unsigned char gm = flags >> 7;
		unsigned char kind = flags & 0x7F;

		bool own = kind == 0;

		int notify;
		unsigned long color = 0xFFD2A2FF;
		if (gm) {
			color = 0xFF6EECFB;
		}

		std::string nickname, message;
		std::string text;

		packet.DecodeStr(nickname);
		packet.DecodeStr(message);

		WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringHack(message.c_str(), false);

		if (own) {
			text = WSingleton<CSharedDoc>::Instance()->m_userInfo.nickname;
			text += ">";
			text += nickname;
		} else {
			if (!WSingleton<COption>::Instance()->gIsWhisperAllowed()) {
				break;
			}

			std::list<std::string>::iterator it = std::find(WSingleton<CSharedDoc>::Instance()->m_ignoreList.begin(), WSingleton<CSharedDoc>::Instance()->m_ignoreList.end(), nickname);
			if (it != WSingleton<CSharedDoc>::Instance()->m_ignoreList.end()) {
				break;
			}

			GetActor("Lobby") << MsgObject(0, 0x3F, (intptr_t)&notify, 0, 0, 0, 0);
			GetActor("Lobby") << MsgObject(0, 0x38, (intptr_t)nickname.c_str(), 0, 0, 0, 0);

			text = nickname + ">" + WSingleton<CSharedDoc>::Instance()->m_userInfo.nickname;
		}

		text += " : \x08'";
		text += message;
		text += "'";

		if (AfxGetTask()->IsRecieveWhisper()) {
			std::string plain;
			WSingleton<CChatMsg>::Instance()->RemoveColor(text, plain);
			GetActor("Lobby") << MsgObject(0, 3, (intptr_t)plain.c_str(), color, 0, 0, 0);
		} else {
			// This task takes no whispers: tell the sender.
			WSendPacket send((enumClientPacket)0xE0);
			send.EncodeStr(nickname);
			send.Send(SEND_TO_DEFAULT);
		}
		break;
	}
	case 0xAE: {
		// A team chat line.
		std::string nickname;
		std::string message;
		std::string text;

		packet.DecodeStr(nickname);
		packet.DecodeStr(message);

		if (!WSingleton<COption>::Instance()->gIsWhisperAllowed()) {
			break;
		}

		std::list<std::string>::iterator it = std::find(WSingleton<CSharedDoc>::Instance()->m_ignoreList.begin(), WSingleton<CSharedDoc>::Instance()->m_ignoreList.end(), nickname);
		if (it != WSingleton<CSharedDoc>::Instance()->m_ignoreList.end()) {
			break;
		}

		WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringHack(message.c_str(), false);

		// "<nickname>>to the team : <message>"
		text = nickname + ">" + "\xC6\xC0\xBF\xA1\xB0\xD4";
		text += " : ";
		text += message;

		std::string plain;
		WSingleton<CChatMsg>::Instance()->RemoveColor(text, plain);
		GetActor("Lobby") << MsgObject(0, 3, (intptr_t)plain.c_str(), 0xFF00C6FF, 0, 0, 0);
		break;
	}
	case 0x48: {
		// The room's settings changed.
		unsigned short changes = packet.Decode2();

		std::string title;
		std::string password;

		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType = packet.Decode1();
		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_course = packet.Decode1();
		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_holeCount = packet.Decode1();
		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_value4B = packet.Decode1();
		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_userLimit = packet.Decode1();
		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_value46 = packet.Decode1();
		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_option[0] = packet.Decode1() == 1;
		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_value4D = packet.Decode4();
		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_value51 = packet.Decode4();
		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_trophy = packet.Decode4();

		packet.DecodeStr(password);
		memset(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_password, 0, sizeof(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_password));
		strncpy(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_password, password.c_str(), sizeof(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_password) - 1);

		packet.DecodeStr(title);
		WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringHack(title.c_str(), false);
		memset(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_title, 0, sizeof(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_title));
		strncpy(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_title, title.c_str(), sizeof(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_title) - 1);

		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_isPublic = strlen(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_password) == 0;

		// A title or password longer than the room's fields: report it.
		if (strlen(title.c_str()) > strlen(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_title) || strlen(password.c_str()) > strlen(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_password)) {
			int titleLength = strlen(title.c_str());
			int passwordLength = strlen(password.c_str());

			WSingleton<CProjectG>::Instance()->ReportError(6, "[%d %d] %d %d %d %d %d %d(%d %d)", titleLength, passwordLength, changes,
				WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType, WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomMode,
				WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_course, WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_holeCount,
				WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_userLimit, WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomNumber,
				WSingleton<CSharedDoc>::Instance()->m_userInfo.guid);
		}

		// The lobby answers through refresh whether it shows the room.
		int refresh;
		GetActor("Lobby") << MsgObject(0, 0x17, (intptr_t)&refresh, 0, 0, 0, 0);
		if (g_pFresh->IsCurrentLayout("GAMEROOM") || g_pFresh->IsCurrentLayout("GAMEROOM_EXT") || refresh != 0) {
			GetActor("Lobby") << MsgObject(0, 2, 0, 0, 0, 0, 0);
			GetActor("Lobby") << MsgObject(0, 0x10, 0, 0, 0, 0, 0);
			GetActor("Lobby") << MsgObject(0, 0x11, WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_course, 0, 0, 0, 0);
			GetActor("Lobby") << MsgObject(0, 0x12, WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_value4B, 0, 0, 0, 0);
			GetActor("Lobby") << MsgObject(0, 0x2F, 0, 0, 0, 0, 0);
		}
		break;
	}
	case 0x47: {
		// The reply to entering a room: 0 entered (the room's information
		// follows), otherwise the error code.
		WSingleton<CIntrusion>::Instance()->FinishGame();

		GetActor("Lobby") << MsgObject(0, 2, 0, 0, 0, 0, 0);

		if (IsLocalContent((localContentType_t)0x54)) {
			WSingleton<CSharedDoc>::Instance()->m_value5CAC = 0;
		}

		unsigned char result = packet.Decode1();
		if (result == 0) {
			// Whether the player watches as a gallery member, with the guid used there.
			if (packet.Decode1()) {
				WSingleton<CSharedDoc>::Instance()->m_userInfo.identity |= 2;
				WSingleton<CSharedDoc>::Instance()->m_userInfo.galleryGuid = packet.Decode4();
			} else {
				WSingleton<CSharedDoc>::Instance()->m_userInfo.identity &= ~2;
				WSingleton<CSharedDoc>::Instance()->m_userInfo.galleryGuid = 0xFFFFFFFF;
			}

			((std::list<sSlotInfo> &)WSingleton<CSharedDoc>::Instance()->m_roomPlayerList).clear();
			WSingleton<CSharedDoc>::Instance()->m_gameResultList.clear();
			WSingleton<CSharedDoc>::Instance()->m_vector4524.clear();
			WSingleton<CSharedDoc>::Instance()->m_gameOver = false;

			packet.DecodeBuffer(&WSingleton<CSharedDoc>::Instance()->m_roomInfo, sizeof(sRoomInfo));

			WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringHack(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_title, false);
			WSingleton<CSharedDoc>::Instance()->m_mySlot = WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomNumber;

			std::map<unsigned long, sUserInfoTime>::iterator info;
			if (WSingleton<CSharedDoc>::Instance()->FindUserInfoTimeGUID(WSingleton<CSharedDoc>::Instance()->m_userInfo.guid, info)) {
				info->second.m_userInfo.m_slot = WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomNumber;
			}

			const char *layout;
			if (IsMassGame(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType)) {
				GetActor("Lobby") << MsgObject(0, 0x2E, 0, 0, 0, 0, 0);
				layout = "GAMEROOM_EXT";
			} else {
				layout = "GAMEROOM";
			}

			WSingleton<CSharedDoc>::Instance()->m_chatLineList.clear();

			// Tell the messenger server where the player now is.
			if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN) && WSingleton<CMessengerInfo>::Instance()->m_serverListRequest == 4) {
				sUserPosition position;
				position.m_userValue = WSingleton<CSharedDoc>::Instance()->m_value84;
				position.m_channelId = WSingleton<CSharedDoc>::Instance()->m_currentChannel.id;
				wsprintfA(position.m_channelName, "%s", WSingleton<CSharedDoc>::Instance()->m_currentChannel.name);
				position.m_roomNumber = WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomNumber;
				position.m_value2 = WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType;

				WSendPacket send(0x23);
				send.EncodeBuffer(&position, sizeof(position));
				send.Send(SEND_TO_MSN);
			}

			if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 2) {
				// The avatar chat room has its own task.
				WSingleton<CMouseCursor>::Instance()->SetActive(false);
				WSingleton<COption>::Instance()->vSetMinimumScreenSize(1, 800, 600);
				WSingleton<CTaskManager>::Instance()->ChangeTask("ntAvatarChatTask", "CGolfDoc", false);
				WSingleton<CTaskManager>::Instance()->PostMsg(0, "AvatarChat", 0, (intptr_t)"AVATARCHAT_MAIN", 0, 0, 0);
				return 1;
			}

			if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomMode == 14 && WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_masterUID == WSingleton<CSharedDoc>::Instance()->m_myUID) {
				// The room's owner spends one of the items that open this mode.
				std::list<sItemInfo>::iterator it;
				for (it = WSingleton<CSharedDoc>::Instance()->m_itemInfoList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_itemInfoList.end(); ++it) {
					if (it->m_typeId == 0x1A0000F7 && it->m_count > 0) {
						it->m_count--;
						if (it->m_count < 1) {
							WSingleton<CSharedDoc>::Instance()->m_itemInfoList.erase(it);
						}
						break;
					}
				}
			}

			GetActor("Lobby") << MsgObject(0, 0, (intptr_t)layout, 0, 0, 0, 0);
		} else {
			GetActor("Lobby") << MsgObject(0, 0x3B, result, 0, 0, 0, 0);
		}
		break;
	}
	case 0x80: {
		// A player was sent out of the room: the local player goes back to
		// the room list, another one leaves the player list.
		std::list<sSlotInfo> &players = (std::list<sSlotInfo> &)WSingleton<CSharedDoc>::Instance()->m_roomPlayerList;

		std::list<sSlotInfo>::iterator it;
		unsigned long uid = packet.Decode4();

		if (uid != 0xFFFFFFFF) {
			if (uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid) {
				AfxPostMsg(0, "Lobby", 2, 0, 0, 0, 0);
				AfxPostMsg(0, "Lobby", 0, (intptr_t)"ROOMLIST", 0, 0, 0);
			} else {
				it = std::find(players.begin(), players.end(), uid);
				if (it != players.end()) {
					GetActor("Lobby") << MsgObject(0, 8, (intptr_t)&*it, 0, 0, 0, 0);
					players.erase(it);
				}
			}
		}

		GetActor("Lobby") << MsgObject(0, 0x41, (intptr_t)&*it, 0, 0, 0, 0);
		break;
	}
	case 0xA6: {
		// Everyone but the given player and the administrators leaves the room.
		unsigned long uid = packet.Decode4();

		std::list<sSlotInfo> &players = (std::list<sSlotInfo> &)WSingleton<CSharedDoc>::Instance()->m_roomPlayerList;

		std::list<sSlotInfo>::iterator it = players.begin();
		while (it != players.end()) {
			if (it->m_uid != uid && !it->IsIdentity(4)) {
				GetActor("Lobby") << MsgObject(0, 8, (intptr_t)&*it, 0, 0, 0, 0);
				it = players.erase(it);
			} else {
				++it;
			}
		}

		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_userCount = (unsigned char)players.size();

		GetActor("Lobby") << MsgObject(0, 0x2F, 0, 0, 0, 0, 0);
		GetActor("Lobby") << MsgObject(0, 0x0F, 0, 0, 0, 0, 0);
		GetActor("Lobby") << MsgObject(0, 0x10, 0, 0, 0, 0, 0);
		break;
	}
	case 0xAC: {
		// The room's second player list: 0 the whole list, 1 one added,
		// 2 one removed.
		std::list<sSlotInfo> &players = (std::list<sSlotInfo> &)WSingleton<CSharedDoc>::Instance()->m_roomPlayerList2;

		unsigned char type = packet.Decode1();
		switch (type) {
		case 0: {
			players.clear();

			unsigned char count = packet.Decode1();
			sSlotInfo slot;
			for (int i = 0; i < count; i++) {
				packet.DecodeBuffer(&slot, sizeof(slot));
				players.push_back(slot);
			}

			GetActor("Lobby") << MsgObject(0, 0x2F, 0, 0, 0, 0, 0);
			GetActor("Lobby") << MsgObject(0, 0x0F, 0, 0, 0, 0, 0);
			if (WSingleton<CSharedDoc>::Instance()->m_roomPlayerList.size() == 0) {
				GetActor("Lobby") << MsgObject(0, 0x0E, 0, 0, 0, 0, 0);
			}
			break;
		}
		case 1: {
			sSlotInfo slot;
			packet.DecodeBuffer(&slot, sizeof(slot));

			if (std::find(players.begin(), players.end(), slot.m_uid) == players.end()) {
				players.push_back(slot);
			}
			break;
		}
		case 2: {
			unsigned long uid = packet.Decode4();

			std::list<sSlotInfo>::iterator it = std::find(players.begin(), players.end(), uid);
			if (it != players.end()) {
				players.erase(it);
			}
			break;
		}
		}

		GetActor("Lobby") << MsgObject(0, 0x10, 0, 0, 0, 0, 0);
		break;
	}
	case 0x46: {
		// The room's player list: 0 the whole list on entry, 5 more players,
		// 1 a player joined, 2 a player left, 3 a player's slot changed.
		// The original's list is a std::list<sSlotInfo> (the push_back and
		// find instantiations name it); SharedDoc.h still declares it with
		// sRoomPlayer, the same 0x152-byte record.
		std::list<sSlotInfo> &players = (std::list<sSlotInfo> &)WSingleton<CSharedDoc>::Instance()->m_roomPlayerList;

		unsigned char type = packet.Decode1();
		packet.Decode2();

		switch (type) {
		case 0:
		case 5: {
			if (type == 0) {
				players.clear();
				GetActor("Lobby") << MsgObject(0, 0x59, 0, 0, 0, 0, 0);
			}

			unsigned char count = packet.Decode1();
			sSlotInfo slot;
			sCharacterInfo character;
			for (int i = 0; i < count; i++) {
				packet.DecodeBuffer(&slot, sizeof(slot));
				players.push_back(slot);

				if (!IsMassGame(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType)) {
					packet.DecodeBuffer(&character, sizeof(character));
					if (IsLocalContent((localContentType_t)0x57)) {
						RequestUccInfo(character);
					}
					GetActor("Lobby") << MsgObject(0, 0x5A, (intptr_t)&slot, (intptr_t)&character, 0, 0, 0);
				}
			}

			if (IsLocalContent((localContentType_t)0x54)) {
				unsigned char extraCount = packet.Decode1();
				for (int i = 0; i < extraCount; i++) {
					packet.DecodeBuffer(&slot, sizeof(slot));
					players.push_back(slot);
				}
			}

			if (type == 0) {
				// The local player's equipment, for the room's own avatar.
				CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
				std::map<unsigned int, sCaddieInfo>::iterator caddie = doc->m_caddieMap.find(doc->m_myCaddieId);
				if (caddie == doc->m_caddieMap.end()) {
					GetActor("Lobby") << MsgObject(0, 0x13, 0, 0, 0, 0, 0);
				} else {
					GetActor("Lobby") << MsgObject(0, 0x13, caddie->second.typeId, 1, 0, 0, 0);
				}
				GetActor("Lobby") << MsgObject(0, 0x14, WSingleton<CSharedDoc>::Instance()->m_myClubId, 0, 0, 0, 0);
				GetActor("Lobby") << MsgObject(0, 0x15, WSingleton<CSharedDoc>::Instance()->m_myBallId, 0, 0, 0, 0);
			}

			GetActor("Lobby") << MsgObject(0, 0x0E, 0, 0, 0, 0, 0);
			break;
		}
		case 1: {
			// A player joined.
			sSlotInfo slot;
			packet.DecodeBuffer(&slot, sizeof(slot));

			std::list<sSlotInfo>::iterator it = std::find(players.begin(), players.end(), slot.m_uid);
			if (it != players.end()) {
				break;
			}

			players.push_back(slot);

			sSlotInfo &added = players.back();
			if (IsMassGame(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType)) {
				GetActor("Lobby") << MsgObject(0, 7, (intptr_t)&added, 0, 0, 0, 0);
			} else {
				sCharacterInfo character;

				character.id = slot.m_uid;
				if (!slot.m_flags151.flag) {
					packet.DecodeBuffer(&character, sizeof(character));
					if (IsLocalContent((localContentType_t)0x57)) {
						RequestUccInfo(character);
					}
				}

				GetActor("Lobby") << MsgObject(0, 7, (intptr_t)&added, (intptr_t)&character, 0, 0, 0);
			}
			break;
		}
		case 2: {
			// A player left: the players after it move up one connection place.
			unsigned long uid = packet.Decode4();

			std::list<sSlotInfo>::iterator it = std::find(players.begin(), players.end(), uid);
			if (it == players.end()) {
				break;
			}

			sSlotInfo *player = &*it;
			if (player == NULL) {
				players.erase(it);
				break;
			}

			unsigned long playerUid = player->m_uid;
			unsigned char team = player->m_attribute & 3;
			unsigned char connection = player->m_connection;

			players.erase(it);

			for (it = players.begin(); it != players.end(); ++it) {
				if (it->m_connection > connection) {
					it->m_connection--;
				}
			}

			GetActor("Lobby") << MsgObject(0, 8, (intptr_t)player, playerUid, team, 0, 0);
			break;
		}
		case 3: {
			// A player's slot changed.
			sSlotInfo slot;
			unsigned long uid = packet.Decode4();
			packet.DecodeBuffer(&slot, sizeof(slot));

			std::list<sSlotInfo>::iterator it = std::find(players.begin(), players.end(), uid);
			if (it == players.end()) {
				break;
			}

			GetActor("Lobby") << MsgObject(0, 9, (intptr_t)&slot, 0, 0, 0, 0);
			break;
		}
		}

		WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_userCount = (unsigned char)players.size();

		GetActor("Lobby") << MsgObject(0, 0x2F, 0, 0, 0, 0, 0);
		GetActor("Lobby") << MsgObject(0, 0x0F, 0, 0, 0, 0, 0);
		GetActor("Lobby") << MsgObject(0, 0x10, 0, 0, 0, 0, 0);
		if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 6) {
			GetActor("Lobby") << MsgObject(0, 0x71, 0, 0, 0, 0, 0);
		}
		break;
	}
	case 0x1: {
		// The login server's answer to the login: 0 accepted, otherwise the
		// reason it was refused (each shown by the lobby's message 0x1E).
		unsigned char result = packet.Decode1();
		switch (result) {
		case 0: {
			std::string id;
			packet.DecodeStr(id);

			if (_stricmp(id.c_str(), MyId())) {
				GetActor("Lobby") << MsgObject(0, 0x1E, 0xC, 0, 0, 0, 0);
				break;
			}

			WSingleton<CSharedDoc>::Instance()->m_myUID = packet.Decode4();
			WSingleton<CSharedDoc>::Instance()->m_userInfo.identity = packet.Decode4();
			strcpy(WSingleton<CSharedDoc>::Instance()->m_userInfo.id, id.c_str());

			WSingleton<CSharedDoc>::Instance()->m_billingType = CLoginInfo::Instance()->ProvType();
			WSingleton<CSharedDoc>::Instance()->m_myLevel = packet.Decode1();
			WSingleton<CSharedDoc>::Instance()->m_loginTime = timeGetTime();

			_GGC_SendUserID(MyId());

			WSingleton<CSharedDoc>::Instance()->m_value4D70 = packet.Decode1() == 1;

			unsigned long flagBlock = packet.Decode4();
			int timeBlock = packet.Decode4();

			if (IsLocalContent((localContentType_t)8)) {
				WSingleton<CSharedDoc>::Instance()->m_userInfo.flagBlock = flagBlock;
				WSingleton<CSharedDoc>::Instance()->m_userInfo.timeBlock = timeBlock;
			}

			if (IsLocalContent((localContentType_t)0x81)) {
				std::string nickname;
				packet.DecodeStr(nickname);
				strcpy(WSingleton<CSharedDoc>::Instance()->m_userInfo.nickname, nickname.c_str());
			}

			GetActor("Lobby") << MsgObject(0, 0x1E, 4, 0, 0, 0, 0);
			break;
		}
		case 0xDC: {
			// A second password is required.
			bool hasSecondPassword = packet.Decode1() == 1;

			GetActor("Lobby") << MsgObject(0, 0, (intptr_t)"SECOND_PWD", 0, 0, 0, 0);
			IActor *pActor = GetActor(data_00ab2c88);
			pActor << MsgObject(0, 0x26D, 3, (intptr_t)&hasSecondPassword, 0, 0, 0);
			pActor << MsgObject(0, 0x26D, 5, 0, 0, 0, 0);
			break;
		}
		case 0xDD: {
			IActor *pActor = GetActor(data_00ab2c88);
			pActor << MsgObject(0, 0x26D, 0x13, 0, 0, 0, 0);
			pActor << MsgObject(0, 0x26D, 7, 0, 0, 0, 0);
			break;
		}
		case 0xDE:
			GetActor(data_00ab2c88) << MsgObject(0, 0x26D, 0x13, 0, 0, 0, 0);
			break;
		case 0xDF:
			packet.Decode4();
			GetActor("Lobby") << MsgObject(0, 0x1E, 0xC, 0, 0, 0, 0);
			break;
		case 0xD8:
			// The nickname must be changed (-1: only on the login screen).
			if (packet.Decode4() != 0xFFFFFFFF) {
				WSingleton<CSharedDoc>::Instance()->SetForceNicknameChange(2);
				GetActor("Lobby") << MsgObject(0, 0, (intptr_t)"BLANK", 0, 0, 0, 0);
			} else {
				WSingleton<CSharedDoc>::Instance()->SetForceNicknameChange(1);
				if (g_pFresh->IsCurrentLayout("LOGIN") == true) {
					GetActor("Lobby") << MsgObject(0, 0, (intptr_t)"BLANK", 0, 0, 0, 0);
				}
			}
			break;
		case 0xD9:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0xD, 0, 0, 0, 0);
			break;
		case 2:
			GetActor("Lobby") << MsgObject(0, 0x1E, 6, 0, 0, 0, 0);
			break;
		case 6:
			GetActor("Lobby") << MsgObject(0, 0x1E, 7, 0, 0, 0, 0);
			break;
		case 4:
			GetActor("Lobby") << MsgObject(0, 0x1E, 8, 0, 0, 0, 0);
			break;
		case 5:
			WSingleton<CSharedDoc>::Instance()->m_string4D14 = packet.DecodeStr();
			GetActor("Lobby") << MsgObject(0, 0x1E, 9, 0, 0, 0, 0);
			break;
		case 0x13:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0x17, 0, 0, 0, 0);
			break;
		case 7:
			WSingleton<CSharedDoc>::Instance()->m_value4D04 = packet.Decode4();
			WSingleton<CSharedDoc>::Instance()->m_string4D14 = packet.DecodeStr();
			GetActor("Lobby") << MsgObject(0, 0x1E, 0xA, 0, 0, 0, 0);
			break;
		case 8:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0xB, 0, 0, 0, 0);
			break;
		case 0xC:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0x10, 0, 0, 0, 0);
			break;
		case 0xA:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0xF, 0, 0, 0, 0);
			break;
		case 9:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0xE, 0, 0, 0, 0);
			break;
		case 0xD:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0x11, 0, 0, 0, 0);
			break;
		case 0xE:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0x12, 0, 0, 0, 0);
			break;
		case 0xF:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0x13, 0, 0, 0, 0);
			break;
		case 0x10:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0x14, 0, 0, 0, 0);
			break;
		case 0x12:
		case 0xC8:
		case 0xE0:
			break;
		case 0xD4:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0x18, 0, 0, 0, 0);
			break;
		case 0xCF:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0x1A, 0, 0, 0, 0);
			break;
		default:
			GetActor("Lobby") << MsgObject(0, 0x1E, 0xC, 0, 0, 0, 0);
			break;
		}
		break;
	}
	case 0xB:
		packet.DecodeStr(WSingleton<CSharedDoc>::Instance()->m_string5024);
		break;
	case 0x11:
		if (packet.Decode1()) {
			// "Character setup failed. Please reconnect."
			GetActor("Lobby") << MsgObject(0, 0x23, (intptr_t)"\xc4\xb3\xb8\xaf\xc5\xcd \xbc\xb3\xc1\xa4\xbf\xa1 \xbd\xc7\xc6\xd0\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9. \xc0\xe7\xc1\xa2\xbc\xd3\xc7\xcf\xbd\xc3\xb1\xe2 \xb9\xd9\xb6\xf8\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
		}
		break;
	case 0x1CD: {
		// The second password's result (set).
		GetActor("Lobby") << MsgObject(0, 2, 0, 0, 0, 0, 0);
		unsigned long result = packet.Decode4();

		IActor *pActor = GetActor(data_00ab2c88);
		if (result == 0) {
			pActor << MsgObject(0, 0x26D, 8, 0, 0, 0, 0);
			pActor << MsgObject(0, 0x26D, 4, 0, 0, 0, 0);
		} else {
			pActor << MsgObject(0, 0x26D, 1, result, 0, 0, 0);
		}
		break;
	}
	case 0x1CE: {
		// The second password's result (check).
		GetActor("Lobby") << MsgObject(0, 2, 0, 0, 0, 0, 0);
		unsigned long result = packet.Decode4();

		IActor *pActor = GetActor(data_00ab2c88);
		if (result == 0) {
			pActor << MsgObject(0, 0x26D, 8, 0, 0, 0, 0);
			pActor << MsgObject(0, 0x26D, 0xE, 0, 0, 0, 0);
		} else {
			pActor << MsgObject(0, 0x26D, 0xB, result, 0, 0, 0);
		}
		break;
	}
	case 0x10:
		WSingleton<CSharedDoc>::Instance()->SetLoginAuthKey(packet.DecodeStr());
		break;
	case 0xF: {
		// The login server re-confirms the account (after a reconnection).
		WSingleton<CSharedDoc>::Instance()->m_logoutPending = true;
		WSingleton<CSharedDoc>::Instance()->m_flag50F6 = true;

		unsigned char eventFlag = packet.Decode1();
		WSingleton<CSharedDoc>::Instance()->m_userInfo.eventFlags =
			(WSingleton<CSharedDoc>::Instance()->m_userInfo.eventFlags & ~2) | ((eventFlag & 1) << 1);

		std::string id;
		packet.DecodeStr(id);
		if (_stricmp(id.c_str(), MyId())) {
			GetActor("Lobby") << MsgObject(0, 0x1E, 0xC, 0, 0, 0, 0);
			break;
		}

		_GGC_SendUserID(MyId());
		break;
	}
	case 0x2: {
		// The game server list, from the login server.
		GetActor("Lobby") << MsgObject(0, 2, 0, 0, 0, 0, 0);

		bool bReady = false;
		AfxGetTask()->GetActor(data_00ab2c88) << MsgObject(0, 0x26D, 0x14, (intptr_t)&bReady, 0, 0, 0);
		if (!bReady) {
			break;
		}

		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0, (intptr_t)"SERVERLIST", 0, 0, 0, 0);

		WSingleton<CSharedDoc>::Instance()->m_gameServerList.clear();
		int count = packet.Decode1();
		sGameServerInfo info;
		for (int i = 0; i < count; i++) {
			memset(&info, 0, sizeof(info));
			packet.DecodeBuffer(&info, sizeof(info));
			WSingleton<CSharedDoc>::Instance()->m_gameServerList.push_back(info);
		}

		GetActor("Lobby") << MsgObject(0, 0x19, 0, 0, 0, 0, 0);
		break;
	}
	case 0x9: {
		// The messenger server list, from the login server.
		WSingleton<CMessengerInfo>::Instance()->m_serverList.clear();

		MSNServerInfo info;
		info.m_tried = false;
		int count = packet.Decode1();
		for (int i = 0; i < count; i++) {
			memset(&info, 0, sizeof(sGameServerInfo));
			packet.DecodeBuffer(&info, sizeof(sGameServerInfo));
			WSingleton<CMessengerInfo>::Instance()->m_serverList.push_back(info);
		}
		break;
	}
	case 0x4:
		WSingleton<CSharedDoc>::Instance()->m_value94 = packet.Decode1();
		break;
	case 0x3: {
		WNetworkSystem *pNetwork = WSingleton<WNetworkSystem>::Instance();

		if (pNetwork->IsConnected(WNetworkSystem::NET_UNIT_LOGIN)) {
			pNetwork->ForceShutDown(WNetworkSystem::NET_UNIT_LOGIN);
		}
		break;
	}
	case 0x5: {
		// The PC bang event's reward.
		unsigned char kind = packet.Decode1();
		if (kind == 3) {
			unsigned long count = packet.Decode4();
			unsigned long typeId = packet.Decode4();

			if (count > 0 && typeId > 0) {
				char message[0x400] = { 0 };
				IFF_STRUCT::sItem *pItem = WSingleton<CSharedDoc>::Instance()->m_itemManager.FindItem(typeId);

				if (pItem) {
					// "PC bang attendance event: <count> times, <item> received"
					sprintf(message, " PC\xb9\xe6 \xc3\xe2\xbc\xae \xc0\xcc\xba\xa5\xc6\xae\n\n \\cT\\c\\c0xffff0000\\c%d \\c0xff000000\\c\xb9\xf8 \xc0\xc0\xb8\xf0\n \\cT\\c\\c0xffff0000\\c%s \\c0xff000000\\c\xbe\xc6\xc0\xcc\xc5\xdb \xc1\xf6\xb1\xde", count, pItem->m_name);
				} else {
					sprintf(message, " PC\xb9\xe6 \xc3\xe2\xbc\xae \xc0\xcc\xba\xa5\xc6\xae\n\n \\cT\\c\\c0xffff0000\\c%d \\c0xff000000\\c\xb9\xf8 \xc0\xc0\xb8\xf0\n \\cT\\c\\c0xffff0000\\c\xbe\xc6\xc0\xcc\xc5\xdb \\c0xff000000\\c \xc1\xf6\xb1\xde", count);
				}

				GetActor("Lobby") << MsgObject(0, 0x23, (intptr_t)message, 0, 0, 0, 0);
			}
		}
		break;
	}
	case 0x6: {
		// The account's saved game options (macros, PC bang mascot message).
		sOption option;
		sGameOptionInfo info;
		packet.DecodeBuffer(&info, sizeof(info));

		WSingleton<COption>::Instance()->mSetPCBangMascotMsg(info.m_pcBangMascotMsg);

		if (WSingleton<COption>::Instance()->mGetSaveMacros()) {
			for (int i = 0; i < 8; i++) {
				WSingleton<COption>::Instance()->mSetMacroText(i, info.m_macroText[i]);
			}
			WSingleton<COption>::Instance()->mSetPCBangMascotMsg(info.m_pcBangMascotMsg);
		}
		break;
	}
	case 0x8:
		CLoginInfo::Instance()->EnablePcBang(packet.Decode4() == 1);
		break;
	case 0xD: {
		// The result of registering a nickname.
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 2, 0, 0, 0, 0, 0);

		std::string message;
		unsigned long result = packet.Decode4();
		switch (result) {
		case 0: {
			std::string nickname;
			packet.DecodeStr(nickname);
			GetActor("Lobby") << MsgObject(0, 0x249, 0, (intptr_t)nickname.c_str(), 0, 0, 0);
			break;
		}
		case 1:
			// "An error occurred while setting the nickname."
			message = "\xb4\xeb\xc8\xad\xb8\xed \xbc\xb3\xc1\xa4\xc7\xcf\xb4\xc2 \xb0\xfa\xc1\xa4\xbf\xa1\xbc\xad \xbf\xc0\xb7\xf9\xb0\xa1 \xb9\xdf\xbb\xfd\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		case 2:
			// "This nickname is already in use."
			message = "\xc0\xcc\xb9\xcc \xbb\xe7\xbf\xeb\xc7\xcf\xb0\xed \xc0\xd6\xb4\xc2 \xb4\xeb\xc8\xad\xb8\xed\xc0\xd4\xb4\xcf\xb4\xd9.";
			break;
		case 3:
			// "The length or format is not valid."
			message = "\xb1\xe6\xc0\xcc \xb6\xc7\xb4\xc2 \xc7\xfc\xbd\xc4\xc0\xcc \xb8\xc2\xc1\xf6 \xbe\xca\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		case 4:
			// "Not enough cookies."
			message = "\xba\xb8\xc0\xaf\xc7\xcf\xb0\xed \xc0\xd6\xb4\xc2 \xc4\xed\xc5\xb0\xb0\xa1 \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.";
			break;
		case 5:
			// "It contains a forbidden word."
			message = "\xbb\xe7\xbf\xeb\xc7\xd2 \xbc\xf6 \xbe\xf8\xb4\xc2 \xb4\xdc\xbe\xee\xb0\xa1 \xc6\xf7\xc7\xd4\xb5\xc7\xbe\xee \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9..";
			break;
		case 6:
			// "The database has a problem right now."
			message = "\xc7\xf6\xc0\xe7 DB\xbf\xa1 \xb9\xae\xc1\xa6\xb0\xa1 \xb9\xdf\xbb\xfd\xc7\xcf\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		case 10:
			// "The nickname differs from the one already checked. Please check again."
			message = "\xb5\xee\xb7\xcf\xc7\xcf\xb7\xc1\xb4\xc2 \xb4\xd0\xb3\xd7\xc0\xd3\xc0\xcc \xc0\xcc\xb9\xcc \xc3\xbc\xc5\xa9\xc7\xd1 \xb4\xd0\xb3\xd7\xc0\xd3\xb0\xfa \xb5\xbf\xc0\xcf\xc7\xcf\xc1\xf6 \xbe\xca\xbd\xc0\xb4\xcf\xb4\xd9. \xb4\xd9\xbd\xc3 \xc8\xae\xc0\xce\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.";
			break;
		default: {
			// "The change failed for an unknown reason. - Code : <result>"
			char code[8] = { 0 };
			message = "\xbe\xcb \xbc\xf6 \xbe\xf8\xb4\xc2 \xc0\xcc\xc0\xaf\xb7\xce \xba\xaf\xb0\xe6\xbf\xa1 \xbd\xc7\xc6\xd0\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9. - Code : ";
			_itoa(result, code, 10);
			message += code;
			break;
		}
		}

		if (result != 0) {
			GetActor("Lobby") << MsgObject(0, 0x23, (intptr_t)message.c_str(), 0, 0, 0, 0);
		}
		break;
	}
	case 0xE: {
		// The result of checking a nickname.
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 2, 0, 0, 0, 0, 0);

		std::string message;
		switch (packet.Decode4()) {
		case 0: {
			std::string nickname;
			packet.DecodeStr(nickname);
			GetActor("Lobby") << MsgObject(0, 0x249, 1, (intptr_t)nickname.c_str(), 0, 0, 0);

			if (IsLocalContent((localContentType_t)0x81)) {
				GetActor("Lobby") << MsgObject(0, 0x6E, 0, 0, 0, 0, 0);
			}
			// "This nickname can be used."
			message = "\xbb\xe7\xbf\xeb \xb0\xa1\xb4\xc9\xc7\xd1 \xb4\xeb\xc8\xad\xb8\xed\xc0\xd4\xb4\xcf\xb4\xd9.";
			break;
		}
		case 1:
			// "An error occurred while checking the nickname."
			message = "\xb4\xeb\xc8\xad\xb8\xed\xc0\xbb \xb0\xcb\xbb\xe7\xc7\xcf\xb4\xc2 \xb0\xfa\xc1\xa4\xbf\xa1\xbc\xad \xbf\xc0\xb7\xf9\xb0\xa1 \xb9\xdf\xbb\xfd\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		case 2:
			message = "\xc0\xcc\xb9\xcc \xbb\xe7\xbf\xeb\xc7\xcf\xb0\xed \xc0\xd6\xb4\xc2 \xb4\xeb\xc8\xad\xb8\xed\xc0\xd4\xb4\xcf\xb4\xd9.";
			break;
		case 3:
			message = "\xb1\xe6\xc0\xcc \xb6\xc7\xb4\xc2 \xc7\xfc\xbd\xc4\xc0\xcc \xb8\xc2\xc1\xf6 \xbe\xca\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		case 4:
			message = "\xba\xb8\xc0\xaf\xc7\xcf\xb0\xed \xc0\xd6\xb4\xc2 \xc4\xed\xc5\xb0\xb0\xa1 \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.";
			break;
		case 5:
			message = "\xbb\xe7\xbf\xeb\xc7\xd2 \xbc\xf6 \xbe\xf8\xb4\xc2 \xb4\xdc\xbe\xee\xb0\xa1 \xc6\xf7\xc7\xd4\xb5\xc7\xbe\xee \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9..";
			break;
		case 6:
			message = "\xc7\xf6\xc0\xe7 DB\xbf\xa1 \xb9\xae\xc1\xa6\xb0\xa1 \xb9\xdf\xbb\xfd\xc7\xcf\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		case 8:
			if (IsLocalContent((localContentType_t)0x81)) {
				GetActor("Lobby") << MsgObject(0, 0x6E, 0, 8, 0, 0, 0);
				// "The same nickname is used as it is."
				message = "\xb5\xbf\xc0\xcf\xc7\xd1 \xb4\xd0\xb3\xd7\xc0\xd3\xc0\xbb \xb1\xd7\xb4\xeb\xb7\xce \xbb\xe7\xbf\xeb\xc7\xd5\xb4\xcf\xb4\xd9.";
			}
			break;
		}

		GetActor("Lobby") << MsgObject(0, 0x23, (intptr_t)message.c_str(), 0, 0, 0, 0);
		break;
	}
	case 0xD2: {
		// Up to twelve 0x19-byte records, handed to the lobby as one block.
		char records[12][0x19];
		memset(records, 0, sizeof(records));
		int count = packet.Decode4();

		for (int i = 0; i < count; i++) {
			packet.DecodeBuffer(records[i], 0x19);
		}

		if (count > 0) {
			GetActor("Lobby") << MsgObject(0, 0x61, count, (intptr_t)records, 0, 0, 0);
		}
		break;
	}
	case 0x76: {
		unsigned long guid = packet.Decode4();
		unsigned char value = packet.Decode1();

		GetActor("Lobby") << MsgObject(0, 0x29, guid, value, 0, 0, 0);
		break;
	}
	case 0x6A:
		// A mass game player's state changed (2: finished, 3: skipped).
		if (IsMassGame(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType)) {
			unsigned long uid = packet.Decode4();
			unsigned char state = packet.Decode1();

			if (WSingleton<CSharedDoc>::Instance()->AreWePlayingTogether(uid)) {
				std::list<sSlotInfo> &players = (std::list<sSlotInfo> &)WSingleton<CSharedDoc>::Instance()->m_roomPlayerList;
				std::list<sSlotInfo>::iterator it;

				switch (state) {
				case 3:
					for (std::vector<sGameResultInfo>::iterator rival = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin(); rival != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++rival) {
						if (rival->m_state != 3) {
							WSingleton<CSharedDoc>::Instance()->m_gameResultList[WSingleton<CSharedDoc>::Instance()->GetIndex(uid)].m_sortGroup--;
						}
					}

					it = std::find(players.begin(), players.end(), uid);

					if (it != players.end()) {
						if (WSingleton<CSharedDoc>::Instance()->m_gameResultList[WSingleton<CSharedDoc>::Instance()->GetIndex(uid)].m_state == 2) {
							GetActor("Lobby") << MsgObject(0, 8, (intptr_t)&*it, 0, 0, 0, 0);
						}
					}
					break;
				case 2:
					it = std::find(players.begin(), players.end(), uid);
					if (it != players.end()) {
						GetActor("Lobby") << MsgObject(0, 7, (intptr_t)&*it, 0, 0, 0, 0);
					}
					break;
				}

				WSingleton<CSharedDoc>::Instance()->m_gameResultList[WSingleton<CSharedDoc>::Instance()->GetIndex(uid)].m_state = state;
				GetActor("Lobby") << MsgObject(0, 0x2A, 0, 0, 0, 0, 0);
			}
		}
		break;
	case 0x6C:
		// A mass game player's ball position (and, in approach games, the
		// distance and points).
		if (IsMassGame()) {
			unsigned long uid = packet.Decode4();
			packet.Decode1();
			float pos[2];
			packet.DecodeBuffer(pos, 8);

			unsigned int distance;
			int point;
			if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 10) {
				distance = packet.Decode4();
				point = packet.Decode4();
			}

			packet.Decode2();

			if (WSingleton<CSharedDoc>::Instance()->AreWePlayingTogether(uid)) {
				unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(uid);
				std::vector<sGameResultInfo> &rivals = WSingleton<CSharedDoc>::Instance()->m_gameResultList;

				if (!WisEqual(rivals[index].m_ballPos.x, pos[0], 0.00001f) && !WisEqual(rivals[index].m_ballPos.z, pos[1], 0.00001f)) {
					rivals[index].m_ballPos = WVector(pos[0], 0.0f, pos[1]);

					if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 10) {
						WSingleton<CSharedDoc>::Instance()->m_gameResultList[index].m_approachDistanceCopy = distance;
						WSingleton<CSharedDoc>::Instance()->m_gameResultList[index].m_approachDistance = distance;
						WSingleton<CSharedDoc>::Instance()->m_gameResultList[index].m_approachPoint = point;
					}
				}
			}
		}
		break;
	case 0x6B:
		// A mass game player finished a hole.
		if (IsMassGame()) {
			unsigned long uid = packet.Decode4();
			unsigned char hole = packet.Decode1();
			unsigned char shots = packet.Decode1();
			int score = packet.Decode4();

			__int64 pang = packet.Decode8();
			__int64 extraPang = packet.Decode8();

			int finished = packet.Decode1();

			if (WSingleton<CSharedDoc>::Instance()->AreWePlayingTogether(uid)) {
				WSingleton<CSharedDoc>::Instance()->LogGstUpdateHole(uid, hole, score, pang);
				std::vector<sGameResultInfo>::iterator end = WSingleton<CSharedDoc>::Instance()->m_gameResultList.end();
				std::vector<sGameResultInfo>::iterator it = std::find_if(WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin(), end, sFindRival(uid));

				if (it == end) {
					break;
				}

				if (hole != it->m_hole) {
					break;
				}

				if (finished == 1) {
					unsigned char holeShots = shots - it->m_totalShots;
					it->m_holeShots[hole - 1] = holeShots;
					it->m_holeScore[hole - 1] = score - it->m_score;
					it->m_holePang[hole - 1] = pang - it->m_pang;

					it->m_score = score;
					it->m_totalShots = shots;
					it->m_hole = WSingleton<CSharedDoc>::Instance()->m_holeOrder[GetHoleIndex(hole)];
				}

				it->m_pang = pang;
				it->m_extraPang = extraPang;

				if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
					GetActor("Lobby") << MsgObject(0, 0x5E, uid, hole, 0, 0, 0);
				}

				GetActor("Lobby") << MsgObject(0, 0x2A, 0, 0, 0, 0, 0);
			}
		}
		break;
	case 0x77: {
		// The finished game's results, taken once.
		if (WSingleton<CSharedDoc>::Instance()->m_gameStatistics[0].exp != 0xFFFFFFFF) {
			break;
		}

		WSingleton<CSharedDoc>::Instance()->m_gameStatistics[0].exp = packet.Decode4();
		WSingleton<CSharedDoc>::Instance()->m_matchTypeId = packet.Decode4();
		WSingleton<CSharedDoc>::Instance()->m_value456C = packet.Decode1();
		WSingleton<CSharedDoc>::Instance()->m_winningTeam = packet.Decode1();

		if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 6) {
			WSingleton<CSharedDoc>::Instance()->m_value5018 = packet.Decode4();
			WSingleton<CSharedDoc>::Instance()->m_matchGameCount = packet.Decode4();

			WSingleton<CSharedDoc>::Instance()->m_value47B4[0] = packet.Decode4();
			WSingleton<CSharedDoc>::Instance()->m_value47B4[1] = packet.Decode4();
		}

		packet.DecodeBuffer(WSingleton<CSharedDoc>::Instance()->m_awardPlayers, 0x60);

		GetActor("Lobby") << MsgObject(0, 0x2D, 0, 0, 0, 0, 0);
		break;
	}
	case 0x7A: {
		// The room's master changed: attribute bit 3 marks the master.
		std::list<sSlotInfo> &players = (std::list<sSlotInfo> &)WSingleton<CSharedDoc>::Instance()->m_roomPlayerList;

		unsigned long uid = packet.Decode4();
		packet.Decode2();

		for (std::list<sSlotInfo>::iterator it = players.begin(); it != players.end(); ++it) {
			if (it->m_uid == uid) {
				it->m_attribute = (it->m_attribute & ~0x10) | 0x08;
				it->m_flags55.flag1 = 1;
			} else {
				it->m_attribute &= ~0x18;
			}
		}

		GetActor("Lobby") << MsgObject(0, 0x0F, 0, 0, 0, 0, 0);
		GetActor("Lobby") << MsgObject(0, 0x5F, uid, 0, 0, 0, 0);
		break;
	}
	case 0x7B: {
		// A player changed team (the low two attribute bits).
		std::list<sSlotInfo> &players = (std::list<sSlotInfo> &)WSingleton<CSharedDoc>::Instance()->m_roomPlayerList;

		unsigned long uid = packet.Decode4();
		unsigned char team = packet.Decode1();

		std::list<sSlotInfo>::iterator it = std::find(players.begin(), players.end(), uid);
		if (it == players.end()) {
			break;
		}

		it->m_attribute = (it->m_attribute & ~3) | (team & 3);

		GetActor("Lobby") << MsgObject(0, 0x46, uid, team, 0, 0, 0);
		GetActor("Lobby") << MsgObject(0, 0x0F, 0, 0, 0, 0, 0);
		break;
	}
	case 0x7D: {
		// The game could not start; the reason code picks the message.
		unsigned char reason = packet.Decode1();

		std::string message;

		switch (reason) {
		case 1:
			// "Not enough players to start the game."
			message = "\xb0\xd4\xc0\xd3\xc0\xbb \xbd\xc3\xc0\xdb\xc7\xcf\xb1\xe2 \xc0\xa7\xc7\xd8 \xc7\xca\xbf\xe4\xc7\xd1 \xc0\xaf\xc0\xfa\xc0\xc7 \xbc\xf6\xb0\xa1 \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.";
			break;
		case 2:
			// "The teams do not have the same number of players."
			message = "\xc6\xc0\xc0\xc7 \xc0\xce\xbf\xf8\xbc\xf6\xb0\xa1 \xbc\xad\xb7\xce \xb8\xc2\xc1\xf6 \xbe\xca\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		case 3:
			// "Not enough Ready players to start the game."
			message = "\xb0\xd4\xc0\xd3\xc0\xbb \xbd\xc3\xc0\xdb\xc7\xcf\xb1\xe2 \xc0\xa7\xc7\xd8 \xc7\xca\xbf\xe4\xc7\xd1 Ready\xb5\xc8 \xc0\xaf\xc0\xfa\xc0\xc7 \xbc\xf6\xb0\xa1 \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.";
			break;
		case 4:
			// "Too many players to start the game."
			message = "\xb0\xd4\xc0\xd3\xc0\xbb \xbd\xc3\xc0\xdb\xc7\xcf\xb1\xe2\xbf\xa1 \xc0\xce\xbf\xf8\xc0\xcc \xb3\xca\xb9\xab \xb8\xb9\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		case 6:
			// "No player has been chosen for the first gallery."
			message = "\xc3\xb3\xc0\xbd\xbf\xa1 \xb0\xb6\xb7\xaf\xb8\xae\xc7\xd2 \xc0\xaf\xc0\xfa\xb0\xa1 \xbc\xb1\xc5\xc3\xb5\xc7\xbe\xee \xc0\xd6\xc1\xf6 \xbe\xca\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		case 7:
		case 8:
			// "Failed to start the game."
			message = "\xb0\xd4\xc0\xd3 \xbd\xba\xc5\xb8\xc6\xae\xbf\xa1 \xbd\xc7\xc6\xd0\xc7\xdf\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		case 9:
			// "An update to the latest version is required."
			message = "\xc3\xd6\xbd\xc5 \xb9\xf6\xc1\xaf\xc0\xb8\xb7\xce \xbe\xf7\xb5\xa5\xc0\xcc\xc6\xae\xb0\xa1 \xc7\xca\xbf\xe4\xc7\xd5\xb4\xcf\xb4\xd9.";
			break;
		case 12:
			// "You cannot play any more match games with the other player."
			message = "\xbb\xf3\xb4\xeb \xc7\xc3\xb7\xb9\xc0\xcc\xbe\xee\xbf\xcd \xb8\xc5\xc4\xa1 \xc7\xc3\xb7\xb9\xc0\xcc\xb8\xa6 \xb4\xf5 \xc0\xcc\xbb\xf3 \xc7\xd2\xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		case 13:
			// "A player lacks the pang needed to start the game."
			message = "\xb0\xd4\xc0\xd3\xc0\xbb \xbd\xc3\xc0\xdb\xc7\xcf\xb1\xe2 \xc0\xa7\xc7\xd8\xbc\xad \xc7\xca\xbf\xe4\xc7\xd1 \xc6\xce\xc0\xcc \xba\xce\xc1\xb7\xc7\xd1 \xc0\xaf\xc0\xfa\xb0\xa1 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.";
			break;
		}

		std::list<sSlotInfo> &players = (std::list<sSlotInfo> &)WSingleton<CSharedDoc>::Instance()->m_roomPlayerList;
		std::list<sSlotInfo>::iterator it = std::find(players.begin(), players.end(), WSingleton<CSharedDoc>::Instance()->m_userInfo.guid);
		if (it == players.end()) {
			break;
		}

		if (it->m_attribute & 0x18) {
			// We are the room's master: show the reason and refresh.
			GetActor("Lobby") << MsgObject(0, 0x23, (intptr_t)message.c_str(), 0, 0, 0, 0);
			GetActor("Lobby") << MsgObject(0, 0x0F, 0, 0, 0, 0, 0);
		} else if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 6) {
			GetActor("Lobby") << MsgObject(0, 0x23, (intptr_t)message.c_str(), 0, 0, 0, 0);
		} else {
			GetActor("Lobby") << MsgObject(0, 0x2B, 0, 0, 0, 0, 0);
		}
		break;
	}
	case 0x7F: {
		std::string nickName;
		packet.DecodeStr(nickName);
		// "<nick>: you are about to force this player out.\nDo you agree?"
		nickName += "\xb4\xd4\xc0\xbb \xb0\xad\xc1\xa6 \xc5\xf0\xc0\xe5\xbd\xc3\xc5\xb0\xb7\xc1 \xc7\xd5\xb4\xcf\xb4\xd9.\n\xc2\xf9\xbc\xba\xc7\xcf\xbd\xca\xb4\xcf\xb1\xee?";
		GetActor("Lobby") << MsgObject(0, 0x31, (intptr_t)nickName.c_str(), packet.Decode4(), 0, 0, 0);
		break;
	}
	case 0x35:
	case 0x81:
		// Direct move to a user's room (on another server).
		if (IsLocalContent((localContentType_t)0x54)) {
			unsigned short result = packet.Decode2();

			if (result != 0) {
				break;
			}

			unsigned long uid = packet.Decode4();
			unsigned char server = packet.Decode1();
			unsigned short room = packet.Decode2();

			unsigned long roomKey = packet.Decode4();
			std::string password = packet.DecodeStr();
			unsigned long moveKey = packet.Decode4();

			WSingleton<CSharedDoc>::Instance()->SetDirectMoveRoomInfo(server, room, uid);

			WSendPacket send(0x24);
			send.Encode4(moveKey);
			send.Send(SEND_TO_MSN);

			GetActor("Lobby") << MsgObject(0, 0x30, uid, server, room, roomKey, (intptr_t)&password);
		} else {
			unsigned short room = packet.Decode2();
			unsigned long roomKey = packet.Decode4();
			GetActor("Lobby") << MsgObject(0, 0x30, room, roomKey, 0, 0, 0);
		}
		break;
	case 0x84: {
		// The detailed info of a room: its game settings and its players.
		int count = packet.Decode1();
		if (count) {
			sRoomGameDesc desc;
			packet.DecodeBuffer(&desc, sizeof(desc));

			WSingleton<CSharedDoc>::Instance()->m_gameDesc = desc;

			std::list<sRoomUserInfo>& userList = WSingleton<CSharedDoc>::Instance()->m_roomUserList;
			userList.clear();
			for (int i = 0; i < count; i++) {
				sRoomUserInfo info;
				memset(&info, 0, sizeof(info));
				packet.DecodeBuffer(&info, sizeof(info));
				userList.push_back(info);
			}

			GetActor("Lobby") << MsgObject(0, 0x33, 0, 0, 0, 0, 0);
		} else {
			GetActor("Lobby") << MsgObject(0, 0x33, 1, 0, 0, 0, 0);
		}
		break;
	}
	case 0x86: {
		unsigned long count = packet.Decode4();

		for (unsigned long i = 0; i < count; i++) {
			unsigned long uid = packet.Decode4();
			unsigned short roomNumber = packet.Decode2();

			std::map<unsigned long, sBriefUserInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_userMap.find(uid);
			if (it != WSingleton<CSharedDoc>::Instance()->m_userMap.end()) {
				it->second.roomNumber = roomNumber;
			}
		}
		break;
	}
	case 0x8A: {
		// The game ended: every player still playing gives up the rest of
		// the course, each remaining hole scored par + 5.
		unsigned char finishOrder = (unsigned char)WSingleton<CSharedDoc>::Instance()->m_gameResultList.size();
		for (std::vector<sGameResultInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin();
			it != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++it) {
			if (it->m_state == 0) {
				it->m_finishOrder = finishOrder--;
				it->m_state = 2;

				for (;; it->m_hole = WSingleton<CSharedDoc>::Instance()->m_holeOrder[GetHoleIndex(it->m_hole)]) {
					it->m_holePang[it->m_hole - 1] = 0;
					it->m_holeShots[it->m_hole - 1] = WSingleton<CSharedDoc>::Instance()->m_courseHoleOn[it->m_hole - 1] + 5;
					it->m_holeScore[it->m_hole - 1] = 5;
					it->m_score += 5;

					if (it->m_hole == GetEndHole()) {
						break;
					}
				}
			}
		}

		GetActor("Lobby") << MsgObject(0, 0x2A, 0, 0, 0, 0, 0);
		break;
	}
	case 0x8C: {
		unsigned long guid = packet.Decode4();
		unsigned char value = packet.Decode1();

		GetActor("Lobby") << MsgObject(0, 0x3D, guid, value, 0, 0, 0);
		break;
	}
	case 0x94:
		// The bonus cash; a pending message moves to the shop.
		WSingleton<CSharedDoc>::Instance()->m_myBonusCash = packet.Decode8();
		WSingleton<CSharedDoc>::Instance()->RefreshAllCookie();

		GetActor("Lobby") << MsgObject(0, 2, 0, 0, 0, 0, 0);

		if (*WSingleton<CSharedDoc>::Instance()->m_lobbyMessage) {
			WSingleton<CMouseCursor>::Instance()->SetActive(false);

			if (WSingleton<CUserInfo>::Instance()) {
				WSingleton<CUserInfo>::Instance()->Close();
			}

			WSingleton<CTaskManager>::Instance()->ChangeTask("CShopTask", "", false);
			AfxPostMsg(0, "Shop", 0, (intptr_t)WSingleton<CSharedDoc>::Instance()->m_lobbyMessage, 0, 0, 0);
		}
		break;
	case 0x96:
		// Gallery mode: our own player's info.
		if (GalleryMode()) {
			packet.DecodeBuffer(&WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0], sizeof(sUserInfo));
			WSingleton<CSharedDoc>::Instance()->m_userInfo.galleryGuid = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0].m_guid;

			if (IsLocalContent((localContentType_t)0x57)) {
				RequestUccInfo(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0]);
			}
		}
		break;
	case 0x95:
		WSingleton<CSharedDoc>::Instance()->m_matchTypeId = packet.Decode4();
		break;
	case 0xBA:
		// TODO lobbytask.cpp:2274-2356 (824 B): WReceivedPacket::Decode1, std::string, CChatMsg::AddChatMsg, WReceivedPacket::DecodeBuffer, WReceivedPacket::Decode4, clear, sSlotInfo ctor, push_back, IsMassGame, IsLocalContent, RequestUccInfo
		break;
	case 0xBD: {
		// The guild match pairings: the one with our player sets the trophies.
		unsigned char count = packet.Decode1();
		for (int i = 0; i < count; i++) {
			sGuildMatchup matchup;
			packet.DecodeBuffer(&matchup, sizeof(matchup));

			unsigned long myGuid = WSingleton<CSharedDoc>::Instance()->m_userInfo.guid;

			if (myGuid == matchup.m_guidA) {
				WSingleton<CSharedDoc>::Instance()->m_trophyUID[0] = matchup.m_guidA;
				WSingleton<CSharedDoc>::Instance()->m_trophyUID[1] = matchup.m_guidB;
				WSingleton<CSharedDoc>::Instance()->m_flag5020 = matchup.m_kind;
			} else if (myGuid == matchup.m_guidB) {
				WSingleton<CSharedDoc>::Instance()->m_trophyUID[0] = matchup.m_guidB;
				WSingleton<CSharedDoc>::Instance()->m_trophyUID[1] = matchup.m_guidA;
				WSingleton<CSharedDoc>::Instance()->m_flag5020 = matchup.m_kind;
			}

			WSingleton<CSharedDoc>::Instance()->m_vector4524.push_back(matchup);
		}
		break;
	}
	case 0xBE:
		packet.DecodeBuffer(WSingleton<CSharedDoc>::Instance()->m_block4540, sizeof(WSingleton<CSharedDoc>::Instance()->m_block4540));
		WSingleton<CUserInfo>::Instance()->Refresh();
		break;
	case 0xBF: {
		// The guild match flags of the guild of the player CUserInfo shows.
		unsigned long guildUID = packet.Decode4();

		std::map<unsigned long, sBriefUserInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_userMap.find(WSingleton<CUserInfo>::Instance()->m_guid);
		if (it != WSingleton<CSharedDoc>::Instance()->m_userMap.end()) {
			if (guildUID == it->second.guildUID) {
				unsigned short count = packet.Decode2();
				int n = count > 10 ? 10 : count;

				for (int i = 0; i < n; i++) {
					sGuildMatchFlagInfo info;
					packet.DecodeBuffer(&info, 0x2E);
					WSingleton<CSharedDoc>::Instance()->m_guildMatchFlags.push_back(info);
				}

				WSingleton<CUserInfo>::Instance()->Refresh();
			}
		}
		break;
	}
	case 0xC0:
		// TODO lobbytask.cpp:2441-2469 (271 B): WReceivedPacket::Decode4, WReceivedPacket::Decode2, WReceivedPacket::Decode1, CSharedDoc::GetIndex, ??A?$vector@UsRivalData@@V?$allocator@UsRivalData@@@std@@@std@@QAEAAUsRivalData@@I@Z, CSharedDoc::GetRival, MsgObject, CTask::GetActor, operator<<(IActor*)
		break;
	case 0xC1:
		GetActor("Lobby") << MsgObject(0, 0x2B, 1, 0, 0, 0, 0);
		GetActor("Lobby") << MsgObject(0, 0x58, 0, 0, 0, 0, 0);
		break;
	case 0xCD: {
		// The player's quests.
		unsigned short count = packet.Decode2();
		WSingleton<CSharedDoc>::Instance()->m_questList.clear();

		for (int i = 0; i < count; i++) {
			sQuest quest;
			quest.m_typeId = packet.Decode4();
			quest.m_state = packet.Decode1();
			quest.m_iff = WSingleton<CSharedDoc>::Instance()->m_itemManager.FindQuest(quest.m_typeId);
			quest.m_flagC = false;

			WSingleton<CSharedDoc>::Instance()->m_questList.push_back(quest);
		}
		break;
	}
	case 0x117:
		// TODO lobbytask.cpp:2501-2513 (103 B): IsLocalContent, WReceivedPacket::Decode4, CSharedDoc::AreWePlayingTogether, CSharedDoc::GetIndex, ??A?$vector@UsRivalData@@V?$allocator@UsRivalData@@@std@@@std@@QAEAAUsRivalData@@I@Z
		break;
	case 0x127:
		// TODO lobbytask.cpp:2514-2617 (672 B): IsLocalContent, MsgObject, CTask::GetActor, operator<<(IActor*), WReceivedPacket::Decode2, WReceivedPacket::Decode4, WReceivedPacket::Decode1, WReceivedPacket::DecodeStr, WSendPacket ctor, WSendPacket::Encode4, WNetworkSystem::IsConnected
		break;
	case 0x128:
		// TODO lobbytask.cpp:2618-2684 (380 B): IsLocalContent, WReceivedPacket::Decode4, erase, MsgObject, CTask::GetActor, operator<<(IActor*)
		break;
	case 0x132:
		if (IsLocalContent((localContentType_t)0x59)) {
			unsigned long value = 0;
			value = packet.Decode4();
			std::string name = packet.DecodeStr();
			unsigned char kind = packet.Decode1();

			GetActor("Lobby") << MsgObject(0, 0x21C, (intptr_t)&name, (intptr_t)&kind, (intptr_t)&value, 0, 0);
		}
		break;
	case 0x134:
		if (IsLocalContent((localContentType_t)0x59)) {
			packet.Decode1();
			packet.Decode1();
		}
		break;
	case 0x133:
		// TODO lobbytask.cpp:2713-2760 (298 B): IsLocalContent, WReceivedPacket::Decode1, MakeStr
		break;
	case 0x135:
		// TODO lobbytask.cpp:2761-2801 (363 B): IsLocalContent, WReceivedPacket::Decode4, WReceivedPacket::Decode1, WReceivedPacket::Decode2, WSendPacket ctor, WSendPacket::Encode1, WSendPacket::Encode2, WSendPacket::Send, dtor, Fresh::IsCurrentLayout, MsgObject, AfxGetTask
		break;
	case 0x136:
		if (IsLocalContent((localContentType_t)0x59)) {
			// "The match was cancelled. Please try again shortly."
			GetActor("Lobby") << MsgObject(0, 0x21D, 0, (intptr_t)MakeStr("\xb8\xc5\xc4\xaa\xc0\xcc \xc3\xeb\xbc\xd2\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9. \xc0\xe1\xbd\xc3\xc8\xc4 \xb4\xd9\xbd\xc3 \xbd\xc3\xb5\xb5\xc7\xd8 \xc1\xd6\xbd\xc3\xb1\xe2 \xb9\xd9\xb6\xf8\xb4\xcf\xb4\xd9."), 0, 0, 0);
		}
		break;
	case 0x137: {
		CMatchingSystem *pMatching = WSingleton<CMatchingSystem>::Instance();
		if (pMatching) {
			unsigned char count = packet.Decode1();

			pMatching->Clear();
			for (int i = 0; i < count; i++) {
				unsigned char code = packet.Decode1();
				pMatching->SetMyIntrest(code);
			}
		}
		break;
	}
	case 0x180:
		if (IsLocalContent((localContentType_t)0x72)) {
			unsigned short result = packet.Decode2();

			if (result == 1) {
				GetActor("Lobby") << MsgObject(0, 0x240, 0, 0, 0, 0, 0);
			}
		}
		break;
	case 0x182:
		WSingleton<CSharedDoc>::Instance()->m_userInfo.flagBlock = packet.Decode4();
		break;
	case 0x18C:
		// TODO lobbytask.cpp:2848-2925 (550 B): IsLocalContent, WReceivedPacket::Decode1, WReceivedPacket::Decode4, find, AfxGetTask, MsgObject
		break;
	case 0x18E:
		// TODO lobbytask.cpp:2926-2946 (136 B): IsLocalContent, ??$GetContainer@PAVCMissionEvent@@@CContentsDoc@@QAE_NW4localContentType_t@@AAPAVCMissionEvent@@@Z, CMissionEvent::DecodeMissionInfo, AfxGetTask, MsgObject
		break;
	case 0x18F:
		// TODO lobbytask.cpp:2947-2959 (113 B): IsLocalContent, WReceivedPacket::Decode4, AfxGetTask, MsgObject
		break;
	case 0x190:
		// TODO lobbytask.cpp:2960-3005 (346 B): AfxGetTask, MsgObject, IsLocalContent, WReceivedPacket::Decode4, _wsprintfA, WReceivedPacket::DecodeBuffer
		break;
	case 0x191:
		// TODO lobbytask.cpp:3006-3029 (197 B): IsLocalContent, WReceivedPacket::Decode4, push_back, AfxGetTask, MsgObject, dtor
		break;
	case 0x19F:
		// TODO lobbytask.cpp:3030-3073 (471 B): AfxGetTask, MsgObject, WReceivedPacket::Decode4, _wsprintfA, push_back, dtor
		break;
	case 0x1A0:
		// TODO lobbytask.cpp:3074-3198 (252 B): AfxGetTask, MsgObject, WReceivedPacket::Decode4, _wsprintfA
		break;
	case 0x19D:
		// TODO lobbytask.cpp:3199-3429 (159 B): WReceivedPacket::Decode4, CItemManager::GetItemName, _sprintf, AfxGetTask, MsgObject
		break;
	case 0x1B6: {
		// Our guild's info.
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		GUILD_INFO info;
		packet.DecodeBuffer(&info, 0x129);

		IActor *pActor = GetActor(data_00ab2c7c);

		pActor << MsgObject(0, 0x263, 4, (intptr_t)&info, 0, 0, 0);
		break;
	}
	case 0x1B7: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
		} else {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, 0x403, 0, 0, 0);
		}
		break;
	}
	case 0x1B8: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
		} else {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, 0x402, 0, 0, 0);
		}
		break;
	}
	case 0x1BC: {
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		std::list<GUILD_HISTORY> history;

		int entryCount = packet.Decode2();
		for (int i = 0; i < entryCount; i++) {
			GUILD_HISTORY entry;
			packet.DecodeBuffer(&entry, 0x31);
			history.push_back(entry);
		}

		IActor *pActor = GetActor(data_00ab2c7c);

		pActor << MsgObject(0, 0x263, 9, (intptr_t)&history, 0, 0, 0);
		break;
	}
	case 0x1BD: {
		// A guild member's info.
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		GUILD_USER_INFO info;
		packet.DecodeBuffer(&info, 0x119);

		GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 5, (intptr_t)&info, 0, 0, 0);
		break;
	}
	case 0x1C4: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		// Two list parameters the guild dialog receives as they are.
		unsigned long listParam1 = packet.Decode4();
		unsigned long listParam2 = packet.Decode4();
		int memberCount = packet.Decode2();
		std::list<GUILD_USER_LIST> members;

		for (int i = 0; i < memberCount; i++) {
			GUILD_USER_LIST member;
			packet.DecodeBuffer(&member, 0x51);
			members.push_back(member);
		}

		if (memberCount != 0) {
			IActor *pActor = GetActor(data_00ab2c7c);

			pActor << MsgObject(0, 0x263, 7, (intptr_t)&members, listParam1, listParam2, 0);
		}
		break;
	}
	case 0x1BF: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		IActor *pActor = GetActor(data_00ab2c7c);

		pActor << MsgObject(0, 0x263, 0xA, 0, 0, 0, 0);
		break;
	}
	case 0x1C3: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		IActor *pActor = GetActor(data_00ab2c7c);

		pActor << MsgObject(0, 0x263, 0x12, 0, 0, 0, 0);
		break;
	}
	case 0x1BA:
	case 0x1BB: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		// Two list parameters the guild dialog receives as they are.
		unsigned long listParam1 = packet.Decode4();
		unsigned long listParam2 = packet.Decode4();
		int guildCount = packet.Decode2();

		std::list<GUILD_LIST> guilds;
		for (int i = 0; i < guildCount; i++) {
			GUILD_LIST guild;
			packet.DecodeBuffer(&guild, 0xC4);

			guilds.push_back(guild);
		}

		GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 8, (intptr_t)&guilds, listParam1, listParam2, 0);
		break;
	}
	case 0x1C6: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		IActor *pActor = GetActor(data_00ab2c7c);

		pActor << MsgObject(0, 0x263, 0xD, 0, 0, 0, 0);
		break;
	}
	case 0x1BE: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x11, result, 0, 0, 0);
		} else {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0xE, 0, 0, 0, 0);
		}
		break;
	}
	case 0x1C0: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0xF, 0, 0, 0, 0);
		break;
	}
	case 0x1C1: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x10, 0, 0, 0, 0);
		break;
	}
	case 0x1C2: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		unsigned long value = packet.Decode4();

		IActor *pActor = GetActor(data_00ab2c7c);
		pActor << MsgObject(0, 0x263, 0x13, value, 0, 0, 0);
		break;
	}
	case 0x1C5: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		IActor *pActor = GetActor(data_00ab2c7c);

		pActor << MsgObject(0, 0x263, 0xC, 0, 0, 0, 0);
		break;
	}
	case 0x1B9: {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		unsigned long result = packet.Decode4();
		if (result != 1) {
			GetActor(data_00ab2c7c) << MsgObject(0, 0x263, 0x1A, result, 0, 0, 0);
			break;
		}

		IActor *pActor = GetActor(data_00ab2c7c);

		pActor << MsgObject(0, 0x263, 0xB, 0, 0, 0, 0);
		break;
	}
	default:
		return OnPacketCommon(packet);
	}

	return 1;
}

// Instantiates the singleton accessors of TicketInfo this unit emits.
template TicketInfo* WSingleton<TicketInfo>::Instance();
template bool WSingleton<TicketInfo>::IsInstantiated();
