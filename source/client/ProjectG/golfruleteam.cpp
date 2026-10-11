// Original: d:\build\custom\temp\code\client\projectg\golfruleteam.cpp
#include "GolfRuleTeam.h"

#include "GolfDoc.h"
#include "SharedDoc.h"

// golfruleteam.cpp:21
CGolfRuleTeam::CGolfRuleTeam(CGolfRule *pRule) {
	m_pRule = pRule;
}

// golfruleteam.cpp:26
CGolfRuleTeam::~CGolfRuleTeam(void) {
}

// golfruleteam.cpp:138
void CGolfRuleTeam::SetGrade(void) {
	for (unsigned char i = 0; i < 2; i++) {
		unsigned char sum = 0;

		for (i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
			if (GOLFDOC()->GetPlayer(i)->m_attribute != i) {
				continue;
			}

			if (WSingleton<CSharedDoc>::Instance()->m_roomPlayers[i].m_level < 6) {
				sum += 1;
			} else {
				sum += (WSingleton<CSharedDoc>::Instance()->m_roomPlayers[i].m_level - 1) / 5 + 1;
			}
		}

		unsigned char grade = sum / 2;
		if (GOLFDOC()->m_maxGrade < grade) {
			GOLFDOC()->m_maxGrade = grade;
		}
	}
}

// golfruleteam.cpp:164
void CGolfRuleTeam::SetReady(void) {
	CGolfRuleBase::SetReady();
}

// golfruleteam.cpp:291
int CGolfRuleTeam::GetNumTimeout(void) {
	int numTimeout = PLAYER(GOLFDOC()->m_myPlayerIndex)->m_numTimeout;

	if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_partnerIndex != 0xFF) {
		numTimeout += PLAYER(PLAYER(GOLFDOC()->m_myPlayerIndex)->m_partnerIndex)->m_numTimeout;
	}

	return numTimeout;
}

// golfruleteam.cpp:302
bool CGolfRuleTeam::CheckGiveUp(void) {
	if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_state == 3) {
		return false;
	}

	unsigned char limit = GOLFDOC()->GetHoleData().m_par + 4;
	int state = TEAM(PLAYER(GOLFDOC()->m_myPlayerIndex)->m_attribute)->m_state;
	sTeamHoleData &hole = TEAM(PLAYER(GOLFDOC()->m_myPlayerIndex)->m_attribute)->m_holes[GOLFDOC()->m_currentHole - 1];
	unsigned char score = ~(hole.m_unkE ^ hole.m_unk0);

	return state == 1 ? score > limit : score >= limit;
}
