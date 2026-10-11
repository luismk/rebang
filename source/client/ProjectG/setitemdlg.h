// Original: d:\build\custom\temp\code\client\projectg\setitemdlg.h
#ifndef PG_CLIENT_PROJECTG_SETITEMDLG_H
#define PG_CLIENT_PROJECTG_SETITEMDLG_H

#include "..\..\defines.h"
#include "frform.h"
#include "ItemManager.h"

class FrArea;
class FrEdit;
class FrListBox;

// Set-item dialog: a portrait, a description edit and the list of the set's
// component items; its size (0x120) is the `new` of FrSetItemDlgMakeInstance.
// The destructor is implicit (a bare jump to ~FrForm, no line marks).
class FrSetItemDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // setitemdlg.h:11

	FrSetItemDlg(void);

	void SetSetItem(unsigned long typeId);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnPortraitInit(intptr_t param);
	void OnNameInit(intptr_t param);
	void OnComponentInit(intptr_t param);
	void OnComponentOwnerDraw(intptr_t param);

private:
	FrArea* m_pPortrait;                 // 0x110 ("portrait")
	FrEdit* m_pName;                     // 0x114 ("name")
	FrListBox* m_pComponent;             // 0x118 ("component")
	IFF_STRUCT::sSetItem* m_pSetItem;    // 0x11C, FindSetItem's result (SetSetItem)
};

CHECK_SIZE(FrSetItemDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrSetItemDlg)
	CHECK_FIELD(FrSetItemDlg, m_pPortrait, 0x110);
	CHECK_FIELD(FrSetItemDlg, m_pName, 0x114);
	CHECK_FIELD(FrSetItemDlg, m_pComponent, 0x118);
	CHECK_FIELD(FrSetItemDlg, m_pSetItem, 0x11C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SETITEMDLG_H
