// Original: d:\build\custom\temp\code\client\projectg\furniture_ability.cpp
#include "Furniture_Ability.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "commonutil.h"
#include "bitmap.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frwndmanager.h"
#include "../shared/ItemManager.h"
#include "../shared/classdefine.h"
#include "../shared/globalgamedefine.h"
#include "wfont.h"
#include "wresrcmng.h"

#include <string.h>

// File names of the two title fonts (nameless data): the number font's at
// 0xa87e08, the title font's right after it at 0xa87e0c; the originals cut
// them as one symbol.
extern "C" char* data_a87e08[2];

// furniture_ability.cpp:31 (run-time type and factory registration of CFurniture_AbilityDlg)
const WRTTI CFurniture_AbilityDlg::m_RTTI("CFurniture_AbilityDlg", &FrForm::m_RTTI);

IObject* CFurniture_AbilityDlgMakeInstance(void) {
	return new CFurniture_AbilityDlg;
}

struct __sCFurniture_AbilityDlg {
	__sCFurniture_AbilityDlg(void) {
		ObjectFactory().AddObjectFunctor(CFurniture_AbilityDlgMakeInstance, "CFurniture_AbilityDlg");
	}
};
static __sCFurniture_AbilityDlg s_registerCFurniture_AbilityDlg;

// furniture_ability.cpp:33
const sFRESH_MSGMAP* CFurniture_AbilityDlg::GetMessageMap(void) const {
	return &CFurniture_AbilityDlg::_MsgMap;
}

sFRESH_MSGMAP CFurniture_AbilityDlg::_MsgMap = { &FrForm::_MsgMap, &CFurniture_AbilityDlg::_MsgEntries[0] };

sFRESH_ENTRY CFurniture_AbilityDlg::_MsgEntries[] = {
	// furniture_ability.cpp:36
	{ "myitem_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CFurniture_AbilityDlg::OnFurnitureAbility_ItemListInit },
	{ "myitem_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CFurniture_AbilityDlg::OnFurnitureAbility_ItemListOwnerDraw },
	{ "myitem_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CFurniture_AbilityDlg::OnFurnitureAbility_ItemListBtnDown },
	{ "myitem_list", FRCMD_MOUSEMOVE, FrFuncVV, (FRESH_PMSG)&CFurniture_AbilityDlg::OnFurnitureAbility_ItemListMouseMove },

	// furniture_ability.cpp:41
	{ "select_itemlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CFurniture_AbilityDlg::OnFurnitureAbility_SelectListInit },
	{ "select_item_img", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CFurniture_AbilityDlg::OnFurnitureAbility_ImgSelect },

	// furniture_ability.cpp:44
	{ "Edit_array", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CFurniture_AbilityDlg::OnFurnitureAbility_EditArrayInit },

	// furniture_ability.cpp:46
	{ "Btn_Array", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CFurniture_AbilityDlg::OnFurnitureAbility_BtnArrayInit },
	{ "select_text_img", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CFurniture_AbilityDlg::OnFurnitureAbility_TextImg },

	// furniture_ability.cpp:49
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// furniture_ability.cpp:57
CFurniture_AbilityDlg::CFurniture_AbilityDlg(void) {
	m_value11c = 0;
	m_pBtnArray = 0;
	m_pEditArray = 0;

	m_pSelectNormal = g_pFresh->GetBitmap("rmr_select_n");
	m_pSelectDown = g_pFresh->GetBitmap("rmr_select_d");

	m_pLine = g_pFresh->GetBitmap("rmr_line");

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = &data_a87e08[1];
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:,";

	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);

	tagWTITLEFONT numberInfo;
	memset(&numberInfo, 0, sizeof(numberInfo));
	numberInfo.filename = data_a87e08;
	numberInfo.fontw = 9;
	numberInfo.fonth = 16;
	numberInfo.numPages = 1;
	numberInfo.texw = 128;
	numberInfo.texh = 32;
	numberInfo.pCharSet = "1234567890,-";

	m_pNumberFont = g_resrcmng->GetTitleFont();
	m_pNumberFont->Create(&numberInfo);

	SetList();
}

// furniture_ability.cpp:111
CFurniture_AbilityDlg::~CFurniture_AbilityDlg(void) {
	if (m_pTitleFont) {
		m_pTitleFont->Erase();
	}

	if (m_pNumberFont) {
		m_pNumberFont->Erase();
	}

	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
	if (g_resrcmng && m_pNumberFont) { g_resrcmng->Release(m_pNumberFont); m_pNumberFont = 0; }
}

// furniture_ability.cpp:136
void CFurniture_AbilityDlg::OnFurnitureAbility_ItemListInit(intptr_t param) {
	m_pItemList = DYNAMIC_CAST(FrListBox, param);
}

// furniture_ability.cpp:150
void CFurniture_AbilityDlg::OnFurnitureAbility_ItemListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0)
		return;

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pItem->pData == 0)
		return;

	const Bitmap* pSelect;
	if (pItem->selected) {
		pSelect = m_pSelectDown;
		pGDI->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
	} else if (pItem->underCursor) {
		pSelect = m_pSelectNormal;
		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	} else {
		pSelect = 0;
		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	}

	sItemInfo* pInfo = (sItemInfo*)pItem->pData;

	WRect rc;
	WRect rcLine;
	GetClientRect(rc);
	for (int row = 0; row < 2; row++) {
		float y = row * 110.0f + 87.0f;
		for (int col = 0; col < 3; col++) {
			rcLine = WRect(col * 92.0f + rc.x + 126.0f, y + rc.y, m_pLine->Width(), m_pLine->Height());

			pGDI->DrawTexture(m_pLine, rcLine, 0xFFFFFFFF, 0);
		}
	}

	if (pSelect) {
		pItem = (FrListItem*)param;
		pGDI->DrawTexture(pSelect, WRect(pItem->pos.x - 2.0f, pItem->pos.y, pSelect->Width(), pSelect->Height()), 0xFFFFFFFF, 0);
	}

	if ((pInfo->m_typeId & 0xFC000000) == 0x8000000) {
		IFF_STRUCT::sChar* pChar = ItemManager()->FindChar(((pInfo->m_typeId >> 18) & 0xFF) | 0x4000000);
		if (pChar) {
			const Bitmap* pBitmap = g_pFresh->GetBitmap(MakeStr("s_%s", pChar->m_icon));
			if (pBitmap) {
				pGDI->DrawTexture(pBitmap, WRect(pItem->pos.x + 7.0f, pItem->pos.y + 5.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
			}
		}
	}
}

// furniture_ability.cpp:232
void CFurniture_AbilityDlg::OnFurnitureAbility_ItemListBtnDown(void) {
	FrListItem* pItem = m_pItemList->GetItemUnderCursor();

	if (pItem == 0) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		// "The selected item has invalid data."
		pForm->SetMessage("\xBC\xB1\xC5\xC3\xB5\xC8 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBA \xB5\xA5\xC0\xCC\xC5\xCD\xB0\xA1 \xC0\xDF\xB8\xF8\xB5\xC8 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xD4\xB4\xCF\xB4\xD9.", false);
		pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
		return;
	}

	sItemInfo* pInfo = (sItemInfo*)pItem->pData;

	if (pInfo == 0) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		// "The selected item is an invalid item."
		pForm->SetMessage("\xBC\xB1\xC5\xC3\xC7\xD1 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBA \xC0\xDF\xB8\xF8\xB5\xC8 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xD4\xB4\xCF\xB4\xD9.", false);
		pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
		return;
	}

	m_pSelectedItem = pInfo;
}

// furniture_ability.cpp:263
void CFurniture_AbilityDlg::OnFurnitureAbility_ItemListMouseMove(void) {
}

// furniture_ability.cpp:274
void CFurniture_AbilityDlg::OnFurnitureAbility_ImgSelect(intptr_t param) {
	m_pImgSelect = DYNAMIC_CAST(FrArea, param);
	if (m_pImgSelect) {
		m_pImgSelect->SetVisible(false);
	}
}

// furniture_ability.cpp:289
void CFurniture_AbilityDlg::OnFurnitureAbility_SelectListInit(intptr_t param) {
	m_pSelectList = DYNAMIC_CAST(FrListBox, param);
	if (m_pSelectList) {
		m_pSelectList->SetVisible(false);
	}
}

// furniture_ability.cpp:304
void CFurniture_AbilityDlg::OnFurnitureAbility_EditArrayInit(intptr_t param) {
	m_pEditArray = DYNAMIC_CAST(FrEdit, param);
	if (m_pEditArray) {
		m_pEditArray->SetVisible(false);
	}
}

// furniture_ability.cpp:320
void CFurniture_AbilityDlg::OnFurnitureAbility_BtnArrayInit(intptr_t param) {
	m_pBtnArray = DYNAMIC_CAST(FrButton, param);
	if (m_pBtnArray) {
		m_pBtnArray->SetVisible(false);
	}
}

// furniture_ability.cpp:338
void CFurniture_AbilityDlg::OnFurnitureAbility_TextImg(intptr_t param) {
	m_pTextImg = DYNAMIC_CAST(FrArea, param);
	if (m_pTextImg) {
		m_pTextImg->SetVisible(false);
	}
}

// furniture_ability.cpp:354
void CFurniture_AbilityDlg::SetList(void) {
	for (std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_itemInfoList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_itemInfoList.end(); ++it) {
		sItemInfo* pInfo = &(*it);

		if (ItemManager()->GetCouponKind(pInfo->m_typeId) == 0) {
			// the short at 0xC + 4*2 of the union (meaning not identified yet)
			if (pInfo->m_clubStatLevel[4] != 0) {
				m_pItemList->AddItem(pInfo);
			}
		}
	}
}

// furniture_ability.cpp:380
void CFurniture_AbilityDlg::GetItemInfo(sItemInfo *pInfo) {
}
