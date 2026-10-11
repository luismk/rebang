// Original: d:\build\custom\temp\code\client\projectg\giftdlg.h
#ifndef PG_CLIENT_PROJECTG_GIFTDLG_H
#define PG_CLIENT_PROJECTG_GIFTDLG_H

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "UccManager.h"

class FrButton;
class FrComboBox;
struct sPangYaUserInfo;
class FrEmoticonDlg;
class FrEdit;
class FrFriendDlg;
class FrListBox;
class FrStatic;

// Gift (send an item to another player) dialog; its size (0x470) is the
// `new` of FrGiftDlgMakeInstance. Its destructor is compiler-generated (no
// line marks). Only the members this unit defines so far are declared.
class FrGiftDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
	// FrFriendDlg fills the receiver edit boxes of its parent dialog
	friend class FrFriendDlg;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // giftdlg.h:25

	// One selectable rental period with its price, read by
	// RecalculateTotalCookie and GiftProgress.
	struct sPeriodPrice {
		char name[0x20];   // 0x0, the text shown in the days combo box
		UNKNOWN(0x20, 0x40);
	};

	class MiniBtn {
	public:
		MiniBtn(void);
	};

	FrGiftDlg(void);   // giftdlg.cpp:77

	void SetReceiverInfo(unsigned long uid, const sPangYaUserInfo& info);   // giftdlg.cpp:476
	void GiftProgress(void);
	void UpdateFriendList(void);   // giftdlg.h:33, emitted by taskmain.cpp

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // giftdlg.cpp:28

	bool OnFriendListResult(int ret, FrForm* pForm);   // giftdlg.cpp:618
	bool OnConfirmResult(int ret, FrForm* pForm);   // giftdlg.cpp:763
	bool OnChargeResult(int ret, FrForm* pForm);
	void OnCheckoutInit(intptr_t param);   // giftdlg.cpp:789
	void OnFriendBtnInit(intptr_t param);   // giftdlg.cpp:863
	void OnCartInit(intptr_t param);   // giftdlg.cpp:901
	void OnCartOwnerDraw(intptr_t param);
	void OnFriendBtnUp(void);   // giftdlg.cpp:869
	void OnSxIdInit(intptr_t param);   // giftdlg.cpp:1353
	void OnEdIdInit(intptr_t param);   // giftdlg.cpp:1365
	bool OnEdIdEnterKey(int);   // giftdlg.cpp:1380
	void OnEdIdDown(void);   // giftdlg.cpp:1387
	void OnEdNickDown(void);   // giftdlg.cpp:1394
	void OnBtIdCheckInit(intptr_t param);   // giftdlg.cpp:1401
	void OnBtIdCheckUp(void);   // giftdlg.cpp:1413
	void OnSxNickInit(intptr_t param);   // giftdlg.cpp:1446
	void OnEdNickInit(intptr_t param);   // giftdlg.cpp:1451
	void OnBtNickCheckUp(void);
	bool OnEdNickEnterKey(int);   // giftdlg.cpp:1463
	void OnCheckoutBtnUp(void);
	void OnMsgInit(intptr_t param);   // giftdlg.cpp:1470
	bool OnMsgEnterKey(int);   // giftdlg.cpp:1476
	void OnEdConfirmInit(intptr_t param);   // giftdlg.cpp:1483
	void OnBtNickCheckInit(intptr_t param);   // giftdlg.cpp:1492
	void OnPriceInit(intptr_t param);   // giftdlg.cpp:1552
	void OnMyMoneyInit(intptr_t param);   // giftdlg.cpp:1558
	void OnMoneyLeftInit(intptr_t param);   // giftdlg.cpp:1564
	void OnPrice_CookieInit(intptr_t param);   // giftdlg.cpp:1570
	void OnMyMoney_CookieInit(intptr_t param);   // giftdlg.cpp:1578
	void OnMoneyLeft_CookieInit(intptr_t param);   // giftdlg.cpp:1586
	void OnNumStaticInit(intptr_t param);   // giftdlg.cpp:1594
	void OnNumInit(intptr_t param);   // giftdlg.cpp:1600
	void OnNumPrevInit(intptr_t param);   // giftdlg.cpp:1606
	void OnNumPrevBtnUp(void);   // giftdlg.cpp:1614
	void OnNumNextInit(intptr_t param);   // giftdlg.cpp:1627
	void OnNumNextBtnUp(void);   // giftdlg.cpp:1635
	void OnDaysInit(intptr_t param);   // giftdlg.cpp:1831
	void OnWarnInit(intptr_t param);   // giftdlg.cpp:1868
	void OnEmoticonBtnInit(intptr_t param);   // giftdlg.cpp:1878
	void OnExplaneStaticInit(intptr_t param);   // giftdlg.cpp:1901
	void SetPrice(int num);
	void RecalculateTotalCookie(__int64 pang, __int64 cookie, bool update);
	void OnDaysBtnDown(void);   // giftdlg.cpp:1836
	void OnEmoticonBtnUp(void);   // giftdlg.cpp:1883
	bool OnEmoticonResult(int ret, FrForm* pForm);   // giftdlg.cpp:1907

private:
	FrButton* m_pCheckoutBtn;          // 0x110
	FrButton* m_pFriendBtn;            // 0x114
	FrListBox* m_pCart;                // 0x118
	FrStatic* m_pSxId;                 // 0x11c
	FrEdit* m_pEdId;                   // 0x120
	FrButton* m_pBtIdCheck;            // 0x124
	FrStatic* m_pSxNick;               // 0x128
	FrEdit* m_pEdNick;                 // 0x12c
	FrButton* m_pBtNickCheck;          // 0x130
	FrEdit* m_pEdConfirm;              // 0x134
	FrEdit* m_pMsg;                    // 0x138
	FrEdit* m_pPrice;                  // 0x13c
	FrEdit* m_pMyMoney;                // 0x140
	FrEdit* m_pMoneyLeft;              // 0x144
	FrEdit* m_pPriceCookie;            // 0x148
	FrEdit* m_pMyMoneyCookie;          // 0x14c
	FrEdit* m_pMoneyLeftCookie;        // 0x150
	FrStatic* m_pNumStatic;            // 0x154
	FrEdit* m_pNum;                    // 0x158
	FrButton* m_pNumPrev;              // 0x15c
	FrButton* m_pNumNext;              // 0x160
	FrStatic* m_pWarn;                 // 0x164
	FrStatic* m_pExplaneStatic;        // 0x168
	FrButton* m_pEmoticonBtn;          // 0x16c
	FrEmoticonDlg* m_pEmoticonDlg;     // 0x170, cleared by OnEmoticonResult
	FrComboBox* m_pDays;               // 0x174
	int m_daysIndex;                   // 0x178, the selected period (OnDaysBtnDown)
	PADDING(0x17c, 0x180);
	IFF_ITEM_COMMON m_item;            // 0x180, the item to give (SendGift)
	__int64 m_pangPrice;               // 0x210, pang per item (SetPrice multiplies it by the count; SendGift zeroes it)
	__int64 m_cookiePrice;             // 0x218, cookie price (SetPrice; SendGift zeroes it)
	std::string m_receiverNick;        // 0x220
	unsigned long m_receiverUID;       // 0x23c, -1: none
	bool m_bPayPang;                   // 0x240, set by SetPrice
	bool m_bPayCookie;                 // 0x241, set by SetPrice
	FrFriendDlg* m_pFriendListDlg;     // 0x244, cleared by OnFriendListResult
	__int64 m_pangAmount;              // 0x248
	__int64 m_cookieAmount;            // 0x250
	__int64 m_totalCookie;             // 0x258, set by RecalculateTotalCookie
	bool m_bTotalCookieSet;            // 0x260, set by RecalculateTotalCookie
	PADDING(0x261, 0x2f4);
	WRect m_rcClock;                   // 0x2f4, (40, 35) and the clock icon's size (OnCartInit)
	const Bitmap* m_pClock;            // 0x304, ICONS "clock"
	PADDING(0x308, 0x30c);
	int m_periodCount;                 // 0x30c, entries of m_periods (0: the default days table)
	sPeriodPrice m_periods[5];         // 0x310, matched by name against the days combo box
	std::string m_message;             // 0x450
	int m_receiverMode;                // 0x46c, -1: none, 0: by id, 1: by nick
};

CHECK_SIZE(FrGiftDlg::sPeriodPrice, 0x40);
CHECK_SIZE(FrGiftDlg, 0x470);
BEGIN_LAYOUT_CHECK(FrGiftDlg)
	CHECK_FIELD(FrGiftDlg, m_pCheckoutBtn, 0x110);
	CHECK_FIELD(FrGiftDlg, m_pDays, 0x174);
	CHECK_FIELD(FrGiftDlg, m_item, 0x180);
	CHECK_FIELD(FrGiftDlg, m_pangPrice, 0x210);
	CHECK_FIELD(FrGiftDlg, m_cookiePrice, 0x218);
	CHECK_FIELD(FrGiftDlg, m_receiverNick, 0x220);
	CHECK_FIELD(FrGiftDlg, m_receiverUID, 0x23c);
	CHECK_FIELD(FrGiftDlg, m_bPayPang, 0x240);
	CHECK_FIELD(FrGiftDlg, m_bPayCookie, 0x241);
	CHECK_FIELD(FrGiftDlg, m_pFriendListDlg, 0x244);
	CHECK_FIELD(FrGiftDlg, m_pangAmount, 0x248);
	CHECK_FIELD(FrGiftDlg, m_cookieAmount, 0x250);
	CHECK_FIELD(FrGiftDlg, m_totalCookie, 0x258);
	CHECK_FIELD(FrGiftDlg, m_bTotalCookieSet, 0x260);
	CHECK_FIELD(FrGiftDlg, m_daysIndex, 0x178);
	CHECK_FIELD(FrGiftDlg, m_rcClock, 0x2f4);
	CHECK_FIELD(FrGiftDlg, m_pClock, 0x304);
	CHECK_FIELD(FrGiftDlg, m_periodCount, 0x30c);
	CHECK_FIELD(FrGiftDlg, m_periods, 0x310);
	CHECK_FIELD(FrGiftDlg, m_message, 0x450);
	CHECK_FIELD(FrGiftDlg, m_receiverMode, 0x46c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GIFTDLG_H
