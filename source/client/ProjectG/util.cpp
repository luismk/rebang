// Original: d:\build\custom\temp\code\client\projectg\util.cpp
#include <windows.h>
#include <stdio.h>
#include <string>

#include "commonutil.h"
#include "ObjectFactory.h"
#include "thirties/gamath.h"
#include "polysoup.h"

namespace Ut {

// util.cpp:19
// Builds "<prefix>_<class name>_<id, 6 digits>" (the prefix part only when
// one is given).
void MakeObjectName(std::string& name, const char* pszPrefix, const IObject& object, int id) {
	name.clear();

	if (pszPrefix != 0) {
		name = pszPrefix;
		name += "_";
	}

	name += object.GetRTTI()->GetName();
	name += MakeStr("_%06d", id);
}

// util.cpp:32
// Appends to positions the centre of each triangle that is not within range
// of a centre already in the list.
void BuildAppropriatePosList(const std::vector<WPolySoup::sTriangle*>* triangles, float range, std::vector<WVector>& positions) {
	std::vector<WPolySoup::sTriangle*>::const_iterator it;
	for (it = triangles->begin(); it != triangles->end(); ++it) {
		WVector v0 = (*it)->m_vertex[0];
		WVector v1 = (*it)->m_vertex[1];
		WVector v2 = (*it)->m_vertex[2];
		WVector center;
		center.x = (v0.x + v1.x + v2.x) * 0.33333334f;
		center.y = (v0.y + v1.y + v2.y) * 0.33333334f;
		center.z = (v0.z + v1.z + v2.z) * 0.33333334f;

		std::vector<WVector>::iterator pos = positions.begin();
		for (; pos != positions.end(); ++pos) {
			float dx = center.x - pos->x;
			float dy = center.y - pos->y;
			float dz = center.z - pos->z;
			if (dx * dx + dy * dy + dz * dz < range * range) {
				break;
			}
		}

		if (pos == positions.end()) {
			positions.push_back(center);
		}
	}
}

// util.cpp:65
// Formats the local time with pszFormat (year, month, day, hour, minute,
// second, milliseconds) into pszBuffer and returns pszBuffer.
char* GenDateTimeStrA(char* pszBuffer, const char* pszFormat) {
	SYSTEMTIME time;

	GetLocalTime(&time);

	sprintf(pszBuffer, pszFormat, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	return pszBuffer;
}

// util.cpp:79
// The shortest-arc rotation turning `from` onto `to`. When the two point in
// opposite directions, the half turn is made around fallbackAxis, or around
// an axis perpendicular to `from` when fallbackAxis is zero.
gaQ GetRotationQuaternion(const gaV3& from, const gaV3& to, const gaV3& fallbackAxis) {
	gaQ q;
	gaV3 v0 = from;
	gaV3 v1 = to;
	v0.Normalize();
	v1.Normalize();

	float d = v0.Dot(v1);

	if (d >= gaMath::ID) {
		return gaQ::ID;
	}

	if (d < gaMath::TOLERANCE - gaMath::ID) {
		if (fallbackAxis != gaV3::ZERO) {
			q.FromAxisAngle(fallbackAxis, gaMath::PI);
		} else {
			gaV3 axis = gaV3::UNIT_X.Cross(from);
			if (gaMath::NearlyZero(axis.Length(), gaMath::TOLERANCE * gaMath::TOLERANCE)) {
				axis = gaV3::UNIT_Y.Cross(from);
			}
			q.FromAxisAngle(axis, gaMath::PI);
		}
	} else {
		float s = gaMath::Sqrt((gaMath::ID + d) * gaMath::TWO);
		float invs = gaMath::ID / s;
		gaV3 c = v0.Cross(v1);
		q.x = c.x * invs;
		q.y = c.y * invs;
		q.z = c.z * invs;
		q.w = s * gaMath::HALF;
		q.Normalize();
	}

	return q;
}

} // namespace Ut
