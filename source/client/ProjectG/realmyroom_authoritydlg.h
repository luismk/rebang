// Original: d:\build\custom\temp\code\client\projectg\realmyroom_authoritydlg.h
#ifndef PG_CLIENT_PROJECTG_REALMYROOM_AUTHORITYDLG_H
#define PG_CLIENT_PROJECTG_REALMYROOM_AUTHORITYDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include <string>

class Bitmap;
class FrButton;
class FrEdit;
class FrListBox;

// My-room authority dialog: public/private setting and storage lock. Size
// 0x148, the `new` of CRealMyRoom_AuthorityDlgMakeInstance.
class CRealMyRoom_AuthorityDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // realmyroom_authoritydlg.h:24

	CRealMyRoom_AuthorityDlg(void);
	virtual ~CRealMyRoom_AuthorityDlg(void);

	bool GetAuthSetting(void);
	void GetPassWord(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnOK(void);
	virtual void OnCancle(void);

	void OnAuth_KindPowerInit(intptr_t);
	void OnAuth_KindPowerOwnerdraw(intptr_t);
	void OnAuth_KindPowerBtnDown(void);
	void OnAuthEditBgInit(int);
	void OnAuth_PasswordInit(intptr_t);
	bool OnAuth_PWEnterKey(int);
	void OnInitStorageLock(intptr_t);
	void OnInitStorageUnLock(intptr_t);
	void OnInitStorageChangePass(intptr_t);
	void OnLBDownStorageLock(void);
	void OnLBDownStorageUnLock(void);
	void OnLBDownStorageChangePass(void);
	void OnLoadImage(void);

	FrListBox* m_pKindPowerList;          // 0x110
	FrEdit* m_pPasswordEdit;              // 0x114
	UNKNOWN(0x118, 0x11C);
	const Bitmap* m_pButtonImage[2][3];   // 0x11C (open, close) x (normal, over, disabled)
	bool m_bAuthSetting;                  // 0x134
	bool m_bStorageLock;                  // 0x135
	FrButton* m_pStorageLockBtn;          // 0x138
	FrButton* m_pStorageUnLockBtn;        // 0x13C
	FrButton* m_pStorageChangePassBtn;    // 0x140
	UNKNOWN(0x144, 0x148);
};

CHECK_SIZE(CRealMyRoom_AuthorityDlg, 0x148);
BEGIN_LAYOUT_CHECK(CRealMyRoom_AuthorityDlg)
	CHECK_FIELD(CRealMyRoom_AuthorityDlg, m_pKindPowerList, 0x110);
	CHECK_FIELD(CRealMyRoom_AuthorityDlg, m_pPasswordEdit, 0x114);
	CHECK_FIELD(CRealMyRoom_AuthorityDlg, m_pButtonImage, 0x11C);
	CHECK_FIELD(CRealMyRoom_AuthorityDlg, m_bAuthSetting, 0x134);
	CHECK_FIELD(CRealMyRoom_AuthorityDlg, m_bStorageLock, 0x135);
	CHECK_FIELD(CRealMyRoom_AuthorityDlg, m_pStorageLockBtn, 0x138);
	CHECK_FIELD(CRealMyRoom_AuthorityDlg, m_pStorageUnLockBtn, 0x13C);
	CHECK_FIELD(CRealMyRoom_AuthorityDlg, m_pStorageChangePassBtn, 0x140);
END_LAYOUT_CHECK

// Password-entry dialog of the my room. Size 0x138, the `new` of
// CRealMyRoom_EnterSettingMakeInstance.
class CRealMyRoom_EnterSetting : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // realmyroom_authoritydlg.h:85

	CRealMyRoom_EnterSetting(void);
	virtual ~CRealMyRoom_EnterSetting(void);

	std::string GetPassWord(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnOK(void);
	virtual void OnCancle(void);

	void OnInputPwInit(intptr_t);
	bool OnInputPwEnterKey(int);

	FrEdit* m_pInputPwEdit;   // 0x110
	UNKNOWN(0x114, 0x118);
	std::string m_password;   // 0x118
};

CHECK_SIZE(CRealMyRoom_EnterSetting, 0x138);
BEGIN_LAYOUT_CHECK(CRealMyRoom_EnterSetting)
	CHECK_FIELD(CRealMyRoom_EnterSetting, m_pInputPwEdit, 0x110);
	CHECK_FIELD(CRealMyRoom_EnterSetting, m_password, 0x118);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_REALMYROOM_AUTHORITYDLG_H
