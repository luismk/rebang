// Original: d:\build\custom\temp\code\client\projectg\tooltipmanager.h
#ifndef PG_CLIENT_PROJECTG_TOOLTIPMANAGER_H
#define PG_CLIENT_PROJECTG_TOOLTIPMANAGER_H

#include "..\..\defines.h"
#include "ToolTip.h"
#include <vector>

class FrWnd;

// A window registered with CToolTipManager::Add.
struct sToolTip {
	FrWnd** ppWnd;       // 0x0
	eBarButton button;   // 0x4
};

// Shows the tool tip of the windows registered with Add (each by the address
// of the window pointer, so the window may be created later).
class CToolTipManager {
	LAYOUT_CHECK_FRIEND;
public:
	CToolTipManager(void);
	~CToolTipManager(void);

	void Add(FrWnd** ppWnd, eBarButton button);
	void Process(void);

private:
	std::vector<sToolTip> m_toolTips;   // 0x0
};

CHECK_SIZE(sToolTip, 0x8);
CHECK_FIELD(sToolTip, ppWnd, 0x0);
CHECK_FIELD(sToolTip, button, 0x4);
CHECK_SIZE(CToolTipManager, 0x10);
BEGIN_LAYOUT_CHECK(CToolTipManager)
	CHECK_FIELD(CToolTipManager, m_toolTips, 0x0);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_TOOLTIPMANAGER_H
