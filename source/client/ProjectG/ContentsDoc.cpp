// Original: d:\build\custom\temp\code\client\projectg\contentsdoc.cpp
#include "contentsdoc.h"
#include "Packet.h"
#include "commonutil.h"
#include "SharedDoc.h"
#include "../shared/s5/SharedUtilities.h"
#include "../shared/luasystem/LuaScriptManager.h"
#include "../shared/luasystem/luatablehelper.h"

#include <algorithm>
#include <stdlib.h>
#include <string.h>

// contentsdoc.cpp:45
void CGiftBoxEvent::SetOpenedBoxTID(unsigned long openedBoxTID) {
	m_openedBoxTID = openedBoxTID;
}

// contentsdoc.cpp:50
unsigned long CGiftBoxEvent::GetOpenedBoxTID(void) {
	return m_openedBoxTID;
}

// contentsdoc.cpp:55
bool CGiftBoxEvent::Initialize(void) {
	m_openedBoxTID = 0;
	return true;
}

// contentsdoc.cpp:61
bool CGiftBoxEvent::Release(void) {
	return true;
}

// contentsdoc.cpp:68
void CWorldTourEvent::SetMapFlag(unsigned long mapFlag) {
	m_mapFlag = mapFlag;
}

// contentsdoc.cpp:73
void CWorldTourEvent::SetGotGift(bool gotGift) {
	m_gotGift = gotGift;
}

// contentsdoc.cpp:78
void CWorldTourEvent::SetCanGift(bool canGift) {
	m_canGift = canGift;
}

// contentsdoc.cpp:83
unsigned long CWorldTourEvent::GetMapFlag(void) const {
	return m_mapFlag;
}

// contentsdoc.cpp:88
bool CWorldTourEvent::GetGotGift(void) const {
	return m_gotGift;
}

// contentsdoc.cpp:93
bool CWorldTourEvent::GetCanGift(void) const {
	return m_canGift;
}

// contentsdoc.cpp:98
bool CWorldTourEvent::Initialize(void) {
	m_mapFlag = 0;
	m_gotGift = false;
	m_canGift = false;
	return true;
}

// contentsdoc.cpp:106
bool CWorldTourEvent::Release(void) {
	return Initialize();
}

// contentsdoc.cpp:114
void CHalloweenEvent::SetMapFlag(unsigned long mapFlag) {
	m_mapFlag = mapFlag;
}

// contentsdoc.cpp:119
void CHalloweenEvent::SetGiftCnt(unsigned long giftCnt) {
	m_giftCnt = giftCnt;
}

// contentsdoc.cpp:124
void CHalloweenEvent::SetShowGiftDlg(bool showGiftDlg) {
	m_showGiftDlg = showGiftDlg;
}

// contentsdoc.cpp:129
void CHalloweenEvent::SetEventState(bool eventState) {
	m_eventState = eventState;
}

// contentsdoc.cpp:134
unsigned long CHalloweenEvent::GetMapFlag(void) const {
	return m_mapFlag;
}

// contentsdoc.cpp:139
unsigned long CHalloweenEvent::GetGiftCnt(void) const {
	return m_giftCnt;
}

// contentsdoc.cpp:144
bool CHalloweenEvent::GetShowGiftDlg(void) const {
	return m_showGiftDlg;
}

// contentsdoc.cpp:149
bool CHalloweenEvent::GetEventState(void) const {
	return m_eventState;
}

// contentsdoc.cpp:154
bool CHalloweenEvent::Initialize(void) {
	m_mapFlag = 0;
	m_giftCnt = 0;
	m_showGiftDlg = false;
	m_eventState = false;
	return true;
}

// contentsdoc.cpp:163
bool CHalloweenEvent::Release(void) {
	return Initialize();
}

// contentsdoc.cpp:169
void CEscapeBeginnerEvent::SetFlag(bool flag) {
	m_flag = flag;
}

// contentsdoc.cpp:174
bool CEscapeBeginnerEvent::GetFlag(void) const {
	return m_flag;
}

// contentsdoc.cpp:179
bool CEscapeBeginnerEvent::Initialize(void) {
	m_flag = false;
	return true;
}

// contentsdoc.cpp:185
bool CEscapeBeginnerEvent::Release(void) {
	return Initialize();
}

// contentsdoc.cpp:191
void CPassingTitleChk::SetFlag(bool flag) {
	m_flag = flag;
}

// contentsdoc.cpp:196
bool CPassingTitleChk::GetFlag(void) const {
	return m_flag;
}

// contentsdoc.cpp:201
bool CPassingTitleChk::Initialize(void) {
	m_flag = false;
	return true;
}

// contentsdoc.cpp:207
bool CPassingTitleChk::Release(void) {
	return Initialize();
}

// contentsdoc.cpp:213
void CChristmasEvent::SetFlag(bool flag) {
	m_flag = flag;
}

// contentsdoc.cpp:218
bool CChristmasEvent::GetFlag(void) const {
	return m_flag;
}

// contentsdoc.cpp:223
bool CChristmasEvent::Initialize(void) {
	m_flag = true;
	return true;
}

// contentsdoc.cpp:229
bool CChristmasEvent::Release(void) {
	return Initialize();
}

// contentsdoc.cpp:235
void CTicketExchange::SetFlag(bool flag) {
	m_flag = flag;
}

// contentsdoc.cpp:240
bool CTicketExchange::GetFlag(void) const {
	return m_flag;
}

// contentsdoc.cpp:245
bool CTicketExchange::Initialize(void) {
	m_flag = true;
	return true;
}

// contentsdoc.cpp:251
bool CTicketExchange::Release(void) {
	return Initialize();
}

// contentsdoc.cpp:258
bool CMissionEvent::Initialize(void) {
	m_eventFlag = 0;
	m_dayGiftFlag = false;
	m_termGiftFlag = false;

	return true;
}

// contentsdoc.cpp:267
bool CMissionEvent::Release(void) {
	return true;
}

// contentsdoc.cpp:272
void CMissionEvent::SetEventFlag(unsigned int eventFlag) {
	m_eventFlag = eventFlag;
}

// contentsdoc.cpp:277
unsigned int CMissionEvent::GetEventFlag(void) const {
	return m_eventFlag;
}

// contentsdoc.cpp:282
bool CMissionEvent::IsCompleteMission(int index) {
	return m_missionInfo[index].complete;
}

// contentsdoc.cpp:288
bool CMissionEvent::IsCompleteAllDayMission(void) {
	for (int i = 0; i < 2; ++i) {
		if (!IsCompleteMission(i)) {
			return false;
		}
	}

	return true;
}

// contentsdoc.cpp:301
bool CMissionEvent::IsCompleteAllTermMission(void) {
	for (int i = 2; i < 7; ++i) {
		if (!IsCompleteMission(i)) {
			return false;
		}
	}

	return true;
}

// contentsdoc.cpp:314
bool CMissionEvent::IsCompleteCourse(int course) {
	return (GetCondition(6) & GetMissionField(course)) ? true : false;
}

// contentsdoc.cpp:319
void CMissionEvent::DecodeMissionInfo(WReceivedPacket &packet) {
	if (!m_missionInfo.empty()) {
		ClearMissionInfo();
	}

	int count = packet.Decode4();

	for (int i = 0; i < count; ++i) {
		GlobalEnum::sMissionInfo info;
		packet.DecodeBuffer(&info, sizeof(info));

		m_missionInfo.push_back(info);
	}

	unsigned char dayGiftFlag = packet.Decode1();
	m_dayGiftFlag = dayGiftFlag != 0;
	unsigned char termGiftFlag = packet.Decode1();
	m_termGiftFlag = termGiftFlag != 0;
}

// contentsdoc.cpp:344
int CMissionEvent::GetMissionField(int field) const {
	// The bit of each mission field; field 12 and up skip bit 12.
	static const int s_fieldBits[] = {
		0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
		0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000
	};

	if (field < 12) {
		return s_fieldBits[field];
	}
	return s_fieldBits[field + 1];
}

// contentsdoc.cpp:525
bool CPeriodContents::Initialize(void) {
	return true;
}

// contentsdoc.cpp:530
bool CPeriodContents::Release(void) {
	return true;
}

// contentsdoc.cpp:535
void CPeriodContents::AddUsableContents(int type, const _SYSTEMTIME *begin, const _SYSTEMTIME *end, bool noEnd) {
	if (type >= 4) {
		return;
	}

	sPeriodUnit unit;
	unit.begin = *begin;
	if (!noEnd) {
		unit.end = *end;
	}
	unit.noEnd = noEnd;
	unit.usable = false;

	m_contents.insert(std::map<int, sPeriodUnit>::value_type(type, unit));
}

// contentsdoc.cpp:555
bool CPeriodContents::IsUsableContents(int type) {
	std::map<int, sPeriodUnit>::const_iterator it = m_contents.find(type);
	if (it == m_contents.end()) {
		return false;
	}

	return it->second.usable;
}

// contentsdoc.cpp:567
void CPeriodContents::Set_PreSettingContentsDate(void) {
	_SYSTEMTIME begin = S5::MakeSysTime(2011, 4, 14, 8);
	_SYSTEMTIME end = S5::MakeSysTime(2011, 5, 3, 8);

	AddUsableContents(3, &begin, &end, false);

	CheckAllContents();
}

// contentsdoc.cpp:585
void CPeriodContents::CheckAllContents(void) {
	for (std::map<int, sPeriodUnit>::iterator it = m_contents.begin(); it != m_contents.end(); ++it) {
		IsUsable(it->second);
	}
}

// contentsdoc.cpp:594
bool CPeriodContents::CheckContent(int type) {
	std::map<int, sPeriodUnit>::iterator it = m_contents.find(type);
	if (it == m_contents.end()) {
		return false;
	}

	sPeriodUnit &unit = it->second;
	IsUsable(unit);

	return unit.usable;
}

// contentsdoc.cpp:608
void CPeriodContents::IsUsable(sPeriodUnit &unit) {
	_SYSTEMTIME &now = WSingleton<CSharedDoc>::Instance()->GetServerTime();

	// Usable from the hour the period begins.
	if (unit.begin.wYear <= now.wYear && unit.begin.wMonth <= now.wMonth
		&& unit.begin.wDay <= now.wDay && unit.begin.wHour <= now.wHour) {
		unit.usable = true;
	}

	if (unit.noEnd == true) {
		return;
	}

	unit.usable = ItemManager()->IsBetween(&unit.begin, &unit.end, now) ? true : false;
}

// contentsdoc.cpp:643
// Replaces every backquote of a guild text by a single quote, in place.
namespace _guild {
void ExchangeQuotation(char* str) {
	std::string text(str);
	str_Replace(text, "`", "'");
	strcpy(str, text.c_str());
}
} // namespace _guild

// contentsdoc.cpp:651
bool _guild::CGuildInfo::Initialize(void) {
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1000, "\xB1\xE6\xB5\xE5\xB8\xED\xC0\xBB \xC0\xD4\xB7\xC2\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1001, "\xB1\xE6\xB5\xE5\xB8\xA6 \xBC\xD2\xB0\xB3\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1005, "\xB1\xE6\xB5\xE5\xB8\xA6 \xBB\xFD\xBC\xBA\xC7\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1006, "\xB1\xE6\xB5\xE5\xB8\xB6\xC5\xA9\xB8\xA6 \xB5\xEE\xB7\xCF \xC7\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1007, "\xB1\xE6\xB5\xE5\xB8\xED\xC0\xBB \xBA\xAF\xB0\xE6\xC7\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1008, "\xBD\xC2\xC0\xCE(\xBD\xC2\xB1\xDE)|\xB0\xAD\xB5\xEE|\xB1\xE6\xB5\xE5\xBF\xF8 \xC3\xDF\xB9\xE6|\xB1\xE6\xB5\xE5\xC0\xE5 \xC0\xA7\xC0\xD3|\xC1\xA4\xBA\xB8\xBA\xB8\xB1\xE2"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1009, "\xBD\xC2\xC0\xCE(\xBD\xC2\xB1\xDE)|\xB1\xE6\xB5\xE5\xBF\xF8 \xC3\xDF\xB9\xE6|\xC1\xA4\xBA\xB8\xBA\xB8\xB1\xE2"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1010, "\xC1\xA4\xBA\xB8\xBA\xB8\xB1\xE2"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1011, "\xB4\xF5 \xC0\xCC\xBB\xF3 \xBD\xC2\xB1\xDE\xBD\xC3\xC5\xB3 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1012, "\xB4\xF5 \xC0\xCC\xBB\xF3 \xB0\xAD\xB5\xEE\xBD\xC3\xC5\xB3 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1036, "\xB1\xE6\xB5\xE5\xC0\xE5\xC0\xBB \xC0\xA7\xC0\xD3\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB5\xEE\xB1\xDE\xC0\xD4\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1037, "\xB0\xCB\xBB\xF6\xBE\xEE\xB8\xA6 \xC0\xD4\xB7\xC2\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1038, "\xB0\xA1\xC0\xD4\xC7\xD2 \xB1\xE6\xB5\xE5\xB8\xA6 \xBC\xB1\xC5\xC3\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4."));

	m_guildMsgs.insert(std::map<int, const char *>::value_type(1013, "\xC0\xCC\xB9\xCC \xB1\xE6\xB5\xE5\xBF\xA1 \xB0\xA1\xC0\xD4\xB5\xC7\xBE\xEE \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1014, "\xB0\xA1\xC0\xD4 \xBD\xC5\xC3\xBB \xB1\xDB\xC0\xBB \xC0\xDB\xBC\xBA\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1015, "\xB0\xA1\xC0\xD4 \xBD\xC5\xC3\xBB\xC0\xBB \xC3\xEB\xBC\xD2\xC7\xD5\xB4\xCF\xB4\xD9. \xC1\xF8\xC7\xE0\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1016, "\xB0\xA1\xC0\xD4 \xBD\xC5\xC3\xBB\xC0\xCC \xC3\xEB\xBC\xD2\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1017, "\xB0\xAD\xC1\xA6 \xC5\xBB\xC5\xF0\xBD\xC3\xC4\xD7\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1018, "\xB0\xA1\xC0\xD4 \xBD\xC5\xC3\xBB\xC0\xBB \xB0\xC5\xBA\xCE\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1019, "\xB0\xA1\xC0\xD4 \xBD\xC5\xC3\xBB\xC0\xBB \xBD\xC2\xC0\xCE\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1020, "\xB0\xA1\xC0\xD4 \xBD\xC5\xC3\xBB\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1021, "\xB1\xE6\xB5\xE5\xB8\xA6 \xC5\xBB\xC5\xF0\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1022, "\xB1\xE6\xB5\xE5\xB8\xA6 \xC5\xBB\xC5\xF0\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB5\xEE\xB1\xDE\xC0\xD4\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1047, "\xB1\xE6\xB5\xE5\xB8\xA6 \xC5\xBB\xC5\xF0\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1024, "\xB1\xE6\xB5\xE5\xB8\xA6 \xC6\xF3\xBC\xE2\xC7\xD5\xB4\xCF\xB4\xD9. (\xB1\xE6\xB5\xE5\xB8\xA6 \xC6\xF3\xBC\xE2\xC7\xCF\xB8\xE9 24\xBD\xC3\xB0\xA3 \xC0\xCC\xB3\xBB\xBF\xA1 \xBB\xF5\xB7\xCE \xB1\xE6\xB5\xE5\xB8\xA6 \xB8\xB8\xB5\xE9 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.)"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1025, "\xB1\xE6\xB5\xE5\xB8\xA6 \xC6\xF3\xBC\xE2\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1026, "\xB1\xE6\xB5\xE5 \xBC\xD2\xB0\xB3\xB8\xA6 \xBA\xAF\xB0\xE6\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1027, "\xB1\xE6\xB5\xE5 \xB0\xF8\xC1\xF6\xB8\xA6 \xBA\xAF\xB0\xE6\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1028, "\xB1\xE6\xB5\xE5 \xC0\xCC\xB8\xA7\xC0\xBB \xB4\xD9\xBD\xC3 \xC8\xAE\xC0\xCE\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1029, "\xBB\xE7\xBF\xEB \xB0\xA1\xB4\xC9\xC7\xD1 \xB1\xE6\xB5\xE5\xB8\xED\xC0\xD4\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1030, "\xB0\xB3\xC0\xCE \xB8\xDE\xBD\xC3\xC1\xF6\xB8\xA6 \xBA\xAF\xB0\xE6\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1031, "\xB1\xE6\xB5\xE5\xB0\xA1 \xBB\xFD\xBC\xBA\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1032, "\xBC\xB1\xC5\xC3\xC7\xD1 \xBA\xCE\xB1\xE6\xB5\xE5\xC0\xE5\xBF\xA1\xB0\xD4 \xB1\xE6\xB5\xE5\xC0\xE5\xC0\xBB \xC0\xA7\xC0\xD3\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1033, "%s \xB1\xC7\xC7\xD1\xC0\xBB \xC0\xA7\xC0\xD3\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.\x0A%s\xC0\xCC \xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1034, "%s \xB1\xC7\xC7\xD1\xC0\xB8\xB7\xCE \xBA\xAF\xB0\xE6\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1061, "\xB0\xA1\xC0\xD4\xBD\xC5\xC3\xBB"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1062, "\xB0\xA1\xC0\xD4\xC3\xEB\xBC\xD2"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1063, "\xB0\xA1\xC0\xD4\xBD\xC2\xC0\xCE"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1064, "\xB0\xA1\xC0\xD4\xB0\xC5\xC0\xFD"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1065, "\xB1\xE6\xB5\xE5\xBF\xF8\xB5\xEE\xB1\xDE\xBA\xAF\xB0\xE6"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1066, "\xB1\xE6\xB5\xE5\xBF\xF8 \xC3\xDF\xB9\xE6"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1067, "\xB1\xE6\xB5\xE5\xC5\xBB\xC5\xF0"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1068, "\xB1\xE6\xB5\xE5\xB0\xB3\xBC\xB3"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1069, "\xB1\xE6\xB5\xE5\xC6\xF3\xBC\xE2"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1070, "\xB1\xE6\xB5\xE5\xC0\xE5 \xC0\xA7\xC0\xD3"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1071, "\xB1\xE6\xB5\xE5\xC0\xE5 \xC0\xD3\xB8\xED"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1072, "\xBA\xCE\xB1\xE6\xB5\xE5\xC0\xE5 \xC0\xD3\xB8\xED"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1073, "\xB1\xE6\xB5\xE5\xBF\xF8 \xBD\xC2\xC0\xCE"));

	m_guildMsgs.insert(std::map<int, const char *>::value_type(1074, "\xB1\xE6\xB5\xE5\xB8\xB6\xC5\xA9\xBA\xED\xB7\xB0"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1075, "\xB1\xE6\xB5\xE5\xB8\xB6\xC5\xA9\xBA\xED\xB7\xB0\xC7\xD8\xC1\xA6"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1076, "GM\xBF\xA1\xC0\xC7\xC7\xD1\xC3\xDF\xB9\xE6"));

	m_guildMsgs.insert(std::map<int, const char *>::value_type(1077, "\xB1\xE6\xB5\xE5\xC0\xE5"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1078, "\xBA\xCE\xB1\xE6\xB5\xE5\xC0\xE5"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1079, "\xB1\xE6\xB5\xE5\xBF\xF8"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1080, "\xB0\xA1\xC0\xD4\xB4\xEB\xB1\xE2\xC0\xDA"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1081, "\xBD\xC2\xC0\xCE(\xBD\xC2\xB1\xDE)"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1082, "\xB0\xAD\xB5\xEE"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1083, "\xB1\xE6\xB5\xE5\xBF\xF8 \xC3\xDF\xB9\xE6"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1084, "\xC1\xA4\xBA\xB8\xBA\xB8\xB1\xE2"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1035, "\xB0\xCB\xBB\xF6\xB5\xC8 \xB1\xE6\xB5\xE5\xB0\xA1 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1039, "\xB1\xE6\xB5\xE5\xB8\xED\xC0\xCC \xBA\xAF\xB0\xE6\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1040, "\xB0\xA1\xC0\xD4\xC7\xD1 \xB1\xE6\xB5\xE5\xB0\xA1 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1048, "%s \xB4\xD4\xC0\xCC \xB1\xE6\xB5\xE5\xBF\xA1 \xB0\xA1\xC0\xD4\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1049, "%s \xB4\xD4\xC0\xCC \xB1\xE6\xB5\xE5\xB8\xA6 \xC5\xBB\xC5\xF0\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1050, "\xB5\xEE\xB7\xCF\xB5\xC8 \xB1\xE6\xB5\xE5\xB8\xB6\xC5\xA9\xB0\xA1 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1051, "\xB0\xA1\xB7\xCE 22, \xBC\xBC\xB7\xCE 20 \xC5\xA9\xB1\xE2\xB0\xA1 \xBE\xC6\xB4\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1052, "png \xC6\xC4\xC0\xCF\xC0\xCC \xBE\xC6\xB4\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1053, "\xB1\xE6\xB5\xE5 \xB8\xB6\xC5\xA9 \xBE\xF7\xB7\xCE\xB5\xE5\xBF\xA1 \xBC\xBA\xB0\xF8\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1054, "\xB1\xE6\xB5\xE5 \xB8\xB6\xC5\xA9 \xBE\xF7\xB7\xCE\xB5\xE5\xBF\xA1 \xBD\xC7\xC6\xD0\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1056, "\xC6\xC4\xC0\xCF \xC0\xD0\xB1\xE2\xBF\xA1 \xBD\xC7\xC6\xD0\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1057, "\xC6\xC4\xC0\xCF \xC3\xA3\xB1\xE2\xBF\xA1 \xBD\xC7\xC6\xD0\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1058, "\xC6\xC4\xC0\xCF \xC5\xA9\xB1\xE2\xB0\xA1 \xB3\xCA\xB9\xAB \xC5\xAE\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1059, "32\xBA\xF1\xC6\xAE png \xC6\xC4\xC0\xCF\xB8\xB8 \xB5\xEE\xB7\xCF\xC0\xCC \xB0\xA1\xB4\xC9\xC7\xD5\xB4\xCF\xB4\xD9."));

	m_guildMsgs.insert(std::map<int, const char *>::value_type(1041, "\\c0xffff0000\\c\xBF\xB5\xB9\xAE %d - %d \xB1\xDB\xC0\xDA\\c0xff000000\\c \xB6\xC7\xB4\xC2 \\c0xffff0000\\c\xC7\xD1\xB1\xDB %d - %d\xB1\xDB\xC0\xDA\\c0xff000000\\c\xC0\xC7 \xB1\xE6\xC0\xCC\xB8\xA6 \xB0\xA1\xC1\xAE\xBE\xDF \xC7\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1042, "\\c0xffff0000\\c\xBF\xB5\xB9\xAE %d\xB1\xDB\xC0\xDA\\c0xff000000\\c \xB6\xC7\xB4\xC2 \\c0xffff0000\\c\xC7\xD1\xB1\xDB %d\xB1\xDB\xC0\xDA\\c0xff000000\\c\xC0\xCC\xBB\xF3 \xC0\xD4\xB7\xC2\xC7\xCF\xBF\xA9\xBE\xDF \xC7\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1043, "\\c0xffff0000\\c\xB0\xF8\xB9\xE9\xB9\xAE\xC0\xDA\\c0xff000000\\c\xB8\xA6 \xC6\xF7\xC7\xD4\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1044, "\xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB9\xAE\xC0\xDA\xB8\xA6 \xC6\xF7\xC7\xD4\xC7\xCF\xB0\xED \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1045, "\xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB4\xDC\xBE\xEE\xB8\xA6 \xC6\xF7\xC7\xD4\xC7\xCF\xB0\xED \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1046, "\xC0\xD4\xB7\xC2\xB5\xC8 \xB1\xDB\xC0\xCC \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1055, "\xB1\xE6\xB5\xE5 \xB8\xB6\xC5\xA9 \xB5\xEE\xB7\xCF\xC0\xCC \xB1\xDD\xC1\xF6\xB5\xC8 \xBB\xF3\xC5\xC2\xC0\xD4\xB4\xCF\xB4\xD9. \xB0\xED\xB0\xB4 \xC1\xF6\xBF\xF8 \xBC\xBE\xC5\xCD\xB7\xCE \xB9\xAE\xC0\xC7 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9."));

	m_guildMsgs.insert(std::map<int, const char *>::value_type(1060, "\xB3\xBB\xBF\xEB\xC0\xCC \xB9\xD9\xB2\xEE\xC1\xF6 \xBE\xCA\xBE\xD2\xBD\xC0\xB4\xCF\xB4\xD9."));

	m_guildMsgs.insert(std::map<int, const char *>::value_type(0, "\xC1\xA4\xC0\xC7\xB5\xC7\xC1\xF6 \xBE\xCA\xC0\xBA \xBF\xC0\xB7\xF9\xC0\xD4\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(1, "\xBC\xBA\xB0\xF8\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54001, "\xB1\xE6\xB5\xE5 \xC1\xA4\xBA\xB8 \xC3\xB3\xB8\xAE \xBD\xC7\xC6\xD0\xC0\xD4\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54002, "\xC1\xB8\xC0\xE7\xC7\xCF\xC1\xF6 \xBE\xCA\xB4\xC2 \xB0\xE8\xC1\xA4\xC0\xD4\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54003, "\xC0\xCC\xB9\xCC \xB5\xBF\xC0\xCF\xC7\xD1 \xC0\xCC\xB8\xA7\xC0\xC7 \xB1\xE6\xB5\xE5\xB0\xA1 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54004, "\xB7\xB9\xBA\xA7\xC0\xCC \xBA\xCE\xC1\xB7\xC7\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54005, "\xC0\xCC\xB9\xCC \xB1\xE6\xB5\xE5\xBF\xA1 \xB0\xA1\xC0\xD4\xB5\xC7\xBE\xEE \xC0\xD6\xB0\xC5\xB3\xAA \xB0\xA1\xC0\xD4 \xBD\xC5\xC3\xBB \xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54006, "\xB1\xE6\xB5\xE5 \xB0\xB3\xBC\xB3/\xC6\xF3\xBC\xE2/\xC5\xBB\xC5\xF0 \xC8\xC4 24\xBD\xC3\xB0\xA3\xC0\xCC \xC1\xF6\xB3\xAA\xC1\xF6 \xBE\xCA\xBE\xD2\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54019, "\xBF\xC0\xB7\xF9\xB0\xA1 \xB9\xDF\xBB\xFD\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.(\xB1\xE6\xB5\xE5 \xBB\xFD\xBC\xBA \xB9\xF8\xC8\xA3)"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54008, "\xB8\xED\xB7\xC9\xC0\xBB \xBD\xC7\xC7\xE0\xC7\xD2 \xB1\xC7\xC7\xD1\xC0\xCC \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54009, "\xBD\xC5\xC3\xBB\xC7\xD1 \xB1\xE6\xB5\xE5\xC0\xC7 \xC0\xCE\xBF\xF8\xC0\xCC \xB8\xF0\xB5\xCE \xC2\xF7\xBC\xAD \xB4\xF5\xB4\xC2 \xB0\xA1\xC0\xD4\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54010, "\xBF\xE4\xC3\xBB\xC7\xD1 \xC0\xAF\xC0\xFA\xB0\xA1 \xB1\xE6\xB5\xE5\xBF\xF8\xC0\xCC \xBE\xC6\xB4\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54011, "\xBA\xCE\xB1\xE6\xB5\xE5\xC0\xE5\xC0\xCC \xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54012, "\xBA\xCE\xB1\xE6\xB5\xE5\xC0\xE5 \xC1\xA6\xC7\xD1 \xC0\xCE\xBF\xF8 \xC3\xCA\xB0\xFA\xC0\xD4\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54013, "\xB1\xE6\xB5\xE5\xC0\xE5 \xC3\xDF\xB9\xE6"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54014, "\xB3\xB2\xBE\xC6\xC0\xD6\xB4\xC2 \xB1\xE6\xB5\xE5\xBF\xF8\xC0\xCC \xC0\xD6\xB1\xE2 \xB6\xA7\xB9\xAE\xBF\xA1 \xB1\xE6\xB5\xE5\xB8\xA6 \xC6\xF3\xBC\xE2\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54015, "\xB1\xE6\xB5\xE5\xB0\xA1 \xC1\xB8\xC0\xE7\xC7\xCF\xC1\xF6 \xBE\xCA\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54016, "\xB1\xE6\xB5\xE5\xBF\xF8\xC0\xCC \xB3\xB2\xBE\xC6 \xC0\xD6\xB4\xC2 \xBB\xF3\xC5\xC2\xBF\xA1\xBC\xAD \xB1\xE6\xB5\xE5\xB8\xA6 \xC5\xBB\xC5\xF0\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54017, "\xC0\xCC\xB9\xCC \xB1\xE6\xB5\xE5\xBF\xA1 \xB0\xA1\xC0\xD4\xB5\xC7\xBE\xEE \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54018, "\xB0\xA1\xC0\xD4 \xB4\xEB\xB1\xE2 \xC1\xDF\xC0\xCE \xB1\xE6\xB5\xE5\xBF\xF8\xC0\xCC \xBE\xC6\xB4\xD5\xB4\xCF\xB4\xD9."));

	m_guildMsgs.insert(std::map<int, const char *>::value_type(54019, "\xB0\xA1\xC0\xD4 \xBD\xC5\xC3\xBB \xBB\xF3\xC5\xC2\xB0\xA1 \xBE\xC6\xB4\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54020, "\xB0\xA1\xC0\xD4 \xB4\xEB\xB1\xE2\xC0\xDA\xB0\xA1 \xBE\xC6\xB4\xCF\xB9\xC7\xB7\xCE \xB1\xE6\xB5\xE5 \xB0\xA1\xC0\xD4 \xC3\xB6\xC8\xB8\xB8\xA6 \xBF\xE4\xC3\xBB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54021, "\xB0\xA1\xC0\xD4 \xB4\xEB\xB1\xE2\xC0\xDA\xB4\xC2 \xB1\xE6\xB5\xE5\xB8\xA6 \xC5\xBB\xC5\xF0\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));

	m_guildMsgs.insert(std::map<int, const char *>::value_type(54030, "\xB1\xE6\xB5\xE5 \xB4\xEB\xC0\xFC \xB0\xE1\xB0\xFA \xC0\xFA\xC0\xE5 \xBF\xC0\xB7\xF9"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54500, "\xB5\xA5\xC0\xCC\xC5\xCD \xBA\xA3\xC0\xCC\xBD\xBA \xBF\xC0\xB7\xF9"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54501, "\xBD\xC3\xBD\xBA\xC5\xDB \xBD\xC3\xB0\xA3 \xBA\xAF\xC8\xAF \xBD\xC7\xC6\xD0"));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54502, "\xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBB \xBC\xD2\xC0\xAF\xC7\xCF\xB0\xED \xC0\xD6\xC1\xF6 \xBE\xCA\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54503, "\xB1\xE6\xB5\xE5 \xBB\xFD\xBC\xBA\xC0\xBA \xBC\xBA\xB0\xF8\xC7\xCF\xBF\xB4\xC0\xB8\xB3\xAA, \xBE\xC6\xC0\xCC\xC5\xDB \xBB\xE8\xC1\xA6\xBF\xA1 \xBD\xC7\xC6\xD0\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54504, "\xC7\xF6\xC0\xE7 \xB5\xEE\xB1\xDE\xBF\xA1\xBC\xAD\xB4\xC2 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB1\xE2\xB4\xC9\xC0\xD4\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54505, "\xB1\xE6\xB5\xE5\xC0\xE5\xC0\xCC \xBE\xC6\xB4\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54506, "\xBA\xCE\xB1\xE6\xB5\xE5\xC0\xE5\xC0\xCC \xBE\xC6\xB4\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54507, "\xB0\xFC\xB8\xAE\xB1\xC7\xC7\xD1\xC0\xCC \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54508, "\xB1\xE6\xB5\xE5\xBF\xA1 \xB0\xA1\xC0\xD4\xB5\xC7\xBE\xEE \xC0\xD6\xC1\xF6 \xBE\xCA\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54509, "\xB0\xA1\xC0\xD4 \xB4\xEB\xB1\xE2\xC0\xDA\xB0\xA1 \xBE\xC6\xB4\xD5\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54510, "\xC0\xCC\xB9\xCC \xB1\xE6\xB5\xE5\xBF\xA1 \xB0\xA1\xC0\xD4\xB5\xC7\xBE\xEE \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54511, "\xC0\xAF\xC8\xBF\xC7\xCF\xC1\xF6 \xBE\xCA\xC0\xBA \xB1\xE6\xB5\xE5\xBB\xF3\xC5\xC2\xC0\xD4\xB4\xCF\xB4\xD9. \xC0\xE7\xC1\xA2\xBC\xD3 \xC8\xC4 \xB4\xD9\xBD\xC3 \xBD\xC3\xB5\xB5\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4."));
	m_guildMsgs.insert(std::map<int, const char *>::value_type(54022, "\xB1\xE6\xB5\xE5 \xB8\xB6\xC5\xA9 \xB5\xEE\xB7\xCF\xC0\xCC \xB1\xDD\xC1\xF6\xB5\xC8 \xBB\xF3\xC5\xC2\xC0\xD4\xB4\xCF\xB4\xD9. \xB0\xED\xB0\xB4 \xC1\xF6\xBF\xF8 \xBC\xBE\xC5\xCD\xB7\xCE \xB9\xAE\xC0\xC7 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9."));

	ClearAllData();

	return true;
}

// contentsdoc.cpp:789
bool _guild::CGuildInfo::Release(void) {
	ClearAllData();

	return true;
}

// contentsdoc.cpp:796
void _guild::CGuildInfo::ClearAllData(void) {
	m_myGuildInfo.classIndex = 0;
	m_myGuildInfo.valueFF = -1;
	m_myGuildInfo.guildNumber = 0;
	m_myGuildInfo.value19 = 0;
	m_myGuildInfo.value1D = 0;
	m_myGuildInfo.value21 = 0;
	memset(m_myGuildInfo.data119, 0, sizeof(m_myGuildInfo.data119));
	memset(m_myGuildInfo.introduce, 0, sizeof(m_myGuildInfo.introduce));
	memset(m_myGuildInfo.emblem, 0, sizeof(m_myGuildInfo.emblem));
	memset(m_myGuildInfo.text103, 0, sizeof(m_myGuildInfo.text103));
	memset(m_myGuildInfo.name, 0, sizeof(m_myGuildInfo.name));
	memset(m_myGuildInfo.notice, 0, sizeof(m_myGuildInfo.notice));

	m_allMemberCount = 0;
	m_allGuildCount = 0;
	ClearMemberPage();
	ClearHistoryList();
}

// contentsdoc.cpp:818
void _guild::CGuildInfo::ClearHistoryList(void) {
	if (m_historyList.size() != 0) {
		m_historyList.clear();
	}
}

// contentsdoc.cpp:826
void _guild::CGuildInfo::ClearMemberPage(void) {
	std::map<int, std::list<GUILD_USER_LIST> >::iterator it = m_memberPages.begin();
	for (; it != m_memberPages.end(); ++it) {
		it->second.clear();
	}
}

// contentsdoc.cpp:836
void _guild::CGuildInfo::ClearMemberList(int page) {
	std::map<int, std::list<GUILD_USER_LIST> >::iterator it = m_memberPages.find(page);
	if (it != m_memberPages.end()) {
		std::list<GUILD_USER_LIST> &members = it->second;
		members.clear();
	}
}

namespace _guild {
// Replaces the backquotes of a guild member's text (std::for_each functor).
struct sExchangerQuota {
	// contentsdoc.cpp:851
	void operator()(GUILD_USER_LIST &member) {
		ExchangeQuotation(member.text);
	}
};
} // namespace _guild

// contentsdoc.cpp:856
void _guild::CGuildInfo::SetMemberPage(int page, std::list<GUILD_USER_LIST> *pList) {
	std::for_each(pList->begin(), pList->end(), sExchangerQuota());

	std::map<int, std::list<GUILD_USER_LIST> >::iterator it = m_memberPages.find(page);
	if (it != m_memberPages.end()) {
		it->second.swap(*pList);
	} else {
		std::list<GUILD_USER_LIST> members;
		m_memberPages.insert(std::make_pair(page, members));
		m_memberPages[page].swap(*pList);
	}
}

// contentsdoc.cpp:877
void _guild::CGuildInfo::ClearSearchPage(void) {
	std::map<int, std::list<GUILD_LIST> >::iterator it = m_searchPages.begin();
	for (; it != m_searchPages.end(); ++it) {
		it->second.clear();
	}
}

// contentsdoc.cpp:887
void _guild::CGuildInfo::ClearSearchList(int page) {
	std::map<int, std::list<GUILD_LIST> >::iterator it = m_searchPages.find(page);
	if (it != m_searchPages.end()) {
		std::list<GUILD_LIST> &guilds = it->second;
		guilds.clear();
	}
}

// contentsdoc.cpp:899
void _guild::CGuildInfo::SetMyGuildInfo(GUILD_INFO &info) {
	m_myGuildInfo = info;

	ExchangeQuotation(m_myGuildInfo.introduce);
	ExchangeQuotation(m_myGuildInfo.notice);
}

// contentsdoc.cpp:907
void _guild::CGuildInfo::SetMyGuildUserInfo(GUILD_USER_INFO &info) {
	static_cast<GUILD_USER_INFO &>(m_myGuildInfo) = info;

	ExchangeQuotation(m_myGuildInfo.introduce);
	ExchangeQuotation(m_myGuildInfo.notice);
}

// contentsdoc.cpp:951
void _guild::CGuildInfo::SetGuildEmblem(GUILD_USER_INFO *pInfo) {
	strncpy(m_myGuildInfo.emblem, pInfo->emblem, sizeof(m_myGuildInfo.emblem) - 1);
}

// contentsdoc.cpp:956
void _guild::CGuildInfo::SetGuildName(GUILD_INFO *pInfo) {
	strncpy(m_myGuildInfo.name, pInfo->name, sizeof(m_myGuildInfo.name) - 1);
}

// contentsdoc.cpp:962
void _guild::CGuildInfo::SetGuildIntroduce(const std::string &introduce) {
	strcpy(m_myGuildInfo.introduce, introduce.c_str());
}

// contentsdoc.cpp:967
void _guild::CGuildInfo::SetGuildNotice(const std::string &notice) {
	strcpy(m_myGuildInfo.notice, notice.c_str());
}

// contentsdoc.cpp:915
void _guild::CGuildInfo::SetHistoryList(std::list<GUILD_HISTORY> *pList) {
	if (m_historyList.size() != 0) {
		m_historyList.clear();
	}

	std::list<GUILD_HISTORY>::iterator it = pList->begin();
	for (; it != pList->end(); ++it) {
		m_historyList.push_back(*it);
	}
}

// contentsdoc.cpp:930
void _guild::CGuildInfo::SetSearchList(int page, std::list<GUILD_LIST> *pList) {
	std::map<int, std::list<GUILD_LIST> >::iterator it = m_searchPages.find(page);
	if (it != m_searchPages.end()) {
		it->second.swap(*pList);
	} else {
		std::list<GUILD_LIST> guilds;
		m_searchPages.insert(std::make_pair(page, guilds));
		m_searchPages[page].swap(*pList);
	}
}

// contentsdoc.cpp:973
const GUILD_INFO &_guild::CGuildInfo::GetMyGuildInfo(void) const {
	return m_myGuildInfo;
}

// contentsdoc.cpp:978
const char *_guild::CGuildInfo::GuildMsg(int index) {
	std::map<int, const char *>::iterator it = m_guildMsgs.find(index);
	if (it == m_guildMsgs.end()) {
		return 0;
	}

	return it->second;
}

// contentsdoc.cpp:989
bool _guild::CGuildInfo::IsGuildMember(void) const {
	switch (m_myGuildInfo.classIndex) {
	case 1:
	case 2:
	case 3:
		return true;

	default:
		return false;
	}
}

// contentsdoc.cpp:1002
bool _guild::CGuildInfo::IsGuildMember(GUILD_CLASS_IDX classIndex) {
	switch (classIndex) {
	case 1:
	case 2:
	case 3:
		return true;

	default:
		return false;
	}
}

// contentsdoc.cpp:1015
bool _guild::CGuildInfo::IsManager(void) const {
	switch (m_myGuildInfo.classIndex) {
	case 1:
	case 2:
		return true;

	default:
		return false;
	}
}

// contentsdoc.cpp:1027
bool _guild::CGuildInfo::IsManager(GUILD_CLASS_IDX classIndex) {
	switch (classIndex) {
	case 1:
	case 2:
		return true;

	default:
		return false;
	}
}

// contentsdoc.cpp:1039
int _guild::CGuildInfo::GetMaxMemberPage(void) {
	if (GetAllMemberCount() == 0) {
		return 1;
	}

	int maxPage = GetAllMemberCount() / 15;
	if (GetAllMemberCount() % 15 != 0)
		maxPage += 1;

	return maxPage;
}

// contentsdoc.cpp:1054
int _guild::CGuildInfo::GetMaxSearchPage(void) {
	if (GetAllGuildCount() == 0) {
		return 1;
	}

	int maxPage = GetAllGuildCount() / 15;
	if (GetAllGuildCount() % 15 != 0)
		maxPage += 1;

	return maxPage;
}

// contentsdoc.cpp:1069
const std::list<GUILD_USER_LIST> *_guild::CGuildInfo::GetMemberList(int page) {
	std::map<int, std::list<GUILD_USER_LIST> >::iterator it = m_memberPages.find(page);
	if (it == m_memberPages.end()) {
		return 0;
	}

	return &it->second;
}

// contentsdoc.cpp:1081
const std::list<GUILD_LIST> *_guild::CGuildInfo::GetSearchList(int page) {
	std::map<int, std::list<GUILD_LIST> >::iterator it = m_searchPages.find(page);
	if (it == m_searchPages.end()) {
		return 0;
	}

	return &it->second;
}

// contentsdoc.cpp:1215
void CChristmasSockEvent::SetFlag(bool flag) {
	m_flag = flag;
}

// contentsdoc.cpp:1220
bool CChristmasSockEvent::GetFlag(void) const {
	return m_flag;
}

// contentsdoc.cpp:1225
bool CChristmasSockEvent::Initialize(void) {
	m_flag = true;
	return true;
}

// contentsdoc.cpp:1231
bool CChristmasSockEvent::Release(void) {
	return Initialize();
}

// contentsdoc.cpp:1237
bool CVectorSlideTestContainer::Initialize(void) {
	m_newTypeVectorSlide = true;
	m_limitSpeed = 80;
	m_boundHeight = 25;
	m_limitVectorSlideCount = 35;

	return true;
}

// contentsdoc.cpp:1246
bool CVectorSlideTestContainer::Release(void) {
	return Initialize();
}

// contentsdoc.cpp:1254
bool CGimmickContainer::Initialize(void) {
	m_randomSeed = rand();
	m_minDistBtwItems = 40.0f;

	for (int hole = 0; hole < 18; hole++) {
		std::vector<GimmickDispositionInformation> positions;
		m_gimmickPositions[hole] = positions;
	}

	m_magicCarpetBoundingSeq = "wizcity_magic_carpet_bounding.seq";
	m_pangCoinCrashSeq = "wizcity_field_item_pangcoin_crash.seq";
	m_randomBoxCrashSeq = "wizcity_field_item_randombox_crash.seq";

	return true;
}

// contentsdoc.cpp:1283
bool CGimmickContainer::Release(void) {
	return Initialize();
}

// contentsdoc.cpp:1288
void CGimmickContainer::AddGimmickPosition(unsigned char hole, const std::vector<GimmickDispositionInformation> &positions) {
	m_gimmickPositions[hole] = positions;
}

// contentsdoc.cpp:1293
std::vector<GimmickDispositionInformation> &CGimmickContainer::GetGimmickPosition(unsigned char hole) {
	return m_gimmickPositions[hole];
}

// contentsdoc.cpp:1298
void CGimmickContainer::ResetGimmikFlag(unsigned char hole, GimmickDispositionInformation::eFlag flag) {
	std::vector<GimmickDispositionInformation> &positions = GetGimmickPosition(hole);

	for (unsigned int i = 0; i < positions.size(); i++) {
		positions[i].flag = flag;
	}
}

// contentsdoc.cpp:1308
GimmickDispositionInformation *CGimmickContainer::FindDisposeInfo(unsigned char hole, GIMMICK_ITEM_TYPE itemType, GimmickDispositionInformation::eFlag flag) {
	std::vector<GimmickDispositionInformation> &positions = GetGimmickPosition(hole);

	for (unsigned int i = 0; i < positions.size(); i++) {
		bool typeMatch = false;
		bool flagMatch = false;

		if (itemType == GIMMICK_ITEM_ANY) {
			typeMatch = true;
		} else if (positions[i].itemType == itemType) {
			typeMatch = true;
		}

		if (flag == GimmickDispositionInformation::FLAG_ANY) {
			flagMatch = true;
		} else if (positions[i].flag == flag) {
			flagMatch = true;
		}

		if (typeMatch && flagMatch) {
			return &positions[i];
		}
	}

	return NULL;
}

// contentsdoc.cpp:1344
int CGimmickContainer::GetNumDisposeInfo(unsigned char hole, GIMMICK_ITEM_TYPE itemType, GimmickDispositionInformation::eFlag flag) {
	std::vector<GimmickDispositionInformation> &positions = GetGimmickPosition(hole);

	int count = 0;
	for (unsigned int i = 0; i < positions.size(); i++) {
		bool typeMatch = false;
		bool flagMatch = false;

		if (itemType == GIMMICK_ITEM_ANY) {
			typeMatch = true;
		} else if (positions[i].itemType == itemType) {
			typeMatch = true;
		}

		if (flag == GimmickDispositionInformation::FLAG_ANY) {
			flagMatch = true;
		} else if (positions[i].flag == flag) {
			flagMatch = true;
		}

		if (typeMatch && flagMatch) {
			count++;
		}
	}

	return count;
}

// contentsdoc.cpp:1382
void CGimmickContainer::LoadFromPacket(WReceivedPacket &packet) {
	Release();

	m_randomSeed = packet.Decode4();

	for (int hole = 0; hole < 18; hole++) {
		int count = packet.Decode1();

		for (int i = 0; i < count; i++) {
			GimmickDispositionInformation info;
			packet.DecodeBuffer(&info, sizeof(info));
			std::vector<GimmickDispositionInformation> &positions = m_gimmickPositions[info.hole];
			positions.push_back(info);
		}
	}
}

// contentsdoc.cpp:1405
bool CGimmickContainer::LoadFromScript(const char *fileName) {
	Release();

	lua_system::lua_table_helper table = lua_system::lua_script_manger::GetInstance()->GetTable(fileName);

	for (int hole = 1; hole <= 18; hole++) {
		std::vector<GimmickDispositionInformation> positions;

		int randomBoxCount = table.GetValueByName<int>("gimmick_randombox_count");
		int pangCount = table.GetValueByName<int>("gimmick_pang_count");

		GimmickDispositionInformation info;

		for (int i = 0; i < randomBoxCount; i++) {
			info.value4 = i;
			info.itemType = GIMMICK_ITEM_RANDOM_BOX;
			positions.push_back(info);
		}

		for (int i = 0; i < pangCount; i++) {
			int index = randomBoxCount + i;

			info.itemType = GIMMICK_ITEM_PANG_COIN;
			info.value4 = index;
			positions.push_back(info);
		}

		AddGimmickPosition(hole, positions);
	}

	m_randomSeed = table.GetValueByName<int>("gimmick_randomseed");
	m_minDistBtwItems = (float)table.GetValueByName<int>("coin_object_placement_gap");

	m_magicCarpetBoundingSeq = table.GetValueByName<const char *>("FxForBoundOnMagicCarpet");
	m_pangCoinCrashSeq = table.GetValueByName<const char *>("FxForCrashFieldPang");
	m_randomBoxCrashSeq = table.GetValueByName<const char *>("FxForCrashFieldRandomBox");

	return true;
}

// contentsdoc.cpp:1451
bool CGimmickContainer::NeedToMakeTriPtArray(const char *textureName, FieldItem::eDisposeTextureType *textureType) const {
	if (strstr(textureName, "ct_green_b") != NULL) {
		if (textureType != NULL) {
			*textureType = FieldItem::DISPOSE_TEXTURE_GREEN_B;
		}
		return true;
	}

	if (strcmp(textureName, "ct_booster.dds") == 0) {
		if (textureType != NULL) {
			*textureType = FieldItem::DISPOSE_TEXTURE_BOOSTER;
		}
		return true;
	}

	return false;
}

// contentsdoc.cpp:1470
void CGimmickContainer::AddItemAcquired(const FieldItem::AcquiredItemInfo &info) {
	m_acquiredItems.push_back(info);
}
