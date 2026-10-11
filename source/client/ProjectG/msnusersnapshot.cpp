// Original: d:\build\custom\temp\code\client\projectg\msnusersnapshot.cpp
#include "MsnUserSnapshot.h"
#include "ObjectFactory.h"
#include "frstatic.h"

// msnusersnapshot.cpp:22 (run-time type and factory registration of ntMsnUserSnapshotDlg)
const WRTTI ntMsnUserSnapshotDlg::m_RTTI("ntMsnUserSnapshotDlg", &FrForm::m_RTTI);

IObject* ntMsnUserSnapshotDlgMakeInstance(void) {
	return new ntMsnUserSnapshotDlg;
}

struct __sntMsnUserSnapshotDlg {
	__sntMsnUserSnapshotDlg(void) {
		ObjectFactory().AddObjectFunctor(ntMsnUserSnapshotDlgMakeInstance, "ntMsnUserSnapshotDlg");
	}
};
static __sntMsnUserSnapshotDlg s_registerntMsnUserSnapshotDlg;

// msnusersnapshot.cpp:24
const sFRESH_MSGMAP* ntMsnUserSnapshotDlg::GetMessageMap(void) const {
	return &ntMsnUserSnapshotDlg::_MsgMap;
}

sFRESH_MSGMAP ntMsnUserSnapshotDlg::_MsgMap = { &FrForm::_MsgMap, &ntMsnUserSnapshotDlg::_MsgEntries[0] };

sFRESH_ENTRY ntMsnUserSnapshotDlg::_MsgEntries[] = {
	// msnusersnapshot.cpp:26
	{ "id", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntMsnUserSnapshotDlg::OnIdInit },
	{ "gender", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntMsnUserSnapshotDlg::OnGenderInit },
	{ "level", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntMsnUserSnapshotDlg::OnLevelInit },
	{ "guild", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntMsnUserSnapshotDlg::OnGuildInit },
	{ "school", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntMsnUserSnapshotDlg::OnSchoolInit },
	{ "status", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&ntMsnUserSnapshotDlg::OnStatusInit },

	// msnusersnapshot.cpp:33
	{ "detail", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&ntMsnUserSnapshotDlg::OnDetailClick },

	// msnusersnapshot.cpp:35
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// msnusersnapshot.cpp:42
ntMsnUserSnapshotDlg::ntMsnUserSnapshotDlg(void) {
	m_pId = 0;
	m_pGender = 0;
	m_pLevel = 0;
	m_pGuild = 0;
	m_pSchool = 0;
	m_pStatus = 0;
}

// msnusersnapshot.cpp:53
ntMsnUserSnapshotDlg::~ntMsnUserSnapshotDlg(void) {
}

// msnusersnapshot.cpp:58
void ntMsnUserSnapshotDlg::OnIdInit(intptr_t param) {
	m_pId = DYNAMIC_CAST(FrStatic, param);
}

// msnusersnapshot.cpp:64
void ntMsnUserSnapshotDlg::OnGenderInit(intptr_t param) {
	m_pGender = DYNAMIC_CAST(FrStatic, param);
}

// msnusersnapshot.cpp:70
void ntMsnUserSnapshotDlg::OnLevelInit(intptr_t param) {
	m_pLevel = DYNAMIC_CAST(FrStatic, param);
}

// msnusersnapshot.cpp:76
void ntMsnUserSnapshotDlg::OnGuildInit(intptr_t param) {
	m_pGuild = DYNAMIC_CAST(FrStatic, param);
}

// msnusersnapshot.cpp:82
void ntMsnUserSnapshotDlg::OnSchoolInit(intptr_t param) {
	m_pSchool = DYNAMIC_CAST(FrStatic, param);
}

// msnusersnapshot.cpp:88
void ntMsnUserSnapshotDlg::OnStatusInit(intptr_t param) {
	m_pStatus = DYNAMIC_CAST(FrStatic, param);
}

// msnusersnapshot.cpp:94
void ntMsnUserSnapshotDlg::OnDetailClick(void) {
}
