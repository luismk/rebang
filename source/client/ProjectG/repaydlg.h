// Original: d:\build\custom\temp\code\client\projectg\repaydlg.h
#ifndef PG_CLIENT_PROJECTG_REPAYDLG_H
#define PG_CLIENT_PROJECTG_REPAYDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "UccManager.h"

class FrEdit;
class FrListBox;

// Repay dialog: lists the item to pay for in a cart list box and shows its
// pang and cookie prices, the player's money and what is left after paying;
// its size (0x1D0) is the `new` of FrRepayDlgMakeInstance.
class FrRepayDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // repaydlg.h:20

	FrRepayDlg(void);

	void Repay(const IFF_ITEM_COMMON& item, unsigned long guid, __int64 myPang, __int64 myCookie);

	unsigned long GetTid(void) { return m_item.m_typeId; }          // repaydlg.h:27
	unsigned long GetGuid(void) { return m_guid; }                  // repaydlg.h:28

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnCartInit(intptr_t param);
	void OnCartOwnerDraw(intptr_t param);
	void OnPriceInit(intptr_t param);
	void OnMyMoneyInit(intptr_t param);
	void OnMoneyLeftInit(intptr_t param);
	void OnPrice_CookieInit(intptr_t param);
	void OnMyMoney_CookieInit(intptr_t param);
	void OnMoneyLeft_CookieInit(intptr_t param);

private:
	FrListBox* m_pCart;                // 0x110 (OnCartInit)
	FrEdit* m_pPrice;                  // 0x114 (OnPriceInit)
	FrEdit* m_pMyMoney;                // 0x118 (OnMyMoneyInit)
	FrEdit* m_pMoneyLeft;              // 0x11c (OnMoneyLeftInit)
	FrEdit* m_pPriceCookie;            // 0x120 (OnPrice_CookieInit)
	FrEdit* m_pMyMoneyCookie;          // 0x124 (OnMyMoney_CookieInit)
	FrEdit* m_pMoneyLeftCookie;        // 0x128 (OnMoneyLeft_CookieInit)
	IFF_ITEM_COMMON m_item;            // 0x12c (the item being paid for)
	unsigned long m_guid;              // 0x1bc
	__int64 m_pangPrice;               // 0x1c0 (total in pang)
	__int64 m_cookiePrice;             // 0x1c8 (total in cookies)
};
BEGIN_LAYOUT_CHECK(FrRepayDlg)
	CHECK_FIELD(FrRepayDlg, m_pCart, 0x110);
	CHECK_FIELD(FrRepayDlg, m_pPrice, 0x114);
	CHECK_FIELD(FrRepayDlg, m_pMyMoney, 0x118);
	CHECK_FIELD(FrRepayDlg, m_pMoneyLeft, 0x11c);
	CHECK_FIELD(FrRepayDlg, m_pPriceCookie, 0x120);
	CHECK_FIELD(FrRepayDlg, m_pMyMoneyCookie, 0x124);
	CHECK_FIELD(FrRepayDlg, m_pMoneyLeftCookie, 0x128);
	CHECK_FIELD(FrRepayDlg, m_item, 0x12c);
	CHECK_FIELD(FrRepayDlg, m_guid, 0x1bc);
	CHECK_FIELD(FrRepayDlg, m_pangPrice, 0x1c0);
	CHECK_FIELD(FrRepayDlg, m_cookiePrice, 0x1c8);
END_LAYOUT_CHECK
CHECK_SIZE(FrRepayDlg, 0x1d0);

#endif // PG_CLIENT_PROJECTG_REPAYDLG_H
