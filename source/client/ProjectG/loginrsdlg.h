// Original: d:\build\custom\temp\code\client\projectg\loginrsdlg.h
#ifndef PG_CLIENT_PROJECTG_LOGINRSDLG_H
#define PG_CLIENT_PROJECTG_LOGINRSDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;

// Progress of the connection to the ranking server (FrLoginRsDlg::OnProc).
enum eLoginToRs {
	LOGIN_TO_RS_NONE = 0,
	LOGIN_TO_RS_CONNECTING = 1,
	LOGIN_TO_RS_CONNECTED = 2,
	LOGIN_TO_RS_DONE = 3
};

// "Connecting to the ranking server" dialog: waits for the ranking server
// connection, then opens the ranking. Its size (0x118) is the `new` of
// FrLoginRsDlgMakeInstance.
class FrLoginRsDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // loginrsdlg.h:31

	FrLoginRsDlg(void);

	static void SetState(eLoginToRs state);
	static eLoginToRs GetState(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual void OnProc(const float deltaTime);   // loginrsdlg.cpp:53
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnCancelInit(intptr_t param);

	static eLoginToRs m_connState;

private:
	float m_elapsed;                   // 0x110 (time since the dialog opened, OnProc)
	FrButton* m_pCancel;               // 0x114 (OnCancelInit)
};
BEGIN_LAYOUT_CHECK(FrLoginRsDlg)
	CHECK_FIELD(FrLoginRsDlg, m_elapsed, 0x110);
	CHECK_FIELD(FrLoginRsDlg, m_pCancel, 0x114);
END_LAYOUT_CHECK
CHECK_SIZE(FrLoginRsDlg, 0x118);

#endif // PG_CLIENT_PROJECTG_LOGINRSDLG_H
