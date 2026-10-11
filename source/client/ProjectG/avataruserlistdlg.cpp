// Original: d:\build\custom\temp\code\client\projectg\avataruserlistdlg.cpp
#include "AvatarUserListDlg.h"
#include "ObjectFactory.h"
#include "frlistbox.h"
#include "mousecursor.h"

// avataruserlistdlg.cpp:13 (run-time type and factory registration of FrAvatarUserListDlg)
const WRTTI FrAvatarUserListDlg::m_RTTI("FrAvatarUserListDlg", &FrForm::m_RTTI);

IObject* FrAvatarUserListDlgMakeInstance(void) {
	return new FrAvatarUserListDlg;
}

struct __sFrAvatarUserListDlg {
	__sFrAvatarUserListDlg(void) {
		ObjectFactory().AddObjectFunctor(FrAvatarUserListDlgMakeInstance, "FrAvatarUserListDlg");
	}
};
static __sFrAvatarUserListDlg s_registerFrAvatarUserListDlg;

// avataruserlistdlg.cpp:15
const sFRESH_MSGMAP* FrAvatarUserListDlg::GetMessageMap(void) const {
	return &FrAvatarUserListDlg::_MsgMap;
}

sFRESH_MSGMAP FrAvatarUserListDlg::_MsgMap = { &FrForm::_MsgMap, &FrAvatarUserListDlg::_MsgEntries[0] };

sFRESH_ENTRY FrAvatarUserListDlg::_MsgEntries[] = {
	// avataruserlistdlg.cpp:17
	{ "userlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAvatarUserListDlg::OnUserListInit },
	// avataruserlistdlg.cpp:18
	{ "userlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAvatarUserListDlg::OnUserListOwnerDraw },
	// avataruserlistdlg.cpp:19
	{ "userlist", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrAvatarUserListDlg::OnUserListLBtnDown },
	// avataruserlistdlg.cpp:20
	{ "userlist", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrAvatarUserListDlg::OnUserListRBtnUp },
	// avataruserlistdlg.cpp:22
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// avataruserlistdlg.cpp:27
FrAvatarUserListDlg::FrAvatarUserListDlg(void) {
	m_pUserList = 0;
	m_alpha = 0;
	m_selected = -1;
}

// avataruserlistdlg.cpp:34
FrAvatarUserListDlg::~FrAvatarUserListDlg(void) {
}

// avataruserlistdlg.cpp:39
void FrAvatarUserListDlg::OnUserListInit(intptr_t param) {
	m_pUserList = (FrListBox*)param;
	m_pUserList->UseRightButton(true);

	m_pBitmaps[0] = m_pUserList->GetBitmap("i_male");
	m_pBitmaps[1] = m_pUserList->GetBitmap("i_female");
	m_pBitmaps[2] = m_pUserList->GetBitmap("i_male_02");
	m_pBitmaps[3] = m_pUserList->GetBitmap("i_female_02");
	m_pBitmaps[4] = m_pUserList->GetBitmap("i_male_03");
	m_pBitmaps[5] = m_pUserList->GetBitmap("i_female_03");
	m_pBitmaps[6] = m_pUserList->GetBitmap("i_male_manner");
	m_pBitmaps[7] = m_pUserList->GetBitmap("i_female_manner");
	m_pBitmaps[8] = m_pUserList->GetBitmap("i_male_angel");
	m_pBitmaps[9] = m_pUserList->GetBitmap("i_female_angel");
	m_pBitmaps[10] = m_pUserList->GetBitmap("ingame");
}

// avataruserlistdlg.cpp:196
void FrAvatarUserListDlg::OnUserListLBtnDown(void) {
}

// avataruserlistdlg.cpp:210
void FrAvatarUserListDlg::OnUserListRBtnUp(void) {
}

// avataruserlistdlg.cpp:263
void FrAvatarUserListDlg::OnProc(const float dt) {
	bool inArea = CMouseCursor::Instance()->InArea(m_rect);

	if (inArea) {
		m_alpha += dt * 2.0f;
		if (m_alpha > 1.0f) {
			m_alpha = 1.0f;
		}
	} else {
		m_alpha -= dt * 2.0f;
		if (m_alpha < 0.6f) {
			m_alpha = 0.6f;
		}
	}

	SetAlpha2ToChild(m_alpha);
}
