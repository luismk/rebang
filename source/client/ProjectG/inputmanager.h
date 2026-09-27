#pragma once

class CInputManager
{
public:
	int GetDown(const char* action, bool exclusive);
	void ExclusiveGetDownUseDone(const char* action);
};

extern CInputManager* g_input;

class WInputDev;
extern WInputDev* g_ime;
