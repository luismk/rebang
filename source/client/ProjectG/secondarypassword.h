// Original: d:\build\custom\temp\code\client\projectg\secondarypassword.h
#ifndef PG_CLIENT_PROJECTG_SECONDARYPASSWORD_H
#define PG_CLIENT_PROJECTG_SECONDARYPASSWORD_H

#include "..\..\defines.h"
#include "gatewayinterface.h"

class FrForm;

namespace _gateway {

class FrExplanePassword;
class FrPasswordInputDlg;
class FrSelectCertification;
class FrPasswordNotifyDlg;

// Handler of the second-password gateway content: CGatewayHandler (0x1C
// bytes), then the four dialogs it opens (each cleared by its form-result
// callback).
class CSecondaryPassword : public CGatewayHandler {
	LAYOUT_CHECK_FRIEND;

public:
	CSecondaryPassword(void);
	virtual ~CSecondaryPassword(void);

	virtual bool Initialize(void);
	virtual bool Execute(const MsgObject& msg);

	bool OnPasswordExplaneFormResult(int result, FrForm* pForm);
	bool OnInputPasswordFormResult(int result, FrForm* pForm);
	bool OnSelectCertifyFormResult(int result, FrForm* pForm);
	bool OnSystemMsgFormResult(int result, FrForm* pForm);

private:
	void OnClosePasswordDlg(void);
	void OnCloseSelectCertifyDlg(void);
	void OnWaitGameServerList(void);
	void OnRequestStoreCertifyFlag(intptr_t param);
	void OnOpenSelfCertifyWebPage(void);
	void OnNotifyPasswordInfo(intptr_t param);
	void OnRequestAvailableContents(intptr_t param);
	void OnNotifyCannotUseContents(void);
	void OnNotifySecondPasswordConfirm(void);
	void OnRequestQualifyLogin(intptr_t param);
	void OnRequestRegistPassword(intptr_t param);
	void OnOpenPasswordExplaneDlg(void);
	void OnOpenPasswordCreateDlg(void);
	void OnOpenPasswordLoginDlg(void);
	void OnOpenSelectCertifyDlg(void);
	void OnRequestLoginPassword(intptr_t param);
	void OnOpenSystemMsg(int param);
	void OnOpenSystemMsg(int param, const char* message);
	void OnResultRegistPassword(intptr_t param);
	void OnRequestCheckCertifyFlag(void);
	void OnResultLoginPassword(intptr_t param);
	void OnRequestCheckPasswordValid(intptr_t param);
	void OnNotifySystemMsg(intptr_t param);

	FrExplanePassword* m_pExplaneDlg;           // 0x1C
	FrPasswordInputDlg* m_pInputDlg;            // 0x20
	FrSelectCertification* m_pSelectCertifyDlg; // 0x24
	FrPasswordNotifyDlg* m_pSystemMsgDlg;       // 0x28
};

CHECK_SIZE(CSecondaryPassword, 0x2C);

}

BEGIN_LAYOUT_CHECK(_gateway::CSecondaryPassword)
	CHECK_FIELD(_gateway::CSecondaryPassword, m_pExplaneDlg, 0x1C);
	CHECK_FIELD(_gateway::CSecondaryPassword, m_pInputDlg, 0x20);
	CHECK_FIELD(_gateway::CSecondaryPassword, m_pSelectCertifyDlg, 0x24);
	CHECK_FIELD(_gateway::CSecondaryPassword, m_pSystemMsgDlg, 0x28);
END_LAYOUT_CHECK

#endif
