// Original: d:\build\custom\temp\code\client\projectg\parttidlist.h
#ifndef PG_CLIENT_PROJECTG_PARTTIDLIST_H
#define PG_CLIENT_PROJECTG_PARTTIDLIST_H

#include "..\..\defines.h"

class CItemManager;
struct sCharacterInfo;

// The type ids (and item ids) of the parts a character wears. Layout from
// CPartTidList's constructor and the CPartList functions comparing with it;
// only the members the units built so far use are declared.
class CPartTidList {
	LAYOUT_CHECK_FRIEND;
public:
	static void SetItemManager(CItemManager *pItemManager);           // parttidlist.cpp:19
	CPartTidList(void);                                                 // parttidlist.cpp:25
	bool SetTids(sCharacterInfo *pCharInfo, unsigned char level);       // parttidlist.cpp:32
	bool SetTid(unsigned long tid, unsigned long id, const char *texture, unsigned short slot); // parttidlist.cpp:218
	void SetDefaultTids(void);                                          // parttidlist.cpp:436
	bool IsComboValid(void) const;                                      // parttidlist.cpp:446
	bool IsEquipped(unsigned long tid, unsigned long id) const;         // parttidlist.cpp:509
	unsigned long GetOverlapped(unsigned long tid) const;               // parttidlist.cpp:533
	bool CanSetPart(unsigned long tid) const;                           // parttidlist.cpp:571

protected:
	bool SetTids(unsigned long tidChar, unsigned char hairClr, unsigned char shirtsClr,
		unsigned long *partTids, unsigned char level, unsigned long *partIds); // parttidlist.cpp:48
	void EmptyList(unsigned long mask);                                 // parttidlist.cpp:415

public:
	static CItemManager *m_pItemManager;

	unsigned long m_tidChar;            // 0x00
	unsigned char m_unk04;              // 0x04
	unsigned char m_numTotalParts;      // 0x05
	unsigned char m_hairClr;            // 0x06
	unsigned char m_shirtsClr;          // 0x07
	unsigned long m_partTids[24];       // 0x08
	unsigned long m_partIds[24];        // 0x68, cleared only for local content 0x5F
};

BEGIN_LAYOUT_CHECK(CPartTidList)
	CHECK_FIELD(CPartTidList, m_tidChar, 0x0);
	CHECK_FIELD(CPartTidList, m_numTotalParts, 0x5);
	CHECK_FIELD(CPartTidList, m_hairClr, 0x6);
	CHECK_FIELD(CPartTidList, m_shirtsClr, 0x7);
	CHECK_FIELD(CPartTidList, m_partTids, 0x8);
	CHECK_FIELD(CPartTidList, m_partIds, 0x68);
	CHECK_SIZE(CPartTidList, 0xC8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_PARTTIDLIST_H
