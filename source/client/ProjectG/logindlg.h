// Original: d:\build\custom\temp\code\client\projectg\logindlg.h
#ifndef PG_CLIENT_PROJECTG_LOGINDLG_H
#define PG_CLIENT_PROJECTG_LOGINDLG_H

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrEdit;

// Connection state of the login dialog (FrLoginDlg::m_connState). Besides
// the states the dialog sets itself, the login server's answers land here
// (FrLoginDlg::OnProc reports each); names follow OnProc's messages.
enum eLoginState {
	LOGIN_STATE_NONE = 0,
	LOGIN_STATE_CONNECT = 1,                  // BeginConnect: connecting to the login server
	LOGIN_STATE_LOGGED_IN = 3,                // logged in, the lobby takes over
	LOGIN_STATE_SUCCESS = 4,                  // "login succeeded"
	LOGIN_STATE_INPUT = 5,                    // ResetInput: waiting for the user's input
	LOGIN_STATE_INVALID_ID = 6,
	LOGIN_STATE_WRONG_PASSWORD = 7,
	LOGIN_STATE_ALREADY_CONNECTED = 8,        // the id is in use: offer to close the other session
	LOGIN_STATE_BLOCKED = 9,
	LOGIN_STATE_BLOCKED2 = 10,
	LOGIN_STATE_PASSWORD_LOCKED = 11,         // 5 wrong passwords: the client quits
	LOGIN_STATE_SERVER_ERROR = 12,
	LOGIN_STATE_CREATE_CHARACTER = 13,        // the lobby gets "CREATE"
	LOGIN_STATE_PARENT_CONSENT_PENDING = 14,
	LOGIN_STATE_NO_PARENT_CONSENT = 15,
	LOGIN_STATE_WRONG_SSN = 16,
	LOGIN_STATE_WITHDRAWN = 17,
	LOGIN_STATE_NOT_INVITED = 18,
	LOGIN_STATE_MAINTENANCE = 19,
	LOGIN_STATE_REGION_BLOCKED = 20,
	LOGIN_STATE_NO_TICKET = 21,
	LOGIN_STATE_NOT_VERIFIED = 22,
	LOGIN_STATE_ACCOUNT_TRANSFERRED = 23,
	LOGIN_STATE_CONTACT_SUPPORT = 24,
	LOGIN_STATE_INVALID_USER_INFO = 25,
	LOGIN_STATE_CBT_NO_SIGNUP = 26,
	LOGIN_STATE_RESET = 27                    // back to the input state, nothing reported
};

// The login dialog; its size (0x178) is the `new` of FrLoginDlgMakeInstance.
class FrLoginDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // logindlg.h:70

	FrLoginDlg(void);
	virtual ~FrLoginDlg(void);

	void SetNoticeMsg(const char* msg);
	static void SetState(eLoginState state);
	static eLoginState GetState(void);
	void EnableLoginControls(bool bEnable);
	void BeginConnect(void);
	void Login(void);

private:
	bool PrepareWebLogin(void);
	void BeginClientLogin(void);

	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	bool OnJoinDlgResult(int ret, FrForm* pForm);
	bool OnFindDlgResult(int ret, FrForm* pForm);
	bool OnDisconPrevSession(int ret, FrForm* pForm);
	bool OnEnterKey(intptr_t key);
	void OnIdInit(intptr_t param);
	void OnPasswordInit(intptr_t param);
	void OnAutoIdInit(intptr_t param);
	void OnViewInit(intptr_t param);
	void OnLoginInit(intptr_t param);
	void OnJoinBtnInit(intptr_t param);
	void OnFindBtnInit(intptr_t param);
	void OnHideStaticInit(intptr_t param);
	void OnParanAreaInit(intptr_t param);
	void OnHideAreaInit(intptr_t param);
	void OnLoginNoticeInit(intptr_t param);
	void OnHideBtnInit(int param);
	void OnAutoIdBtnUp(void);
	const char* QueryWelcomeMessage(void);
	void OnLoginBtnUp(void);
	void OnJoinBtnUp(void);
	void OnFindBtnUp(void);
	void ResetInput(const char* msg);
	void OnHideAreaDraw(void);

	static eLoginState m_connState;

	std::string m_str110;          // 0x110 (use not transcribed yet)
	std::string m_str12C;          // 0x12c (use not transcribed yet)
	FrEdit* m_pIdEdit;             // 0x148
	FrEdit* m_pPasswordEdit;       // 0x14c
	FrEdit* m_pViewEdit;           // 0x150, the notice/message view
	FrButton* m_pLoginBtn;         // 0x154
	FrButton* m_pAutoIdBtn;        // 0x158, "remember the id" check button
	UNKNOWN(0x15C, 0x160);
	FrButton* m_pJoinBtn;          // 0x160
	FrButton* m_pFindBtn;          // 0x164
	void* m_p168;                  // 0x168 (cleared by the constructor; use not transcribed yet)
	int m_loginFailCount;          // 0x16c, wrong passwords so far (OnProc quits after the fifth)
	void* m_p170;                  // 0x170 (cleared by the constructor; use not transcribed yet)
	UNKNOWN(0x174, 0x178);
};

CHECK_SIZE(FrLoginDlg, 0x178);
BEGIN_LAYOUT_CHECK(FrLoginDlg)
	CHECK_FIELD(FrLoginDlg, m_str110, 0x110);
	CHECK_FIELD(FrLoginDlg, m_str12C, 0x12C);
	CHECK_FIELD(FrLoginDlg, m_pIdEdit, 0x148);
	CHECK_FIELD(FrLoginDlg, m_pPasswordEdit, 0x14C);
	CHECK_FIELD(FrLoginDlg, m_pViewEdit, 0x150);
	CHECK_FIELD(FrLoginDlg, m_pLoginBtn, 0x154);
	CHECK_FIELD(FrLoginDlg, m_pAutoIdBtn, 0x158);
	CHECK_FIELD(FrLoginDlg, m_pJoinBtn, 0x160);
	CHECK_FIELD(FrLoginDlg, m_pFindBtn, 0x164);
	CHECK_FIELD(FrLoginDlg, m_p168, 0x168);
	CHECK_FIELD(FrLoginDlg, m_loginFailCount, 0x16C);
	CHECK_FIELD(FrLoginDlg, m_p170, 0x170);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_LOGINDLG_H
