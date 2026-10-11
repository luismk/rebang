// Original: d:\build\custom\temp\code\client\projectg\golfrulequickrace.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULEQUICKRACE_H
#define PG_CLIENT_PROJECTG_GOLFRULEQUICKRACE_H

#include "..\..\defines.h"
#include "GolfRuleBase.h"

class CGolfRule;

class CGolfRuleQuickrace : public CGolfRuleBase {
public:
	CGolfRuleQuickrace(CGolfRule *pRule);
	virtual ~CGolfRuleQuickrace(void);
	virtual void SetPlayer(void);
	virtual void SetGrade(void);
	virtual void SetReady(void);
};
CHECK_SIZE(CGolfRuleQuickrace, 0x18);

#endif // PG_CLIENT_PROJECTG_GOLFRULEQUICKRACE_H
