// Original: d:\build\custom\temp\code\client\projectg\golfrulematch.cpp
#include "GolfRuleMatch.h"

#include "GolfDoc.h"
#include "SharedDoc.h"

// golfrulematch.cpp:19
CGolfRuleMatch::CGolfRuleMatch(CGolfRule *pRule) {
	m_pRule = pRule;
}

// golfrulematch.cpp:24
CGolfRuleMatch::~CGolfRuleMatch(void) {
}

// golfrulematch.cpp:125
void CGolfRuleMatch::SetGrade(void) {
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

// golfrulematch.cpp:147
void CGolfRuleMatch::SetReady(void) {
	CGolfRuleBase::SetReady();
}
