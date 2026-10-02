#pragma once

#include "encrypttypes.h"

class CClub
{
public:
	struct sClub;

	int PowerShotType() { return m_powerShot; }

private:
	int m_reserved;
	sClub* m_pClub;
	WList<sClub*> m_clubList;
	unsigned char m_curIndex;
	unsigned char m_limitIndex;
	WCrypticValue<int> m_powerShot;
	// TODO: incomplete
};

CClub& GolfClub();
