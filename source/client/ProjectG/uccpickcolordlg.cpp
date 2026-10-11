// Original: d:\build\custom\temp\code\client\projectg\uccpickcolordlg.cpp
#include "UccPickColorDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frlistbox.h"
#include "frbutton.h"
#include "frgraphicinterface.h"
#include "fresh.h"
#include "UccLibrary.h"
#include "UccBrush.h"
#include "UccDrawDlg.h"
#include "InputManager.h"
#include "frwndmanager.h"

// uccpickcolordlg.cpp:9 (run-time type and factory registration of FrUccPickColorDlg)
const WRTTI FrUccPickColorDlg::m_RTTI("FrUccPickColorDlg", &FrForm::m_RTTI);

IObject* FrUccPickColorDlgMakeInstance(void) {
	return new FrUccPickColorDlg;
}

struct __sFrUccPickColorDlg {
	__sFrUccPickColorDlg(void) {
		ObjectFactory().AddObjectFunctor(FrUccPickColorDlgMakeInstance, "FrUccPickColorDlg");
	}
};
static __sFrUccPickColorDlg s_registerFrUccPickColorDlg;

// uccpickcolordlg.cpp:11
const sFRESH_MSGMAP* FrUccPickColorDlg::GetMessageMap(void) const {
	return &FrUccPickColorDlg::_MsgMap;
}

sFRESH_MSGMAP FrUccPickColorDlg::_MsgMap = { &FrForm::_MsgMap, &FrUccPickColorDlg::_MsgEntries[0] };

sFRESH_ENTRY FrUccPickColorDlg::_MsgEntries[] = {
	// uccpickcolordlg.cpp:13
	{ "cancel_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnCancelBtnInit },
	{ "cancel_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccPickColorDlg::OnCancelBtnUp },
	{ "ok_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnOkBtnInit },
	{ "ok_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccPickColorDlg::OnOkBtnUp },
	{ "usecolor_btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnUseColorBtnInit },
	{ "usecolor_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccPickColorDlg::OnUseColorBtnUp },

	// uccpickcolordlg.cpp:20
	{ "color_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnColorListInit },
	{ "color_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnColorListOwnerDraw },
	{ "color_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrUccPickColorDlg::OnColorListBtnDown },
	{ "canvas", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnCanvasInit },
	{ "canvas_cursor", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnCanvasCursorInit },
	{ "brightness_canvas", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnBrightnessCanvasInit },
	{ "brightness_cursor1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnBrightnessCursor1Init },
	{ "brightness_cursor2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnBrightnessCursor2Init },
	{ "new_color", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnNewColorInit },
	{ "prev_color", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccPickColorDlg::OnPrevColorInit },

	// uccpickcolordlg.cpp:31
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// uccpickcolordlg.cpp:34
FrUccPickColorDlg::FrUccPickColorDlg(void)
	: m_red(0), m_green(0), m_blue(0) {
	Init();
}

// uccpickcolordlg.cpp:39
void FrUccPickColorDlg::SetParent(FrUccDrawDlg* pParent) {
	m_pParent = pParent;
	BuildColorList();
}

// uccpickcolordlg.cpp:45
void FrUccPickColorDlg::Init(void) {
	m_pParent = 0;

	m_prevColorBmp.Create(1, 1, 32);
	m_newColorBmp.Create(1, 1, 32);

	m_pSelectColor = g_pFresh->RegisterBitmap("ucc_select_color");
}

// uccpickcolordlg.cpp:56
void FrUccPickColorDlg::BuildColorList(void) {
	if (m_pColorList && m_pParent) {
		m_pColorList->ClearItem();

		for (int i = 0; i < 18; ++i) {
			m_pColorList->AddItem(&m_pParent->m_palette[i]);
		}
	}
}

// uccpickcolordlg.cpp:69
void FrUccPickColorDlg::MoveCanvasCursor(unsigned char red, unsigned char green, unsigned char blue) {
	unsigned char hue, lightness, saturation;
	RGBToHLS(red, green, blue, hue, lightness, saturation);

	Bitmap* pTexture = m_colorPicker.GetTexture32BPP(false);

	int x, y;
	if (pTexture) {
		x = hue * pTexture->Width() / 240;
		y = pTexture->Height() - saturation * pTexture->Height() / 240;
	}

	MoveCanvasCursor(x, y);
}

// uccpickcolordlg.cpp:90
void FrUccPickColorDlg::MoveCanvasCursor(const WPoint& point) {
	const WRect& canvasRect = m_pCanvas->GetRect();
	WPoint pos(canvasRect.x, canvasRect.y);

	float y = point.y - pos.y;

	MoveCanvasCursor((unsigned char)(point.x - pos.x), (unsigned char)y);
}

// uccpickcolordlg.cpp:100
void FrUccPickColorDlg::MoveCanvasCursor(unsigned char x, unsigned char y) {
	if (m_pCanvasCursor) {
		const WRect& cursorRect = m_pCanvasCursor->GetRect();
		WRect canvasRect = m_pCanvas->GetRect();

		WPoint pt;
		pt.x = x - cursorRect.w * 0.5f;
		pt.y = y - cursorRect.h * 0.5f;
		pt.x += canvasRect.x;
		pt.y += canvasRect.y;
		if (pt.x < 0.0f) pt.x = 0.0f;
		if (pt.y < 0.0f) pt.y = 0.0f;

		m_pCanvasCursor->MoveWindow(pt);
		m_pCanvasCursor->SetVisible(true);
	}
}

// uccpickcolordlg.cpp:119
void FrUccPickColorDlg::MoveBrightnessCursor(unsigned char red, unsigned char green, unsigned char blue) {
	unsigned char hue, lightness, saturation;
	RGBToHLS(red, green, blue, hue, lightness, saturation);
	MoveBrightnessCursor(lightness);
}

// uccpickcolordlg.cpp:126
void FrUccPickColorDlg::MoveBrightnessCursor(unsigned char brightness) {
	if (m_pBrightnessCanvas && m_pBrightnessCursor1 && m_pBrightnessCursor2) {
		const WRect& canvasRect = m_pBrightnessCanvas->GetRect();

		WRect rect1;
		rect1 = m_pBrightnessCursor1->GetRect();
		WRect rect2;
		rect2 = m_pBrightnessCursor2->GetRect();

		int height = (int)canvasRect.h;

		int pos = height - brightness * height / 255;

		WPoint pt1(rect1.x, 0.0f);
		WPoint pt2(rect2.x, 0.0f);
		pt1.y = canvasRect.y + pos - rect1.h * 0.5f;
		pt2.y = pt1.y;
		m_pBrightnessCursor1->MoveWindow(pt1);
		m_pBrightnessCursor2->MoveWindow(pt2);
	}
}

// uccpickcolordlg.cpp:152
void FrUccPickColorDlg::MoveBrightnessCursor(const WPoint& point) {
	if (m_pBrightnessCanvas && m_pBrightnessCursor1 && m_pBrightnessCursor2) {
		const WRect& canvasRect = m_pBrightnessCanvas->GetRect();
		WRect rect1;
		rect1 = m_pBrightnessCursor1->GetRect();
		WRect rect2;
		rect2 = m_pBrightnessCursor2->GetRect();

		WPoint pt1(rect1.x, rect1.y);
		WPoint pt2(rect2.x, rect2.y);

		if (canvasRect.y <= point.y && point.y <= canvasRect.y + canvasRect.h) {
			pt1.y = point.y;
			pt2.y = point.y;
		}

		pt1.y -= rect1.h * 0.5f;
		pt2.y -= rect2.h * 0.5f;
		m_pBrightnessCursor1->MoveWindow(pt1);
		m_pBrightnessCursor2->MoveWindow(pt2);
	}
}

// uccpickcolordlg.cpp:181
void FrUccPickColorDlg::OnColorListInit(intptr_t param) {
	m_pColorList = DYNAMIC_CAST(FrListBox, param);
}

// uccpickcolordlg.cpp:186
void FrUccPickColorDlg::OnColorListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0) {
		return;
	}

	FrGraphicInterface* pGDI = GetGDI();
	if (pGDI == 0) {
		return;
	}

	WRect rect(pItem->pos.x + 1.0f, pItem->pos.y + 1.0f, 9.0f, 9.0f);
	pGDI->DrawTexture((const Bitmap*)pItem->pData, rect, 0xffffffff, 0);

	if (pItem->selected == true) {
		pGDI->DrawTexture(m_pSelectColor,
			WRect(pItem->pos.x - 3.0f, pItem->pos.y - 3.0f, m_pSelectColor->Width(), m_pSelectColor->Height()),
			0xffffffff, 0);
	}
}

// uccpickcolordlg.cpp:211
void FrUccPickColorDlg::OnColorListBtnDown(void) {
	FrListItem* pItem = m_pColorList->GetItemUnderCursor();
	if (pItem == 0) {
		return;
	}

	Bitmap* pBitmap = (Bitmap*)pItem->pData;
	if (pBitmap == 0) {
		return;
	}

	unsigned char red, green, blue, alpha;
	pBitmap->GetPixel(0, 0, red, green, blue, alpha);

	if (alpha == 0xff) {
		m_colorBrightness.GenerateBrightnessMap(red, green, blue);
		SetCurrentColor(red, green, blue);
		FindColor(red, green, blue);
	}
}

// uccpickcolordlg.cpp:233
void FrUccPickColorDlg::OnCancelBtnInit(intptr_t param) {
	m_pCancelBtn = DYNAMIC_CAST(FrButton, param);
}

// uccpickcolordlg.cpp:238
void FrUccPickColorDlg::OnOkBtnInit(intptr_t param) {
	m_pOkBtn = DYNAMIC_CAST(FrButton, param);
}

// uccpickcolordlg.cpp:243
void FrUccPickColorDlg::OnUseColorBtnInit(intptr_t param) {
	m_pUseColorBtn = DYNAMIC_CAST(FrButton, param);
}

// uccpickcolordlg.cpp:248
void FrUccPickColorDlg::OnCanvasInit(intptr_t param) {
	m_pCanvas = DYNAMIC_CAST(FrArea, param);

	m_colorPicker.Initialize(m_pCanvas);
	m_pCanvas->SetBgImg(m_colorPicker.GetTexture32BPP(false));
}

// uccpickcolordlg.cpp:256
void FrUccPickColorDlg::OnCanvasCursorInit(intptr_t param) {
	m_pCanvasCursor = DYNAMIC_CAST(FrArea, param);

	m_pCanvasCursor->SetVisible(false);
}

// uccpickcolordlg.cpp:263
void FrUccPickColorDlg::OnBrightnessCanvasInit(intptr_t param) {
	m_pBrightnessCanvas = DYNAMIC_CAST(FrArea, param);

	m_colorBrightness.Initialize(m_pBrightnessCanvas);

	unsigned char red, green, blue;
	UccCurrentBrush()->GetBrushColor(red, green, blue);
	m_colorBrightness.GenerateBrightnessMap(red, green, blue);
	m_pBrightnessCanvas->SetBgImg(m_colorBrightness.GetTexture32BPP(false));
}

// uccpickcolordlg.cpp:274
void FrUccPickColorDlg::OnBrightnessCursor1Init(intptr_t param) {
	m_pBrightnessCursor1 = DYNAMIC_CAST(FrArea, param);
}

// uccpickcolordlg.cpp:279
void FrUccPickColorDlg::OnBrightnessCursor2Init(intptr_t param) {
	m_pBrightnessCursor2 = DYNAMIC_CAST(FrArea, param);

	unsigned char red, green, blue;
	UccCurrentBrush()->GetBrushColor(red, green, blue);
}

// uccpickcolordlg.cpp:287
void FrUccPickColorDlg::OnNewColorInit(intptr_t param) {
	m_pNewColor = DYNAMIC_CAST(FrArea, param);
}

// uccpickcolordlg.cpp:292
void FrUccPickColorDlg::OnPrevColorInit(intptr_t param) {
	m_pPrevColor = DYNAMIC_CAST(FrArea, param);

	unsigned char red, green, blue;
	UccCurrentBrush()->GetBrushColor(red, green, blue);

	m_prevColorBmp.SetPixel(0, 0, red, green, blue, 0xff);
	m_pPrevColor->SetBgImg(&m_prevColorBmp);
}

// uccpickcolordlg.cpp:304
void FrUccPickColorDlg::OnCancelBtnUp(void) {
	OnCancel();
}

// uccpickcolordlg.cpp:309
void FrUccPickColorDlg::OnOkBtnUp(void) {
	if (m_pParent) {
		m_pParent->SetCurrentColor(m_red, m_green, m_blue);
		FindColor(m_red, m_green, m_blue);
	}

	OnOK();
}

// uccpickcolordlg.cpp:324
void FrUccPickColorDlg::OnUseColorBtnUp(void) {
	static int s_nextSlot = 8;  // the last of the slots 9..17 overwritten when none is free

	FrListItem* pItem = m_pColorList->GetSelected();

	int index = -1;
	bool bFindEmpty = true;  // nothing selected, or one of the 9 fixed colours

	if (pItem) {
		bFindEmpty = false;
		void* pData = pItem->pData;
		for (int i = 0; i < 18; ++i) {
			if (&m_pParent->m_palette[i] == pData) {
				if (i >= 9) {
					index = i;
				} else {
					bFindEmpty = true;
				}
				break;
			}
		}
	}

	if (bFindEmpty) {
		for (int slot = 0; slot < 18; ++slot) {
			unsigned char red, green, blue, alpha;
			m_pParent->m_palette[slot].GetPixel(0, 0, red, green, blue, alpha);
			if (alpha == 0) {
				index = slot;
				break;
			}
		}
	}

	if (index == -1) {
		if (++s_nextSlot >= 18) {
			s_nextSlot = 9;
		}
		index = s_nextSlot;
	}

	m_pParent->m_palette[index].SetPixel(0, 0, m_red, m_green, m_blue, 0xff);

	RefreshTexCache(m_pParent->m_palette[index]);
}

// uccpickcolordlg.cpp:385
void FrUccPickColorDlg::FindColor(unsigned char red, unsigned char green, unsigned char blue) {
	unsigned char hue, lightness, saturation;
	RGBToHLS(red, green, blue, hue, lightness, saturation);

	Bitmap* pTexture = m_colorPicker.GetTexture32BPP(false);

	int x, y;
	if (pTexture) {
		x = hue * pTexture->Width() / 240;
		y = pTexture->Height() - saturation * pTexture->Height() / 240;
	}

	MoveCanvasCursor(x, y);

	MoveBrightnessCursor(lightness);
}

// uccpickcolordlg.cpp:410
void FrUccPickColorDlg::SetCurrentColor(unsigned char red, unsigned char green, unsigned char blue) {
	m_red = red; m_green = green; m_blue = blue;

	m_newColorBmp.SetPixel(0, 0, red, green, blue, 0xff);

	RefreshTexCache(m_newColorBmp);

	m_pNewColor->SetBgImg(&m_newColorBmp);
}

// uccpickcolordlg.cpp:427
void FrUccPickColorDlg::OnProc(const float dt) {
	const WPoint& mouse = g_pFresh->GetManager()->GetMousePos();

	if (m_colorPicker.IsMouseOver(mouse) && g_input->GetButton(LEFT_BUTTON) > 0) {
		unsigned char red, green, blue;
		m_colorPicker.GetColor(mouse, red, green, blue);
		m_colorBrightness.GenerateBrightnessMap(red, green, blue);
		SetCurrentColor(red, green, blue);

		MoveCanvasCursor(mouse);
		unsigned char hue, lightness, saturation;
		RGBToHLS(red, green, blue, hue, lightness, saturation);
		MoveBrightnessCursor(lightness);
	}

	if (m_colorBrightness.IsMouseOver(mouse) && g_input->GetButton(LEFT_BUTTON) > 0) {
		unsigned char red, green, blue;
		m_colorBrightness.GetColor(mouse, red, green, blue);
		SetCurrentColor(red, green, blue);

		MoveBrightnessCursor(mouse);
	}
}
