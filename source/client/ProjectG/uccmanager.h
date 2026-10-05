#pragma once

struct sUccClothes;

class CUccManager
{
public:
	sUccClothes* FindClothes(unsigned long typeId, const char* uccIndex);
};

CUccManager* UccManager();
