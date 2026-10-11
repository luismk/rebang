// Original: d:\build\custom\temp\code\client\projectg\tradedlg.h
#ifndef PG_CLIENT_PROJECTG_TRADEDLG_H
#define PG_CLIENT_PROJECTG_TRADEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "frlistbox.h"
#include "ToolTipManager.h"
#include "../shared/globalnetworkdefine.h"

#include <list>
#include <vector>

class Bitmap;
class FrArea;
class FrButton;
class FrEdit;
class FrListBox;
class FrStatic;
class WTitleFont;
struct sTradeItem;

// How a personal shop sells its items; 2 sells the whole list as one package
// (the only value the code compares against). Enumerator names invented.
enum eTypeSalesTrade {
	TYPE_SALES_SINGLE = 1,   // FrTradeEditDlg::OnOpenBnUp stores 1 or 2
	TYPE_SALES_PACKAGE = 2
};

// Result of a trade request (FrTradeEditDlg::SendTradeTitle); anything but
// 1 is reported through SetTradeResultCode. Enumerator name invented.
enum eResultTrade {
	RESULT_TRADE_OK = 1
};

// tradedlg.h:13 (constructor). Size not established; the constructor
// clears the first two dwords.
struct sPartEquipInfo {
	sPartEquipInfo(void);

	unsigned long typeId;   // 0x0
	unsigned long guid;     // 0x4
};
CHECK_FIELD(sPartEquipInfo, typeId, 0x0);
CHECK_FIELD(sPartEquipInfo, guid, 0x4);

// Sizes from the operator new of each XxxMakeInstance. The accessors are
// defined in the class body in the original (tradedlg.h line comments); this
// unit emitted them, so they are defined out of line in tradedlg.cpp.
class FrTradeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // tradedlg.h:29

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // tradedlg.cpp:363

public:
	FrTradeDlg(void);
	virtual ~FrTradeDlg(void);

	class MiniBtn {
	public:
		MiniBtn(void);
	};

	eTypeSalesTrade GetPackageSale(void) const;   // tradedlg.h:140
	bool IsPackageSale(void);                     // tradedlg.h:141

	void DrawPang(void);
	void OpenTradeShop(unsigned long uid);   // tradedlg.cpp:1185
	void SetListTradeItem(bool bClear);      // tradedlg.cpp:1290
	void UpdateTradeTitle(void);     // tradedlg.cpp:1368
	void UpdateCountVisitor(void);   // tradedlg.cpp:1378

protected:
	void OnTradeDlgInit(int);                // tradedlg.cpp:513
	void OnVisitorBackInit(int);             // tradedlg.cpp:518
	void OnVisitorInit(int);                 // tradedlg.cpp:984
	void OnVisitorBackOwnerDraw(void);       // tradedlg.cpp:524
	void OnTradeItemListLClick(void);        // tradedlg.cpp:552
	void OnTradeItemListRClick(void);        // tradedlg.cpp:625
	void OnTradeItemListOwnerDraw(int);      // tradedlg.cpp:722
	void OnMyPangInit(intptr_t);                  // tradedlg.cpp:529
	void OnMyPangOwnerDraw(void);            // tradedlg.cpp:535
	void OnTradeItemListInit(int);           // tradedlg.cpp:541
	void OnNoItem01Init(int);                // tradedlg.cpp:962
	void OnNoItem02Init(int);                // tradedlg.cpp:973
	void OnUserIdInit(int);                  // tradedlg.cpp:992
	void OnTitleInit(intptr_t);                   // tradedlg.cpp:998
	void OnEditBnInit(int);                  // tradedlg.cpp:1006
	void OnEditBnUp(void);                   // tradedlg.cpp:1012
	void OnCloseBnInit(intptr_t);                 // tradedlg.cpp:1041
	void OnHideBnInit(int);                  // tradedlg.cpp:1060
	void OnExitBnInit(int);                  // tradedlg.cpp:1080
	void OnPackagePurchaseButtonInit(int);   // tradedlg.cpp:1099
	void OnPackagePurchaseLButtonUp(void);   // tradedlg.cpp:1104
	void OnCloseBnUp(void);                  // tradedlg.cpp:1047
	void OnHideBnUp(void);                   // tradedlg.cpp:1066
	void OnExitBnUp(void);                   // tradedlg.cpp:1086
	virtual bool OnInit(void);               // tradedlg.cpp:1150

public:
	bool OnTradeBuyDlgResult(int ret, FrForm* pForm);   // tradedlg.cpp:1390
	void EnableSendingServer(bool bSending);       // tradedlg.cpp:1401
	void ShowStaticOutOfStock(bool bShow);         // tradedlg.cpp:1412
	void EnableHideBn(bool bEnable);               // tradedlg.cpp:1421
	void SetPackageSale(eTypeSalesTrade type);     // tradedlg.cpp:1430

protected:
	// The controls are the windows the OnXxxInit handlers store.
	FrArea* m_pVisitorBack;            // 0x110
	FrListBox* m_pTradeItemList;       // 0x114
	FrStatic* m_pNoItem01;             // 0x118
	FrStatic* m_pNoItem02;             // 0x11c
	FrEdit* m_pCountVisitor;           // 0x120, UpdateCountVisitor
	FrEdit* m_pUserId;                 // 0x124
	FrEdit* m_pTitleEdit;              // 0x128, the shop's title (UpdateTradeTitle)
	FrButton* m_pEditBn;               // 0x12c
	FrButton* m_pCloseBn;              // 0x130
	FrButton* m_pHideBn;               // 0x134
	FrButton* m_pExitBn;               // 0x138
	UNKNOWN(0x13c, 0x158);
	WTitleFont* m_pTitleFont;          // 0x158, released by the destructor
	WTitleFont* m_pPangFont;           // 0x15c, DrawPang
	bool m_bMyShop;                    // 0x160, the shop opened by OpenTradeShop is the local player's
	UNKNOWN(0x161, 0x164);
	unsigned long m_shopOwnerUID;      // 0x164, OpenTradeShop
	UNKNOWN(0x168, 0x188);
	bool m_bSendingServer;             // 0x188
	UNKNOWN(0x189, 0x190);
	FrButton* m_pPackagePurchaseButton;   // 0x190
	UNKNOWN(0x194, 0x198);
	__int64 m_totalPrice;              // 0x198, SetListTradeItem (the shop's total price)
	eTypeSalesTrade m_packageSale;     // 0x1a0
	UNKNOWN(0x1a4, 0x220);
};
CHECK_SIZE(FrTradeDlg, 0x220);
BEGIN_LAYOUT_CHECK(FrTradeDlg)
	CHECK_FIELD(FrTradeDlg, m_pVisitorBack, 0x110);
	CHECK_FIELD(FrTradeDlg, m_pTradeItemList, 0x114);
	CHECK_FIELD(FrTradeDlg, m_pNoItem01, 0x118);
	CHECK_FIELD(FrTradeDlg, m_pNoItem02, 0x11c);
	CHECK_FIELD(FrTradeDlg, m_pCountVisitor, 0x120);
	CHECK_FIELD(FrTradeDlg, m_pUserId, 0x124);
	CHECK_FIELD(FrTradeDlg, m_pEditBn, 0x12c);
	CHECK_FIELD(FrTradeDlg, m_pCloseBn, 0x130);
	CHECK_FIELD(FrTradeDlg, m_pHideBn, 0x134);
	CHECK_FIELD(FrTradeDlg, m_pExitBn, 0x138);
	CHECK_FIELD(FrTradeDlg, m_pTitleFont, 0x158);
	CHECK_FIELD(FrTradeDlg, m_pPangFont, 0x15c);
	CHECK_FIELD(FrTradeDlg, m_bMyShop, 0x160);
	CHECK_FIELD(FrTradeDlg, m_shopOwnerUID, 0x164);
	CHECK_FIELD(FrTradeDlg, m_bSendingServer, 0x188);
	CHECK_FIELD(FrTradeDlg, m_pPackagePurchaseButton, 0x190);
	CHECK_FIELD(FrTradeDlg, m_pTitleEdit, 0x128);
	CHECK_FIELD(FrTradeDlg, m_totalPrice, 0x198);
	CHECK_FIELD(FrTradeDlg, m_packageSale, 0x1a0);
END_LAYOUT_CHECK

class FrTradeEditDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
	friend class FrTradeDlg;   // OnEditBnUp copies m_packageSale
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // tradedlg.h:310

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // tradedlg.cpp:2872

public:
	FrTradeEditDlg(void);
	virtual ~FrTradeEditDlg(void);

	class MiniBtn {
	public:
		MiniBtn(void);
	};

	void SetPackageSale(eTypeSalesTrade type);   // tradedlg.h:464
	bool IsPackageSale(void);                    // tradedlg.h:465

	void SetTradeMyItem(sTradeItem* pItem);
	void SetControlsTradeMyItem(sTradeItem* pItem, bool bEnable);
	eResultTrade SendTradeTitle(void);
	void SetListTradeItem(void);   // tradedlg.cpp:4228
	void SetListMyItem(void);      // tradedlg.cpp:4184
	void DelListTradeItem(sTradeItem* pItem);   // tradedlg.cpp:4415
	void DelListMyItem(sTradeItem* pItem);      // tradedlg.cpp:4466
	void SetListEnableTradingMyItem(void);
	void AddListTradeItem(sTradeItem* pItem);   // tradedlg.cpp:4364
	void AddListMyItem(sTradeItem* pItem);      // tradedlg.cpp:4433
	int CheckTradeEdit(void);
	void CancelSelectedTradeItem(void);         // tradedlg.cpp:4517
	void SetListEquipParts(void);               // tradedlg.cpp:4082
	bool IsEquipParts(unsigned long typeId, unsigned long guid);   // tradedlg.cpp:4111
	int GetNumEquipItem(unsigned long typeId);  // tradedlg.cpp:4137
	int GetNumTradingItem(unsigned long guid);  // tradedlg.cpp:4154
	void SetListTradingItem(void);              // tradedlg.cpp:4172
	void DrawPang(void);
	void DrawItem(WPoint pt, sTradeItem* pItem, unsigned char flag);
	void UpdateTradeTitle(void);                // tradedlg.cpp:4769
	void UpdateTradeTitleEdit(void);            // tradedlg.cpp:4778

protected:
	void OnTradeEditDlgInit(int);       // tradedlg.cpp:3029
	void OnMyItemListInit(intptr_t);   // tradedlg.cpp:3034
	void OnMyItemListLClick(void);   // tradedlg.cpp:3045
	void OnMyItemListRClick(void);   // tradedlg.cpp:3068
	void OnMyItemListOwnerDraw(int);   // tradedlg.cpp:3164
	void OnTradeItemListInit(int);   // tradedlg.cpp:3234
	void OnVisitorInit(int);     // tradedlg.cpp:3342
	void OnUserIdInit(int);      // tradedlg.cpp:3357
	void OnTitleInit(intptr_t);       // tradedlg.cpp:3369
	void OnTitleEditInit(int);   // tradedlg.cpp:3377
	bool OnTitleEditEnterKey(int);   // tradedlg.cpp:3383
	void OnTitleEditBnInit(int);   // tradedlg.cpp:3395
	void OnTitleEditBnUp(void);    // tradedlg.cpp:3401
	void OnTradeMyItemIconInit(int);   // tradedlg.cpp:3411
	void OnTradeMyItemIconOwnerDraw(void);   // tradedlg.cpp:3417
	void OnTradeMyItemNameInit(int);   // tradedlg.cpp:3438
	void OnTradeMyItemAbilityInit(int);   // tradedlg.cpp:3444
	void OnTradeMyItemNumberInit(int);   // tradedlg.cpp:3450
	void OnTradeMyItemOkBnInit(int);   // tradedlg.cpp:3475
	void OnTradeMyItemCancelBnInit(int);   // tradedlg.cpp:3619
	void OnTradeMyItemCancelBnUp(void);    // tradedlg.cpp:3625
	void OnOpenBnInit(int);   // tradedlg.cpp:3632
	void OnOpenBnUp(void);    // tradedlg.cpp:3638
	void OnCloseBnInit(intptr_t);   // tradedlg.cpp:3665
	void OnExitBnInit(int);   // tradedlg.cpp:3677
	void OnExitBnUp(void);         // tradedlg.cpp:3683
	void OnPackageSaleInit(int);   // tradedlg.cpp:3698
	void OnTooltipAreaInit(int);   // tradedlg.cpp:3717
	void OnTradeMyItemPriceInit(int);   // tradedlg.cpp:3456
	void OnTradeMyItemPriceClick(void); // tradedlg.cpp:3468
	void OnCloseBnUp(void);             // tradedlg.cpp:3671
	void OnPackageSaleButtonUp(void);   // tradedlg.cpp:3707
	void OnTooltipAreaOwnerDraw(int);   // tradedlg.cpp:3722
	virtual bool OnInit(void);          // tradedlg.cpp:3729
	bool PushPackageSaleButton(void);   // tradedlg.cpp:3756
	bool IsPushedPackageSaleButton(void);   // tradedlg.cpp:3768

public:
	void SetControlsButton(bool bOpen, bool bClose);   // tradedlg.cpp:4668

protected:
	// Controls stored by the OnXxxInit handlers.
	FrListBox* m_pTradeItemList;       // 0x110
	FrListBox* m_pMyItemList;          // 0x114
	FrEdit* m_pVisitor;                // 0x118, OnVisitorInit (the visitor count)
	FrEdit* m_pUserId;                 // 0x11c, OnUserIdInit
	FrEdit* m_pTitle;                  // 0x120, OnTitleInit (the shop's title, read-only)
	FrEdit* m_pTitleEdit;              // 0x124
	FrArea* m_pTradeMyItemIcon;        // 0x128
	FrEdit* m_pTradeMyItemName;        // 0x12c
	FrEdit* m_pTradeMyItemAbility;     // 0x130
	FrEdit* m_pTradeMyItemNumber;      // 0x134
	FrEdit* m_pTradeMyItemPrice;       // 0x138
	FrButton* m_pTradeMyItemOkBn;      // 0x13c
	FrButton* m_pTradeMyItemCancelBn;  // 0x140
	FrButton* m_pOpenBn;               // 0x144
	FrButton* m_pCloseBn;              // 0x148
	FrButton* m_pExitBn;               // 0x14c
	FrButton* m_pPackageSaleBtn;       // 0x150
	FrArea* m_pTooltipArea;            // 0x154
	CToolTipManager m_toolTipManager;   // 0x158
	const Bitmap* m_pMyItemBaseClickBmp;   // 0x168, "trade_myitem_base_click"
	const Bitmap* m_pMyItemBaseOverBmp;    // 0x16c, "trade_myitem_base_over"
	UNKNOWN(0x170, 0x188);
	WTitleFont* m_pTitleFont;          // 0x188, "1234567890ym%-.?/:" 16x16
	WTitleFont* m_pNumberFont;         // 0x18c, "1234567890,-" 9x16
	UNKNOWN(0x190, 0x1b0);
	char m_tradeTitle[0x20];                 // 0x1b0, handed to FrTradeFinishEditDlg::SetTradeTitle
	std::list<sTradeItem> m_myItemList;      // 0x1d0, SetListMyItem
	std::list<sTradeItem> m_tradeItemList;   // 0x1dc, SetListTradeItem
	sTradeItem* m_pTradeMyItem;              // 0x1e8, SetTradeMyItem
	std::list<sPartEquipInfo> m_equipPartsList;   // 0x1ec, IsEquipParts
	eTypeSalesTrade m_packageSale;   // 0x1f8
	UNKNOWN(0x1fc, 0x290);
};
CHECK_SIZE(FrTradeEditDlg, 0x290);
BEGIN_LAYOUT_CHECK(FrTradeEditDlg)
	CHECK_FIELD(FrTradeEditDlg, m_pTradeItemList, 0x110);
	CHECK_FIELD(FrTradeEditDlg, m_pMyItemList, 0x114);
	CHECK_FIELD(FrTradeEditDlg, m_pVisitor, 0x118);
	CHECK_FIELD(FrTradeEditDlg, m_pUserId, 0x11c);
	CHECK_FIELD(FrTradeEditDlg, m_pTitle, 0x120);
	CHECK_FIELD(FrTradeEditDlg, m_pTitleEdit, 0x124);
	CHECK_FIELD(FrTradeEditDlg, m_pTradeMyItemIcon, 0x128);
	CHECK_FIELD(FrTradeEditDlg, m_pTradeMyItemName, 0x12c);
	CHECK_FIELD(FrTradeEditDlg, m_pTradeMyItemAbility, 0x130);
	CHECK_FIELD(FrTradeEditDlg, m_pTradeMyItemNumber, 0x134);
	CHECK_FIELD(FrTradeEditDlg, m_pTradeMyItemPrice, 0x138);
	CHECK_FIELD(FrTradeEditDlg, m_pTradeMyItemOkBn, 0x13c);
	CHECK_FIELD(FrTradeEditDlg, m_pTradeMyItemCancelBn, 0x140);
	CHECK_FIELD(FrTradeEditDlg, m_pOpenBn, 0x144);
	CHECK_FIELD(FrTradeEditDlg, m_pCloseBn, 0x148);
	CHECK_FIELD(FrTradeEditDlg, m_pExitBn, 0x14c);
	CHECK_FIELD(FrTradeEditDlg, m_pTooltipArea, 0x154);
	CHECK_FIELD(FrTradeEditDlg, m_pPackageSaleBtn, 0x150);
	CHECK_FIELD(FrTradeEditDlg, m_toolTipManager, 0x158);
	CHECK_FIELD(FrTradeEditDlg, m_pMyItemBaseClickBmp, 0x168);
	CHECK_FIELD(FrTradeEditDlg, m_pMyItemBaseOverBmp, 0x16c);
	CHECK_FIELD(FrTradeEditDlg, m_pTitleFont, 0x188);
	CHECK_FIELD(FrTradeEditDlg, m_pNumberFont, 0x18c);
	CHECK_FIELD(FrTradeEditDlg, m_tradeTitle, 0x1b0);
	CHECK_FIELD(FrTradeEditDlg, m_myItemList, 0x1d0);
	CHECK_FIELD(FrTradeEditDlg, m_tradeItemList, 0x1dc);
	CHECK_FIELD(FrTradeEditDlg, m_pTradeMyItem, 0x1e8);
	CHECK_FIELD(FrTradeEditDlg, m_equipPartsList, 0x1ec);
	CHECK_FIELD(FrTradeEditDlg, m_packageSale, 0x1f8);
END_LAYOUT_CHECK

class FrTradeFinishEditDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
	friend class FrTradeEditDlg;   // OnOpenBnUp fills m_pUserId and m_packageSale
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // tradedlg.h:478

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // tradedlg.cpp:4828

public:
	FrTradeFinishEditDlg(void);
	virtual ~FrTradeFinishEditDlg(void);

	class MiniBtn {
	public:
		MiniBtn(void);
	};

	void SetPackageSale(eTypeSalesTrade type);   // tradedlg.h:544
	bool IsPackageSale(void);                    // tradedlg.h:545

	void SetTradeUserId(void);                   // tradedlg.cpp:5206
	void SetTradeTitle(char* title);             // tradedlg.cpp:5217
	void SetListTradeItem(std::list<sTradeItem>* pList);   // tradedlg.cpp:5251

protected:
	void OnTradeFinishEditDlgInit(int);   // tradedlg.cpp:4898
	void OnUserIdInit(int);   // tradedlg.cpp:4903
	void OnTitleInit(intptr_t);   // tradedlg.cpp:4909
	void OnOkBnInit(intptr_t);   // tradedlg.cpp:4915
	void OnOkBnUp(void);    // tradedlg.cpp:4921
	void OnTradeItemListInit(int);   // tradedlg.cpp:4979
	virtual bool OnInit(void);            // tradedlg.cpp:4970

	FrListBox* m_pTradeItemList;     // 0x110
	FrEdit* m_pUserId;               // 0x114
	FrEdit* m_pTitle;                // 0x118
	UNKNOWN(0x11c, 0x138);
	WTitleFont* m_pTitleFont;        // 0x138, released by the destructor
	char m_tradeTitle[0x20];         // 0x13c, SetTradeTitle
	std::list<sTradeItem>* m_pListTradeItem;   // 0x15c, SetListTradeItem
	unsigned char m_tradeTitleSource;   // 0x160, 0 none, 1 the shared doc's title, 2 SetTradeTitle's argument
	UNKNOWN(0x161, 0x164);
	eTypeSalesTrade m_packageSale;   // 0x164
	UNKNOWN(0x168, 0x198);
};
CHECK_SIZE(FrTradeFinishEditDlg, 0x198);
BEGIN_LAYOUT_CHECK(FrTradeFinishEditDlg)
	CHECK_FIELD(FrTradeFinishEditDlg, m_pTradeItemList, 0x110);
	CHECK_FIELD(FrTradeFinishEditDlg, m_pUserId, 0x114);
	CHECK_FIELD(FrTradeFinishEditDlg, m_pTitle, 0x118);
	CHECK_FIELD(FrTradeFinishEditDlg, m_pTitleFont, 0x138);
	CHECK_FIELD(FrTradeFinishEditDlg, m_tradeTitle, 0x13c);
	CHECK_FIELD(FrTradeFinishEditDlg, m_pListTradeItem, 0x15c);
	CHECK_FIELD(FrTradeFinishEditDlg, m_tradeTitleSource, 0x160);
	CHECK_FIELD(FrTradeFinishEditDlg, m_packageSale, 0x164);
END_LAYOUT_CHECK

class FrTradeBuyDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
	friend class FrTradeDlg;   // OnPackagePurchaseLButtonUp
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // tradedlg.h:154

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // tradedlg.cpp:1443

public:
	FrTradeBuyDlg(void);
	virtual ~FrTradeBuyDlg(void);

	bool IsClosing(void) { return m_bClosing; }   // tradedlg.h:220
	void SetClosing(bool bClosing);          // tradedlg.h:222
	void SetPackageSale(void);               // tradedlg.h:233
	eTypeSalesTrade GetPackageSale(void);    // tradedlg.h:234
	void ClearTradeList(void) { m_pPurchaseList->ClearItem(); }   // tradedlg.h:236

	void UpdatePrice(void);
	void UpdatePriceList(void);
	void SetControlsBuyItem(sTradeItem* pItem);   // tradedlg.cpp:2309
	void SetBuyItemList(void);               // tradedlg.cpp:2269
	void SetControlsItemList(void);          // tradedlg.cpp:2279

protected:
	void OnTradeBuyDlgInit(int);   // tradedlg.cpp:1526
	void OnOkBnInit(intptr_t);   // tradedlg.cpp:1531
	void OnBuyItemIconInit(int);   // tradedlg.cpp:1738
	void OnBuyItemIconOwnerDraw(void);   // tradedlg.cpp:1744
	void OnBuyItemNameInit(int);   // tradedlg.cpp:1853
	void OnBuyItemAbilityInit(int);   // tradedlg.cpp:1859
	void OnMyPangInit(intptr_t);   // tradedlg.cpp:1865
	void OnPriceInit(intptr_t);   // tradedlg.cpp:1880
	void OnPangLeftInit(int);   // tradedlg.cpp:1886
	void OnNumberInit(int);   // tradedlg.cpp:1892
	void OnNumberNextBnInit(int);   // tradedlg.cpp:1898
	void OnNumberNextBnUp(void);    // tradedlg.cpp:1909
	void OnNumberPrevBnInit(int);   // tradedlg.cpp:1933
	void OnNumberPrevBnUp(void);    // tradedlg.cpp:1944
	void OnPurchaseListInit(int);   // tradedlg.cpp:1968
	virtual bool OnInit(void);     // tradedlg.cpp:2112
	virtual void OnProc(const float dt);   // tradedlg.cpp:2120

	FrArea* m_pBuyItemIcon;          // 0x110
	FrEdit* m_pBuyItemName;          // 0x114
	FrEdit* m_pBuyItemAbility;       // 0x118
	FrEdit* m_pMyPang;               // 0x11c, OnMyPangInit (the local player's pang)
	FrEdit* m_pPrice;                // 0x120
	FrEdit* m_pPangLeft;             // 0x124
	FrButton* m_pNumberNextBn;       // 0x128, OnNumberNextBnInit
	FrButton* m_pNumberPrevBn;       // 0x12c, OnNumberPrevBnInit
	FrEdit* m_pNumber;               // 0x130
	FrListBox* m_pPurchaseList;      // 0x134
	eTypeSalesTrade m_packageSale;   // 0x138
	sTradeItem m_buyOrder;           // 0x13c, the order sent by SendTradeBuyItem (OnConfirmResult)
	UNKNOWN(0x1e6, 0x1e8);
	sTradeItem* m_pBuyItem;          // 0x1e8, the item being bought (count = stock)
	bool m_bClosing;                 // 0x1ec
	UNKNOWN(0x1ed, 0x1f0);
	const Bitmap* m_pBmpUccOriginal;    // 0x1f0, "ucc_state_original"
	const Bitmap* m_pBmpUccCopied;      // 0x1f4, "ucc_state_copied"
	const Bitmap* m_pBmpUccTempSaved;   // 0x1f8, "ucc_state_temp_saved"
	WTitleFont* m_pTitleFont;        // 0x1fc, released by the destructor
	int m_lastNumber;                // 0x200, the count UpdatePrice last priced
	UNKNOWN(0x204, 0x208);
};
CHECK_SIZE(FrTradeBuyDlg, 0x208);
BEGIN_LAYOUT_CHECK(FrTradeBuyDlg)
	CHECK_FIELD(FrTradeBuyDlg, m_pBuyItemIcon, 0x110);
	CHECK_FIELD(FrTradeBuyDlg, m_pBuyItemName, 0x114);
	CHECK_FIELD(FrTradeBuyDlg, m_pBuyItemAbility, 0x118);
	CHECK_FIELD(FrTradeBuyDlg, m_pMyPang, 0x11c);
	CHECK_FIELD(FrTradeBuyDlg, m_pPrice, 0x120);
	CHECK_FIELD(FrTradeBuyDlg, m_pPangLeft, 0x124);
	CHECK_FIELD(FrTradeBuyDlg, m_pNumberNextBn, 0x128);
	CHECK_FIELD(FrTradeBuyDlg, m_pNumberPrevBn, 0x12c);
	CHECK_FIELD(FrTradeBuyDlg, m_pNumber, 0x130);
	CHECK_FIELD(FrTradeBuyDlg, m_pPurchaseList, 0x134);
	CHECK_FIELD(FrTradeBuyDlg, m_packageSale, 0x138);
	CHECK_FIELD(FrTradeBuyDlg, m_buyOrder, 0x13c);
	CHECK_FIELD(FrTradeBuyDlg, m_pBuyItem, 0x1e8);
	CHECK_FIELD(FrTradeBuyDlg, m_pBmpUccOriginal, 0x1f0);
	CHECK_FIELD(FrTradeBuyDlg, m_pBmpUccCopied, 0x1f4);
	CHECK_FIELD(FrTradeBuyDlg, m_pBmpUccTempSaved, 0x1f8);
	CHECK_FIELD(FrTradeBuyDlg, m_lastNumber, 0x200);
	CHECK_FIELD(FrTradeBuyDlg, m_bClosing, 0x1ec);
	CHECK_FIELD(FrTradeBuyDlg, m_pTitleFont, 0x1fc);
END_LAYOUT_CHECK

class FrTradeResultDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // tradedlg.h:249

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // tradedlg.cpp:2379

public:
	FrTradeResultDlg(void);
	virtual ~FrTradeResultDlg(void);

	void SetResultItem(sTradeItem* pItem);
	void SetResultItemList(std::vector<sTradeItem>* pList);   // tradedlg.cpp:2731

protected:
	void OnTradeResultDlgInit(int);   // tradedlg.cpp:2441
	void OnResultItemIconInit(intptr_t);   // tradedlg.cpp:2446
	void OnResultItemNameInit(intptr_t);   // tradedlg.cpp:2552
	void OnResultItemAbilityInit(intptr_t);   // tradedlg.cpp:2558
	void OnPurchaseListInit(int);   // tradedlg.cpp:2564
	virtual bool OnInit(void);        // tradedlg.cpp:2704

	FrArea* m_pResultItemIcon;      // 0x110
	FrEdit* m_pResultItemName;      // 0x114
	FrEdit* m_pResultItemAbility;   // 0x118
	const Bitmap* m_pBmpUccOriginal;    // 0x11c, "ucc_state_original"
	const Bitmap* m_pBmpUccCopied;      // 0x120, "ucc_state_copied"
	const Bitmap* m_pBmpUccTempSaved;   // 0x124, "ucc_state_temp_saved"
	sTradeItem m_resultItem;        // 0x128, cleared by the constructor
	UNKNOWN(0x1d2, 0x1d4);
	WTitleFont* m_pTitleFont;       // 0x1d4, released by the destructor
	UNKNOWN(0x1d8, 0x1dc);
	std::vector<sTradeItem> m_resultItemList;   // 0x1dc, SetResultItemList
	FrListBox* m_pPurchaseList;     // 0x1ec
};
CHECK_SIZE(FrTradeResultDlg, 0x1f0);
BEGIN_LAYOUT_CHECK(FrTradeResultDlg)
	CHECK_FIELD(FrTradeResultDlg, m_pResultItemIcon, 0x110);
	CHECK_FIELD(FrTradeResultDlg, m_pResultItemName, 0x114);
	CHECK_FIELD(FrTradeResultDlg, m_pResultItemAbility, 0x118);
	CHECK_FIELD(FrTradeResultDlg, m_pBmpUccOriginal, 0x11c);
	CHECK_FIELD(FrTradeResultDlg, m_pBmpUccCopied, 0x120);
	CHECK_FIELD(FrTradeResultDlg, m_pBmpUccTempSaved, 0x124);
	CHECK_FIELD(FrTradeResultDlg, m_resultItem, 0x128);
	CHECK_FIELD(FrTradeResultDlg, m_pTitleFont, 0x1d4);
	CHECK_FIELD(FrTradeResultDlg, m_resultItemList, 0x1dc);
	CHECK_FIELD(FrTradeResultDlg, m_pPurchaseList, 0x1ec);
END_LAYOUT_CHECK

class FrTradeWarningDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // tradedlg.h:558

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // tradedlg.cpp:5265

public:
	FrTradeWarningDlg(void);            // tradedlg.cpp:5278
	virtual ~FrTradeWarningDlg(void);   // tradedlg.cpp:5286

	void SetControls(int type);                                   // tradedlg.cpp:5367
	void SetWarningMessageType(int type, unsigned char subType);
	void SetWarningMessageForPackage(int code);                   // tradedlg.cpp:5587

protected:
	void OnTradeWarningDlgInit(int);   // tradedlg.cpp:5291
	void OnMsg01Init(int);   // tradedlg.cpp:5296
	void OnMsg02Init(int);   // tradedlg.cpp:5302
	void OnOkBnInit(intptr_t);   // tradedlg.cpp:5308
	void OnOkBnUp(void);    // tradedlg.cpp:5314
	void OnCancelBnInit(int);   // tradedlg.cpp:5355
	void OnCancelBnUp(void);           // tradedlg.cpp:5361

	// Controls stored by the OnXxxInit handlers.
	FrStatic* m_pMsg01;    // 0x110
	FrStatic* m_pMsg02;    // 0x114
	FrButton* m_pOkBn;     // 0x118
	FrButton* m_pCancelBn; // 0x11c
	int m_warningType;     // 0x120, set by SetWarningMessageType
	UNKNOWN(0x124, 0x128);
};
CHECK_SIZE(FrTradeWarningDlg, 0x128);
BEGIN_LAYOUT_CHECK(FrTradeWarningDlg)
	CHECK_FIELD(FrTradeWarningDlg, m_pMsg01, 0x110);
	CHECK_FIELD(FrTradeWarningDlg, m_pMsg02, 0x114);
	CHECK_FIELD(FrTradeWarningDlg, m_pOkBn, 0x118);
	CHECK_FIELD(FrTradeWarningDlg, m_pCancelBn, 0x11c);
	CHECK_FIELD(FrTradeWarningDlg, m_warningType, 0x120);
END_LAYOUT_CHECK

class FrTradeLevelLimitDlg : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // tradedlg.h:600

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // tradedlg.cpp:5605

public:
	FrTradeLevelLimitDlg(void);            // tradedlg.cpp:5613
	virtual ~FrTradeLevelLimitDlg(void);   // tradedlg.cpp:5617

protected:
	void OnOkBnInit(intptr_t);        // tradedlg.cpp:5621
	void OnOkBnUp(void);         // tradedlg.cpp:5626
	virtual bool OnInit(void);   // tradedlg.cpp:5631
};
CHECK_SIZE(FrTradeLevelLimitDlg, 0x110);

#endif // PG_CLIENT_PROJECTG_TRADEDLG_H
