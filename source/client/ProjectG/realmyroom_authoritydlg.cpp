// Original: d:\build\custom\temp\code\client\projectg\realmyroom_authoritydlg.cpp
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer
#include "RealMyRoom_AuthorityDlg.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frbutton.h"
#include "fredit.h"
#include "frlistbox.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "bitmap.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "actor.h"
#include "commonutil.h"
#include <stdio.h>
#include <string.h>

// Names of the two authority kinds ("public", "private" in EUC-KR), read
// through constants: ListName is initialized by code (a dynamic initializer
// that stores each element).
static const char* const AUTH_PUBLIC_NAME = "\xb0\xf8\x20\x20\xb0\xb3";
static const char* const AUTH_PRIVATE_NAME = "\xba\xf1\xb0\xf8\xb0\xb3";

// realmyroom_authoritydlg.cpp:28
const char* ListName[] = { AUTH_PUBLIC_NAME, AUTH_PRIVATE_NAME };

// realmyroom_authoritydlg.cpp:31 (run-time type and factory registration of CRealMyRoom_AuthorityDlg)
const WRTTI CRealMyRoom_AuthorityDlg::m_RTTI("CRealMyRoom_AuthorityDlg", &FrForm::m_RTTI);

IObject* CRealMyRoom_AuthorityDlgMakeInstance(void) {
	return new CRealMyRoom_AuthorityDlg;
}

struct __sCRealMyRoom_AuthorityDlg {
	__sCRealMyRoom_AuthorityDlg(void) {
		ObjectFactory().AddObjectFunctor(CRealMyRoom_AuthorityDlgMakeInstance, "CRealMyRoom_AuthorityDlg");
	}
};
static __sCRealMyRoom_AuthorityDlg s_registerCRealMyRoom_AuthorityDlg;

// realmyroom_authoritydlg.cpp:33
const sFRESH_MSGMAP* CRealMyRoom_AuthorityDlg::GetMessageMap(void) const {
	return &CRealMyRoom_AuthorityDlg::_MsgMap;
}

sFRESH_MSGMAP CRealMyRoom_AuthorityDlg::_MsgMap = { &FrForm::_MsgMap, &CRealMyRoom_AuthorityDlg::_MsgEntries[0] };

sFRESH_ENTRY CRealMyRoom_AuthorityDlg::_MsgEntries[] = {
	// realmyroom_authoritydlg.cpp:35
	{ "kind_authority", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CRealMyRoom_AuthorityDlg::OnAuth_KindPowerInit },
	// realmyroom_authoritydlg.cpp:36
	{ "kind_authority", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CRealMyRoom_AuthorityDlg::OnAuth_KindPowerOwnerdraw },
	// realmyroom_authoritydlg.cpp:37
	{ "kind_authority", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CRealMyRoom_AuthorityDlg::OnAuth_KindPowerBtnDown },

	// realmyroom_authoritydlg.cpp:39
	{ "authority_pw", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CRealMyRoom_AuthorityDlg::OnAuth_PasswordInit },
	// realmyroom_authoritydlg.cpp:40
	{ "authority_pw", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CRealMyRoom_AuthorityDlg::OnAuth_PWEnterKey },

	// realmyroom_authoritydlg.cpp:42
	{ "auth_frm03", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CRealMyRoom_AuthorityDlg::OnAuthEditBgInit },

	// realmyroom_authoritydlg.cpp:44
	{ "storage_lock", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CRealMyRoom_AuthorityDlg::OnInitStorageLock },
	// realmyroom_authoritydlg.cpp:45
	{ "storage_lock", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CRealMyRoom_AuthorityDlg::OnLBDownStorageLock },
	// realmyroom_authoritydlg.cpp:46
	{ "storage_unlock", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CRealMyRoom_AuthorityDlg::OnInitStorageUnLock },
	// realmyroom_authoritydlg.cpp:47
	{ "storage_unlock", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CRealMyRoom_AuthorityDlg::OnLBDownStorageUnLock },
	// realmyroom_authoritydlg.cpp:48
	{ "storage_changepass", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CRealMyRoom_AuthorityDlg::OnInitStorageChangePass },
	// realmyroom_authoritydlg.cpp:49
	{ "storage_changepass", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CRealMyRoom_AuthorityDlg::OnLBDownStorageChangePass },

	// realmyroom_authoritydlg.cpp:52
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// realmyroom_authoritydlg.cpp:56
CRealMyRoom_AuthorityDlg::CRealMyRoom_AuthorityDlg(void) {
	m_bAuthSetting = WSingleton<CSharedDoc>::Instance()->m_authSetting;
	OnLoadImage();

	m_bStorageLock = WSingleton<CSharedDoc>::Instance()->m_storageLock;
}

// realmyroom_authoritydlg.cpp:65
CRealMyRoom_AuthorityDlg::~CRealMyRoom_AuthorityDlg(void) {
}

// realmyroom_authoritydlg.cpp:75
void CRealMyRoom_AuthorityDlg::OnAuth_KindPowerInit(intptr_t param) {
	m_pKindPowerList = DYNAMIC_CAST(FrListBox, param);
	if (m_pKindPowerList) {


		m_pKindPowerList->SetMultiSelect(false);


		m_pKindPowerList->AddItem((void*)ListName[0]);


		m_pKindPowerList->AddItem((void*)ListName[1]);


		FrListItem* pItem = m_pKindPowerList->FindItem((void*)ListName[m_bAuthSetting]);
		m_pKindPowerList->SelectItem(pItem, true);
	}
}

// realmyroom_authoritydlg.cpp:96
void CRealMyRoom_AuthorityDlg::OnAuth_KindPowerOwnerdraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;

	if (pItem) {
		const Bitmap* pBitmap;
		FrGraphicInterface* pGI = g_pFresh->GetManager()->GetGDI();

		if (pItem->pData == 0) {
			pBitmap = m_pButtonImage[pItem->no][2];
		}
		else if (pItem->selected) {
			pBitmap = m_pButtonImage[pItem->no][2];
			pGI->SetTextColor(0xffff0000, 0xffffffff);
		}
		else if (pItem->underCursor) {
			pBitmap = m_pButtonImage[pItem->no][1];
			pGI->SetTextColor(0xff000000, 0xffffffff);
		}
		else {
			pBitmap = m_pButtonImage[pItem->no][0];
			pGI->SetTextColor(0xff000000, 0xffffffff);
		}

		if (pBitmap) {
			pGI->DrawTexture(pBitmap, WRect(pItem->pos.x, pItem->pos.y, pBitmap->Width(), pBitmap->Height()), 0xffffffff, 0);
		}
	}
}

// realmyroom_authoritydlg.cpp:128
void CRealMyRoom_AuthorityDlg::OnAuth_KindPowerBtnDown(void) {
	FrListItem* pItem = m_pKindPowerList->GetItemUnderCursor();
	if (pItem == 0) {
		return;
	}

	if (pItem->no == m_bAuthSetting) {
		m_pKindPowerList->SelectItem(pItem, true);
	}
	m_bAuthSetting = pItem->no != 0;

	m_pPasswordEdit->SetVisible(m_bAuthSetting);
}

// realmyroom_authoritydlg.cpp:154
void CRealMyRoom_AuthorityDlg::OnAuth_PasswordInit(intptr_t param) {
	m_pPasswordEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pPasswordEdit) {
		m_pPasswordEdit->SetVisible(m_bAuthSetting);

		if (m_bAuthSetting == true) {
			m_pPasswordEdit->AddLine(MakeStr("%s", WSingleton<CSharedDoc>::Instance()->m_authPassword), 0, false);
		}
	}
}

// realmyroom_authoritydlg.cpp:168
void CRealMyRoom_AuthorityDlg::OnAuthEditBgInit(int) {
}

// realmyroom_authoritydlg.cpp:183
void CRealMyRoom_AuthorityDlg::OnInitStorageLock(intptr_t param) {
	m_pStorageLockBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pStorageLockBtn) {

		m_pStorageLockBtn->SetPushDelay(0.1f);

		if (m_bStorageLock == true) {

			m_pStorageLockBtn->SetButtonImg("rmr_set_lock_o", FrButton::NORMAL);
		}
		else {
			m_pStorageLockBtn->SetButtonImg("rmr_set_lock_n", FrButton::NORMAL);
		}
	}
}

// realmyroom_authoritydlg.cpp:202
void CRealMyRoom_AuthorityDlg::OnLBDownStorageLock(void) {
	m_bStorageLock = true;

	if (WSingleton<CSharedDoc>::Instance()->m_storageLock != true) {

		m_pStorageLockBtn->SetButtonImg("rmr_set_lock_o", FrButton::NORMAL);
		m_pStorageUnLockBtn->SetButtonImg("rmr_set_unlock_n", FrButton::NORMAL);

		AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x238, 0, 0, 0, 0, 0);
		Close(FrCANCEL, true);
	}
}

// realmyroom_authoritydlg.cpp:216
void CRealMyRoom_AuthorityDlg::OnInitStorageUnLock(intptr_t param) {
	m_pStorageUnLockBtn = DYNAMIC_CAST(FrButton, param);

	// The original tests the lock button here, not the unlock button.
	if (m_pStorageLockBtn) {

		m_pStorageUnLockBtn->SetPushDelay(0.1f);

		if (m_bStorageLock == false) {

			m_pStorageUnLockBtn->SetButtonImg("rmr_set_unlock_o", FrButton::NORMAL);
		}
		else {
			m_pStorageUnLockBtn->SetButtonImg("rmr_set_unlock_n", FrButton::NORMAL);
		}
	}
}

// realmyroom_authoritydlg.cpp:235
void CRealMyRoom_AuthorityDlg::OnLBDownStorageUnLock(void) {
	m_bStorageLock = false;

	if (WSingleton<CSharedDoc>::Instance()->m_storageLock != false) {

		m_pStorageLockBtn->SetButtonImg("rmr_set_lock_n", FrButton::NORMAL);
		m_pStorageUnLockBtn->SetButtonImg("rmr_set_unlock_o", FrButton::NORMAL);

		AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x239, 0, 0, 0, 0, 0);
		Close(FrCANCEL, true);
	}
}

// realmyroom_authoritydlg.cpp:249
void CRealMyRoom_AuthorityDlg::OnInitStorageChangePass(intptr_t param) {
	m_pStorageChangePassBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pStorageChangePassBtn) {

		m_pStorageChangePassBtn->SetPushDelay(0.1f);
	}
}

// realmyroom_authoritydlg.cpp:259
void CRealMyRoom_AuthorityDlg::OnLBDownStorageChangePass(void) {
	AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x237, 0, 0, 0, 0, 0);
	Close(FrCANCEL, true);
}

// realmyroom_authoritydlg.cpp:267
bool CRealMyRoom_AuthorityDlg::OnAuth_PWEnterKey(int) {
	OnFreshOkay();
	return true;
}

// realmyroom_authoritydlg.cpp:280
void CRealMyRoom_AuthorityDlg::OnLoadImage(void) {
	m_pButtonImage[0][0] = g_pFresh->RegisterBitmap("rmr_btn_open_n");
	m_pButtonImage[0][1] = g_pFresh->RegisterBitmap("rmr_btn_open_o");
	m_pButtonImage[0][2] = g_pFresh->RegisterBitmap("rmr_btn_open_d");
	m_pButtonImage[1][0] = g_pFresh->RegisterBitmap("rmr_btn_close_n");
	m_pButtonImage[1][1] = g_pFresh->RegisterBitmap("rmr_btn_close_o");
	m_pButtonImage[1][2] = g_pFresh->RegisterBitmap("rmr_btn_close_d");
}

// realmyroom_authoritydlg.cpp:300
void CRealMyRoom_AuthorityDlg::OnOK(void) {
	std::string password = m_pPasswordEdit->GetLine(1, false);
	char buffer[100];

	sprintf(buffer, "%s", m_pPasswordEdit->GetLine(1, false));

	FrForm::OnOK();
}

// realmyroom_authoritydlg.cpp:325
bool CRealMyRoom_AuthorityDlg::GetAuthSetting(void) {
	return m_bAuthSetting;
}

// realmyroom_authoritydlg.cpp:331
void CRealMyRoom_AuthorityDlg::GetPassWord(void) {

	std::string password;

	if (m_pPasswordEdit) {

		password = m_pPasswordEdit->GetLine(1, false);

		strcpy(WSingleton<CSharedDoc>::Instance()->m_authPassword, password.c_str());
	}
}

// realmyroom_authoritydlg.cpp:348
void CRealMyRoom_AuthorityDlg::OnCancle(void) {
	FrForm::OnCancel();
}

// realmyroom_authoritydlg.cpp:361 (run-time type and factory registration of CRealMyRoom_EnterSetting)
const WRTTI CRealMyRoom_EnterSetting::m_RTTI("CRealMyRoom_EnterSetting", &FrForm::m_RTTI);

IObject* CRealMyRoom_EnterSettingMakeInstance(void) {
	return new CRealMyRoom_EnterSetting;
}

struct __sCRealMyRoom_EnterSetting {
	__sCRealMyRoom_EnterSetting(void) {
		ObjectFactory().AddObjectFunctor(CRealMyRoom_EnterSettingMakeInstance, "CRealMyRoom_EnterSetting");
	}
};
static __sCRealMyRoom_EnterSetting s_registerCRealMyRoom_EnterSetting;

// realmyroom_authoritydlg.cpp:363
const sFRESH_MSGMAP* CRealMyRoom_EnterSetting::GetMessageMap(void) const {
	return &CRealMyRoom_EnterSetting::_MsgMap;
}

sFRESH_MSGMAP CRealMyRoom_EnterSetting::_MsgMap = { &FrForm::_MsgMap, &CRealMyRoom_EnterSetting::_MsgEntries[0] };

sFRESH_ENTRY CRealMyRoom_EnterSetting::_MsgEntries[] = {
	// realmyroom_authoritydlg.cpp:365
	{ "input_pw", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CRealMyRoom_EnterSetting::OnInputPwInit },
	// realmyroom_authoritydlg.cpp:366
	{ "input_pw", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CRealMyRoom_EnterSetting::OnInputPwEnterKey },

	// realmyroom_authoritydlg.cpp:373
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// realmyroom_authoritydlg.cpp:377
CRealMyRoom_EnterSetting::CRealMyRoom_EnterSetting(void) {
}

// realmyroom_authoritydlg.cpp:381
CRealMyRoom_EnterSetting::~CRealMyRoom_EnterSetting(void) {
}

// realmyroom_authoritydlg.cpp:391
void CRealMyRoom_EnterSetting::OnInputPwInit(intptr_t param) {
	m_pInputPwEdit = DYNAMIC_CAST(FrEdit, param);

	if (m_pInputPwEdit) {



		m_pInputPwEdit->ClearLine();
	}
}

// realmyroom_authoritydlg.cpp:469
void CRealMyRoom_EnterSetting::OnOK(void) {
	if (m_pInputPwEdit != 0) {
		m_password = m_pInputPwEdit->GetLine(1, false);
	}

	FrForm::OnOK();
}

// realmyroom_authoritydlg.cpp:478
void CRealMyRoom_EnterSetting::OnCancle(void) {
	FrForm::OnCancel();
}

// realmyroom_authoritydlg.cpp:483
std::string CRealMyRoom_EnterSetting::GetPassWord(void) {
	return m_password;
}

// realmyroom_authoritydlg.cpp:494
bool CRealMyRoom_EnterSetting::OnInputPwEnterKey(int) {
	OnFreshOkay();
	return true;
}
