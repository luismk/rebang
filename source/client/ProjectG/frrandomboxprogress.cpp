// Original: d:\build\custom\temp\code\client\projectg\frrandomboxprogress.cpp
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "FrRandomBoxProgress.h"
#include "ObjectFactory.h"
#include "frgaugebar.h"
#include "frstatic.h"
#include "Packet.h"
#include "TaskManager.h"

// frrandomboxprogress.cpp:7 (run-time type and factory registration of FrRandomBoxProgress)
const WRTTI FrRandomBoxProgress::m_RTTI("FrRandomBoxProgress", &FrForm::m_RTTI);

IObject* FrRandomBoxProgressMakeInstance(void) {
	return new FrRandomBoxProgress;
}

struct __sFrRandomBoxProgress {
	__sFrRandomBoxProgress(void) {
		ObjectFactory().AddObjectFunctor(FrRandomBoxProgressMakeInstance, "FrRandomBoxProgress");
	}
};
static __sFrRandomBoxProgress s_registerFrRandomBoxProgress;

// frrandomboxprogress.cpp:9
const sFRESH_MSGMAP* FrRandomBoxProgress::GetMessageMap(void) const {
	return &FrRandomBoxProgress::_MsgMap;
}

sFRESH_MSGMAP FrRandomBoxProgress::_MsgMap = { &FrForm::_MsgMap, &FrRandomBoxProgress::_MsgEntries[0] };

sFRESH_ENTRY FrRandomBoxProgress::_MsgEntries[] = {
	// frrandomboxprogress.cpp:11
	{ "waitbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRandomBoxProgress::OnProgressInit },
	// frrandomboxprogress.cpp:12
	{ "cancle", FRCMD_LBUTTONUP, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRandomBoxProgress::OnCancelUp },
	// frrandomboxprogress.cpp:13
	{ "explane", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRandomBoxProgress::OnExplaneStatic_Init },

	// frrandomboxprogress.cpp:15
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frrandomboxprogress.cpp:18
FrRandomBoxProgress::FrRandomBoxProgress(void) {
	m_pWaitBar = 0;
	m_elapsed = 0;
	m_duration = 0;
	m_pExplaneStatic = 0;
}

// frrandomboxprogress.cpp:26
FrRandomBoxProgress::~FrRandomBoxProgress(void) {
}

// frrandomboxprogress.cpp:30
void FrRandomBoxProgress::SetExplane(const char* text) {
	if (text == 0) {
		return;
	}

	if (m_pExplaneStatic == 0) {
		return;
	}

	m_pExplaneStatic->Enable(true);
	m_pExplaneStatic->SetVisible(true);
	m_pExplaneStatic->SetCaption(text);
}

// frrandomboxprogress.cpp:52
void FrRandomBoxProgress::OnProgressInit(intptr_t param) {
	m_pWaitBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_pWaitBar->SetRange(0, 100, 0);
}

// frrandomboxprogress.cpp:58
void FrRandomBoxProgress::OnCancelUp(int param) {
	FrForm::OnCancel();
}

// frrandomboxprogress.cpp:63
void FrRandomBoxProgress::OnExplaneStatic_Init(intptr_t param) {
	m_pExplaneStatic = DYNAMIC_CAST(FrStatic, param);
	if (m_pExplaneStatic) {
		m_pExplaneStatic->SetVisible(false);
		m_pExplaneStatic->Enable(false);
	}
}

// frrandomboxprogress.cpp:73
bool FrRandomBoxProgress::OnInit(void) {
	const char* text = m_pExplaneStatic->GetCaption();
	if (strlen(text) != 0) {
		FrGaugeBar* pWaitBar = m_pWaitBar;
		WRect rect = pWaitBar->GetRect();

		rect.y += 15.0f;
		pWaitBar->SetRect(rect);
	}

	srand(GetTickCount());
	m_duration = (float)(rand() % 4 + 4);

	return true;
}

// frrandomboxprogress.cpp:97
void FrRandomBoxProgress::OnProc(const float dt) {
	m_elapsed += dt;

	if (m_elapsed > m_duration) {
		m_pWaitBar->SetPos(100);

		AfxGetTask()->PassMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
		WSendPacket packet((enumClientPacket)0xF1);
		packet.Encode4(m_boxTid);
		packet.Send(SEND_TO_DEFAULT);

		m_elapsed = 0;
		m_duration = 0;
		FrForm::OnCancel();
	} else {
		int percent = (int)(m_elapsed / m_duration * 100.0f);
		if (percent < 100) {
			m_pWaitBar->SetPos(percent);
		}
	}
}
