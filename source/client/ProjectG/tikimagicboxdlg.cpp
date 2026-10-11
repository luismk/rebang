// Original: d:\build\custom\temp\code\client\projectg\tikimagicboxdlg.cpp
#include <stdio.h>

#include "TikiMagicBoxDlg.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frbutton.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frwndmanager.h"
#include "../client/Wangreal/include/bitmap.h"

// tikimagicboxdlg.cpp:21 (run-time type and factory registration of FrTikiMagicBoxDlg)
const WRTTI FrTikiMagicBoxDlg::m_RTTI("FrTikiMagicBoxDlg", &FrForm::m_RTTI);

IObject* FrTikiMagicBoxDlgMakeInstance(void) {
	return new FrTikiMagicBoxDlg;
}

struct __sFrTikiMagicBoxDlg {
	__sFrTikiMagicBoxDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTikiMagicBoxDlgMakeInstance, "FrTikiMagicBoxDlg");
	}
};
static __sFrTikiMagicBoxDlg s_registerFrTikiMagicBoxDlg;

// tikimagicboxdlg.cpp:23
const sFRESH_MSGMAP* FrTikiMagicBoxDlg::GetMessageMap(void) const {
	return &FrTikiMagicBoxDlg::_MsgMap;
}

sFRESH_MSGMAP FrTikiMagicBoxDlg::_MsgMap = { &FrForm::_MsgMap, &FrTikiMagicBoxDlg::_MsgEntries[0] };

// tikimagicboxdlg.cpp:25
sFRESH_ENTRY FrTikiMagicBoxDlg::_MsgEntries[] = {
	// tikimagicboxdlg.cpp:26
	{ "back_img", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnBackImageOwnerDraw },
	// tikimagicboxdlg.cpp:27
	{ "myitem_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnMyItemListInit },
	// tikimagicboxdlg.cpp:28
	{ "myitem_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnMyItemListOwnerDraw },
	// tikimagicboxdlg.cpp:29
	{ "myitem_list", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnMyItemListDBClick },
	// tikimagicboxdlg.cpp:30
	{ "myitem_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnMyItemListLBDown },
	// tikimagicboxdlg.cpp:31
	{ "myitem_list", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnMyItemListLBUp },
	// tikimagicboxdlg.cpp:33
	{ "mtr_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnMtrListInit },
	// tikimagicboxdlg.cpp:34
	{ "mtr_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnMtrListOwnerDraw },
	// tikimagicboxdlg.cpp:35
	{ "mtr_list", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnMtrListDBClick },
	// tikimagicboxdlg.cpp:36
	{ "mtr_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnMtrListLBDown },
	// tikimagicboxdlg.cpp:37
	{ "mtr_list", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnMtrListLBUp },
	// tikimagicboxdlg.cpp:39
	{ "category_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnCategoryListInit },
	// tikimagicboxdlg.cpp:40
	{ "category_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnCategoryListOwnerDraw },
	// tikimagicboxdlg.cpp:41
	{ "category_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnCategoryListLBDown },
	// tikimagicboxdlg.cpp:43
	{ "character_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnCharacterListInit },
	// tikimagicboxdlg.cpp:44
	{ "character_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnCharacterListOwnerDraw },
	// tikimagicboxdlg.cpp:45
	{ "character_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnCharacterListLBDown },
	// tikimagicboxdlg.cpp:47
	{ "help_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnHelpBtnInit },
	// tikimagicboxdlg.cpp:48
	{ "help_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnHelpBtnLBUp },
	// tikimagicboxdlg.cpp:49
	{ "fy_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnCloseBtnInit },
	// tikimagicboxdlg.cpp:50
	{ "fy_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnCloseBtnLBUp },
	// tikimagicboxdlg.cpp:51
	{ "char_tab", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnCharTabInit },
	// tikimagicboxdlg.cpp:52
	{ "char_tab", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnCharTabLBUp },
	// tikimagicboxdlg.cpp:53
	{ "item_tab", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnItemTabInit },
	// tikimagicboxdlg.cpp:54
	{ "item_tab", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnItemTabLBUp },
	// tikimagicboxdlg.cpp:55
	{ "insert_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnInsertBtnInit },
	// tikimagicboxdlg.cpp:56
	{ "insert_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnInsertBtnLBUp },
	// tikimagicboxdlg.cpp:57
	{ "mix_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnMixBtnInit },
	// tikimagicboxdlg.cpp:58
	{ "mix_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnMixBtnLBUp },
	// tikimagicboxdlg.cpp:59
	{ "ok_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTikiMagicBoxDlg::OnCancelInit },
	// tikimagicboxdlg.cpp:60
	{ "ok_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTikiMagicBoxDlg::OnCancelLBUp },

	// tikimagicboxdlg.cpp:62
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// tikimagicboxdlg.cpp:67
void FrTikiMagicBoxDlg::OnBackImageOwnerDraw(int param) {
	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	if (pDevice == 0) {
		return;
	}

	DrawBitmapOnDlg(0.0f, 0.0f, "tiki_bg_1");
	DrawBitmapOnDlg(256.0f, 0.0f, "tiki_bg_2");
	DrawBitmapOnDlg(512.0f, 0.0f, "tiki_bg_3");
	DrawBitmapOnDlg(0.0f, 256.0f, "tiki_bg_4");
	DrawBitmapOnDlg(256.0f, 256.0f, "tiki_bg_5");
	DrawBitmapOnDlg(512.0f, 256.0f, "tiki_bg_6");

	DrawBitmapOnDlg(480.0f, 17.0f, "tiki_x_l");

	DrawBitmapOnDlg(38.0f, 137.0f, "tiki_tab_box_left");
	DrawBitmapOnDlg(257.0f, 137.0f, "tiki_tab_box_right");

	// "Please insert the %dth material item." (EUC-KR)
	char szText[128] = "\0";
	sprintf(szText, "%d\xb9\xf8\xc2\xb0 \xc0\xe7\xb7\xe1 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xb3\xd6\xbe\xee \xc1\xd6\xbc\xbc\xbf\xe4.", m_mtrIndex + 1);

	pDevice->SetTextStyle(1);
	PrintTextOnDlg(215.0f, 285.0f, szText, 0xffff0000);
	pDevice->SetTextStyle(0);
}

// tikimagicboxdlg.cpp:100
void FrTikiMagicBoxDlg::OnMyItemListInit(intptr_t param) {
	BindListBox(m_pMyItemList, param);
}

// tikimagicboxdlg.cpp:105
void FrTikiMagicBoxDlg::OnMyItemListOwnerDraw(int param) {
}

// tikimagicboxdlg.cpp:110
void FrTikiMagicBoxDlg::OnMyItemListDBClick(void) {
}

// tikimagicboxdlg.cpp:114
void FrTikiMagicBoxDlg::OnMyItemListLBDown(void) {
}

// tikimagicboxdlg.cpp:118
void FrTikiMagicBoxDlg::OnMyItemListLBUp(void) {
}

// tikimagicboxdlg.cpp:124
void FrTikiMagicBoxDlg::OnMtrListInit(int param) {
	BindListBox(m_pMtrList, param);
}

// tikimagicboxdlg.cpp:129
void FrTikiMagicBoxDlg::OnMtrListOwnerDraw(int param) {
}

// tikimagicboxdlg.cpp:133
void FrTikiMagicBoxDlg::OnMtrListDBClick(void) {
}

// tikimagicboxdlg.cpp:137
void FrTikiMagicBoxDlg::OnMtrListLBDown(void) {
}

// tikimagicboxdlg.cpp:141
void FrTikiMagicBoxDlg::OnMtrListLBUp(void) {
}

// tikimagicboxdlg.cpp:147
void FrTikiMagicBoxDlg::OnCategoryListInit(int param) {
	BindListBox(m_pCategoryList, param);
}

// tikimagicboxdlg.cpp:152
void FrTikiMagicBoxDlg::OnCategoryListOwnerDraw(int param) {
}

// tikimagicboxdlg.cpp:156
void FrTikiMagicBoxDlg::OnCategoryListLBDown(void) {
}

// tikimagicboxdlg.cpp:162
void FrTikiMagicBoxDlg::OnCharacterListInit(int param) {
	BindListBox(m_pCharacterList, param);
}

// tikimagicboxdlg.cpp:167
void FrTikiMagicBoxDlg::OnCharacterListOwnerDraw(int param) {
}

// tikimagicboxdlg.cpp:171
void FrTikiMagicBoxDlg::OnCharacterListLBDown(void) {
}

// tikimagicboxdlg.cpp:178
void FrTikiMagicBoxDlg::OnHelpBtnInit(intptr_t param) {
	BindButton(m_pHelpBtn, param);
}

// tikimagicboxdlg.cpp:183
void FrTikiMagicBoxDlg::OnHelpBtnLBUp(void) {
}

// tikimagicboxdlg.cpp:189
void FrTikiMagicBoxDlg::OnCloseBtnInit(intptr_t param) {
	BindButton(m_pCloseBtn, param);
}

// tikimagicboxdlg.cpp:194
void FrTikiMagicBoxDlg::OnCloseBtnLBUp(void) {
}

// tikimagicboxdlg.cpp:200
void FrTikiMagicBoxDlg::OnCharTabInit(int param) {
	BindButton(m_pCharTab, param);
}

// tikimagicboxdlg.cpp:205
void FrTikiMagicBoxDlg::OnCharTabLBUp(void) {
}

// tikimagicboxdlg.cpp:211
void FrTikiMagicBoxDlg::OnItemTabInit(int param) {
	BindButton(m_pItemTab, param);
}

// tikimagicboxdlg.cpp:216
void FrTikiMagicBoxDlg::OnItemTabLBUp(void) {
}

// tikimagicboxdlg.cpp:221
void FrTikiMagicBoxDlg::OnInsertBtnInit(int param) {
	BindButton(m_pInsertBtn, param);
}

// tikimagicboxdlg.cpp:226
void FrTikiMagicBoxDlg::OnInsertBtnLBUp(void) {
}

// tikimagicboxdlg.cpp:232
void FrTikiMagicBoxDlg::OnMixBtnInit(int param) {
	BindButton(m_pMixBtn, param);
}

// tikimagicboxdlg.cpp:237
void FrTikiMagicBoxDlg::OnMixBtnLBUp(void) {
}

// tikimagicboxdlg.cpp:243
void FrTikiMagicBoxDlg::OnCancelInit(intptr_t param) {
	BindButton(m_pCancel, param);
}

// tikimagicboxdlg.cpp:248
void FrTikiMagicBoxDlg::OnCancelLBUp(void) {
}

// tikimagicboxdlg.cpp:257
FrTikiMagicBoxDlg::FrTikiMagicBoxDlg(void) {
	m_mtrIndex = 0;
}

// tikimagicboxdlg.cpp:263
FrTikiMagicBoxDlg::~FrTikiMagicBoxDlg(void) {
}

// tikimagicboxdlg.cpp:270
void FrTikiMagicBoxDlg::BindButton(FrButton*& pButton, int param) {
	pButton = DYNAMIC_CAST(FrButton, param);

	if (pButton) {
		pButton->SetPushDelay(0.1f);
		m_bindList.push_back(pButton);
	}
}

// tikimagicboxdlg.cpp:284
void FrTikiMagicBoxDlg::BindListBox(FrListBox*& pListBox, int param) {
	pListBox = DYNAMIC_CAST(FrListBox, param);

	if (pListBox) {
		pListBox->ClearItem();
		m_bindList.push_back(pListBox);
	}
}

// tikimagicboxdlg.cpp:298
void FrTikiMagicBoxDlg::DrawBitmapOnDlg(float x, float y, const char* name) {
	if (name) {
		FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
		if (pDevice) {
			const Bitmap* pBitmap = g_pFresh->GetBitmap(name);
			if (pBitmap) {
				WRect wndRect = GetRect();
				WRect rect(wndRect.x + x, wndRect.y + y, pBitmap->Width(), pBitmap->Height());
				pDevice->DrawTexture(pBitmap, rect, 0xffffffff, 0);
			}
		}
	}
}

// tikimagicboxdlg.cpp:317
void FrTikiMagicBoxDlg::PrintTextOnDlg(float x, float y, const char* text, unsigned long color) {
	if (text) {
		FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
		if (pDevice) {
			WRect rect = GetRect();

			pDevice->SetTextColor(color, 0xffffffff);
			pDevice->PrintText(WPoint(rect.x + x, rect.y + y), 0, text, 0);
			pDevice->SetTextColor(0xff000000, 0xffffffff);
		}
	}
}

// tikimagicboxdlg.cpp:343
int FrTikiMagicBoxDlg::InsertMaterial(const sTikiMagicBoxMtr& mtr) {
	return 0;
}

// tikimagicboxdlg.cpp:352
int FrTikiMagicBoxDlg::DeleteMaterial(const sTikiMagicBoxMtr& mtr) {
	return 0;
}
