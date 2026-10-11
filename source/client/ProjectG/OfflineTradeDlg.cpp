// Original: d:\build\custom\temp\code\client\projectg\offlinetradedlg.cpp
// This unit emits the out-of-line copies of CSharedDoc::GetTradeMode and
// GetMaketList (defined in the class body in shareddoc.h).
#define OFFLINETRADEDLG_SHAREDDOC_INLINE
#include "OfflineTradeDlg.h"
#include "ObjectFactory.h"
#include "../shared/Localize.h"
#include "ToolTip.h"
#include "fredit.h"
#include "frcombobox.h"
#include "frlistbox.h"
#include "frbutton.h"
#include "frarea.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "actor.h"
#include "commonutil.h"
#include "binstr.h"
#include "StandingInLine.h"
#include "Packet.h"
#include "fresh.h"
#include "frviewer.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"

#include <stdlib.h>
#include <string.h>

// offlinetradedlg.h: no line record (empty constructor of a nested helper class)
FrOfflineTradeFinishEditDlg::MiniBtn::MiniBtn(void) {
}

// offlinetradedlg.h: no line record (empty constructor of a nested helper class)
COfflineTradeDlg::MiniBtn::MiniBtn(void) {
}

// offlinetradedlg.h: no line record (empty constructor of a nested helper class)
COfflineShopEditDlg::MiniBtn::MiniBtn(void) {
}

// offlinetradedlg.h: no line record (empty constructor of a nested helper class)
COfflineShopDlg::MiniBtn::MiniBtn(void) {
}

// offlinetradedlg.h: no line record (empty constructor of a nested helper class)
COfflineShopVisitorDlg::MiniBtn::MiniBtn(void) {
}

// offlinetradedlg.cpp:34 (run-time type and factory registration of FrOfflineTradeFinishEditDlg)
const WRTTI FrOfflineTradeFinishEditDlg::m_RTTI("FrOfflineTradeFinishEditDlg", &FrForm::m_RTTI);

IObject* FrOfflineTradeFinishEditDlgMakeInstance(void) {
	return new FrOfflineTradeFinishEditDlg;
}

struct __sFrOfflineTradeFinishEditDlg {
	__sFrOfflineTradeFinishEditDlg(void) {
		ObjectFactory().AddObjectFunctor(FrOfflineTradeFinishEditDlgMakeInstance, "FrOfflineTradeFinishEditDlg");
	}
};
static __sFrOfflineTradeFinishEditDlg s_registerFrOfflineTradeFinishEditDlg;

// offlinetradedlg.cpp:36
const sFRESH_MSGMAP* FrOfflineTradeFinishEditDlg::GetMessageMap(void) const {
	return &FrOfflineTradeFinishEditDlg::_MsgMap;
}

// offlinetradedlg.cpp:112
FrOfflineTradeFinishEditDlg::~FrOfflineTradeFinishEditDlg(void) {
}

// offlinetradedlg.cpp:324
void FrOfflineTradeFinishEditDlg::SetListTradeItem(std::list<sTradeItem>* pList) {
	sStall* pStall = CSharedDoc::Instance()->GetStallItem();

	m_pTradeItemList->ClearItem();

	for (int i = 0; i < pStall->open; i++) {
		m_pTradeItemList->AddItem(&pStall->slots[i]);
	}
}

// offlinetradedlg.cpp:347
void FrOfflineTradeFinishEditDlg::GetSelectSlotInfo(int& index, int& used) {
	if (m_pSelectedSlotItem == 0) {
		index = used = 0;
		return;
	}

	sStallSlot* pSlot = (sStallSlot*)m_pSelectedSlotItem->pData;
	index = pSlot->index;
	used = pSlot->used;
}

// offlinetradedlg.cpp:363
void FrOfflineTradeFinishEditDlg::OnToolTipInit(intptr_t param) {
	m_pToolTip = DYNAMIC_CAST(FrArea, param);
}

// offlinetradedlg.cpp:369
void FrOfflineTradeFinishEditDlg::OnToolTipOnwerDraw(int) {
	CIconToolTip::GetInstance()->Render();
}

// offlinetradedlg.cpp:397 (run-time type and factory registration of COfflineTradeDlg)
const WRTTI COfflineTradeDlg::m_RTTI("COfflineTradeDlg", &FrForm::m_RTTI);

IObject* COfflineTradeDlgMakeInstance(void) {
	return new COfflineTradeDlg;
}

struct __sCOfflineTradeDlg {
	__sCOfflineTradeDlg(void) {
		ObjectFactory().AddObjectFunctor(COfflineTradeDlgMakeInstance, "COfflineTradeDlg");
	}
};
static __sCOfflineTradeDlg s_registerCOfflineTradeDlg;

// offlinetradedlg.cpp:399
const sFRESH_MSGMAP* COfflineTradeDlg::GetMessageMap(void) const {
	return &COfflineTradeDlg::_MsgMap;
}

sFRESH_MSGMAP COfflineTradeDlg::_MsgMap = { &FrForm::_MsgMap, &COfflineTradeDlg::_MsgEntries[0] };

sFRESH_ENTRY COfflineTradeDlg::_MsgEntries[] = {
	// offlinetradedlg.cpp:402
	{ "AreaSub_Nick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnAreaSubNickInit },
	// offlinetradedlg.cpp:403
	{ "AreaSub_Nick", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnAreaSubNickOwnerDraw },
	// offlinetradedlg.cpp:404
	{ "AreaSub_Nick", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnAreaSubNickLBtnUP },

	// offlinetradedlg.cpp:406
	{ "AreaSub_Price", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnAreaSubPriceInit },
	// offlinetradedlg.cpp:407
	{ "AreaSub_Price", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnAreaSubPriceOwnerDraw },
	// offlinetradedlg.cpp:408
	{ "AreaSub_Price", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnAreaSubPriceLBtnUP },

	// offlinetradedlg.cpp:410
	{ "AreaSub_ItemName", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnAreaSubItemNameInit },
	// offlinetradedlg.cpp:411
	{ "AreaSub_ItemName", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnAreaSubItemNameOwnerDraw },
	// offlinetradedlg.cpp:412
	{ "AreaSub_ItemName", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnAreaSubItemNameLBtnUP },

	// offlinetradedlg.cpp:414
	{ "Edit_ItemSearch", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnItemSearchInit },
	// offlinetradedlg.cpp:415
	{ "Edit_ItemSearch", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&COfflineTradeDlg::OnItemSearchEnterKey },
	// offlinetradedlg.cpp:416
	{ "Edit_ItemSearch", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnItemSearchLBtnUp },

	// offlinetradedlg.cpp:418
	{ "BtnMain_search", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnBtnMainSearchInit },
	// offlinetradedlg.cpp:419
	{ "BtnMain_search", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnBtnMainSearchUp },

	// offlinetradedlg.cpp:421
	{ "num_maxpage", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnEditMaxPageInit },
	// offlinetradedlg.cpp:422
	{ "num_curpage", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnEditCurrentPageInit },
	// offlinetradedlg.cpp:423
	{ "num_curpage", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&COfflineTradeDlg::OnEditCurrentPageEnterKey },

	// offlinetradedlg.cpp:425
	{ "BtnPage_Prev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnBtnPrevInit },
	// offlinetradedlg.cpp:426
	{ "BtnPage_Prev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnBtnPrevBtnUp },

	// offlinetradedlg.cpp:428
	{ "BtnPage_Next", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnBtnNextInit },
	// offlinetradedlg.cpp:429
	{ "BtnPage_Next", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnBtnNextBtnUp },

	// offlinetradedlg.cpp:431
	{ "BtnPage_go", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnBtnGoPageInit },
	// offlinetradedlg.cpp:432
	{ "BtnPage_go", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnBtnGoPageBtnUp },

	// offlinetradedlg.cpp:434
	{ "BtnRefresh", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnBtnRefreshInit },
	// offlinetradedlg.cpp:435
	{ "BtnRefresh", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnBtnRefreshBtnUp },

	// offlinetradedlg.cpp:437
	{ "BtnSub_Quick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnBtnDetailInit },
	// offlinetradedlg.cpp:438
	{ "BtnSub_Quick", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnBtnDetailBtnUp },

	// offlinetradedlg.cpp:440
	{ "offlinetrade_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnMaketListInit },
	// offlinetradedlg.cpp:441
	{ "offlinetrade_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnMaketListOwnerDraw },
	// offlinetradedlg.cpp:442
	{ "offlinetrade_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnMaketListBtnDown },

	// offlinetradedlg.cpp:447
	{ "Edit_Msg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnEditMsgInit },
	// offlinetradedlg.cpp:448
	{ "Edit_Msg", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&COfflineTradeDlg::OnEditMsgLBtnUp },

	// offlinetradedlg.cpp:450
	{ "tooltip", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnToolTipInit },
	// offlinetradedlg.cpp:451
	{ "tooltip", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&COfflineTradeDlg::OnToolTipOnwerDraw },

	// offlinetradedlg.cpp:454
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// offlinetradedlg.cpp:458
COfflineTradeDlg::COfflineTradeDlg(void) {
	m_pAreaSubNick = 0;
	m_pAreaSubPrice = 0;
	m_pAreaSubItemName = 0;
	m_pItemSearchEdit = 0;
	m_pMaketList = 0;
	m_pCurrentPageEdit = 0;
	m_pMaxPageEdit = 0;
	m_pBtnPrev = 0;
	m_pBtnNext = 0;
	m_pBtnGoPage = 0;
	m_pBtnMainSearch = 0;
	m_pBtnRefresh = 0;
	m_pStandingInLine = 0;
	m_pBtnDetail = 0;
	m_pEditMsg = 0;
	m_maxPage = 0;
	m_currentPage = 0;
	m_searchType = 1;
	m_searchOrder = 0;
	m_searchString.clear();

	// "Pick an item"
	SetSearchString("\xB9\xB0\xB0\xC7\xC0\xBB \xB0\xF1\xB6\xF3\xBA\xB8\xB7\xC5");

	m_myRoomState = false;
	m_canVisitMyRoom = 0;

	m_moveBtnNormal.pBitmap = g_pFresh->GetBitmap("rmr_btn_move_n");
	m_moveBtnNormal.rect = WRect(490.0f, 1.0f, m_moveBtnNormal.pBitmap->Width(), m_moveBtnNormal.pBitmap->Height());
	m_moveBtnOver.pBitmap = g_pFresh->GetBitmap("rmr_btn_move_o");
	m_moveBtnOver.rect = WRect(490.0f, 1.0f, m_moveBtnOver.pBitmap->Width(), m_moveBtnOver.pBitmap->Height());
}

// offlinetradedlg.cpp:507
COfflineTradeDlg::~COfflineTradeDlg(void) {
}

// offlinetradedlg.cpp:511
bool COfflineTradeDlg::OnInit(void) {
	if (m_pItemSearchEdit) {
		m_pItemSearchEdit->SetFontColor(0xFFD1D1D0);
		m_pItemSearchEdit->ClearLine();
		m_pItemSearchEdit->AddLine("\xb9\xb0\xb0\xc7\xc0\xbb \xb0\xf1\xb6\xf3\xba\xb8\xb7\xc5", 0, false);
		m_pItemSearchEdit->SetKeyFocus(true);
	}

	int canVisit = 0;
	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x231, (intptr_t)&canVisit, 2, 3, 0, 0));

	m_canVisitMyRoom = canVisit;

	if (m_canVisitMyRoom == 0) {
		m_pEditMsg->SetVisible(true);
		m_pEditMsg->AddText("* \xc7\xf6\xc0\xe7 \xbb\xf3\xc5\xc2\xbf\xa1\xbc\xad\xb4\xc2 \xb4\xd9\xb8\xa5 "
			"\xbb\xe7\xb6\xf7\xc0\xc7 \xb8\xb6\xc0\xcc\xb7\xeb\xc0\xbb \xb9\xe6\xb9\xae "
			"\xc7\xcf\xbd\xc7\xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", false, true);
	} else {
		m_pEditMsg->SetVisible(false);
	}

	return true;
}

// offlinetradedlg.cpp:538
void COfflineTradeDlg::OnProc(const float dt) {
	FrForm::OnProc(dt);
	CIconToolTip::GetInstance()->ClearToolTip();
}

// offlinetradedlg.cpp:544
void COfflineTradeDlg::OnAreaSubNickInit(intptr_t param) {
	m_pAreaSubNick = DYNAMIC_CAST(FrViewer, param);

	if (m_pAreaSubNick) {
		WRect rect = m_pAreaSubNick->GetRect();

		m_pAreaSubNick->SetRect(WRect(rect.x, rect.y, rect.w + 55.0f, rect.h));
	}
}

// offlinetradedlg.cpp:556
void COfflineTradeDlg::OnAreaSubNickOwnerDraw(int) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	pGDI->SetTextColor(0xFFFFFFFF, 0xFFFFFFFF);

	WRect rect = m_pAreaSubNick->GetRect();

	const WPoint& mouse = g_pFresh->GetManager()->GetMousePos();

	if (mouse.x >= rect.x && mouse.x <= rect.x + rect.w && mouse.y >= rect.y && mouse.y <= rect.y + rect.h) {
		pGDI->SetTextColor(0xFFFFFFFF, 0xFF000000);
		pGDI->SetTextStyle(2);
	} else {
		pGDI->SetTextStyle(0);
	}

	pGDI->Print(WPoint(rect.x + 10.0f, rect.y), 0, "\xb4\xd0\xb3\xd7\xc0\xd3");
}

// offlinetradedlg.cpp:582
void COfflineTradeDlg::OnAreaSubNickLBtnUP(void) {
	m_searchType = 4;
	m_searchOrder = 0;
	m_currentPage = 1;
	if (SetSearchString("")) {
		RequestMaketList();
	}
}

// offlinetradedlg.cpp:592
void COfflineTradeDlg::OnAreaSubPriceInit(intptr_t param) {
	m_pAreaSubPrice = DYNAMIC_CAST(FrViewer, param);

	if (m_pAreaSubPrice) {
		WRect rect = m_pAreaSubPrice->GetRect();

		m_pAreaSubPrice->SetRect(WRect(rect.x, rect.y, rect.w + 35.0f, rect.h));
	}
}

// offlinetradedlg.cpp:603
void COfflineTradeDlg::OnAreaSubPriceOwnerDraw(int) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	pGDI->SetTextColor(0xFFFFFFFF, 0xFFFFFFFF);

	WRect rect = m_pAreaSubPrice->GetRect();

	const WPoint& mouse = g_pFresh->GetManager()->GetMousePos();

	if (mouse.x >= rect.x && mouse.x <= rect.x + rect.w && mouse.y >= rect.y && mouse.y <= rect.y + rect.h) {
		pGDI->SetTextColor(0xFFFFFFFF, 0xFF000000);
		pGDI->SetTextStyle(2);
	} else {
		pGDI->SetTextStyle(0);
	}

	pGDI->Print(WPoint(rect.x + 10.0f, rect.y), 0, "\xb0\xa1\xb0\xdd");
}

// offlinetradedlg.cpp:632
void COfflineTradeDlg::OnAreaSubPriceLBtnUP(void) {
	m_searchType = 3;
	m_searchOrder = 0;
	m_currentPage = 1;
	if (SetSearchString("")) {
		RequestMaketList();
	}
}

// offlinetradedlg.cpp:643
void COfflineTradeDlg::OnAreaSubItemNameInit(intptr_t param) {
	m_pAreaSubItemName = DYNAMIC_CAST(FrViewer, param);

	if (m_pAreaSubItemName) {
		WRect rect = m_pAreaSubItemName->GetRect();

		m_pAreaSubItemName->SetRect(WRect(rect.x, rect.y, rect.w + 75.0f, rect.h));
	}
}

// offlinetradedlg.cpp:655
void COfflineTradeDlg::OnAreaSubItemNameOwnerDraw(int) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	pGDI->SetTextColor(0xFFFFFFFF, 0xFFFFFFFF);

	WRect rect = m_pAreaSubItemName->GetRect();

	const WPoint& mouse = g_pFresh->GetManager()->GetMousePos();

	if (mouse.x >= rect.x && mouse.x <= rect.x + rect.w && mouse.y >= rect.y && mouse.y <= rect.y + rect.h) {
		pGDI->SetTextColor(0xFFFFFFFF, 0xFF000000);
		pGDI->SetTextStyle(2);
	} else {
		pGDI->SetTextStyle(0);
	}

	pGDI->Print(WPoint(rect.x + 10.0f, rect.y), 0, "\xbe\xc6\xc0\xcc\xc5\xdb\xc0\xcc\xb8\xa7");
}

// offlinetradedlg.cpp:682
void COfflineTradeDlg::OnAreaSubItemNameLBtnUP(void) {
	m_searchType = 1;
	m_searchOrder = 0;
	m_currentPage = 1;
	if (SetSearchString("")) {
		RequestMaketList();
	}
}

// offlinetradedlg.cpp:691
void COfflineTradeDlg::OnMaketListInit(intptr_t param) {
	m_pMaketList = DYNAMIC_CAST(FrListBox, param);
}

// offlinetradedlg.cpp:706
void COfflineTradeDlg::SetListItem(void) {
	char text[32];  // unused: keeps the original's /GS cookie

	std::list<sSaleItem>* pList = CSharedDoc::Instance()->GetMaketList();

	if (m_pMaketList) {
		m_pMaketList->ClearItem();

		for (std::list<sSaleItem>::iterator it = pList->begin(); it != pList->end(); ++it) {
			m_pMaketList->AddItem(&(*it));
		}
	}
}

// offlinetradedlg.cpp:835
void COfflineTradeDlg::OnItemSearchInit(intptr_t param) {
	m_pItemSearchEdit = DYNAMIC_CAST(FrEdit, param);

	if (m_pItemSearchEdit) {
		m_pItemSearchEdit->SetFontColor(0xFFD1D1D0);
		m_pItemSearchEdit->ClearLine();
		m_pItemSearchEdit->AddLine("\xb9\xb0\xb0\xc7\xc0\xbb \xb0\xf1\xb6\xf3\xba\xb8\xb7\xc5", 0, false);
		m_pItemSearchEdit->SetKeyFocus(true);
	}
}

// offlinetradedlg.cpp:849
bool COfflineTradeDlg::OnItemSearchEnterKey(int) {
	OnAreaSubItemNameLBtnUP();
	return true;
}

// offlinetradedlg.cpp:857
void COfflineTradeDlg::OnItemSearchLBtnUp(void) {
	std::string text = m_pItemSearchEdit->GetLine(1, false);
	std::string empty;
	empty.clear();

	const char* pText = text.c_str();

	if (stricmp("\xb9\xb0\xb0\xc7\xc0\xbb \xb0\xf1\xb6\xf3\xba\xb8\xb7\xc5", pText) == 0) {
		m_pItemSearchEdit->SetFontColor(0xFF000000);
		m_pItemSearchEdit->ClearLine();

		m_pItemSearchEdit->AddLine("", 0, false);
		m_pItemSearchEdit->SetKeyFocus(true);
	}
}

// offlinetradedlg.cpp:877
void COfflineTradeDlg::OnBtnMainSearchInit(intptr_t param) {
	m_pBtnMainSearch = DYNAMIC_CAST(FrButton, param);
}

// offlinetradedlg.cpp:885
void COfflineTradeDlg::OnBtnMainSearchUp(void) {
	m_searchType = 1;
	m_searchOrder = 0;
	m_currentPage = 1;
	if (SetSearchString("")) {
		RequestMaketList();
	}
}

// offlinetradedlg.cpp:911
void COfflineTradeDlg::OnEditMaxPageInit(intptr_t param) {
	m_pMaxPageEdit = DYNAMIC_CAST(FrEdit, param);

	if (m_pMaxPageEdit) {
		m_pMaxPageEdit->AddLine(MakeStr("%d", m_maxPage), 0, false);
	}
}

// offlinetradedlg.cpp:922
void COfflineTradeDlg::OnEditCurrentPageInit(intptr_t param) {
	m_pCurrentPageEdit = DYNAMIC_CAST(FrEdit, param);

	if (m_pCurrentPageEdit) {
		m_pCurrentPageEdit->AddLine(MakeStr("%d", m_currentPage), 0, false);
	}
}

// offlinetradedlg.cpp:932
bool COfflineTradeDlg::OnEditCurrentPageEnterKey(int) {
	return true;
}

// offlinetradedlg.cpp:938
void COfflineTradeDlg::SetPageData(int currentPage, int maxPage) {
	if (currentPage != -1) {
		m_currentPage = currentPage;
		m_pBtnDetail->SetVisible(false);
	}

	m_maxPage = maxPage;

	if (m_currentPage < 1) {
		m_currentPage = 1;
	}
	if (m_currentPage > m_maxPage) {
		m_currentPage = m_maxPage;
	}

	if (m_pCurrentPageEdit) {
		m_pCurrentPageEdit->ClearLine();
		m_pCurrentPageEdit->AddLine(MakeStr("%d", m_currentPage), 0, false);
	}

	if (m_pMaxPageEdit) {
		m_pMaxPageEdit->ClearLine();
		m_pMaxPageEdit->AddLine(MakeStr("%d", m_maxPage), 0, false);
	}
}

// offlinetradedlg.cpp:969
void COfflineTradeDlg::RequestMaketList(void) {
	const char* pText = m_searchString.c_str();

	if (stricmp("\xb9\xb0\xb0\xc7\xc0\xbb \xb0\xf1\xb6\xf3\xba\xb8\xb7\xc5", pText) == 0) {
		m_searchString.clear();
	}

	if (m_searchString.size() != 0) {
		if (IsLocalContent((localContentType_t)0x66)) {
			WSendPacket packet((enumClientPacket)0xC9);
			packet.Encode1((unsigned char)m_searchType);
			packet.Encode1((unsigned char)m_searchOrder);
			packet.EncodeStr(m_searchString);
			packet.Encode2((unsigned short)m_currentPage);
			packet.Send((eSendTo)0);

			CTaskManager* pTaskManager = CTaskManager::Instance();
			(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));

			m_pBtnDetail->SetVisible(true);
		}
	} else {
		if (IsLocalContent((localContentType_t)0x66)) {
			WSendPacket packet((enumClientPacket)0xCA);
			packet.Encode2((unsigned short)m_currentPage);
			packet.Send((eSendTo)0);

			CTaskManager* pTaskManager = CTaskManager::Instance();
			(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
		}
	}
}

// offlinetradedlg.cpp:1021
void COfflineTradeDlg::OnBtnPrevInit(intptr_t param) {
	m_pBtnPrev = DYNAMIC_CAST(FrButton, param);
}

// offlinetradedlg.cpp:1029
void COfflineTradeDlg::OnBtnPrevBtnUp(void) {
	SetPageData(--m_currentPage, m_maxPage);
	RequestMaketList();
}

// offlinetradedlg.cpp:1048
void COfflineTradeDlg::OnBtnNextInit(intptr_t param) {
	m_pBtnNext = DYNAMIC_CAST(FrButton, param);
}

// offlinetradedlg.cpp:1057
void COfflineTradeDlg::OnBtnNextBtnUp(void) {
	SetPageData(++m_currentPage, m_maxPage);
	RequestMaketList();
}

// offlinetradedlg.cpp:1076
void COfflineTradeDlg::OnBtnGoPageInit(intptr_t param) {
	m_pBtnGoPage = DYNAMIC_CAST(FrButton, param);
}

// offlinetradedlg.cpp:1085
void COfflineTradeDlg::OnBtnGoPageBtnUp(void) {
	m_currentPage = atoi(m_pCurrentPageEdit->GetLine(1, false));

	SetPageData(m_currentPage, m_maxPage);
	RequestMaketList();
}

// offlinetradedlg.cpp:1104
void COfflineTradeDlg::OnBtnRefreshInit(intptr_t param) {
	m_pBtnRefresh = DYNAMIC_CAST(FrButton, param);
}

// offlinetradedlg.cpp:1112
void COfflineTradeDlg::OnBtnRefreshBtnUp(void) {
	SetPageData(m_currentPage = 1, m_maxPage);

	m_searchType = 1;
	m_searchOrder = 0;
	m_searchString.clear();

	SetSearchString("\xb9\xb0\xb0\xc7\xc0\xbb \xb0\xf1\xb6\xf3\xba\xb8\xb7\xc5");

	m_myRoomState = false;
	m_canVisitMyRoom = 0;
	m_pEditMsg->SetVisible(true);
	m_pEditMsg->AddText("* \xc7\xf6\xc0\xe7 \xbb\xf3\xc5\xc2\xbf\xa1\xbc\xad\xb4\xc2 \xb4\xd9\xb8\xa5 "
		"\xbb\xe7\xb6\xf7\xc0\xc7 \xb8\xb6\xc0\xcc\xb7\xeb\xc0\xbb \xb9\xe6\xb9\xae "
		"\xc7\xcf\xbd\xc7\xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", false, true);

	if (IsLocalContent((localContentType_t)0x66)) {
		WSendPacket packet((enumClientPacket)0xCA);
		packet.Encode2((unsigned short)m_currentPage);
		packet.Send((eSendTo)0);

		m_pItemSearchEdit->SetFontColor(0xFFD1D1D0);
		m_pItemSearchEdit->ClearLine();
		m_pItemSearchEdit->AddLine("\xb9\xb0\xb0\xc7\xc0\xbb \xb0\xf1\xb6\xf3\xba\xb8\xb7\xc5", 0, false);
		m_pItemSearchEdit->SetKeyFocus(true);

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
	}

	m_pBtnDetail->SetVisible(false);
}

// offlinetradedlg.cpp:1153
void COfflineTradeDlg::OnBtnDetailInit(intptr_t param) {
	m_pBtnDetail = DYNAMIC_CAST(FrButton, param);

	if (m_pBtnDetail) {
		m_pBtnDetail->SetVisible(false);
	}
}

// offlinetradedlg.cpp:1163
void COfflineTradeDlg::OnBtnDetailBtnUp(void) {
	WRect rect = m_pBtnDetail->GetRect();

	if (!m_pStandingInLine) {
		m_pStandingInLine = CreateForm<CStandingInLine>(g_pFresh->GetManager(), this, "arraydlg", 0);
		if (m_pStandingInLine) {
			m_pStandingInLine->Open((FRESH_RESULT_CALLBACK)&COfflineTradeDlg::OnStandingInLineDlgResult,
				WPoint(rect.x + 10.0f, rect.y), 1);
		}
	}
}

// offlinetradedlg.cpp:1180
void COfflineTradeDlg::OnEditMsgInit(intptr_t param) {
	m_pEditMsg = DYNAMIC_CAST(FrEdit, param);

	if (m_pEditMsg) {
		m_pEditMsg->SetVisible(false);
	}
}

// offlinetradedlg.cpp:1191
void COfflineTradeDlg::OnEditMsgLBtnUp(void) {
}

// offlinetradedlg.cpp:1198
void COfflineTradeDlg::OnToolTipInit(intptr_t param) {
	m_pToolTip = DYNAMIC_CAST(FrArea, param);
}

// offlinetradedlg.cpp:1204
void COfflineTradeDlg::OnToolTipOnwerDraw(int) {
	CIconToolTip::GetInstance()->Render();
}

// offlinetradedlg.cpp:1229
bool COfflineTradeDlg::SetSearchString(std::string searchString) {
	if (searchString.size() == 0) {
		std::string text;

		if (m_pItemSearchEdit) {
			text = m_pItemSearchEdit->GetLine(1, false);
		}

		if (text.size() == 0) {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
			pForm->SetMessage("\xb0\xcb\xbb\xf6\xc7\xcf\xb7\xc1\xb4\xc2 \xb4\xdc\xbe\xee\xb0\xa1 "
				"\xbf\xc3\xb9\xd9\xb8\xa3\xc1\xf6 \xbe\xca\xb0\xc5\xb3\xaa \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9. "
				"\xb4\xd9\xbd\xc3 \xc0\xd4\xb7\xc2\xc7\xcf\xbc\xbc\xbf\xe4", false);
			pForm->Open(0, 3);

			m_searchString.clear();
			return false;
		}

		SetSearchString(text);
	} else {
		m_searchString = searchString;
	}

	return true;
}

// offlinetradedlg.cpp:1265
bool COfflineTradeDlg::OnStandingInLineDlgResult(int ret, FrForm* pForm) {
	int selectNum = 0;

	if (ret == 1) {
		if (m_pStandingInLine) {
			selectNum = m_pStandingInLine->GetSelectNum();
		}
	}

	switch (selectNum) {
	case 0: m_searchOrder = 0; break;
	case 1: m_searchOrder = 10; break;
	case 2: m_searchOrder = 11; break;
	case 3: m_searchOrder = 12; break;
	case 4: m_searchOrder = 13; break;
	case 5: m_searchOrder = 14; break;
	case 6: m_searchOrder = 15; break;
	case 7: m_searchOrder = 16; break;
	case 8: m_searchOrder = 17; break;
	case 9: m_searchOrder = 18; break;
	case 11: m_searchOrder = 21; break;
	case 12: m_searchOrder = 23; break;
	case 13: m_searchOrder = 22; break;
	case 14: m_searchOrder = 24; break;
	case 15: m_searchOrder = 19; break;
	}

	m_currentPage = 1;
	RequestMaketList();

	m_pStandingInLine = 0;

	return true;
}

// offlinetradedlg.cpp:1313 (run-time type and factory registration of CBuyingSlotDlg)
const WRTTI CBuyingSlotDlg::m_RTTI("CBuyingSlotDlg", &FrForm::m_RTTI);

IObject* CBuyingSlotDlgMakeInstance(void) {
	return new CBuyingSlotDlg;
}

struct __sCBuyingSlotDlg {
	__sCBuyingSlotDlg(void) {
		ObjectFactory().AddObjectFunctor(CBuyingSlotDlgMakeInstance, "CBuyingSlotDlg");
	}
};
static __sCBuyingSlotDlg s_registerCBuyingSlotDlg;

// offlinetradedlg.cpp:1314
const sFRESH_MSGMAP* CBuyingSlotDlg::GetMessageMap(void) const {
	return &CBuyingSlotDlg::_MsgMap;
}

sFRESH_MSGMAP CBuyingSlotDlg::_MsgMap = { &FrForm::_MsgMap, &CBuyingSlotDlg::_MsgEntries[0] };

sFRESH_ENTRY CBuyingSlotDlg::_MsgEntries[] = {
	// offlinetradedlg.cpp:1316
	{ "days", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CBuyingSlotDlg::OnDaysInit },
	// offlinetradedlg.cpp:1317
	{ "days", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CBuyingSlotDlg::OnDaysBtnDown },

	// offlinetradedlg.cpp:1319
	{ "my_money", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CBuyingSlotDlg::OnMyMoneyInit },
	// offlinetradedlg.cpp:1320
	{ "money_left", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CBuyingSlotDlg::OnMoneyLeftInit },
	// offlinetradedlg.cpp:1321
	{ "price", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CBuyingSlotDlg::OnPriceInit },

	// offlinetradedlg.cpp:1323
	{ "checkout", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CBuyingSlotDlg::OnCheckOutInit },
	// offlinetradedlg.cpp:1324
	{ "checkout", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CBuyingSlotDlg::OnCheckOutBtnDown },

	// offlinetradedlg.cpp:1326
	{ "slot_price", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CBuyingSlotDlg::OnSlotPriceInit },

	// offlinetradedlg.cpp:1328
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// offlinetradedlg.cpp:1333
struct sdays_t {
	const char* const m_name;   // 0x0
	const int m_days;           // 0x4
};

sdays_t sDay[] = {
	{ "1\xc0\xcf", 1 },
	{ "7\xc0\xcf", 7 },
	{ "15\xc0\xcf", 15 },
	{ "30\xc0\xcf", 30 },
	{ "365\xc0\xcf", 365 },
	{ "\xb9\xab\xc1\xa6\xc7\xd1", 0 },
};

// offlinetradedlg.cpp:1342
CBuyingSlotDlg::CBuyingSlotDlg(void) {
	m_pDays = 0;
	m_pMyMoney = 0;
	m_pMoneyLeft = 0;
	m_pPrice = 0;
	m_pCheckOut = 0;
	m_pSlotPrice = 0;

	m_price = 0;
}

// offlinetradedlg.cpp:1358
CBuyingSlotDlg::~CBuyingSlotDlg(void) {
}

// offlinetradedlg.cpp:1362
void CBuyingSlotDlg::OnDaysInit(intptr_t param) {
	m_pDays = DYNAMIC_CAST(FrComboBox, param);

	if (m_pDays) {
		m_pDays->ClearList();

		IFF_STRUCT::sFurniture* pFurniture = ItemManager()->FindFurniture(0x4800601E);

		for (int i = 0; i < 4; i++) {
			if (pFurniture->m_dayPrices[i] != 0) {
				m_pDays->AddString(sDay[i].m_name);
			}
		}

		m_pDays->SetLine(1, sDay[0].m_name, 0, false, 0);
		m_price = pFurniture->m_dayPrices[0];
		m_pSlotPrice->ClearLine();
		m_pSlotPrice->AddText(MakeStr("%d \xc6\xce", m_price), false, true);
	}
}

// offlinetradedlg.cpp:1387
void CBuyingSlotDlg::OnDaysBtnDown(void) {
	IFF_STRUCT::sFurniture* pFurniture = ItemManager()->FindFurniture(0x4800601E);
	const char* pText = m_pDays->GetLine(1, false);

	for (int i = 0; i < 5; i++) {
		if (stricmp(sDay[i].m_name, pText) == 0) {
			m_price = pFurniture->m_dayPrices[i];
			m_pSlotPrice->ClearLine();
			m_pSlotPrice->AddText(MakeStr("%d \xc6\xce", m_price), false, true);
		}
	}

	RecalculatePang();
}

// offlinetradedlg.cpp:1405
void CBuyingSlotDlg::OnPriceInit(intptr_t param) {
	m_pPrice = DYNAMIC_CAST(FrEdit, param);

	if (m_pPrice) {
		char buf[32];

		MakePriceStrA(buf, m_price);
		strcat(buf, "\xc6\xce");
		m_pPrice->SetLine(1, buf, 0, false, 0);
	}
}

// offlinetradedlg.cpp:1420
void CBuyingSlotDlg::OnMyMoneyInit(intptr_t param) {
	m_pMyMoney = DYNAMIC_CAST(FrEdit, param);

	if (m_pMyMoney) {
		char buf[32];

		MakePriceStrA(buf, CSharedDoc::Instance()->m_myPang);
		strcat(buf, "\xc6\xce");
		m_pMyMoney->SetLine(1, buf, 0, false, 0);
	}
}

// offlinetradedlg.cpp:1434
void CBuyingSlotDlg::OnMoneyLeftInit(intptr_t param) {
	m_pMoneyLeft = DYNAMIC_CAST(FrEdit, param);

	if (m_pMoneyLeft) {
		char buf[32];

		__int64 left = CSharedDoc::Instance()->m_myPang - m_price;
		if (left < 0) {
			MakePriceStrA(buf, m_price - CSharedDoc::Instance()->m_myPang);
			strcat(buf, "\xc6\xce \xba\xce\xc1\xb7");
			m_pMoneyLeft->SetLine(1, buf, 0xFFFF0000, false, 0);
		} else {
			MakePriceStrA(buf, left);
			strcat(buf, "\xc6\xce");
			m_pMoneyLeft->SetLine(1, buf, 0, false, 0);
		}
	}
}

// offlinetradedlg.cpp:1457
void CBuyingSlotDlg::RecalculatePang(void) {
	char buf[32];

	if (m_pPrice) {
		MakePriceStrA(buf, m_price);
		strcat(buf, "\xc6\xce");
		m_pPrice->SetLine(1, buf, 0, false, 0);
	}

	if (m_pMyMoney) {
		MakePriceStrA(buf, CSharedDoc::Instance()->m_myPang);
		strcat(buf, "\xc6\xce");
		m_pMyMoney->SetLine(1, buf, 0, false, 0);
	}

	if (m_pMoneyLeft) {
		__int64 left = CSharedDoc::Instance()->m_myPang - m_price;
		if (left < 0) {
			MakePriceStrA(buf, m_price - CSharedDoc::Instance()->m_myPang);
			strcat(buf, "\xc6\xce \xba\xce\xc1\xb7");
			m_pMoneyLeft->SetLine(1, buf, 0xFFFF0000, false, 0);
		} else {
			MakePriceStrA(buf, left);
			strcat(buf, "\xc6\xce");
			m_pMoneyLeft->SetLine(1, buf, 0, false, 0);
		}
	}
}

// offlinetradedlg.cpp:1529
void CBuyingSlotDlg::OnCheckOutInit(intptr_t param) {
	m_pCheckOut = DYNAMIC_CAST(FrButton, param);
}

// offlinetradedlg.cpp:1597
void CBuyingSlotDlg::OnSlotPriceInit(intptr_t param) {
	m_pSlotPrice = DYNAMIC_CAST(FrEdit, param);

	if (m_pSlotPrice) {
		m_pSlotPrice->ClearLine();
		m_pSlotPrice->AddText(MakeStr("%d \xc6\xce", m_price), false, true);
	}
}

// offlinetradedlg.cpp:1657 (run-time type and factory registration of COfflineShopEditDlg)
const WRTTI COfflineShopEditDlg::m_RTTI("COfflineShopEditDlg", &FrForm::m_RTTI);

IObject* COfflineShopEditDlgMakeInstance(void) {
	return new COfflineShopEditDlg;
}

struct __sCOfflineShopEditDlg {
	__sCOfflineShopEditDlg(void) {
		ObjectFactory().AddObjectFunctor(COfflineShopEditDlgMakeInstance, "COfflineShopEditDlg");
	}
};
static __sCOfflineShopEditDlg s_registerCOfflineShopEditDlg;

// offlinetradedlg.cpp:1659
const sFRESH_MSGMAP* COfflineShopEditDlg::GetMessageMap(void) const {
	return &COfflineShopEditDlg::_MsgMap;
}

// offlinetradedlg.cpp:1748
COfflineShopEditDlg::~COfflineShopEditDlg(void) {
	if (m_closing == 0) {
		if (IsLocalContent((localContentType_t)102)) {
		}
	}
}

// offlinetradedlg.cpp:1770
void COfflineShopEditDlg::OnAreaBg01Init(int param) {
	m_pAreaBg01 = DYNAMIC_CAST(FrArea, param);

	if (m_pAreaBg01) {
		m_pAreaBg01->SetBgImg("rmr_trade_edit_back_01");
	}
}

// offlinetradedlg.cpp:1787
void COfflineShopEditDlg::OnAreaBg02Init(int param) {
	m_pAreaBg02 = DYNAMIC_CAST(FrArea, param);

	if (m_pAreaBg02) {
		m_pAreaBg02->SetBgImg("rmr_trade_edit_back_02");
	}
}

// offlinetradedlg.cpp:1804
void COfflineShopEditDlg::OnAreaBg03Init(int param) {
	m_pAreaBg03 = DYNAMIC_CAST(FrArea, param);

	if (m_pAreaBg03) {
		m_pAreaBg03->SetBgImg("rmr_trade_edit_back_03");
	}
}

// offlinetradedlg.cpp:1822
void COfflineShopEditDlg::OnAreaBg04Init(int param) {
	m_pAreaBg04 = DYNAMIC_CAST(FrArea, param);

	if (m_pAreaBg04) {
		m_pAreaBg04->SetBgImg("rmr_trade_edit_back_04");
	}
}

// offlinetradedlg.cpp:1839
void COfflineShopEditDlg::OnAreaBg05Init(int param) {
	m_pAreaBg05 = DYNAMIC_CAST(FrArea, param);

	if (m_pAreaBg05) {
		m_pAreaBg05->SetVisible(false);
	}
}

// offlinetradedlg.cpp:1855
void COfflineShopEditDlg::OnStallSlotNumInit(int param) {
	m_pStallSlotNum = DYNAMIC_CAST(FrEdit, param);
}

// offlinetradedlg.cpp:1864
void COfflineShopEditDlg::OnSlotNumInit(int param) {
	m_pSlotNum = DYNAMIC_CAST(FrEdit, param);
}

// offlinetradedlg.cpp:1873
void COfflineShopEditDlg::OnTradeItemListInit(int param) {
	m_pShopItemList = DYNAMIC_CAST(FrListBox, param);

	if (IsLocalContent((localContentType_t)0x66) && CSharedDoc::Instance()->GetTradeMode() == 1) {
		sStall* pStall = CSharedDoc::Instance()->GetStallItem();

		for (int i = 0; i < pStall->open; i++) {
			m_pShopItemList->AddItem(&pStall->slots[i]);
		}
	}
}

// offlinetradedlg.cpp:1889
void COfflineShopEditDlg::OnTradeItemListLClick(void) {
	FrListItem* pItem = m_pShopItemList->GetItemUnderCursor();

	if (pItem == 0 || pItem->pData == 0) {
		return;
	}

	pItem->selected = !pItem->selected;
}

// offlinetradedlg.cpp:2044
void COfflineShopEditDlg::OnVisitorInit(int param) {
	m_pVisitor = DYNAMIC_CAST(FrEdit, param);

	if (IsLocalContent((localContentType_t)0x66) && CSharedDoc::Instance()->GetTradeMode() == 1) {
		m_pVisitor->SetVisible(false);
	}
}

// offlinetradedlg.cpp:2062
void COfflineShopEditDlg::OnOpenBnInit(int param) {
	m_pShopOpenBn = DYNAMIC_CAST(FrButton, param);

	if (m_pShopOpenBn) {
		m_pShopOpenBn->SetVisible(false);
	}
}

// offlinetradedlg.cpp:2260
void COfflineShopEditDlg::SetSelectData(unsigned long first, unsigned long second) {
	m_selectData[0] = first;
	m_selectData[1] = second;
}

// offlinetradedlg.cpp:2266
void COfflineShopEditDlg::OnCloseBnInit(intptr_t param) {
	m_pShopCloseBn = DYNAMIC_CAST(FrButton, param);

	if (m_pShopCloseBn) {
		m_pShopCloseBn->SetVisible(false);
	}
}

// offlinetradedlg.cpp:2298
void COfflineShopEditDlg::OnExitBnUp(void) {
	m_closing = 1;

	COfflineShopDlg* pDlg = CreateForm<COfflineShopDlg>(g_pFresh->GetManager(), this, "offlinetradedlg", 0);

	if (pDlg) {
		pDlg->Open((FRESH_RESULT_CALLBACK)0, 0xC0);
		pDlg->OpenTradeShop(CSharedDoc::Instance()->m_myRoomOwnerUID);
		pDlg->SetListTradeItem(false);
		pDlg->SetFadeout(false);
	}

	OnFreshCancel();
}

// offlinetradedlg.cpp:2317
bool COfflineShopEditDlg::OnInit(void) {
	FrForm::EnableDrag(false);

	SetListEnableTradingMyItem();
	SetListTradingItem();
	SetListMyItem();
	SetListTradeItem();
	SetTradeMyItem(0);
	SetControlsTradeMyItem(0, false);

	return true;
}

// offlinetradedlg.cpp:2336
void COfflineShopEditDlg::InitAllUI(void) {
	FrForm::EnableDrag(false);

	SetListEnableTradingMyItem();
	SetListTradingItem();
	SetListMyItem();
	SetListTradeItem();
	SetTradeMyItem(0);
	SetControlsTradeMyItem(0, false);
}

// offlinetradedlg.cpp:2349
void COfflineShopEditDlg::OnProc(const float dt) {
	FrForm::OnProc(dt);
	CIconToolTip::GetInstance()->ClearToolTip();
}

// offlinetradedlg.cpp:2356
void COfflineShopEditDlg::SetListTradeItem(void) {
	FrTradeEditDlg::SetListTradeItem();

	if (m_pShopItemList) {
		m_pShopItemList->ClearItem();
		m_usedSlotCount = 0;

		sStall* pStall = CSharedDoc::Instance()->GetStallItem();
		if (!pStall) {
			std::list<sStall>::iterator it = CSharedDoc::Instance()->GetListStallItem()->begin();
			pStall = &(*it);
		}
		if (!pStall) {
			return;
		}

		for (int i = 0; i < pStall->open; i++) {
			if (pStall->slots[i].used) {
				m_pShopItemList->AddItem(&pStall->slots[i]);
			}
			if (pStall->slots[i].item.typeId != 0) {
				m_usedSlotCount++;
			}
		}

		if (m_pStallSlotNum) {
			m_pStallSlotNum->ClearLine();
			m_pStallSlotNum->AddLine(MakeStr("%d", pStall->open), 0, false);
		}

		if (m_pSlotNum) {
			m_pSlotNum->ClearLine();
			m_pSlotNum->AddLine(MakeStr("%2d/%2d", m_usedSlotCount, pStall->visible), 0, false);
		}
	}
}

// offlinetradedlg.cpp:2410
void COfflineShopEditDlg::SetSlotData(void) {
	m_usedSlotCount = 0;

	sStall* pStall = CSharedDoc::Instance()->GetStallItem();
	if (!pStall) {
		std::list<sStall>::iterator it = CSharedDoc::Instance()->GetListStallItem()->begin();
		pStall = &(*it);
	}
	if (!pStall) {
		return;
	}

	for (int i = 0; i < pStall->open; i++) {
		if (pStall->slots[i].item.typeId != 0) {
			m_usedSlotCount++;
		}
	}

	if (m_pStallSlotNum) {
		m_pStallSlotNum->ClearLine();
		m_pStallSlotNum->AddLine(MakeStr("%d", pStall->open), 0, false);
	}

	if (m_pSlotNum) {
		m_pSlotNum->ClearLine();
		m_pSlotNum->AddLine(MakeStr("%2d/%2d", m_usedSlotCount, pStall->visible), 0, false);
	}
}

// offlinetradedlg.cpp:2448
bool COfflineShopEditDlg::IsCheckCardPats(unsigned long partsId, unsigned long partsTypeId) {
	std::list<sSCardAvilityPeriodInfo>& cards = CSharedDoc::Instance()->m_cardPeriodList[0];
	std::list<sSCardAvilityPeriodInfo>::iterator it;
	for (it = cards.begin(); it != cards.end(); ++it) {
		if (it->partsTypeId == partsTypeId && it->partsId == partsId) {
			break;
		}
	}

	return it == cards.end() ? true : false;
}

// offlinetradedlg.cpp:2468
bool COfflineShopEditDlg::OnOfflineEquitCancelDlgResult(int ret, FrForm* pForm) {
	if (ret == 1) {
		FrListItem* pItem = m_pShopItemList->GetItemUnderCursor();

		if (pItem) {
			if (IsLocalContent((localContentType_t)0x66)) {
				WSendPacket packet((enumClientPacket)0xC8);
				packet.Encode4(CSharedDoc::Instance()->GetStallItem()->ownerUID);
				packet.Encode2(1);
				packet.Encode1(pItem->idx + 1);
				packet.Send((eSendTo)0);

				CTaskManager* pTaskManager = CTaskManager::Instance();
				(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
			}
		}
	}

	return true;
}

// offlinetradedlg.cpp:2573
void COfflineShopEditDlg::OnToolTipInit(intptr_t param) {
	m_pToolTip = DYNAMIC_CAST(FrArea, param);
}

// offlinetradedlg.cpp:2579
void COfflineShopEditDlg::OnToolTipOnwerDraw(int) {
	CIconToolTip::GetInstance()->Render();
}

// offlinetradedlg.cpp:2604
void COfflineShopEditDlg::DelListMyItem(sTradeItem* pItem, unsigned long, unsigned char) {
	if (pItem->typeId == 0) {
		SetListMyItem();
	} else {
		FrTradeEditDlg::DelListMyItem(pItem);
	}

	SetTradeMyItem(0);
	SetControlsTradeMyItem(0, false);

	SetListTradeItem();

	if (m_pMyItemList) {
		m_pMyItemList->UnselectAllItem();
	}
}

// offlinetradedlg.cpp:2641
bool COfflineShopEditDlg::OnOfflineTradeBuyDlgResult(int ret, FrForm* pForm) {
	if (ret == 1) {
		FrOfflineTradeFinishEditDlg* pDlg = CreateForm<FrOfflineTradeFinishEditDlg>(g_pFresh->GetManager(), this, "offlinetradefinisheditdlg", 0);

		if (pDlg) {
			pDlg->Open((FRESH_RESULT_CALLBACK)0, 0xC0);
			pDlg->SetFadeout(false);
			pDlg->SetListTradeItem(&m_tradeItemList);
			pDlg->SetTradeUserId();
			pDlg->SetTradeTitle(m_tradeTitle);
		}
	}

	return true;
}

// offlinetradedlg.cpp:2666 (run-time type and factory registration of COfflineShopDlg)
const WRTTI COfflineShopDlg::m_RTTI("COfflineShopDlg", &FrForm::m_RTTI);

IObject* COfflineShopDlgMakeInstance(void) {
	return new COfflineShopDlg;
}

struct __sCOfflineShopDlg {
	__sCOfflineShopDlg(void) {
		ObjectFactory().AddObjectFunctor(COfflineShopDlgMakeInstance, "COfflineShopDlg");
	}
};
static __sCOfflineShopDlg s_registerCOfflineShopDlg;

// offlinetradedlg.cpp:2668
const sFRESH_MSGMAP* COfflineShopDlg::GetMessageMap(void) const {
	return &COfflineShopDlg::_MsgMap;
}

// offlinetradedlg.cpp:2762
COfflineShopDlg::~COfflineShopDlg(void) {
	if (m_closing == 0) {
		if (IsLocalContent((localContentType_t)102)) {
		}
	}
}

// offlinetradedlg.cpp:2802
void COfflineShopDlg::OnVisitorBackOwnerDraw(void) {
}

// offlinetradedlg.cpp:2807
void COfflineShopDlg::OnVisitorInit(int param) {
	m_pVisitor = DYNAMIC_CAST(FrEdit, param);

	if (IsLocalContent((localContentType_t)0x66) && CSharedDoc::Instance()->GetTradeMode() == 1) {
		m_pVisitor->SetVisible(false);
	}
}

// offlinetradedlg.cpp:2826
void COfflineShopDlg::OnCloseBnUp(void) {
	m_closing = 1;

	if (IsLocalContent((localContentType_t)0x66)) {
		CTaskManager* pTaskManager = CTaskManager::Instance();
		(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0x22F, 0, 0, 0, 0, 0));
	}

	FrForm::OnOK();
}

// offlinetradedlg.cpp:2847
void COfflineShopDlg::OnVisitorBackInit(int param) {
	m_pVisitorBack = DYNAMIC_CAST(FrArea, param);

	if (m_pVisitorBack) {
		m_pVisitorBack->SetVisible(false);
	}
}

// offlinetradedlg.cpp:3021
void COfflineShopDlg::OnToolTipInit(intptr_t param) {
	m_pToolTip = DYNAMIC_CAST(FrArea, param);
}

// offlinetradedlg.cpp:3027
void COfflineShopDlg::OnToolTipOnwerDraw(int) {
	CIconToolTip::GetInstance()->Render();
}

// offlinetradedlg.cpp:3137
bool COfflineShopDlg::OnOfflineShopEditDlgCreateResult(int ret, FrForm* pForm) {
	if (ret == 1) {
		if (m_pSelectedSlotItem && m_pSelectedSlotItem->pData) {
			sStallSlot* pSlot = (sStallSlot*)m_pSelectedSlotItem->pData;

			m_closing = 1;
			COfflineShopEditDlg* pDlg = CreateForm<COfflineShopEditDlg>(g_pFresh->GetManager(), this, "tradeeditdlg", 0);

			if (pDlg) {
				pDlg->Open((FRESH_RESULT_CALLBACK)0, 0xC0);
				pDlg->SetSelectData(pSlot->index, pSlot->used);
				pDlg->SetFadeout(false);

				m_pSelectedSlotItem = 0;
				OnFreshOkay();
			}
		} else {
			FrForm* pNotify = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
			pNotify->SetMessage("\xbc\xb1\xc5\xc3\xc7\xd1 \xbd\xbd\xb7\xd4\xc0\xc7 \xc1\xa4\xba\xb8\xb0\xa1 "
				"\xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9. \xc6\xed\xc1\xfd \xbd\xc7\xc6\xd0\xc0\xd4\xb4\xcf\xb4\xd9.", false);
			pNotify->Open(0, 3);
		}
	} else {
		m_pSelectedSlotItem = 0;
	}

	return true;
}

// offlinetradedlg.cpp:3174
void COfflineShopDlg::OnProc(const float dt) {
	FrForm::OnProc(dt);
	CIconToolTip::GetInstance()->ClearToolTip();
}

// offlinetradedlg.cpp:3181
void COfflineShopDlg::SetListTradeItem(bool) {
	FrTradeDlg::SetListTradeItem(false);

	m_pTradeItemList->UseRightButton(false);

	sStall* pStall = CSharedDoc::Instance()->GetStallItem();

	m_pTradeItemList->ClearItem();

	for (int i = 0; i < pStall->open; i++) {
		m_pTradeItemList->AddItem(&pStall->slots[i]);
	}
}

// offlinetradedlg.cpp:3204 (run-time type and factory registration of COfflineShopVisitorDlg)
const WRTTI COfflineShopVisitorDlg::m_RTTI("COfflineShopVisitorDlg", &FrForm::m_RTTI);

IObject* COfflineShopVisitorDlgMakeInstance(void) {
	return new COfflineShopVisitorDlg;
}

struct __sCOfflineShopVisitorDlg {
	__sCOfflineShopVisitorDlg(void) {
		ObjectFactory().AddObjectFunctor(COfflineShopVisitorDlgMakeInstance, "COfflineShopVisitorDlg");
	}
};
static __sCOfflineShopVisitorDlg s_registerCOfflineShopVisitorDlg;

// offlinetradedlg.cpp:3206
const sFRESH_MSGMAP* COfflineShopVisitorDlg::GetMessageMap(void) const {
	return &COfflineShopVisitorDlg::_MsgMap;
}

// offlinetradedlg.cpp:3299
COfflineShopVisitorDlg::~COfflineShopVisitorDlg(void) {
}

// offlinetradedlg.cpp:3304
void COfflineShopVisitorDlg::OnEditBnUp(void) {
}

// offlinetradedlg.cpp:3320
void COfflineShopVisitorDlg::OnVisitorBackOwnerDraw(void) {
}

// offlinetradedlg.cpp:3326
void COfflineShopVisitorDlg::OnCloseBnUp(void) {
	FrTradeDlg::OnOK();
}

// offlinetradedlg.cpp:3341
void COfflineShopVisitorDlg::OnVisitorInit(int param) {
	m_pVisitor = DYNAMIC_CAST(FrEdit, param);

	if (IsLocalContent((localContentType_t)0x66) && CSharedDoc::Instance()->GetTradeMode() == 1) {
		m_pVisitor->SetVisible(false);
	}
}

// offlinetradedlg.cpp:3360
void COfflineShopVisitorDlg::OnVisitorBackInit(int param) {
	m_pVisitorBack = DYNAMIC_CAST(FrArea, param);

	if (m_pVisitorBack) {
		m_pVisitorBack->SetVisible(false);
	}
}

// offlinetradedlg.cpp:3517
void COfflineShopVisitorDlg::OnToolTipInit(intptr_t param) {
	m_pToolTip = DYNAMIC_CAST(FrArea, param);
}

// offlinetradedlg.cpp:3523
void COfflineShopVisitorDlg::OnToolTipOnwerDraw(int) {
	CIconToolTip::GetInstance()->Render();
}

// offlinetradedlg.cpp:3548
bool COfflineShopVisitorDlg::OnOfflineShopEditDlgCreateResult(int, FrForm *) {
	return true;
}

// offlinetradedlg.cpp:3554
void COfflineShopVisitorDlg::OnProc(const float dt) {
	FrForm::OnProc(dt);
	CIconToolTip::GetInstance()->ClearToolTip();
}

// offlinetradedlg.cpp:3561
void COfflineShopVisitorDlg::SetListTradeItem(bool) {
	FrTradeDlg::SetListTradeItem(false);

	sStall* pStall = CSharedDoc::Instance()->GetStallItem();

	m_pTradeItemList->UseRightButton(false);
	m_pTradeItemList->ClearItem();

	for (int i = 0; i < pStall->open; i++) {
		if (pStall->slots[i].reserved == 1 && pStall->slots[i].item.typeId != 0) {
			m_pTradeItemList->AddItem(&pStall->slots[i]);
		}
	}
}

// offlinetradedlg.cpp:3584 (run-time type and factory registration of COfflineBuyDlg)
const WRTTI COfflineBuyDlg::m_RTTI("COfflineBuyDlg", &FrForm::m_RTTI);

IObject* COfflineBuyDlgMakeInstance(void) {
	return new COfflineBuyDlg;
}

struct __sCOfflineBuyDlg {
	__sCOfflineBuyDlg(void) {
		ObjectFactory().AddObjectFunctor(COfflineBuyDlgMakeInstance, "COfflineBuyDlg");
	}
};
static __sCOfflineBuyDlg s_registerCOfflineBuyDlg;

// offlinetradedlg.cpp:3586
const sFRESH_MSGMAP* COfflineBuyDlg::GetMessageMap(void) const {
	return &COfflineBuyDlg::_MsgMap;
}

// offlinetradedlg.cpp:3607
COfflineBuyDlg::COfflineBuyDlg(void) {
}

// offlinetradedlg.cpp:3645
COfflineBuyDlg::~COfflineBuyDlg(void) {
}

// offlinetradedlg.cpp:3733
bool COfflineBuyDlg::OnConfirmResult(int result, FrForm* pForm) {
	if (result == 1) {
		if (pForm) {
			if (IsLocalContent((localContentType_t)0x66)) {
				WSendPacket packet((enumClientPacket)0xc6);
				packet.Encode4(CSharedDoc::Instance()->m_myRoomOwnerUID);
				packet.Encode4(m_buyItem.index);
				packet.Encode1(m_buyItem.used);
				packet.Encode2(m_buyCount);
				packet.Encode8(m_buyItem.item.price);
				packet.Send((eSendTo)0);

				CTaskManager* pTaskManager = CTaskManager::Instance();
				(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));

				m_buyCount = 0;
			}

			OnFreshOkay();
		}
	}

	return true;
}
