// Original: d:\build\custom\temp\code\client\projectg\golfruleguildmatch.cpp
#include "GolfRuleGuildMatch.h"

// golfruleguildmatch.cpp:20
CGolfRuleGuildMatch::CGolfRuleGuildMatch(CGolfRule *pRule) {
	m_pRule = pRule;
}

// golfruleguildmatch.cpp:25
CGolfRuleGuildMatch::~CGolfRuleGuildMatch(void) {
}

// golfruleguildmatch.cpp:290
void CGolfRuleGuildMatch::SetGrade(void) {
}

// golfruleguildmatch.cpp:295
void CGolfRuleGuildMatch::SetReady(void) {
	CGolfRuleBase::SetReady();
}
