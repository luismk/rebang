// Original: d:\build\custom\temp\code\client\projectg\winmain.cpp
#include <atlbase.h>
#include <atlapp.h>
#include <math.h>
#include <process.h>
#include <string.h>
#include <commctrl.h>
#include <shellapi.h>

#include "Splash.h"
#include "PPL.h"
#include "ClientSetting.h"
#include "logininfo.h"
#include "../shared/Localize.h"
#include "../shared/lfh.h"
#include "ProjectG.h"
#include "Browser.h"
#include "mousecursor.h"
#include "InputManager.h"
#include "../shared/TikiMagicBoxTable.h"


// WTL's application module (atlapp.h).
WTL::CAppModule _Module;

extern HINSTANCE g_instance;
extern HWND g_hwnd;
extern bool g_showTime;

int _GGC_Init(void);
void _GGC_End(void);
void _GGC_SetHwnd(HWND hWnd);
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool CheckWndVersion(void);
bool CheckRelatedDLL(HWND hWnd);
void SetWindowTitle(HWND hWnd);
unsigned int __stdcall DrawLoadingByThread(void* pParam);
void ShowSplash(bool bShow);
void HideSplash(void);
void ShowLoading(bool bShow);

HHOOK hhook;
char class_name[256];
char window_name[256];
HANDLE g_hDrawLoading;
unsigned long g_iLoadingThreadID;
bool g_bExitDrawLoading;
int g_iLoadCurNum;
int g_iLoadNextNum;
int g_iLoadTotalNum = 100;

// Written by the loading screen (the progress bar's position); no symbol.
static int data_00b7c478;
static int data_00b7c47c;

// One ball of the loading animation (0x30 bytes, five of them).
struct sLoadingBall {
	float velocity[3];  // 0x0
	float move[3];      // 0xC
	float pos[3];       // 0x18
	int screenX;        // 0x24, where DrawLoadingByThread drew it last
	int screenY;        // 0x28
	bool bounced;       // 0x2C
	bool stopped;       // 0x2D
};

// winmain.cpp:268
static void FillBlackRect(HDC hdc, int x, int y, int width, int height) {
	RECT rc;
	rc.left = x;
	rc.right = x + width;
	rc.top = y;
	rc.bottom = y + height;
	FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
}

// winmain.cpp:279: copies a w x h block of a 24-bit bottom-up DIB into
// another, skipping black (the color key).
static void BlitColorKey(unsigned char* dst, int dstHeight, int x, int y, int w, int h, int dstPitch,
	unsigned char* src, unsigned int srcX, unsigned int srcY, unsigned int srcW, unsigned int srcH, int srcPitch) {
	if (!dst || !src)
		return;

	if (x < 0 || x + w >= 800)
		return;

	if (y < 0 || y + h >= 600)
		return;

	int dstOffset = (dstHeight - y - srcH) * dstPitch + x * 3;
	int srcOffset = srcY * srcPitch + srcX * 3;
	int d = dstOffset, s = srcOffset;

	int xEnd = srcX + srcW;
	if (xEnd >= dstPitch / 3) xEnd = dstPitch / 3;
	int yEnd = srcY + srcH;
	if (yEnd >= h * dstPitch / 3) yEnd = h * dstPitch / 3;

	for (unsigned int j = srcY; j < yEnd; j++) {
		for (unsigned int i = srcX; i < xEnd; i++) {
			if (src[s] || src[s + 1] || src[s + 2]) {
				memcpy(&dst[d], &src[s], 3);
			}
			d += 3;
			s += 3;
		}
		d = dstOffset + (j + 1) * dstPitch;
		s = srcOffset + (j + 1) * srcPitch;
	}
}

// winmain.cpp:312
static void DrawPercent(HDC hdc, int x, int y, HDC hdcDigits, float rate) {
	int percent = (int)(100.0f - rate * 100.0f);
	int hundreds = percent / 100;
	int tens = (percent - hundreds * 100) / 10;
	int ones = percent - (hundreds * 10 + tens) * 10;

	if (hundreds) {
		BitBlt(hdc, x, y, 7, 12, hdcDigits, hundreds * 7, 0, SRCCOPY);
		BitBlt(hdc, x + 7, y, 7, 12, hdcDigits, tens * 7, 0, SRCCOPY);
		BitBlt(hdc, x + 14, y, 7, 12, hdcDigits, ones * 7, 0, SRCCOPY);
	} else if (tens) {
		FillBlackRect(hdc, x, y, 7, 12);
		BitBlt(hdc, x + 7, y, 7, 12, hdcDigits, tens * 7, 0, SRCCOPY);
		BitBlt(hdc, x + 14, y, 7, 12, hdcDigits, ones * 7, 0, SRCCOPY);
	} else if (ones) {
		FillBlackRect(hdc, x, y, 14, 12);
		BitBlt(hdc, x + 14, y, 7, 12, hdcDigits, ones * 7, 0, SRCCOPY);
	} else {
		FillBlackRect(hdc, x, y, 21, 12);
		BitBlt(hdc, x + 14, y, 7, 12, hdcDigits, 0, 0, SRCCOPY);
	}

	BitBlt(hdc, x + 21, y, 7, 12, hdcDigits, 70, 0, SRCCOPY);
	BitBlt(hdc, x + 28, y, 14, 12, hdcDigits, 77, 0, SRCCOPY);
}

// winmain.cpp:347
static void InitLoadingBalls(sLoadingBall* balls) {
	balls[0].velocity[0] = 70.0f; balls[0].velocity[1] = 200.0f; balls[0].velocity[2] = 30.0f;
	balls[1].velocity[0] = -50.0f; balls[1].velocity[1] = 190.0f; balls[1].velocity[2] = 40.0f;
	balls[2].velocity[0] = 10.0f; balls[2].velocity[1] = 260.0f; balls[2].velocity[2] = 70.0f;
	balls[3].velocity[0] = 90.0f; balls[3].velocity[1] = 130.0f; balls[3].velocity[2] = -60.0f;
	balls[4].velocity[0] = -80.0f; balls[4].velocity[1] = 280.0f; balls[4].velocity[2] = -80.0f;

	for (int i = 0; i < 5; i++) {
		balls[i].pos[0] = 0.0f;
		balls[i].pos[1] = -0.001f;
		balls[i].pos[2] = 0.0f;
		balls[i].screenX = 0;
		balls[i].screenY = 0;
		balls[i].bounced = false;
		balls[i].stopped = false;
	}
}

// winmain.cpp:367
static void MoveLoadingBalls(sLoadingBall* balls, float elapsed) {
	if (0.13f < elapsed)
		elapsed = 0.13f;

	for (int i = 0; i < 5; i++) {
		balls[i].move[0] = balls[i].velocity[0] * elapsed;
		balls[i].move[1] = balls[i].velocity[1] * elapsed;
		balls[i].move[2] = balls[i].velocity[2] * elapsed;
		balls[i].pos[0] += balls[i].move[0] * elapsed;
		balls[i].pos[1] += balls[i].move[1] * elapsed;
		balls[i].pos[2] += balls[i].move[2] * elapsed;

		if (!balls[i].stopped) {
			balls[i].velocity[0] -= balls[i].move[0] * 0.1f;
			balls[i].velocity[1] -= 9.8f;
			balls[i].velocity[2] -= balls[i].move[1] * 0.1f;
		}

		if (balls[i].pos[1] <= 0.0f && balls[i].bounced) {
			balls[i].pos[1] = 0.001f;
			balls[i].velocity[1] *= -0.4f;

			if (fabs(balls[i].velocity[1]) < 30.0f) {
				balls[i].velocity[0] = 0.0f;
				balls[i].velocity[1] = 0.0f;
				balls[i].velocity[2] = 0.0f;
				balls[i].stopped = true;
			}
		} else {
			balls[i].bounced = true;
		}
	}
}

// Loads bitmap resource id into a 24-bit DIB section selected into a new
// memory DC (one source line in the original: a macro).
#define LOAD_24BIT_DIB(id, bits, pitch, dc, oldBmp) \
	bits = NULL; \
	hBitmap = LoadBitmapA(hRes, MAKEINTRESOURCEA(id)); \
	GetObjectA(hBitmap, sizeof(BITMAP), &bm); \
	if (GetObjectA(hBitmap, sizeof(BITMAP), &bm)) { \
		BITMAPINFO bmi; \
		memset(&bmi, 0, sizeof(bmi)); \
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); \
		bmi.bmiHeader.biWidth = bm.bmWidth; \
		bmi.bmiHeader.biHeight = bm.bmHeight; \
		bmi.bmiHeader.biPlanes = 1; \
		bmi.bmiHeader.biBitCount = 24; \
		bmi.bmiHeader.biCompression = BI_RGB; \
		bmi.bmiHeader.biClrUsed = 0; \
		bmi.bmiHeader.biClrImportant = 0; \
		bmi.bmiHeader.biSizeImage = 0; \
		HDC hScreenDC = GetDC(NULL); \
		hDib = CreateDIBSection(hScreenDC, &bmi, DIB_RGB_COLORS, (void**)&bits, NULL, 0); \
		ReleaseDC(NULL, hScreenDC); \
		DIBSECTION ds; \
		GetObjectA(hDib, sizeof(DIBSECTION), &ds); \
		pitch = ds.dsBm.bmWidthBytes; \
		if (ds.dsBm.bmBitsPixel != 24) \
			MessageBoxA(NULL, "Use 24bit bitmap!!!", "alert!", MB_OK); \
		HDC hSrcDC = CreateCompatibleDC(NULL); \
		dc = CreateCompatibleDC(NULL); \
		HGDIOBJ hOldSrc = SelectObject(hSrcDC, hBitmap); \
		oldBmp = SelectObject(dc, hDib); \
		BitBlt(dc, 0, 0, bm.bmWidth, bm.bmHeight, hSrcDC, 0, 0, SRCCOPY); \
		DeleteObject(SelectObject(hSrcDC, hOldSrc)); \
		DeleteDC(hSrcDC); \
	} \
	DeleteObject(hBitmap)

// Loads bitmap resource id selected into a new DC compatible with the window
// (a macro in the original, likewise).
#define LOAD_BITMAP_DC(id, dc, oldBmp) \
	hTmp = LoadBitmapA(hRes, MAKEINTRESOURCEA(id)); \
	dc = CreateCompatibleDC(hdc); \
	oldBmp = SelectObject(dc, hTmp)

// winmain.cpp:412
unsigned int __stdcall DrawLoadingByThread(void* pParam) {
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);

	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	_splitpath(path, drive, dir, fname, ext);

	// LoadingRes[_<suffix of the exe's name>].dll holds the loading screen's bitmaps.
	char dllName[128];
	char suffix[20];
	strcpy(dllName, "LoadingRes");
	char* pSuffix = strrchr(fname, '_');
	if (pSuffix) {
		strcpy(suffix, pSuffix);
		strcat(dllName, suffix);
	}
	strcat(dllName, ".dll");

	HMODULE hRes = LoadLibraryA(dllName);

	if (hRes) {
		Sleep(100);
		if (g_iLoadCurNum >= g_iLoadTotalNum)
			g_bExitDrawLoading = true;

		HWND hWnd = g_hwnd;
		HDC hdc = GetDC(hWnd);

		RECT rcClient;
		GetClientRect(hWnd, &rcClient);

		DWORD startTime = timeGetTime();

		DWORD loadingTime = 0; unsigned int bgType = startTime % 3 > 2 ? 2 : startTime % 3;

		sLoadingBall balls[5];
		InitLoadingBalls(balls);

		// The back buffer: a 24-bit DIB section the size of the client area.
		unsigned char* backBits = NULL;
		BITMAPINFO backInfo;
		memset(&backInfo, 0, sizeof(backInfo));
		backInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		backInfo.bmiHeader.biWidth = rcClient.right - rcClient.left;
		backInfo.bmiHeader.biHeight = rcClient.bottom - rcClient.top;
		backInfo.bmiHeader.biPlanes = 1;
		backInfo.bmiHeader.biBitCount = 24;
		backInfo.bmiHeader.biCompression = BI_RGB;
		backInfo.bmiHeader.biSizeImage = 0;
		backInfo.bmiHeader.biClrUsed = 0;
		backInfo.bmiHeader.biClrImportant = 0;
		HDC backDC = CreateCompatibleDC(hdc);
		HGDIOBJ oldBack = SelectObject(backDC, CreateDIBSection(backDC, &backInfo, DIB_RGB_COLORS, (void**)&backBits, NULL, 0));

		HBITMAP hBitmap;
		HBITMAP hDib;
		BITMAP bm;
		unsigned char* barBits;
		int barPitch;
		HDC barDC;
		HGDIOBJ oldBar;
		LOAD_24BIT_DIB(0x6C, barBits, barPitch, barDC, oldBar);
		unsigned char* ballBits;
		int ballPitch;
		HDC ballDC;
		HGDIOBJ oldBall;
		LOAD_24BIT_DIB(0x65, ballBits, ballPitch, ballDC, oldBall);

		int bgId;
		if (bgType == 0)
			bgId = 0x81;
		else
			bgId = bgType == 1 ? 0x82 : 0x83;

		HGDIOBJ hTmp;
		HDC bgDC;
		HGDIOBJ oldBg;
		LOAD_BITMAP_DC(bgId, bgDC, oldBg);
		HDC textDC;
		HGDIOBJ oldText;
		LOAD_BITMAP_DC(0x67, textDC, oldText);
		HDC digitsDC;
		HGDIOBJ oldDigits;
		LOAD_BITMAP_DC(0x6D, digitsDC, oldDigits);

		HDC logoDC;
		HGDIOBJ oldLogo;
		LOAD_BITMAP_DC(0x7A, logoDC, oldLogo);
		HDC frameDC;
		HGDIOBJ oldFrame;
		LOAD_BITMAP_DC(0x7B, frameDC, oldFrame);

		FillRect(backDC, &rcClient, (HBRUSH)GetStockObject(BLACK_BRUSH));

		BitBlt(backDC, 685, 3, 118, 134, logoDC, 0, 0, SRCCOPY);
		BitBlt(backDC, 519, 521, 275, 72, frameDC, 0, 0, SRCCOPY);

		DWORD lastTime = timeGetTime();

		int width = rcClient.right - rcClient.left; DWORD doneTime = 0;
		int height = rcClient.bottom - rcClient.top;
		bool soundPlayed = false;
		while (!g_bExitDrawLoading) {
			Sleep(20);

			DWORD now = timeGetTime();

			DWORD elapsed = now - lastTime;
			lastTime = now;
			loadingTime += elapsed;

			if (g_iLoadCurNum < g_iLoadNextNum) {
				g_iLoadCurNum++;
			} else if (g_iLoadCurNum >= g_iLoadTotalNum) {
				// Loading done: keep the finished screen for two seconds.
				doneTime += elapsed;
				if (doneTime > 2000)
					break;
			}

			float rate = (float)g_iLoadCurNum / g_iLoadTotalNum;

			// The background animation: 16 frames of 117 x 190 or 20 of 87 x 182.
			if (bgType == 0) {
				unsigned int frame = loadingTime % 800 / 50 > 15 ? 15 : loadingTime % 800 / 50;
				BitBlt(backDC, 356, 169, 117, 190, bgDC, (frame & 3) * 117, (frame >> 2) * 190, SRCCOPY);
			} else {
				unsigned int frame = loadingTime % 1000 / 50 > 19 ? 19 : loadingTime % 1000 / 50;
				BitBlt(backDC, 356, 169, 87, 182, bgDC, frame % 5 * 87, frame / 5 * 182, SRCCOPY);
			}

			if (g_iLoadCurNum < g_iLoadTotalNum) {
				RECT rcBar = { 300, 330, 520, 350 }; FillRect(backDC, &rcBar, (HBRUSH)GetStockObject(BLACK_BRUSH));
				BitBlt(backDC, 500, 240, 31, 119, textDC, 0, 10, SRCCOPY);
				DrawPercent(backDC, 490, 240, digitsDC, rate);

				unsigned int frame = (1000 - loadingTime % 1000) / 90 > 10 ? 10 : (1000 - loadingTime % 1000) / 90;

				int barX = 300 - (int)(rate * -200.0f);
				data_00b7c47c = 330;
				data_00b7c478 = barX;
				BlitColorKey(backBits, height, barX, 330, 20, 20, width * 3, barBits, frame % 11 * 20, 0, 20, 20, barPitch);
			} else {
				if (!soundPlayed) {
					if (COption::Instance()->aIsMssEnabled())
						PlaySoundA(MAKEINTRESOURCEA(0x79), hRes, SND_RESOURCE | SND_ASYNC);

					soundPlayed = true;
				}

				RECT rcBar = { 300, 330, 520, 350 }; FillRect(backDC, &rcBar, (HBRUSH)GetStockObject(BLACK_BRUSH));

				for (int i = 0; i < 5; i++) {
					RECT rcBall = { balls[i].screenX, balls[i].screenY, balls[i].screenX + 19, balls[i].screenY + 19 }; FillRect(backDC, &rcBall, (HBRUSH)GetStockObject(BLACK_BRUSH));
				}

				BitBlt(backDC, 500, 240, 31, 119, textDC, 31, 10, SRCCOPY);
				MoveLoadingBalls(balls, elapsed * 0.005f);
				for (int j = 0; j < 5; j++) {
					unsigned int frame = loadingTime % 300 / 42 > 6 ? 6 : loadingTime % 300 / 42;
					balls[j].screenX = (int)balls[j].pos[0] + 510;
					balls[j].screenY = 320 - (int)(balls[j].pos[2] * 0.15f) - (int)balls[j].pos[1];

					BlitColorKey(backBits, height, balls[j].screenX, balls[j].screenY, 19, 19, width * 3, ballBits, frame % 7 * 19, 0, 19, 19, ballPitch);
				}
			}

			BitBlt(hdc, 0, 0, width, height, backDC, 0, 0, SRCCOPY);
		}

		FillRect(hdc, &rcClient, (HBRUSH)GetStockObject(BLACK_BRUSH));

		DeleteObject(SelectObject(frameDC, oldFrame));
		DeleteObject(SelectObject(logoDC, oldLogo));

		DeleteObject(SelectObject(digitsDC, oldDigits));
		DeleteObject(SelectObject(textDC, oldText));
		DeleteObject(SelectObject(ballDC, oldBall));
		DeleteObject(SelectObject(barDC, oldBar));
		DeleteObject(SelectObject(bgDC, oldBg));
		DeleteObject(SelectObject(backDC, oldBack));

		DeleteDC(frameDC);
		DeleteDC(logoDC);

		DeleteDC(digitsDC);
		DeleteDC(textDC);
		DeleteDC(ballDC);
		DeleteDC(barDC);
		DeleteDC(bgDC);
		DeleteDC(backDC);
		ReleaseDC(hWnd, hdc);
	}

	CloseHandle(g_hDrawLoading);
	g_hDrawLoading = NULL;
	g_iLoadingThreadID = 0;

	if (hRes)
		FreeLibrary(hRes);

	_endthreadex(0);

	return 0;
}

// winmain.cpp:660
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
	if (wParam == VK_SNAPSHOT)
		return 0;

	return CallNextHookEx(hhook, code, wParam, lParam);
}

// winmain.cpp:669
static void RunMainLoop(HWND hWnd) {
	new CProjectG(hWnd);
	new CTikiMagicBoxDoc;

	if (CProjectG::Instance()->Init()) {
		ShowLoading(true);
		HideSplash();

		if (CProjectG::Instance()->Ready()) {
			COption::Instance()->vApplyLobbyScreenSize();
			CMouseCursor::Instance()->SetMode(CMouseCursor::SCREEN_LOBBY);

			g_input->SetMousePointToLoginBox();

			new CBrowser;

			RECT rect;
			GetClientRect(hWnd, &rect);
			CBrowser::Instance()->Init(g_instance, hWnd, rect);

			WaitForSingleObject(g_hDrawLoading, 90000);

			ShowSplash(false);

			ShowLoading(false);
			_GGC_SetHwnd(g_hwnd);

			MSG msg;
			int result = -1;
			while (result <= 0) {
				if (PeekMessageA(&msg, NULL, 0, 0, PM_NOREMOVE)) {
					if (!GetMessageA(&msg, NULL, 0, 0)) {
						_GGC_End();
						break;
					}

					if (CBrowser::Instance()) {
						if (CBrowser::Instance()->PreTranslateMessage(&msg))
							continue;
					}

					TranslateMessage(&msg);
					DispatchMessageA(&msg);
				} else {
					result = CProjectG::Instance()->MainLoop(0);
				}
			}

			if (CBrowser::Instance())
				delete CBrowser::Instance();
		}
	}

	ShowLoading(false);

	CLoginInfo::Instance()->Destroy();
	if (CTikiMagicBoxDoc::Instance())
		delete CTikiMagicBoxDoc::Instance();
	if (CProjectG::Instance())
		delete CProjectG::Instance();
}

// winmain.cpp:821
HANDLE __stdcall GetPangYaMutex(void) {
	HANDLE hMutex = CreateMutexA(NULL, TRUE, "{071784A2-EE35-4e6a-92D0-6E7A4B985171}");
	if (hMutex) {
		DWORD dwError = GetLastError();

		if (dwError != ERROR_ALREADY_EXISTS && dwError != ERROR_ACCESS_DENIED)
			return hMutex;

		HWND hWnd = FindWindowA(class_name, NULL);
		if (hWnd) {
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
		}
	}

	ShowSplash(false);
	return NULL;
}

// winmain.cpp:902
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	LFHInstaller::install();

	InitLocalizeSystem();

	CoInitialize(NULL);

	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_BAR_CLASSES | ICC_COOL_CLASSES;
	InitCommonControlsEx(&icc);

	_Module.Init(NULL, hInstance);

	ATL::AtlAxWinInit();

	RECT rc = { 0, 0, 800, 600 }; strcpy(class_name, "PangYa");

	HANDLE hMutex = GetPangYaMutex();
	if (hMutex) {
		if (CLoginInfo::Instance()->InitWebLogin(GetCommandLineA())) {
			ShellExecuteA(NULL, NULL, "http://qa.pangya.gametree.co.kr/", NULL, NULL, SW_SHOW);
		} else {
			ShowSplash(true);

			g_instance = hInstance;

			WNDCLASSA wc;
			wc.style = 0;
			wc.lpfnWndProc = WinProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(101));
			wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = class_name;
			if (RegisterClassA(&wc)) {
				if (!CheckWndVersion()) {
					// "윈도우즈 98 혹은 윈도우즈 2000 이상의 운영체제가 필요합니다" (EUC-KR)
					MessageBoxA(NULL,
						"\xC0\xA9\xB5\xB5\xBF\xEC\xC1\xEE 98 \xC8\xA4\xC0\xBA \xC0\xA9\xB5\xB5\xBF\xEC\xC1\xEE 2000 "
						"\xC0\xCC\xBB\xF3\xC0\xC7 \xBF\xEE\xBF\xB5\xC3\xBC\xC1\xA6\xB0\xA1 \xC7\xCA\xBF\xE4\xC7\xD5\xB4\xCF\xB4\xD9",
						"\xC6\xCE\xBE\xDF", MB_OK);
				} else {
					AdjustWindowRectEx(&rc, WS_CAPTION | WS_THICKFRAME, FALSE, 0);
					int x = (GetSystemMetrics(SM_CXSCREEN) + rc.left - rc.right) / 2;
					int y = (GetSystemMetrics(SM_CYSCREEN) + rc.top - rc.bottom) / 2;
					OffsetRect(&rc, x, y);

					HWND hWnd = CreateWindowExA(0, class_name, window_name, WS_CAPTION | WS_THICKFRAME,
						x, y, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

					ShowWindow(hWnd, SW_HIDE);

					if (CheckRelatedDLL(hWnd)) {
						g_hwnd = hWnd;

						if (_GGC_Init()) {
							SetWindowTitle(hWnd);

							RunMainLoop(hWnd);
						}
					}
				}
			}
		}
	}

	ShowSplash(false);

	_Module.Term();
	CoUninitialize();

	if (hMutex) {
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	}

	_GGC_End();

	return 0;
}

// winmain.cpp:1139
void ShowSplash(bool bShow) {
	if (bShow) {
		new CSplash;
		CSplash::Instance()->SetBitmap("PangYaSetup.bmp");
		CSplash::Instance()->SetTransparentColor(0xFF00);
		CSplash::Instance()->ShowSplash();
	} else {
		if (CSplash::Instance()) {
			CSplash::Instance()->CloseSplash();
			delete CSplash::Instance();
		}
	}
}

// winmain.cpp:1159
void HideSplash(void) {
	if (CSplash::Instance())
		CSplash::Instance()->HideSplash();
}

// winmain.cpp:1176
void ShowLoading(bool bShow) {
	if (bShow) {
		if (!g_hDrawLoading && !g_iLoadingThreadID)
			g_hDrawLoading = (HANDLE)_beginthreadex(NULL, 0, DrawLoadingByThread, NULL, 0, (unsigned int*)&g_iLoadingThreadID);
	} else {
		g_bExitDrawLoading = true;
	}
}

// winmain.cpp:1203
void ShowPPL(void) {
	new CPPL;

	if (COption::Instance()) {
		if (CPPL::Instance()->IsPPLEnable())
			WinExec("PangyaPPL.exe", SW_HIDE);

		delete CPPL::Instance();
	}
}

// winmain.cpp:1222
BOOL GetLastWriteTime(HANDLE hFile, LPSTR lpszString) {
	FILETIME ftCreate, ftAccess, ftWrite, ftLocal;
	SYSTEMTIME stLocal;

	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return FALSE;

	if (!FileTimeToLocalFileTime(&ftWrite, &ftLocal))
		return FALSE;

	FileTimeToSystemTime(&ftLocal, &stLocal);

	// "%d년 %02d월 %02d일 %02d시 %02d분" (EUC-KR)
	wsprintfA(lpszString, "%d\xB3\xE2%02d\xBF\xF9%02d\xC0\xCF %02d\xBD\xC3%02d\xBA\xD0",
		stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute);

	return TRUE;
}

// winmain.cpp:1246
void SetWindowTitle(HWND hWnd) {
	// "팡야 United(QA)" (EUC-KR)
	const char* pszTitle = "\xC6\xCE\xBE\xDF United(QA)";

	strcpy(window_name, pszTitle);

	SetWindowTextA(hWnd, window_name);
}

// winmain.cpp:1256
void SetDirectory(void) {
}

// winmain.cpp:1276
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 1;

	case WM_USER:
		DestroyWindow(hWnd);
		break;

	case WM_ERASEBKGND:
		return 0;

	case WM_SYSCOMMAND:
		if (wParam == SC_RESTORE)
			break;

		if (wParam != SC_CLOSE)
			return 0;
		break;

	case WM_SIZE:
		g_showTime = (wParam != SIZE_MAXHIDE && wParam != SIZE_MINIMIZED) ? true : false;
		break;
	}

	CProjectG* pProjectG = CProjectG::Instance();
	if (pProjectG)
		return pProjectG->WinProc(hWnd, message, wParam, lParam);
	return DefWindowProcA(hWnd, message, wParam, lParam);
}

// winmain.cpp:1343
bool CheckSSE2_N_OSSupport(void) {
	return false;
}

// winmain.cpp:1382
bool CheckPentium4(void) {
	if (strstr(GetCommandLineA(), "-sse2"))
		return true;
	if (strstr(GetCommandLineA(), "-nosse2"))
		return false;

	return CheckSSE2_N_OSSupport();
}

// winmain.cpp:1391
bool CheckWndVersion(void) {
	OSVERSIONINFOA osvi;
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	GetVersionExA(&osvi);

	if (osvi.dwPlatformId != VER_PLATFORM_WIN32s && (osvi.dwMajorVersion > 4 || (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion > 0)))
		return true;

	return false;
}

// winmain.cpp:1405
bool CheckRelatedDLL(HWND hWnd) {
	HMODULE hModule = LoadLibraryA("dinput8.dll");

	if (!hModule) {
		// "DirectX 관련 파일을 찾을 수 없습니다.\nDirectX 9.0c 이상을 설치 후 다시 실행해 주세요." (EUC-KR)
		MessageBoxA(hWnd,
			"DirectX \xB0\xFC\xB7\xC3 \xC6\xC4\xC0\xCF\xC0\xBB \xC3\xA3\xC0\xBB \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.\n"
			"DirectX 9.0c \xC0\xCC\xBB\xF3\xC0\xBB \xBC\xB3\xC4\xA1 \xC8\xC4 \xB4\xD9\xBD\xC3 \xBD\xC7\xC7\xE0\xC7\xD8 \xC1\xD6\xBC\xBC\xBF\xE4.",
			"\xC6\xCE\xBE\xDF", MB_OK);
		return false;
	}

	FreeLibrary(hModule);
	return true;
}
