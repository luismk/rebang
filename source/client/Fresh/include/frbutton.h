// Original: d:\build\custom\temp\code\client\fresh\include\frbutton.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRBUTTON_H
#define PG_CLIENT_FRESH_INCLUDE_FRBUTTON_H

#include "..\..\defines.h"
#include "frwnd.h"

class Bitmap;
class FrGuiItem;

// A push, check or radio button. Layout, access and methods from the PDB
// (size 0x11c, base FrWnd, no new virtual). The bodies written here are the
// inline ones whose COMDATs carry frbutton.h line marks (lines 32-55).
class FrButton : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	enum eButMode {
		NORMAL = 0,
		OVER = 1,
		PRESSED = 2,
		BLINK = 3
	};

	enum eButStyle {
		BT_NONE = 0,
		BT_NORMAL = 1,
		BT_3TYPES = 2,
		BT_ALWAYSON = 3
	};

	enum eButPushStyle {
		BP_NONE = 0,
		BP_PUSHBUTTON = 1,
		BP_CHECKBUTTON = 2,
		BP_RADIOBUTTON = 3
	};

	enum eButDrawStyle {
		BD_NONE = 0,
		BD_OWNERDRAW = 1
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frbutton.h:32

	FrButton(void);
	virtual ~FrButton(void);

	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent, bool bParseParam = true);
	void InitExtern(const Bitmap* pBmpNormal, const Bitmap* pBmpOver, const Bitmap* pBmpSelected, eButStyle style,
		eButPushStyle pushStyle);
	void SetButtonImg(const char* name, eButMode mode);
	void SetButtonBelowImg(const char* name, eButMode mode);
	void SetStatus(eButMode status);
	eButMode GetStatus(void) { return m_status; }                  // frbutton.h:47
	void SetPushDelay(float delay) {                               // frbutton.h:49
		m_pushDelay = delay;
		m_pushedTime = delay + 0.1f;
	}
	void UseRightButton(bool use) { m_useRightButton = use; }     // frbutton.h:53
	void SetStyle(eButStyle style) { m_style = style; }           // frbutton.h:55
	void EnableBlink(bool enable);
	virtual void Enable(bool enable);

protected:
	virtual void OnSetCursor(bool bInClient, const WPoint& mousePos);
	virtual bool OnLButtonDown(const WPoint& mousePos);
	virtual bool OnLButtonUp(const WPoint& mousePos);
	virtual bool OnRButtonDown(const WPoint& mousePos);
	virtual bool OnRButtonUp(const WPoint& mousePos);
	virtual void OnDraw(void);
	virtual void OnProc(const float deltaTime);
	virtual void PreCreateWindow(FrWndManager* pManager, unsigned long dwStyle, const WRect& rect, FrWnd* pParentWnd);

	FrGuiItem* m_pItem;                  // 0xd4
	const Bitmap* m_pBitmap[4];          // 0xd8
	const Bitmap* m_pBtnBg[4];           // 0xe8
	eButMode m_status;                   // 0xf8
	eButStyle m_style;                   // 0xfc
	eButPushStyle m_pushStyle;           // 0x100
	eButDrawStyle m_drawStyle;           // 0x104
	float m_pushDelay;                   // 0x108
	float m_pushedTime;                  // 0x10c
	float m_fBlinktime;                  // 0x110
	bool m_bPressed;                     // 0x114
	bool m_bPrevPressed;                 // 0x115
	bool m_useRightButton;               // 0x116
	bool m_blink;                        // 0x117
	bool m_blinkEnable;                  // 0x118
};

CHECK_SIZE(FrButton, 0x11c);
BEGIN_LAYOUT_CHECK(FrButton)
	CHECK_FIELD(FrButton, m_pItem, 0xd4);
	CHECK_FIELD(FrButton, m_pBitmap, 0xd8);
	CHECK_FIELD(FrButton, m_pBtnBg, 0xe8);
	CHECK_FIELD(FrButton, m_status, 0xf8);
	CHECK_FIELD(FrButton, m_style, 0xfc);
	CHECK_FIELD(FrButton, m_pushStyle, 0x100);
	CHECK_FIELD(FrButton, m_drawStyle, 0x104);
	CHECK_FIELD(FrButton, m_pushDelay, 0x108);
	CHECK_FIELD(FrButton, m_pushedTime, 0x10c);
	CHECK_FIELD(FrButton, m_fBlinktime, 0x110);
	CHECK_FIELD(FrButton, m_bPressed, 0x114);
	CHECK_FIELD(FrButton, m_bPrevPressed, 0x115);
	CHECK_FIELD(FrButton, m_useRightButton, 0x116);
	CHECK_FIELD(FrButton, m_blink, 0x117);
	CHECK_FIELD(FrButton, m_blinkEnable, 0x118);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRBUTTON_H
