// Original: d:\build\custom\temp\code\client\projectg\golfruleteam.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULETEAM_H
#define PG_CLIENT_PROJECTG_GOLFRULETEAM_H

#include "..\..\defines.h"
#include "GolfRuleBase.h"

class CGolfRule;

// The team game rule (vtable ??_7CGolfRuleTeam@@6B@, 0x9EB190). Adds no
// member to CGolfRuleBase.
class CGolfRuleTeam : public CGolfRuleBase {
public:
	CGolfRuleTeam(CGolfRule *pRule);
	virtual ~CGolfRuleTeam(void);
	virtual void SetPlayer(void);
	virtual void SetGrade(void);
	virtual void SetReady(void);
	virtual void AddPlayer(unsigned char, char const *, unsigned long, unsigned long, unsigned long);
	virtual int GetNumTimeout(void);
	virtual bool CheckGiveUp(void);
};
CHECK_SIZE(CGolfRuleTeam, 0x18);

#endif // PG_CLIENT_PROJECTG_GOLFRULETEAM_H
