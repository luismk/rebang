// Original: d:\build\custom\temp\code\client\projectg\ucccopydlg.cpp
#include <string.h>

#include "UccCopyDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "fresh.h"
#include "UccLibrary.h"
#include "UccManager.h"
#include "Packet.h"
#include "../shared/globalgamedefine.h"
#include "frbutton.h"
#include "frlistbox.h"
#include "frgraphicinterface.h"
#include "SharedDoc.h"

// ucccopydlg.cpp:14 (run-time type and factory registration of FrUccCopyDlg)
const WRTTI FrUccCopyDlg::m_RTTI("FrUccCopyDlg", &FrForm::m_RTTI);

IObject* FrUccCopyDlgMakeInstance(void) {
	return new FrUccCopyDlg;
}

struct __sFrUccCopyDlg {
	__sFrUccCopyDlg(void) {
		ObjectFactory().AddObjectFunctor(FrUccCopyDlgMakeInstance, "FrUccCopyDlg");
	}
};
static __sFrUccCopyDlg s_registerFrUccCopyDlg;

// ucccopydlg.cpp:16
const sFRESH_MSGMAP* FrUccCopyDlg::GetMessageMap(void) const {
	return &FrUccCopyDlg::_MsgMap;
}

sFRESH_MSGMAP FrUccCopyDlg::_MsgMap = { &FrForm::_MsgMap, &FrUccCopyDlg::_MsgEntries[0] };

sFRESH_ENTRY FrUccCopyDlg::_MsgEntries[] = {
	// ucccopydlg.cpp:18
	{ "char_icon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyDlg::OnCharIconInit },
	// ucccopydlg.cpp:19
	{ "item_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyDlg::OnItemListInit },
	// ucccopydlg.cpp:20
	{ "item_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyDlg::OnItemListOwnerDraw },
	// ucccopydlg.cpp:21
	{ "item_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrUccCopyDlg::OnItemListBtnDown },
	// ucccopydlg.cpp:22
	{ "ok_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyDlg::OnOkBtnInit },
	// ucccopydlg.cpp:23
	{ "ok_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccCopyDlg::OnOkBtnUp },
	// ucccopydlg.cpp:24
	{ "cancel_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyDlg::OnCancelBtnInit },
	// ucccopydlg.cpp:25
	{ "cancel_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccCopyDlg::OnCancelBtnUp },
	// ucccopydlg.cpp:26
	{ "etc", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyDlg::OnEtcOwnerDraw },

	// ucccopydlg.cpp:28
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ucccopydlg.cpp:32
FrUccCopyDlg::FrUccCopyDlg(void) {
	Init();
}

// ucccopydlg.cpp:37
FrUccCopyDlg::~FrUccCopyDlg(void) {
}

// ucccopydlg.cpp:41
void FrUccCopyDlg::Init(void) {
	m_pItem = 0;
	m_pConfirmDlg = 0;

	m_pItemBaseNormal = g_pFresh->RegisterBitmap("item_base_n");
	m_pItemBaseOver = g_pFresh->RegisterBitmap("item_base_o");
	m_pItemBaseDown = g_pFresh->RegisterBitmap("item_base_d");
}

// ucccopydlg.cpp:51
void FrUccCopyDlg::SetItem(sItemInfo* pItem) {
	m_pItem = pItem;
	BuildItemList();
}

// ucccopydlg.cpp:57
void FrUccCopyDlg::OnCharIconInit(intptr_t param) {
	m_pCharIcon = DYNAMIC_CAST(FrArea, param);

	if (m_pCharIcon && m_pItem) {

		IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(m_pItem->m_typeId);
		if (pPart) {

			const Bitmap* pBitmap = g_pFresh->GetBitmap(pPart->m_icon);
			if (pBitmap) {

				m_pCharIcon->SetBgImg(pBitmap);
			}
		}
	}
}

// ucccopydlg.cpp:75
void FrUccCopyDlg::OnItemListInit(intptr_t param) {
	m_pItemList = DYNAMIC_CAST(FrListBox, param);
}

// ucccopydlg.cpp:124
void FrUccCopyDlg::OnItemListBtnDown(void) {
}

// ucccopydlg.cpp:129
void FrUccCopyDlg::OnOkBtnInit(intptr_t param) {
	m_pOkBtn = DYNAMIC_CAST(FrButton, param);
}

// ucccopydlg.cpp:158
void FrUccCopyDlg::OnCancelBtnInit(intptr_t param) {
	m_pCancelBtn = DYNAMIC_CAST(FrArea, param);
}

// ucccopydlg.cpp:163
void FrUccCopyDlg::OnCancelBtnUp(void) {
	OnFreshCancel();
}

// ucccopydlg.cpp:207
void FrUccCopyDlg::BuildItemList(void) {
	if (!m_pItemList) {
		return;
	}
	IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(m_pItem->m_typeId);


	if (!pPart) {
		return;
	}
	std::list<sItemInfo>::iterator it;
	for (it = WSingleton<CSharedDoc>::Instance()->m_myItemList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_myItemList.end(); ++it) {






		sItemInfo* pItem = &(*it);

		if (pItem && (pItem->m_uccStatus & 1) && pItem->m_uccSeq == 1) {

			IFF_STRUCT::sPart* pOtherPart = ItemManager()->FindPart(pItem->m_typeId);


			if (pOtherPart) {
				if (IsSameClothes(pPart, pOtherPart)) {



					m_pItemList->AddItem(pItem);
				}
			}
		}
	}
}

// ucccopydlg.cpp:254
bool FrUccCopyDlg::OnUccCopyConfirmDlgResult(int ret, FrForm* pForm) {
	if (pForm) {
		FrForm::ms_bHasTail = true;
	}

	m_pConfirmDlg = 0;
	if (ret == FrOK) {
		OnFreshOkay();
	}

	return true;
}

// ucccopydlg.cpp:277 (run-time type and factory registration of FrUccCopyConfirmDlg)
const WRTTI FrUccCopyConfirmDlg::m_RTTI("FrUccCopyConfirmDlg", &FrForm::m_RTTI);

IObject* FrUccCopyConfirmDlgMakeInstance(void) {
	return new FrUccCopyConfirmDlg;
}

struct __sFrUccCopyConfirmDlg {
	__sFrUccCopyConfirmDlg(void) {
		ObjectFactory().AddObjectFunctor(FrUccCopyConfirmDlgMakeInstance, "FrUccCopyConfirmDlg");
	}
};
static __sFrUccCopyConfirmDlg s_registerFrUccCopyConfirmDlg;

// ucccopydlg.cpp:279
const sFRESH_MSGMAP* FrUccCopyConfirmDlg::GetMessageMap(void) const {
	return &FrUccCopyConfirmDlg::_MsgMap;
}

sFRESH_MSGMAP FrUccCopyConfirmDlg::_MsgMap = { &FrForm::_MsgMap, &FrUccCopyConfirmDlg::_MsgEntries[0] };

sFRESH_ENTRY FrUccCopyConfirmDlg::_MsgEntries[] = {
	// ucccopydlg.cpp:281
	{ "icon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyConfirmDlg::OnIconInit },
	// ucccopydlg.cpp:282
	{ "ok_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyConfirmDlg::OnOkBtnInit },
	// ucccopydlg.cpp:283
	{ "ok_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccCopyConfirmDlg::OnOkBtnUp },
	// ucccopydlg.cpp:284
	{ "cancel_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyConfirmDlg::OnCancelBtnInit },
	// ucccopydlg.cpp:285
	{ "cancel_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccCopyConfirmDlg::OnCancelBtnUp },
	// ucccopydlg.cpp:286
	{ "etc", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyConfirmDlg::OnEtcOwnerDraw },

	// ucccopydlg.cpp:288
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ucccopydlg.cpp:291
FrUccCopyConfirmDlg::FrUccCopyConfirmDlg(void) {
	m_pSrcItem = 0;
	m_pDestItem = 0;
}

// ucccopydlg.cpp:297
FrUccCopyConfirmDlg::~FrUccCopyConfirmDlg(void) {
}

// ucccopydlg.cpp:301
void FrUccCopyConfirmDlg::OnIconInit(intptr_t param) {
	m_pIcon = DYNAMIC_CAST(FrArea, param);
}

// ucccopydlg.cpp:306
void FrUccCopyConfirmDlg::OnOkBtnInit(intptr_t param) {
	m_pOkBtn = DYNAMIC_CAST(FrButton, param);
}

// ucccopydlg.cpp:311
void FrUccCopyConfirmDlg::OnOkBtnUp(void) {
	WSendPacket packet(0xb1);
	packet.Encode1(2);
	packet.Encode4(m_pDestItem->m_typeId);
	packet.EncodeStr(m_pDestItem->m_uccKey);
	packet.Encode2(m_pDestItem->m_uccSeq);
	packet.Encode4(m_pSrcItem->m_serial);
	packet.Send(SEND_TO_DEFAULT);

	OnFreshOkay();
}

// ucccopydlg.cpp:324
void FrUccCopyConfirmDlg::OnCancelBtnInit(intptr_t param) {
	m_pCancelBtn = DYNAMIC_CAST(FrButton, param);
}

// ucccopydlg.cpp:329
void FrUccCopyConfirmDlg::OnCancelBtnUp(void) {
	SetFadeout(true);
	OnFreshCancel();
}

// ucccopydlg.cpp:335
void FrUccCopyConfirmDlg::OnEtcOwnerDraw(int param) {
	FrGraphicInterface* pGDI = GetGDI();
	if (!pGDI) {
		return;
	}
	if (!m_pDestItem) {
		return;
	}

	sUccClothes* pClothes = UccManager()->FindClothes(m_pDestItem->m_typeId, m_pDestItem->m_uccKey);
	if (pClothes) {
		WRect rect = GetRect();

		WPoint pos(rect.x + 100.0f, rect.y + 28.0f);
		// "Do you want to copy it?"
		pGDI->PrintText(pos, 0, "\xBA\xB9\xC1\xA6\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", 0);
		pGDI->SetTextStyle(1);
		pGDI->PrintText(WPoint(pos.x, pos.y + 16.0f), 0, pClothes->m_uccName, 0);
		pGDI->SetTextStyle(0);
	}
}

// ucccopydlg.cpp:372
void FrUccCopyConfirmDlg::Init(sItemInfo* pSrcItem, sItemInfo* pDestItem) {
	m_pSrcItem = pSrcItem;
	m_pDestItem = pDestItem;

	if (m_pIcon && m_pDestItem) {
		const Bitmap* pIcon = GetUccItemIcon(m_pDestItem);
		if (pIcon) {
			m_pIcon->SetBgImg(pIcon);
		}
	}
}

// ucccopydlg.cpp:392 (run-time type and factory registration of FrUccCopyCompletedDlg)
const WRTTI FrUccCopyCompletedDlg::m_RTTI("FrUccCopyCompletedDlg", &FrForm::m_RTTI);

IObject* FrUccCopyCompletedDlgMakeInstance(void) {
	return new FrUccCopyCompletedDlg;
}

struct __sFrUccCopyCompletedDlg {
	__sFrUccCopyCompletedDlg(void) {
		ObjectFactory().AddObjectFunctor(FrUccCopyCompletedDlgMakeInstance, "FrUccCopyCompletedDlg");
	}
};
static __sFrUccCopyCompletedDlg s_registerFrUccCopyCompletedDlg;

// ucccopydlg.cpp:394
const sFRESH_MSGMAP* FrUccCopyCompletedDlg::GetMessageMap(void) const {
	return &FrUccCopyCompletedDlg::_MsgMap;
}

sFRESH_MSGMAP FrUccCopyCompletedDlg::_MsgMap = { &FrForm::_MsgMap, &FrUccCopyCompletedDlg::_MsgEntries[0] };

sFRESH_ENTRY FrUccCopyCompletedDlg::_MsgEntries[] = {
	// ucccopydlg.cpp:396
	{ "ok_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyCompletedDlg::OnOkBtnInit },
	// ucccopydlg.cpp:397
	{ "ok_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccCopyCompletedDlg::OnOkBtnUp },
	// ucccopydlg.cpp:398
	{ "etc", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccCopyCompletedDlg::OnEtcOwnerDraw },

	// ucccopydlg.cpp:400
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ucccopydlg.cpp:403
FrUccCopyCompletedDlg::FrUccCopyCompletedDlg(void) {
	m_typeId = 0;
	m_name[0] = '\0';
}

// ucccopydlg.cpp:409
FrUccCopyCompletedDlg::~FrUccCopyCompletedDlg(void) {
}

// ucccopydlg.cpp:413
void FrUccCopyCompletedDlg::Init(unsigned long typeId, const char* name) {
	m_typeId = typeId;
	strncpy(m_name, name, 9);
}

// ucccopydlg.cpp:419
void FrUccCopyCompletedDlg::OnOkBtnInit(intptr_t param) {
	m_pOkBtn = DYNAMIC_CAST(FrButton, param);
}

// ucccopydlg.cpp:424
void FrUccCopyCompletedDlg::OnOkBtnUp(void) {
	OnFreshOkay();
}
