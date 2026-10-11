// Original: d:\build\custom\temp\code\client\projectg\networkmonitor.cpp
#include "../client/ProjectG/networkmonitor.h"

#include <string.h>

#include "../client/Wangreal/include/wview.h"

// Graph placement and scale.
static const float kGraphX = 10.0f;
static const float kGraphY = 200.0f;
static const float kGraphHeight = 50.0f;
static const float kMaxBytes = 100.0f;   // bytes drawn at full height

// networkmonitor.cpp:25
CNetworkMonitor::CNetworkMonitor(void) : m_bShow(true) {
}

// networkmonitor.cpp:29
CNetworkMonitor::~CNetworkMonitor(void) {
}

// networkmonitor.cpp:33
void CNetworkMonitor::Init(void) {
	memset(m_inBytes, 0, sizeof(m_inBytes));
	memset(m_outBytes, 0, sizeof(m_outBytes));

	m_elapsed = 0;
	m_current = 0;
}

// networkmonitor.cpp:42
void CNetworkMonitor::TraceInBytes(unsigned int bytes) {
	m_inBytes[m_current] += bytes;
}

// networkmonitor.cpp:47
void CNetworkMonitor::TraceOutBytes(unsigned int bytes) {
	m_outBytes[m_current] += bytes;
}

// networkmonitor.cpp:52
void CNetworkMonitor::Render(WView* view) const {
	if (m_bShow) {
		for (int i = 0; i < HISTORY_SIZE; i++) {
			int index = (m_current + i + 1) % HISTORY_SIZE;
			float in = (float)m_inBytes[index];
			float out = (float)m_outBytes[index];

			in = (in < kMaxBytes) ? in : kMaxBytes;
			out = (out < kMaxBytes) ? out : kMaxBytes;

			in = in / kMaxBytes;

			float y = kGraphY + kGraphHeight - in * kGraphHeight;
			float x = kGraphX + i;
			_WPOINT a = { x, kGraphY + kGraphHeight + 1.0f };
			_WPOINT b = { x, y };
			view->DrawLine2D(a, b, 0xFFFF0000, 0);

			_WPOINT c = { x, y };
			_WPOINT d = { x, y - out / kMaxBytes * kGraphHeight - 1.0f };
			view->DrawLine2D(c, d, 0xFF0000FF, 0);
		}
	}
}

// networkmonitor.cpp:76
void CNetworkMonitor::Process(float elapsed) {
	m_elapsed += elapsed;
	if (m_elapsed >= 1.0f) {
		m_elapsed -= 1.0f;
		m_current = (m_current + 1) % HISTORY_SIZE;
		m_inBytes[m_current] = 0;
		m_outBytes[m_current] = 0;
	}
}
