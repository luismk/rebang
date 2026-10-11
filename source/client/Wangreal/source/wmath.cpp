// Original: d:\build\custom\temp\code\client\wangreal\source\wmath.cpp
#include "../include/wmath.h"
#include "../include/wutil.h"

void __cdecl InitMath();
void __cdecl UninitMath();

__declspec(align(8)) const WVector4 WVector4::ONE(1.0f, 1.0f, 1.0f, 1.0f);
__declspec(align(8)) const WVector4 WVector4::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

// The WVector constants (the class declares them; wmath.cpp is where the
// original emits their storage). Without these the 3D basis vectors are zero
// and every camera matrix collapses.
const WVector WVector::ONE(1.0f, 1.0f, 1.0f);
const WVector WVector::ZERO(0.0f, 0.0f, 0.0f);
const WVector WVector::UNIT_POS_X(1.0f, 0.0f, 0.0f);
const WVector WVector::UNIT_POS_Y(0.0f, 1.0f, 0.0f);
const WVector WVector::UNIT_POS_Z(0.0f, 0.0f, 1.0f);
const WVector WVector::UNIT_NEG_X(-1.0f, 0.0f, 0.0f);
const WVector WVector::UNIT_NEG_Y(0.0f, -1.0f, 0.0f);
const WVector WVector::UNIT_NEG_Z(0.0f, 0.0f, -1.0f);

const WMatrix WMatrix::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
const WMatrix WMatrix::ZERO(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

const WMatrix4 WMatrix4::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
const WMatrix4 WMatrix4::ZERO(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

// wmath.cpp:58
void WMatrix::Rotate(float angle, char direct) {
	if (direct >= 3)
		*this = RotMat(angle, direct - 3) * *this;
	else
		*this = *this * RotMat(angle, direct);
}

// wmath.cpp:66
void WMatrix::Rotate(const WVector& rot) {
	*this = *this * (RotMat(rot.x, 0) * RotMat(rot.y, 1) * RotMat(rot.z, 2));
}

// wmath.cpp:73
void WMatrix::Normalize(void) {
	xa.Normalize();
	ya.Normalize();
	za.Normalize();
}

// wmath.cpp:83
void WMatrix::GetRotMatrix(WMatrix* m) const {
	WVector v(1.0f, 0.0f, 0.0f);
	*m = *this;

	m->xm = m->ym = m->zm = 0.0f;
	v = v * *m;
	float s = 1.0f / v.Magnitude();

	*m *= s;
}

// wmath.cpp:101
void WMatrix::AxisScale(WVector& scale) {
	Normalize();

	xa.x *= scale.x;
	xa.y *= scale.x;
	xa.z *= scale.x;
	ya.x *= scale.y;
	ya.y *= scale.y;
	ya.z *= scale.y;
	za.x *= scale.z;
	za.y *= scale.z;
	za.z *= scale.z;
}

// wmath.cpp:119
void WQuat::Reset(void) {
	x = y = z = 0.0f;
	w = 1.0f;
}

// wmath.cpp:128
void WQuat::Normalize(void) {
	float inv = 1.0f / sqrtf(x * x + y * y + z * z + w * w);
	x *= inv;
	y *= inv;
	z *= inv;
	w *= inv;
}

// wmath.cpp:140
void WQuat::SetFromAxisAngle(const WVector& axis, float angle) {
	float half = angle * 0.5f; float s = sinf(half);
	x = s * axis.x;
	y = s * axis.y;
	z = s * axis.z;
	w = cosf(half);
}

// wmath.cpp:152
void WQuat::ConvertToAxisAngle(WVector* v, float* fTheta) {
	*fTheta = (float)acos(w) * 2.0f;
	if (Wabs(*fTheta) < g_EPSILON) {
		v->x = 1.0f;
		v->y = 0.0f;
		v->z = 0.0f;
	} else {
		float sinValue = 1.0f / (float)sin(*fTheta * 0.5f);
		v->x = sinValue * x;
		v->y = sinValue * y;
		v->z = sinValue * z;
	}
}

// wmath.cpp:169
void WQuat::ConvertToRotationMatrix(WMatrix& m) const {
	float x2 = x + x, y2 = y + y, z2 = z + z;
	float wx = x2 * w, wy = y2 * w, wz = z2 * w;
	float xx = x2 * x, xy = y2 * x, xz = z2 * x;
	float yy = y2 * y, yz = z2 * y, zz = z2 * z;

	m.xx = 1.0f - (yy + zz); m.yx = xy - wz; m.zx = xz + wy;
	m.xy = xy + wz; m.yy = 1.0f - (xx + zz); m.zy = yz - wx;
	m.xz = xz - wy; m.yz = yz + wx; m.zz = 1.0f - (xx + yy);
	m.xm = m.ym = m.zm = 0.0f;
}

// wmath.cpp:201
void WQuat::SetFromAngles(float fYaw, float fPitch, float fRoll) {
	float fSinYaw = sinf(fYaw * 0.5f);
	float fSinPitch = sinf(fPitch * 0.5f);
	float fSinRoll = sinf(fRoll * 0.5f);
	float fCosYaw = cosf(fYaw * 0.5f);
	float fCosPitch = cosf(fPitch * 0.5f);
	float fCosRoll = cosf(fRoll * 0.5f);

	x = fCosPitch * fCosYaw * fSinRoll - fCosRoll * fSinPitch * fSinYaw;
	y = fCosPitch * fSinRoll * fSinYaw + fCosRoll * fCosYaw * fSinPitch;
	z = fCosRoll * fCosPitch * fSinYaw - fCosYaw * fSinRoll * fSinPitch;
	w = fCosRoll * fCosPitch * fCosYaw + fSinRoll * fSinPitch * fSinYaw;

	Normalize();
}

// wmath.cpp:218
void WQuat::operator=(const WMatrix& matrix) {
	float t = matrix.xx + matrix.yy + matrix.zz;
	float len;
	float s;
	if (t > 0.0f) {
		len = (float)sqrt((float)(t + 1.0f));
		s = 0.5f / len;
		w = len * 0.5f;
		x = (matrix.yz - matrix.zy) * s;
		y = (matrix.zx - matrix.xz) * s;
		z = (matrix.xy - matrix.yx) * s;
		return;
	}

	switch (matrix.xx > matrix.yy ? (matrix.xx > matrix.zz ? 0 : 2)
	                              : (matrix.yy > matrix.zz ? 1 : 2)) {
	case 0:
		len = (float)sqrt(1.0f + matrix.xx - matrix.yy - matrix.zz);
		s = 0.5f / len;
		x = len * 0.5f;
		y = (matrix.xy + matrix.yx) * s;
		z = (matrix.xz + matrix.zx) * s;
		w = (matrix.yz - matrix.zy) * s;
		break;
	case 1:
		len = (float)sqrt(1.0f - matrix.xx + matrix.yy - matrix.zz);
		s = 0.5f / len;
		x = (matrix.xy + matrix.yx) * s;
		y = len * 0.5f;
		z = (matrix.yz + matrix.zy) * s;
		w = (matrix.zx - matrix.xz) * s;
		break;
	case 2:
		len = (float)sqrt(1.0f - matrix.xx - matrix.yy + matrix.zz);
		s = 0.5f / len;
		x = (matrix.xz + matrix.zx) * s;
		y = (matrix.yz + matrix.zy) * s;
		z = len * 0.5f;
		w = (matrix.xy - matrix.yx) * s;
		break;
	}
}

// wmath.cpp:269
void WMatrix::operator=(const WQuat& q) {
	float qxx = q.x * q.x * 2.0f;
	float qyy = q.y * q.y * 2.0f;
	float qzz = q.z * q.z * 2.0f;
	float qxy = q.x * q.y * 2.0f;
	float qxz = q.z * q.x * 2.0f;
	float qyz = q.z * q.y * 2.0f;
	float qwx = q.x * q.w * 2.0f;
	float qwy = q.w * q.y * 2.0f;
	float qwz = q.z * q.w * 2.0f;

	xx = 1.0f - (qzz + qyy);
	yx = qxy - qwz;
	zx = qwy + qxz;

	xy = qwz + qxy;
	yy = 1.0f - (qzz + qxx);
	zy = qyz - qwx;

	xz = qxz - qwy;
	yz = qwx + qyz;
	zz = 1.0f - (qyy + qxx);
}

// wmath.cpp:376
// Sweeps box by vel against target: the fraction of vel that can be moved
// before touching it (1 = no contact), and the contact plane's normal.
float WCollisionTest(const Waabb& origin, const WVector& vec, const Waabb& target, WPlane* plane) {
	Waabb area(origin.min, origin.max);
	WVector gap;
	area += vec;
	if (!(area & target) || WisEqual(vec, WVector::ZERO, g_EPSILON))
		return 1.0f;

	int direct = 3;
	float t = 1.0f;
	float t2;
	for (int i = 0; i < 3; i++) {
		if (Wabs(vec.p[i]) < g_EPSILON)
			continue;
		if (vec.p[i] > 0.0f)
			t2 = 1.0f - (area.max.p[i] - target.min.p[i]) / vec.p[i];
		else
			t2 = 1.0f - (area.min.p[i] - target.max.p[i]) / vec.p[i];
		if (direct == 3 || t < t2) {
			t = t2;
			direct = i;
		}
	}

	if (direct >= 3 || t >= 1.0f)
		return 1.0f;
	if (Wabs(t) < g_EPSILON)
		return 0.0f;

	gap = vec * t;
	if (WisEqual(WCollisionTest(origin + gap, vec * (1.0f - t), target, 0), 1.0f, g_EPSILON))
		return 1.0f;

	if (plane) {
		plane->normal = WVector::ZERO;
		plane->normal.p[direct] = vec.p[direct] > 0.0f ? -1.0f : 1.0f;
	}

	return t;
}

// wmath.cpp:446
WVector MinPointLineSegment(WVector& vp, WVector& va, WVector& vb, WVector* vt) {
	WVector vm = vb - va;
	WVector vdiff = vp - va;
	float t = vdiff * vm;
	float mSqr;

	if (t <= 0.0f) {
		*vt = va;
	} else if (t >= (mSqr = vm.SquareMagnitude())) {
		vdiff = vdiff - vm;
		*vt = vb;
	} else {
		t /= mSqr;
		*vt = va + vm * t;
		vdiff = vp - *vt;
	}

	return vdiff;
}

// wmath.cpp:541
void __cdecl InitMath() {
}

// wmath.cpp:566
void __cdecl UninitMath() {
}

// wmath.cpp:575
float __cdecl CalcDeltaAngle(const WVector& v1, const WVector& v2) {
	WVector va, vb; va = WVector(v1.x, 0.0f, v1.z); vb = WVector(v2.x, 0.0f, v2.z);
	float la = va.Magnitude();
	float lb = vb.Magnitude();

	if (Wabs(la) < g_EPSILON || Wabs(lb) < g_EPSILON)
		return 0.0f;

	float ang = acosf(Between<float>(-1.0f, va * vb / (la * lb), 1.0f));

	return (v2.x * v1.z - v2.z * v1.x > 0.0f) ? ang : -ang;
}
