// Original: d:\build\custom\temp\code\client\projectg\golfball.cpp
#include "golfball.h"
#include "hackingmanager.h"

#include <string.h>

// golfball.cpp:17
CGolfBall::CGolfBall(void) {
	m_pPreDataOverflow = 0;
	m_radius = 0.14698039f;
	m_mass = 0.045927f;
}

// golfball.cpp:24
void CGolfBall::ClearBuffer(void) {
	m_preDataOverflowBlocks = 0; if (m_pPreDataOverflow) { delete[] m_pPreDataOverflow; m_pPreDataOverflow = 0; }
}

// golfball.cpp:30
void CGolfBall::AddCrypticValue(void) {
	if (HackingManager::Instance()) {
		HackingManager::Instance()->AddCrypticIntValue(&m_holeIn);
	}
}

// golfball.cpp:38
void CGolfBall::Reset(WVector pos) {
	memset(m_preData, 0, sizeof(m_preData));

	ClearBuffer();

	m_pos = pos;
	m_preData[0].m_pos = pos;

	m_velocity.x = m_velocity.y = m_velocity.z = 0.0f;
	m_rotation[0] = m_rotation[1] = 0.0f;

	m_state = 0;
	m_powerFlags = 0;
	m_flags = 0;
	m_landed = false;
	m_lieSlope = 0.0f;
	m_lieAngle = 0.0f;
	m_shotCounters[0] = 0;
	m_shotCounters[1] = 0;
	m_lie = 0;
	m_groundType = 1;
	m_holeIn = 0;

	m_preDataIndex = -1;
	m_bounceCount = 0;
	m_shotStage = 0;

	m_preDataCount = 0;

	m_vectorSlideCount = 0;
	m_lastEventPreDataIndex = -1;
	m_eventPreDataIndex[0] = -1;
	m_eventPreDataIndex[1] = -1;
	m_eventPreDataIndex[2] = -1;
	m_eventPreDataIndex[3] = -1;
	m_eventPreDataIndex[4] = -1;
	m_eventPreDataIndex[5] = -1;
	m_eventPreDataIndex[8] = -1;
	m_eventPreDataIndex[6] = -1;
	m_eventPreDataIndex[7] = -1;
	m_eventPreDataIndex[9] = -1;
	m_eventPreDataIndex[10] = -1;
	m_eventPreDataIndex[12] = -1;
	m_eventPreDataIndex[13] = -1;
	m_eventPreDataIndex[14] = -1;
	m_eventPreDataIndex[11] = -1;
	m_firstContactIndex = -1;
	m_firstContactId = -1;
	m_lastContactIndex = -1;
	m_lastContactId = -1;
}

// golfball.cpp:102
void CGolfBall::DbgTraceOutput(const char *label, float value) {
	// Debug trace: compiled out of the ReleaseQA build.
}

// golfball.cpp:133
void CGolfBall::DbgTraceToFile(const char *label, const WVector &vector) {
	// Debug trace to a file: compiled out of the ReleaseQA build.
}
