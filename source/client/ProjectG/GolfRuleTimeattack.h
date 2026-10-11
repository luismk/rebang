// Original: d:\build\custom\temp\code\client\projectg\golfruletimeattack.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULETIMEATTACK_H
#define PG_CLIENT_PROJECTG_GOLFRULETIMEATTACK_H

#include "..\..\defines.h"
#include "GolfRuleBase.h"

class CGolfRule;

class CGolfRuleTimeattack : public CGolfRuleBase {
public:
	CGolfRuleTimeattack(CGolfRule *pRule);
	virtual ~CGolfRuleTimeattack(void);
	virtual void SetPlayer(void);
	virtual void SetGrade(void);
	virtual void SetReady(void);
};
CHECK_SIZE(CGolfRuleTimeattack, 0x18);

#endif // PG_CLIENT_PROJECTG_GOLFRULETIMEATTACK_H
