// Original: d:\build\custom\temp\code\client\wangreal\include\wmath.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WMATH_H
#define PG_CLIENT_WANGREAL_INCLUDE_WMATH_H

#include "..\..\defines.h"
#include <math.h>
#include <string.h>

// The original's out-of-line copies of some wmath inlines sit in one unit's
// object; that unit defines its macro empty before its includes.
#ifndef AVATAR_AVATAR_WMATH_INLINE
#define AVATAR_AVATAR_WMATH_INLINE inline
#endif
#ifndef CAMERAWORK_WMATH_INLINE
#define CAMERAWORK_WMATH_INLINE inline
#endif
#ifndef AMBIENTSOUND_WMATH_INLINE
#define AMBIENTSOUND_WMATH_INLINE inline
#endif
#ifndef GOLFDOC_WMATH_INLINE
#define GOLFDOC_WMATH_INLINE inline
#endif
#ifndef WBONE_WMATH_INLINE
#define WBONE_WMATH_INLINE inline
#endif
#ifndef WVIEW_WMATH_INLINE
#define WVIEW_WMATH_INLINE inline
#endif
#ifndef CLIENTSETTING_WMATH_INLINE
#define CLIENTSETTING_WMATH_INLINE inline
#endif
#ifndef SEA_WMATH_INLINE
#define SEA_WMATH_INLINE inline
#endif
#ifndef POOL_WMATH_INLINE
#define POOL_WMATH_INLINE inline
#endif
#ifndef GOLFRULE_WMATH_INLINE
#define GOLFRULE_WMATH_INLINE inline
#endif
#ifndef WIND_WMATH_INLINE
#define WIND_WMATH_INLINE inline
#endif
#ifndef QUADTREE_WMATH_INLINE
#define QUADTREE_WMATH_INLINE inline
#endif

// Under /Op (Wangreal) these are read from storage (_g_HUGE, _g_EPSILON).
const float g_HUGE = 3.402823466e+38f;
const float g_EPSILON = 0.00001f;
const float g_CM_TO_WU = 0.32f;

// Nameless .rdata floats of Wangreal's math (pi, pi/2, degrees to radians,
// radians to degrees, epsilon), read through memory rather than literals.
extern "C" const float data_9eb420[5];

// wmath.h:98 (emitted out of line; the body sits in avatar_avatar.cpp for now)
float RadianNormalize2PI(float radian);
// wmath.h:106
float RadianNormalizePI(float radian);
// wmath.h:115
float RadianDelta(float a, float b);

// wmath.h:80
// Absolute value by the sign bit: the stored float's sign is tested, not an
// fabs instruction.
CLIENTSETTING_WMATH_INLINE float Wabs(const float& value) {
	return (*(const unsigned long*)&value & 0x80000000) ? -value : value;
}

// wmath.h:92
CLIENTSETTING_WMATH_INLINE int WisEqual(const float& a, const float& b, float eps) {
	return Wabs(a - b) < eps;
}

class WVector;
class WMatrix;

class WQuat {
public:
	WQuat() {
	}
	WQuat(float a, float b, float c, float d) {
		x = a; y = b; z = c; w = d;
	}

	void Reset(void);
	void Normalize(void);
	void ConvertToAxisAngle(WVector* axis, float* angle);
	void ConvertToRotationMatrix(WMatrix& m) const;
	void SetFromAngles(float x, float y, float z);
	void operator=(const WMatrix& m);
	WQuat& operator=(const WQuat& q);

private:
	void SetFromAxisAngle(const WVector& axis, float angle);

public:
	float x; // 0x0
	float y; // 0x4
	float z; // 0x8
	float w; // 0xC
};
CHECK_SIZE(WQuat, 0x10);
CHECK_FIELD(WQuat, x, 0x0);
CHECK_FIELD(WQuat, y, 0x4);
CHECK_FIELD(WQuat, z, 0x8);
CHECK_FIELD(WQuat, w, 0xC);

WQuat WQuaternionSlerp(const WQuat& from, const WQuat& to, float t);

class WVector {
public:
	WVector() {
	}
	WVector(const float a, const float b, const float c) {
		x = a; y = b; z = c;
	}
	WVector(const float* v) {
		x = v[0]; y = v[1]; z = v[2];
	}

	WVector operator-(void) const;
	WVector& operator=(const WVector& v);
	void operator+=(const WVector& v);
	void operator-=(const WVector& v);
	void operator*=(const float f);
	void operator/=(const float f);
	void Reset(void);
	float SquareMagnitude(void) const;
	float Magnitude(void) const;
	WVector& Normalize(void);
	void Negate(void);

	static const WVector ONE;
	static const WVector ZERO;
	static const WVector UNIT_POS_X;
	static const WVector UNIT_POS_Y;
	static const WVector UNIT_POS_Z;
	static const WVector UNIT_NEG_X;
	static const WVector UNIT_NEG_Y;
	static const WVector UNIT_NEG_Z;

	union {
		struct { float x, y, z; };
		float p[3];
	};
};
CHECK_SIZE(WVector, 0xC);
CHECK_FIELD(WVector, x, 0x0);
CHECK_FIELD(WVector, y, 0x4);
CHECK_FIELD(WVector, z, 0x8);

int WisEqual(const WVector& a, const WVector& b, float eps);
float __cdecl CalcDeltaAngle(const WVector& a, const WVector& b);

class WMatrix;
WVector operator+(const WVector& a, const WVector& b);
WVector operator-(const WVector& a, const WVector& b);
WVector operator*(const WVector& v, float f);
WVector operator*(float f, const WVector& v);
WVector operator*(const WVector& v, const WMatrix& m);
float WDotProduct(const WVector& a, const WVector& b);
float WDotProduct(const WQuat& a, const WQuat& b);
WVector WCrossProduct(const WVector& a, const WVector& b);
// wmath.h:442
float WVectorLen(const WVector& v);

class WPlane;
class WMatrix {
public:
	WMatrix() {   // wmath.h:274
	}
	WMatrix(float xx, float yx, float zx, float xy, float yy, float zy, float xz, float yz, float zz);   // wmath.inl:441
	WMatrix(const WPlane& plane);   // wmath.inl:455: the reflection about the plane

	WMatrix& operator=(const WMatrix& m);
	void operator=(const WQuat& q);
	void operator*=(const float f);
	void Reset(void);
	void Normalize(void);
	void GetRotMatrix(WMatrix* m) const;
	void AxisScale(WVector& scale);
	void Rotate(float angle, char axis);
	void Rotate(const WVector& angles);
	WMatrix operator~(void) const;   // wmath.inl:405 (emitted out of line in pool)

	static const WMatrix IDENTITY;
	static const WMatrix ZERO;

	union {
		struct { WVector xa, ya, za, pivot; };
		struct {
			float xx, yx, zx;
			float xy, yy, zy;
			float xz, yz, zz;
			float xm, ym, zm;
		};
		float p[12];
	};
};
CHECK_SIZE(WMatrix, 0x30);
CHECK_FIELD(WMatrix, xa, 0x0);
CHECK_FIELD(WMatrix, ya, 0xC);
CHECK_FIELD(WMatrix, za, 0x18);
CHECK_FIELD(WMatrix, pivot, 0x24);
CHECK_FIELD(WMatrix, xx, 0x0);
CHECK_FIELD(WMatrix, xy, 0xC);
CHECK_FIELD(WMatrix, xz, 0x18);
CHECK_FIELD(WMatrix, xm, 0x24);
CHECK_FIELD(WMatrix, p, 0x0);

WMatrix operator*(const WMatrix& a, const WMatrix& b);
float TransformZ(const WVector& v, const WMatrix& m);
// wmath.inl:264 (emitted out of line in ball)
WMatrix operator*(const WMatrix& m, float f);
// wmath.inl:286 (inline in the original; the body inlines into w3danispr when given here)
WMatrix RotMat(WVector angles);
WMatrix RotMat(float angle, char axis);
WMatrix RotMat(const WVector& axis, float angle);
float TransformZ(float x, float y, float z, const WMatrix& m);
bool operator==(const WMatrix& mat1, const WMatrix& mat2);
bool operator!=(const WMatrix& mat1, const WMatrix& mat2);
// wmath.inl:328 (emitted out of line in GolfRule)
WVector RotVec(const WVector& v, const WMatrix& m);

class WPlane {
public:
	WPlane();
	WPlane(const WVector& n, const WVector& p);   // wmath.inl:745 (emitted out of line in wview)
	WPlane(float a, float b, float c, float d) { x = a; y = b; z = c; dis = d; }   // wmath.h:419
	WPlane(const WVector& n, float d) { normal = n; dis = d; }                       // wmath.h:420
	WPlane& operator=(const WPlane& p);   // wmath.inl:751
	void Normalize();

	union {
		struct { float x, y, z, dis; };
		struct { float a, b, c, d; };
		struct { WVector normal; };
	};
};
CHECK_SIZE(WPlane, 0x10);
CHECK_FIELD(WPlane, x, 0x0);
CHECK_FIELD(WPlane, y, 0x4);
CHECK_FIELD(WPlane, z, 0x8);
CHECK_FIELD(WPlane, dis, 0xC);
CHECK_FIELD(WPlane, normal, 0x0);

// wmath.h:418
AVATAR_AVATAR_WMATH_INLINE WPlane::WPlane() {
}

class WMatrix4 {
public:
	WMatrix4() {
	}
	WMatrix4(float a00, float a01, float a02, float a03, float a10, float a11, float a12, float a13,
	         float a20, float a21, float a22, float a23, float a30, float a31, float a32, float a33);

	WMatrix4& operator=(const WMatrix4& m);   // wmath.inl:568 (emitted out of line in wview)

	static const WMatrix4 IDENTITY;
	static const WMatrix4 ZERO;

	union {
		struct {
			WVector xa; float wx;
			WVector ya; float wy;
			WVector za; float wz;
			WVector pivot; float wm;
		};
		struct {   // the w column is unnamed here (the PDB lists no alias)
			float xx, yx, zx; int : 32;
			float xy, yy, zy; int : 32;
			float xz, yz, zz; int : 32;
			float xm, ym, zm; int : 32;
		};
		float m[4][4];
		float p[16];
	};
};
CHECK_SIZE(WMatrix4, 0x40);
CHECK_FIELD(WMatrix4, xa, 0x0);
CHECK_FIELD(WMatrix4, wx, 0xC);
CHECK_FIELD(WMatrix4, ya, 0x10);
CHECK_FIELD(WMatrix4, wy, 0x1C);
CHECK_FIELD(WMatrix4, za, 0x20);
CHECK_FIELD(WMatrix4, wz, 0x2C);
CHECK_FIELD(WMatrix4, pivot, 0x30);
CHECK_FIELD(WMatrix4, wm, 0x3C);
CHECK_FIELD(WMatrix4, xx, 0x0);
CHECK_FIELD(WMatrix4, xm, 0x30);
CHECK_FIELD(WMatrix4, m, 0x0);
CHECK_FIELD(WMatrix4, p, 0x0);

void SetWMatrix4FromWMatrix(WMatrix4& m4, const WMatrix& m);
WMatrix4 operator*(const WMatrix4& a, const WMatrix4& b);   // wmath.inl:510 (emitted out of line in pool)

class WVector2D {
public:
	WVector2D() {
	}
	WVector2D(const float a, const float b) {
		x = a; y = b;
	}

	WVector2D& operator=(const WVector2D& v);
	void operator+=(const WVector2D& v);
	void operator*=(const float f);
	void operator/=(const float f);
	void Reset(void);
	WVector2D& Normalize(void);

	float x; // 0x0
	float y; // 0x4
};
CHECK_SIZE(WVector2D, 0x8);
CHECK_FIELD(WVector2D, x, 0x0);
CHECK_FIELD(WVector2D, y, 0x4);

class WVector4 {
public:
	WVector4() {
	}
	WVector4(float x, float y, float z, float w);

	union {
		struct { float x, y, z, w; };
		float p[4];
	};

	static const WVector4 ONE;
	static const WVector4 ZERO;
};
CHECK_SIZE(WVector4, 0x10);
CHECK_FIELD(WVector4, x, 0x0);
CHECK_FIELD(WVector4, y, 0x4);
CHECK_FIELD(WVector4, z, 0x8);
CHECK_FIELD(WVector4, w, 0xC);
CHECK_FIELD(WVector4, p, 0x0);

class WSphere {
public:
	WSphere() {
	}
	WSphere(const WVector& p, float r) : pos(p), radius(r) {
	}

	WVector pos;    // 0x0
	float radius;   // 0xC
};
CHECK_SIZE(WSphere, 0x10);
CHECK_FIELD(WSphere, pos, 0x0);
CHECK_FIELD(WSphere, radius, 0xC);

class Waabb {
public:
	Waabb() { min.Reset(); max.Reset(); }   // wmath.h:535
	Waabb(const WVector& vmin, const WVector& vmax);   // wmath.h:536

	Waabb& operator+=(const WVector& v);

	// wmath.h:546
	void Reset(void) {
		min.Reset();
		max.Reset();
	}

	void Clear(void);
	void AddPoint(const WVector& v);

	// wmath.h:557
	bool IsInclude(const WVector& pos) const {
		return min.x <= pos.x && pos.x <= max.x
			&& min.y <= pos.y && pos.y <= max.y
			&& min.z <= pos.z && pos.z <= max.z ? true : false;
	}

	// wmath.h:571
	bool IsInclude(const Waabb& b) const {
		return min.x <= b.min.x && b.max.x <= max.x
			&& min.y <= b.min.y && b.max.y <= max.y
			&& min.z <= b.min.z && b.max.z <= max.z ? true : false;
	}

	WVector min;    // 0x0
	WVector max;    // 0xC
};
CHECK_SIZE(Waabb, 0x18);
CHECK_FIELD(Waabb, min, 0x0);
CHECK_FIELD(Waabb, max, 0xC);

// wmath.h:536
SEA_WMATH_INLINE Waabb::Waabb(const WVector& vmin, const WVector& vmax) {
	min = vmin;
	max = vmax;
}

int operator&(const Waabb& a, const Waabb& b);
Waabb operator+(const Waabb& a, const WVector& v);

// An oriented box: its center and its three half-axes.
class Wobb {
public:
	Wobb() {   // wmath.h:617
	}
	Wobb(const Wobb& b);
	Wobb(const WMatrix& m);

	void operator+=(const WVector& v);
	void operator*=(const WVector& v);
	void operator*=(float f);
	Wobb& operator=(const WMatrix& m);
	Wobb& operator=(const Waabb& b);
	bool IsInclude(const WVector& pos) const;
	bool IsIncludeObb(const Wobb& b, const WVector& v) const;
	void Normalize(void);

	WVector center;     // 0x0
	WVector extend[3];  // 0xC
};
CHECK_SIZE(Wobb, 0x30);
CHECK_FIELD(Wobb, center, 0x0);
CHECK_FIELD(Wobb, extend, 0xC);

#include "wmath.inl"
#include "wmathutil.inl"

#endif // PG_CLIENT_WANGREAL_INCLUDE_WMATH_H
