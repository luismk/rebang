// Original: d:\build\custom\temp\code\client\projectg\levelupitemdlg.cpp
#include <stdio.h>
#include <string.h>
#include "LevelUpItemDlg.h"
#include "ObjectFactory.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "bitmap.h"
#include "frarea.h"
#include "fredit.h"
#include "fresh.h"
#include "frlistbox.h"
#include "frwndmanager.h"
#include "wfont.h"
#include "wresrcmng.h"
#include "wview.h"
#include "UccManager.h"
#include "commonutil.h"
#include "frgraphicinterface.h"

// The title font's file name list (0xab4fd8, no symbol).
// Provide a minimal placeholder so code that expects a filename list
// can operate. Real filenames are not known; use a single null entry.
extern "C" char* data_ab4fd8[] = { NULL };

// The reward item and its explanation of each level (0xab4d90, no symbol; a
// file-local table of levelupitemdlg.h, lines 106-202, built by sub_009d2db0).
// Provide a minimal placeholder table with empty strings to satisfy the
// linker. Real localized strings should replace these.
extern "C" const char* data_ab4d90[22][3] = { { "", "", "" } };

// The reward of each level (0xa22f70, no symbol).
struct sLevelUpReward {
	int level;              // 0x0
	unsigned long typeId;   // 0x4
	int count;              // 0x8
};
CHECK_SIZE(sLevelUpReward, 0xc);
// Minimal placeholder rewards table.
extern "C" const sLevelUpReward data_a22f70[22] = { {0,0,0} };

// levelupitemdlg.cpp:17 (run-time type and factory registration of FrLevelupItemForm)
const WRTTI FrLevelupItemForm::m_RTTI("FrLevelupItemForm", &FrForm::m_RTTI);

IObject* FrLevelupItemFormMakeInstance(void) {
	return new FrLevelupItemForm;
}

struct __sFrLevelupItemForm {
	__sFrLevelupItemForm(void) {
		ObjectFactory().AddObjectFunctor(FrLevelupItemFormMakeInstance, "FrLevelupItemForm");
	}
};
static __sFrLevelupItemForm s_registerFrLevelupItemForm;

// levelupitemdlg.cpp:19
const sFRESH_MSGMAP* FrLevelupItemForm::GetMessageMap(void) const {
	return &FrLevelupItemForm::_MsgMap;
}

sFRESH_MSGMAP FrLevelupItemForm::_MsgMap = { &FrForm::_MsgMap, &FrLevelupItemForm::_MsgEntries[0] };

sFRESH_ENTRY FrLevelupItemForm::_MsgEntries[] = {
	// levelupitemdlg.cpp:21
	{ "levelupitem", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLevelupItemForm::OnLevelUpItemInit },
	// levelupitemdlg.cpp:22
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrLevelupItemForm::OnCloseUp },
	// levelupitemdlg.cpp:23
	{ "level", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLevelupItemForm::OnLevelIcon },
	// levelupitemdlg.cpp:24
	{ "frame2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLevelupItemForm::OnFrame2Init },
	// levelupitemdlg.cpp:25
	{ "frame3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLevelupItemForm::OnFrame3Init },
	// levelupitemdlg.cpp:26
	{ "itemsend", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLevelupItemForm::OnItemSendInit },
	// levelupitemdlg.cpp:27
	{ "leveluplist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLevelupItemForm::OnItemListInit },
	// levelupitemdlg.cpp:28
	{ "leveluplist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLevelupItemForm::OnItemListOwnerDraw },
	// levelupitemdlg.cpp:30
	{ "itemexplane", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLevelupItemForm::OnItemExplaneInit },

	// levelupitemdlg.cpp:32
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// levelupitemdlg.cpp:35
FrLevelupItemForm::FrLevelupItemForm(void) {
	m_pLevelIcon = 0;
	m_pFrame2 = 0;
	m_pFrame3 = 0;
	m_pItemSend = 0;
	m_pItemList = 0;
	m_pItemExplane = 0;
	m_level = 0;
	m_rewardReceived = 0;
	m_rewardTypeId = 0;
	m_bonusTypeId = 1;
	m_hasReward = false;

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = data_ab4fd8;
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:";

	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);
}

// levelupitemdlg.cpp:66
FrLevelupItemForm::~FrLevelupItemForm(void) {
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// levelupitemdlg.cpp:71
void FrLevelupItemForm::OnLevelUpItemInit(int) {
}

// levelupitemdlg.cpp:76
void FrLevelupItemForm::OnCloseUp(void) {
	Close(FrNONE, true);
}

// levelupitemdlg.cpp:82
bool FrLevelupItemForm::Close(eFormRet ret, bool bDestroy) {
	return FrForm::Close(ret, bDestroy);
}

// levelupitemdlg.cpp:87
void FrLevelupItemForm::OnLevelIcon(intptr_t param) {
	m_pLevelIcon = DYNAMIC_CAST(FrArea, param);
}

// levelupitemdlg.cpp:92
void FrLevelupItemForm::OnFrame2Init(intptr_t param) {
	m_pFrame2 = DYNAMIC_CAST(FrArea, param);
}

// levelupitemdlg.cpp:97
void FrLevelupItemForm::OnFrame3Init(intptr_t param) {
	m_pFrame3 = DYNAMIC_CAST(FrArea, param);
}

// levelupitemdlg.cpp:102
void FrLevelupItemForm::OnItemSendInit(intptr_t param) {
	m_pItemSend = DYNAMIC_CAST(FrEdit, param);
}

// levelupitemdlg.cpp:109
void FrLevelupItemForm::OnItemListInit(intptr_t param) {
	m_pItemList = DYNAMIC_CAST(FrListBox, param);

	m_clockBtn.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "clock");
	m_clockBtn.rect = WRect(50.0f, 35.0f, m_clockBtn.pBitmap->Width(), m_clockBtn.pBitmap->Height());
	m_deleteBtn.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "delete");
	m_deleteBtn.rect = WRect(50.0f, 35.0f, m_deleteBtn.pBitmap->Width(), m_deleteBtn.pBitmap->Height());
}

// levelupitemdlg.cpp:119
void FrLevelupItemForm::OnItemListOwnerDraw(intptr_t param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	FrListItem* pItem = (FrListItem*)param;

	if (pGDI == 0 || pItem == 0) return;

	unsigned long* pTypeId = (unsigned long*)pItem->pData;

	if (*pTypeId == 0) return;

	IFF_ITEM_COMMON* pData = ItemManager()->FindCommonItem(*pTypeId);

	if (pData == 0) return;

	unsigned char timeType = pData->m_timeType;

	const Bitmap* pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS", pData->m_icon);
	if (pBitmap == 0) return;

	float offsetX = 25.0f;
	float offsetY = 30.0f;
	float countOffset = 0.0f;
	int couponKind = ItemManager()->GetCouponKind(*pTypeId);

	if (m_level > 1) {

		if ((*pTypeId & 0xFC000000) == 0x70000000) {

			offsetX = 55.0f;
			offsetY = 0.0f;

		} else {

			offsetX = 75.0f;
			offsetY = 30.0f;
		}

		countOffset = 55.0f;
	}

	WRect rect(pItem->pos.x + offsetX + 18.0f, pItem->pos.y + offsetY + 18.0f, pBitmap->Width(), pBitmap->Height());

	pGDI->DrawTexture(pBitmap, rect, 0xFFFFFFFF, 0);
	pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	pGDI->SetTextStyle(4);

	if (timeType >= 2) {

		pGDI->DrawTexture(m_clockBtn.pBitmap, m_clockBtn.rect + WPoint(pItem->pos.x + 15.0f, pItem->pos.y + 15.0f), 0xFFFFFFFF, 0);

	} else {
		int count;

		if (m_level == 1) {

			count = 10;

		} else {

			if (m_level >= 21) return;

			count = data_a22f70[m_level].count;

			if (count <= 1) return;
		}

		const char* text = MakeStr("%d", count);
		float x = pItem->pos.x + 90.0f;

		for (unsigned int i = 0; i < strlen(text); i++) {
			x -= m_pTitleFont->GetCharWidthA(g_view, text[i] - '0');
		}

		m_pTitleFont->Print(g_view, x + countOffset, pItem->pos.y + 75.0f, text, 0, 0xFFFFFFFF, 0);
	}
}

// levelupitemdlg.cpp:207
void FrLevelupItemForm::OnItemExplaneInit(intptr_t param) {
	m_pItemExplane = DYNAMIC_CAST(FrEdit, param);
}

// levelupitemdlg.cpp:212
bool FrLevelupItemForm::OnInit(void) {
	char name[32];

	if (m_pLevelIcon) {
		sprintf(name, "level_%03d", m_level + 1);
		m_pLevelIcon->SetBgImg(name);
	}

	SetItemInfo();

	if (!m_rewardReceived && m_hasReward) {
		WSendPacket packet((enumClientPacket)0x93);
		packet.Send(SEND_TO_DEFAULT);
	}

	return true;
}

// levelupitemdlg.cpp:233
void FrLevelupItemForm::SetItemInfo(void) {
	if (m_level > 21) {
		m_pItemSend->AddText(data_ab4d90[21][0], false, true);
		m_pItemExplane->AddText(data_ab4d90[21][1], false, true);
	} else {
		m_pItemSend->AddText(data_ab4d90[m_level][0], false, true);
		m_pItemExplane->AddText(data_ab4d90[m_level][1], false, true);
	}

	if (m_level == 1) {
		m_rewardTypeId = 0x18000008;
		m_bonusTypeId = 0x18000007;
		m_pItemList->AddItem(&m_rewardTypeId);
		m_pItemList->AddItem(&m_bonusTypeId);
	} else {
		if (m_level > 21)
			m_rewardTypeId = 0x1A000033;
		else
			m_rewardTypeId = data_a22f70[m_level].typeId;
		m_pItemList->AddItem(&m_rewardTypeId);
	}

	float y = m_pFrame3->GetRect().y;

	m_pItemExplane->MoveWindow(WPoint(m_pFrame3->GetRect().x + 50.0f, y + 15.0f));

	if (ItemManager()->GetCouponKind(data_a22f70[m_level].typeId) == 1) {
		// "Check the acquired item in the coupon box."
		SetDesc("     \xc8\xb9\xb5\xe6 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xba \xc4\xed\xc6\xf9\xc7\xd4\xbf\xa1\xbc\xad \xc8\xae\xc0\xce\xc7\xcf\xbc\xbc\xbf\xe4.");
	} else {
		// "Check the acquired item in the gift box."
		SetDesc("     \xc8\xb9\xb5\xe6 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xba \xbc\xb1\xb9\xb0\xc7\xd4\xbf\xa1\xbc\xad \xc8\xae\xc0\xce\xc7\xcf\xbc\xbc\xbf\xe4.");
	}
}

// levelupitemdlg.h: no line record (empty constructor of a nested helper class)
FrLevelupItemForm::MiniBtn::MiniBtn(void) {
}

// levelupitemdlg.cpp:280
void FrLevelupItemForm::OnProc(const float dt) {
}

// levelupitemdlg.cpp:285
bool FrLevelupItemForm::SetData(sLevelUpDone data) {
	m_level = data.m_level;
	m_rewardReceived = data.m_rewardReceived;
	m_hasReward = data.m_hasReward != 0;

	return true;
}
