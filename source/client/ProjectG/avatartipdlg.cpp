// Original: d:\build\custom\temp\code\client\projectg\avatartipdlg.cpp
#include "AvatarTipDlg.h"
#include "ObjectFactory.h"
#include "commonutil.h"
#include "frbutton.h"
#include "frviewer.h"

// avatartipdlg.cpp:8 (run-time type and factory registration of FrAvatarTipDlg)
const WRTTI FrAvatarTipDlg::m_RTTI("FrAvatarTipDlg", &FrForm::m_RTTI);

IObject* FrAvatarTipDlgMakeInstance(void) {
	return new FrAvatarTipDlg;
}

struct __sFrAvatarTipDlg {
	__sFrAvatarTipDlg(void) {
		ObjectFactory().AddObjectFunctor(FrAvatarTipDlgMakeInstance, "FrAvatarTipDlg");
	}
};
static __sFrAvatarTipDlg s_registerFrAvatarTipDlg;

// avatartipdlg.cpp:10
const sFRESH_MSGMAP* FrAvatarTipDlg::GetMessageMap(void) const {
	return &FrAvatarTipDlg::_MsgMap;
}

sFRESH_MSGMAP FrAvatarTipDlg::_MsgMap = { &FrForm::_MsgMap, &FrAvatarTipDlg::_MsgEntries[0] };

sFRESH_ENTRY FrAvatarTipDlg::_MsgEntries[] = {
	// avatartipdlg.cpp:13
	{ "tip", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAvatarTipDlg::OnTipInit },
	// avatartipdlg.cpp:14
	{ "tip", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAvatarTipDlg::OnNextBtnUp },
	// avatartipdlg.cpp:15
	{ "prev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAvatarTipDlg::OnPrevInit },
	// avatartipdlg.cpp:16
	{ "prev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAvatarTipDlg::OnPrevBtnUp },
	// avatartipdlg.cpp:17
	{ "next", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAvatarTipDlg::OnNextInit },
	// avatartipdlg.cpp:18
	{ "next", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAvatarTipDlg::OnNextBtnUp },
	// avatartipdlg.cpp:19
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAvatarTipDlg::OnCloseInit },
	// avatartipdlg.cpp:20
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAvatarTipDlg::OnCloseBtnUp },

	// avatartipdlg.cpp:23
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// avatartipdlg.cpp:26
FrAvatarTipDlg::FrAvatarTipDlg(void) {
	m_tipIndex = 0;
}

// avatartipdlg.cpp:31
FrAvatarTipDlg::~FrAvatarTipDlg(void) {
}

// avatartipdlg.cpp:36
void FrAvatarTipDlg::OnTipInit(intptr_t param) {
	m_pTip = DYNAMIC_CAST(FrViewer, param);
}

// avatartipdlg.cpp:41
void FrAvatarTipDlg::OnPrevInit(intptr_t param) {
	m_pPrev = DYNAMIC_CAST(FrButton, param);
	if (m_pPrev) {
		m_pPrev->SetPushDelay(0.0f);
	}
}

// avatartipdlg.cpp:48
void FrAvatarTipDlg::OnPrevBtnUp(void) {
	m_tipIndex--;
	if (m_tipIndex < 0)
		m_tipIndex = 5;

	if (m_pTip)
		m_pTip->Open(MakeStr("tip%d.tga", m_tipIndex + 1));
}

// avatartipdlg.cpp:58
void FrAvatarTipDlg::OnNextInit(intptr_t param) {
	m_pNext = DYNAMIC_CAST(FrButton, param);
	if (m_pNext) {
		m_pNext->SetPushDelay(0.0f);
	}
}

// avatartipdlg.cpp:65
void FrAvatarTipDlg::OnNextBtnUp(void) {
	m_tipIndex++;
	if (m_tipIndex > 5)
		m_tipIndex = 0;

	if (m_pTip)
		m_pTip->Open(MakeStr("tip%d.tga", m_tipIndex + 1));
}

// avatartipdlg.cpp:75
void FrAvatarTipDlg::OnCloseInit(intptr_t param) {
	m_pClose = DYNAMIC_CAST(FrButton, param);
}

// avatartipdlg.cpp:80
void FrAvatarTipDlg::OnCloseBtnUp(void) {
	Close(true);
}
