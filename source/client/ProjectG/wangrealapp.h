// Original: d:\build\custom\temp\code\client\projectg\wangrealapp.h
#ifndef PG_CLIENT_PROJECTG_WANGREALAPP_H
#define PG_CLIENT_PROJECTG_WANGREALAPP_H

#include "..\..\defines.h"

// Interface of a Wangreal application (only a vtable; CProjectG's second
// base, at 0x2C). Its constructor, destructor and Ready are emitted by
// projectg.cpp.
class CWangrealApplication {
public:
	// wangrealapp.h:9
	virtual ~CWangrealApplication(void) {}

	// Slot 1: winmain.cpp's main loop calls MainLoop through +4, Ready through +8.
	virtual int MainLoop(int) = 0;

	// wangrealapp.h:12
	virtual bool Ready(void) { return true; }

	// Slot 3: winmain.cpp's WinProc forwards every message here.
	virtual long __stdcall WinProc(struct HWND__* hWnd, unsigned int message, unsigned int wParam, long lParam) = 0;
};
CHECK_SIZE(CWangrealApplication, 0x4);

#endif // PG_CLIENT_PROJECTG_WANGREALAPP_H
