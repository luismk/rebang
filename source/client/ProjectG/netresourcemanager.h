#pragma once

#include "singleton.h"

class Bitmap;

// TODO: incomplete
class NetResourceManager : public WSingleton<NetResourceManager>
{
public:
	const Bitmap* GetEmblemByName(const char* name);
};
