// Original: d:\build\custom\temp\code\client\projectg\s5\supplypack.h
#ifndef PG_CLIENT_PROJECTG_S5_SUPPLYPACK_H
#define PG_CLIENT_PROJECTG_S5_SUPPLYPACK_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../shared/globalgamedefine.h"
#include <list>

class CExhibition;
class CWindFont;
class CBlueWindFontBig;
class CFontAnimation;
class FrButton;
class FrListBox;
struct FrListItem;

namespace S5 {

// Supply-pack opening dialog (size 0x190, the `new` of
// FrSupplyPackDlgMakeInstance). Only the members this unit defines so far
// are declared.
class FrSupplyPackDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // supplypack.h:20

	FrSupplyPackDlg(void);
	virtual ~FrSupplyPackDlg(void);

	void Initialize(unsigned long typeId);
	void UpdateData(unsigned int supplyCount);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	virtual void OnCancel(void) {}   // supplypack.h:31

	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnResultConfirm(int result, FrForm* pForm);

	void OnItemListInit(intptr_t param);
	void OnOkayBtnInit(intptr_t param);
	void OnCancelBtnInit(intptr_t param);
	void OnCancelBtnUp(void);
	void OnOkayBtnUp(void);
	void OnItemListUp(void);
	void OnBgAreaDraw(void);
	void OnItemListDraw(int param);

	virtual void OnProc(const float dt);

	unsigned long m_typeId;              // 0x110, the supply pack being opened (0 closes the dialog)
	FrListBox* m_pItemList;              // 0x114
	FrButton* m_pCancelBtn;              // 0x118
	FrButton* m_pOkayBtn;                // 0x11c
	CExhibition* m_pExhibition;          // 0x120
	float m_exhibitionAlpha;             // 0x124, fades the exhibited item in up to 255
	FrListItem* m_pSelectedItem;         // 0x128, the item whose supply is being confirmed
	CWindFont* m_pWindFont;              // 0x12c
	CBlueWindFontBig* m_pBlueFont;       // 0x130
	float m_slotDelay;                   // 0x134, seconds left before the slots stop being revealed
	int m_animating;                     // 0x138, set by UpdateData while the supply animation runs
	unsigned int m_supplyCount;          // 0x13c, how many items UpdateData received
	CFontAnimation* m_pFontAnimation;    // 0x140
	float m_slotTimer;                   // 0x144, seconds since the last slot was revealed
	float m_arrowOffset;                 // 0x148, bounces between -3 and 3
	int m_slotCounts[10];                // 0x14c, items given to each of the ten slots
	std::list<sItemInfo> m_itemList;     // 0x174
	int m_heldCount;                     // 0x180, how many of the item the player held before the supply
	int m_displayCount;                  // 0x184, the count shown, raised as each slot is revealed
	int m_slotIndex;                     // 0x188, the next slot to reveal (10: all revealed)
	UNKNOWN(0x18C, 0x190);
};

} // namespace S5

BEGIN_LAYOUT_CHECK(S5::FrSupplyPackDlg)
	CHECK_FIELD(S5::FrSupplyPackDlg, m_typeId, 0x110);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_pItemList, 0x114);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_pCancelBtn, 0x118);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_pOkayBtn, 0x11C);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_pExhibition, 0x120);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_exhibitionAlpha, 0x124);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_pSelectedItem, 0x128);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_slotDelay, 0x134);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_animating, 0x138);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_supplyCount, 0x13C);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_slotTimer, 0x144);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_arrowOffset, 0x148);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_slotCounts, 0x14C);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_pWindFont, 0x12C);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_pBlueFont, 0x130);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_pFontAnimation, 0x140);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_itemList, 0x174);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_heldCount, 0x180);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_displayCount, 0x184);
	CHECK_FIELD(S5::FrSupplyPackDlg, m_slotIndex, 0x188);
END_LAYOUT_CHECK
CHECK_SIZE(S5::FrSupplyPackDlg, 0x190);

#endif
