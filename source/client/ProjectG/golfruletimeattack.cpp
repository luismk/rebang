// Original: d:\build\custom\temp\code\client\projectg\golfruletimeattack.cpp
#include "GolfRuleTimeattack.h"
#include "GolfRule.h"
#include "../shared/Localize.h"

// golfruletimeattack.cpp:19
CGolfRuleTimeattack::CGolfRuleTimeattack(CGolfRule *pRule) {
	m_pRule = pRule;
}

// golfruletimeattack.cpp:24
CGolfRuleTimeattack::~CGolfRuleTimeattack(void) {
}

// golfruletimeattack.cpp:212
void CGolfRuleTimeattack::SetGrade(void) {
}

// golfruletimeattack.cpp:217
void CGolfRuleTimeattack::SetReady(void) {
	if (IsLocalContent((localContentType_t)0x30)) {
		CheckCalipersCount();
	}
	m_pRule->ChangeGameMode(1);
}
