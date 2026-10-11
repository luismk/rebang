// Original: d:\build\custom\temp\code\client\fresh\include\frwnd.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRWND_H
#define PG_CLIENT_FRESH_INCLUDE_FRWND_H

#include <list>
#include <string>

#include "..\..\defines.h"
#include "ObjectFactory.h"
#include "frcmdtarget.h"
#include "wflag.h"
#include "../Wangreal/include/wtypes.h"

// Fresh's cast helper: every object including this header gets its own
// thread-local `__rtti_obj` (PDB S_LTHREAD32, void*), which the cast assigns
// before the IObject::DynamicCast chain (docs/tough_problems/004). The macro
// name is invented; callers need IObject (ObjectFactory.h) visible.
static __declspec(thread) void* __rtti_obj;
#define DYNAMIC_CAST(T, p) ((T*)((__rtti_obj = (void*)(p)) != 0 ? ((IObject*)__rtti_obj)->DynamicCast(&T::m_RTTI) : 0))

// Unchecked cast of a window pointer: a non-null pointer is used as is (no
// IObject::DynamicCast), only a null one is stored into __rtti_obj
// (docs/tough_problems/013). The name is invented.
template <class T>
inline T* FrStaticCast(void* p) { return (T*)(p != 0 ? p : (__rtti_obj = p)); }

class CChatMsg;
class FrEmoticon;
class FrGraphicInterface;
class FrScrollBar;
class FrToolTip;
class FrWnd;
class FrWndManager;
struct FrInputState;

// Callback of FrWnd::EnumerateChildWindow (PDB typedef).
typedef bool (*ENUM_CHILD_WINDOW_CALLBACK)(FrWnd*, void*);

// Window style bits (PDB enum), the `style` of FrWnd::Create.
enum eFrStyle {
	FWS_NONE = 0x0,
	FWS_VISIBLE = 0x1,
	FWS_DISABLED = 0x2,
	FWS_HASTITLE = 0x4,
	FWS_CHILD = 0x8,
	FWS_TOPMOST = 0x10,
	FWS_MOVEFRAME = 0x20,
	FWS_NOMOUSEEVENT = 0x40,
	FWS_KEYEVENT = 0x80,
	FWS_FIXED = 0x100,
	FWS_NODBLCLICK = 0x200,
	FWS_HOVER = 0x400,
	FWS_NOWHEELEVENT = 0x800
};

// Base of every Fresh window. Layout, access and vtable from the PDB
// (size 0xd4; IObject at 0, FrCmdTarget at 4). The virtuals are declared in
// slot order; the bodies written here are the inline ones of frwnd.h whose
// COMDATs the line marks show.
class FrWnd : public IObject, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;
	// FrWndManager creates and destroys the shared tooltip and walks
	// m_childList / m_fadeTime.
	friend class FrWndManager;
	// The factory function of the registration macro (frwnd.cpp:18) calls the
	// protected constructor.
	friend IObject* FrWndMakeInstance(void);
public:
	struct sToolTipData {
		sToolTipData(void) {}
		int bFixWnd;              // 0x0
		WPoint posFixWnd;         // 0x4
		unsigned long style;      // 0xc
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frwnd.h:96

	virtual ~FrWnd(void);

	// vtable slots 2-7
	virtual bool Close(bool bDestroy);
	virtual bool Create(const char* name, const char* text, FrWndManager* pWndManager, unsigned long style,
		const WRect& rect, FrWnd* pParent);
	virtual void PreCreateWindow(FrWndManager* pWndManager, unsigned long style, const WRect& rect, FrWnd* pParent);
	virtual void MoveWindow(const WPoint& pt);
	// frwnd.h:185; branch order not yet compared (its COMDAT is TaskMain's)
	virtual void Enable(bool bEnable) {
		if (!bEnable) {
			m_dwStyle.Enable(0x2);
		} else {
			m_dwStyle.Disable(0x2);
		}
	}
	virtual void SetVisible(bool bVisible);

	void DestroyChild(void);
	bool SendCmdToOwnerTarget(FrCmdTarget* pTarget, eFrCmd cmd, intptr_t param, sFRESH_HANDLER* pHandler);
	bool SendCmdToOwnerTarget(eFrCmd cmd, intptr_t param, sFRESH_HANDLER* pHandler);
	void SetOwner(FrCmdTarget* pOwner);
	void AddChild(FrWnd* pChild);
	FrWndManager* WndManager(void) const { return m_pWndManager; }   // frwnd.h:118
	FrGraphicInterface* GDI(void) const;
	FrEmoticon* Emo(void) const;
	FrWnd* GetParent(void) const;
	FrWnd* FindChild(const char* name);
	FrWnd* FindChild(const FrWnd* pWnd);
	FrWnd* FindChildByName(const char* name);
	FrWnd* FindChildByStyle(unsigned long style);
	FrWnd* FindChildForm(const char* name);
	bool IsChild(const FrWnd* pWnd) const;
	void CloseChild(FrWnd* pChild, bool bDestroy, bool bAll);
	void CloseChildForm(bool bDestroy, bool bAll);
	void RemoveWindow(FrWnd* pWnd);
	void EnumerateChildWindow(ENUM_CHILD_WINDOW_CALLBACK pfn, void* pParam);
	void GetClientRect(WRect& rect) const;
	void SetClientRect(const WRect& rect);
	void ClientToScreen(WPoint& pt) const;
	void ClientToScreen(WRect& rect) const;
	void ScreenToClient(WPoint& pt) const;
	void ScreenToClient(WRect& rect) const;
	void SetRect(const WRect& rect);
	const WRect& GetRect(void) { return m_rect; }   // frwnd.h:147
	// The original wrote SetWindowText etc. under <windows.h>, whose macros
	// made them the A names the PDB records.
	void SetWindowTextA(const char* text);
	void GetWindowTextA(std::string& text) const;
	void GetWindowTextA(char* text, unsigned int size) const;
	unsigned int GetWindowTextLengthA(void) const;
	void SetWindowName(const char* name);
	void GetWindowName(std::string& name) const;
	void GetWindowName(char* name, unsigned int size) const;
	unsigned int GetWindowNameLength(void) const;
	void EnableToolTip(bool bEnable) { m_nFlags.Turn(0x1, bEnable); }   // frwnd.h:160
	bool IsEnableToolTip(void) const { return m_nFlags.GetFlag(0x1); }   // frwnd.h:161
	void SetToolTipText(const std::string& text);
	std::string GetToolTipText(void) const { return m_wndToolTip; }   // frwnd.h:163
	void SetFixTooltipWnd(const WPoint& pt);
	void SetUnFixToolTipWnd(void);
	int IsFixedToolTip(void) const;
	const WPoint& GetToolTipWndPos(void) const;
	void SetToolTipFrameStyle(unsigned long style);
	unsigned long GetToolTipFrameStyle(void) const;
	void OnDisplay(bool b1, bool b2, bool b3);
	void OnProcess(const float dt, FrInputState& input, bool b1, bool b2);
	const WFlags& GetStyle(void) const { return m_dwStyle; }   // frwnd.h:178
	float GetAlpha(void) const { return m_wndAlpha; }   // frwnd.h:179
	void SetAlpha(float alpha);
	float GetAlpha2(void) const;
	void SetAlpha2(float alpha);
	void SetAlpha2ToChild(float alpha);
	bool IsEnabled(void) const { return !m_dwStyle.GetFlag(0x2); }   // frwnd.h:186
	bool IsVisible(void) const { return m_dwStyle.GetFlag(0x1); }   // frwnd.h:188
	void SetKeyEvent(bool bEnable) { m_dwStyle.Turn(0x80, bEnable); }   // frwnd.h:189
	void SetTopmost(bool bTopmost);
	void SetFixed(bool bFixed) { m_dwStyle.Turn(0x100, bFixed); }   // frwnd.h:191
	bool IsFixed(void) { return m_dwStyle.GetFlag(0x100); }   // frwnd.h:192
	void UseDblClick(bool bUse) {   // frwnd.h:193
		if (!bUse) {
			m_dwStyle.Enable(0x200);
		} else {
			m_dwStyle.Disable(0x200);
		}
	}
	void EnableHover(bool bEnable, float time);
	void SetWheelEvent(bool bEnable);
	void HidePrivacy(bool bHide) { m_nFlags.Turn(0x1000, bHide); }   // frwnd.h:197
	void SetFadeout(bool bFadeout);
	void SetElementFadeOut(bool bFadeout);
	void SetViewFocus(bool bFocus);
	void ResetViewFocus(FrWnd* pWnd);
	FrWnd* FindViewFocused(bool b);
	bool IsViewFocused(void) const;
	bool IsTopFocus(void) const;
	bool SetKeyFocus(bool bFocus);
	bool HasKeyFocus(void);
	bool ResetKeyFocus(void);
	void FindNextTopFocus(bool b);
	bool SetCapture(void);
	FrWnd* GetCapture(void);
	bool ReleaseCapture(void);
	void SetCursor(int cursor);
	int GetCursor(void) const;
	void MoveCursor(const char* name);
	void SetPushSound(const char* sound);
	bool PlayPushSound(void);
	FrScrollBar* GetScrollBar(void) { return m_pScrBar; }   // frwnd.h:231
	void SetWheelFocus(void);

protected:
	FrWnd(void);

	// vtable slots 8-22 (frwnd.h:236-251)
	virtual void OnDraw(void);
	virtual void OnProc(const float dt) {}                           // frwnd.h:237
	// frwnd.h:238; its body is still out of line in client/ProjectG/TaskMain.cpp
	virtual void OnResize(void);
	virtual void OnMouseMove(const WPoint& pt) {}                    // frwnd.h:240
	virtual bool OnLButtonUp(const WPoint& pt) { return false; }     // frwnd.h:241
	virtual bool OnLButtonDown(const WPoint& pt) { return false; }   // frwnd.h:242
	virtual bool OnRButtonUp(const WPoint& pt) { return false; }     // frwnd.h:243
	virtual bool OnRButtonDown(const WPoint& pt) { return false; }   // frwnd.h:244
	virtual void OnDblClick(const WPoint& pt) {}                     // frwnd.h:245
	virtual void OnWheel(FrInputState& input) {}                     // frwnd.h:246
	virtual const char* OnSelectText(const FrInputState* pInput) { return 0; }   // frwnd.h:247
	virtual void OnKeyFocus(CChatMsg* pMsg) {}                       // frwnd.h:248
	virtual void OnSetCursor(bool b, const WPoint& pt) {}            // frwnd.h:249
	virtual void EnableKeyFocus(FrInputState& input) { m_nFlags.Enable(0x10); }   // frwnd.h:250
	virtual void SetIconRect(const WRect& rect) {}                   // frwnd.h:251

private:
	void DoFadeDisplay(void);
	void DoFadeProcess(const float dt, bool b);
	void CheckHover(float dt, bool b, bool& bHover);
	static FrToolTip* ToolTip(void);
	static bool IsToolTipInstantiated(void);
	static void CreateToolTip(FrWndManager* pWndManager);
	static void DestroyToolTip(void);

public:
	WFlags m_nFlags;                      // 0x8

protected:
	FrWnd* m_pParentWnd;                  // 0xc
	FrCmdTarget* m_pOwner;                // 0x10
	std::list<FrWnd*> m_childList;        // 0x14
	WRect m_rect;                         // 0x20
	WFlags m_dwStyle;                     // 0x30
	std::string m_wndText;                // 0x34
	std::string m_wndName;                // 0x50
	float m_wndAlpha;                     // 0x6c
	float m_wndAlpha2;                    // 0x70
	unsigned int m_nRefID;                // 0x74
	FrWndManager* m_pWndManager;          // 0x78
	FrScrollBar* m_pScrBar;               // 0x7c
	float m_fadeTime;                     // 0x80
	WRect m_iconRect;                     // 0x84
	const char* m_szPushSound;            // 0x94
	float m_dblClickTimeout;              // 0x98
	bool m_dblClicked;                    // 0x9c
	WPoint m_dblClickPos;                 // 0xa0
	bool m_hoverOn;                       // 0xa8
	float m_hoverTime;                    // 0xac
	float m_accHoverTime;                 // 0xb0

private:
	std::string m_wndToolTip;             // 0xb4
	static FrToolTip* m_pToolTip;
	sToolTipData* m_pToolTipData;         // 0xd0
};

CHECK_SIZE(FrWnd::sToolTipData, 0x10);
CHECK_SIZE(FrWnd, 0xd4);
BEGIN_LAYOUT_CHECK(FrWnd)
	CHECK_FIELD(FrWnd, m_nFlags, 0x8);
	CHECK_FIELD(FrWnd, m_pParentWnd, 0xc);
	CHECK_FIELD(FrWnd, m_pOwner, 0x10);
	CHECK_FIELD(FrWnd, m_childList, 0x14);
	CHECK_FIELD(FrWnd, m_rect, 0x20);
	CHECK_FIELD(FrWnd, m_dwStyle, 0x30);
	CHECK_FIELD(FrWnd, m_wndText, 0x34);
	CHECK_FIELD(FrWnd, m_wndName, 0x50);
	CHECK_FIELD(FrWnd, m_wndAlpha, 0x6c);
	CHECK_FIELD(FrWnd, m_wndAlpha2, 0x70);
	CHECK_FIELD(FrWnd, m_nRefID, 0x74);
	CHECK_FIELD(FrWnd, m_pWndManager, 0x78);
	CHECK_FIELD(FrWnd, m_pScrBar, 0x7c);
	CHECK_FIELD(FrWnd, m_fadeTime, 0x80);
	CHECK_FIELD(FrWnd, m_iconRect, 0x84);
	CHECK_FIELD(FrWnd, m_szPushSound, 0x94);
	CHECK_FIELD(FrWnd, m_dblClickTimeout, 0x98);
	CHECK_FIELD(FrWnd, m_dblClicked, 0x9c);
	CHECK_FIELD(FrWnd, m_dblClickPos, 0xa0);
	CHECK_FIELD(FrWnd, m_hoverOn, 0xa8);
	CHECK_FIELD(FrWnd, m_hoverTime, 0xac);
	CHECK_FIELD(FrWnd, m_accHoverTime, 0xb0);
	CHECK_FIELD(FrWnd, m_wndToolTip, 0xb4);
	CHECK_FIELD(FrWnd, m_pToolTipData, 0xd0);
END_LAYOUT_CHECK

// frwnd.h:301
inline bool FrWnd::IsToolTipInstantiated(void) {
	return m_pToolTip != 0;
}

#endif // PG_CLIENT_FRESH_INCLUDE_FRWND_H
