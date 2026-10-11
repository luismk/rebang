// Original: d:\build\custom\temp\code\client\projectg\monitor.h
#ifndef PG_CLIENT_PROJECTG_MONITOR_H
#define PG_CLIENT_PROJECTG_MONITOR_H

#include <vector>

#include "..\..\defines.h"
#include "../client/Wangreal/include/wtypes.h"

class WView;

// A ring buffer of integer samples drawn as a graph inside a box
// (monitor.cpp). Vtable: ~CMonitor, Init, Trace, Process, Render.
class CMonitor {
	LAYOUT_CHECK_FRIEND;
public:
	CMonitor(void);
	virtual ~CMonitor(void);

	virtual void Init(int count, int scale, WRect& rect);
	virtual void Trace(int value);
	virtual void Process(float elapsed);
	virtual bool Render(WView* view) const;

	// monitor.h:31
	void SetColor(unsigned long boxColor, unsigned long graphColor) {
		m_boxColor = boxColor;
		m_graphColor = graphColor;
	}

protected:
	unsigned long m_boxColor;    // 0x4
	unsigned long m_graphColor;  // 0x8
	bool m_bVisible;             // 0xc
	int m_nCur;                  // 0x10 slot of the current sample
	int m_nCount;                // 0x14 number of samples
	int m_nScale;                // 0x18 sample value drawn at the full height
	WRect m_rect;                // 0x1c
	std::vector<int> m_samples;  // 0x2c
};
CHECK_SIZE(CMonitor, 0x3c);
BEGIN_LAYOUT_CHECK(CMonitor)
	CHECK_FIELD(CMonitor, m_boxColor, 0x4);
	CHECK_FIELD(CMonitor, m_graphColor, 0x8);
	CHECK_FIELD(CMonitor, m_bVisible, 0xc);
	CHECK_FIELD(CMonitor, m_nCur, 0x10);
	CHECK_FIELD(CMonitor, m_nCount, 0x14);
	CHECK_FIELD(CMonitor, m_nScale, 0x18);
	CHECK_FIELD(CMonitor, m_rect, 0x1c);
	CHECK_FIELD(CMonitor, m_samples, 0x2c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MONITOR_H
