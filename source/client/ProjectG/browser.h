// Original: d:\build\custom\temp\code\client\projectg\browser.h
#ifndef PG_CLIENT_PROJECTG_BROWSER_H
#define PG_CLIENT_PROJECTG_BROWSER_H

#include <windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <exdispid.h>

#include <list>
#include <string>
#include <utility>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "AnotherHand.h"

// The event descriptions of CBrowserView's sink entries (browser.cpp).
extern ATL::_ATL_FUNC_INFO WindowClosingInfo;
extern ATL::_ATL_FUNC_INFO DownloadInfo;

// The browser's view: a WebBrowser control hosted in an ATL window, with
// the WebBrowser's DWebBrowserEvents2 sink.
class CBrowserView : public ATL::CWindowImpl<CBrowserView, ATL::CAxWindow>,
	public WTL::CMessageFilter,
	public WTL::CIdleHandler,
	public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
	public ATL::IDispEventSimpleImpl<37, CBrowserView, &DIID_DWebBrowserEvents2> {
public:
	// browser.h:62
	DECLARE_WND_SUPERCLASS(NULL, ATL::CAxWindow::GetWndClassName())

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle(void);

	// browser.h:68
	BEGIN_MSG_MAP(CBrowserView)
		MESSAGE_HANDLER(WM_DESTROY, OnDestory)
	END_MSG_MAP()

	BEGIN_COM_MAP(CBrowserView)
	END_COM_MAP()

	// browser.h:76
	BEGIN_SINK_MAP(CBrowserView)
		SINK_ENTRY_INFO(37, DIID_DWebBrowserEvents2, DISPID_WINDOWCLOSING, OnWindowClosing, &WindowClosingInfo)
		SINK_ENTRY_INFO(37, DIID_DWebBrowserEvents2, DISPID_DOWNLOADBEGIN, OnDownloadBegin, &DownloadInfo)
		SINK_ENTRY_INFO(37, DIID_DWebBrowserEvents2, DISPID_DOWNLOADCOMPLETE, OnDownloadComplete, &DownloadInfo)
	END_SINK_MAP()

	LRESULT OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void __stdcall OnWindowClosing(VARIANT_BOOL bIsChildWindow, VARIANT_BOOL* Cancel);
	void __stdcall OnDownloadBegin(void);
	void __stdcall OnDownloadComplete(void);

	int m_downloadCount; // 0x5C, pending downloads (OnDownloadBegin/OnDownloadComplete)
};
CHECK_FIELD(CBrowserView, m_hWnd, 0x4);
CHECK_FIELD(CBrowserView, m_pfnSuperWindowProc, 0x20);
CHECK_FIELD(CBrowserView, m_dwRef, 0x58);
CHECK_FIELD(CBrowserView, m_downloadCount, 0x5C);
CHECK_SIZE(CBrowserView, 0x60);
CHECK_SIZE(ATL::CWndClassInfo, 0x68);

// browser.h:116. The view's COM object, on the stack (in CBrowser): no
// reference counting.
template <class Base>
class CComObjectStack2 : public ATL::CComObjectStack<Base> {
public:
	// browser.h:118
	CComObjectStack2(void) {
	}

	// browser.h:119
	virtual ~CComObjectStack2(void) {
	}

	// browser.h:121
	STDMETHOD_(ULONG, AddRef)(void) {
		return 1;
	}

	// browser.h:122
	STDMETHOD_(ULONG, Release)(void) {
		return 1;
	}

	// browser.h:124
	STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject) {
		return this->_InternalQueryInterface(iid, ppvObject);
	}
};

// The in-game web browser window. Vtable (0xA08158): ~CBrowser,
// PrepareOpen, GetPostArgument; CHandOwner's vtable at 0x28.
class CBrowser : public WSingleton<CBrowser>, public CHandOwner {
	LAYOUT_CHECK_FRIEND;
public:
	// Page shown by the browser (SetPagetype); enumerator names from the
	// earlier build's source.
	enum eWebPage {
		WEBPAGE_NORMAL,      // 500x600 window
		WEBPAGE_SELFCERTIFY  // 800x600 window, not topmost
	};

	CBrowser(void);
	virtual ~CBrowser(void);

	void Init(HINSTANCE hInstance, HWND hWnd, const RECT& rect);
	void Open(const char* url, bool arg2, bool arg3);
	void OpenPost(const char* url, bool arg2, bool arg3);
	void Close(void);
	void Refresh(void);
	int PreTranslateMessage(MSG* msg);
	void Process(float elapsed);
	void Display(void);
	void DrawCloseButton(tagDRAWITEMSTRUCT* drawItem);
	void DrawBgButton(tagDRAWITEMSTRUCT* drawItem);
	void InitializePostArgument(void);
	void AddPostArgument(const char* name, const char* value);

	// browser.h:188
	void SetPagetype(eWebPage type) {
		m_pageType = type;
	}

	// browser.h:213
	bool IsOpen(void) {
		return m_spBrowser != NULL;
	}

	// browser.h:215
	void ResetCount(void) {
		m_count = 0;
	}

	// browser.h:221
	void SetWaitMsg(const char* msg) {
		m_waitMsg = msg;
	}

protected:
	virtual void PrepareOpen(const char* url, const char* bgImage, bool restoreScreenSize, bool forceWindowed);
	virtual std::string GetPostArgument(void) const;
	virtual void OnAnotherHand(int nWork, void* pData);

	eWebPage m_pageType;            // 0x2C
	HINSTANCE m_hInstance;          // 0x30
	HWND m_hParent;                 // 0x34
	RECT m_rect;                    // 0x38
	bool m_isShown;                 // 0x48, set by PrepareOpen
	UNKNOWN(0x49, 0x4C);
	DWORD m_openTick;               // 0x4C, GetTickCount() at PrepareOpen, -1 when closed
	int m_state50;                  // 0x50, cleared by the constructor (use unknown)
	int m_count;                    // 0x54, ResetCount
	std::string m_url;              // 0x58, PrepareOpen
	std::string m_waitMsg;          // 0x74, SetWaitMsg
	int m_savedWindowed;            // 0x90, sOption::m_windowed before PrepareOpen (restored by Close)
	int m_savedFillScreen;          // 0x94, sOption::m_fillScreen before PrepareOpen (restored by Close)
	PADDING(0x98, 0xA4);
	bool m_forceWindowed;           // 0xA4, PrepareOpen switches to windowed, no fill screen
	bool m_restoreScreenSize;       // 0xA5, Close re-applies the game screen size
	PADDING(0xA6, 0xA8);
	HWND m_hViewWnd;                // 0xA8, the view window (Process creates it, Close clears it)
	CComObjectStack2<CBrowserView> m_view; // 0xAC
	ATL::CComQIPtr<IWebBrowser2> m_spBrowser; // 0x110, the view's WebBrowser control (Process), IsOpen
	DWORD m_viewStyle;              // 0x114, extra styles of the view window (Process), cleared by Close
	std::list<std::pair<std::string, std::string> > m_postArguments; // 0x118, name/value pairs (AddPostArgument)
};
BEGIN_LAYOUT_CHECK(CBrowser)
	CHECK_FIELD(CBrowser, m_pageType, 0x2C);
	CHECK_FIELD(CBrowser, m_hInstance, 0x30);
	CHECK_FIELD(CBrowser, m_hParent, 0x34);
	CHECK_FIELD(CBrowser, m_rect, 0x38);
	CHECK_FIELD(CBrowser, m_isShown, 0x48);
	CHECK_FIELD(CBrowser, m_openTick, 0x4C);
	CHECK_FIELD(CBrowser, m_count, 0x54);
	CHECK_FIELD(CBrowser, m_url, 0x58);
	CHECK_FIELD(CBrowser, m_waitMsg, 0x74);
	CHECK_FIELD(CBrowser, m_savedWindowed, 0x90);
	CHECK_FIELD(CBrowser, m_savedFillScreen, 0x94);
	CHECK_FIELD(CBrowser, m_forceWindowed, 0xA4);
	CHECK_FIELD(CBrowser, m_restoreScreenSize, 0xA5);
	CHECK_FIELD(CBrowser, m_state50, 0x50);
	CHECK_FIELD(CBrowser, m_hViewWnd, 0xA8);
	CHECK_FIELD(CBrowser, m_view, 0xAC);
	CHECK_FIELD(CBrowser, m_viewStyle, 0x114);
	CHECK_FIELD(CBrowser, m_spBrowser.p, 0x110); // CComQIPtr overloads operator&
	CHECK_FIELD(CBrowser, m_postArguments, 0x118);
END_LAYOUT_CHECK
CHECK_SIZE(CBrowser, 0x124);


#endif
