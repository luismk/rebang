// Original: d:\build\custom\temp\code\client\wangreal\include\wmathutil.inl
// Free angle helpers; included at the end of wmath.h.

// wmathutil.inl:114 (emitted out of line in quadtree)
// The fraction of moveDelta at which a sphere of the given radius moving from
// pos first touches the solid box, 1.0 when it does not; the face it touches
// goes to pReturnPlane. The axes are tried in the order x, z, y.
QUADTREE_WMATH_INLINE float RayIntersectAABBSolid(const Waabb& aabb, const WVector& pos, const WVector& moveDelta, WPlane* pReturnPlane, float radius) {
	int i;
	int j;
	float t;
	float hit;

	for (i = 0; i < 3; i = i == 0 ? 2 : (i == 1 ? 3 : 1)) {
		if (Wabs(moveDelta.p[i]) >= g_EPSILON) {
			if (moveDelta.p[i] > 0.0f) {
				if (aabb.max.p[i] + radius < pos.p[i] || pos.p[i] + moveDelta.p[i] < aabb.min.p[i] - radius)
					return 1.0f;
				t = (aabb.min.p[i] - radius - pos.p[i]) / moveDelta.p[i];
			} else {
				if (aabb.min.p[i] - radius > pos.p[i] || pos.p[i] + moveDelta.p[i] > aabb.max.p[i] + radius)
					return 1.0f;
				t = (aabb.max.p[i] + radius - pos.p[i]) / moveDelta.p[i];
			}

			for (j = 0; j < 3; j++) {
				if (j == i)
					continue;

				hit = pos.p[j] + moveDelta.p[j] * t;
				if (hit < aabb.min.p[j] - radius || hit > aabb.max.p[j] + radius)
					break;
			}

			if (j == 3) {
				if (pReturnPlane) {
					for (j = 0; j < 3; j++) {
						if (j != i) {
							pReturnPlane->normal.p[j] = 0.0f;
						} else {
							pReturnPlane->normal.p[j] = moveDelta.p[i] < 0.0f ? 1.0f : -1.0f;
							pReturnPlane->dis = -((pos.p[i] + moveDelta.p[i] * t) * pReturnPlane->normal.p[i]);
						}
					}
				}
				return t;
			}
		}
	}

	return 1.0f;
}

// wmathutil.inl:173
// The angle brought into (-pi, pi].
inline float GetBaseAngle(float angle) {
	if ((angle > 0.0f ? angle : -angle) > 6.2831855f)
		angle = angle - (float)floor(angle * 0.15915494f) * 6.2831855f;

	if (angle > 3.1415927f)
		return angle - 6.2831855f;
	else if (angle < -3.1415927f)
		return angle + 6.2831855f;

	return angle;
}
