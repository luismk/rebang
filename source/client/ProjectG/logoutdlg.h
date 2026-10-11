// Original: d:\build\custom\temp\code\client\projectg\logoutdlg.h
#ifndef PG_CLIENT_PROJECTG_LOGOUTDLG_H
#define PG_CLIENT_PROJECTG_LOGOUTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;

// Logout dialog: the server-select, logout and exit buttons, with the
// description of the hovered one. Its size (0x120) is the `new` of
// FrLogoutDlgMakeInstance.
class FrLogoutDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // logoutdlg.h:8

	FrLogoutDlg(void);

	void DisableServerSelect(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnProc(const float dt);

	void OnServerBtnInit(intptr_t param);
	void OnServerBtnUp(void);
	void OnLogoutBtnInit(intptr_t param);
	void OnLogoutBtnUp(void);
	void OnExitBtnInit(intptr_t param);
	void OnExitBtnUp(void);

private:
	FrButton* m_pServerBtn;            // 0x110 (OnServerBtnInit)
	FrButton* m_pLogoutBtn;            // 0x114 (OnLogoutBtnInit)
	FrButton* m_pExitBtn;              // 0x118 (OnExitBtnInit)
	FrButton* m_pHoverBtn;             // 0x11c (button whose description is shown, OnProc)
};
BEGIN_LAYOUT_CHECK(FrLogoutDlg)
	CHECK_FIELD(FrLogoutDlg, m_pServerBtn, 0x110);
	CHECK_FIELD(FrLogoutDlg, m_pLogoutBtn, 0x114);
	CHECK_FIELD(FrLogoutDlg, m_pExitBtn, 0x118);
	CHECK_FIELD(FrLogoutDlg, m_pHoverBtn, 0x11c);
END_LAYOUT_CHECK
CHECK_SIZE(FrLogoutDlg, 0x120);

#endif // PG_CLIENT_PROJECTG_LOGOUTDLG_H
