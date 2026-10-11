// Original: d:\build\custom\temp\code\client\fresh\include\frcmdtarget.h
// (no inline code of this header carries line marks; the name follows the
// unit client/Fresh/source/FrCmdTarget)
#ifndef PG_CLIENT_FRESH_INCLUDE_FRCMDTARGET_H
#define PG_CLIENT_FRESH_INCLUDE_FRCMDTARGET_H

#include "..\..\..\defines.h"
#include "..\..\..\padding.h"
class FrCmdTarget;
class FrForm;

// Commands a window sends to its owner's message map (PDB enum).
enum eFrCmd {
	FRCMD_NONE = 0,
	FRCMD_INIT = 1,
	FRCMD_MOUSEMOVE = 2,
	FRCMD_LBUTTONDOWN = 3,
	FRCMD_LBUTTONUP = 4,
	FRCMD_RBUTTONDOWN = 5,
	FRCMD_RBUTTONUP = 6,
	FRCMD_DBLCLICK = 7,
	FRCMD_OWNERDRAW = 8,
	FRCMD_ENTERKEY = 9,
	FRCMD_LOSTKEYFOCUS = 10,
	FRCMD_HOVERON = 11,
	FRCMD_HOVEROFF = 12,
	FRCMD_FINISH = 13,
	FRCMD_DESTROY = 14
};

// Signature of a message-map handler (PDB enum): V = void, B = bool, I = int.
enum eFrFuncType {
	FrFuncNone = 0,
	FrFuncVV = 1,
	FrFuncBV = 2,
	FrFuncBI = 3,
	FrFuncVI = 4,
	FrFuncBI2 = 5,
	FrFuncBI3 = 6
};

// The handler pointers (PDB typedefs) are declared while FrCmdTarget is still
// incomplete, so cl gives them the general 16-byte pointer-to-member
// representation (the PDB's CV_PMTYPE_Undef, 8-byte aligned): that is what
// puts sFRESH_ENTRY::pFn at 0x10 and FrForm::m_pFnResult at 0xe0..0xf0.
typedef void (FrCmdTarget::*FRESH_PMSG)(void);
typedef bool (FrCmdTarget::*FRESH_PMSGBV)(void);
typedef void (FrCmdTarget::*FRESH_PMSGVI)(intptr_t);
typedef bool (FrCmdTarget::*FRESH_PMSGBI)(intptr_t);
typedef bool (FrCmdTarget::*FRESH_RESULT_CALLBACK)(int, FrForm*);

// One message-map row: the control name, the command and the handler.
struct sFRESH_ENTRY {
	char* pName;          // 0x0
	eFrCmd cmd;           // 0x4
	eFrFuncType eFt;      // 0x8
	FRESH_PMSG pFn;       // 0x10
};
CHECK_SIZE(sFRESH_ENTRY, 0x20);
CHECK_FIELD(sFRESH_ENTRY, pName, 0x0);
CHECK_FIELD(sFRESH_ENTRY, cmd, 0x4);
CHECK_FIELD(sFRESH_ENTRY, eFt, 0x8);
CHECK_FIELD(sFRESH_ENTRY, pFn, 0x10);

// A class's message map: its rows and its base class's map.
struct sFRESH_MSGMAP {
	sFRESH_MSGMAP* pBaseMsgMap;   // 0x0
	sFRESH_ENTRY* pEntry;         // 0x4
};
CHECK_SIZE(sFRESH_MSGMAP, 0x8);
CHECK_FIELD(sFRESH_MSGMAP, pBaseMsgMap, 0x0);
CHECK_FIELD(sFRESH_MSGMAP, pEntry, 0x4);

// A resolved handler, passed along with a command.
struct sFRESH_HANDLER {
	FrCmdTarget* pFresh;   // 0x0
	eFrFuncType eFt;       // 0x4
	FRESH_PMSG pFn;        // 0x8
};
CHECK_SIZE(sFRESH_HANDLER, 0x18);
CHECK_FIELD(sFRESH_HANDLER, pFresh, 0x0);
CHECK_FIELD(sFRESH_HANDLER, eFt, 0x4);
CHECK_FIELD(sFRESH_HANDLER, pFn, 0x8);

// Receiver of Fresh commands (layout and vtable from the PDB: slot 0 the
// destructor, slot 1 GetMessageMap). A class with a message map declares
// _MsgEntries (private) and _MsgMap (protected) and overrides GetMessageMap.
class FrCmdTarget {
public:
	FrCmdTarget(void);
	virtual ~FrCmdTarget(void);

	bool OnFreshMsg(const char* name, eFrCmd cmd, intptr_t param, sFRESH_HANDLER* pHandler);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	sFRESH_ENTRY* FindFreshEntry(sFRESH_ENTRY* pEntry, const char* name, eFrCmd cmd);
};

CHECK_SIZE(FrCmdTarget, 0x4);

#endif // PG_CLIENT_FRESH_INCLUDE_FRCMDTARGET_H
