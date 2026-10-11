// Original: d:\build\custom\temp\code\client\wangreal\include\wmath.inl
// Inline members of the wmath.h types; included at the end of wmath.h.

// Wabs (wmath.h) under the name its callers used before the PDB name was
// recovered; same body.
inline float WAbsBySign(const float& f) {
	return (*(const unsigned long*)&f & 0x80000000) ? -f : f;
}

// g_EPSILON as a literal, for callers written before wmath.h carried the
// const.
#define WMATH_EPSILON 1e-5f

// wmath.inl:12
GOLFDOC_WMATH_INLINE int WisEqual(const WVector& a, const WVector& b, float eps) {
	if (WisEqual(a.x, b.x, eps) && WisEqual(a.y, b.y, eps) && WisEqual(a.z, b.z, eps))
		return 1;
	else
		return 0;
}

// wmath.inl:30
inline WVector WVector::operator-(void) const {
	return WVector(-x, -y, -z);
}

// wmath.inl:35
inline WVector operator+(const WVector& a, const WVector& b) {
	return WVector(a.x + b.x, a.y + b.y, a.z + b.z);
}

// wmath.inl:44
inline WVector operator-(const WVector& a, const WVector& b) {
	return WVector(a.x - b.x, a.y - b.y, a.z - b.z);
}

// wmath.inl:53
GOLFDOC_WMATH_INLINE WVector operator*(const WVector& v, float f) {
	return WVector(v.x * f, v.y * f, v.z * f);
}

// wmath.inl:62
inline WVector operator/(const WVector& v, float f) {
	return v * (1.0f / f);
}

// wmath.inl:67
inline WVector operator*(float f, const WVector& v) {
	return v * f;
}

// wmath.inl:82
// Same body as WDotProduct: the out-of-line copy in SharedDoc is byte for
// byte camerawork's WDotProduct.
inline float operator*(const WVector& a, const WVector& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

// wmath.inl:93
CAMERAWORK_WMATH_INLINE float WDotProduct(const WVector& a, const WVector& b) {
	return a.z * b.z + a.y * b.y + a.x * b.x;
}

// wmath.inl:325
CAMERAWORK_WMATH_INLINE float WDotProduct(const WQuat& a, const WQuat& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// wmath.inl:98
inline WVector WCrossProduct(const WVector& a, const WVector& b) {
	return WVector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

// wmath.inl:48
inline float WVectorLen(const WVector& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

// wmath.inl:328 (emitted out of line in GolfRule)
GOLFRULE_WMATH_INLINE WVector RotVec(const WVector& v, const WMatrix& m) {
	return WVector(v.x * m.xx + v.y * m.xy + v.z * m.xz,
		v.x * m.yx + v.y * m.yy + v.z * m.yz,
		v.x * m.zx + v.y * m.zy + v.z * m.zz);
}

// wmath.inl:118
inline WVector& WVector::operator=(const WVector& v) {
	memcpy(this, &v, sizeof(WVector));
	return *this;
}

// wmath.inl:124
inline void WVector::operator+=(const WVector& v) {
	x = v.x + x;
	y = v.y + y;
	z = v.z + z;
}

// wmath.inl:135
inline void WVector::operator-=(const WVector& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

// wmath.inl:146
inline void WVector::operator*=(const float f) {
	x *= f;
	y *= f;
	z *= f;
}

// wmath.inl:157
inline void WVector::operator/=(const float f) {
	*this *= 1.0f / f;
}

// wmath.inl:162
inline void WVector::Reset(void) {
	x = y = z = 0.0f;
}

// wmath.inl:167
AVATAR_AVATAR_WMATH_INLINE float WVector::SquareMagnitude(void) const {
	return x * x + y * y + z * z;
}

// wmath.inl:172
inline float WVector::Magnitude(void) const {
	return WVectorLen(*this);
}

// wmath.inl:177
GOLFDOC_WMATH_INLINE WVector& WVector::Normalize(void) {
	if (WisEqual(*this, ZERO, g_EPSILON))
		*this *= 0.0f;
	else
		*this *= (float)((1.0f / (Magnitude())));
	return *this;
}

// wmath.inl:186
inline void WVector::Negate(void) {
	x = -x; y = -y; z = -z;
}

// wmath.inl:198
GOLFDOC_WMATH_INLINE WVector operator*(const WVector& v, const WMatrix& m) {
	return WVector(v.x * m.xx + v.y * m.xy + v.z * m.xz + m.xm,
	               v.x * m.yx + v.y * m.yy + v.z * m.yz + m.ym,
	               v.x * m.zx + v.y * m.zy + v.z * m.zz + m.zm);
}

// wmath.inl:213
inline WMatrix operator*(const WMatrix& a, const WMatrix& b) {
	WMatrix m;

	m.xx = a.xx * b.xx + a.yx * b.xy + a.zx * b.xz;
	m.yx = a.xx * b.yx + a.yx * b.yy + a.zx * b.yz;
	m.zx = a.xx * b.zx + a.yx * b.zy + a.zx * b.zz;

	m.xy = a.xy * b.xx + a.yy * b.xy + a.zy * b.xz;
	m.yy = a.xy * b.yx + a.yy * b.yy + a.zy * b.yz;
	m.zy = a.xy * b.zx + a.yy * b.zy + a.zy * b.zz;

	m.xz = a.xz * b.xx + a.yz * b.xy + a.zz * b.xz;
	m.yz = a.xz * b.yx + a.yz * b.yy + a.zz * b.yz;
	m.zz = a.xz * b.zx + a.yz * b.zy + a.zz * b.zz;

	m.pivot = a.pivot * b; return m;
}

// wmath.inl:286
// A rotation by angles.x about x, then angles.y about y, then angles.z
// about z.
inline WMatrix RotMat(WVector angles) {
	return RotMat(angles.x, 0) * RotMat(angles.y, 1) * RotMat(angles.z, 2);
}

// wmath.inl:293
// A rotation of angle radians about axis 0 (x), 1 (y) or 2 (z), no translation.
inline WMatrix RotMat(float angle, char axis) {
	WMatrix m;
	float c = cosf(angle);
	float s = sinf(angle);
	switch (axis) {
	case 0:
		m.xx = 1.0f; m.yx = 0.0f; m.zx = 0.0f; m.xy = 0.0f; m.yy = c; m.zy = -s; m.xz = 0.0f; m.yz = s; m.zz = c;
		break;
	case 2:
		m.xx = c; m.yx = s; m.zx = 0.0f; m.xy = -s; m.yy = c; m.zy = 0.0f; m.xz = 0.0f; m.yz = 0.0f; m.zz = 1.0f;
		break;
	default:
		m.xx = c; m.yx = 0.0f; m.zx = s; m.xy = 0.0f; m.yy = 1.0f; m.zy = 0.0f; m.xz = -s; m.yz = 0.0f; m.zz = c;
		break;
	}
	m.xm = m.ym = m.zm = 0.0f;
	return m;
}

// wmath.inl:309
// A rotation of angle radians about axis (normalized here), no translation.
CAMERAWORK_WMATH_INLINE WMatrix RotMat(const WVector& axis, float angle) {
	WMatrix m;
	float c = cosf(angle);
	WVector a = axis;
	float s = sinf(angle);
	a.Normalize();
	float t = 1.0f - c;
	m.xx = a.x * a.x * t + c;
	m.yx = a.x * a.y * t - a.z * s;
	m.zx = a.x * a.z * t + a.y * s;
	m.xy = a.x * a.y * t + a.z * s;
	m.yy = a.y * a.y * t + c;
	m.zy = a.y * a.z * t - a.x * s;
	m.xz = a.x * a.z * t - a.y * s;
	m.yz = a.y * a.z * t + a.x * s;
	m.zz = a.z * a.z * t + c;
	m.xm = m.ym = m.zm = 0.0f;
	return m;
}

// wmath.inl:343
WVIEW_WMATH_INLINE float TransformZ(float x, float y, float z, const WMatrix& m) {
	return x * m.zx + y * m.zy + z * m.zz + m.zm;
}

// wmath.inl:348
inline float TransformZ(const WVector& v, const WMatrix& m) {
	return (v.z * m.zz) + (v.x * m.zx) + (v.y * m.zy) + m.zm;
}

// wmath.inl:377
inline WMatrix& WMatrix::operator=(const WMatrix& m) {
	memcpy(&xa, &m.xa, sizeof(WVector));
	memcpy(&ya, &m.ya, sizeof(WVector));
	memcpy(&za, &m.za, sizeof(WVector));
	memcpy(&pivot, &m.pivot, sizeof(WVector));
	return *this;
}

// wmath.inl:386
inline void WMatrix::operator*=(const float f) {
	xa *= f;
	ya *= f;
	za *= f;
}

// wmath.inl:441
GOLFDOC_WMATH_INLINE WMatrix::WMatrix(float _xx, float _yx, float _zx, float _xy, float _yy, float _zy,
                                      float _xz, float _yz, float _zz) {
	xx = _xx; yx = _yx; zx = _zx;
	xy = _xy; yy = _yy; zy = _zy;
	xz = _xz; yz = _yz; zz = _zz;
	pivot.Reset();
}

// wmath.inl:455
POOL_WMATH_INLINE WMatrix::WMatrix(const WPlane &plane)
{
	WVector n;
	n.x = plane.x;
	n.y = plane.y;
	n.z = plane.z;
	float d = plane.dis;

	float inv = 1.0f / sqrtf(n.z * n.z + n.y * n.y + n.x * n.x);
	n.x *= inv;
	n.y *= inv;
	n.z *= inv;
	d *= inv;

	xx = 1.0f - 2.0f * n.x * n.x;
	yx = n.y * n.x * -2.0f;
	zx = n.z * n.x * -2.0f;
	xy = yx;
	yy = 1.0f - 2.0f * n.y * n.y;
	zy = n.z * n.y * -2.0f;
	xz = zx;
	yz = zy;
	zz = 1.0f - 2.0f * n.z * n.z;
	xm = n.x * d * -2.0f;
	ym = n.y * d * -2.0f;
	zm = n.z * d * -2.0f;
}

// wmath.inl:475
inline void WMatrix::Reset(void) {
	xx = yy = zz = 1.0f;
	yx = zx = xy = zy = xz = yz = xm = ym = zm = 0.0f;
}

// wmath.inl:481
WBONE_WMATH_INLINE bool operator==(const WMatrix& mat1, const WMatrix& mat2) {
	return WisEqual(mat1.xa, mat2.xa, WMATH_EPSILON) && WisEqual(mat1.ya, mat2.ya, WMATH_EPSILON)
		&& WisEqual(mat1.za, mat2.za, WMATH_EPSILON) && WisEqual(mat1.pivot, mat2.pivot, WMATH_EPSILON);
}

// wmath.inl:489
WBONE_WMATH_INLINE bool operator!=(const WMatrix& mat1, const WMatrix& mat2) {
	return !(mat1 == mat2);
}

// wmath.inl:510
POOL_WMATH_INLINE WMatrix4 operator*(const WMatrix4& a, const WMatrix4& b) {
	WMatrix4 m;

	m.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0];
	m.m[0][1] = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1];
	m.m[0][2] = a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2];
	m.m[0][3] = a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + a.m[0][2] * b.m[2][3] + a.m[0][3] * b.m[3][3];

	m.m[1][0] = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0];
	m.m[1][1] = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1];
	m.m[1][2] = a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2];
	m.m[1][3] = a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + a.m[1][2] * b.m[2][3] + a.m[1][3] * b.m[3][3];

	m.m[2][0] = a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0];
	m.m[2][1] = a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1];
	m.m[2][2] = a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2];
	m.m[2][3] = a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + a.m[2][2] * b.m[2][3] + a.m[2][3] * b.m[3][3];

	m.m[3][0] = a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0];
	m.m[3][1] = a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1];
	m.m[3][2] = a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2];
	m.m[3][3] = a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + a.m[3][2] * b.m[2][3] + a.m[3][3] * b.m[3][3];

	return m;
}

// wmath.inl:537
WVIEW_WMATH_INLINE void SetWMatrix4FromWMatrix(WMatrix4& m4, const WMatrix& m) {
	m4.xa = m.xa; m4.wx = 0.0f;
	m4.ya = m.ya; m4.wy = 0.0f;
	m4.za = m.za; m4.wz = 0.0f;
	m4.pivot = m.pivot; m4.wm = 1.0f;
}

// wmath.inl:548
WVIEW_WMATH_INLINE WMatrix4::WMatrix4(float a00, float a01, float a02, float a03, float a10, float a11, float a12, float a13,
                                      float a20, float a21, float a22, float a23, float a30, float a31, float a32, float a33) {
	p[0] = a00;
	p[1] = a01;
	p[2] = a02;
	p[3] = a03;
	p[4] = a10;
	p[5] = a11;
	p[6] = a12;
	p[7] = a13;
	p[8] = a20;
	p[9] = a21;
	p[10] = a22;
	p[11] = a23;
	p[12] = a30;
	p[13] = a31;
	p[14] = a32;
	p[15] = a33;
}

// wmath.inl:568
WVIEW_WMATH_INLINE WMatrix4& WMatrix4::operator=(const WMatrix4& m) {
	memcpy(this, &m, sizeof(WMatrix4));
	return *this;
}

// wmath.inl:592
inline WVector2D operator*(const WVector2D& v, float f) {
	return WVector2D(v.x * f, v.y * f);
}

// wmath.inl:618
inline void WVector2D::Reset(void) {
	x = y = 0.0f;
}

// wmath.inl:642
inline WVector2D& WVector2D::operator=(const WVector2D& v) {
	x = v.x;
	y = v.y;
	return *this;
}

// wmath.inl:651
inline void WVector2D::operator+=(const WVector2D& v) {
	x += v.x;
	y += v.y;
}

// wmath.inl:665
inline void WVector2D::operator*=(const float f) {
	x *= f;
	y *= f;
}

// wmath.inl:672
inline void WVector2D::operator/=(const float f) {
	float inv = 1.0f / f; x *= inv;
	y *= inv;
}

// wmath.inl:679
inline WVector2D& WVector2D::Normalize(void) {
	if (WAbsBySign(x) < g_EPSILON && WAbsBySign(y) < g_EPSILON) {
		x *= 0.0f; y *= 0.0f;
	} else {
		float inv = 1.0f / sqrtf(x * x + y * y);
		x *= inv; y *= inv;
	}
	return *this;
}

// wmath.inl:692
inline WVector4::WVector4(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w) {
}

// wmath.inl:714
inline float operator*(const WPlane& p, const WVector& v) {
	return p.normal * v + p.dis;
}

// wmath.inl:725
inline float operator*(const WVector& v, const WPlane& p) {
	const WVector& value = v;
	return p.normal * value + p.dis;
}

// wmath.inl:745
WVIEW_WMATH_INLINE WPlane::WPlane(const WVector& n, const WVector& p) : normal(n) {
	dis = -(normal * p);
}

// wmath.inl:751
AVATAR_AVATAR_WMATH_INLINE WPlane& WPlane::operator=(const WPlane& p) {
	normal = p.normal;
	dis = p.dis;
	return *this;
}

// wmath.inl:786
// Spherical interpolation from `from` (t = 0) to `to` (t = 1) along the
// shorter arc; linear when the two are nearly equal, through a
// perpendicular quaternion when they are nearly opposite.
CAMERAWORK_WMATH_INLINE WQuat WQuaternionSlerp(const WQuat& from, const WQuat& to, float t) {
	WQuat q;
	float omega, cosom, sinom, scale0, scale1;

	cosom = from.x * to.x + from.z * to.z + from.y * to.y + from.w * to.w;

	if (cosom < 0.0f) {
		q = WQuat(-from.x, -from.y, -from.z, -from.w);
		cosom = -cosom;
	} else {
		q = from;
	}

	if ((1.0f + cosom) > 0.05f) {
		if ((1.0f - cosom) < 0.05f) {
			scale0 = 1.0f - t;
			scale1 = t;
		} else {
			omega = (float)acos(cosom);
			sinom = sinf(omega);
			scale0 = sinf((1.0f - t) * omega) * (1.0f / sinom);
			scale1 = sinf(omega * t) * (1.0f / sinom);
		}
		return WQuat(scale0 * q.x + scale1 * to.x, scale0 * q.y + scale1 * to.y,
		             scale0 * q.z + scale1 * to.z, scale0 * q.w + scale1 * to.w);
	} else {
		scale0 = sinf((1.0f - t) * data_9eb420[1]);
		scale1 = sinf(t * data_9eb420[1]);
		return WQuat(scale0 * q.x - scale1 * q.y, scale0 * q.y + scale1 * q.x,
		             scale0 * q.z - scale1 * q.w, scale0 * q.w + scale1 * q.z);
	}
}

// wmath.inl:379
CAMERAWORK_WMATH_INLINE WQuat& WQuat::operator=(const WQuat& q) {
	memcpy(this, &q, sizeof(WQuat));
	return *this;
}

// wmath.inl:385
WIND_WMATH_INLINE int operator&(const Waabb& a, const Waabb& b) {
	for (int i = 0; i < 3; i++) {
		if (a.min.p[i] > b.max.p[i] || a.max.p[i] < b.min.p[i])
			return 0;
	}
	return 1;
}

// wmath.h:166
QUADTREE_WMATH_INLINE void Waabb::Clear(void) {
	min = WVector(3.402823466e+38f, 3.402823466e+38f, 3.402823466e+38f);
	max = WVector(-3.402823466e+38f, -3.402823466e+38f, -3.402823466e+38f);
}

// wmath.h:171
QUADTREE_WMATH_INLINE void Waabb::AddPoint(const WVector& v) {
	if (v.x > max.x) max.x = v.x;
	if (v.x < min.x) min.x = v.x;
	if (v.y > max.y) max.y = v.y;
	if (v.y < min.y) min.y = v.y;
	if (v.z > max.z) max.z = v.z;
	if (v.z < min.z) min.z = v.z;
}

// wmath.inl:939
// The box moved by v.
inline Waabb operator+(const Waabb& a, const WVector& v) {
	Waabb r = a;
	r.min += v;
	r.max += v;
	return r;
}

// wmath.inl:1000
inline Waabb& Waabb::operator+=(const WVector& v) {
	if (v.x > 0.0f) max.x += v.x;
	else min.x += v.x;

	if (v.y > 0.0f) max.y += v.y;
	else min.y += v.y;

	if (v.z > 0.0f) max.z += v.z;
	else min.z += v.z;

	return *this;
}

// wmath.inl:1050
inline void Wobb::operator*=(const WVector& v) {
	extend[0] *= v.x; extend[1] *= v.y; extend[2] *= v.z;
}

// wmath.inl:1065
// The oriented box of an axis-aligned one.
inline Wobb& Wobb::operator=(const Waabb& b) {
	center = (b.max + b.min) * 0.5f;
	extend[0] = WVector((b.max.x - b.min.x) * 0.5f, 0.0f, 0.0f);
	extend[1] = WVector(0.0f, (b.max.y - b.min.y) * 0.5f, 0.0f);
	extend[2] = WVector(0.0f, 0.0f, (b.max.z - b.min.z) * 0.5f);

	return *this;
}

// wmath.inl:1075
// The box of a transform: its pivot is the center, its axes the half-axes.
inline Wobb& Wobb::operator=(const WMatrix& m) {
	center = m.pivot;
	extend[0] = m.xa;
	extend[1] = m.ya;
	extend[2] = m.za;

	return *this;
}

// wmath.h:619
inline Wobb::Wobb(const WMatrix& m) {
	*this = m;
}

// wmath.inl:1091
// Whether pos lies inside the box: its offset from the center projects on
// each half-axis within that half-axis.
AMBIENTSOUND_WMATH_INLINE bool Wobb::IsInclude(const WVector& pos) const {
	WVector d = pos - center;
	float d0, d1, d2;

	return ((d0 = WDotProduct(d, extend[0])) > 0.0f ? d0 : -d0) <= WDotProduct(extend[0], extend[0])
		&& ((d1 = WDotProduct(d, extend[1])) > 0.0f ? d1 : -d1) <= WDotProduct(extend[1], extend[1])
		&& ((d2 = WDotProduct(d, extend[2])) > 0.0f ? d2 : -d2) <= WDotProduct(extend[2], extend[2]) ? true : false;
}
