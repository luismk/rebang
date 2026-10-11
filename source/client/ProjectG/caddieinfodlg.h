// Original: d:\build\custom\temp\code\client\projectg\caddieinfodlg.h
#ifndef PG_CLIENT_PROJECTG_CADDIEINFODLG_H
#define PG_CLIENT_PROJECTG_CADDIEINFODLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../shared/globalgamedefine.h"

class FrArea;
class FrButton;
class FrEdit;
class FrStatic;
class FrWnd;

// Caddie information dialog: portrait, vacation/limit areas, name, contract
// and fee, sign-up / ok / cancel buttons and the warning check box; its size
// (0x160) is the `new` of FrCaddieInfoDlgMakeInstance.
class FrCaddieInfoDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // caddieinfodlg.h:18

	FrCaddieInfoDlg(void);

	sCaddieInfo GetCaddieInfo(void) { return m_caddieInfo; }       // caddieinfodlg.h:25

	void SetCaddieInfo(const sCaddieInfo& info);
	void EndContract(int param);
	void HideContractControls(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);

	void OnPortraitInit(intptr_t param);
	void OnVacationInit(intptr_t param);
	void OnLimitInit(intptr_t param);
	void OnNameInit(intptr_t param);
	void OnContractInit(intptr_t param);
	void OnCaddieFeeInit(intptr_t param);
	void OnSignUpInit(intptr_t param);
	void OnOkInit(intptr_t param);
	void OnCancelInit(intptr_t param);
	void OnWarningButtonInit(intptr_t param);
	void OnWarningTextInit(intptr_t param);
	void SendCaddieWarningCheckOption(void);
	void OnOkBtnUp(void);
	void OnCancelBtnUp(void);
	bool OnSignUpConfirmDlgResult(int result, FrForm* pForm);
	void OnSignUpBtnUp(void);

private:
	sCaddieInfo m_caddieInfo;            // 0x110
	FrArea* m_pPortraitArea;             // 0x12c ("portrait")
	FrArea* m_pVacationArea;             // 0x130 ("vacation")
	FrArea* m_pLimitArea;                // 0x134 ("limit")
	FrEdit* m_pNameEdit;                 // 0x138 ("name")
	FrWnd* m_pContractWnd;               // 0x13c (hidden with the contract controls)
	FrStatic* m_pContractStatic;         // 0x140 ("contract")
	FrEdit* m_pCaddieFeeEdit;            // 0x144 ("caddiefee")
	FrButton* m_pSignUpBtn;              // 0x148 ("signup")
	FrButton* m_pOkBtn;                  // 0x14c ("ok")
	FrButton* m_pCancelBtn;              // 0x150 ("cancel")
	FrButton* m_pWarningBtn;             // 0x154 ("warning_button")
	FrStatic* m_pWarningText;            // 0x158 ("warning_text")
	FrForm* m_pContractForm;             // 0x15c ("cad_contract" confirmation form)
};

CHECK_SIZE(FrCaddieInfoDlg, 0x160);
BEGIN_LAYOUT_CHECK(FrCaddieInfoDlg)
	CHECK_FIELD(FrCaddieInfoDlg, m_caddieInfo, 0x110);
	CHECK_FIELD(FrCaddieInfoDlg, m_pPortraitArea, 0x12c);
	CHECK_FIELD(FrCaddieInfoDlg, m_pContractWnd, 0x13c);
	CHECK_FIELD(FrCaddieInfoDlg, m_pSignUpBtn, 0x148);
	CHECK_FIELD(FrCaddieInfoDlg, m_pWarningText, 0x158);
	CHECK_FIELD(FrCaddieInfoDlg, m_pContractForm, 0x15c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CADDIEINFODLG_H
