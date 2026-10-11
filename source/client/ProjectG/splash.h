// Original: d:\build\custom\temp\code\client\projectg\splash.h
#ifndef PG_CLIENT_PROJECTG_SPLASH_H
#define PG_CLIENT_PROJECTG_SPLASH_H

#include <windows.h>

#include "..\..\defines.h"
#include "singleton.h"

// Splash window shown while the client starts: a popup that paints a bitmap
// (layered with a color key when SetLayeredWindowAttributes exists), or a line
// of text when there is no bitmap.
class CSplash : public WSingleton<CSplash> {
	LAYOUT_CHECK_FRIEND;
	// The window class procedure (splash.cpp:43, file-local) paints through
	// OnPaint.
	friend LRESULT CALLBACK SplashClassWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	typedef BOOL (WINAPI* lpfnSetLayeredWindowAttributes)(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

	CSplash(void);
	CSplash(const char* lpszFileName, unsigned long colTrans);
	virtual ~CSplash(void);

	void ShowSplash(void);
	int DoLoop(void);
	int CloseSplash(void);
	void HideSplash(void);
	void SetText(char* text);
	unsigned long SetBitmap(const char* lpszFileName);
	unsigned long SetBitmap(HBITMAP hBitmap);
	bool SetTransparentColor(unsigned long col);
	long __stdcall WindowProc(HWND hwnd, unsigned int uMsg, unsigned int wParam, long lParam);

private:
	void Init(void);
	void OnPaint(HWND hwnd);
	bool MakeTransparent(void);
	HWND RegAndCreateWindow(void);
	void FreeResources(void);

	COLORREF m_colTrans;                                           // 0x28
	DWORD m_dwWidth;                                               // 0x2C
	DWORD m_dwHeight;                                              // 0x30
	HBITMAP m_hBitmap;                                             // 0x34

public:
	HWND m_hwnd;                                                   // 0x38

private:
	const char* m_lpszClassName;                                   // 0x3C
	char m_szText[0x80];                                           // 0x40
	int m_nTextX;                                                  // 0xC0
	int m_nTextY;                                                  // 0xC4
	lpfnSetLayeredWindowAttributes m_pSetLayeredWindowAttributes;  // 0xC8
	int m_reserved;                                                // 0xCC, only cleared by Init
};

CHECK_SIZE(CSplash, 0xD0);
BEGIN_LAYOUT_CHECK(CSplash)
	CHECK_FIELD(CSplash, m_colTrans, 0x28);
	CHECK_FIELD(CSplash, m_dwWidth, 0x2C);
	CHECK_FIELD(CSplash, m_dwHeight, 0x30);
	CHECK_FIELD(CSplash, m_hBitmap, 0x34);
	CHECK_FIELD(CSplash, m_hwnd, 0x38);
	CHECK_FIELD(CSplash, m_lpszClassName, 0x3C);
	CHECK_FIELD(CSplash, m_szText, 0x40);
	CHECK_FIELD(CSplash, m_nTextX, 0xC0);
	CHECK_FIELD(CSplash, m_nTextY, 0xC4);
	CHECK_FIELD(CSplash, m_pSetLayeredWindowAttributes, 0xC8);
	CHECK_FIELD(CSplash, m_reserved, 0xCC);
END_LAYOUT_CHECK

#endif
