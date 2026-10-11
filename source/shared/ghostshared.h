// Original: d:\build\custom\temp\code\shared\ghostshared.h
#ifndef PG_SHARED_GHOSTSHARED_H
#define PG_SHARED_GHOSTSHARED_H

#include <vector>

#include "..\..\defines.h"

// Bonus computation of a ghost game, shared with the server.
class CGhostGameBonusUtil {
public:
	struct GAMEBONUS {
		int m_pang;            // 0x0
		unsigned char m_rate;  // 0x4
		int m_bonus;           // 0x8

		GAMEBONUS(int pang, unsigned char rate, int bonus);   // ghostshared.h:43
	};

	CGhostGameBonusUtil(void);    // ghostshared.h:48
	~CGhostGameBonusUtil(void);   // ghostshared.h:67

	bool IsUserWin(int userScore, int userShot, int ghostScore, int ghostShot);   // ghostshared.h:76
	int GetOneClickBonus(unsigned char rate, bool win, int basePoint, int holeCount, unsigned char userRank, unsigned char ghostRank);   // ghostshared.h:87

private:
	int GetBonusPoint(bool win, int basePoint, int holeCount, unsigned char userRank, unsigned char ghostRank);   // ghostshared.h:102
	int GetPangBonusIndex(int pang);   // ghostshared.h:117

	std::vector<GAMEBONUS> m_pangBonus;   // 0x0
};

CHECK_SIZE(CGhostGameBonusUtil::GAMEBONUS, 0xc);
CHECK_FIELD(CGhostGameBonusUtil::GAMEBONUS, m_pang, 0x0);
CHECK_FIELD(CGhostGameBonusUtil::GAMEBONUS, m_rate, 0x4);
CHECK_FIELD(CGhostGameBonusUtil::GAMEBONUS, m_bonus, 0x8);
CHECK_SIZE(CGhostGameBonusUtil, 0x10);

#endif // PG_SHARED_GHOSTSHARED_H
