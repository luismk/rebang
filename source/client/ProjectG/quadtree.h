#pragma once

#include <vector>
#include "polysoup.h"

// TODO: incomplete
class CQuadTree
{
public:
	void ResetHoleData();
	int GetMeshIndex(const WVector& pos, bool bObject);
	void PreProcess(float dt, bool bOneFrame);

	const std::vector<WPolySoup::sTriangle*>* GetTriArray(int texHandle) const
	{
		return m_pPolySoup->GetTriArray(texHandle);
	}

	WPolySoup* m_pPolySoup;
};

CQuadTree& GetPVS();
