// Original: d:\build\custom\temp\code\client\projectg\secondarypassworddlg.h
#ifndef PG_CLIENT_PROJECTG_SECONDARYPASSWORDDLG_H
#define PG_CLIENT_PROJECTG_SECONDARYPASSWORDDLG_H

#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrArea;
class FrButton;
class FrEdit;
class FrGraphicInterface;

namespace _gateway {

// Kind of secondary password dialog (create / login ...); the enumerators
// are not recovered yet.
enum eSecondPwdInputType {
};

// Secondary password keypad dialog: ten shuffled number slots, the
// edit/clear/confirm buttons and the two password edits; its size (0x1f8)
// is the `new` of FrPasswordInputDlgMakeInstance.
class FrPasswordInputDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // secondarypassworddlg.h:23

	// Input step of the dialog (enumerators not recovered yet).
	enum eInputSequence {
	};

	FrPasswordInputDlg(void);
	virtual ~FrPasswordInputDlg(void);

	virtual bool OnInit(void);

	void SetDlgType(eSecondPwdInputType type) { m_dlgType = type; }   // secondarypassworddlg.h:88

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitNumberSlot0(int param);
	void OnInitNumberSlot1(int param);
	void OnInitNumberSlot2(int param);
	void OnInitNumberSlot3(int param);
	void OnInitNumberSlot4(int param);
	void OnInitNumberSlot5(int param);
	void OnInitNumberSlot6(int param);
	void OnInitNumberSlot7(int param);
	void OnInitNumberSlot8(int param);
	void OnInitNumberSlot9(int param);
	void OnBackSpace_ButtonInit(int param);
	void OnClear_ButtonInit(int param);
	void OnCreatePassword_ButtonInit(int param);
	void OnConfirm_ButtonInit(intptr_t param);
	void OnChangePassword_ButtonInit(int param);
	void OnReport_ButtonInit(int param);
	void OnConfirmFirstPassword_ButtonInit(int param);
	void OnInputAgain_ButtonInit(int param);
	void OnClose_ButtonInit(intptr_t param);
	void OnCaption_AreaInit(intptr_t param);
	void OnExplaneBlock_AreaInit(intptr_t param);
	void OnExplaneBlock_AreaOwnerdraw(int param);
	void OnPasswordInput_EditInit(intptr_t param);
	void OnPasswordConfirm_EditInit(intptr_t param);

	void OnLButtonUpNumberSlot0(void);
	void OnLButtonUpNumberSlot1(void);
	void OnLButtonUpNumberSlot2(void);
	void OnLButtonUpNumberSlot3(void);
	void OnLButtonUpNumberSlot4(void);
	void OnLButtonUpNumberSlot5(void);
	void OnLButtonUpNumberSlot6(void);
	void OnLButtonUpNumberSlot7(void);
	void OnLButtonUpNumberSlot8(void);
	void OnLButtonUpNumberSlot9(void);
	void OnBackSpace_LButtonUp(void);
	void OnClear_LButtonUp(void);
	void OnCreatePassword_LButtonUp(void);
	void OnConfirm_LButtonUp(void);
	void OnChangePassword_LButtonUp(void);
	void OnReport_LButtonUp(void);
	void OnConfirmFirstPassword_LButtonUp(void);
	void OnInputAgain_LButtonUp(void);
	void OnClose_LButtonUp(void);

private:
	void InitVariable(void);
	void LoadResource(void);
	bool ConvertCreateInput(void);
	bool ConvertConfirmInput(void);
	bool SetButtonProperty(int index, int param);
	void PushNumButton(int slot);
	void InsertPasswordChar(int number);
	void ErasePasswordChar(void);
	void ClearPassword(void);
	bool ChangeInputSequence(eInputSequence sequence);
	bool CheckInputPassword(void);
	void ArrangeButtonImage(void);
	void DrawCreateDlgExplane(FrGraphicInterface* pGI);
	void DrawLoginDlgExplane(FrGraphicInterface* pGI);
	void CheckEnableCreateButton(void);

	eSecondPwdInputType m_dlgType;     // 0x110 (SetDlgType)
	eInputSequence m_inputSequence;    // 0x114 (ChangeInputSequence)
	FrArea* m_pCaptionArea;            // 0x118 ("Caption" area)
	FrEdit* m_pPasswordEdit[2];        // 0x11c (the "PasswordInput" and "PasswordConfirm" edits)
	FrEdit* m_pCurrentEdit;            // 0x124 (edit the keypad types into)
	FrArea* m_pExplaneBlockArea;       // 0x128 ("ExplaneBlock" area)
	FrButton* m_pButtons[19];          // 0x12c (0-9 number slots, then BackSpace, Clear, CreatePassword, Confirm, ChangePassword, Report, ConfirmFirstPassword, InputAgain, Close)
	const Bitmap* m_pBitmaps[13];      // 0x178 (LoadResource)
	std::vector<int> m_slotNumbers;    // 0x1ac (number shown on each slot)
	std::string m_password;            // 0x1bc
	std::string m_confirmPassword;     // 0x1d8
	UNKNOWN(0x1f4, 0x1f8);
};

CHECK_SIZE(FrPasswordInputDlg, 0x1f8);

// Certification choice dialog ("RequestCertify" / "PassCertify"); size 0x138
// (FrSelectCertificationMakeInstance).
class FrSelectCertification : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // secondarypassworddlg.h:193

	FrSelectCertification(void);
	virtual ~FrSelectCertification(void);

	virtual bool OnInit(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnRequestCertify_ButtonInit(intptr_t param);
	void OnRequestCertify_LButtonDown(void);
	void OnPassCertify_ButtonInit(intptr_t param);
	void OnPassCertify_LButtonDown(void);
	void OnLeft_EditInit(intptr_t param);
	void OnRight_EditInit(intptr_t param);
	void OnInFrame_AreaInit(intptr_t param);
	void OnInFrame_AreaOwnerDraw(int param);

private:
	FrButton* m_pButtons[2];           // 0x110 ("RequestCertify", "PassCertify")
	FrEdit* m_pEdits[2];               // 0x118 ("Left", "Right")
	FrArea* m_pInFrameArea;            // 0x120 ("InFrame")
	const Bitmap* m_pBitmaps[4];       // 0x124 (OnInit)
	UNKNOWN(0x134, 0x138);
};

CHECK_SIZE(FrSelectCertification, 0x138);

// Secondary password explanation dialog; size 0x118
// (FrExplanePasswordMakeInstance).
class FrExplanePassword : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // secondarypassworddlg.h:253

	FrExplanePassword(void);
	virtual ~FrExplanePassword(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnOpenPasswordCreate_ButtonInit(intptr_t param);
	void OnOpenPasswordCreate_LButtonUp(void);
	void OnExplane_EditInit(intptr_t param);

private:
	FrButton* m_pOpenPasswordCreateButton;   // 0x110
	FrEdit* m_pExplaneEdit;                  // 0x114
};

CHECK_SIZE(FrExplanePassword, 0x118);

// Secondary password notice box (icon, system message, close button); size
// 0x158 (FrPasswordNotifyDlgMakeInstance).
class FrPasswordNotifyDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // secondarypassworddlg.h:275

	FrPasswordNotifyDlg(void);
	virtual ~FrPasswordNotifyDlg(void);

	virtual bool OnInit(void);

	void SetNotifyType(int type) { m_notifyType = type; }   // secondarypassworddlg.h:293
	void SetMessage(const char* message);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnIcon_AreaInit(intptr_t param);
	void OnIcon_AreaOwnerdraw(int param);
	void OnSystemMsg_EditInit(intptr_t param);
	void OnClose_ButtonInit(intptr_t param);
	void OnClose_LButtonUp(void);

private:
	int m_notifyType;                  // 0x110 (SetNotifyType)
	int m_iconIndex;                   // 0x114 (icon OnInit picks from the notify type)
	UNKNOWN(0x118, 0x11c);
	FrArea* m_pIconArea;               // 0x11c ("Icon")
	FrEdit* m_pSystemMsgEdit;          // 0x120 ("SystemMsg")
	FrButton* m_pCloseButton;          // 0x124 ("Close")
	const Bitmap* m_pIconBitmaps[3];   // 0x128 (OnInit; drawn by OnIcon_AreaOwnerdraw)
	std::string m_message;             // 0x134 (SetMessage)
	bool m_bMessage;                   // 0x150 (SetMessage was called)
};

CHECK_SIZE(FrPasswordNotifyDlg, 0x158);

} // namespace _gateway

BEGIN_LAYOUT_CHECK(_gateway::FrPasswordInputDlg)
	CHECK_FIELD(_gateway::FrPasswordInputDlg, m_dlgType, 0x110);
	CHECK_FIELD(_gateway::FrPasswordInputDlg, m_pCaptionArea, 0x118);
	CHECK_FIELD(_gateway::FrPasswordInputDlg, m_pPasswordEdit, 0x11c);
	CHECK_FIELD(_gateway::FrPasswordInputDlg, m_pCurrentEdit, 0x124);
	CHECK_FIELD(_gateway::FrPasswordInputDlg, m_pExplaneBlockArea, 0x128);
	CHECK_FIELD(_gateway::FrPasswordInputDlg, m_pButtons, 0x12c);
	CHECK_FIELD(_gateway::FrPasswordInputDlg, m_pBitmaps, 0x178);
	CHECK_FIELD(_gateway::FrPasswordInputDlg, m_slotNumbers, 0x1ac);
	CHECK_FIELD(_gateway::FrPasswordInputDlg, m_password, 0x1bc);
	CHECK_FIELD(_gateway::FrPasswordInputDlg, m_confirmPassword, 0x1d8);
END_LAYOUT_CHECK

BEGIN_LAYOUT_CHECK(_gateway::FrSelectCertification)
	CHECK_FIELD(_gateway::FrSelectCertification, m_pButtons, 0x110);
	CHECK_FIELD(_gateway::FrSelectCertification, m_pEdits, 0x118);
	CHECK_FIELD(_gateway::FrSelectCertification, m_pInFrameArea, 0x120);
	CHECK_FIELD(_gateway::FrSelectCertification, m_pBitmaps, 0x124);
END_LAYOUT_CHECK

BEGIN_LAYOUT_CHECK(_gateway::FrExplanePassword)
	CHECK_FIELD(_gateway::FrExplanePassword, m_pOpenPasswordCreateButton, 0x110);
	CHECK_FIELD(_gateway::FrExplanePassword, m_pExplaneEdit, 0x114);
END_LAYOUT_CHECK

BEGIN_LAYOUT_CHECK(_gateway::FrPasswordNotifyDlg)
	CHECK_FIELD(_gateway::FrPasswordNotifyDlg, m_notifyType, 0x110);
	CHECK_FIELD(_gateway::FrPasswordNotifyDlg, m_pIconArea, 0x11c);
	CHECK_FIELD(_gateway::FrPasswordNotifyDlg, m_pSystemMsgEdit, 0x120);
	CHECK_FIELD(_gateway::FrPasswordNotifyDlg, m_pCloseButton, 0x124);
	CHECK_FIELD(_gateway::FrPasswordNotifyDlg, m_iconIndex, 0x114);
	CHECK_FIELD(_gateway::FrPasswordNotifyDlg, m_pIconBitmaps, 0x128);
	CHECK_FIELD(_gateway::FrPasswordNotifyDlg, m_message, 0x134);
	CHECK_FIELD(_gateway::FrPasswordNotifyDlg, m_bMessage, 0x150);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SECONDARYPASSWORDDLG_H
