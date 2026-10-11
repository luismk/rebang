// Original: d:\build\custom\temp\code\client\projectg\browser.cpp
#include <windows.h>
#include <atlbase.h>

#include "Browser.h"
#include "ClientSetting.h"
#include "InputManager.h"
#include "Registry.h"
#include "winput.h"
#include "wview.h"
#include "woverlay.h"

// The main window (?g_hwnd@@3PAUHWND__@@A) and the mouse
// (?g_mouse@@3PAVWInputDev@@A); their header is not known yet.
extern HWND g_hwnd;
extern WInputDev* g_mouse;

// The event descriptions CBrowserView's sink map points at (DWebBrowserEvents2's
// WindowClosing(VARIANT_BOOL, VARIANT_BOOL*) and the parameterless download
// events).
ATL::_ATL_FUNC_INFO WindowClosingInfo = { CC_STDCALL, VT_EMPTY, 2, { VT_BOOL, VT_BYREF | VT_BOOL } };
ATL::_ATL_FUNC_INFO DownloadInfo = { CC_STDCALL, VT_EMPTY, 0 };

// browser.cpp:26
BOOL CBrowserView::PreTranslateMessage(MSG* pMsg) {
	if ((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
		(pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST)) {
		return FALSE;
	}

	return (BOOL)::SendMessage(m_hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);
}

// browser.cpp:37
int CBrowserView::OnIdle(void) {
	return 0;
}

// browser.cpp:42
long CBrowserView::OnDestory(unsigned int uMsg, unsigned int wParam, long lParam, int& bHandled) {
	if (CBrowser::Instance()->IsOpen()) {
		CBrowser::Instance()->Close();
	}

	return 1;
}

// browser.cpp:87
void CBrowserView::OnWindowClosing(short bIsChildWindow, short* Cancel) {
	USES_CONVERSION;

	*Cancel = VARIANT_TRUE;
	if (CBrowser::Instance()->IsOpen()) {
		CBrowser::Instance()->Close();
	}
}

// browser.cpp:130
void CBrowserView::OnDownloadBegin(void) {
	++m_downloadCount;
}

// browser.cpp:140
void CBrowserView::OnDownloadComplete(void) {
	if (m_downloadCount > 0) {
		m_downloadCount--;
	}
}

// browser.cpp:156
CBrowser::CBrowser(void) {
	m_isShown = false;
	m_openTick = INFINITE;
	m_state50 = 0;
	m_count = 0;
	m_hInstance = NULL;
	m_hParent = NULL;

	m_hViewWnd = NULL;

	m_viewStyle = 0;

	sOption option = *WSingleton<COption>::Instance()->GetOption();

	m_savedWindowed = option.m_windowed;
	m_savedFillScreen = option.m_fillScreen;
	m_pageType = (eWebPage)0;
}

// browser.cpp:220
void CBrowser::Init(HINSTANCE hInstance, HWND hWnd, const RECT& rect) {
	m_hInstance = hInstance;
	m_hParent = hWnd;
	m_rect = rect;
}

// browser.cpp:732
void CBrowser::Refresh(void) {
}

// browser.cpp:780
int CBrowser::PreTranslateMessage(MSG* msg) {
	if (!m_isShown) {
		return 0;
	}

	return m_view.PreTranslateMessage(msg);
}

// browser.cpp:789
void CBrowser::Process(float elapsed) {
	if (!m_isShown) {
		return;
	}

	if (m_openTick >= INFINITE) {
		return;
	}

	WVideoDev* video = g_view->GetVideoDevice();

	if (!video || (video->GetRenderCount() & 1) || GetTickCount() - m_openTick <= 200) {
		return;
	}
	m_openTick = INFINITE;

	if (m_spBrowser == NULL) {
		int width = (int)g_view->GetWidth();
		int height = (int)g_view->GetHeight();
		RECT rect = { 0, 0, width, height };

		m_hViewWnd = m_view.Create(m_hParent, rect, "about:blank", m_viewStyle | WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU);
		m_view.SetWindowText("\xc6\xce\xbe\xdf");

		RECT viewRect;
		m_view.GetWindowRect(&viewRect);
		RECT mainRect;
		GetWindowRect(g_hwnd, &mainRect);

		HWND insertAfter = HWND_TOP;
		UINT flags = 0;
		int viewWidth = 0;
		int viewHeight = 0;

		switch (m_pageType) {
		case WEBPAGE_NORMAL:
			viewWidth = 500;
			viewHeight = 600;
			insertAfter = HWND_TOP;
			break;
		case WEBPAGE_SELFCERTIFY:
			viewWidth = 800;
			viewHeight = 600;
			insertAfter = HWND_NOTOPMOST;
			flags = SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED;
			break;
		}

		viewRect.left = mainRect.left + (mainRect.right - mainRect.left) / 2 - (viewWidth - viewRect.left) / 2;
		viewRect.top = mainRect.top + (mainRect.bottom - mainRect.top) / 2 - (viewHeight - viewRect.top) / 2;
		viewRect.right = viewRect.left + viewWidth;
		viewRect.bottom = viewRect.top + viewHeight;

		m_view.SetWindowPos(insertAfter, &viewRect, flags);

		m_view.m_downloadCount = 0;
		m_view.QueryControl(&m_spBrowser);
		m_view.DispEventAdvise(m_spBrowser);

		CComVariant empty;

		if (!m_postArguments.empty()) {
			VARIANT headers = { 0 };
			VARIANT postData = { 0 };
			WTL::CString headerText("Content-Type: application/x-www-form-urlencoded\r\n");
			headers.vt = VT_BSTR;
			headers.bstrVal = headerText.AllocSysString();
			std::string postArgument = GetPostArgument();
			SAFEARRAY* array = SafeArrayCreateVector(VT_UI1, 0, postArgument.length());
			if (array) {
				void* data;
				SafeArrayAccessData(array, &data);
				memcpy(data, postArgument.c_str(), postArgument.length());
				SafeArrayUnaccessData(array);

				postData.vt = VT_ARRAY | VT_UI1;
				postData.parray = array;
			}

			m_spBrowser->Navigate(CComBSTR(m_url.c_str()), &empty, &empty, &postData, &headers);
			SafeArrayDestroy(array);
			SysFreeString(headers.bstrVal);
		} else {
			m_spBrowser->Navigate(CComBSTR(m_url.c_str()), &empty, &empty, &empty, &empty);
		}

		g_input->SetActive(false);
		g_mouse->InitDevice(m_hParent, false);
	}
}

// browser.cpp:966
void CBrowser::Display(void) {
	if (m_isShown && m_openTick < INFINITE) {
		WRect rect(0, 0, g_view->GetWidth(), g_view->GetHeight());

		WOverlay::DrawBox(g_view, rect, 0, 0x88404040, 0.001f);
	}
}

// browser.cpp:1072
void CBrowser::DrawCloseButton(tagDRAWITEMSTRUCT* drawItem) {
}

// browser.cpp:1088
void CBrowser::DrawBgButton(tagDRAWITEMSTRUCT* drawItem) {
}

// browser.cpp:1124
void CBrowser::PrepareOpen(const char* url, const char* bgImage, bool restoreScreenSize, bool forceWindowed) {
	m_forceWindowed = forceWindowed;
	m_restoreScreenSize = restoreScreenSize;

	if (m_isShown) {
		return;
	}

	sOption option = *WSingleton<COption>::Instance()->GetOption();
	m_savedFillScreen = option.m_fillScreen;
	m_savedWindowed = option.m_windowed;
	if (m_forceWindowed) {
		if (!option.m_windowed || option.m_fillScreen) {
			option.m_windowed = 1;
			option.m_fillScreen = 0;
			WSingleton<COption>::Instance()->ApplyChange(option, false);
		}
	}

	m_isShown = true;
	m_url = url;
	m_openTick = GetTickCount();
	m_waitMsg = "\xb7\xce\xb5\xf9 \xc1\xdf\xc0\xd4\xb4\xcf\xb4\xd9...";
}

// browser.cpp:1163
void CBrowser::Open(const char* url, bool arg2, bool arg3) {
	m_postArguments.clear();
	PrepareOpen(url, "[text_pangya00.jpg", arg2, arg3);
}

// browser.cpp:1197
void CBrowser::OpenPost(const char* url, bool arg2, bool arg3) {
	PrepareOpen(url, "[text_pangya00.jpg", arg2, arg3);
}

// browser.cpp:1203
void CBrowser::InitializePostArgument(void) {
	m_postArguments.clear();
}

// browser.cpp:1208
void CBrowser::AddPostArgument(const char* name, const char* value) {
	m_postArguments.push_back(std::pair<std::string, std::string>(std::string(name), std::string(value)));
}

// browser.cpp:1213
std::string CBrowser::GetPostArgument(void) const {
	std::string result;
	std::list<std::pair<std::string, std::string> >::const_iterator it = m_postArguments.begin();
	while (it != m_postArguments.end()) {
		result += it->first;
		result += "=";
		result += it->second;

		if (++it != m_postArguments.end()) {
			result += "&";
		}
	}

	return result;
}
