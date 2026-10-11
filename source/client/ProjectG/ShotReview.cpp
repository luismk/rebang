// Original: d:\build\custom\temp\code\client\projectg\shotreview.cpp
#include "ShotReview.h"
#include "GolfDoc.h"
#include "SharedDoc.h"
#include "wview.h"
#include "ObjectFactory.h"
#include "TitleFontEx.h"
#include "InputManager.h"
#include "wresrcmng.h"
#include "woverlay.h"
#include "quadtree.h"
#include "commonutil.h"
#include <math.h>
#include "../shared/globalgamedefine.h"
#include <algorithm>
#include <functional>

// shotreview.cpp:19
SpotHistory::SpotHistory() {
	m_pBarFont = new CBarFont;
	m_pBarFont->SetColor(0xFFFFFFFF);

	m_pIndicator1 = g_resrcmng->GetOverlay("[fall_indicator1.png", 0);
	m_pIndicator2 = g_resrcmng->GetOverlay("[fall_indicator2.png", 0);
}

// shotreview.cpp:28
SpotHistory::~SpotHistory() {
	if (g_resrcmng && m_pIndicator2) { g_resrcmng->Release(m_pIndicator2); m_pIndicator2 = 0; }
	if (g_resrcmng && m_pIndicator1) { g_resrcmng->Release(m_pIndicator1); m_pIndicator1 = 0; }
	if (m_pBarFont) { delete m_pBarFont; m_pBarFont = 0; }
}

// shotreview.cpp:35
void SpotHistory::Clear() {
	m_spots.clear();
}

// shotreview.cpp:96
void SpotHistory::AddItem(unsigned int param1, unsigned int param2) {
	const WVector& start = *(const WVector*)param1;
	const WVector& end = *(const WVector*)param2;
	m_spots.push_back(std::pair<WVector, WVector>(start, end));
}

// shotreview.cpp:103
void SpotHistory::PopBack() {
	if (!m_spots.empty()) {
		m_spots.pop_back();
	}
}

// shotreview.cpp:114
BallTraceHistory::BallTraceHistory() {
}

// shotreview.cpp:118
BallTraceHistory::~BallTraceHistory() {
}

// shotreview.cpp:122
void BallTraceHistory::Clear() {
	m_traces.clear();
}

// shotreview.cpp:127
void BallTraceHistory::Display() {
	if (!(GOLFDOC()->GameMode() & 0x200)) {
		return;
	}
	if (m_traces.empty()) {
		return;
	}
	std::list<std::vector<WVector> >::iterator it = m_traces.begin();
	while (it != m_traces.end()) {
		if (it->size() != 0) {
			float alpha = 100.0f;
			float step = 140.0f / it->size();
			for (unsigned int i = 0; i < it->size() - 1; ++i) {
				g_view->DrawLine2D((*it)[i], (*it)[i + 1], ((unsigned char)alpha << 24) | 0xC81414, 1.0f);
				alpha += step;
			}
		}
		++it;
	}
}

// shotreview.cpp:160
void BallTraceHistory::AddItem(unsigned int param1, unsigned int param2) {
	m_traces.push_back(*(const std::vector<WVector>*)param1);
}

// shotreview.cpp:166
void BallTraceHistory::PopBack() {
	if (!m_traces.empty()) {
		m_traces.pop_back();
	}
}

// shotreview.cpp:177
FirstBoundHistory::FirstBoundHistory() {
	m_pIndicator = g_resrcmng->GetOverlay("{indicator.bmp", 0);
	m_pBarFont = new CBarFont;
	m_pBarFont->SetColor(0xFFFFFFFF);
}

// shotreview.cpp:184
FirstBoundHistory::~FirstBoundHistory() {
	if (m_pBarFont) { delete m_pBarFont; m_pBarFont = 0; }
	if (g_resrcmng && m_pIndicator) { g_resrcmng->Release(m_pIndicator); m_pIndicator = 0; }
}

// shotreview.cpp:190
void FirstBoundHistory::Clear() {
	m_type0Points.clear();
	m_type1Points.clear();
}

// shotreview.cpp:196
void FirstBoundHistory::Display() {
	if (!(GOLFDOC()->GameMode() & 0x200)) {
		return;
	}
	std::vector<WVector>::iterator it;
	for (it = m_type1Points.begin(); it != m_type1Points.end(); ++it) {
		DrawIndicator(*it, 0xC8C8C8C8);
	}
	for (it = m_type0Points.begin(); it != m_type0Points.end(); ++it) {
		DrawIndicator(*it, 0xC8FF0000);
	}
}

// shotreview.cpp:210
void FirstBoundHistory::AddItem(unsigned int param1, unsigned int param2) {
	if (param1 == 0) {
		m_type0Points.push_back(*(const WVector*)param2);
	} else if (param1 == 1) {
		m_type1Points.push_back(*(const WVector*)param2);
	}
}

// shotreview.cpp:225
void FirstBoundHistory::PopBack() {
	if (!m_type0Points.empty()) {
		m_type0Points.pop_back();
	}
	if (!m_type1Points.empty()) {
		m_type1Points.pop_back();
	}
}

// shotreview.cpp:234
// Draws the indicator on the ground under pos and, above it, the distance
// from pos to the hole's cup.
void FirstBoundHistory::DrawIndicator(const WVector& pos, unsigned long color) {
	int groundIndex;
	WVector ground = GetPVS().GetGroundPoint(pos, true, &groundIndex, true);

	WTVertex vtx;
	g_view->Projection2(&vtx, ground);

	ground.x = vtx.x;
	ground.y = vtx.y;
	{
		WRECT dst = { ground.x - 10.0f, ground.y - 20.0f, 20.0f, 20.0f };
		WRECT src = { 0.0f, 0.0f, 1.0f, 1.0f };
		m_pIndicator->Render(g_view, src, dst, 0, color, 0, 0);
	}

	WVector ballPos = pos;

	WVector cupPos = GOLFDOC()->GetHoleData().m_cupPos;
	float distance = (float)sqrt((ballPos.x - cupPos.x) * (ballPos.x - cupPos.x) + (ballPos.z - cupPos.z) * (ballPos.z - cupPos.z));
	m_pBarFont->Print(ground.x, ground.y - 40.0f - 3.0f, MakeStr("%.1fy", distance * 0.3125f), TITLEFONT_ALIGN_CENTER, 0, 1.0f);
}

// shotreview.cpp:271 (run-time type and factory registration of ShotReview)
const WRTTI ShotReview::m_RTTI("ShotReview", &IActor::m_RTTI);

IObject *ShotReviewMakeInstance(void) {
	return new ShotReview;
}

struct __impShotReview {
	__impShotReview(void) {
		ObjectFactory().AddObjectFunctor(ShotReviewMakeInstance, "ShotReview");
		ActorFactory().AddActorClass("ShotReview");
	}
};
static __impShotReview s_impShotReview;

// shotreview.cpp:274
ShotReview::ShotReview(void) {
	m_show = true; m_enabled = true; AddSkipList(0x1000);

	m_priority = 4;
	CSceneManager::Instance()->RegisterElement((eObjType)5, 0, this, "ShotReviewScene", 0, 0);

	m_histories.push_back(new SpotHistory);
	m_histories.push_back(new BallTraceHistory);
	m_histories.push_back(new FirstBoundHistory);
}

// shotreview.cpp:285
ShotReview::~ShotReview(void) {
	std::for_each(m_histories.begin(), m_histories.end(), Delete_Object());
}

// shotreview.cpp:291
void ShotReview::OnInit(void) {
	std::for_each(m_histories.begin(), m_histories.end(), std::mem_fun(&IReviewHistory::Clear));
}

// shotreview.cpp:296
void ShotReview::OnProcess(float dt) {
	if (g_input->Get("LCONTROL", true) && g_input->GetDown("REPLAY", true)) {
		m_show = !m_show;
	}
}

// shotreview.cpp:302
void ShotReview::Display(void) {
	if (m_show) {
		std::for_each(m_histories.begin(), m_histories.end(), std::mem_fun(&IReviewHistory::Display));
	}
}
