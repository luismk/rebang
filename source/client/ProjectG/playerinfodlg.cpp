// Original: d:\build\custom\temp\code\client\projectg\playerinfodlg.cpp
#include "PlayerInfoDlg.h"
#include "ObjectFactory.h"
#include "fredit.h"

// playerinfodlg.cpp:21 (run-time type and factory registration of FrPlayerInfoDlg)
const WRTTI FrPlayerInfoDlg::m_RTTI("FrPlayerInfoDlg", &FrForm::m_RTTI);

IObject* FrPlayerInfoDlgMakeInstance(void) {
	return new FrPlayerInfoDlg;
}

struct __sFrPlayerInfoDlg {
	__sFrPlayerInfoDlg(void) {
		ObjectFactory().AddObjectFunctor(FrPlayerInfoDlgMakeInstance, "FrPlayerInfoDlg");
	}
};
static __sFrPlayerInfoDlg s_registerFrPlayerInfoDlg;

// playerinfodlg.cpp:23
const sFRESH_MSGMAP* FrPlayerInfoDlg::GetMessageMap(void) const {
	return &FrPlayerInfoDlg::_MsgMap;
}

sFRESH_MSGMAP FrPlayerInfoDlg::_MsgMap = { &FrForm::_MsgMap, &FrPlayerInfoDlg::_MsgEntries[0] };

sFRESH_ENTRY FrPlayerInfoDlg::_MsgEntries[] = {
	// playerinfodlg.cpp:25
	{ "user", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPlayerInfoDlg::OnUserInfoInit },
	// playerinfodlg.cpp:26
	{ "character", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPlayerInfoDlg::OnCharInfoInit },
	// playerinfodlg.cpp:27
	{ "caddie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPlayerInfoDlg::OnCadInfoInit },

	// playerinfodlg.cpp:29
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// playerinfodlg.cpp:33
FrPlayerInfoDlg::FrPlayerInfoDlg(void) {
}

// playerinfodlg.cpp:38
FrPlayerInfoDlg::~FrPlayerInfoDlg(void) {
}

// playerinfodlg.cpp:254
void FrPlayerInfoDlg::OnUserInfoInit(intptr_t param) {
	m_pUserInfo = DYNAMIC_CAST(FrEdit, param);
}

// playerinfodlg.cpp:260
void FrPlayerInfoDlg::OnCharInfoInit(intptr_t param) {
	m_pCharInfo = DYNAMIC_CAST(FrEdit, param);
}

// playerinfodlg.cpp:266
void FrPlayerInfoDlg::OnCadInfoInit(intptr_t param) {
	m_pCadInfo = DYNAMIC_CAST(FrEdit, param);
}
