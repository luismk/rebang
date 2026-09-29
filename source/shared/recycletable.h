#pragma once

struct sRecycleItem
{
	unsigned long typeId;
	unsigned long id;
	unsigned short quantity;
	unsigned char level;
	short character;
	short index;
	char reserved[20];
	unsigned long boxRandomId;
	char name[40];
};

extern int g_NumIndexRecycleItem;
extern int g_NumLowRecycleItem;
extern int g_NumMidRecycleItem;
extern int g_NumHighRecycleItem;
extern int g_NumSpecialRecycleItem;
extern int g_NumEventRecycleItem;
extern sRecycleItem** g_ArrayTidMixItem;
extern sRecycleItem* g_ArrayRecycleItem;

void InitIndexRecycleItem()
{
	for (short i = 0; i < g_NumIndexRecycleItem; i++)
	{
		g_ArrayRecycleItem[i].index = i;
	}
}

unsigned long GetTidMixItem(short index, int mix)
{
	if (index < 0)
		return 0;
	if (index >= g_NumIndexRecycleItem)
		return 0;
	if (mix < 0)
		return 0;
	if (mix >= 4)
		return 0;

	return g_ArrayTidMixItem[index][mix].typeId;
}

unsigned short GetNumMixItem(short index, int mix)
{
	if (index < 0)
		return 0;
	if (index >= g_NumIndexRecycleItem)
		return 0;
	if (mix < 0)
		return 0;
	if (mix >= 4)
		return 0;

	return g_ArrayTidMixItem[index][mix].quantity;
}

unsigned long GetTidRecycleItem(short index)
{
	if (index < 0)
		return 0;
	if (index >= g_NumIndexRecycleItem)
		return 0;

	return g_ArrayRecycleItem[index].typeId;
}

unsigned short GetNumRecycleItem(short index)
{
	if (index < 0)
		return 0;
	if (index >= g_NumIndexRecycleItem)
		return 0;

	return g_ArrayRecycleItem[index].quantity;
}

const sRecycleItem* GetRecycleItem(short index)
{
	if (index < 0)
		return 0;
	if (index >= g_NumIndexRecycleItem)
		return 0;

	return &g_ArrayRecycleItem[index];
}
