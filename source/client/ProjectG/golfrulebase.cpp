// Original: d:\build\custom\temp\code\client\projectg\golfrulebase.cpp
#include "GolfRuleBase.h"

#include "GolfDoc.h"
#include "SharedDoc.h"
#include "GolfRule.h"
#include "TaskManager.h"
#include "actor.h"
#include "../shared/Localize.h"

// Actor name pointer the ready message 0x26F goes to (no symbol; not cut yet).
extern "C" const char *data_a7e3c8;

// golfrulebase.cpp:24
CGolfRuleBase::CGolfRuleBase(void) {
	m_pRule = 0;

	m_colors[0] = 0xFD0002;
	m_colors[1] = 0xDC00FD;
	m_colors[2] = 0x27D628;
	m_colors[3] = 0x2778D6;
}

// golfrulebase.cpp:34
CGolfRuleBase::~CGolfRuleBase(void) {
}

// golfrulebase.cpp:184
void CGolfRuleBase::SetGrade(void) {
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

// golfrulebase.cpp:206
void CGolfRuleBase::SetReady(void) {
	if (IsLocalContent((localContentType_t)0x30)) {
		CheckCalipersCount();
	}

	AfxGetTask()->GetActor(data_a7e3c8) << MsgObject(0, 0x26F, 10, 0, 0, 0, 0);

	m_pRule->ChangeGameMode(1);
}

// golfrulebase.cpp:515
int CGolfRuleBase::GetNumTimeout(void) {
	return PLAYER(GOLFDOC()->m_myPlayerIndex)->m_numTimeout;
}

// golfrulebase.cpp:521
bool CGolfRuleBase::CheckGiveUp(void) {
	if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_state == 3) {
		return false;
	}

	unsigned char limit = GOLFDOC()->GetHoleData().m_par + 4;
	int state = PLAYER(GOLFDOC()->m_myPlayerIndex)->m_state;
	unsigned char score = PLAYER(GOLFDOC()->m_myPlayerIndex)->m_holes[GOLFDOC()->m_currentHole - 1];

	return state == 1 ? score > limit : score >= limit;
}
