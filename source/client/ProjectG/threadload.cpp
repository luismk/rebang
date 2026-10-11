// Original: d:\build\custom\temp\code\client\projectg\threadload.cpp
#include <windows.h>
#include <mmsystem.h>
#include <process.h>
#include <string.h>

#include "ThreadLoad.h"
#include "wresrcmng.h"

template <> CThreadLoad* WSingleton<CThreadLoad>::m_pInstance = NULL;

sParam g_loadParam;

// threadload.cpp:19
CThreadLoad::CThreadLoad(void) : m_hThread(NULL) {
	g_loadParam.result.pPuppet = NULL;
	g_loadParam.bLoaded = true;
}

// threadload.cpp:25
CThreadLoad::~CThreadLoad(void) {
	if (m_hThread != NULL) {
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

// threadload.cpp:34
unsigned int __stdcall ThreadProc(void* param) {
	sParam* loadParam = (sParam*)param;

	switch (loadParam->type) {
	case THREADLOAD_PUPPET:
		loadParam->result.pPuppet = g_resrcmng->GetPuppet(loadParam->name, false, true, false);
		break;

	case THREADLOAD_TEXTURE:
		loadParam->result.texture = g_resrcmng->LoadTexture(loadParam->name, 0, 0, NULL);
		break;

	case THREADLOAD_OVERLAY:
		loadParam->result.pOverlay = g_resrcmng->GetOverlay(loadParam->name, 0);
		break;
	}

	g_loadParam.bLoaded = true;

	return 0;
}

// threadload.cpp:62
void CThreadLoad::LoadPuppet(const char* name) {
	Load(name, THREADLOAD_PUPPET);
}

// threadload.cpp:68
void CThreadLoad::LoadTexture(const char* name) {
	Load(name, THREADLOAD_TEXTURE);
}

// threadload.cpp:73
void CThreadLoad::LoadOverlay(const char* name) {
	Load(name, THREADLOAD_OVERLAY);
}

// threadload.cpp:78
void CThreadLoad::Load(const char* name, int type) {
	strcpy(g_loadParam.name, name);
	g_loadParam.result.pPuppet = NULL;
	g_loadParam.type = type;
	g_loadParam.bLoaded = false;
	g_loadParam.startTime = timeGetTime();

	unsigned int threadId;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, &g_loadParam, 0, &threadId);

	if (m_hThread == NULL) {
		// "_beginthreadex() Failed", caption "경고" (warning, EUC-KR)
		MessageBoxA(NULL, "_beginthreadex() Failed", "\xb0\xe6\xb0\xed", MB_OK);
		return;
	}

	SetThreadPriority(m_hThread, THREAD_PRIORITY_BELOW_NORMAL);
}

// threadload.cpp:104
void CThreadLoad::StopLoading(void) {
	if (m_hThread != NULL) {
		DWORD exitCode;
		GetExitCodeThread(m_hThread, &exitCode);
		ExitThread(exitCode);
	}
}
