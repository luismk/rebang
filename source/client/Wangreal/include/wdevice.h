#pragma once
class WProc;

class WDevice
{
public:
#ifdef WANGREAL_DEVICE
	virtual ~WDevice() { }
	virtual char* GetDeviceName() { return 0; }
	virtual char* EnumModeName() { return "\0"; }
	virtual WProc* ExternProc() { return 0; }
#else
	virtual ~WDevice();
	virtual char* GetDeviceName();
	virtual char* EnumModeName();
	virtual WProc* ExternProc();
#endif
};
