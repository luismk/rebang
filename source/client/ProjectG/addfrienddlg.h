// Original: d:\build\custom\temp\code\client\projectg\addfrienddlg.h
#ifndef PG_CLIENT_PROJECTG_ADDFRIENDDLG_H
#define PG_CLIENT_PROJECTG_ADDFRIENDDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrEdit;
class FrEmoticonDlg;

// Add-friend dialog: the nickname to look up ("id" edit), the nickname found
// ("nick"), the check / ok buttons and the emoticon button; its size (0x130)
// is the `new` of FrAddFriendDlgMakeInstance.
class FrAddFriendDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // addfrienddlg.h:9

	FrAddFriendDlg(void)                                            // addfrienddlg.h:15
		: m_pIdEdit(0), m_pNickEdit(0), m_elapsed(0), m_bFocused(false), m_pEmoticonDlg(0) {
		m_findUID = (unsigned long)-1;
	}

	void SetNick(const char* nick);
	void SetTargetNick(const char* nick);
	void SetFindUID(unsigned long uid);
	void CheckRequestNickname(const char* nick);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnCancel(void);
	void OnEmoticonInit(intptr_t param);

private:
	bool OnEmoticonResult(int ret, FrForm* pForm);

protected:
	void OnDispNickInit(intptr_t param);
	virtual bool OnInit(void);
	virtual void OnProc(const float dt);
	void OnEmoticonUp(void);
	void OnEditNiclInit(intptr_t param);
	void OnEditNickDown(void);
	void OnCheckBtnUp(void);
	void OnOKBtnUp(void);
	bool OnEditNickEnterKey(int param);

private:
	FrEdit* m_pIdEdit;                   // 0x110 ("id", the nickname to look up)
	FrEdit* m_pNickEdit;                 // 0x114 ("nick", the nickname found)
	unsigned long m_findUID;             // 0x118 (uid found, -1 when none)
	float m_elapsed;                     // 0x11c (time since OnInit)
	bool m_bFocused;                     // 0x120 (key focus given to "id")
	FrEmoticonDlg* m_pEmoticonDlg;       // 0x124 (emoticon selection dialog while open)
	FrButton* m_pEmoticonBtn;            // 0x128 ("emoticon")
	UNKNOWN(0x12c, 0x130);
};

CHECK_SIZE(FrAddFriendDlg, 0x130);
BEGIN_LAYOUT_CHECK(FrAddFriendDlg)
	CHECK_FIELD(FrAddFriendDlg, m_pIdEdit, 0x110);
	CHECK_FIELD(FrAddFriendDlg, m_pNickEdit, 0x114);
	CHECK_FIELD(FrAddFriendDlg, m_findUID, 0x118);
	CHECK_FIELD(FrAddFriendDlg, m_elapsed, 0x11c);
	CHECK_FIELD(FrAddFriendDlg, m_bFocused, 0x120);
	CHECK_FIELD(FrAddFriendDlg, m_pEmoticonDlg, 0x124);
	CHECK_FIELD(FrAddFriendDlg, m_pEmoticonBtn, 0x128);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_ADDFRIENDDLG_H
