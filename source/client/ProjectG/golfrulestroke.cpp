// Original: d:\build\custom\temp\code\client\projectg\golfrulestroke.cpp
#include "GolfRuleStroke.h"

#include "GolfDoc.h"
#include "SharedDoc.h"

// golfrulestroke.cpp:20
CGolfRuleStroke::CGolfRuleStroke(CGolfRule *pRule) {
	m_pRule = pRule;
}

// golfrulestroke.cpp:25
CGolfRuleStroke::~CGolfRuleStroke(void) {
}

// golfrulestroke.cpp:149
void CGolfRuleStroke::SetGrade(void) {
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

// golfrulestroke.cpp:171
void CGolfRuleStroke::SetReady(void) {
	CGolfRuleBase::SetReady();
}
