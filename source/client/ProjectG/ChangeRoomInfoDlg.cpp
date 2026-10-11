// Original: d:\build\custom\temp\code\client\projectg\changeroominfodlg.cpp
#include "ChangeRoomInfoDlg.h"
#include "LobbyMain.h"
#include "TreasureDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frviewer.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "SoundManager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "SharedDoc.h"
#include "InputManager.h"
#include "commonutil.h"

// Game type names drawn on the game type buttons, indexed by game type
// (0xab3290, no symbol; the same strings as FrMakeRoomDlg's table).
static const char* s_gameTypeName[] = {
	"\xbd\xba\xc6\xae\xb7\xce\xc5\xa9",
	"\xb8\xc5\xc4\xa1",
	"\xc3\xa4\xc6\xc3\xb9\xe6",
	"\xb7\xa1\xb4\xf5",
	"\xb4\xeb\xc8\xb8",
	"\xb4\xeb\xc8\xb8\xc6\xc0\xc0\xfc",
	"\xb1\xe6\xb5\xe5\xb4\xeb\xc0\xfc",
	"\xc6\xce\xb9\xe8\xc6\xb2",
	"",
	"",
	"\xbe\xee\xc7\xc1\xb7\xce\xc4\xa1",
	"",
	"",
	"\xbc\xc5\xc7\xc3\xc4\xda\xbd\xba"
};

namespace {

// Functor of std::for_each over a password: flags a space.
struct _invalidPwd {
	_invalidPwd(void);
	void operator()(char c);

	bool m_bInvalid;   // 0x0
};

// changeroominfodlg.cpp:31
_invalidPwd::_invalidPwd(void) : m_bInvalid(false) {}

// changeroominfodlg.cpp:32
void _invalidPwd::operator()(char c) {
	if (c == ' ')
		m_bInvalid = true;
}

} // namespace

// changeroominfodlg.cpp:41 (run-time type and factory registration of FrChangeRoomInfoDlg)
const WRTTI FrChangeRoomInfoDlg::m_RTTI("FrChangeRoomInfoDlg", &FrForm::m_RTTI);

IObject* FrChangeRoomInfoDlgMakeInstance(void) {
	return new FrChangeRoomInfoDlg;
}

struct __sFrChangeRoomInfoDlg {
	__sFrChangeRoomInfoDlg(void) {
		ObjectFactory().AddObjectFunctor(FrChangeRoomInfoDlgMakeInstance, "FrChangeRoomInfoDlg");
	}
};
static __sFrChangeRoomInfoDlg s_registerFrChangeRoomInfoDlg;

// changeroominfodlg.cpp:43
const sFRESH_MSGMAP* FrChangeRoomInfoDlg::GetMessageMap(void) const {
	return &FrChangeRoomInfoDlg::_MsgMap;
}

sFRESH_MSGMAP FrChangeRoomInfoDlg::_MsgMap = { &FrForm::_MsgMap, &FrChangeRoomInfoDlg::_MsgEntries[0] };

sFRESH_ENTRY FrChangeRoomInfoDlg::_MsgEntries[] = {
	// changeroominfodlg.cpp:46
	{ "text", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnTextInit },

	// changeroominfodlg.cpp:49
	{ "title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnTitleInit },
	// changeroominfodlg.cpp:50
	{ "title", FRCMD_ENTERKEY, FrFuncVV, (FRESH_PMSG)&FrChangeRoomInfoDlg::OnOkBtnUp },
	// changeroominfodlg.cpp:51
	{ "password", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnPasswordInit },
	// changeroominfodlg.cpp:52
	{ "password", FRCMD_ENTERKEY, FrFuncVV, (FRESH_PMSG)&FrChangeRoomInfoDlg::OnOkBtnUp },

	// changeroominfodlg.cpp:56
	{ "mapprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnMapPrevInit },
	// changeroominfodlg.cpp:57
	{ "mapprev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChangeRoomInfoDlg::OnMapPrevUp },
	// changeroominfodlg.cpp:58
	{ "mapnext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnMapNextInit },
	// changeroominfodlg.cpp:59
	{ "mapnext", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChangeRoomInfoDlg::OnMapNextUp },

	// changeroominfodlg.cpp:62
	{ "gametype", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnGameTypeInit },
	// changeroominfodlg.cpp:63
	{ "gametype", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnGameTypeOwnerDraw },
	// changeroominfodlg.cpp:64
	{ "userlimit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnUserLimitInit },
	// changeroominfodlg.cpp:65
	{ "userlimit", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnUserLimitOwnerDraw },
	// changeroominfodlg.cpp:66
	{ "hole", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnHoleInit },
	// changeroominfodlg.cpp:67
	{ "hole", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnHoleOwnerDraw },
	// changeroominfodlg.cpp:68
	{ "timelimit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnTimeLimitInit },
	// changeroominfodlg.cpp:69
	{ "timelimit", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnTimeLimitOwnerDraw },
	// changeroominfodlg.cpp:70
	{ "course", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnCourseInit },
	// changeroominfodlg.cpp:71
	{ "course", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChangeRoomInfoDlg::OnCourseUp },
	// changeroominfodlg.cpp:72
	{ "course", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnCourseOwnerDraw },
	// changeroominfodlg.cpp:73
	{ "holetype", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnHoleTypeInit },
	// changeroominfodlg.cpp:74
	{ "holetype", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChangeRoomInfoDlg::OnHoleTypeOwnerDraw },

	// changeroominfodlg.cpp:76
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChangeRoomInfoDlg::OnOkBtnUp },

	// changeroominfodlg.cpp:78
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// changeroominfodlg.cpp:149
FrChangeRoomInfoDlg::~FrChangeRoomInfoDlg(void) {
}

// changeroominfodlg.cpp:155
bool FrChangeRoomInfoDlg::OnInit(void) {
	ChangeGameType(m_roomInfo.m_gameType, false, true);
	ChangeCourse(m_roomInfo.m_course);

	if (m_gameType == 10 || m_gameType == 6) {
		if (m_pMapPrev)
			m_pMapPrev->Enable(false);
		if (m_pMapNext)
			m_pMapNext->Enable(false);
	}

	return true;
}

// changeroominfodlg.cpp:169
void FrChangeRoomInfoDlg::OnTextInit(intptr_t param) {
	m_pText = DYNAMIC_CAST(FrArea, param);
}

// changeroominfodlg.cpp:176
void FrChangeRoomInfoDlg::OnTitleInit(intptr_t param) {
	m_pTitle = DYNAMIC_CAST(FrEdit, param);

	if (m_pTitle) {
		m_pTitle->SetCharLimit(20, false);
		m_pTitle->SetLine(1, m_roomInfo.m_title, 0, false, 0);
	}
}

// changeroominfodlg.cpp:187
void FrChangeRoomInfoDlg::OnPasswordInit(intptr_t param) {
	m_pPassword = DYNAMIC_CAST(FrEdit, param);

	if (m_pPassword) {
		m_pPassword->SetCharLimit(7, false);
		m_pPassword->SetLine(1, m_roomInfo.m_password, 0, false, 0);
	}
}

// changeroominfodlg.cpp:206
void FrChangeRoomInfoDlg::OnMapPrevInit(intptr_t param) {
	m_pMapPrev = DYNAMIC_CAST(FrButton, param);

	if (m_pMapPrev) {
		m_pMapPrev->SetPushDelay(0.0f);
		m_pMapPrev->SetVisible(true);
	}
}

// changeroominfodlg.cpp:217
void FrChangeRoomInfoDlg::OnMapPrevUp(void) {
	if (m_pMapPrev) {
		unsigned char course = PrevMap((unsigned char)m_course);
		ChangeCourse(course);
	}
}

// changeroominfodlg.cpp:239
void FrChangeRoomInfoDlg::OnMapNextInit(intptr_t param) {
	m_pMapNext = DYNAMIC_CAST(FrButton, param);

	if (m_pMapNext) {
		m_pMapNext->SetPushDelay(0.0f);
		m_pMapNext->SetVisible(true);
	}
}

// changeroominfodlg.cpp:250
void FrChangeRoomInfoDlg::OnMapNextUp(void) {
	if (m_pMapNext) {
		unsigned char course = NextMap((unsigned char)m_course, m_gameType == 2);
		ChangeCourse(course);
	}
}

// changeroominfodlg.cpp:272
void FrChangeRoomInfoDlg::OnGameTypeInit(intptr_t param) {
	m_pGameType = DYNAMIC_CAST(FrArea, param);
}

// changeroominfodlg.cpp:280
void FrChangeRoomInfoDlg::DrawGameTypeButton(FrGraphicInterface* pDevice, int gameType, WRect rect, bool b) {
	const Bitmap* pImg = m_pSelectButtonImg[0];
	unsigned long color = 0xFFFFFFFF;

	if (b) {
		if (m_gameType == gameType) {
			pImg = m_pSelectButtonImg[2];
		} else {
			const WPoint& mousePos = g_pFresh->GetManager()->GetMousePos();
			if (m_pMapSelectDlg == 0 && m_btnDelay < 0.0f && rect.IsInRect(mousePos)) {
				if (g_input->GetButton(LEFT_BUTTON) == 2)
					ChangeGameType(gameType, false, false);

				pImg = m_pSelectButtonImg[1];
			}
		}
	} else {
		color = 0xFF909090;
	}

	pDevice->DrawTexture(pImg, rect, color, 0);

	pDevice->PrintText11(WPoint(rect.x + rect.w * 0.5f, rect.y + 5.0f), 1, s_gameTypeName[gameType], 0);
}

// changeroominfodlg.cpp:381
void FrChangeRoomInfoDlg::OnUserLimitInit(intptr_t param) {
	m_pUserLimit = DYNAMIC_CAST(FrArea, param);
}

// changeroominfodlg.cpp:389
void FrChangeRoomInfoDlg::DrawUserLimitButton(FrGraphicInterface* pDevice, int userLimit, WRect rect) {
	const Bitmap* pImg = m_pSelectButtonImg[0];

	if (m_userLimit == userLimit) {
		pImg = m_pSelectButtonImg[2];
	} else {
		const WPoint& mousePos = g_pFresh->GetManager()->GetMousePos();
		if (m_pMapSelectDlg == 0 && m_btnDelay < 0.0f && rect.IsInRect(mousePos)) {
			if (g_input->GetButton(LEFT_BUTTON) == 2)
				m_userLimit = userLimit;

			pImg = m_pSelectButtonImg[1];
		}
	}

	pDevice->DrawTexture(pImg, rect, 0xFFFFFFFF, 0);

	pDevice->PrintText11(WPoint(rect.x + rect.w * 0.5f, rect.y + 5.0f), 1, MakeStr("%d", userLimit), 0);
}

// changeroominfodlg.cpp:497
void FrChangeRoomInfoDlg::OnHoleInit(intptr_t param) {
	m_hole = m_roomInfo.m_holeCount;
	m_pHole = DYNAMIC_CAST(FrArea, param);
}

// changeroominfodlg.cpp:506
void FrChangeRoomInfoDlg::DrawHoleButton(FrGraphicInterface* pDevice, int hole, WRect rect) {
	const Bitmap* pImg = m_pSelectButtonImg[0];

	if (m_hole == hole) {
		pImg = m_pSelectButtonImg[2];
	} else {
		const WPoint& mousePos = g_pFresh->GetManager()->GetMousePos();
		if (m_pMapSelectDlg == 0 && m_btnDelay < 0.0f && rect.IsInRect(mousePos)) {
			if (g_input->GetButton(LEFT_BUTTON) == 2)
				ChangeHole(hole);

			pImg = m_pSelectButtonImg[1];
		}
	}

	pDevice->DrawTexture(pImg, rect, 0xFFFFFFFF, 0);

	pDevice->PrintText11(WPoint(rect.x + rect.w * 0.5f, rect.y + 5.0f), 1, MakeStr("%d", hole), 0);
}

// changeroominfodlg.cpp:532
void FrChangeRoomInfoDlg::OnHoleOwnerDraw(intptr_t param) {
	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	if (pDevice == 0)
		return;

	WRect rect = m_pHole->GetRect();

	switch (m_gameType) {
	case 0: {
		WRect rc1(rect.x, rect.y, rect.w * 0.25f, rect.h);
		WRect rc2(rect.x + rect.w * 0.25f, rect.y, rect.w * 0.25f, rect.h);
		WRect rc3(rect.x + rect.w * 0.5f, rect.y, rect.w * 0.25f, rect.h);
		WRect rc4(rect.x + rect.w * 0.75f, rect.y, rect.w * 0.25f, rect.h);
		DrawHoleButton(pDevice, 3, rc1);
		DrawHoleButton(pDevice, 6, rc2);
		DrawHoleButton(pDevice, 9, rc3);
		DrawHoleButton(pDevice, 18, rc4);
		break;
	}
	case 1:
	case 7: {
		WRect rc1(rect.x, rect.y, rect.w * 0.33333334f, rect.h);
		WRect rc2(rect.x + rect.w * 0.33333334f, rect.y, rect.w * 0.33333334f, rect.h);
		WRect rc3(rect.x + rect.w * 0.6666667f, rect.y, rect.w * 0.33333334f, rect.h);
		DrawHoleButton(pDevice, 6, rc1);
		DrawHoleButton(pDevice, 9, rc2);
		DrawHoleButton(pDevice, 18, rc3);
		break;
	}
	case 4:
	case 5:
	case 6: {
		WRect rc1(rect.x, rect.y, rect.w * 0.33333334f, rect.h);
		WRect rc2(rect.x + rect.w * 0.33333334f, rect.y, rect.w * 0.33333334f, rect.h);
		WRect rc3(rect.x + rect.w * 0.6666667f, rect.y, rect.w * 0.33333334f, rect.h);
		DrawHoleButton(pDevice, 3, rc1);
		DrawHoleButton(pDevice, 6, rc2);
		DrawHoleButton(pDevice, 9, rc3);
		break;
	}
	case 9:
	case 10: {
		WRect rc1(rect.x, rect.y, rect.w * 0.5f, rect.h);
		WRect rc2(rect.x + rect.w * 0.5f, rect.y, rect.w * 0.5f, rect.h);
		DrawHoleButton(pDevice, 9, rc1);
		DrawHoleButton(pDevice, 18, rc2);
		break;
	}
	}
}

// changeroominfodlg.cpp:588
void FrChangeRoomInfoDlg::OnTimeLimitInit(intptr_t param) {
	m_pTimeLimit = DYNAMIC_CAST(FrArea, param);
}

// changeroominfodlg.cpp:596
void FrChangeRoomInfoDlg::DrawTimeLimitButton(FrGraphicInterface* pDevice, int timeLimit, WRect rect) {
	const Bitmap* pImg = m_pSelectButtonImg[0];

	if (m_timeLimit == timeLimit) {
		pImg = m_pSelectButtonImg[2];
	} else {
		const WPoint& mousePos = g_pFresh->GetManager()->GetMousePos();
		if (m_pMapSelectDlg == 0 && m_btnDelay < 0.0f && rect.IsInRect(mousePos)) {
			if (g_input->GetButton(LEFT_BUTTON) == 2)
				m_timeLimit = timeLimit;

			pImg = m_pSelectButtonImg[1];
		}
	}

	pDevice->DrawTexture(pImg, rect, 0xFFFFFFFF, 0);

	pDevice->PrintText11(WPoint(rect.x + rect.w * 0.5f, rect.y + 5.0f), 1, MakeStr("%d", timeLimit), 0);
}

// changeroominfodlg.cpp:728
void FrChangeRoomInfoDlg::OnCourseInit(intptr_t param) {
	m_pCourse = DYNAMIC_CAST(FrViewer, param);
}

// changeroominfodlg.cpp:733
bool FrChangeRoomInfoDlg::OnMapSelectDlgResult(int result, FrForm* pForm) {
	unsigned char course = 0;
	if (pForm) {

		course = static_cast<FrTreasureCourse*>(pForm)->GetCurMapIdx();
	}

	ChangeCourse(course);
	m_pMapSelectDlg = 0;
	m_btnDelay = 0.7f;
	return true;
}

// changeroominfodlg.cpp:749
void FrChangeRoomInfoDlg::OnCourseUp(void) {
	if (m_gameType == 10 || m_gameType == 6)
		return;

	g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);

	m_pMapSelectDlg = CreateForm<FrTreasureCourse>(g_pFresh->GetManager(), this, "treasuremap", 0);
	if (m_pMapSelectDlg) {
		m_pMapSelectDlg->SetSelectMap((unsigned char)m_course);
		m_pMapSelectDlg->SetMapIndexChanged(false);
		m_pMapSelectDlg->EnableDrag(false);
		m_pMapSelectDlg->Open((FRESH_RESULT_CALLBACK)&FrChangeRoomInfoDlg::OnMapSelectDlgResult, 3);
	}
}

// changeroominfodlg.cpp:770
void FrChangeRoomInfoDlg::OnCourseOwnerDraw(int param) {
	if (m_gameType == 10 || m_gameType == 6)
		return;

	if (m_pCourse == 0)
		return;
	if (m_pMapSelectImg == 0)
		return;

	WRect rect = m_pCourse->GetRect();
	FrWndManager* pManager = g_pFresh->GetManager();
	if (rect.IsInRect(pManager->GetMousePos())) {
		FrGraphicInterface* pDevice = pManager->GetGDI();
		if (pDevice)
			pDevice->DrawTexture(m_pMapSelectImg, WRect(rect.x - 25.0f, rect.y - 15.0f, m_pMapSelectImg->Width(), m_pMapSelectImg->Height()), 0xFFFFFFFF, 0);
	}
}

// changeroominfodlg.cpp:790
void FrChangeRoomInfoDlg::OnHoleTypeInit(intptr_t param) {
	m_pHoleType = DYNAMIC_CAST(FrArea, param);
}

// changeroominfodlg.cpp:798
void FrChangeRoomInfoDlg::DrawHoleTypeButton(FrGraphicInterface* pDevice, eHoleType holeType, WRect rect, bool b) {
	const Bitmap* pImg = m_pHoleTypeImg[holeType * 2];
	unsigned long color = 0xFFFFFFFF;

	if (b) {
		if (m_holeType == holeType) {
			pImg = m_pHoleTypeImg[holeType * 2 + 1];
		} else {
			const WPoint& mousePos = g_pFresh->GetManager()->GetMousePos();
			if (m_pMapSelectDlg == 0 && m_btnDelay < 0.0f && rect.IsInRect(mousePos)) {
				if (g_input->GetButton(LEFT_BUTTON) == 2)
					m_holeType = holeType;
			}
		}
	} else {
		color = 0xFF909090;
	}

	pDevice->DrawTexture(pImg, rect, color, 0);
}

// changeroominfodlg.cpp:1200
void FrChangeRoomInfoDlg::ChangeUserLimit(int userLimit) {
	m_userLimit = userLimit;
}

// changeroominfodlg.cpp:1205
void FrChangeRoomInfoDlg::ChangeHole(int hole) {
	m_hole = hole;
	switch (m_gameType) {
	case 0:
	case 1:
	case 7:
		if (hole == 18) {
			if (m_holeType == 1 || m_holeType == 2) {
				m_holeType = 0;
			}
		}
		break;
	case 4:
	case 5:
		if (hole == 9) {
			m_timeLimit = 15;
		} else if (hole == 18) {
			m_timeLimit = 30;
			if (m_holeType == 1 || m_holeType == 2) {
				m_holeType = 0;
			}
		}
		break;
	case 6:
		if (hole == 9) {
			m_timeLimit = 20;
		} else if (hole == 18) {
			m_timeLimit = 30;
			m_holeType = 0;
		}
		break;
	}
}

// changeroominfodlg.cpp:1252
void FrChangeRoomInfoDlg::ChangeTimeLimit(int timeLimit) {
	m_timeLimit = timeLimit;
}

// changeroominfodlg.cpp:1257
void FrChangeRoomInfoDlg::ChangeCourse(int course) {
	IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(0x28000000 | ((course > 0x7F && course != 0xFD) ? course - 0x80 : course));
	if (pCourse == 0)
		return;

	if (course != 0x7F) {
		m_pCourse->Open(MakeStr("small_%s.tga", pCourse->m_icon));
	} else {
		m_pCourse->Open("map_random_middle.tga");
	}

	m_course = course;

	if (m_pMapPrev)
		m_pMapPrev->Enable(m_course != FirstMap());

	if (m_pMapNext)
		m_pMapNext->Enable(m_course != LastMap(false));
}

// changeroominfodlg.cpp:1282
void FrChangeRoomInfoDlg::ChangeHoleType(eHoleType holeType) {
	m_holeType = holeType;
}

// changeroominfodlg.cpp:1289
void FrChangeRoomInfoDlg::OnProc(const float dt) {
	FrForm::OnProc(dt);

	if (m_pMapSelectDlg == 0 && m_btnDelay >= 0.0f) {
		m_btnDelay -= dt;
	}
}

// changeroominfodlg.cpp:1299
bool FrChangeRoomInfoDlg::IsBtnEnable(void) {
	if (m_pMapSelectDlg == 0 && m_btnDelay < 0.0f)
		return true;

	return false;
}
