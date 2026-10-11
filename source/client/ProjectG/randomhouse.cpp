// Original: d:\build\custom\temp\code\client\projectg\randomhouse.cpp
#include "RandomHouse.h"

// Mersenne Twister parameters (period 2^19937 - 1).
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL
#define UPPER_MASK 0x80000000UL
#define LOWER_MASK 0x7fffffffUL
#define TEMPERING_MASK_B 0x9d2c5680UL
#define TEMPERING_MASK_C 0xefc60000UL
#define TEMPERING_SHIFT_U(y) ((y) >> 11)
#define TEMPERING_SHIFT_S(y) ((y) << 7)
#define TEMPERING_SHIFT_T(y) ((y) << 15)
#define TEMPERING_SHIFT_L(y) ((y) >> 18)

template <> CRandomHouse* WSingleton<CRandomHouse>::m_pInstance = NULL;

// randomhouse.cpp:65
CRandomHouse::CRandomHouse(void) : m_state(0), m_index(N + 1), m_seed(4357) {
	m_state = new unsigned long[N];
}

// randomhouse.cpp:70
CRandomHouse::~CRandomHouse(void) {
	if (m_state) { delete[] m_state; m_state = 0; }
}

// randomhouse.cpp:77
void CRandomHouse::SetRandomSeed(unsigned long seed) {
	if (seed == 0)
		seed = m_seed;
	else
		m_seed = seed;

	int i;
	for (i = 0; i < N; i++) {
		m_state[i] = seed & 0xffff0000;
		seed = 69069 * seed + 1;
		m_state[i] |= (seed & 0xffff0000) >> 16;
		seed = 69069 * seed + 1;
	}

	m_index = N;
}

// randomhouse.cpp:95
unsigned long CRandomHouse::GetRandom(void) {
	unsigned long y;
	static unsigned long mag01[2] = { 0x0, MATRIX_A };

	if (m_index >= N) {
		int kk;

		if (m_index == N + 1)
			SetRandomSeed(0);

		for (kk = 0; kk < N - M; kk++) {
			y = (m_state[kk] & UPPER_MASK) | (m_state[kk + 1] & LOWER_MASK);
			m_state[kk] = m_state[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (; kk < N - 1; kk++) {
			y = (m_state[kk] & UPPER_MASK) | (m_state[kk + 1] & LOWER_MASK);
			m_state[kk] = m_state[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (m_state[N - 1] & UPPER_MASK) | (m_state[0] & LOWER_MASK);
		m_state[N - 1] = m_state[M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

		m_index = 0;
	}

	y = m_state[m_index++];
	y ^= TEMPERING_SHIFT_U(y);
	y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
	y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
	y ^= TEMPERING_SHIFT_L(y);

	return y;
}
