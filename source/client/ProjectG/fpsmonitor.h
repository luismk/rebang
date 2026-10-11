// Original: d:\build\custom\temp\code\client\projectg\fpsmonitor.h
#ifndef PG_CLIENT_PROJECTG_FPSMONITOR_H
#define PG_CLIENT_PROJECTG_FPSMONITOR_H

#include "..\..\defines.h"
#include "../client/ProjectG/monitor.h"
#include "../client/Wangreal/include/wview.h"

// The frame-rate graph: draws the samples as a line, oldest first.
class CFpsMonitor : public CMonitor {
public:
	CFpsMonitor(void) {}            // fpsmonitor.h:19
	virtual ~CFpsMonitor(void) {}   // fpsmonitor.h:20

	// fpsmonitor.h:24
	virtual bool Render(WView* view) const {
		if (!CMonitor::Render(view))
			return false;

		int prev = -1;
		for (int i = 0; i < m_nCount; i++) {
			int cur = m_samples[(m_nCur + i - 1) % m_nCount];
			int last = (prev == -1) ? cur : prev;
			prev = cur;
			float yCur = m_rect.y + m_rect.h - (float)(cur < m_nScale ? cur : m_nScale) / m_nScale * m_rect.h;
			float yLast = m_rect.y + m_rect.h - (float)(last < m_nScale ? last : m_nScale) / m_nScale * m_rect.h;
			view->DrawLine2D(WPoint(m_rect.x + i - 1.0f, yLast), WPoint(m_rect.x + i, yCur), m_graphColor, 0);
		}
		return true;
	}
};
CHECK_SIZE(CFpsMonitor, 0x3c);

#endif // PG_CLIENT_PROJECTG_FPSMONITOR_H
