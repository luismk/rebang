// Original: d:\build\custom\temp\code\client\fresh\source\frcmdtarget.cpp
#include "frcmdtarget.h"

#include <string.h>

// A message-map handler seen through each of its signatures (PDB type).
union uFreshFunctions {
	FRESH_PMSG pFn;
	FRESH_PMSG pFn_FRESH;
	FRESH_PMSG pFn_FRESH_VV;
	FRESH_PMSGBV pFn_FRESH_BV;
	FRESH_PMSGVI pFn_FRESH_VI;
	FRESH_PMSGBI pFn_FRESH_BI;
};
CHECK_SIZE(uFreshFunctions, 0x10);

// frcmdtarget.cpp:14 (the null handler's dynamic initializer, $E1)
sFRESH_ENTRY FrCmdTarget::_MsgEntries[] = {
	{ 0, FRCMD_NONE, FrFuncNone, 0 }
};

sFRESH_MSGMAP FrCmdTarget::_MsgMap = { 0, &FrCmdTarget::_MsgEntries[0] };

// frcmdtarget.cpp:15
const sFRESH_MSGMAP* FrCmdTarget::GetMessageMap(void) const {
	return 0;
}

// frcmdtarget.cpp:18
bool FreshDispatchMsg(FrCmdTarget* pTarget, intptr_t var1, sFRESH_ENTRY* pEntry, sFRESH_HANDLER* pHandler) {
	if (pHandler != 0) {
		pHandler->pFn = pEntry->pFn;
		pHandler->eFt = pEntry->eFt;
		pHandler->pFresh = pTarget;
		return true;
	}

	uFreshFunctions mmf;
	mmf.pFn = pEntry->pFn;

	bool bResult = true;
	switch (pEntry->eFt) {
	case FrFuncVV:
		(pTarget->*mmf.pFn_FRESH_VV)();
		break;
	case FrFuncBV:
		bResult = (pTarget->*mmf.pFn_FRESH_BV)();
		break;
	case FrFuncBI:
		(pTarget->*mmf.pFn_FRESH_VI)(var1);
		break;
	case FrFuncVI:
		bResult = (pTarget->*mmf.pFn_FRESH_BI)(var1);
		break;
	}

	return bResult;
}

// frcmdtarget.cpp:54
FrCmdTarget::FrCmdTarget(void) {
}

// frcmdtarget.cpp:58
FrCmdTarget::~FrCmdTarget(void) {
}

// frcmdtarget.cpp:62
sFRESH_ENTRY* FrCmdTarget::FindFreshEntry(sFRESH_ENTRY* pBegin, const char* pName, eFrCmd cmd) {
	sFRESH_ENTRY* pEntry;
	for (pEntry = pBegin; pEntry != 0 && pEntry->pFn != 0; ++pEntry) {
		if (_stricmp(pEntry->pName, pName) == 0 && pEntry->cmd == cmd) {
			return pEntry;
		}
	}

	for (pEntry = pBegin; pEntry != 0 && pEntry->pFn != 0; ++pEntry) {
		if (pEntry->pName[0] == '\0' && pEntry->cmd == cmd) {
			return pEntry;
		}
	}

	return 0;
}

// frcmdtarget.cpp:104
bool FrCmdTarget::OnFreshMsg(const char* pName, eFrCmd cmd, intptr_t var1, sFRESH_HANDLER* pHandler) {
	for (const sFRESH_MSGMAP* pMsgMap = GetMessageMap(); pMsgMap != 0 && pMsgMap->pEntry != 0; pMsgMap = pMsgMap->pBaseMsgMap) {
		sFRESH_ENTRY* pEntry = FindFreshEntry(pMsgMap->pEntry, pName, cmd);
		if (pEntry != 0) {
			return FreshDispatchMsg(this, var1, pEntry, pHandler);
		}
	}

	return true;
}
