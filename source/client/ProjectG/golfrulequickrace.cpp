// Original: d:\build\custom\temp\code\client\projectg\golfrulequickrace.cpp
#include "GolfRuleQuickrace.h"
#include "GolfRule.h"
#include "../shared/Localize.h"

// golfrulequickrace.cpp:19
CGolfRuleQuickrace::CGolfRuleQuickrace(CGolfRule *pRule) {
	m_pRule = pRule;
}

// golfrulequickrace.cpp:24
CGolfRuleQuickrace::~CGolfRuleQuickrace(void) {
}

// golfrulequickrace.cpp:212
void CGolfRuleQuickrace::SetGrade(void) {
}

// golfrulequickrace.cpp:217
void CGolfRuleQuickrace::SetReady(void) {
	if (IsLocalContent((localContentType_t)0x30)) {
		CheckCalipersCount();
	}
	m_pRule->ChangeGameMode(1);
}
