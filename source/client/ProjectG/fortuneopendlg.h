// Original: d:\build\custom\temp\code\client\projectg\fortuneopendlg.h
#ifndef PG_CLIENT_PROJECTG_FORTUNEOPENDLG_H
#define PG_CLIENT_PROJECTG_FORTUNEOPENDLG_H

#include <list>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrListBox;
class FrStatic;

// Fortune box opening dialog: the list of the items a fortune box can give,
// their total price and a description line. Size 0x130 (the `new` of
// FrFortuneOpenDlgMakeInstance).
class FrFortuneOpenDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // fortuneopendlg.h:89

	FrFortuneOpenDlg(void);
	virtual ~FrFortuneOpenDlg(void);

	unsigned long GetTypeId(void);                                    // fortuneopendlg.h:99, defined in RealMyRoomMainUI.cpp
	void SetTypeId(unsigned long typeId);
	void SetItemList(unsigned long* pTypeIds, int count);
	void SetItemNewList(unsigned long* pTypeIds, int count);

	// One row of the item list: an item type and how many of it the box holds.
	struct sFortune {
		unsigned long m_typeId;   // 0x0
		short m_count;            // 0x4

		bool operator==(const unsigned long& typeId) const {           // fortuneopendlg.h:126
			return m_typeId == typeId;
		}
	};

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnItemListInit(intptr_t param);
	void OnItemListOwnerDraw(intptr_t param);
	void OnTotalPriceInit(intptr_t param);
	void OnDescItemsInit(intptr_t param);

	UNKNOWN(0x110, 0x114);
	FrListBox* m_pItemList;              // 0x114
	FrStatic* m_pTotalPrice;             // 0x118
	FrStatic* m_pDescItems;              // 0x11c
	unsigned long m_typeId;              // 0x120
	std::list<sFortune> m_fortunes;      // 0x124
};

CHECK_SIZE(FrFortuneOpenDlg, 0x130);
BEGIN_LAYOUT_CHECK(FrFortuneOpenDlg)
	CHECK_FIELD(FrFortuneOpenDlg, m_pItemList, 0x114);
	CHECK_FIELD(FrFortuneOpenDlg, m_pTotalPrice, 0x118);
	CHECK_FIELD(FrFortuneOpenDlg, m_pDescItems, 0x11c);
	CHECK_FIELD(FrFortuneOpenDlg, m_typeId, 0x120);
	CHECK_FIELD(FrFortuneOpenDlg, m_fortunes, 0x124);
END_LAYOUT_CHECK
CHECK_SIZE(FrFortuneOpenDlg::sFortune, 0x8);

#endif // PG_CLIENT_PROJECTG_FORTUNEOPENDLG_H
