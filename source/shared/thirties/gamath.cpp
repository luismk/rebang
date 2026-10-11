// Original: d:\build\custom\temp\code\shared\thirties\gamath.cpp
#include <stdlib.h>

#include "gamath.h"

// A single-precision positive infinity (0x7f800000), stored as little-endian
// bytes; the original's static is named `_inf` (its relocation is `__inf`).
// The original reads it at run time ($E1: a 4-byte load into INF): cl 13.10
// folds the read when the static is a 4-byte scalar or a 4-byte element
// array, not through a byte array.
static unsigned char _inf[4] = { 0x00, 0x00, 0x80, 0x7f };

// gamath.cpp:29
const float gaMath::INF = *(float *)_inf;

// gamath.cpp:42
float gaMath::UnitRandom(unsigned int uiSeed) {
	if (uiSeed > 0)
		srand(uiSeed);

	return rand() * (1.0f / RAND_MAX);
}

// gamath.cpp:50
float gaMath::SymmetricRandom(unsigned int uiSeed) {
	if (uiSeed > 0)
		srand(uiSeed);

	return rand() * (2.0f / RAND_MAX) - ID;
}

// gamath.cpp:58
float gaMath::SymmetricIntervalRandom(float fInterval, unsigned int uiSeed) {
	if (uiSeed > 0)
		srand(uiSeed);

	return rand() * fInterval * (2.0f / RAND_MAX) - fInterval;
}

// gamath.cpp:66
float gaMath::SymmetricHalfRandom(unsigned int uiSeed) {
	if (uiSeed > 0)
		srand(uiSeed);

	return rand() * (1.0f / RAND_MAX) - HALF;
}

// gamath.cpp:74
float gaMath::IntervalRandom(float fMin, float fMax, unsigned int uiSeed) {
	if (uiSeed > 0)
		srand(uiSeed);

	return rand() * (fMax - fMin) * (1.0f / RAND_MAX) + fMin;
}

// gamath.cpp:82
void gaMath::UnitRandomArray(float *afArray, int iCount, unsigned int uiSeed) {
	if (uiSeed > 0)
		srand(uiSeed);

	for (int i = 0; i < iCount; i++)
		afArray[i] = rand() * (1.0f / RAND_MAX);
}

// gamath.cpp:93
void gaMath::SymmetricRandomArray(float *afArray, int iCount, unsigned int uiSeed) {
	if (uiSeed > 0)
		srand(uiSeed);

	for (int i = 0; i < iCount; i++)
		afArray[i] = rand() * (2.0f / RAND_MAX) - ID;
}

// gamath.cpp:111
const gaV2 gaV2::INF(gaMath::INF, gaMath::INF);

// gamath.cpp:116
gaV2 gaV2::UnitRandom(unsigned int uiSeed) {
	float fX = gaMath::UnitRandom(uiSeed);
	float fY = gaMath::UnitRandom();
	return gaV2(fX, fY);
}

// gamath.cpp:124
void gaV2::Orthonormalize(gaV2 akV[]) {
	// u0 = v0 / |v0|
	akV[0].Normalize();

	// u1 = (v1 - (u0 . v1) u0) / |v1 - (u0 . v1) u0|
	float fDot0 = akV[0].Dot(akV[1]);
	akV[1] -= akV[0] * fDot0;
	akV[1].Normalize();
}

// gamath.cpp:152
const gaV3 gaV3::INF(gaMath::INF, gaMath::INF, gaMath::INF);

// gamath.cpp:157
gaV3 gaV3::UnitRandom(unsigned int uiSeed) {
	float fZ = gaMath::SymmetricRandom(uiSeed);
	float fR = gaMath::Sqrt(gaMath::ID - fZ * fZ);
	float fAngle = gaMath::UnitRandom() * gaMath::TWO_PI;
	return gaV3(fR * gaMath::Cos(fAngle), fR * gaMath::Sin(fAngle), fZ);
}

// gamath.cpp:165
void gaV3::Orthonormalize(gaV3 akV[]) {
	// u0 = v0 / |v0|
	akV[0].Normalize();

	// u1 = (v1 - (u0 . v1) u0) / |v1 - (u0 . v1) u0|
	float fDot0 = akV[0].Dot(akV[1]);
	akV[1] -= fDot0 * akV[0];
	akV[1].Normalize();

	// u2 = (v2 - (u0 . v2) u0 - (u1 . v2) u1) / |v2 - (u0 . v2) u0 - (u1 . v2) u1|
	float fDot1 = akV[1].Dot(akV[2]);
	fDot0 = akV[0].Dot(akV[2]);
	akV[2] -= fDot0 * akV[0] + fDot1 * akV[1];
	akV[2].Normalize();
}

// gamath.cpp:192
void gaV3::GenerateOrthonormalBasis(gaV3 &rkU, gaV3 &rkV, gaV3 &rkW, bool bUnitLengthW) {
	if (!bUnitLengthW)
		rkW.Normalize();

	float fInvLength;

	if (gaMath::FAbs(rkW.x) >= gaMath::FAbs(rkW.y)) {
		fInvLength = gaMath::ID / gaMath::Sqrt(rkW.x * rkW.x + rkW.z * rkW.z);
		rkU.x = -rkW.z * fInvLength;
		rkU.y = 0.0f;
		rkU.z = rkW.x * fInvLength;
	} else {
		fInvLength = gaMath::ID / gaMath::Sqrt(rkW.y * rkW.y + rkW.z * rkW.z);
		rkU.x = 0.0f;
		rkU.y = rkW.z * fInvLength;
		rkU.z = -rkW.y * fInvLength;
	}

	rkV = rkW.Cross(rkU);
}

// gamath.cpp:219
void gaV3::GenerateOrthonormalBasis2(gaV3 &N, gaV3 &T, gaV3 &K, bool bUnitLengthN) {
	if (!bUnitLengthN) {
		N.Normalize();
	}

	if (gaMath::FAbs(N.z) > gaMath::SQRT_HALF) {
		float a = N.y * N.y + N.z * N.z;
		float k = gaMath::ID / gaMath::Sqrt(a);
		T.x = 0.0f;
		T.y = -N.z * k;
		T.z = N.y * k;

		K.x = a * k;
		K.y = -N.x * T.z;
		K.z = N.x * T.y;
	} else {
		float a = N.x * N.x + N.y * N.y;
		float k = gaMath::ID / gaMath::Sqrt(a);
		T.x = -N.y * k;
		T.y = N.x * k;
		T.z = 0.0f;

		K.x = -N.z * T.y;
		K.y = N.z * T.x;
		K.z = a * k;
	}
}

// gamath.cpp:465
void gaM3::FromAxisAngle(const gaV3 &axis, float radian) {
	float fCos = gaMath::Cos(radian);
	float fSin = gaMath::Sin(radian);
	float fOneMinusCos = gaMath::ID - fCos;
	float fX2 = axis.x * axis.x;
	float fY2 = axis.y * axis.y;
	float fZ2 = axis.z * axis.z;
	float fXYM = axis.x * axis.y * fOneMinusCos;
	float fXZM = axis.x * axis.z * fOneMinusCos;
	float fYZM = axis.y * axis.z * fOneMinusCos;
	float fXSin = axis.x * fSin;
	float fYSin = axis.y * fSin;
	float fZSin = axis.z * fSin;

	m[0][0] = fX2 * fOneMinusCos + fCos;
	m[0][1] = fXYM - fZSin;
	m[0][2] = fXZM + fYSin;
	m[1][0] = fXYM + fZSin;
	m[1][1] = fY2 * fOneMinusCos + fCos;
	m[1][2] = fYZM - fXSin;
	m[2][0] = fXZM - fYSin;
	m[2][1] = fYZM + fXSin;
	m[2][2] = fZ2 * fOneMinusCos + fCos;
}

// gamath.cpp:491
bool gaM3::ToEulerAnglesXYZ(float &rfXAngle, float &rfYAngle, float &rfZAngle) const {
	if (m[0][2] < gaMath::ID) {
		if (m[0][2] > -1.0f) {
			rfXAngle = atan2(-m[1][2], m[2][2]);
			rfYAngle = asin(m[0][2]);
			rfZAngle = atan2(-m[0][1], m[0][0]);
			return true;
		} else {
			// Not unique.
			rfXAngle = -atan2(m[1][0], m[1][1]);
			rfYAngle = -gaMath::HALF_PI;
			rfZAngle = 0.0f;
			return false;
		}
	} else {
		// Not unique.
		rfXAngle = atan2(m[1][0], m[1][1]);
		rfYAngle = gaMath::HALF_PI;
		rfZAngle = 0.0f;
		return false;
	}
}

// gamath.cpp:525
bool gaM3::ToEulerAnglesXZY(float &rfXAngle, float &rfZAngle, float &rfYAngle) const {
	if (m[0][1] < gaMath::ID) {
		if (m[0][1] > -1.0f) {
			rfXAngle = atan2(m[2][1], m[1][1]);
			rfZAngle = asin(-m[0][1]);
			rfYAngle = atan2(m[0][2], m[0][0]);
			return true;
		} else {
			// Not unique.
			rfXAngle = atan2(m[2][0], m[2][2]);
			rfZAngle = gaMath::HALF_PI;
			rfYAngle = 0.0f;
			return false;
		}
	} else {
		// Not unique.
		rfXAngle = atan2(-m[2][0], m[2][2]);
		rfZAngle = -gaMath::HALF_PI;
		rfYAngle = 0.0f;
		return false;
	}
}

// gamath.cpp:559
bool gaM3::ToEulerAnglesYXZ(float &rfYAngle, float &rfXAngle, float &rfZAngle) const {
	if (m[1][2] < gaMath::ID) {
		if (m[1][2] > -1.0f) {
			rfYAngle = atan2(m[0][2], m[2][2]);
			rfXAngle = asin(-m[1][2]);
			rfZAngle = atan2(m[1][0], m[1][1]);
			return true;
		} else {
			// Not unique.
			rfYAngle = atan2(m[0][1], m[0][0]);
			rfXAngle = gaMath::HALF_PI;
			rfZAngle = 0.0f;
			return false;
		}
	} else {
		// Not unique.
		rfYAngle = atan2(-m[0][1], m[0][0]);
		rfXAngle = -gaMath::HALF_PI;
		rfZAngle = 0.0f;
		return false;
	}
}

// gamath.cpp:593
bool gaM3::ToEulerAnglesYZX(float &rfYAngle, float &rfZAngle, float &rfXAngle) const {
	if (m[1][0] < gaMath::ID) {
		if (m[1][0] > -1.0f) {
			rfYAngle = atan2(-m[2][0], m[0][0]);
			rfZAngle = asin(m[1][0]);
			rfXAngle = atan2(-m[1][2], m[1][1]);
			return true;
		} else {
			// Not unique.
			rfYAngle = -atan2(m[2][1], m[2][2]);
			rfZAngle = -gaMath::HALF_PI;
			rfXAngle = 0.0f;
			return false;
		}
	} else {
		// Not unique.
		rfYAngle = atan2(m[2][1], m[2][2]);
		rfZAngle = gaMath::HALF_PI;
		rfXAngle = 0.0f;
		return false;
	}
}

// gamath.cpp:627
bool gaM3::ToEulerAnglesZXY(float &rfZAngle, float &rfXAngle, float &rfYAngle) const {
	if (m[2][1] < gaMath::ID) {
		if (m[2][1] > -1.0f) {
			rfZAngle = atan2(-m[0][1], m[1][1]);
			rfXAngle = asin(m[2][1]);
			rfYAngle = atan2(-m[2][0], m[2][2]);
			return true;
		} else {
			// Not unique.
			rfZAngle = -atan2(m[0][2], m[0][0]);
			rfXAngle = -gaMath::HALF_PI;
			rfYAngle = 0.0f;
			return false;
		}
	} else {
		// Not unique.
		rfZAngle = atan2(m[0][2], m[0][0]);
		rfXAngle = gaMath::HALF_PI;
		rfYAngle = 0.0f;
		return false;
	}
}

// gamath.cpp:661
bool gaM3::ToEulerAnglesZYX(float &rfZAngle, float &rfYAngle, float &rfXAngle) const {
	if (m[2][0] < gaMath::ID) {
		if (m[2][0] > -1.0f) {
			rfZAngle = atan2(m[1][0], m[0][0]);
			rfYAngle = asin(-m[2][0]);
			rfXAngle = atan2(m[2][1], m[2][2]);
			return true;
		} else {
			// Not unique.
			rfZAngle = -atan2(m[0][1], m[0][2]);
			rfYAngle = gaMath::HALF_PI;
			rfXAngle = 0.0f;
			return false;
		}
	} else {
		// Not unique.
		rfZAngle = atan2(-m[0][1], -m[0][2]);
		rfYAngle = -gaMath::HALF_PI;
		rfXAngle = 0.0f;
		return false;
	}
}

// gamath.cpp:695
void gaM3::FromEulerAnglesXYZ(float fYAngle, float fPAngle, float fRAngle) {
	float fCos, fSin;

	fCos = gaMath::Cos(fYAngle);
	fSin = gaMath::Sin(fYAngle);
	gaM3 kXMat(1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos);

	fCos = gaMath::Cos(fPAngle);
	fSin = gaMath::Sin(fPAngle);
	gaM3 kYMat(fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos);

	fCos = gaMath::Cos(fRAngle);
	fSin = gaMath::Sin(fRAngle);
	gaM3 kZMat(fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f);

	*this = kXMat * (kYMat * kZMat);
}

// gamath.cpp:714
void gaM3::FromEulerAnglesXZY(float fYAngle, float fPAngle, float fRAngle) {
	float fCos, fSin;

	fCos = gaMath::Cos(fYAngle);
	fSin = gaMath::Sin(fYAngle);
	gaM3 kXMat(1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos);

	fCos = gaMath::Cos(fPAngle);
	fSin = gaMath::Sin(fPAngle);
	gaM3 kZMat(fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f);

	fCos = gaMath::Cos(fRAngle);
	fSin = gaMath::Sin(fRAngle);
	gaM3 kYMat(fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos);

	*this = kXMat * (kZMat * kYMat);
}

// gamath.cpp:733
void gaM3::FromEulerAnglesYXZ(float fYAngle, float fPAngle, float fRAngle) {
	float fCos, fSin;

	fCos = gaMath::Cos(fYAngle);
	fSin = gaMath::Sin(fYAngle);
	gaM3 kYMat(fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos);

	fCos = gaMath::Cos(fPAngle);
	fSin = gaMath::Sin(fPAngle);
	gaM3 kXMat(1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos);

	fCos = gaMath::Cos(fRAngle);
	fSin = gaMath::Sin(fRAngle);
	gaM3 kZMat(fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f);

	*this = kYMat * (kXMat * kZMat);
}

// gamath.cpp:752
void gaM3::FromEulerAnglesYZX(float fYAngle, float fPAngle, float fRAngle) {
	float fCos, fSin;

	fCos = gaMath::Cos(fYAngle);
	fSin = gaMath::Sin(fYAngle);
	gaM3 kYMat(fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos);

	fCos = gaMath::Cos(fPAngle);
	fSin = gaMath::Sin(fPAngle);
	gaM3 kZMat(fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f);

	fCos = gaMath::Cos(fRAngle);
	fSin = gaMath::Sin(fRAngle);
	gaM3 kXMat(1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos);

	*this = kYMat * (kZMat * kXMat);
}

// gamath.cpp:771
void gaM3::FromEulerAnglesZXY(float fYAngle, float fPAngle, float fRAngle) {
	float fCos, fSin;

	fCos = gaMath::Cos(fYAngle);
	fSin = gaMath::Sin(fYAngle);
	gaM3 kZMat(fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f);

	fCos = gaMath::Cos(fPAngle);
	fSin = gaMath::Sin(fPAngle);
	gaM3 kXMat(1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos);

	fCos = gaMath::Cos(fRAngle);
	fSin = gaMath::Sin(fRAngle);
	gaM3 kYMat(fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos);

	*this = kZMat * (kXMat * kYMat);
}

// gamath.cpp:790
void gaM3::FromEulerAnglesZYX(float fYAngle, float fPAngle, float fRAngle) {
	float fCos, fSin;

	fCos = gaMath::Cos(fYAngle);
	fSin = gaMath::Sin(fYAngle);
	gaM3 kZMat(fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f);

	fCos = gaMath::Cos(fPAngle);
	fSin = gaMath::Sin(fPAngle);
	gaM3 kYMat(fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos);

	fCos = gaMath::Cos(fRAngle);
	fSin = gaMath::Sin(fRAngle);
	gaM3 kXMat(1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos);

	*this = kZMat * (kYMat * kXMat);
}

// gamath.cpp:811
const gaT3 gaT3::ID(gaM3::ID, gaV3::ZERO, gaV3::ONE);
