// Original: d:\build\custom\temp\code\client\projectg\frreadytoall.cpp
#include "FrReadyToAll.h"
#include "ObjectFactory.h"
#include "Packet.h"
#include "SharedDoc.h"

// frreadytoall.cpp:12 (run-time type and factory registration of FrReadyToAll)
const WRTTI FrReadyToAll::m_RTTI("FrReadyToAll", &FrForm::m_RTTI);

IObject* FrReadyToAllMakeInstance(void) {
	return new FrReadyToAll;
}

struct __sFrReadyToAll {
	__sFrReadyToAll(void) {
		ObjectFactory().AddObjectFunctor(FrReadyToAllMakeInstance, "FrReadyToAll");
	}
};
static __sFrReadyToAll s_registerFrReadyToAll;

// frreadytoall.cpp:14
const sFRESH_MSGMAP* FrReadyToAll::GetMessageMap(void) const {
	return &FrReadyToAll::_MsgMap;
}

sFRESH_MSGMAP FrReadyToAll::_MsgMap = { &FrForm::_MsgMap, &FrReadyToAll::_MsgEntries[0] };

sFRESH_ENTRY FrReadyToAll::_MsgEntries[] = {
	// frreadytoall.cpp:16
	{ "ReadyStart", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrReadyToAll::OnReadyBtnUp },
	// frreadytoall.cpp:17
	{ "WaitCancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrReadyToAll::OnWaitBtnUp },

	// frreadytoall.cpp:19
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frreadytoall.cpp:22
FrReadyToAll::FrReadyToAll(void) {
}

// frreadytoall.cpp:26
FrReadyToAll::~FrReadyToAll(void) {
}

// frreadytoall.cpp:30
void FrReadyToAll::OnReadyBtnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(2)) {
		return;
	}

	WSendPacket packet((enumClientPacket)0x0D);
	packet.Encode1(0);
	packet.Send(SEND_TO_DEFAULT);

	Close(true);
}

// frreadytoall.cpp:44
void FrReadyToAll::OnWaitBtnUp(void) {
	Close(true);
}

// frreadytoall.cpp:53 (run-time type and factory registration of FrReadyToMaster)
const WRTTI FrReadyToMaster::m_RTTI("FrReadyToMaster", &FrForm::m_RTTI);

IObject* FrReadyToMasterMakeInstance(void) {
	return new FrReadyToMaster;
}

struct __sFrReadyToMaster {
	__sFrReadyToMaster(void) {
		ObjectFactory().AddObjectFunctor(FrReadyToMasterMakeInstance, "FrReadyToMaster");
	}
};
static __sFrReadyToMaster s_registerFrReadyToMaster;

// frreadytoall.cpp:55
const sFRESH_MSGMAP* FrReadyToMaster::GetMessageMap(void) const {
	return &FrReadyToMaster::_MsgMap;
}

sFRESH_MSGMAP FrReadyToMaster::_MsgMap = { &FrForm::_MsgMap, &FrReadyToMaster::_MsgEntries[0] };

sFRESH_ENTRY FrReadyToMaster::_MsgEntries[] = {
	// frreadytoall.cpp:57
	{ "ReadyStart", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrReadyToMaster::OnStartBtnUp },
	// frreadytoall.cpp:58
	{ "WaitCancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrReadyToMaster::OnExitBtnUp },

	// frreadytoall.cpp:60
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frreadytoall.cpp:63
FrReadyToMaster::FrReadyToMaster(void) {
}

// frreadytoall.cpp:67
FrReadyToMaster::~FrReadyToMaster(void) {
}

// frreadytoall.cpp:71
void FrReadyToMaster::OnStartBtnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(2)) {
		return;
	}

	WSendPacket packet((enumClientPacket)0x0E);
	packet.Encode4(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid);
	packet.Send(SEND_TO_DEFAULT);

	Close(true);
}

// frreadytoall.cpp:85
void FrReadyToMaster::OnExitBtnUp(void) {
	Close(true);
}

// frreadytoall.cpp:96
void FrReadyToMaster::OnProc(const float dt) {
}
