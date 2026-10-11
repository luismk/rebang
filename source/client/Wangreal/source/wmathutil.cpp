// Original: d:\build\custom\temp\code\client\wangreal\source\wmathutil.cpp
#include "../include/wmath.h"
#include "../include/wview.h"
#include "../client/Wangreal/include/wtypes.h"

// Defined in client/ProjectG/Avatar_Avatar.
float RayIntersectAABB(const Waabb& aabb, const WVector& vtrPos, const WVector& vtrMoveDelta, WPlane* pReturnPlane, float fRadius);

void __cdecl MakePlaneEq(WPlane* plane, const Waabb& aabb, const WMatrix* pmtxWorld);

// The box corner each of the eight vertices takes per axis: 0 = min, 1 = max.
static int box_v[8][3] = {
	{ 0, 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 }, { 1, 1, 0 },
	{ 0, 0, 1 }, { 1, 0, 1 }, { 0, 1, 1 }, { 1, 1, 1 }
};

// The four corners of each face of the box, in pairs of opposite faces.
static int box_p[6][4] = {
	{ 0, 2, 3, 1 }, { 4, 5, 7, 6 },
	{ 0, 1, 5, 4 }, { 3, 2, 6, 7 },
	{ 1, 3, 7, 5 }, { 2, 0, 4, 6 }
};

// wmathutil.cpp:34
// The six face planes of aabb, transformed by matrix when there is one.
void __cdecl MakePlaneEq(WPlane* out_plane, const Waabb& aabb, const WMatrix* matrix) {
	WVector vt[8];
	int i;

	if (matrix != NULL) {
		for (i = 0; i < 8; i++)
			vt[i] = WVector(box_v[i][0] ? aabb.max.x : aabb.min.x, box_v[i][1] ? aabb.max.y : aabb.min.y, box_v[i][2] ? aabb.max.z : aabb.min.z) * *matrix;
	} else {
		for (i = 0; i < 8; i++)
			vt[i] = WVector(box_v[i][0] ? aabb.max.x : aabb.min.x, box_v[i][1] ? aabb.max.y : aabb.min.y, box_v[i][2] ? aabb.max.z : aabb.min.z);
	}

	for (i = 0; i < 6; i += 2) {
		out_plane[i].normal = WCrossProduct(vt[box_p[i][0]] - vt[box_p[i][1]], vt[box_p[i][1]] - vt[box_p[i][2]]);
		out_plane[i].dis = -(out_plane[i].normal * vt[box_p[i][3]]);

		out_plane[i + 1].normal = -out_plane[i].normal;
		out_plane[i + 1].dis = -(out_plane[i + 1].normal * vt[box_p[i + 1][3]]);
	}
}

// wmathutil.cpp:61
bool PlaneFromVecs(WPlane* out_plane, const WVector& dir1, const WVector& dir2, const WVector& p) {
	out_plane->normal = WCrossProduct(dir1, dir2);
	out_plane->normal.Normalize();
	if (out_plane->normal.x * out_plane->normal.x + out_plane->normal.y * out_plane->normal.y + out_plane->normal.z * out_plane->normal.z == 0.0f)
		return false;

	out_plane->dis = -(p.x * out_plane->normal.x + p.y * out_plane->normal.y + p.z * out_plane->normal.z);
	return true;
}

// wmathutil.cpp:72
// The fraction of vec travelled before pivot (a sphere of radius rlen) hits the
// box bounded by the three plane pairs; 1.0f when it does not.
float __cdecl RayIntersect(const WVector& pivot, const WVector& vec, WPlane* plane, WPlane* out, float rlen) {
	WVector cross;
	float dotproduct;
	float len;
	float dis;
	int i;

	for (i = 0; i < 6; i += 2) {
		WPlane* p = &plane[i];

		dotproduct = p->y * vec.y + (p->x * vec.x + p->z * vec.z);

		if (WAbsBySign(dotproduct) < g_EPSILON)
			continue;

		if (dotproduct > 0.0f) {
			p++;
			dotproduct = -dotproduct;
		}

		dis = *p * pivot - rlen;
		dotproduct = -dotproduct;

		if (dis > dotproduct)
			return 1.0f;

		if (!(dis >= 0.0f))
			continue;

		len = dis / dotproduct;
		cross = pivot + vec * len;

		int j;
		for (j = 0; j < 6; j += 2) {
			if (j == i)
				continue;
			if (plane[j] * cross - rlen > 0.0f || plane[j + 1] * cross - rlen > 0.0f)
				break;
		}

		if (j == 6) {
			if (out != NULL)
				*out = WPlane(p->x, p->y, p->z, p->dis - rlen);
			return len;
		}
	}

	return 1.0f;
}

// wmathutil.cpp:123
float __cdecl RayIntersectAABB(const Waabb& aabb, const WVector& vtrPos, const WVector& vtrMoveDelta, WMatrix* pmtxWorld, WPlane* pReturnPlane, float fRadius) {
	WPlane plane[6];

	if (pmtxWorld == NULL)
		return RayIntersectAABB(aabb, vtrPos, vtrMoveDelta, pReturnPlane, fRadius);

	MakePlaneEq(plane, aabb, pmtxWorld);
	return RayIntersect(vtrPos, vtrMoveDelta, plane, pReturnPlane, fRadius);
}

// wmathutil.cpp:135
// AabbIntersect against an oriented box: the fraction of direction src_aabb
// travels before touching dest_obb; 1.0f when it does not.
float __cdecl ObbIntersect(const Waabb& src_aabb, const WVector& direction, const Wobb& dest_obb, WPlane* out_plane) {
	float t1;
	WVector normal;
	int i;
	WVector vec;

	for (i = 0; i < 3; i++) {
		if (WAbsBySign(direction.p[i]) < g_EPSILON)
			continue;

		if (direction.p[i] > 0.0f) {
			if (!dest_obb.IsInclude(src_aabb.min) || !dest_obb.IsInclude(src_aabb.min + direction))
				continue;

			t1 = (dest_obb.center.p[i] - src_aabb.max.p[i]) / direction.p[i];
		} else {
			// The x sum's operand order follows the original's scheduling.
			if (!dest_obb.IsInclude(src_aabb.max) || !dest_obb.IsInclude(WVector(direction.x + src_aabb.max.x, src_aabb.max.y + direction.y, src_aabb.max.z + direction.z)))
				continue;

			t1 = (dest_obb.center.p[i] - src_aabb.min.p[i]) / direction.p[i];
		}

		if (t1 < 0.0f) {
			if (t1 > -0.001f)
				t1 = 0.0f;
		}
		if (!(t1 >= 0.0f))
			continue;

		vec = direction * t1;

		int j;
		for (j = 0; j < 3; j++) {
			if (j == i)
				continue;
			if (src_aabb.max.p[j] + vec.p[j] < dest_obb.center.p[j] || src_aabb.min.p[j] + vec.p[j] > dest_obb.center.p[j])
				break;
		}

		if (j == 3) {
			if (out_plane != NULL) {
				normal.Reset();
				normal.p[i] = direction.p[i] < 0.0f ? 1.0f : -1.0f;
				*out_plane = WPlane(normal, vec.p[i] < 0.0f ? WVector(vec.x + src_aabb.min.x, vec.y + src_aabb.min.y, vec.z + src_aabb.min.z) : WVector(vec.x + src_aabb.max.x, vec.y + src_aabb.max.y, vec.z + src_aabb.max.z));
			}
			return t1;
		}
	}

	return 1.0f;
}

// wmathutil.cpp:367
// The fraction of direction src_aabb travels before touching dest_aabb; 1.0f
// when it does not. out_plane receives the face of src_aabb that touches.
float __cdecl AabbIntersect(const Waabb& src_aabb, const WVector& direction, const Waabb& dest_aabb, WPlane* out_plane) {
	float t1;
	WVector normal;
	int i;
	WVector vec;

	for (i = 0; i < 3; i++) {
		if (WAbsBySign(direction.p[i]) < g_EPSILON)
			continue;

		if (direction.p[i] > 0.0f) {
			if (src_aabb.min.p[i] > dest_aabb.max.p[i] || src_aabb.max.p[i] + direction.p[i] < dest_aabb.min.p[i])
				continue;

			t1 = (dest_aabb.min.p[i] - src_aabb.max.p[i]) / direction.p[i];
		} else {
			if (src_aabb.min.p[i] + direction.p[i] > dest_aabb.max.p[i] || dest_aabb.min.p[i] > src_aabb.max.p[i])
				continue;

			t1 = (dest_aabb.max.p[i] - src_aabb.min.p[i]) / direction.p[i];
		}

		if (t1 < 0.0f) {
			if (t1 > -0.001f)
				t1 = 0.0f;
		}
		if (!(t1 >= 0.0f))
			continue;

		vec = direction * t1;

		int j;
		for (j = 0; j < 3; j++) {
			if (j == i)
				continue;
			if (src_aabb.max.p[j] + vec.p[j] < dest_aabb.min.p[j] || src_aabb.min.p[j] + vec.p[j] > dest_aabb.max.p[j])
				break;
		}

		if (j == 3) {
			if (out_plane != NULL) {
				normal.Reset();
				normal.p[i] = direction.p[i] < 0.0f ? 1.0f : -1.0f;
				*out_plane = WPlane(normal, vec.p[i] < 0.0f ? WVector(vec.x + src_aabb.min.x, vec.y + src_aabb.min.y, vec.z + src_aabb.min.z) : WVector(vec.x + src_aabb.max.x, vec.y + src_aabb.max.y, vec.z + src_aabb.max.z));
			}
			return t1;
		}
	}

	return 1.0f;
}

// wmathutil.cpp:471
float __cdecl DotContact(const WVector& vec, const WPlane* plane) {
	float max, d;

	for (int i = 0; i < 3; i++) {
		d = vec * plane[i * 2];
		if (i == 0)
			max = d;
		else if (d > max)
			max = d;

		if (!(d > 0.0f)) {
			d = vec * plane[i * 2 + 1];
			if (d > max)
				max = d;
		}
	}

	return max;
}

// wmathutil.cpp:494
WMatrix __cdecl Make3rdCamMatrix(const WVector& vtrPos, const WVector& vtrTarget) {
	WMatrix mat;

	mat.za = vtrTarget - vtrPos;
	mat.za.Normalize();

	mat.ya = WVector::UNIT_POS_Y;
	mat.xa = WCrossProduct(mat.ya, mat.za);
	mat.xa.Normalize();

	mat.ya = WCrossProduct(mat.za, mat.xa);
	mat.ya.Normalize();

	mat.pivot = vtrPos;

	return mat;
}

// wmathutil.cpp:514
WRect __cdecl ScaleRect(WView* pView, WRect* prcSrc) {
	WRect dest;

	dest.x = pView->GetWidth() * prcSrc->x / 640.0f;
	dest.y = pView->GetHeight() * prcSrc->y / 480.0f;
	dest.w = pView->GetWidth() * prcSrc->w / 640.0f;
	dest.h = pView->GetHeight() * prcSrc->h / 480.0f;

	return dest;
}

// wmathutil.cpp:533
void __cdecl ScalePoint(WView* pView, WPoint* pPoint) {
	pPoint->x = pView->GetHeight() * pPoint->x / 640.0f;
	pPoint->y = pView->GetHeight() * pPoint->y / 480.0f;
}
