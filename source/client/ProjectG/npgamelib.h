#pragma once

struct _GG_AUTH_DATA
{
	unsigned long dwIndex;
	unsigned long dwValue1;
	unsigned long dwValue2;
	unsigned long dwValue3;
};
typedef _GG_AUTH_DATA GG_AUTH_DATA;
typedef _GG_AUTH_DATA* PGG_AUTH_DATA;

#define NPGAMEMON_SUCCESS 0x755
#define NPGAMEMON_CHECK_CSAUTH2 1017

extern "C"
{
	DWORD __cdecl InitNPGameMon();
	void __cdecl SetHwndToGameMon(HWND hWnd);
	DWORD __cdecl GGGetLastError();
	DWORD __cdecl PreInitNPGameMonA(LPCSTR szGameName);
	DWORD __cdecl CheckNPGameMon();
	DWORD __cdecl SendUserIDToGameMonA(LPCSTR szUserID);
	DWORD __cdecl SendCSAuthToGameMon(DWORD dwAuth);
	DWORD __cdecl SendCSAuth2ToGameMon(PGG_AUTH_DATA pAuth);
	LPCSTR __cdecl GetInfoFromGameMon();
	LPBYTE __cdecl GetHackInfoFromGameMon(LPDWORD pdwSize);
	DWORD __cdecl CloseNPGameMon();
}

class CNPGameLib
{
public:
	CNPGameLib(LPCSTR lpszGameName) { PreInitNPGameMonA(lpszGameName); }

	~CNPGameLib() { CloseNPGameMon(); }

	DWORD Init() { return InitNPGameMon(); }

	void SetHwnd(HWND hWnd) { SetHwndToGameMon(hWnd); }

	DWORD Check() { return CheckNPGameMon(); }

	DWORD Send(LPCSTR lpszUserID) { return SendUserIDToGameMonA(lpszUserID); }

	DWORD Auth2(PGG_AUTH_DATA pCallbackData)
	{
		return SendCSAuth2ToGameMon(pCallbackData);
	}
};
