// Original: d:\build\custom\temp\code\client\wangreal\include\wutil.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WUTIL_H
#define PG_CLIENT_WANGREAL_INCLUDE_WUTIL_H

// wutil.h:218
template <class T> T Min(T a, T b) { return (a > b) ? b : a; }
// wutil.h:219
template <class T> T Max(T a, T b) { return (a < b) ? b : a; }
// wutil.h:220
template <class T> T Abs(T a) { return a > 0 ? a : -a; }
// wutil.h:221
template <class T> T Between(T low, T value, T high) { return value < low ? low : (value > high ? high : value); }
// wutil.h:222
template <class T> void Swap(T& a, T& b) {
	T t = a;
	a = b;
	b = t;
}

// wutil.h:227
template <class T, class P> bool TSafeNew(P& p, unsigned int count) {
	bool ret = false;

	if (p == 0 && count > 0) {
		p = new T[count];
		if (p == 0) {
			return false;
		}
		ret = true;
	}
	return ret;
}

// wutil.h:245
template <class P> void TSafeDelete(P& p) {
	if (p) {
		delete[] p;
		p = 0;
	}
}

// wutil.h:257 (djb2)
inline unsigned long GetHashCode(const char* str) {
	unsigned long hash = 5381;
	int c;
	while ((c = *str++) != 0)
		hash += (hash << 5) + c;
	return hash;
}

// wbone.cpp defines WBONE_WUTIL_INLINE empty before its includes: the original
// holds the out-of-line copies of Modulate and AddDiffuse in wbone's object;
// every other unit inlines them.
#ifndef WBONE_WUTIL_INLINE
#define WBONE_WUTIL_INLINE inline
#endif

// wutil.h:266 (per-channel multiply of two colours, alpha dropped)
WBONE_WUTIL_INLINE unsigned long Modulate(unsigned long left, unsigned long right) {
	unsigned long red = ((left >> 16 & 0xff) * (right >> 16 & 0xff)) >> 8;
	unsigned long green = ((left >> 8 & 0xff) * (right >> 8 & 0xff)) >> 8;
	unsigned long blue = ((left & 0xff) * (right & 0xff)) >> 8;
	return (red << 16) + (green << 8) + blue;
}

// wutil.h:277 (per-channel saturating add of two colours, alpha dropped)
WBONE_WUTIL_INLINE unsigned long AddDiffuse(unsigned long left, unsigned long right) {
	unsigned long blue = Min<unsigned long>(0xff, (left & 0xff) + (right & 0xff));
	unsigned long green = Min<unsigned long>(0xff, (left >> 8 & 0xff) + (right >> 8 & 0xff));
	unsigned long red = Min<unsigned long>(0xff, (left >> 16 & 0xff) + (right >> 16 & 0xff));
	return (red << 16) + (green << 8) + blue;
}

// Defined out of line in ProjectG/pool.cpp.
unsigned long __fastcall AddDiffuse(unsigned long left, unsigned long right, unsigned char value);

#endif // PG_CLIENT_WANGREAL_INCLUDE_WUTIL_H
