// Original: d:\build\custom\temp\code\client\projectg\scratchdlg.h
#ifndef PG_CLIENT_PROJECTG_SCRATCHDLG_H
#define PG_CLIENT_PROJECTG_SCRATCHDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "bitmap.h"
#include "BongdariShopDlg.h"

class FrArea;
class FrButton;
class FrEdit;
class FrTextButton;
class FrViewer;
class WTitleFont;

// Scratch card dialog; its size (0x390) is the `new` of
// FrScratchDlgMakeInstance. Only the members this unit defines so far are
// declared.
class FrScratchDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scratchdlg.h:49

	FrScratchDlg(void);
	virtual ~FrScratchDlg(void);   // scratchdlg.cpp:112

	void EnableNextPageButton(bool bEnable);
	void EnableScratchSerialNumber(bool bEnable);
	void ClearScratchItem(void);
	void ClearDataCheckScratchItem(void);
	void ProcessChangeScratchCardNumber(float dt);
	void InitChangeScratchCardNumber(int number);
	void OpenScratchCover(const WPoint& pt);
	void RequestScratchItem(void);
	void DrawScratchItem(void);
	void DrawChangeScratchCardNumber(void);
	void OpenScratchResultDlg(void);
	void SendScratchSerialNumber(void);
	void SetBmpColsedScratchCover(void);
	void ResetBmpScratchCover(int index);
	void SetScratchItem(int count, sBonusBall* pItems);
	void UpdateListMyItem(sBonusBall* pItem);
	void InitScratchCard(void);
	void SetBmpScratchItem(void);
	void SetRectScratchItem(void);
	void SetRectCheckScratchItemRect(void);
	void SetRectCheckScratchItemRectByIndex(int index, WPoint offset);
	bool OnConfirmResult(int result, FrForm* pForm);
	void ProcessResultText(float dt);
	void SetBmpResultText(int result);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // scratchdlg.cpp:20

	void OnScratchDlgInit(int);   // scratchdlg.cpp:135
	void OnScratchBottomInit(int param);   // scratchdlg.cpp:140
	void OnScratchCover01Init(int param);   // scratchdlg.cpp:153
	void OnScratchCover02Init(int param);   // scratchdlg.cpp:171
	void OnScratchCardInit(int param);   // scratchdlg.cpp:189
	void OnScratchNextPageBnInit(int param);   // scratchdlg.cpp:201
	void OnScratchNextPageBnUp(void);   // scratchdlg.cpp:212
	void OnSerialNumberInit(int param);   // scratchdlg.cpp:233
	void OnSerialNumberBnInit(int param);   // scratchdlg.cpp:248
	void OnAboutBnInit(int param);   // scratchdlg.cpp:267
	void OnCloseBnInit(intptr_t param);   // scratchdlg.cpp:284
	void OnScratchBottomOwnerDraw(void);   // scratchdlg.cpp:146
	void OnScratchCover01OwnerDraw(void);   // scratchdlg.cpp:166
	void OnScratchCover02OwnerDraw(void);   // scratchdlg.cpp:184
	bool OnScratchCardEnterKey(int);   // scratchdlg.cpp:195
	bool OnSerialNumberEnterKey(int);   // scratchdlg.cpp:242
	void OnSerialNumberBnUp(void);   // scratchdlg.cpp:257
	void OnAboutBnUp(void);   // scratchdlg.cpp:273
	void OnCloseBnUp(void);   // scratchdlg.cpp:290
	virtual bool OnInit(void);   // scratchdlg.cpp:296
	virtual void OnProc(const float dt);   // scratchdlg.cpp:316
	virtual void OnMouseMove(const WPoint& pt);   // scratchdlg.cpp:324
	virtual bool OnLButtonDown(const WPoint& pt);   // scratchdlg.cpp:335
	virtual bool OnLButtonUp(const WPoint& pt);   // scratchdlg.cpp:356
	virtual void OnCancel(void);   // scratchdlg.cpp:365

private:
	FrArea* m_pScratchCover[2];        // 0x110, cover areas 01 and 02
	FrEdit* m_pScratchCardEdit;        // 0x118, scratch card count
	FrButton* m_pNextPageBn;           // 0x11c
	FrEdit* m_pSerialNumberEdit;       // 0x120
	FrButton* m_pSerialNumberBn;       // 0x124
	WTitleFont* m_pTitleFont;          // 0x128, scratch card number font
	const Bitmap* m_pCoverAreaBitmap[2]; // 0x12c, bitmap of each cover area at init
	Bitmap* m_pBmpClosedCover;         // 0x134, cover shown with no card
	Bitmap m_bmpScratchCover[2];       // 0x138, covers 01 and 02
	const Bitmap* m_pBmpEraser;        // 0x160, "scratch_eraser"
	const Bitmap* m_pBmpChangeCardNumber[2]; // 0x164, by card number (DrawChangeScratchCardNumber)
	const Bitmap* m_pBmpScratchItem[11]; // 0x16c, icon of each won item (SetBmpScratchItem)
	PADDING(0x198, 0x1a4);
	WRect m_rectEraseArea;             // 0x1a4, area the eraser opens (OnInit, OpenScratchCover)
	PADDING(0x1b4, 0x1bc);
	bool m_bLButtonDown;               // 0x1bc, set by OnLButtonDown
	PADDING(0x1bd, 0x1c0);
	int m_scratchCardCount;            // 0x1c0, scratch coupons owned (InitScratchCard)
	sBonusBall m_scratchItem[11];      // 0x1c4, items won by the scratch
	int m_scratchItemCount;            // 0x2a0
	int m_serialNumberResult;          // 0x2a4, result code of ResponseScratchSerialNumber
	bool m_bEnableRequestScratchItem;  // 0x2a8
	bool m_bReceivedScratchItem;       // 0x2a9, set by SetScratchItem
	bool m_bEnableSerialNumber;        // 0x2aa
	bool m_bWaitResultText;            // 0x2ab, result text shown, waiting (ProcessResultText)
	int m_changeCardNumber[2];         // 0x2ac, card number shown by each slot
	float m_changeCardTime[2];         // 0x2b4, time since the slot changed
	int m_changeCardPos[2];            // 0x2bc, scroll position of each slot
	float m_resultTextTime;            // 0x2c4, time the result text has been shown
	int m_checkScratchPos[2];          // 0x2c8
	int m_checkScratchItem[48];        // 0x2d0
};

CHECK_SIZE(FrScratchDlg, 0x390);
BEGIN_LAYOUT_CHECK(FrScratchDlg)
	CHECK_FIELD(FrScratchDlg, m_pScratchCover, 0x110);
	CHECK_FIELD(FrScratchDlg, m_pScratchCardEdit, 0x118);
	CHECK_FIELD(FrScratchDlg, m_pNextPageBn, 0x11c);
	CHECK_FIELD(FrScratchDlg, m_pSerialNumberEdit, 0x120);
	CHECK_FIELD(FrScratchDlg, m_pSerialNumberBn, 0x124);
	CHECK_FIELD(FrScratchDlg, m_pTitleFont, 0x128);
	CHECK_FIELD(FrScratchDlg, m_pCoverAreaBitmap, 0x12c);
	CHECK_FIELD(FrScratchDlg, m_pBmpClosedCover, 0x134);
	CHECK_FIELD(FrScratchDlg, m_bmpScratchCover, 0x138);
	CHECK_FIELD(FrScratchDlg, m_pBmpEraser, 0x160);
	CHECK_FIELD(FrScratchDlg, m_pBmpChangeCardNumber, 0x164);
	CHECK_FIELD(FrScratchDlg, m_pBmpScratchItem, 0x16c);
	CHECK_FIELD(FrScratchDlg, m_rectEraseArea, 0x1a4);
	CHECK_FIELD(FrScratchDlg, m_bLButtonDown, 0x1bc);
	CHECK_FIELD(FrScratchDlg, m_scratchCardCount, 0x1c0);
	CHECK_FIELD(FrScratchDlg, m_scratchItem, 0x1c4);
	CHECK_FIELD(FrScratchDlg, m_scratchItemCount, 0x2a0);
	CHECK_FIELD(FrScratchDlg, m_serialNumberResult, 0x2a4);
	CHECK_FIELD(FrScratchDlg, m_bEnableRequestScratchItem, 0x2a8);
	CHECK_FIELD(FrScratchDlg, m_bReceivedScratchItem, 0x2a9);
	CHECK_FIELD(FrScratchDlg, m_bEnableSerialNumber, 0x2aa);
	CHECK_FIELD(FrScratchDlg, m_bWaitResultText, 0x2ab);
	CHECK_FIELD(FrScratchDlg, m_changeCardNumber, 0x2ac);
	CHECK_FIELD(FrScratchDlg, m_changeCardTime, 0x2b4);
	CHECK_FIELD(FrScratchDlg, m_changeCardPos, 0x2bc);
	CHECK_FIELD(FrScratchDlg, m_resultTextTime, 0x2c4);
	CHECK_FIELD(FrScratchDlg, m_checkScratchPos, 0x2c8);
	CHECK_FIELD(FrScratchDlg, m_checkScratchItem, 0x2d0);
END_LAYOUT_CHECK

// Result card of a scratch; its size (0x158) is the `new` of
// FrScratchResultDlgMakeInstance. Only the members this unit defines so far
// are declared.
class FrScratchResultDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scratchdlg.h:172

	FrScratchResultDlg(void);   // scratchdlg.cpp:1324
	virtual ~FrScratchResultDlg(void);   // scratchdlg.cpp:1353

	void SetResultItem(sBonusBall* pItems, int count);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // scratchdlg.cpp:1313

	void OnScratchResultDlgInit(int);   // scratchdlg.cpp:1364
	void OnResultItemIconInit(intptr_t param);   // scratchdlg.cpp:1369
	void OnResultItemIconOwnerDraw(void);   // scratchdlg.cpp:1375
	void OnResultItemNameInit(intptr_t param);   // scratchdlg.cpp:1468
	void OnResultItemAbilityInit(intptr_t param);   // scratchdlg.cpp:1474
	virtual bool OnInit(void);   // scratchdlg.cpp:1480
	virtual void OnProc(const float dt);   // scratchdlg.cpp:1486

private:
	FrArea* m_pResultItemIcon;         // 0x110
	FrEdit* m_pResultItemName;         // 0x114
	FrEdit* m_pResultItemAbility;      // 0x118
	WTitleFont* m_pTitleFont;          // 0x11c
	sBonusBall m_resultItem[2];        // 0x120, copied by SetResultItem
	int m_resultItemCount;             // 0x148
	bool m_bShowItemDetail;            // 0x14c, set by SetResultItem unless the item kind is 5 or 6 (OnResultItemIconOwnerDraw)
	PADDING(0x14d, 0x150);
	const Bitmap* m_pBmpResultItem[2]; // 0x150, icon of each result item (SetResultItem)
};

CHECK_SIZE(FrScratchResultDlg, 0x158);
BEGIN_LAYOUT_CHECK(FrScratchResultDlg)
	CHECK_FIELD(FrScratchResultDlg, m_pResultItemIcon, 0x110);
	CHECK_FIELD(FrScratchResultDlg, m_pResultItemName, 0x114);
	CHECK_FIELD(FrScratchResultDlg, m_pResultItemAbility, 0x118);
	CHECK_FIELD(FrScratchResultDlg, m_pTitleFont, 0x11c);
	CHECK_FIELD(FrScratchResultDlg, m_resultItem, 0x120);
	CHECK_FIELD(FrScratchResultDlg, m_resultItemCount, 0x148);
	CHECK_FIELD(FrScratchResultDlg, m_bShowItemDetail, 0x14c);
	CHECK_FIELD(FrScratchResultDlg, m_pBmpResultItem, 0x150);
END_LAYOUT_CHECK

// Help pages of the scratch card; its size (0x138) is the `new` of
// FrScratchAboutDlgMakeInstance. Only the members this unit defines so far
// are declared.
class FrScratchAboutDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scratchdlg.h:217

	FrScratchAboutDlg(void);   // scratchdlg.cpp:1661
	virtual ~FrScratchAboutDlg(void);   // scratchdlg.cpp:1675

	void SetImgAboutIndex(int index);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // scratchdlg.cpp:1638

	void OnAboutViewerInit(intptr_t param);   // scratchdlg.cpp:1679
	void OnIndex01Init(intptr_t param);   // scratchdlg.cpp:1685
	void OnIndex02Init(intptr_t param);   // scratchdlg.cpp:1695
	void OnIndex03Init(intptr_t param);   // scratchdlg.cpp:1705
	void OnIndex04Init(intptr_t param);   // scratchdlg.cpp:1715
	void OnIndex05Init(intptr_t param);   // scratchdlg.cpp:1725
	void OnBackwardBnInit(intptr_t param);   // scratchdlg.cpp:1735
	void OnForwardBnInit(intptr_t param);   // scratchdlg.cpp:1746
	void OnIndex01BnUp(void);   // scratchdlg.cpp:1690
	void OnIndex02BnUp(void);   // scratchdlg.cpp:1700
	void OnIndex03BnUp(void);   // scratchdlg.cpp:1710
	void OnIndex04BnUp(void);   // scratchdlg.cpp:1720
	void OnIndex05BnUp(void);   // scratchdlg.cpp:1730
	void OnBackwardBnUp(void);   // scratchdlg.cpp:1740
	void OnForwardBnUp(void);   // scratchdlg.cpp:1751
	void OnOkBnInit(intptr_t param);   // scratchdlg.cpp:1757
	void OnOkBnUp(void);   // scratchdlg.cpp:1762
	virtual bool OnInit(void);   // scratchdlg.cpp:1767

private:
	FrTextButton* m_pIndexBn[5];       // 0x110, page buttons 01-05
	int m_curIndex;                    // 0x124, page shown
	FrButton* m_pBackwardBn;           // 0x128
	FrButton* m_pForwardBn;            // 0x12c
	FrViewer* m_pAboutViewer;          // 0x130
	PADDING(0x134, 0x138);
};

CHECK_SIZE(FrScratchAboutDlg, 0x138);
BEGIN_LAYOUT_CHECK(FrScratchAboutDlg)
	CHECK_FIELD(FrScratchAboutDlg, m_pIndexBn, 0x110);
	CHECK_FIELD(FrScratchAboutDlg, m_curIndex, 0x124);
	CHECK_FIELD(FrScratchAboutDlg, m_pBackwardBn, 0x128);
	CHECK_FIELD(FrScratchAboutDlg, m_pForwardBn, 0x12c);
	CHECK_FIELD(FrScratchAboutDlg, m_pAboutViewer, 0x130);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SCRATCHDLG_H
