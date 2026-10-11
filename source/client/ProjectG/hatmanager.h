// Original: d:\build\custom\temp\code\client\projectg\hatmanager.h
#ifndef PG_CLIENT_PROJECTG_HATMANAGER_H
#define PG_CLIENT_PROJECTG_HATMANAGER_H

#include "..\..\defines.h"
#include "..\..\padding.h"

// Lines 38-43 of the original header held six file-scope `const char *`
// FilteringNick reasons with dynamic initializers, emitted by every object
// that defines PG_HATMANAGER_FILE_STRINGS before its first include:
// hatManager (sub_009b9010..sub_009b9060, data from 0xB79A80) and
// UccSetInfoDlg (sub_00997320..sub_00997370, data 0xAF5594..0xAF55A8 right
// after FrUccSetInfoDlg::m_RTTI). CSharedDoc holds a ChatManager by value:
// every SharedDoc.h includer needs this class but must not get the strings.
// They sit on lines 38-43 below, inside the guard, so their initializers
// keep their original line marks. In order: no nickname, bad length,
// contains a space, forbidden character, forbidden word, contains a
// forbidden word.
//
//
//
//
//
//
// Through const variables: the original initializes the six pointers
// dynamically (docs/tough_problems/010; the unit must include <atlbase.h>
// first so an earlier initializer makes a call).
#ifdef PG_HATMANAGER_FILE_STRINGS
static const char* const NICK_REASON_EMPTY = "\xB4\xEB\xC8\xAD\xB8\xED\xC0\xBB \xC0\xD4\xB7\xC2\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4. ";
static const char* const NICK_REASON_LENGTH = "\xB4\xEB\xC8\xAD\xB8\xED\xC0\xBA \\c0xffff0000\\c\xBF\xB5\xB9\xAE 4 - 16 \xB1\xDB\xC0\xDA\\c0xff000000\\c \xB6\xC7\xB4\xC2 \\c0xffff0000\\c\xC7\xD1\xB1\xDB 2 - 8\xB1\xDB\xC0\xDA\\c0xff000000\\c\xC0\xC7 \xB1\xE6\xC0\xCC\xB8\xA6 \xB0\xA1\xC1\xAE\xBE\xDF \xC7\xD5\xB4\xCF\xB4\xD9.";
static const char* const NICK_REASON_SPACE = "\xB4\xEB\xC8\xAD\xB8\xED\xC0\xBA \\c0xffff0000\\c\xB0\xF8\xB9\xE9\xB9\xAE\xC0\xDA\\c0xff000000\\c\xB8\xA6 \xC6\xF7\xC7\xD4\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.";
static const char* const NICK_REASON_BAD_CHAR = "\xB4\xEB\xC8\xAD\xB8\xED\xBF\xA1\xBC\xAD \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB9\xAE\xC0\xDA\xB8\xA6 \xC6\xF7\xC7\xD4\xC7\xCF\xB0\xED \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.";
static const char* const NICK_REASON_BAD_WORD = "\xB4\xEB\xC8\xAD\xB8\xED\xC0\xB8\xB7\xCE \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB4\xDC\xBE\xEE\xC0\xD4\xB4\xCF\xB4\xD9.";
static const char* const NICK_REASON_HAS_BAD_WORD = "\xB4\xEB\xC8\xAD\xB8\xED\xC0\xB8\xB7\xCE \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xB4\xC2 \xB4\xDC\xBE\xEE\xB8\xA6 \xC6\xF7\xC7\xD4\xC7\xCF\xB0\xED \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.";

// hatmanager.h:38-43

static const char* s_nickReasonEmpty = NICK_REASON_EMPTY;
static const char* s_nickReasonLength = NICK_REASON_LENGTH;
static const char* s_nickReasonSpace = NICK_REASON_SPACE;
static const char* s_nickReasonBadChar = NICK_REASON_BAD_CHAR;
static const char* s_nickReasonBadWord = NICK_REASON_BAD_WORD;
static const char* s_nickReasonHasBadWord = NICK_REASON_HAS_BAD_WORD;
#endif

// The chat manager: chat and nickname filters, emoticons, chat commands,
// flood control and the chat history. CSharedDoc holds one at 0x4C6C.
class ChatManager {
public:
	ChatManager(void);
	virtual ~ChatManager(void);

	bool Load(void);
	void ParseString(const char *);
	int GetFaceIndex(const char *);
	const char *GetMotionName(const char *, unsigned long);
	void FilteringHack(const char *, bool);
	bool Filtering(char *);
	const char *FilteringEmoticon(unsigned short *);
	const char *FilteringNick(char *);
	int FilteringGuildString(const char *, int, int, bool);
	void Display(const char *, int);
	int CheckChatCommand(const char *, int);
	int SaveChatHistory(int, int);
	void SendChatMessage2(const char *, const char *, bool);
	void CheckPenaltyTime(void);
	void SendChatMessage(const char *, const char *, bool);

private:
	bool LoadChatFilter(void);
	bool LoadNickFilter(void);
	const char *GetAlignedString(const char *, int, int);
	void RecordTournament(void);
	void RecordMyChatHistory(const char *);

public:
	// vtable                                      // 0x00
	unsigned short (*m_chatFilter)[15];            // 0x04, chat.bin's words (new[]'d by LoadChatFilter)
	unsigned short (*m_nickFilter)[15];            // 0x08, the nickname filter's words (LoadNickFilter)
	int m_chatFilterCount;                         // 0x0C
	int m_nickFilterCount;                         // 0x10
	unsigned long m_lastChatTime;                  // 0x14, g_CurrentTime of the last message sent
	int m_floodCount;                              // 0x18, messages sent in a row (5 starts the penalty)
	int m_repeatCount;                             // 0x1C, same message repeated (3 starts the penalty)
	bool m_floodPenalty;                           // 0x20, chat blocked for 60 s after flooding
	bool m_repeatPenalty;                          // 0x21, chat blocked for 60 s after repeating
	bool m_ctorClearedFlag;                        // 0x22, only the constructor writes it
	PADDING(0x23, 0x24);
};

BEGIN_LAYOUT_CHECK(ChatManager)
	CHECK_FIELD(ChatManager, m_chatFilter, 0x04);
	CHECK_FIELD(ChatManager, m_nickFilter, 0x08);
	CHECK_FIELD(ChatManager, m_chatFilterCount, 0x0C);
	CHECK_FIELD(ChatManager, m_nickFilterCount, 0x10);
	CHECK_FIELD(ChatManager, m_lastChatTime, 0x14);
	CHECK_FIELD(ChatManager, m_floodCount, 0x18);
	CHECK_FIELD(ChatManager, m_repeatCount, 0x1C);
	CHECK_FIELD(ChatManager, m_floodPenalty, 0x20);
	CHECK_FIELD(ChatManager, m_repeatPenalty, 0x21);
	CHECK_FIELD(ChatManager, m_ctorClearedFlag, 0x22);
END_LAYOUT_CHECK
CHECK_SIZE(ChatManager, 0x24);

#endif // PG_CLIENT_PROJECTG_HATMANAGER_H
