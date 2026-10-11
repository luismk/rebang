// Original: d:\build\custom\temp\code\client\projectg\panggiftdlg.cpp
#include "PangGiftDlg.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "UccManager.h"
#include "bitmap.h"
#include "commonutil.h"
#include "frarea.h"
#include "fredit.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"

// panggiftdlg.cpp:17 (run-time type and factory registration of FrPangGiftDlg)
const WRTTI FrPangGiftDlg::m_RTTI("FrPangGiftDlg", &FrForm::m_RTTI);

IObject* FrPangGiftDlgMakeInstance(void) {
	return new FrPangGiftDlg;
}

struct __sFrPangGiftDlg {
	__sFrPangGiftDlg(void) {
		ObjectFactory().AddObjectFunctor(FrPangGiftDlgMakeInstance, "FrPangGiftDlg");
	}
};
static __sFrPangGiftDlg s_registerFrPangGiftDlg;

// panggiftdlg.cpp:18
const sFRESH_MSGMAP* FrPangGiftDlg::GetMessageMap(void) const {
	return &FrPangGiftDlg::_MsgMap;
}

sFRESH_MSGMAP FrPangGiftDlg::_MsgMap = { &FrForm::_MsgMap, &FrPangGiftDlg::_MsgEntries[0] };

sFRESH_ENTRY FrPangGiftDlg::_MsgEntries[] = {
	// panggiftdlg.cpp:20
	{ "my_pang", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPangGiftDlg::OnMyPangInit },
	// panggiftdlg.cpp:21
	{ "send_pang", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPangGiftDlg::OnSendPangInit },
	// panggiftdlg.cpp:22
	{ "total_pang", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPangGiftDlg::OnTotalPangInit },
	// panggiftdlg.cpp:23
	{ "item", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPangGiftDlg::OnItemInit },
	// panggiftdlg.cpp:24
	{ "item", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPangGiftDlg::OnItemOwnerDraw },

	// panggiftdlg.cpp:26
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// panggiftdlg.cpp:30
FrPangGiftDlg::FrPangGiftDlg(void) {
	m_pMyPangEdit = 0;
	m_pSendPangEdit = 0;
	m_pTotalPangEdit = 0;

	m_itemInfo = 0;
}

// panggiftdlg.cpp:39
void FrPangGiftDlg::OnMyPangInit(intptr_t param) {
	m_pMyPangEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pMyPangEdit)
		m_pMyPangEdit->SetLine(1, MakeStr("%I64d\xC6\xCE", WSingleton<CSharedDoc>::Instance()->m_myPang), 0, false, 0);
}

// panggiftdlg.cpp:47
void FrPangGiftDlg::OnSendPangInit(intptr_t param) {
	m_pSendPangEdit = DYNAMIC_CAST(FrEdit, param);
}

// panggiftdlg.cpp:53
void FrPangGiftDlg::OnTotalPangInit(intptr_t param) {
	m_pTotalPangEdit = DYNAMIC_CAST(FrEdit, param);
}

// panggiftdlg.cpp:58
void FrPangGiftDlg::SetItemInfo(sGiftInfo* pInfo) {
	if (m_pSendPangEdit) {
		// panggiftdlg.cpp:61
		m_pSendPangEdit->SetLine(1, MakeStr("%d\xC6\xCE", pInfo->m_pang), 0, false, 0);
	}

	if (m_pTotalPangEdit) {
		// panggiftdlg.cpp:66
		m_pTotalPangEdit->SetLine(1, MakeStr("%I64d\xC6\xCE", WSingleton<CSharedDoc>::Instance()->m_myPang + pInfo->m_pang), 0, false, 0);
	}

	m_itemInfo = pInfo;
}

// panggiftdlg.cpp:73
void FrPangGiftDlg::OnItemInit(intptr_t param) {
	m_pItemArea = DYNAMIC_CAST(FrArea, param);
}

// panggiftdlg.cpp:78
void FrPangGiftDlg::OnItemOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {


		IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(0x1A000010);
		if (pItem) {


			const Bitmap* pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS", pItem->m_icon);
			if (pBitmap) {

				WRect rect(m_pItemArea->GetRect().x + 20.0f, m_pItemArea->GetRect().y + 30.0f, pBitmap->Width(), pBitmap->Height());
				pGDI->DrawTexture(pBitmap, rect, 0xFFFFFFFF, 0);
			}



			pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
			pGDI->SetTextStyle(1);
			pGDI->Print(WPoint(m_pItemArea->GetRect().x + 175.0f, m_pItemArea->GetRect().y + 15.0f), 2, "%s", pItem->m_name);
			pGDI->SetTextStyle(0);


			pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
			pGDI->SetTextStyle(0);
			pGDI->Print(WPoint(m_pItemArea->GetRect().x + 170.0f, m_pItemArea->GetRect().y + 40.0f), 2, "%d\xC6\xCE", m_itemInfo->m_pang);
		}
	}
}
