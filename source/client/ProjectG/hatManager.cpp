// Original: d:\build\custom\temp\code\client\projectg\hatmanager.cpp
#define PG_HATMANAGER_FILE_STRINGS
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer

#include "hatManager.h"
#include "guilddefine.h"
#include "ChatMsg.h"
#include "TaskManager.h"
#include "actor.h"
#include "SharedDoc.h"
#include "commonutil.h"
#include "../Wangreal/source/XUnzip.h"

#include <io.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <string>
#include <algorithm>

// <direct.h> (missing from externs/vs-2003, declared as westpak.cpp does)
extern "C" __declspec(dllimport) int __cdecl mkdir(const char *dirname);

extern unsigned long g_CurrentTime;

// The emoticon words, three faces of 12 words each.
extern const char *FacialTextTable[][12];

// A chat motion: its name and up to seven "/command" words.
struct sMotionText {
	const char *m_name;          // 0x00
	const char *m_command[7];    // 0x04
};
CHECK_SIZE(sMotionText, 0x20);

// The motion commands, 15 per motion type (no symbol).
extern sMotionText data_00a9e9d8[][15];

namespace BABO_STR {

// hatmanager.cpp:397
unsigned short TryToLower(unsigned short ch) {
	return (iswascii(ch) && iswupper(ch)) ? towlower(ch) : ch;
}

// hatmanager.cpp:499
// Returns true when the two words differ (case-insensitive, without their
// last character).
bool WStrCmp(unsigned short *word, int wordLen, unsigned short *text, int textLen) {
	if (wordLen != textLen)
		return true;

	for (int i = 0; i < wordLen - 1; i++) {
		if (tolower(word[i]) != tolower(text[i]))
			return true;
	}

	return false;
}

// hatmanager.cpp:641
// WStrCmp for Japanese: only ASCII characters compare case-insensitively.
bool WStrCmp_JP(unsigned short *word, int wordLen, unsigned short *text, int textLen) {
	if (wordLen != textLen)
		return true;

	for (int i = 0; i < wordLen - 1; i++) {
		if (word[i] < 0x80 && *text < 0x80) {
			if (tolower(text[i]) != tolower(word[i]))
				return true;
		} else {
			if (word[i] != text[i])
				return true;
		}
	}

	return false;
}

bool WStrStr(char *, unsigned short *, int, unsigned short *, int, bool); // 0x715110

} // namespace BABO_STR

// hatmanager.cpp:826
ChatManager::ChatManager(void) {
	m_floodCount = 0;
	m_repeatCount = 0;
	m_lastChatTime = g_CurrentTime;
	m_floodPenalty = false;
	m_repeatPenalty = false;
	m_chatFilter = 0;
	m_nickFilter = 0;
	m_chatFilterCount = 0;
	m_nickFilterCount = 0;
	m_ctorClearedFlag = false;
}

// hatmanager.cpp:840
ChatManager::~ChatManager(void) {
	if (m_chatFilter) { delete[] m_chatFilter; m_chatFilter = 0; }
	if (m_nickFilter) { delete[] m_nickFilter; m_nickFilter = 0; }
}

// hatmanager.cpp:847
bool ChatManager::Load(void) {
	bool loaded = false;

	loaded |= LoadChatFilter();
	loaded |= LoadNickFilter();

	return loaded;
}

// hatmanager.cpp:857
// Reads chat.bin: a zip whose items, bit-inverted, hold a word count and
// the forbidden chat words.
bool ChatManager::LoadChatFilter(void) {
	WFile file("chat.bin");

	int length = file.Length();
	char *data = new char[length];

	file.Read(data, length);
	file.Close();

	HZIP zip = OpenZipU(data, length, 3);
	if (zip == 0) {
		delete[] data;
		return false;
	}

	ZIPENTRY entry;
	GetZipItemA(zip, -1, &entry);
	int count = entry.index;

	for (int i = 0; i < count; i++) {
		GetZipItemA(zip, i, &entry);
		char *item = new char[entry.unc_size];
		UnzipItem(zip, i, item, entry.unc_size, 3);

		for (int k = 0; k < entry.unc_size; k++)
			item[k] = ~item[k];

		m_chatFilterCount = 0;
		int wordCount = *(int *)item;

		m_chatFilterCount = wordCount;
		if (m_chatFilterCount > 0) {
			m_chatFilter = new unsigned short[m_chatFilterCount][15];
			memcpy(m_chatFilter, item + 4, m_chatFilterCount * 30);
		}

		delete[] item;
	}

	CloseZipU(zip);
	delete[] data;

	return true;
}

// hatmanager.cpp:912
// Reads nick.bin, laid out as chat.bin.
bool ChatManager::LoadNickFilter(void) {
	WFile file("nick.bin");

	int length = file.Length();
	char *data = new char[length];

	file.Read(data, length);
	file.Close();

	HZIP zip = OpenZipU(data, length, 3);
	if (zip == 0) {
		delete[] data;
		return false;
	}

	ZIPENTRY entry;
	GetZipItemA(zip, -1, &entry);
	int count = entry.index;

	for (int i = 0; i < count; i++) {
		GetZipItemA(zip, i, &entry);
		char *item = new char[entry.unc_size];
		UnzipItem(zip, i, item, entry.unc_size, 3);

		for (int k = 0; k < entry.unc_size; k++)
			item[k] = ~item[k];

		m_nickFilterCount = 0;
		int wordCount = *(int *)item;

		m_nickFilterCount = wordCount;
		if (m_nickFilterCount > 0) {
			m_nickFilter = new unsigned short[m_nickFilterCount][15];
			memcpy(m_nickFilter, item + 4, m_nickFilterCount * 30);
		}

		delete[] item;
	}

	CloseZipU(zip);
	delete[] data;

	return true;
}

// hatmanager.cpp:969
void ChatManager::ParseString(const char *) {
}

// hatmanager.cpp:1262
// Returns true when `text` contains one of chat.bin's words; the query
// characters of `text` are then encoded (FilteringHack).
bool ChatManager::Filtering(char *text) {
	if (text == 0)
		return false;

	int i;
	bool found;
	unsigned short wideWord[0x400];
	unsigned short wideText[0x400];

	found = false;
	int textLen = MultiByteToWideChar(0, 0, text, strlen(text) + 1, wideText, 0x400);

	for (i = 0; i < m_chatFilterCount; i++) {
		char *word = (char *)m_chatFilter[i];
		int wordLen = MultiByteToWideChar(0, 0, word, strlen(word) + 1, wideWord, 0x400);

		if (BABO_STR::WStrStr(text, wideText, textLen, wideWord, wordLen, false))
			found = true;
	}

	FilteringHack(text, true);

	return found;
}

// hatmanager.cpp:1308
int ChatManager::GetFaceIndex(const char *text) {
	for (int face = 0; face < 3; face++) {
		for (int i = 0; i < 12; i++) {
			if (strstr(text, FacialTextTable[face][i]))
				return face;
		}
	}

	return 5;
}

// hatmanager.cpp:1340
const char *ChatManager::GetMotionName(const char *text, unsigned long type) {
	char command[64];

	for (int pass = 0; pass < 9; pass++) {
		for (int i = 0; i < 14; i++) {
			sMotionText *motion = &data_00a9e9d8[type][i];
			if (motion == 0)
				break;

			for (int k = 0; k < 7; k++) {
				if (motion->m_command[k] == 0)
					break;

				sprintf(command, "/%s", motion->m_command[k]);
				if (strstr(text, command))
					return data_00a9e9d8[type][i].m_name;
			}
		}
	}

	return 0;
}

// hatmanager.cpp:1384
// Encodes (or decodes back) the characters that could break a query:
// ';', '"', '\'' and "--".
void ChatManager::FilteringHack(const char *text, bool encode) {
	if (text == 0)
		return;

	char hackChar[4] = { ';', '"', '\'', '-' };
	char hackCode[4] = { 0x11, 0x12, 0x13, 0x14 };
	char *str = (char *)text;
	int len = strlen(str);

	for (int i = 0; i < len; i++) {
		if (encode) {
			for (int k = 0; k < 4; k++) {
				if (str[i] == hackChar[k]) {
					if (k == 3) {
						if (i < len - 1) {
							if (str[i + 1] == '-') {
								str[i] = hackCode[k];
								str[i + 1] = hackCode[k];
							}
						}
					} else {
						str[i] = hackCode[k];
					}
				}
			}
		} else {
			for (int k = 0; k < 4; k++) {
				if (str[i] == hackCode[k])
					str[i] = hackChar[k];
			}
		}
	}
}

// hatmanager.cpp:2103
// Writes the chat view's lines to history/chatting/chat<NNN>.txt: the
// first free number when `index` is negative, else index + 1. Returns the
// number used.
int ChatManager::SaveChatHistory(int index, int type) {
	if (WSingleton<CSharedDoc>::Instance()->m_chatLineList.size() == 0)
		return 0;

	mkdir("history");
	mkdir("history/chatting");

	char fileName[MAX_PATH];

	if (index < 0) {
		int i = 1;
		do {
			sprintf(fileName, "history/chatting/chat%03d.txt", i++);
		} while (_access(fileName, 0) != -1);
		index = i - 1;
	} else {
		index++;
		sprintf(fileName, "history/chatting/chat%03d.txt", index);
	}

	FILE *fp = fopen(fileName, "wt");
	if (fp == 0)
		return 0;

	for (std::list<sChatLine>::iterator it = WSingleton<CSharedDoc>::Instance()->m_chatLineList.begin();
		it != WSingleton<CSharedDoc>::Instance()->m_chatLineList.end(); ++it) {
		fwrite(it->m_text.c_str(), strlen(it->m_text.c_str()), 1, fp);
		fwrite("\n", 1, 1, fp);
	}

	fclose(fp);

	Display(MakeStr("\xC3\xA4\xC6\xC3 \xB3\xBB\xBF\xEB\xC0\xCC %s\xBF\xA1 \xC0\xFA\xC0\xE5\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", fileName), type);

	return index;
}

// hatmanager.cpp:2147
void ChatManager::Display(const char *text, int type) {
	if (type == 0) {
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 3, (intptr_t)text, 0xFFFF7878, 0, 0, 0);
	} else if (type == 1) {
		WSingleton<CChatMsg>::Instance()->AddChatMsg(text, 0xFFFF7878, true, false);
	}
}

// The ring of GetAlignedString's results.
static char s_alignedString[8][128];
static int s_alignedIndex;

// hatmanager.cpp:2205
// Pads `text` with spaces to `width` characters: 0 left, 1 centred, 2 right.
const char *ChatManager::GetAlignedString(const char *text, int width, int align) {
	if (width > 128)
		return text;

	s_alignedIndex++;

	memset(s_alignedString[s_alignedIndex], ' ', width);

	int offset;
	switch (align) {
	case 0:
		offset = 0;
		break;
	case 1:
		offset = (width - strlen(text)) / 2;
		break;
	case 2:
		offset = width - strlen(text);
		break;
	}

	strncpy(s_alignedString[s_alignedIndex] + offset, text, strlen(text));
	s_alignedString[s_alignedIndex][width] = 0;

	return s_alignedString[s_alignedIndex];
}

// hatmanager.cpp:2241
// Remembers a sent line: moved to the end when already known, else the
// oldest of 20 is dropped.
void ChatManager::RecordMyChatHistory(const char *text) {
	std::list<std::string> &history = WSingleton<CSharedDoc>::Instance()->m_chatHistory;
	std::list<std::string>::iterator it = std::find(history.begin(), history.end(), text);

	if (it != history.end()) {
		history.erase(it);
	} else if (WSingleton<CSharedDoc>::Instance()->m_chatHistory.size() == 20) {
		WSingleton<CSharedDoc>::Instance()->m_chatHistory.erase(history.begin());
	}

	history.push_back(text);
}
