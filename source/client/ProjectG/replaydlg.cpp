// Original: d:\build\custom\temp\code\client\projectg\replaydlg.cpp
#include "ReplayDlg.h"
#include "ObjectFactory.h"
#include "frbutton.h"
#include "SharedDoc.h"
#include "InputManager.h"
#include "mousecursor.h"
#include "wview.h"

// replaydlg.h:109
int CReplayControlManager::GetFadeType(void) {
	return m_nFadeType;
}

// replaydlg.h:110
void CReplayControlManager::SetFadeType(int nFadeType) {
	m_nFadeType = nFadeType;
}

// replaydlg.h:112
WRect CReplayControlManager::GetControlRect(void) {
	return m_rcControl;
}

// replaydlg.h:113
void CReplayControlManager::SetControlRect(WRect rc) {
	m_rcControl = rc;
}

// replaydlg.cpp:20 (run-time type and factory registration of FrReplayControlDlg)
const WRTTI FrReplayControlDlg::m_RTTI("FrReplayControlDlg", &FrForm::m_RTTI);

IObject* FrReplayControlDlgMakeInstance(void) {
	return new FrReplayControlDlg;
}

struct __sFrReplayControlDlg {
	__sFrReplayControlDlg(void) {
		ObjectFactory().AddObjectFunctor(FrReplayControlDlgMakeInstance, "FrReplayControlDlg");
	}
};
static __sFrReplayControlDlg s_registerFrReplayControlDlg;

// replaydlg.cpp:22
const sFRESH_MSGMAP* FrReplayControlDlg::GetMessageMap(void) const {
	return &FrReplayControlDlg::_MsgMap;
}

// replaydlg.cpp:44
void FrReplayControlDlg::Init(void) {
}

// replaydlg.cpp:49
FrReplayControlDlg::FrReplayControlDlg(void) {
	m_pPlay = 0;

	m_alpha = 0.0f;
	m_bPlaying = true;
}

// replaydlg.cpp:56
FrReplayControlDlg::~FrReplayControlDlg(void) {
}

// replaydlg.cpp:61
bool FrReplayControlDlg::OnInit(void) {
	FrForm::OnInit();

	EnableDrag(false);
	return true;
}

// replaydlg.cpp:69
void FrReplayControlDlg::OnPlayInit(intptr_t param) {
	m_pPlay = DYNAMIC_CAST(FrButton, param);
}

// replaydlg.cpp:74
void FrReplayControlDlg::TogglePlayButton(void) {
	if (m_bPlaying) {
		WSingleton<CSharedDoc>::Instance()->m_replayState = 4;
		m_pPlay->SetButtonImg("replay_play_n", FrButton::NORMAL);
		m_pPlay->SetButtonImg("replay_btn_o_big", FrButton::OVER);
		m_pPlay->SetButtonImg("replay_play_n", FrButton::PRESSED);
		m_pPlay->SetButtonBelowImg("replay_play_n", FrButton::OVER);
	} else {
		if (WSingleton<CSharedDoc>::Instance()->m_replayStartB < 2) {
			WSingleton<CSharedDoc>::Instance()->m_replayState = 1;
		} else {
			WSingleton<CSharedDoc>::Instance()->m_replayState = 2;
		}
		m_pPlay->SetButtonImg("replay_stop_n", FrButton::NORMAL);
		m_pPlay->SetButtonImg("replay_btn_o_big", FrButton::OVER);
		m_pPlay->SetButtonImg("replay_stop_n", FrButton::PRESSED);
		m_pPlay->SetButtonBelowImg("replay_stop_n", FrButton::OVER);
	}

	m_bPlaying = !m_bPlaying;
}

// replaydlg.cpp:104
void FrReplayControlDlg::OnPlayBtnUp(void) {
	TogglePlayButton();
}

// replaydlg.cpp:109
void FrReplayControlDlg::OnBeforeShotBtnInit(int param) {
	m_pBeforeShotBtn = DYNAMIC_CAST(FrButton, param);
	m_pBeforeShotBtn->Enable(false);
}

// replaydlg.cpp:120
void FrReplayControlDlg::OnBeforeShotBtnUp(void) {
	if (!m_bPlaying) {
		TogglePlayButton();
	}

	WSingleton<CSharedDoc>::Instance()->m_replayState = 11;
}

// replaydlg.cpp:130
void FrReplayControlDlg::OnNextShotBtnInit(int param) {
	m_pNextShotBtn = DYNAMIC_CAST(FrButton, param);
	m_pNextShotBtn->Enable(false);
}

// replaydlg.cpp:140
void FrReplayControlDlg::OnNextShotBtnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_replayPlayType == 2) {
		if (!m_bPlaying) {
			TogglePlayButton();
		}

		WSingleton<CSharedDoc>::Instance()->m_replayState = 12;
	}
}

// replaydlg.cpp:155
void FrReplayControlDlg::OnExitBtnInit(intptr_t param) {
	m_pExitBtn = DYNAMIC_CAST(FrButton, param);
}

// replaydlg.cpp:160
void FrReplayControlDlg::OnExitBtnUp(void) {
	WSingleton<CSharedDoc>::Instance()->m_replayStartB = 0;
	WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags = WSingleton<CSharedDoc>::Instance()->m_replayChannelFlags;
	WSingleton<CSharedDoc>::Instance()->m_replayChannelFlags = 0;

	WSingleton<CSharedDoc>::Instance()->m_replayPlayType = 0;

	WSingleton<CSharedDoc>::Instance()->m_replayState = 5;
	Close(true);
}

// replaydlg.cpp:172
void FrReplayControlDlg::OnMinimizeBtnInit(int param) {
	m_pMinimizeBtn = DYNAMIC_CAST(FrButton, param);
}

// replaydlg.cpp:177
void FrReplayControlDlg::OnMinimizeBtnUp(void) {
	CMouseCursor::Instance()->ForceMove(g_view->GetWidth() * 0.5f, g_view->GetHeight() * 0.5f, -1.0f);
	SetVisible(false);
}

// replaydlg.cpp:184
void FrReplayControlDlg::OnRecordBtnInit(int param) {
	m_pRecordBtn = DYNAMIC_CAST(FrButton, param);
	m_pRecordBtn->Enable(false);

	if (WSingleton<CSharedDoc>::Instance()->m_replayState == 9) {
		m_pRecordBtn->Enable(false);
	}
}

// replaydlg.cpp:197
void FrReplayControlDlg::OnRecordBtnUp(void) {
	WSingleton<CSharedDoc>::Instance()->m_replayState = 6;
}

// replaydlg.cpp:230
void FrReplayControlDlg::OnProc(const float dt) {
	bool bInArea = CMouseCursor::Instance()->InArea(m_rect);

	if (bInArea) {
		m_alpha += dt * 2;
		if (m_alpha > 1.0f) {
			m_alpha = 1.0f;
		}
	} else {
		m_alpha -= dt * 2;
		if (m_alpha < 0.6f) {
			m_alpha = 0.6f;
		}
	}

	SetAlpha2ToChild(m_alpha);
}

// replaydlg.cpp:262
void CReplayControlManager::Init(void) {
	m_rcControl.x = g_view->GetWidth();
	m_rcControl.y = g_view->GetHeight() - 560.0f;
	m_rcControl.w = 335.0f;
	m_rcControl.h = 101.0f;

	m_fadeAlpha = 0.7f;
	m_fadeTime = 0.0f;
}

// replaydlg.cpp:278
CReplayControlManager::CReplayControlManager(void) {
	m_nFadeType = 0;
	m_rcControl.x = 10.0f;
	m_rcControl.y = 10.0f;
	m_rcControl.w = 335.0f;
	m_rcControl.h = 101.0f;

	m_fadeAlpha = 0.5f;
	m_fadeTime = 0.0f;
}

// replaydlg.cpp:296
CReplayControlManager::~CReplayControlManager(void) {
}

// replaydlg.cpp:304
bool CReplayControlManager::CalcReplayControlPos(void) {
	WRect rc = m_rcControl;
	WVector mousePos = g_input->GetMousePoint();

	float showX = g_view->GetWidth() - 340.0f;
	if (showX < mousePos.x && mousePos.x < g_view->GetWidth() && 0.0f < mousePos.y && mousePos.y < 140.0f) {
		if (rc.x > showX) {
			rc.x -= 15.0f;
		}
		m_rcControl = WRect(rc.x, rc.y, rc.w, rc.h);

		if (500.0f < rc.x) {
			m_nFadeType = 2;
		}
		return true;
	} else if (m_nFadeType == 2) {
		if (rc.x >= g_view->GetWidth()) {
			m_nFadeType = 0;
		} else {
			m_nFadeType = 2;
		}

		m_rcControl = WRect(rc.x + 15.0f, rc.y, rc.w, rc.h);
		return true;
	}

	return false;
}
