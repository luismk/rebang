// Original: d:\build\custom\temp\code\shared\thirties\thgeometry.h
#ifndef PG_SHARED_THIRTIES_THGEOMETRY_H
#define PG_SHARED_THIRTIES_THGEOMETRY_H

#include "..\..\defines.h"
#include "thirties/gamath.h"

// A bounding sphere (ThSphere::Set stores the centre then the radius). Only
// the members the decompiled units use are transcribed.
struct ThSphere {
	gaV3 center;    // 0x0
	float radius;   // 0xc
};

CHECK_SIZE(ThSphere, 0x10);

#endif // PG_SHARED_THIRTIES_THGEOMETRY_H
