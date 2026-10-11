// Original: d:\build\custom\temp\code\client\projectg\gmtoolkit.h
#ifndef PG_CLIENT_PROJECTG_GMTOOLKIT_H
#define PG_CLIENT_PROJECTG_GMTOOLKIT_H

#include <list>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"

// The state of the GM command parser (GetToolKitStatus). Only the values the
// transcribed functions use are named.
enum eGMToolkitStatus {
	GM_TOOLKIT_NORMAL = 0,      // Confirm: back to plain command input
	GM_TOOLKIT_CONFIRM_WAIT = 1 // SetConfirmWaitTag: waiting for a confirmation
};

// A GM command: the tag of a command table entry (sGMCommand::tag), the
// command a confirmation is awaited for (SetConfirmWaitTag), the case of
// Parser's dispatch, and the tag of the status packets (SendStatus).
enum eGMCommandTag {
	GM_COMMAND_NONE = 0,               // Parser: syntax error
	GM_COMMAND_HELP = 1,
	GM_COMMAND_COMMAND = 2,
	GM_COMMAND_VISIBLE = 3,
	GM_COMMAND_WHISPER = 4,
	GM_COMMAND_CHANNEL = 5,
	GM_COMMAND_SHOW_STATUS = 6,
	GM_COMMAND_LIST = 7,
	GM_COMMAND_OPEN = 8,               // CommandOpen: a chat partner added
	GM_COMMAND_CLOSE = 9,              // CommandClose: a chat partner removed
	GM_COMMAND_KICK_USER = 0xA,
	GM_COMMAND_DISCONNECT = 0xB,
	GM_COMMAND_DISCON_UID = 0xC,
	GM_COMMAND_DESTROY = 0xD,          // CommandDestroy: a room destroyed
	GM_COMMAND_WIND = 0xE,             // CommandWind
	GM_COMMAND_WEATHER = 0xF,          // CommandWeather
	GM_COMMAND_IDENTITY = 0x10,
	GM_COMMAND_NOTICE = 0x11,
	GM_COMMAND_GIVE_ITEM = 0x12,
	GM_COMMAND_GOLDEN_BELL = 0x13,
	GM_COMMAND_SET_PRIZE = 0x14,
	GM_COMMAND_UNSET_PRIZE = 0x15,     // CommandUnsetPrize: confirmation awaited
	GM_COMMAND_SHOW_PRIZE = 0x16,
	GM_COMMAND_NOTICE_PRIZE = 0x19,    // CommandNoticePrize: the prize list sent
	GM_COMMAND_GET_TID = 0x1A,
	GM_COMMAND_SET_MISSION = 0x1B,
	GM_COMMAND_FIND_ITEM = 0x1C,
	GM_COMMAND_CATEGORY = 0x1D,
	GM_COMMAND_MATCH_MAP_SETTING = 0x1E,
	GM_COMMAND_MATCH_HOLE_SETTING = 0x1F
};

// SetStatus: clear (0) or set (1) a status flag.
enum eGMStatusSwitch {
	GM_STATUS_OFF = 0,
	GM_STATUS_ON = 1
};

// The result of a GM command. Only the values the transcribed commands
// return are named.
enum eGMToolResult {
	GM_RESULT_SYNTAX_ERROR = 0, // Command: "Syntax error."
	GM_RESULT_BAD_ARGUMENT = 1, // too few arguments
	GM_RESULT_SUCCEEDED = 2
};

// A bit of CGMToolkit's status flags (GetStatus/SetStatus), named after the
// lines CommandShowStatus prints for them.
enum eGMStatusFlag {
	GM_STATUS_VISIBLE = 1,
	GM_STATUS_WHISPER = 2,
	GM_STATUS_CHANNEL = 4
};

// An entry of the GM command table (CommandCommand lists its keywords).
struct sGMCommand {
	bool enabled;        // 0x0, false: "currently unusable command" (Parser, CommandHelp)
	eGMCommandTag tag;   // 0x4, Parser's dispatch case
	const char *keyword; // 0x8
	const char *help;    // 0xC
};
CHECK_SIZE(sGMCommand, 0x10);
CHECK_FIELD(sGMCommand, enabled, 0x0);
CHECK_FIELD(sGMCommand, tag, 0x4);
CHECK_FIELD(sGMCommand, keyword, 0x8);
CHECK_FIELD(sGMCommand, help, 0xC);

// A prize registered by CommandSetPrize (CGMToolkit::m_prizeList), sent
// whole by CommandNoticePrize.
struct sPrize {
	UNKNOWN(0x0, 0x14);
};
CHECK_SIZE(sPrize, 0x14);

// The client's GM command console (the WSingleton<CGMToolkit> instance). Only
// the members the transcribed functions touch are named; the size is not
// known yet.
class WSendPacket;

class CGMToolkit : public WSingleton<CGMToolkit> {
	LAYOUT_CHECK_FRIEND;
public:
	CGMToolkit(void);
	virtual ~CGMToolkit(void);

	bool GetPrizeSwitch(void) const;    // gmtoolkit.h:168
	bool GetStatus(eGMStatusFlag flag);
	void SetStatus(eGMStatusFlag flag, eGMStatusSwitch statusSwitch);
	bool IsAdministrator(void) const;
	bool IsGM(void) const;
	void Display(const char *message, unsigned long color);
	void SendStatus(eGMCommandTag tag);
	void SendStringBuffer(eGMCommandTag tag, const char *buffer);
	void SendCharacterBuffer(eGMCommandTag tag, const char *buffer);
	bool Command(const char *command);

protected:
	// Defined in the class body in the original (gmtoolkit.h line marks);
	// this unit emits them, so they are defined out of line in gmtoolkit.cpp.
	void ReleaseStringBuffer(void);
	eGMCommandTag GetConfirmWaitTag(void) const;    // gmtoolkit.h:180
	eGMToolkitStatus GetToolKitStatus(void) const;  // gmtoolkit.h:184
	void SetConfirmWaitTag(eGMCommandTag tag);      // gmtoolkit.h:188
	void SetToolKitStatus(eGMToolkitStatus status); // gmtoolkit.h:193

	bool PhraseSaperator(const char *command);
	eGMToolResult Parser(void);
	int GetGMCommandTableNumber(void);
	bool Confirm(void);
	bool FindKeywordInString(const char *const string, const char *const keyword);
	bool FindItem(unsigned long typeId, char *name) const;

	eGMToolResult CommandCommand(int count);
	eGMToolResult CommandHelp(int count);
	eGMToolResult CommandKickUser(eGMToolkitStatus status);
	eGMToolResult CommandDisconnect(eGMToolkitStatus status);
	eGMToolResult CommandDiscon_UID(eGMToolkitStatus status);
	eGMToolResult CommandIdentity(eGMToolkitStatus status);
	eGMToolResult CommandGiveItem(eGMToolkitStatus status);
	eGMToolResult CommandGoldenBell(eGMToolkitStatus status);
	eGMToolResult CommandSetPrize(eGMToolkitStatus status);
	eGMToolResult CommandShowPrize(eGMToolkitStatus status);
	eGMToolResult CommandFindItem(eGMToolkitStatus status);
	eGMToolResult CommandChannel(eGMToolkitStatus status);
	eGMToolResult CommandCategory(eGMToolkitStatus status);
	eGMToolResult CommandVisible(eGMToolkitStatus status);
	eGMToolResult CommandWhisper(eGMToolkitStatus status);
	eGMToolResult CommandShowStatus(eGMToolkitStatus status);
	eGMToolResult CommandWind(eGMToolkitStatus status);
	eGMToolResult CommandWeather(eGMToolkitStatus status);
	eGMToolResult CommandNotice(eGMToolkitStatus status);
	eGMToolResult CommandUnsetPrize(eGMToolkitStatus status);
	eGMToolResult CommandNoticePrize(eGMToolkitStatus status);
	eGMToolResult CommandList(eGMToolkitStatus status);
	eGMToolResult CommandOpen(eGMToolkitStatus status);
	eGMToolResult CommandClose(eGMToolkitStatus status);
	eGMToolResult CommandDestroy(eGMToolkitStatus status);
	eGMToolResult CommandMatchMapSetting(eGMToolkitStatus status);
	eGMToolResult CommandMatchHoleSetting(eGMToolkitStatus status);
	eGMToolResult CommandLoadScript(eGMToolkitStatus status);
	eGMToolResult CommandGetTID(eGMToolkitStatus status);
	eGMToolResult CommandSetMission(eGMToolkitStatus status);

	eGMToolkitStatus m_toolkitStatus;   // 0x28, GetToolKitStatus
	eGMCommandTag m_confirmWaitTag;     // 0x2C
	WSendPacket *m_confirmPacket;       // 0x30, sent by Confirm on "y"
	bool m_isGM;                        // 0x34
	unsigned short m_statusFlags;       // 0x36, eGMStatusFlag bits
	char **m_arguments;                 // 0x38, words of the parsed command (new[]'d)
	int m_argumentCount;                // 0x3C, entries of m_arguments
	std::vector<std::string> m_chatPartnerList; // 0x40, printed by CommandList
	char m_clearedBuffer0[0x12];        // 0x50, only zeroed (ctor) in this unit
	char m_clearedBuffer1[0x24];        // 0x62, only zeroed (ctor) in this unit
	UNKNOWN(0x86, 0x88);
	std::list<sPrize *> m_prizeList;    // 0x88, CommandSetPrize/CommandNoticePrize
	sPrize *m_pendingPrize;             // 0x94, CommandSetPrize: pushed to m_prizeList on confirmation
	bool m_prizeSwitch;                 // 0x98, GetPrizeSwitch
};

BEGIN_LAYOUT_CHECK(CGMToolkit)
	CHECK_FIELD(CGMToolkit, m_toolkitStatus, 0x28);
	CHECK_FIELD(CGMToolkit, m_confirmWaitTag, 0x2C);
	CHECK_FIELD(CGMToolkit, m_confirmPacket, 0x30);
	CHECK_FIELD(CGMToolkit, m_isGM, 0x34);
	CHECK_FIELD(CGMToolkit, m_statusFlags, 0x36);
	CHECK_FIELD(CGMToolkit, m_arguments, 0x38);
	CHECK_FIELD(CGMToolkit, m_argumentCount, 0x3C);
	CHECK_FIELD(CGMToolkit, m_chatPartnerList, 0x40);
	CHECK_FIELD(CGMToolkit, m_clearedBuffer0, 0x50);
	CHECK_FIELD(CGMToolkit, m_clearedBuffer1, 0x62);
	CHECK_FIELD(CGMToolkit, m_prizeList, 0x88);
	CHECK_FIELD(CGMToolkit, m_pendingPrize, 0x94);
	CHECK_FIELD(CGMToolkit, m_prizeSwitch, 0x98);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GMTOOLKIT_H
