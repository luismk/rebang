#include "projectg.h"
#include "clientsetting.h"
#include "mousecursor.h"
#include "splash.h"
#include "ppl.h"
#include "browser.h"
#include "ggc_wrapper.h"
#include "../../shared/lfh.h"
#include "../../shared/localize.h"
#include "../../shared/tikimagicboxtable.h"
#include <process.h>
#include "wlocalize.h"

// HACK: some headers are avoided to control IL space, since the inline asm
// functions take up dramatically more space in IL, which can cause symbols
// to go out of order...
//
// Once we get DrawLoadingByThread byte-for-byte we can avoid this.

class CLoginInfo
{
public:
	static CLoginInfo* Instance();
	int Destroy();
	int InitWebLogin(const char* cmdLine);
};

ILFILLW1

extern HWND g_hwnd;
extern int g_iLoadTotalNum;
extern int g_iLoadCurNum;
extern int g_iLoadNextNum;
extern bool g_bExitDrawLoading;

static bool IsZero(float f)
{
	return f < g_EPSILON;
}

#define LOAD_DIB_BITMAP(id, hdcDib, hOldDib, pBits, pitch) \
	{ \
		pBits = NULL; \
		HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(id)); \
		BITMAP bm; \
		GetObject(hBitmap, sizeof(BITMAP), &bm); \
		if (!GetObject(hBitmap, sizeof(BITMAP), &bm)) \
			DeleteObject(hBitmap); \
		else \
		{ \
			BYTE biBuffer[sizeof(BITMAPINFO)]; \
			BITMAPINFO& bi = *(BITMAPINFO*)biBuffer; \
			memset(&bi, 0, sizeof(bi)); \
			bi.bmiHeader.biWidth = (WORD)bm.bmWidth; \
			bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); \
			bi.bmiHeader.biHeight = (WORD)bm.bmHeight; \
			bi.bmiHeader.biPlanes = 1; \
			bi.bmiHeader.biBitCount = 24; \
			bi.bmiHeader.biCompression = BI_RGB; \
			bi.bmiHeader.biClrUsed = 0; \
			bi.bmiHeader.biClrImportant = 0; \
			bi.bmiHeader.biSizeImage = 0; \
			HDC hdcScreen = GetDC(NULL); \
			HBITMAP hDib = CreateDIBSection(hdcScreen, &bi, DIB_RGB_COLORS, \
				&pBits, NULL, 0); \
			ReleaseDC(NULL, hdcScreen); \
			DIBSECTION ds; \
			GetObject(hDib, sizeof(DIBSECTION), &ds); \
			pitch = ds.dsBm.bmWidthBytes; \
			if (ds.dsBm.bmBitsPixel != 24) \
				MessageBox(NULL, "Use 24bit bitmap!!!", "alert!", MB_OK); \
			HDC hdcBitmap = CreateCompatibleDC(NULL); \
			hdcDib = CreateCompatibleDC(NULL); \
			HGDIOBJ hOldBitmap = SelectObject(hdcBitmap, hBitmap); \
			hOldDib = SelectObject(hdcDib, hDib); \
			BitBlt(hdcDib, 0, 0, bm.bmWidth, bm.bmHeight, hdcBitmap, 0, 0, \
				SRCCOPY); \
			DeleteObject(SelectObject(hdcBitmap, hOldBitmap)); \
			DeleteDC(hdcBitmap); \
			DeleteObject(hBitmap); \
		} \
	}

#define CLEAR_RECT(hdc, x, y, w, h) \
	{ \
		RECT rcClear; \
		rcClear.left = (x); \
		rcClear.right = (x) + (w); \
		rcClear.top = (y); \
		rcClear.bottom = (y) + (h); \
		FillRect(hdc, &rcClear, (HBRUSH)GetStockObject(BLACK_BRUSH)); \
	}

#define LOAD_BITMAP_DC(id, hdcBitmap, hOldBitmap) \
	{ \
		HBITMAP hBitmap = LoadBitmap(hInst, (LPCSTR)(id)); \
		hdcBitmap = CreateCompatibleDC(hdc); \
		hOldBitmap = SelectObject(hdcBitmap, hBitmap); \
	}

WTL::CAppModule _Module;

unsigned long g_iLoadingThreadID;
HANDLE g_hDrawLoading;
char g_executeFilePath[MAX_PATH];
bool g_enable_p4;
HINSTANCE g_instance;
char class_name[128];
char window_name[128];
char computer_name[128];
HHOOK hhook;

bool g_showTime = true;
bool g_processalway = true;
bool g_checkIntegrity = true;

struct sVector
{
	float x, y, z;
};

struct sLoadingBall
{
	sVector vel;
	sVector delta;
	sVector pos;
	int x;
	int y;
	bool bBounce;
	bool bStop;
};

unsigned int __stdcall DrawLoadingByThread(void* pArg);
HANDLE WINAPI GetPangYaMutex();
void ShowSplash(bool bShow);
void HideSplash();
void ShowLoading(bool bShow);
void ShowPPL();
BOOL GetLastWriteTime(HANDLE hFile, char* lpszString);
void SetWindowTitle(HWND hWnd);
void SetDirectory();
LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool CheckSSE2_N_OSSupport();
bool CheckPentium4();
bool CheckWndVersion();
bool CheckRelatedDLL(HWND hWnd);

#ifndef REBANG_SEMANTIC_ONLY

extern "C" void __cdecl rb_aIsMssEnabled_COption__QAE_NXZ(void);
#pragma comment(linker, \
	"/alternatename:_rb_aIsMssEnabled_COption__QAE_NXZ=?aIsMssEnabled@COption@@QAE_NXZ")
extern "C" int rb_g_bExitDrawLoading__3_NA;
#pragma comment(linker, \
	"/alternatename:_rb_g_bExitDrawLoading__3_NA=?g_bExitDrawLoading@@3_NA")
extern "C" int rb_g_hDrawLoading__3PAXA;
#pragma comment(linker, \
	"/alternatename:_rb_g_hDrawLoading__3PAXA=?g_hDrawLoading@@3PAXA")
extern "C" int rb_g_hwnd__3PAUHWND____A;
#pragma comment(linker, \
	"/alternatename:_rb_g_hwnd__3PAUHWND____A=?g_hwnd@@3PAUHWND__@@A")
extern "C" int rb_g_iLoadCurNum__3HA;
#pragma comment(linker, \
	"/alternatename:_rb_g_iLoadCurNum__3HA=?g_iLoadCurNum@@3HA")
extern "C" int rb_g_iLoadNextNum__3HA;
#pragma comment(linker, \
	"/alternatename:_rb_g_iLoadNextNum__3HA=?g_iLoadNextNum@@3HA")
extern "C" int rb_g_iLoadTotalNum__3HA;
#pragma comment(linker, \
	"/alternatename:_rb_g_iLoadTotalNum__3HA=?g_iLoadTotalNum@@3HA")
extern "C" int rb_g_iLoadingThreadID__3KA;
#pragma comment(linker, \
	"/alternatename:_rb_g_iLoadingThreadID__3KA=?g_iLoadingThreadID@@3KA")
extern "C" int rb_m_pInstance___WSingleton_VCOption____1PAVCOption__A;
#pragma comment(linker, \
	"/alternatename:_rb_m_pInstance___WSingleton_VCOption____1PAVCOption__A=?m_pInstance@?$WSingleton@VCOption@@@@1PAVCOption@@A")
extern "C" int rb_real_3e19999a;
#pragma comment(linker, "/alternatename:_rb_real_3e19999a=__real@3e19999a")
extern "C" int rb_real_42c80000;
#pragma comment(linker, "/alternatename:_rb_real_42c80000=__real@42c80000")
extern "C" int rb_real_4f800000;
#pragma comment(linker, "/alternatename:_rb_real_4f800000=__real@4f800000")
extern "C" void __cdecl rb_security_check_cookie_4(void);
#pragma comment(linker, \
	"/alternatename:_rb_security_check_cookie_4=@__security_check_cookie@4")
extern "C" int rb_security_cookie;
#pragma comment(linker, "/alternatename:_rb_security_cookie=___security_cookie")
#endif

#ifndef REBANG_SEMANTIC_ONLY
static __declspec(naked) void ClearRect(HDC hdc, int x, int y, int w, int h)
{
	__asm {
		push ebp
		mov ebp, esp
		sub esp, 10h
		mov dword ptr [ebp-10h], ecx
		add ecx, edx
		mov dword ptr [ebp-8], ecx
		mov ecx, dword ptr [ebp+0Ch]
		mov dword ptr [ebp-0Ch], eax
		add eax, ecx
		push 4
		mov dword ptr [ebp-4], eax
		call dword ptr [GetStockObject]
		push eax
		mov eax, dword ptr [ebp+8]
		lea edx, [ebp-10h]
		push edx
		push eax
		call dword ptr [FillRect]
		mov esp, ebp
		pop ebp
		ret 8
	}
}
#else
static void ClearRect(HDC hdc, int x, int y, int w, int h)
{
	RECT rc;
	rc.left = x;
	rc.right = x + w;
	rc.top = y;
	rc.bottom = y + h;
	FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
}
#endif

#ifndef REBANG_SEMANTIC_ONLY
static __declspec(naked) void DrawSprite(BYTE* pDst, int x, int y, int w, int h,
	int dstPitch, int dstHeight, BYTE* pSrc, int sx, int sy, int sw, int sh,
	int srcPitch)
{
	__asm {
		push ebp
		mov ebp, esp
		push ecx
		cmp dword ptr [ebp+8], 0
		mov edx, dword ptr [ebp+24h]
		push ebx
		mov ebx, dword ptr [ebp+1Ch]
		push esi
		mov esi, ecx
		mov ecx, dword ptr [ebp+0Ch]
		je L_0153
		cmp dword ptr [ebp+20h], 0
		je L_0153
		test ecx, ecx
		jl L_0153
		push edi
		mov edi, dword ptr [ebp+14h]
		add ecx, edi
		cmp ecx, 320h
		jge L_0152
		mov ecx, dword ptr [ebp+10h]
		test ecx, ecx
		jl L_0152
		mov edi, dword ptr [ebp+18h]
		add edi, ecx
		cmp edi, 258h
		jge L_0152
		sub eax, ecx
		sub eax, dword ptr [ebp+2Ch]
		mov ecx, dword ptr [ebp+0Ch]
		imul eax, ebx
		lea ecx, [ecx+ecx*2]
		add eax, ecx
		mov ecx, esi
		imul ecx, dword ptr [ebp+30h]
		mov dword ptr [ebp+0Ch], eax
		mov dword ptr [ebp-4], eax
		mov eax, dword ptr [ebp+28h]
		lea edi, [edx+edx*2]
		add ecx, edi
		lea edi, [edx+eax]
		mov eax, 55555556h
		imul ebx
		mov eax, edx
		shr eax, 1Fh
		add eax, edx
		cmp edi, eax
		mov dword ptr [ebp+14h], edi
		jl L_009a
		mov dword ptr [ebp+14h], eax
L_009a:
		mov eax, dword ptr [ebp+18h]
		imul eax, ebx
		mov edx, eax
		mov eax, 55555556h
		imul edx
		mov eax, edx
		shr eax, 1Fh
		add eax, edx
		mov edx, dword ptr [ebp+2Ch]
		add edx, esi
		cmp edx, eax
		jl L_00bb
		mov edx, eax
L_00bb:
		cmp esi, edx
		jae L_0152
		lea eax, [esi+1]
		mov edi, eax
		imul eax, ebx
		imul edi, dword ptr [ebp+30h]
		add eax, dword ptr [ebp+0Ch]
		mov ebx, dword ptr [ebp+20h]
		add edi, ecx
		sub edx, esi
		mov dword ptr [ebp+18h], eax
		mov eax, dword ptr [ebp-4]
		mov dword ptr [ebp+2Ch], edi
		mov dword ptr [ebp+10h], edx
L_00e5:
		mov edx, dword ptr [ebp+14h]
		cmp dword ptr [ebp+24h], edx
		jae L_0139
		mov esi, dword ptr [ebp+8]
		add eax, esi
		sub edx, dword ptr [ebp+24h]
		mov dword ptr [ebp+0Ch], eax
		lea esi, [ecx+ebx]
		jmp L_0100
		_emit 0x8d
		_emit 0x49
		_emit 0x00
L_0100:
		cmp byte ptr [esi], 0
		jne L_0113
		cmp byte ptr [ecx+ebx+1], 0
		jne L_0113
		cmp byte ptr [ecx+ebx+2], 0
		je L_012a
L_0113:
		mov edi, dword ptr [ebp+0Ch]
		mov eax, esi
		mov bx, word ptr [eax]
		mov word ptr [edi], bx
		mov al, byte ptr [eax+2]
		mov ebx, dword ptr [ebp+20h]
		mov byte ptr [edi+2], al
		mov edi, dword ptr [ebp+2Ch]
L_012a:
		add dword ptr [ebp+0Ch], 3
		add ecx, 3
		add esi, 3
		sub edx, 1
		jne L_0100
L_0139:
		mov edx, dword ptr [ebp+18h]
		mov eax, edx
		add edx, dword ptr [ebp+1Ch]
		mov ecx, edi
		add edi, dword ptr [ebp+30h]
		sub dword ptr [ebp+10h], 1
		mov dword ptr [ebp+18h], edx
		mov dword ptr [ebp+2Ch], edi
		jne L_00e5
L_0152:
		pop edi
L_0153:
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		ret 2Ch
	}
}
#else
static void DrawSprite(BYTE* pDst, int x, int y, int w, int h, int dstPitch,
	int dstHeight, BYTE* pSrc, int sx, int sy, int sw, int sh, int srcPitch)
{
	if (pDst == NULL || pSrc == NULL)
		return;
	if (x < 0 || x + w >= 800)
		return;
	if (y < 0 || y + h >= 600)
		return;

	int dstOffset = (dstHeight - y - sh) * dstPitch + x * 3;
	int srcOffset = sy * srcPitch + sx * 3;
	int d = dstOffset;
	int s = srcOffset;
	int ex = min(sx + sw, dstPitch / 3);
	int ey = min(sy + sh, h * dstPitch / 3);

	for (DWORD j = sy; j < (DWORD)ey; j++)
	{
		for (DWORD i = sx; i < (DWORD)ex; i++)
		{
			if (pSrc[s] || pSrc[s + 1] || pSrc[s + 2])
			{
				memcpy(&pDst[d], &pSrc[s], 3);
			}
			d += 3;
			s += 3;
		}
		d = dstOffset + (j + 1) * dstPitch;
		s = srcOffset + (j + 1) * srcPitch;
	}
}
#endif

#ifndef REBANG_SEMANTIC_ONLY
static __declspec(naked) void DrawPercent(HDC hdc, int x, int y, HDC hdcNumber,
	float ratio)
{
	__asm {
		push ebp
		mov ebp, esp
		sub esp, 14h
		fld dword ptr [ebp+0Ch]
		push edi
		fmul dword ptr [rb_real_42c80000]
		mov edi, eax
		fnstcw word ptr [ebp+0Eh]
		movzx eax, word ptr [ebp+0Eh]
		fsubr dword ptr [rb_real_42c80000]
		or ah, 0Ch
		mov dword ptr [ebp-4], eax
		fldcw word ptr [ebp-4]
		mov eax, 51EB851Fh
		fistp dword ptr [ebp-4]
		imul dword ptr [ebp-4]
		fldcw word ptr [ebp+0Eh]
		sar edx, 5
		mov ecx, edx
		shr ecx, 1Fh
		add ecx, edx
		lea eax, [ecx+ecx*2]
		lea edx, [ecx+eax*8]
		add edx, edx
		add edx, edx
		mov eax, edx
		mov edx, dword ptr [ebp-4]
		sub edx, eax
		mov eax, 66666667h
		imul edx
		sar edx, 2
		mov eax, edx
		shr eax, 1Fh
		add eax, edx
		mov dword ptr [ebp+0Ch], eax
		lea edx, [ecx+ecx*4]
		lea eax, [eax+edx*2]
		lea eax, [eax+eax*4]
		add eax, eax
		mov edx, eax
		mov eax, dword ptr [ebp-4]
		sub eax, edx
		test ecx, ecx
		mov dword ptr [ebp-4], eax
		je L_00ee
		push 0CC0020h
		push 0
		lea eax, [ecx*8]
		sub eax, ecx
		mov ecx, dword ptr [ebp+8]
		push eax
		push ecx
		push 0Ch
		push 7
		push esi
		push edi
		push ebx
		call dword ptr [BitBlt]
		mov eax, dword ptr [ebp+0Ch]
		push 0CC0020h
		push 0
		lea edx, [eax*8]
		sub edx, eax
		mov eax, dword ptr [ebp+8]
		push edx
		push eax
		push 0Ch
		push 7
		push esi
		lea ecx, [edi+7]
		push ecx
		push ebx
		call dword ptr [BitBlt]
		mov eax, dword ptr [ebp-4]
		push 0CC0020h
		push 0
		lea edx, [eax*8]
		sub edx, eax
		mov eax, dword ptr [ebp+8]
		push edx
		push eax
		push 0Ch
		push 7
		push esi
		lea ecx, [edi+0Eh]
		push ecx
		jmp L_01bd
L_00ee:
		cmp dword ptr [ebp+0Ch], 0
		je L_0165
		lea eax, [edi+7]
		lea edx, [esi+0Ch]
		push 4
		mov dword ptr [ebp-14h], edi
		mov dword ptr [ebp-0Ch], eax
		mov dword ptr [ebp-10h], esi
		mov dword ptr [ebp-8], edx
		call dword ptr [GetStockObject]
		push eax
		lea eax, [ebp-14h]
		push eax
		push ebx
		call dword ptr [FillRect]
		mov eax, dword ptr [ebp+0Ch]
		mov edx, dword ptr [ebp+8]
		push 0CC0020h
		push 0
		lea ecx, [eax*8]
		sub ecx, eax
		push ecx
		push edx
		push 0Ch
		push 7
		push esi
		lea eax, [edi+7]
		push eax
		push ebx
		call dword ptr [BitBlt]
		mov eax, dword ptr [ebp-4]
		mov edx, dword ptr [ebp+8]
		push 0CC0020h
		push 0
		lea ecx, [eax*8]
		sub ecx, eax
		push ecx
		push edx
		push 0Ch
		push 7
		push esi
		lea eax, [edi+0Eh]
		push eax
		jmp L_01bd
L_0165:
		push 0Ch
		test eax, eax
		mov ecx, edi
		push ebx
		mov eax, esi
		je L_019d
		mov edx, 0Eh
		call ClearRect
		mov eax, dword ptr [ebp-4]
		mov edx, dword ptr [ebp+8]
		push 0CC0020h
		push 0
		lea ecx, [eax*8]
		sub ecx, eax
		push ecx
		push edx
		push 0Ch
		push 7
		push esi
		lea eax, [edi+0Eh]
		push eax
		jmp L_01bd
L_019d:
		mov edx, 15h
		call ClearRect
		mov ecx, dword ptr [ebp+8]
		push 0CC0020h
		push 0
		push 0
		push ecx
		push 0Ch
		push 7
		push esi
		lea edx, [edi+0Eh]
		push edx
L_01bd:
		push ebx
		call dword ptr [BitBlt]
		mov eax, dword ptr [ebp+8]
		push 0CC0020h
		push 0
		push 46h
		push eax
		push 0Ch
		push 7
		push esi
		lea ecx, [edi+15h]
		push ecx
		push ebx
		call dword ptr [BitBlt]
		mov edx, dword ptr [ebp+8]
		push 0CC0020h
		push 0
		push 4Dh
		push edx
		push 0Ch
		push 0Eh
		push esi
		add edi, 1Ch
		push edi
		push ebx
		call dword ptr [BitBlt]
		pop edi
		mov esp, ebp
		pop ebp
		ret 8
	}
}
#else
static void DrawPercent(HDC hdc, int x, int y, HDC hdcNumber, float ratio)
{
	int percent = (int)(100.0f - ratio * 100.0f);
	int hundreds = percent / 100;
	int tens = (percent - hundreds * 100) / 10;
	int ones = percent - hundreds * 100 - tens * 10;

	if (hundreds)
	{
		BitBlt(hdc, x, y, 7, 12, hdcNumber, 7 * hundreds, 0, SRCCOPY);
		BitBlt(hdc, x + 7, y, 7, 12, hdcNumber, 7 * tens, 0, SRCCOPY);
		BitBlt(hdc, x + 14, y, 7, 12, hdcNumber, 7 * ones, 0, SRCCOPY);
	}
	else if (tens)
	{
		ClearRect(hdc, x, y, 7, 12);
		BitBlt(hdc, x + 7, y, 7, 12, hdcNumber, 7 * tens, 0, SRCCOPY);
		BitBlt(hdc, x + 14, y, 7, 12, hdcNumber, 7 * ones, 0, SRCCOPY);
	}
	else
	{
		if (ones)
		{
			ClearRect(hdc, x, y, 14, 12);
			BitBlt(hdc, x + 14, y, 7, 12, hdcNumber, 7 * ones, 0, SRCCOPY);
		}
		else
		{
			ClearRect(hdc, x, y, 21, 12);
			BitBlt(hdc, x + 14, y, 7, 12, hdcNumber, 0, 0, SRCCOPY);
		}
	}

	BitBlt(hdc, x + 21, y, 7, 12, hdcNumber, 70, 0, SRCCOPY);
	BitBlt(hdc, x + 28, y, 14, 12, hdcNumber, 77, 0, SRCCOPY);
}
#endif

#ifndef REBANG_SEMANTIC_ONLY
static __declspec(naked) void InitBalls(sLoadingBall* balls)
{
	__asm {
		mov ecx, 0C2A00000h
		mov dword ptr [eax+0C0h], ecx
		mov dword ptr [eax+0C8h], ecx
		mov dword ptr [eax], 428C0000h
		mov dword ptr [eax+4], 43480000h
		mov dword ptr [eax+8], 41F00000h
		mov dword ptr [eax+30h], 0C2480000h
		mov dword ptr [eax+34h], 433E0000h
		mov dword ptr [eax+38h], 42200000h
		mov dword ptr [eax+60h], 41200000h
		mov dword ptr [eax+64h], 43820000h
		mov dword ptr [eax+68h], 428C0000h
		mov dword ptr [eax+90h], 42B40000h
		mov dword ptr [eax+94h], 43020000h
		mov dword ptr [eax+98h], 0C2700000h
		mov dword ptr [eax+0C4h], 438C0000h
		add eax, 1Ch
		mov edx, 5
		xor ecx, ecx
L_0081:
		mov dword ptr [eax-4], ecx
		mov dword ptr [eax], 0BA83126Fh
		mov dword ptr [eax+4], ecx
		mov dword ptr [eax+8], ecx
		mov dword ptr [eax+0Ch], ecx
		mov byte ptr [eax+10h], cl
		mov byte ptr [eax+11h], cl
		add eax, 30h
		sub edx, 1
		jne L_0081
		ret
	}
}
#else
static void InitBalls(sLoadingBall* balls)
{
	balls[0].vel.x = 70.0f;
	balls[0].vel.y = 200.0f;
	balls[0].vel.z = 30.0f;
	balls[1].vel.x = -50.0f;
	balls[1].vel.y = 190.0f;
	balls[1].vel.z = 40.0f;
	balls[2].vel.x = 10.0f;
	balls[2].vel.y = 260.0f;
	balls[2].vel.z = 70.0f;
	balls[3].vel.x = 90.0f;
	balls[3].vel.y = 130.0f;
	balls[3].vel.z = -60.0f;
	balls[4].vel.x = -80.0f;
	balls[4].vel.y = 280.0f;
	balls[4].vel.z = -80.0f;

	for (int i = 0; i < 5; i++)
	{
		balls[i].pos.x = 0.0f;
		balls[i].pos.y = -0.001f;
		balls[i].pos.z = 0.0f;
		balls[i].x = 0;
		balls[i].y = 0;
		balls[i].bBounce = false;
		balls[i].bStop = false;
	}
}
#endif

static void UpdateBalls(sLoadingBall* balls, float dt)
{
	if (0.13f < dt)
		dt = 0.13f;

	for (int i = 0; i < 5; i++)
	{
		balls[i].delta.x = balls[i].vel.x * dt;
		balls[i].delta.y = balls[i].vel.y * dt;
		balls[i].delta.z = balls[i].vel.z * dt;
		balls[i].pos.x += balls[i].delta.x * dt;
		balls[i].pos.y += balls[i].delta.y * dt;
		balls[i].pos.z += balls[i].delta.z * dt;
		if (!balls[i].bStop)
		{
			balls[i].vel.x -= balls[i].delta.x * 0.1f;
			balls[i].vel.y -= 9.8f;
			balls[i].vel.z -= balls[i].delta.y * 0.1f;
		}

		if (balls[i].pos.y <= 0.0f && balls[i].bBounce)
		{
			balls[i].pos.y = 0.001f;
			balls[i].vel.y *= -0.4f;

			if (fabs(balls[i].vel.y) < 30.0f)
			{
				balls[i].vel.x = 0.0f;
				balls[i].vel.y = 0.0f;
				balls[i].vel.z = 0.0f;
				balls[i].bStop = true;
			}
		}
		else
		{
			balls[i].bBounce = true;
		}
	}
}

#ifdef REBANG_SEMANTIC_ONLY
unsigned int __stdcall DrawLoadingByThread(void* pArg)
{
	char szPath[MAX_PATH];
	char drive[4], dir[MAX_PATH], fname[128], ext[8];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	_splitpath(szPath, drive, dir, fname, ext);

	char szDll[128];
	char szSuffix[20];
	strcpy(szDll, "LoadingRes");
	char* p = strrchr(fname, '_');
	if (p)
	{
		strcpy(szSuffix, p);
		strcat(szDll, szSuffix);
		LogOut(0, "%s\n", szSuffix);
	}
	strcat(szDll, ".dll");

	HINSTANCE hInst = LoadLibrary(szDll);
	if (hInst)
	{
		Sleep(100);
		if (g_iLoadCurNum >= g_iLoadTotalNum)
			g_bExitDrawLoading = true;

		HWND hWnd = g_hwnd;
		HDC hdc = GetDC(hWnd);
		RECT rc;
		GetClientRect(hWnd, &rc);

		DWORD r = timeGetTime();

		r %= 3;
		DWORD elapsed = 0;
		DWORD type = Min(r, (DWORD)2);

		sLoadingBall balls[5];
		InitBalls(balls);

		void* pBackBits;
		HDC hdcBack;
		HGDIOBJ hOldBack;
		{
			BITMAPINFO bi;
			memset(&bi, 0, sizeof(bi));
			pBackBits = NULL;
			bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bi.bmiHeader.biWidth = rc.right - rc.left;
			bi.bmiHeader.biHeight = rc.bottom - rc.top;
			bi.bmiHeader.biPlanes = 1;
			bi.bmiHeader.biBitCount = 24;
			bi.bmiHeader.biCompression = BI_RGB;
			bi.bmiHeader.biSizeImage = 0;
			bi.bmiHeader.biClrUsed = 0;
			bi.bmiHeader.biClrImportant = 0;
			hdcBack = CreateCompatibleDC(hdc);
			HBITMAP hBack = CreateDIBSection(hdcBack, &bi, DIB_RGB_COLORS,
				&pBackBits, NULL, 0);
			hOldBack = SelectObject(hdcBack, hBack);
		}

		void* pGaugeBits;
		HDC hdcGauge;
		HGDIOBJ hOldGauge;
		int gaugePitch;
		LOAD_DIB_BITMAP(108, hdcGauge, hOldGauge, pGaugeBits, gaugePitch);

		void* pBallBits;
		HDC hdcBall;
		HGDIOBJ hOldBall;
		int ballPitch;
		LOAD_DIB_BITMAP(101, hdcBall, hOldBall, pBallBits, ballPitch);

		int id;
		if (type == 0)
			id = 129;
		else
			id = (type == 1) ? 130 : 131;

		HDC hdcAni, hdcBar, hdcNumber, hdcLogo, hdcCopyright;
		HGDIOBJ hOldAni, hOldBar, hOldNumber, hOldLogo, hOldCopyright;
		LOAD_BITMAP_DC(id, hdcAni, hOldAni);
		LOAD_BITMAP_DC(MAKEINTRESOURCE(103), hdcBar, hOldBar);
		LOAD_BITMAP_DC(MAKEINTRESOURCE(109), hdcNumber, hOldNumber);
		LOAD_BITMAP_DC(MAKEINTRESOURCE(122), hdcLogo, hOldLogo);
		LOAD_BITMAP_DC(MAKEINTRESOURCE(123), hdcCopyright, hOldCopyright);

		FillRect(hdcBack, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

		BitBlt(hdcBack, 685, 3, 118, 134, hdcLogo, 0, 0, SRCCOPY);
		BitBlt(hdcBack, 519, 521, 275, 72, hdcCopyright, 0, 0, SRCCOPY);

		DWORD lastTime = timeGetTime();

		DWORD waitTime = 0;
		bool bSound = false;
		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		while (!g_bExitDrawLoading)
		{
			Sleep(20);

			DWORD curTime = timeGetTime();
			DWORD dt = curTime - lastTime;
			lastTime = curTime;
			elapsed += dt;

			if (g_iLoadCurNum < g_iLoadNextNum)
			{
				g_iLoadCurNum++;
			}
			else
			{
				if (g_iLoadCurNum >= g_iLoadTotalNum)
				{
					waitTime += dt;
					if (waitTime > 2000)
						break;
				}
			}

			float ratio = (float)g_iLoadCurNum / g_iLoadTotalNum;

			if (type == 0)
			{
				DWORD frame = Min(elapsed % 800 / 50, (DWORD)15);
				BitBlt(hdcBack, 356, 169, 117, 190, hdcAni, 117 * (frame % 4),
					190 * (frame / 4), SRCCOPY);
			}
			else
			{
				DWORD frame = Min(elapsed % 1000 / 50, (DWORD)19);
				BitBlt(hdcBack, 356, 169, 87, 182, hdcAni, 87 * (frame % 5),
					182 * (frame / 5), SRCCOPY);
			}

			if (g_iLoadCurNum < g_iLoadTotalNum)
			{
				CLEAR_RECT(hdcBack, 300, 330, 220, 20);
				BitBlt(hdcBack, 500, 240, 31, 119, hdcBar, 0, 10, SRCCOPY);
				DrawPercent(hdcBack, 490, 240, hdcNumber, ratio);

				DWORD frame = Min((1000 - elapsed % 1000) / 90, (DWORD)10);

				static int s_x;
				static int s_y;
				s_x = 300 - (int)(ratio * -200.0f);
				s_y = 330;
				DrawSprite((BYTE*)pBackBits, s_x, s_y, 20, 20, width * 3,
					height, (BYTE*)pGaugeBits, 20 * (frame % 11), 0, 20, 20,
					gaugePitch);
			}
			else
			{
				if (!bSound)
				{
					if (COption::Instance()->aIsMssEnabled())
					{
						PlaySound(MAKEINTRESOURCE(121), hInst,
							SND_RESOURCE | SND_ASYNC);
					}
					bSound = true;
				}

				CLEAR_RECT(hdcBack, 300, 330, 220, 20);

				for (int i = 0; i < 5; i++)
					CLEAR_RECT(hdcBack, balls[i].x, balls[i].y, 19, 19);

				BitBlt(hdcBack, 500, 240, 31, 119, hdcBar, 31, 10, SRCCOPY);
				UpdateBalls(balls, (float)dt * 0.001f * 5.0f);
				DWORD fr = elapsed % 300 / 42;

				for (int j = 0; j < 5; j++)
				{
					DWORD f = Min(fr, (DWORD)6);
					balls[j].x = (int)balls[j].pos.x + 510;
					balls[j].y = 320 - (int)(balls[j].pos.z * 0.15f) -
						(int)balls[j].pos.y;
					DrawSprite((BYTE*)pBackBits, balls[j].x, balls[j].y, 19, 19,
						width * 3, height, (BYTE*)pBallBits, 19 * (f % 7), 0,
						19, 19, ballPitch);
				}
			}

			BitBlt(hdc, 0, 0, width, height, hdcBack, 0, 0, SRCCOPY);
		}

		FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

		DeleteObject(SelectObject(hdcCopyright, hOldCopyright));
		DeleteObject(SelectObject(hdcLogo, hOldLogo));

		DeleteObject(SelectObject(hdcNumber, hOldNumber));
		DeleteObject(SelectObject(hdcBar, hOldBar));
		DeleteObject(SelectObject(hdcBall, hOldBall));
		DeleteObject(SelectObject(hdcGauge, hOldGauge));
		DeleteObject(SelectObject(hdcAni, hOldAni));
		DeleteObject(SelectObject(hdcBack, hOldBack));

		DeleteDC(hdcCopyright);
		DeleteDC(hdcLogo);

		DeleteDC(hdcNumber);
		DeleteDC(hdcBar);
		DeleteDC(hdcBall);
		DeleteDC(hdcGauge);
		DeleteDC(hdcAni);
		DeleteDC(hdcBack);
		ReleaseDC(hWnd, hdc);
	}

	CloseHandle(g_hDrawLoading);
	g_hDrawLoading = NULL;
	g_iLoadingThreadID = 0;
	if (hInst)
		FreeLibrary(hInst);

	_endthreadex(0);

	return 0;
}
#endif

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == VK_SNAPSHOT)
		return 0;

	return CallNextHookEx(hhook, nCode, wParam, lParam);
}

static void RunProjectG(HWND hWnd)
{
	new CProjectG(hWnd);
	new CTikiMagicBoxDoc;

	if (CProjectG::Instance()->Init())
	{
		ShowLoading(true);
		HideSplash();

		if (CProjectG::Instance()->Ready())
		{
			COption::Instance()->vApplyLobbyScreenSize();
			CMouseCursor::Instance()->SetMode(0);

			g_input->SetMousePointToLoginBox();

			new CBrowser;
			RECT rc;
			GetClientRect(hWnd, &rc);
			CBrowser::Instance()->Init(g_instance, hWnd, rc);

			WaitForSingleObject(g_hDrawLoading, 90000);

			ShowSplash(false);

			ShowLoading(false);
			_GGC_SetHwnd(g_hwnd);

			MSG msg;
			int ret = -1;

			while (ret <= 0)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
				{
					if (!GetMessage(&msg, NULL, 0, 0))
					{
						_GGC_End();
						break;
					}

					BOOL bRet = FALSE;
					if (CBrowser::IsInstantiated())
						bRet = CBrowser::Instance()->PreTranslateMessage(&msg);

					if (!bRet)
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				else
				{
					ret = CProjectG::Instance()->MainLoop(0);
				}
			}

			if (CBrowser::IsInstantiated())
				delete CBrowser::Instance();
		}
	}

	ShowLoading(false);

	CLoginInfo::Instance()->Destroy();
	if (CTikiMagicBoxDoc::IsInstantiated())
		delete CTikiMagicBoxDoc::Instance();
	if (CProjectG::IsInstantiated())
		delete CProjectG::Instance();
}

HANDLE WINAPI GetPangYaMutex()
{
	HANDLE hMutex =
		CreateMutex(NULL, TRUE, "{071784A2-EE35-4e6a-92D0-6E7A4B985171}");
	if (hMutex == NULL)
	{
		ShowSplash(false);
		return NULL;
	}

	DWORD dwError = GetLastError();
	if (dwError == 182 || dwError == ERROR_ACCESS_DENIED)
	{
		HWND hWnd = FindWindow(class_name, NULL);
		if (hWnd)
		{
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
		}

		ShowSplash(false);
		return NULL;
	}

	return hMutex;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	LFHInstaller::install();

	InitLocalizeSystem();

	CoInitialize(NULL);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);

	_Module.Init(NULL, hInstance);

	AtlAxWinInit();

	RECT rc = { 0, 0, 800, 600 };

	strcpy(class_name, "PangYa");

	HANDLE hMutex = GetPangYaMutex();
	if (hMutex)
	{
		int webLogin = CLoginInfo::Instance()->InitWebLogin(GetCommandLine());

		if (webLogin)
		{
			ShellExecute(NULL, NULL, "http://qa.pangya.gametree.co.kr/", NULL,
				NULL, SW_SHOW);
		}
		else
		{
			ShowSplash(true);

			g_instance = hInstance;
			WNDCLASS wc;
			wc.style = 0;
			wc.lpfnWndProc = WinProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = class_name;
			if (RegisterClass(&wc))
			{
				if (!CheckWndVersion())
				{
					MessageBox(NULL,
						K2L_Compatibility(
							"\300\251\265\265\277\354\301\356 98 \310\244\300\272 \300\251\265\265\277\354\301\356 2000 \300\314\273\363\300\307 \277\356\277\265\303\274\301\246\260\241 \307\312\277\344\307\325\264\317\264\331"),
						K2L_Compatibility("\306\316\276\337"), MB_OK);
				}
				else
				{
					AdjustWindowRectEx(&rc, WS_CAPTION | WS_THICKFRAME, FALSE,
						0);
					int x =
						(GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) /
						2;
					int y =
						(GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) /
						2;
					OffsetRect(&rc, x, y);

					HWND hWnd = CreateWindowEx(0, class_name, window_name,
						WS_CAPTION | WS_THICKFRAME, x, y, rc.right - rc.left,
						rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
					ShowWindow(hWnd, SW_HIDE);

					if (CheckRelatedDLL(hWnd))
					{
						g_hwnd = hWnd;
						if (_GGC_Init())
						{
							SetWindowTitle(hWnd);
							RunProjectG(hWnd);
						}
					}
				}
			}
		}
	}

	ShowSplash(false);

	_Module.Term();
	CoUninitialize();

	if (hMutex)
	{
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	}

	_GGC_End();

	return 0;
}

#ifndef REBANG_SEMANTIC_ONLY
extern "C" int rb_C__04HLONOPDM__4dll__AA;
#pragma comment(linker, \
	"/alternatename:_rb_C__04HLONOPDM__4dll__AA=??_C@_04HLONOPDM@?4dll?$AA@")
#pragma warning(push)
#pragma warning(disable : 4325)
#pragma section(".rdata", read)
#pragma warning(pop)

__declspec(naked) unsigned int __stdcall DrawLoadingByThread(void* pArg)
{
	static int s_x;
	static int s_y;
	__declspec(allocate(".rdata")) static const unsigned long s_dtScale =
		0x3ba3d70b;
	__declspec(allocate(".rdata")) static const unsigned long s_minus200 =
		0xc3480000;
	__declspec(allocate(".rdata")) static const char s_use24[] =
		"Use 24bit bitmap!!!";
	__declspec(allocate(".rdata")) static const char s_alert[] = "alert!";
	__declspec(allocate(".rdata")) static const char s_loadingRes[] =
		"LoadingRes";
	__asm {
		push ebp
		mov ebp, esp
		sub esp, 588h
		mov eax, dword ptr [rb_security_cookie]
		push esi
		xor eax, ebp
		push edi
		mov dword ptr [ebp-4], eax
		push 104h
		lea eax, [ebp-250h]
		push eax
		xor esi, esi
		push esi
		call dword ptr [GetModuleFileNameA]
		lea ecx, [ebp-4Ch]
		push ecx
		lea edx, [ebp-14Ch]
		push edx
		lea eax, [ebp-354h]
		push eax
		lea ecx, [ebp-444h]
		push ecx
		lea edx, [ebp-250h]
		push edx
		call dword ptr [_splitpath]
		mov ecx, dword ptr [s_loadingRes+4h]
		mov eax, dword ptr [s_loadingRes]
		mov dx, word ptr [s_loadingRes+8h]
		mov dword ptr [ebp-0C8h], ecx
		mov dword ptr [ebp-0CCh], eax
		mov al, byte ptr [s_loadingRes+0Ah]
		lea ecx, [ebp-14Ch]
		push 5Fh
		push ecx
		mov word ptr [ebp-0C4h], dx
		mov byte ptr [ebp-0C2h], al
		call dword ptr [strrchr]
		add esp, 1Ch
		cmp eax, esi
		je L_00e1
		lea edx, [ebp-44h]
		sub edx, eax
		jmp L_00a0
		_emit 0x8d
		_emit 0x49
		_emit 0x00
L_00a0:
		mov cl, byte ptr [eax]
		mov byte ptr [edx+eax], cl
		add eax, 1
		test cl, cl
		jne L_00a0
		lea eax, [ebp-44h]
		mov edx, eax
L_00b1:
		mov cl, byte ptr [eax]
		add eax, 1
		test cl, cl
		jne L_00b1
		lea edi, [ebp-0CCh]
		sub eax, edx
		add edi, -1
L_00c5:
		mov cl, byte ptr [edi+1]
		add edi, 1
		test cl, cl
		jne L_00c5
		mov ecx, eax
		shr ecx, 2
		mov esi, edx
		rep movsd
		mov ecx, eax
		and ecx, 3
		rep movsb
		xor esi, esi
L_00e1:
		lea edi, [ebp-0CCh]
		add edi, -1
		_emit 0x8d
		_emit 0x9b
		_emit 0x00
		_emit 0x00
		_emit 0x00
		_emit 0x00
L_00f0:
		mov al, byte ptr [edi+1]
		add edi, 1
		test al, al
		jne L_00f0
		mov edx, dword ptr [rb_C__04HLONOPDM__4dll__AA]
	}
	*(volatile char*)(".dll" + 4);
	__asm {
		lea ecx, [ebp-0CCh]
		mov dword ptr [edi], edx
		push ecx
		mov byte ptr [edi+4], al
		call dword ptr [LoadLibraryA]
		mov edi, eax
		cmp edi, esi
		mov dword ptr [ebp-36Ch], edi
		je L_0db4
		push 64h
		call dword ptr [Sleep]
		mov edx, dword ptr [rb_g_iLoadCurNum__3HA]
		cmp edx, dword ptr [rb_g_iLoadTotalNum__3HA]
		jl L_0144
		mov byte ptr [rb_g_bExitDrawLoading__3_NA], 1
L_0144:
		mov edi, dword ptr [rb_g_hwnd__3PAUHWND____A]
		push ebx
		push edi
		mov dword ptr [ebp-404h], edi
		call dword ptr [GetDC]
		mov ebx, eax
		lea eax, [ebp-3C0h]
		push eax
		push edi
		mov dword ptr [ebp-37Ch], ebx
		call dword ptr [GetClientRect]
		call dword ptr [timeGetTime]
		xor edx, edx
		mov ecx, 3
		div ecx
		mov dword ptr [ebp-374h], esi
		mov dword ptr [ebp-398h], 2
		cmp edx, 2
		ja L_0198
		mov dword ptr [ebp-398h], edx
L_0198:
		lea eax, [ebp-588h]
		call InitBalls
		mov edx, dword ptr [ebp-3B8h]
		sub edx, dword ptr [ebp-3C0h]
		xor eax, eax
		mov ecx, 0Bh
		lea edi, [ebp-3ECh]
		rep stosd
		mov eax, dword ptr [ebp-3B4h]
		sub eax, dword ptr [ebp-3BCh]
		push ebx
		mov dword ptr [ebp-394h], esi
		mov dword ptr [ebp-3ECh], 28h
		mov dword ptr [ebp-3E8h], edx
		mov dword ptr [ebp-3E4h], eax
		mov word ptr [ebp-3E0h], 1
		mov word ptr [ebp-3DEh], 18h
		mov dword ptr [ebp-3DCh], esi
		mov dword ptr [ebp-3D8h], esi
		mov dword ptr [ebp-3CCh], esi
		mov dword ptr [ebp-3C8h], esi
		call dword ptr [CreateCompatibleDC]
		push esi
		push esi
		lea ecx, [ebp-394h]
		push ecx
		push esi
		lea edx, [ebp-3ECh]
		mov ebx, eax
		push edx
		push ebx
		call dword ptr [CreateDIBSection]
		mov esi, dword ptr [SelectObject]
		push eax
		push ebx
		call esi
		mov dword ptr [ebp-41Ch], eax
		mov eax, dword ptr [ebp-36Ch]
		push 6Ch
		push eax
		mov dword ptr [ebp-3A4h], 0
		call dword ptr [LoadBitmapA]
		lea ecx, [ebp-3D8h]
		push ecx
		mov edi, eax
		push 18h
		push edi
		mov dword ptr [ebp-370h], edi
		call dword ptr [GetObjectA]
		lea edx, [ebp-3D8h]
		push edx
		push 18h
		push edi
		call dword ptr [GetObjectA]
		test eax, eax
		jne L_028c
		push edi
		jmp L_03c3
L_028c:
		xor eax, eax
		mov ecx, 0Bh
		lea edi, [ebp-30h]
		rep stosd
		movzx eax, word ptr [ebp-3D4h]
		movzx ecx, word ptr [ebp-3D0h]
		mov dword ptr [ebp-2Ch], eax
		xor eax, eax
		push eax
		mov dword ptr [ebp-30h], 28h
		mov dword ptr [ebp-28h], ecx
		mov word ptr [ebp-24h], 1
		mov word ptr [ebp-22h], 18h
		mov dword ptr [ebp-20h], eax
		mov dword ptr [ebp-10h], eax
		mov dword ptr [ebp-0Ch], eax
		mov dword ptr [ebp-1Ch], eax
		call dword ptr [GetDC]
		push 0
		push 0
		lea edx, [ebp-3A4h]
		push edx
		mov edi, eax
		push 0
		lea eax, [ebp-30h]
		push eax
		push edi
		call dword ptr [CreateDIBSection]
		push edi
		push 0
		mov dword ptr [ebp-368h], eax
		call dword ptr [ReleaseDC]
		mov edx, dword ptr [ebp-368h]
		lea ecx, [ebp-498h]
		push ecx
		push 54h
		push edx
		call dword ptr [GetObjectA]
		cmp word ptr [ebp-486h], 18h
		mov eax, dword ptr [ebp-48Ch]
		mov dword ptr [ebp-400h], eax
		je L_033d
		push 0
		push offset s_alert
		push offset s_use24
		push 0
		call dword ptr [MessageBoxA]
L_033d:
		push 0
		call dword ptr [CreateCompatibleDC]
		push 0
		mov edi, eax
		call dword ptr [CreateCompatibleDC]
		mov ecx, dword ptr [ebp-370h]
		push ecx
		push edi
		mov dword ptr [ebp-388h], eax
		call esi
		mov edx, dword ptr [ebp-368h]
		mov dword ptr [ebp-358h], eax
		mov eax, dword ptr [ebp-388h]
		push edx
		push eax
		call esi
		mov ecx, dword ptr [ebp-3D0h]
		mov edx, dword ptr [ebp-3D4h]
		push 0CC0020h
		push 0
		push 0
		push edi
		push ecx
		push edx
		push 0
		mov dword ptr [ebp-3F8h], eax
		mov eax, dword ptr [ebp-388h]
		push 0
		push eax
		call dword ptr [BitBlt]
		mov ecx, dword ptr [ebp-358h]
		push ecx
		push edi
		call esi
		push eax
		call dword ptr [DeleteObject]
		push edi
		call dword ptr [DeleteDC]
		mov edx, dword ptr [ebp-370h]
		push edx
L_03c3:
		call dword ptr [DeleteObject]
		mov eax, dword ptr [ebp-36Ch]
		push 65h
		push eax
		mov dword ptr [ebp-3A8h], 0
		call dword ptr [LoadBitmapA]
		lea ecx, [ebp-3D8h]
		push ecx
		mov edi, eax
		push 18h
		push edi
		mov dword ptr [ebp-370h], edi
		call dword ptr [GetObjectA]
		lea edx, [ebp-3D8h]
		push edx
		push 18h
		push edi
		call dword ptr [GetObjectA]
		test eax, eax
		jne L_0414
		push edi
		jmp L_054b
L_0414:
		xor eax, eax
		mov ecx, 0Bh
		lea edi, [ebp-30h]
		rep stosd
		movzx eax, word ptr [ebp-3D4h]
		movzx ecx, word ptr [ebp-3D0h]
		mov dword ptr [ebp-2Ch], eax
		xor eax, eax
		push eax
		mov dword ptr [ebp-30h], 28h
		mov dword ptr [ebp-28h], ecx
		mov word ptr [ebp-24h], 1
		mov word ptr [ebp-22h], 18h
		mov dword ptr [ebp-20h], eax
		mov dword ptr [ebp-10h], eax
		mov dword ptr [ebp-0Ch], eax
		mov dword ptr [ebp-1Ch], eax
		call dword ptr [GetDC]
		push 0
		push 0
		lea edx, [ebp-3A8h]
		push edx
		mov edi, eax
		push 0
		lea eax, [ebp-30h]
		push eax
		push edi
		call dword ptr [CreateDIBSection]
		push edi
		push 0
		mov dword ptr [ebp-368h], eax
		call dword ptr [ReleaseDC]
		mov edx, dword ptr [ebp-368h]
		lea ecx, [ebp-498h]
		push ecx
		push 54h
		push edx
		call dword ptr [GetObjectA]
		cmp word ptr [ebp-486h], 18h
		mov eax, dword ptr [ebp-48Ch]
		mov dword ptr [ebp-3F0h], eax
		je L_04c5
		push 0
		push offset s_alert
		push offset s_use24
		push 0
		call dword ptr [MessageBoxA]
L_04c5:
		push 0
		call dword ptr [CreateCompatibleDC]
		push 0
		mov edi, eax
		call dword ptr [CreateCompatibleDC]
		mov ecx, dword ptr [ebp-370h]
		push ecx
		push edi
		mov dword ptr [ebp-380h], eax
		call esi
		mov edx, dword ptr [ebp-368h]
		mov dword ptr [ebp-358h], eax
		mov eax, dword ptr [ebp-380h]
		push edx
		push eax
		call esi
		mov ecx, dword ptr [ebp-3D0h]
		mov edx, dword ptr [ebp-3D4h]
		push 0CC0020h
		push 0
		push 0
		push edi
		push ecx
		push edx
		push 0
		mov dword ptr [ebp-40Ch], eax
		mov eax, dword ptr [ebp-380h]
		push 0
		push eax
		call dword ptr [BitBlt]
		mov ecx, dword ptr [ebp-358h]
		push ecx
		push edi
		call esi
		push eax
		call dword ptr [DeleteObject]
		push edi
		call dword ptr [DeleteDC]
		mov edx, dword ptr [ebp-370h]
		push edx
L_054b:
		call dword ptr [DeleteObject]
		mov eax, dword ptr [ebp-398h]
		test eax, eax
		jne L_0562
		mov eax, 81h
		jmp L_0572
L_0562:
		xor ecx, ecx
		cmp eax, 1
		setne cl
		add ecx, 82h
		mov eax, ecx
L_0572:
		mov edx, dword ptr [ebp-36Ch]
		push eax
		push edx
		call dword ptr [LoadBitmapA]
		mov edi, dword ptr [ebp-37Ch]
		push edi
		mov dword ptr [ebp-358h], eax
		call dword ptr [CreateCompatibleDC]
		mov ecx, dword ptr [ebp-358h]
		push ecx
		push eax
		mov dword ptr [ebp-384h], eax
		call esi
		mov edx, dword ptr [ebp-36Ch]
		push 67h
		push edx
		mov dword ptr [ebp-3FCh], eax
		call dword ptr [LoadBitmapA]
		push edi
		mov dword ptr [ebp-358h], eax
		call dword ptr [CreateCompatibleDC]
		mov ecx, dword ptr [ebp-358h]
		push ecx
		push eax
		mov dword ptr [ebp-378h], eax
		call esi
		mov edx, dword ptr [ebp-36Ch]
		push 6Dh
		push edx
		mov dword ptr [ebp-420h], eax
		call dword ptr [LoadBitmapA]
		push edi
		mov dword ptr [ebp-358h], eax
		call dword ptr [CreateCompatibleDC]
		mov ecx, dword ptr [ebp-358h]
		push ecx
		push eax
		mov dword ptr [ebp-368h], eax
		call esi
		mov edx, dword ptr [ebp-36Ch]
		push 7Ah
		push edx
		mov dword ptr [ebp-418h], eax
		call dword ptr [LoadBitmapA]
		push edi
		mov dword ptr [ebp-358h], eax
		call dword ptr [CreateCompatibleDC]
		mov ecx, dword ptr [ebp-358h]
		push ecx
		push eax
		mov dword ptr [ebp-370h], eax
		call esi
		mov edx, dword ptr [ebp-36Ch]
		push 7Bh
		push edx
		mov dword ptr [ebp-410h], eax
		call dword ptr [LoadBitmapA]
		push edi
		mov dword ptr [ebp-358h], eax
		call dword ptr [CreateCompatibleDC]
		mov ecx, dword ptr [ebp-358h]
		push ecx
		push eax
		mov dword ptr [ebp-390h], eax
		call esi
		push 4
		mov dword ptr [ebp-408h], eax
		call dword ptr [GetStockObject]
		push eax
		lea edx, [ebp-3C0h]
		push edx
		push ebx
		call dword ptr [FillRect]
		mov eax, dword ptr [ebp-370h]
		push 0CC0020h
		push 0
		push 0
		push eax
		push 86h
		push 76h
		push 3
		push 2ADh
		push ebx
		call dword ptr [BitBlt]
		mov ecx, dword ptr [ebp-390h]
		push 0CC0020h
		push 0
		push 0
		push ecx
		push 48h
		push 113h
		push 209h
		push 207h
		push ebx
		call dword ptr [BitBlt]
		call dword ptr [timeGetTime]
		mov dword ptr [ebp-3B0h], eax
		mov eax, dword ptr [ebp-3B8h]
		sub eax, dword ptr [ebp-3C0h]
		mov dword ptr [ebp-3A0h], 0
		mov dword ptr [ebp-39Ch], eax
		mov eax, dword ptr [ebp-3B4h]
		sub eax, dword ptr [ebp-3BCh]
		cmp byte ptr [rb_g_bExitDrawLoading__3_NA], 0
		mov byte ptr [ebp-361h], 0
		mov dword ptr [ebp-38Ch], eax
		jne L_0c8c
L_0723:
		push 14h
		call dword ptr [Sleep]
		call dword ptr [timeGetTime]
		mov edx, dword ptr [rb_g_iLoadCurNum__3HA]
		mov ecx, eax
		sub ecx, dword ptr [ebp-3B0h]
		mov dword ptr [ebp-3B0h], eax
		mov eax, dword ptr [ebp-374h]
		add eax, ecx
		cmp edx, dword ptr [rb_g_iLoadNextNum__3HA]
		mov dword ptr [ebp-358h], ecx
		mov dword ptr [ebp-374h], eax
		jge L_076c
		add edx, 1
		mov dword ptr [rb_g_iLoadCurNum__3HA], edx
		jmp L_078e
L_076c:
		cmp edx, dword ptr [rb_g_iLoadTotalNum__3HA]
		jl L_078e
		mov edx, dword ptr [ebp-3A0h]
		add edx, ecx
		cmp edx, 7D0h
		mov dword ptr [ebp-3A0h], edx
		ja L_0c8c
L_078e:
		fild dword ptr [rb_g_iLoadCurNum__3HA]
		xor edx, edx
		cmp dword ptr [ebp-398h], edx
		fidiv dword ptr [rb_g_iLoadTotalNum__3HA]
		fstp dword ptr [ebp-35Ch]
		jne L_0801
		mov ecx, 320h
		div ecx
		mov eax, 51EB851Fh
		mov ecx, 0Fh
		mul edx
		shr edx, 4
		cmp edx, 0Fh
		ja L_07c7
		mov ecx, edx
L_07c7:
		mov eax, ecx
		shr eax, 2
		and ecx, 3
		lea edx, [eax+eax*2]
		shl edx, 5
		sub edx, eax
		mov eax, ecx
		shl ecx, 4
		sub ecx, eax
		add ecx, ecx
		add ecx, ecx
		push 0CC0020h
		sub ecx, eax
		add edx, edx
		push edx
		mov edx, dword ptr [ebp-384h]
		add ecx, ecx
		sub ecx, eax
		push ecx
		push edx
		push 0BEh
		push 75h
		jmp L_085d
L_0801:
		mov ecx, 3E8h
		div ecx
		mov eax, 51EB851Fh
		mul edx
		shr edx, 4
		cmp edx, 13h
		mov eax, 13h
		ja L_081e
		mov eax, edx
L_081e:
		xor edx, edx
		mov ecx, 5
		div ecx
		push 0CC0020h
		lea ecx, [eax+eax*2]
		add ecx, ecx
		add ecx, ecx
		add ecx, ecx
		sub ecx, eax
		add ecx, ecx
		add ecx, ecx
		sub ecx, eax
		mov eax, edx
		shl eax, 4
		sub eax, edx
		add eax, eax
		add ecx, ecx
		push ecx
		sub eax, edx
		lea edx, [eax+eax*2]
		mov eax, dword ptr [ebp-384h]
		push edx
		push eax
		push 0B6h
		push 57h
L_085d:
		push 0A9h
		push 164h
		push ebx
		call dword ptr [BitBlt]
		mov ecx, dword ptr [rb_g_iLoadCurNum__3HA]
		cmp ecx, dword ptr [rb_g_iLoadTotalNum__3HA]
		jge L_09cc
		mov edi, 14Ah
		push 4
		mov dword ptr [ebp-440h], 12Ch
		mov dword ptr [ebp-438h], 208h
		mov dword ptr [ebp-43Ch], edi
		mov dword ptr [ebp-434h], 15Eh
		call dword ptr [GetStockObject]
		push eax
		lea edx, [ebp-440h]
		push edx
		push ebx
		call dword ptr [FillRect]
		mov eax, dword ptr [ebp-378h]
		push 0CC0020h
		push 0Ah
		push 0
		push eax
		push 77h
		push 1Fh
		push 0F0h
		push 1F4h
		push ebx
		call dword ptr [BitBlt]
		mov ecx, dword ptr [ebp-35Ch]
		mov edx, dword ptr [ebp-368h]
		push ecx
		push edx
		mov esi, 0F0h
		mov eax, 1EAh
		call DrawPercent
		mov eax, dword ptr [ebp-374h]
		xor edx, edx
		mov ecx, 3E8h
		div ecx
		mov eax, 6C16C16Dh
		sub ecx, edx
		mul ecx
		sub ecx, edx
		shr ecx, 1
		add ecx, edx
		shr ecx, 6
		cmp ecx, 0Ah
		jbe L_092d
		mov ecx, 0Ah
L_092d:
		fld dword ptr [ebp-35Ch]
		mov esi, 12Ch
		fmul dword ptr [s_minus200]
		mov dword ptr [s_y], edi
		fnstcw word ptr [ebp-35Ah]
		mov edi, dword ptr [ebp-39Ch]
		movzx eax, word ptr [ebp-35Ah]
		or ah, 0Ch
		mov dword ptr [ebp-358h], eax
		fldcw word ptr [ebp-358h]
		mov eax, dword ptr [ebp-400h]
		push eax
		mov eax, ecx
		mov ecx, 0Bh
		push 14h
		fistp dword ptr [ebp-358h]
		push 14h
		mov edx, dword ptr [ebp-358h]
		sub esi, edx
		xor edx, edx
		div ecx
		fldcw word ptr [ebp-35Ah]
		mov eax, dword ptr [ebp-3A4h]
		lea ecx, [edi+edi*2]
		mov dword ptr [s_x], esi
		lea edx, [edx+edx*4]
		add edx, edx
		add edx, edx
		push edx
		mov edx, dword ptr [ebp-394h]
		push eax
		mov eax, dword ptr [ebp-38Ch]
		push ecx
		push 14h
		push 14h
		push 14Ah
		push esi
		push edx
		xor ecx, ecx
		call DrawSprite
		jmp L_0c50
L_09cc:
		cmp byte ptr [ebp-361h], 0
		jne L_09ff
		mov ecx, dword ptr [rb_m_pInstance___WSingleton_VCOption____1PAVCOption__A]
		call rb_aIsMssEnabled_COption__QAE_NXZ
		test al, al
		je L_09f8
		mov eax, dword ptr [ebp-36Ch]
		push 40005h
		push eax
		push 79h
		call dword ptr [PlaySoundA]
L_09f8:
		mov byte ptr [ebp-361h], 1
L_09ff:
		push 4
		mov dword ptr [ebp-430h], 12Ch
		mov dword ptr [ebp-428h], 208h
		mov dword ptr [ebp-42Ch], 14Ah
		mov dword ptr [ebp-424h], 15Eh
		call dword ptr [GetStockObject]
		push eax
		lea ecx, [ebp-430h]
		push ecx
		push ebx
		call dword ptr [FillRect]
		lea esi, [ebp-560h]
		mov edi, 5
		_emit 0x8d
		_emit 0xa4
		_emit 0x24
		_emit 0x00
		_emit 0x00
		_emit 0x00
		_emit 0x00
L_0a50:
		mov eax, dword ptr [esi-4]
		mov dword ptr [ebp-3D0h], eax
		add eax, 13h
		mov dword ptr [ebp-3C8h], eax
		mov eax, dword ptr [esi]
		mov dword ptr [ebp-3CCh], eax
		add eax, 13h
		push 4
		mov dword ptr [ebp-3C4h], eax
		call dword ptr [GetStockObject]
		push eax
		lea edx, [ebp-3D0h]
		push edx
		push ebx
		call dword ptr [FillRect]
		add esi, 30h
		sub edi, 1
		jne L_0a50
		mov eax, dword ptr [ebp-378h]
		push 0CC0020h
		push 0Ah
		push 1Fh
		push eax
		push 77h
		push 1Fh
		push 0F0h
		push 1F4h
		push ebx
		call dword ptr [BitBlt]
		fild dword ptr [ebp-358h]
		mov ecx, dword ptr [ebp-358h]
		test ecx, ecx
		jge L_0acd
		fadd dword ptr [rb_real_4f800000]
L_0acd:
		fmul dword ptr [s_dtScale]
		push ecx
		lea ecx, [ebp-588h]
		fstp dword ptr [esp]
		call UpdateBalls
		mov eax, dword ptr [ebp-374h]
		xor edx, edx
		mov ecx, 12Ch
		div ecx
		mov eax, 86186187h
		lea esi, [ebp-564h]
		mov dword ptr [ebp-358h], 5
		mov ecx, edx
		mul ecx
		mov eax, dword ptr [ebp-39Ch]
		sub ecx, edx
		shr ecx, 1
		add ecx, edx
		shr ecx, 5
		lea edx, [eax+eax*2]
		mov dword ptr [ebp-414h], ecx
		mov dword ptr [ebp-3F4h], edx
		jmp L_0b30
L_0b2a:
		mov ecx, dword ptr [ebp-414h]
L_0b30:
		cmp ecx, 6
		mov edx, 6
		ja L_0b3c
		mov edx, ecx
L_0b3c:
		fld dword ptr [esi-0Ch]
		mov ecx, 140h
		fnstcw word ptr [ebp-35Ah]
		mov edi, 7
		movzx eax, word ptr [ebp-35Ah]
		or ah, 0Ch
		mov dword ptr [ebp-360h], eax
		fldcw word ptr [ebp-360h]
		fistp dword ptr [ebp-360h]
		mov eax, dword ptr [ebp-360h]
		add eax, 1FEh
		mov dword ptr [ebp-3ACh], eax
		fldcw word ptr [ebp-35Ah]
		mov dword ptr [esi], eax
		fld dword ptr [esi-4]
		fmul dword ptr [rb_real_3e19999a]
		fnstcw word ptr [ebp-35Ah]
		movzx eax, word ptr [ebp-35Ah]
		or ah, 0Ch
		mov dword ptr [ebp-360h], eax
		fldcw word ptr [ebp-360h]
		fistp dword ptr [ebp-360h]
		mov eax, dword ptr [ebp-360h]
		sub ecx, eax
		fldcw word ptr [ebp-35Ah]
		fnstcw word ptr [ebp-35Ah]
		fld dword ptr [esi-8]
		movzx eax, word ptr [ebp-35Ah]
		or ah, 0Ch
		mov dword ptr [ebp-360h], eax
		mov eax, dword ptr [ebp-3F0h]
		fldcw word ptr [ebp-360h]
		push eax
		mov eax, edx
		xor edx, edx
		div edi
		fistp dword ptr [ebp-360h]
		fldcw word ptr [ebp-35Ah]
		sub ecx, dword ptr [ebp-360h]
		push 13h
		push 13h
		mov dword ptr [esi+4], ecx
		lea eax, [edx+edx*4]
		add eax, eax
		add eax, eax
		sub eax, edx
		mov edx, dword ptr [ebp-3A8h]
		push eax
		mov eax, dword ptr [ebp-3F4h]
		push edx
		mov edx, dword ptr [ebp-394h]
		push eax
		mov eax, dword ptr [ebp-38Ch]
		push 13h
		push 13h
		push ecx
		mov ecx, dword ptr [ebp-3ACh]
		push ecx
		push edx
		xor ecx, ecx
		call DrawSprite
		add esi, 30h
		sub dword ptr [ebp-358h], 1
		jne L_0b2a
		mov edi, dword ptr [ebp-39Ch]
L_0c50:
		mov eax, dword ptr [ebp-38Ch]
		mov ecx, dword ptr [ebp-37Ch]
		push 0CC0020h
		push 0
		push 0
		push ebx
		push eax
		push edi
		push 0
		push 0
		push ecx
		call dword ptr [BitBlt]
		cmp byte ptr [rb_g_bExitDrawLoading__3_NA], 0
		mov edi, dword ptr [ebp-37Ch]
		mov esi, dword ptr [SelectObject]
		je L_0723
L_0c8c:
		push 4
		call dword ptr [GetStockObject]
		push eax
		lea edx, [ebp-3C0h]
		push edx
		push edi
		call dword ptr [FillRect]
		mov eax, dword ptr [ebp-408h]
		mov ecx, dword ptr [ebp-390h]
		push eax
		push ecx
		call esi
		push eax
		call dword ptr [DeleteObject]
		mov edx, dword ptr [ebp-410h]
		mov eax, dword ptr [ebp-370h]
		push edx
		push eax
		call esi
		push eax
		call dword ptr [DeleteObject]
		mov ecx, dword ptr [ebp-418h]
		mov edx, dword ptr [ebp-368h]
		push ecx
		push edx
		call esi
		push eax
		call dword ptr [DeleteObject]
		mov eax, dword ptr [ebp-420h]
		mov ecx, dword ptr [ebp-378h]
		push eax
		push ecx
		call esi
		push eax
		call dword ptr [DeleteObject]
		mov edx, dword ptr [ebp-40Ch]
		mov eax, dword ptr [ebp-380h]
		push edx
		push eax
		call esi
		push eax
		call dword ptr [DeleteObject]
		mov ecx, dword ptr [ebp-3F8h]
		mov edx, dword ptr [ebp-388h]
		push ecx
		push edx
		call esi
		push eax
		call dword ptr [DeleteObject]
		mov eax, dword ptr [ebp-3FCh]
		mov ecx, dword ptr [ebp-384h]
		push eax
		push ecx
		call esi
		push eax
		call dword ptr [DeleteObject]
		mov edx, dword ptr [ebp-41Ch]
		push edx
		push ebx
		call esi
		push eax
		call dword ptr [DeleteObject]
		mov eax, dword ptr [ebp-390h]
		mov esi, dword ptr [DeleteDC]
		push eax
		call esi
		mov ecx, dword ptr [ebp-370h]
		push ecx
		call esi
		mov edx, dword ptr [ebp-368h]
		push edx
		call esi
		mov eax, dword ptr [ebp-378h]
		push eax
		call esi
		mov ecx, dword ptr [ebp-380h]
		push ecx
		call esi
		mov edx, dword ptr [ebp-388h]
		push edx
		call esi
		mov eax, dword ptr [ebp-384h]
		push eax
		call esi
		push ebx
		call esi
		mov ecx, dword ptr [ebp-404h]
		push edi
		push ecx
		call dword ptr [ReleaseDC]
		mov edi, dword ptr [ebp-36Ch]
		xor esi, esi
		pop ebx
L_0db4:
		mov edx, dword ptr [rb_g_hDrawLoading__3PAXA]
		push edx
		call dword ptr [CloseHandle]
		cmp edi, esi
		mov dword ptr [rb_g_hDrawLoading__3PAXA], esi
		mov dword ptr [rb_g_iLoadingThreadID__3KA], esi
		je L_0dd8
		push edi
		call dword ptr [FreeLibrary]
L_0dd8:
		push esi
		call dword ptr [_endthreadex]
		mov ecx, dword ptr [ebp-4]
		add esp, 4
		pop edi
		xor ecx, ebp
		xor eax, eax
		pop esi
		call rb_security_check_cookie_4
		mov esp, ebp
		pop ebp
		ret 4
	}
}
#endif

void ShowSplash(bool bShow)
{
	if (bShow)
	{
		new CSplash;
		CSplash::Instance()->SetBitmap("PangYaSetup.bmp");
		CSplash::Instance()->SetTransparentColor(RGB(0, 255, 0));
		CSplash::Instance()->ShowSplash();
	}
	else
	{
		if (CSplash::IsInstantiated())
		{
			CSplash::Instance()->CloseSplash();
			delete CSplash::Instance();
		}
	}
}

void HideSplash()
{
	if (CSplash::IsInstantiated())
		CSplash::Instance()->HideSplash();
}

void ShowLoading(bool bShow)
{
	if (bShow)
	{
		if (!g_hDrawLoading && !g_iLoadingThreadID)
		{
			g_hDrawLoading =
				(HANDLE)_beginthreadex(NULL, 0, DrawLoadingByThread, NULL, 0,
					(unsigned int*)&g_iLoadingThreadID);
		}
	}
	else
	{
		g_bExitDrawLoading = true;
	}
}

void ShowPPL()
{
	new CPPL;
	if (COption::IsInstantiated())
	{
		if (CPPL::Instance()->IsPPLEnable())
		{
			WinExec("PangyaPPL.exe", SW_HIDE);
		}
		if (CPPL::IsInstantiated())
			delete CPPL::Instance();
	}
}

BOOL GetLastWriteTime(HANDLE hFile, char* lpszString)
{
	FILETIME ftCreate, ftAccess, ftWrite, ftLocal;
	SYSTEMTIME stCreate;

	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return FALSE;

	if (!FileTimeToLocalFileTime(&ftWrite, &ftLocal))
		return FALSE;

	FileTimeToSystemTime(&ftLocal, &stCreate);

	wsprintf(lpszString,
		"%d\263\342%02d\277\371%02d\300\317 %02d\275\303%02d\272\320",
		stCreate.wYear, stCreate.wMonth, stCreate.wDay, stCreate.wHour,
		stCreate.wMinute);

	return TRUE;
}

void SetWindowTitle(HWND hWnd)
{
	strcpy(window_name, K2L_Compatibility("\306\316\276\337 United(QA)"));
	SetWindowText(hWnd, window_name);
}

void SetDirectory()
{
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
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
		g_showTime = !(wParam == SIZE_MAXHIDE || wParam == SIZE_MINIMIZED);
		break;
	}

	if (CProjectG::IsInstantiated())
		return CProjectG::Instance()->WinProc(hWnd, msg, wParam, lParam);
	else
		return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool CheckSSE2_N_OSSupport()
{
	return false;
}

bool CheckPentium4()
{
	if (strstr(GetCommandLine(), "-sse2"))
		return true;
	if (strstr(GetCommandLine(), "-nosse2"))
		return false;

	return false;
}

bool CheckWndVersion()
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	if (osvi.dwPlatformId != VER_PLATFORM_WIN32s &&
		(osvi.dwMajorVersion > 4 ||
			(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion > 0)))
		return true;
	return false;
}

bool CheckRelatedDLL(HWND hWnd)
{
	HMODULE hDll = LoadLibrary("dinput8.dll");
	if (hDll == NULL)
	{
		MessageBox(hWnd,
			K2L_Compatibility(
				"DirectX \260\374\267\303 \306\304\300\317\300\273 \303\243\300\273 \274\366 \276\370\275\300\264\317\264\331.\nDirectX 9.0c \300\314\273\363\300\273 \274\263\304\241 \310\304 \264\331\275\303 \275\307\307\340\307\330 \301\326\274\274\277\344."),
			K2L_Compatibility("\306\316\276\337"), MB_OK);
		return false;
	}

	FreeLibrary(hDll);
	return true;
}
