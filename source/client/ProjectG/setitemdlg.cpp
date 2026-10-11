// Original: d:\build\custom\temp\code\client\projectg\setitemdlg.cpp
#include "SetItemDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "fredit.h"
#include "frlistbox.h"
#include "SharedDoc.h"
#include "UccManager.h"
#include "bitmap.h"
#include "commonutil.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"

// setitemdlg.cpp:17 (run-time type and factory registration of FrSetItemDlg)
const WRTTI FrSetItemDlg::m_RTTI("FrSetItemDlg", &FrForm::m_RTTI);

IObject* FrSetItemDlgMakeInstance(void) {
	return new FrSetItemDlg;
}

struct __sFrSetItemDlg {
	__sFrSetItemDlg(void) {
		ObjectFactory().AddObjectFunctor(FrSetItemDlgMakeInstance, "FrSetItemDlg");
	}
};
static __sFrSetItemDlg s_registerFrSetItemDlg;

// setitemdlg.cpp:19
const sFRESH_MSGMAP* FrSetItemDlg::GetMessageMap(void) const {
	return &FrSetItemDlg::_MsgMap;
}

sFRESH_MSGMAP FrSetItemDlg::_MsgMap = { &FrForm::_MsgMap, &FrSetItemDlg::_MsgEntries[0] };

sFRESH_ENTRY FrSetItemDlg::_MsgEntries[] = {
	// setitemdlg.cpp:21
	{ "portrait", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSetItemDlg::OnPortraitInit },
	// setitemdlg.cpp:22
	{ "name", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSetItemDlg::OnNameInit },
	// setitemdlg.cpp:23
	{ "component", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSetItemDlg::OnComponentInit },
	// setitemdlg.cpp:24
	{ "component", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSetItemDlg::OnComponentOwnerDraw },

	// setitemdlg.cpp:29
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// setitemdlg.cpp:33
FrSetItemDlg::FrSetItemDlg(void) {
	m_pPortrait = 0;
	m_pName = 0;
	m_pComponent = 0;
	m_pSetItem = 0;
}

// setitemdlg.cpp:112
void FrSetItemDlg::OnPortraitInit(intptr_t param) {
	m_pPortrait = DYNAMIC_CAST(FrArea, param);
}

// setitemdlg.cpp:117
void FrSetItemDlg::OnNameInit(intptr_t param) {
	m_pName = DYNAMIC_CAST(FrEdit, param);
}

// setitemdlg.cpp:122
void FrSetItemDlg::OnComponentInit(intptr_t param) {
	m_pComponent = DYNAMIC_CAST(FrListBox, param);
}

// setitemdlg.cpp:133
void FrSetItemDlg::OnComponentOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0) {
		return;
	}

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0) {
		return;
	}

	IFF_ITEM_COMMON* pData = (IFF_ITEM_COMMON*)pItem->pData;
	if (pData == 0) {
		return;
	}

	const Bitmap* pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS", pData->m_icon);
	if (pBitmap == 0) {
		pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS_FASHION", pData->m_icon);
	}
	if (pBitmap) {
		WRect rect(pItem->pos.x, pItem->pos.y, pBitmap->Width(), pBitmap->Height());

		pGDI->DrawTexture(pBitmap, rect, 0xFFFFFFFF, 0);
	}

	pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	pGDI->SetTextStyle(1);
	pGDI->Print(WPoint(pItem->pos.x + 90.0f, pItem->pos.y + 14.0f), 0, "%s", pData->m_name);

	pGDI->SetTextStyle(0);

	pBitmap = g_pFresh->GetManager()->GetBitmap("LEVELS", MakeStr("level_%03d", (pData->m_level & 0x7F) + 1));
	if (pBitmap) {
		float x = pGDI->GetTextExtend("\xB7\xB9    \xBA\xA7:") + pItem->pos.x;

		WRect rect(x + 102.0f, pItem->pos.y + 31.0f, pBitmap->Width(),
			pBitmap->Height());
		pGDI->DrawTexture(pBitmap, rect, 0xFFFFFFFF, 0);

		pGDI->Print(WPoint(pItem->pos.x + 90.0f, pItem->pos.y + 34.0f), 0, "\xB7\xB9    \xBA\xA7:");

		if ((pData->m_level & 0x7F) > CSharedDoc::Instance()->m_myLevel) {
			pGDI->Print(WPoint(rect.x + rect.w + 5.0f, pItem->pos.y + 34.0f), 0, "\xC0\xCC\xBB\xF3");
			pGDI->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
		} else {
			pGDI->Print(WPoint(rect.x + rect.w + 5.0f, pItem->pos.y + 34.0f), 0, "\xC0\xCC\xBB\xF3");
		}
	}
}
