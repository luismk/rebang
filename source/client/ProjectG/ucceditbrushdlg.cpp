// Original: d:\build\custom\temp\code\client\projectg\ucceditbrushdlg.cpp
#include "ucceditbrushdlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "UccLibrary.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "InputManager.h"
#include "../client/Wangreal/include/wtypes.h"
#include <stdlib.h>

// ucceditbrushdlg.cpp:12
CUccEditBrush::CUccEditBrush(void) {
}

// ucceditbrushdlg.cpp:16
CUccEditBrush::~CUccEditBrush(void) {
}

// ucceditbrushdlg.cpp:20
void CUccEditBrush::Process(void) {
	CUccBase::Process();

	RefreshTexCache(m_pTexture);
}

// ucceditbrushdlg.cpp:27
void CUccEditBrush::DrawLine(IPoint from, IPoint to, Bitmap* pBrush) {
	if (pBrush == NULL) {
		return;
	}

	int length = abs(to.x - from.x) >= abs(to.y - from.y) ? abs(to.x - from.x) : abs(to.y - from.y);

	float stepX = (float)(to.x - from.x) / length;
	float stepY = (float)(to.y - from.y) / length;

	WPoint pos;
	pos.x = (float)from.x;
	pos.y = (float)from.y;

	while (true) {
		// snap to the centre of the 10x10 cell of the magnified brush
		WPoint pt((float)((int)pos.x / 10 * 10 + 5), (float)((int)pos.y / 10 * 10 + 5));
		Draw((int)pt.x, (int)pt.y, pBrush, false);

		if ((length == abs(to.x - from.x) && pos.x == to.x) || (length == abs(to.y - from.y) && pos.y == to.y)) {
			break;
		}

		pos.x += stepX;
		pos.y += stepY;
	}

	m_refreshFlags |= 4;
}

// ucceditbrushdlg.cpp:67
Bitmap* CUccEditBrush::GetTexture32BPP(void) {
	Convert24to32(m_pTexture, m_pTexture32, NULL);
	return m_pTexture32;
}

// ucceditbrushdlg.cpp:73
Bitmap* CUccEditBrush::ProduceOriginalBrush(void) {
	unsigned char r, g, b;

	m_originalBrush.Create(24, 24, 32);

	int destY = 0;
	for (int y = 0; y < 240; y += 10) {
		int destX = 0;
		for (int x = 0; x < 240; x += 10) {
			m_pTexture->GetPixel(x, y, r, g, b);
			m_originalBrush.SetPixel(destX, destY, r, g, b, 0xff);
			destX++;
		}
		destY++;
	}

	return &m_originalBrush;
}

// ucceditbrushdlg.cpp:95 (run-time type and factory registration of FrUccEditBrushDlg)
const WRTTI FrUccEditBrushDlg::m_RTTI("FrUccEditBrushDlg", &FrForm::m_RTTI);

IObject* FrUccEditBrushDlgMakeInstance(void) {
	return new FrUccEditBrushDlg;
}

struct __sFrUccEditBrushDlg {
	__sFrUccEditBrushDlg(void) {
		ObjectFactory().AddObjectFunctor(FrUccEditBrushDlgMakeInstance, "FrUccEditBrushDlg");
	}
};
static __sFrUccEditBrushDlg s_registerFrUccEditBrushDlg;

// ucceditbrushdlg.cpp:97
const sFRESH_MSGMAP* FrUccEditBrushDlg::GetMessageMap(void) const {
	return &FrUccEditBrushDlg::_MsgMap;
}

// ucceditbrushdlg.cpp:98
sFRESH_MSGMAP FrUccEditBrushDlg::_MsgMap = { &FrForm::_MsgMap, &FrUccEditBrushDlg::_MsgEntries[0] };

sFRESH_ENTRY FrUccEditBrushDlg::_MsgEntries[] = {
	{ "canvas", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrUccEditBrushDlg::OnCanvasInit },
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccEditBrushDlg::OnOKBtnUp },
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrUccEditBrushDlg::OnCancelBtnUp },

	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ucceditbrushdlg.cpp:107
FrUccEditBrushDlg::FrUccEditBrushDlg(void) {
	m_brushIndex = -2;
	m_defaultBrush[0].Create(10, 10, 32);
	m_defaultBrush[1].Create(10, 10, 32);

	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			m_defaultBrush[0].SetPixel(x, y, 0, 0, 0, 0xff);
			m_defaultBrush[1].SetPixel(x, y, 0xff, 0xff, 0xff, 0xff);
		}
	}

	m_pCanvasImage = 0;
}

// ucceditbrushdlg.cpp:128
FrUccEditBrushDlg::~FrUccEditBrushDlg(void) {
}

// ucceditbrushdlg.cpp:132
void FrUccEditBrushDlg::InitNew(void) {
	Bitmap* pTexture = new Bitmap(240, 240, 32);

	for (int y = 0; y < 240; y++) {
		for (int x = 0; x < 240; x++) {
			pTexture->SetPixel(x, y, 0xff, 0xff, 0xff, 0xff);
		}
	}

	m_editBrush.SetTexture(pTexture);

	delete pTexture;

	m_pCanvasImage = m_editBrush.GetTexture();

	m_pCanvas->SetBgImg(m_pCanvasImage);
}

// ucceditbrushdlg.cpp:158
void FrUccEditBrushDlg::InitModify(void) {
	unsigned char r, g, b;

	Bitmap* pTexture = new Bitmap(240, 240, 32);
	Bitmap* pBrush = UccBrushList(m_brushType)->GetBrush(m_brushIndex);
	if (pBrush == NULL) {
		delete pTexture;
		return;
	}

	// each pixel of the 24x24 brush becomes a 10x10 cell of the canvas
	for (int y = 0, srcY = 0; y < 240; srcY++, y += 10) {
		for (int x = 0, srcX = 0; x < 240; srcX++, x += 10) {
			pBrush->GetPixel(srcX, srcY, r, g, b);

			for (int destY = y; destY < y + 10; destY++) {
				for (int destX = x; destX < x + 10; destX++) {
					pTexture->SetPixel(destX, destY, r, g, b, 0xff);
				}
			}
		}
	}

	m_editBrush.SetTexture(pTexture);

	delete pTexture;

	m_pCanvasImage = m_editBrush.GetTexture();

	m_pCanvas->SetBgImg(m_pCanvasImage);
}

// ucceditbrushdlg.cpp:200
void FrUccEditBrushDlg::SetIndex(eBrushType type, int index) {
	if (m_brushIndex == -2) {
		m_brushIndex = index;
		m_brushType = type;
		if (m_brushIndex == -1) {
			InitNew();
		} else {
			InitModify();
		}
	}
}

// ucceditbrushdlg.cpp:217
void FrUccEditBrushDlg::OnCanvasInit(intptr_t param) {
	m_pCanvas = DYNAMIC_CAST(FrArea, param);
	m_editBrush.SetArea(m_pCanvas);
}

// ucceditbrushdlg.cpp:223
void FrUccEditBrushDlg::OnOKBtnUp(void) {
	if (m_brushIndex != -2) {
		Bitmap* pBrush = m_editBrush.ProduceOriginalBrush();

		GetGDI();

		if (m_brushIndex == -1) {
			UccBrushList(m_brushType)->AddBrush(pBrush);
		} else {
			UccBrushList(m_brushType)->SetBrush(m_brushIndex, pBrush);

			RefreshTexCache(UccBrushList(m_brushType)->GetBrush(m_brushIndex));
		}
	}

	OnFreshOkay();
}

// ucceditbrushdlg.cpp:245
void FrUccEditBrushDlg::OnCancelBtnUp(void) {
	OnFreshCancel();
}

// ucceditbrushdlg.cpp:250
void FrUccEditBrushDlg::OnProc(const float dt) {
	const WPoint& mouse = g_pFresh->GetManager()->GetMousePos();

	if (m_editBrush.IsMouseOver(mouse)) {
		const WRect& rc = m_pCanvas->GetRect();

		float left = rc.x, top = rc.y;

		// snap to the centre of the 10x10 cell under the mouse
		WPoint pt;
		pt.x = (int)(mouse.x - left) / 10 * 10.0f + left + 5.0f;
		pt.y = (int)(mouse.y - top) / 10 * 10.0f + top + 5.0f;

		if (g_input->GetButton(LEFT_BUTTON) > 0) {
			m_editBrush.SetDefaultBrush(&m_defaultBrush[0]);
			m_editBrush.MouseEventHandler(LEFT_BUTTON, g_input->GetButton(LEFT_BUTTON), pt);
		} else if (g_input->GetButton(RIGHT_BUTTON) > 0) {
			m_editBrush.SetDefaultBrush(&m_defaultBrush[1]);
			m_editBrush.MouseEventHandler(RIGHT_BUTTON, g_input->GetButton(RIGHT_BUTTON), pt);
		}
	}

	m_editBrush.Process();
}

// ucceditbrushdlg.h:28
void CUccEditBrush::Refresh2D(void) {
}

// ucceditbrushdlg.h:29
void CUccEditBrush::Refresh3D(void) {
}
