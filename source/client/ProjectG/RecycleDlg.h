// Original: d:\build\custom\temp\code\client\projectg\recycledlg.h
#ifndef PG_CLIENT_PROJECTG_RECYCLEDLG_H
#define PG_CLIENT_PROJECTG_RECYCLEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include <map>

class CExhibition;
class CSimpleUI;
class WTitleFont;
class Bitmap;
class FrArea;
class FrButton;
class FrEdit;
class FrGraphicInterface;
class FrListBox;
class FrTextButton;
class FrSelectPartDlg;
class FrViewer;
struct sItemInfo;
struct FrListItem;

// A recycle recipe entry (an ingredient or a result item). Size 0x50: the
// stride of FrRecycleDlg's ingredient arrays at 0x1E8 and 0x328 (assumed to
// be this type). Only the members the written functions read are named.
struct sRecycleItem {
	unsigned long tid;       // 0x0, the item's type id
	unsigned long serial;    // 0x4, the owned item's serial (m_haveItem, SendDataRecycleItem)
	unsigned short count;    // 0x8
	unsigned char level;     // 0xA, the player level the recipe needs
	UNKNOWN(0xB, 0xC);
	short character;         // 0xC, the character the recipe is for (-1, -2: any; SetListRecycleItem)
	short index;             // 0xE, the recipe's index (ClickRecycleItem)
	UNKNOWN(0x10, 0x24);
	unsigned long singleRecycle;   // 0x24, non-zero limits the recycle number to 1 (the random sequence number)
	char randName[0x28];     // 0x28, the random sequence's name (MakeCadieMagicBoxMap)
};

CHECK_SIZE(sRecycleItem, 0x50);
BEGIN_LAYOUT_CHECK(sRecycleItem)
	CHECK_FIELD(sRecycleItem, tid, 0x0);
	CHECK_FIELD(sRecycleItem, count, 0x8);
	CHECK_FIELD(sRecycleItem, serial, 0x4);
	CHECK_FIELD(sRecycleItem, level, 0xA);
	CHECK_FIELD(sRecycleItem, character, 0xC);
	CHECK_FIELD(sRecycleItem, index, 0xE);
	CHECK_FIELD(sRecycleItem, singleRecycle, 0x24);
	CHECK_FIELD(sRecycleItem, randName, 0x28);
END_LAYOUT_CHECK

extern int g_NumLowRecycleItem;
extern int g_NumMidRecycleItem;
extern int g_NumHighRecycleItem;
extern int g_NumSpecialRecycleItem;
extern int g_NumEventRecycleItem;
extern sRecycleItem* g_ArrayRecycleItem;

bool CheckRandomRecycleItem(const sRecycleItem* pItem);

// Recycle (item mix) dialog. Size 0x630, the `new` of FrRecycleDlgMakeInstance.
// Only the members this unit's written functions use are named so far.
class FrRecycleDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // recycledlg.h:14

	// A small icon drawn over a recycle list item (OnRecycleItemListInit).
	class MiniBtn {
	public:
		MiniBtn(void);

		WRect rect;               // 0x0
		const Bitmap* pBitmap;    // 0x10
		UNKNOWN(0x14, 0x18);
	};

	FrRecycleDlg(void);
	virtual ~FrRecycleDlg(void);

	void ClickRecycleItem(void);
	void DrawArrow(void);   // recycledlg.cpp:1589
	void DrawRecycleItem(void);
	void ControlRecycleNumber(int);
	void OpenInformation(unsigned long);
	void SetListRecycleItem(int, int);
	void SendDataRecycleItem(int, int);
	void DrawPartsSlot(FrGraphicInterface* pGI, sItemInfo* pItem, FrArea* pArea);
	void SetCadieMessage(const char* msg);
	bool SetEventMessage(int level);   // recycledlg.cpp:2693
	void InitDataCharacterSelect(void);   // recycledlg.cpp:1177
	void InitListCharacterSelect(void);   // recycledlg.cpp:1188
	void EnableTotalControls(bool enable);   // recycledlg.cpp:1611
	void EnableMixControls(bool enable);   // recycledlg.cpp:1629
	void SetMaxEnableRecycle(int);   // recycledlg.cpp:1995
	void InitPositionItemData(void);   // recycledlg.cpp:958
	bool IsHaveItem(unsigned long tid);

private:
	void ReInitializeHaveItem(void);   // recycledlg.cpp:422
	void AddHaveItemMap(const sRecycleItem* pItem);   // recycledlg.cpp:427
	bool IsHaveItem(const sRecycleItem* pItem);   // recycledlg.cpp:2454
	bool IsUsableParts(sItemInfo& item);   // recycledlg.cpp:1638

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);   // recycledlg.cpp:811
	virtual void OnProc(const float dt);   // recycledlg.cpp:850
	void OnRecycleDlgInit(int);   // recycledlg.cpp:186
	void OnAboutBnInit(int);   // recycledlg.cpp:191
	void OnAboutBnUp(void);   // recycledlg.cpp:197
	void OnCharacterSelectListLBnUp(void);   // recycledlg.cpp:373
	void OnCloseBnUp(void);   // recycledlg.cpp:214
	void OnRecycleItemListLBnUp(void);   // recycledlg.cpp:463
	bool OnRecycleNumberEnterKey(int);   // recycledlg.cpp:608
	void OnArrowMinimumBnDown(void);   // recycledlg.cpp:662
	void OnArrowLeftBnUp(void);   // recycledlg.cpp:704
	void OnArrowRightBnUp(void);   // recycledlg.cpp:751
	void OnAreaPictureInit(int);   // recycledlg.cpp:774
	void OnAreaPictureOwnerDraw(void);   // recycledlg.cpp:780
	void OnArrowRightBnDown(void);   // recycledlg.cpp:721
	virtual void OnCancel(void);   // recycledlg.cpp:951
	bool OnSelectPartDlgResult(int ret, FrForm* pForm);   // recycledlg.cpp:3073
	void OnCloseBnInit(intptr_t);   // recycledlg.cpp:208
	void OnLevelLowBnInit(int);   // recycledlg.cpp:220
	void OnLevelLowBnUp(void);   // recycledlg.cpp:231
	void OnLevelMidBnInit(int);   // recycledlg.cpp:246
	void OnLevelMidBnUp(void);   // recycledlg.cpp:252
	void OnLevelHighBnInit(int);   // recycledlg.cpp:267
	void OnLevelHighBnUp(void);   // recycledlg.cpp:273
	void OnLevelSpecialBnUp(void);   // recycledlg.cpp:302
	void OnLevelEventBnUp(void);   // recycledlg.cpp:336
	void OnMixBnInit(int);   // recycledlg.cpp:614
	void OnMixBnUp(void);   // recycledlg.cpp:620
	void OnArrowMinimumBnInit(int);   // recycledlg.cpp:656
	void OnArrowLeftBnInit(int);   // recycledlg.cpp:668
	void OnArrowRightBnInit(int);   // recycledlg.cpp:715
	void OnArrowMaximumBnInit(int);   // recycledlg.cpp:762
	void OnRecycleItemNameInit(int);   // recycledlg.cpp:794
	void OnCardSlotInit(int);   // recycledlg.cpp:3095
	void OnCardSlotResultInit(int);   // recycledlg.cpp:3101
	void OnCardSlotOwnerDraw(int);   // recycledlg.cpp:3107
	void OnLevelEventBnInit(int);   // recycledlg.cpp:317
	void OnCharacterSelectListInit(int);   // recycledlg.cpp:362
	void OnRecycleItemListRBnUp(void);   // recycledlg.cpp:469
	void OnRecycleItemListInit(int);   // recycledlg.cpp:447
	void OnRecycleNumberInit(int);   // recycledlg.cpp:600
	void OnArrowLeftBnDown(void);   // recycledlg.cpp:674
	void OnSelectPartInit(int);   // recycledlg.cpp:3045
	void OnSelectPartLBDown(void);   // recycledlg.cpp:3057
	void OnLevelSpecialBnInit(int);   // recycledlg.cpp:288
	void OnCharacterSelectListOwnerDraw(int);   // recycledlg.cpp:386
	void OnArrowMaximumBnDown(void);   // recycledlg.cpp:768
	void OnCadieMessageInit(int);   // recycledlg.cpp:800

	FrButton* m_pCloseBn;                // 0x110
	FrButton* m_pLevelLowBn;             // 0x114
	FrButton* m_pLevelMidBn;             // 0x118
	FrButton* m_pLevelHighBn;            // 0x11C
	FrButton* m_pLevelSpecialBn;         // 0x120
	FrButton* m_pLevelEventBn;           // 0x124
	FrListBox* m_pCharacterSelectList;   // 0x128
	FrListBox* m_pRecycleItemList;       // 0x12C
	FrEdit* m_pRecycleNumber;            // 0x130
	FrButton* m_pMixBn;                  // 0x134
	FrButton* m_pArrowMinimumBn;         // 0x138
	FrButton* m_pArrowLeftBn;            // 0x13C
	FrButton* m_pArrowRightBn;           // 0x140
	FrButton* m_pArrowMaximumBn;         // 0x144
	FrEdit* m_pRecycleItemName;          // 0x148
	FrEdit* m_pCadieMsg;                 // 0x14C, the caddie's message box
	WTitleFont* m_pTitleFont;            // 0x150, released through g_resrcmng by the destructor
	const Bitmap* m_pItemBnNormal;       // 0x154, RECYCLEDLG "recycle_item_bn_normal"
	const Bitmap* m_pItemBnClick;        // 0x158, RECYCLEDLG "recycle_item_bn_click"
	CSimpleUI* m_pArrowUI;               // 0x15C, draws the two arrows (DrawArrow)
	int m_listLevel;                     // 0x160, the level SetListRecycleItem filled the recycle list with
	unsigned long m_characterTid[10];    // 0x164, the character type ids of the character select list
	int m_selectedIdx;                   // 0x18C, the selected recipe's index (-1: none; ClickRecycleItem)
	int m_characterIdx;                  // 0x190, the selected character list item's index
	int m_level;                         // 0x194, the recycle level shown (low 0 .. event 4)
	sRecycleItem m_resultItem;           // 0x198, the selected recipe's result item
	sRecycleItem m_needItem[4];          // 0x1E8, the ingredients the selected recipe needs
	sRecycleItem m_haveItem[4];          // 0x328, the ingredients the player put in
	int m_recycleNumber;                 // 0x468, how many times to recycle (the "recyclenumber" edit)
	int m_maxEnableRecycle;              // 0x46C, how many times the recipe can be made
	sRecycleItem m_mixedItem;            // 0x470, cleared when a mix starts and when m_mixResultTime runs out
	WPoint m_itemPos[10];         // 0x4C0, item slot positions: result, 4 needed, 4 owned, one more (InitPositionItemData)
	CExhibition* m_pExhibition;   // 0x510, the result item's model in the picture area
	bool m_arrowLeftDown;     // 0x514, cleared when the left arrow is released
	bool m_arrowRightDown;    // 0x515, cleared when the right arrow is released
	UNKNOWN(0x516, 0x518);
	float m_arrowHoldTime;    // 0x518, reset when an arrow is pressed
	bool m_mixing;            // 0x51C, set by OnMixBnUp
	UNKNOWN(0x51D, 0x520);
	int m_arrowState[2];      // 0x520, per arrow: 0 "RIGHT", 1 "RIGHT_S" (DrawArrow)
	float m_arrowBlinkTime[2];   // 0x528, per arrow: time since its state last toggled (OnProc)
	float m_mixSendDelay;     // 0x530, seconds before the mix request is sent (OnMixBnUp: 1.5)
	float m_mixResultTime;    // 0x534, seconds left before m_mixedItem is cleared
	float m_cadieMsgTime;     // 0x538, seconds left on the caddie's message (SetEventMessage: 5)
	unsigned long m_selectedTid;           // 0x53C, the selected recipe's type id (ClickRecycleItem)
	unsigned long m_selectedPart;          // 0x540, FrSelectPartDlg's result (0 when cancelled)
	bool m_partSelected;                   // 0x544
	UNKNOWN(0x545, 0x550);
	FrButton* m_pSelectPartBn;             // 0x550 ("selectpart")
	FrArea* m_pCardSlot;                   // 0x554 ("cardslot")
	FrArea* m_pCardSlotResult;             // 0x558
	FrSelectPartDlg* m_pSelectPartDlg;     // 0x55C, the open part selection dialog
	UNKNOWN(0x560, 0x5A8);
	MiniBtn m_deleteBtn;                   // 0x5A8, "delete" icon of the recycle list items
	MiniBtn m_clockBtn;                    // 0x5C0, "clock" icon of the recycle list items
	UNKNOWN(0x5D8, 0x620);
	std::map<const sRecycleItem*, int> m_haveItemMap;   // 0x620
	UNKNOWN(0x62C, 0x630);
};

CHECK_SIZE(FrRecycleDlg::MiniBtn, 0x18);
CHECK_SIZE(FrRecycleDlg, 0x630);
BEGIN_LAYOUT_CHECK(FrRecycleDlg)
	CHECK_FIELD(FrRecycleDlg, m_pCloseBn, 0x110);
	CHECK_FIELD(FrRecycleDlg, m_pLevelLowBn, 0x114);
	CHECK_FIELD(FrRecycleDlg, m_pLevelMidBn, 0x118);
	CHECK_FIELD(FrRecycleDlg, m_pLevelHighBn, 0x11C);
	CHECK_FIELD(FrRecycleDlg, m_pMixBn, 0x134);
	CHECK_FIELD(FrRecycleDlg, m_pArrowMinimumBn, 0x138);
	CHECK_FIELD(FrRecycleDlg, m_pArrowLeftBn, 0x13C);
	CHECK_FIELD(FrRecycleDlg, m_pArrowRightBn, 0x140);
	CHECK_FIELD(FrRecycleDlg, m_pArrowMaximumBn, 0x144);
	CHECK_FIELD(FrRecycleDlg, m_pRecycleItemName, 0x148);
	CHECK_FIELD(FrRecycleDlg, m_pCardSlotResult, 0x558);
	CHECK_FIELD(FrRecycleDlg, m_cadieMsgTime, 0x538);
	CHECK_FIELD(FrRecycleDlg, m_pLevelSpecialBn, 0x120);
	CHECK_FIELD(FrRecycleDlg, m_pLevelEventBn, 0x124);
	CHECK_FIELD(FrRecycleDlg, m_pCharacterSelectList, 0x128);
	CHECK_FIELD(FrRecycleDlg, m_pRecycleItemList, 0x12C);
	CHECK_FIELD(FrRecycleDlg, m_pCadieMsg, 0x14C);
	CHECK_FIELD(FrRecycleDlg, m_characterIdx, 0x190);
	CHECK_FIELD(FrRecycleDlg, m_listLevel, 0x160);
	CHECK_FIELD(FrRecycleDlg, m_selectedIdx, 0x18C);
	CHECK_FIELD(FrRecycleDlg, m_level, 0x194);
	CHECK_FIELD(FrRecycleDlg, m_arrowLeftDown, 0x514);
	CHECK_FIELD(FrRecycleDlg, m_arrowRightDown, 0x515);
	CHECK_FIELD(FrRecycleDlg, m_selectedPart, 0x540);
	CHECK_FIELD(FrRecycleDlg, m_partSelected, 0x544);
	CHECK_FIELD(FrRecycleDlg, m_pCardSlot, 0x554);
	CHECK_FIELD(FrRecycleDlg, m_pSelectPartDlg, 0x55C);
	CHECK_FIELD(FrRecycleDlg, m_haveItemMap, 0x620);
	CHECK_FIELD(FrRecycleDlg, m_pRecycleNumber, 0x130);
	CHECK_FIELD(FrRecycleDlg, m_characterTid, 0x164);
	CHECK_FIELD(FrRecycleDlg, m_needItem, 0x1E8);
	CHECK_FIELD(FrRecycleDlg, m_resultItem, 0x198);
	CHECK_FIELD(FrRecycleDlg, m_recycleNumber, 0x468);
	CHECK_FIELD(FrRecycleDlg, m_haveItem, 0x328);
	CHECK_FIELD(FrRecycleDlg, m_maxEnableRecycle, 0x46C);
	CHECK_FIELD(FrRecycleDlg, m_arrowHoldTime, 0x518);
	CHECK_FIELD(FrRecycleDlg, m_pSelectPartBn, 0x550);
	CHECK_FIELD(FrRecycleDlg, m_pTitleFont, 0x150);
	CHECK_FIELD(FrRecycleDlg, m_pArrowUI, 0x15C);
	CHECK_FIELD(FrRecycleDlg, m_pExhibition, 0x510);
	CHECK_FIELD(FrRecycleDlg, m_arrowState, 0x520);
	CHECK_FIELD(FrRecycleDlg, m_deleteBtn, 0x5A8);
	CHECK_FIELD(FrRecycleDlg, m_itemPos, 0x4C0);
	CHECK_FIELD(FrRecycleDlg, m_clockBtn, 0x5C0);
	CHECK_FIELD(FrRecycleDlg, m_pItemBnNormal, 0x154);
	CHECK_FIELD(FrRecycleDlg, m_pItemBnClick, 0x158);
	CHECK_FIELD(FrRecycleDlg, m_mixedItem, 0x470);
	CHECK_FIELD(FrRecycleDlg, m_mixing, 0x51C);
	CHECK_FIELD(FrRecycleDlg, m_arrowBlinkTime, 0x528);
	CHECK_FIELD(FrRecycleDlg, m_mixSendDelay, 0x530);
	CHECK_FIELD(FrRecycleDlg, m_mixResultTime, 0x534);
	CHECK_FIELD(FrRecycleDlg, m_selectedTid, 0x53C);
END_LAYOUT_CHECK

// Recycle help dialog: five index buttons, backward/forward buttons and a
// viewer showing help_cadie_%02d.jpg. Size 0x138, the `new` of
// FrRecycleAboutDlgMakeInstance.
class FrRecycleAboutDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // recycledlg.h:219

	FrRecycleAboutDlg(void);
	virtual ~FrRecycleAboutDlg(void);

	void SetImgAboutIndex(int);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);   // recycledlg.cpp:2846
	void OnAboutInit(int);   // recycledlg.cpp:2754
	void OnAboutViewerInit(intptr_t);   // recycledlg.cpp:2758
	void OnIndex01Init(intptr_t);   // recycledlg.cpp:2764
	void OnIndex01BnUp(void);   // recycledlg.cpp:2769
	void OnIndex02Init(intptr_t);
	void OnIndex02BnUp(void);
	void OnIndex03Init(intptr_t);
	void OnIndex03BnUp(void);
	void OnIndex04Init(intptr_t);
	void OnIndex04BnUp(void);
	void OnIndex05Init(intptr_t);
	void OnIndex05BnUp(void);   // recycledlg.cpp:2809
	void OnBackwardBnInit(intptr_t);   // recycledlg.cpp:2814
	void OnBackwardBnUp(void);   // recycledlg.cpp:2819
	void OnForwardBnInit(intptr_t);   // recycledlg.cpp:2825
	void OnForwardBnUp(void);   // recycledlg.cpp:2830
	void OnOkBnInit(intptr_t);   // recycledlg.cpp:2836
	void OnOkBnUp(void);   // recycledlg.cpp:2841

	FrTextButton* m_pIndexBn[5];   // 0x110 ("index01".."index05")
	int m_aboutIndex;              // 0x124, the shown help page (0-4)
	FrButton* m_pBackwardBn;       // 0x128
	FrButton* m_pForwardBn;        // 0x12C
	FrViewer* m_pAboutViewer;      // 0x130
	UNKNOWN(0x134, 0x138);
};

CHECK_SIZE(FrRecycleAboutDlg, 0x138);
BEGIN_LAYOUT_CHECK(FrRecycleAboutDlg)
	CHECK_FIELD(FrRecycleAboutDlg, m_pIndexBn, 0x110);
	CHECK_FIELD(FrRecycleAboutDlg, m_aboutIndex, 0x124);
	CHECK_FIELD(FrRecycleAboutDlg, m_pBackwardBn, 0x128);
	CHECK_FIELD(FrRecycleAboutDlg, m_pForwardBn, 0x12C);
	CHECK_FIELD(FrRecycleAboutDlg, m_pAboutViewer, 0x130);
END_LAYOUT_CHECK

// Part selection list of the recycle dialog. Size 0x120, the `new` of
// FrSelectPartDlgMakeInstance.
class FrSelectPartDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // recycledlg.h:266

	FrSelectPartDlg(void);
	virtual ~FrSelectPartDlg(void);

	void SetPartTID(unsigned long);
	void DrawStatus2(FrGraphicInterface* pGI, FrListItem* pItem, short* const status, short* const slotStatus, float x);   // recycledlg.cpp:3404

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitListBox(intptr_t);   // recycledlg.cpp:3256
	void OnLBUpListBox(void);   // recycledlg.cpp:3262
	void OnOwnerDrawListBox(int);

	FrListBox* m_pListBox;     // 0x110
	unsigned long m_partTid;   // 0x114, set by SetPartTID

public:
	unsigned long m_result;    // 0x118, the chosen part, read by FrRecycleDlg::OnSelectPartDlgResult

protected:
	UNKNOWN(0x11C, 0x120);
};

CHECK_SIZE(FrSelectPartDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrSelectPartDlg)
	CHECK_FIELD(FrSelectPartDlg, m_pListBox, 0x110);
	CHECK_FIELD(FrSelectPartDlg, m_partTid, 0x114);
	CHECK_FIELD(FrSelectPartDlg, m_result, 0x118);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_RECYCLEDLG_H
