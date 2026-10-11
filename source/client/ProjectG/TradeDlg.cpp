// Original: d:\build\custom\temp\code\client\projectg\tradedlg.cpp
#include "TradeDlg.h"
#include "SharedDoc.h"
#include "../shared/globalnetworkdefine.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frlistbox.h"
#include "frstatic.h"
#include "commonutil.h"
#include "binstr.h"
#include "TaskManager.h"
#include "actor.h"
#include "../shared/Localize.h"
#include "wfont.h"
#include "wresrcmng.h"
#include "fresh.h"
#include "frdesktop.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "UccManager.h"
#include "bitmap.h"
#include "wtemplate.h"
#include "ChatMsg.h"
#include "ItemStorage.h"

// Title-font file list of FrTradeResultDlg (no symbol).
extern "C" char* data_a9cd08[];
// Title-font file list of FrTradeBuyDlg (no symbol).
extern "C" char* data_a9ccf8[];

#include <stdlib.h>

// WTL::CString. The original's _AtlWinModule initializer pair (_$E1/_$E2)
// was folded by the linker into another unit's identical copy, so the
// unit's own _$E<n> are re-paired in symbol_mappings.json.
#include <windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>

#include <algorithm>

bool IsUccClothes(unsigned long typeId);
void SetTradeResultCode(int code, int msgId, sTradeItem* pItem);
void DrawPartsSlot(WPoint pt, sTradeItem* pItem);   // tradedlg.cpp:27

static const char* s_gatewayActorName = "Gateway";

// tradedlg.cpp:27
void DrawPartsSlot(WPoint pt, sTradeItem* pItem) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0) {
		return;
	}

	IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(pItem->typeId);
	if (pPart == 0) {
		return;
	}

	float charX = 5.0f;
	float caddieX = 5.0f;

	for (int slot = 1; slot < pPart->m_charCardSlots + 1; slot++) {
		std::list<sSCardAvilityPeriodInfo>::iterator it;
		bool bFound = false;
		for (it = WSingleton<CSharedDoc>::Instance()->m_cardPeriodList[0].begin(); it != WSingleton<CSharedDoc>::Instance()->m_cardPeriodList[0].end(); ++it) {
			if (pItem->typeId != it->partsTypeId || pItem->guid != it->partsId) {
				continue;
			}
			IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(it->typeId);
			if (pCard == 0) {
				continue;
			}
			if (slot != it->slot) {
				continue;
			}
			if (it->kind != 0) {
				continue;
			}
			bFound = true;
			const Bitmap* pBmp = g_pFresh->GetBitmap(pCard->m_texture);
			if (pBmp == 0) {
				continue;
			}
			WRect rect(pt.x + charX, pt.y + 54.0f, (float)pBmp->Width(), (float)pBmp->Height());
			pGDI->DrawTexture(pBmp, rect, 0xFFFFFFFF, 0);
			charX += 16.0f;
		}
		if (!bFound) {
			const Bitmap* pBmp = g_pFresh->GetBitmap("card_character_empty");
			if (pBmp) {
				WRect rect(pt.x + charX, pt.y + 54.0f, (float)pBmp->Width(), (float)pBmp->Height());
				pGDI->DrawTexture(pBmp, rect, 0xFFFFFFFF, 0);
				charX += 16.0f;
			}
		}
	}

	for (int slot = 1; slot < pPart->m_caddieCardSlots + 1; slot++) {
		std::list<sSCardAvilityPeriodInfo>::iterator it;
		bool bFound = false;
		for (it = WSingleton<CSharedDoc>::Instance()->m_cardPeriodList[0].begin(); it != WSingleton<CSharedDoc>::Instance()->m_cardPeriodList[0].end(); ++it) {
			if (pItem->typeId != it->partsTypeId || pItem->guid != it->partsId) {
				continue;
			}
			IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(it->typeId);
			if (pCard == 0) {
				continue;
			}
			if (slot != it->slot) {
				continue;
			}
			if (it->kind != 1) {
				continue;
			}
			bFound = true;
			const Bitmap* pBmp = g_pFresh->GetBitmap(MakeStr(pCard->m_texture));
			if (pBmp == 0) {
				continue;
			}
			WRect rect(pt.x + caddieX, pt.y + 70.0f, (float)pBmp->Width(), (float)pBmp->Height());
			pGDI->DrawTexture(pBmp, rect, 0xFFFFFFFF, 0);
			caddieX += 16.0f;
		}
		if (!bFound) {
			const Bitmap* pBmp = g_pFresh->GetBitmap("card_caddie_empty");
			if (pBmp) {
				WRect rect(pt.x + caddieX, pt.y + 70.0f, (float)pBmp->Width(), (float)pBmp->Height());
				pGDI->DrawTexture(pBmp, rect, 0xFFFFFFFF, 0);
				caddieX += 16.0f;
			}
		}
	}
}

// shareddoc.h:570
const char* CSharedDoc::GetTradeTitle(void) {
	return m_tradeTitle.c_str();
}

// shareddoc.h:572
int CSharedDoc::GetCountVisitor(void) {
	return m_countVisitor;
}

// shareddoc.h:590
__int64 CSharedDoc::GetTradeIncome(void) {
	return m_tradeIncome;
}

// tradedlg.h:13
sPartEquipInfo::sPartEquipInfo(void) {
	typeId = 0;
	guid = 0;
}

// tradedlg.h:140
eTypeSalesTrade FrTradeDlg::GetPackageSale(void) const {
	return m_packageSale;
}

// tradedlg.h:141
bool FrTradeDlg::IsPackageSale(void) {
	return m_packageSale == TYPE_SALES_PACKAGE ? true : false;
}

// tradedlg.h:222
void FrTradeBuyDlg::SetClosing(bool bClosing) {
	m_bClosing = bClosing;
}

// tradedlg.h:233
void FrTradeBuyDlg::SetPackageSale(void) {
	m_packageSale = TYPE_SALES_PACKAGE;
}

// tradedlg.h:234
eTypeSalesTrade FrTradeBuyDlg::GetPackageSale(void) {
	return m_packageSale;
}

// tradedlg.h:464
void FrTradeEditDlg::SetPackageSale(eTypeSalesTrade type) {
	m_packageSale = type;
}

// tradedlg.h:465
bool FrTradeEditDlg::IsPackageSale(void) {
	return m_packageSale == TYPE_SALES_PACKAGE ? true : false;
}

// tradedlg.h:544
void FrTradeFinishEditDlg::SetPackageSale(eTypeSalesTrade type) {
	m_packageSale = type;
}

// tradedlg.h:545
bool FrTradeFinishEditDlg::IsPackageSale(void) {
	return m_packageSale == TYPE_SALES_PACKAGE ? true : false;
}

// tradedlg.h: no line record (empty constructor of a nested helper class)
FrTradeDlg::MiniBtn::MiniBtn(void) {
}

// tradedlg.h: no line record (empty constructor of a nested helper class)
FrTradeEditDlg::MiniBtn::MiniBtn(void) {
}

// tradedlg.h: no line record (empty constructor of a nested helper class)
FrTradeFinishEditDlg::MiniBtn::MiniBtn(void) {
}

// tradedlg.cpp:235
__int64 SumForTradeItemList(__int64 sum, const sTradeItem& item) {
	return sum + item.price;
}

// tradedlg.cpp:240
bool CheckPackageSaleItem(const sTradeItem& item) {
	switch (item.typeId >> 26) {
	case 2:
		return IsUccClothes(item.typeId) ? true : false;
	case 4:
		return false;
	}

	return true;
}

// tradedlg.cpp:256
bool CannotPurchaseItem(const FrListItem* pListItem) {
	const sTradeItem* pItem = (const sTradeItem*)pListItem->pData;

	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pItem->typeId);

	return (pItem->statFlag & 1) ? true : false;
}

// tradedlg.cpp:273
void SetTextColorByPrice(__int64 price) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		if (price > 9999999) {
			pGDI->SetTextColor(0xFFF50018, 0xFFFFFFFF);
		} else if (price > 999999) {
			pGDI->SetTextColor(0xFFD5009B, 0xFFFFFFFF);
		} else if (price > 9999) {
			pGDI->SetTextColor(0xFFC79500, 0xFFFFFFFF);
		} else if (price > 999) {
			pGDI->SetTextColor(0xFF0E0E0E, 0xFFFFFFFF);
		} else {
			pGDI->SetTextColor(0xFF626262, 0xFFFFFFFF);
		}
	}
}

// tradedlg.cpp:325
const char* MakeStatusString(int type, int value, int slot) {
	const char* names[5] = {
		"\xC6\xC4\xBF\xF6",
		"\xC4\xC1\xC6\xAE\xB7\xD1",
		"\xC1\xA4\xC8\xAE\xB5\xB5",
		"\xBD\xBA\xC7\xC9",
		"\xC4\xBF\xBA\xEA"
	};

	if (value) {
		if (slot) {
			return MakeStr("%s+%d/%s+%d", names[type], value, "\xBD\xBD\xB7\xD4", slot);
		}
		else
			return MakeStr("%s+%d", names[type], value);
	}
	else if (slot) {
		return MakeStr("%s%s+%d", names[type], "\xBD\xBD\xB7\xD4", slot);
	}
	return "";
}

// tradedlg.cpp:346
const char* MakeCardStatusString(int type, int value) {
	const char* names[5] = {
		"\xC6\xC4\xBF\xF6\xBD\xBD\xB7\xD4",
		"\xC4\xC1\xC6\xAE\xB7\xD1\xBD\xBD\xB7\xD4",
		"\xC1\xA4\xC8\xAE\xB5\xB5\xBD\xBD\xB7\xD4",
		"\xBD\xBA\xC7\xC9\xBD\xBD\xB7\xD4",
		"\xC4\xBF\xBA\xEA\xBD\xBD\xB7\xD4"
	};

	if (value) {
		return MakeStr("%s+%d", names[type], value);
	}
	return "";
}

// tradedlg.cpp:361 (run-time type and factory registration of FrTradeDlg)
const WRTTI FrTradeDlg::m_RTTI("FrTradeDlg", &FrForm::m_RTTI);

IObject* FrTradeDlgMakeInstance(void) {
	return new FrTradeDlg;
}

struct __sFrTradeDlg {
	__sFrTradeDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTradeDlgMakeInstance, "FrTradeDlg");
	}
};
static __sFrTradeDlg s_registerFrTradeDlg;

// tradedlg.cpp:363
const sFRESH_MSGMAP* FrTradeDlg::GetMessageMap(void) const {
	return &FrTradeDlg::_MsgMap;
}

// tradedlg.cpp:513
void FrTradeDlg::OnTradeDlgInit(int) {
}

// tradedlg.cpp:518
void FrTradeDlg::OnVisitorBackInit(int param) {
	m_pVisitorBack = DYNAMIC_CAST(FrArea, param);
}

// tradedlg.cpp:524
void FrTradeDlg::OnVisitorBackOwnerDraw(void) {
}

// tradedlg.cpp:529
void FrTradeDlg::OnMyPangInit(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
}

// tradedlg.cpp:535
void FrTradeDlg::OnMyPangOwnerDraw(void) {
	DrawPang();
}

// tradedlg.cpp:541
void FrTradeDlg::OnTradeItemListInit(int param) {
	m_pTradeItemList = DYNAMIC_CAST(FrListBox, param);

	if (m_pTradeItemList) {
		m_pTradeItemList->UseRightButton(true);
	}
}

// tradedlg.cpp:962
void FrTradeDlg::OnNoItem01Init(int param) {
	m_pNoItem01 = DYNAMIC_CAST(FrStatic, param);

	if (m_pNoItem01) {
		m_pNoItem01->SetVisible(false);
	}
}

// tradedlg.cpp:973
void FrTradeDlg::OnNoItem02Init(int param) {
	m_pNoItem02 = DYNAMIC_CAST(FrStatic, param);

	if (m_pNoItem02) {
		m_pNoItem02->SetVisible(false);
	}
}

// tradedlg.cpp:984
void FrTradeDlg::OnVisitorInit(int param) {
	m_pCountVisitor = DYNAMIC_CAST(FrEdit, param);
	UpdateCountVisitor();
}

// tradedlg.cpp:992
void FrTradeDlg::OnUserIdInit(int param) {
	m_pUserId = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:998
void FrTradeDlg::OnTitleInit(intptr_t param) {
	m_pTitleEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pTitleEdit) { m_pTitleEdit->ClearLine(); m_pTitleEdit->SetLine(1, WSingleton<CSharedDoc>::Instance()->GetTradeTitle(), 0, false, 0); }
}

// tradedlg.cpp:1006
void FrTradeDlg::OnEditBnInit(int param) {
	m_pEditBn = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:1012
void FrTradeDlg::OnEditBnUp(void) {
	IActor* pActor = AfxGetTask()->GetActor(s_gatewayActorName);
	if (pActor) {
		pActor << MsgObject(0, 0x273, m_packageSale == TYPE_SALES_PACKAGE ? 4 : 5, 0, 0, 0, 0);
	}

	FrTradeEditDlg* pDlg = CreateForm<FrTradeEditDlg>(g_pFresh->GetManager(), this, "tradeeditdlg", 0);

	if (pDlg) {
		pDlg->m_packageSale = m_packageSale;

		pDlg->Open((FRESH_RESULT_CALLBACK)0, 0xC0);
		pDlg->SetFadeout(false);

		WSingleton<CSharedDoc>::Instance()->SendTradeEditShop();

		OnFreshOkay();
	}
}

// tradedlg.cpp:1041
void FrTradeDlg::OnCloseBnInit(intptr_t param) {
	m_pCloseBn = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:1047
void FrTradeDlg::OnCloseBnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->GetStateTrade() == STATE_TRADE_OPEN) {
		SetTradeResultCode(0x13, 225, 0);
	} else {
		SetTradeResultCode(0x12, 225, 0);
	}
}

// tradedlg.cpp:1060
void FrTradeDlg::OnHideBnInit(int param) {
	m_pHideBn = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:1066
void FrTradeDlg::OnHideBnUp(void) {
	IActor* pActor = AfxGetTask()->GetActor(s_gatewayActorName);
	if (pActor) {
		pActor << MsgObject(0, 0x273, m_packageSale == TYPE_SALES_PACKAGE ? 4 : 5, 0, 0, 0, 0);
	}

	OnFreshOkay();
}

// tradedlg.cpp:1080
void FrTradeDlg::OnExitBnInit(int param) {
	m_pExitBn = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:1086
void FrTradeDlg::OnExitBnUp(void) {
	IActor* pActor = AfxGetTask()->GetActor(s_gatewayActorName);
	if (pActor) {
		pActor << MsgObject(0, 0x273, 5, 0, 0, 0, 0);
	}

	OnFreshOkay();
}

// tradedlg.cpp:1099
void FrTradeDlg::OnPackagePurchaseButtonInit(int param) {
	m_pPackagePurchaseButton = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:1150
bool FrTradeDlg::OnInit(void) {
	EnableDrag(false);

	switch (WSingleton<CSharedDoc>::Instance()->GetStateTrade()) {
	case 1:
	case STATE_TRADE_OPEN:
	case STATE_TRADE_EDIT:
		if (WSingleton<CSharedDoc>::Instance()->GetListTradeItem()->size() <= 0) {
			if (m_pNoItem01) m_pNoItem01->SetVisible(true); if (m_pNoItem02) m_pNoItem02->SetVisible(true);
			if (m_pHideBn) m_pHideBn->Enable(false);
		} else {
			if (m_pNoItem01) m_pNoItem01->SetVisible(false); if (m_pNoItem02) m_pNoItem02->SetVisible(false);
			if (m_pHideBn) m_pHideBn->Enable(true);
		}
		break;
	}

	return true;
}

// tradedlg.cpp:1185
void FrTradeDlg::OpenTradeShop(unsigned long uid) {
	if (uid == 0xFFFFFFFF) {
		OnFreshOkay();
		return;
	}

	m_shopOwnerUID = uid;

	if (uid == WSingleton<CSharedDoc>::Instance()->m_myUID) {
		m_bMyShop = true;
		SetListTradeItem(false);

		if (m_pVisitorBack) m_pVisitorBack->SetVisible(true);
		if (m_pCountVisitor) m_pCountVisitor->SetVisible(true);
		if (m_pEditBn) m_pEditBn->SetVisible(true);
		if (m_pCloseBn) m_pCloseBn->SetVisible(true);
		if (m_pHideBn) m_pHideBn->SetVisible(true);
		if (m_pExitBn) m_pExitBn->SetVisible(false);
		if (m_pPackagePurchaseButton) m_pPackagePurchaseButton->SetVisible(false);
	}
	else {
		m_bMyShop = false;
		if (m_pVisitorBack) m_pVisitorBack->SetVisible(false);
		if (m_pCountVisitor) m_pCountVisitor->SetVisible(false);
		if (m_pEditBn) m_pEditBn->SetVisible(false);
		if (m_pCloseBn) m_pCloseBn->SetVisible(false);
		if (m_pHideBn) m_pHideBn->SetVisible(false);
		if (m_pExitBn) m_pExitBn->SetVisible(true);
	}

	if (m_pUserId) {
		char szName[64] = "";
		sprintf(szName, "\xB3\xAA\xC0\xC7 \xBB\xF3\xC1\xA1");

		if (!m_bMyShop) {
			std::list<sRoomPlayer>::iterator it;
			for (it = WSingleton<CSharedDoc>::Instance()->m_roomPlayerList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_roomPlayerList.end(); ++it) {
				if (it->m_ruleUID == WSingleton<CSharedDoc>::Instance()->GetTradeUID()) {
					sprintf(szName, MakeStr("%s \xB4\xD4\xC0\xC7 \xBB\xF3\xC1\xA1", it->m_nickname));
					break;
				}
			}
		}

		m_pUserId->SetLine(1, szName, 0, false, 0);
	}

	if (!m_bMyShop) {
		m_pNoItem01->SetCaption("\xB8\xF0\xB5\xE7 \xBB\xF3\xC7\xB0\xC0\xCC \xC7\xB0\xC0\xFD\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.");
		m_pNoItem02->SetCaption("\xC0\xCC\xBF\xEB\xC7\xD8 \xC1\xD6\xBC\xC5\xBC\xAD \xB0\xA8\xBB\xE7\xC7\xD5\xB4\xCF\xB4\xD9.");
	}
}

// tradedlg.cpp:1263
template <class T>
struct _TradeItemSort {
	bool operator()(const T& a, const T& b) const {
		if ((a.typeId >> 26) == 2 && (b.typeId >> 26) != 2) {
			return false;
		}

		if ((a.typeId >> 26) == 2 && (b.typeId >> 26) == 2) {
			int aIndex = a.typeId & 0x3FFFFFF;
			int bIndex = b.typeId & 0x3FFFFFF;

			if (aIndex < bIndex) {
				return true;
			}
			if (aIndex > bIndex) {
				return false;
			}
		}

		if (a.typeId < b.typeId) {
			return true;
		}
		if (a.typeId > b.typeId) {
			return false;
		}

		return a.guid < b.guid;
	}
};

// tradedlg.cpp:1290
void FrTradeDlg::SetListTradeItem(bool bClear) {
	if (m_pTradeItemList == 0)
		return;

	m_totalPrice = 0;

	m_pTradeItemList->ClearItem();

	if (bClear)
		return;

	std::list<sTradeItem>* pList = WSingleton<CSharedDoc>::Instance()->GetListTradeItem();
	for (std::list<sTradeItem>::iterator it = pList->begin(); it != pList->end(); ++it) {
		IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(it->typeId);

		if (pCommon) {
			it->statFlag = 0;

			if ((pCommon->m_level & 0x7F) > WSingleton<CSharedDoc>::Instance()->m_myLevel)
				it->statFlag |= 1;

			m_pTradeItemList->AddItem(&*it);

			m_totalPrice += it->price;
		}
	}
}

// tradedlg.cpp:1330
void FrTradeDlg::DrawPang(void) {
	if (!g_pFresh->GetManager()->GetGDI())
		return;
	if (!m_pPangFont)
		return;

	char buf[32] = ""; WPoint pos(m_rect.x, m_rect.y);
	static int s_fontWidth = m_pPangFont->GetFontInfo()->fontw;
	static float s_textWidth;

	if (m_bMyShop) {
		MakePriceStrA(buf, WSingleton<CSharedDoc>::Instance()->GetTradeIncome());
	} else if (GetPackageSale() == TYPE_SALES_SINGLE) {
		MakePriceStrA(buf, WSingleton<CSharedDoc>::Instance()->m_myPang);
	} else {
		MakePriceStrA(buf, m_totalPrice);
	}

	s_textWidth = (s_fontWidth - 1.0f) * (strlen(buf) - 1);
	m_pPangFont->Print(g_view, pos.x + 130.0f - s_textWidth, pos.y + 380.0f, buf, 0, 0xffffffff, 0);
}

// tradedlg.cpp:1368
void FrTradeDlg::UpdateTradeTitle(void) {
	if (m_pTitleEdit) {
		m_pTitleEdit->ClearLine();
		m_pTitleEdit->SetLine(1, WSingleton<CSharedDoc>::Instance()->GetTradeTitle(), 0, false, 0);
	}
}

// tradedlg.cpp:1378
void FrTradeDlg::UpdateCountVisitor(void) {
	if (m_pCountVisitor) {
		char buf[32] = { 0 };
		MakePriceStrA(buf, WSingleton<CSharedDoc>::Instance()->GetCountVisitor());
		m_pCountVisitor->SetLine(1, buf, 0, false, 0);
	}
}

// tradedlg.cpp:1390
bool FrTradeDlg::OnTradeBuyDlgResult(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		EnableSendingServer(true);
	}

	return true;
}

// tradedlg.cpp:1401
void FrTradeDlg::EnableSendingServer(bool bSending) {
	m_bSendingServer = bSending;

	if (m_pExitBn) {
		m_pExitBn->Enable(!bSending);
	}
}

// tradedlg.cpp:1412
void FrTradeDlg::ShowStaticOutOfStock(bool bShow) {
	if (m_pNoItem01) {
		m_pNoItem01->SetVisible(bShow);
	}
	if (m_pNoItem02) {
		m_pNoItem02->SetVisible(bShow);
	}
}

// tradedlg.cpp:1421
void FrTradeDlg::EnableHideBn(bool bEnable) {
	if (m_pHideBn) {
		m_pHideBn->Enable(bEnable);
	}
}

// tradedlg.cpp:1430
void FrTradeDlg::SetPackageSale(eTypeSalesTrade type) {
	m_packageSale = type;
	if (type != TYPE_SALES_PACKAGE && m_pPackagePurchaseButton) {
		m_pPackagePurchaseButton->SetVisible(false);
		m_pPackagePurchaseButton->Enable(false);
	}
}

// tradedlg.cpp:1441 (run-time type and factory registration of FrTradeBuyDlg)
const WRTTI FrTradeBuyDlg::m_RTTI("FrTradeBuyDlg", &FrForm::m_RTTI);

IObject* FrTradeBuyDlgMakeInstance(void) {
	return new FrTradeBuyDlg;
}

struct __sFrTradeBuyDlg {
	__sFrTradeBuyDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTradeBuyDlgMakeInstance, "FrTradeBuyDlg");
	}
};
static __sFrTradeBuyDlg s_registerFrTradeBuyDlg;

// tradedlg.cpp:1443
const sFRESH_MSGMAP* FrTradeBuyDlg::GetMessageMap(void) const {
	return &FrTradeBuyDlg::_MsgMap;
}

// tradedlg.cpp:1470
FrTradeBuyDlg::FrTradeBuyDlg(void) {
	m_pBuyItemIcon = 0;
	m_pBuyItemName = 0;
	m_pBuyItemAbility = 0;
	m_pMyPang = 0;
	m_pPrice = 0;
	m_pPangLeft = 0;
	m_pNumberNextBn = 0;
	m_pNumberPrevBn = 0;
	m_pNumber = 0;
	memset(&m_buyOrder, 0, sizeof(m_buyOrder));

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	m_pBuyItem = 0;
	m_bClosing = false;
	m_pPurchaseList = 0;
	m_packageSale = (eTypeSalesTrade)0;
	info.filename = data_a9ccf8;
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:";
	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);

	m_pBmpUccOriginal = g_pFresh->RegisterBitmap("ucc_state_original");
	m_pBmpUccCopied = g_pFresh->RegisterBitmap("ucc_state_copied");
	m_pBmpUccTempSaved = g_pFresh->RegisterBitmap("ucc_state_temp_saved");

	m_lastNumber = 0;
}

// tradedlg.cpp:1515
FrTradeBuyDlg::~FrTradeBuyDlg(void) {
	if (m_pTitleFont) {
		m_pTitleFont->Erase();
	}

	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// tradedlg.cpp:1526
void FrTradeBuyDlg::OnTradeBuyDlgInit(int) {
}

// tradedlg.cpp:1531
void FrTradeBuyDlg::OnOkBnInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:1650
bool CheckTradeItem(unsigned long typeId, const sTradeItem& item) {
	if (WSingleton<CSharedDoc>::Instance()->GetStateTrade() == STATE_TRADE_EDIT) {
		if (typeId == 0 || typeId == 0xFFFFFFFF)
			return false;
		if (item.guid == 0)
			return false;
		if (item.typeId == 0)
			return false;
		if (item.price == 0)
			return false;
		return item.count > 0;
	}

	return false;
}

// tradedlg.cpp:1738
void FrTradeBuyDlg::OnBuyItemIconInit(int param) {
	m_pBuyItemIcon = DYNAMIC_CAST(FrArea, param);
}

// tradedlg.cpp:1853
void FrTradeBuyDlg::OnBuyItemNameInit(int param) {
	m_pBuyItemName = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:1859
void FrTradeBuyDlg::OnBuyItemAbilityInit(int param) {
	m_pBuyItemAbility = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:1865
void FrTradeBuyDlg::OnMyPangInit(intptr_t param) {
	m_pMyPang = DYNAMIC_CAST(FrEdit, param);

	if (m_pMyPang) {
		char buf[32] = { 0 };
		MakePriceStrA(buf, WSingleton<CSharedDoc>::Instance()->m_myPang);

		m_pMyPang->SetLine(1, buf, 0, false, 0);
	}
}

// tradedlg.cpp:1880
void FrTradeBuyDlg::OnPriceInit(intptr_t param) {
	m_pPrice = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:1886
void FrTradeBuyDlg::OnPangLeftInit(int param) {
	m_pPangLeft = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:1892
void FrTradeBuyDlg::OnNumberInit(int param) {
	m_pNumber = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:1898
void FrTradeBuyDlg::OnNumberNextBnInit(int param) {
	m_pNumberNextBn = DYNAMIC_CAST(FrButton, param);
	if (m_pNumberNextBn) {
		m_pNumberNextBn->SetPushDelay(0.0f);
	}
}

// tradedlg.cpp:1909
void FrTradeBuyDlg::OnNumberNextBnUp(void) {
	if (m_pNumberNextBn == 0) return;
	if (m_pNumber == 0) return;
	if (m_pBuyItem == 0) return;

	int number = atoi(m_pNumber->GetLine(1, false));

	if (++number >= (int)m_pBuyItem->count) {
		number = m_pBuyItem->count;
		m_pNumberNextBn->Enable(false);
	}

	m_pNumberPrevBn->Enable(true);
	m_pNumber->ClearLine();
	m_pNumber->SetLine(1, MakeStr("%d", number), 0, false, 0);

	UpdatePrice();
}

// tradedlg.cpp:1933
void FrTradeBuyDlg::OnNumberPrevBnInit(int param) {
	m_pNumberPrevBn = DYNAMIC_CAST(FrButton, param);
	if (m_pNumberPrevBn) {
		m_pNumberPrevBn->SetPushDelay(0.0f);
	}
}

// tradedlg.cpp:1944
void FrTradeBuyDlg::OnNumberPrevBnUp(void) {
	if (m_pNumberPrevBn == 0) return;
	if (m_pNumber == 0) return;
	if (m_pBuyItem == 0) return;

	int number = atoi(m_pNumber->GetLine(1, false));

	--number;

	if (number <= 1) {
		number = 1;
		m_pNumberPrevBn->Enable(false);
	}

	m_pNumberNextBn->Enable(true);
	m_pNumber->ClearLine();
	m_pNumber->SetLine(1, MakeStr("%d", number), 0, false, 0);

	UpdatePrice();
}

// tradedlg.cpp:1968
void FrTradeBuyDlg::OnPurchaseListInit(int param) {
	m_pPurchaseList = DYNAMIC_CAST(FrListBox, param);
}

// tradedlg.cpp:2112
bool FrTradeBuyDlg::OnInit(void) {
	EnableDrag(false);

	return true;
}

// tradedlg.cpp:2120
void FrTradeBuyDlg::OnProc(const float dt) {
	if (m_pBuyItem && m_pNumberNextBn && m_pNumberPrevBn) {
		int number = atoi(m_pNumber->GetLine(1, false));

		if (number <= 1) {
			m_pNumberNextBn->Enable(true);
			m_pNumberPrevBn->Enable(false);
			m_pNumber->ClearLine();
			m_pNumber->SetLine(1, MakeStr("%d", 1), 0, false, 0);
		} else {
			int count = m_pBuyItem->count;
			if (number >= count) {
				m_pNumberNextBn->Enable(false);
				m_pNumberPrevBn->Enable(true);
				m_pNumber->ClearLine();
				m_pNumber->SetLine(1, MakeStr("%d", count), 0, false, 0);
			} else {
				m_pNumberNextBn->Enable(true);
				m_pNumberPrevBn->Enable(true);
			}
		}

		UpdatePrice();
	}
}

// tradedlg.cpp:2269
void FrTradeBuyDlg::SetBuyItemList(void) {
	std::list<sTradeItem>* pList = WSingleton<CSharedDoc>::Instance()->GetListTradeItem();
	for (std::list<sTradeItem>::iterator it = pList->begin(); it != pList->end(); ++it) {
		m_pPurchaseList->AddItem(&*it);
	}
}

// tradedlg.cpp:2279
void FrTradeBuyDlg::SetControlsItemList(void) {
	m_pNumber->SetLine(1, "1", 0, false, 0);

	UpdatePriceList();

	m_pNumberNextBn->Enable(false);
	m_pNumberPrevBn->Enable(false);
	m_pNumber->Enable(false);
}

// tradedlg.cpp:2290
void FrTradeBuyDlg::UpdatePriceList(void) {
	char priceStr[32] = { 0 };
	char pangLeftStr[32] = { 0 };

	std::list<sTradeItem>* pList = WSingleton<CSharedDoc>::Instance()->GetListTradeItem();
	// std::accumulate in the original, most likely (<numeric> is missing from externs/).
	__int64 total = 0;
	for (std::list<sTradeItem>::iterator it = pList->begin(); it != pList->end(); ++it) total = SumForTradeItemList(total, *it);

	MakePriceStrA(priceStr, total);
	m_pPrice->SetLine(1, priceStr, 0, false, 0);

	MakePriceStrA(pangLeftStr, WSingleton<CSharedDoc>::Instance()->m_myPang - total);
	m_pPangLeft->SetLine(1, pangLeftStr, 0, false, 0);
}

// tradedlg.cpp:2309
void FrTradeBuyDlg::SetControlsBuyItem(sTradeItem* pItem) {
	if (m_pPrice == 0) return;
	if (m_pPangLeft == 0) return;
	if (m_pNumberNextBn == 0) return;
	if (m_pNumberPrevBn == 0) return;
	if (m_pNumber == 0) return;

	m_pPrice->ClearLine();
	m_pPangLeft->ClearLine();
	m_pNumber->ClearLine();

	if (pItem == 0) {
		m_pNumberNextBn->Enable(false);
		m_pNumberPrevBn->Enable(false);
		m_pNumber->Enable(false);
		return;
	}

	m_pNumber->SetLine(1, "1", 0, false, 0);

	UpdatePrice();

	if (pItem->count == 1) {
		m_pNumberNextBn->Enable(false);
		m_pNumberPrevBn->Enable(false);
		m_pNumber->Enable(false);
	} else {
		m_pNumberNextBn->Enable(true);
		m_pNumberPrevBn->Enable(true);
		m_pNumber->Enable(true);
	}
}

// tradedlg.cpp:2351
void FrTradeBuyDlg::UpdatePrice(void) {
	if (m_pPrice == 0) return;
	if (m_pNumber == 0) return;
	if (m_pBuyItem == 0) return;

	char priceStr[32] = { 0 };
	char pangLeftStr[32] = { 0 };
	int number = atoi(m_pNumber->GetLine(1, false));

	if (m_lastNumber != number) {
		MakePriceStrA(priceStr, m_pBuyItem->price * number);
		m_pPrice->SetLine(1, priceStr, 0, false, 0);

		MakePriceStrA(pangLeftStr, WSingleton<CSharedDoc>::Instance()->m_myPang - m_pBuyItem->price * number);
		m_pPangLeft->SetLine(1, pangLeftStr, 0, false, 0);

		m_lastNumber = number;
	}
}

// tradedlg.cpp:2377 (run-time type and factory registration of FrTradeResultDlg)
const WRTTI FrTradeResultDlg::m_RTTI("FrTradeResultDlg", &FrForm::m_RTTI);

IObject* FrTradeResultDlgMakeInstance(void) {
	return new FrTradeResultDlg;
}

struct __sFrTradeResultDlg {
	__sFrTradeResultDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTradeResultDlgMakeInstance, "FrTradeResultDlg");
	}
};
static __sFrTradeResultDlg s_registerFrTradeResultDlg;

// tradedlg.cpp:2379
const sFRESH_MSGMAP* FrTradeResultDlg::GetMessageMap(void) const {
	return &FrTradeResultDlg::_MsgMap;
}

// tradedlg.cpp:2396
FrTradeResultDlg::FrTradeResultDlg(void) {
	m_pResultItemIcon = 0;
	m_pResultItemName = 0;
	m_pResultItemAbility = 0;
	memset(&m_resultItem, 0, sizeof(m_resultItem));

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = data_a9cd08;
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:";
	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);

	m_pBmpUccOriginal = g_pFresh->RegisterBitmap("ucc_state_original");
	m_pBmpUccCopied = g_pFresh->RegisterBitmap("ucc_state_copied");
	m_pBmpUccTempSaved = g_pFresh->RegisterBitmap("ucc_state_temp_saved");

	m_pPurchaseList = 0;
}

// tradedlg.cpp:2430
FrTradeResultDlg::~FrTradeResultDlg(void) {
	if (m_pTitleFont) {
		m_pTitleFont->Erase();
	}

	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// tradedlg.cpp:2441
void FrTradeResultDlg::OnTradeResultDlgInit(int) {
}

// tradedlg.cpp:2446
void FrTradeResultDlg::OnResultItemIconInit(intptr_t param) {
	m_pResultItemIcon = DYNAMIC_CAST(FrArea, param);
}

// tradedlg.cpp:2552
void FrTradeResultDlg::OnResultItemNameInit(intptr_t param) {
	m_pResultItemName = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:2558
void FrTradeResultDlg::OnResultItemAbilityInit(intptr_t param) {
	m_pResultItemAbility = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:2564
void FrTradeResultDlg::OnPurchaseListInit(int param) {
	m_pPurchaseList = DYNAMIC_CAST(FrListBox, param);
}

// tradedlg.cpp:2704
bool FrTradeResultDlg::OnInit(void) {
	EnableDrag(false);

	return true;
}

// Fills a list box with the items of a container (used with std::for_each).
class CListAssignHelper {
public:
	CListAssignHelper(FrListBox* pList);

	template <class T>
	void operator()(T& item);

private:
	FrListBox* m_pList;   // 0x0
};

// tradedlg.cpp:2716
CListAssignHelper::CListAssignHelper(FrListBox* pList)
	: m_pList(pList) {
}

// tradedlg.cpp:2722
template <class T>
void CListAssignHelper::operator()(T& item) {
	m_pList->AddItem(&item);
}

// tradedlg.cpp:2731
void FrTradeResultDlg::SetResultItemList(std::vector<sTradeItem>* pList) {
	m_resultItemList.swap(*pList);

	std::for_each(m_resultItemList.begin(), m_resultItemList.end(), CListAssignHelper(m_pPurchaseList));
}

// tradedlg.cpp:2870 (run-time type and factory registration of FrTradeEditDlg)
const WRTTI FrTradeEditDlg::m_RTTI("FrTradeEditDlg", &FrForm::m_RTTI);

IObject* FrTradeEditDlgMakeInstance(void) {
	return new FrTradeEditDlg;
}

struct __sFrTradeEditDlg {
	__sFrTradeEditDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTradeEditDlgMakeInstance, "FrTradeEditDlg");
	}
};
static __sFrTradeEditDlg s_registerFrTradeEditDlg;

// tradedlg.cpp:2872
const sFRESH_MSGMAP* FrTradeEditDlg::GetMessageMap(void) const {
	return &FrTradeEditDlg::_MsgMap;
}

// tradedlg.cpp:3012
FrTradeEditDlg::~FrTradeEditDlg(void) {
	if (m_pNumberFont) {
		m_pNumberFont->Erase();
	}

	if (m_pTitleFont) {
		m_pTitleFont->Erase();
	}

	if (g_resrcmng && m_pNumberFont) { g_resrcmng->Release(m_pNumberFont); m_pNumberFont = 0; }
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// tradedlg.cpp:3029
void FrTradeEditDlg::OnTradeEditDlgInit(int) {
}

// tradedlg.cpp:3034
void FrTradeEditDlg::OnMyItemListInit(intptr_t param) {
	m_pMyItemList = DYNAMIC_CAST(FrListBox, param);

	if (m_pMyItemList) {
		m_pMyItemList->UseRightButton(true);
	}
}

// tradedlg.cpp:3045
void FrTradeEditDlg::OnMyItemListLClick(void) {
	if (m_tradeItemList.size() < 6) {
		FrListItem* pListItem = m_pMyItemList->GetItemUnderCursor();

		if (pListItem) {
			if (!pListItem->selected) {
				m_pMyItemList->ToggleItem(pListItem);
			} else {
				sTradeItem* pItem = (sTradeItem*)pListItem->pData;
				SetTradeMyItem(pItem);
				SetControlsTradeMyItem(pItem, false);
			}
		}
	}
}

// tradedlg.cpp:3164
void FrTradeEditDlg::OnMyItemListOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0) {
		return;
	}

	FrListItem* pListItem = (FrListItem*)param;
	if (pListItem == 0) {
		return;
	}

	sTradeItem* pItem = (sTradeItem*)pListItem->pData;
	const Bitmap* pBaseBmp = 0;

	if (pListItem->selected) {
		pBaseBmp = m_pMyItemBaseClickBmp;
		pGDI->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
	} else if (pListItem->underCursor) {
		pBaseBmp = m_pMyItemBaseOverBmp;
		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	} else {
		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	}

	if (pBaseBmp != 0) {
		WRect rect(pListItem->pos.x + 2.0f, pListItem->pos.y, (float)pBaseBmp->Width(), (float)pBaseBmp->Height());

		pGDI->DrawTexture(pBaseBmp, rect, 0xFFFFFFFF, 0);
	}

	if ((pItem->typeId & 0xFC000000) == 0x08000000) {
		IFF_STRUCT::sChar* pChar = ItemManager()->FindChar(((pItem->typeId >> 18) & 0xFF) | 0x04000000);
		if (pChar != 0) {
			const Bitmap* pCharBmp = g_pFresh->GetBitmap(MakeStr("s_%s", pChar->m_icon));
			if (pCharBmp != 0) {
				WRect rect(pListItem->pos.x + 7.0f, pListItem->pos.y + 5.0f, (float)pCharBmp->Width(), (float)pCharBmp->Height());
				pGDI->DrawTexture(pCharBmp, rect, 0xFFFFFFFF, 0);
			}
		}
	}

	WPoint pt(pListItem->pos.x + 13.0f, pListItem->pos.y + 20.0f);
	DrawItem(pt, pItem, 0);
	if (IsLocalContent((localContentType_t)0x51)) {
		if ((pItem->typeId & 0xFC000000) == 0x08000000) {
			sTradeItem* pData = (sTradeItem*)pListItem->pData;
			DrawPartsSlot(pt, pData);
		}
	}
}

// tradedlg.cpp:3234
void FrTradeEditDlg::OnTradeItemListInit(int param) {
	m_pTradeItemList = DYNAMIC_CAST(FrListBox, param);
}

// tradedlg.cpp:3342
void FrTradeEditDlg::OnVisitorInit(int param) {
	m_pVisitor = DYNAMIC_CAST(FrEdit, param);

	if (m_pVisitor) {
		char buf[32] = { 0 };
		MakePriceStrA(buf, WSingleton<CSharedDoc>::Instance()->GetCountVisitor());
		m_pVisitor->SetLine(1, buf, 0, false, 0);
	}
}

// tradedlg.cpp:3357
void FrTradeEditDlg::OnUserIdInit(int param) {
	m_pUserId = DYNAMIC_CAST(FrEdit, param);

	if (m_pUserId) {
		m_pUserId->SetLine(1, "\xB3\xAA\xC0\xC7 \xBB\xF3\xC1\xA1", 0, false, 0);
	}
}

// tradedlg.cpp:3369
void FrTradeEditDlg::OnTitleInit(intptr_t param) {
	m_pTitle = DYNAMIC_CAST(FrEdit, param);
	if (m_pTitle) {
		m_pTitle->ClearLine();
		m_pTitle->SetLine(1, WSingleton<CSharedDoc>::Instance()->GetTradeTitle(), 0, false, 0);
	}
}

// tradedlg.cpp:3377
void FrTradeEditDlg::OnTitleEditInit(int param) {
	m_pTitleEdit = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:3383
bool FrTradeEditDlg::OnTitleEditEnterKey(int) {
	eResultTrade ret = SendTradeTitle();
	if (ret != RESULT_TRADE_OK) {
		SetTradeResultCode(ret, 225, 0);
	}

	return true;
}

// tradedlg.cpp:3395
void FrTradeEditDlg::OnTitleEditBnInit(int param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:3401
void FrTradeEditDlg::OnTitleEditBnUp(void) {
	eResultTrade ret = SendTradeTitle();
	if (ret != RESULT_TRADE_OK) {
		SetTradeResultCode(ret, 225, 0);
	}
}

// tradedlg.cpp:3411
void FrTradeEditDlg::OnTradeMyItemIconInit(int param) {
	m_pTradeMyItemIcon = DYNAMIC_CAST(FrArea, param);
}

// tradedlg.cpp:3417
void FrTradeEditDlg::OnTradeMyItemIconOwnerDraw(void) {
	if (m_pTradeMyItem) {
		WPoint pt(m_pTradeMyItemIcon->GetRect().x, m_pTradeMyItemIcon->GetRect().y);

		DrawItem(pt, m_pTradeMyItem, 0);
		if (IsLocalContent((localContentType_t)0x51)) {
			if ((m_pTradeMyItem->typeId & 0xFC000000) == 0x08000000) {
				DrawPartsSlot(pt, m_pTradeMyItem);
			}
		}
	}

	DrawPang();
}

// tradedlg.cpp:3438
void FrTradeEditDlg::OnTradeMyItemNameInit(int param) {
	m_pTradeMyItemName = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:3444
void FrTradeEditDlg::OnTradeMyItemAbilityInit(int param) {
	m_pTradeMyItemAbility = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:3450
void FrTradeEditDlg::OnTradeMyItemNumberInit(int param) {
	m_pTradeMyItemNumber = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:3456
void FrTradeEditDlg::OnTradeMyItemPriceInit(int param) {
	m_pTradeMyItemPrice = DYNAMIC_CAST(FrEdit, param);
	if (m_pTradeMyItemPrice) {
		m_pTradeMyItemPrice->SetCharLimit(8, false);
	}
}

// tradedlg.cpp:3468
void FrTradeEditDlg::OnTradeMyItemPriceClick(void) {
	if (m_pTradeMyItemPrice) {
		m_pTradeMyItemPrice->SetKeyFocus(true);
	}
}

// tradedlg.cpp:3475
void FrTradeEditDlg::OnTradeMyItemOkBnInit(int param) {
	m_pTradeMyItemOkBn = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:3619
void FrTradeEditDlg::OnTradeMyItemCancelBnInit(int param) {
	m_pTradeMyItemCancelBn = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:3625
void FrTradeEditDlg::OnTradeMyItemCancelBnUp(void) {
	SetTradeMyItem(0);
	if (m_pTradeMyItemNumber) {
		m_pTradeMyItemNumber->ClearLine();
	}
	if (m_pTradeMyItemPrice) {
		m_pTradeMyItemPrice->ClearLine();
	}
	m_pTradeMyItemNumber->Enable(false);
	m_pTradeMyItemPrice->Enable(false);
	m_pTradeMyItemOkBn->Enable(false);
	m_pTradeMyItemCancelBn->Enable(false);
}

// tradedlg.cpp:3632
void FrTradeEditDlg::OnOpenBnInit(int param) {
	m_pOpenBn = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:3638
void FrTradeEditDlg::OnOpenBnUp(void) {
	int ret = CheckTradeEdit();

	if (ret == 1) {
		FrTradeFinishEditDlg* pDlg = CreateForm<FrTradeFinishEditDlg>(g_pFresh->GetManager(), this, "tradefinisheditdlg", 0);
		if (pDlg) {
			pDlg->Open((FRESH_RESULT_CALLBACK)0, 0xC0);
			pDlg->SetFadeout(false);
			pDlg->SetListTradeItem(&m_tradeItemList);
			if (pDlg->m_pUserId) pDlg->m_pUserId->SetLine(1, "\xB3\xAA\xC0\xC7 \xBB\xF3\xC1\xA1", 0, false, 0);
			pDlg->SetTradeTitle(m_tradeTitle);

			pDlg->m_packageSale = m_pPackageSaleBtn->GetStatus() == FrButton::PRESSED ? TYPE_SALES_PACKAGE : TYPE_SALES_SINGLE;
		}
	}
	else {
		SetTradeResultCode(ret, 225, 0);
	}
}

// tradedlg.cpp:3665
void FrTradeEditDlg::OnCloseBnInit(intptr_t param) {
	m_pCloseBn = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:3671
void FrTradeEditDlg::OnCloseBnUp(void) {
	SetTradeResultCode(18, 225, 0);
}

// tradedlg.cpp:3677
void FrTradeEditDlg::OnExitBnInit(int param) {
	m_pExitBn = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:3683
void FrTradeEditDlg::OnExitBnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->GetListTradeItem()->size() > 0) {
		WSingleton<CSharedDoc>::Instance()->SendTradeOpenShop();
	} else {
		WSingleton<CSharedDoc>::Instance()->SendTradeCloseShop();
	}

	OnFreshCancel();
}

// tradedlg.cpp:3698
void FrTradeEditDlg::OnPackageSaleInit(int param) {
	m_pPackageSaleBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pPackageSaleBtn) {
		m_toolTipManager.Add((FrWnd**)&m_pPackageSaleBtn, (eBarButton)0x47);
	}
}

// tradedlg.cpp:3707
void FrTradeEditDlg::OnPackageSaleButtonUp(void) {
}

// tradedlg.cpp:3717
void FrTradeEditDlg::OnTooltipAreaInit(int param) {
	m_pTooltipArea = DYNAMIC_CAST(FrArea, param);
}

// tradedlg.cpp:3722
void FrTradeEditDlg::OnTooltipAreaOwnerDraw(int) {
	m_toolTipManager.Process();
}

// tradedlg.cpp:3729
bool FrTradeEditDlg::OnInit(void) {
	EnableDrag(false);

	SetListEnableTradingMyItem();
	SetListTradingItem();
	SetListMyItem();
	SetListTradeItem();
	SetTradeMyItem(0);
	if (m_pTradeMyItemNumber) m_pTradeMyItemNumber->ClearLine(); if (m_pTradeMyItemPrice) m_pTradeMyItemPrice->ClearLine(); m_pTradeMyItemNumber->Enable(false); m_pTradeMyItemPrice->Enable(false); m_pTradeMyItemOkBn->Enable(false); m_pTradeMyItemCancelBn->Enable(false);

	if (WSingleton<CSharedDoc>::Instance()->GetListTradeItem()->size() > 0) {
		m_pOpenBn->Enable(true); m_pCloseBn->Enable(true); m_pExitBn->Enable(true);
	} else {
		m_pOpenBn->Enable(false); m_pCloseBn->Enable(false); m_pExitBn->Enable(true);
	}

	if (m_packageSale == TYPE_SALES_PACKAGE) {
		if (m_pPackageSaleBtn) m_pPackageSaleBtn->SetStatus(FrButton::PRESSED);
	}

	return true;
}

// tradedlg.cpp:3756
bool FrTradeEditDlg::PushPackageSaleButton(void) {
	if (!m_pPackageSaleBtn) {
		return false;
	}

	m_pPackageSaleBtn->SetStatus(FrButton::PRESSED);

	return true;
}

// tradedlg.cpp:3768
bool FrTradeEditDlg::IsPushedPackageSaleButton(void) {
	return m_pPackageSaleBtn->GetStatus() == FrButton::PRESSED ? true : false;
}

// tradedlg.cpp:4082
void FrTradeEditDlg::SetListEquipParts(void) {
	m_equipPartsList.clear();

	for (std::map<unsigned int, sCharacterInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_characterMap.begin(); it != WSingleton<CSharedDoc>::Instance()->m_characterMap.end(); ++it) {
		for (int i = 0; i < 24; i++) {
			if (it->second.partTypeIds[i] != 0) {
				sPartEquipInfo info;
				info.typeId = it->second.partTypeIds[i];
				info.guid = it->second.partIds[i];
				m_equipPartsList.push_back(info);
			}
		}
	}
}

// tradedlg.cpp:4111
bool FrTradeEditDlg::IsEquipParts(unsigned long typeId, unsigned long guid) {
	bool bFound = false;

	for (std::list<sPartEquipInfo>::iterator it = m_equipPartsList.begin(); it != m_equipPartsList.end(); ++it) {
		if ((it->guid == 0 && it->typeId == typeId) || (it->guid != 0 && it->typeId == typeId && it->guid == guid)) {
			m_equipPartsList.erase(it);
			bFound = true;
			break;
		}
	}

	return bFound;
}

// tradedlg.cpp:4137
int FrTradeEditDlg::GetNumEquipItem(unsigned long typeId) {
	int num = 0;

	for (int i = 0; i < 10; i++) {
		if (typeId == WSingleton<CSharedDoc>::Instance()->m_equipItem[i]) {
			num++;
		}
	}

	return num;
}

// tradedlg.cpp:4154
int FrTradeEditDlg::GetNumTradingItem(unsigned long guid) {
	int num = 0;

	std::list<sTradeItem>* pList = WSingleton<CSharedDoc>::Instance()->GetListTradeItem();
	for (std::list<sTradeItem>::iterator it = pList->begin(); it != pList->end(); ++it) {
		if (it->guid == guid) {
			num += it->count;
		}
	}

	return num;
}

// tradedlg.cpp:4172
void FrTradeEditDlg::SetListTradingItem(void) {
	std::list<sTradeItem>* pList = WSingleton<CSharedDoc>::Instance()->GetListTradeItem();
	for (std::list<sTradeItem>::iterator it = pList->begin(); it != pList->end(); ++it) {
		m_tradeItemList.push_back(*it);
	}
}

// tradedlg.cpp:4184
void FrTradeEditDlg::SetListMyItem(void) {
	if (m_pMyItemList) {
		m_pMyItemList->ClearItem();

		_TradeItemSort<sTradeItem> sorter;
		m_myItemList.sort(sorter);
		for (std::list<sTradeItem>::iterator it = m_myItemList.begin(); it != m_myItemList.end(); ++it) {
			m_pMyItemList->AddItem(&*it);
		}
	}
}

// tradedlg.cpp:4228
void FrTradeEditDlg::SetListTradeItem(void) {
	if (m_pTradeItemList) {
		m_pTradeItemList->ClearItem();

		for (std::list<sTradeItem>::iterator it = m_tradeItemList.begin(); it != m_tradeItemList.end(); ++it) {
			m_pTradeItemList->AddItem(&*it);
		}
	}
}

// tradedlg.cpp:4364
void FrTradeEditDlg::AddListTradeItem(sTradeItem* pItem) {
	if (pItem == 0) return;
	if (m_pTradeMyItem == 0) return;

	std::list<sTradeItem>::iterator it;
	for (it = m_tradeItemList.begin(); it != m_tradeItemList.end(); ++it) {
		if (&*it == m_pTradeMyItem) break;
	}

	bool bExist = false;
	if (it != m_tradeItemList.end()) {
		if (&*it == m_pTradeMyItem) {
			bExist = true;
		}
	}

	if (!bExist) {
		m_tradeItemList.push_back(*pItem);
		m_pTradeItemList->AddItem(&m_tradeItemList.back());
	}
	else {
		*m_pTradeMyItem = *pItem;
	}
}

// tradedlg.cpp:4415
void FrTradeEditDlg::DelListTradeItem(sTradeItem* pItem) {
	if (pItem) {
		for (std::list<sTradeItem>::iterator it = m_tradeItemList.begin(); it != m_tradeItemList.end(); ++it) {
			if (&*it == pItem) {
				m_pTradeItemList->DelItem(&*it);
				m_tradeItemList.erase(it);
				break;
			}
		}
	}
}

// tradedlg.cpp:4433
void FrTradeEditDlg::AddListMyItem(sTradeItem* pItem) {
	if (pItem == 0) return;

	std::list<sTradeItem>::iterator it;
	for (it = m_myItemList.begin(); it != m_myItemList.end(); ++it) {
		if (it->guid == pItem->guid) break;
	}

	if (it == m_myItemList.end()) {
		m_myItemList.push_back(*pItem);
		m_pMyItemList->AddItem(&m_myItemList.back());
	}
	else {
		if ((int)pItem->count > 0) {
			it->count += pItem->count;
		}
	}
}

// tradedlg.cpp:4466
void FrTradeEditDlg::DelListMyItem(sTradeItem* pItem) {
	if (pItem) {
		if (m_pTradeMyItem) {
			std::list<sTradeItem>::iterator it;
			for (it = m_myItemList.begin(); it != m_myItemList.end(); ++it) {
				if (it->guid == pItem->guid) {
					break;
				}
			}

			if (it == m_myItemList.end()) {
				return;
			}

			unsigned long tradingCount = 0;
			for (std::list<sTradeItem>::iterator trading = m_tradeItemList.begin(); trading != m_tradeItemList.end(); ++trading) {
				if (&*trading == m_pTradeMyItem) {
					tradingCount = trading->count;
					break;
				}
			}

			if (&*it == m_pTradeMyItem) {
				if (it->count == pItem->count) {
					m_pMyItemList->DelItem(&*it);
					m_myItemList.erase(it);
				} else if ((int)it->count > (int)pItem->count) {
					it->count -= pItem->count;
				}
			} else {
				it->count += tradingCount - pItem->count;
			}
		}
	}
}

// tradedlg.cpp:4517
void FrTradeEditDlg::CancelSelectedTradeItem(void) {
	if (m_pTradeItemList) {
		FrListItem* pSelected = m_pTradeItemList->GetSelected();

		if (pSelected) {
			sTradeItem* pItem = (sTradeItem*)pSelected->pData;

			if (pItem) {
				if (pItem == m_pTradeMyItem) {
					SetTradeMyItem(0);
					SetControlsTradeMyItem(0, false);
				}

				AddListMyItem(pItem);
				DelListTradeItem(pItem);

				if (m_tradeItemList.size() <= 0) {
					if (WSingleton<CSharedDoc>::Instance()->GetListTradeItem()->size() > 0) {
						SetControlsButton(false, true);
					} else {
						SetControlsButton(false, false);
					}
				}
			}
		}
	}
}

// tradedlg.cpp:4550
void FrTradeEditDlg::DrawPang(void) {
	if (!g_pFresh->GetManager()->GetGDI())
		return;
	if (!m_pNumberFont)
		return;

	char buf[32] = "";
	WRect rect = GetRect();
	static int s_fontWidth = m_pNumberFont->GetFontInfo()->fontw;
	static float s_textWidth;

	MakePriceStrA(buf, WSingleton<CSharedDoc>::Instance()->m_myPang);
	s_textWidth = (s_fontWidth - 1.0f) * (strlen(buf) - 1);
	m_pNumberFont->Print(g_view, rect.x + 130.0f - s_textWidth, rect.y + 483.0f, buf, 0, 0xffffffff, 0);
}

// tradedlg.cpp:4668
void FrTradeEditDlg::SetControlsButton(bool bOpen, bool bClose) {
	m_pOpenBn->Enable(bOpen);
	m_pCloseBn->Enable(bClose);
	m_pExitBn->Enable(true);
}

// tradedlg.cpp:4676
void FrTradeEditDlg::SetControlsTradeMyItem(sTradeItem* pItem, bool bEnable) {
	if (m_pTradeMyItemNumber)
		m_pTradeMyItemNumber->ClearLine();
	if (m_pTradeMyItemPrice)
		m_pTradeMyItemPrice->ClearLine();

	if (pItem == 0) {
		m_pTradeMyItemNumber->Enable(false);
		m_pTradeMyItemPrice->Enable(false);
		m_pTradeMyItemOkBn->Enable(false);
		m_pTradeMyItemCancelBn->Enable(false);
	} else {
		m_pTradeMyItemNumber->Enable(true);
		m_pTradeMyItemPrice->Enable(true);
		m_pTradeMyItemOkBn->Enable(true);
		m_pTradeMyItemCancelBn->Enable(true);

		if (pItem->count == 1) {
			m_pTradeMyItemNumber->SetLine(1, "1", 0, false, 0);
			m_pTradeMyItemNumber->Enable(bEnable);
		}

		if (pItem->price > 0) {
			m_pTradeMyItemNumber->SetLine(1, MakeStr("%d", pItem->count), 0, false, 0);
			m_pTradeMyItemPrice->SetLine(1, MakeStr("%I64d", pItem->price), 0, false, 0);
		}
	}
}

// tradedlg.cpp:4712
int FrTradeEditDlg::CheckTradeEdit(void) {
	memset(m_tradeTitle, 0, sizeof(m_tradeTitle));

	if (m_tradeItemList.size() <= 0) {
		return 8;
	}

	const char* title = WSingleton<CSharedDoc>::Instance()->GetTradeTitle();
	if (strlen(title) >= 0x20) {
		return 6;
	}

	char buf[0x100] = {0};
	strcpy(buf, title);

	if (WSingleton<CSharedDoc>::Instance()->m_chatManager.Filtering(buf)) {
		return 7;
	}

	int len = strlen(title);
	if (len == 0) {
		len = strlen(WSingleton<CSharedDoc>::Instance()->GetTradeTitle());
		if (len == 0) {
			return 5;
		}
	}

	WTL::CString strTitle(title);
	strTitle.TrimRight();
	if (strTitle.GetLength() == 0) {
		return 0x11;
	}

	if (m_pPackageSaleBtn->GetStatus() == FrButton::PRESSED) {
		if (FindIf(m_tradeItemList, CheckPackageSaleItem)) {
			return 0x1e;
		}
	}

	strcpy(m_tradeTitle, title);
	return 1;
}

// tradedlg.cpp:4769
void FrTradeEditDlg::UpdateTradeTitle(void) {
	if (m_pTitle) {
		m_pTitle->ClearLine();
		m_pTitle->SetLine(1, WSingleton<CSharedDoc>::Instance()->GetTradeTitle(), 0, false, 0);
	}
}

// tradedlg.cpp:4778
void FrTradeEditDlg::UpdateTradeTitleEdit(void) {
	if (m_pTitleEdit) {
		m_pTitleEdit->ClearLine();
		m_pTitleEdit->SetLine(1, WSingleton<CSharedDoc>::Instance()->GetTradeTitle(), 0, false, 0);
	}
}

// tradedlg.cpp:4788
eResultTrade FrTradeEditDlg::SendTradeTitle(void) {
	eResultTrade ret = RESULT_TRADE_OK;

	if (m_pTitleEdit) {
		WTL::CString strTitle(m_pTitleEdit->GetLine(1, false));
		strTitle.TrimRight();
		if (strTitle.GetLength() == 0) {
			return (eResultTrade)5;
		}

		const char* line = m_pTitleEdit->GetLine(1, false);
		int len = strlen(line);

		char buf[0x100] = {0};
		strcpy(buf, line);

		if (WSingleton<CSharedDoc>::Instance()->m_chatManager.Filtering(buf)) {
			ret = (eResultTrade)7;
		} else if (len > 0 && len < 0x20) {
			if (strcmp(line, WSingleton<CSharedDoc>::Instance()->GetTradeTitle()) != 0) {
				WSingleton<CSharedDoc>::Instance()->SendTradeTitle(std::string(line));
			}
		}

		m_pTitleEdit->ClearLine();
	}

	return ret;
}

// tradedlg.cpp:4826 (run-time type and factory registration of FrTradeFinishEditDlg)
const WRTTI FrTradeFinishEditDlg::m_RTTI("FrTradeFinishEditDlg", &FrForm::m_RTTI);

IObject* FrTradeFinishEditDlgMakeInstance(void) {
	return new FrTradeFinishEditDlg;
}

struct __sFrTradeFinishEditDlg {
	__sFrTradeFinishEditDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTradeFinishEditDlgMakeInstance, "FrTradeFinishEditDlg");
	}
};
static __sFrTradeFinishEditDlg s_registerFrTradeFinishEditDlg;

// tradedlg.cpp:4828
const sFRESH_MSGMAP* FrTradeFinishEditDlg::GetMessageMap(void) const {
	return &FrTradeFinishEditDlg::_MsgMap;
}

// tradedlg.cpp:4887
FrTradeFinishEditDlg::~FrTradeFinishEditDlg(void) {
	if (m_pTitleFont) {
		m_pTitleFont->Erase();
	}

	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// tradedlg.cpp:4898
void FrTradeFinishEditDlg::OnTradeFinishEditDlgInit(int) {
}

// tradedlg.cpp:4903
void FrTradeFinishEditDlg::OnUserIdInit(int param) {
	m_pUserId = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:4909
void FrTradeFinishEditDlg::OnTitleInit(intptr_t param) {
	m_pTitle = DYNAMIC_CAST(FrEdit, param);
}

// tradedlg.cpp:4915
void FrTradeFinishEditDlg::OnOkBnInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:4970
bool FrTradeFinishEditDlg::OnInit(void) {
	EnableDrag(false);

	return true;
}

// tradedlg.cpp:4979
void FrTradeFinishEditDlg::OnTradeItemListInit(int param) {
	m_pTradeItemList = DYNAMIC_CAST(FrListBox, param);
}

// tradedlg.cpp:5206
void FrTradeFinishEditDlg::SetTradeUserId(void) {
	if (m_pUserId) {
		m_pUserId->SetLine(1, "\xB3\xAA\xC0\xC7 \xBB\xF3\xC1\xA1", 0, false, 0);
	}
}

// tradedlg.cpp:5217
void FrTradeFinishEditDlg::SetTradeTitle(char* title) {
	if (title) {
		if (strlen(title) < 0x20) {
			m_tradeTitleSource = 0;
			int len = strlen(title);
			int docLen = strlen(WSingleton<CSharedDoc>::Instance()->GetTradeTitle());

			if (len <= 0 && docLen <= 0) {
				m_tradeTitleSource = 0;
				return;
			}

			if (len > 0 && len < 0x20) {
				strcpy(m_tradeTitle, title);
				m_tradeTitleSource = 2;
			} else if (docLen > 0 && docLen < 0x20) {
				strcpy(m_tradeTitle, WSingleton<CSharedDoc>::Instance()->GetTradeTitle());
				m_tradeTitleSource = 1;
			}

			if (m_pTitle) {
				m_pTitle->SetLine(1, m_tradeTitle, 0, false, 0);
			}
		}
	}
}

// tradedlg.cpp:5251
void FrTradeFinishEditDlg::SetListTradeItem(std::list<sTradeItem>* pList) {
	if (m_pTradeItemList) {
		m_pListTradeItem = pList;
		m_pTradeItemList->ClearItem();

		for (std::list<sTradeItem>::iterator it = m_pListTradeItem->begin(); it != m_pListTradeItem->end(); ++it) {
			m_pTradeItemList->AddItem(&*it);
		}
	}
}

// tradedlg.cpp:5263 (run-time type and factory registration of FrTradeWarningDlg)
const WRTTI FrTradeWarningDlg::m_RTTI("FrTradeWarningDlg", &FrForm::m_RTTI);

IObject* FrTradeWarningDlgMakeInstance(void) {
	return new FrTradeWarningDlg;
}

struct __sFrTradeWarningDlg {
	__sFrTradeWarningDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTradeWarningDlgMakeInstance, "FrTradeWarningDlg");
	}
};
static __sFrTradeWarningDlg s_registerFrTradeWarningDlg;

// tradedlg.cpp:5265
const sFRESH_MSGMAP* FrTradeWarningDlg::GetMessageMap(void) const {
	return &FrTradeWarningDlg::_MsgMap;
}

// tradedlg.cpp:5278
FrTradeWarningDlg::FrTradeWarningDlg(void)
	: m_pMsg01(0)
	, m_pMsg02(0) {
	m_warningType = 1;
}

// tradedlg.cpp:5286
FrTradeWarningDlg::~FrTradeWarningDlg(void) {
}

// tradedlg.cpp:5291
void FrTradeWarningDlg::OnTradeWarningDlgInit(int) {
}

// tradedlg.cpp:5296
void FrTradeWarningDlg::OnMsg01Init(int param) {
	m_pMsg01 = DYNAMIC_CAST(FrStatic, param);
}

// tradedlg.cpp:5302
void FrTradeWarningDlg::OnMsg02Init(int param) {
	m_pMsg02 = DYNAMIC_CAST(FrStatic, param);
}

// tradedlg.cpp:5308
void FrTradeWarningDlg::OnOkBnInit(intptr_t param) {
	m_pOkBn = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:5314
void FrTradeWarningDlg::OnOkBnUp(void) {
	switch (m_warningType) {
	case 0x12:
	case 0x13: {
		WSingleton<CSharedDoc>::Instance()->SendTradeCloseShop();

		IActor* pActor = AfxGetTask()->GetActor(s_gatewayActorName);
		if (pActor) {
			pActor << MsgObject(0, 0x273, 5, 0, 0, 0, 0);
		}
		break;
	}
	case 0x14: {
		FrTradeEditDlg* pDlg = FrStaticCast<FrTradeEditDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("tradeeditdlg"));

		if (pDlg) {
			pDlg->CancelSelectedTradeItem();
		}
		break;
	}
	}

	OnFreshOkay();
}

// tradedlg.cpp:5355
void FrTradeWarningDlg::OnCancelBnInit(int param) {
	m_pCancelBn = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:5361
void FrTradeWarningDlg::OnCancelBnUp(void) {
	OnFreshCancel();
}

// tradedlg.cpp:5367
void FrTradeWarningDlg::SetControls(int type) {
	if (type == 0) {
		m_pCancelBn->SetVisible(false);
		m_pOkBn->MoveWindow(WPoint(m_rect.x + 92.0f, m_rect.y + 137.0f));
	} else {
		m_pOkBn->SetVisible(true);
		m_pCancelBn->SetVisible(true);
		m_pOkBn->MoveWindow(WPoint(m_rect.x + 50.0f, m_rect.y + 137.0f));
		m_pCancelBn->MoveWindow(WPoint(m_rect.x + 135.0f, m_rect.y + 137.0f));
	}
}

// tradedlg.cpp:5587
void FrTradeWarningDlg::SetWarningMessageForPackage(int code) {
	if (code >= 1900001 && code <= 1900039) {
		m_pMsg01->SetCaption(MakeStr("\xB9\xAD\xC0\xBD\xC6\xC7\xB8\xC5 \xBF\xC0\xB7\xF9 [%d]", code));
		SetControls(0);
	} else {
		SetWarningMessageType(code, 0);
	}
}

// tradedlg.cpp:5603 (run-time type and factory registration of FrTradeLevelLimitDlg)
const WRTTI FrTradeLevelLimitDlg::m_RTTI("FrTradeLevelLimitDlg", &FrForm::m_RTTI);

IObject* FrTradeLevelLimitDlgMakeInstance(void) {
	return new FrTradeLevelLimitDlg;
}

struct __sFrTradeLevelLimitDlg {
	__sFrTradeLevelLimitDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTradeLevelLimitDlgMakeInstance, "FrTradeLevelLimitDlg");
	}
};
static __sFrTradeLevelLimitDlg s_registerFrTradeLevelLimitDlg;

// tradedlg.cpp:5605
const sFRESH_MSGMAP* FrTradeLevelLimitDlg::GetMessageMap(void) const {
	return &FrTradeLevelLimitDlg::_MsgMap;
}

sFRESH_MSGMAP FrTradeLevelLimitDlg::_MsgMap = { &FrForm::_MsgMap, &FrTradeLevelLimitDlg::_MsgEntries[0] };

sFRESH_ENTRY FrTradeLevelLimitDlg::_MsgEntries[] = {
	// tradedlg.cpp:5607
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTradeLevelLimitDlg::OnOkBnInit },
	// tradedlg.cpp:5608
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTradeLevelLimitDlg::OnOkBnUp },

	// tradedlg.cpp:5610
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// tradedlg.cpp:5613
FrTradeLevelLimitDlg::FrTradeLevelLimitDlg(void) {
}

// tradedlg.cpp:5617
FrTradeLevelLimitDlg::~FrTradeLevelLimitDlg(void) {
}

// tradedlg.cpp:5621
void FrTradeLevelLimitDlg::OnOkBnInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
}

// tradedlg.cpp:5626
void FrTradeLevelLimitDlg::OnOkBnUp(void) {
	OnFreshOkay();
}

// tradedlg.cpp:5631
bool FrTradeLevelLimitDlg::OnInit(void) {
	return true;
}

// tradedlg.cpp:5637
void SetTradeResultCode(int code, int msgId, sTradeItem* pItem) {
	if (code == 15) {
		FrTradeWarningDlg* pWarn = FrStaticCast<FrTradeWarningDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("tradewarningdlg"));
		if (!pWarn) {
			FrTradeWarningDlg* pDlg = CreateForm<FrTradeWarningDlg>(g_pFresh->GetManager(), 0, "tradewarningdlg", 0);
			if (pDlg) {
				pDlg->Open((FRESH_RESULT_CALLBACK)0, 1);
				pDlg->SetWarningMessageType(15, (unsigned char)msgId);
				pDlg->SetFadeout(false);
			}
		}
	}
	else if (code == 1) {
		if (msgId == 234) {
			if (IsLocalContent((localContentType_t)0x71) == true && WSingleton<CItemStorage>::Instance()) {
				if (WSingleton<CItemStorage>::Instance()->IsVisible()) {
					return;
				}
			}

			FrTradeResultDlg* pDlg = CreateForm<FrTradeResultDlg>(g_pFresh->GetManager(), 0, "traderesultdlg", 0);
			if (pDlg) {
				pDlg->Open((FRESH_RESULT_CALLBACK)0, 1);
				pDlg->SetFadeout(false);
				pDlg->SetResultItem(pItem);
			}
		}
	}
	else if (code == 23) {
		FrTradeLevelLimitDlg* pDlg = CreateForm<FrTradeLevelLimitDlg>(g_pFresh->GetManager(), 0, "tradelevellimitdlg", 0);
		if (pDlg) {
			pDlg->Open((FRESH_RESULT_CALLBACK)0, 0x40);
		}
	}
	else {
		FrForm* pResult = FrStaticCast<FrForm>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("traderesultdlg"));
		if (!pResult) {
			FrTradeWarningDlg* pWarn = FrStaticCast<FrTradeWarningDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("tradewarningdlg"));
			if (!pWarn) {
				FrTradeWarningDlg* pDlg = CreateForm<FrTradeWarningDlg>(g_pFresh->GetManager(), 0, "tradewarningdlg", 0);
				if (pDlg) {
					pDlg->Open((FRESH_RESULT_CALLBACK)0, 1);
					pDlg->SetWarningMessageType(code, 0);
					pDlg->SetFadeout(false);
				}
			}
		}
		else {
			switch (code) {
			case 3:
				WSingleton<CChatMsg>::Instance()->AddChatMsg(std::string("\xBB\xF3\xC1\xA1\xC0\xCC \xB4\xDD\xC7\xFB\xBD\xC0\xB4\xCF\xB4\xD9. \xB4\xD9\xC0\xBD \xB1\xE2\xC8\xB8\xBF\xA1 \xC0\xCC\xBF\xEB\xC7\xD8\xC1\xD6\xBC\xBC\xBF\xE4."), 0xFFFF, false, false);
				break;
			case 4:
				WSingleton<CChatMsg>::Instance()->AddChatMsg(std::string("\xBB\xF3\xC1\xA1\xC0\xCC \xC6\xED\xC1\xFD\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9. \xB0\xB3\xBC\xB3 \xC8\xC4 \xC0\xCC\xBF\xEB\xC7\xD8\xC1\xD6\xBC\xBC\xBF\xE4."), 0xFFFF, false, false);
				break;
			}
		}
	}
}
