#pragma once

#include "../../shared/classdefine.h"

class WPuppet;

class CMascot
{
public:
	static int ChangeMascotMsgInTex(const char* msg,
		const IFF_STRUCT::sMascot* pMascot, WPuppet* pPuppet,
		const char* texName);
};
