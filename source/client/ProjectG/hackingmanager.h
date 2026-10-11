// Original: d:\build\custom\temp\code\client\projectg\hackingmanager.h
#ifndef PG_CLIENT_PROJECTG_HACKINGMANAGER_H
#define PG_CLIENT_PROJECTG_HACKINGMANAGER_H

#include <windows.h>
#include <list>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "encrypttypes.h"

// One record of BBH.bin (the known hack tools), 0x124 bytes: the tool's
// file size and 32 bytes read from the middle of the file.
struct sHackToolInfo {
	unsigned long fileSize;                             // 0x0
	UNKNOWN(0x4, 0x104);
	unsigned char midBytes[0x20];                       // 0x104
};
CHECK_SIZE(sHackToolInfo, 0x124);
BEGIN_LAYOUT_CHECK(sHackToolInfo)
	CHECK_FIELD(sHackToolInfo, fileSize, 0x0);
	CHECK_FIELD(sHackToolInfo, midBytes, 0x104);
END_LAYOUT_CHECK

typedef void (__stdcall *HookInitFunc)(HWND hwnd);
typedef void (__stdcall *HookSwitchFunc)(int flag);

// Anti-hacking manager (WSingleton<HackingManager>, 0x90 bytes): the hack
// tool list, the JERR.dll keyboard hooks and the encrypted values to watch.
class HackingManager : public WSingleton<HackingManager> {
	LAYOUT_CHECK_FRIEND;
public:
	HackingManager(void);
	virtual ~HackingManager(void);

	bool Init(void);
	void Process(void);
	bool AllProcessScan(void);
	bool CheckExcuteProgam(void);
	void ResetCrypticValue(int range);
	void ClearCrypticList(void);
	void CheckCrypticValue(void);

	// hackingmanager.h:41-43 (GolfDoc holds the out-of-line copies)
	void AddCrypticByteValue(WCrypticValue<unsigned char> *value) { m_byteList.push_back(value); }
	void AddCrypticIntValue(WCrypticValue<int> *value) { m_intList.push_back(value); }
	void AddCrypticFloatValue(WCrypticValue<float> *value) { m_floatList.push_back(value); }

private:
	void Close(void);
	bool CompareHackToolByName(const char *name);

	std::string m_name;                                 // 0x28
	int m_hackToolCount;                                // 0x44
	sHackToolInfo *m_hackTools;                         // 0x48
	HMODULE m_hHookDll;                                 // 0x4c
	HANDLE m_hFileMapping;                              // 0x50
	void *m_pMappedView;                                // 0x54
	HookInitFunc m_pfnInitHooksDll;                     // 0x58
	HookSwitchFunc m_pfnInstallHook;                    // 0x5c
	HookSwitchFunc m_pfnUnInstallHook;                  // 0x60
	int m_checkCount;                                   // 0x64
	int m_checkInterval;                                // 0x68
	std::list<WCrypticValue<unsigned char>*> m_byteList;   // 0x6c
	std::list<WCrypticValue<int>*> m_intList;           // 0x78
	std::list<WCrypticValue<float>*> m_floatList;       // 0x84
};
CHECK_SIZE(HackingManager, 0x90);
BEGIN_LAYOUT_CHECK(HackingManager)
	CHECK_FIELD(HackingManager, m_name, 0x28);
	CHECK_FIELD(HackingManager, m_hackToolCount, 0x44);
	CHECK_FIELD(HackingManager, m_pfnUnInstallHook, 0x60);
	CHECK_FIELD(HackingManager, m_checkInterval, 0x68);
	CHECK_FIELD(HackingManager, m_byteList, 0x6c);
	CHECK_FIELD(HackingManager, m_floatList, 0x84);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_HACKINGMANAGER_H
