#pragma once

struct sUccItem
{
	unsigned long id;
	char uccIndex[9];
	unsigned long typeId;
	char uccName[41];
	unsigned long unused40;
	unsigned long unused44;
	unsigned long unused48;
	_SYSTEMTIME date;
	Bitmap* icon;
	unsigned char downloadCount;
};

struct sUccClothes : public sUccItem
{
};

class CUccManager
{
public:
	sUccClothes* FindClothes(unsigned long typeId, const char* uccIndex);
};

CUccManager* UccManager();
