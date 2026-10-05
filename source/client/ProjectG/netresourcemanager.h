#pragma once

#include "singleton.h"

class Bitmap;

// TODO: incomplete
class NetResourceManager : public WSingleton<NetResourceManager>
{
public:
	const Bitmap* GetEmblemByName(const char* name);
	void SetClearGarbage() { m_bClearGarbage = true; }

	unsigned char m_reserved28[0x4ad0];
	bool m_bClearGarbage;
};
