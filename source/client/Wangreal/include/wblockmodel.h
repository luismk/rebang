// Original: d:\build\custom\temp\code\client\wangreal\include\wblockmodel.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WBLOCKMODEL_H
#define PG_CLIENT_WANGREAL_INCLUDE_WBLOCKMODEL_H

#include "..\..\defines.h"
#include "wlist.h"
#include "wmath.h"

class WView;

class WBlockModel {
	LAYOUT_CHECK_FRIEND;
public:
	struct WConvexArea {
		WConvexArea();

		Waabb aabb;         // 0x0
		int count;          // 0x18
		int check;          // 0x1C
		WPlane plane[1];    // 0x20
	};
	struct WOctree {
		struct FruitList {
			unsigned char flag;     // 0x0
			WConvexArea* area;      // 0x4
		};
		WOctree();

		WVector pivot;          // 0x0
		unsigned char mask;     // 0xC
		WOctree* leaf[8];       // 0x10
		int check;              // 0x30
		int listnum;            // 0x34
		FruitList list[1];      // 0x38
	};

	WBlockModel();
	~WBlockModel();
	void Reset();
	void Build();
	float CollTest(const WVector&, const WVector&, float, WPlane*);
	void AddTriangleList(const WVector*, int);
	void DebugRender(WView*, const Waabb&, WOctree*, int);
	void DebugRender(WView*);

private:
	static float CollTest(const WVector&, const WVector&, float, WPlane*, WConvexArea*, float);
	void Add(WConvexArea*);
	bool IsConvex(const WVector*, int, WPlane*);
	Waabb ExtendAABB(const WVector&, const WVector&, float);
	Waabb ExtendAABB(const Waabb&, const Waabb&);
	static bool ColTestRough(const Waabb&, const Waabb&);
	static bool ColTestRough(const WVector&, const WVector&, float, const Waabb&);
	int PositionDecision(int);
	WOctree* ExtendTree(WList<WConvexArea*>*);
	static int CalcDepth(WOctree*);
	static unsigned char CalcFlag(const Waabb&, const WVector&);
	static WVector FixedPivot(WList<WConvexArea*>*, WVector);
	static float Explore(WOctree*, const Waabb&, const WVector&, const WVector&, float, WPlane*, float);

	static unsigned long g_treemask[8];

	WList<WConvexArea*> m_colarea;  // 0x0
	Waabb m_colbound;               // 0x20
	WOctree* m_root;                // 0x38
};
CHECK_SIZE(WBlockModel, 0x3C);
BEGIN_LAYOUT_CHECK(WBlockModel)
	CHECK_FIELD(WBlockModel, m_colarea, 0x0);
	CHECK_FIELD(WBlockModel, m_colbound, 0x20);
	CHECK_FIELD(WBlockModel, m_root, 0x38);
END_LAYOUT_CHECK
CHECK_SIZE(WBlockModel::WConvexArea, 0x30);
CHECK_FIELD(WBlockModel::WConvexArea, aabb, 0x0);
CHECK_FIELD(WBlockModel::WConvexArea, count, 0x18);
CHECK_FIELD(WBlockModel::WConvexArea, check, 0x1C);
CHECK_FIELD(WBlockModel::WConvexArea, plane, 0x20);
CHECK_SIZE(WBlockModel::WOctree, 0x40);
CHECK_FIELD(WBlockModel::WOctree, pivot, 0x0);
CHECK_FIELD(WBlockModel::WOctree, mask, 0xC);
CHECK_FIELD(WBlockModel::WOctree, leaf, 0x10);
CHECK_FIELD(WBlockModel::WOctree, check, 0x30);
CHECK_FIELD(WBlockModel::WOctree, listnum, 0x34);
CHECK_FIELD(WBlockModel::WOctree, list, 0x38);
CHECK_SIZE(WBlockModel::WOctree::FruitList, 0x8);

#endif // PG_CLIENT_WANGREAL_INCLUDE_WBLOCKMODEL_H
