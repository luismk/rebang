// Original: d:\build\custom\temp\code\client\projectg\uccdrawdlg.cpp
// The out-of-line copies of uccbase.h:141/144 (CUccBase::GetTexture/GetMask)
// live in this object (see the end of UccBase.h).
#define UCCDRAWDLG_UCCBASE_INLINE
#include "UccDrawDlg.h"
#include "ObjectFactory.h"
#include "UccBase.h"
#include "UccBrush.h"
#include "OverlayCursor.h"
#include "SecurityKeyManager.h"
#include "../shared/globalgamedefine.h"
#include "frlistbox.h"
#include "frbutton.h"
#include "frarea.h"
#include "frgraphicinterface.h"
#include "NotifyForm.h"
#include "UccLibrary.h"
#include "UccPickColorDlg.h"
#include "UccSetInfoDlg.h"
#include "fresh.h"
#include "TaskManager.h"
#include "wresrcmng.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// uccbase.h:106
void CUccBase::SetParent(FrUccDrawDlg* pParent) {
	m_pParent = pParent;
}

// uccbase.h:212
char* CUccBaseItemDraw::GetTextureName(void) {
	return m_textureName;
}

// overlaycursor.h:44
void COverlayCursor::SetRect(FrWnd** ppWnd) {
	m_ppWnd = ppWnd;
}

// overlaycursor.h:47
void COverlayCursor::SetCursor(eCursorType type) {
	m_cursorType = type;
}

// uccdrawdlg.h:86
bool FrUccDrawDlg::IsSaved(void) {
	return m_bSaved;
}

// uccbrush.h:41
int CUccBrush::GetBrushNum(void) {
	return m_brushes.size();
}

// uccdrawdlg.cpp:24 (run-time type and factory registration of FrUccDrawDlg)
const WRTTI FrUccDrawDlg::m_RTTI("FrUccDrawDlg", &FrForm::m_RTTI);

IObject* FrUccDrawDlgMakeInstance(void) {
	return new FrUccDrawDlg;
}

struct __sFrUccDrawDlg {
	__sFrUccDrawDlg(void) {
		ObjectFactory().AddObjectFunctor(FrUccDrawDlgMakeInstance, "FrUccDrawDlg");
	}
};
static __sFrUccDrawDlg s_registerFrUccDrawDlg;

// uccdrawdlg.cpp:26
const sFRESH_MSGMAP* FrUccDrawDlg::GetMessageMap(void) const {
	return &FrUccDrawDlg::_MsgMap;
}

// uccdrawdlg.cpp:133
FrUccDrawDlg::FrUccDrawDlg(void) {
	Init();
}

// uccdrawdlg.cpp:138
FrUccDrawDlg::~FrUccDrawDlg(void) {
	Clear();
}

// uccdrawdlg.cpp:286
void FrUccDrawDlg::Clear(void) {
	UccCurrentBrush()->SetBrush((eBrushType)0, 0, 0, 0, 0);
	SetCurrentColor(0, 0, 0);

	if (m_pBrushList->GetSelected() != NULL) {
		m_pBrushList->GetSelected()->selected = false;
	}
	if (m_pPenList->GetSelected() != NULL) {
		m_pPenList->GetSelected()->selected = false;
	}
	if (m_pPatternList->GetSelected() != NULL) {
		m_pPatternList->GetSelected()->selected = false;
	}

	m_pBrushList->SelectItem(m_pBrushList->GetItem(0), true);

	if (m_pUccBase[0] != NULL) { delete m_pUccBase[0]; m_pUccBase[0] = NULL; }
	if (m_pUccBase[1] != NULL) { delete m_pUccBase[1]; m_pUccBase[1] = NULL; }

	m_bShowGrid = false;
}

// uccdrawdlg.cpp:317
bool FrUccDrawDlg::Open(FRESH_RESULT_CALLBACK pfnResult, const WPoint& pt, unsigned long flag) {
	bool bRet = FrForm::Open(pfnResult, pt, flag);

	WPoint pos(GetRect().x, GetRect().y);

	const WRect& canvasRect = m_pCanvasWnd->GetRect();
	m_canvasRect = WRect(canvasRect.x - pos.x, canvasRect.y - pos.y, canvasRect.w, canvasRect.h);

	MoveCanvasToCenter();

	return bRet;
}

// uccdrawdlg.cpp:330
void FrUccDrawDlg::OnCancel(void) {
	if (m_bSaved) {
		m_pAskCancelDlg = NULL;
		Clear();
		FrForm::OnCancel();
	} else if (m_pAskCancelDlg == NULL) {
		m_pAskCancelDlg = CreateForm<FrNotifyForm>(g_pFresh->GetManager(), this, "notify_yesnocancel", 0);
		m_pAskCancelDlg->SetMessage("\xB1\xD7\xB8\xAE\xB4\xF8 \xC0\xC7\xBB\xF3\xC0\xBB \xC0\xD3\xBD\xC3\xC0\xFA\xC0\xE5\xC7\xCF\xB0\xED \xB3\xA1\xB3\xBB\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
		m_pAskCancelDlg->Open((FRESH_RESULT_CALLBACK)&FrUccDrawDlg::OnAskCancelDlgResult, 1);
	}
}

// uccdrawdlg.cpp:347
bool FrUccDrawDlg::Close(eFormRet ret, bool bDestroy) {
	CloseWaitingForm();
	return FrForm::Close(ret, bDestroy);
}

// uccdrawdlg.cpp:354
void FrUccDrawDlg::SetName(const char* name) {
	strcpy(m_name, name);
}

// uccdrawdlg.cpp:394
void FrUccDrawDlg::Notify(const char* text) {
	if (AfxGetTask()->GetActor("RealMyRoom") != NULL) {
		AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)text, 0, 0, 0, 0);
	} else {
		AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)text, 0, 0, 0, 0);
	}
}

// uccdrawdlg.cpp:406
void FrUccDrawDlg::SetCurrentColor(unsigned char r, unsigned char g, unsigned char b) {
	m_currentColor.SetPixel(0, 0, r, g, b, 0xff);

	RefreshTexCache(m_currentColor);

	UccCurrentBrush()->SetBrush(r, g, b);
}

// uccdrawdlg.cpp:415
void FrUccDrawDlg::MoveCanvasToCenter(void) {
	WPoint pt(GetRect().x, GetRect().y);
	pt.x += m_canvasRect.x;
	pt.y += m_canvasRect.y;

	Bitmap* pTexture = m_pUccBase[m_curSide]->GetTexture32BPP(false);
	if (pTexture != NULL) {
		int offsetX = (256 - pTexture->Width()) / 2;
		int offsetY = (256 - pTexture->Height()) / 2;
		pt.x += offsetX;
		pt.y += offsetY;
	}

	m_pCanvasWnd->MoveWindow(pt);
}

// uccdrawdlg.cpp:815
void FrUccDrawDlg::OnUccCanvasInit(int param) {
	m_pCanvasWnd = DYNAMIC_CAST(FrArea, param);
}

// uccdrawdlg.cpp:820
void FrUccDrawDlg::OnUndoBtnInit(int param) {
	m_pUndoBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:825
void FrUccDrawDlg::OnUndoBtnUp(void) {
	if (m_pUccBase[m_curSide] != NULL) {
		m_pUccBase[m_curSide]->Undo();
	}
}

// uccdrawdlg.cpp:833
void FrUccDrawDlg::OnRedoBtnInit(int param) {
	m_pRedoBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:838
void FrUccDrawDlg::OnRedoBtnUp(void) {
	if (m_pUccBase[m_curSide] != NULL) {
		m_pUccBase[m_curSide]->Redo();
	}
}

// uccdrawdlg.cpp:846
void FrUccDrawDlg::OnFlipBtnInit(int param) {
	m_pFlipBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:851
void FrUccDrawDlg::OnFlipBtnOwnerDraw(int param) {
	WRect rect = m_pFlipBtn->GetRect();

	FrGraphicInterface* pGDI = GetGDI();
	if (pGDI == NULL) {
		return;
	}

	if (m_pToolChangeIcon != NULL) {
		pGDI->DrawTexture(m_pToolChangeIcon, WRect(rect.x + 6.0f, rect.y + 6.0f, (float)m_pToolChangeIcon->Width(), (float)m_pToolChangeIcon->Height()), 0xffffffff, 0);
	}
}

// uccdrawdlg.cpp:866
void FrUccDrawDlg::OnFlipBtnUp(void) {
	eMode mode = m_pUccBase[m_curSide]->GetMode();

	m_pUccBase[m_curSide]->SetMode(UCC_MODE_DRAW);
	m_curSide = !m_curSide;

	m_pUccBase[m_curSide]->m_refreshFlags |= 1;

	if (m_pSide1Area != NULL) {
		m_pSide1Area->SetBgImg(m_pSideTextBg[0][m_curSide]);
	}
	if (m_pSide2Area != NULL) {
		m_pSide2Area->SetBgImg(m_pSideTextBg[1][1 - m_curSide]);
	}

	m_pZoomInBtn->Enable(m_pUccBase[m_curSide]->m_zoomRate < 8);
	m_pZoomOutBtn->Enable(m_pUccBase[m_curSide]->m_zoomRate > 1);

	MoveCanvasToCenter();

	m_pUccBase[m_curSide]->SetMode(mode);
}

// uccdrawdlg.cpp:902
void FrUccDrawDlg::OnResetBtnInit(int param) {
	m_pResetBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:907
void FrUccDrawDlg::OnResetBtnUp(void) {
	if (m_pAskResetDlg == NULL) {
		m_pAskResetDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
		m_pAskResetDlg->SetMessage("\xC1\xA4\xB8\xBB \xC3\xB3\xC0\xBD\xBA\xCE\xC5\xCD \xB4\xD9\xBD\xC3 \xB1\xD7\xB8\xAE\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE? \n\n(\xC7\xF6\xC0\xE7 \xC0\xCC \xB1\xD7\xB8\xAE\xB1\xE2\xBF\xEB \xC0\xC7\xBB\xF3\xBF\xA1 \xB1\xD7\xB7\xC1\xC1\xAE \xC0\xD6\xB4\xC2 \n   \xB8\xF0\xB5\xE7 \xB1\xD7\xB8\xB2\xC0\xCC \xC1\xF6\xBF\xF6\xC1\xFD\xB4\xCF\xB4\xD9)", false);
		m_pAskResetDlg->Open((FRESH_RESULT_CALLBACK)&FrUccDrawDlg::OnAskResetDlgResult, 1);
	}
}

// uccdrawdlg.cpp:917
void FrUccDrawDlg::OnGridBtnInit(int param) {
	m_pGridBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:922
void FrUccDrawDlg::OnGridBtnUp(void) {
	m_bShowGrid = !m_bShowGrid;
}

// uccdrawdlg.cpp:927
void FrUccDrawDlg::OnDragBtnInit(int param) {
	m_pDragBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:932
void FrUccDrawDlg::OnDragBtnUp(void) {
	CUccBase* pUccBase = m_pUccBase[m_curSide];
	if (pUccBase->GetMode() == UCC_MODE_MOVE) {
		pUccBase->SetMode(UCC_MODE_DRAW);
		m_overlayCursor.SetCursor((COverlayCursor::eCursorType)UCC_MODE_DRAW);
	} else {
		pUccBase->SetMode(UCC_MODE_MOVE);
		m_overlayCursor.SetCursor((COverlayCursor::eCursorType)UCC_MODE_MOVE);
	}
	SetToolbarStatus(m_pDragBtn);
}

// uccdrawdlg.cpp:948
void FrUccDrawDlg::OnZoomInBtnInit(int param) {
	m_pZoomInBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:953
void FrUccDrawDlg::OnZoomInBtnUp(void) {
	SetToolbarStatus(m_pZoomInBtn);
	m_overlayCursor.SetCursor((COverlayCursor::eCursorType)6);
	m_pUccBase[m_curSide]->SetMode(UCC_MODE_ZOOM_IN);
}

// uccdrawdlg.cpp:962
void FrUccDrawDlg::OnZoomOutBtnInit(int param) {
	m_pZoomOutBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:967
void FrUccDrawDlg::OnZoomOutBtnUp(void) {
	SetToolbarStatus(m_pZoomOutBtn);
	m_overlayCursor.SetCursor((COverlayCursor::eCursorType)7);
	m_pUccBase[m_curSide]->SetMode(UCC_MODE_ZOOM_OUT);
}

// uccdrawdlg.cpp:976
void FrUccDrawDlg::AfterZoom(void) {
	m_pZoomInBtn->Enable(m_pUccBase[m_curSide]->m_zoomRate < 8);
	m_pZoomOutBtn->Enable(m_pUccBase[m_curSide]->m_zoomRate > 1);

	MoveCanvasToCenter();

	if (m_pUccBase[m_curSide]->GetMode() == UCC_MODE_DRAW) {
		m_overlayCursor.SetCursor((COverlayCursor::eCursorType)0);
		SetToolbarStatus(m_pDrawBtn);
	}
}

// uccdrawdlg.cpp:991
void FrUccDrawDlg::SetSaved(bool bSaved) {
	m_bSaved = bSaved;

	if (bSaved) {
		m_fadingText.Print("\xBC\xBF\xC7\xC1\xB5\xF0\xC0\xDA\xC0\xCE \xC0\xC7\xBB\xF3\xC0\xCC \xC0\xD3\xBD\xC3 \xC0\xFA\xC0\xE5\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", 3.0f, this, 105.0f, 20.0f, eFADE_IN_OUT, 0xff000000);
	}

	if (m_bCloseAfterSave) {
		Close(FrOK, true);
	}
}

// uccdrawdlg.cpp:1006
void FrUccDrawDlg::OnSide1AreaInit(int param) {
	m_pSide1Area = DYNAMIC_CAST(FrArea, param);
}

// uccdrawdlg.cpp:1011
void FrUccDrawDlg::OnSide2AreaInit(int param) {
	m_pSide2Area = DYNAMIC_CAST(FrArea, param);
}

// uccdrawdlg.cpp:1016
void FrUccDrawDlg::OnTempSaveBtnInit(int param) {
	m_pTempSaveBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:1021
void FrUccDrawDlg::OnTempSaveBtnUp(void) {
	SecurityKeyManager()->ReqCreateKey(SK_CONTENT_UCC, m_pItemInfo->m_serial);
	m_bSavingTemporarily = true;
}

// uccdrawdlg.cpp:1030
void FrUccDrawDlg::OnTabDrawBtnInit(int param) {
	m_pDrawBtn = DYNAMIC_CAST(FrButton, param);
	m_pDrawBtn->SetStatus(FrButton::PRESSED);
}

// uccdrawdlg.cpp:1036
void FrUccDrawDlg::OnTabDrawBtnUp(void) {
	ChangeTab(UCC_TAB_DRAW);
	m_pUccBase[m_curSide]->SetMode(UCC_MODE_DRAW);
	m_overlayCursor.SetCursor((COverlayCursor::eCursorType)0);
	SetToolbarStatus(m_pDrawBtn);
}

// uccdrawdlg.cpp:1045
void FrUccDrawDlg::OnTabPatternBtnInit(int param) {
	m_pTabPatternBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:1050
void FrUccDrawDlg::OnTabPatternBtnUp(void) {
	ChangeTab(UCC_TAB_PATTERN);
	m_pUccBase[m_curSide]->SetMode(UCC_MODE_DRAW);

	m_overlayCursor.SetCursor((COverlayCursor::eCursorType)0);
	SetToolbarStatus(m_pTabPatternBtn);
}

// uccdrawdlg.cpp:1059
void FrUccDrawDlg::OnTabEmoticonBtnInit(int param) {
	m_pTabEmoticonBtn = DYNAMIC_CAST(FrButton, param);
	m_pTabEmoticonBtn->Enable(false);
}

// uccdrawdlg.cpp:1065
void FrUccDrawDlg::OnTabEmoticonBtnUp(void) {
}

// uccdrawdlg.cpp:1070
void FrUccDrawDlg::OnTabTextBtnInit(int param) {
	m_pTabTextBtn = DYNAMIC_CAST(FrButton, param);
	m_pTabTextBtn->Enable(false);
}

// uccdrawdlg.cpp:1076
void FrUccDrawDlg::OnTabTextBtnUp(void) {
}

// uccdrawdlg.cpp:1082
void FrUccDrawDlg::OnFinishBtnInit(int param) {
	m_pFinishBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:1087
void FrUccDrawDlg::Finish(void) {
	if (m_pItemInfo == NULL) {
		return;
	}
	if (m_pUccSetInfoDlg == NULL) {
		WRect rect = GetRect();
		m_pUccSetInfoDlg = CreateForm<FrUccSetInfoDlg>(g_pFresh->GetManager(), this, "ucc_setinfo", 0);
		m_pUccSetInfoDlg->SetParent(this);
		m_pUccSetInfoDlg->Open((FRESH_RESULT_CALLBACK)&FrUccDrawDlg::OnUccSetInfoDlgResult, WPoint(rect.x + 30.0f, rect.y + 30.0f), 0);
	}
}

// uccdrawdlg.cpp:1105
void FrUccDrawDlg::OnFinishBtnUp(void) {
	Finish();
}

// uccdrawdlg.cpp:1135
bool FrUccDrawDlg::OnAskCompleteDlgResult(int ret, FrForm* pForm) {
	m_pAskCompleteDlg = NULL;
	if (ret == FrOK) {
		Finish();
	}
	return true;
}

// uccdrawdlg.cpp:1147
bool FrUccDrawDlg::OnAskCancelDlgResult(int ret, FrForm* pForm) {
	m_pAskCancelDlg = NULL;

	if (ret == FrYES) {
		m_bCloseAfterSave = true;
		SecurityKeyManager()->ReqCreateKey(SK_CONTENT_UCC, m_pItemInfo->m_serial);
		m_bSavingTemporarily = true;
	} else if (ret == FrNO) {
		Clear();

		FrForm::OnCancel();
	}

	return true;
}

// uccdrawdlg.cpp:1171
bool FrUccDrawDlg::OnAskResetDlgResult(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		Reset();
	}
	m_pAskResetDlg = NULL;
	return true;
}

// uccdrawdlg.cpp:1183
bool FrUccDrawDlg::OnUccSetInfoDlgResult(int ret, FrForm* pForm) {
	m_pUccSetInfoDlg = NULL;
	if (ret == FrOK) {
		SecurityKeyManager()->ReqCreateKey(SK_CONTENT_UCC, m_pItemInfo->m_serial);
		m_bSavingTemporarily = true;
	}
	return true;
}

// uccdrawdlg.cpp:1196
void FrUccDrawDlg::OnCloseBtnInit(intptr_t param) {
	m_pCloseBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:1201
void FrUccDrawDlg::OnCloseBtnUp(void) {
	OnCancel();
}

// uccdrawdlg.cpp:1210
void FrUccDrawDlg::OnPaintBtnInit(int param) {
	m_pPaintBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:1215
void FrUccDrawDlg::OnPaintBtnUp(void) {
	m_pUccBase[m_curSide]->SetMode(UCC_MODE_PAINT);
	m_overlayCursor.SetCursor((COverlayCursor::eCursorType)4);
	SetToolbarStatus(m_pDrawBtn);
}

// uccdrawdlg.cpp:1223
void FrUccDrawDlg::OnPickColorBtnInit(int param) {
	m_pPickColorBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:1228
void FrUccDrawDlg::OnPickColorBtnUp(void) {
	m_pUccBase[m_curSide]->SetMode(UCC_MODE_SPOID);
	m_overlayCursor.SetCursor((COverlayCursor::eCursorType)3);
	SetToolbarStatus(m_pDrawBtn);
}

// uccdrawdlg.cpp:1236
void FrUccDrawDlg::OnCurrentColorBtnInit(int param) {
	m_pCurrentColorBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:1241
void FrUccDrawDlg::OnCurrentColorBtnUp(void) {
	if (m_pUccPickColorDlg == NULL) {
		WRect rect = GetRect();
		m_pUccPickColorDlg = CreateForm<FrUccPickColorDlg>(g_pFresh->GetManager(), this, "ucc_pickcolor", 0);
		m_pUccPickColorDlg->SetParent(this);
		m_pUccPickColorDlg->Open((FRESH_RESULT_CALLBACK)&FrUccDrawDlg::OnUccPickColorDlgResult, WPoint(rect.x + 30.0f, rect.y + 30.0f), 0);
	}
}

// uccdrawdlg.cpp:1252
bool FrUccDrawDlg::OnUccPickColorDlgResult(int ret, FrForm* pForm) {
	m_pUccPickColorDlg = NULL;
	return true;
}

// uccdrawdlg.cpp:1259
void FrUccDrawDlg::OnCurrentColorAreaInit(int param) {
	m_pCurrentColorArea = DYNAMIC_CAST(FrArea, param);

	m_currentColor.Create(1, 1, 32);
	m_currentColor.SetPixel(0, 0, 0, 0, 0, 0xff);
	m_pCurrentColorArea->SetBgImg(&m_currentColor);
}

// uccdrawdlg.cpp:1268
void FrUccDrawDlg::OnColorListInit(intptr_t param) {
	m_pColorList = DYNAMIC_CAST(FrListBox, param);

	BuildColorList();
}

// uccdrawdlg.cpp:1275
void FrUccDrawDlg::OnColorListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == NULL) {
		return;
	}

	FrGraphicInterface* pGDI = GetGDI();
	if (pGDI == NULL) {
		return;
	}

	// the palette entry, inside its one-pixel border
	WRect colorRect(pItem->pos.x + 1.0f, pItem->pos.y + 1.0f, 9.0f, 9.0f);
	pGDI->DrawTexture((Bitmap*)pItem->pData, colorRect, 0xffffffff, 0);

	if (pItem->selected) {
		WRect selectRect(pItem->pos.x - 3.0f, pItem->pos.y - 3.0f, (float)m_pSelectColor->Width(), (float)m_pSelectColor->Height());
		pGDI->DrawTexture(m_pSelectColor, selectRect, 0xffffffff, 0);
	}
}

// uccdrawdlg.cpp:1301
void FrUccDrawDlg::OnColorListBtnDown(void) {
	FrListItem* pItem = m_pColorList->GetItemUnderCursor();
	if (pItem == NULL) {
		return;
	}

	Bitmap* pColor = (Bitmap*)pItem->pData;
	if (pColor == NULL) {
		return;
	}

	unsigned char r, g, b, a;
	pColor->GetPixel(0, 0, r, g, b, a);

	if (a == 0xff) {
		SetCurrentColor(r, g, b);
	}

	if (m_brushType == 2) {
		UccCurrentBrush()->SetBrush((eBrushType)2, m_pEraserList->GetSelected()->idx);
	}
}

// uccdrawdlg.cpp:1325
void FrUccDrawDlg::OnEraserListInit(int param) {
	m_pEraserList = DYNAMIC_CAST(FrListBox, param);

	BuildEraserList();
}

// uccdrawdlg.cpp:1331
void FrUccDrawDlg::OnEraserListOwnerDraw(int param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == NULL) {
		return;
	}

	FrGraphicInterface* pGDI = GetGDI();
	if (pGDI == NULL) {
		return;
	}

	if (m_pEraserList->GetSelected() == NULL && pItem->idx == m_eraserIndex) {
		m_pEraserList->SelectItem(pItem, true);
	}

	if (pItem->selected) {
		if (m_brushType != 2) {
			pItem->selected = false;
		}
	}

	if (pItem->selected) {
		const Bitmap* pSelect = m_pSelectBoxYellow;
		pGDI->DrawTexture(pSelect, WRect(pItem->pos.x + 1.0f, pItem->pos.y + 1.0f, pSelect->Width(), pSelect->Height()), 0xffffffff, 0);
	}
}

// uccdrawdlg.cpp:1364
void FrUccDrawDlg::OnEraserListBtnDown(void) {
	FrListItem* pItem = m_pEraserList->GetItemUnderCursor();
	if (pItem == NULL) {
		return;
	}
	if (pItem->pData == NULL) {
		return;
	}

	FrListItem* pSelected = m_pBrushList->GetSelected();
	if (pSelected != NULL) {
		pSelected->selected = false;
	}

	pSelected = m_pPenList->GetSelected();
	if (pSelected != NULL) {
		pSelected->selected = false;
	}

	pSelected = m_pPatternList->GetSelected();
	if (pSelected != NULL) {
		pSelected->selected = false;
		m_patternIndex = -1;
	}

	m_brushType = 2;

	UccCurrentBrush()->SetBrush((eBrushType)2, pItem->idx);
	m_overlayCursor.SetCursor((COverlayCursor::eCursorType)2);

	OnTabDrawBtnUp();
}

// uccdrawdlg.cpp:1397
void FrUccDrawDlg::OnToolInfoEraserAreaInit(int param) {
	m_pToolInfoEraserArea = DYNAMIC_CAST(FrArea, param);
}

// uccdrawdlg.cpp:1402
void FrUccDrawDlg::OnBaseColorListInit(int param) {
	m_pBaseColorListArea = DYNAMIC_CAST(FrArea, param);
}

// uccdrawdlg.cpp:1407
void FrUccDrawDlg::OnExtendedBasePatternListInit(int param) {
	m_pExtendedBasePatternListArea = DYNAMIC_CAST(FrArea, param);
}

// uccdrawdlg.cpp:1414
void FrUccDrawDlg::OnBrushListInit(int param) {
	m_pBrushList = DYNAMIC_CAST(FrListBox, param);

	BuildBrushList();
}

// uccdrawdlg.cpp:1420
void FrUccDrawDlg::OnBrushListOwnerDraw(int param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == NULL) {
		return;
	}

	FrGraphicInterface* pGDI = GetGDI();
	if (pGDI == NULL) {
		return;
	}

	if (m_pBrushList->GetSelected() == NULL && m_pPenList->GetSelected() == NULL && pItem->idx == UccCurrentBrush()->m_brushIndex && UccCurrentBrush()->m_brushType == 0) {
		m_pBrushList->SelectItem(pItem, true);
	}

	if (pItem->selected) {
		if (m_brushType == 2) {
			pItem->selected = false;
		}
	}

	if (pItem->selected) {
		const Bitmap* pSelect = m_pSelectBoxYellow;
		pGDI->DrawTexture(pSelect, WRect(pItem->pos.x + 1.0f, pItem->pos.y + 1.0f, pSelect->Width(), pSelect->Height()), 0xffffffff, 0);
	}
}

// uccdrawdlg.cpp:1454
void FrUccDrawDlg::OnBrushListBtnDown(void) {
	FrListItem* pItem = m_pBrushList->GetItemUnderCursor();
	if (pItem == NULL) {
		return;
	}
	if (pItem->pData == NULL) {
		return;
	}

	FrListItem* pSelected = m_pPenList->GetSelected();
	if (pSelected != NULL) {
		pSelected->selected = false;
	}

	m_brushType = 0;

	UccCurrentBrush()->SetBrush((eBrushType)0, pItem->idx);

	OnTabDrawBtnUp();
}

// uccdrawdlg.cpp:1475
void FrUccDrawDlg::OnPenListInit(int param) {
	m_pPenList = DYNAMIC_CAST(FrListBox, param);

	BuildPenList();
}

// uccdrawdlg.cpp:1481
void FrUccDrawDlg::OnPenListOwnerDraw(int param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == NULL) {
		return;
	}

	FrGraphicInterface* pGDI = GetGDI();
	if (pGDI == NULL) {
		return;
	}

	if (m_pBrushList->GetSelected() == NULL && m_pPenList->GetSelected() == NULL && pItem->idx == UccCurrentBrush()->m_brushIndex && UccCurrentBrush()->m_brushType == 1) {
		m_pPenList->SelectItem(pItem, true);
	}

	if (pItem->selected) {
		if (m_brushType == 2) {
			pItem->selected = false;
		}
	}

	if (pItem->selected) {
		const Bitmap* pSelect = m_pSelectBoxYellow;
		pGDI->DrawTexture(pSelect, WRect(pItem->pos.x + 1.0f, pItem->pos.y + 1.0f, pSelect->Width(), pSelect->Height()), 0xffffffff, 0);
	}
}

// uccdrawdlg.cpp:1515
void FrUccDrawDlg::OnPenListBtnDown(void) {
	FrListItem* pItem = m_pPenList->GetItemUnderCursor();
	if (pItem == NULL) {
		return;
	}
	if (pItem->pData == NULL) {
		return;
	}

	FrListItem* pSelected = m_pBrushList->GetSelected();
	if (pSelected != NULL) {
		pSelected->selected = false;
	}

	m_brushType = 1;

	UccCurrentBrush()->SetBrush((eBrushType)1, pItem->idx);

	OnTabDrawBtnUp();
}

// uccdrawdlg.cpp:1536
void FrUccDrawDlg::OnToolInfoPenAreaInit(int param) {
	m_pToolInfoPenArea = DYNAMIC_CAST(FrArea, param);
}

// uccdrawdlg.cpp:1541
void FrUccDrawDlg::OnToolInfoBrushAreaInit(int param) {
	m_pToolInfoBrushArea = DYNAMIC_CAST(FrArea, param);
}

// uccdrawdlg.cpp:1546
void FrUccDrawDlg::OnColorListsAreaInit(int param) {
	m_pColorListsArea = DYNAMIC_CAST(FrArea, param);
}

// uccdrawdlg.cpp:1553
void FrUccDrawDlg::OnArrowDownBtnInit(int param) {
	m_pArrowDownBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:1558
void FrUccDrawDlg::OnArrowDownBtnUp(void) {
	m_bPatternListExpanded = !m_bPatternListExpanded;

	if (m_bPatternListExpanded) {
		m_pPatternLeftBtn->Enable(m_patternIndex > 0);
		m_pPatternRightBtn->Enable(m_patternIndex + 6 < 24);
		m_pExtendedBasePatternListArea->SetVisible(false);

		WRect listRect = m_pPatternList->GetRect();
		listRect.h = 27.0f;
		m_pPatternList->SetRect(listRect);

		WRect arrowRect = m_pArrowDownBtn->GetRect();
		arrowRect.y -= 77.0f;
		m_pArrowDownBtn->SetRect(arrowRect);

		m_pArrowDownBtn->SetButtonImg("ucc_arrow_down_n", FrButton::NORMAL);
		m_pArrowDownBtn->SetButtonImg("ucc_arrow_down_o", FrButton::OVER);
	} else {
		m_pPatternLeftBtn->Enable(false);
		m_pPatternRightBtn->Enable(false);
		m_pExtendedBasePatternListArea->SetVisible(true);

		WRect listRect = m_pPatternList->GetRect();
		listRect.h = 101.0f;
		m_pPatternList->SetRect(listRect);

		WRect arrowRect = m_pArrowDownBtn->GetRect();
		arrowRect.y += 77.0f;
		m_pArrowDownBtn->SetRect(arrowRect);

		m_pArrowDownBtn->SetButtonImg("ucc_arrow_up_n", FrButton::NORMAL);
		m_pArrowDownBtn->SetButtonImg("ucc_arrow_up_o", FrButton::OVER);
	}

	BuildPatternList(m_patternIndex, m_bPatternListExpanded);
}

// uccdrawdlg.cpp:1600
void FrUccDrawDlg::OnArrowRightBtnInit(int param) {
	m_pPatternRightBtn = DYNAMIC_CAST(FrButton, param);
}

// uccdrawdlg.cpp:1605
void FrUccDrawDlg::OnArrowRightBtnUp(void) {
	if (m_pPatternList->m_itemList.size() != 0) {
		// the items hold the pattern index itself, stored in the data pointer
		FrListItem* pFirst = m_pPatternList->m_itemList.front();

		int first = (int)(intptr_t)pFirst->pData;

		if (first + 6 < 24) {
			m_pPatternLeftBtn->Enable(first + 1 > 0);
			m_pPatternRightBtn->Enable(first + 7 < 24);

			m_pPatternList->ClearItem();
			for (int i = 0; i < 6; i++) {
				m_pPatternList->AddItem((void*)(intptr_t)(first + 1 + i));
			}
		}
	}
}

// uccdrawdlg.cpp:1629
void FrUccDrawDlg::OnArrowLeftBtnInit(int param) {
	m_pPatternLeftBtn = DYNAMIC_CAST(FrButton, param);
	m_pPatternLeftBtn->Enable(false);
}

// uccdrawdlg.cpp:1635
void FrUccDrawDlg::OnArrowLeftBtnUp(void) {
	if (m_pPatternList->m_itemList.size() != 0) {
		// the items hold the pattern index itself, stored in the data pointer
		FrListItem* pFirst = m_pPatternList->m_itemList.front();

		int first = (int)(intptr_t)pFirst->pData;

		if (first != 0) {
			m_pPatternLeftBtn->Enable(first - 1 > 0);
			m_pPatternRightBtn->Enable(first + 5 < 24);

			m_pPatternList->ClearItem();
			for (int i = 0; i < 6; i++) {
				m_pPatternList->AddItem((void*)(intptr_t)(first - 1 + i));
			}
		}
	}
}

// uccdrawdlg.cpp:1659
void FrUccDrawDlg::OnDeleteBtnInit(int param) {
	m_pDeleteBtn = DYNAMIC_CAST(FrButton, param);
	m_pDeleteBtn->Enable(false);
}

// uccdrawdlg.cpp:1665
void FrUccDrawDlg::OnDeleteBtnUp(void) {
}

// uccdrawdlg.cpp:1669
void FrUccDrawDlg::OnModifyBtnInit(int param) {
	m_pModifyBtn = DYNAMIC_CAST(FrButton, param);
	m_pModifyBtn->Enable(false);
}

// uccdrawdlg.cpp:1675
void FrUccDrawDlg::OnModifyBtnUp(void) {
}

// uccdrawdlg.cpp:1679
void FrUccDrawDlg::OnMakeBtnInit(int param) {
	m_pMakeBtn = DYNAMIC_CAST(FrButton, param);
	m_pMakeBtn->Enable(false);
}

// uccdrawdlg.cpp:1685
void FrUccDrawDlg::OnMakeBtnUp(void) {
}

// uccdrawdlg.cpp:1689
void FrUccDrawDlg::OnRateListInit(int param) {
	m_pRateList = DYNAMIC_CAST(FrListBox, param);
	m_pRateList->AddItem((void*)-1);
	m_pRateList->AddItem((void*)-1);
	m_pRateList->AddItem((void*)-1);
	m_pRateList->AddItem((void*)-1);
}

// uccdrawdlg.cpp:1698
void FrUccDrawDlg::OnRateListOwnerDraw(int param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == NULL) {
		return;
	}

	FrGraphicInterface* pGDI = GetGDI();
	if (pGDI == NULL) {
		return;
	}

	int rate = (pItem->idx + 1) * 2;
	char filename[40];
	sprintf(filename, "ucc_tool_scale_%d.tga", rate);
	Bitmap* pScale = g_resrcmng->LoadBitmap(filename, 0, false);

	if (pScale == NULL) {
		return;
	}

	pGDI->DrawTexture(pScale, WRect(pItem->pos.x, pItem->pos.y, pScale->Width(), pScale->Height()), 0xffffffff, 0);

	if (rate == m_patternRate) {
		const Bitmap* pSelect = m_pSelectSBox2;
		pGDI->DrawTexture(pSelect, WRect(pItem->pos.x, pItem->pos.y, pSelect->Width(), pSelect->Height()), 0xffffffff, 0);
	}
}

// uccdrawdlg.cpp:1734
void FrUccDrawDlg::OnRateListBtnDown(void) {
	FrListItem* pItem = m_pRateList->GetItemUnderCursor();
	if (pItem != NULL) {
		m_patternRate = (pItem->idx + 1) * 2;
		BuildCurrentPattern();
	}
}

// uccdrawdlg.cpp:1746
void FrUccDrawDlg::OnPatternListInit(int param) {
	m_pPatternList = DYNAMIC_CAST(FrListBox, param);

	BuildPatternList(0, true);
}

// uccdrawdlg.cpp:1753
void FrUccDrawDlg::OnPatternListOwnerDraw(int param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == NULL) {
		return;
	}

	FrGraphicInterface* pGDI = GetGDI();
	if (pGDI == NULL) {
		return;
	}

	bool bSelected = (int)(intptr_t)pItem->pData == m_patternIndex;

	if (bSelected) {
		if (m_brushType == 2) {
			pItem->selected = false;
		}
	}

	pGDI->DrawTexture(m_pToolBox, WRect(pItem->pos.x, pItem->pos.y, m_pToolBox->Width(), m_pToolBox->Height()), 0xffffffff, 0);

	const Bitmap* pPattern = &m_patternThumbnails[(int)(intptr_t)pItem->pData];
	if (pPattern != NULL) {
		if (bSelected) {
			pGDI->DrawTexture(&m_patternHighlight, WRect(pItem->pos.x, pItem->pos.y, m_patternHighlight.Width(), m_patternHighlight.Height()), 0xffffffff, 0);
		}

		pGDI->DrawTexture(pPattern, WRect(pItem->pos.x + 3.0f, pItem->pos.y + 3.0f, pPattern->Width(), pPattern->Height()), 0xffffffff, 0);
	}

	if (bSelected) {
		const Bitmap* pSelect = m_pSelectBox;
		pGDI->DrawTexture(pSelect, WRect(pItem->pos.x - 2.0f, pItem->pos.y - 2.0f, pSelect->Width(), pSelect->Height()), 0xffffffff, 0);
	}
}

// uccdrawdlg.cpp:1799
void FrUccDrawDlg::OnPatternListBtnDown(void) {
	FrListItem* pItem = m_pPatternList->GetItemUnderCursor();
	if (pItem != NULL) {
		// the items hold the pattern index itself, stored in the data pointer
		m_patternIndex = (int)(intptr_t)pItem->pData;

		m_brushType = 3;
		BuildCurrentPattern();

		if (!m_bPatternListExpanded) {
			OnArrowDownBtnUp();
		}
	}
}

// uccdrawdlg.cpp:1816
void FrUccDrawDlg::OnEtcInit(int param) {
	m_pEtcArea = DYNAMIC_CAST(FrArea, param);
	ChangeTab(UCC_TAB_DRAW);
}

// uccdrawdlg.cpp:1905
void FrUccDrawDlg::OnToolTipInit(intptr_t param) {
	m_pToolTipArea = DYNAMIC_CAST(FrArea, param);
}

// uccdrawdlg.cpp:1910
void FrUccDrawDlg::OnToolTipOnwerDraw(int param) {
	m_toolTipManager.Process();
}

// uccdrawdlg.cpp:1918
void FrUccDrawDlg::BuildBrushList(void) {
	if (m_pBrushList != NULL) {
		m_pBrushList->ClearItem();
		for (int i = 0; i < UccBrushList((eBrushType)0)->GetBrushNum(); i++) {
			m_pBrushList->AddItem(UccBrushList((eBrushType)0)->GetBrush(i));
		}
	}
}

// uccdrawdlg.cpp:1931
void FrUccDrawDlg::BuildPenList(void) {
	if (m_pPenList != NULL) {
		m_pPenList->ClearItem();
		for (int i = 0; i < UccBrushList((eBrushType)1)->GetBrushNum(); i++) {
			m_pPenList->AddItem(UccBrushList((eBrushType)1)->GetBrush(i));
		}
	}
}

// uccdrawdlg.cpp:1944
void FrUccDrawDlg::BuildEraserList(void) {
	if (m_pEraserList != NULL) {
		m_pEraserList->ClearItem();
		for (int i = 0; i < UccBrushList((eBrushType)2)->GetBrushNum(); i++) {
			m_pEraserList->AddItem(UccBrushList((eBrushType)2)->GetBrush(i));
		}
	}
}

// uccdrawdlg.cpp:1957
void FrUccDrawDlg::BuildPatternList(int first, bool b) {
	if (m_pPatternList == NULL) {
		return;
	}

	int start = (first + 1) / 6 * 6;
	int end;

	if (b) {
		int count = UccBrushList((eBrushType)3)->GetBrushNum();
		count = count < 6 ? count : 6;

		end = start + count;

		if (end > 24) {
			start = start - end + 24;
			end = 24;
		}
	} else {
		start = 0;
		end = 24;
	}

	m_pPatternList->ClearItem();
	for (int i = start; i < end; i++) {
		m_pPatternList->AddItem((void*)(intptr_t)i);
	}
}

// uccdrawdlg.cpp:1992
void FrUccDrawDlg::BuildColorList(void) {
	if (m_pColorList != NULL) {
		m_pColorList->ClearItem();
		for (int i = 0; i < 18; i++) {
			m_pColorList->AddItem(&m_palette[i]);
		}
	}
}

// uccdrawdlg.cpp:2005
void FrUccDrawDlg::BuildCurrentPattern(void) {
	Bitmap* pPattern = UccBrushList((eBrushType)3)->GetBrush(m_patternIndex);
	Bitmap pattern;

	if (pPattern == NULL) {
		return;
	}

	// the tiled preview of the pattern is not built in this version
}

// uccdrawdlg.cpp:2024
void FrUccDrawDlg::ChangeTab(eUccTab tab) {
	m_tabManager.Open(tab);

	m_curTab = tab;

	if (!m_bPatternListExpanded) {
		OnArrowDownBtnUp();
	}

	switch (tab) {
	case UCC_TAB_DRAW:
		m_pColorList->Enable(true);
		m_pPickColorBtn->Enable(true);
		m_pCurrentColorBtn->Enable(true);

		if (m_pPenList->GetSelected() != NULL) {
			m_brushType = 1;
		} else {
			m_brushType = 0;
		}
		break;
	case UCC_TAB_PATTERN:
		m_pColorList->Enable(false);
		m_pPickColorBtn->Enable(false);
		m_pCurrentColorBtn->Enable(false);
		m_brushType = 3;
		break;
	}
}

// uccdrawdlg.cpp:2080
void FrUccDrawDlg::Reset(void) {
	eDrawMode mode = m_drawMode;
	sItemInfo* pItemInfo = m_pItemInfo;

	Clear();
	SetUccDrawInfo(mode, pItemInfo, true);
	ChangeTab(UCC_TAB_DRAW);

	for (int i = 1; i >= 0; i--) {
		if (m_pUccBase[i] != NULL) {
			m_pUccBase[i]->m_refreshFlags |= 2;
			m_pUccBase[i]->Process();
		}
	}
}

// uccdrawdlg.cpp:2102
void FrUccDrawDlg::CloseWaitingForm(void) {
	if (m_pWaitingForm != NULL) {
		m_pWaitingForm->Close(FrOK, true);
		m_pWaitingForm = NULL;
		m_waitingFormParam = 0;
		m_bSavingTemporarily = false;
	}
}

// uccdrawdlg.cpp:2113
void FrUccDrawDlg::SetToolbarStatus(FrButton* pButton) {
	m_pGridBtn->SetStatus(FrButton::NORMAL);
	m_pDragBtn->SetStatus(FrButton::NORMAL);
	m_pZoomInBtn->SetStatus(FrButton::NORMAL);
	m_pZoomOutBtn->SetStatus(FrButton::NORMAL);
	if (m_pDrawBtn != NULL) {
		m_pDrawBtn->SetStatus(FrButton::NORMAL);
	}
	if (m_pTabPatternBtn != NULL) {
		m_pTabPatternBtn->SetStatus(FrButton::NORMAL);
	}
	if (m_pTabEmoticonBtn != NULL) {
		m_pTabEmoticonBtn->SetStatus(FrButton::NORMAL);
	}
	if (m_pTabTextBtn != NULL) {
		m_pTabTextBtn->SetStatus(FrButton::NORMAL);
	}

	if (pButton != NULL) {
		pButton->SetStatus(FrButton::PRESSED);
	}
}

// uccdrawdlg.cpp:2134
bool FrUccDrawDlg::IsSavingTemporarily(void) {
	return strlen(m_name) == 0;
}
