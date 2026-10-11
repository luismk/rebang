// Original: d:\build\custom\temp\code\client\projectg\secondarypassword.cpp
#include <stdio.h>

#include "SecondaryPassword.h"
#include "GatewayDoc.h"
#include "SecondaryPasswordDlg.h"
#include "actor.h"
#include "MessageManager.h"
#include "commonutil.h"
#include "logininfo.h"
#include "fresh.h"
#include "Packet.h"
#include "TaskManager.h"
#include "Browser.h"
#include "ClientSetting.h"
#include "../shared/s5/SharedUtilities.h"

// gatewaydoc.h:45
void _gateway::CSecondPwdDoc::SetPasswordState(eSecondPasswordState state) {
	m_ePasswordState = state;
}

// gatewaydoc.h:46
eSecondPasswordState _gateway::CSecondPwdDoc::GetPasswordState(void) {
	return m_ePasswordState;
}

// gatewaydoc.h:48
void _gateway::CSecondPwdDoc::SetRemainDay(int remainDay) {
	m_iRemainDay = remainDay;
}

// gatewaydoc.h:49
int _gateway::CSecondPwdDoc::GetRemainDay(void) {
	return m_iRemainDay;
}

// gatewaydoc.h:51
void _gateway::CSecondPwdDoc::IncreseInvalidInputPassword(void) {
	++m_iInvalidInputCount;
}

// gatewaydoc.h:54
void _gateway::CSecondPwdDoc::SetSecondPasswordConfirm(void) {
	m_bSecondPasswordConfirm = true;
}

// gatewaydoc.h:55
bool _gateway::CSecondPwdDoc::GetSecondPasswordConfirm(void) {
	return m_bSecondPasswordConfirm;
}

// gatewaydoc.h:57
void _gateway::CSecondPwdDoc::SetCertifyFlag(void) {
	m_bCertifyFlag = true;
}

// gatewaydoc.h:58
bool _gateway::CSecondPwdDoc::GetCertifyFlag(void) {
	return m_bCertifyFlag;
}

namespace _gateway {

// secondarypassword.cpp:34
CSecondaryPassword::CSecondaryPassword(void)
	: m_pExplaneDlg(0), m_pInputDlg(0), m_pSelectCertifyDlg(0), m_pSystemMsgDlg(0) {
}

// secondarypassword.cpp:39
CSecondaryPassword::~CSecondaryPassword(void) {
}

// secondarypassword.cpp:44
bool CSecondaryPassword::Initialize(void) {
	AddFunctionType(1, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CSecondaryPassword::*)(void))&CSecondaryPassword::OnResultRegistPassword);
	AddFunctionType(2, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CSecondaryPassword::*)(void))&CSecondaryPassword::OnRequestRegistPassword);

	AddFunctionType(3, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CSecondaryPassword::*)(void))&CSecondaryPassword::OnRequestStoreCertifyFlag);
	AddFunctionType(4, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSecondaryPassword::OnRequestCheckCertifyFlag);

	AddFunctionType(5, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSecondaryPassword::OnOpenPasswordExplaneDlg);
	AddFunctionType(6, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSecondaryPassword::OnOpenPasswordCreateDlg);
	AddFunctionType(7, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSecondaryPassword::OnOpenPasswordLoginDlg);
	AddFunctionType(8, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSecondaryPassword::OnClosePasswordDlg);
	AddFunctionType(9, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSecondaryPassword::OnOpenSelectCertifyDlg);
	AddFunctionType(10, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSecondaryPassword::OnCloseSelectCertifyDlg);

	AddFunctionType(13, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSecondaryPassword::OnOpenSelfCertifyWebPage);
	AddFunctionType(12, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CSecondaryPassword::*)(void))&CSecondaryPassword::OnRequestLoginPassword);
	AddFunctionType(11, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CSecondaryPassword::*)(void))&CSecondaryPassword::OnResultLoginPassword);
	AddFunctionType(14, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSecondaryPassword::OnWaitGameServerList);
	AddFunctionType(15, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CSecondaryPassword::*)(void))&CSecondaryPassword::OnNotifyPasswordInfo);
	AddFunctionType(16, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CSecondaryPassword::*)(void))&CSecondaryPassword::OnRequestAvailableContents);
	AddFunctionType(17, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSecondaryPassword::OnNotifyCannotUseContents);
	AddFunctionType(19, FUNCTION_TYPE_VV, (FUNCTION_MAPPER_PFN)&CSecondaryPassword::OnNotifySecondPasswordConfirm);
	AddFunctionType(20, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CSecondaryPassword::*)(void))&CSecondaryPassword::OnRequestQualifyLogin);
	AddFunctionType(21, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CSecondaryPassword::*)(void))&CSecondaryPassword::OnRequestCheckPasswordValid);

	AddFunctionType(22, FUNCTION_TYPE_VP, (FUNCTION_MAPPER_PFN)(void (CSecondaryPassword::*)(void))&CSecondaryPassword::OnNotifySystemMsg);

	return true;
}

// secondarypassword.cpp:77
bool CSecondaryPassword::Execute(const MsgObject& msg) {
	SetFunctionType(msg.m_param1);
	// The message travels through Exec's int parameter (the original's
	// 32-bit pointer-in-int; not portable to 64-bit).
	return Exec(this, (intptr_t)&msg);
}

// secondarypassword.cpp:84
// param is the MsgObject's address (pointer in an int: not portable to 64-bit).
void CSecondaryPassword::OnResultRegistPassword(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;
	int errorCode = pMsg->m_param2;

	char message[256] = { 0 };
	int msgId = 0;
	switch (errorCode) {
	case 0x22345:
	case 0x22346:
	case 0x2240D:
	case 0x23669:
	case 0x2366A:
	case 0x2366B:
	case 0x2366C:
	case 0x2366D:
	case 0xB2392:
	case 0xB2393:
		msgId = 0x69;
		break;
	}

	sprintf(message, "%s : %d", WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)msgId), errorCode);
	OnOpenSystemMsg(msgId, message);
}

// secondarypassword.cpp:114
// param is the MsgObject's address; m_param2 the address of the password
// (pointers in ints, as the original: not portable to 64-bit).
void CSecondaryPassword::OnRequestRegistPassword(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;
	std::string password((const char*)pMsg->m_param2);

	WSendPacket packet(9);
	packet.EncodeStr(password);
	packet.Send((eSendTo)3);

	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// secondarypassword.cpp:126
// param is the MsgObject's address; m_param2 the address of the bool flag
// (pointers in ints, as the original: not portable to 64-bit).
void CSecondaryPassword::OnRequestStoreCertifyFlag(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;

	CSecondPwdDoc* pDoc = GetDocument<CSecondPwdDoc*>();
	if (pDoc == 0) {
		return;
	}

	if (*(bool*)pMsg->m_param2 == true) {
		pDoc->SetCertifyFlag();
	}
}

// secondarypassword.cpp:144
void CSecondaryPassword::OnRequestCheckCertifyFlag(void) {
	CSecondPwdDoc* pDoc = GetDocument<CSecondPwdDoc*>();
	if (pDoc == 0) {
		return;
	}

	if (pDoc->GetCertifyFlag() == true) {
		OnOpenPasswordLoginDlg();
	} else {
		OnOpenSelectCertifyDlg();
	}
}

// secondarypassword.cpp:163
void CSecondaryPassword::OnOpenPasswordExplaneDlg(void) {
	if (m_pExplaneDlg != 0) {
		return;
	}

	m_pExplaneDlg = CreateForm<FrExplanePassword>(g_pFresh->GetManager(), this, "explanepassword", 0);
	m_pExplaneDlg->EnableDrag(false);
	m_pExplaneDlg->Open((FRESH_RESULT_CALLBACK)&CSecondaryPassword::OnPasswordExplaneFormResult, 0);
}

// secondarypassword.cpp:175
void CSecondaryPassword::OnOpenPasswordCreateDlg(void) {
	if (m_pInputDlg != 0) {
		return;
	}

	m_pInputDlg = CreateForm<FrPasswordInputDlg>(g_pFresh->GetManager(), this, "passwordinputdlg", 0);
	m_pInputDlg->EnableDrag(false);
	m_pInputDlg->SetDlgType((eSecondPwdInputType)0);
	m_pInputDlg->Open((FRESH_RESULT_CALLBACK)&CSecondaryPassword::OnInputPasswordFormResult, 2);
}

// secondarypassword.cpp:188
void CSecondaryPassword::OnOpenPasswordLoginDlg(void) {
	if (m_pInputDlg != 0 || m_pSelectCertifyDlg != 0) {
		return;
	}

	m_pInputDlg = CreateForm<FrPasswordInputDlg>(g_pFresh->GetManager(), this, "passwordinputdlg", 0);
	m_pInputDlg->EnableDrag(false);
	m_pInputDlg->SetDlgType((eSecondPwdInputType)1);
	m_pInputDlg->Open((FRESH_RESULT_CALLBACK)&CSecondaryPassword::OnInputPasswordFormResult, 2);
}

// secondarypassword.cpp:201
void CSecondaryPassword::OnClosePasswordDlg(void) {
	if (m_pInputDlg != 0) {
		m_pInputDlg->Close(FrOK, true);
	}
}

// secondarypassword.cpp:209
void CSecondaryPassword::OnOpenSelectCertifyDlg(void) {
	if (m_pSelectCertifyDlg != 0) {
		return;
	}

	m_pSelectCertifyDlg = CreateForm<FrSelectCertification>(g_pFresh->GetManager(), this, "selectcertifydlg", 0);
	m_pSelectCertifyDlg->EnableDrag(false);
	m_pSelectCertifyDlg->Open((FRESH_RESULT_CALLBACK)&CSecondaryPassword::OnSelectCertifyFormResult, 0);
}

// secondarypassword.cpp:221
void CSecondaryPassword::OnCloseSelectCertifyDlg(void) {
	if (m_pSelectCertifyDlg != 0) {
		m_pSelectCertifyDlg->Close(FrOK, true);
	}
}

// secondarypassword.cpp:229
void CSecondaryPassword::OnOpenSelfCertifyWebPage(void) {
	if (WSingleton<CBrowser>::Instance()->IsOpen()) {
		return;
	}

	WSingleton<COption>::Instance()->vApplyLobbyScreenSize();

	WSingleton<CBrowser>::Instance()->SetPagetype((CBrowser::eWebPage)1);
	WSingleton<CBrowser>::Instance()->InitializePostArgument();
	WSingleton<CBrowser>::Instance()->AddPostArgument("sn", "9");
	WSingleton<CBrowser>::Instance()->AddPostArgument("mn", MakeStr("%d", CLoginInfo::Instance()->AuthUid()));
	WSingleton<CBrowser>::Instance()->OpenPost("https://qa.pangya.gametree.co.kr/InGame/Certify/Default.aspx", false, true);
}

// secondarypassword.cpp:246
// param is the MsgObject's address; m_param2 the address of the password
// (pointers in ints, as the original: not portable to 64-bit).
void CSecondaryPassword::OnRequestLoginPassword(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;
	std::string password((const char*)pMsg->m_param2);

	WSendPacket packet(0xA);
	packet.EncodeStr(password);
	packet.Send((eSendTo)3);

	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// secondarypassword.cpp:258
// param is the MsgObject's address (pointer in an int: not portable to 64-bit).
void CSecondaryPassword::OnResultLoginPassword(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;
	int errorCode = pMsg->m_param2;
	CSecondPwdDoc* pDoc = GetDocument<CSecondPwdDoc*>();
	if (pDoc == 0) {
		return;
	}

	switch (errorCode) {
	case 0x23670:
		OnOpenSystemMsg(0x6E);

		pDoc->IncreseInvalidInputPassword();
		break;
	case 0x22472:
		// The message text travels in the int parameter (pointer in an
		// int, as the original: not portable to 64-bit).
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x25, (intptr_t)WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)0x6F), 0, 0, 0, 0));
		break;
	case 0x22345:
	case 0x22346:
	case 0x22471:
	case 0x2366D:
	case 0x2366E:
	case 0xB2392:
	case 0xB2393: {
		char message[256] = { 0 };
		sprintf(message, "%s : %d", WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)0x77), errorCode);
		OnOpenSystemMsg(0x77, message);
		break;
	}
	}
}

// secondarypassword.cpp:310
void CSecondaryPassword::OnWaitGameServerList(void) {
}

// secondarypassword.cpp:315
// param is the MsgObject's address (pointer in an int: not portable to 64-bit).
void CSecondaryPassword::OnNotifyPasswordInfo(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;
	int remainDay = pMsg->m_param3;

	CSecondPwdDoc* pDoc = GetDocument<CSecondPwdDoc*>();
	if (pDoc == 0) {
		return;
	}

	pDoc->SetPasswordState((eSecondPasswordState)pMsg->m_param2);
	pDoc->SetRemainDay(remainDay);
}

// secondarypassword.cpp:332
// param is the MsgObject's address; m_param2 the address of the bool result
// (pointers in ints, as the original: not portable to 64-bit).
void CSecondaryPassword::OnRequestAvailableContents(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;
	bool* pAvailable = (bool*)pMsg->m_param2;

	CSecondPwdDoc* pDoc = GetDocument<CSecondPwdDoc*>();
	if (pDoc == 0) {
		*pAvailable = false;
		return;
	}

	switch (pDoc->GetPasswordState()) {
	case 1:
		*pAvailable = true;
		return;
	case 2:
		*pAvailable = false;
		break;
	case 3:
		*pAvailable = true;
		return;
	case 4:
		*pAvailable = true;
		return;
	}

	if (pDoc->GetRemainDay() <= 0) {
		*pAvailable = true;
	}
}

// secondarypassword.cpp:374
void CSecondaryPassword::OnNotifyCannotUseContents(void) {
	CSecondPwdDoc* pDoc = GetDocument<CSecondPwdDoc*>();
	if (pDoc == 0) {
		return;
	}

	const char* message = MakeStr(WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)0x6D), pDoc->GetRemainDay());
	WSingleton<_systemmsg::CMessageManager>::Instance()->NotifyNormalMessage(message);
}

// secondarypassword.cpp:387
// param is the MsgObject's address; m_param2 the address of the password,
// m_param3 the address of the bool result (pointers in ints, as the
// original: not portable to 64-bit).
void CSecondaryPassword::OnRequestCheckPasswordValid(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;
	bool* pValid = (bool*)pMsg->m_param3;
	const std::string* pPassword = (const std::string*)pMsg->m_param2;

	*pValid = false;
	_util::ePwdErrorCode errorCode = _util::VerifyInputPassword(*pPassword);
	switch (errorCode) {
	case _util::PWD_EMPTY:
		OnOpenSystemMsg(0x6A);
		break;
	case _util::PWD_NOT_NUMBER:
		OnOpenSystemMsg(0x6C);
		break;
	case _util::PWD_TOO_SHORT:
		OnOpenSystemMsg(0x66);
		break;
	case _util::PWD_TOO_LONG:
		OnOpenSystemMsg(0x65);
		break;
	default:
		*pValid = true;
		break;
	}

	if (*pValid == false) {
		return;
	}

	*pValid = false;
	switch (_util::VerifyStringContents_Limit(*pPassword)) {
	case _util::PWD_SAME_DIGITS:
		OnOpenSystemMsg(0x70);
		break;
	case _util::PWD_ASCENDING:
		OnOpenSystemMsg(0x71);
		break;
	case _util::PWD_DESCENDING:
		OnOpenSystemMsg(0x71);
		break;
	default:
		*pValid = true;
		break;
	}
}

// secondarypassword.cpp:440
void CSecondaryPassword::OnNotifySecondPasswordConfirm(void) {
	CSecondPwdDoc* pDoc = GetDocument<CSecondPwdDoc*>();
	if (pDoc == 0) {
		return;
	}

	pDoc->SetSecondPasswordConfirm();
}

// secondarypassword.cpp:452
// param is the MsgObject's address; m_param2 the address of the bool result
// (pointers in ints, as the original: not portable to 64-bit).
void CSecondaryPassword::OnRequestQualifyLogin(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;

	CSecondPwdDoc* pDoc = GetDocument<CSecondPwdDoc*>();
	bool* pQualify = (bool*)pMsg->m_param2;
	if (pDoc == 0) {
		*pQualify = false;
		return;
	}

	unsigned long provType = CLoginInfo::Instance()->ProvType();

	if (provType == 2) {
		*pQualify = pDoc->GetSecondPasswordConfirm() == true;
	} else if (provType == 4) {
		*pQualify = true;
	}
}

// secondarypassword.cpp:481
// param is the MsgObject's address (pointer in an int: not portable to 64-bit).
void CSecondaryPassword::OnNotifySystemMsg(intptr_t param) {
	const MsgObject* pMsg = (const MsgObject*)param;

	OnOpenSystemMsg(pMsg->m_param2);
}

// secondarypassword.cpp:489
bool CSecondaryPassword::OnPasswordExplaneFormResult(int result, FrForm* pForm) {
	m_pExplaneDlg = 0;

	return true;
}

// secondarypassword.cpp:496
bool CSecondaryPassword::OnInputPasswordFormResult(int result, FrForm* pForm) {
	m_pInputDlg = 0;

	return true;
}

// secondarypassword.cpp:503
bool CSecondaryPassword::OnSelectCertifyFormResult(int result, FrForm* pForm) {
	m_pSelectCertifyDlg = 0;

	return true;
}

// secondarypassword.cpp:510
bool CSecondaryPassword::OnSystemMsgFormResult(int result, FrForm* pForm) {
	m_pSystemMsgDlg = 0;

	return true;
}

// secondarypassword.cpp:517
void CSecondaryPassword::OnOpenSystemMsg(int param) {
	if (m_pSystemMsgDlg != 0) {
		return;
	}

	m_pSystemMsgDlg = CreateForm<FrPasswordNotifyDlg>(g_pFresh->GetManager(), this, "passwordnotify", 0);
	m_pSystemMsgDlg->SetNotifyType(param);
	m_pSystemMsgDlg->Open((FRESH_RESULT_CALLBACK)&CSecondaryPassword::OnSystemMsgFormResult, 1);
}

// secondarypassword.cpp:530
void CSecondaryPassword::OnOpenSystemMsg(int param, const char* message) {
	if (m_pSystemMsgDlg != 0) {
		return;
	}

	m_pSystemMsgDlg = CreateForm<FrPasswordNotifyDlg>(g_pFresh->GetManager(), this, "passwordnotify", 0);
	m_pSystemMsgDlg->SetNotifyType(param);
	m_pSystemMsgDlg->SetMessage(message);
	m_pSystemMsgDlg->Open((FRESH_RESULT_CALLBACK)&CSecondaryPassword::OnSystemMsgFormResult, 1);
}

}
