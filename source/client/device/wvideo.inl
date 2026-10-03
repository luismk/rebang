#pragma once
#include <wdevice.h>

inline void WVideoDev::DrawIndexedTriangles(WTVertex* p, int pNum,
	unsigned short* fList, int fNum, int iType, int iType2)
{
	static WTVertex* t[4];
	for (; fNum > 0; fNum -= 3)
	{
		t[0] = &p[*fList++];
		t[1] = &p[*fList++];
		t[2] = &p[*fList++];
		DrawPolygonFan(t, iType, 3, iType2, 0);
	}
}
