// Original: d:\build\custom\temp\code\client\projectg\golfruleguildmatch.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULEGUILDMATCH_H
#define PG_CLIENT_PROJECTG_GOLFRULEGUILDMATCH_H

#include "..\..\defines.h"
#include "GolfRuleBase.h"

class CGolfRule;

class CGolfRuleGuildMatch : public CGolfRuleBase {
public:
	CGolfRuleGuildMatch(CGolfRule *pRule);
	virtual ~CGolfRuleGuildMatch(void);
	virtual void SetPlayer(void);
	virtual void SetGrade(void);
	virtual void SetReady(void);
};
CHECK_SIZE(CGolfRuleGuildMatch, 0x18);

#endif // PG_CLIENT_PROJECTG_GOLFRULEGUILDMATCH_H
