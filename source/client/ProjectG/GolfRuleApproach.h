// Original: d:\build\custom\temp\code\client\projectg\golfruleapproach.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULEAPPROACH_H
#define PG_CLIENT_PROJECTG_GOLFRULEAPPROACH_H

#include "..\..\defines.h"
#include "GolfRuleBase.h"
#include "wmath.h"

class CGolfRule;

class CGolfRuleApproach : public CGolfRuleBase {
public:
	CGolfRuleApproach(CGolfRule *pRule);
	virtual ~CGolfRuleApproach(void);
	virtual void SetPlayer(void);
	virtual void SetGrade(void);
	virtual void SetReady(void);

private:
	WVector GetRandomStartPos(unsigned long seed);
};
CHECK_SIZE(CGolfRuleApproach, 0x18);

#endif // PG_CLIENT_PROJECTG_GOLFRULEAPPROACH_H
