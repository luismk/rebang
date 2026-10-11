// Original: d:\build\custom\temp\code\client\projectg\gmtoolkit.cpp
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <string>

#include "GMToolkit.h"
#include "ChatMsg.h"
#include "commonutil.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "ItemManager.h"
#include "UccManager.h"

// The GM command table (sGMCommand entries, filled by a dynamic initializer).
extern "C" sGMCommand data_aa53d8[];

// gmtoolkit.h:168
bool CGMToolkit::GetPrizeSwitch(void) const {
	return m_prizeSwitch;
}

// gmtoolkit.h:180
eGMCommandTag CGMToolkit::GetConfirmWaitTag(void) const {
	return m_confirmWaitTag;
}

// gmtoolkit.h:184
eGMToolkitStatus CGMToolkit::GetToolKitStatus(void) const {
	return m_toolkitStatus;
}

// gmtoolkit.h:188
void CGMToolkit::SetConfirmWaitTag(eGMCommandTag tag) {
	m_confirmWaitTag = tag;
	m_toolkitStatus = GM_TOOLKIT_CONFIRM_WAIT;
}

// gmtoolkit.h:193
void CGMToolkit::SetToolKitStatus(eGMToolkitStatus status) {
	m_toolkitStatus = status;
}

// gmtoolkit.cpp:75
CGMToolkit::CGMToolkit(void) {
	m_isGM = false;
	m_statusFlags = 0;
	m_arguments = 0;
	m_argumentCount = 0;
	m_toolkitStatus = GM_TOOLKIT_NORMAL;
	m_confirmWaitTag = (eGMCommandTag)0;
	m_confirmPacket = 0;

	memset(m_clearedBuffer0, 0, sizeof(m_clearedBuffer0));
	memset(m_clearedBuffer1, 0, sizeof(m_clearedBuffer1));

	m_prizeList.clear();

	m_pendingPrize = 0;
	m_prizeSwitch = false;
}

// gmtoolkit.cpp:96
CGMToolkit::~CGMToolkit(void) {
	ReleaseStringBuffer();

	if (m_confirmPacket) {
		delete m_confirmPacket;
		m_confirmPacket = 0;
	}

	if (m_pendingPrize) {
		delete m_pendingPrize;
		m_pendingPrize = 0;
	}

	for (std::list<sPrize *>::iterator it = m_prizeList.begin(); it != m_prizeList.end(); ++it) {
		if (*it) {
			delete *it;
			*it = 0;
		}
	}

	m_prizeList.clear();
}

// gmtoolkit.cpp:130
bool CGMToolkit::GetStatus(eGMStatusFlag flag) {
	return (m_statusFlags & flag) ? true : false;
}

// gmtoolkit.cpp:147
void CGMToolkit::SetStatus(eGMStatusFlag flag, eGMStatusSwitch statusSwitch) {
	if (statusSwitch == GM_STATUS_ON) {
		m_statusFlags |= flag;
	} else if (statusSwitch == GM_STATUS_OFF) {
		m_statusFlags &= ~flag;
	}
}

// gmtoolkit.cpp:161
void CGMToolkit::ReleaseStringBuffer(void) {
	for (int i = 0; i < m_argumentCount; i++) {
		if (m_arguments[i]) {
			delete[] m_arguments[i];
			m_arguments[i] = 0;
		}
	}

	if (m_arguments) {
		delete[] m_arguments;
		m_arguments = 0;
	}

	m_argumentCount = 0;
}

// gmtoolkit.cpp:184
bool CGMToolkit::PhraseSaperator(const char *command) {
	bool result = false;
	ReleaseStringBuffer();

	int length = strlen(command);

	if (command == 0) {
		return result;
	}

	if (command[0] == '/') {
		for (int i = 0; command[i] != '\0'; i++) {
			if (command[i] == '/') {
				if (i != 0) {
					m_argumentCount = 0;
					break;
				}
			} else {
				m_argumentCount = 1;
			}
		}
	}

	if (m_argumentCount <= 0) {
		return result;
	}

	for (int i = 0; command[i] != '\0'; i++) {
		if (command[i] == ' ' && i < length) {
			if (command[i + 1] != ' ' && command[i + 1] != '\0') {
				m_argumentCount++;
			}
		}
	}

	m_arguments = new char *[m_argumentCount];
	for (int i = 0; i < m_argumentCount; i++) {
		m_arguments[i] = 0;
	}

	int remaining = m_argumentCount;
	char word[128] = { 0 };
	int position = 0;
	bool inWord = false;

	for (int i = 1; command[i] != '\0'; i++) {
		if (command[i] != ' ' && command[i] != '\0') {
			word[position] = command[i];
			position++;
			inWord = true;
		}

		if ((command[i] == ' ' || command[i + 1] == '\0') && inWord == true) {
			if (remaining > 0) {
				int wordLength = strlen(word) + 1;

				m_arguments[m_argumentCount - remaining] = new char[wordLength];
				memset(m_arguments[m_argumentCount - remaining], 0, wordLength);

				strcpy(m_arguments[m_argumentCount - remaining], word);

				memset(word, 0, 64);

				remaining--;
				position = 0;
				inWord = false;
			}
		}
	}

	result = true;
	return result;
}

// gmtoolkit.cpp:276
int CGMToolkit::GetGMCommandTableNumber(void) {
	for (int i = 0; i < 31; i++) {
		if (strcmp(m_arguments[0], data_aa53d8[i].keyword) == 0) {
			return i;
		}
	}

	return 0;
}

// gmtoolkit.cpp:299
eGMToolResult CGMToolkit::Parser(void) {
	eGMToolkitStatus status = GetToolKitStatus();
	eGMCommandTag tag = GM_COMMAND_NONE;
	int count = 0;
	eGMToolResult result = GM_RESULT_SUCCEEDED;

	if (status == GM_TOOLKIT_NORMAL) {
		count = 31;
		int index = GetGMCommandTableNumber();
		tag = data_aa53d8[index].tag;

		if (data_aa53d8[index].enabled == false && tag != GM_COMMAND_NONE) {
			Display("\xc7\xf6\xc0\xe7 \xbb\xe7\xbf\xeb \xc7\xd2 \xbc\xf6 \xbe\xf8\xb4\xc2 \xb8\xed\xb7\xc9\xbe\xee \xc0\xd4\xb4\xcf\xb4\xd9.", 0xFF7802);
			return result;
		}
	} else if (status == GM_TOOLKIT_CONFIRM_WAIT) {
		tag = GetConfirmWaitTag();
	}

	switch (tag) {
	case GM_COMMAND_NONE:
		result = GM_RESULT_SYNTAX_ERROR;
		break;
	case GM_COMMAND_HELP:
		result = CommandHelp(count);
		break;
	case GM_COMMAND_COMMAND:
		result = CommandCommand(count);
		break;
	case GM_COMMAND_VISIBLE:
		result = CommandVisible(status);
		break;
	case GM_COMMAND_WHISPER:
		result = CommandWhisper(status);
		break;
	case GM_COMMAND_CHANNEL:
		result = CommandChannel(status);
		break;
	case GM_COMMAND_LIST:
		result = CommandList(status);
		break;
	case GM_COMMAND_OPEN:
		result = CommandOpen(status);
		break;
	case GM_COMMAND_CLOSE:
		result = CommandClose(status);
		break;
	case GM_COMMAND_KICK_USER:
		result = CommandKickUser(status);
		break;
	case GM_COMMAND_DISCONNECT:
		result = CommandDisconnect(status);
		break;
	case GM_COMMAND_DISCON_UID:
		result = CommandDiscon_UID(status);
		break;
	case GM_COMMAND_SHOW_STATUS:
		result = CommandShowStatus(status);
		break;
	case GM_COMMAND_DESTROY:
		result = CommandDestroy(status);
		break;
	case GM_COMMAND_WIND:
		result = CommandWind(status);
		break;
	case GM_COMMAND_WEATHER:
		result = CommandWeather(status);
		break;
	case GM_COMMAND_IDENTITY:
		result = CommandIdentity(status);
		break;
	case GM_COMMAND_NOTICE:
		result = CommandNotice(status);
		break;
	case GM_COMMAND_GIVE_ITEM:
		result = CommandGiveItem(status);
		break;
	case GM_COMMAND_GOLDEN_BELL:
		result = CommandGoldenBell(status);
		break;
	case GM_COMMAND_SET_PRIZE:
		result = CommandSetPrize(status);
		break;
	case GM_COMMAND_UNSET_PRIZE:
		result = CommandUnsetPrize(status);
		break;
	case GM_COMMAND_SHOW_PRIZE:
		result = CommandShowPrize(status);
		break;
	case GM_COMMAND_NOTICE_PRIZE:
		result = CommandNoticePrize(status);
		break;
	case GM_COMMAND_GET_TID:
		result = CommandGetTID(status);
		break;
	case GM_COMMAND_SET_MISSION:
		result = CommandSetMission(status);
		break;
	case GM_COMMAND_FIND_ITEM:
		result = CommandFindItem(status);
		break;
	case GM_COMMAND_CATEGORY:
		result = CommandCategory(status);
		break;
	case GM_COMMAND_MATCH_MAP_SETTING:
		result = CommandMatchMapSetting(status);
		break;
	case GM_COMMAND_MATCH_HOLE_SETTING:
		result = CommandMatchHoleSetting(status);
		break;
	}

	return result;
}

// gmtoolkit.cpp:462
bool CGMToolkit::IsAdministrator(void) const {
	return WSingleton<CSharedDoc>::Instance()->m_userInfo.identityBits.administrator ? true : false;
}

// gmtoolkit.cpp:470
bool CGMToolkit::IsGM(void) const {
	return m_isGM;
}

// gmtoolkit.cpp:478
bool CGMToolkit::Command(const char *command) {
	bool result = false;

	if (PhraseSaperator(command)) {
		Display(MakeStr("GM_Command:%s", command), 0x6EECFF);

		switch (Parser()) {
		case GM_RESULT_SYNTAX_ERROR:
			Display("Syntax error.", 0xFF7802);
			result = false;
			break;
		case GM_RESULT_BAD_ARGUMENT:
			Display("Argument error.", 0xFF7802);
			result = false;
			break;
		case GM_RESULT_SUCCEEDED:
			result = true;
			break;
		}
	}

	return result;
}

// gmtoolkit.cpp:512
void CGMToolkit::Display(const char *message, unsigned long color) {
	WSingleton<CChatMsg>::Instance()->AddChatMsg(message, color, true, false);
}

// gmtoolkit.cpp:519
void CGMToolkit::SendStatus(eGMCommandTag tag) {
	WSendPacket packet(0x8C);
	packet.Encode2(tag);
	packet.Encode2(m_statusFlags);
	packet.Send(SEND_TO_DEFAULT);
}

// gmtoolkit.cpp:530
void CGMToolkit::SendStringBuffer(eGMCommandTag tag, const char *buffer) {
	std::string str(buffer);

	WSendPacket packet(0x8C);
	packet.Encode2(tag);
	packet.EncodeStr(str);
	packet.Send(SEND_TO_DEFAULT);
}

// gmtoolkit.cpp:540
void CGMToolkit::SendCharacterBuffer(eGMCommandTag tag, const char *buffer) {
	int length = strlen(buffer) + 1;

	WSendPacket packet(0x8C);
	packet.Encode2(tag);
	packet.EncodeBuffer(buffer, length);
	packet.Send(SEND_TO_DEFAULT);
}

// gmtoolkit.cpp:685
bool CGMToolkit::FindItem(unsigned long typeId, char *name) const {
	IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(typeId);

	if (item == 0) {
		return false;
	}

	strcpy(name, item->m_name);
	return true;
}

// gmtoolkit.cpp:704
bool CGMToolkit::Confirm(void) {
	bool result = false;

	if (m_argumentCount != 1) {
		result = false;
	} else {
		if (strcmp(m_arguments[0], "y") == 0 || strcmp(m_arguments[0], "Y") == 0) {
			result = true;
			if (m_confirmPacket) {
				m_confirmPacket->Send(SEND_TO_DEFAULT);
			}
		}
	}

	if (m_confirmPacket) {
		delete m_confirmPacket;
		m_confirmPacket = 0;
	}

	SetToolKitStatus(GM_TOOLKIT_NORMAL);
	return result;
}

// gmtoolkit.cpp:738
bool CGMToolkit::FindKeywordInString(const char *const string, const char *const keyword) {
	int stringLength = strlen(string);
	int keywordLength = strlen(keyword);
	int matched = 0;

	for (int i = 0; i < stringLength; i++) {
		if (string[i] == keyword[matched]) {
			matched++;

			if (matched == keywordLength) {
				return true;
			}
		} else {
			matched = 0;
		}
	}

	return false;
}

// gmtoolkit.cpp:769
eGMToolResult CGMToolkit::CommandHelp(int count) {
	eGMToolResult result = GM_RESULT_SUCCEEDED;

	if (m_argumentCount > 1) {
		for (int i = 1; i < count; i++) {
			if (strcmp(m_arguments[1], data_aa53d8[i].keyword) == 0) {
				if (data_aa53d8[i].enabled == true) {
					Display(data_aa53d8[i].help, 0xC6FF85);
				} else {
					Display("\xc7\xf6\xc0\xe7 \xbb\xe7\xbf\xeb \xc7\xd2 \xbc\xf6 \xbe\xf8\xb4\xc2 \xb8\xed\xb7\xc9\xbe\xee \xc0\xd4\xb4\xcf\xb4\xd9.", 0xFF7802);
				}

				return GM_RESULT_SUCCEEDED;
			} else {
				result = GM_RESULT_BAD_ARGUMENT;
			}
		}
	} else {
		if (m_argumentCount != 1) {
			return GM_RESULT_BAD_ARGUMENT;
		}

		Display("command \xb8\xed\xb7\xc9\xc0\xbb \xc0\xcc\xbf\xeb\xc7\xcf\xbf\xa9 \xb8\xed\xb7\xc9\xbe\xee \xc8\xae\xc0\xce\xc0\xcc \xb0\xa1\xb4\xc9\xc7\xd5\xb4\xcf\xb4\xd9.", 0xC6FF85);
	}

	return result;
}

// gmtoolkit.cpp:814
eGMToolResult CGMToolkit::CommandCommand(int count) {
	for (int i = 1; i < count; i++) {
		if (data_aa53d8[i].enabled == true) {
			Display(data_aa53d8[i].keyword, 0xC6FF85);
		} else {
			Display(data_aa53d8[i].keyword, 0xFF7802);
		}
	}

	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:835
eGMToolResult CGMToolkit::CommandVisible(eGMToolkitStatus status) {
	if (m_argumentCount > 1) {
		if (strcmp(m_arguments[1], "on") == 0) {
			SetStatus(GM_STATUS_VISIBLE, GM_STATUS_ON);
			SendStatus(GM_COMMAND_VISIBLE);
			// "Status flag changed, visible on."
			Display(MakeStr("\xbb\xf3\xc5\xc2\xc7\xc3\xb7\xa1\xb1\xd7 \xba\xaf\xb0\xe6, %s", "visible on."), 0xC6FF85);
		} else if (strcmp(m_arguments[1], "off") == 0) {
			SetStatus(GM_STATUS_VISIBLE, GM_STATUS_OFF);
			SendStatus(GM_COMMAND_VISIBLE);
			Display(MakeStr("\xbb\xf3\xc5\xc2\xc7\xc3\xb7\xa1\xb1\xd7 \xba\xaf\xb0\xe6, %s", "visible off."), 0xC6FF85);
		} else {
			return GM_RESULT_BAD_ARGUMENT;
		}
	} else {
		return GM_RESULT_BAD_ARGUMENT;
	}

	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:868
eGMToolResult CGMToolkit::CommandWhisper(eGMToolkitStatus status) {
	if (m_argumentCount > 1) {
		if (strcmp(m_arguments[1], "on") == 0) {
			SetStatus(GM_STATUS_WHISPER, GM_STATUS_ON);
			SendStatus(GM_COMMAND_WHISPER);
			// "Status flag changed, whisper on." (and, as in the original,
			// "channel off.")
			Display("\xbb\xf3\xc5\xc2\xc7\xc3\xb7\xa1\xb1\xd7 \xba\xaf\xb0\xe6, whisper on.", 0xC6FF85);
			Display("\xbb\xf3\xc5\xc2\xc7\xc3\xb7\xa1\xb1\xd7 \xba\xaf\xb0\xe6, channel off.", 0xC6FF85);
		} else if (strcmp(m_arguments[1], "off") == 0) {
			SetStatus(GM_STATUS_WHISPER, GM_STATUS_OFF);
			SendStatus(GM_COMMAND_WHISPER);
			Display("\xbb\xf3\xc5\xc2\xc7\xc3\xb7\xa1\xb1\xd7 \xba\xaf\xb0\xe6, whisper off.", 0xC6FF85);
		} else {
			return GM_RESULT_BAD_ARGUMENT;
		}
	} else {
		return GM_RESULT_BAD_ARGUMENT;
	}

	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:902
eGMToolResult CGMToolkit::CommandChannel(eGMToolkitStatus status) {
	if (m_argumentCount > 1) {
		if (strcmp(m_arguments[1], "on") == 0) {
			// Channel chat and whisper exclude each other.
			m_statusFlags = (m_statusFlags & ~GM_STATUS_WHISPER) | GM_STATUS_CHANNEL;
			SendStatus(GM_COMMAND_CHANNEL);
			// "Status flag changed, channel on." and "whisper off."
			Display("\xbb\xf3\xc5\xc2\xc7\xc3\xb7\xa1\xb1\xd7 \xba\xaf\xb0\xe6, channel on.", 0xC6FF85);
			Display("\xbb\xf3\xc5\xc2\xc7\xc3\xb7\xa1\xb1\xd7 \xba\xaf\xb0\xe6, whisper off.", 0xC6FF85);
		} else if (strcmp(m_arguments[1], "off") == 0) {
			SetStatus(GM_STATUS_CHANNEL, GM_STATUS_OFF);
			SendStatus(GM_COMMAND_CHANNEL);
			Display("\xbb\xf3\xc5\xc2\xc7\xc3\xb7\xa1\xb1\xd7 \xba\xaf\xb0\xe6, channel off.", 0xC6FF85);
		} else {
			return GM_RESULT_BAD_ARGUMENT;
		}
	} else {
		return GM_RESULT_BAD_ARGUMENT;
	}

	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:937
eGMToolResult CGMToolkit::CommandList(eGMToolkitStatus status) {
	int count = m_chatPartnerList.size();

	if (count > 0) {
		for (int i = 0; i < count; i++) {
			WSingleton<CChatMsg>::Instance()->AddChatMsg(m_chatPartnerList[i], 0xC6FF85, true, false);
		}
	} else {
		// "No chat partner has been added."
		Display("\xc3\xdf\xb0\xa1\xb5\xc8 \xb4\xeb\xc8\xad\xbb\xf3\xb4\xeb\xb0\xa1 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0xFF7802);
	}

	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:960
eGMToolResult CGMToolkit::CommandOpen(eGMToolkitStatus status) {
	eGMToolResult result = GM_RESULT_SUCCEEDED;
	if (m_argumentCount > 1) {
		std::string name(m_arguments[1]);

		std::vector<std::string>::iterator it;
		it = std::find(m_chatPartnerList.begin(), m_chatPartnerList.end(), name);

		if (it != m_chatPartnerList.end()) {
			// "Already added to the chat partner list."
			Display("\xb4\xeb\xc8\xad\xbb\xf3\xb4\xeb \xb8\xae\xbd\xba\xc6\xae\xbf\xa1 \xc0\xcc\xb9\xcc \xc3\xdf\xb0\xa1\xb5\xc8 \xbb\xf3\xb4\xeb\xc0\xd4\xb4\xcf\xb4\xd9.", 0xFF7802);
		} else {
			m_chatPartnerList.push_back(name);
			SendStringBuffer(GM_COMMAND_OPEN, m_arguments[1]);
			// "Added to the chat partner list."
			Display("\xb4\xeb\xc8\xad\xbb\xf3\xb4\xeb \xb8\xae\xbd\xba\xc6\xae\xbf\xa1 \xc3\xdf\xb0\xa1\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.", 0xC6FF85);
		}
	} else {
		result = GM_RESULT_BAD_ARGUMENT;
	}
	return result;
}

// gmtoolkit.cpp:991
eGMToolResult CGMToolkit::CommandClose(eGMToolkitStatus status) {
	eGMToolResult result = GM_RESULT_SUCCEEDED;
	if (m_argumentCount > 1) {
		std::string name(m_arguments[1]);

		std::vector<std::string>::iterator it;
		it = std::find(m_chatPartnerList.begin(), m_chatPartnerList.end(), name);

		if (it != m_chatPartnerList.end()) {
			m_chatPartnerList.erase(it);
			// "Removed from the chat partner list."
			Display("\xb4\xeb\xc8\xad\xbb\xf3\xb4\xeb \xb8\xae\xbd\xba\xc6\xae\xbf\xa1\xbc\xad \xbb\xe8\xc1\xa6\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.", 0xC6FF85);
			SendStringBuffer(GM_COMMAND_CLOSE, m_arguments[1]);
		} else {
			// "Not in the chat partner list."
			Display("\xb4\xeb\xc8\xad\xbb\xf3\xb4\xeb \xb8\xae\xbd\xba\xc6\xae\xbf\xa1 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0xFF7802);
		}
	} else {
		result = GM_RESULT_BAD_ARGUMENT;
	}
	return result;
}

// gmtoolkit.cpp:1075
eGMToolResult CGMToolkit::CommandDisconnect(eGMToolkitStatus status) {
	if (m_argumentCount <= 1) {
		return GM_RESULT_BAD_ARGUMENT;
	}

	sBriefUserInfo info;
	std::map<unsigned long, sBriefUserInfo>::iterator it;
	for (it = WSingleton<CSharedDoc>::Instance()->m_userMap.begin(); it != WSingleton<CSharedDoc>::Instance()->m_userMap.end(); ++it) {
		info = it->second;
		if (strcmp(info.nick, m_arguments[1]) == 0) {
			break;
		}
	}

	info = it->second;
	if (it != WSingleton<CSharedDoc>::Instance()->m_userMap.end()) {
		if (info.identityBits.administrator) {
			Display("\xbf\xee\xbf\xb5\xc0\xda\xb4\xc2 \xb5\xf0\xbd\xba\xc4\xbf\xb3\xd8\xc6\xae \xbd\xc3\xc5\xb3 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0xFF7802);
		} else {
			WSendPacket packet(0x8C);
			packet.Encode2(0xB);
			packet.Encode4(info.uid);
			packet.Send(SEND_TO_DEFAULT);

			WSendPacket disconnectPacket(0x61);
			disconnectPacket.Encode4(info.uid);
			disconnectPacket.Send(SEND_TO_DEFAULT);
		}
	} else {
		Display("\xc7\xf6\xc0\xe7 \xbc\xad\xb9\xf6\xbf\xa1 \xc1\xa2\xbc\xd3\xc7\xcf\xb0\xed \xc0\xd6\xc1\xf6 \xbe\xca\xb0\xc5\xb3\xaa \xbe\xf8\xb4\xc2 \xc0\xaf\xc0\xfa\xc0\xd4\xb4\xcf\xb4\xd9.", 0xFF7802);
	}

	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:1126
eGMToolResult CGMToolkit::CommandDiscon_UID(eGMToolkitStatus status) {
	eGMToolResult result = GM_RESULT_SUCCEEDED;

	if (m_argumentCount > 1) {
		std::map<unsigned long, sBriefUserInfo>::iterator it;
		if (!WSingleton<CSharedDoc>::Instance()->FindUserInfoUID(atoi(m_arguments[1]), it)) {
			return GM_RESULT_BAD_ARGUMENT;
		}

		sBriefUserInfo info = it->second;

		if (it != WSingleton<CSharedDoc>::Instance()->m_userMap.end()) {
			if (info.identityBits.administrator) {
				Display("\xbf\xee\xbf\xb5\xc0\xda\xb4\xc2 \xb5\xf0\xbd\xba\xc4\xbf\xb3\xd8\xc6\xae \xbd\xc3\xc5\xb3 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0xFF7802);
			} else {
				WSendPacket packet((enumClientPacket)0x8C);
				packet.Encode2(0xB);
				packet.Encode4(info.uid);
				packet.Send(SEND_TO_DEFAULT);

				WSendPacket disconnectPacket((enumClientPacket)0x61);
				disconnectPacket.Encode4(info.uid);
				disconnectPacket.Send(SEND_TO_DEFAULT);
			}
		} else {
			Display("\xc7\xf6\xc0\xe7 \xbc\xad\xb9\xf6\xbf\xa1 \xc1\xa2\xbc\xd3\xc7\xcf\xb0\xed \xc0\xd6\xc1\xf6 \xbe\xca\xb0\xc5\xb3\xaa \xbe\xf8\xb4\xc2 \xc0\xaf\xc0\xfa\xc0\xd4\xb4\xcf\xb4\xd9.", 0xFF7802);
		}
	} else {
		result = GM_RESULT_BAD_ARGUMENT;
	}

	return result;
}

// gmtoolkit.cpp:1176
eGMToolResult CGMToolkit::CommandDestroy(eGMToolkitStatus status) {
	eGMToolResult result = GM_RESULT_SUCCEEDED;
	if (m_argumentCount > 1) {
		if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomNumber == 0xFFFF) {
			int roomNumber = atoi(m_arguments[1]);
			WSendPacket destroyPacket((enumClientPacket)0x60);
			destroyPacket.Encode2(roomNumber);
			destroyPacket.Send(SEND_TO_DEFAULT);

			WSendPacket packet((enumClientPacket)0x8C);
			packet.Encode2(GM_COMMAND_DESTROY);
			packet.Send(SEND_TO_DEFAULT);
		} else {
			// "Cannot be used outside the lobby."
			Display("\xb7\xce\xba\xf1 \xc0\xcc\xbf\xdc\xc0\xc7 \xc0\xe5\xbc\xd2\xbf\xa1\xbc\xad \xbb\xe7\xbf\xeb \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0xFF7802);
		}
	} else {
		result = GM_RESULT_BAD_ARGUMENT;
	}
	return result;
}

// gmtoolkit.cpp:1209
eGMToolResult CGMToolkit::CommandShowStatus(eGMToolkitStatus status) {
	if (m_statusFlags & GM_STATUS_VISIBLE) {
		Display("Visible : on", 0xC6FF85);
	} else {
		Display("Visible : off", 0xC6FF85);
	}

	if (m_statusFlags & GM_STATUS_WHISPER) {
		Display("Whisper : on", 0xC6FF85);
	} else {
		Display("Whisper : off", 0xC6FF85);
	}

	if (m_statusFlags & GM_STATUS_CHANNEL) {
		Display("Channel : on", 0xC6FF85);
	} else {
		Display("Channel : off", 0xC6FF85);
	}

	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:1240
eGMToolResult CGMToolkit::CommandWind(eGMToolkitStatus status) {
	eGMToolResult result = GM_RESULT_SUCCEEDED;

	if (m_argumentCount > 2) {
		unsigned char roomType = WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType;
		switch (roomType) {
		case 4:
		case 5:
		case 6:
		case 9:
		case 10:
		case 14:
			break;
		default:
			if (roomType != 2) {
				unsigned char speed = atoi(m_arguments[1]);
				unsigned char direction = atoi(m_arguments[2]);

				if (speed > 18) {
					Display("Wind speed value [0-18]", 0xFF7802);
					result = GM_RESULT_BAD_ARGUMENT;
				}

				if (direction > 360) {
					Display("Wind direction value [0-360]", 0xFF7802);
					result = GM_RESULT_BAD_ARGUMENT;
				} else if (result != GM_RESULT_BAD_ARGUMENT) {
					WSendPacket packet((enumClientPacket)0x8C);
					packet.Encode2(GM_COMMAND_WIND);
					packet.Encode1(speed - 1);
					packet.Encode1(direction);
					packet.Send(SEND_TO_DEFAULT);
				}
			} else {
				// "Cannot be used in a chat room."
				Display("\xb4\xeb\xc8\xad\xb9\xe6\xbf\xa1\xbc\xad\xb4\xc2 \xbb\xe7\xbf\xeb \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0xFF7802);
			}
			break;
		}
	} else {
		result = GM_RESULT_BAD_ARGUMENT;
	}

	return result;
}

// gmtoolkit.cpp:1290
eGMToolResult CGMToolkit::CommandWeather(eGMToolkitStatus status) {
	eGMToolResult result = GM_RESULT_SUCCEEDED;
	if (m_argumentCount > 1) {
		int weather;
		if (strcmp(m_arguments[1], "fine") == 0) {
			weather = 0;
		} else if (strcmp(m_arguments[1], "cloud") == 0) {
			weather = 1;
		} else if (strcmp(m_arguments[1], "rain") == 0) {
			weather = 2;
		} else if (strcmp(m_arguments[1], "snow") == 0) {
			weather = 3;
		} else {
			return GM_RESULT_BAD_ARGUMENT;
		}

		WSendPacket packet((enumClientPacket)0x8C);
		packet.Encode2(GM_COMMAND_WEATHER);
		packet.Encode1(weather);
		packet.Send(SEND_TO_DEFAULT);
	} else {
		result = GM_RESULT_BAD_ARGUMENT;
	}
	return result;
}

// gmtoolkit.cpp:1447
eGMToolResult CGMToolkit::CommandNotice(eGMToolkitStatus status) {
	if (m_argumentCount > 1) {
		std::string notice("");

		for (int i = 1; i < m_argumentCount; i++) {
			notice = notice + m_arguments[i];
			notice = notice + " ";
		}

		WSendPacket packet(0x57);
		packet.EncodeStr(notice);
		packet.Send(SEND_TO_DEFAULT);
	} else {
		return GM_RESULT_BAD_ARGUMENT;
	}

	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:1967
eGMToolResult CGMToolkit::CommandUnsetPrize(eGMToolkitStatus status) {
	if (status == GM_TOOLKIT_NORMAL) {
		if (m_argumentCount >= 1) {
			// "Delete the whole prize list? [/Y, /N]"
			Display("\xbd\xc3\xbb\xf3\xc7\xb0\xb8\xf1 \xb8\xae\xbd\xba\xc6\xae\xb8\xa6 \xc0\xfc\xba\xce \xbb\xe8\xc1\xa6\xc7\xcf\xb0\xda\xbd\xc0\xb4\xcf\xb1\xee?. [/Y, /N]", 0xC6FF85);
			SetConfirmWaitTag(GM_COMMAND_UNSET_PRIZE);
		} else {
			return GM_RESULT_SYNTAX_ERROR;
		}
	} else {
		if (Confirm()) {
			for (std::list<sPrize *>::iterator it = m_prizeList.begin(); it != m_prizeList.end(); ++it) {
				if (*it != 0) {
					delete *it;
					*it = 0;
				}
			}

			m_prizeList.clear();

			// "Prize list deleted."
			Display("\xbd\xc3\xbb\xf3\xc7\xb0\xb8\xf1 \xb8\xae\xbd\xba\xc6\xae \xbb\xe8\xc1\xa6\xbf\xcf\xb7\xe1.", 0xC6FF85);
			m_prizeSwitch = false;
		} else {
			// "The 'unsetprize' command was cancelled."
			Display("'unsetprize' \xb8\xed\xb7\xc9\xc0\xcc \xc3\xeb\xbc\xd2 \xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.", 0xC6FF85);
		}

		SetToolKitStatus(GM_TOOLKIT_NORMAL);
	}

	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:2110
eGMToolResult CGMToolkit::CommandLoadScript(eGMToolkitStatus status) {
	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:2119
eGMToolResult CGMToolkit::CommandNoticePrize(eGMToolkitStatus status) {
	int count = m_prizeList.size();

	if (count != 0) {
		WSendPacket packet((enumClientPacket)0x8C);
		packet.Encode2(GM_COMMAND_NOTICE_PRIZE);
		packet.Encode4(count);

		for (std::list<sPrize *>::iterator it = m_prizeList.begin(); it != m_prizeList.end(); ++it) {
			sPrize *prize = *it;

			if (prize != 0) {
				packet.EncodeBuffer(prize, sizeof(sPrize));
			}
		}

		packet.Send(SEND_TO_DEFAULT);
	} else {
		// "No registered conditions."
		Display("\xb5\xee\xb7\xcf\xb5\xc8 \xc1\xb6\xb0\xc7\xc0\xcc \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0xC6FF85);
	}

	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:2157
eGMToolResult CGMToolkit::CommandGetTID(eGMToolkitStatus status) {
	eGMToolResult result = GM_RESULT_SUCCEEDED;
	if (m_argumentCount > 1) {
		Display("Succeeded command.", 0xC6FF85);
	} else {
		result = GM_RESULT_BAD_ARGUMENT;
	}
	return result;
}

// gmtoolkit.cpp:2177
eGMToolResult CGMToolkit::CommandSetMission(eGMToolkitStatus status) {
	eGMToolResult result = GM_RESULT_SUCCEEDED;
	if (m_argumentCount > 1) {
		int mission = atoi(m_arguments[1]);

		WSendPacket packet((enumClientPacket)0x8C);
		packet.Encode2(GM_COMMAND_SET_MISSION);
		packet.Encode4(mission);
		packet.Send(SEND_TO_DEFAULT);

		Display("Succeeded command.", 0xC6FF85);
	} else {
		result = GM_RESULT_BAD_ARGUMENT;
	}
	return result;
}

// gmtoolkit.cpp:2314
eGMToolResult CGMToolkit::CommandCategory(eGMToolkitStatus status) {
	eGMToolResult result = GM_RESULT_SUCCEEDED;
	if (m_argumentCount > 0) {
		// The item categories, in Korean.
		Display("\xbe\xc6\xc0\xcc\xc5\xdb", 0xC6FF85);
		Display("\xc4\xab\xb5\xe5", 0xC6FF85);
		Display("\xc6\xc4\xc3\xf7", 0xC6FF85);
		Display("\xba\xb8\xc1\xb6\xc6\xc4\xc3\xf7", 0xC6FF85);
		Display("\xc5\xac\xb7\xb4", 0xC6FF85);
		Display("\xbe\xc6\xc1\xee\xc5\xd8", 0xC6FF85);
		Display("\xbc\xbc\xc6\xae", 0xC6FF85);
		Display("\xb0\xa1\xb1\xb8", 0xC6FF85);
		Display("\xb8\xf0\xb5\xce", 0xC6FF85);
	} else {
		result = GM_RESULT_BAD_ARGUMENT;
	}
	return result;
}

// gmtoolkit.cpp:2338
eGMToolResult CGMToolkit::CommandMatchMapSetting(eGMToolkitStatus status) {
	if (m_argumentCount != 2) {
		return GM_RESULT_BAD_ARGUMENT;
	}

	int map = atoi(m_arguments[1]);
	if (map < 0 || map > 20) {
		return GM_RESULT_BAD_ARGUMENT;
	}

	WSendPacket packet((enumClientPacket)0x8C);
	packet.Encode2(GM_COMMAND_MATCH_MAP_SETTING);
	packet.Encode4(map);
	packet.Send(SEND_TO_DEFAULT);

	return GM_RESULT_SUCCEEDED;
}

// gmtoolkit.cpp:2360
eGMToolResult CGMToolkit::CommandMatchHoleSetting(eGMToolkitStatus status) {
	if (m_argumentCount != 2) {
		return GM_RESULT_BAD_ARGUMENT;
	}

	int holeCount = atoi(m_arguments[1]);
	switch (holeCount) {
	case 3:
	case 6:
	case 9:
	case 18: {
		WSendPacket packet((enumClientPacket)0x8C);
		packet.Encode2(GM_COMMAND_MATCH_HOLE_SETTING);
		packet.Encode4(holeCount);
		packet.Send(SEND_TO_DEFAULT);
		break;
	}
	default:
		return GM_RESULT_BAD_ARGUMENT;
	}

	return GM_RESULT_SUCCEEDED;
}
