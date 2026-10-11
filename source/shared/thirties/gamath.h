// Original: d:\build\custom\temp\code\shared\thirties\gamath.h
#ifndef PG_SHARED_THIRTIES_GAMATH_H
#define PG_SHARED_THIRTIES_GAMATH_H

#include <math.h>

#include "..\..\defines.h"


// The "thirties" library's scalar helpers (shared with the server). Only
// the members the decompiled units use are transcribed.
struct gaMath {
	static const float ZERO;       // ?ZERO@gaMath@@2MB
	static const float HALF;       // ?HALF@gaMath@@2MB
	static const float ID;         // ?ID@gaMath@@2MB
	static const float TOLERANCE;  // ?TOLERANCE@gaMath@@2MB
	static const float INF;        // ?INF@gaMath@@2MB
	static const float TWO_PI;     // ?TWO_PI@gaMath@@2MB
	static const float TWO;        // ?TWO@gaMath@@2MB
	static const float PI;         // ?PI@gaMath@@2MB
	static const float HALF_PI;    // ?HALF_PI@gaMath@@2MB
	static const float SQRT_HALF;  // ?SQRT_HALF@gaMath@@2MB

	static float Sin(float fValue);
	static float Cos(float fValue);
	static float Sqrt(float fValue);
	static float FAbs(float fValue);
	static bool NearlyZero(float fValue, float fTolerance);

	static float UnitRandom(unsigned int uiSeed = 0);
	static float SymmetricRandom(unsigned int uiSeed = 0);
	static float SymmetricIntervalRandom(float fInterval, unsigned int uiSeed);
	static float SymmetricHalfRandom(unsigned int uiSeed);
	static float IntervalRandom(float fMin, float fMax, unsigned int uiSeed);
	static void UnitRandomArray(float *afArray, int iCount, unsigned int uiSeed);
	static void SymmetricRandomArray(float *afArray, int iCount, unsigned int uiSeed);
};

CHECK_SIZE(gaMath, 0x1);

inline float gaMath::Sin(float fValue) {
	return (float)sin(fValue);
}

inline float gaMath::Cos(float fValue) {
	return (float)cos(fValue);
}

inline float gaMath::Sqrt(float fValue) {
	return (float)sqrt(fValue);
}

// gamath.h:771
inline float gaMath::FAbs(float fValue) {
	return (float)fabs(fValue);
}

// gamath.h:801
inline bool gaMath::NearlyZero(float fValue, float fTolerance) {
	return FAbs(fValue) < fTolerance;
}

// The "thirties" library's 2-vector.
struct gaV2 {
	gaV2(void) {}
	gaV2(float fX, float fY);

	static const gaV2 INF;     // ?INF@gaV2@@2U1@B

	gaV2 &operator-=(const gaV2 &v);
	gaV2 &operator*=(float fScalar);
	gaV2 operator*(float fScalar) const;
	float Length(void) const;
	float Dot(const gaV2 &v) const;
	float Normalize(void);

	static gaV2 UnitRandom(unsigned int uiSeed = 0);
	static void Orthonormalize(gaV2 akV[]);

	float x;    // 0x0
	float y;    // 0x4
};

CHECK_SIZE(gaV2, 0x8);

// gamath.h:833
inline gaV2::gaV2(float fX, float fY) {
	x = fX;
	y = fY;
}

// gamath.h:929
inline gaV2 &gaV2::operator-=(const gaV2 &v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

// gamath.h:936
inline gaV2 &gaV2::operator*=(float fScalar) {
	x *= fScalar;
	y *= fScalar;
	return *this;
}

// gamath.h:975
inline gaV2 gaV2::operator*(float fScalar) const {
	return gaV2(x * fScalar, y * fScalar);
}

// gamath.h:1005
inline float gaV2::Length(void) const {
	return gaMath::Sqrt(x * x + y * y);
}

// gamath.h:1015
inline float gaV2::Dot(const gaV2 &v) const {
	return x * v.x + y * v.y;
}

// gamath.h:1020
inline float gaV2::Normalize(void) {
	float fLength = Length();

	if (fLength < gaMath::TOLERANCE) {
		x = y = 0.0f;
		return gaMath::ZERO;
	}

	*this *= gaMath::ID / fLength;

	return fLength;
}

// The "thirties" library's 3-vector (shared with the server). Only the
// members the decompiled units use are transcribed.
struct gaV3 {
	// gamath.h:213
	gaV3(void) {}
	gaV3(float fX, float fY, float fZ);
	gaV3(const gaV3 &v);

	static const gaV3 ZERO;    // ?ZERO@gaV3@@2U1@B
	static const gaV3 ONE;     // ?ONE@gaV3@@2U1@B
	static const gaV3 MAX;     // ?MAX@gaV3@@2U1@B
	static const gaV3 INF;     // ?INF@gaV3@@2U1@B
	static const gaV3 UNIT_X;  // ?UNIT_X@gaV3@@2U1@B
	static const gaV3 UNIT_Y;  // ?UNIT_Y@gaV3@@2U1@B

	gaV3 &SetZero(void);
	gaV3 &operator-=(const gaV3 &v);
	gaV3 &operator*=(float fScalar);
	gaV3 operator+(const gaV3 &v) const;
	bool operator!=(const gaV3 &v) const;
	float Length(void) const;
	float Dot(const gaV3 &v) const;
	float Normalize(void);
	gaV3 Cross(const gaV3 &v) const;

	static gaV3 UnitRandom(unsigned int uiSeed = 0);
	static void Orthonormalize(gaV3 akV[]);
	static void GenerateOrthonormalBasis(gaV3 &rkU, gaV3 &rkV, gaV3 &rkW, bool bUnitLengthW);
	static void GenerateOrthonormalBasis2(gaV3 &N, gaV3 &T, gaV3 &K, bool bUnitLengthN);

	float x;    // 0x0
	float y;    // 0x4
	float z;    // 0x8
};

CHECK_SIZE(gaV3, 0xc);

inline gaV3::gaV3(float fX, float fY, float fZ) {
	x = fX;
	y = fY;
	z = fZ;
}

// gamath.h:1109
inline gaV3::gaV3(const gaV3 &v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

// gamath.h:1131
inline gaV3 &gaV3::SetZero(void) {
	x = y = z = gaMath::ZERO;
	return *this;
}

// gamath.h:1224
inline gaV3 &gaV3::operator-=(const gaV3 &v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

// gamath.h:1232
inline gaV3 &gaV3::operator*=(float fScalar) {
	x *= fScalar;
	y *= fScalar;
	z *= fScalar;
	return *this;
}

// gamath.h:1263
inline gaV3 gaV3::operator+(const gaV3 &v) const {
	return gaV3(x + v.x, y + v.y, z + v.z);
}

// gamath.h:1287
inline gaV3 operator*(float fScalar, const gaV3 &v) {
	return gaV3(fScalar * v.x, fScalar * v.y, fScalar * v.z);
}

// gamath.h:1297
inline bool gaV3::operator!=(const gaV3 &v) const {
	return x != v.x || y != v.y || z != v.z;
}

// gamath.h:1303
inline float gaV3::Length(void) const {
	return gaMath::Sqrt(x * x + y * y + z * z);
}

// gamath.h:1313
inline float gaV3::Dot(const gaV3 &v) const {
	return z * v.z + y * v.y + x * v.x;
}

// gamath.h:1318
inline float gaV3::Normalize(void) {
	float fLength = Length();

	if (fLength < gaMath::TOLERANCE) {
		SetZero();
		return gaMath::ZERO;
	}

	*this *= gaMath::ID / fLength;

	return fLength;
}

// gamath.h:1359
inline gaV3 gaV3::Cross(const gaV3 &v) const {
	return gaV3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

// The "thirties" library's 3x3 matrix.
struct gaM3 {
	gaM3(void) {}
	gaM3(float fEntry00, float fEntry01, float fEntry02,
		float fEntry10, float fEntry11, float fEntry12,
		float fEntry20, float fEntry21, float fEntry22);

	static const gaM3 ID;      // ?ID@gaM3@@2U1@B

	gaM3 operator*(const gaM3 &rkM) const;

	bool ToEulerAnglesXYZ(float &rfXAngle, float &rfYAngle, float &rfZAngle) const;
	bool ToEulerAnglesXZY(float &rfXAngle, float &rfZAngle, float &rfYAngle) const;
	bool ToEulerAnglesYXZ(float &rfYAngle, float &rfXAngle, float &rfZAngle) const;
	bool ToEulerAnglesYZX(float &rfYAngle, float &rfZAngle, float &rfXAngle) const;
	bool ToEulerAnglesZXY(float &rfZAngle, float &rfXAngle, float &rfYAngle) const;
	bool ToEulerAnglesZYX(float &rfZAngle, float &rfYAngle, float &rfXAngle) const;
	void FromEulerAnglesXYZ(float fYAngle, float fPAngle, float fRAngle);
	void FromEulerAnglesXZY(float fYAngle, float fPAngle, float fRAngle);
	void FromEulerAnglesYXZ(float fYAngle, float fPAngle, float fRAngle);
	void FromEulerAnglesYZX(float fYAngle, float fPAngle, float fRAngle);
	void FromEulerAnglesZXY(float fYAngle, float fPAngle, float fRAngle);
	void FromEulerAnglesZYX(float fYAngle, float fPAngle, float fRAngle);
	void FromAxisAngle(const gaV3 &axis, float radian);

	union {
		float m[3][3];    // 0x0
		struct {
			float m11;    // 0x0
			float m12;    // 0x4
			float m13;    // 0x8
			float m21;    // 0xc
			float m22;    // 0x10
			float m23;    // 0x14
			float m31;    // 0x18
			float m32;    // 0x1c
			float m33;    // 0x20
		};
	};
};

CHECK_SIZE(gaM3, 0x24);

// gamath.h:1403
inline gaM3::gaM3(float fEntry00, float fEntry01, float fEntry02,
	float fEntry10, float fEntry11, float fEntry12,
	float fEntry20, float fEntry21, float fEntry22) {
	m[0][0] = fEntry00; m[0][1] = fEntry01; m[0][2] = fEntry02;
	m[1][0] = fEntry10; m[1][1] = fEntry11; m[1][2] = fEntry12;
	m[2][0] = fEntry20; m[2][1] = fEntry21; m[2][2] = fEntry22;
}

// gamath.h:1582
inline gaM3 gaM3::operator*(const gaM3 &rkM) const {
	gaM3 kProd;
	for (int iRow = 0; iRow < 3; iRow++) {
		for (int iCol = 0; iCol < 3; iCol++) {
			kProd.m[iRow][iCol] =
				m[iRow][0] * rkM.m[0][iCol] +
				m[iRow][1] * rkM.m[1][iCol] +
				m[iRow][2] * rkM.m[2][iCol];
		}
	}
	return kProd;
}

// The "thirties" library's transform: rotation, translation, scale.
struct gaT3 {
	gaT3(void) {}
	gaT3(const gaM3 &kR, const gaV3 &kT, const gaV3 &kS) : R(kR), T(kT), S(kS) {}

	static const gaT3 ID;      // ?ID@gaT3@@2U1@B

	gaM3 R;    // 0x0
	gaV3 T;    // 0x24
	gaV3 S;    // 0x30
};

CHECK_SIZE(gaT3, 0x3c);
CHECK_FIELD(gaT3, T, 0x24);
CHECK_FIELD(gaT3, S, 0x30);

// The "thirties" library's quaternion. Only the members the decompiled
// units use are transcribed.
struct gaQ {
	// gamath.h:543
	gaQ(void) {}
	gaQ(float fX, float fY, float fZ, float fW);
	gaQ(const gaQ &q);
	gaQ(const gaV3 &kAxis, float fAngle);

	static const gaQ ID;       // ?ID@gaQ@@2U1@B

	void FromAxisAngle(const gaV3 &kAxis, float fAngle);
	float Length(void) const;
	float Normalize(void);

	float x;    // 0x0
	float y;    // 0x4
	float z;    // 0x8
	float w;    // 0xc
};

CHECK_SIZE(gaQ, 0x10);
CHECK_FIELD(gaQ, w, 0xc);

// gamath.h:2260
inline gaQ::gaQ(float fX, float fY, float fZ, float fW) {
	x = fX;
	y = fY;
	z = fZ;
	w = fW;
}

// gamath.h:2268
inline gaQ::gaQ(const gaQ &q) {
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
}

// gamath.h:2281
inline gaQ::gaQ(const gaV3 &kAxis, float fAngle) {
	FromAxisAngle(kAxis, fAngle);
}

// gamath.h:2545
inline void gaQ::FromAxisAngle(const gaV3 &kAxis, float fAngle) {
	float fHalfAngle = gaMath::HALF * fAngle;
	float fSin = gaMath::Sin(fHalfAngle);
	x = fSin * kAxis.x;
	y = fSin * kAxis.y;
	z = fSin * kAxis.z;
	w = gaMath::Cos(fHalfAngle);
}

// gamath.h:2610
inline float gaQ::Length(void) const {
	return gaMath::Sqrt(x * x + y * y + z * z + w * w);
}

// gamath.h:2620
inline float gaQ::Normalize(void) {
	float fLength = Length();

	if (fLength > gaMath::TOLERANCE) {
		float fInvLength = gaMath::ID / fLength;
		x *= fInvLength;
		y *= fInvLength;
		z *= fInvLength;
		w *= fInvLength;
	} else {
		fLength = gaMath::ZERO;
		x = gaMath::ZERO;
		y = gaMath::ZERO;
		z = gaMath::ZERO;
		w = gaMath::ZERO;
	}

	return fLength;
}

#endif // PG_SHARED_THIRTIES_GAMATH_H
