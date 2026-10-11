// Original: d:\build\custom\temp\code\client\projectg\guildformcontroller.cpp
#include <windows.h>
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer
#include <algorithm>

#include "guildformcontroller.h"
#include "frwnd.h"
#include "frbutton.h"
#include "fredit.h"
#include "s5/Utilities.h"
#include "../shared/globalgamedefine.h"
#include "guilddefine.h"

namespace _guild {

// The number of rows of one page of a paged guild list.
static const unsigned int PAGE_ROW_COUNT = 5;

// guildformcontroller.cpp:21
CGuildFormController::CGuildFormController(void) : m_formStyle((eFormStyle)0) {
}

// guildformcontroller.cpp:26
CGuildFormController::~CGuildFormController(void) {
	std::for_each(m_postActions.begin(), m_postActions.end(), Delete_SecondObject());
}

// guildformcontroller.cpp:32
void CGuildFormController::ControllForm(eFormStyle formStyle, eAuthority authority) {
	m_formStyle = formStyle;

	std::for_each(m_controls.begin(), m_controls.end(), S5::DisableFreshWindow);

	std::map<sPostAction*, sPostAction*>::iterator it = m_postActions.begin();
	for (; it != m_postActions.end(); ++it) {
		sPostAction* pAction = it->second;
		if (pAction->m_formStyle & formStyle) {
			if (pAction->m_authority & authority) {
				(*pAction)(this);
			}
		}
	}
}

// guildformcontroller.cpp:52
void CGuildFormController::InsertControl(FrWnd* pWnd) {
	m_controls.push_back(pWnd);
}

// guildformcontroller.cpp:58
void CGuildFormController::ReadOnlywindow(sPostAction* pAction) {
	std::map<sPostAction*, sPostAction*>::iterator it = m_postActions.find(pAction);
	if (it == m_postActions.end()) {
		return;
	}

	if (it->second->m_type == 4) {
		static_cast<FrEdit*>(pAction->m_pWnd)->SetReadOnly(true);
	}
}

// guildformcontroller.cpp:77
void CGuildFormController::WritableWindow(sPostAction* pAction) {
	std::map<sPostAction*, sPostAction*>::iterator it = m_postActions.find(pAction);
	if (it == m_postActions.end()) {
		return;
	}

	if (it->second->m_type == 4) {
		static_cast<FrEdit*>(pAction->m_pWnd)->SetReadOnly(false);
	}
}

// guildformcontroller.cpp:96
void CGuildFormController::EnableWindow(sPostAction* pAction) {
	S5::EnableFreshWindow(pAction->m_pWnd, true);
}

// guildformcontroller.cpp:101
void CGuildFormController::DisableWindow(sPostAction* pAction) {
	S5::EnableFreshWindow(pAction->m_pWnd, false);
}

// guildformcontroller.cpp:106
void CGuildFormController::MoveLeftSide(sPostAction* pAction) {
	FrWnd* pWnd = pAction->m_pWnd;

	WRect rect = pWnd->GetRect();
	rect.x = 145.0f;
	pWnd->SetRect(rect);
}

// guildformcontroller.cpp:115
void CGuildFormController::MoveRightSide(sPostAction* pAction) {
	FrWnd* pWnd = pAction->m_pWnd;

	WRect rect = pWnd->GetRect();
	rect.x = 375.0f;
	pWnd->SetRect(rect);
}

// guildformcontroller.cpp:124
void CGuildFormController::ClearEdit(sPostAction* pAction) {
	std::map<sPostAction*, sPostAction*>::iterator it = m_postActions.find(pAction);
	if (it == m_postActions.end()) {
		return;
	}

	if (it->second->m_type == 4) {
		static_cast<FrEdit*>(pAction->m_pWnd)->ClearLine();
	}
}

// guildformcontroller.cpp:143
void CGuildFormController::VisibleWindow(sPostAction* pAction) {
	pAction->m_pWnd->SetVisible(true);
}

// guildformcontroller.cpp:148
void CGuildFormController::InVisibleWindow(sPostAction* pAction) {
	pAction->m_pWnd->SetVisible(false);
}

// guildformcontroller.cpp:191
void CPageArrow::InitControl(void) {
	m_arrowFlags = 0;
	m_pLeftArrow->Enable(false);
	m_pRightArrow->Enable(false);
}

// guildformcontroller.cpp:158
void CPageArrow::RefreshArrowButton(int totalCount, int firstRow) {
	InitControl();

	if (firstRow > PAGE_ROW_COUNT) {
		m_arrowFlags |= 1;
	}

	if ((firstRow - 1) / PAGE_ROW_COUNT < (totalCount - 1) / PAGE_ROW_COUNT) {
		m_arrowFlags |= 2;
	}

	if (m_arrowFlags & 1) {
		m_pLeftArrow->Enable(true);
	}

	if ((m_arrowFlags >> 1) & 1) {
		m_pRightArrow->Enable(true);
	}
}

// guildformcontroller.cpp:199
int ConvertGuildHistory(GUILD_STATE_IDX state) {
	switch (state) {
	case 0:
		return 0;
	case 1:
		return 0x425;
	case 2:
		return 0x426;
	case 3:
		return 0x427;
	case 4:
		return 0x428;
	case 5:
		return 0x429;
	case 6:
		return 0x42a;
	case 7:
		return 0x42b;
	case 8:
		return 0x42c;
	case 9:
		return 0x42d;
	case 10:
		return 0x42e;
	case 11:
		return 0x42f;
	case 12:
		return 0x430;
	case 13:
		return 0x431;
	case 51:
		return 0x432;
	case 52:
		return 0x433;
	case 53:
		return 0x434;
	}

	return 0;
}

} // namespace _guild
