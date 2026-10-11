// Original: d:\build\custom\temp\code\client\projectg\monitor.cpp
#include "../client/ProjectG/monitor.h"
#include "../client/Wangreal/include/woverlay.h"

// monitor.cpp:21
CMonitor::CMonitor(void)
	: m_boxColor(0xff000000), m_graphColor(0xffff0000), m_bVisible(true), m_nCur(0), m_nCount(0), m_nScale(1) {
}

// monitor.cpp:26
CMonitor::~CMonitor(void) {
	m_samples.clear();
}

// monitor.cpp:31
void CMonitor::Init(int count, int scale, WRect& rect) {
	m_nCur = 0;
	m_nCount = count;
	m_nScale = scale;
	m_rect = rect;

	m_samples.reserve(count);

	for (int i = 0; i < count; i++)
		m_samples[i] = 0;
}

// monitor.cpp:50
void CMonitor::Trace(int value) {
	if (value == -1) {
		if (m_nCur == 0) {
			if (m_nCount <= 0)
				m_samples[0] = 0;
			else
				m_samples[0] = m_samples[m_nCount - 1];
		} else {
			m_samples[m_nCur] = m_samples[m_nCur - 1];
		}
	} else {
		m_samples[m_nCur] = value;
	}
}

// monitor.cpp:68
void CMonitor::Process(float elapsed) {
	m_nCur = (m_nCur + 1) % m_nCount;
}

// monitor.cpp:73
bool CMonitor::Render(WView* view) const {
	if (!m_bVisible)
		return false;

	WOverlay::DrawLineBox(view, m_rect, 0, m_boxColor);
	return true;
}
