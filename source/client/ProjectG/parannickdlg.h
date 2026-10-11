// Original: d:\build\custom\temp\code\client\projectg\parannickdlg.h
#ifndef PG_CLIENT_PROJECTG_PARANNICKDLG_H
#define PG_CLIENT_PROJECTG_PARANNICKDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrEdit;

// Paran nickname dialog: the "nick" edit and the "ok" button; its size
// (0x118) is the `new` of FrParanNickDlgMakeInstance.
class FrParanNickDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // parannickdlg.h:15

	FrParanNickDlg(void);

	void CloseDlg(void);
	void EnableControls(bool bEnable);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnNickInit(intptr_t param);
	void OnOkBtnUp(void);

private:
	FrEdit* m_pNick;   // 0x110 ("nick")
	UNKNOWN(0x114, 0x118);
};

CHECK_SIZE(FrParanNickDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrParanNickDlg)
	CHECK_FIELD(FrParanNickDlg, m_pNick, 0x110);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_PARANNICKDLG_H
