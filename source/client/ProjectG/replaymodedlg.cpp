// Original: d:\build\custom\temp\code\client\projectg\replaymodedlg.cpp
#include "ReplayModeDlg.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frbutton.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "actor.h"

#include <string.h>

// replaymodedlg.cpp:20 (run-time type and factory registration of FrReplayModeDlg)
const WRTTI FrReplayModeDlg::m_RTTI("FrReplayModeDlg", &FrForm::m_RTTI);

IObject* FrReplayModeDlgMakeInstance(void) {
	return new FrReplayModeDlg;
}

struct __sFrReplayModeDlg {
	__sFrReplayModeDlg(void) {
		ObjectFactory().AddObjectFunctor(FrReplayModeDlgMakeInstance, "FrReplayModeDlg");
	}
};
static __sFrReplayModeDlg s_registerFrReplayModeDlg;

// replaymodedlg.cpp:22
const sFRESH_MSGMAP* FrReplayModeDlg::GetMessageMap(void) const {
	return &FrReplayModeDlg::_MsgMap;
}

sFRESH_MSGMAP FrReplayModeDlg::_MsgMap = { &FrForm::_MsgMap, &FrReplayModeDlg::_MsgEntries[0] };

sFRESH_ENTRY FrReplayModeDlg::_MsgEntries[] = {
	// replaymodedlg.cpp:24
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrReplayModeDlg::OnOkInit },
	// replaymodedlg.cpp:25
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrReplayModeDlg::OnOkBtnUp },
	// replaymodedlg.cpp:26
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrReplayModeDlg::OnCancelInit },
	// replaymodedlg.cpp:27
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrReplayModeDlg::OnCancelBtnUp },

	// replaymodedlg.cpp:29
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// replaymodedlg.cpp:33
FrReplayModeDlg::FrReplayModeDlg(void) {
	m_pOkBtn = 0;
	m_pCancelBtn = 0;
	m_replayMode = 0;
}

// replaymodedlg.cpp:40
void FrReplayModeDlg::OnOkInit(intptr_t param) {
	m_pOkBtn = DYNAMIC_CAST(FrButton, param);
}

// replaymodedlg.cpp:45
void FrReplayModeDlg::OnOkBtnUp(void) {
	if (m_replayMode == 1) {
		// Play the replay.
		WSingleton<CSharedDoc>::Instance()->m_replayStartB = 0;
		WSingleton<CSharedDoc>::Instance()->m_replayStartA = 0;
		WSingleton<CSharedDoc>::Instance()->m_replayChannelFlags = WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags;
		WSingleton<CSharedDoc>::Instance()->m_replayState = 14;
	}
	else if (m_replayMode == 2) {
		// Delete the replay file.
		for (std::list<sReplayFileInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_replayFileList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_replayFileList.end(); ++it) {
			if (!stricmp(it->m_fileName, WSingleton<CSharedDoc>::Instance()->m_fileName)) {
				WSingleton<CSharedDoc>::Instance()->m_replayFileList.erase(it);

				CTaskManager *taskManager;
				if (DeleteFileA(WSingleton<CSharedDoc>::Instance()->m_fileName)) {
					taskManager = WSingleton<CTaskManager>::Instance();
					(taskManager == NULL ? NULL : taskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(NULL, 0x23, (intptr_t)"\xC1\xA4\xBB\xF3\xC0\xFB\xC0\xB8\xB7\xCE \xBB\xE8\xC1\xA6\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0));
				}
				else {
					taskManager = WSingleton<CTaskManager>::Instance();
					(taskManager == NULL ? NULL : taskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(NULL, 0x23, (intptr_t)"\xBB\xE8\xC1\xA6\xC7\xCF\xB4\xC2 \xB5\xB5\xC1\xDF \xBF\xC0\xB7\xF9\xB0\xA1 \xB9\xDF\xBB\xFD\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0));
				}
				break;
			}
		}
	}

	m_replayMode = 0;
	OnFreshOkay();
}

// replaymodedlg.cpp:82
void FrReplayModeDlg::OnCancelInit(intptr_t param) {
	m_pCancelBtn = DYNAMIC_CAST(FrButton, param);
}

// replaymodedlg.cpp:87
void FrReplayModeDlg::OnCancelBtnUp(void) {
	OnFreshCancel();
}

// replaymodedlg.cpp:92
void FrReplayModeDlg::SetReplayMode(int mode) {
	m_replayMode = mode;
}

// replaymodedlg.cpp:97
int FrReplayModeDlg::GetReplayMode(void) {
	return m_replayMode;
}
