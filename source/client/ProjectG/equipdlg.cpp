// Original: d:\build\custom\temp\code\client\projectg\equipdlg.cpp
#include "EquipDlg.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "bitmap.h"
#include "frbutton.h"
#include "frlistbox.h"
#include "fresh.h"
#include "wfont.h"
#include "wresrcmng.h"

#include <string.h>

// File names of the item count title font (nameless data).
// Provide a minimal placeholder to satisfy the linker. Real filenames
// are unknown; use a single null pointer entry so title font creation
// receives an empty filename list.
extern "C" char* data_ab3fe8[];

// Ensure the symbol is defined in this translation unit.
extern "C" char* data_ab3fe8[] = { NULL };

// equipdlg.cpp:18 (run-time type and factory registration of FrEquipDlg)
const WRTTI FrEquipDlg::m_RTTI("FrEquipDlg", &FrForm::m_RTTI);

IObject* FrEquipDlgMakeInstance(void) {
	return new FrEquipDlg;
}

struct __sFrEquipDlg {
	__sFrEquipDlg(void) {
		ObjectFactory().AddObjectFunctor(FrEquipDlgMakeInstance, "FrEquipDlg");
	}
};
static __sFrEquipDlg s_registerFrEquipDlg;

// equipdlg.cpp:20
const sFRESH_MSGMAP* FrEquipDlg::GetMessageMap(void) const {
	return &FrEquipDlg::_MsgMap;
}

sFRESH_MSGMAP FrEquipDlg::_MsgMap = { &FrForm::_MsgMap, &FrEquipDlg::_MsgEntries[0] };

sFRESH_ENTRY FrEquipDlg::_MsgEntries[] = {
	// equipdlg.cpp:22
	{ "additem", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEquipDlg::OnEquipSlotInit },
	// equipdlg.cpp:23
	{ "additem", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrEquipDlg::OnEquipSlotBtnUp },
	// equipdlg.cpp:24
	{ "delitem", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEquipDlg::OnUnequipSlotInit },
	// equipdlg.cpp:25
	{ "delitem", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrEquipDlg::OnUnequipSlotBtnUp },
	// equipdlg.cpp:26
	{ "reset", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEquipDlg::OnResetSlotInit },
	// equipdlg.cpp:27
	{ "reset", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrEquipDlg::OnResetSlotBtnUp },
	// equipdlg.cpp:28
	{ "close", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEquipDlg::OnCloseInit },
	// equipdlg.cpp:29
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrEquipDlg::OnCloseBtnUp },
	// equipdlg.cpp:30
	{ "warehouse", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEquipDlg::OnWarehouseInit },
	// equipdlg.cpp:31
	{ "warehouse", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEquipDlg::OnWarehouseOwnerDraw },
	// equipdlg.cpp:32
	{ "warehouse", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrEquipDlg::OnWarehouseBtnDown },
	// equipdlg.cpp:33
	{ "warehouse", FRCMD_RBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrEquipDlg::OnWarehouseRBtnDown },
	// equipdlg.cpp:34
	{ "warehouse", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&FrEquipDlg::OnWarehouseDblClick },
	// equipdlg.cpp:35
	{ "itemslot", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEquipDlg::OnItemSlotInit },
	// equipdlg.cpp:36
	{ "itemslot", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrEquipDlg::OnItemSlotOwnerDraw },
	// equipdlg.cpp:37
	{ "itemslot", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&FrEquipDlg::OnItemSlotDblClick },
	// equipdlg.cpp:38
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// equipdlg.h: no line record (empty constructor of a nested helper class)
FrEquipDlg::MiniBtn::MiniBtn(void) {
}

// equipdlg.cpp:42
FrEquipDlg::FrEquipDlg(void) {
	m_pWarehouse = 0;
	m_pItemSlot = 0;

	memset(m_myItemSlot, 0, sizeof(m_myItemSlot));

	m_pWarehouseBnNormal = g_pFresh->GetBitmap("equipitem_warehouse_bn_normal");
	m_pWarehouseBnOver = g_pFresh->GetBitmap("equipitem_warehouse_bn_over");
	m_pWarehouseBnClick = g_pFresh->GetBitmap("equipitem_warehouse_bn_click");

	m_pItemSelectBnOver = g_pFresh->GetBitmap("equipitem_itemselect_bn_over");
	m_pItemSelectBnClick = g_pFresh->GetBitmap("equipitem_itemselect_bn_click");

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = data_ab3fe8;
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:";

	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);
}

// equipdlg.cpp:73
FrEquipDlg::~FrEquipDlg(void) {
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// equipdlg.cpp:196
void FrEquipDlg::OnEquipSlotInit(intptr_t param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	if (pBtn) {
		if (GalleryMode()) {
			pBtn->Enable(false);
		} else {
			pBtn->SetPushDelay(0.0f);
		}
	}
}

// equipdlg.cpp:208
void FrEquipDlg::OnEquipSlotBtnUp(void) {
	FrListItem* pSelected = m_pWarehouse->GetSelected();
	if (pSelected) {
		EquipItem(pSelected);
	}
}

// equipdlg.cpp:216
void FrEquipDlg::OnUnequipSlotInit(intptr_t param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	if (pBtn) {
		if (GalleryMode()) {
			pBtn->Enable(false);
		} else {
			pBtn->SetPushDelay(0.0f);
		}
	}
}

// equipdlg.cpp:228
void FrEquipDlg::OnUnequipSlotBtnUp(void) {
	FrListItem* pSelected = m_pItemSlot->GetSelected();
	if (pSelected) {
		// select the next equipped slot after the one emptied
		FrListBox::ITEM_LIST::iterator it = m_pItemSlot->m_itemList.begin();
		for (; it != m_pItemSlot->m_itemList.end(); ++it) {
			if (*it == pSelected)
				break;
		}

		for (++it; it != m_pItemSlot->m_itemList.end(); ++it) {
			if ((*it)->pData) {
				m_pItemSlot->SelectItem(*it, true);
				break;
			}
		}

		UnequipItem(pSelected);
	}
}

// equipdlg.cpp:279
void FrEquipDlg::OnResetSlotInit(intptr_t param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	if (pBtn) {
		if (GalleryMode()) {
			pBtn->Enable(false);
		} else {
			pBtn->SetPushDelay(0.0f);
		}
	}
}

// equipdlg.cpp:291
void FrEquipDlg::OnResetSlotBtnUp(void) {
	FrListBox::ITEM_LIST::iterator it = m_pItemSlot->m_itemList.begin();
	for (; it != m_pItemSlot->m_itemList.end(); ++it) {
		UnequipItem(*it);
	}
}

// equipdlg.cpp:300
void FrEquipDlg::OnCloseInit(intptr_t) {
}

// equipdlg.cpp:304
void FrEquipDlg::OnCloseBtnUp(void) {
	Close((eFormRet)!GalleryMode(), true);
}

// equipdlg.cpp:309
void FrEquipDlg::OnWarehouseInit(intptr_t param) {
	m_pWarehouse = DYNAMIC_CAST(FrListBox, param);
	m_pWarehouse->UseRightButton(true);

	m_clockBtn.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "clock");
	m_clockBtn.rect = WRect(50.0f, 35.0f, m_clockBtn.pBitmap->Width(), m_clockBtn.pBitmap->Height());
	m_deleteBtn.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "delete");
	m_deleteBtn.rect = WRect(50.0f, 35.0f, m_deleteBtn.pBitmap->Width(), m_deleteBtn.pBitmap->Height());
}

// equipdlg.cpp:405
void FrEquipDlg::OnWarehouseBtnDown(void) {
}

// equipdlg.cpp:411
void FrEquipDlg::OnWarehouseRBtnDown(void) {
	FrListItem* pItem = m_pWarehouse->GetItemUnderCursor();

	if (pItem) {
		OpenInformation(((sItemInfo*)pItem->pData)->m_typeId);
	}
}

// Minimal stubs for methods referenced by other units but not yet
// implemented. These are placeholders to allow linking; behavior must
// be implemented properly later.
void FrEquipDlg::OpenInformation(unsigned long typeId) {
	// Placeholder: show item info is optional; no-op for now.
	(void)typeId;
}

void FrEquipDlg::EquipItem(FrListItem* pItem) {
	// Minimal behavior: if item has a type id pointer, set it in the
	// first empty slot and remove from warehouse selection.
	if (!pItem) return;
	unsigned long* pTypeId = (unsigned long*)pItem->pData;
	if (!pTypeId || *pTypeId == 0) return;

	for (int i = 0; i < 11; ++i) {
		if (m_myItemSlot[i] == 0) {
			m_myItemSlot[i] = *pTypeId;
			break;
		}
	}
}

void FrEquipDlg::OnWarehouseOwnerDraw(int) { }
void FrEquipDlg::OnItemSlotOwnerDraw(int) { }

// equipdlg.cpp:426
void FrEquipDlg::OnWarehouseDblClick(void) {
	if (GalleryMode())
		return;

	FrListItem* pItem = m_pWarehouse->GetItemUnderCursor();
	if (pItem) {
		EquipItem(pItem);

		FrListItem* pSelected = m_pWarehouse->GetSelected();
		if (pSelected)
			m_pWarehouse->UnselectItem(pSelected);
	}
}

// equipdlg.cpp:443
void FrEquipDlg::OnItemSlotInit(intptr_t param) {
	m_pItemSlot = DYNAMIC_CAST(FrListBox, param);
}

// equipdlg.cpp:526
void FrEquipDlg::OnItemSlotDblClick(void) {
	if (GalleryMode())
		return;

	FrListItem* pItem = m_pItemSlot->GetItemUnderCursor();
	if (pItem) {
		unsigned long* pTypeId = (unsigned long*)pItem->pData;
		if (pTypeId && *pTypeId) {
			UnequipItem(pItem);
		}
	}
}

// equipdlg.cpp:542
unsigned long* FrEquipDlg::GetMyItemSlot(void) {
	return m_myItemSlot;
}

// equipdlg.cpp:615
void FrEquipDlg::UnequipItem(FrListItem* pItem) {
	unsigned long* pTypeId = (unsigned long*)pItem->pData;

	if (pTypeId && *pTypeId) {
		// give the item back to the warehouse
		if (m_itemCountMap.find(*pTypeId) == m_itemCountMap.end()) {
			m_itemCountMap[*pTypeId] = 1;

			CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
			for (std::list<sItemInfo>::iterator it = pDoc->m_itemInfoList.begin(); it != pDoc->m_itemInfoList.end(); ++it) {
				if ((*it).m_typeId == *pTypeId) {
					m_pWarehouse->AddItem(&*it);
					break;
				}
			}
		} else {
			m_itemCountMap[*pTypeId] = m_itemCountMap[*pTypeId] + 1;
		}

		*pTypeId = 0;
	}

	m_pItemSlot->UnselectItem(pItem);
}
