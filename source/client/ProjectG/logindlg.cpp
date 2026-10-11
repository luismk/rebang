// Original: d:\build\custom\temp\code\client\projectg\logindlg.cpp
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include <process.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "LoginDlg.h"
#include "ClientSetting.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "fredit.h"
#include "frbutton.h"
#include "logininfo.h"
#include "frstatic.h"
#include "frarea.h"
#include "fresh.h"
#include "Localize.h"
#include "binstr.h"
#include "GenericHTTPClient.h"
#include "TaskManager.h"
#include "frwndmanager.h"
#include "frdesktop.h"
#include "ProjectG.h"
#include "Packet.h"
#include "NetworkSystem.h"
#include "commonutil.h"

// Set to leave the game's main loop (defined with the client's main loop).
extern bool g_bQuit;

unsigned int __stdcall HttpGetNtreevUserAuthKey(void* param);

// clientsetting.h:391
void COption::gSetLastLoginID(const char* id) {
	strncpy(m_option.m_lastLoginID, id, sizeof(m_option.m_lastLoginID));
}

// clientsetting.h:392
const char* COption::gGetLastLoginID(void) const {
	return m_option.m_lastLoginID;
}

namespace {

// logindlg.cpp:36 (functor of std::for_each: flags a character that is not
// alphanumeric)
struct _ValidId {
	bool m_invalid;   // 0x0

	_ValidId(void) : m_invalid(false) {}
	void operator()(char c) {
		if (!isalnum(c)) {
			m_invalid = true;
		}
	}
};

// logindlg.cpp:68 (id and password handed to the authentication thread)
class cNTAUTHPARAM {
public:
	cNTAUTHPARAM(const std::string& id, const std::string& passwd) : m_id(id), m_passwd(passwd) {}

	std::string m_id;       // 0x0
	std::string m_passwd;   // 0x1c
};

} // namespace

// logindlg.cpp:75 (run-time type and factory registration of FrLoginDlg)
const WRTTI FrLoginDlg::m_RTTI("FrLoginDlg", &FrForm::m_RTTI);

IObject* FrLoginDlgMakeInstance(void) {
	return new FrLoginDlg;
}

struct __sFrLoginDlg {
	__sFrLoginDlg(void) {
		ObjectFactory().AddObjectFunctor(FrLoginDlgMakeInstance, "FrLoginDlg");
	}
};
static __sFrLoginDlg s_registerFrLoginDlg;

// Define the static connection state declared in logindlg.h
eLoginState FrLoginDlg::m_connState = LOGIN_STATE_NONE;

// logindlg.cpp:77
const sFRESH_MSGMAP* FrLoginDlg::GetMessageMap(void) const {
	return &FrLoginDlg::_MsgMap;
}

sFRESH_MSGMAP FrLoginDlg::_MsgMap = { &FrForm::_MsgMap, &FrLoginDlg::_MsgEntries[0] };

sFRESH_ENTRY FrLoginDlg::_MsgEntries[] = {
	// logindlg.cpp:79
	{ "id", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnIdInit },
	// logindlg.cpp:80
	{ "id", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrLoginDlg::OnEnterKey },
	// logindlg.cpp:81
	{ "password", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnPasswordInit },
	// logindlg.cpp:82
	{ "password", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrLoginDlg::OnEnterKey },
	// logindlg.cpp:83
	{ "auto_id", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnAutoIdInit },
	// logindlg.cpp:84
	{ "auto_id", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrLoginDlg::OnAutoIdBtnUp },
	// logindlg.cpp:85
	{ "view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnViewInit },
	// logindlg.cpp:86
	{ "login", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnLoginInit },
	// logindlg.cpp:87
	{ "login", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrLoginDlg::OnLoginBtnUp },
	// logindlg.cpp:102
	{ "join", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnJoinBtnInit },
	// logindlg.cpp:103
	{ "find", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnFindBtnInit },
	// logindlg.cpp:104
	{ "join", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrLoginDlg::OnJoinBtnUp },
	// logindlg.cpp:105
	{ "find", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrLoginDlg::OnFindBtnUp },
	// logindlg.cpp:108
	{ "logo1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnParanAreaInit },
	// logindlg.cpp:109
	{ "logo2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnParanAreaInit },
	// logindlg.cpp:110
	{ "bkframe1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnHideAreaInit },
	// logindlg.cpp:111
	{ "bkframe1", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrLoginDlg::OnHideAreaDraw },
	// logindlg.cpp:113
	{ "ID_EDIT_NOTICE", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnLoginNoticeInit },
	// logindlg.cpp:115
	{ "icon1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnHideAreaInit },
	// logindlg.cpp:116
	{ "icon2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnHideAreaInit },
	// logindlg.cpp:117
	{ "box1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnHideAreaInit },
	// logindlg.cpp:118
	{ "box2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnHideAreaInit },
	// logindlg.cpp:119
	{ "s_id", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnHideStaticInit },
	// logindlg.cpp:120
	{ "s_passwd", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnHideStaticInit },
	// logindlg.cpp:121
	{ "s_save", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrLoginDlg::OnHideStaticInit },
	// logindlg.cpp:145
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// logindlg.cpp:152
FrLoginDlg::FrLoginDlg(void) {
	m_pViewEdit = 0;
	m_pLoginBtn = 0;
	m_loginFailCount = 0;
	m_connState = LOGIN_STATE_NONE;
	m_p170 = 0;
	m_p168 = 0;
	m_pJoinBtn = 0;
	m_pFindBtn = 0;
	WSingleton<CSharedDoc>::Instance()->m_loginDlgCreated = true;
}

// logindlg.cpp:193
FrLoginDlg::~FrLoginDlg(void) {
}

// logindlg.cpp:213
void FrLoginDlg::SetNoticeMsg(const char* msg) {
	if (m_pViewEdit) {
		m_pViewEdit->AddLine(msg, 0, false);
	}
	EnableLoginControls(true);
}

// logindlg.cpp:221
void FrLoginDlg::SetState(eLoginState state) {
	m_connState = state;
}

// logindlg.cpp:226
eLoginState FrLoginDlg::GetState(void) {
	return m_connState;
}

// logindlg.cpp:286 (thread procedure of the client login: asks the web
// service for the authentication key of pParam's id and password)
unsigned int __stdcall HttpGetNtreevUserAuthKey(void* param) {
	cNTAUTHPARAM* pParam = (cNTAUTHPARAM*)param;
	GenericHTTPClient http;
	http.InitilizePostArguments();
	http.AddPostArguments("id", pParam->m_id.c_str(), FALSE);
	http.AddPostArguments("pwd", pParam->m_passwd.c_str(), FALSE);
	http.AddPostArguments("gamecode", 9);

	FrWnd* pWnd = g_pFresh->GetManager()->GetDesktop()->FindChildForm("login");
	FrLoginDlg* pDlg = DYNAMIC_CAST(FrLoginDlg, pWnd);

	BOOL ret = http.Request("http://qa.pangya.gametree.co.kr/Secure/Login/LoginForGame.aspx", 3, "MERONG(0.9/;p)");
	const char* response = http.QueryHTTPResponse();
	if (!ret || strlen(response) > 0x208) {
		// "Web service authentication failed."
		AfxGetTask()->PassMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\xC0\xA5\xBC\xAD\xBA\xF1\xBD\xBA \xC0\xCE\xC1\xF5\xBF\xA1 \xBD\xC7\xC6\xD0\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0));
		if (pDlg) {
			pDlg->EnableLoginControls(true);
		}
		delete pParam;
		return 0;
	}

	cHttpNtreevAuthResult result(response);
	if (result.GetResult()) {
		std::string authKey = result.GetFieldValue("AuthKey");
		std::string memberNo = result.GetFieldValue("MemberNo");
		std::string pcBangNo = result.GetFieldValue("PCBangNo");
		if (pDlg) {
			CLoginInfo::Instance()->SetPw(authKey);
			CLoginInfo::Instance()->SetAuthUid(strtoul(memberNo.c_str(), 0, 10));
			CLoginInfo::Instance()->EnablePcBang(pcBangNo == "0" ? 0 : 1);
			pDlg->BeginConnect();
		} else {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), 0, "notify", 0);
			// "Login authentication timed out. Please try again."
			pForm->SetMessage("\xB7\xCE\xB1\xD7\xC0\xCE \xC0\xCE\xC1\xF5 \xBD\xC3\xB0\xA3 \xC3\xCA\xB0\xFA. \xB4\xD9\xBD\xC3 \xBD\xC3\xB5\xB5 \xC7\xCF\xBC\xBC\xBF\xE4.", false);
		}
	} else {
		std::string msg = result.GetMessage();
		if (pDlg) {
			pDlg->SetNoticeMsg(msg.c_str());
			pDlg->EnableLoginControls(true);
		} else {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), 0, "notify", 0);
			pForm->SetMessage(msg.c_str(), false);
			pForm->Open(0, 3);
		}
	}
	delete pParam;
	return 0;
}

// logindlg.cpp:378 (thread procedure of the NHN login: as
// HttpGetNtreevUserAuthKey, and also stores the NHN member's profile)
unsigned int __stdcall HttpGetNhnUserAuthKey(void* param) {
	cNTAUTHPARAM* pParam = (cNTAUTHPARAM*)param;
	GenericHTTPClient http;
	http.InitilizePostArguments();
	http.AddPostArguments("id", pParam->m_id.c_str(), FALSE);
	http.AddPostArguments("pwd", pParam->m_passwd.c_str(), FALSE);
	http.AddPostArguments("gamecode", 9);

	FrWnd* pWnd = g_pFresh->GetManager()->GetDesktop()->FindChildForm("login");
	FrLoginDlg* pDlg = DYNAMIC_CAST(FrLoginDlg, pWnd);

	BOOL ret = http.Request("http://qa.pangya.gametree.co.kr/Secure/Login/LoginForGame.aspx", 3, "MERONG(0.9/;p)");
	const char* response = http.QueryHTTPResponse();
	if (!ret || strlen(response) > 0x208) {
		// "Web service authentication failed."
		AfxGetTask()->PassMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\xC0\xA5\xBC\xAD\xBA\xF1\xBD\xBA \xC0\xCE\xC1\xF5\xBF\xA1 \xBD\xC7\xC6\xD0\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0));
		if (pDlg) {
			pDlg->EnableLoginControls(true);
		}
		delete pParam;
		return 0;
	}

	cHttpNtreevAuthResult result(response);
	if (result.GetResult()) {
		std::string authKey = result.GetFieldValue("AuthKey");
		std::string memberNo = result.GetFieldValue("MemberNo");
		std::string pcBangNo = result.GetFieldValue("PCBangNo");
		std::string siteMemberNo = result.GetFieldValue("SiteMemberNo");
		std::string zipCode = result.GetFieldValue("ZipCode");
		std::string birthday = result.GetFieldValue("Birthday");
		std::string siteCode = result.GetFieldValue("SiteCode");
		std::string sex = result.GetFieldValue("Sex");
		if (pDlg) {
			CLoginInfo::Instance()->SetZipCode(zipCode);
			CLoginInfo::Instance()->SetBirthDay(birthday);
			CLoginInfo::Instance()->SetGender(sex.find('M') == std::string::npos);

			NhnLogin* pNhnLogin = (NhnLogin*)CLoginInfo::Instance()->QueryInterface();
			pNhnLogin->SetNHNMemberNumber(_atoi64(siteMemberNo.c_str()));

			CLoginInfo::Instance()->SetPw(authKey);
			CLoginInfo::Instance()->SetAuthUid(strtoul(memberNo.c_str(), 0, 10));
			CLoginInfo::Instance()->EnablePcBang(pcBangNo == "0" ? 0 : 1);
			pDlg->BeginConnect();
		} else {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), 0, "notify", 0);
			// "Login authentication timed out. Please try again."
			pForm->SetMessage("\xB7\xCE\xB1\xD7\xC0\xCE \xC0\xCE\xC1\xF5 \xBD\xC3\xB0\xA3 \xC3\xCA\xB0\xFA. \xB4\xD9\xBD\xC3 \xBD\xC3\xB5\xB5 \xC7\xCF\xBC\xBC\xBF\xE4.", false);
		}
	} else {
		std::string msg = result.GetMessage();
		if (pDlg) {
			pDlg->SetNoticeMsg(msg.c_str());
			pDlg->EnableLoginControls(true);
		} else {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), 0, "notify", 0);
			pForm->SetMessage(msg.c_str(), false);
			pForm->Open(0, 3);
		}
	}
	delete pParam;
	return 0;
}

// logindlg.cpp:471
void FrLoginDlg::BeginClientLogin(void) {
	if (m_pIdEdit && m_pIdEdit->IsVisible()) {
		char buf[260] = { 0 };
		strcpy(buf, m_pIdEdit->GetLine(1, false));
		m_str110 = TrimA(buf, " \t\n\r\0");
		if (m_str110.empty()) {
			if (m_pViewEdit) {
				// "Please enter your ID."
				m_pViewEdit->AddLine("\xBE\xC6\xC0\xCC\xB5\xF0\xB8\xA6 \xB3\xD6\xBE\xEE\xC1\xD6\xBC\xBC\xBF\xE4.", 0, false);
			}
			if (m_pIdEdit) m_pIdEdit->SetKeyFocus(true);
			return;
		}
	}

	if (m_pPasswordEdit && m_pPasswordEdit->IsVisible()) {
		char buf[260] = { 0 };
		strcpy(buf, m_pPasswordEdit->GetLine(1, false));
		m_str12C = TrimA(buf, " \t\n\r\0");
		if (m_str12C.empty()) {
			if (m_pViewEdit) {
				// "Please enter your password."
				m_pViewEdit->AddLine("\xBA\xF1\xB9\xD0\xB9\xF8\xC8\xA3\xB8\xA6 \xC0\xD4\xB7\xC2\xC7\xD8\xC1\xD6\xBC\xBC\xBF\xE4.", 0, false);
			}
			if (m_pPasswordEdit) m_pPasswordEdit->SetKeyFocus(true);
			return;
		}
	}

	_ValidId valid = std::for_each(m_str110.begin(), m_str110.end(), _ValidId());
	if (valid.m_invalid && m_pViewEdit) {
		// "The ID may only use letters, digits and '_'."
		m_pViewEdit->AddLine("\xBE\xC6\xC0\xCC\xB5\xF0\xB4\xC2 \xBF\xB5\xB9\xAE\xC0\xDA\xBF\xCD \xBC\xFD\xC0\xDA, '_'\xB8\xB8 \xBB\xE7\xBF\xEB\xC7\xCF\xBD\xC7 \xBC\xF6 ", 0, false);
		m_pViewEdit->AddLine("\xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", 0, false);
		return;
	}

	CLoginInfo::Instance()->SetId(m_str110);
	CLoginInfo::Instance()->SetPw(m_str12C);

	cNTAUTHPARAM* pParam = new cNTAUTHPARAM(m_str110, m_str12C);

	if (!_beginthreadex(0, 0, HttpGetNtreevUserAuthKey, pParam, 0, 0)) {
		if (m_pViewEdit) {
			// "The login failed. Please try again."
			m_pViewEdit->AddLine("\xB7\xCE\xB1\xD7\xC0\xCE\xBF\xA1 \xC0\xE5\xBE\xD6\xB0\xA1 \xB9\xDF\xBB\xFD\xC7\xDF\xBD\xC0\xB4\xCF\xB4\xD9. \xB4\xD9\xBD\xC3 \xBD\xC3\xB5\xB5 \xC7\xD8\xC1\xD6\xBC\xBC\xBF\xE4.", 0, false);
			// "If the problem persists, please contact the development team."
			m_pViewEdit->AddLine("\xB9\xAE\xC1\xA6\xB0\xA1 \xC1\xF6\xBC\xD3 \xB5\xC7\xB8\xE9 \xB0\xB3\xB9\xDF\xC6\xC0\xBF\xA1 \xB9\xAE\xC0\xC7 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9.", 0, false);
		}
		delete pParam;
		return;
	}

	EnableLoginControls(false);
}

// logindlg.cpp:582
bool FrLoginDlg::PrepareWebLogin(void) {
	return strlen(CLoginInfo::Instance()->Id()) && strlen(CLoginInfo::Instance()->Passwd());
}

// logindlg.cpp:587
void FrLoginDlg::EnableLoginControls(bool bEnable) {
	m_pIdEdit->Enable(bEnable);
	m_pPasswordEdit->Enable(bEnable);
	m_pLoginBtn->Enable(bEnable);
}

// logindlg.cpp:594
void FrLoginDlg::BeginConnect(void) {
	m_defaultRetCode = FrNONE;

	m_str110 = CLoginInfo::Instance()->Id();
	m_str12C = CLoginInfo::Instance()->Passwd();

	char* id = MyId();
	memset(id, 0, 0x16);
	strncpy(id, m_str110.c_str(), 0x15);
	WSingleton<CSharedDoc>::Instance()->m_loginServerAddress = m_str12C;

	WNetworkSystem::Instance()->Init(WNetworkSystem::NET_UNIT_ALL);
	if (m_pViewEdit) {
		// "%s connecting...|"
		const char* id = CLoginInfo::Instance()->Id();
		m_pViewEdit->AddLine(MakeStr("%s \xC1\xA2\xBC\xD3 \xC1\xDF...|", id), 0, false);
	}

	m_connState = LOGIN_STATE_CONNECT;
	WNetworkSystem::Instance()->SendMessage(WNetworkSystem::NET_UNIT_LOGIN, NetworkUnit::EVENT_NEXT);
}

// logindlg.cpp:624
void FrLoginDlg::Login(void) {
	if (m_pLoginBtn && m_pLoginBtn->IsEnabled()) {
		if (CLoginInfo::Instance()->IsWebLogin()) {
			if (PrepareWebLogin()) {
				EnableLoginControls(false);
				BeginConnect();
			} else if (m_pViewEdit) {
				m_pViewEdit->AddLine("�α��� �ý��� �������Դϴ�.", 0, false);
			}
		} else {
			BeginClientLogin();
		}
	}
}

// logindlg.cpp:649
bool FrLoginDlg::OnJoinDlgResult(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		ShellExecuteA(NULL, NULL, "http://qa.www.gametree.co.kr/SignUp/Join.aspx?rsn=9", NULL, NULL, SW_SHOWMAXIMIZED);
		if (!WSingleton<COption>::Instance()->vIsWindowed()) {
			g_bQuit = true;
		}
	}
	return true;
}

// logindlg.cpp:666
bool FrLoginDlg::OnDisconPrevSession(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		WSingleton<CProjectG>::Instance()->m_bMoveLoginServer = false;
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0, (intptr_t)"SERVERLIST", 0, 0, 0, 0));
		WSendPacket packet(4);
		packet.Send((eSendTo)3);
		return true;
	}

	m_pIdEdit->SetLine(1, "", 0, false, 0);
	m_pIdEdit->ClearLine();
	m_pPasswordEdit->SetLine(1, "", 0, false, 0);
	m_pPasswordEdit->ClearLine();

	m_pIdEdit->SetKeyFocus(true);

	m_connState = LOGIN_STATE_INPUT;
	SetFadeout(true);
	return true;
}

// logindlg.cpp:692
bool FrLoginDlg::OnFindDlgResult(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		ShellExecuteA(NULL, NULL, "http://qa.www.gametree.co.kr/Secure/MyPage/Member/FindId.aspx", NULL, NULL,
			SW_SHOWMAXIMIZED);
		if (!WSingleton<COption>::Instance()->vIsWindowed()) {
			g_bQuit = true;
		}
	}
	return true;
}

// Clock of OnProc's "connecting" spinner (unnamed data).
extern "C" float data_b7c970;

// logindlg.cpp:707
void FrLoginDlg::OnProc(const float dt) {
	if (m_connState == LOGIN_STATE_NONE) {
		bool bLogin = false;
		if (CLoginInfo::Instance()->IsWebLogin()) {
			bLogin = true;
		}
		if ((m_connState == LOGIN_STATE_NONE && WSingleton<CProjectG>::Instance()->m_bMoveLoginServer) || bLogin) {
			Login();
		}
	}

	if (!WNetworkSystem::Instance()) {
		return;
	}
	WNetworkSystem* pNet = WNetworkSystem::Instance();

	switch (m_connState) {
	case LOGIN_STATE_CONNECT: {
		int lineNum = m_pViewEdit->GetLineNum();
		if (lineNum) {
			char spin[3];
			spin[0] = '-';
			spin[1] = '/';
			spin[2] = '|';
			const char* line = m_pViewEdit->GetLine(lineNum, false);
			int len = strlen(line);
			char buf[1024];
			strcpy(buf, line);
			char* pLast = &buf[len - 1];
			if (*pLast == '-' || *pLast == '/' || *pLast == '|') {
				int index = (int)data_b7c970 % 3;
				*pLast = spin[index];
				data_b7c970 += dt * 10.0f;
				m_pViewEdit->SetLine(lineNum, buf, 0, false, 0);
			}
		}

		if (pNet->IsConnectionComplete(WNetworkSystem::NET_UNIT_LOGIN)) {
			if (!pNet->IsConnected(WNetworkSystem::NET_UNIT_LOGIN)) {
				m_connState = LOGIN_STATE_INPUT;
				if (m_pViewEdit) {
					// "Cannot connect to the login server."
					m_pViewEdit->AddLine("\xB7\xCE\xB1\xD7\xC0\xCE \xBC\xAD\xB9\xF6\xBF\xA1 \xC1\xA2\xBC\xD3 \xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0, false);
					EnableLoginControls(true);
				}
			}
		}
		break;
	}

	case LOGIN_STATE_SUCCESS:
		if (m_pViewEdit) {
			// "Login succeeded."
			m_pViewEdit->AddLine("\xB7\xCE\xB1\xD7\xC0\xCE \xBC\xBA\xB0\xF8.", 0, false);
			// "Receiving the server list."
			m_pViewEdit->AddLine("\xBC\xAD\xB9\xF6\xB8\xAE\xBD\xBA\xC6\xAE\xB8\xA6 \xB9\xDE\xB0\xED \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", 0, false);
		}
		if (m_pAutoIdBtn->GetStatus() == FrButton::PRESSED) {
			WSingleton<COption>::Instance()->gSetLastLoginID(m_str110.c_str());
		}
		m_connState = LOGIN_STATE_LOGGED_IN;
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 1, 0, 0, 0, 0, 0);
		break;

	case LOGIN_STATE_INVALID_ID:
		EnableLoginControls(true);
		// "The ID is wrong."
		ResetInput("\xBE\xC6\xC0\xCC\xB5\xF0\xB0\xA1 \xC0\xDF\xB8\xF8\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.");
		break;

	case LOGIN_STATE_ALREADY_CONNECTED:
		m_connState = LOGIN_STATE_LOGGED_IN;
		if (IsLocalContent((localContentType_t)0x5A)) {
			if (WSingleton<CProjectG>::Instance()->m_bMoveLoginServer == true) {
				WSendPacket packet(4);
				packet.Send((eSendTo)3);
				WSingleton<CProjectG>::Instance()->m_bMoveLoginServer = false;
				break;
			}

			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_okcancel", 0);
			if (pForm) {
				// "The ID is in use.\nClose the previous connection?"
				pForm->SetMessage("\xC7\xF6\xC0\xE7 \xBE\xC6\xC0\xCC\xB5\xF0\xB0\xA1 \xBB\xE7\xBF\xEB\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.\n\xC0\xCC\xC0\xFC \xC1\xA2\xBC\xD3\xC0\xBB \xC1\xBE\xB7\xE1\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
				pForm->Open((FRESH_RESULT_CALLBACK)&FrLoginDlg::OnDisconPrevSession, 0);
			}
		} else {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_okcancel", 0);
			if (pForm) {
				pForm->SetMessage("\xC7\xF6\xC0\xE7 \xBE\xC6\xC0\xCC\xB5\xF0\xB0\xA1 \xBB\xE7\xBF\xEB\xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.\n\xC0\xCC\xC0\xFC \xC1\xA2\xBC\xD3\xC0\xBB \xC1\xBE\xB7\xE1\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
				pForm->Open((FRESH_RESULT_CALLBACK)&FrLoginDlg::OnDisconPrevSession, 0);
			}
		}
		break;

	case LOGIN_STATE_WRONG_PASSWORD:
		EnableLoginControls(true);
		m_loginFailCount++;
		if (m_pViewEdit) {
			// "The ID or password is wrong."
			m_pViewEdit->AddLine("\xBE\xC6\xC0\xCC\xB5\xF0\xB3\xAA \xBA\xF1\xB9\xD0\xB9\xF8\xC8\xA3\xB0\xA1 \xC0\xDF\xB8\xF8 \xC0\xD4\xB7\xC2\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", 0, false);
			// "Check whether 'Caps Lock' is on."
			m_pViewEdit->AddLine("\\c0xffff0000,0xffffffff\\c'Caps Lock'\\c0xff000000,0xffffffff\\c \xC5\xB0\xB0\xA1 \xB4\xAD\xB7\xAF\xC1\xAE \xC0\xD6\xB4\xC2\xC1\xF6 \xC8\xAE\xC0\xCE\xC7\xD8\xBA\xB8\xBC\xBC\xBF\xE4.", 0, false);
			m_connState = LOGIN_STATE_INPUT;
			if (m_loginFailCount > 4) {
				g_bQuit = true;
			}
			m_pPasswordEdit->SetLine(1, "", 0, false, 0);
			m_pPasswordEdit->ClearLine();
			m_pPasswordEdit->SetKeyFocus(true);
		}
		break;

	case LOGIN_STATE_SERVER_ERROR:
		EnableLoginControls(true);
		// "Cannot log in because of a server problem."
		ResetInput("\xBC\xAD\xB9\xF6\xB9\xAE\xC1\xA6\xB7\xCE \xB7\xCE\xB1\xD7\xC0\xCE \xC7\xD2\xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.");
		m_connState = LOGIN_STATE_INPUT;
		break;

	case LOGIN_STATE_WRONG_SSN:
		EnableLoginControls(true);
		// "The registered resident number is not valid."
		m_pViewEdit->AddLine("\xB0\xA1\xC0\xD4\xB5\xC8 \xC1\xD6\xB9\xCE \xB9\xF8\xC8\xA3\xB0\xA1 \xBF\xC3\xB9\xD9\xB8\xA3\xC1\xF6 \xBE\xCA\xBD\xC0\xB4\xCF\xB4\xD9.", 0, false);
		// "Please mail 'customer support'."
		m_pViewEdit->AddLine("\\c0xffff0000,0xffffffff\\c'\xB0\xED\xB0\xB4 \xC1\xF6\xBF\xF8'\\c0xff000000,0xffffffff\\c \xC0\xC7 \xB8\xDE\xC0\xCF \xB9\xAE\xC0\xC7\xB8\xA6 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9.", 0, false);
		m_connState = LOGIN_STATE_INPUT;
		break;

	case LOGIN_STATE_ACCOUNT_TRANSFERRED:
		EnableLoginControls(true);
		// "The account was transferred and cannot connect."
		ResetInput("\xB0\xE8\xC1\xA4 \xC0\xCC\xC0\xFC\xC0\xCC \xB5\xC7\xBE\xEE \xC1\xA2\xBC\xD3\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.");
		break;

	case LOGIN_STATE_BLOCKED:
	case LOGIN_STATE_BLOCKED2:
		EnableLoginControls(true);
		if (m_pViewEdit) {
			// TODO(header): the first AddText shows the block reason, CSharedDoc's
			// std::string at 0x4D14 (still PADDING in SharedDoc.h):
			// m_pViewEdit->AddText(MakeStr("\\c0xffff0000,0xffffffff\\c%s", <0x4D14>.c_str()), false, true);
			// "Please contact the customer support center."
			m_pViewEdit->AddText("\\c0xff000000,0xffffffff\\c\xB0\xED\xB0\xB4 \xC1\xF6\xBF\xF8 \xBC\xBE\xC5\xCD\xB7\xCE \xB9\xAE\xC0\xC7 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9.", false, true);
			m_connState = LOGIN_STATE_INPUT;
		}
		break;

	case LOGIN_STATE_WITHDRAWN:
		EnableLoginControls(true);
		// "This ID has left the membership."
		ResetInput("\xC8\xB8\xBF\xF8\xC0\xBB \xC5\xBB\xC5\xF0\xC7\xD1 \xBE\xC6\xC0\xCC\xB5\xF0\xC0\xD4\xB4\xCF\xB4\xD9.");
		break;

	case LOGIN_STATE_NO_PARENT_CONSENT:
		EnableLoginControls(true);
		if (m_pViewEdit) {
			// "(Notice)---"
			m_pViewEdit->AddText("(\xBE\xCB\xB8\xB2)-----------------------------------", false, true);
			// "Members under 14 need their parents' consent ... not registered ..."
			m_pViewEdit->AddText("\\c0xffff0000,0xffffffff\\c\xB8\xB8 14\xBC\xBC \xB9\xCC\xB8\xB8\\c0xffffffff,0xff000000\\c\xC0\xC7 \xC8\xB8\xBF\xF8\xC0\xBA \\c0xffff0000,0xffffffff\\c\xBA\xCE\xB8\xF0\xB4\xD4\xC0\xC7 \xB5\xBF\xC0\xC7\\c0xffffffff,0xff000000\\c\xB0\xA1 \xC7\xCA\xBF\xE4\xC7\xD5\xB4\xCF\xB4\xD9. \xC7\xF6\xC0\xE7 \xBA\xCE\xB8\xF0\xB4\xD4 \xB5\xBF\xC0\xC7 \xB0\xA1\xC0\xD4\xC0\xCC \xB5\xC7\xBE\xEE \xC0\xD6\xC1\xF6 \xBE\xCA\xBD\xC0\xB4\xCF\xB4\xD9. \\c0xffff0000,0xffffffff\\c\xC6\xCE\xBE\xDF \xC8\xA8\xC6\xE4\xC0\xCC\xC1\xF6\\c0xffffffff,0xff000000\\c\xBF\xA1 \xB7\xCE\xB1\xD7\xC0\xCE\xC7\xD1 \xC8\xC4 \\c0xffff0000,0xffffffff\\c\xBA\xCE\xB8\xF0\xB5\xBF\xC0\xC7\xBC\xAD \xC0\xE7\xB9\xDF\xBC\xDB\\c0xffffffff,0xff000000\\c\xC0\xBB \xB4\xA9\xB8\xA3\xB8\xE9 \xBA\xCE\xB8\xF0\xB4\xD4\xB5\xBF\xC0\xC7 \xBD\xC5\xC3\xBB\xC0\xCC \xC0\xCC\xB7\xE7\xBE\xEE\xC1\xF6\xB0\xED \xB9\xD9\xB7\xCE \xB0\xD4\xC0\xD3\xC0\xCC \xB0\xA1\xB4\xC9\xC7\xD5\xB4\xCF\xB4\xD9.", false, true);
			m_pViewEdit->AddText("----------------------------------------", false, true);
			m_connState = LOGIN_STATE_INPUT;
		}
		break;

	case LOGIN_STATE_PARENT_CONSENT_PENDING:
		EnableLoginControls(true);
		if (m_pViewEdit) {
			// "(Notice)---"
			m_pViewEdit->AddText("(\xBE\xCB\xB8\xB2)----------------------------------", false, true);
			// "Members under 14 need their parents' consent ... not approved yet."
			m_pViewEdit->AddText("\\c0xffff0000,0xffffffff\\c\xB8\xB8 14\xBC\xBC \xB9\xCC\xB8\xB8\\c0xffffffff,0xff000000\\c\xC0\xC7 \xC8\xB8\xBF\xF8\xC0\xBA \\c0xffff0000,0xffffffff\\c\xBA\xCE\xB8\xF0\xB4\xD4\xC0\xC7 \xB5\xBF\xC0\xC7\\c0xffffffff,0xff000000\\c\xB0\xA1 \xC7\xCA\xBF\xE4\xC7\xD5\xB4\xCF\xB4\xD9. \xBA\xCE\xB8\xF0\xB4\xD4 \xB5\xBF\xC0\xC7\xBC\xAD\xB0\xA1 \xBE\xC6\xC1\xF7 \xBD\xC2\xC0\xCE\xB5\xC7\xC1\xF6 \xBE\xCA\xC0\xBA \xBB\xF3\xC5\xC2\xC0\xD4\xB4\xCF\xB4\xD9.", false, true);
			// "Please visit the PangYa homepage for inquiries."
			m_pViewEdit->AddText("\n\\c0xff0000ff,0xffffffff\\c\xC6\xCE\xBE\xDF \xC8\xA8\xC6\xE4\xC0\xCC\xC1\xF6\xBF\xA1 \xB9\xE6\xB9\xAE\xC7\xCF\xBF\xA9 \xB9\xAE\xC0\xC7\xC7\xCF\xBD\xC3\xB1\xE2 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9.\\c0xffffffff,0xff000000\\c", false, true);
			m_pViewEdit->AddText("---------------------------------------", false, true);
			m_connState = LOGIN_STATE_INPUT;
		}
		break;

	case LOGIN_STATE_PASSWORD_LOCKED:
		EnableLoginControls(true);
		if (m_pViewEdit) {
			// "The password was entered wrong 5 times,"
			m_pViewEdit->AddLine("\xBA\xF1\xB9\xD0\xB9\xF8\xC8\xA3\xB0\xA1 5\xC8\xB8 \xC0\xDF\xB8\xF8 \xC0\xD4\xB7\xC2\xB5\xC7\xBE\xEE", 0, false);
			// "you cannot connect to the server for 1 hour!"
			m_pViewEdit->AddLine("1\xBD\xC3\xB0\xA3\xB5\xBF\xBE\xC8 \xBC\xAD\xB9\xF6\xBF\xA1 \xC1\xA2\xBC\xD3\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9!", 0, false);
			m_pViewEdit->AddLine("------------------------------------", 0, false);
			// "====== PangYa will now quit. ======"
			m_pViewEdit->AddLine("====== \xC6\xCE\xBE\xDF\xB8\xA6 \xC1\xBE\xB7\xE1\xC7\xCF\xB0\xDA\xBD\xC0\xB4\xCF\xB4\xD9. ======", 0, false);
			m_pViewEdit->AddLine("------------------------------------", 0, false);
		}
		ResetInput(0);
		g_bQuit = true;
		break;

	case LOGIN_STATE_NOT_INVITED:
		EnableLoginControls(true);
		// "Only users granted entry can connect."
		ResetInput("\xC0\xD4\xC0\xE5 \xC0\xDA\xB0\xDD\xC0\xBB \xB9\xDE\xC0\xBA \xC0\xAF\xC0\xFA\xB8\xB8 \xC1\xA2\xBC\xD3\xC7\xD2 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.");
		break;

	case LOGIN_STATE_CREATE_CHARACTER:
		EnableLoginControls(true);
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0, (intptr_t)"CREATE", 0, 0, 0, 0);
		break;

	case LOGIN_STATE_MAINTENANCE:
		EnableLoginControls(true);
		if (m_pViewEdit) {
			// "Login was stopped for maintenance."
			m_pViewEdit->AddLine("\xC1\xA1\xB0\xCB\xC0\xB8\xB7\xCE \xC0\xCE\xC7\xD8 \xB7\xCE\xB1\xD7\xC0\xCE \xC1\xDF\xC1\xF6 \xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", 0, false);
		}
		ResetInput(0);
		break;

	case LOGIN_STATE_CONTACT_SUPPORT:
		EnableLoginControls(true);
		if (m_pViewEdit) {
			// "Please file a 1:1 inquiry with the customer center."
			m_pViewEdit->AddLine("\xB0\xED\xB0\xB4\xBC\xBE\xC5\xCD 1:1 \xB9\xAE\xC0\xC7\xB7\xCE \xC1\xA2\xBC\xF6 \xBA\xCE\xC5\xB9\xB5\xE5\xB8\xB3\xB4\xCF\xB4\xD9.", 0, false);
		}
		ResetInput(0);
		break;

	case LOGIN_STATE_REGION_BLOCKED:
		EnableLoginControls(true);
		m_pViewEdit->AddText("PangYa is not available from your area.", false, true);
		m_pViewEdit->AddText("We are truly sorry for any inconvenience.", false, true);
		ResetInput(0);
		m_connState = LOGIN_STATE_INPUT;
		break;

	case LOGIN_STATE_NOT_VERIFIED:
		EnableLoginControls(true);
		// "Verification is not complete. Please check the registration mail."
		ResetInput("\xC0\xCE\xC1\xF5\xC0\xCC \xBF\xCF\xB7\xE1\xB5\xC7\xC1\xF6 \xBE\xCA\xBE\xD2\xBD\xC0\xB4\xCF\xB4\xD9.\xB5\xEE\xB7\xCF \xB8\xDE\xC0\xCF\xC0\xBB \xC8\xAE\xC0\xCE\xC7\xD8 \xC1\xD6\xBD\xCA\xBD\xC3\xBF\xC0.");
		break;

	case LOGIN_STATE_NO_TICKET:
		EnableLoginControls(true);
		// "Cannot log in without an entry ticket."
		m_pViewEdit->AddText("\xC0\xD4\xC0\xE5\xB1\xC7\xC0\xCC \xBE\xF8\xBE\xEE\xBC\xAD \xB7\xCE\xB1\xD7\xC0\xCE \xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false, true);
		// "Free entry tickets are issued on the PangYa homepage."
		m_pViewEdit->AddText("\xC6\xCE\xBE\xDF \xC8\xA8\xC6\xE4\xC0\xCC\xC1\xF6\xBF\xA1\xBC\xAD \xB9\xAB\xB7\xE1 \xC0\xD4\xC0\xE5\xB1\xC7\xC0\xBB \xB9\xDF\xB1\xDE \xB9\xDE\xC0\xB8\xBD\xC7 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", false, true);
		m_connState = LOGIN_STATE_INPUT;
		break;

	case LOGIN_STATE_INVALID_USER_INFO:
		if (IsLocalContent((localContentType_t)0x81)) {
			EnableLoginControls(true);
			if (m_pViewEdit) {
				// "Invalid user information was read."
				m_pViewEdit->AddText("\xC0\xDF\xB8\xF8\xB5\xC8 \xC0\xAF\xC0\xFA\xC1\xA4\xBA\xB8\xB8\xA6 \xC0\xD0\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", false, true);
				// "Log in again or contact the customer support center."
				m_pViewEdit->AddText("\xC0\xE7 \xB7\xCE\xB1\xD7\xC0\xCE\xC7\xCF\xBD\xC3\xB0\xC5\xB3\xAA, \xB0\xED\xB0\xB4 \xC1\xF6\xBF\xF8 \xBC\xBE\xC5\xCD\xB7\xCE \xB9\xAE\xC0\xC7 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9.", false, true);
			}
			ResetInput(0);
			m_connState = LOGIN_STATE_INPUT;
		}
		break;

	case LOGIN_STATE_CBT_NO_SIGNUP:
		EnableLoginControls(true);
		if (m_pViewEdit) {
			// "New registrations are closed during the CBT."
			m_pViewEdit->AddText("CBT\xB1\xE2\xB0\xA3\xC0\xBA \xBD\xC5\xB1\xD4\xB5\xEE\xB7\xCF\xC0\xBB \xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false, true);
		}
		ResetInput(0);
		m_connState = LOGIN_STATE_INPUT;
		break;

	case LOGIN_STATE_RESET:
		EnableLoginControls(true);
		m_connState = LOGIN_STATE_INPUT;
		break;
	}
}

// logindlg.cpp:1115
void FrLoginDlg::OnIdInit(intptr_t param) {
	m_pIdEdit = DYNAMIC_CAST(FrEdit, param);
	m_pIdEdit->HidePrivacy(true);
	m_pIdEdit->SetCharLimit(22, true);
	m_pIdEdit->SetLine(1, WSingleton<COption>::Instance()->gGetLastLoginID(), 0, false, 0);

	if (WSingleton<CProjectG>::Instance()->m_bMoveLoginServer) {
		m_pIdEdit->SetLine(1, WSingleton<CProjectG>::Instance()->m_moveLoginId.c_str(), 0, false, 0);
	}

	const char* line = m_pIdEdit->GetLine(1, false);
	if (!line || !strlen(line)) {
		m_pIdEdit->SetKeyFocus(true);
	}

	if (CLoginInfo::Instance()->IsWebLogin()) {
		m_pIdEdit->SetLine(1, CLoginInfo::Instance()->Id(), 0, false, 0);
		m_pIdEdit->SetVisible(false);
	}
}

// logindlg.cpp:1148
void FrLoginDlg::OnPasswordInit(intptr_t param) {
	m_pPasswordEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pPasswordEdit) {
		if (CLoginInfo::Instance()->IsWebLogin()) {
			m_pPasswordEdit->SetLine(1, CLoginInfo::Instance()->Passwd(), 0, false, 0);
		} else if (WSingleton<CProjectG>::Instance()->m_bMoveLoginServer) {
			m_pPasswordEdit->SetLine(1, WSingleton<CProjectG>::Instance()->m_moveLoginPassword.c_str(), 0, false, 0);
		}

		const char* line = m_pIdEdit->GetLine(1, false);
		if (line && strlen(line)) {
			m_pPasswordEdit->SetKeyFocus(true);
		}

		if (CLoginInfo::Instance()->IsWebLogin()) {
			m_pPasswordEdit->SetVisible(false);
		}
	}
}

// logindlg.cpp:1180
bool FrLoginDlg::OnEnterKey(intptr_t key) {
	return false;
}

// logindlg.cpp:1186
void FrLoginDlg::OnAutoIdInit(intptr_t param) {
	m_pAutoIdBtn = DYNAMIC_CAST(FrButton, param);
	const char* id = WSingleton<COption>::Instance()->gGetLastLoginID();
	if (id && strlen(id)) {
		m_pAutoIdBtn->SetStatus(FrButton::PRESSED);
	}
	if (CLoginInfo::Instance()->IsWebLogin()) {
		m_pAutoIdBtn->SetVisible(false);
	}
}

// logindlg.cpp:1217
void FrLoginDlg::OnAutoIdBtnUp(void) {
	if (m_pAutoIdBtn->GetStatus() != FrButton::PRESSED) {
		WSingleton<COption>::Instance()->gSetLastLoginID("");
	}
}

// logindlg.cpp:1226
void FrLoginDlg::OnViewInit(intptr_t param) {
	m_pViewEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pViewEdit) {
		// "Welcome to the world of PangYa"
		m_pViewEdit->AddText("\xC6\xCE\xBE\xDF\xC0\xC7 \xBC\xBC\xB0\xE8\xBF\xA1 \xBF\xC0\xBD\xC5 \xB0\xCD\xC0\xBB \xC8\xAF\xBF\xB5\xC7\xD5\xB4\xCF\xB4\xD9",
			false, true);
	}
}

// logindlg.cpp:1236
const char* FrLoginDlg::QueryWelcomeMessage(void) {
	return 0;
}

// logindlg.cpp:1274
void FrLoginDlg::OnLoginInit(intptr_t param) {
	m_pLoginBtn = DYNAMIC_CAST(FrButton, param);
	if (IsLocalContent((localContentType_t)0x10) && CLoginInfo::Instance()->IsWebLogin()) {
		m_pLoginBtn->SetVisible(false);
	}
}

// logindlg.cpp:1284
void FrLoginDlg::OnLoginBtnUp(void) {
	Login();
}

// logindlg.cpp:1378
void FrLoginDlg::OnJoinBtnInit(intptr_t param) {
	m_pJoinBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pJoinBtn) {
		if (CLoginInfo::Instance()->IsWebLogin()) {
			m_pJoinBtn->SetVisible(false);
		}
	}
}

// logindlg.cpp:1390
void FrLoginDlg::OnFindBtnInit(intptr_t param) {
	m_pFindBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pFindBtn) {
		if (CLoginInfo::Instance()->IsWebLogin()) {
			m_pFindBtn->SetVisible(false);
		}
	}
}

// logindlg.cpp:1402
void FrLoginDlg::OnJoinBtnUp(void) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_okcancel", 0);
	if (pForm) {
		// "Do you want to create a PangYa ID?"
		pForm->SetMessage("\xC6\xCE\xBE\xDF \xBE\xC6\xC0\xCC\xB5\xF0\xB8\xA6 \xB8\xB8\xB5\xE5\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
		pForm->Open((FRESH_RESULT_CALLBACK)&FrLoginDlg::OnJoinDlgResult, 0);
	}
}

// logindlg.cpp:1412
void FrLoginDlg::OnFindBtnUp(void) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_okcancel", 0);
	if (pForm) {
		// "Do you want to find your PangYa ID and password?"
		pForm->SetMessage("\xC6\xCE\xBE\xDF \xBE\xC6\xC0\xCC\xB5\xF0, \xBA\xF1\xB9\xD0\xB9\xF8\xC8\xA3\xB8\xA6 \xC3\xA3\xC0\xB8\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
		pForm->Open((FRESH_RESULT_CALLBACK)&FrLoginDlg::OnFindDlgResult, 0);
	}
}

// logindlg.cpp:1424
void FrLoginDlg::ResetInput(const char* msg) {
	if (m_pViewEdit && msg) {
		m_pViewEdit->AddLine(msg, 0, false);
	}
	if (m_pPasswordEdit) {
		m_pPasswordEdit->SetLine(1, "", 0, false, 0);
		m_pPasswordEdit->ClearLine();
	}
	if (m_pIdEdit && m_pIdEdit->HasKeyFocus()) {
		m_pIdEdit->SetKeyFocus(true);
	} else if (m_pPasswordEdit && m_pPasswordEdit->HasKeyFocus()) {
		m_pPasswordEdit->SetKeyFocus(true);
	}
	m_connState = LOGIN_STATE_INPUT;
}

// logindlg.cpp:1443
bool FrLoginDlg::OnInit(void) {
	WSingleton<CProjectG>::Instance()->m_loginDlgFlag = false;
	// "Test phrase"
	SetMessage("\xC5\xD7\xBD\xBA\xC6\xAE \xB1\xB8\xB9\xAE", false);
	return true;
}

// logindlg.cpp:1491
void FrLoginDlg::OnHideStaticInit(intptr_t param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (CLoginInfo::Instance()->IsWebLogin()) {
		pStatic->SetVisible(false);
	}
}

// logindlg.cpp:1506
void FrLoginDlg::OnParanAreaInit(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (!CLoginInfo::Instance()->IsWebLogin()) {
		pArea->SetVisible(false);
	}
}

// logindlg.cpp:1520
void FrLoginDlg::OnHideAreaInit(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (CLoginInfo::Instance()->IsWebLogin()) {
		pArea->SetVisible(false);
	}
}

// logindlg.cpp:1536
void FrLoginDlg::OnHideAreaDraw(void) {
}

// logindlg.cpp:1553
void FrLoginDlg::OnLoginNoticeInit(intptr_t param) {
	// "Important: are you a Hanbitsoft PangYa member? You must apply for the
	// transfer of your member information on the PangYa homepage before
	// logging in to play normally."
	DYNAMIC_CAST(FrEdit, param)->AddText("\xA1\xD8\xC1\xDF\xBF\xE4 : \xC7\xD1\xBA\xFB\xBC\xD2\xC7\xC1\xC6\xAE \xC6\xCE\xBE\xDF \xC8\xB8\xBF\xF8\xC0\xCC\xBD\xC5\xB0\xA1\xBF\xE4?\n"
		"\xC6\xCE\xBE\xDF \xC8\xA8\xC6\xE4\xC0\xCC\xC1\xF6\xBF\xA1\xBC\xAD \xB9\xDD\xB5\xE5\xBD\xC3 \xC8\xB8\xBF\xF8 \xC1\xA4\xBA\xB8 \xC0\xCC\xC0\xFC \xBD\xC5\xC3\xBB\xC0\xBB \xC7\xCF\xBD\xC5 \xC8\xC4\xBF\xA1 \xB7\xCE\xB1\xD7\xC0\xCE\xC7\xCF\xBC\xC5\xBE\xDF \xC1\xA4\xBB\xF3\xC0\xFB\xC0\xCE \xB0\xD4\xC0\xD3\xC0\xCC \xB0\xA1\xB4\xC9\xC7\xD5\xB4\xCF\xB4\xD9.",
		false, true);
}

// logindlg.cpp:1559
void FrLoginDlg::OnHideBtnInit(int param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	if (CLoginInfo::Instance()->IsWebLogin()) {
		pBtn->SetVisible(false);
	}
}
