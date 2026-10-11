// Original: d:\build\custom\temp\code\client\projectg\golfrulebase.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULEBASE_H
#define PG_CLIENT_PROJECTG_GOLFRULEBASE_H

#include "..\..\defines.h"

class CGolfRule;

// Rule strategy base. The vtable order is read from ??_7CGolfRuleBase@@6B@
// (0x9EB0B0); the members from the constructor (golfrulebase.cpp:24-30).
class CGolfRuleBase {
	LAYOUT_CHECK_FRIEND;
public:
	CGolfRuleBase(void);
	virtual void SetPlayer(void);                       // vtable slot 0
	virtual void SetGrade(void);                        // vtable slot 1
	virtual void SetReady(void);                        // vtable slot 2
	virtual void AddPlayer(unsigned char, char const *, unsigned long, unsigned long, unsigned long); // 3
	virtual void AddPlayerOffline(unsigned char, char const *, unsigned long, unsigned long);          // 4
	virtual int GetNumTimeout(void);                    // vtable slot 5
	virtual bool CheckGiveUp(void);                     // vtable slot 6
	virtual ~CGolfRuleBase(void);                       // vtable slot 7

	void CheckCalipersCount(void);

protected:
	CGolfRule *m_pRule;                                 // 0x4, the owning rule task
	// 0x8: four 0x00RRGGBB colours set by the constructor (red, magenta,
	// green, blue)
	unsigned long m_colors[4];
};
CHECK_SIZE(CGolfRuleBase, 0x18);
BEGIN_LAYOUT_CHECK(CGolfRuleBase)
	CHECK_FIELD(CGolfRuleBase, m_pRule, 0x4);
	CHECK_FIELD(CGolfRuleBase, m_colors, 0x8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GOLFRULEBASE_H
