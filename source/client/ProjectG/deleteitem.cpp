// Original: d:\build\custom\temp\code\client\projectg\deleteitem.cpp
#include <stdlib.h>
#include "DeleteItem.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "commonutil.h"
#include "fresh.h"
#include "Packet.h"
#include "TaskManager.h"
#include "SharedDoc.h"
#include "../shared/ItemManager.h"
#include "../shared/classdefine.h"
#include "UccManager.h"
#include "bitmap.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"

// deleteitem.cpp:17 (run-time type and factory registration of FrDeleteItemDlg)
const WRTTI FrDeleteItemDlg::m_RTTI("FrDeleteItemDlg", &FrForm::m_RTTI);

IObject* FrDeleteItemDlgMakeInstance(void) {
	return new FrDeleteItemDlg;
}

struct __sFrDeleteItemDlg {
	__sFrDeleteItemDlg(void) {
		ObjectFactory().AddObjectFunctor(FrDeleteItemDlgMakeInstance, "FrDeleteItemDlg");
	}
};
static __sFrDeleteItemDlg s_registerFrDeleteItemDlg;

// deleteitem.cpp:19
const sFRESH_MSGMAP* FrDeleteItemDlg::GetMessageMap(void) const {
	return &FrDeleteItemDlg::_MsgMap;
}

sFRESH_MSGMAP FrDeleteItemDlg::_MsgMap = { &FrForm::_MsgMap, &FrDeleteItemDlg::_MsgEntries[0] };

sFRESH_ENTRY FrDeleteItemDlg::_MsgEntries[] = {
	// deleteitem.cpp:21
	{ "item", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDeleteItemDlg::OnItemInit },
	// deleteitem.cpp:22
	{ "item", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDeleteItemDlg::OnItemOwnerDraw },
	// deleteitem.cpp:23
	{ "desc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDeleteItemDlg::OnDescInit },
	// deleteitem.cpp:24
	{ "warn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDeleteItemDlg::OnWarnInit },
	// deleteitem.cpp:25
	{ "item_num", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDeleteItemDlg::OnItemNumInit },
	// deleteitem.cpp:26
	{ "num", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDeleteItemDlg::OnNumInit },
	// deleteitem.cpp:27
	{ "num", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrDeleteItemDlg::OnNumEnterKey },
	// deleteitem.cpp:28
	{ "num_prev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDeleteItemDlg::OnNumPrevInit },
	// deleteitem.cpp:29
	{ "num_prev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDeleteItemDlg::OnNumPrevBtnUp },
	// deleteitem.cpp:30
	{ "num_next", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrDeleteItemDlg::OnNumNextInit },
	// deleteitem.cpp:31
	{ "num_next", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDeleteItemDlg::OnNumNextBtnUp },
	// deleteitem.cpp:32
	{ "check", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrDeleteItemDlg::OnCheckBtnUp },

	// deleteitem.cpp:34
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// deleteitem.cpp:37
FrDeleteItemDlg::FrDeleteItemDlg(void) {
	m_delItemTypeId = 0;
}

// deleteitem.cpp:42
void FrDeleteItemDlg::SetDelItem(unsigned long typeId, unsigned long count) {
	m_delItemTypeId = typeId;
	m_delItemCount = count;

	if (m_pItemNum) {
		m_pItemNum->SetLine(1, MakeStr("%d", m_delItemCount), 0, false, 0);
	}

	IFF_STRUCT::sDesc* pDesc = ItemManager()->FindDesc(m_delItemTypeId);

	if (m_pDesc) {
		m_pDesc->AddText(pDesc ? pDesc->m_text : "", false, true);
	}
}

// deleteitem.cpp:56
void FrDeleteItemDlg::OnItemInit(intptr_t param) {
	m_pItem = DYNAMIC_CAST(FrArea, param);
}

// deleteitem.cpp:61
void FrDeleteItemDlg::OnItemOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0) {
		return;
	}

	WRect rect = m_pItem->GetRect();

	IFF_ITEM_COMMON* pData = ItemManager()->FindCommonItem(m_delItemTypeId);
	if (pData == 0) {
		return;
	}

	const Bitmap* pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS", pData->m_icon);

	if (pBitmap == 0) {
		pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS_FASHION", pData->m_icon);
	}
	if (pBitmap) {
		pGDI->DrawTexture(pBitmap, WRect(rect.x, rect.y, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pGDI->Print(WPoint(rect.x + 100.0f, rect.y), 2, "\xC0\xCC\xB8\xA7:");
	pGDI->Print(WPoint(rect.x + 110.0f, rect.y), 0, pData->m_name);

	pGDI->Print(WPoint(rect.x + 100.0f, rect.y + 30.0f), 2, "\xB7\xB9\xBA\xA7:");

	pBitmap = g_pFresh->GetManager()->GetBitmap("LEVELS", MakeStr("level_%03d", (pData->m_level & 0x7F) + 1));
	if (pBitmap) {
		pGDI->DrawTexture(pBitmap, WRect(rect.x + 110.0f, rect.y + 27.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pGDI->Print(WPoint(rect.x + 170.0f, rect.y + 30.0f), 0, "\xC0\xCC\xBB\xF3");
}

// deleteitem.cpp:94
void FrDeleteItemDlg::OnDescInit(intptr_t param) {
	m_pDesc = DYNAMIC_CAST(FrEdit, param);
}

// deleteitem.cpp:101
void FrDeleteItemDlg::OnWarnInit(intptr_t param) {
	FrEdit* pWarn = DYNAMIC_CAST(FrEdit, param);
	if (pWarn) {
		// "You can tidy up your storage by deleting items you do not need.
		// Delete the unneeded item?"
		pWarn->AddText("\xB3\xAA\xBF\xA1\xB0\xD4 \xC7\xCA\xBF\xE4\xBE\xF8\xB4\xC2 \xBE\xC6\xC0\xCC\xC5\xDB\xB5\xE9\xC0\xBB \xBB\xE8\xC1\xA6\xC7\xCF\xBF\xA9 \xC3\xA2\xB0\xED\xB8\xA6 \xC1\xA4\xB8\xAE\xC7\xD2 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.\n\xC7\xCA\xBF\xE4\xBE\xF8\xB4\xC2 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBB \xBB\xE8\xC1\xA6\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false, true);
	}
}

// deleteitem.cpp:110
void FrDeleteItemDlg::OnItemNumInit(intptr_t param) {
	m_pItemNum = DYNAMIC_CAST(FrEdit, param);
}

// deleteitem.cpp:115
void FrDeleteItemDlg::OnNumInit(intptr_t param) {
	m_pNum = DYNAMIC_CAST(FrEdit, param);
	if (m_pNum) {
		m_pNum->SetLine(1, "1", 0, false, 0);
	}
}

// deleteitem.cpp:124
bool FrDeleteItemDlg::OnNumEnterKey(int param) {
	return false;
}

// deleteitem.cpp:129
void FrDeleteItemDlg::OnNumPrevInit(intptr_t param) {
	m_pNumPrev = DYNAMIC_CAST(FrButton, param);
	if (m_pNumPrev) {
		m_pNumPrev->SetPushDelay(0.0f);
	}
}

// deleteitem.cpp:138
void FrDeleteItemDlg::OnNumPrevBtnUp(void) {
	if (m_pNum) {

		const char* text = m_pNum->GetLine(1, false);
		int num = atoi(text);

		if (num > 1) {
			m_pNum->SetLine(1, MakeStr("%d", num - 1), 0, false, 0);
		}
	}
}

// deleteitem.cpp:150
void FrDeleteItemDlg::OnNumNextInit(intptr_t param) {
	m_pNumNext = DYNAMIC_CAST(FrButton, param);
	if (m_pNumNext) {
		m_pNumNext->SetPushDelay(0.0f);
	}
}

// deleteitem.cpp:159
void FrDeleteItemDlg::OnNumNextBtnUp(void) {
	if (m_pNum) {

		const char* text = m_pNum->GetLine(1, false);
		int num = atoi(text);

		if (num < 99) {
			m_pNum->SetLine(1, MakeStr("%d", num + 1), 0, false, 0);
		}
	}
}

// deleteitem.cpp:171
void FrDeleteItemDlg::OnCheckBtnUp(void) {
	int num = atoi(m_pNum->GetLine(1, false));
	if (num <= 0) {
		// "You entered an invalid quantity."
		AfxGetTask()->PassMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\xC0\xDF\xB8\xF8\xB5\xC8 \xBC\xF6\xB7\xAE\xC0\xBB \xC0\xD4\xB7\xC2\xC7\xCF\xBC\xCC\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0));
		return;
	}
	if ((unsigned long)num > m_delItemCount) {
		// "The limit was exceeded."
		AfxGetTask()->PassMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\xC7\xD1\xB5\xB5\xB8\xA6 \xC3\xCA\xB0\xFA \xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0));
		return;
	}

	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_okcancel", 0);
	// "Do you really want to delete it?"
	pForm->SetMessage("\\c0xffff0000\\c\xC1\xA4\xB8\xBB \xBB\xE8\xC1\xA6\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
	pForm->Open((FRESH_RESULT_CALLBACK)&FrDeleteItemDlg::OnConfirmResult, 3);
}

// deleteitem.cpp:197
bool FrDeleteItemDlg::OnConfirmResult(int result, FrForm* pForm) {
	if (result == 1) {
		WSendPacket packet((enumClientPacket)0x64);
		packet.Encode4(m_delItemTypeId);
		packet.Encode4(atoi(m_pNum->GetLine(1, false)));
		packet.Send(SEND_TO_DEFAULT);

		AfxGetTask()->PassMsgToActor("RealMyRoom", MsgObject(0, 1, 0, 0, 0, 0, 0));
		Close(FrOK, true);
	}

	return true;
}
