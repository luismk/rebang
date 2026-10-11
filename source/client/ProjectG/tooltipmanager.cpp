// Original: d:\build\custom\temp\code\client\projectg\tooltipmanager.cpp
#include "ToolTipManager.h"
#include "fresh.h"
#include "frwnd.h"
#include "frwndmanager.h"

// tooltipmanager.cpp:8
CToolTipManager::CToolTipManager(void) {
}

// tooltipmanager.cpp:12
CToolTipManager::~CToolTipManager(void) {
	TOOLTIP()->ClearToolTip();
}

// tooltipmanager.cpp:17
void CToolTipManager::Add(FrWnd** ppWnd, eBarButton button) {
	sToolTip toolTip;
	toolTip.ppWnd = ppWnd;
	toolTip.button = button;

	m_toolTips.push_back(toolTip);
}

// tooltipmanager.cpp:26
void CToolTipManager::Process(void) {
	const WPoint& mousePos = g_pFresh->GetManager()->GetMousePos();

	bool bShow = false;

	for (unsigned int i = 0; i < m_toolTips.size(); ++i) {
		WRect rect = (*m_toolTips[i].ppWnd)->GetRect();

		if (rect.x <= mousePos.x && mousePos.x <= rect.x + rect.w && rect.y <= mousePos.y && mousePos.y <= rect.y + rect.h) {
			bShow = true;
			TOOLTIP()->SetToolTip(m_toolTips[i].button);
		}
	}

	if (!bShow)
		TOOLTIP()->ClearToolTip();
	else
		TOOLTIP()->Render();
}
