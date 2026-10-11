// Original: d:\build\custom\temp\code\client\projectg\frfindeborteventdlg.h
#ifndef PG_CLIENT_PROJECTG_FRFINDEBORTEVENTDLG_H
#define PG_CLIENT_PROJECTG_FRFINDEBORTEVENTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../client/Wangreal/include/wtypes.h"

// "Find the Ebort" event dialog: shows how many of each of the four event
// items (0x1A000044..0x1A000047) the player holds. Its size (0x148) is the
// `new` of FrFindEbortEventDlgMakeInstance.
class FrFindEbortEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frfindeborteventdlg.h:13

	FrFindEbortEventDlg(void);
	virtual ~FrFindEbortEventDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);

	void OnCurrentItemOwnerDraw(int param);

private:
	int m_itemCounts[4];        // 0x110 (OnInit: counts of items 0x1A000045, 0x1A000044, 0x1A000047, 0x1A000046)
	WPoint m_countPositions[4]; // 0x120 (OnInit, OnCurrentItemOwnerDraw: where each count is printed)
	int m_reserved140;          // 0x140 (zeroed by the constructor)
	UNKNOWN(0x144, 0x148);
};
BEGIN_LAYOUT_CHECK(FrFindEbortEventDlg)
	CHECK_FIELD(FrFindEbortEventDlg, m_itemCounts, 0x110);
	CHECK_FIELD(FrFindEbortEventDlg, m_countPositions, 0x120);
	CHECK_FIELD(FrFindEbortEventDlg, m_reserved140, 0x140);
END_LAYOUT_CHECK
CHECK_SIZE(FrFindEbortEventDlg, 0x148);

#endif // PG_CLIENT_PROJECTG_FRFINDEBORTEVENTDLG_H
