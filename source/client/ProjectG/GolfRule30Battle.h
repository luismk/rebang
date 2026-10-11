// Original: d:\build\custom\temp\code\client\projectg\golfrule30battle.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULE30BATTLE_H
#define PG_CLIENT_PROJECTG_GOLFRULE30BATTLE_H

#include "..\..\defines.h"
#include "GolfRuleBase.h"

class CGolfRule;

class CGolfRule30Battle : public CGolfRuleBase {
public:
	CGolfRule30Battle(CGolfRule *pRule);
	virtual ~CGolfRule30Battle(void);
	virtual void SetPlayer(void);
	virtual void SetGrade(void);
	virtual void SetReady(void);
};
CHECK_SIZE(CGolfRule30Battle, 0x18);

#endif // PG_CLIENT_PROJECTG_GOLFRULE30BATTLE_H
