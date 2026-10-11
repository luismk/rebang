// Original: d:\build\custom\temp\code\client\projectg\createnickdlg.h
#ifndef PG_CLIENT_PROJECTG_CREATENICKDLG_H
#define PG_CLIENT_PROJECTG_CREATENICKDLG_H

#include "..\..\defines.h"
#include "frform.h"

class FrButton;
class FrEdit;
class FrEmoticonDlg;

// Nickname creation dialog: the nickname edit, its preview, the emoticon
// button and picker and the "nickname checked" flag; its size (0x140) is the `new` of
// FrCreateNickDlgMakeInstance.
class FrCreateNickDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // createnickdlg.h:16

	FrCreateNickDlg(void);

	virtual void OnProc(const float dt);

	void CloseDlg(int ret);
	void SetNick(const char* nick);
	void ReceiveCheckCode(int code);
	void SetCheckFlag(bool bCheck) { m_bChecked = bCheck; }   // createnickdlg.h:30
	bool GetCheckFlag(void) const { return m_bChecked; }      // createnickdlg.h:31

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnEditNickInit(intptr_t param);
	bool OnEditNickEnterKey(int param);
	void OnEditNickDown(void);
	void OnDispNickInit(intptr_t param);
	void OnMessasgeInit(intptr_t param);
	void OnOKBtnUp(void);
	void OnCheckBtnUp(void);
	void OnCancelBtnUp(void);
	void OnEmoticonInit(intptr_t param);
	void OnEmoticonUp(void);

private:
	bool OnQuitDlgResult(int ret, FrForm* pForm);
	bool OnEmoticonResult(int ret, FrForm* pForm);
	bool CheckNickByClient(void);

	FrEdit* m_pEditNick;   // 0x110 ("editnick")
	FrEdit* m_pDispNick;   // 0x114
	FrButton* m_pEmoticonBtn;        // 0x118 ("emoticon")
	char m_nick[24];       // 0x11c
	FrEmoticonDlg* m_pEmoticonDlg;   // 0x134, the open emoticon picker
	bool m_bChecked;       // 0x138 (the server accepted the nickname)
};

CHECK_SIZE(FrCreateNickDlg, 0x140);
BEGIN_LAYOUT_CHECK(FrCreateNickDlg)
	CHECK_FIELD(FrCreateNickDlg, m_pEditNick, 0x110);
	CHECK_FIELD(FrCreateNickDlg, m_pDispNick, 0x114);
	CHECK_FIELD(FrCreateNickDlg, m_pEmoticonBtn, 0x118);
	CHECK_FIELD(FrCreateNickDlg, m_nick, 0x11c);
	CHECK_FIELD(FrCreateNickDlg, m_pEmoticonDlg, 0x134);
	CHECK_FIELD(FrCreateNickDlg, m_bChecked, 0x138);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CREATENICKDLG_H
