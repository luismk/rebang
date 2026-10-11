// Original: d:\build\custom\temp\code\client\projectg\kbdmanualdlg.h
#ifndef PG_CLIENT_PROJECTG_KBDMANUALDLG_H
#define PG_CLIENT_PROJECTG_KBDMANUALDLG_H

#include "..\..\defines.h"
#include "frform.h"

// Keyboard-manual dialog; its size (0x110, no members of its own) is the
// `new` of FrKbdManualDlgMakeInstance.
class FrKbdManualDlg : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // kbdmanualdlg.h:8

	FrKbdManualDlg(void) {}   // kbdmanualdlg.h:11

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnCloseBtnUp(void);
};

CHECK_SIZE(FrKbdManualDlg, 0x110);

#endif // PG_CLIENT_PROJECTG_KBDMANUALDLG_H
