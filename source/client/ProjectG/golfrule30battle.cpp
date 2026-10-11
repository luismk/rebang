// Original: d:\build\custom\temp\code\client\projectg\golfrule30battle.cpp
#include "GolfRule30Battle.h"

// golfrule30battle.cpp:19
CGolfRule30Battle::CGolfRule30Battle(CGolfRule *pRule) {
	m_pRule = pRule;
}

// golfrule30battle.cpp:24
CGolfRule30Battle::~CGolfRule30Battle(void) {
}

// golfrule30battle.cpp:224
void CGolfRule30Battle::SetGrade(void) {
}

// golfrule30battle.cpp:229
void CGolfRule30Battle::SetReady(void) {
	CGolfRuleBase::SetReady();
}
