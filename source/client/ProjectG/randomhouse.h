// Original: d:\build\custom\temp\code\client\projectg\randomhouse.h
#ifndef PG_CLIENT_PROJECTG_RANDOMHOUSE_H
#define PG_CLIENT_PROJECTG_RANDOMHOUSE_H

#include "..\..\defines.h"
#include "singleton.h"

// The game's random number source: a Mersenne Twister (MT19937, the
// original 1998 sgenrand/genrand pair seeded with Knuth's 69069 generator)
// whose state table is allocated by the constructor.
class CRandomHouse : public WSingleton<CRandomHouse> {
	LAYOUT_CHECK_FRIEND;
public:
	CRandomHouse(void);
	virtual ~CRandomHouse(void);

	void SetRandomSeed(unsigned long seed);
	unsigned long GetRandom(void);

private:
	unsigned long *m_state;             // 0x28, N (624) words, new[] in the constructor
	int m_index;                        // 0x2C, next word of m_state; N + 1 = never seeded
	unsigned long m_seed;               // 0x30, the last seed (4357 by default)
};
CHECK_SIZE(CRandomHouse, 0x34);
BEGIN_LAYOUT_CHECK(CRandomHouse)
	CHECK_FIELD(CRandomHouse, m_state, 0x28);
	CHECK_FIELD(CRandomHouse, m_index, 0x2C);
	CHECK_FIELD(CRandomHouse, m_seed, 0x30);
END_LAYOUT_CHECK

#endif
