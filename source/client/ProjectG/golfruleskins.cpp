// Original: d:\build\custom\temp\code\client\projectg\golfruleskins.cpp
#include "GolfRuleSkins.h"

#include "GolfDoc.h"
#include "SharedDoc.h"

// golfruleskins.cpp:19
CGolfRuleSkins::CGolfRuleSkins(CGolfRule *pRule) {
	m_pRule = pRule;
}

// golfruleskins.cpp:24
CGolfRuleSkins::~CGolfRuleSkins(void) {
}

// golfruleskins.cpp:125
void CGolfRuleSkins::SetGrade(void) {
	for (unsigned char i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		if (GOLFDOC()->GetPlayer(i)->m_state == 3) {
			continue;
		}

		unsigned char grade;
		if (WSingleton<CSharedDoc>::Instance()->m_roomPlayers[i].m_level < 6) {
			grade = 1;
		} else {
			grade = (WSingleton<CSharedDoc>::Instance()->m_roomPlayers[i].m_level - 1) / 5 + 1;
		}

		if (GOLFDOC()->m_maxGrade < grade) {
			GOLFDOC()->m_maxGrade = grade;
		}
	}
}

// golfruleskins.cpp:147
void CGolfRuleSkins::SetReady(void) {
	CGolfRuleBase::SetReady();
}
