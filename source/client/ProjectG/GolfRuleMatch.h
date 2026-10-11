// Original: d:\build\custom\temp\code\client\projectg\golfrulematch.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULEMATCH_H
#define PG_CLIENT_PROJECTG_GOLFRULEMATCH_H

#include "..\..\defines.h"
#include "GolfRuleBase.h"

class CGolfRule;

// The match (stroke play) rule. Adds no member to CGolfRuleBase.
class CGolfRuleMatch : public CGolfRuleBase {
public:
	CGolfRuleMatch(CGolfRule *pRule);
	virtual ~CGolfRuleMatch(void);
	virtual void SetPlayer(void);
	virtual void SetGrade(void);
	virtual void SetReady(void);
	virtual void AddPlayer(unsigned char, char const *, unsigned long, unsigned long, unsigned long);
};
CHECK_SIZE(CGolfRuleMatch, 0x18);

#endif // PG_CLIENT_PROJECTG_GOLFRULEMATCH_H
