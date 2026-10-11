// Original: d:\build\custom\temp\code\client\projectg\npgamelib.h
// nProtect GameGuard client interface (NPGameLib.lib, third-party, not
// decompiled). Only the API ProjectG uses is declared; the CNPGameLib
// wrappers are the header's own out-of-line bodies, emitted in the one
// object that includes it (GGC_Wrapper).
#ifndef PG_CLIENT_PROJECTG_NPGAMELIB_H
#define PG_CLIENT_PROJECTG_NPGAMELIB_H

#include <windows.h>

#include "..\..\defines.h"

// Messages and results of the GameGuard monitor.
#define NPGAMEMON_CHECK_CSAUTH2 1017   // 0x3F9
#define NPGAMEMON_SUCCESS       1877   // 0x755

// Server authentication query/answer (CS authentication 2).
typedef struct _GG_AUTH_DATA {
	DWORD dwIndex;    // 0x0
	DWORD dwValue1;   // 0x4
	DWORD dwValue2;   // 0x8
	DWORD dwValue3;   // 0xC
} GG_AUTH_DATA, *PGG_AUTH_DATA;
CHECK_SIZE(_GG_AUTH_DATA, 0x10);
CHECK_FIELD(_GG_AUTH_DATA, dwIndex, 0x0);
CHECK_FIELD(_GG_AUTH_DATA, dwValue1, 0x4);
CHECK_FIELD(_GG_AUTH_DATA, dwValue2, 0x8);
CHECK_FIELD(_GG_AUTH_DATA, dwValue3, 0xC);

extern "C" {
DWORD __cdecl PreInitNPGameMonA(LPCSTR szGameName);
DWORD __cdecl InitNPGameMon(void);
void __cdecl SetHwndToGameMon(HWND hWnd);
DWORD __cdecl CheckNPGameMon(void);
DWORD __cdecl SendUserIDToGameMonA(LPCSTR szUserID);
DWORD __cdecl SendCSAuth2ToGameMon(PGG_AUTH_DATA pAuthData);
BOOL __cdecl CloseNPGameMon(void);
}

// Callback the monitor calls with its messages (defined by the game).
BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg);

class CNPGameLib {
public:
	CNPGameLib(LPCSTR lpszGameName);
	~CNPGameLib(void);

	DWORD Init(void);
	void SetHwnd(HWND hWnd);
	DWORD Check(void);
	DWORD Send(LPCSTR lpszUserId);
	DWORD Auth2(PGG_AUTH_DATA pAuthData);
};
CHECK_SIZE(CNPGameLib, 0x1);

// npgamelib.h:176
CNPGameLib::CNPGameLib(LPCSTR lpszGameName) {
	PreInitNPGameMonA(lpszGameName);
}

// npgamelib.h:194
CNPGameLib::~CNPGameLib(void) {
	CloseNPGameMon();
}

// npgamelib.h:203
DWORD CNPGameLib::Init(void) {
	return InitNPGameMon();
}

// npgamelib.h:211
void CNPGameLib::SetHwnd(HWND hWnd) {
	SetHwndToGameMon(hWnd);
}

// npgamelib.h:219
DWORD CNPGameLib::Check(void) {
	return CheckNPGameMon();
}

// npgamelib.h:227
DWORD CNPGameLib::Send(LPCSTR lpszUserId) {
	return SendUserIDToGameMonA(lpszUserId);
}

// npgamelib.h:257
DWORD CNPGameLib::Auth2(PGG_AUTH_DATA pAuthData) {
	return SendCSAuth2ToGameMon(pAuthData);
}

#endif // PG_CLIENT_PROJECTG_NPGAMELIB_H
