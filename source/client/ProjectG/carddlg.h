// Original: d:\build\custom\temp\code\client\projectg\carddlg.h
#ifndef PG_CLIENT_PROJECTG_CARDDLG_H
#define PG_CLIENT_PROJECTG_CARDDLG_H
 
#include "frform.h"
#include <list>
#include <vector>

class FrArea;
class FrButton;
class FrEdit;
class Bitmap;
class FrGraphicInterface;
class FrListBox;
class FrStatic;
class WOverlay;
struct sSCardAvilityPeriodInfo;

// Special-card use dialog; its size (0x128) is the `new` of
// FrSpCardUseDlgMakeInstance. Only the members this unit defines so far are
// declared.
class FrSpCardUseDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // carddlg.h:24

	FrSpCardUseDlg(void);
	virtual ~FrSpCardUseDlg(void);

	// carddlg.h:30
	unsigned long& GetSpCardTid(void) {
		return m_spCardTid;
	}

	unsigned long GetOverlappedCard(void);
	void SetSpCardInfo(unsigned long& spCardTid);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // carddlg.cpp:17

	void OnItemInit(intptr_t param);    // carddlg.cpp:74
	void OnItemOwnerDraw(intptr_t nIndex);
	void OnText1Init(intptr_t param);   // carddlg.cpp:79
	void OnText2Init(intptr_t param);   // carddlg.cpp:89

	unsigned long m_overlappedCard;   // 0x110
	unsigned long m_spCardTid;        // 0x114
	FrArea* m_pItem;                  // 0x118
	FrStatic* m_pText1;               // 0x11c
	FrStatic* m_pText2;               // 0x120
	PADDING(0x124, 0x128);
};

CHECK_SIZE(FrSpCardUseDlg, 0x128);
BEGIN_LAYOUT_CHECK(FrSpCardUseDlg)
	CHECK_FIELD(FrSpCardUseDlg, m_overlappedCard, 0x110);
	CHECK_FIELD(FrSpCardUseDlg, m_spCardTid, 0x114);
	CHECK_FIELD(FrSpCardUseDlg, m_pItem, 0x118);
	CHECK_FIELD(FrSpCardUseDlg, m_pText1, 0x11c);
	CHECK_FIELD(FrSpCardUseDlg, m_pText2, 0x120);
END_LAYOUT_CHECK

// Card use dialog (size 0x128, the `new` of FrCardUseDlgMakeInstance).
class FrCardUseDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
	friend class CCardSlot;   // fills the card in (UseCard) and reads it back (On*DlgResult)
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // carddlg.h:56

	FrCardUseDlg(void);
	virtual ~FrCardUseDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // carddlg.cpp:239

	void OnItemInit(intptr_t param);   // carddlg.cpp:262
	void OnItemOwnerDraw(intptr_t nIndex);

	unsigned long m_cardId;       // 0x110, the card's id (CCardSlot::UseCard)
	unsigned long m_cardTypeId;   // 0x114, the card's type id
	unsigned long m_value118;   // 0x118
	unsigned long m_value11c;   // 0x11c
	FrArea* m_pItem;            // 0x120
	PADDING(0x124, 0x128);
};

CHECK_SIZE(FrCardUseDlg, 0x128);
BEGIN_LAYOUT_CHECK(FrCardUseDlg)
	CHECK_FIELD(FrCardUseDlg, m_cardId, 0x110);
	CHECK_FIELD(FrCardUseDlg, m_cardTypeId, 0x114);
	CHECK_FIELD(FrCardUseDlg, m_value118, 0x118);
	CHECK_FIELD(FrCardUseDlg, m_value11c, 0x11c);
	CHECK_FIELD(FrCardUseDlg, m_pItem, 0x120);
END_LAYOUT_CHECK

// Card pack opening dialog (size 0x120, the `new` of
// FrOpenCardPackDlgMakeInstance).
class FrOpenCardPackDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
	friend class CCardSlot;   // fills the card in (UseCard) and reads it back (On*DlgResult)
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // carddlg.h:81

	FrOpenCardPackDlg(void);
	virtual ~FrOpenCardPackDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // carddlg.cpp:429

	void OnLevelImgInit(intptr_t param);   // carddlg.cpp:450
	void OnItemInit(intptr_t param);       // carddlg.cpp:455
	void OnItemOwnerDraw(intptr_t nIndex);

	unsigned long m_cardId;       // 0x110, the card's id (CCardSlot::UseCard)
	unsigned long m_cardTypeId;   // 0x114, the card's type id
	FrArea* m_pItem;            // 0x118
	FrArea* m_pLevelImg;        // 0x11c
};

CHECK_SIZE(FrOpenCardPackDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrOpenCardPackDlg)
	CHECK_FIELD(FrOpenCardPackDlg, m_cardId, 0x110);
	CHECK_FIELD(FrOpenCardPackDlg, m_cardTypeId, 0x114);
	CHECK_FIELD(FrOpenCardPackDlg, m_pItem, 0x118);
	CHECK_FIELD(FrOpenCardPackDlg, m_pLevelImg, 0x11c);
END_LAYOUT_CHECK

// Result of a card pack opening (size 0x128, the `new` of
// FrOpenCardPackResultDlgMakeInstance).
class FrOpenCardPackResultDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // carddlg.h:105

	FrOpenCardPackResultDlg(void);
	virtual ~FrOpenCardPackResultDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // carddlg.cpp:537

	void OnItemInit(intptr_t param);         // carddlg.cpp:559
	void OnCardFrmNumInit(intptr_t param);   // carddlg.cpp:564
	void OnItemOwnerDraw(intptr_t nIndex);

	unsigned long m_values[3];  // 0x110
	unsigned long m_value11c;   // 0x11c, tested with m_values[1] and [2] by OnCardFrmNumInit
	FrArea* m_pItem;            // 0x120
	FrArea* m_pCardFrmNum;      // 0x124
};

CHECK_SIZE(FrOpenCardPackResultDlg, 0x128);
BEGIN_LAYOUT_CHECK(FrOpenCardPackResultDlg)
	CHECK_FIELD(FrOpenCardPackResultDlg, m_values, 0x110);
	CHECK_FIELD(FrOpenCardPackResultDlg, m_value11c, 0x11c);
	CHECK_FIELD(FrOpenCardPackResultDlg, m_pItem, 0x120);
	CHECK_FIELD(FrOpenCardPackResultDlg, m_pCardFrmNum, 0x124);
END_LAYOUT_CHECK

// Card attaching dialog (size 0x140, the `new` of
// FrCardAttachDlgMakeInstance).
class FrCardAttachDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // carddlg.h:128

	FrCardAttachDlg(void);
	virtual ~FrCardAttachDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // carddlg.cpp:645

	void OnCardAvilityInit(intptr_t param);      // carddlg.cpp:805
	void OnCardAvilityOldInit(intptr_t param);   // carddlg.cpp:811
	void OnItemInit(intptr_t param);             // carddlg.cpp:817
	void OnItemOwnerDraw(intptr_t nIndex);       // carddlg.cpp:822

public:
	bool OnSetDlgTxt(void);

	// Set by FrItemAttachCardDlg::OnOpenAttachDlg.
	unsigned long m_value110;    // 0x110, the card's id
	unsigned long m_value114;    // 0x114, the card's type id
	unsigned long m_value118;    // 0x118, the item's id
	unsigned long m_value11c;    // 0x11c, the item's type id
	unsigned long m_value120;    // 0x120, the character slot
	unsigned long m_value124;    // 0x124, the caddie slot

protected:
	unsigned long m_value128;    // 0x128
	unsigned long m_value12c;    // 0x12c
	FrArea* m_pItem;             // 0x130
	FrEdit* m_pCardAvility;      // 0x134
	FrEdit* m_pCardAvilityOld;   // 0x138
	PADDING(0x13c, 0x140);
};

CHECK_SIZE(FrCardAttachDlg, 0x140);
BEGIN_LAYOUT_CHECK(FrCardAttachDlg)
	CHECK_FIELD(FrCardAttachDlg, m_value110, 0x110);
	CHECK_FIELD(FrCardAttachDlg, m_value114, 0x114);
	CHECK_FIELD(FrCardAttachDlg, m_value118, 0x118);
	CHECK_FIELD(FrCardAttachDlg, m_value11c, 0x11c);
	CHECK_FIELD(FrCardAttachDlg, m_value120, 0x120);
	CHECK_FIELD(FrCardAttachDlg, m_value124, 0x124);
	CHECK_FIELD(FrCardAttachDlg, m_value128, 0x128);
	CHECK_FIELD(FrCardAttachDlg, m_value12c, 0x12c);
	CHECK_FIELD(FrCardAttachDlg, m_pItem, 0x130);
	CHECK_FIELD(FrCardAttachDlg, m_pCardAvility, 0x134);
	CHECK_FIELD(FrCardAttachDlg, m_pCardAvilityOld, 0x138);
END_LAYOUT_CHECK

// Moves a row of frames together (the card slots of FrItemAttachCardDlg).
class CTFrameMove {
	LAYOUT_CHECK_FRIEND;
public:
	CTFrameMove(void);
	CTFrameMove(unsigned char count);
	~CTFrameMove(void);

	void AddFrame(FrArea* pFrame);
	bool SetFrameWidth(float width);
	bool SetFrameHeight(float height);

private:
	// carddlg.h:174
	void Reset(void) {
		if (m_frames.size() != 0) {
			m_frames.clear();
		}
	}

	unsigned char m_count;           // 0x0
	std::vector<FrArea*> m_frames;   // 0x4
};

CHECK_SIZE(CTFrameMove, 0x14);
BEGIN_LAYOUT_CHECK(CTFrameMove)
	CHECK_FIELD(CTFrameMove, m_count, 0x0);
	CHECK_FIELD(CTFrameMove, m_frames, 0x4);
END_LAYOUT_CHECK

// An item of the attaching dialog's list (FrItemAttachCardDlg::m_myItemList,
// the list box data), also an equipped part (m_equipParts).
struct sItemAttachCard {
	unsigned long typeId;   // 0x0
	unsigned long id;       // 0x4
};

CHECK_SIZE(sItemAttachCard, 0x8);
CHECK_FIELD(sItemAttachCard, typeId, 0x0);
CHECK_FIELD(sItemAttachCard, id, 0x4);

// Card-to-item attaching dialog (size 0x390, the `new` of
// FrItemAttachCardDlgMakeInstance). Only the members this unit uses so far
// are declared.
class FrItemAttachCardDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
	friend class CCardSlot;   // fills the card in and reads the choice back (OnCardDlgResult, OnItemAttachCardDlgResult)
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // carddlg.h:188

	FrItemAttachCardDlg(void);
	virtual ~FrItemAttachCardDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // carddlg.cpp:950

	virtual bool OnInit(void);           // carddlg.cpp:1068
	virtual void OnProc(float fDelta);   // carddlg.cpp:1833

	void initCardSlotBtnTxt(void);        // carddlg.cpp:1080
	void OnAttachCardIconInit(int param);      // carddlg.cpp:1101
	void OnAttachCardIconOwnerDraw(void);      // carddlg.cpp:1106
	void OnAttachCardNameInit(int param);      // carddlg.cpp:1117
	void OnAttachCardAbilityInit(int param);   // carddlg.cpp:1123
	void OnAttachItemIconInit(int param);      // carddlg.cpp:1128
	void OnAttachItemIconOwnerDraw(void);      // carddlg.cpp:1133
	void OnAttachItemNameInit(int param);      // carddlg.cpp:1145
	void OnAttachItemAbilityInit(int param);   // carddlg.cpp:1151
	void OnAreaCharImgInit(int param);         // carddlg.cpp:1156
	void OnItemListFoldInit(int param);        // carddlg.cpp:1161
	void OnItemListUnfoldInit(int param);      // carddlg.cpp:1167
	void OnDlgFrm_04Init(int param);           // carddlg.cpp:1203
	void OnDlgFrm_05Init(int param);           // carddlg.cpp:1208
	void OnDlgFrm_06Init(int param);           // carddlg.cpp:1213
	void OnDlgFrm_07Init(int param);           // carddlg.cpp:1218
	void OnDlgFrm_08Init(int param);           // carddlg.cpp:1223
	void OnDlgFrm_09Init(int param);           // carddlg.cpp:1228
	void OnOpenAttachDlg(int charSlot, int caddieSlot);   // carddlg.cpp:1312
	void SetCharacterAttatchBtnTxt(unsigned long typeId, char* text);   // carddlg.cpp:1331
	void SetCaddieAttatchBtnTxt(unsigned long typeId, char* text);   // carddlg.cpp:1388
	void DrawBtnCharSlotimg(float x, float y, unsigned long cardTypeId);   // carddlg.cpp:1414
	void OnCharSlotBtnOwnerDraw_1(void);       // carddlg.cpp:1482
	void OnCharSlotBtnOwnerDraw_2(void);       // carddlg.cpp:1529
	void OnCharSlotBtnOwnerDraw_3(void);       // carddlg.cpp:1576
	void OnCharSlotBtnOwnerDraw_4(void);       // carddlg.cpp:1623
	void DrawBtnCaddieSlotimg(float x, float y, unsigned long cardTypeId);
	void OnCaddieSlotBtnOwnerDraw_1(void);     // carddlg.cpp:1670
	void OnCaddieSlotBtnOwnerDraw_2(void);     // carddlg.cpp:1716
	void OnCaddieSlotBtnOwnerDraw_3(void);     // carddlg.cpp:1762
	void OnCaddieSlotBtnOwnerDraw_4(void);     // carddlg.cpp:1809
	void OnCharSlotBtnInit_1(int param);       // carddlg.cpp:1458
	void OnCharSlotBtnDown_1(void);            // carddlg.cpp:1464
	void OnCharSlotBtnInit_2(int param);       // carddlg.cpp:1505
	void OnCharSlotBtnDown_2(void);            // carddlg.cpp:1511
	void OnCharSlotBtnInit_3(int param);       // carddlg.cpp:1552
	void OnCharSlotBtnDown_3(void);            // carddlg.cpp:1558
	void OnCharSlotBtnInit_4(int param);       // carddlg.cpp:1599
	void OnCharSlotBtnDown_4(void);            // carddlg.cpp:1605
	void OnCaddieSlotBtnInit_1(int param);     // carddlg.cpp:1646
	void OnCaddieSlotBtnDown_1(void);          // carddlg.cpp:1652
	void OnCaddieSlotBtnInit_2(int param);     // carddlg.cpp:1692
	void OnCaddieSlotBtnDown_2(void);          // carddlg.cpp:1698
	void OnCaddieSlotBtnInit_3(int param);     // carddlg.cpp:1738
	void OnCaddieSlotBtnDown_3(void);          // carddlg.cpp:1744
	void OnCaddieSlotBtnInit_4(int param);     // carddlg.cpp:1785
	void OnCaddieSlotBtnDown_4(void);          // carddlg.cpp:1791
	void OnMyItemListInit(intptr_t param);          // carddlg.cpp:1855
	void OnMyItemListOwnerDraw(intptr_t nIndex);    // carddlg.cpp:1900
	void DrawItem(WPoint pt, unsigned long typeId, unsigned long id);
	void DrawPartsSlot(WPoint pt, unsigned long typeId, unsigned long id, FrGraphicInterface* pGraphic);
	void SetCardInfo(unsigned long cardTypeId);
	void SetListMyItem(void);
	void SetPartsSlotBtnTxt(void);
	void ResetCardSlotInfo(void);
	bool OnCardAttachResult(int ret, FrForm* pForm);   // carddlg.cpp:1233
	void SetListEquipParts(void);   // carddlg.cpp:2198
	bool IsEquipParts(unsigned long typeId, unsigned long id);   // carddlg.cpp:2221
	void SetPartsSlot(unsigned long typeId);   // carddlg.cpp:2346
	void DrawPartsLine(void);   // carddlg.cpp:2566
	void SetSelectMyItem(sItemAttachCard* pItem);
	void SetListCardUsableMyItem(void);
	void AgreeAttaching(void);
	bool OnAgreeAttachingResult(int ret, FrForm* pForm);   // carddlg.cpp:2607
	void OnMyItemListRClick(void);       // carddlg.cpp:1893
	void OnItemListFoldLClick(void);     // carddlg.cpp:1174
	void OnItemListUnfoldLClick(void);   // carddlg.cpp:1189
	void OnMyItemListLClick(void);       // carddlg.cpp:1871

	unsigned long m_cardTypeId;               // 0x110
	unsigned long m_cardId;                   // 0x114
	unsigned long m_cardKind;                 // 0x118, 0 character card, 1 caddie card
	unsigned long m_itemTypeId;               // 0x11c, the selected item's
	unsigned long m_itemId;                   // 0x120
	int m_selectedSlot;                       // 0x124
	FrListBox* m_pMyItemList;                 // 0x128
	FrArea* m_pAttachCardIcon;                // 0x12c
	FrEdit* m_pAttachCardName;                // 0x130
	FrEdit* m_pAttachCardAbility;             // 0x134
	FrArea* m_pAttachItemIcon;                // 0x138
	FrEdit* m_pAttachItemName;                // 0x13c
	FrEdit* m_pAttachItemAbility;             // 0x140
	FrArea* m_pAreaCharImg;                   // 0x144
	FrButton* m_pItemListFold;                // 0x148
	FrButton* m_pItemListUnfold;              // 0x14c
	FrButton* m_pCharSlotBtn[4];              // 0x150
	FrButton* m_pCaddieSlotBtn[4];            // 0x160
	PADDING(0x170, 0x180);
	FrCardAttachDlg* m_pCardAttachDlg;        // 0x180, the open "attach_card" dialog
	const Bitmap* m_pMyItemBaseClickBmp;      // 0x184, "trade_myitem_base_click"
	const Bitmap* m_pMyItemBaseOverBmp;       // 0x188, "trade_myitem_base_over"
	sItemAttachCard* m_pSelectedItem;         // 0x18c
	std::list<sItemAttachCard> m_myItemList;  // 0x190
	std::list<sItemAttachCard> m_equipParts;  // 0x19c, the parts the owned characters wear (SetListEquipParts, IsEquipParts)
	CTFrameMove m_cardSlotMove;               // 0x1a8, moved by the item list fold
	const Bitmap* m_pEmptySlotBmp;            // 0x1bc, drawn on a character slot without a card
	const Bitmap* m_pEmptyCaddieSlotBmp;      // 0x1c0, drawn on a caddie slot without a card
	WOverlay* m_pLineOverlay;                 // 0x1c4, "line.tga"
	bool m_bItemListFolded;                   // 0x1c8
	char m_charSlotBtnTxt[4][50];             // 0x1c9
	char m_caddieSlotBtnTxt[4][50];           // 0x291
	bool m_charSlotAttached[4];               // 0x359
	bool m_caddieSlotAttached[4];             // 0x35d
	PADDING(0x361, 0x364);
	unsigned long m_charSlotCard[4];          // 0x364
	unsigned long m_caddieSlotCard[4];        // 0x374
	const Bitmap* m_pUccStateBmp[3];          // 0x384, "ucc_state_original", "ucc_state_copied", "ucc_state_temp_saved"
};

CHECK_SIZE(FrItemAttachCardDlg, 0x390);
BEGIN_LAYOUT_CHECK(FrItemAttachCardDlg)
	CHECK_FIELD(FrItemAttachCardDlg, m_cardTypeId, 0x110);
	CHECK_FIELD(FrItemAttachCardDlg, m_selectedSlot, 0x124);
	CHECK_FIELD(FrItemAttachCardDlg, m_cardId, 0x114);
	CHECK_FIELD(FrItemAttachCardDlg, m_cardKind, 0x118);
	CHECK_FIELD(FrItemAttachCardDlg, m_itemTypeId, 0x11c);
	CHECK_FIELD(FrItemAttachCardDlg, m_itemId, 0x120);
	CHECK_FIELD(FrItemAttachCardDlg, m_pMyItemList, 0x128);
	CHECK_FIELD(FrItemAttachCardDlg, m_pAttachCardIcon, 0x12c);
	CHECK_FIELD(FrItemAttachCardDlg, m_pAttachCardName, 0x130);
	CHECK_FIELD(FrItemAttachCardDlg, m_pAttachCardAbility, 0x134);
	CHECK_FIELD(FrItemAttachCardDlg, m_pAttachItemIcon, 0x138);
	CHECK_FIELD(FrItemAttachCardDlg, m_pAttachItemName, 0x13c);
	CHECK_FIELD(FrItemAttachCardDlg, m_pAttachItemAbility, 0x140);
	CHECK_FIELD(FrItemAttachCardDlg, m_pAreaCharImg, 0x144);
	CHECK_FIELD(FrItemAttachCardDlg, m_pItemListFold, 0x148);
	CHECK_FIELD(FrItemAttachCardDlg, m_pItemListUnfold, 0x14c);
	CHECK_FIELD(FrItemAttachCardDlg, m_pMyItemBaseClickBmp, 0x184);
	CHECK_FIELD(FrItemAttachCardDlg, m_pMyItemBaseOverBmp, 0x188);
	CHECK_FIELD(FrItemAttachCardDlg, m_pUccStateBmp, 0x384);
	CHECK_FIELD(FrItemAttachCardDlg, m_pCharSlotBtn, 0x150);
	CHECK_FIELD(FrItemAttachCardDlg, m_pCaddieSlotBtn, 0x160);
	CHECK_FIELD(FrItemAttachCardDlg, m_pCardAttachDlg, 0x180);
	CHECK_FIELD(FrItemAttachCardDlg, m_pSelectedItem, 0x18c);
	CHECK_FIELD(FrItemAttachCardDlg, m_myItemList, 0x190);
	CHECK_FIELD(FrItemAttachCardDlg, m_equipParts, 0x19c);
	CHECK_FIELD(FrItemAttachCardDlg, m_cardSlotMove, 0x1a8);
	CHECK_FIELD(FrItemAttachCardDlg, m_pEmptySlotBmp, 0x1bc);
	CHECK_FIELD(FrItemAttachCardDlg, m_pEmptyCaddieSlotBmp, 0x1c0);
	CHECK_FIELD(FrItemAttachCardDlg, m_pLineOverlay, 0x1c4);
	CHECK_FIELD(FrItemAttachCardDlg, m_bItemListFolded, 0x1c8);
	CHECK_FIELD(FrItemAttachCardDlg, m_charSlotBtnTxt, 0x1c9);
	CHECK_FIELD(FrItemAttachCardDlg, m_caddieSlotBtnTxt, 0x291);
	CHECK_FIELD(FrItemAttachCardDlg, m_charSlotAttached, 0x359);
	CHECK_FIELD(FrItemAttachCardDlg, m_caddieSlotAttached, 0x35d);
	CHECK_FIELD(FrItemAttachCardDlg, m_charSlotCard, 0x364);
	CHECK_FIELD(FrItemAttachCardDlg, m_caddieSlotCard, 0x374);
END_LAYOUT_CHECK

// Special-card buff list (size 0x118, the `new` of
// FrSPCardBuffDlgMakeInstance). Its position is shared by every instance.
class FrSPCardBuffDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // carddlg.h:355

	FrSPCardBuffDlg(void);
	virtual ~FrSPCardBuffDlg(void);

	const WPoint& GetPosition(void);   // carddlg.h:363, emitted by taskmain.cpp

	// carddlg.h:365
	static void SetPosition(const WPoint& pos) { m_Pos = pos; }
	// carddlg.h:366
	static void SetSpecialBuffStartTick(unsigned long tick) { m_SpecialBuffStartTick = tick; }

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // carddlg.cpp:2696

public:
	virtual bool OnInit(void);   // carddlg.cpp:2722
	virtual bool Close(eFormRet ret, bool bDestroy);   // carddlg.cpp:2736

protected:
	virtual void OnProc(float fDelta);   // carddlg.cpp:2751
	void OnSPCardListInit(intptr_t param);    // carddlg.cpp:2811
	void OnSPCardListOwnerDraw(intptr_t nIndex);

public:
	int GetSpecialCardNum(void);         // carddlg.cpp:2801
	void SetBuffItem(const std::list<sSCardAvilityPeriodInfo>& buffList, bool clear);

protected:
	int GetRemainedSec(const sSCardAvilityPeriodInfo* pInfo);

	static WPoint m_Pos;
	static unsigned long m_SpecialBuffStartTick;   // g_CurrentTime when the special buffs were last reset

	FrListBox* m_pSPCardList;   // 0x110
	PADDING(0x114, 0x118);
};

CHECK_SIZE(FrSPCardBuffDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrSPCardBuffDlg)
	CHECK_FIELD(FrSPCardBuffDlg, m_pSPCardList, 0x110);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CARDDLG_H
