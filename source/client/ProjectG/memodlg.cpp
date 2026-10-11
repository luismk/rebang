// Original: d:\build\custom\temp\code\client\projectg\memodlg.cpp
#include <string.h>

#include "MemoDlg.h"
#include "ObjectFactory.h"
#include "GolfTask.h"
#include "TaskManager.h"
#include "fresh.h"
#include "fredit.h"
#include "frwndmanager.h"
#include "frdesktop.h"
#include "SharedDoc.h"

// memodlg.cpp:17 (run-time type and factory registration of FrMemoDlg)
const WRTTI FrMemoDlg::m_RTTI("FrMemoDlg", &FrForm::m_RTTI);

IObject* FrMemoDlgMakeInstance(void) {
	return new FrMemoDlg;
}

struct __sFrMemoDlg {
	__sFrMemoDlg(void) {
		ObjectFactory().AddObjectFunctor(FrMemoDlgMakeInstance, "FrMemoDlg");
	}
};
static __sFrMemoDlg s_registerFrMemoDlg;

// memodlg.cpp:19
const sFRESH_MSGMAP* FrMemoDlg::GetMessageMap(void) const {
	return &FrMemoDlg::_MsgMap;
}

sFRESH_MSGMAP FrMemoDlg::_MsgMap = { &FrForm::_MsgMap, &FrMemoDlg::_MsgEntries[0] };

sFRESH_ENTRY FrMemoDlg::_MsgEntries[] = {
	// memodlg.cpp:21
	{ "memo", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMemoDlg::OnMemoInit },
	// memodlg.cpp:22
	{ "memo", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMemoDlg::OnMemoLButtonDown },
	// memodlg.cpp:23
	{ "memo", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrMemoDlg::OnMemoEnterKey },
	// memodlg.cpp:24
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMemoDlg::OnOkBtnUp },

	// memodlg.cpp:26
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// memodlg.cpp:29
void FrMemoDlg::OnMemoInit(intptr_t param) {
	m_pMemo = DYNAMIC_CAST(FrEdit, param);
	m_pMemo->SetCharLimit(10, false);
	m_pMemo->SetKeyFocus(true);

	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		g_pFresh->GetManager()->SetExclusiveKey(true);
	}
}

// memodlg.cpp:42
void FrMemoDlg::OnMemoLButtonDown(void) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		g_pFresh->GetManager()->SetExclusiveKey(true);
	}
}

// memodlg.cpp:51
bool FrMemoDlg::OnMemoEnterKey(int param) {
	if (!m_pMemo->GetStyle().GetFlag(0x2)) {
		OnOkBtnUp();
	}

	return false;
}

// memodlg.cpp:59
void FrMemoDlg::OnOkBtnUp(void) {
	if (strlen(m_pMemo->GetLine(1, false)) == 0) {
		m_pMemo->SetLine(1, "Friend", 0, false, 0);
	}

	OnFreshOkay();
}

// memodlg.cpp:67
const char* FrMemoDlg::GetMemo(void) {
	if (!m_pMemo) {
		return 0;
	}

	const char* text = m_pMemo->GetLine(1, false);
	WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringHack(text, true);

	return text;
}

// memodlg.cpp:91
void FrMemoDlg::OnProc(const float dt) {
	if (!m_bFocused) {
		if (m_elapsed > 0.3f) {
			m_pWndManager->GetDesktop()->ResetKeyFocus();
			m_pMemo->SetKeyFocus(true);
			m_bFocused = true;
		}

		m_elapsed += dt;
	}
}
