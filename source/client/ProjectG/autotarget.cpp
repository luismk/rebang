// Original: d:\build\custom\temp\code\client\projectg\autotarget.cpp
#include "autotarget.h"
#include "golfball.h"
#include "quadtree.h"

#include <math.h>

// autotarget.cpp:20
CAutoTarget::~CAutoTarget() {
	m_pointList.clear();
}

// autotarget.cpp:25
void CAutoTarget::Reset(void) {
	m_pointList.clear();
}

// autotarget.cpp:30
float CAutoTarget::GetAngle(void) {
	return CalcDeltaAngle(GetPoint(0) - GolfBall().m_pos, WVector::UNIT_POS_Z);
}

// autotarget.cpp:35
float CAutoTarget::InArea(const WVector &a, const WVector &b, float radius) const {
	return sqrtf((a.x - b.x) * (a.x - b.x) + (a.z - b.z) * (a.z - b.z)) - radius;
}

// autotarget.cpp:107
// The point of segment [index, index + 1] nearest to the ball on ground type 1,
// sampled every 16 units.
WVector CAutoTarget::GetNearest(unsigned char index) {
	float t = 0.0f;
	float nearest = 1.0f;
	float minDist = 3200.0f;
	float step = 1.0f / ceil((m_pointList[index + 1] - m_pointList[index]).Magnitude() * 0.0625f);
	WVector pos;

	for (; t < 1.0f; t += step) {
		pos = (1.0f - t) * m_pointList[index] + t * m_pointList[index + 1];

		if (GetPVS().GetGroundType(pos, true, 0) == 1) {
			float dist = (GolfBall().m_pos - pos).Magnitude();
			if (dist < minDist) {
				minDist = dist;
				nearest = t;
			}
		}
	}

	return (1.0f - nearest) * m_pointList[index] + nearest * m_pointList[index + 1];
}

// autotarget.cpp:137
float CAutoTarget::GetDistance(const WVector &a, const WVector &b) {
	float dx = a.x - b.x;
	float dz = a.z - b.z;

	return sqrtf(dx * dx + dz * dz);
}

// autotarget.cpp:190
// Where the line through points index and index + 1 (relative to the ball)
// crosses the circle of the given radius around the ball, on the XZ plane.
WVector CAutoTarget::SolveEquation(unsigned char index, float radius) {
	WVector a, b, pos;
	a = m_pointList[index] - GolfBall().m_pos;
	b = m_pointList[index + 1] - GolfBall().m_pos;
	float x1, z1, x2, z2;

	if (WAbsBySign(a.x - b.x) < 1e-5f) {
		x1 = x2 = a.x;

		z1 = sqrtf(radius * radius - a.x * a.x);
		z2 = z1 * -1.0f;
	} else {
		float slope = (b.z - a.z) / (b.x - a.x);
		float intercept = a.z - slope * a.x;

		float disc = (slope * slope + 1.0f) * radius * radius - intercept * intercept;

		x1 = (sqrtf(disc) - slope * intercept) / (slope * slope + 1.0f);
		x2 = (slope * intercept * -1.0f - sqrtf(disc)) / (slope * slope + 1.0f);

		z1 = slope * x1 + intercept;
		z2 = slope * x2 + intercept;
	}

	if (x1 >= (a.x > b.x ? b.x : a.x) && x1 <= (a.x < b.x ? b.x : a.x)) {
		pos.x = x1;
		pos.z = z1;
	} else {
		pos.x = x2; pos.z = z2;
	}

	return pos;
}
