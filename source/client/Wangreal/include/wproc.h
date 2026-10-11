// Original: d:\build\custom\temp\code\client\wangreal\include\wproc.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WPROC_H
#define PG_CLIENT_WANGREAL_INCLUDE_WPROC_H

#include <windows.h>

#include "..\..\defines.h"

class WProc;

// Dispatches the window messages to up to 16 registered procedures (PDB
// layout, size 0x84).
class WProcManager {
	LAYOUT_CHECK_FRIEND;
public:
	struct w_handle {
		HWND__* hwnd;  // 0x0
		WProc* proc;   // 0x4
	};

	// wproc.h:23
	WProcManager() { procNum = 0; }

	void AddProc(WProc* proc, HWND__* hwnd);
	void DelProc(WProc* proc);
	long __stdcall WinProc(HWND__* hwnd, unsigned int msg, unsigned int wParam, long lParam);

private:
	w_handle procList[16];  // 0x0
	int procNum;            // 0x80
};

// A window-message procedure registered with a WProcManager (PDB layout,
// size 0x8).
class WProc {
	LAYOUT_CHECK_FRIEND;
public:
	virtual ~WProc() {}
	virtual int WinProc(unsigned int msg, unsigned long wParam, unsigned long lParam) = 0;

	// wproc.h:44
	void SetProc(WProcManager* manager, HWND__* hwnd) {
		include = manager;
		manager->AddProc(this, hwnd);
	}

private:
	WProcManager* include;  // 0x4
};

// wproc.h:64
inline void WProcManager::AddProc(WProc* proc, HWND__* hwnd) {
	procList[procNum].hwnd = hwnd;
	procList[procNum].proc = proc;

	procNum++;
}

// wproc.h:74
inline long __stdcall WProcManager::WinProc(HWND__* hwnd, unsigned int msg, unsigned int wParam, long lParam) {
	int result = 0;

	for (int i = 0; i < procNum; i++) {
		result |= procList[i].proc->WinProc(msg, wParam, lParam);
	}

	return (result == 0 || result == -1) ? DefWindowProcA(hwnd, msg, wParam, lParam) : 0;
}

CHECK_SIZE(WProcManager, 0x84);
BEGIN_LAYOUT_CHECK(WProcManager)
	CHECK_FIELD(WProcManager, procList, 0x0);
	CHECK_FIELD(WProcManager, procNum, 0x80);
END_LAYOUT_CHECK

CHECK_SIZE(WProc, 0x8);
BEGIN_LAYOUT_CHECK(WProc)
	CHECK_FIELD(WProc, include, 0x4);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WPROC_H
