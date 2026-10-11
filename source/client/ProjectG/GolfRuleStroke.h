// Original: d:\build\custom\temp\code\client\projectg\golfrulestroke.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULESTROKE_H
#define PG_CLIENT_PROJECTG_GOLFRULESTROKE_H

#include "..\..\defines.h"
#include "GolfRuleBase.h"

class CGolfRule;

// The stroke play rule. Adds no member to CGolfRuleBase.
class CGolfRuleStroke : public CGolfRuleBase {
public:
	CGolfRuleStroke(CGolfRule *pRule);
	virtual ~CGolfRuleStroke(void);
	virtual void SetPlayer(void);
	virtual void SetGrade(void);
	virtual void SetReady(void);
};
CHECK_SIZE(CGolfRuleStroke, 0x18);

#endif // PG_CLIENT_PROJECTG_GOLFRULESTROKE_H
