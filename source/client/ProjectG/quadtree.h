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
	WVector GetRayIntersection(WVector start, WVector ray);
	void BallProcess(float dt);
	void Reserve(short num);
	bool IsOBArea(const WVector& pos);
	void MakeBaseStructure(WPolySoup::sModel* model, bool bRebuild);
	void MakeObjStructure(short index, int modelIndex, bool bRebuild);

	void SetPolySoup(WPolySoup* pPolySoup) { m_pPolySoup = pPolySoup; }

	const std::vector<WPolySoup::sTriangle*>* GetTriArray(int texHandle) const
	{
		return m_pPolySoup->GetTriArray(texHandle);
	}

	WPolySoup* m_pPolySoup;
};

CQuadTree& GetPVS();
