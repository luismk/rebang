// Original: d:\build\custom\temp\code\client\projectg\offlinetradedlg.h
#ifndef PG_CLIENT_PROJECTG_OFFLINETRADEDLG_H
#define PG_CLIENT_PROJECTG_OFFLINETRADEDLG_H

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "TradeDlg.h"
#include "ToolTipManager.h"
#include "../shared/globalnetworkdefine.h"

class FrComboBox;
class FrViewer;
class CStandingInLine;
struct FrListItem;

// The dialogs of the personal (offline) shop. Sizes are the `new` of each
// XxxMakeInstance; only the members the unit's functions so far touch are
// declared.

class FrOfflineTradeFinishEditDlg : public FrTradeFinishEditDlg {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // offlinetradedlg.h:27

	class MiniBtn {
	public:
		MiniBtn(void);
	};

	FrOfflineTradeFinishEditDlg(void);
	virtual ~FrOfflineTradeFinishEditDlg(void);   // offlinetradedlg.cpp:112

	void SetListTradeItem(std::list<sTradeItem>* pList);   // offlinetradedlg.cpp:324
	void GetSelectSlotInfo(int& index, int& used);   // offlinetradedlg.cpp:347

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // offlinetradedlg.cpp:36

	void OnToolTipInit(intptr_t);                // offlinetradedlg.cpp:363
	void OnToolTipOnwerDraw(int);           // offlinetradedlg.cpp:369

private:
	FrListItem* m_pSelectedSlotItem;    // 0x198, its pData is a sStallSlot
	FrArea* m_pToolTip;                 // 0x19c, OnToolTipInit
	CToolTipManager m_toolTipManager;   // 0x1a0
	PADDING(0x1b0, 0x300);
};

CHECK_SIZE(FrOfflineTradeFinishEditDlg, 0x300);
BEGIN_LAYOUT_CHECK(FrOfflineTradeFinishEditDlg)
	CHECK_FIELD(FrOfflineTradeFinishEditDlg, m_pSelectedSlotItem, 0x198);
	CHECK_FIELD(FrOfflineTradeFinishEditDlg, m_pToolTip, 0x19c);
	CHECK_FIELD(FrOfflineTradeFinishEditDlg, m_toolTipManager, 0x1a0);
END_LAYOUT_CHECK

class COfflineTradeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // offlinetradedlg.h:133

	// A small button drawn on each market list row (the constructor, OnMaketListBtnDown).
	class MiniBtn {
	public:
		MiniBtn(void);

		WRect rect;              // 0x0, relative to the row
		const Bitmap* pBitmap;   // 0x10
		UNKNOWN(0x14, 0x18);
	};

	COfflineTradeDlg(void);
	virtual ~COfflineTradeDlg(void);   // offlinetradedlg.cpp:507

	void SetPageData(int currentPage, int maxPage);
	void RequestMaketList(void);
	void SetListItem(void);                 // offlinetradedlg.cpp:706
	bool SetSearchString(std::string searchString);
	bool OnStandingInLineDlgResult(int ret, FrForm* pForm);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // offlinetradedlg.cpp:399

	virtual bool OnInit(void);              // offlinetradedlg.cpp:511
	void OnProc(const float dt);            // offlinetradedlg.cpp:538
	void OnAreaSubNickInit(intptr_t);            // offlinetradedlg.cpp:544
	void OnAreaSubNickOwnerDraw(int);       // offlinetradedlg.cpp:556
	void OnAreaSubNickLBtnUP(void);         // offlinetradedlg.cpp:582
	void OnAreaSubPriceInit(intptr_t);           // offlinetradedlg.cpp:592
	void OnAreaSubPriceOwnerDraw(int);      // offlinetradedlg.cpp:603
	void OnAreaSubItemNameInit(intptr_t);        // offlinetradedlg.cpp:643
	void OnAreaSubItemNameOwnerDraw(int);   // offlinetradedlg.cpp:655
	void OnAreaSubPriceLBtnUP(void);        // offlinetradedlg.cpp:632
	void OnAreaSubItemNameLBtnUP(void);     // offlinetradedlg.cpp:682
	void OnMaketListInit(intptr_t);              // offlinetradedlg.cpp:691
	void OnMaketListOwnerDraw(int);         // offlinetradedlg.cpp:726
	void OnMaketListBtnDown(void);          // offlinetradedlg.cpp:795
	void OnItemSearchInit(intptr_t);             // offlinetradedlg.cpp:835
	bool OnItemSearchEnterKey(int);         // offlinetradedlg.cpp:849
	void OnItemSearchLBtnUp(void);          // offlinetradedlg.cpp:857
	void OnBtnMainSearchInit(intptr_t);          // offlinetradedlg.cpp:877
	void OnBtnMainSearchUp(void);           // offlinetradedlg.cpp:885
	void OnEditMaxPageInit(intptr_t);            // offlinetradedlg.cpp:911
	void OnEditCurrentPageInit(intptr_t);        // offlinetradedlg.cpp:922

	bool OnEditCurrentPageEnterKey(int);    // offlinetradedlg.cpp:932
	void OnBtnGoPageInit(intptr_t);              // offlinetradedlg.cpp:1076
	void OnBtnRefreshInit(intptr_t);             // offlinetradedlg.cpp:1104
	void OnBtnRefreshBtnUp(void);           // offlinetradedlg.cpp:1112
	void OnToolTipInit(intptr_t);                // offlinetradedlg.cpp:1198
	void OnBtnDetailInit(intptr_t);              // offlinetradedlg.cpp:1153
	void OnBtnDetailBtnUp(void);            // offlinetradedlg.cpp:1163
	void OnEditMsgInit(intptr_t);                // offlinetradedlg.cpp:1180
	void OnBtnPrevInit(intptr_t);                // offlinetradedlg.cpp:1021
	void OnBtnPrevBtnUp(void);              // offlinetradedlg.cpp:1029
	void OnBtnNextInit(intptr_t);                // offlinetradedlg.cpp:1048
	void OnBtnNextBtnUp(void);              // offlinetradedlg.cpp:1057
	void OnBtnGoPageBtnUp(void);            // offlinetradedlg.cpp:1085
	void OnToolTipOnwerDraw(int);           // offlinetradedlg.cpp:1204
	void OnEditMsgLBtnUp(void);             // offlinetradedlg.cpp:1191

private:
	FrViewer* m_pAreaSubNick;           // 0x110, OnAreaSubNickInit
	FrViewer* m_pAreaSubPrice;          // 0x114, OnAreaSubPriceInit
	FrViewer* m_pAreaSubItemName;       // 0x118, OnAreaSubItemNameInit
	FrEdit* m_pItemSearchEdit;          // 0x11c, OnItemSearchLBtnUp
	FrEdit* m_pMaxPageEdit;             // 0x120, OnEditMaxPageInit
	FrEdit* m_pCurrentPageEdit;         // 0x124, OnEditCurrentPageInit
	FrListBox* m_pMaketList;            // 0x128, OnMaketListInit
	FrButton* m_pBtnPrev;               // 0x12c, OnBtnPrevInit
	FrButton* m_pBtnNext;               // 0x130, OnBtnNextInit
	FrButton* m_pBtnGoPage;             // 0x134, OnBtnGoPageInit
	FrButton* m_pBtnMainSearch;         // 0x138, OnBtnMainSearchInit
	FrButton* m_pBtnRefresh;            // 0x13c, OnBtnRefreshInit
	FrButton* m_pBtnDetail;             // 0x140, OnBtnDetailInit
	FrEdit* m_pEditMsg;                 // 0x144, OnEditMsgInit
	FrArea* m_pToolTip;                 // 0x148, OnToolTipInit
	CToolTipManager m_toolTipManager;   // 0x14c
	CStandingInLine* m_pStandingInLine; // 0x15c, OnBtnDetailBtnUp
	MiniBtn m_moveBtnNormal;            // 0x160, "rmr_btn_move_n"
	MiniBtn m_moveBtnOver;              // 0x178, "rmr_btn_move_o"
	int m_maxPage;                      // 0x190
	int m_currentPage;                  // 0x194
	int m_searchType;                   // 0x198, 1 item name, 3 price, 4 nick
	int m_searchOrder;                  // 0x19c
	bool m_myRoomState;                 // 0x1a0, cleared with m_canVisitMyRoom (constructor, OnBtnRefreshBtnUp)
	PADDING(0x1a1, 0x1a4);
	int m_canVisitMyRoom;               // 0x1a4, answer of main-actor message 0x231 (OnInit)
	std::string m_searchString;         // 0x1a8 (SetSearchString)
	PADDING(0x1c4, 0x1c8);
};

CHECK_SIZE(COfflineTradeDlg, 0x1c8);
BEGIN_LAYOUT_CHECK(COfflineTradeDlg)
	CHECK_FIELD(COfflineTradeDlg, m_pAreaSubNick, 0x110);
	CHECK_FIELD(COfflineTradeDlg, m_pAreaSubPrice, 0x114);
	CHECK_FIELD(COfflineTradeDlg, m_pAreaSubItemName, 0x118);
	CHECK_FIELD(COfflineTradeDlg, m_pItemSearchEdit, 0x11c);
	CHECK_FIELD(COfflineTradeDlg, m_pMaxPageEdit, 0x120);
	CHECK_FIELD(COfflineTradeDlg, m_pCurrentPageEdit, 0x124);
	CHECK_FIELD(COfflineTradeDlg, m_pMaketList, 0x128);
	CHECK_FIELD(COfflineTradeDlg, m_pBtnPrev, 0x12c);
	CHECK_FIELD(COfflineTradeDlg, m_pBtnNext, 0x130);
	CHECK_FIELD(COfflineTradeDlg, m_pBtnGoPage, 0x134);
	CHECK_FIELD(COfflineTradeDlg, m_pBtnMainSearch, 0x138);
	CHECK_FIELD(COfflineTradeDlg, m_pBtnRefresh, 0x13c);
	CHECK_FIELD(COfflineTradeDlg, m_pToolTip, 0x148);
	CHECK_FIELD(COfflineTradeDlg, m_toolTipManager, 0x14c);
	CHECK_FIELD(COfflineTradeDlg, m_pStandingInLine, 0x15c);
	CHECK_FIELD(COfflineTradeDlg, m_moveBtnNormal, 0x160);
	CHECK_FIELD(COfflineTradeDlg, m_moveBtnOver, 0x178);
	CHECK_FIELD(COfflineTradeDlg, m_maxPage, 0x190);
	CHECK_FIELD(COfflineTradeDlg, m_currentPage, 0x194);
	CHECK_FIELD(COfflineTradeDlg, m_searchType, 0x198);
	CHECK_FIELD(COfflineTradeDlg, m_searchOrder, 0x19c);
	CHECK_FIELD(COfflineTradeDlg, m_myRoomState, 0x1a0);
	CHECK_FIELD(COfflineTradeDlg, m_canVisitMyRoom, 0x1a4);
	CHECK_FIELD(COfflineTradeDlg, m_searchString, 0x1a8);
END_LAYOUT_CHECK

class CBuyingSlotDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // offlinetradedlg.h:274

	CBuyingSlotDlg(void);                // offlinetradedlg.cpp:1342
	virtual ~CBuyingSlotDlg(void);       // offlinetradedlg.cpp:1358

	void RecalculatePang(void);          // offlinetradedlg.cpp:1457

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // offlinetradedlg.cpp:1314

	void OnDaysInit(intptr_t);                   // offlinetradedlg.cpp:1362
	void OnDaysBtnDown(void);               // offlinetradedlg.cpp:1387
	void OnPriceInit(intptr_t);                  // offlinetradedlg.cpp:1405
	void OnMyMoneyInit(intptr_t);                // offlinetradedlg.cpp:1420
	void OnMoneyLeftInit(intptr_t);              // offlinetradedlg.cpp:1434
	void OnCheckOutBtnDown(void);           // offlinetradedlg.cpp:1537
	void OnCheckOutInit(intptr_t);               // offlinetradedlg.cpp:1529
	void OnSlotPriceInit(intptr_t);              // offlinetradedlg.cpp:1597

private:
	FrComboBox* m_pDays;        // 0x110, OnDaysInit
	FrEdit* m_pMyMoney;         // 0x114, OnMyMoneyInit
	FrEdit* m_pMoneyLeft;       // 0x118, OnMoneyLeftInit
	FrEdit* m_pPrice;           // 0x11c, OnPriceInit
	FrButton* m_pCheckOut;      // 0x120, OnCheckOutInit
	FrEdit* m_pSlotPrice;       // 0x124, OnSlotPriceInit
	__int64 m_price;            // 0x128
	int m_slotNum;              // 0x130, SetSlotNum
	PADDING(0x134, 0x138);
};

CHECK_SIZE(CBuyingSlotDlg, 0x138);
BEGIN_LAYOUT_CHECK(CBuyingSlotDlg)
	CHECK_FIELD(CBuyingSlotDlg, m_pDays, 0x110);
	CHECK_FIELD(CBuyingSlotDlg, m_pSlotPrice, 0x124);
	CHECK_FIELD(CBuyingSlotDlg, m_price, 0x128);
	CHECK_FIELD(CBuyingSlotDlg, m_slotNum, 0x130);
END_LAYOUT_CHECK

class COfflineShopEditDlg : public FrTradeEditDlg {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // offlinetradedlg.h:340

	class MiniBtn {
	public:
		MiniBtn(void);
	};

	COfflineShopEditDlg(void);
	virtual ~COfflineShopEditDlg(void);   // offlinetradedlg.cpp:1748

	void SetSelectData(unsigned long first, unsigned long second);   // offlinetradedlg.cpp:2260
	void SetListTradeItem(void);                                              // offlinetradedlg.cpp:2356
	void SetSlotData(void);                                                   // offlinetradedlg.cpp:2410
	bool IsCheckCardPats(unsigned long partsId, unsigned long partsTypeId);   // offlinetradedlg.cpp:2448
	bool OnOfflineEquitCancelDlgResult(int ret, FrForm* pForm);               // offlinetradedlg.cpp:2468
	void InitAllUI(void);                                                     // offlinetradedlg.cpp:2336
	void DelListMyItem(sTradeItem* pItem, unsigned long, unsigned char);      // offlinetradedlg.cpp:2604
	bool OnOfflineTradeBuyDlgResult(int ret, FrForm* pForm);                  // offlinetradedlg.cpp:2641

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // offlinetradedlg.cpp:1659

	void OnAreaBg01Init(int);               // offlinetradedlg.cpp:1770
	void OnAreaBg02Init(int);               // offlinetradedlg.cpp:1787
	void OnAreaBg03Init(int);               // offlinetradedlg.cpp:1804
	void OnAreaBg04Init(int);               // offlinetradedlg.cpp:1822
	void OnAreaBg05Init(int);               // offlinetradedlg.cpp:1839
	void OnStallSlotNumInit(int);           // offlinetradedlg.cpp:1855
	void OnOpenBnInit(int);                 // offlinetradedlg.cpp:2062
	void OnCloseBnInit(intptr_t);                // offlinetradedlg.cpp:2266
	void OnSlotNumInit(int);                // offlinetradedlg.cpp:1864
	void OnTradeItemListInit(int);          // offlinetradedlg.cpp:1873
	void OnTradeItemListLClick(void);       // offlinetradedlg.cpp:1889
	void OnVisitorInit(int);                // offlinetradedlg.cpp:2044
	void OnToolTipInit(intptr_t);                // offlinetradedlg.cpp:2573
	void OnProc(const float dt);            // offlinetradedlg.cpp:2349
	void OnExitBnUp(void);                  // offlinetradedlg.cpp:2298
	virtual bool OnInit(void);              // offlinetradedlg.cpp:2317
	void OnToolTipOnwerDraw(int);           // offlinetradedlg.cpp:2579

private:
	FrListBox* m_pShopItemList;         // 0x290, OnTradeItemListInit
	FrEdit* m_pVisitor;                 // 0x294, OnVisitorInit
	FrButton* m_pShopOpenBn;            // 0x298, OnOpenBnInit
	FrButton* m_pShopCloseBn;           // 0x29c, OnCloseBnInit
	PADDING(0x2a0, 0x2a4);
	FrArea* m_pAreaBg01;                // 0x2a4, OnAreaBg01Init
	FrArea* m_pAreaBg02;                // 0x2a8, OnAreaBg02Init
	FrArea* m_pAreaBg03;                // 0x2ac, OnAreaBg03Init
	FrArea* m_pAreaBg04;                // 0x2b0, OnAreaBg04Init
	FrArea* m_pAreaBg05;                // 0x2b4, OnAreaBg05Init
	FrEdit* m_pStallSlotNum;            // 0x2b8, OnStallSlotNumInit
	FrEdit* m_pSlotNum;                 // 0x2bc, OnSlotNumInit
	FrArea* m_pToolTip;                 // 0x2c0, OnToolTipInit
	CToolTipManager m_toolTipManager;   // 0x2c4
	PADDING(0x2d4, 0x338);
	int m_usedSlotCount;                // 0x338, slots holding an item (SetSlotData)
	int m_closing;                      // 0x33c, set by OnExitBnUp, tested by the destructor
	unsigned long m_selectData[2];      // 0x340, SetSelectData
};

CHECK_SIZE(COfflineShopEditDlg, 0x348);
BEGIN_LAYOUT_CHECK(COfflineShopEditDlg)
	CHECK_FIELD(COfflineShopEditDlg, m_toolTipManager, 0x2c4);
	CHECK_FIELD(COfflineShopEditDlg, m_pShopItemList, 0x290);
	CHECK_FIELD(COfflineShopEditDlg, m_pVisitor, 0x294);
	CHECK_FIELD(COfflineShopEditDlg, m_pAreaBg01, 0x2a4);
	CHECK_FIELD(COfflineShopEditDlg, m_pAreaBg04, 0x2b0);
	CHECK_FIELD(COfflineShopEditDlg, m_pStallSlotNum, 0x2b8);
	CHECK_FIELD(COfflineShopEditDlg, m_pSlotNum, 0x2bc);
	CHECK_FIELD(COfflineShopEditDlg, m_pToolTip, 0x2c0);
	CHECK_FIELD(COfflineShopEditDlg, m_usedSlotCount, 0x338);
	CHECK_FIELD(COfflineShopEditDlg, m_closing, 0x33c);
	CHECK_FIELD(COfflineShopEditDlg, m_selectData, 0x340);
END_LAYOUT_CHECK

class COfflineShopDlg : public FrTradeDlg {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // offlinetradedlg.h:546

	class MiniBtn {
	public:
		MiniBtn(void);
	};

	COfflineShopDlg(void);
	virtual ~COfflineShopDlg(void);   // offlinetradedlg.cpp:2762

	bool OnOfflineShopEditDlgCreateResult(int ret, FrForm* pForm);   // offlinetradedlg.cpp:3137
	void SetListTradeItem(bool bClear);   // offlinetradedlg.cpp:3181

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // offlinetradedlg.cpp:2668

	void OnVisitorBackOwnerDraw(void);      // offlinetradedlg.cpp:2802
	void OnVisitorInit(int);                // offlinetradedlg.cpp:2807
	void OnCloseBnUp(void);                 // offlinetradedlg.cpp:2826
	void OnVisitorBackInit(int);            // offlinetradedlg.cpp:2847
	void OnToolTipInit(intptr_t);                // offlinetradedlg.cpp:3021
	void OnToolTipOnwerDraw(int);           // offlinetradedlg.cpp:3027
	void OnProc(const float dt);            // offlinetradedlg.cpp:3174

private:
	FrArea* m_pVisitorBack;             // 0x220, OnVisitorBackInit
	FrEdit* m_pVisitor;                 // 0x224, OnVisitorInit
	FrArea* m_pToolTip;                 // 0x228, OnToolTipInit
	CToolTipManager m_toolTipManager;   // 0x22c
	PADDING(0x23c, 0x38c);
	int m_closing;                      // 0x38c, set by OnCloseBnUp, tested by the destructor
	FrListItem* m_pSelectedSlotItem;    // 0x390, its pData is the sStallSlot the edit dialog opens on
	PADDING(0x394, 0x398);
};

CHECK_SIZE(COfflineShopDlg, 0x398);
BEGIN_LAYOUT_CHECK(COfflineShopDlg)
	CHECK_FIELD(COfflineShopDlg, m_pVisitor, 0x224);
	CHECK_FIELD(COfflineShopDlg, m_toolTipManager, 0x22c);
	CHECK_FIELD(COfflineShopDlg, m_closing, 0x38c);
	CHECK_FIELD(COfflineShopDlg, m_pSelectedSlotItem, 0x390);
END_LAYOUT_CHECK

class COfflineShopVisitorDlg : public FrTradeDlg {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // offlinetradedlg.h:688

	class MiniBtn {
	public:
		MiniBtn(void);
	};

	COfflineShopVisitorDlg(void);
	virtual ~COfflineShopVisitorDlg(void);   // offlinetradedlg.cpp:3299

	bool OnOfflineShopEditDlgCreateResult(int, FrForm *);  // offlinetradedlg.cpp:3548
	void SetListTradeItem(bool bClear);                    // offlinetradedlg.cpp:3561

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // offlinetradedlg.cpp:3206

	void OnEditBnUp(void);                  // offlinetradedlg.cpp:3304
	void OnVisitorBackOwnerDraw(void);      // offlinetradedlg.cpp:3320
	void OnCloseBnUp(void);                 // offlinetradedlg.cpp:3326
	void OnVisitorInit(int);                // offlinetradedlg.cpp:3341
	void OnVisitorBackInit(int);            // offlinetradedlg.cpp:3360
	void OnToolTipInit(intptr_t);                // offlinetradedlg.cpp:3517
	void OnToolTipOnwerDraw(int);           // offlinetradedlg.cpp:3523
	void OnProc(const float dt);            // offlinetradedlg.cpp:3554

private:
	FrEdit* m_pVisitor;                 // 0x220, OnVisitorInit
	FrArea* m_pVisitorBack;             // 0x224, OnVisitorBackInit
	FrArea* m_pToolTip;                 // 0x228, OnToolTipInit
	CToolTipManager m_toolTipManager;   // 0x22c
	PADDING(0x23c, 0x2d0);
};

CHECK_SIZE(COfflineShopVisitorDlg, 0x2d0);
BEGIN_LAYOUT_CHECK(COfflineShopVisitorDlg)
	CHECK_FIELD(COfflineShopVisitorDlg, m_pVisitor, 0x220);
	CHECK_FIELD(COfflineShopVisitorDlg, m_toolTipManager, 0x22c);
END_LAYOUT_CHECK

class COfflineBuyDlg : public FrTradeBuyDlg {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // offlinetradedlg.h:788

	COfflineBuyDlg(void);
	virtual ~COfflineBuyDlg(void);     // offlinetradedlg.cpp:3645

	void SetBuyitemData(sStallSlot slot) { m_buyItem = slot; }   // offlinetradedlg.h:842
	bool OnConfirmResult(int result, FrForm* pForm);   // offlinetradedlg.cpp:3733

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // offlinetradedlg.cpp:3586

private:
	sStallSlot m_buyItem;              // 0x208, SetBuyitemData
	PADDING(0x2c9, 0x2cc);
	int m_buyCount;                    // 0x2cc, count sent by OnConfirmResult (OnOkBnUp)
};

CHECK_SIZE(COfflineBuyDlg, 0x2d0);
BEGIN_LAYOUT_CHECK(COfflineBuyDlg)
	CHECK_FIELD(COfflineBuyDlg, m_buyItem, 0x208);
	CHECK_FIELD(COfflineBuyDlg, m_buyCount, 0x2cc);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_OFFLINETRADEDLG_H
