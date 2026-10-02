#pragma once

struct sRecycleItem
{
	unsigned long dwTid;
	unsigned long dwItemid;
	unsigned short iNum;
	unsigned char byLevel;
	short iChar;
	short iIndex;
	unsigned char byItemType;
	unsigned short wTime;
	_SYSTEMTIME date;
	unsigned long dwRandSeq;
	char szRandName[40];
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
		g_ArrayRecycleItem[i].iIndex = i;
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

	return g_ArrayTidMixItem[index][mix].dwTid;
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

	return g_ArrayTidMixItem[index][mix].iNum;
}

unsigned long GetTidRecycleItem(short index)
{
	if (index < 0)
		return 0;
	if (index >= g_NumIndexRecycleItem)
		return 0;

	return g_ArrayRecycleItem[index].dwTid;
}

unsigned short GetNumRecycleItem(short index)
{
	if (index < 0)
		return 0;
	if (index >= g_NumIndexRecycleItem)
		return 0;

	return g_ArrayRecycleItem[index].iNum;
}

const sRecycleItem* GetRecycleItem(short index)
{
	if (index < 0)
		return 0;
	if (index >= g_NumIndexRecycleItem)
		return 0;

	return &g_ArrayRecycleItem[index];
}
