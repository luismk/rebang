// Original: d:\build\custom\temp\code\client\wangreal\source\wdevmng.cpp
#include <stdlib.h>
#include <string.h>

#include "../include/wdevmng.h"
#include "../include/wvideo.h"
#include "../include/winput.h"

// Entry points a Wangreal device module exports (GetProcAddress).
typedef WVideoDev *(__cdecl *WEnumVideoDevicesProc)(int);
typedef WInputDev *(__cdecl *WEnumInputDevicesProc)(int);

// wdevmng.cpp:25
WDeviceManager::WDeviceManager(const char *directory, int loadtype)
	: vidList(8, 0), audList(8, 0), inpList(8, 0), dllList(8, 0), devList(8, 0) {
	LoadModule(directory, loadtype);
}

// wdevmng.cpp:30
WDeviceManager::~WDeviceManager(void) {
	WDevice *dev;
	while ((dev = devList.Start()) != NULL) {
		Release(dev);
	}
	for (HINSTANCE module = dllList.Start(); module != NULL; module = dllList.Next()) {
		FreeLibrary(module);
	}
}

// wdevmng.cpp:42
void WDeviceManager::LoadModule(const char *directory, int loadtype) {
	WIN32_FIND_DATAA fd;
	char filename[128];
	char path[260];
	HANDLE find;
	char drive[4];
	char szModulePath[260];
	char ext[8];
	HINSTANCE module;
	char name[128];
	WEnumVideoDevicesProc WEnumVideoDevices;
	WEnumInputDevicesProc WEnumInputDevices;
	char filter[128];
	char configuration[64];
	char *suffix;
	WVideoDev *video;
	WInputDev *input;
	int i;

	GetModuleFileNameA(NULL, szModulePath, 260);

	_splitpath(szModulePath, drive, path, name, ext);

	strcpy(filter, "wangreal");
	suffix = strrchr(name, '_');
	if (suffix != NULL) {


		strcpy(configuration, suffix);
		strcat(filter, configuration);
	}

	strcat(filter, ".dll");

	find = FindFirstFileA(filter, &fd);
	if (find != INVALID_HANDLE_VALUE) {
		do {
			if (strstr(fd.cFileName, "wang") != NULL) {
				filename[0] = '\0'; strcat(filename, fd.cFileName);
				module = LoadLibraryA(filename);
				if (module != NULL) {
					WEnumVideoDevices = (loadtype & 1) ? (WEnumVideoDevicesProc)GetProcAddress(module, "WEnumVideoDevices") : NULL;
					WEnumInputDevices = (loadtype & 8) ? (WEnumInputDevicesProc)GetProcAddress(module, "WEnumInputDevices") : NULL;
					GetProcAddress(module, "WVersion");
					if (WEnumVideoDevices != NULL || WEnumInputDevices != NULL) {
						for (i = 0; WEnumVideoDevices != NULL && (video = WEnumVideoDevices(i)) != NULL; i++) {
							vidList += video;
						}
						for (i = 0; WEnumInputDevices != NULL && (input = WEnumInputDevices(i)) != NULL; i++) {
							inpList += input;
						}
						dllList += module;
					} else {
						FreeLibrary(module);
					}
				}
			}
		} while (FindNextFileA(find, &fd));
	}


	FindClose(find);
}

// wdevmng.cpp:170
WVideoDev *WDeviceManager::CreateVideoDevice(char *deviceName, char *modeName, int iTnL) {
	WVideoDev *ret = NULL;

	for (WVideoDev *dev = vidList.Start(); dev != NULL && ret == NULL; dev = vidList.Next()) {
		if (deviceName == NULL || _strcmpi(dev->GetDeviceName(), deviceName) == 0) {
			ret = dev->MakeClone(modeName, hwnd, iTnL);
		}
	}
	if (ret != NULL) {
		devList += ret;
	}
	return ret;
}

// wdevmng.cpp:191
bool WDeviceManager::ResetVideoDevice(bool bWindowed, int iWidth, int iHeight, int iColor, long lWndStyle, int fillMode) {
	WDevice *dev;
	for (WVideoDev *video = vidList.Start(); video != NULL; video = vidList.Next()) {
		for (dev = devList.Start(); dev != NULL; dev = devList.Next()) {
			if (strstr(dev->GetDeviceName(), video->GetDeviceName()) != NULL || strstr(video->GetDeviceName(), dev->GetDeviceName()) != NULL) {
				goto found;
			}
		}
	}
	return false;
found:

	return ((WVideoDev *)dev)->Reset(bWindowed, iWidth, iHeight, iColor, lWndStyle, fillMode);
}

// wdevmng.cpp:282
WInputDev *WDeviceManager::CreateInputDevice(char *devName, char *modeName) {
	WInputDev *ret = NULL;

	for (WInputDev *dev = inpList.Start(); dev != NULL && ret == NULL; dev = inpList.Next()) {
		if (devName == NULL || _strcmpi(dev->GetDeviceName(), devName) == 0) {
			ret = dev->MakeClone(modeName, hwnd);
		}
	}

	if (ret != NULL) {
		devList += ret;
	}

	return ret;
}

// wdevmng.cpp:296
void WDeviceManager::Release(WDevice *device) {
	devList -= device;
	delete device;
}

// wdevmng.cpp:302
char *WDeviceManager::EnumVideoDevice(void) {
	// The device names, each NUL terminated, the list closed by an empty name.
	static char devices[1024];
	WVideoDev *dev;
	int len;


	for (dev = vidList.Start(), len = 0; dev != NULL; dev = vidList.Next()) {

		strcpy(&devices[len], dev->GetDeviceName());
		len += strlen(dev->GetDeviceName()) + 1;
	}


	devices[len] = '\0';

	return devices;
}

// wdevmng.cpp:338
char *WDeviceManager::EnumVideoMode(char *deviceName) {


	for (WVideoDev *dev = vidList.Start(); dev != NULL; dev = vidList.Next()) {
		if (_strcmpi(dev->GetDeviceName(), deviceName) == 0) {
			return dev->EnumModeName();
		}
	}
	return NULL;
}
