// Original: d:\build\custom\temp\code\client\projectg\golfrulenewapproach.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULENEWAPPROACH_H
#define PG_CLIENT_PROJECTG_GOLFRULENEWAPPROACH_H

#include "..\..\defines.h"
#include "GolfRuleBase.h"
#include "wmath.h"

class CGolfRule;

class CGolfRuleNewApproach : public CGolfRuleBase {
public:
	CGolfRuleNewApproach(CGolfRule *pRule);
	virtual ~CGolfRuleNewApproach(void);
	virtual void SetPlayer(void);
	virtual void SetGrade(void);
	virtual void SetReady(void);

private:
	WVector GetRandomStartPos(unsigned long seed);
};
CHECK_SIZE(CGolfRuleNewApproach, 0x18);

#endif // PG_CLIENT_PROJECTG_GOLFRULENEWAPPROACH_H
