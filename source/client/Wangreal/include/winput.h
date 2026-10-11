// Original: d:\build\custom\temp\code\client\wangreal\include\winput.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WINPUT_H
#define PG_CLIENT_WANGREAL_INCLUDE_WINPUT_H

#include "..\..\defines.h"

struct HWND__;
class WProc;

// Base of the engine's devices (PDB: size 0x4, a vfptr only).
class WDevice {
public:
	virtual ~WDevice();                     // vtable 0x0
	virtual char *GetDeviceName(void);      // vtable 0x4
	virtual char *EnumModeName(void);       // vtable 0x8
	virtual WProc *ExternProc(void);        // vtable 0xC
};
CHECK_SIZE(WDevice, 0x4);

// Input device (keyboard, mouse, IME). PDB layout, size 0xC.
class WInputDev : public WDevice {
public:
	virtual bool InitDevice(HWND__ *hWnd, bool exclusive);  // vtable 0x10
	virtual void SetActive(bool active);                    // vtable 0x14
	virtual void Reset(void);                               // vtable 0x18
	virtual void Update(unsigned long time);                // vtable 0x1C
	virtual int GetState(int state, int arg);               // vtable 0x20
	virtual unsigned long GetEventTime(int state);          // vtable 0x24
	virtual bool IsAlphaNumericMode(void);                  // vtable 0x28
	// cl 13.10 lays overloads out in reverse declaration order:
	// SetAlphaNumericMode(void) gets 0x2C, SetAlphaNumericMode(bool) 0x30.
	virtual void SetAlphaNumericMode(bool on);              // vtable 0x30
	virtual void SetAlphaNumericMode(void);                 // vtable 0x2C
	virtual bool IsUpdated(void);                           // vtable 0x34
	virtual unsigned long GetLastInputTime(void);           // vtable 0x38
	virtual void ResetInputTime(void);                      // vtable 0x3C
	virtual void SetState(int state, int value);            // vtable 0x40
	virtual void SetOpenStatus(bool open);                  // vtable 0x44
	virtual WInputDev *MakeClone(char *name, HWND__ *hWnd); // vtable 0x48

protected:
	bool active;                    // 0x4
	bool bUpdated;                  // 0x5
	unsigned long dwLastInputTime;  // 0x8

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(WInputDev)
	CHECK_SIZE(WInputDev, 0xC);
	CHECK_FIELD(WInputDev, active, 0x4);
	CHECK_FIELD(WInputDev, bUpdated, 0x5);
	CHECK_FIELD(WInputDev, dwLastInputTime, 0x8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WINPUT_H
