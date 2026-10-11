// Original: d:\build\custom\temp\code\client\projectg\deleteitem.h
#ifndef PG_CLIENT_PROJECTG_DELETEITEM_H
#define PG_CLIENT_PROJECTG_DELETEITEM_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrArea;
class FrButton;
class FrEdit;

// "Delete item" form: shows the item to delete (icon, name, level), the
// number owned and an edit box with -/+ buttons to choose how many to delete
// (1-99). Its size (0x130) is the `new` of FrDeleteItemDlgMakeInstance.
class FrDeleteItemDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // deleteitem.h:8

	FrDeleteItemDlg(void);

	void SetDelItem(unsigned long typeId, unsigned long count);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnItemInit(intptr_t param);
	void OnItemOwnerDraw(int param);
	void OnDescInit(intptr_t param);
	void OnWarnInit(intptr_t param);
	void OnItemNumInit(intptr_t param);
	void OnNumInit(intptr_t param);
	bool OnNumEnterKey(int param);
	void OnNumPrevInit(intptr_t param);
	void OnNumPrevBtnUp(void);
	void OnNumNextInit(intptr_t param);
	void OnNumNextBtnUp(void);
	void OnCheckBtnUp(void);
	bool OnConfirmResult(int result, FrForm* pForm);

	FrArea* m_pItem;                 // 0x110, "item" (icon, name and level drawn on it)
	FrEdit* m_pDesc;                 // 0x114, "desc" (the item's description)
	FrEdit* m_pItemNum;              // 0x118, "item_num" (number owned)
	FrEdit* m_pNum;                  // 0x11c, "num" (number to delete)
	FrButton* m_pNumPrev;            // 0x120, "num_prev"
	FrButton* m_pNumNext;            // 0x124, "num_next"
	unsigned long m_delItemTypeId;   // 0x128
	unsigned long m_delItemCount;    // 0x12c, number owned
};

CHECK_SIZE(FrDeleteItemDlg, 0x130);
BEGIN_LAYOUT_CHECK(FrDeleteItemDlg)
	CHECK_FIELD(FrDeleteItemDlg, m_pItem, 0x110);
	CHECK_FIELD(FrDeleteItemDlg, m_pDesc, 0x114);
	CHECK_FIELD(FrDeleteItemDlg, m_pItemNum, 0x118);
	CHECK_FIELD(FrDeleteItemDlg, m_pNum, 0x11c);
	CHECK_FIELD(FrDeleteItemDlg, m_pNumPrev, 0x120);
	CHECK_FIELD(FrDeleteItemDlg, m_pNumNext, 0x124);
	CHECK_FIELD(FrDeleteItemDlg, m_delItemTypeId, 0x128);
	CHECK_FIELD(FrDeleteItemDlg, m_delItemCount, 0x12c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_DELETEITEM_H
