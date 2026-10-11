// Original: d:\build\custom\temp\code\client\wangreal\include\wdevmng.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WDEVMNG_H
#define PG_CLIENT_WANGREAL_INCLUDE_WDEVMNG_H

#include <windows.h>

#include "..\..\defines.h"
#include "wlist.h"

class WDevice;
class WVideoDev;
class WAudioDev;
class WInputDev;

// Owner of the Wangreal devices (video, audio, input) and of the modules
// they were loaded from. Layout from the PDB.
class WDeviceManager {
	LAYOUT_CHECK_FRIEND;

public:
	WDeviceManager(const char *, int);
	~WDeviceManager(void);

	// wdevmng.h:38
	void SetHWND(HWND hWnd) { hwnd = hWnd; }

	WVideoDev *CreateVideoDevice(char *, char *, int);
	WAudioDev *CreateAudioDevice(char *, char *);
	WInputDev *CreateInputDevice(char *, char *);
	bool ResetVideoDevice(bool, int, int, int, long, int);
	char *EnumVideo(char *);
	char *EnumAudio(void);
	void Release(WDevice *);

private:
	void LoadModule(const char *, int);
	char *EnumVideoDevice(void);
	char *EnumVideoMode(char *);
	char *EnumAudioDevice(void);

	WList<WVideoDev *> vidList;    // 0x0
	WList<WAudioDev *> audList;    // 0x20
	WList<WInputDev *> inpList;    // 0x40
	WList<HINSTANCE> dllList;      // 0x60
	WList<WDevice *> devList;      // 0x80
	HWND hwnd;                     // 0xA0
};
CHECK_SIZE(WDeviceManager, 0xA4);
BEGIN_LAYOUT_CHECK(WDeviceManager)
	CHECK_FIELD(WDeviceManager, vidList, 0x0);
	CHECK_FIELD(WDeviceManager, audList, 0x20);
	CHECK_FIELD(WDeviceManager, inpList, 0x40);
	CHECK_FIELD(WDeviceManager, dllList, 0x60);
	CHECK_FIELD(WDeviceManager, devList, 0x80);
	CHECK_FIELD(WDeviceManager, hwnd, 0xA0);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WDEVMNG_H
