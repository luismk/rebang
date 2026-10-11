// Original: d:\build\custom\temp\code\client\projectg\ggc_wrapper.cpp
#include "NPGameLib.h"
#include "GGC_Helper.h"
#include "Packet.h"
#include "ProjectG.h"
#include "TaskManager.h"
#include "actor.h"

// The game's GameGuard instance (NULL until _GGC_Init).
CNPGameLib* g_pGameGuard;

// ggc_wrapper.cpp:30
int _GGC_Init(void) {
#ifdef PG_NO_GAMEGUARD
	// nProtect GameGuard is not loaded in this build (CMake PG_NO_GAMEGUARD):
	// report success and leave g_pGameGuard NULL, so every _GGC_* wrapper
	// below stays inert.
	return 1;
#else
	if (!g_pGameGuard) {
		g_pGameGuard = new CNPGameLib("PangyaTest");
	}
	DWORD dwResult = g_pGameGuard->Init();
	if (dwResult == NPGAMEMON_SUCCESS) {
		return 1;
	}
	return GGC_GetHelper()->StackError(dwResult, true);
#endif
}

// ggc_wrapper.cpp:72
void _GGC_End(void) {
	if (g_pGameGuard) {
		delete g_pGameGuard;
		g_pGameGuard = NULL;
	}
}

// ggc_wrapper.cpp:84
void _GGC_SetHwnd(HWND hWnd) {
	if (g_pGameGuard) {
		g_pGameGuard->SetHwnd(hWnd);
	}
}

// ggc_wrapper.cpp:93
void _GGC_SendUserID(const char* userId) {
	if (g_pGameGuard) {
		g_pGameGuard->Send(userId);
	}
}

// ggc_wrapper.cpp:101
int _GGC_CheckGameMon(void) {
	if (g_pGameGuard) {
		return g_pGameGuard->Check() == NPGAMEMON_SUCCESS;
	}
	return 1;
}

// ggc_wrapper.cpp:119
void _GGC_CSAuth2(WReceivedPacket& packet) {
	if (g_pGameGuard) {
		GG_AUTH_DATA authData;
		packet.DecodeBuffer(&authData, sizeof(authData));

		GG_AUTH_DATA errorData = { 1000, 0, 0, 0 };
		if (memcmp(&authData, &errorData, sizeof(GG_AUTH_DATA)) == 0) {
			CProjectG::Instance()->GameGuardLog("CSAuth2 requested with error.");
		} else {
			CProjectG::Instance()->GameGuardLog("CSAuth2 requested.");
		}
		g_pGameGuard->Auth2(&authData);
	}
}

// ggc_wrapper.cpp:144
BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg) {
	if (dwMsg == NPGAMEMON_CHECK_CSAUTH2) {
		GG_AUTH_DATA authData;
		memcpy(&authData, (void*)dwArg, sizeof(GG_AUTH_DATA));

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x26B, (intptr_t)&authData, 0, 0, 0, 0));
		return 1;
	}
	return GGC_GetHelper()->StackError(dwMsg, false);
}
