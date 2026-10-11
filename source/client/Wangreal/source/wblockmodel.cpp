// Original: d:\build\custom\temp\code\client\wangreal\source\wblockmodel.cpp
#include "../include/wblockmodel.h"
#include "../include/wview.h"
#include "../include/wutil.h"

unsigned long WBlockModel::g_treemask[8] = { 0x15, 0x16, 0x19, 0x1A, 0x25, 0x26, 0x29, 0x2A };

// wblockmodel.cpp:21
WBlockModel::WBlockModel() : m_colarea(8, 0) {
	m_root = NULL;
}

// wblockmodel.cpp:26
WBlockModel::~WBlockModel() {
	Reset();
}

// wblockmodel.cpp:31
void WBlockModel::Reset() {
	for (WConvexArea* area = m_colarea.Start(); area != NULL; area = m_colarea.Next())
		delete [] (char*)area;
	m_colarea.Reset();
}

// wblockmodel.cpp:39
void WBlockModel::Build() {
	m_root = ExtendTree(&m_colarea);
}

// wblockmodel.cpp:46
int WBlockModel::CalcDepth(WOctree* tree) {
	int depth = 0;

	for (int i = 0; i < 8; i++) {

		if (tree->mask & (1 << i)) {

			int d = CalcDepth(tree->leaf[i]);
			if (d > depth)
				depth = d;
		}
	}

	return depth + 1;
}

// wblockmodel.cpp:63
int WBlockModel::PositionDecision(int flag) {
	int result = 0;

	for (int i = 0; i < 6; i += 2) {

		if ((flag & (3 << i)) == (3 << i))
			return 8;

		if (!(flag & (1 << i)))
			result |= 1 << (i / 2);
	}

	return result;
}

// wblockmodel.cpp:81
WVector WBlockModel::FixedPivot(WList<WConvexArea*>* list, WVector pivot) {
	for (int i = 0; i < 3; i++) {

		WVector fixed_pivot = pivot;
		WConvexArea* area = list->Start();
		int cnt = 0;
		for (; area != NULL; area = list->Next()) {

			if (((CalcFlag(area->aabb, pivot) >> (i * 2)) & 3) == 3) {

				float fixed = Abs(area->aabb.min.p[i] - pivot.p[i]) < Abs(area->aabb.max.p[i] - pivot.p[i]) ? area->aabb.min.p[i] - 1e-4f : area->aabb.max.p[i] + 1e-4f;
				if (cnt == 0 || Abs(fixed - pivot.p[i]) < Abs(fixed_pivot.p[i] - pivot.p[i]))
					fixed_pivot.p[i] = fixed;
				cnt++;
			}
		}

		area = list->Start();
		int fixed_cnt = 0;
		for (; area != NULL; area = list->Next()) {

			if (((CalcFlag(area->aabb, fixed_pivot) >> (i * 2)) & 3) == 3)
				fixed_cnt++;
		}

		if (fixed_cnt < cnt)
			pivot = fixed_pivot;
	}

	return pivot;
}

// wblockmodel.cpp:121
WBlockModel::WOctree* WBlockModel::ExtendTree(WList<WConvexArea*>* list) {
	WList<WConvexArea*> sublist[9];
	WConvexArea* area;
	int cnt;
	int childcnt;
	int mask;
	int i;

	WVector pivot = WVector::ZERO;

	for (area = list->Start(), cnt = 0; area != NULL; area = list->Next(), cnt++)
		pivot += (area->aabb.min + area->aabb.max) * 0.5f;

	if (cnt == 0)
		return NULL;

	pivot *= 1.0f / cnt;

	for (area = list->Start(); area != NULL; area = list->Next()) {

		if (CalcFlag(area->aabb, pivot) != 0x3f) {

			pivot = FixedPivot(list, pivot);
			break;
		}
	}

	for (area = list->Start(), childcnt = 0, cnt = 0, mask = 0; area != NULL; area = list->Next(), cnt++) {

		int slot = PositionDecision(CalcFlag(area->aabb, pivot));
		sublist[slot] += area;
		if (slot < 8) {

			childcnt++;
			mask |= 1 << slot;
		}
	}

	WOctree* tree = (WOctree*)new char[sizeof(WOctree) + (cnt - childcnt - 1) * sizeof(WOctree::FruitList)];
	tree->mask = (unsigned char)mask;
	tree->listnum = cnt - childcnt;
	tree->pivot = pivot;
	tree->check = 0;

	for (area = sublist[8].Start(), i = 0; area != NULL; area = sublist[8].Next(), i++) {

		tree->list[i].area = area;
		tree->list[i].flag = CalcFlag(area->aabb, pivot);
	}

	for (i = 0; i < 8; i++)
		tree->leaf[i] = ExtendTree(&sublist[i]);

	return tree;
}

// wblockmodel.cpp:185
void WBlockModel::Add(WConvexArea* area) {
	if (m_colarea.Start() == NULL)
		m_colbound = area->aabb;
	else
		m_colbound = ExtendAABB(m_colbound, area->aabb);
	m_colarea += area;
}

// wblockmodel.cpp:194
Waabb WBlockModel::ExtendAABB(const Waabb& aabb1, const Waabb& aabb2) {

	return Waabb(WVector(Min(aabb1.min.x, aabb2.min.x), Min(aabb1.min.y, aabb2.min.y), Min(aabb1.min.z, aabb2.min.z)), WVector(Max(aabb1.max.x, aabb2.max.x), Max(aabb1.max.y, aabb2.max.y), Max(aabb1.max.z, aabb2.max.z)));
}

// wblockmodel.cpp:200
Waabb WBlockModel::ExtendAABB(const WVector& v1, const WVector& v2, float rlen) {

	return Waabb(WVector(Min(v1.x, v2.x) - rlen, Min(v1.y, v2.y) - rlen, Min(v1.z, v2.z) - rlen), WVector(Max(v1.x, v2.x) + rlen, Max(v1.y, v2.y) + rlen, Max(v1.z, v2.z) + rlen));
}

// wblockmodel.cpp:206
bool WBlockModel::ColTestRough(const WVector& pos, const WVector& vec, float rlen, const Waabb& aabb) {

	for (int i = 0; i < 3; i++) {

		if (vec.p[i] > 0) {

			if (pos.p[i] + vec.p[i] + rlen < aabb.min.p[i] || pos.p[i] - rlen > aabb.max.p[i])
				return false;
		} else {

			if (pos.p[i] + vec.p[i] - rlen > aabb.max.p[i] || pos.p[i] + rlen < aabb.min.p[i])
				return false;
		}
	}

	return true;
}

// wblockmodel.cpp:234
float WBlockModel::CollTest(const WVector& pos, const WVector& vec, float rlen, WPlane* out, WConvexArea* area, float min) {
	float t[3];
	WVector contact;

	for (int i = 0; i < area->count; i++) {


		t[0] = area->plane[i] * pos - rlen; t[1] = area->plane[i] * (pos + vec) - rlen;

		if (t[0] > 0 && t[1] > 0)
			return min;

		if (t[0] >= 0 && t[1] <= 0 && t[0] > t[1]) {

			t[2] = t[0] / (t[0] - t[1]);

			if (min > t[2]) {

				contact = pos + vec * t[2];

				int j;
				for (j = 0; j < area->count; j++) {
					if (i != j) {
						const WPlane& plane = area->plane[j];
						if (plane.normal * contact + plane.dis - rlen > 0)
							break;
					}
				}

				if (j == area->count) {

					if (out)
						*out = area->plane[i];
					return t[2];
				}
			}
		}
	}

	return min;
}

// wblockmodel.cpp:273
float WBlockModel::CollTest(const WVector& pos, const WVector& vec, float rlen, WPlane* out) {
	float min = 1.0f;


	if (ColTestRough(pos, vec, rlen, m_colbound)) {

		if (m_root) {
			return Explore(m_root, ExtendAABB(pos, WVector(pos.x + vec.x, pos.y + vec.y, pos.z + vec.z), rlen), pos, vec, rlen, out, 1.0f);
		} else {
			for (WConvexArea* area = m_colarea.Start(); area != NULL; area = m_colarea.Next()) {
				if (ColTestRough(pos, vec, rlen, area->aabb))
					min = CollTest(pos, vec, rlen, out, area, min);
			}
		}
	}

	return min;
}

// wblockmodel.cpp:291
bool WBlockModel::IsConvex(const WVector* veclist, int vtxnum, WPlane* out) {
	WPlane plane;
	float t;



	for (int i = 0; i < vtxnum; i += 3) {

		plane = WPlane(WCrossProduct(veclist[i + 1] - veclist[i], veclist[i + 2] - veclist[i]).Normalize(), veclist[i]);

		for (int j = 0; j < vtxnum; j++) {
			t = WDotProduct(plane.normal, veclist[j]) + plane.dis;
			if (t > 0.001f) {

				*out = plane;
				return false;
			}
		}
	}

	return true;
}

// wblockmodel.cpp:314
void WBlockModel::AddTriangleList(const WVector* veclist, int vtxnum) {
	WPlane plist[32];
	WPlane plane;
	Waabb aabb;
	int n = 0;

	IsConvex(veclist, vtxnum, &plane);



	for (int i = 0; i < vtxnum;) {

		plane = WPlane(WCrossProduct(veclist[i + 1] - veclist[i], veclist[i + 2] - veclist[i]).Normalize(), veclist[i]);

		int j;
		for (j = 0; j < n; j++) {



			if (Abs(plane.x - plist[j].x) < 0.001f && Abs(plane.y - plist[j].y) < 0.001f && Abs(plane.z - plist[j].z) < 0.001f && Abs(plane.dis - plist[j].dis) < 0.001f)
				break;
		}

		if (j == n)
			plist[n++] = plane;

		for (int k = 0; k < 3; k++, i++) {

			if (i == 0 || aabb.min.x > veclist[i].x)
				aabb.min.x = veclist[i].x;
			if (i == 0 || aabb.min.y > veclist[i].y)
				aabb.min.y = veclist[i].y;
			if (i == 0 || aabb.min.z > veclist[i].z)
				aabb.min.z = veclist[i].z;
			if (i == 0 || aabb.max.x < veclist[i].x)
				aabb.max.x = veclist[i].x;
			if (i == 0 || aabb.max.y < veclist[i].y)
				aabb.max.y = veclist[i].y;
			if (i == 0 || aabb.max.z < veclist[i].z)
				aabb.max.z = veclist[i].z;
		}
	}

	WConvexArea* area = (WConvexArea*)new char[sizeof(WConvexArea) + (n - 1) * sizeof(WPlane)];

	area->aabb = aabb;

	area->count = n; memcpy(area->plane, plist, n * sizeof(WPlane));

	Add(area);
}

// wblockmodel.cpp:366
unsigned char WBlockModel::CalcFlag(const Waabb& aabb, const WVector& pivot) {
	int flag = 0;
	if (aabb.min.x < pivot.x)
		flag = 1;
	if (aabb.max.x > pivot.x)
		flag |= 2;
	if (aabb.min.y < pivot.y)
		flag |= 4;
	if (aabb.max.y > pivot.y)
		flag |= 8;
	if (aabb.min.z < pivot.z)
		flag |= 16;
	if (aabb.max.z > pivot.z)
		flag |= 32;
	return (unsigned char)flag;
}

// wblockmodel.cpp:384
float WBlockModel::Explore(WOctree* tree, const Waabb& aabb, const WVector& pos, const WVector& vec, float rlen, WPlane* out, float min) {







	tree->check = 1;
	unsigned int flag = CalcFlag(aabb, tree->pivot);
	WOctree::FruitList* fruit = tree->list;
	for (int i = tree->listnum - 1; i >= 0; i--, fruit++) {

		unsigned int f = fruit->flag & flag;

		if ((f & 3) && (f & 0xC) && (f & 0x30)) {

			WConvexArea* area = fruit->area;
			if (ColTestRough(pos, vec, rlen, area->aabb)) {

				min = CollTest(pos, vec, rlen, out, area, min);

				fruit->area->check = 1;
			}
		}
	}

	for (int j = 0; j < 8; j++) {
		if ((tree->mask & (1 << j)) && (g_treemask[j] & flag) == g_treemask[j])
			min = Explore(tree->leaf[j], aabb, pos, vec, rlen, out, min);
	}

	return min;
}

// wblockmodel.cpp:425
static void RenderAABB(const Waabb& aabb, WView* view, unsigned long diffuse, float rlen) {
	// The 6 faces of a box, each as 4 corner indices in edge order.
	static int box_p[6][4] = {
		{ 0, 2, 3, 1 }, { 4, 5, 7, 6 }, { 0, 1, 5, 4 }, { 3, 2, 6, 7 }, { 1, 3, 7, 5 }, { 2, 0, 4, 6 }
	};
	// The 8 corners of a box: per axis, nonzero takes the max side.
	static int box_v[8][3] = {
		{ 0, 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 }, { 1, 1, 0 }, { 0, 0, 1 }, { 1, 0, 1 }, { 0, 1, 1 }, { 1, 1, 1 }
	};
	WVector vt[8];
	for (int i = 0; i < 8; i++)
		vt[i] = WVector(box_v[i][0] ? (float)(aabb.max.x + rlen) : (float)(aabb.min.x - rlen), box_v[i][1] ? (float)(aabb.max.y + rlen) : (float)(aabb.min.y - rlen), box_v[i][2] ? (float)(aabb.max.z + rlen) : (float)(aabb.min.z - rlen));

	for (int f = 0; f < 6; f++) {
		for (int j = 0; j < 4; j++)
			view->DrawLine(vt[box_p[f][(j - 1) & 3]], diffuse, vt[box_p[f][j]], diffuse, 0);
	}
}

// wblockmodel.cpp:443
void WBlockModel::DebugRender(WView* view) {


	DebugRender(view, m_colbound, m_root, 0);
}

// wblockmodel.cpp:451
void WBlockModel::DebugRender(WView* view, const Waabb& aabb, WOctree* tree, int level) {

	Waabb subaabb;





	if (tree->check) {

		RenderAABB(aabb, view, 0x4000FF00, 0);
		tree->check = 0;
	}



	for (int k = tree->listnum - 1; k >= 0; k--) {


		if (tree->list[k].area->check) {



			tree->list[k].area->check = 0;
			RenderAABB(tree->list[k].area->aabb, view, 0xFFFF0000, 0);
		}
	}

	for (int i = 0; i < 8; i++) {

		if (tree->mask & (1 << i)) {

			subaabb = ExtendAABB(tree->pivot, WVector(!(i & 1) ? aabb.min.x : aabb.max.x, !(i & 2) ? aabb.min.y : aabb.max.y, !(i & 4) ? aabb.min.z : aabb.max.z), 0);
			DebugRender(view, subaabb, tree->leaf[i], level + 1);
		}
	}
}
