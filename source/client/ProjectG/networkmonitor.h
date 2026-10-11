// Original: d:\build\custom\temp\code\client\projectg\networkmonitor.h
#ifndef PG_CLIENT_PROJECTG_NETWORKMONITOR_H
#define PG_CLIENT_PROJECTG_NETWORKMONITOR_H

#include "..\..\defines.h"
#include "../client/Wangreal/include/singleton.h"

class WView;

// The network traffic graph: bytes received and sent per second over the
// last 30 seconds (a ring buffer advanced once a second by Process).
class CNetworkMonitor : public WSingleton<CNetworkMonitor> {
	LAYOUT_CHECK_FRIEND;

public:
	CNetworkMonitor(void);
	virtual ~CNetworkMonitor(void);

	void Init(void);
	void TraceInBytes(unsigned int bytes);
	void TraceOutBytes(unsigned int bytes);
	void Render(WView* view) const;
	void Process(float elapsed);

private:
	enum { HISTORY_SIZE = 30 };

	bool m_bShow;                                // 0x28
	int m_current;                               // 0x2C
	float m_elapsed;                             // 0x30
	unsigned int m_inBytes[HISTORY_SIZE];        // 0x34
	unsigned int m_outBytes[HISTORY_SIZE];       // 0xAC
};
CHECK_SIZE(CNetworkMonitor, 0x124);
BEGIN_LAYOUT_CHECK(CNetworkMonitor)
	CHECK_FIELD(CNetworkMonitor, m_bShow, 0x28);
	CHECK_FIELD(CNetworkMonitor, m_current, 0x2C);
	CHECK_FIELD(CNetworkMonitor, m_elapsed, 0x30);
	CHECK_FIELD(CNetworkMonitor, m_inBytes, 0x34);
	CHECK_FIELD(CNetworkMonitor, m_outBytes, 0xAC);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_NETWORKMONITOR_H
