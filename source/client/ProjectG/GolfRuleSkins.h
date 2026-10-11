// Original: d:\build\custom\temp\code\client\projectg\golfruleskins.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULESKINS_H
#define PG_CLIENT_PROJECTG_GOLFRULESKINS_H

#include "..\..\defines.h"
#include "GolfRuleBase.h"

class CGolfRule;

class CGolfRuleSkins : public CGolfRuleBase {
public:
	CGolfRuleSkins(CGolfRule *pRule);
	virtual ~CGolfRuleSkins(void);
	virtual void SetPlayer(void);
	virtual void SetGrade(void);
	virtual void SetReady(void);
};
CHECK_SIZE(CGolfRuleSkins, 0x18);

#endif // PG_CLIENT_PROJECTG_GOLFRULESKINS_H
