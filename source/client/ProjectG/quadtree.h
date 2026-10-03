#pragma once

#include <vector>
#include "polysoup.h"
#include "golfball.h"

// TODO: incomplete
class CQuadTree
{
public:
	void ResetHoleData();
	int GetMeshIndex(const WVector& pos, bool bObject);
	void PreProcess(float dt, bool bOneFrame);
	void AddWater(Waabb aabb, int type);
	unsigned char GetGroundType(WVector pos, bool bCheckTee,
		CGolfBall::eWater* water);
	WVector GetGroundPoint(WVector pos, bool bObject, int* index, bool bWater);
	void BallProcess(float dt);

	const std::vector<WPolySoup::sTriangle*>* GetTriArray(int texHandle) const
	{
		return m_pPolySoup->GetTriArray(texHandle);
	}

	WPolySoup* m_pPolySoup;
};

CQuadTree& GetPVS();
