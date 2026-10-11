// Original: d:\build\custom\temp\code\client\projectg\golfruleapproach.cpp
#include "GolfRuleApproach.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "polysoup.h"

// golfruleapproach.cpp:19
CGolfRuleApproach::CGolfRuleApproach(CGolfRule *pRule) {
	m_pRule = pRule;

	GOLFDOC()->m_approachMode = true;
}

// golfruleapproach.cpp:26
CGolfRuleApproach::~CGolfRuleApproach(void) {
}

// golfruleapproach.cpp:222
void CGolfRuleApproach::SetGrade(void) {
}

// golfruleapproach.cpp:227
void CGolfRuleApproach::SetReady(void) {
	CGolfRuleBase::SetReady();
}

// The CSharedDoc member this file reads that SharedDoc.h does not declare yet
// (its PADDING(0x4604, 0x4650) covers it).
struct CSharedDocRandoms {
	UNKNOWN(0x0, 0x4608);
	unsigned long m_randoms[18];    // 0x4608, also recorded by CGhostDocument::RecordGameData
};

// golfruleapproach.cpp:239
WVector CGolfRuleApproach::GetRandomStartPos(unsigned long seed) {
	// WPolySoup 0xEC: the course's bounding box (a Waabb polysoup.h does not
	// declare yet).
	Waabb box = *(Waabb *)((char *)GOLFDOC()->m_pPolySoup + 0xEC);
	const CSharedDocRandoms *doc = (const CSharedDocRandoms *)WSingleton<CSharedDoc>::Instance();

	WVector pos;
	pos.x = (box.max.x - box.min.x) * ((doc->m_randoms[seed % 18] + seed) & 0x7F) / 128.0f + box.min.x;

	pos.y = 0.0f; pos.z = (box.max.z - box.min.z) * ((doc->m_randoms[17 - seed % 18] + seed) & 0x7F) / 128.0f + box.min.z;

	return pos;
}
