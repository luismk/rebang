// Original: d:\build\custom\temp\code\client\projectg\obmarker.cpp
#include "obmarker.h"

#include <string>

#include "SharedDoc.h"
#include "quadtree.h"
#include "w3dspr.h"
#include "wresrcmng.h"
#include "wview.h"

// obmarker.cpp:21
CMarker::CMarker(void) : m_pSpr(NULL) {
}

// obmarker.cpp:25
CMarker::~CMarker(void) {
	Reset();
}

// obmarker.cpp:30
void CMarker::Reset(void) {
	m_list.clear();

	if (m_pSpr != NULL) { delete m_pSpr; m_pSpr = NULL; }
}

// obmarker.cpp:40
void CMarker::AddList(const std::vector<WVector>& list) {
	WVector point;
	int area;

	for (unsigned int i = 0; i < list.size(); i++) {
		point = GetPVS().GetGroundPoint(list[i], true, &area, false);

		if (area >= 0)
			m_list.push_back(point);
	}
}

// obmarker.cpp:55
void CMarker::Display(void) {
	int count = m_list.size();
	if (count == 0)
		return;
	if (m_pSpr == NULL)
		return;

	for (unsigned int i = 0; i < m_list.size(); i++) {
		m_pSpr->SetPos(m_list[i]);
		m_pSpr->Render(g_view, 0, W3dSpr::CAMERA_X_ALIGN);
	}
}

// obmarker.cpp:76
void CMarker::GetReady(void) {
	int count = m_list.size();
	if (count == 0)
		return;

	std::string fileName;

	switch (GetCurMap()) {
	case 9:
		fileName = "ob_counter03.jpg";
		break;
	case 13:
		fileName = "ob_counter04.jpg";
		break;
	default:
		fileName = "ob_counter01.jpg";
		break;
	}

	m_pSpr = g_resrcmng->Get3DSpr(fileName.c_str(), 0);
	m_pSpr->SetRect(1.0f, 2.0f, 0.5f, 0.0f);
}
