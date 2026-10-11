// Original: d:\build\custom\temp\code\client\projectg\recycletable.h
#ifndef PG_CLIENT_PROJECTG_RECYCLETABLE_H
#define PG_CLIENT_PROJECTG_RECYCLETABLE_H

// The recycle recipe table and its accessors. The accessors are defined
// (not inline) in this header: the original emits them, public, in the one
// object including it (RecycleDlg). sRecycleItem is still declared in
// RecycleDlg.h.
#include "RecycleDlg.h"

extern int g_NumIndexRecycleItem;
extern sRecycleItem** g_ArrayTidMixItem;   // per recipe, its 4 ingredients

// recycletable.h:109
void InitIndexRecycleItem(void) {
	for (short i = 0; i < g_NumIndexRecycleItem; i++) {
		g_ArrayRecycleItem[i].index = i;
	}
}

// recycletable.h:120
unsigned long GetTidMixItem(short index, int slot) {
	if (index >= 0
		&& index < g_NumIndexRecycleItem
		&& slot >= 0
		&& slot < 4) {
		return g_ArrayTidMixItem[index][slot].tid;
	}
	return 0;
}

// recycletable.h:131
unsigned short GetNumMixItem(short index, int slot) {
	if (index >= 0
		&& index < g_NumIndexRecycleItem
		&& slot >= 0
		&& slot < 4) {
		return g_ArrayTidMixItem[index][slot].count;
	}
	return 0;
}

// recycletable.h:142
unsigned long GetTidRecycleItem(short index) {
	if (index >= 0
		&& index < g_NumIndexRecycleItem) {
		return g_ArrayRecycleItem[index].tid;
	}
	return 0;
}

// recycletable.h:151
unsigned short GetNumRecycleItem(short index) {
	if (index >= 0
		&& index < g_NumIndexRecycleItem) {
		return g_ArrayRecycleItem[index].count;
	}
	return 0;
}

// recycletable.h:169
const sRecycleItem* GetRecycleItem(short index) {
	if (index >= 0
		&& index < g_NumIndexRecycleItem) {
		return &g_ArrayRecycleItem[index];
	}
	return 0;
}

#endif // PG_CLIENT_PROJECTG_RECYCLETABLE_H
