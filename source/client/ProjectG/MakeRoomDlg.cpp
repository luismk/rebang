// Original: d:\build\custom\temp\code\client\projectg\makeroomdlg.cpp
#include "MakeRoomDlg.h"
#include "TreasureDlg.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "fredit.h"
#include "frarea.h"
#include "frbutton.h"
#include "fresh.h"
#include "frtabbutton.h"
#include "frviewer.h"
#include "frwndmanager.h"
#include "frgaugebar.h"
#include "frgraphicinterface.h"
#include "LobbyMain.h"
#include "Treasure.h"
#include "commonutil.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "TaskManager.h"
#include "bitmap.h"
#include "Packet.h"
#include "../shared/Localize.h"

#include <algorithm>
#include <stdlib.h>
#include <string.h>

namespace nsInvalidCheck {

// Functor of std::for_each over a password: flags a space.
struct _invalidPwd {
	_invalidPwd(void);
	void operator()(char c);

	bool m_bInvalid;   // 0x0
};

// makeroomdlg.cpp:48
_invalidPwd::_invalidPwd(void) : m_bInvalid(false) {}

// makeroomdlg.cpp:49
void _invalidPwd::operator()(char c) {
	if (c == ' ')
		m_bInvalid = true;
}

} // namespace nsInvalidCheck

// Game type names drawn on the game type tabs, indexed by game type
// (0xab5990, no symbol).
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

// makeroomdlg.cpp:60 (run-time type and factory registration of FrMakeRoomDlg)
const WRTTI FrMakeRoomDlg::m_RTTI("FrMakeRoomDlg", &FrForm::m_RTTI);

IObject* FrMakeRoomDlgMakeInstance(void) {
	return new FrMakeRoomDlg;
}

struct __sFrMakeRoomDlg {
	__sFrMakeRoomDlg(void) {
		ObjectFactory().AddObjectFunctor(FrMakeRoomDlgMakeInstance, "FrMakeRoomDlg");
	}
};
static __sFrMakeRoomDlg s_registerFrMakeRoomDlg;

// makeroomdlg.cpp:62
const sFRESH_MSGMAP* FrMakeRoomDlg::GetMessageMap(void) const {
	return &FrMakeRoomDlg::_MsgMap;
}

sFRESH_MSGMAP FrMakeRoomDlg::_MsgMap = { &FrForm::_MsgMap, &FrMakeRoomDlg::_MsgEntries[0] };

sFRESH_ENTRY FrMakeRoomDlg::_MsgEntries[] = {
	// makeroomdlg.cpp:64
	{ "make_vs", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnMakeVsInit },
	{ "make_vs", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnMakeVsDown },
	{ "make_mass", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnMakeMassInit },
	{ "make_mass", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnMakeMassDown },
	{ "make_battle", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnMakeBattleInit },
	{ "make_battle", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnMakeBattleDown },
	{ "make_chat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnMakeChatInit },
	{ "make_chat", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnMakeChatDown },
	{ "make_chaos", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnMakeChaosInit },
	{ "make_chaos", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnMakeChaosDown },
	{ "picture", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnPictureInit },
	// makeroomdlg.cpp:79
	{ "text", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnTextInit },
	{ "title", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnTitleInit },
	// makeroomdlg.cpp:85
	{ "title", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrMakeRoomDlg::OnTitleEnterKey },
	{ "password", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnPasswordInit },
	{ "mapprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnMapPrevInit },
	{ "mapprev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnMapPrevUp },
	{ "mapnext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnMapNextInit },
	{ "mapnext", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnMapNextUp },
	{ "mapprev_chat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnMapPrevChatInit },
	{ "mapprev_chat", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnMapPrevUp },
	// makeroomdlg.cpp:95
	{ "mapnext_chat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnMapNextChatInit },
	{ "mapnext_chat", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnMapNextUp },
	{ "gametype", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnGameTypeInit },
	{ "gametype", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnGameTypeButtonUp },
	{ "gametype", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnGameTypeOwnerDraw },
	{ "userlimit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnUserLimitInit },
	{ "userlimit", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnUserLimitButtonUp },
	{ "userlimit", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnUserLimitOwnerDraw },
	{ "hole", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnHoleInit },
	{ "hole", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnHoleButtonUp },
	{ "hole", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnHoleOwnerDraw },
	{ "timelimit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnTimeLimitInit },
	{ "timelimit", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnTimeLimitButtonUp },
	{ "timelimit", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnTimeLimitOwnerDraw },
	{ "course", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnCourseInit },
	{ "course", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnCourseUp },
	{ "course", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnCourseOwnerDraw },
	// makeroomdlg.cpp:113
	{ "holetype", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnHoleTypeInit },
	{ "holetype", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnHoleTypeOwnerDraw },
	{ "course_chat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnCourseChatInit },
	{ "course_chat", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnCourseChatUp },
	{ "hole_min", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnHoleMinInit },
	{ "hole_min", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnHoleMinUp },
	// makeroomdlg.cpp:120
	{ "hole_prev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnHolePrevInit },
	{ "hole_prev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnHolePrevUp },
	{ "hole_chat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnHoleChatInit },
	{ "hole_chat", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnHoleChatOwnerDraw },
	{ "hole_next", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnHoleNextInit },
	{ "hole_next", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnHoleNextUp },
	{ "hole_max", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnHoleMaxInit },
	{ "hole_max", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMakeRoomDlg::OnHoleMaxUp },
	// makeroomdlg.cpp:127
	{ "overgauge", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMakeRoomDlg::OnOverGauge_Init },

	// makeroomdlg.cpp:129
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// makeroomdlg.cpp:132
FrMakeRoomDlg::FrMakeRoomDlg(void) {
	memset(m_pMakeBtn, 0, sizeof(m_pMakeBtn));

	if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x80) {
		m_gameType = 3;
	} else {
		m_gameType = (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x4000) ? 7 : 0;
	}

	m_pMapPrev = 0;
	m_pMapNext = 0;
	m_pMapPrevChat = 0;
	m_pMapNextChat = 0;

	m_pMapSelectDlg = 0;

	m_btnDelay = 0.7f;
}

// makeroomdlg.cpp:155
FrMakeRoomDlg::~FrMakeRoomDlg(void) {
}

// makeroomdlg.cpp:160
bool FrMakeRoomDlg::OnInit(void) {
	m_holeType = 0;
	m_startHole = 1;
	m_gameTime = 0;
	m_turnTime = 0;
	m_courseX = m_pCourse->GetRect().x;

	m_pMapSelectImg = g_pFresh->GetManager()->GetBitmap("MAKEROOM", "map_select_o");

	ChangeGameType(m_gameType, true);

	if (m_pMakeBtn[m_makeBtnIndex])
		m_pMakeBtn[m_makeBtnIndex]->SetStatus(FrButton::PRESSED);

	ChangeCourse(FirstMap());

	return true;
}

// makeroomdlg.cpp:222
void FrMakeRoomDlg::OnMakeVsInit(int param) {
	m_pMakeBtn[0] = DYNAMIC_CAST(FrButton, param);
}

// makeroomdlg.cpp:230
void FrMakeRoomDlg::OnMakeVsDown(void) {
	ChangeGameType(0, true);
}

// makeroomdlg.cpp:235
void FrMakeRoomDlg::OnMakeMassInit(int param) {
	m_pMakeBtn[1] = DYNAMIC_CAST(FrButton, param);
}

// makeroomdlg.cpp:243
void FrMakeRoomDlg::OnMakeMassDown(void) {
	ChangeGameType(4, true);
}

// makeroomdlg.cpp:248
void FrMakeRoomDlg::OnMakeBattleInit(int param) {
	m_pMakeBtn[2] = DYNAMIC_CAST(FrButton, param);
}

// makeroomdlg.cpp:256
void FrMakeRoomDlg::OnMakeBattleDown(void) {
	ChangeGameType(10, true);
}

// makeroomdlg.cpp:283
void FrMakeRoomDlg::OnMakeChatInit(int param) {
	m_pMakeBtn[3] = DYNAMIC_CAST(FrButton, param);
}

// makeroomdlg.cpp:291
void FrMakeRoomDlg::OnMakeChatDown(void) {
	ChangeGameType(2, true);
}

// makeroomdlg.cpp:297
void FrMakeRoomDlg::OnMakeChaosInit(int param) {
	m_pMakeBtn[4] = DYNAMIC_CAST(FrButton, param);
	if (m_pMakeBtn[4]) {
		bool found = false;
		std::list<sItemInfo> &itemList = CSharedDoc::Instance()->m_itemInfoList;

		for (std::list<sItemInfo>::iterator it = itemList.begin(); it != itemList.end(); ++it) {
			if (it->m_typeId == 0x1A0000F7) {
				found = true;
				break;
			}
		}

		if (!found) {
			m_pMakeBtn[4]->SetVisible(false);
		}

		if (CSharedDoc::Instance()->m_serverChannel.flags & 0x40) {
			m_pMakeBtn[4]->SetVisible(false);
		}

		m_pMakeBtn[4]->EnableBlink(true);
	}
}

// makeroomdlg.cpp:332
void FrMakeRoomDlg::OnMakeChaosDown(void) {
	AfxGetTask()->SendGlobalMsg(MsgObject(0, 0x23, (intptr_t)MakeStr("\xbd\xba\xc6\xf3\xbc\xc8 \xbc\xc5\xc7\xc3 \xb8\xf0\xb5\xe5 \xb9\xe6\xc0\xbb \xb8\xb8\xb5\xe9 \xb6\xa7, \n\\c0xffff0000\\c%s\\c0xff000000\\c\xc0\xcc \xbc\xd2\xb8\xf0 \xb5\xcb\xb4\xcf\xb4\xd9.", ItemManager()->GetItemName(0x1A0000F7)), 0, 0, 0, 0));

	ChangeGameType(14, true);
}

// makeroomdlg.cpp:354
void FrMakeRoomDlg::OnPictureInit(int param) {
	m_pPicture = DYNAMIC_CAST(FrViewer, param);

	if (m_pPicture) {
		m_pPicture->SetMouseEvent(false);
	}
}

// makeroomdlg.cpp:363
void FrMakeRoomDlg::OnTextInit(intptr_t param) {
	m_pText = DYNAMIC_CAST(FrArea, param);
}

// makeroomdlg.cpp:370
void FrMakeRoomDlg::OnTitleInit(intptr_t param) {
	m_pTitleEdit = DYNAMIC_CAST(FrEdit, param);

	if (m_pTitleEdit) {
		m_pTitleEdit->SetCharLimit(20, false);
		
		m_pTitleEdit->SetLine(1, WSingleton<CSharedDoc>::Instance()->GetDefaultTitle(m_gameType), 0, false, 0);
	}
}

// makeroomdlg.cpp:381
bool FrMakeRoomDlg::OnTitleEnterKey(int param) {
	return false;
}

// makeroomdlg.cpp:387
void FrMakeRoomDlg::OnPasswordInit(intptr_t param) {
	FrEdit* pPassword = DYNAMIC_CAST(FrEdit, param);
}

// makeroomdlg.cpp:413
void FrMakeRoomDlg::OnMapPrevInit(intptr_t param) {
	m_pMapPrev = DYNAMIC_CAST(FrButton, param);

	if (m_pMapPrev) {
		m_pMapPrev->SetPushDelay(0.0f);
	}
}

// makeroomdlg.cpp:421
void FrMakeRoomDlg::OnMapPrevUp(void) {
	if (m_pMapPrev && m_pMapPrevChat) {
		unsigned char course = PrevMap(m_course);

		ChangeCourse(course);
	}
}

// makeroomdlg.cpp:442
void FrMakeRoomDlg::OnMapNextInit(intptr_t param) {
	m_pMapNext = DYNAMIC_CAST(FrButton, param);

	if (m_pMapNext) {
		m_pMapNext->SetPushDelay(0.0f);
	}
}

// makeroomdlg.cpp:450
void FrMakeRoomDlg::OnMapNextUp(void) {
	if (m_pMapNext && m_pMapNextChat) {
		unsigned char course = NextMap(m_course, m_gameType == 2);

		ChangeCourse(course);
	}
}

// makeroomdlg.cpp:471
void FrMakeRoomDlg::OnMapPrevChatInit(int param) {
	m_pMapPrevChat = DYNAMIC_CAST(FrButton, param);

	if (m_pMapPrevChat) {
		m_pMapPrevChat->SetPushDelay(0.0f);
		m_pMapPrevChat->SetVisible(false);
	}
}

// makeroomdlg.cpp:482
void FrMakeRoomDlg::OnMapNextChatInit(int param) {
	m_pMapNextChat = DYNAMIC_CAST(FrButton, param);

	if (m_pMapNextChat) {
		m_pMapNextChat->SetPushDelay(0.0f);
		m_pMapNextChat->SetVisible(false);
	}
}

// makeroomdlg.cpp:493
bool FrMakeRoomDlg::MakeRoom(bool b) {
	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(0x40) && m_gameType == 0) {
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x272, 0x40, 0, 0, 0, 0));
		return false;
	}

	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(0x80) && m_gameType == 1) {
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x272, 0x80, 0, 0, 0, 0));
		return false;
	}

	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(0x100) && m_gameType == 4) {
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x272, 0x100, 0, 0, 0, 0));
		return false;
	}

	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(0x2000000) && m_gameType == 14) {
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x272, 0x2000000, 0, 0, 0, 0));
		return false;
	}

	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(0x200) && m_gameType == 5) {
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x272, 0x200, 0, 0, 0, 0));
		return false;
	}

	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(0x400) && m_gameType == 6) {
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x272, 0x400, 0, 0, 0, 0));
		return false;
	}

	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(0x800) && m_gameType == 7) {
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x272, 0x800, 0, 0, 0, 0));
		return false;
	}

	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(0x1000) && m_gameType == 10) {
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0x272, 0x1000, 0, 0, 0, 0);
		return false;
	}

	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(0x2000) && m_gameType == 2) {
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0x272, 0x2000, 0, 0, 0, 0);
		return false;
	}

	std::string title;

	FrWnd* pWnd = FindChildByName("title");
	FrEdit* pTitleEdit = DYNAMIC_CAST(FrEdit, pWnd);
	if (pTitleEdit)
		title = pTitleEdit->GetLine(1, false);

	if (m_gameType == 6) {
		if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guildUID == 0) {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
			// "Only guild members can make a room."
			pForm->SetMessage("\xb1\xe6\xb5\xe5 \xb0\xa1\xc0\xd4\xc0\xda\xb8\xb8 \xb9\xe6\xc0\xbb \xb8\xb8\xb5\xe5\xbd\xc7 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", false);
			pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
			return false;
		}
	}

	if (m_gameType == 2) {
		if (m_course == 0x7F)
			m_course = rand() % 11;
	}

	if (title.empty() || title.find_first_not_of(' ', 0) == std::string::npos) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		// "Please enter a room title"
		pForm->SetMessage("\xb9\xe6 \xc1\xa6\xb8\xf1\xc0\xbb \xb3\xd6\xbe\xee\xc1\xd6\xbc\xbc\xbf\xe4", false);
		pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
		return false;
	}

	if (WSingleton<CSharedDoc>::Instance()->m_chatManager.Filtering((char*)title.c_str())) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		// "The room title contains characters that cannot be used."
		pForm->SetMessage("\xb9\xe6 \xc1\xa6\xb8\xf1\xbf\xa1\xbc\xad \xbb\xe7\xbf\xeb\xc7\xd2 \xbc\xf6 \xbe\xf8\xb4\xc2 \xb9\xae\xc0\xda\xb8\xa6 \xc6\xf7\xc7\xd4\xc7\xcf\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", false);
		pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
		return false;
	}

	pWnd = FindChildByName("password");
	FrEdit* pPasswordEdit = DYNAMIC_CAST(FrEdit, pWnd);

	std::string password;
	if (pPasswordEdit) {
		const char* line = pPasswordEdit->GetLine(1, false);
		password = line;
		if (!b)
			AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0x27, (intptr_t)line, 0, 0, 0, 0);
		else
			AfxGetTask()->GetActor("AvatarChat") << MsgObject(0, 0x27, (intptr_t)line, 0, 0, 0, 0);
	}

	if (IsMassGame(m_gameType)) {
		m_turnTime = 0;
		if (m_gameType == 10)
			m_gameTime = m_timeLimit * 1000;
		else
			m_gameTime = m_timeLimit * 60000;
	} else {
		m_turnTime = m_timeLimit * 1000;
		m_gameTime = 0;
	}

	if (m_gameType == 2)
		m_hole = (unsigned char)m_startHole;

	if (IsLocalContent((localContentType_t)0x3E)) {
		if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800) {
			if (m_gameType == 7) {
				FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
				// "Pang Battle games cannot be played on the rookie channel."
				pForm->SetMessage("\xb7\xe7\xc5\xb0 \xc3\xa4\xb3\xce\xbf\xa1\xbc\xad\xb4\xc2 \xc6\xce\xb9\xe8\xc6\xb2 \xb0\xd4\xc0\xd3\xc0\xbb \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", false);
				pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
				return false;
			}
		}
	}

	nsInvalidCheck::_invalidPwd result = std::for_each(password.begin(), password.end(), nsInvalidCheck::_invalidPwd());
	if (result.m_bInvalid) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		// "Spaces cannot be used in the password."
		pForm->SetMessage("\xba\xf1\xb9\xd0\xb9\xf8\xc8\xa3\xbf\xa1 \xb0\xf8\xb9\xe9\xc0\xba \xbb\xe7\xbf\xeb\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", false);
		pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
		return false;
	}

	WSendPacket packet((enumClientPacket)8);
	packet.Encode1(b ? 1 : 0);
	packet.Encode4(m_turnTime);
	packet.Encode4(m_gameTime);
	packet.Encode1(m_userLimit);
	packet.Encode1(m_gameType);
	packet.Encode1(m_hole);
	packet.Encode1(m_course);
	packet.Encode1(m_holeType);
	packet.EncodeStr(title);
	packet.EncodeStr(password);
	packet.Send(SEND_TO_DEFAULT);

	if (!b) {
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0x16, 1, 0, 0, 0, 0);
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 1, 0, 0, 0, 0, 0);
	} else {
		AfxGetTask()->GetActor("AvatarChat") << MsgObject(0, 0x16, 1, 0, 0, 0, 0);
		AfxGetTask()->GetActor("AvatarChat") << MsgObject(0, 1, 0, 0, 0, 0, 0);
	}

	return true;
}

// makeroomdlg.cpp:825
void FrMakeRoomDlg::SetDefaultTitle(void) {
	m_pTitleEdit->SetLine(1, WSingleton<CSharedDoc>::Instance()->GetDefaultTitle(m_gameType), 0, false, 0);
}

// makeroomdlg.cpp:830
void FrMakeRoomDlg::ChangeGameType(unsigned char gameType, bool b) {
	if (m_gameType == 2 && gameType != 2) {
		m_pMapPrev->SetVisible(true);
		m_pMapNext->SetVisible(true);
		m_pMapPrevChat->SetVisible(false);
		m_pMapNextChat->SetVisible(false);
		m_pCourseChat->SetVisible(false);
		m_pHoleChat->SetVisible(false);
		m_pHoleMin->SetVisible(false);
		m_pHolePrev->SetVisible(false);
		m_pHoleNext->SetVisible(false);
		m_pHoleMax->SetVisible(false);
		m_pHoleType->SetVisible(true);
		m_pCourse->SetVisible(true);

		m_pOverGauge->SetVisible(true);
	}

	if (m_gameType != 2 && gameType == 2) {
		m_pMapPrev->SetVisible(false);
		m_pMapNext->SetVisible(false);
		m_pMapPrevChat->SetVisible(true);
		m_pMapNextChat->SetVisible(true);
		m_pCourseChat->SetVisible(true);
		m_pHoleChat->SetVisible(true);
		m_pHoleMin->SetVisible(true);
		m_pHolePrev->SetVisible(true);
		m_pHoleNext->SetVisible(true);
		m_pHoleMax->SetVisible(true);
		m_pHoleType->SetVisible(false);
		m_pCourse->SetVisible(false);

		m_pOverGauge->SetVisible(false);

		if (m_course == 0x7F)
			ChangeCourse(FirstMap());
	} else if (m_gameType == 14 && gameType != 14) {
		m_pCourse->SetMouseEvent(true);
		m_pMapPrev->SetVisible(true);
		m_pMapNext->SetVisible(true);
		m_pHoleType->SetVisible(true);
		m_pCourse->SetVisible(true);
		m_pOverGauge->SetVisible(true);

		if (m_course == 0x7F)
			ChangeCourse(FirstMap());
	}

	if (m_gameType == 10 && gameType != 10) {
		m_pCourse->SetMouseEvent(true);
		ChangeCourse(FirstMap());
	} else if (m_gameType == 6 && gameType != 6) {
		m_pCourse->SetMouseEvent(true);
		ChangeCourse(FirstMap());
	} else if ((m_gameType != 6 && gameType == 6) || (m_gameType != 10 && gameType == 10)) {
		m_pCourse->SetMouseEvent(false);
		ChangeCourse(0x7F);

		m_pMapPrev->Enable(false);
		m_pMapPrevChat->Enable(false);

		m_pMapNext->Enable(false);
		m_pMapNextChat->Enable(false);
	}

	if (m_gameType != 14 && gameType == 14) {
		m_pCourse->SetMouseEvent(false);

		m_pMapPrev->SetVisible(false);
		m_pMapPrevChat->SetVisible(false);
		m_pMapNext->SetVisible(false);
		m_pMapNextChat->SetVisible(false);

		m_pHoleType->SetVisible(false);
		m_pCourse->SetVisible(false);
		m_pOverGauge->SetVisible(false);

		ChangeCourse(0x7F);
	}

	m_gameType = gameType;

	for (int i = 0; i < 5; i++)
		m_pMakeBtn[i]->SetStatus(FrButton::NORMAL);

	if (IsLocalContent((localContentType_t)0x3E)) {
		if ((WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800) && WSingleton<CSharedDoc>::Instance()->m_myLevel == 0)
			m_pMakeBtn[2]->Enable(false);
	}

	m_pUserLimitTab->ClearTab();
	m_pHoleTab->ClearTab();
	m_pTimeLimitTab->ClearTab();

	switch (m_gameType) {
	case 0:
		m_pPicture->Open("picture_stroke.tga");
		m_pText->SetBgImg("text_vs");

		m_makeBtnIndex = 0;
		m_userLimit = 2;
		m_hole = 3;
		m_turnTime = 40000;
		m_timeLimit = 40;

		m_pUserLimitTab->AddTab("select_button", 2, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 3, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 4, &WSize(50.0f, 20.0f));

		m_pHoleTab->AddTab("select_button", 3, &WSize(37.5f, 20.0f));
		m_pHoleTab->AddTab("select_button", 6, &WSize(37.5f, 20.0f));
		m_pHoleTab->AddTab("select_button", 9, &WSize(37.5f, 20.0f));
		m_pHoleTab->AddTab("select_button", 18, &WSize(37.5f, 20.0f));

		m_pTimeLimitTab->AddTab("select_button", 40, &WSize(37.5f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 60, &WSize(37.5f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 90, &WSize(37.5f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 120, &WSize(37.5f, 20.0f));

		if (!b)
			m_pMakeBtn[0]->SetStatus(FrButton::PRESSED);
		break;

	case 1:
		m_pPicture->Open("picture_match.tga");
		m_pText->SetBgImg("text_vs");

		m_makeBtnIndex = 0;
		m_userLimit = 4;
		m_hole = 6;
		m_turnTime = 40000;
		m_timeLimit = 40;

		m_pUserLimitTab->AddTab("select_button", 2, &WSize(75.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 4, &WSize(75.0f, 20.0f));

		m_pHoleTab->AddTab("select_button", 6, &WSize(50.0f, 20.0f));
		m_pHoleTab->AddTab("select_button", 9, &WSize(50.0f, 20.0f));
		m_pHoleTab->AddTab("select_button", 18, &WSize(50.0f, 20.0f));

		m_pTimeLimitTab->AddTab("select_button", 40, &WSize(37.5f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 60, &WSize(37.5f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 90, &WSize(37.5f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 120, &WSize(37.5f, 20.0f));

		if (!b)
			m_pMakeBtn[0]->SetStatus(FrButton::PRESSED);
		break;

	case 4:
		m_pPicture->Open("picture_30s.tga");
		m_pText->SetBgImg("text_mass");

		m_makeBtnIndex = 1;
		m_userLimit = 30;
		m_hole = 18;
		m_gameTime = 1800000;
		m_timeLimit = 30;

		if (IsLocalContent((localContentType_t)0x2B) && (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity & 4)) {
			m_pUserLimitTab->AddTab("select_button", 10, &WSize(25.0f, 20.0f));
			m_pUserLimitTab->AddTab("select_button", 20, &WSize(25.0f, 20.0f));
			m_pUserLimitTab->AddTab("select_button", 30, &WSize(25.0f, 20.0f));
			m_pUserLimitTab->AddTab("select_button", 100, &WSize(25.0f, 20.0f));
			m_pUserLimitTab->AddTab("select_button", 200, &WSize(25.0f, 20.0f));
			m_pUserLimitTab->AddTab("select_button", 250, &WSize(25.0f, 20.0f));
		} else {
			m_pUserLimitTab->AddTab("select_button", 10, &WSize(50.0f, 20.0f));
			m_pUserLimitTab->AddTab("select_button", 20, &WSize(50.0f, 20.0f));
			m_pUserLimitTab->AddTab("select_button", 30, &WSize(50.0f, 20.0f));
		}

		m_pHoleTab->AddTab("select_button", 9, &WSize(75.0f, 20.0f));
		m_pHoleTab->AddTab("select_button", 18, &WSize(75.0f, 20.0f));

		m_pTimeLimitTab->AddTab("select_button", 30, &WSize(30.0f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 35, &WSize(30.0f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 40, &WSize(30.0f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 45, &WSize(30.0f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 50, &WSize(30.0f, 20.0f));

		if (!b)
			m_pMakeBtn[1]->SetStatus(FrButton::PRESSED);
		break;

	case 5:
		m_pPicture->Open("picture_30steam.tga");
		m_pText->SetBgImg("text_mass");

		m_makeBtnIndex = 1;
		m_userLimit = 30;
		m_hole = 18;
		m_gameTime = 1800000;
		m_timeLimit = 30;

		m_pUserLimitTab->AddTab("select_button", 10, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 20, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 30, &WSize(50.0f, 20.0f));

		m_pHoleTab->AddTab("select_button", 9, &WSize(75.0f, 20.0f));
		m_pHoleTab->AddTab("select_button", 18, &WSize(75.0f, 20.0f));

		m_pTimeLimitTab->AddTab("select_button", 30, &WSize(30.0f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 35, &WSize(30.0f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 40, &WSize(30.0f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 45, &WSize(30.0f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 50, &WSize(30.0f, 20.0f));

		if (!b)
			m_pMakeBtn[1]->SetStatus(FrButton::PRESSED);
		break;

	case 6:
		m_pPicture->Open("picture_guild.tga");
		m_pText->SetBgImg("text_mass");

		m_makeBtnIndex = 1;
		m_userLimit = 30;
		m_hole = 18;
		m_gameTime = 1800000;
		m_timeLimit = 30;

		m_pUserLimitTab->AddTab("select_button", 10, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 20, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 30, &WSize(50.0f, 20.0f));

		m_pHoleTab->AddTab("select_button", 9, &WSize(75.0f, 20.0f));
		m_pHoleTab->AddTab("select_button", 18, &WSize(75.0f, 20.0f));

		m_pTimeLimitTab->AddTab("select_button", 30, &WSize(50.0f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 35, &WSize(50.0f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 40, &WSize(50.0f, 20.0f));

		if (!b)
			m_pMakeBtn[1]->SetStatus(FrButton::PRESSED);
		break;

	case 7:
		m_pPicture->Open("picture_battle.tga");
		m_pText->SetBgImg("text_vs");

		m_makeBtnIndex = 2;
		m_userLimit = 2;
		m_hole = 6;
		m_turnTime = 40000;
		m_timeLimit = 40;

		m_pUserLimitTab->AddTab("select_button", 2, &WSize(75.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 4, &WSize(75.0f, 20.0f));

		m_pHoleTab->AddTab("select_button", 6, &WSize(50.0f, 20.0f));
		m_pHoleTab->AddTab("select_button", 9, &WSize(50.0f, 20.0f));
		m_pHoleTab->AddTab("select_button", 18, &WSize(50.0f, 20.0f));

		m_pTimeLimitTab->AddTab("select_button", 40, &WSize(37.5f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 60, &WSize(37.5f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 90, &WSize(37.5f, 20.0f));
		m_pTimeLimitTab->AddTab("select_button", 120, &WSize(37.5f, 20.0f));

		if (!b)
			m_pMakeBtn[2]->SetStatus(FrButton::PRESSED);
		break;

	case 10:
		m_pPicture->Open("picture_approach.tga");
		m_pText->SetBgImg("text_vs");

		m_makeBtnIndex = 2;
		m_userLimit = 6;
		m_hole = 3;
		m_gameTime = 40000;
		m_timeLimit = 40;
		m_holeType = 3;

		m_pUserLimitTab->AddTab("select_button", 6, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 20, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 30, &WSize(50.0f, 20.0f));

		m_pHoleTab->AddTab("select_button", 3, &WSize(50.0f, 20.0f));
		m_pHoleTab->AddTab("select_button", 6, &WSize(50.0f, 20.0f));
		m_pHoleTab->AddTab("select_button", 9, &WSize(50.0f, 20.0f));

		m_pTimeLimitTab->AddTab("select_button", 40, &WSize(150.0f, 20.0f));

		if (!b)
			m_pMakeBtn[2]->SetStatus(FrButton::PRESSED);
		break;

	case 9:
		m_pPicture->Open("picture_approach.tga");
		m_pText->SetBgImg("text_vs");

		m_makeBtnIndex = 2;
		m_userLimit = 30;
		m_hole = 1;
		m_gameTime = 40000;
		m_timeLimit = 40;
		m_holeType = 2;

		m_pUserLimitTab->AddTab("select_button", 10, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 20, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 30, &WSize(50.0f, 20.0f));

		m_pHoleTab->AddTab("select_button", 1, &WSize(150.0f, 20.0f));

		m_pTimeLimitTab->AddTab("select_button", 40, &WSize(150.0f, 20.0f));

		if (!b)
			m_pMakeBtn[2]->SetStatus(FrButton::PRESSED);
		break;

	case 2:
		m_pPicture->Open("picture_chat.tga");
		m_pText->SetBgImg("text_chat");

		m_makeBtnIndex = 3;
		m_userLimit = 20;
		m_hole = 1;

		m_pUserLimitTab->AddTab("select_button", 10, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 20, &WSize(50.0f, 20.0f));
		m_pUserLimitTab->AddTab("select_button", 30, &WSize(50.0f, 20.0f));

		if (!b)
			m_pMakeBtn[3]->SetStatus(FrButton::PRESSED);
		break;

	case 14:
		m_pPicture->Open("picture_chaos.tga");
		m_pText->SetBgImg("text_chaos");

		m_makeBtnIndex = 4;
		m_userLimit = 30;
		m_hole = 18;
		m_gameTime = 1800000;
		m_timeLimit = 30;
		m_holeType = 4;

		if (!b)
			m_pMakeBtn[4]->SetStatus(FrButton::PRESSED);
		break;
	}

	m_pTimeLimitTab->SetVisible(m_gameType != 6);
	m_pTitleEdit->SetLine(1, WSingleton<CSharedDoc>::Instance()->GetDefaultTitle(m_gameType), 0, false, 0);

	if (m_hole == 18 && m_gameType != 10 && m_gameType != 14) {
		if (m_holeType == 1 || m_holeType == 2)
			m_holeType = 0;
	}

	if (b) {
		m_pGameType->ClearTab();

		switch (m_makeBtnIndex) {
		case 0:
			m_pGameType->AddTab("select_button", 0, &WSize(75.0f, 20.0f));
			m_pGameType->AddTab("select_button", 1, &WSize(75.0f, 20.0f));
			break;

		case 1:
			m_pGameType->AddTab("select_button", 4, &WSize(50.0f, 20.0f));
			m_pGameType->AddTab("select_button", 5, &WSize(50.0f, 20.0f));
			m_pGameType->AddTab("select_button", 6, &WSize(50.0f, 20.0f));
			break;

		case 2:
			if (IsLocalContent((localContentType_t)0x3E)) {
				if (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800) {
					m_pGameType->AddTab("select_button", 10, &WSize(150.0f, 20.0f));
				} else {
					m_pGameType->AddTab("select_button", 10, &WSize(75.0f, 20.0f));
					m_pGameType->AddTab("select_button", 7, &WSize(75.0f, 20.0f));
				}
			} else {
				m_pGameType->AddTab("select_button", 10, &WSize(75.0f, 20.0f));
				m_pGameType->AddTab("select_button", 7, &WSize(75.0f, 20.0f));
			}
			break;

		case 3:
			m_pGameType->AddTab("select_button", 2, &WSize(150.0f, 20.0f));
			break;

		case 4:
			m_pGameType->ClearTab();
			break;
		}
	}

	m_pGameType->Select(m_gameType);

	m_pUserLimitTab->Select(m_userLimit);
	m_pHoleTab->Select(m_hole);
	m_pTimeLimitTab->Select(m_timeLimit);
}

// makeroomdlg.cpp:1387
void FrMakeRoomDlg::ChangeUserLimit(unsigned char userLimit) {
	m_userLimit = userLimit;
}

// makeroomdlg.cpp:1392
void FrMakeRoomDlg::ChangeHole(unsigned char hole) {
	if (m_hole == hole)
		return;

	m_hole = hole;

	switch (m_gameType) {
	case 0:
	case 1:
	case 7:
		if (hole == 18) {
			if (m_holeType == 1 || m_holeType == 2)
				m_holeType = 0;
		}
		break;

	case 4:
	case 5:
		if (hole == 9) {
			m_pTimeLimitTab->ClearTab();
			m_pTimeLimitTab->AddTab("select_button", 15, &WSize(37.5f, 20.0f));
			m_pTimeLimitTab->AddTab("select_button", 20, &WSize(37.5f, 20.0f));
			m_pTimeLimitTab->AddTab("select_button", 25, &WSize(37.5f, 20.0f));
			m_pTimeLimitTab->AddTab("select_button", 30, &WSize(37.5f, 20.0f));

			m_timeLimit = 15;
			m_pTimeLimitTab->Select(15);
		} else if (hole == 18) {
			m_pTimeLimitTab->ClearTab();
			m_pTimeLimitTab->AddTab("select_button", 30, &WSize(30.0f, 20.0f));
			m_pTimeLimitTab->AddTab("select_button", 35, &WSize(30.0f, 20.0f));
			m_pTimeLimitTab->AddTab("select_button", 40, &WSize(30.0f, 20.0f));
			m_pTimeLimitTab->AddTab("select_button", 45, &WSize(30.0f, 20.0f));
			m_pTimeLimitTab->AddTab("select_button", 50, &WSize(30.0f, 20.0f));

			m_timeLimit = 30;
			m_pTimeLimitTab->Select(30);
			if (m_holeType == 1 || m_holeType == 2)
				m_holeType = 0;
		}
		break;

	case 6:
		if (hole == 9) {
			m_pTimeLimitTab->ClearTab();
			m_pTimeLimitTab->AddTab("select_button", 20, &WSize(37.5f, 20.0f));

			m_timeLimit = 20;
			m_pTimeLimitTab->Select(20);
		} else if (hole == 18) {
			m_pTimeLimitTab->ClearTab();
			m_pTimeLimitTab->AddTab("select_button", 30, &WSize(30.0f, 20.0f));
			m_pTimeLimitTab->AddTab("select_button", 35, &WSize(30.0f, 20.0f));
			m_pTimeLimitTab->AddTab("select_button", 40, &WSize(30.0f, 20.0f));

			m_timeLimit = 30;
			m_pTimeLimitTab->Select(30);
		}
		m_pTimeLimitTab->SetVisible(false);
		m_holeType = 0;
		break;

	case 14:
		m_pTimeLimitTab->ClearTab();
		m_pTimeLimitTab->SetVisible(false);
		m_timeLimit = 30;
		m_holeType = 2;
		break;
	}
}

// makeroomdlg.cpp:1502
void FrMakeRoomDlg::ChangeTimeLimit(unsigned long timeLimit) {
	m_timeLimit = timeLimit;
}

// makeroomdlg.cpp:1507
void FrMakeRoomDlg::ChangeCourse(unsigned char course) {
	IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(0x28000000 | ((course > 0x7F && course != 0xFD) ? course - 0x80 : course));
	if (pCourse == 0)
		return;

	if (course != 0x7F) {
		m_pCourse->Open(MakeStr("small_%s.tga", pCourse->m_icon));
		m_pCourseChat->Open(MakeStr("small_%s.tga", pCourse->m_icon));

		unsigned long gauge = WSingleton<CTHunter>::Instance()->GetTHunterGauge((eMapType)course);
		if (gauge != 9999) {
			m_pOverGauge->ResetGauge();
			m_pOverGauge->SetDestPos(gauge < 701 ? 0 : gauge - 700, false);
		}
	} else {
		m_pCourse->Open("map_random_middle.tga");
		m_pCourseChat->Open("map_random_middle.tga");

		if (m_pOverGauge) {
			m_pOverGauge->ResetGauge();
			m_pOverGauge->SetDestPos(0, false);
		}
	}

	m_course = course;

	if (m_pMapPrev)
		m_pMapPrev->Enable(m_course != FirstMap());
	if (m_pMapPrevChat)
		m_pMapPrevChat->Enable(m_course != FirstMap());

	if (m_pMapNext)
		m_pMapNext->Enable(m_course != LastMap(false));
	if (m_pMapNextChat)
		m_pMapNextChat->Enable(m_course != LastMap(true));
}

// makeroomdlg.cpp:1610
void FrMakeRoomDlg::ChangeHoleType(eHoleType holeType) {
	m_holeType = holeType;
}

// makeroomdlg.cpp:1615
void FrMakeRoomDlg::OnGameTypeInit(intptr_t param) {
	m_pGameType = DYNAMIC_CAST(FrTabButton, param);

	if (m_pGameType) {
		m_pGameType->SetSepWidth(1.0f);
	}
}

// makeroomdlg.cpp:1624
void FrMakeRoomDlg::OnGameTypeButtonUp(void) {
	TButtonItem* pItem = m_pGameType->GetSelected();
	if (pItem) {
		ChangeGameType((unsigned char)pItem->index, false);
	}
}

// makeroomdlg.cpp:1633
void FrMakeRoomDlg::OnGameTypeOwnerDraw(intptr_t param) {
	TButtonItem* pItem = (TButtonItem*)param;
	if (pItem == 0)
		return;

	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	if (pDevice == 0)
		return;

	pDevice->PrintText11(WPoint(pItem->rect.x + pItem->rect.w * 0.5f, pItem->rect.y + 5.0f), 1, s_gameTypeName[pItem->index], 0);
}

// makeroomdlg.cpp:1646
void FrMakeRoomDlg::OnUserLimitInit(intptr_t param) {
	m_pUserLimitTab = DYNAMIC_CAST(FrTabButton, param);

	if (m_pUserLimitTab) {
		m_pUserLimitTab->SetSepWidth(1.0f);
	}
}

// makeroomdlg.cpp:1655
void FrMakeRoomDlg::OnUserLimitButtonUp(void) {
	TButtonItem* pItem = m_pUserLimitTab->GetSelected();
	if (pItem) {
		m_userLimit = (unsigned char)pItem->index;
	}
}

// makeroomdlg.cpp:1664
void FrMakeRoomDlg::OnUserLimitOwnerDraw(intptr_t param) {
	TButtonItem* pItem = (TButtonItem*)param;
	if (pItem == 0)
		return;

	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	if (pDevice == 0)
		return;

	pDevice->PrintText11(WPoint(pItem->rect.x + pItem->rect.w * 0.5f, pItem->rect.y + 5.0f), 1, MakeStr("%d", pItem->index), 0);
}

// makeroomdlg.cpp:1677
void FrMakeRoomDlg::OnHoleInit(intptr_t param) {
	m_pHoleTab = DYNAMIC_CAST(FrTabButton, param);

	if (m_pHoleTab) {
		m_pHoleTab->SetSepWidth(1.0f);
	}
}

// makeroomdlg.cpp:1686
void FrMakeRoomDlg::OnHoleButtonUp(void) {
	TButtonItem* pItem = m_pHoleTab->GetSelected();
	if (pItem) {
		ChangeHole((unsigned char)pItem->index);
	}
}

// makeroomdlg.cpp:1695
void FrMakeRoomDlg::OnHoleOwnerDraw(intptr_t param) {
	TButtonItem* pItem = (TButtonItem*)param;
	if (pItem == 0)
		return;

	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	if (pDevice == 0)
		return;

	pDevice->PrintText11(WPoint(pItem->rect.x + pItem->rect.w * 0.5f, pItem->rect.y + 5.0f), 1, MakeStr("%d", pItem->index), 0);
}

// makeroomdlg.cpp:1708
void FrMakeRoomDlg::OnTimeLimitInit(intptr_t param) {
	m_pTimeLimitTab = DYNAMIC_CAST(FrTabButton, param);

	if (m_pTimeLimitTab) {
		m_pTimeLimitTab->SetSepWidth(1.0f);
	}
}

// makeroomdlg.cpp:1717
void FrMakeRoomDlg::OnTimeLimitButtonUp(void) {
	TButtonItem* pItem = m_pTimeLimitTab->GetSelected();
	if (pItem) {
		m_timeLimit = pItem->index;
	}
}

// makeroomdlg.cpp:1726
void FrMakeRoomDlg::OnTimeLimitOwnerDraw(intptr_t param) {
	TButtonItem* pItem = (TButtonItem*)param;
	if (pItem == 0)
		return;

	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	if (pDevice == 0)
		return;

	pDevice->PrintText11(WPoint(pItem->rect.x + pItem->rect.w * 0.5f, pItem->rect.y + 5.0f), 1, MakeStr("%d", pItem->index), 0);
}

// makeroomdlg.cpp:1739
void FrMakeRoomDlg::OnCourseInit(intptr_t param) {
	m_pCourse = DYNAMIC_CAST(FrViewer, param);
}

// makeroomdlg.cpp:1744
bool FrMakeRoomDlg::OnMapSelectDlgResult(int result, FrForm* pForm) {
	unsigned char course = 0;
	if (pForm) {

		course = static_cast<FrTreasureCourse*>(pForm)->GetCurMapIdx();
	}

	ChangeCourse(course);
	m_pMapSelectDlg = 0;
	m_btnDelay = 0.7f;
	return true;
}

// makeroomdlg.cpp:1759
void FrMakeRoomDlg::OnCourseUp(void) {
	if (m_gameType == 10 || m_gameType == 6 || m_gameType == 14)
		return;

	g_audio->PlaySfx("ui_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);

	m_pMapSelectDlg = CreateForm<FrTreasureCourse>(g_pFresh->GetManager(), this, "treasuremap", 0);
	if (m_pMapSelectDlg) {
		m_pMapSelectDlg->SetSelectMap(m_course);
		m_pMapSelectDlg->SetMapIndexChanged(false);
		// FrTreasureCourse 0x1eb: the random-course lock (FrTreasureCourse::LockRandom, treasuredlg.h:187, not yet declared)
		*((bool*)m_pMapSelectDlg + 0x1eb) = (m_gameType == 2);
		m_pMapSelectDlg->EnableDrag(false);
		m_pMapSelectDlg->Open((FRESH_RESULT_CALLBACK)&FrMakeRoomDlg::OnMapSelectDlgResult, 3);
	}
}

// makeroomdlg.cpp:1777
void FrMakeRoomDlg::OnCourseOwnerDraw(int param) {
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
			pDevice->DrawTexture(m_pMapSelectImg, WRect(rect.x - 25.0f, rect.y - 11.0f, m_pMapSelectImg->Width(), m_pMapSelectImg->Height()), 0xFFFFFFFF, 0);
	}
}

// makeroomdlg.cpp:1795
void FrMakeRoomDlg::OnHoleTypeInit(intptr_t param) {
	m_pHoleType = DYNAMIC_CAST(FrArea, param);

	if (m_pHoleType) {
		m_pHoleTypeImg[0] = g_pFresh->GetManager()->GetBitmap("ITEMS", "hole_front_n");
		m_pHoleTypeImg[1] = g_pFresh->GetManager()->GetBitmap("ITEMS", "hole_front_d");
		m_pHoleTypeImg[2] = g_pFresh->GetManager()->GetBitmap("ITEMS", "hole_back_n");
		m_pHoleTypeImg[3] = g_pFresh->GetManager()->GetBitmap("ITEMS", "hole_back_d");
		m_pHoleTypeImg[4] = g_pFresh->GetManager()->GetBitmap("ITEMS", "hole_random_n");
		m_pHoleTypeImg[5] = g_pFresh->GetManager()->GetBitmap("ITEMS", "hole_random_d");
		m_pHoleTypeImg[6] = g_pFresh->GetManager()->GetBitmap("ITEMS", "hole_shuffle_n");
		m_pHoleTypeImg[7] = g_pFresh->GetManager()->GetBitmap("ITEMS", "hole_shuffle_d");
	}
}

// makeroomdlg.cpp:1811
void FrMakeRoomDlg::DrawHoleTypeButton(FrGraphicInterface* pDevice, eHoleType holeType, WRect rect, bool b) {
	const Bitmap* pImg = m_pHoleTypeImg[holeType * 2];
	unsigned long color = 0xFFFFFFFF;

	if (b) {
		if (m_holeType == holeType) {
			pImg = m_pHoleTypeImg[holeType * 2 + 1];
		} else {
			const WPoint& mouse = g_pFresh->GetManager()->GetMousePos();
			if (IsBtnEnable() && rect.IsInRect(mouse)) {
				if (g_input->GetButton(LEFT_BUTTON) == 2)
					m_holeType = holeType;
			}
		}
	} else {
		color = 0xFF909090;
	}

	pDevice->DrawTexture(pImg, rect, color, 0);
}

// makeroomdlg.cpp:1843
void FrMakeRoomDlg::OnHoleTypeOwnerDraw(int param) {
	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	if (pDevice == 0)
		return;

	WPoint pos(m_pHoleType->GetRect().x, m_pHoleType->GetRect().y);

	WRect front(pos.x, pos.y, m_pHoleTypeImg[0]->Width(), m_pHoleTypeImg[0]->Height());

	WRect back(front.x + front.w, pos.y, m_pHoleTypeImg[2]->Width(), front.h);
	WRect random(back.x + back.w, pos.y, m_pHoleTypeImg[4]->Width(), front.h);
	WRect shuffle(random.x + random.w, pos.y, m_pHoleTypeImg[6]->Width(), front.h);

	if (m_gameType == 10) {
		DrawHoleTypeButton(pDevice, (eHoleType)0, front, false);
		DrawHoleTypeButton(pDevice, (eHoleType)1, back, false);
		DrawHoleTypeButton(pDevice, (eHoleType)2, random, false);
		DrawHoleTypeButton(pDevice, (eHoleType)3, shuffle, false);
	} else if (m_gameType == 14) {
		DrawHoleTypeButton(pDevice, (eHoleType)0, front, false);
		DrawHoleTypeButton(pDevice, (eHoleType)1, back, false);
		DrawHoleTypeButton(pDevice, (eHoleType)2, random, false);
		DrawHoleTypeButton(pDevice, (eHoleType)3, shuffle, false);
	} else if (m_hole == 18) {
		DrawHoleTypeButton(pDevice, (eHoleType)0, front, true);
		DrawHoleTypeButton(pDevice, (eHoleType)1, back, false);
		DrawHoleTypeButton(pDevice, (eHoleType)2, random, false);
		DrawHoleTypeButton(pDevice, (eHoleType)3, shuffle, true);
	} else {
		DrawHoleTypeButton(pDevice, (eHoleType)0, front, true);
		DrawHoleTypeButton(pDevice, (eHoleType)1, back, true);
		DrawHoleTypeButton(pDevice, (eHoleType)2, random, true);
		DrawHoleTypeButton(pDevice, (eHoleType)3, shuffle, true);
	}
}

// makeroomdlg.cpp:1950
void FrMakeRoomDlg::OnCourseChatInit(int param) {
	m_pCourseChat = DYNAMIC_CAST(FrViewer, param);

	if (m_pCourseChat) {
		m_pCourseChat->SetVisible(false);
	}
}

// makeroomdlg.cpp:1959
void FrMakeRoomDlg::OnCourseChatUp(void) {
	OnCourseUp();
}

// makeroomdlg.cpp:1964
void FrMakeRoomDlg::OnHoleChatInit(int param) {
	m_pHoleChat = DYNAMIC_CAST(FrArea, param);

	if (m_pHoleChat) {
		m_pHoleChat->SetVisible(false);
	}
}

// makeroomdlg.cpp:1973
void FrMakeRoomDlg::OnHoleChatOwnerDraw(int param) {
	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	if (pDevice) {
		WRect rect = m_pHoleChat->GetRect();

		pDevice->PrintText11(WPoint(rect.x + 17.0f, rect.y + 5.0f), 1, MakeStr("%d", m_startHole), 0);
	}
}

// makeroomdlg.cpp:1985
void FrMakeRoomDlg::OnHoleMinInit(int param) {
	m_pHoleMin = DYNAMIC_CAST(FrButton, param);

	if (m_pHoleMin) {
		m_pHoleMin->SetVisible(false);
		m_pHoleMin->SetPushDelay(0.0f);
	}
}

// makeroomdlg.cpp:1995
void FrMakeRoomDlg::OnHoleMinUp(void) {
	m_startHole = 1;
}

// makeroomdlg.cpp:2000
void FrMakeRoomDlg::OnHolePrevInit(int param) {
	m_pHolePrev = DYNAMIC_CAST(FrButton, param);

	if (m_pHolePrev) {
		m_pHolePrev->SetVisible(false);
		m_pHolePrev->SetPushDelay(0.0f);
	}
}

// makeroomdlg.cpp:2010
void FrMakeRoomDlg::OnHolePrevUp(void) {
	if (m_startHole > 1)
		m_startHole--;
}

// makeroomdlg.cpp:2016
void FrMakeRoomDlg::OnHoleNextInit(int param) {
	m_pHoleNext = DYNAMIC_CAST(FrButton, param);

	if (m_pHoleNext) {
		m_pHoleNext->SetVisible(false);
		m_pHoleNext->SetPushDelay(0.0f);
	}
}

// makeroomdlg.cpp:2026
void FrMakeRoomDlg::OnHoleNextUp(void) {
	if (m_startHole < 18)
		m_startHole++;
}

// makeroomdlg.cpp:2032
void FrMakeRoomDlg::OnHoleMaxInit(int param) {
	m_pHoleMax = DYNAMIC_CAST(FrButton, param);

	if (m_pHoleMax) {
		m_pHoleMax->SetVisible(false);
		m_pHoleMax->SetPushDelay(0.0f);
	}
}

// makeroomdlg.cpp:2042
void FrMakeRoomDlg::OnHoleMaxUp(void) {
	m_startHole = 18;
}

// makeroomdlg.cpp:2047
void FrMakeRoomDlg::OnOverGauge_Init(int param) {
	m_pOverGauge = DYNAMIC_CAST(FrGaugeBarImage, param);

	if (m_pOverGauge) {
		m_pOverGauge->SetRange(0, 300, 0);
		m_pOverGauge->SetBarSpeed(200.0f);
	}
}

// makeroomdlg.cpp:2060
void FrMakeRoomDlg::OnProc(const float dt) {
	FrForm::OnProc(dt);

	if (m_pMapSelectDlg == 0 && m_btnDelay >= 0.0f) {
		m_btnDelay -= dt;
	}
}

// makeroomdlg.cpp:2080
bool FrMakeRoomDlg::IsBtnEnable(void) {
	if (m_pMapSelectDlg == 0 && m_btnDelay < 0.0f)
		return true;

	return false;
}
