// Original: d:\build\custom\temp\code\client\projectg\golfrulenewapproach.cpp
#include "GolfRuleNewApproach.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "polysoup.h"

// golfrulenewapproach.cpp:16
CGolfRuleNewApproach::CGolfRuleNewApproach(CGolfRule *pRule) {
	m_pRule = pRule;

	GOLFDOC()->m_approachMode = true;
}

// golfrulenewapproach.cpp:23
CGolfRuleNewApproach::~CGolfRuleNewApproach(void) {
}

// golfrulenewapproach.cpp:30
void CGolfRuleNewApproach::SetReady(void) {
	CGolfRuleBase::SetReady();

	GOLFDOC()->ApproachDataAllClear();
}

// golfrulenewapproach.cpp:47
void CGolfRuleNewApproach::SetGrade(void) {
}

// The CSharedDoc member this file reads that SharedDoc.h does not declare yet
// (its PADDING(0x4604, 0x4650) covers it).
struct CSharedDocRandoms {
	UNKNOWN(0x0, 0x4608);
	unsigned long m_randoms[18];    // 0x4608
};

// golfrulenewapproach.cpp:55
WVector CGolfRuleNewApproach::GetRandomStartPos(unsigned long seed) {
	// WPolySoup 0xEC: the course's bounding box (a Waabb polysoup.h does not
	// declare yet).
	Waabb box = *(Waabb *)((char *)GOLFDOC()->m_pPolySoup + 0xEC);
	const CSharedDocRandoms *doc = (const CSharedDocRandoms *)WSingleton<CSharedDoc>::Instance();

	WVector pos;
	pos.x = (box.max.x - box.min.x) * ((doc->m_randoms[seed % 18] + seed) & 0x7F) / 128.0f + box.min.x;

	pos.y = 0.0f; pos.z = (box.max.z - box.min.z) * ((doc->m_randoms[17 - seed % 18] + seed) & 0x7F) / 128.0f + box.min.z;

	return pos;
}
