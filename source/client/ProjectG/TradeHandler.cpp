// Original: d:\build\custom\temp\code\client\projectg\tradehandler.cpp
#include "TradeHandler.h"
#include "GatewayDoc.h"
#include "actor.h"
#include "ChatMsg.h"
#include "SharedDoc.h"
#include "TradeDlg.h"
#include "commonutil.h"
#include "fresh.h"
#include "frdesktop.h"
#include "frwndmanager.h"
#include "MessageManager.h"
#include "UccLibrary.h"
#include "UccManager.h"
#include "../shared/Localize.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

// shareddoc.h:560
void CSharedDoc::SetStateTrade(eStateTrade state) {
	m_stateTrade = state;
}

// shareddoc.h:589
void CSharedDoc::AddTradeIncome(__int64 income) {
	m_tradeIncome += income;
}

namespace _gateway {

// Defined earlier in this file (not decompiled yet).
void SetTradeResult(int result, int code, std::vector<sTradeItem>* pItemList);

// tradehandler.cpp:27
void UpdateBuyItem(const sTradeBuyBase& item) {
	switch (item.countType) {
	case 2:
	case 3:
		switch (item.info.m_typeId >> 26) {
		case 2:
			if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(item.info.m_typeId)) {
				WSingleton<CSharedDoc>::Instance()->m_myItemList.push_back(item.info);
				UccManager()->AddClothes(1, item.info.m_serial, item.info.m_typeId, item.info.m_uccKey, false, false);
			} else {
				WSingleton<CSharedDoc>::Instance()->AddMyPartsList(item.info);
			}
			break;
		case 4:
			WSingleton<CSharedDoc>::Instance()->m_myItemMap.insert(std::map<unsigned int, sItemInfo>::value_type(item.info.m_serial, item.info));
			break;
		}
		break;
	default:
		WSingleton<_systemmsg::CMessageManager>::Instance()->NotifyNormalMessage("Invalid Counting Type!");
		break;
	}
}

// tradehandler.cpp:66
void UpdateSellItem(const sTradeItem& item) {
	switch (item.typeId >> 26) {
	case 2:
		if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(item.typeId)) {

			for (std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myItemList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_myItemList.end(); ++it) {

				if (it->m_serial == item.guid && it->m_typeId == item.typeId && strcmpi(it->m_uccKey, item.uccIdx) == 0 && it->m_uccSeq == item.uccSeq) {

					WSingleton<CSharedDoc>::Instance()->m_myItemList.erase(it);
					if (IsLocalContent((localContentType_t)0x6F)) {

						WSingleton<CSharedDoc>::Instance()->DeleteCardToPartsAttach(item.guid);
					}
					return;
				}
			}
		} else {
			for (std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myPartsList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_myPartsList.end(); ++it) {

				if (it->m_serial == item.guid) {

					if (it->m_typeId == item.typeId) {

						WSingleton<CSharedDoc>::Instance()->m_myPartsList.erase(it);
						if (IsLocalContent((localContentType_t)0x6F)) {

							WSingleton<CSharedDoc>::Instance()->DeleteCardToPartsAttach(item.guid);
						}
					}
					return;
				}
			}
		}
		break;
	case 4:
		{
			std::map<unsigned int, sItemInfo>::iterator end = WSingleton<CSharedDoc>::Instance()->m_myItemMap.end();

			for (std::map<unsigned int, sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myItemMap.begin(); it != end; ++it) {

				if (it->second.m_serial == item.guid) {

					if (it->second.m_typeId == item.typeId) {

						WSingleton<CSharedDoc>::Instance()->m_myItemMap.erase(it);
					}
					return;
				}
			}
		}
		break;
	}
}

// tradehandler.cpp:133
void SetTradeResult(int result, int code, std::vector<sTradeItem>* pItemList) {
	if (result == 1) {
		if (code >= 0x1D6 && code <= 0x1D7) {
			FrTradeResultDlg* pDlg = CreateForm<FrTradeResultDlg>(g_pFresh->GetManager(), 0, "traderesultdlg", 0);
			if (!pDlg || !pItemList) {
				return;
			}

			pDlg->Open(0, 1);
			pDlg->SetFadeout(false);
			pDlg->SetResultItemList(pItemList);
		}
		return;
	}

	switch (result) {
	case 0x17: {
		FrTradeLevelLimitDlg* pDlg = CreateForm<FrTradeLevelLimitDlg>(g_pFresh->GetManager(), 0, "tradelevellimitdlg", 0);
		if (!pDlg) {
			return;
		}

		pDlg->Open(0, 0x40);
		return;
	}
	case 0xF:
		if (FrStaticCast<FrTradeWarningDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("tradewarningdlg")) == 0) {
			FrTradeWarningDlg* pDlg = CreateForm<FrTradeWarningDlg>(g_pFresh->GetManager(), 0, "tradewarningdlg", 0);
			if (pDlg) {
				pDlg->Open(0, 1);
				pDlg->SetWarningMessageType(0xF, 0);
				pDlg->SetFadeout(false);
			}
		}
		break;
	}

	if (FrStaticCast<FrTradeResultDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("traderesultdlg")) == 0) {
		if (FrStaticCast<FrTradeWarningDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("tradewarningdlg")) != 0) {
			return;
		}

		FrTradeWarningDlg* pDlg = CreateForm<FrTradeWarningDlg>(g_pFresh->GetManager(), 0, "tradewarningdlg", 0);
		if (!pDlg) {
			return;
		}

		pDlg->Open(0, 1);
		pDlg->SetWarningMessageForPackage(result);
		pDlg->SetFadeout(false);
	} else {
		switch (result) {
		case 3:
			// "The shop has closed. Please use it next time."
			WSingleton<CChatMsg>::Instance()->AddChatMsg(std::string("\xbb\xf3\xc1\xa1\xc0\xcc \xb4\xdd\xc7\xfb\xbd\xc0\xb4\xcf\xb4\xd9. \xb4\xd9\xc0\xbd \xb1\xe2\xc8\xb8\xbf\xa1 \xc0\xcc\xbf\xeb\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4."), 0xFFFF, false, false);
			break;
		case 4:
			// "The shop is being edited. Please use it after it opens."
			WSingleton<CChatMsg>::Instance()->AddChatMsg(std::string("\xbb\xf3\xc1\xa1\xc0\xcc \xc6\xed\xc1\xfd\xc1\xdf\xc0\xd4\xb4\xcf\xb4\xd9. \xb0\xb3\xbc\xb3 \xc8\xc4 \xc0\xcc\xbf\xeb\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4."), 0xFFFF, false, false);
			break;
		}
	}
}

// tradehandler.cpp:232
CTradeHandler::CTradeHandler(void) {
}

// tradehandler.cpp:237
CTradeHandler::~CTradeHandler(void) {
}

// tradehandler.cpp:242
bool CTradeHandler::Initialize(void) {
	AddFunctionType(0, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CTradeHandler::*)(void))&CTradeHandler::OnNotifyTradeErrorCode);
	AddFunctionType(1, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CTradeHandler::*)(void))&CTradeHandler::OnUpdateTradeBuyItem);
	AddFunctionType(2, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CTradeHandler::*)(void))&CTradeHandler::OnUpdateTradeSellItem);
	AddFunctionType(3, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CTradeHandler::*)(void))&CTradeHandler::OnUpdateTradeMinusItem);
	AddFunctionType(4, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CTradeHandler::OnSetEnablePackageSale);
	AddFunctionType(5, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CTradeHandler::OnSetDisablePackageSale);
	AddFunctionType(6, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CTradeHandler::*)(void))&CTradeHandler::OnGetPackageSale);

	return true;
}

// tradehandler.cpp:255
bool CTradeHandler::Execute(const MsgObject& msg) {
	SetFunctionType(msg.m_param1);
	// The message travels through Exec's int parameter (the original's
	// 32-bit pointer-in-int; not portable to 64-bit).
	return Exec(this, (intptr_t)&msg);
}

// tradehandler.cpp:262
// param is the MsgObject's address; m_param2 the result code (pointer in an
// int, as the original: not portable to 64-bit).
void CTradeHandler::OnNotifyTradeErrorCode(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;

	SetTradeResult(pMsg->m_param2, 0, 0);
}

// tradehandler.cpp:272
const sTradeItem sTransformHelper::operator()(const sTradeBuyBase& buyItem) {
	return buyItem.item;
}

// tradehandler.cpp:278
// param is the MsgObject's address; m_param2 the address of the result
// (pointers in ints, as the original: not portable to 64-bit).
void CTradeHandler::OnUpdateTradeBuyItem(intptr_t param) {
	sTradeBuyResult* pResult = (sTradeBuyResult*)((const MsgObject*)param)->m_param2;

	std::for_each(pResult->itemList.begin(), pResult->itemList.end(), UpdateBuyItem);

	WSingleton<CSharedDoc>::Instance()->m_myPang = pResult->pang;

	FrTradeDlg* pTradeDlg = FrStaticCast<FrTradeDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("tradedlg"));
	if (pTradeDlg) {
		pTradeDlg->EnableSendingServer(false);
	}

	std::vector<sTradeItem> tradeList;

	std::transform(pResult->itemList.begin(), pResult->itemList.end(), std::back_inserter(tradeList), sTransformHelper());

	SetTradeResult(pResult->result, 0x1D6, &tradeList);
}

// tradehandler.cpp:304
// param is the MsgObject's address; m_param2 the address of the result
// (pointers in ints, as the original: not portable to 64-bit).
void CTradeHandler::OnUpdateTradeSellItem(intptr_t param) {
	sTradeSellResult* pResult = (sTradeSellResult*)((const MsgObject*)param)->m_param2;

	std::for_each(pResult->itemList.begin(), pResult->itemList.end(), UpdateSellItem);

	WSingleton<CSharedDoc>::Instance()->m_myPang += pResult->pang;
	WSingleton<CSharedDoc>::Instance()->AddTradeIncome(pResult->pang);

	FrTradeDlg* pTradeDlg = FrStaticCast<FrTradeDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("tradedlg"));
	if (pTradeDlg) {
		pTradeDlg->EnableSendingServer(false);
	}

	// "%d items have all been sold."
	std::string msg = MakeStr("%d\xb0\xb3\xc0\xc7 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xcc \xb8\xf0\xb5\xce \xc6\xc7\xb8\xc5\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.", pResult->itemList.size());
	WSingleton<CChatMsg>::Instance()->AddChatMsg(msg, 0xFFFF, true, false);
}

// tradehandler.cpp:416
void CTradeHandler::OnSetEnablePackageSale(void) {
	CTradeDoc* pDoc = GetDocument<CTradeDoc*>();
	if (pDoc) {
		pDoc->m_bUnidentified = true;
	}
}

// tradehandler.cpp:425
void CTradeHandler::OnSetDisablePackageSale(void) {
	CTradeDoc* pDoc = GetDocument<CTradeDoc*>();
	if (pDoc) {
		pDoc->m_bUnidentified = false;
	}
}

// tradehandler.cpp:434
// param is the MsgObject's address; m_param2 the address of the bool result
// (pointers in ints, as the original: not portable to 64-bit).
void CTradeHandler::OnGetPackageSale(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;

	CTradeDoc* pDoc = GetDocument<CTradeDoc*>();
	if (pDoc) {
		*(bool*)pMsg->m_param2 = pDoc->m_bUnidentified;
	}
}

}
