// Original: d:\build\custom\temp\code\client\fresh\source\frbutton.cpp
#include <stdlib.h>
#include <string.h>

#include "frbutton.h"
#include "frelement.h"
#include "frwndmanager.h"
#include "ObjectFactory.h"
#include "bitmap.h"
#include "singleton.h"
#include "CapturedBg.h"
#include "frcursor.h"
#include "frgraphicinterface.h"

// frbutton.cpp:19 (run-time type and factory registration of FrButton)
const WRTTI FrButton::m_RTTI("FrButton", &FrWnd::m_RTTI);

IObject* FrButtonMakeInstance(void) {
	return new FrButton;
}

struct __sFrButton {
	__sFrButton(void) {
		ObjectFactory().AddObjectFunctor(FrButtonMakeInstance, "FrButton");
	}
};
static __sFrButton s_registerFrButton;

// frbutton.cpp:22
FrButton::FrButton(void) {
	m_bPressed = false;
	m_bPrevPressed = false;
	m_useRightButton = false;
	m_blink = false;
	m_blinkEnable = false;
	m_status = NORMAL;
	m_style = BT_NORMAL;
	m_pushStyle = BP_PUSHBUTTON;
	m_drawStyle = BD_NONE;
	m_szPushSound = "ui_icon_click";
	SetPushDelay(1.2f);
	m_fBlinktime = 0.0f;
	m_pItem = 0;
	m_pBitmap[0] = m_pBitmap[1] = m_pBitmap[2] = m_pBitmap[3] = m_pBtnBg[0] = m_pBtnBg[1] = m_pBtnBg[2] = m_pBtnBg[3] = 0;
}

// frbutton.cpp:53
FrButton::~FrButton(void) {
}

// frbutton.cpp:58
void FrButton::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent, bool bParseParam) {
	FrElementDoc* pDoc = pManager->GetDocument();

	m_pItem = &item;

	if (bParseParam) {
		std::map<std::string, std::string>& param = item.m_param;

		m_pBitmap[NORMAL] = pDoc->GetBitmap(param["normal"]);
		m_pBitmap[OVER] = pDoc->GetBitmap(param["over"]);
		m_pBitmap[PRESSED] = pDoc->GetBitmap(param["selected"]);
		m_pBitmap[BLINK] = pDoc->GetBitmap(param["blink"]);
		m_pBtnBg[NORMAL] = pDoc->GetBitmap(param["below_normal"]);
		m_pBtnBg[OVER] = pDoc->GetBitmap(param["below_over"]);
		m_pBtnBg[PRESSED] = pDoc->GetBitmap(param["below_selected"]);
		m_pBtnBg[BLINK] = pDoc->GetBitmap(param["below_blink"]);

		float pushDelay = (float)atof(param["pushdelay"].c_str());
		if (pushDelay != 0.0f)
			SetPushDelay(pushDelay);

		if (m_pBitmap[BLINK])
			m_blinkEnable = true;

		if (param.find("style") != param.end())
			m_style = (eButStyle)atoi(param["style"].c_str());

		if (param.find("pushstyle") != param.end())
			m_pushStyle = (eButPushStyle)atoi(param["pushstyle"].c_str());

		if (param.find("drawstyle") != param.end())
			m_drawStyle = (eButDrawStyle)atoi(param["drawstyle"].c_str());

		if (param.find("tooltip") != param.end())
			SetToolTipText(param["tooltip"]);
	}

	_RectangleSHORT& rc = item.m_rect;

	WRect rect;
	if (bParseParam && m_pBitmap[NORMAL])
		rect = WRect(rc.left, rc.top, m_pBitmap[NORMAL]->Width(), m_pBitmap[NORMAL]->Height());
	else
		rect = WRect(rc.left, rc.top, 0, 0);

	Create(item.m_caption.c_str(), item.m_name.c_str(), pManager, 0x10001, rect, pParent);
}

// frbutton.cpp:124
void FrButton::InitExtern(const Bitmap* pBmpNormal, const Bitmap* pBmpOver, const Bitmap* pBmpSelected, eButStyle style,
	eButPushStyle pushStyle) {
	m_pBitmap[0] = pBmpNormal;
	m_pBitmap[1] = pBmpOver;
	m_pBitmap[2] = pBmpSelected;

	m_style = style;
	m_pushStyle = pushStyle;

	m_rect.w = (float)pBmpNormal->Width();
	m_rect.h = (float)pBmpNormal->Height();
}

// frbutton.cpp:138
void FrButton::SetButtonImg(const char* name, eButMode mode) {
	if (name == 0)
		return;

	const Bitmap* bitmap;

	FrElementDoc* pDoc = WndManager()->GetDocument();
	if (pDoc)
		bitmap = pDoc->GetBitmap(name);

	m_pBitmap[mode] = bitmap;

	m_rect.w = (float)bitmap->Width();
	m_rect.h = (float)bitmap->Height();
}

// frbutton.cpp:156
void FrButton::SetButtonBelowImg(const char* name, eButMode mode) {
	if (name == 0)
		return;

	const Bitmap* bitmap;

	FrElementDoc* pDoc = WndManager()->GetDocument();
	if (pDoc)
		bitmap = pDoc->GetBitmap(name);

	m_pBtnBg[mode] = bitmap;
}

// frbutton.cpp:172
void FrButton::Enable(bool enable) {
	if (!enable)
		m_dwStyle.Enable(0x2);
	else
		m_dwStyle.Disable(0x2);

	if (m_status == OVER)
		m_status = NORMAL;
}

// frbutton.cpp:180
void FrButton::OnSetCursor(bool bInClient, const WPoint& mousePos) {
	switch (m_pushStyle) {
	case BP_CHECKBUTTON:
		if (m_bPressed != m_bPrevPressed) {
			m_bPrevPressed = m_bPressed;
			if (m_bPressed)
				m_status = (m_status == PRESSED) ? NORMAL : PRESSED;
		}

		if (bInClient) {
			if (m_status != PRESSED) {
				m_status = OVER;
				SetCursor(FrCursor::ROLL_OVER);
			} else
				SetCursor(FrCursor::SELECT);
		} else {
			if (m_status != PRESSED)
				m_status = NORMAL;
		}
		break;

	case BP_RADIOBUTTON:
		if (m_bPrevPressed)
			m_status = PRESSED;
		else
			m_status = m_bPressed ? PRESSED : NORMAL;

		if (bInClient) {
			if (m_status != PRESSED) {
				m_status = OVER;
				SetCursor(FrCursor::ROLL_OVER);
			} else
				SetCursor(FrCursor::SELECT);
		} else {
			if (m_status != PRESSED)
				m_status = NORMAL;
		}
		break;

	default:
		if (bInClient) {
			if (m_bPressed) {
				m_status = PRESSED;
				SetCursor(FrCursor::SELECT);
			} else {
				m_status = OVER;
				SetCursor(FrCursor::ROLL_OVER);
			}
		} else
			m_status = NORMAL;
		break;
	}

	if (bInClient == true && m_useRightButton)
		SetCursor(FrCursor::INFO);
}

// frbutton.cpp:268
bool FrButton::OnLButtonDown(const WPoint& mousePos) {
	if (m_pushedTime > m_pushDelay) {
		if (m_pushStyle == BP_PUSHBUTTON)
			m_pushedTime = 0.1f;

		if (SetCapture()) {
			if (m_pushStyle == BP_RADIOBUTTON) {
				if (m_bPrevPressed == true)
					return false;

				m_bPrevPressed = true;
			}

			m_bPressed = true;
			if (m_rect.IsInRect(mousePos)) {
				PlayPushSound();
				SendCmdToOwnerTarget(FRCMD_LBUTTONDOWN, 0, 0);
			}
		}
	}

	return false;
}

// frbutton.cpp:299
bool FrButton::OnLButtonUp(const WPoint& mousePos) {
	if ((m_pushStyle != BP_PUSHBUTTON || m_rect.IsInRect(mousePos)) && m_bPressed == true)
		SendCmdToOwnerTarget(FRCMD_LBUTTONUP, 0, 0);

	m_bPressed = false;
	ReleaseCapture();

	return false;
}

// frbutton.cpp:312
bool FrButton::OnRButtonDown(const WPoint& mousePos) {
	if (m_useRightButton) {
		if (m_pushedTime > m_pushDelay) {
			if (m_pushStyle == BP_PUSHBUTTON)
				m_pushedTime = 0.1f;

			if (SetCapture()) {
				if (m_pushStyle == BP_RADIOBUTTON) {
					if (m_bPrevPressed == true)
						return false;

					m_bPrevPressed = true;
				}

				m_bPressed = true;
				if (m_rect.IsInRect(mousePos)) {
					PlayPushSound();
					SendCmdToOwnerTarget(FRCMD_RBUTTONDOWN, 0, 0);
				}
			}
		}
	}

	return false;
}

// frbutton.cpp:346
bool FrButton::OnRButtonUp(const WPoint& mousePos) {
	if (m_useRightButton) {
		if ((m_pushStyle != BP_PUSHBUTTON || m_rect.IsInRect(mousePos)) && m_bPressed == true)
			SendCmdToOwnerTarget(FRCMD_RBUTTONUP, 0, 0);

		m_bPressed = false;
		ReleaseCapture();
	}

	return false;
}

// frbutton.cpp:362
void FrButton::OnDraw(void) {
	FrWnd::OnDraw();

	unsigned long wndColor = 0;
	eButMode mode = NORMAL;

	switch (m_style) {
	case BT_3TYPES:
		mode = m_status;
		if (m_dwStyle.GetFlag(0x2))
			wndColor = 0x50b0b0b0;
		else
			wndColor = m_pushedTime > m_pushDelay ? 0xffffffff : 0x50b0b0b0;
		break;

	case BT_NORMAL:
		if (m_dwStyle.GetFlag(0x2))
			wndColor = 0x50b0b0b0;
		else {
			switch (m_status) {
			case NORMAL:
				wndColor = m_pushedTime > m_pushDelay ? 0xffffffff : 0x50b0b0b0;
				break;
			case OVER:
				wndColor = m_pushedTime > m_pushDelay ? 0xffe0e0e0 : 0x50b0b0b0;
				break;
			default:
				wndColor = m_pushedTime > m_pushDelay ? 0xffb0b0b0 : 0x50b0b0b0;
				break;
			}
		}
		break;

	case BT_ALWAYSON:
		wndColor = m_pushedTime > m_pushDelay ? 0xffffffff : 0x50b0b0b0;
		break;
	}

	if (!IsViewFocused()) {
		union {
			unsigned long dw;
			unsigned char by[4];
		} color;

		color.dw = wndColor;
		color.by[3] = (unsigned char)(color.by[3] * 0.5f);
		wndColor = color.dw;
	}

	float alphaModifier = m_wndAlpha2 * m_wndAlpha;
	if (m_blinkEnable && m_blink) {
		if (m_pBitmap[BLINK])
			mode = BLINK;
		else
			alphaModifier *= 0.5f;
	}

	if (m_pBtnBg[mode])
		GDI()->DrawTexture(m_pBtnBg[mode], m_rect, FrALPHA(wndColor, alphaModifier), 0);

	GDI()->DrawTexture(m_pBitmap[mode], m_rect, FrALPHA(wndColor, alphaModifier), 0);

	if (m_drawStyle != BD_NONE)
		SendCmdToOwnerTarget(FRCMD_OWNERDRAW, 0, 0);
}

// frbutton.cpp:445
void FrButton::OnProc(const float deltaTime) {
	CCapturedBg* pCapturedBg = WSingleton<CCapturedBg>::Instance();
	if (pCapturedBg && pCapturedBg->IsCaptured())
		return;

	m_pushedTime += deltaTime;

	if (m_blinkEnable) {
		if (m_fBlinktime > 0.7f) {
			m_fBlinktime = 0.0f;
			m_blink = !m_blink;
		}
		m_fBlinktime += deltaTime;
	}
}

// frbutton.cpp:465
void FrButton::PreCreateWindow(FrWndManager* pManager, unsigned long dwStyle, const WRect& rect, FrWnd* pParentWnd) {
	FrWnd::PreCreateWindow(pManager, dwStyle, rect, pParentWnd);

	if (m_pItem == 0)
		return;

	if (m_pItem->m_name.empty())
		return;

	// Disabled once the owner has a handler for every click command.
	sFRESH_HANDLER chk;
	memset(&chk, 0, sizeof(chk));
	SendCmdToOwnerTarget(FRCMD_LBUTTONUP, 0, &chk);
	if (chk.pFn == 0)
		return;

	memset(&chk, 0, sizeof(chk));
	SendCmdToOwnerTarget(FRCMD_LBUTTONDOWN, 0, &chk);
	if (chk.pFn == 0)
		return;

	memset(&chk, 0, sizeof(chk));
	SendCmdToOwnerTarget(FRCMD_RBUTTONUP, 0, &chk);
	if (chk.pFn == 0)
		return;

	memset(&chk, 0, sizeof(chk));
	SendCmdToOwnerTarget(FRCMD_RBUTTONDOWN, 0, &chk);
	if (chk.pFn == 0)
		return;

	m_dwStyle.Enable(0x2);
}

// frbutton.cpp:502
void FrButton::SetStatus(eButMode status) {
	m_status = status;

	if (m_pushStyle == BP_RADIOBUTTON)
		m_bPrevPressed = status == PRESSED;
}

// frbutton.cpp:511
void FrButton::EnableBlink(bool enable) {
	m_blinkEnable = enable;
	m_blink = enable;
}
