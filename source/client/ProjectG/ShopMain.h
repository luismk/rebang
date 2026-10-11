// Original: d:\build\custom\temp\code\client\projectg\shopmain.h
#ifndef PG_CLIENT_PROJECTG_SHOPMAIN_H
#define PG_CLIENT_PROJECTG_SHOPMAIN_H

#include <list>
#include <map>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "../client/Fresh/include/frwnd.h"
#include "../client/Fresh/include/frbutton.h"
#include "../client/Fresh/include/frtabbutton.h"
#include "../client/Fresh/include/frlistbox.h"
#include "../client/Fresh/include/frarea.h"
#include "../client/Fresh/include/frstatic.h"
#include "../client/Fresh/include/frviewer.h"
#include "../client/Fresh/include/frgroupbox.h"
#include "../client/Fresh/include/frgaugebar.h"
#include "TaskMain.h"
#include "AnotherHand.h"
#include "exhibition.h"
#include "../shared/classdefine.h"

class FrArea;
class MsgObject;
class CPartTidList;
class Bitmap;
class FrForm;
class FrGiftDlg;
class FrGraphicInterface;
class WOverlay;
class WRTTI;
class FrEdit;
class FrComboBox;
class FrTextButton;
struct IFF_ITEM_COMMON;
struct sCaddieInfo;
struct sFRESH_MSGMAP;

// The shop task. Its size (0x740) is the `new` of CShopMainMakeInstance;
// CHandOwner (0x1E0) receives the pet model loaded by m_anotherHand
// (OnAnotherHand adjusts `this` by -0x1E0). Only the members the
// transcribed handlers touch are named.
// A part the shopper wore before trying an item on (RestoreShoppersOldItem).
struct sShoppersOldItem {
	unsigned long typeId;      // 0x0, the item it was replaced by
	unsigned long tid;         // 0x4
	unsigned long id;          // 0x8
};
CHECK_SIZE(sShoppersOldItem, 0xC);

// The server's record of a bought item, byte-packed, passed as param5 of the
// shop's item messages 0xB5-0xBC. Only the members HandleMsg reads are named.
#pragma pack(push, 1)
struct sBoughtItem {
	unsigned long typeId;      // 0x0
	unsigned long id;          // 0x4
	unsigned short hours;      // 0x8
	unsigned char flag;        // 0xA
	short count;               // 0xB, the stack size; 0 keeps the item's default quantity
	UNKNOWN(0xD, 0x1D);
	char uccKey[9];            // 0x1D, the self-design key of a UCC item
};
#pragma pack(pop)
CHECK_FIELD(sBoughtItem, count, 0xB);
CHECK_FIELD(sBoughtItem, uccKey, 0x1D);

class CShopMain : public CTaskMain, public CHandOwner {
public:
	CShopMain(void);
	virtual ~CShopMain(void);

	static const WRTTI m_RTTI;

	// Nested helper struct; members not recovered yet.
	struct miniBtn_t {
		miniBtn_t(void);
	};

	// Main tab of the shop (ChangeMainTab); values known by number only.
	enum mainTab_t {
		MAIN_TAB_0 = 0
	};

	// Tab of the status box: per-part stats or the whole set's.
	enum statTab_t {
		STAT_TAB_PART = 0,
		STAT_TAB_ALL = 1
	};

	// Sub tab of the shop (ChangeSubTab); values known by number only.
	enum subTab_t {
		SUB_TAB_NONE = 0,
		SUB_TAB_CLUBSET = 6,   // ChangeClubset
		SUB_TAB_AZTEC = 7,     // ChangeAztec
		SUB_TAB_MASCOT = 12    // ChangeMascot
	};

	virtual void OnInit(void);
	virtual void OnDestroy(void);
	virtual void OnLoad(void);
	virtual void OnPreLoadInit(void);
	virtual void OnProcess(float dt);
	virtual void HandleMsg(const MsgObject &msg);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP *GetMessageMap(void) const;

	virtual void OnAnotherHand(int nWork, void *pData);

	void OnShopMainInit(int);
	void LoadShopImages(void);
	void ShowPet(unsigned long typeId);
	void SetPetCtrl(unsigned long typeId);
	void RestoreShoppersOldItem(unsigned long typeId, CPartTidList &tidList);
	void ShowMainModel(CPartTidList &tidList);
	bool HaveCharacter(unsigned long typeId);
	void OnStatPartOwnerDraw(int);
	void OnPetInit(int);
	void OnPowerBarInit(intptr_t);
	void OnControlBarInit(intptr_t);
	void OnImpactBarInit(intptr_t);
	void OnSpinBarInit(intptr_t);
	void OnCurveBarInit(intptr_t);
	void OnScrollUpInit(int);
	void OnScrollDownInit(int);
	void OnItemSlotFrame1Init(int);
	void OnItemSlotFrame2Init(int);
	void OnItemSlotInit(intptr_t);
	void OnStatAllInit(int);
	void OnStatAllOwnerDraw(int);
	void OnBuyAllInit(int);
	void OnBuyResetInit(int);
	void OnSideTabBuyInit(int);
	void OnSideTabRentalInit(int);
	void OnSideTabBuyUp(void);
	void ResetItemList(bool bKeepType1);
	bool SelectItemList(unsigned long typeId);
	bool SelectItemList(FrListItem *pItem);
	bool UnselectItemFromItemList(unsigned long typeId);
	const Bitmap *GetItemIcon(unsigned long typeId);
	void Repay(const IFF_ITEM_COMMON &item, unsigned long guid);
	bool OnSellDlgResult(int result, FrForm *form);
	void SetMyItemSlot(void);
	void BuildMyItemSlot(bool);
	void OnSideTabRentalUp(void);
	void ClearCartList(bool, CPartTidList *);
	void BuildDisplayRental(subTab_t tab);
	void OnSelectedNameInit(int);
	void OnStatFrameInit(int);
	void OnStatTextInit(int);
	void OnStatPartInit(int);
	void OnTabTxt1Init(int);
	void OnTabTxt2Init(int);
	void OnTabTxt3Init(int);
	void OnSelItemIconInit(int);
	void OnStatBoxInit(int);
	void OnSelFrameInit(int);
	void OnPowerEditInit(int);
	void OnControlEditInit(int);
	void OnImpactEditInit(int);
	void OnSpinEditInit(int);
	void OnCurveEditInit(int);
	void OnSelItemInit(int);
	void OnSelItemPrevInit(int);
	void OnSelItemNextInit(int);
	void OnItemDescBgInit(int);
	void OnTabSubInit(int);
	void OnPartCartListInit(int);
	void OnAuxPartCartListInit(int);
	void OnSkinPetInit(int);
	void OnSkinPet2Init(int);
	void BuildDisplayStand(subTab_t tab);
	void SafeLoadPet(unsigned long *pTypeId, bool bForce);
	void ChangeMascot(unsigned long typeId);
	void ChangeClubset(unsigned long typeId);
	void ChangeAztec(unsigned long typeId);
	void ChangeCharacter(unsigned long typeId);
	void ChangeCaddie(unsigned long typeId);
	void BuildVisibleCaddieList(void);

	void OnSortOptionComboDown(void);
	void OnSearchGoodsLButtonDown(void);
	void OnSelItemOwnerDraw(int itemIndex);
	void DrawSelArrow(FrGraphicInterface *graphic);
	void OnSelItemUp(void);
	void OnStatPartUp(void);
	void OnStatAllUp(void);
	bool OnGiftDlgResult(int result, FrForm *form);
	void ChangeStatTab(statTab_t tab);
	void ChangeMainTab(mainTab_t tab, bool bForce);
	void ChangeSubTab(subTab_t tab, bool bForce);
	void OnTabSubButtonUp(void);
	bool IsSelectedRentalTab(void);
	void SetAutoScroll(void);
	void Net_StartPeriodItem(unsigned long typeId);
	FrListBox::ITEM_LIST::iterator DeleteItemFromCartList(unsigned long typeId);
	FrListBox::ITEM_LIST::iterator DeleteItemFromAuxCartList(unsigned long typeId);
	bool IsVisibleInStock(IFF_ITEM_COMMON &item);
	void InitShopDialogs(void);
	void TurnOnItemStatus(void);
	void TurnOffItemStatus(void);
	void SetLevelBar(void);
	void SetVisibleSideTab(bool bVisible);
	void TurnOffAuxCartScroll(void);
	void TurnOnAuxCartScroll(void);
	void CheckAuxCartScroll(void);
	void OnScrollUpUp(void);
	void OnScrollDownUp(void);
	void TurnOnItemSelect(void);
	void TurnOffItemSelect(void);
	void HidePet(bool bWait);
	void SetPetAreaRect(void);
	virtual void CloseAllDialogs(void);
	void DrawCutinFrame(void);
	void SetItemDescRect(void);
	void ShowItemDesc(unsigned long typeId);
	void DrawStatus2(FrGraphicInterface *graphic, FrListItem *listItem, short *const status, short *const slotStatus, float x);
	void DrawStatusCard(FrGraphicInterface *graphic, FrListItem *listItem, short *const cardStatus, float x);
	void DrawRentalPrice(float x, float y, IFF_ITEM_COMMON &item, FrListItem *listItem);
	void DrawStatus3(FrGraphicInterface *graphic, float x, float y, short *const status, short *const slotStatus);
	void OpenLayout(const char *layoutName);
	bool OnReemployeeResult(int result, FrForm *form);
	void OpenCaddieInfo(const sCaddieInfo &info);
	WOverlay *GetCardImg(unsigned long typeId);
	WOverlay *GetCutinImg(unsigned long typeId);
	void InitalizeUpgradeCaddieInfo(void);
	void OnSortOptionComboInit(int);
	void OnSearchGoodsButtonInit(int);
	void OnItemDescInit(int);
	bool IsItemInCart(FrListItem *listItem);
	void InitShopControls(void);
	void LoadCardImages(void);
	void DeleteCardImages(void);
	void DeleteCutinImages(void);
	void DrawSkinFrame(void);
	void DrawSkinSlot(void);
	void DrawSelItem(FrGraphicInterface *graphic, unsigned long typeId);
	void DrawCharacterList(FrGraphicInterface *graphic, unsigned long typeId);
	void DrawCaddieList(FrGraphicInterface *graphic, unsigned long typeId);
	void OnPetOwnerDraw(int);
	void OnSelItemIconOwnerDraw(int);
	void OnItemListInit(intptr_t);
	void OnBuyHoverOn(int);
	void UnselectItemList(unsigned long typeId);

private:
	LAYOUT_CHECK_FRIEND;

	void SetLevelBar_Char(void);
	void SetLevelBar_Clubset(void);
	void Build_Mascot(subTab_t tab, int);
	void TakeOff_AuxPartNCartList(CPartTidList &tidList, unsigned long overlapMask);

	FrEdit *m_hintWnd;        // 0x1e4, the hint line box, hidden when m_hintTime runs out (OnProcess)
	float m_hintTime;         // 0x1e8, counts down while positive (OnProcess)
	FrStatic *m_selectedName; // 0x1ec (OnSelectedNameInit)
	FrArea *m_statFrame;      // 0x1f0 (OnStatFrameInit)
	FrArea *m_statText;       // 0x1f4 (OnStatTextInit)
	FrViewer *m_statPart;     // 0x1f8 (OnStatPartInit), owner-drawn by OnStatPartOwnerDraw
	FrViewer *m_statAll;      // 0x1fc (OnStatAllInit), its rect is OnStatPartOwnerDraw's second picture
	PADDING(0x200, 0x204);
	FrTabButton *m_tabSubButton; // 0x204, the sub tabs (OnTabSubButtonUp)
	FrButton *m_buyAll;       // 0x208 (OnBuyAllInit)
	FrButton *m_buyReset;     // 0x20c (OnBuyResetInit)
	FrListBox *m_itemListBox; // 0x210, the goods of the current tab (SetAutoScroll, SortItem)
	FrListBox *m_cartList;    // 0x214, the cart (DeleteItemFromCartList)
	FrListBox *m_auxCartList; // 0x218, the second cart, scrolled by OnScrollUpUp/OnScrollDownUp
	FrArea *m_tabTxt1;        // 0x21c (OnTabTxt1Init)
	FrArea *m_tabTxt2;        // 0x220 (OnTabTxt2Init)
	FrArea *m_tabTxt3;        // 0x224 (OnTabTxt3Init)
	PADDING(0x228, 0x22c);
	FrArea *m_petArea;        // 0x22c, one of the three pet areas (SetPetAreaRect)
	FrArea *m_selItemIcon;    // 0x230 (OnSelItemIconInit)
	FrButton *m_selItem;      // 0x234 (OnSelItemInit)
	FrGroupBox *m_statBox;    // 0x238 (OnStatBoxInit)
	FrArea *m_selFrame;       // 0x23c (OnSelFrameInit)
	FrGaugeBar *m_statBar[5];  // 0x240, power/control/impact/spin/curve (On<Stat>BarInit)
	FrStatic *m_statEdit[5];   // 0x254, power/control/impact/spin/curve (On<Stat>EditInit)
	FrButton *m_selItemPrev;  // 0x268 (OnSelItemPrevInit)
	FrButton *m_selItemNext;  // 0x26c (OnSelItemNextInit)
	FrButton *m_scrollUp;     // 0x270 (OnScrollUpInit)
	FrButton *m_scrollDown;   // 0x274 (OnScrollDownInit)
	FrArea *m_itemDescBg;     // 0x278 (OnItemDescBgInit)
	FrEdit *m_itemDesc;       // 0x27c, the description of the selected item (SetItemDescRect)
	FrArea *m_skinPet;        // 0x280 (OnSkinPetInit)
	FrArea *m_skinPet2;       // 0x284 (OnSkinPet2Init)
	FrArea *m_pet;            // 0x288 (OnPetInit)
	FrArea *m_itemSlotFrame1; // 0x28c (OnItemSlotFrame1Init)
	FrArea *m_itemSlotFrame2; // 0x290 (OnItemSlotFrame2Init)
	FrListBox *m_itemSlot;    // 0x294 (OnItemSlotInit)
	FrButton *m_sideTabBuy;   // 0x298 (OnSideTabBuyInit)
	FrButton *m_sideTabRental; // 0x29c (OnSideTabRentalInit)
	FrForm *m_buyDlg;         // 0x2a0, cleared by OnBuyDlgResult
	PADDING(0x2a4, 0x2a8);
	std::map<unsigned long, unsigned long> m_upgradeCaddies; // 0x2a8, caddie type id -> its upgraded type id (InitalizeUpgradeCaddieInfo)
	std::map<unsigned int, IFF_STRUCT::sCaddie> m_visibleCaddies; // 0x2b4, built by BuildVisibleCaddieList
	PADDING(0x2c0, 0x340);
	const Bitmap *m_statPartBitmap[7]; // 0x340, three per kind, the first drawn (m_statPartKind * 3)
	const Bitmap *m_statAllBitmap[3]; // 0x35c, the stat-all frame: normal / under the cursor / selected (OnStatAllOwnerDraw)
	PADDING(0x368, 0x3d8);
	WRect m_itemListNoRButtonRect1; // 0x3d8, where the right button does not act on the item list (OnItemListInit, index 1)
	PADDING(0x3e8, 0x3f4);
	WRect m_itemListNoRButtonRect2; // 0x3f4, same, index 2
	PADDING(0x404, 0x40c);
	bool m_buySideTab;        // 0x40c, true while the Buy side tab is selected (OnSideTabBuyUp)
	PADDING(0x40d, 0x480);
	std::map<unsigned long, const Bitmap *> m_itemIcons; // 0x480, icon cache by type id (GetItemIcon)
	std::map<unsigned int, WOverlay *> m_cardImgs;  // 0x48c, card picture cache by type id (GetCardImg)
	std::map<unsigned int, WOverlay *> m_cutinImgs; // 0x498, cut-in picture cache by type id (GetCutinImg)
	PADDING(0x4a4, 0x4b0);
	bool m_selItemExpanded;   // 0x4b0, toggled by OnSelItemUp
	PADDING(0x4b1, 0x4b4);
	const char *m_charLevels;          // 0x4b4, the shown character's levels (SetLevelBar_Char: GetCharLevel)
	const unsigned long *m_charParts;  // 0x4b8, the shown character's parts (SetLevelBar_Char: GetItemLevel)
	unsigned long m_charPartIds[24];   // 0x4bc, the item id worn in each of m_charParts' slots (HandleMsg 0x6A)
	const unsigned long *m_charCards;  // 0x51c, the shown character's cards (SetLevelBar_Char: GetItemLevel)
	unsigned char m_hairColor;         // 0x520, passed to CPetFrame::SetHairClr (ClearCartList)
	PADDING(0x521, 0x524);
	unsigned long m_standPetTypeId; // 0x524, the pet loaded on a side tab switch (OnSideTabBuyUp)
	unsigned long m_caddieTypeId;  // 0x528, shown on the display stand (OnSelItemIconOwnerDraw)
	unsigned long m_mascotTypeId;  // 0x52c, shown on the display stand (ChangeMascot)
	unsigned long m_aztecTypeId;   // 0x530, shown on the display stand (ChangeAztec)
	unsigned long m_clubsetTypeId; // 0x534, shown on the display stand (ChangeClubset)
	statTab_t m_statTab;      // 0x538
	int m_shopTab;            // 0x53c, the side tabs apply only while it is 0 (IsSelectedRentalTab)
	subTab_t m_subTab;        // 0x540
	float m_blinkValue;       // 0x544, |sin(m_blinkTime * 4)| * 5 (OnProcess)
	float m_blinkTime;        // 0x548, grows by the frame time (OnProcess)
	WPoint m_itemDescOffset;  // 0x54c, depends on the sub tab (SetItemDescRect)
	WPoint m_itemDescPos;     // 0x554, below the description box (OnItemDescInit)
	int m_statPartKind;       // 0x55c, selects the m_statPartBitmap triple

	CExhibition *m_petExhibition; // 0x560, the shown pet model (HidePet)
	CAnotherHand m_anotherHand;   // 0x564, loads the pet model off the main thread
	std::list<unsigned long> m_rbuttonPartTypeIds; // 0x6ec, cart parts right-clicked (OnPartCartListOwnerDraw push_back)
	std::list<sShoppersOldItem> m_shoppersOldItems; // 0x6f8, parts to put back on the pet (RestoreShoppersOldItem)
	PADDING(0x704, 0x71c);
	float m_sidePos[2];       // 0x71c, slide of the two side areas (OnProcess)
	int m_sideDir[2];         // 0x724, 0 hold at zero, 1 slide back, 2 slide out (OnProcess)
	int m_cardImgState;       // 0x72c, cleared before LoadCardImages when IsLocalContent(0x51) (ctor)
	PADDING(0x730, 0x738);
	FrComboBox *m_sortOptionCombo;     // 0x738 (OnSortOptionComboInit)
	FrTextButton *m_searchGoodsButton; // 0x73c (OnSearchGoodsButtonInit)
};
CHECK_SIZE(CShopMain, 0x740);

BEGIN_LAYOUT_CHECK(CShopMain)
	CHECK_FIELD(CShopMain, m_hintWnd, 0x1E4);
	CHECK_FIELD(CShopMain, m_hintTime, 0x1E8);
	CHECK_FIELD(CShopMain, m_blinkValue, 0x544);
	CHECK_FIELD(CShopMain, m_blinkTime, 0x548);
	CHECK_FIELD(CShopMain, m_sidePos, 0x71C);
	CHECK_FIELD(CShopMain, m_sideDir, 0x724);
	CHECK_FIELD(CShopMain, m_selItem, 0x234);
	CHECK_FIELD(CShopMain, m_statBox, 0x238);
	CHECK_FIELD(CShopMain, m_selItemPrev, 0x268);
	CHECK_FIELD(CShopMain, m_selItemNext, 0x26c);
	CHECK_FIELD(CShopMain, m_scrollUp, 0x270);
	CHECK_FIELD(CShopMain, m_scrollDown, 0x274);
	CHECK_FIELD(CShopMain, m_sideTabBuy, 0x298);
	CHECK_FIELD(CShopMain, m_sideTabRental, 0x29c);
	CHECK_FIELD(CShopMain, m_buyDlg, 0x2a0);
	CHECK_FIELD(CShopMain, m_selItemExpanded, 0x4b0);
	CHECK_FIELD(CShopMain, m_statTab, 0x538);
	CHECK_FIELD(CShopMain, m_subTab, 0x540);
	CHECK_FIELD(CShopMain, m_petExhibition, 0x560);
	CHECK_FIELD(CShopMain, m_anotherHand, 0x564);
	CHECK_FIELD(CShopMain, m_shoppersOldItems, 0x6F8);
	CHECK_FIELD(CShopMain, m_tabSubButton, 0x204);
	CHECK_FIELD(CShopMain, m_itemListBox, 0x210);
	CHECK_FIELD(CShopMain, m_cartList, 0x214);
	CHECK_FIELD(CShopMain, m_auxCartList, 0x218);
	CHECK_FIELD(CShopMain, m_shopTab, 0x53c);
	CHECK_FIELD(CShopMain, m_petArea, 0x22c);
	CHECK_FIELD(CShopMain, m_skinPet, 0x280);
	CHECK_FIELD(CShopMain, m_skinPet2, 0x284);
	CHECK_FIELD(CShopMain, m_pet, 0x288);
	CHECK_FIELD(CShopMain, m_statPart, 0x1F8);
	CHECK_FIELD(CShopMain, m_statAll, 0x1FC);
	CHECK_FIELD(CShopMain, m_statPartBitmap, 0x340);
	CHECK_FIELD(CShopMain, m_statAllBitmap, 0x35C);
	CHECK_FIELD(CShopMain, m_statPartKind, 0x55C);
	CHECK_FIELD(CShopMain, m_mascotTypeId, 0x52C);
	CHECK_FIELD(CShopMain, m_aztecTypeId, 0x530);
	CHECK_FIELD(CShopMain, m_clubsetTypeId, 0x534);
	CHECK_FIELD(CShopMain, m_selectedName, 0x1EC);
	CHECK_FIELD(CShopMain, m_statFrame, 0x1F0);
	CHECK_FIELD(CShopMain, m_statText, 0x1F4);
	CHECK_FIELD(CShopMain, m_tabTxt1, 0x21C);
	CHECK_FIELD(CShopMain, m_tabTxt2, 0x220);
	CHECK_FIELD(CShopMain, m_tabTxt3, 0x224);
	CHECK_FIELD(CShopMain, m_selItemIcon, 0x230);
	CHECK_FIELD(CShopMain, m_selFrame, 0x23C);
	CHECK_FIELD(CShopMain, m_statBar, 0x240);
	CHECK_FIELD(CShopMain, m_statEdit, 0x254);
	CHECK_FIELD(CShopMain, m_itemDescBg, 0x278);
	CHECK_FIELD(CShopMain, m_itemSlotFrame1, 0x28C);
	CHECK_FIELD(CShopMain, m_itemSlotFrame2, 0x290);
	CHECK_FIELD(CShopMain, m_itemSlot, 0x294);
	CHECK_FIELD(CShopMain, m_buyAll, 0x208);
	CHECK_FIELD(CShopMain, m_buySideTab, 0x40C);
	CHECK_FIELD(CShopMain, m_itemIcons, 0x480);
	CHECK_FIELD(CShopMain, m_cardImgs, 0x48C);
	CHECK_FIELD(CShopMain, m_cutinImgs, 0x498);
	CHECK_FIELD(CShopMain, m_standPetTypeId, 0x524);
	CHECK_FIELD(CShopMain, m_buyReset, 0x20C);
	CHECK_FIELD(CShopMain, m_itemDesc, 0x27C);
	CHECK_FIELD(CShopMain, m_upgradeCaddies, 0x2A8);
	CHECK_FIELD(CShopMain, m_itemDescOffset, 0x54C);
	CHECK_FIELD(CShopMain, m_itemDescPos, 0x554);
	CHECK_FIELD(CShopMain, m_sortOptionCombo, 0x738);
	CHECK_FIELD(CShopMain, m_searchGoodsButton, 0x73C);
	CHECK_FIELD(CShopMain, m_caddieTypeId, 0x528);
	CHECK_FIELD(CShopMain, m_charLevels, 0x4B4);
	CHECK_FIELD(CShopMain, m_charParts, 0x4B8);
	CHECK_FIELD(CShopMain, m_charPartIds, 0x4BC);
	CHECK_FIELD(CShopMain, m_charCards, 0x51C);
	CHECK_FIELD(CShopMain, m_hairColor, 0x520);
	CHECK_FIELD(CShopMain, m_rbuttonPartTypeIds, 0x6EC);
	CHECK_FIELD(CShopMain, m_itemListNoRButtonRect1, 0x3D8);
	CHECK_FIELD(CShopMain, m_itemListNoRButtonRect2, 0x3F4);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SHOPMAIN_H
