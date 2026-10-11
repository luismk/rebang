// Original: d:\build\custom\temp\code\client\projectg\privatetradedlg.h
#ifndef PG_CLIENT_PROJECTG_PRIVATETRADEDLG_H
#define PG_CLIENT_PROJECTG_PRIVATETRADEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "TitleFontEx.h"

#include <string>
#include <vector>

class FrButton;
class FrEdit;
class FrListBox;
struct FrListItem;
class WRect;
class Bitmap;
struct sWarpItem;

// One item put on the trade (0xd8 bytes: the `new` and memset of the
// FrTradeToolTip constructor, the rep movsd of SetToolTip).
// A card attached to a traded item (0x18 bytes, the stride of SetToolTip's
// card loop).
// Filled by Formator from the card's sSCardAvilityPeriodInfo.
struct _sPrivateTradeCardInfo {
	unsigned long typeId;   // 0x0 (SetToolTip: zero = no card)
	unsigned long id;       // 0x4
	int effect;             // 0x8
	int effectValue;        // 0xc
	int slot;               // 0x10
	int kind;               // 0x14
};
CHECK_SIZE(_sPrivateTradeCardInfo, 0x18);
CHECK_FIELD(_sPrivateTradeCardInfo, typeId, 0x0);
CHECK_FIELD(_sPrivateTradeCardInfo, id, 0x4);
CHECK_FIELD(_sPrivateTradeCardInfo, effect, 0x8);
CHECK_FIELD(_sPrivateTradeCardInfo, effectValue, 0xc);
CHECK_FIELD(_sPrivateTradeCardInfo, slot, 0x10);
CHECK_FIELD(_sPrivateTradeCardInfo, kind, 0x14);

struct _sPrivateTradeItemSingle {
	unsigned long itemId;   // 0x0 (SetToolTip compares both ids)
	unsigned long typeId;   // 0x4 (SetToolTip: the item group in the top bits)
	short count;            // 0x8 (OnResultEditNum: the number put on the trade)
	UNKNOWN(0xa, 0xc);
	_sPrivateTradeCardInfo cards[8];   // 0xc (SetToolTip: one text line per card)
	short slotStats[5];     // 0xcc (SetToolTip: a club set's slot upgrades per stat, GetAblilityText)
	UNKNOWN(0xd6, 0xd8);
};
CHECK_SIZE(_sPrivateTradeItemSingle, 0xd8);
CHECK_FIELD(_sPrivateTradeItemSingle, itemId, 0x0);
CHECK_FIELD(_sPrivateTradeItemSingle, typeId, 0x4);
CHECK_FIELD(_sPrivateTradeItemSingle, count, 0x8);
CHECK_FIELD(_sPrivateTradeItemSingle, cards, 0xc);
CHECK_FIELD(_sPrivateTradeItemSingle, slotStats, 0xcc);

// The items and pang one side puts on the trade (0x368 bytes, the rep movsd
// of OnUpdateTradeItem). The original header is not known yet.
struct _sPrivateTradeItem {
	__int64 pang;               // 0x0 (OnUpdateTradeItem: the pang bar's amount)
	_sPrivateTradeItemSingle items[4];   // 0x8 (GetTradeItems: one per list row)
};
CHECK_SIZE(_sPrivateTradeItem, 0x368);
CHECK_FIELD(_sPrivateTradeItem, pang, 0x0);
CHECK_FIELD(_sPrivateTradeItem, items, 0x8);

// The private trade window and the warehouse list beside it. Layouts are not
// reconstructed yet; only the members the unit's functions so far use are
// declared. FrMyWareHouse and FrPrivateTradeWnd also derive from the
// drag-and-drop interface CTradeDragDrop (at 0x110).

// The item being dragged between the trade windows (12 bytes, the `new` of
// the CTradeDragDrop constructor).
struct sTradeDropItem {
	unsigned long itemId;   // 0x0 (zero = no item)
	unsigned long typeId;   // 0x4 (Formator copies it to _sPrivateTradeItemSingle::typeId)
	int count;              // 0x8

	sTradeDropItem(void) : itemId(0), typeId(0), count(1) {}
	sTradeDropItem(unsigned long i, unsigned long t) : itemId(i), typeId(t), count(1) {}
};
CHECK_SIZE(sTradeDropItem, 0xc);
CHECK_FIELD(sTradeDropItem, itemId, 0x0);
CHECK_FIELD(sTradeDropItem, typeId, 0x4);
CHECK_FIELD(sTradeDropItem, count, 0x8);

// Drag-and-drop interface of the trade windows: one shared dragged item and
// the window it was picked up from (vtable 0x0a0c8a0: destructor, Pop, three
// pure slots, SetPopItem).
class CTradeDragDrop {
public:
	CTradeDragDrop(void);
	virtual ~CTradeDragDrop(void);

	static bool IsPoped(void);
	static void Cancel(void);
	virtual bool Pop(void);

protected:
	virtual bool DropItemUp(void) = 0;      // slot 2 (presumably)
	virtual bool DropItemDown(void) = 0;    // slot 3 (called on the drop target)
	virtual void DropItemMoved(void) = 0;   // slot 4 (called on the window the item left)
	virtual void SetPopItem(const sTradeDropItem& item);

public:
	static void Drop(CTradeDragDrop* pTarget);
	const sTradeDropItem* GetDropItem(void);

private:
	static CTradeDragDrop* m_pCaller;   // the window the item was picked up from
	static sTradeDropItem* m_pItem;     // the dragged item
};
CHECK_SIZE(CTradeDragDrop, 0x4);

// Number entry box ("NumEdit"). Its size (0x128) is the `new` of
// FrNumEditMakeInstance.
class FrNumEdit : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // privatetradedlg.h:82

	FrNumEdit(void);
	virtual ~FrNumEdit(void);

	unsigned long GetValue(void) const { return m_value; }                // privatetradedlg.h:95
	void SetReturnParam(void* param) { m_pReturnParam = param; }         // privatetradedlg.h:98
	void* GetReturnParam(void) { return m_pReturnParam; }                // privatetradedlg.h:99

	void SetValue(unsigned long value);
	void SetEditRange(unsigned long minValue, unsigned long maxValue);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

public:
	void OnViewDraw(void);
	void OnEditInit(intptr_t param);
	bool OnEditEnter(intptr_t param);

protected:
	virtual void OnProc(const float dt);

private:
	FrEdit* m_pEdit;                // 0x110 (SetEditRange: the char limit)
	unsigned long m_value;          // 0x114 (GetValue)
	void* m_pReturnParam;           // 0x118 (SetReturnParam)
	unsigned long m_minValue;       // 0x11c (SetEditRange)
	unsigned long m_maxValue;       // 0x120 (SetEditRange; 0x7fffffff by default)
	UNKNOWN(0x124, 0x128);
};
BEGIN_LAYOUT_CHECK(FrNumEdit)
	CHECK_FIELD(FrNumEdit, m_pEdit, 0x110);
	CHECK_FIELD(FrNumEdit, m_value, 0x114);
	CHECK_FIELD(FrNumEdit, m_pReturnParam, 0x118);
	CHECK_FIELD(FrNumEdit, m_minValue, 0x11c);
	CHECK_FIELD(FrNumEdit, m_maxValue, 0x120);
END_LAYOUT_CHECK
CHECK_SIZE(FrNumEdit, 0x128);

// Pang amount entry box ("EditPang"). Its size (0x128) is the `new` of
// FrPangEditMakeInstance.
class FrPangEdit : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // privatetradedlg.h:118

	FrPangEdit(void);
	virtual ~FrPangEdit(void);

	void SetTexRate(unsigned char rate) { m_texRate = rate; }                // privatetradedlg.h:131
	__int64 GetTex(void) { return m_texRate * m_pangValue / 100; }          // privatetradedlg.h:132
	__int64 GetPangValue(void) const { return m_pangValue; }                // privatetradedlg.h:133

	void SetPangValue(__int64 value);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

public:
	void OnViewDraw(void);
	void OnEditInit(intptr_t param);
	bool OnEditEnter(intptr_t param);

protected:
	virtual void OnProc(const float dt);

private:
	FrEdit* m_pEdit;                // 0x110 (presumably, as FrNumEdit)
	UNKNOWN(0x114, 0x118);
	__int64 m_pangValue;            // 0x118 (GetPangValue)
	unsigned char m_texRate;        // 0x120 (SetTexRate, a percentage)
	UNKNOWN(0x121, 0x128);
};
BEGIN_LAYOUT_CHECK(FrPangEdit)
	CHECK_FIELD(FrPangEdit, m_pEdit, 0x110);
	CHECK_FIELD(FrPangEdit, m_pangValue, 0x118);
	CHECK_FIELD(FrPangEdit, m_texRate, 0x120);
END_LAYOUT_CHECK
CHECK_SIZE(FrPangEdit, 0x128);

// A pang amount drawn with the wind font, optionally inside a bar (size 0x20).
class EncapsulatedPang : public CWindFont {
	LAYOUT_CHECK_FRIEND;

public:
	// Drawing styles, a bit set (names invented: /Zd; 6 by default).
	enum eStyle {
		STYLE_NONE = 0,
		STYLE_EDITABLE = 8     // FrPrivateTradeWnd::OnProc opens the pang editor on a click
	};

	EncapsulatedPang(void);
	virtual ~EncapsulatedPang(void);

	__int64 GetPang(void) const { return m_pang; }                 // privatetradedlg.h:164
	const WRect* GetRectPtr(void) const { return m_pRect; }       // privatetradedlg.h:165
	void SetPang(__int64 pang) { m_pang = pang; }                 // privatetradedlg.h:169

	void SetRect(const WRect& rect);
	int IsStyle(eStyle style);
	int SubPang(__int64 pang);
	void AddPangBarStyle(eStyle style);

protected:
	void DrawPang(void);

private:
	__int64 m_pang;         // 0x10
	WRect* m_pRect;         // 0x18 (SetRect, allocated on first use)
	int m_style;            // 0x1c (eStyle bits)
};
BEGIN_LAYOUT_CHECK(EncapsulatedPang)
	CHECK_FIELD(EncapsulatedPang, m_pang, 0x10);
	CHECK_FIELD(EncapsulatedPang, m_pRect, 0x18);
	CHECK_FIELD(EncapsulatedPang, m_style, 0x1c);
END_LAYOUT_CHECK
CHECK_SIZE(EncapsulatedPang, 0x20);

// Category buttons of the warehouse list (names invented: /Zd).
enum eSetItemButton {
	SET_ITEM_BUTTON_PART = 0,
	SET_ITEM_BUTTON_CLUB = 1,
	SET_ITEM_BUTTON_ITEM = 2,
	SET_ITEM_BUTTON_CARD = 3
};

// Sort orders of the warehouse list (the sort list's row index; names
// invented: /Zd).
enum eSetSortType {
	SET_SORT_TYPE_DEFAULT = 0
};

// The player's warehouse list with its category buttons and sort list. The
// EncapsulatedPang base (at 0x118) draws the player's pang.
class FrMyWareHouse : public FrForm, public CTradeDragDrop, public EncapsulatedPang {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // privatetradedlg.h:192

	FrMyWareHouse(void);
	virtual ~FrMyWareHouse(void);

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	bool SubListItemCount(unsigned long guid, unsigned long typeId, int count);
	bool AddListItemCount(unsigned long guid, unsigned long typeId, int count);
	bool AddListItem(unsigned long guid, unsigned long typeId, int count);
	bool SetListItem(unsigned long guid, unsigned long typeId, int count);
	int GetListItemCount(unsigned long guid, unsigned long typeId);
	bool EraseListItem(unsigned long guid, unsigned long typeId);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool DropItemUp(void);   // privatetradedlg.cpp:1352
	virtual bool DropItemDown(void);
	virtual void DropItemMoved(void);

	void OnViewDraw(void);
	void OnListInit(intptr_t param);
	void OnListDraw(intptr_t param);
	void OnListDblClick(void);
	void OnSortInit(intptr_t param);
	void OnSortDraw(intptr_t param);
	void OnSortDown(int param);
	void OnPartBtnInit(int param);
	void OnClubBtnInit(int param);
	void OnItemBtnInit(int param);
	void OnCardBtnInit(int param);
	void OnPartBtnDown(void);
	void OnClubBtnDown(void);
	void OnItemBtnDown(void);
	void OnCardBtnDown(void);

private:
	FrListItem* FindListItem(unsigned long guid, unsigned long typeId);
	void SetButtonStyle(int param, eSetItemButton button);
	void InitSortList(eSetItemButton button);
	void SetItemList(eSetItemButton button, eSetSortType sortType);
	void UnPushedButtonAll(void);

	std::vector<sWarpItem*> m_warpItems;  // 0x138 (owned; deleted by the destructor)
	std::vector<FrButton*> m_buttons;     // 0x148 (indexed by eSetItemButton)
	FrListBox* m_pItemList;               // 0x158 ("ItemList", the sWarpItem rows)
	FrListBox* m_pSortList;               // 0x15c ("SortItem", presumably)
	eSetItemButton m_curButton;           // 0x160 (presumably; OnSortDown)
	UNKNOWN(0x164, 0x168);
};
BEGIN_LAYOUT_CHECK(FrMyWareHouse)
	CHECK_FIELD(FrMyWareHouse, m_warpItems, 0x138);
	CHECK_FIELD(FrMyWareHouse, m_buttons, 0x148);
	CHECK_FIELD(FrMyWareHouse, m_pItemList, 0x158);
	CHECK_FIELD(FrMyWareHouse, m_pSortList, 0x15c);
	CHECK_FIELD(FrMyWareHouse, m_curButton, 0x160);
END_LAYOUT_CHECK
CHECK_SIZE(FrMyWareHouse, 0x168);

// Tool tip of a dragged trade item.
class FrTradeToolTip : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // privatetradedlg.h:253

	FrTradeToolTip(void);
	virtual ~FrTradeToolTip(void);

	void SetDragDraw(int bDraw) { m_bDragDraw = bDraw; }   // privatetradedlg.h:267
	void SetDragItem(const sTradeDropItem* pItem);
	void SetToolTip(_sPrivateTradeItemSingle* pItem);

private:
	void MakeFormatText(const std::string& text, std::vector<std::string>& lines, int param);

	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnViewDraw(void);
	void OnTextListInit(intptr_t param);
	void OnTextListDraw(intptr_t param);

	virtual void OnProc(const float dt);

private:
	float m_alpha;                          // 0x110 (OnProc fades it; the text alpha)
	FrListBox* m_pTextList;                 // 0x114 ("TextList")
	const Bitmap* m_pIcon;                  // 0x118 (presumably; OnViewDraw draws only when set)
	std::vector<std::string> m_textLines;   // 0x11c (SetToolTip, MakeFormatText)
	_sPrivateTradeItemSingle* m_pItem;      // 0x12c (the item shown, SetToolTip)
	int m_bDragDraw;                        // 0x130 (SetDragDraw)
	const sTradeDropItem* m_pDragItem;      // 0x134 (SetDragItem)
};
CHECK_SIZE(FrTradeToolTip, 0x138);
BEGIN_LAYOUT_CHECK(FrTradeToolTip)
	CHECK_FIELD(FrTradeToolTip, m_alpha, 0x110);
	CHECK_FIELD(FrTradeToolTip, m_pTextList, 0x114);
	CHECK_FIELD(FrTradeToolTip, m_pIcon, 0x118);
	CHECK_FIELD(FrTradeToolTip, m_textLines, 0x11c);
	CHECK_FIELD(FrTradeToolTip, m_pItem, 0x12c);
	CHECK_FIELD(FrTradeToolTip, m_bDragDraw, 0x130);
	CHECK_FIELD(FrTradeToolTip, m_pDragItem, 0x134);
END_LAYOUT_CHECK

// One side of the trade (item list and the four number-entry buttons). The
// EncapsulatedPang base (at 0x118) is the pang put on the trade.
class FrPrivateTradeWnd : public FrForm, public CTradeDragDrop, public EncapsulatedPang {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // privatetradedlg.h:299

	FrPrivateTradeWnd(void);
	virtual ~FrPrivateTradeWnd(void);

	virtual bool OnInit(void);
	void OnUpdateTradeItem(const _sPrivateTradeItem& items);

	void SetNick(const std::string& nick) { m_nick = nick; }                 // privatetradedlg.h:333

	void EnableActiveControl(bool bEnable) { m_bActiveControl = bEnable; }   // privatetradedlg.h:349
	void SetAvatarIndex(int index);
	void GetTradeItems(_sPrivateTradeItem& items);
	void DoConfirm(void);
	void DoPermit(void);
	void DoTradeUnLock(void);
	void SubItem(const sTradeDropItem* pItem);
	void AddItem(const sTradeDropItem* pItem);
	bool OnResultEditNum(int result, FrForm* pForm);
	bool OnResultEditPang(int result, FrForm* pForm);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool DropItemUp(void);   // privatetradedlg.cpp:2228
	virtual bool DropItemDown(void);
	virtual void DropItemMoved(void);

	void OnViewDraw(void);
	void OnListInit(intptr_t param);
	void OnListDraw(intptr_t param);
	void OnListDown(void);
	void OnListDblClick(void);
	void OnNumEditBtn01Init(intptr_t param);
	void OnNumEditBtn01Down(void);
	void OnNumEditBtn02Init(intptr_t param);
	void OnNumEditBtn02Down(void);
	void OnNumEditBtn03Init(intptr_t param);
	void OnNumEditBtn03Down(void);
	void OnNumEditBtn04Init(intptr_t param);
	void OnNumEditBtn04Down(void);

	virtual void OnProc(const float dt);

	void OnSendItemUpdate(void);
	void OpenNumEditWnd(int index);
	void OpenPangEditWnd(void);

private:
	std::string m_nick;       // 0x138 (SetNick: the side's owner)
	int m_avatarIndex;        // 0x154
	FrListBox* m_pItemList;   // 0x158 ("ItemList", sTradeDropItem rows)
	FrPangEdit* m_pPangEditWnd;   // 0x15c (the open pang editor; cleared by OnResultEditPang)
	std::vector<FrButton*> m_buttons;   // 0x160 (the four number-entry buttons)
	FrNumEdit* m_pNumEditWnd;     // 0x170 (the open number editor; cleared by OnResultEditNum)
	bool m_bActiveControl;    // 0x174 (EnableActiveControl; cleared by DoPermit)
	char m_tradeState;        // 0x175 (1 = confirmed, 2 = permitted)
	UNKNOWN(0x176, 0x178);
	_sPrivateTradeItem* m_pTradeItem;   // 0x178 (OnUpdateTradeItem)
	UNKNOWN(0x17c, 0x180);
};
BEGIN_LAYOUT_CHECK(FrPrivateTradeWnd)
	CHECK_FIELD(FrPrivateTradeWnd, m_nick, 0x138);
	CHECK_FIELD(FrPrivateTradeWnd, m_buttons, 0x160);
	CHECK_FIELD(FrPrivateTradeWnd, m_pPangEditWnd, 0x15c);
	CHECK_FIELD(FrPrivateTradeWnd, m_pNumEditWnd, 0x170);
	CHECK_FIELD(FrPrivateTradeWnd, m_pTradeItem, 0x178);
	CHECK_FIELD(FrPrivateTradeWnd, m_avatarIndex, 0x154);
	CHECK_FIELD(FrPrivateTradeWnd, m_pItemList, 0x158);
	CHECK_FIELD(FrPrivateTradeWnd, m_bActiveControl, 0x174);
	CHECK_FIELD(FrPrivateTradeWnd, m_tradeState, 0x175);
END_LAYOUT_CHECK
CHECK_SIZE(FrPrivateTradeWnd, 0x180);

// The private trade dialog (Confirm / Permit / Cancel buttons). The
// EncapsulatedPang base sits at 0x110 (the destructor's vtable stores and the
// ??_E thunk adjusting by 0x110).
class FrPrivateTrade : public FrForm, public EncapsulatedPang {
	LAYOUT_CHECK_FRIEND;
	friend class FrPrivateTradeWnd;   // OnSendItemUpdate unlocks both sides directly
	friend class FrMyWareHouse;       // SetItemList reads the player's side directly

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // privatetradedlg.h:381

	FrPrivateTrade(void);
	virtual ~FrPrivateTrade(void);

	void SetCompanion(const std::string &nickName, unsigned long guid, unsigned long typeId);
	void GetMyTradeItems(_sPrivateTradeItem& items);
	void SetTradeUnLockAll(void);
	void ButtonContolByConfirmCancel(void);
	void UpdateFrTradeItem(const _sPrivateTradeItem& items);
	void FinalFrWait(const _sPrivateTradeItem& items);
	void FinalFrOK(const _sPrivateTradeItem& items);
	void MoveItem(const char* from, const void* pData);

	virtual bool OnInit(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnViewDraw(void);
	void OnInitConfirmBtn(intptr_t param);
	void OnInitPermitBtn(intptr_t param);
	void OnInitCancelBtn(intptr_t param);
	void OnDownConfirmBtn(void);
	void OnDownPermitBtn(void);
	void OnDownCancelBtn(void);

	virtual void OnProc(const float dt);

private:
	FrPrivateTradeWnd* m_pCompanionTradeWnd;   // 0x130 (presumably the partner's side)
	FrPrivateTradeWnd* m_pMyTradeWnd;          // 0x134 (GetMyTradeItems: the player's side)
	FrTradeToolTip* m_pToolTip;                // 0x138 (OnInit)
	FrMyWareHouse* m_pWareHouse;               // 0x13c (OnInit)
	const sTradeDropItem* m_pDragItem;         // 0x140 (handed to the tool tip's SetDragItem)
	FrButton* m_pConfirmBtn;                   // 0x144 (OnInitConfirmBtn)
	FrButton* m_pPermitBtn;                    // 0x148 (OnInitPermitBtn)
	FrButton* m_pCancelBtn;                    // 0x14c (OnInitCancelBtn)
	std::string m_companionNick;        // 0x150, the trade partner's nickname
	unsigned long m_companionGuid;      // 0x16C
	unsigned long m_companionTypeId;    // 0x170, the partner's character type id
	UNKNOWN(0x174, 0x178);
};
BEGIN_LAYOUT_CHECK(FrPrivateTrade)
	CHECK_FIELD(FrPrivateTrade, m_pCompanionTradeWnd, 0x130);
	CHECK_FIELD(FrPrivateTrade, m_pMyTradeWnd, 0x134);
	CHECK_FIELD(FrPrivateTrade, m_pToolTip, 0x138);
	CHECK_FIELD(FrPrivateTrade, m_pWareHouse, 0x13c);
	CHECK_FIELD(FrPrivateTrade, m_pDragItem, 0x140);
	CHECK_FIELD(FrPrivateTrade, m_pConfirmBtn, 0x144);
	CHECK_FIELD(FrPrivateTrade, m_pPermitBtn, 0x148);
	CHECK_FIELD(FrPrivateTrade, m_pCancelBtn, 0x14c);
	CHECK_FIELD(FrPrivateTrade, m_companionNick, 0x150);
	CHECK_FIELD(FrPrivateTrade, m_companionGuid, 0x16C);
	CHECK_FIELD(FrPrivateTrade, m_companionTypeId, 0x170);
END_LAYOUT_CHECK
CHECK_SIZE(FrPrivateTrade, 0x178);

#endif // PG_CLIENT_PROJECTG_PRIVATETRADEDLG_H
