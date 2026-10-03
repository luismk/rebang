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

	float GetRange(const char* name = NULL);
	float GetPower();
	unsigned char GetType() { return m_pClub->type; }
	int PowerShotType() { return m_powerShot; }
	unsigned char GetApproachMode() { return m_approachMode; }

private:
	int m_reserved;
	sClub* m_pClub;
	WList<sClub*> m_clubList;
	unsigned char m_curIndex;
	unsigned char m_limitIndex;
	WCrypticValue<int> m_powerShot;
	unsigned char m_approachMode;
	// TODO: incomplete
};

CClub& GolfClub();
