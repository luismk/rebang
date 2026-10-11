// Original: d:\build\custom\temp\code\shared\mtrand1p1.h
#ifndef PG_SHARED_MTRAND1P1_H
#define PG_SHARED_MTRAND1P1_H

// Richard J. Wagner's Mersenne Twister class (MersenneTwister.h, version 1.1),
// renamed MTRand1p1 by Ntreev. Only the members the game's objects emit are
// transcribed; the line comments are the original header's line marks.

#include <limits.h>
#include <time.h>

#include "..\..\defines.h"

class MTRand1p1 {
	LAYOUT_CHECK_FRIEND;
public:
	typedef unsigned long uint32;

	enum { N = 624 };
	enum { SAVE = N + 1 };

protected:
	enum { M = 397 };

	uint32 state[N];   // 0x0
	uint32 *pNext;     // 0x9c0
	int left;          // 0x9c4

public:
	MTRand1p1();

	uint32 randInt();
	uint32 randInt(const uint32 n);

	void seed(const uint32 oneSeed);
	void seed();

protected:
	void initialize(const uint32 oneSeed);
	void reload();
	// mtrand1p1.h:128
	uint32 hiBit(const uint32 u) const { return u & 0x80000000UL; }
	// mtrand1p1.h:129
	uint32 loBit(const uint32 u) const { return u & 0x00000001UL; }
	// mtrand1p1.h:130
	uint32 loBits(const uint32 u) const { return u & 0x7fffffffUL; }
	// mtrand1p1.h:132
	uint32 mixBits(const uint32 u, const uint32 v) const
		{ return hiBit(u) | loBits(v); }
	// mtrand1p1.h:134
	uint32 magic(const uint32 u) const
		{ return loBit(u) ? 0x9908b0dfUL : 0x0UL; }
	// mtrand1p1.h:136
	uint32 twist(const uint32 m, const uint32 s0, const uint32 s1) const
		{ return m ^ (mixBits(s0, s1) >> 1) ^ magic(s1); }
	static uint32 hash(time_t t, clock_t c);
};

CHECK_SIZE(MTRand1p1, 0x9c8);
BEGIN_LAYOUT_CHECK(MTRand1p1)
	CHECK_FIELD(MTRand1p1, pNext, 0x9c0);
	CHECK_FIELD(MTRand1p1, left, 0x9c4);
END_LAYOUT_CHECK

// mtrand1p1.h:143
inline MTRand1p1::uint32 MTRand1p1::hash(time_t t, clock_t c) {
	// Get a uint32 from t and c (code by Lawrence Kirby)
	static uint32 differ = 0;  // guarantees time-based seeds change

	uint32 h1 = 0;
	unsigned char *p = (unsigned char *)&t;
	for (size_t i = 0; i < sizeof(t); ++i) {
		h1 *= UCHAR_MAX + 2U;
		h1 += p[i];
	}
	uint32 h2 = 0;
	p = (unsigned char *)&c;
	for (size_t j = 0; j < sizeof(c); ++j) {
		h2 *= UCHAR_MAX + 2U;
		h2 += p[j];
	}
	return (h1 + differ++) ^ h2;
}

// mtrand1p1.h:168
inline void MTRand1p1::initialize(const uint32 seed) {
	register uint32 *s = state;
	register uint32 *r = state;
	register int i = 1;
	*s++ = seed & 0xffffffffUL;
	for (; i < N; ++i) {
		*s++ = (1812433253UL * (*r ^ (*r >> 30)) + i) & 0xffffffffUL;
		r++;
	}
}

// mtrand1p1.h:185
inline void MTRand1p1::reload() {
	static const int MmN = int(M) - int(N);  // in case enums are unsigned
	register uint32 *p = state;
	register int i;
	for (i = N - M; i--; ++p)
		*p = twist(p[M], p[0], p[1]);
	for (i = M; --i; ++p)
		*p = twist(p[MmN], p[0], p[1]);
	*p = twist(p[MmN], p[0], state[0]);

	left = N, pNext = state;
}

// mtrand1p1.h:201
inline void MTRand1p1::seed(const uint32 oneSeed) {
	initialize(oneSeed);
	reload();
}

// mtrand1p1.h:243
inline void MTRand1p1::seed() {
	seed(hash(time(NULL), clock()));
}

// mtrand1p1.h:274
inline MTRand1p1::MTRand1p1()
	{ seed(); }

// mtrand1p1.h:287
inline MTRand1p1::uint32 MTRand1p1::randInt() {
	// Pull a 32-bit integer from the generator state
	if (left == 0) reload();
	--left;

	register uint32 s1;
	s1 = *pNext++;
	s1 ^= (s1 >> 11);
	s1 ^= (s1 << 7) & 0x9d2c5680UL;
	s1 ^= (s1 << 15) & 0xefc60000UL;
	return (s1 ^ (s1 >> 18));
}

// mtrand1p1.h:303
inline MTRand1p1::uint32 MTRand1p1::randInt(const uint32 n) {
	// Find which bits are used in n
	uint32 used = n;
	used |= used >> 1;
	used |= used >> 2;
	used |= used >> 4;
	used |= used >> 8;
	used |= used >> 16;

	// Draw numbers until one is found in [0,n]
	uint32 i;
	do
		i = randInt() & used;  // toss unused bits to shorten search
	while (i > n);
	return i;
}

#endif // PG_SHARED_MTRAND1P1_H
