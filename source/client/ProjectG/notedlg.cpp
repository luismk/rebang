// Original: d:\build\custom\temp\code\client\projectg\notedlg.cpp
#include <string.h>

#include "NoteDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "fredit.h"
#include "frstatic.h"
#include "frframe.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frdesktop.h"
#include "commonutil.h"
#include "TaskManager.h"
#include "actor.h"
#include "GolfTask.h"
#include "SharedDoc.h"

// notedlg.cpp:20 (run-time type and factory registration of FrNoteDlg)
const WRTTI FrNoteDlg::m_RTTI("FrNoteDlg", &FrForm::m_RTTI);

IObject* FrNoteDlgMakeInstance(void) {
	return new FrNoteDlg;
}

struct __sFrNoteDlg {
	__sFrNoteDlg(void) {
		ObjectFactory().AddObjectFunctor(FrNoteDlgMakeInstance, "FrNoteDlg");
	}
};
static __sFrNoteDlg s_registerFrNoteDlg;

// notedlg.cpp:22
const sFRESH_MSGMAP* FrNoteDlg::GetMessageMap(void) const {
	return &FrNoteDlg::_MsgMap;
}

sFRESH_MSGMAP FrNoteDlg::_MsgMap = { &FrForm::_MsgMap, &FrNoteDlg::_MsgEntries[0] };

sFRESH_ENTRY FrNoteDlg::_MsgEntries[] = {
	// notedlg.cpp:24
	{ "paid", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoteDlg::OnPaidInit },
	// notedlg.cpp:25
	{ "nick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoteDlg::OnNickInit },
	// notedlg.cpp:26
	{ "snick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoteDlg::OnNickStaticInit },
	// notedlg.cpp:27
	{ "received", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoteDlg::OnReceivedInit },
	// notedlg.cpp:28
	{ "sreceived", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoteDlg::OnReceivedStaticInit },
	// notedlg.cpp:29
	{ "note", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoteDlg::OnNoteInit },
	// notedlg.cpp:30
	{ "note", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNoteDlg::OnNoteLBtnDown },
	// notedlg.cpp:31
	{ "note", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrNoteDlg::OnNoteEnterKey },
	// notedlg.cpp:32
	{ "snote", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoteDlg::OnNoteStaticInit },
	// notedlg.cpp:33
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrNoteDlg::OnOKBtnUp },

	// notedlg.cpp:35
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// notedlg.cpp:38
void FrNoteDlg::OnPaidInit(intptr_t param) {
	m_pPaid = DYNAMIC_CAST(FrArea, param);
}

// notedlg.cpp:43
void FrNoteDlg::OnNoteInit(intptr_t param) {
	m_pNote = DYNAMIC_CAST(FrEdit, param);

	// Fresh's kind-of test: like DYNAMIC_CAST, through __rtti_obj (frwnd.h).
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		g_pFresh->GetManager()->SetExclusiveKey(true);
	}
}

// notedlg.cpp:53
void FrNoteDlg::OnNoteLBtnDown(void) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		g_pFresh->GetManager()->SetExclusiveKey(true);
	}
}

// notedlg.cpp:61
bool FrNoteDlg::OnNoteEnterKey(int param) {
	if (!m_pNote->GetStyle().GetFlag(0x2)) {
		OnOKBtnUp();
	}

	return false;
}

// notedlg.cpp:69
void FrNoteDlg::OnNoteStaticInit(intptr_t param) {
	m_pNoteStatic = DYNAMIC_CAST(FrStatic, param);
}

// notedlg.cpp:74
void FrNoteDlg::OnNickInit(intptr_t param) {
	m_pNick = DYNAMIC_CAST(FrEdit, param);
}

// notedlg.cpp:79
void FrNoteDlg::OnNickStaticInit(intptr_t param) {
	m_pNickStatic = DYNAMIC_CAST(FrStatic, param);
}

// notedlg.cpp:84
void FrNoteDlg::OnReceivedInit(intptr_t param) {
	m_pReceived = DYNAMIC_CAST(FrEdit, param);
}

// notedlg.cpp:89
void FrNoteDlg::OnReceivedStaticInit(intptr_t param) {
	m_pReceivedStatic = DYNAMIC_CAST(FrStatic, param);
}

// notedlg.cpp:94
void FrNoteDlg::SetID(const char* nick, const char* received) {
	m_pNickStatic->SetCaption("\xBE\xC6\xC0\xCC\xB5\xF0");

	SetNick(nick, received);

	if (m_pBaseFrm) {
		m_pBaseFrm->SetDesc("");
	}
}

// notedlg.cpp:104
void FrNoteDlg::SetNick(const char* nick, const char* received) {
	if (m_pNick) {
		m_pNick->SetLine(1, nick, 0, false, 0);
	}

	if (received) {

		if (m_pReceived) {
			m_pReceived->SetLine(1, received, 0, false, 0);
		}

		if (m_pNote) {
			WRect rect;
			m_pNote->GetClientRect(rect);

			rect.y += 30.0f;
			m_pNote->SetClientRect(rect);
		}

		if (m_pNoteStatic) {
			WRect rect;
			m_pNoteStatic->GetClientRect(rect);

			rect.y += 30.0f;
			m_pNoteStatic->SetClientRect(rect);
		}

	} else {
		if (m_pReceived) {
			m_pReceived->SetVisible(false);
		}
		if (m_pReceivedStatic) {
			m_pReceivedStatic->SetVisible(false);
		}
		if (m_pPaid) {
			m_pPaid->SetVisible(false);
		}
	}

	if (m_pBaseFrm) {
		m_pBaseFrm->SetDesc(MakeStr("\xC2\xCA\xC1\xF6\xB8\xA6 \xBA\xB8\xB3\xBB\xB8\xE9 \xBF\xEC\xC7\xA5\xB0\xAA\xC0\xB8\xB7\xCE %d\xC6\xCE\xC0\xCC \xBC\xD2\xBA\xF1\xB5\xCB\xB4\xCF\xB4\xD9.", 10));
	}
}

// notedlg.cpp:145
const char* FrNoteDlg::GetNote(void) {
	if (!m_pNote) {
		return 0;
	}

	const char* text = m_pNote->GetLine(1, false);
	WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringHack(text, true);

	return text;
}

// notedlg.cpp:156
void FrNoteDlg::OnOKBtnUp(void) {
	if (strlen(m_pNote->GetLine(1, false)) == 0) {

		AfxGetTask()->PassMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\xC2\xCA\xC1\xF6 \xB3\xBB\xBF\xEB\xC0\xBB \xC0\xD4\xB7\xC2\xC7\xD8\xC1\xD6\xBC\xBC\xBF\xE4.", 0, 0, 0, 0));
		return;

	}

	OnFreshOkay();
}

// notedlg.cpp:180
void FrNoteDlg::OnProc(const float dt) {
	if (!m_bFocused) {

		if (m_elapsed > 0.3f) {

			m_pWndManager->GetDesktop()->ResetKeyFocus();
			m_pNote->SetKeyFocus(true);
			m_bFocused = true;
		}

		m_elapsed += dt;
	}
}
