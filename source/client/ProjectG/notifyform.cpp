// Original: d:\build\custom\temp\code\client\projectg\notifyform.cpp
#include "NotifyForm.h"
#include "ObjectFactory.h"
#include "../client/ProjectG/SoundManager.h"

// notifyform.cpp:13 (run-time type and factory registration of FrNotifyForm)
const WRTTI FrNotifyForm::m_RTTI("FrNotifyForm", &FrForm::m_RTTI);

IObject* FrNotifyFormMakeInstance(void) {
	return new FrNotifyForm;
}

struct __sFrNotifyForm {
	__sFrNotifyForm(void) {
		ObjectFactory().AddObjectFunctor(FrNotifyFormMakeInstance, "FrNotifyForm");
	}
};
static __sFrNotifyForm s_registerFrNotifyForm;

// notifyform.cpp:18
const sFRESH_MSGMAP* FrNotifyForm::GetMessageMap(void) const {
	return &FrNotifyForm::_MsgMap;
}

sFRESH_MSGMAP FrNotifyForm::_MsgMap = { &FrForm::_MsgMap, &FrNotifyForm::_MsgEntries[0] };

sFRESH_ENTRY FrNotifyForm::_MsgEntries[] = {
	// notifyform.cpp:20
	{ "ok_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrNotifyForm::OnFreshOkay },
	// notifyform.cpp:21
	{ "cancel_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrNotifyForm::OnFreshCancel },
	// notifyform.cpp:22
	{ "yes_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrNotifyForm::OnFreshYes },
	// notifyform.cpp:23
	{ "no_btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrNotifyForm::OnFreshNo },

	// notifyform.cpp:25
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// notifyform.cpp:29
FrNotifyForm::FrNotifyForm(void) {
}

// notifyform.cpp:33
FrNotifyForm::~FrNotifyForm(void) {
}

// notifyform.cpp:37
void FrNotifyForm::OnFreshOkay(void) {
	if (!(m_flag & 0x100)) {
		g_audio->PlaySfx("ui_button_ok_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}

	Close(FrOK, true);
}

// notifyform.cpp:45
void FrNotifyForm::OnFreshCancel(void) {
	if (!(m_flag & 0x100)) {
		g_audio->PlaySfx("ui_button_cancel_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}

	Close(FrCANCEL, true);
}

// notifyform.cpp:53
void FrNotifyForm::OnFreshYes(void) {
	if (!(m_flag & 0x100)) {
		g_audio->PlaySfx("ui_button_ok_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}

	Close(FrYES, true);
}

// notifyform.cpp:61
void FrNotifyForm::OnFreshNo(void) {
	if (!(m_flag & 0x100)) {
		g_audio->PlaySfx("ui_button_cancel_click", 0, 0, 0, 0, 0.5f, 200.0f);
	}

	Close(FrNO, true);
}
