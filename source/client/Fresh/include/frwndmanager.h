// Original: d:\build\custom\temp\code\client\fresh\include\frwndmanager.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRWNDMANAGER_H
#define PG_CLIENT_FRESH_INCLUDE_FRWNDMANAGER_H

#include <list>
#include <string>

#include "..\..\defines.h"
#include "../client/Wangreal/include/wtypes.h"
#include "../client/ProjectG/ChatMsg.h"

class Bitmap;
class CChatMsg;
class FrCmdTarget;
class FrCursor;
class FrDesktop;
class FrEdit;
class FrElementDoc;
class FrEmoticon;
class FrGraphicInterface;
class FrGuiItem;
class FrScrollBar;
class FrWnd;

// The mouse and keyboard state the window manager hands to the windows (PDB
// layout, size 0x28). Declared here, next to its only owner; no inline code
// of it carries line marks.
struct FrInputState {
	unsigned long mouse;        // 0x0
	bool hoverChecked;          // 0x4
	WPoint mousePos;            // 0x8
	WPoint oldMousePos;         // 0x10
	float wheelDelta;           // 0x18
	FrWnd* keyFocused;          // 0x1c
	FrScrollBar* wheelFocused;  // 0x20
	CChatMsg* im;               // 0x24
};

CHECK_SIZE(FrInputState, 0x28);
CHECK_FIELD(FrInputState, mouse, 0x0);
CHECK_FIELD(FrInputState, hoverChecked, 0x4);
CHECK_FIELD(FrInputState, mousePos, 0x8);
CHECK_FIELD(FrInputState, oldMousePos, 0x10);
CHECK_FIELD(FrInputState, wheelDelta, 0x18);
CHECK_FIELD(FrInputState, keyFocused, 0x1c);
CHECK_FIELD(FrInputState, wheelFocused, 0x20);
CHECK_FIELD(FrInputState, im, 0x24);

// The window manager of Fresh: desktop, focus, capture, topmost list and
// layouts. Layout, access and methods from the PDB (size 0x90, vfptr at 0,
// virtual destructor in slot 0). The bodies written here are the inline ones
// whose COMDATs carry frwndmanager.h line marks (lines 70-111).
class FrWndManager {
	LAYOUT_CHECK_FRIEND;
public:
	enum eFadeState {
		OPENING = 0,
		OPENED = 1
	};

	FrWndManager(FrElementDoc* pDoc);
	virtual ~FrWndManager(void);

	bool Init(const char* wallPaper, bool exclusiveKey);
	void Display(bool drawDesktop);
	void Process(const float deltaTime);
	FrElementDoc* GetDocument(void) const;
	const Bitmap* GetBitmap(const char* resource, const char* id) const;
	void CreateToolTip(void);
	void DestroyToolTip(void);
	bool CreateLayout(const char* layout, FrCmdTarget* pOwner, bool first);
	void CloseLayout(void);
	bool IsValidWindow(FrWnd* pWnd);
	bool AddTopmostWindow(FrWnd* pWnd);
	bool DeleteTopmostWindow(FrWnd* pWnd);
	FrWnd* GetModalForm(void);
	FrWnd* DoCreate(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent, FrCmdTarget* pOwner);

	FrDesktop* GetDesktop(void) { return m_pDesktop; }                    // frwndmanager.h:70
	unsigned int& RefIndex(void) { return m_nRefIndex; }                  // frwndmanager.h:71
	FrGraphicInterface* GetGDI(void) { return m_pDevice; }                // frwndmanager.h:72
	const WPoint& GetMousePos(void) const { return m_istate.mousePos; }   // frwndmanager.h:73
	FrEmoticon* GetEmoticon(void) { return m_pEmoticon; }                 // frwndmanager.h:74
	FrWnd* HandOverViewFocus(void);
	bool SetKeyFocus(FrWnd* pWnd, bool resetPrevImeData);
	FrWnd* GetKeyFocused(void) { return m_istate.keyFocused; }            // frwndmanager.h:77
	bool CanGetKeyFocus(FrWnd* pWnd);
	bool ResetKeyFocus(FrWnd* pWnd);
	bool MoveKeyFocusToNext(bool resetPrevImeData);
	FrScrollBar* GetWheelFocus(void) { return m_istate.wheelFocused; }    // frwndmanager.h:81
	void SetWheelFocus(FrScrollBar* pScrBar);
	void SetCursor(int hCursor) { m_cursorIndex = hCursor; }              // frwndmanager.h:83
	int GetCursor(void) { return m_cursorIndex; }                         // frwndmanager.h:84
	void MoveCursor(FrWnd* pParent, const char* name);
	const char* GetLayoutID(void) { return m_layoutID.c_str(); }          // frwndmanager.h:86
	bool SetCapture(FrWnd* pWnd);
	FrWnd* GetCapture(void) { return m_pCaptured; }                       // frwndmanager.h:88
	bool ReleaseCapture(FrWnd* pWnd);
	WPoint GetCreatePosition(const WSize& rectSize);
	void ConfineRect(WRect& dr);
	void RemoveFade(void);
	void CloseWindow(FrWnd* pWnd, bool bFade);
	void CloseForm(bool bFade);
	bool HasEscKeyWindow(void);
	bool HasValidWindow(void);
	void SetExclusiveKey(bool set);
	bool IsKeyExclusive(void) { return m_exclusiveKey; }                  // frwndmanager.h:98
	void ResetKey(void) {                                                 // frwndmanager.h:99
		if (m_istate.im) m_istate.im->Reset();
	}
	void ResetTopFocus(void);
	float GetTextWidth(const char* text);
	float PrintText(const WPoint& pos, unsigned long align, const char* text, float limit, unsigned long emoDiffuse);
	float PrintText11(const WPoint& pos, unsigned long align, const char* text, float limit, unsigned long emoDiffuse);
	FrInputState* GetInputState(void) { return &m_istate; }             // never emitted; body inferred
	void IME_ShowCandWindow(FrEdit* pFocusedEdit, WPoint& caretPos);
	std::list<FrWnd*>& GetTopWndList(void) { return m_topmostList; }      // frwndmanager.h:108
	void HidePrivacy(bool hide) { m_hidePrivacy = hide; }                 // frwndmanager.h:110
	bool HidePrivacy(void) { return m_hidePrivacy; }                      // frwndmanager.h:111

protected:
	void CheckSystemStatus(void);
	void ProcessHotKey(void);
	void ProcessKey(FrWnd* pOldKeyFocused);

	FrElementDoc* m_pDoc;                 // 0x4
	FrDesktop* m_pDesktop;                // 0x8
	FrCursor* m_pCursor;                  // 0xc
	FrInputState m_istate;                // 0x10
	bool m_exclusiveKey;                  // 0x38
	std::list<FrWnd*> m_topmostList;      // 0x3c
	std::string m_layoutID;               // 0x48
	FrWnd* m_pCaptured;                   // 0x64
	FrGraphicInterface* m_pDevice;        // 0x68
	unsigned int m_nRefIndex;             // 0x6c
	int m_cursorIndex;                    // 0x70
	FrEmoticon* m_pEmoticon;              // 0x74
	float m_fadeoutAlpha;                 // 0x78
	eFadeState m_state;                   // 0x7c
	FrEdit* m_pFocusedEdit;               // 0x80
	WPoint m_caretPos;                    // 0x84
	bool m_hidePrivacy;                   // 0x8c
};

CHECK_SIZE(FrWndManager, 0x90);
BEGIN_LAYOUT_CHECK(FrWndManager)
	CHECK_FIELD(FrWndManager, m_pDoc, 0x4);
	CHECK_FIELD(FrWndManager, m_pDesktop, 0x8);
	CHECK_FIELD(FrWndManager, m_pCursor, 0xc);
	CHECK_FIELD(FrWndManager, m_istate, 0x10);
	CHECK_FIELD(FrWndManager, m_exclusiveKey, 0x38);
	CHECK_FIELD(FrWndManager, m_topmostList, 0x3c);
	CHECK_FIELD(FrWndManager, m_layoutID, 0x48);
	CHECK_FIELD(FrWndManager, m_pCaptured, 0x64);
	CHECK_FIELD(FrWndManager, m_pDevice, 0x68);
	CHECK_FIELD(FrWndManager, m_nRefIndex, 0x6c);
	CHECK_FIELD(FrWndManager, m_cursorIndex, 0x70);
	CHECK_FIELD(FrWndManager, m_pEmoticon, 0x74);
	CHECK_FIELD(FrWndManager, m_fadeoutAlpha, 0x78);
	CHECK_FIELD(FrWndManager, m_state, 0x7c);
	CHECK_FIELD(FrWndManager, m_pFocusedEdit, 0x80);
	CHECK_FIELD(FrWndManager, m_caretPos, 0x84);
	CHECK_FIELD(FrWndManager, m_hidePrivacy, 0x8c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRWNDMANAGER_H
