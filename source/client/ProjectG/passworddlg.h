// Original: d:\build\custom\temp\code\client\projectg\passworddlg.h
#ifndef PG_CLIENT_PROJECTG_PASSWORDDLG_H
#define PG_CLIENT_PROJECTG_PASSWORDDLG_H

#include <string>

#include "..\..\defines.h"
#include "frform.h"

class FrEdit;

// Password (the last 7 digits of the resident number) dialog; its size
// (0x130) is the `new` of FrPasswordDlgMakeInstance. Its destructor is
// compiler-generated (no line marks).
class FrPasswordDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // passworddlg.h:16

	FrPasswordDlg(void);

	void InitContent(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnPasswordInit(intptr_t param);
	bool OnPasswordEnterKey(int param);
	void OnOkBtnUp(void);

private:
	std::string m_password;   // 0x110
	FrEdit* m_pPassword;      // 0x12c ("password")
};

CHECK_SIZE(FrPasswordDlg, 0x130);
BEGIN_LAYOUT_CHECK(FrPasswordDlg)
	CHECK_FIELD(FrPasswordDlg, m_password, 0x110);
	CHECK_FIELD(FrPasswordDlg, m_pPassword, 0x12c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_PASSWORDDLG_H
