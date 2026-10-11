// Original: d:\build\custom\temp\code\client\projectg\caddiebonus.h
#ifndef PG_CLIENT_PROJECTG_CADDIEBONUS_H
#define PG_CLIENT_PROJECTG_CADDIEBONUS_H

#include <map>
#include <string>

#include "..\..\defines.h"

// The caddies that give a pang bonus, by the low bits of their item type id
// (the full type id is 0x1C000000 | value).
enum eCaddie {
	CADDIE_W_BIGBONGDARI = 12,      // "W_BIGBONGDARI": a fixed bonus per ticket
	CADDIE_R_BIGBONGDARI = 13       // "R_BIGBONGDARI": a percentage of the pang per ticket
};

// Draws the pang bonus of the bonus caddies from CBonusTicket's ticket
// tables, one table per caddie (registered by name in the constructor).
class CCaddieBonus {
	LAYOUT_CHECK_FRIEND;
public:
	CCaddieBonus(void);
	~CCaddieBonus(void);

	void SetTicket(void);
	__int64 GetBonusPang(eCaddie caddie, __int64 pang);

private:
	bool IsHaveCaddie(eCaddie caddie);
	void Release(void);
	void RegisterBonus(int caddie, std::string name);
	__int64 GetWBigPontaBonus(int ticket);
	__int64 GetRBigPontaBonus(int ticket, __int64 pang);

	std::map<int, std::string> m_bonusTables;   // 0x0, caddie -> ticket table name
};

CHECK_SIZE(CCaddieBonus, 0xc);
BEGIN_LAYOUT_CHECK(CCaddieBonus)
	CHECK_FIELD(CCaddieBonus, m_bonusTables, 0x0);
END_LAYOUT_CHECK

#endif
