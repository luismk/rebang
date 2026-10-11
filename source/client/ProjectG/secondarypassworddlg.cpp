// Original: d:\build\custom\temp\code\client\projectg\secondarypassworddlg.cpp
#include <windows.h>

#include <algorithm>
#include <mbstring.h>

#include "SecondaryPasswordDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "MessageManager.h"
#include "TaskManager.h"
#include "actor.h"
#include "commonutil.h"
#include "s5/Utilities.h"

namespace _gateway {

// Name of the gateway actor the dialogs send their requests to.
static const char* s_gatewayActorName = "Gateway";

// secondarypassworddlg.cpp:27 (run-time type and factory registration of FrPasswordInputDlg)
const WRTTI FrPasswordInputDlg::m_RTTI("FrPasswordInputDlg", &FrForm::m_RTTI);

IObject* FrPasswordInputDlgMakeInstance(void) {
	return new FrPasswordInputDlg;
}

struct __sFrPasswordInputDlg {
	__sFrPasswordInputDlg(void) {
		ObjectFactory().AddObjectFunctor(FrPasswordInputDlgMakeInstance, "FrPasswordInputDlg");
	}
};
static __sFrPasswordInputDlg s_registerFrPasswordInputDlg;

// secondarypassworddlg.cpp:30
const sFRESH_MSGMAP* FrPasswordInputDlg::GetMessageMap(void) const {
	return &FrPasswordInputDlg::_MsgMap;
}

sFRESH_MSGMAP FrPasswordInputDlg::_MsgMap = { &FrForm::_MsgMap, &FrPasswordInputDlg::_MsgEntries[0] };

sFRESH_ENTRY FrPasswordInputDlg::_MsgEntries[] = {
	// secondarypassworddlg.cpp:32
	{ "btn_slot_0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnInitNumberSlot0 },
	// secondarypassworddlg.cpp:33
	{ "btn_slot_1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnInitNumberSlot1 },
	// secondarypassworddlg.cpp:34
	{ "btn_slot_2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnInitNumberSlot2 },
	// secondarypassworddlg.cpp:35
	{ "btn_slot_3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnInitNumberSlot3 },
	// secondarypassworddlg.cpp:36
	{ "btn_slot_4", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnInitNumberSlot4 },
	// secondarypassworddlg.cpp:37
	{ "btn_slot_5", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnInitNumberSlot5 },
	// secondarypassworddlg.cpp:38
	{ "btn_slot_6", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnInitNumberSlot6 },
	// secondarypassworddlg.cpp:39
	{ "btn_slot_7", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnInitNumberSlot7 },
	// secondarypassworddlg.cpp:40
	{ "btn_slot_8", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnInitNumberSlot8 },
	// secondarypassworddlg.cpp:41
	{ "btn_slot_9", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnInitNumberSlot9 },

	// secondarypassworddlg.cpp:43
	{ "btn_back", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnBackSpace_ButtonInit },
	// secondarypassworddlg.cpp:44
	{ "btn_clear", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnClear_ButtonInit },
	// secondarypassworddlg.cpp:45
	{ "btn_create", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnCreatePassword_ButtonInit },
	// secondarypassworddlg.cpp:46
	{ "btn_confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnConfirm_ButtonInit },
	// secondarypassworddlg.cpp:47
	{ "btn_change_pwd", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnChangePassword_ButtonInit },
	// secondarypassworddlg.cpp:48
	{ "btn_report", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnReport_ButtonInit },

	// secondarypassworddlg.cpp:50
	{ "btn_slot_0", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnLButtonUpNumberSlot0 },
	// secondarypassworddlg.cpp:51
	{ "btn_slot_1", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnLButtonUpNumberSlot1 },
	// secondarypassworddlg.cpp:52
	{ "btn_slot_2", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnLButtonUpNumberSlot2 },
	// secondarypassworddlg.cpp:53
	{ "btn_slot_3", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnLButtonUpNumberSlot3 },
	// secondarypassworddlg.cpp:54
	{ "btn_slot_4", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnLButtonUpNumberSlot4 },
	// secondarypassworddlg.cpp:55
	{ "btn_slot_5", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnLButtonUpNumberSlot5 },
	// secondarypassworddlg.cpp:56
	{ "btn_slot_6", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnLButtonUpNumberSlot6 },
	// secondarypassworddlg.cpp:57
	{ "btn_slot_7", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnLButtonUpNumberSlot7 },
	// secondarypassworddlg.cpp:58
	{ "btn_slot_8", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnLButtonUpNumberSlot8 },
	// secondarypassworddlg.cpp:59
	{ "btn_slot_9", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnLButtonUpNumberSlot9 },

	// secondarypassworddlg.cpp:61
	{ "btn_back", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnBackSpace_LButtonUp },
	// secondarypassworddlg.cpp:62
	{ "btn_clear", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnClear_LButtonUp },
	// secondarypassworddlg.cpp:63
	{ "btn_create", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnCreatePassword_LButtonUp },
	// secondarypassworddlg.cpp:64
	{ "btn_confirm", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnConfirm_LButtonUp },
	// secondarypassworddlg.cpp:65
	{ "btn_change_pwd", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnChangePassword_LButtonUp },
	// secondarypassworddlg.cpp:66
	{ "btn_report", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnReport_LButtonUp },

	// secondarypassworddlg.cpp:68
	{ "confirm_first_pwd", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnConfirmFirstPassword_ButtonInit },
	// secondarypassworddlg.cpp:69
	{ "confirm_first_pwd", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnConfirmFirstPassword_LButtonUp },

	// secondarypassworddlg.cpp:71
	{ "caption", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnCaption_AreaInit },

	// secondarypassworddlg.cpp:73
	{ "explane_block1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnExplaneBlock_AreaInit },
	// secondarypassworddlg.cpp:74
	{ "explane_block1", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnExplaneBlock_AreaOwnerdraw },

	// secondarypassworddlg.cpp:76
	{ "password", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnPasswordInput_EditInit },
	// secondarypassworddlg.cpp:77
	{ "password_re", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnPasswordConfirm_EditInit },

	// secondarypassworddlg.cpp:79
	{ "btn_close", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnClose_ButtonInit },
	// secondarypassworddlg.cpp:80
	{ "btn_close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnClose_LButtonUp },

	// secondarypassworddlg.cpp:82
	{ "input_again", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordInputDlg::OnInputAgain_ButtonInit },
	// secondarypassworddlg.cpp:83
	{ "input_again", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordInputDlg::OnInputAgain_LButtonUp },

	// secondarypassworddlg.cpp:85
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// secondarypassworddlg.cpp:91
FrPasswordInputDlg::FrPasswordInputDlg(void)
	: m_dlgType(eSecondPwdInputType(0)), m_inputSequence(eInputSequence(0)) {
	InitVariable();
}

// secondarypassworddlg.cpp:96
FrPasswordInputDlg::~FrPasswordInputDlg(void) {
}

// secondarypassworddlg.cpp:101
bool FrPasswordInputDlg::OnInit(void) {
	LoadResource();

	switch (m_dlgType) {
	case 0:
		ConvertCreateInput();
		ChangeInputSequence(eInputSequence(1));
		break;
	case 1:
		ConvertConfirmInput();
		ChangeInputSequence(eInputSequence(2));
		break;
	}

	for (int i = 0; i < 10; i++) {
		m_slotNumbers.push_back(i);
	}

	ArrangeButtonImage();

	return FrForm::OnInit();
}

// secondarypassworddlg.cpp:132
void FrPasswordInputDlg::InitVariable(void) {
	memset(m_pButtons, 0, sizeof(m_pButtons));
	m_pExplaneBlockArea = 0;
	memset(m_pPasswordEdit, 0, sizeof(m_pPasswordEdit));
	memset(m_pBitmaps, 0, sizeof(m_pBitmaps));
	m_pCurrentEdit = 0;
}

// secondarypassworddlg.cpp:141
void FrPasswordInputDlg::LoadResource(void) {
	const char* szBitmap[13] = {
		"2ndPW_text_title_createpw",
		"2ndPW_text_title_insertpw",
		"2ndPW_text_top_Insertnew",
		"2ndPW_text_top_Insert",
		"2ndPW_text_top_declare",
		"2ndPW_text_newpassword_I",
		"2ndPW_text_newpassword_C",
		"2ndPW_Insertbox",
		"2ndPW_text_warning_01",
		"2ndPW_text_warning_02",
		"2ndPW_Icon_notice_s",
		"2ndPW_Insertbox_mask",
		"2ndPW_Insertbox_mask02"
	};

	for (int i = 0; i < 13; i++) {
		m_pBitmaps[i] = g_pFresh->RegisterBitmap(szBitmap[i]);
	}
}

// secondarypassworddlg.cpp:167
bool FrPasswordInputDlg::ConvertCreateInput(void) {
	S5::DisableFreshWindow(m_pButtons[13]);
	S5::DisableFreshWindow(m_pButtons[14]);
	S5::DisableFreshWindow(m_pButtons[15]);
	S5::DisableFreshWindow(m_pButtons[17]);
	m_pButtons[12]->Enable(false);

	m_pCaptionArea->SetBgImg(m_pBitmaps[0]);

	return true;
}

// secondarypassworddlg.cpp:182
bool FrPasswordInputDlg::ConvertConfirmInput(void) {
	S5::DisableFreshWindow(m_pButtons[12]);
	S5::DisableFreshWindow(m_pButtons[16]);
	S5::DisableFreshWindow(m_pButtons[17]);

	m_pCaptionArea->SetBgImg(m_pBitmaps[1]);

	S5::DisableFreshWindow(m_pPasswordEdit[1]);

	return true;
}

// secondarypassworddlg.cpp:198
bool FrPasswordInputDlg::SetButtonProperty(int index, int param) {
	m_pButtons[index] = DYNAMIC_CAST(FrButton, param);
	if (m_pButtons[index] == 0) {
		return false;
	}

	return true;
}

// secondarypassworddlg.cpp:209
void FrPasswordInputDlg::PushNumButton(int slot) {
	if (m_password.length() >= 8) {
		return;
	}

	InsertPasswordChar(m_slotNumbers[slot]);
	CheckEnableCreateButton();
}

// secondarypassworddlg.cpp:220
void FillAsterisk(char* buffer, int length) {
	for (int i = 0; i < length; i++) {
		buffer[i] = '*';
	}
}

// secondarypassworddlg.cpp:228
void FrPasswordInputDlg::InsertPasswordChar(int number) {
	char szNumber[4] = { 0 };
	char szAsterisk[9] = { 0 };

	_itoa(number, szNumber, 10);
	m_password += szNumber;

	int length = m_password.length();
	if (length <= 8) {
		_mbsnbcpy((unsigned char*)szAsterisk, (const unsigned char*)m_password.c_str(), length);
		FillAsterisk(szAsterisk, length - 1);
	}

	if (m_pCurrentEdit) {
		m_pCurrentEdit->ClearLine();
		m_pCurrentEdit->SetLine(1, szAsterisk, 0, false, 0);
	}
}

// secondarypassworddlg.cpp:253
void FrPasswordInputDlg::ErasePasswordChar(void) {
	if (m_password.empty()) {
		return;
	}

	char szAsterisk[9] = { 0 };

	m_password.erase(m_password.end() - 1);
	int length = m_password.length();
	if (length <= 8) {
		_mbsnbcpy((unsigned char*)szAsterisk, (const unsigned char*)m_password.c_str(), length);
		FillAsterisk(szAsterisk, length);
	}

	if (m_pCurrentEdit) {
		m_pCurrentEdit->ClearLine();
		m_pCurrentEdit->SetLine(1, szAsterisk, 0, false, 0);
	}
}

// secondarypassworddlg.cpp:277
void FrPasswordInputDlg::ClearPassword(void) {
	if (m_password.empty()) {
		return;
	}

	if (m_pCurrentEdit) {
		m_pCurrentEdit->ClearLine();
		m_password.clear();
	}
}

// secondarypassworddlg.cpp:291
bool FrPasswordInputDlg::ChangeInputSequence(eInputSequence sequence) {
	if (sequence == m_inputSequence) {
		return false;
	}

	if (sequence == 1) {
		m_pCurrentEdit = m_pPasswordEdit[0];

		m_pCurrentEdit->ClearLine();
		m_pPasswordEdit[1]->ClearLine();

		m_password.clear();
		m_confirmPassword.clear();

		S5::EnableFreshWindow(m_pButtons[16], true);
	} else if (sequence == 2) {
		if (m_inputSequence == 1) {
			char szAsterisk[9] = { 0 };
			_mbsnbcpy((unsigned char*)szAsterisk, (const unsigned char*)m_password.c_str(), m_password.length());
			FillAsterisk(szAsterisk, m_password.length());

			m_pPasswordEdit[0]->ClearLine();
			m_pPasswordEdit[0]->SetLine(1, szAsterisk, 0, false, 0);

			m_pButtons[16]->Enable(false);
			m_pCurrentEdit = m_pPasswordEdit[1];

			S5::EnableFreshWindow(m_pButtons[17]);
		} else {
			m_pCurrentEdit = m_pPasswordEdit[0];
		}

		m_pPasswordEdit[1]->ClearLine();

		m_confirmPassword = m_password;
		m_password.clear();
	} else {
		return false;
	}

	m_inputSequence = sequence;

	return true;
}

// secondarypassworddlg.cpp:348
bool FrPasswordInputDlg::CheckInputPassword(void) {
	return m_password == m_confirmPassword ? true : false;
}

// secondarypassworddlg.cpp:358
void FrPasswordInputDlg::ArrangeButtonImage(void) {
	std::random_shuffle(m_slotNumbers.begin(), m_slotNumbers.end());
	for (int i = 0; i < 10; i++) {
		m_pButtons[i]->SetButtonImg(MakeStr("2ndPW_no%d_n", m_slotNumbers[i]), FrButton::NORMAL);
		m_pButtons[i]->SetButtonImg(MakeStr("2ndPW_no%d_o", m_slotNumbers[i]), FrButton::OVER);
		m_pButtons[i]->SetButtonImg(MakeStr("2ndPW_no%d_d", m_slotNumbers[i]), FrButton::PRESSED);
	}
}

// secondarypassworddlg.cpp:369
void FrPasswordInputDlg::DrawCreateDlgExplane(FrGraphicInterface* pGI) {
	const Bitmap* pBitmap;

	pBitmap = m_pBitmaps[2];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 135.0f, m_rect.y + 35.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = m_pBitmaps[5];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 93.0f, m_rect.y + 72.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = m_pBitmaps[6];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 93.0f, m_rect.y + 103.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = m_pBitmaps[7];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 199.0f, m_rect.y + 69.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);

		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 199.0f, m_rect.y + 100.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	switch (m_inputSequence) {
	case 1:
		pBitmap = m_pBitmaps[12];
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 199.0f, m_rect.y + 100.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
		break;

	case 2:
		pBitmap = m_pBitmaps[11];
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 199.0f, m_rect.y + 69.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
		break;
	}
}

// secondarypassworddlg.cpp:424
void FrPasswordInputDlg::DrawLoginDlgExplane(FrGraphicInterface* pGI) {
	const Bitmap* pBitmap;

	pBitmap = m_pBitmaps[3];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 151.0f, m_rect.y + 39.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = m_pBitmaps[7];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 199.0f, m_rect.y + 69.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = m_pBitmaps[10];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 87.0f, m_rect.y + 94.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = m_pBitmaps[4];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 115.0f, m_rect.y + 102.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}
}

// secondarypassworddlg.cpp:456
void FrPasswordInputDlg::CheckEnableCreateButton(void) {
	if (m_dlgType != 0) {
		return;
	}

	if (m_inputSequence != 2) {
		return;
	}

	if (m_password.length() > 4 && m_confirmPassword.length() > 4) {
		m_pButtons[12]->Enable(true);
	} else {
		m_pButtons[12]->Enable(false);
	}
}

// secondarypassworddlg.cpp:480
void FrPasswordInputDlg::OnInitNumberSlot0(int param) {
	SetButtonProperty(0, param);
}

// secondarypassworddlg.cpp:485
void FrPasswordInputDlg::OnInitNumberSlot1(int param) {
	SetButtonProperty(1, param);
}

// secondarypassworddlg.cpp:490
void FrPasswordInputDlg::OnInitNumberSlot2(int param) {
	SetButtonProperty(2, param);
}

// secondarypassworddlg.cpp:495
void FrPasswordInputDlg::OnInitNumberSlot3(int param) {
	SetButtonProperty(3, param);
}

// secondarypassworddlg.cpp:500
void FrPasswordInputDlg::OnInitNumberSlot4(int param) {
	SetButtonProperty(4, param);
}

// secondarypassworddlg.cpp:505
void FrPasswordInputDlg::OnInitNumberSlot5(int param) {
	SetButtonProperty(5, param);
}

// secondarypassworddlg.cpp:510
void FrPasswordInputDlg::OnInitNumberSlot6(int param) {
	SetButtonProperty(6, param);
}

// secondarypassworddlg.cpp:515
void FrPasswordInputDlg::OnInitNumberSlot7(int param) {
	SetButtonProperty(7, param);
}

// secondarypassworddlg.cpp:520
void FrPasswordInputDlg::OnInitNumberSlot8(int param) {
	SetButtonProperty(8, param);
}

// secondarypassworddlg.cpp:525
void FrPasswordInputDlg::OnInitNumberSlot9(int param) {
	SetButtonProperty(9, param);
}

// secondarypassworddlg.cpp:530
void FrPasswordInputDlg::OnBackSpace_ButtonInit(int param) {
	SetButtonProperty(10, param);
}

// secondarypassworddlg.cpp:535
void FrPasswordInputDlg::OnClear_ButtonInit(int param) {
	SetButtonProperty(11, param);
}

// secondarypassworddlg.cpp:540
void FrPasswordInputDlg::OnCreatePassword_ButtonInit(int param) {
	SetButtonProperty(12, param);
}

// secondarypassworddlg.cpp:545
void FrPasswordInputDlg::OnConfirm_ButtonInit(intptr_t param) {
	SetButtonProperty(13, param);
}

// secondarypassworddlg.cpp:550
void FrPasswordInputDlg::OnChangePassword_ButtonInit(int param) {
	SetButtonProperty(14, param);
}

// secondarypassworddlg.cpp:555
void FrPasswordInputDlg::OnReport_ButtonInit(int param) {
	SetButtonProperty(15, param);
}

// secondarypassworddlg.cpp:560
void FrPasswordInputDlg::OnLButtonUpNumberSlot0(void) {
	PushNumButton(0);
}

// secondarypassworddlg.cpp:565
void FrPasswordInputDlg::OnLButtonUpNumberSlot1(void) {
	PushNumButton(1);
}

// secondarypassworddlg.cpp:570
void FrPasswordInputDlg::OnLButtonUpNumberSlot2(void) {
	PushNumButton(2);
}

// secondarypassworddlg.cpp:575
void FrPasswordInputDlg::OnLButtonUpNumberSlot3(void) {
	PushNumButton(3);
}

// secondarypassworddlg.cpp:580
void FrPasswordInputDlg::OnLButtonUpNumberSlot4(void) {
	PushNumButton(4);
}

// secondarypassworddlg.cpp:585
void FrPasswordInputDlg::OnLButtonUpNumberSlot5(void) {
	PushNumButton(5);
}

// secondarypassworddlg.cpp:590
void FrPasswordInputDlg::OnLButtonUpNumberSlot6(void) {
	PushNumButton(6);
}

// secondarypassworddlg.cpp:595
void FrPasswordInputDlg::OnLButtonUpNumberSlot7(void) {
	PushNumButton(7);
}

// secondarypassworddlg.cpp:600
void FrPasswordInputDlg::OnLButtonUpNumberSlot8(void) {
	PushNumButton(8);
}

// secondarypassworddlg.cpp:605
void FrPasswordInputDlg::OnLButtonUpNumberSlot9(void) {
	PushNumButton(9);
}

// secondarypassworddlg.cpp:610
void FrPasswordInputDlg::OnBackSpace_LButtonUp(void) {
	ErasePasswordChar();
	CheckEnableCreateButton();
}

// secondarypassworddlg.cpp:616
void FrPasswordInputDlg::OnClear_LButtonUp(void) {
	ClearPassword();
	CheckEnableCreateButton();
}

// secondarypassworddlg.cpp:622
void FrPasswordInputDlg::OnCreatePassword_LButtonUp(void) {
	if (!(m_password == m_confirmPassword)) {
		AfxGetTask()->GetActor(s_gatewayActorName) << MsgObject(0, 0x26d, 0x16, 0x64, 0, 0, 0);
		if (m_inputSequence == 1) {
			ClearPassword();
		} else {
			OnInputAgain_LButtonUp();
			m_pButtons[12]->Enable(false);
		}
	} else {
		bool bValid = true;
		if (m_dlgType == 0) {
			AfxGetTask()->GetActor(s_gatewayActorName) << MsgObject(0, 0x26d, 0x15, (intptr_t)&m_confirmPassword, (intptr_t)&bValid, 0, 0);
		}

		if (!bValid) {
			OnInputAgain_LButtonUp();
			m_pButtons[12]->Enable(false);
		} else {
			AfxGetTask()->GetActor(s_gatewayActorName) << MsgObject(0, 0x26d, 2, (intptr_t)m_confirmPassword.c_str(), 0, 0, 0);
		}
	}
}

// secondarypassworddlg.cpp:660
void FrPasswordInputDlg::OnConfirm_LButtonUp(void) {
	bool bValid = true;

	switch (m_dlgType) {
	case 1:
		AfxGetTask()->GetActor(s_gatewayActorName) << MsgObject(0, 0x26d, 0x15, (intptr_t)&m_password, (intptr_t)&bValid, 0, 0);
		break;
	case 0:
		AfxGetTask()->GetActor(s_gatewayActorName) << MsgObject(0, 0x26d, 0x15, (intptr_t)&m_confirmPassword, (intptr_t)&bValid, 0, 0);
		break;
	}

	if (!bValid) {
		ClearPassword();
		return;
	}

	AfxGetTask()->GetActor(s_gatewayActorName) << MsgObject(0, 0x26d, 0xc, (intptr_t)m_password.c_str(), 0, 0, 0);
	ClearPassword();
}

// secondarypassworddlg.cpp:690
void FrPasswordInputDlg::OnChangePassword_LButtonUp(void) {
	IActor* pActor = AfxGetTask()->GetActor(s_gatewayActorName);
	pActor << MsgObject(0, 0x26d, 0xd, 0, 0, 0, 0);
}

// secondarypassworddlg.cpp:696
void FrPasswordInputDlg::OnReport_LButtonUp(void) {
	AfxGetTask()->GetActor(s_gatewayActorName) << MsgObject(0, 0x26d, 0x16, 0x75, 0, 0, 0);
}

// secondarypassworddlg.cpp:702
void FrPasswordInputDlg::OnConfirmFirstPassword_ButtonInit(int param) {
	SetButtonProperty(16, param);
}

// secondarypassworddlg.cpp:707
void FrPasswordInputDlg::OnConfirmFirstPassword_LButtonUp(void) {
	bool bValid = true;
	if (m_dlgType == 0 && m_inputSequence == 1) {
		AfxGetTask()->GetActor(s_gatewayActorName) << MsgObject(0, 0x26d, 0x15, (intptr_t)&m_password, (intptr_t)&bValid, 0, 0);
	}

	if (!bValid) {
		ClearPassword();
		return;
	}

	ChangeInputSequence(eInputSequence(2));

	S5::DisableFreshWindow(m_pButtons[16]);
}

// secondarypassworddlg.cpp:731
void FrPasswordInputDlg::OnCaption_AreaInit(intptr_t param) {
	m_pCaptionArea = DYNAMIC_CAST(FrArea, param);
}

// secondarypassworddlg.cpp:736
void FrPasswordInputDlg::OnExplaneBlock_AreaInit(intptr_t param) {
	m_pExplaneBlockArea = DYNAMIC_CAST(FrArea, param);
}

// secondarypassworddlg.cpp:741
void FrPasswordInputDlg::OnExplaneBlock_AreaOwnerdraw(int param) {
	FrGraphicInterface* pGI = g_pFresh->GetManager()->GetGDI();

	switch (m_dlgType) {
	case 0:
		DrawCreateDlgExplane(pGI);
		break;
	case 1:
		DrawLoginDlgExplane(pGI);
		break;
	}

	const Bitmap* pBitmap;

	pBitmap = m_pBitmaps[8];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 20.0f, m_rect.y + 384.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = m_pBitmaps[9];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 276.0f, m_rect.y + 384.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}
}

// secondarypassworddlg.cpp:773
void FrPasswordInputDlg::OnPasswordInput_EditInit(intptr_t param) {
	m_pPasswordEdit[0] = DYNAMIC_CAST(FrEdit, param);
}

// secondarypassworddlg.cpp:778
void FrPasswordInputDlg::OnPasswordConfirm_EditInit(intptr_t param) {
	m_pPasswordEdit[1] = DYNAMIC_CAST(FrEdit, param);
}

// secondarypassworddlg.cpp:783
void FrPasswordInputDlg::OnInputAgain_ButtonInit(int param) {
	SetButtonProperty(17, param);
}

// secondarypassworddlg.cpp:788
void FrPasswordInputDlg::OnInputAgain_LButtonUp(void) {
	S5::DisableFreshWindow(m_pButtons[17]);

	ChangeInputSequence(eInputSequence(1));
	m_pButtons[12]->Enable(false);
}

// secondarypassworddlg.cpp:796
void FrPasswordInputDlg::OnClose_ButtonInit(intptr_t param) {
	SetButtonProperty(18, param);
}

// secondarypassworddlg.cpp:801
void FrPasswordInputDlg::OnClose_LButtonUp(void) {
	PostQuitMessage(0);
}

// secondarypassworddlg.cpp:836 (run-time type and factory registration of FrSelectCertification)
const WRTTI FrSelectCertification::m_RTTI("FrSelectCertification", &FrForm::m_RTTI);

IObject* FrSelectCertificationMakeInstance(void) {
	return new FrSelectCertification;
}

struct __sFrSelectCertification {
	__sFrSelectCertification(void) {
		ObjectFactory().AddObjectFunctor(FrSelectCertificationMakeInstance, "FrSelectCertification");
	}
};
static __sFrSelectCertification s_registerFrSelectCertification;

// secondarypassworddlg.cpp:839
const sFRESH_MSGMAP* FrSelectCertification::GetMessageMap(void) const {
	return &FrSelectCertification::_MsgMap;
}

sFRESH_MSGMAP FrSelectCertification::_MsgMap = { &FrForm::_MsgMap, &FrSelectCertification::_MsgEntries[0] };

sFRESH_ENTRY FrSelectCertification::_MsgEntries[] = {
	// secondarypassworddlg.cpp:842
	{ "btn_confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSelectCertification::OnRequestCertify_ButtonInit },
	// secondarypassworddlg.cpp:843
	{ "btn_confirm", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrSelectCertification::OnRequestCertify_LButtonDown },

	// secondarypassworddlg.cpp:845
	{ "btn_cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSelectCertification::OnPassCertify_ButtonInit },
	// secondarypassworddlg.cpp:846
	{ "btn_cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrSelectCertification::OnPassCertify_LButtonDown },

	// secondarypassworddlg.cpp:848
	{ "edit_left", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSelectCertification::OnLeft_EditInit },
	// secondarypassworddlg.cpp:849
	{ "edit_right", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSelectCertification::OnRight_EditInit },

	// secondarypassworddlg.cpp:851
	{ "inframe_bg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSelectCertification::OnInFrame_AreaInit },
	// secondarypassworddlg.cpp:852
	{ "inframe_bg", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSelectCertification::OnInFrame_AreaOwnerDraw },

	// secondarypassworddlg.cpp:855
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// secondarypassworddlg.cpp:858
FrSelectCertification::FrSelectCertification(void) {
	memset(m_pButtons, 0, sizeof(m_pButtons));
	memset(m_pEdits, 0, sizeof(m_pEdits));
	m_pInFrameArea = 0;

	memset(m_pBitmaps, 0, sizeof(m_pBitmaps));
}

// secondarypassworddlg.cpp:867
FrSelectCertification::~FrSelectCertification(void) {
}

// secondarypassworddlg.cpp:872
bool FrSelectCertification::OnInit(void) {
	const char* szBitmap[4] = {
		"2ndPW_inframe_01",
		"2ndPW_inframe_02",
		"2ndPW_Icon_certification",
		"2ndPW_Icon_notice_b"
	};

	for (int i = 0; i < 4; i++) {
		m_pBitmaps[i] = g_pFresh->RegisterBitmap(szBitmap[i]);
	}

	return FrForm::OnInit();
}

// secondarypassworddlg.cpp:894
void FrSelectCertification::OnRequestCertify_ButtonInit(intptr_t param) {
	m_pButtons[0] = DYNAMIC_CAST(FrButton, param);
}

// secondarypassworddlg.cpp:899
void FrSelectCertification::OnRequestCertify_LButtonDown(void) {
	Close(FrOK, true);

	IActor* pActor = AfxGetTask()->GetActor(s_gatewayActorName);
	pActor << MsgObject(0, 0x26d, 7, 0, 0, 0, 0);
	pActor << MsgObject(0, 0x26d, 0xd, 0, 0, 0, 0);
}

// secondarypassworddlg.cpp:912
void FrSelectCertification::OnPassCertify_ButtonInit(intptr_t param) {
	m_pButtons[1] = DYNAMIC_CAST(FrButton, param);
}

// secondarypassworddlg.cpp:917
void FrSelectCertification::OnPassCertify_LButtonDown(void) {
	Close(FrOK, true);

	AfxGetTask()->GetActor(s_gatewayActorName) << MsgObject(0, 0x26d, 7, 0, 0, 0, 0);
}

// secondarypassworddlg.cpp:925
void FrSelectCertification::OnLeft_EditInit(intptr_t param) {
	m_pEdits[0] = DYNAMIC_CAST(FrEdit, param);

	m_pEdits[0]->AddText(WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)0x73), false, true);
}

// secondarypassworddlg.cpp:933
void FrSelectCertification::OnRight_EditInit(intptr_t param) {
	m_pEdits[1] = DYNAMIC_CAST(FrEdit, param);

	m_pEdits[1]->AddText(WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)0x74), false, true);
}

// secondarypassworddlg.cpp:941
void FrSelectCertification::OnInFrame_AreaInit(intptr_t param) {
	m_pInFrameArea = DYNAMIC_CAST(FrArea, param);
}

// secondarypassworddlg.cpp:946
void FrSelectCertification::OnInFrame_AreaOwnerDraw(int param) {
	FrGraphicInterface* pGI = g_pFresh->GetManager()->GetGDI();

	const Bitmap* pBitmap;

	pBitmap = m_pBitmaps[0];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 53.0f, m_rect.y + 95.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 260.0f, m_rect.y + 95.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = m_pBitmaps[1];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 53.0f, m_rect.y + 351.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 260.0f, m_rect.y + 351.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = m_pBitmaps[2];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 126.0f, m_rect.y + 185.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = m_pBitmaps[3];
	if (pBitmap) {
		pGI->DrawTexture(pBitmap, WRect(m_rect.x + 330.0f, m_rect.y + 185.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}
}

// secondarypassworddlg.cpp:982 (run-time type and factory registration of FrExplanePassword)
const WRTTI FrExplanePassword::m_RTTI("FrExplanePassword", &FrForm::m_RTTI);

IObject* FrExplanePasswordMakeInstance(void) {
	return new FrExplanePassword;
}

struct __sFrExplanePassword {
	__sFrExplanePassword(void) {
		ObjectFactory().AddObjectFunctor(FrExplanePasswordMakeInstance, "FrExplanePassword");
	}
};
static __sFrExplanePassword s_registerFrExplanePassword;

// secondarypassworddlg.cpp:983
const sFRESH_MSGMAP* FrExplanePassword::GetMessageMap(void) const {
	return &FrExplanePassword::_MsgMap;
}

sFRESH_MSGMAP FrExplanePassword::_MsgMap = { &FrForm::_MsgMap, &FrExplanePassword::_MsgEntries[0] };

sFRESH_ENTRY FrExplanePassword::_MsgEntries[] = {
	// secondarypassworddlg.cpp:985
	{ "btn_opencreate", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrExplanePassword::OnOpenPasswordCreate_ButtonInit },
	// secondarypassworddlg.cpp:986
	{ "btn_opencreate", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrExplanePassword::OnOpenPasswordCreate_LButtonUp },

	// secondarypassworddlg.cpp:988
	{ "edit_explane", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrExplanePassword::OnExplane_EditInit },

	// secondarypassworddlg.cpp:990
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// secondarypassworddlg.cpp:994
FrExplanePassword::FrExplanePassword(void)
	: m_pOpenPasswordCreateButton(0), m_pExplaneEdit(0) {
}

// secondarypassworddlg.cpp:999
FrExplanePassword::~FrExplanePassword(void) {
}

// secondarypassworddlg.cpp:1004
void FrExplanePassword::OnOpenPasswordCreate_ButtonInit(intptr_t param) {
	m_pOpenPasswordCreateButton = DYNAMIC_CAST(FrButton, param);
}

// secondarypassworddlg.cpp:1010
void FrExplanePassword::OnOpenPasswordCreate_LButtonUp(void) {
	Close(FrOK, true);

	AfxGetTask()->GetActor(s_gatewayActorName) << MsgObject(0, 0x26d, 6, 0, 0, 0, 0);
}

// secondarypassworddlg.cpp:1017
void FrExplanePassword::OnExplane_EditInit(intptr_t param) {
	m_pExplaneEdit = DYNAMIC_CAST(FrEdit, param);

	m_pExplaneEdit->AddText(WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)0x72), false, true);
}

// secondarypassworddlg.cpp:1028 (run-time type and factory registration of FrPasswordNotifyDlg)
const WRTTI FrPasswordNotifyDlg::m_RTTI("FrPasswordNotifyDlg", &FrForm::m_RTTI);

IObject* FrPasswordNotifyDlgMakeInstance(void) {
	return new FrPasswordNotifyDlg;
}

struct __sFrPasswordNotifyDlg {
	__sFrPasswordNotifyDlg(void) {
		ObjectFactory().AddObjectFunctor(FrPasswordNotifyDlgMakeInstance, "FrPasswordNotifyDlg");
	}
};
static __sFrPasswordNotifyDlg s_registerFrPasswordNotifyDlg;

// secondarypassworddlg.cpp:1029
const sFRESH_MSGMAP* FrPasswordNotifyDlg::GetMessageMap(void) const {
	return &FrPasswordNotifyDlg::_MsgMap;
}

sFRESH_MSGMAP FrPasswordNotifyDlg::_MsgMap = { &FrForm::_MsgMap, &FrPasswordNotifyDlg::_MsgEntries[0] };

sFRESH_ENTRY FrPasswordNotifyDlg::_MsgEntries[] = {
	// secondarypassworddlg.cpp:1031
	{ "notify_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordNotifyDlg::OnIcon_AreaInit },
	// secondarypassworddlg.cpp:1032
	{ "notify_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordNotifyDlg::OnIcon_AreaOwnerdraw },

	// secondarypassworddlg.cpp:1034
	{ "edit_msg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordNotifyDlg::OnSystemMsg_EditInit },

	// secondarypassworddlg.cpp:1036
	{ "btn_close", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPasswordNotifyDlg::OnClose_ButtonInit },
	// secondarypassworddlg.cpp:1037
	{ "btn_close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPasswordNotifyDlg::OnClose_LButtonUp },

	// secondarypassworddlg.cpp:1039
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// secondarypassworddlg.cpp:1042
FrPasswordNotifyDlg::FrPasswordNotifyDlg(void) {
	m_notifyType = 0;
	m_pIconArea = 0;
	m_pSystemMsgEdit = 0;
	m_pCloseButton = 0;
	memset(m_pIconBitmaps, 0, sizeof(m_pIconBitmaps));
	m_bMessage = false;
}

// secondarypassworddlg.cpp:1052
FrPasswordNotifyDlg::~FrPasswordNotifyDlg(void) {
}

// secondarypassworddlg.cpp:1057
bool FrPasswordNotifyDlg::OnInit(void) {
	const char* szBitmap[3] = {
		"2ndPW_Icon_Policy",
		"2ndPW_Icon_notice_b",
		"2ndPW_Icon_confirm"
	};

	for (int i = 0; i < 3; i++) {
		m_pIconBitmaps[i] = g_pFresh->RegisterBitmap(szBitmap[i]);
	}

	switch (m_notifyType) {
	case 0x75:
		m_iconIndex = 0;
		break;
	case 0x64:
	case 0x65:
	case 0x66:
	case 0x69:
	case 0x6a:
	case 0x6c:
	case 0x6e:
	case 0x6f:
	case 0x70:
	case 0x71:
		m_iconIndex = 1;
		break;
	case 0x68:
		m_iconIndex = 2;
		break;
	}

	if (!m_bMessage) {
		const char* szMessage = WSingleton<_systemmsg::CMessageManager>::Instance()->GetMsg((_systemmsg::eMsgIdentifier)m_notifyType);
		if (m_pSystemMsgEdit && szMessage) {
			m_pSystemMsgEdit->AddText(szMessage, false, true);
		}
	} else {
		m_pSystemMsgEdit->AddText(m_message.c_str(), false, true);
	}

	return FrForm::OnInit();
}

// secondarypassworddlg.cpp:1113
void FrPasswordNotifyDlg::SetMessage(const char* message) {
	m_bMessage = true;
	m_message = message;
}

// secondarypassworddlg.cpp:1119
void FrPasswordNotifyDlg::OnIcon_AreaInit(intptr_t param) {
	m_pIconArea = DYNAMIC_CAST(FrArea, param);
}

// secondarypassworddlg.cpp:1125
void FrPasswordNotifyDlg::OnIcon_AreaOwnerdraw(int param) {
	FrGraphicInterface* pGI = g_pFresh->GetManager()->GetGDI();

	const Bitmap* pIcon;
	switch (m_iconIndex) {
	case 0:
		pIcon = m_pIconBitmaps[0];
		break;
	case 1:
		pIcon = m_pIconBitmaps[1];
		break;
	case 2:
		pIcon = m_pIconBitmaps[2];
		break;
	default:
		return;
	}

	if (pIcon) {
		pGI->DrawTexture(pIcon, WRect(m_rect.x + 150.0f, m_rect.y, pIcon->Width(), pIcon->Height()), 0xFFFFFFFF, 0);
	}
}

// secondarypassworddlg.cpp:1147
void FrPasswordNotifyDlg::OnSystemMsg_EditInit(intptr_t param) {
	m_pSystemMsgEdit = DYNAMIC_CAST(FrEdit, param);
}

// secondarypassworddlg.cpp:1153
void FrPasswordNotifyDlg::OnClose_ButtonInit(intptr_t param) {
	m_pCloseButton = DYNAMIC_CAST(FrButton, param);
}

// secondarypassworddlg.cpp:1159
void FrPasswordNotifyDlg::OnClose_LButtonUp(void) {
	Close(FrOK, true);
}

} // namespace _gateway
