// Original: d:\build\custom\temp\code\client\projectg\golfruletutorial.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULETUTORIAL_H
#define PG_CLIENT_PROJECTG_GOLFRULETUTORIAL_H

#include "..\..\defines.h"
#include "GolfRuleBase.h"

class CGolfRule;

class CGolfRuleTutorial : public CGolfRuleBase {
public:
	CGolfRuleTutorial(CGolfRule *pRule);
	virtual ~CGolfRuleTutorial(void);
	virtual void SetPlayer(void);
	virtual void SetGrade(void);
	virtual void SetReady(void);

protected:
	void SetEquipCharCaddieInfoForTutorial(void);
};
CHECK_SIZE(CGolfRuleTutorial, 0x18);

#endif // PG_CLIENT_PROJECTG_GOLFRULETUTORIAL_H
