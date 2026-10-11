// Original: d:\build\custom\temp\code\client\projectg\g_starmode.h
#ifndef PG_CLIENT_PROJECTG_G_STARMODE_H
#define PG_CLIENT_PROJECTG_G_STARMODE_H

#include "..\..\defines.h"

struct sCharacterInfo;

// Number of part slots per character entry and of entries (from the 0x300
// bytes the constructor clears and the 0x60-byte row SetParts copies).
#define STARMODE_ENTRY_COUNT 8
#define STARMODE_PART_COUNT 24

// The G-Star (exhibition) mode: preset character parts read from a file.
// Only the members the decompiled functions use are declared.
class CGStarMode {
public:
	CGStarMode(void);
	~CGStarMode(void);

	void GetAllItemFromItemManager(void);
	bool IsInternetConnect(void);
	bool LoadFile(const char *fileName);
	bool SavePartsInfoCharacter(const sCharacterInfo &character, const char *fileName, bool truncate, void (*onError)(void *));
	void Skip30vs(void);
	void SkipServer(void);
	void GStarModeInit(void);

private:
	void Parse(int entry, int part, char *text);
	void SetParts(int entry, unsigned long *parts);
	void SetPlayerData(void);

	unsigned long m_parts[STARMODE_ENTRY_COUNT][STARMODE_PART_COUNT]; // 0x0, part type ids per entry
	char *m_pFirstBuffer;    // 0x300, delete[]d by the destructor
	char *m_pSecondBuffer;   // 0x304, delete[]d by the destructor

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(CGStarMode)
	CHECK_FIELD(CGStarMode, m_parts, 0x0);
	CHECK_FIELD(CGStarMode, m_pFirstBuffer, 0x300);
	CHECK_FIELD(CGStarMode, m_pSecondBuffer, 0x304);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_G_STARMODE_H
