#pragma once

#include "scenemanager.h"

class CFxSequence
{
public:
	unsigned char m_unused0[0x1104];
	bool m_bActive;
	WVector m_pos;
};

class CFx : public CRenderFuncPtr, public WSingleton<CFx>
{
public:
	CFx();
	virtual ~CFx();

	virtual void Process(float elapsed);
	virtual void Display();

	CFxSequence* OpenSequence(const char* filename, bool bDelete);
	void SetActive(bool bActive);
};
