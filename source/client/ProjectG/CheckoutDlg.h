// Original: d:\build\custom\temp\code\client\projectg\checkoutdlg.h
#ifndef PG_CLIENT_PROJECTG_CHECKOUTDLG_H
#define PG_CLIENT_PROJECTG_CHECKOUTDLG_H

#include <list>
#include <map>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrButton;
class FrComboBox;
class FrEdit;
class FrListBox;
class FrStatic;
struct sCaddieInfo;

// Shop checkout dialog (cart, prices in pang and cookies, rental days); its
// size (0x3d0) is the `new` of FrCheckoutDlgMakeInstance. Only the members
// this unit's written functions use are named.
class FrCheckoutDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	struct CartItem;

	// Rental-days selector of a cart line (two per dialog, 12 bytes).
	class MiniBtn {
	public:
		MiniBtn(void);

		FrComboBox* m_pDays;        // 0x0
		CartItem* m_pCartItem;      // 0x4, cart line the selector edits
		bool m_bChanged;            // 0x8
	};

	// One line of the cart (std::list node value, 0x14 bytes).
	struct CartItem {
		void* m_pItem;              // 0x0, shop item of the line
		UNKNOWN(0x4, 0xc);
		int m_days;                 // 0xc, index of the rental period
		MiniBtn* m_pMiniBtn;        // 0x10
	};

	// A discount coupon of the player (std::map value, 0x10 bytes).
	struct sCoupon {
		UNKNOWN(0x0, 0x8);
		bool m_bUsable;             // 0x8
		int m_period;               // 0xc
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // checkoutdlg.h:40

	FrCheckoutDlg(void);

	bool IsUnderLevel(void);
	bool IsOverLevel(void);
	std::list<CartItem>& Cart(void);
	void Process(void);
	void Checkout(__int64 pang, __int64 cookie, bool b);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void SetPrice(int num);
	void RecalculateTotalCookie(__int64 pang, __int64 cookie, bool b);
	unsigned long GetGuidSelectedCoupon(void);
	void OnDaysBtnDown(void);
	void OnDays2BtnDown(void);
	bool OnNumEnterKey(intptr_t key);
	void OnNumPrevLBtnUp(void);
	void OnNumNextLBtnUp(void);
	bool OnChargeResult(int ret, FrForm* pForm);
	void OnCheckoutInit(intptr_t param);
	void OnDaysInit(intptr_t param);
	void OnDays2Init(int param);
	void OnCartInit(intptr_t param);
	void OnPriceInit(intptr_t param);
	void OnMyMoneyInit(intptr_t param);
	void OnMoneyLeftInit(intptr_t param);
	void OnPrice_CookieInit(intptr_t param);
	void OnMyMoney_CookieInit(intptr_t param);
	void OnMoneyLeft_CookieInit(intptr_t param);
	void OnNumStaticInit(intptr_t param);
	void OnExplaneStaticInit(intptr_t param);
	void OnNumInit(intptr_t param);
	void OnNumPrevInit(intptr_t param);
	void OnNumNextInit(intptr_t param);
	void OpenCaddieInfo(const sCaddieInfo& info);

	FrButton* m_pCheckout;               // 0x110
	FrListBox* m_pCart;                  // 0x114
	FrEdit* m_pPrice;                    // 0x118
	FrEdit* m_pMyMoney;                  // 0x11c
	FrEdit* m_pMoneyLeft;                // 0x120
	FrEdit* m_pPrice_Cookie;             // 0x124
	FrEdit* m_pMyMoney_Cookie;           // 0x128
	FrEdit* m_pMoneyLeft_Cookie;         // 0x12c
	FrStatic* m_pNumStatic;              // 0x130
	FrStatic* m_pExplaneStatic;          // 0x134
	FrEdit* m_pNum;                      // 0x138
	FrButton* m_pNumPrev;                // 0x13c
	FrButton* m_pNumNext;                // 0x140
	MiniBtn m_miniBtn[2];                // 0x144
	std::list<CartItem> m_cart;          // 0x15c
	bool m_bUnderLevel;                  // 0x168
	bool m_bOverLevel;                   // 0x169
	bool m_flag16a;                      // 0x16a
	bool m_bCheckoutFlag;                // 0x16b, third argument of Checkout
	PADDING(0x16c, 0x180);               // FrCheckoutDlg members
	bool m_flag180;                      // 0x180
	bool m_flag181;                      // 0x181
	int m_value184;                      // 0x184
	int m_num;                           // 0x188, purchase count (m_pNum)
	PADDING(0x18c, 0x190);               // FrCheckoutDlg members
	__int64 m_pang;                      // 0x190
	__int64 m_cookie;                    // 0x198
	__int64 m_value1a0;                  // 0x1a0
	bool m_flag1a8;                      // 0x1a8
	std::map<unsigned int, sCoupon> m_coupons;   // 0x1ac
	bool m_bCouponFlag;                  // 0x1b8, cleared by GetGuidSelectedCoupon
	int m_selectedCoupon;                // 0x1bc
	int m_value1c0;                      // 0x1c0
	PADDING(0x1c4, 0x1c8);               // FrCheckoutDlg members
	__int64 m_value1c8;                  // 0x1c8
	__int64 m_value1d0;                  // 0x1d0
	int m_value1d8;                      // 0x1d8
	bool m_flag1dc;                      // 0x1dc
	PADDING(0x1dd, 0x270);               // FrCheckoutDlg members
	WRect m_clockRect;                   // 0x270, where the "clock" icon is drawn
	const Bitmap* m_pClockIcon;          // 0x280
	PADDING(0x284, 0x288);               // FrCheckoutDlg members
	int m_dayNameCount;                  // 0x288, entries of m_dayNames in use
	PADDING(0x28c, 0x290);               // FrCheckoutDlg members
	char m_dayNames[5][64];              // 0x290, rental period labels
};

CHECK_SIZE(FrCheckoutDlg, 0x3d0);
BEGIN_LAYOUT_CHECK(FrCheckoutDlg)
	CHECK_FIELD(FrCheckoutDlg, m_pCheckout, 0x110);
	CHECK_FIELD(FrCheckoutDlg, m_pNumNext, 0x140);
	CHECK_FIELD(FrCheckoutDlg, m_miniBtn, 0x144);
	CHECK_FIELD(FrCheckoutDlg, m_cart, 0x15c);
	CHECK_FIELD(FrCheckoutDlg, m_bUnderLevel, 0x168);
	CHECK_FIELD(FrCheckoutDlg, m_bCheckoutFlag, 0x16b);
	CHECK_FIELD(FrCheckoutDlg, m_flag180, 0x180);
	CHECK_FIELD(FrCheckoutDlg, m_value184, 0x184);
	CHECK_FIELD(FrCheckoutDlg, m_num, 0x188);
	CHECK_FIELD(FrCheckoutDlg, m_pang, 0x190);
	CHECK_FIELD(FrCheckoutDlg, m_cookie, 0x198);
	CHECK_FIELD(FrCheckoutDlg, m_value1a0, 0x1a0);
	CHECK_FIELD(FrCheckoutDlg, m_flag1a8, 0x1a8);
	CHECK_FIELD(FrCheckoutDlg, m_coupons, 0x1ac);
	CHECK_FIELD(FrCheckoutDlg, m_bCouponFlag, 0x1b8);
	CHECK_FIELD(FrCheckoutDlg, m_selectedCoupon, 0x1bc);
	CHECK_FIELD(FrCheckoutDlg, m_value1c0, 0x1c0);
	CHECK_FIELD(FrCheckoutDlg, m_value1c8, 0x1c8);
	CHECK_FIELD(FrCheckoutDlg, m_value1d8, 0x1d8);
	CHECK_FIELD(FrCheckoutDlg, m_flag1dc, 0x1dc);
	CHECK_FIELD(FrCheckoutDlg, m_clockRect, 0x270);
	CHECK_FIELD(FrCheckoutDlg, m_pClockIcon, 0x280);
	CHECK_FIELD(FrCheckoutDlg, m_dayNameCount, 0x288);
	CHECK_FIELD(FrCheckoutDlg, m_dayNames, 0x290);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CHECKOUTDLG_H
