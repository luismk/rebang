// Original: d:\build\custom\temp\code\client\projectg\repaydlg.cpp
#include "RepayDlg.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "binstr.h"
#include "bitmap.h"
#include "fredit.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frwndmanager.h"
#include <stdio.h>
#include <string.h>

// repaydlg.cpp:28 (run-time type and factory registration of FrRepayDlg)
const WRTTI FrRepayDlg::m_RTTI("FrRepayDlg", &FrForm::m_RTTI);

IObject* FrRepayDlgMakeInstance(void) {
	return new FrRepayDlg;
}

struct __sFrRepayDlg {
	__sFrRepayDlg(void) {
		ObjectFactory().AddObjectFunctor(FrRepayDlgMakeInstance, "FrRepayDlg");
	}
};
static __sFrRepayDlg s_registerFrRepayDlg;

// repaydlg.cpp:30
const sFRESH_MSGMAP* FrRepayDlg::GetMessageMap(void) const {
	return &FrRepayDlg::_MsgMap;
}

sFRESH_MSGMAP FrRepayDlg::_MsgMap = { &FrForm::_MsgMap, &FrRepayDlg::_MsgEntries[0] };

sFRESH_ENTRY FrRepayDlg::_MsgEntries[] = {
	// repaydlg.cpp:32
	{ "cart", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRepayDlg::OnCartInit },
	// repaydlg.cpp:33
	{ "cart", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRepayDlg::OnCartOwnerDraw },
	// repaydlg.cpp:34
	{ "price", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRepayDlg::OnPriceInit },
	// repaydlg.cpp:35
	{ "my_money", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRepayDlg::OnMyMoneyInit },
	// repaydlg.cpp:36
	{ "money_left", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRepayDlg::OnMoneyLeftInit },
	// repaydlg.cpp:37
	{ "price_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRepayDlg::OnPrice_CookieInit },
	// repaydlg.cpp:38
	{ "my_money_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRepayDlg::OnMyMoney_CookieInit },
	// repaydlg.cpp:39
	{ "money_left_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRepayDlg::OnMoneyLeft_CookieInit },

	// repaydlg.cpp:41
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// repaydlg.cpp:45
FrRepayDlg::FrRepayDlg(void) {
	memset(&m_item, 0, sizeof(IFF_ITEM_COMMON));
	m_guid = 0;
	m_pCart = 0;
	m_pPrice = 0;
	m_pMyMoney = 0;
	m_pMoneyLeft = 0;
	m_pPriceCookie = 0;
	m_pMyMoneyCookie = 0;
	m_pMoneyLeftCookie = 0;
}

// repaydlg.cpp:59
void FrRepayDlg::Repay(const IFF_ITEM_COMMON& item, unsigned long guid, __int64 myPang, __int64 myCookie) {
	m_item = item;
	m_guid = guid;
	m_pangPrice = 0;
	m_cookiePrice = 0;

	if (m_pCart) {

		m_pCart->ClearItem();

		if (m_item.m_flags0 & 1) {
			m_cookiePrice += m_item.m_refundPrice;
		} else {
			m_pangPrice += m_item.m_refundPrice;
		}
		m_pCart->AddItem(&m_item);
	}

	char buf[32];
	if (m_pPrice) {


		MakePriceStrA(buf, m_pangPrice);
		strcat(buf, "\xC6\xCE");
		m_pPrice->SetLine(1, buf, 0, false, 0);
	}

	if (m_pPriceCookie) {


		sprintf(buf, "\xC4\xED\xC5\xB0 %I64d\xB0\xB3", m_cookiePrice);
		m_pPriceCookie->SetLine(1, buf, 0, false, 0);
	}

	if (m_pMyMoney) {


		MakePriceStrA(buf, myPang);
		strcat(buf, "\xC6\xCE");
		m_pMyMoney->SetLine(1, buf, 0, false, 0);
	}

	if (m_pMyMoneyCookie) {


		sprintf(buf, "\xC4\xED\xC5\xB0 %I64d\xB0\xB3", myCookie);
		m_pMyMoneyCookie->SetLine(1, buf, 0, false, 0);
	}

	if (m_pPrice && m_pMyMoney && m_pangPrice) {


		MakePriceStrA(buf, m_pangPrice);
		strcat(buf, "\xC6\xCE");
		m_pPrice->SetLine(1, buf, 0, false, 0);



		if (WSingleton<CSharedDoc>::Instance()->m_myPang + m_pangPrice < 0) {

			if (m_pMoneyLeft) {

				MakePriceStrA(buf, m_pangPrice + myPang);
				strcat(buf, " \xBA\xCE\xC1\xB7");
				m_pMoneyLeft->SetLine(1, buf, 0, false, 0);
			}

			FrWnd* pOk = FindChildByName("ok");
			if (pOk) {
				pOk->Enable(false);
			}

		} else {

			if (m_pMoneyLeft) {

				MakePriceStrA(buf, m_pangPrice + myPang);
				strcat(buf, "\xC6\xCE");
				m_pMoneyLeft->SetLine(1, buf, 0, false, 0);
			}

			FrWnd* pOk = FindChildByName("ok");
			if (pOk) {
				pOk->Enable(true);
			}
		}
	} else if (m_pMoneyLeft) {
		m_pMoneyLeft->SetLine(1, m_pMyMoney->GetLine(1, false), 0, false, 0);
	}

	if (m_pPriceCookie && m_pMyMoneyCookie && m_cookiePrice) {


		sprintf(buf, "\xC4\xED\xC5\xB0 %I64d\xB0\xB3", m_cookiePrice);
		m_pPriceCookie->SetLine(1, buf, 0, false, 0);

		__int64 myCash = WSingleton<CSharedDoc>::Instance()->m_myCash;

		if (myCash + m_cookiePrice < 0) {

			if (m_pMoneyLeftCookie) {

				MakePriceStrA(buf, myCash + m_cookiePrice);
				strcat(buf, " \xBA\xCE\xC1\xB7");
				m_pMoneyLeftCookie->SetLine(1, buf, 0, false, 0);
			}

			FrWnd* pOk = FindChildByName("ok");
			if (pOk) {
				pOk->Enable(false);
			}

		} else {

			if (m_pMoneyLeftCookie) {

				sprintf(buf, "\xC4\xED\xC5\xB0 %I64d\xB0\xB3", myCash + m_cookiePrice);
				m_pMoneyLeftCookie->SetLine(1, buf, 0, false, 0);
			}

			FrWnd* pOk = FindChildByName("ok");
			if (pOk) {
				pOk->Enable(true);
			}
		}
	} else if (m_pMoneyLeftCookie) {
		m_pMoneyLeftCookie->SetLine(1, m_pMyMoneyCookie->GetLine(1, false), 0, false, 0);
	}
}

// repaydlg.cpp:188
void FrRepayDlg::OnCartInit(intptr_t param) {
	m_pCart = DYNAMIC_CAST(FrListBox, param);
}

// repaydlg.cpp:194
void FrRepayDlg::OnCartOwnerDraw(intptr_t param) {

	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0) {
		return;
	}
	FrGraphicInterface* pGDI = m_pCart->WndManager()->GetGDI();


	const IFF_ITEM_COMMON* pData = (const IFF_ITEM_COMMON*)pItem->pData;

	const Bitmap* pBitmap = m_pCart->WndManager()->GetBitmap("ITEMS", pData->m_icon);
	if (pBitmap == 0) {
		pBitmap = m_pCart->WndManager()->GetBitmap("ITEMS_FASHION", pData->m_icon);
	}
	if (pBitmap) {

		WRect rect(pItem->pos.x, pItem->pos.y, pBitmap->Width(), pBitmap->Height());
		pGDI->DrawTexture(pBitmap, rect, 0xFFFFFFFF, 0);
	}



	pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	pGDI->SetTextStyle(1);
	pGDI->Print(WPoint(pItem->pos.x + 90.0f, pItem->pos.y + 14.0f), 0, "%s", pData->m_name);

	pGDI->SetTextStyle(0);

	if (pData->m_flags0 & 1) {
		pGDI->Print(WPoint(pItem->pos.x + 90.0f, pItem->pos.y + 34.0f), 0, "\xBF\xF8\xB7\xA1\xB0\xA1\xB0\xDD:   \xC4\xED\xC5\xB0 %d\xB0\xB3", pData->m_price);
		pGDI->Print(WPoint(pItem->pos.x + 90.0f, pItem->pos.y + 54.0f), 0, "\xC8\xAF\xBA\xD2\xB0\xA1\xB0\xDD:   \xC4\xED\xC5\xB0 %d\xB0\xB3", pData->m_refundPrice);

	} else {

		pGDI->Print(WPoint(pItem->pos.x + 90.0f, pItem->pos.y + 34.0f), 0, "\xBF\xF8\xB7\xA1\xB0\xA1\xB0\xDD:   %d\xC6\xCE", pData->m_price);
		pGDI->Print(WPoint(pItem->pos.x + 90.0f, pItem->pos.y + 54.0f), 0, "\xC8\xAF\xBA\xD2\xB0\xA1\xB0\xDD:   %d\xC6\xCE", pData->m_refundPrice);
	}



	if ((pData->m_typeId & 0xFC000000) == 0x1C000000) {

		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
		pGDI->Print(WPoint(pItem->pos.x + 90.0f, pItem->pos.y + 74.0f), 0, "\xBB\xE7\xBF\xEB\xB1\xE2\xB0\xA3:");
		pGDI->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
		float x = pItem->pos.x + (pGDI->GetTextExtend("\xBB\xE7\xBF\xEB\xB1\xE2\xB0\xA3:") + 95.0f);
		pGDI->Print(WPoint(x, pItem->pos.y + 74.0f), 0, "\xB1\xB8\xC0\xD4\xC0\xCF\xB7\xCE \xBA\xCE\xC5\xCD %d\xC0\xCF \xB5\xBF\xBE\xC8\xB8\xB8", 30);
	}
}

// repaydlg.cpp:243
void FrRepayDlg::OnPriceInit(intptr_t param) {
	m_pPrice = DYNAMIC_CAST(FrEdit, param);
}

// repaydlg.cpp:249
void FrRepayDlg::OnMyMoneyInit(intptr_t param) {
	m_pMyMoney = DYNAMIC_CAST(FrEdit, param);
}

// repaydlg.cpp:255
void FrRepayDlg::OnMoneyLeftInit(intptr_t param) {
	m_pMoneyLeft = DYNAMIC_CAST(FrEdit, param);
}

// repaydlg.cpp:261
void FrRepayDlg::OnPrice_CookieInit(intptr_t param) {
	m_pPriceCookie = DYNAMIC_CAST(FrEdit, param);



}

// repaydlg.cpp:269
void FrRepayDlg::OnMyMoney_CookieInit(intptr_t param) {
	m_pMyMoneyCookie = DYNAMIC_CAST(FrEdit, param);



}

// repaydlg.cpp:277
void FrRepayDlg::OnMoneyLeft_CookieInit(intptr_t param) {
	m_pMoneyLeftCookie = DYNAMIC_CAST(FrEdit, param);



}
