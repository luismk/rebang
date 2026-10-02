#pragma once

#include <vector>

class WBone;

// TODO: incomplete
class WPolySoup
{
public:
	struct sTriangle
	{
		WVector v[3];
		WPlane plane;
		bool bDoubleSide;
		int texHandle;
		int propIndex;
		bool bEdge;
		WBone* bone;

		sTriangle()
			: texHandle(0), propIndex(0), bone(NULL)
		{
		}
	};

	const std::vector<sTriangle*>* GetTriArray(int texHandle) const;
};
