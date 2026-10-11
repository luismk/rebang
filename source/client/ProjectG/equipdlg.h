// Original: d:\build\custom\temp\code\client\projectg\equipdlg.h
#ifndef PG_CLIENT_PROJECTG_EQUIPDLG_H
#define PG_CLIENT_PROJECTG_EQUIPDLG_H

#include <map>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrListBox;
struct FrListItem;
class WTitleFont;

// Item slot equipment dialog; its size (0x228) is the `new` of
// FrEquipDlgMakeInstance. Only the members this unit defines so far are
// declared.
class FrEquipDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // equipdlg.h:21

	// A small icon button drawn over a warehouse item (OnWarehouseInit).
	class MiniBtn {
	public:
		MiniBtn(void);

		WRect rect;               // 0x0
		const Bitmap* pBitmap;    // 0x10
		UNKNOWN(0x14, 0x18);
	};

	FrEquipDlg(void);   // equipdlg.cpp:42
	virtual ~FrEquipDlg(void);   // equipdlg.cpp:73

	void Init(unsigned long* pItemSlot, bool isMine);   // equipdlg.cpp
	unsigned long* GetMyItemSlot(void);   // equipdlg.cpp:542

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // equipdlg.cpp:20

	void OnEquipSlotInit(intptr_t);   // equipdlg.cpp:196
	void OnEquipSlotBtnUp(void);   // equipdlg.cpp:208
	void OnUnequipSlotInit(intptr_t);   // equipdlg.cpp:216
	void OnUnequipSlotBtnUp(void);   // equipdlg.cpp:228
	void OnResetSlotInit(intptr_t);   // equipdlg.cpp:279
	void OnResetSlotBtnUp(void);   // equipdlg.cpp:291
	void OnCloseInit(intptr_t);   // equipdlg.cpp:300
	void OnCloseBtnUp(void);   // equipdlg.cpp:304
	void OnWarehouseInit(intptr_t);   // equipdlg.cpp:309
	void OnWarehouseOwnerDraw(int);
	void OnWarehouseBtnDown(void);   // equipdlg.cpp:405
	void OnWarehouseRBtnDown(void);   // equipdlg.cpp:411
	void OnWarehouseDblClick(void);   // equipdlg.cpp:426
	void OnItemSlotInit(intptr_t);   // equipdlg.cpp:443
	void OnItemSlotOwnerDraw(int);
	void OnItemSlotDblClick(void);   // equipdlg.cpp:526
	void OpenInformation(unsigned long typeId);
	void EquipItem(FrListItem* pItem);
	void UnequipItem(FrListItem* pItem);   // equipdlg.cpp:615

private:
	FrListBox* m_pWarehouse;                       // 0x110, owned items
	FrListBox* m_pItemSlot;                        // 0x114, the slots
	std::map<unsigned long, int> m_itemCountMap;   // 0x118, rebuilt by Init
	unsigned long m_myItemSlot[11];                // 0x124, Init copies the first 10
	const Bitmap* m_pWarehouseBnNormal;            // 0x150
	const Bitmap* m_pWarehouseBnOver;              // 0x154
	const Bitmap* m_pWarehouseBnClick;             // 0x158
	const Bitmap* m_pItemSelectBnOver;             // 0x15c
	const Bitmap* m_pItemSelectBnClick;            // 0x160
	WTitleFont* m_pTitleFont;                      // 0x164, item count font
	PADDING(0x168, 0x1b0);
	MiniBtn m_deleteBtn;                           // 0x1b0, "delete" icon of the warehouse items
	MiniBtn m_clockBtn;                            // 0x1c8, "clock" icon of the warehouse items
	PADDING(0x1e0, 0x228);
};

CHECK_SIZE(FrEquipDlg::MiniBtn, 0x18);
CHECK_FIELD(FrEquipDlg::MiniBtn, rect, 0x0);
CHECK_FIELD(FrEquipDlg::MiniBtn, pBitmap, 0x10);
CHECK_SIZE(FrEquipDlg, 0x228);
BEGIN_LAYOUT_CHECK(FrEquipDlg)
	CHECK_FIELD(FrEquipDlg, m_pWarehouse, 0x110);
	CHECK_FIELD(FrEquipDlg, m_pItemSlot, 0x114);
	CHECK_FIELD(FrEquipDlg, m_itemCountMap, 0x118);
	CHECK_FIELD(FrEquipDlg, m_myItemSlot, 0x124);
	CHECK_FIELD(FrEquipDlg, m_pWarehouseBnNormal, 0x150);
	CHECK_FIELD(FrEquipDlg, m_pItemSelectBnClick, 0x160);
	CHECK_FIELD(FrEquipDlg, m_pTitleFont, 0x164);
	CHECK_FIELD(FrEquipDlg, m_deleteBtn, 0x1b0);
	CHECK_FIELD(FrEquipDlg, m_clockBtn, 0x1c8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_EQUIPDLG_H
