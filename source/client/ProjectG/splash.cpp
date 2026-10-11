// Original: d:\build\custom\temp\code\client\projectg\splash.cpp
#include <windows.h>
#include <string.h>

// File-local in the original (sub_0070a0b0); declared before the class so its
// friend declaration names this function.
static LRESULT CALLBACK SplashClassWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#include "Splash.h"

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif
#ifndef LWA_COLORKEY
#define LWA_COLORKEY 0x00000001
#endif

// The splash object that created the window (WM_CREATE's lpCreateParams).
static CSplash* s_pSplash = NULL;

// splash.cpp:43
static LRESULT CALLBACK SplashClassWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_CREATE)
		s_pSplash = (CSplash*)((LPCREATESTRUCT)lParam)->lpCreateParams;

	if (s_pSplash)
		switch (uMsg) { case WM_PAINT: s_pSplash->OnPaint(hwnd); return 0; }

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// splash.cpp:56
long __stdcall CSplash::WindowProc(HWND hwnd, unsigned int uMsg, unsigned int wParam, long lParam) {
	switch (uMsg) {
	case WM_PAINT:
		OnPaint(hwnd); return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// splash.cpp:77
CSplash::CSplash(void) {
	Init();
}

// splash.cpp:82
CSplash::~CSplash(void) {
	FreeResources();
}

// splash.cpp:98
CSplash::CSplash(const char* lpszFileName, unsigned long colTrans) {
	Init();

	SetBitmap(lpszFileName);
	SetTransparentColor(colTrans);
}

// splash.cpp:116
void CSplash::Init(void) {
	m_hwnd = NULL;
	m_lpszClassName = "SPLASH";
	m_colTrans = 0;
	m_dwHeight = 0;
	m_dwWidth = 0;
	strcpy(m_szText, "Initializing....");
	m_nTextX = 10;
	m_nTextY = 100;

	HMODULE hUser32 = GetModuleHandle("USER32.DLL");
	if (hUser32) {
		m_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
	} else {
		m_pSetLayeredWindowAttributes = NULL;
	}

	m_reserved = 0;
}

// splash.cpp:168
void CSplash::OnPaint(HWND hwnd) {
	if (!m_hBitmap) {
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hwnd, &ps);
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, m_nTextX, m_nTextY, m_szText, strlen(m_szText));
		EndPaint(hwnd, &ps);
		return;
	}

	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hwnd, &ps);

	RECT rect;
	GetClientRect(m_hwnd, &rect);

	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, m_hBitmap);

	BitBlt(hDC, 0, 0, m_dwWidth, m_dwHeight, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBmp);
	DeleteDC(hMemDC);

	EndPaint(hwnd, &ps);
}

// splash.cpp:216
void CSplash::SetText(char* text) {
	strcpy(m_szText, text);
	OnPaint(m_hwnd);
	InvalidateRect(m_hwnd, NULL, FALSE);
}

// splash.cpp:233
HWND CSplash::RegAndCreateWindow(void) {
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
	wndclass.lpfnWndProc = SplashClassWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = DLGWINDOWEXTRA;
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_WAIT);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = m_lpszClassName;
	wndclass.hIconSm = NULL;

	if (!RegisterClassEx(&wndclass))
		return NULL;

	DWORD nScrWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	DWORD nScrHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	if (m_dwHeight == 0 && m_dwWidth == 0) {
		m_dwWidth = 480;
		m_dwHeight = 360;
	}

	DWORD x = (nScrWidth - m_dwWidth) / 2;
	DWORD y = (nScrHeight - m_dwHeight) / 2;
	m_hwnd = CreateWindowEx(WS_EX_TOOLWINDOW, m_lpszClassName, NULL, WS_POPUP, x, y, m_dwWidth, m_dwHeight, NULL, NULL, NULL, this);

	if (m_hwnd) {
		MakeTransparent();
		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);
	}
	return m_hwnd;
}

// splash.cpp:294
void CSplash::ShowSplash(void) {
	CloseSplash();
	RegAndCreateWindow();
	SetText("");
	ShowWindow(m_hwnd, SW_SHOW);
}

// splash.cpp:323
int CSplash::CloseSplash(void) {
	ShowWindow(m_hwnd, SW_HIDE);

	if (m_hwnd) {
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;
		UnregisterClass(m_lpszClassName, GetModuleHandle(NULL));
		return 1;
	}
	return 0;
}

// splash.cpp:346
void CSplash::HideSplash(void) {
	ShowWindow(m_hwnd, SW_HIDE);
}

// splash.cpp:361
unsigned long CSplash::SetBitmap(const char* lpszFileName) {
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, lpszFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	return SetBitmap(hBitmap);
}

// splash.cpp:371
unsigned long CSplash::SetBitmap(HBITMAP hBitmap) {
	int nRetValue;
	BITMAP csBitmapSize;

	FreeResources();

	if (hBitmap) {
		m_hBitmap = hBitmap;

		nRetValue = GetObject(hBitmap, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0) {
			FreeResources();
			return 0;
		}
		m_dwWidth = csBitmapSize.bmWidth;
		m_dwHeight = csBitmapSize.bmHeight;
	}

	return 1;
}

// splash.cpp:402
void CSplash::FreeResources(void) {
	if (m_hBitmap)
		DeleteObject(m_hBitmap);
	m_hBitmap = NULL;
}

// splash.cpp:419
int CSplash::DoLoop(void) {
	if (!m_hwnd)
		ShowSplash();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

// splash.cpp:450
bool CSplash::SetTransparentColor(unsigned long col) {
	m_colTrans = col;
	return MakeTransparent();
}

// splash.cpp:457
bool CSplash::MakeTransparent(void) {
	if (m_hwnd && m_pSetLayeredWindowAttributes && m_colTrans) {
		SetWindowLong(m_hwnd, GWL_EXSTYLE, GetWindowLong(m_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		m_pSetLayeredWindowAttributes(m_hwnd, m_colTrans, 0, LWA_COLORKEY);
	}
	return true;
}
