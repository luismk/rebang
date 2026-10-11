// Original: d:\build\custom\temp\code\client\projectg\hackingmanager.cpp
#include "hackingmanager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commonutil.h"

extern bool g_bQuit;
extern HWND g_hwnd;

// 32 bytes read from the middle of a running program, compared with the
// known hack tools' records.
static unsigned char data_b7964c[0x20];


// hackingmanager.cpp:20
HackingManager::HackingManager(void) {
	m_hackToolCount = 0;
	m_hackTools = 0;
	m_hHookDll = 0;
	m_hFileMapping = 0;
	m_pfnInitHooksDll = 0;
	m_pfnInstallHook = 0;
	m_pfnUnInstallHook = 0;
	ClearCrypticList();
}

// hackingmanager.cpp:33
HackingManager::~HackingManager(void) {
	Close();
	if (m_hackTools) { delete m_hackTools; m_hackTools = 0; }
	if (m_hHookDll)
		FreeLibrary(m_hHookDll);
	if (m_hFileMapping)
		CloseHandle(m_hFileMapping);
}

// hackingmanager.cpp:44
bool HackingManager::Init(void) {
	WFile file("BBH.bin");
	file.Length();
	file.Read(&m_hackToolCount, 4);

	if (m_hackToolCount > 0)
		m_hackTools = new sHackToolInfo[m_hackToolCount];

	for (int i = 0; i < m_hackToolCount; i++) {
		file.Read(&m_hackTools[i], sizeof(sHackToolInfo));
	}

	file.Close();

	m_hFileMapping = CreateFileMappingA(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, 0x2800, "ExMessageData");
	m_pMappedView = MapViewOfFile(m_hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	m_hHookDll = LoadLibraryA("JERR.dll");

	if (m_hHookDll == 0)
		return false;

	m_pfnInitHooksDll = (HookInitFunc)GetProcAddress(m_hHookDll, "InitHooksDll");
	m_pfnInstallHook = (HookSwitchFunc)GetProcAddress(m_hHookDll, "InstallHook");
	m_pfnUnInstallHook = (HookSwitchFunc)GetProcAddress(m_hHookDll, "UnInstallHook");

	if (m_pfnInitHooksDll == 0 || m_pfnInstallHook == 0 || m_pfnUnInstallHook == 0)
		return false;

	m_pfnInitHooksDll(g_hwnd);
	m_pfnInstallHook(1);

	return true;
}

// hackingmanager.cpp:94
void HackingManager::Close(void) {
	if (m_pfnInstallHook)
		m_pfnUnInstallHook(1);
}

// hackingmanager.cpp:101
void HackingManager::Process(void) {
}

// hackingmanager.cpp:105
bool HackingManager::AllProcessScan(void) {
	return false;
}

// hackingmanager.cpp:112
void HackingManager::ClearCrypticList(void) {
	m_byteList.clear();
	m_intList.clear();
	m_floatList.clear();

	m_checkInterval = 0;
	m_checkCount = 0;
}

// hackingmanager.cpp:122
void HackingManager::ResetCrypticValue(int range) {
	m_checkInterval = rand() % range + 1;
	m_checkCount = 0;
}

// hackingmanager.cpp:128
void HackingManager::CheckCrypticValue(void) {
	std::list<WCrypticValue<unsigned char>*>::iterator itByte;
	for (itByte = m_byteList.begin(); itByte != m_byteList.end(); ++itByte) {
		if (!(*itByte)->IsSafe()) {
			g_bQuit = true;
			return;
		}
	}

	std::list<WCrypticValue<int>*>::iterator itInt;
	for (itInt = m_intList.begin(); itInt != m_intList.end(); ++itInt) {
		if (!(*itInt)->IsSafe()) {
			g_bQuit = true;
			return;
		}
	}

	std::list<WCrypticValue<float>*>::iterator itFloat;
	for (itFloat = m_floatList.begin(); itFloat != m_floatList.end(); ++itFloat) {
		if (!(*itFloat)->IsSafe()) {
			g_bQuit = true;
			return;
		}
	}
}

// hackingmanager.cpp:196
bool HackingManager::CheckExcuteProgam(void) {
	char path[256];
	memset(path, 0, sizeof(path));
	strcpy(path, (char*)m_pMappedView);

	int len = strlen(path);
	if (len == 0)
		return false;
	if (stricmp(path, m_name.c_str()) != 0) {
		m_name = path;
		return CompareHackToolByName(path);
	}
	return false;
}

// hackingmanager.cpp:214
bool HackingManager::CompareHackToolByName(const char *name) {
	FILE *fp = fopen(name, "rb");
	if (fp == 0) {
		return false;
	}

	if (fseek(fp, 0, SEEK_END) != 0) {
		return false;
	}

	unsigned long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (size >= 0x20) {
		fseek(fp, size / 2, SEEK_SET);
		fread(data_b7964c, 1, 0x20, fp);
		fclose(fp);

		for (int i = 0; i < m_hackToolCount; i++) {
			if (size == m_hackTools[i].fileSize) {
				if (memcmp(m_hackTools[i].midBytes, data_b7964c, 0x20) == 0)
					return true;
			}
		}
	}

	return false;
}
