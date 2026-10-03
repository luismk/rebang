#pragma once
#include "wdevice.h"
#ifndef WANGREAL_DEVICE
inline WDevice::~WDevice()
{
}
inline char* WDevice::GetDeviceName()
{
	return 0;
}
inline char* WDevice::EnumModeName()
{
	return "\0";
}
inline WProc* WDevice::ExternProc()
{
	return 0;
}

#endif
