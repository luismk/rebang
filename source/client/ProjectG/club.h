#pragma once

#include "encrypttypes.h"

class CClub
{
public:
	struct sClub
	{
		WCrypticValue<float> range;
		WCrypticValue<float> lie;
		unsigned char id;
		unsigned char index;
		unsigned char type;
		WCrypticValue<float> power;
		WCrypticValue<float> curve;
		WCrypticValue<float> spin;
		char name[4];
		float pointFactor;
		WCrypticValue<float> barSpeed;
	};

	unsigned char GetType() { return m_pClub->type; }
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
