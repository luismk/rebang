// Original: d:\build\custom\temp\code\client\projectg\scoreboard.cpp
#define SCOREBOARD_EXHIBITION_INLINE
#include <string.h>

#include "scoreboard.h"
#include "basicdebug.h"
#include "bitmap.h"
#include "QuitWindow.h"
#include "InputManager.h"
#include "LevelUpItemDlg.h"
#include "ChatMsg.h"
#include "ClientSetting.h"
#include "CapturedBg.h"
#include "commonutil.h"
#include "exhibition.h"
#include "fresh.h"
#include "frbutton.h"
#include "frdesktop.h"
#include "frarea.h"
#include "fredit.h"
#include "frframe.h"
#include "frgaugebar.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frscrollbar.h"
#include "frstatic.h"
#include "frwndmanager.h"
#include "GolfDoc.h"
#include "HeadIcon.h"
#include "NetResourceManager.h"
#include "ObjectFactory.h"
#include "ScoreDlg.h"
#include "SharedDoc.h"
#include "SimpleOverlay.h"
#include "SoundManager.h"
#include "TaskManager.h"
#include "Treasure.h"
#include "TreasureDlg.h"
#include "user_info.h"
#include "voiceitem.h"
#include "wutil.h"
#include "wview.h"

// Directory of the score board's character images (defined elsewhere; cl
// 13.10 decorates this declaration ?SCORE_PPL_DIR@@3PBDB, the original's).
extern const char* SCORE_PPL_DIR;

unsigned char GetDifficulty(void);   // club.cpp

// Hole kind points by difficulty and hole count (CScore::HandleMsg 0x13a);
// no symbol, its contents are not transcribed yet.
extern const int data_9f1f48[][19][11];

// The level names (no symbol, contents not transcribed): records of 12 bytes
// whose first member names levels 0-20; FrAwardForm::OnInit appends entry
// 21's to the 20-byte names of levels 21-65.
struct sLevelTitle {
	const char* m_name;  // 0x0
	UNKNOWN(0x4, 0xC);
};
extern sLevelTitle data_a81a50[];
extern const char data_9f19c4[][20];

// treasure.h:100
bool CTHunter::IsReceiveGiftPacket() {
	return m_bReceiveGiftPacket;
}

// shareddoc.h:1143
int CSharedDoc::GetUserInfoMod(void) {
	return m_userInfoMod;
}

// scoreboard.h:670
void FrMatchItems::Reset(void) {
	m_bRedraw = m_bRefresh = true;
}

// scoreboard.cpp:102
bool RankCompare(const void* a, const void* b) {
	const sGameResultInfo* pA = (const sGameResultInfo*)a;
	const sGameResultInfo* pB = (const sGameResultInfo*)b;

	if (pA->m_sortGroup > pB->m_sortGroup) {
		return true;
	} else if (pA->m_sortGroup < pB->m_sortGroup) {
		return false;
	}

	if (pA->m_rank < pB->m_rank) {
		return true;
	} else if (pA->m_rank > pB->m_rank) {
		return false;
	}

	if (pA->m_score < pB->m_score) {
		return true;
	} else if (pA->m_score > pB->m_score) {
		return false;
	}

	if (pA->m_pang > pB->m_pang) {
		return true;
	} else if (pA->m_pang < pB->m_pang) {
		return false;
	}

	return pA->m_value13F < pB->m_value13F;
}

// scoreboard.cpp:138 (run-time type and factory registration of FrRankForm)
const WRTTI FrRankForm::m_RTTI("FrRankForm", &FrForm::m_RTTI);

IObject* FrRankFormMakeInstance(void) {
	return new FrRankForm;
}

struct __impFrRankForm {
	__impFrRankForm(void) {
		ObjectFactory().AddObjectFunctor(FrRankFormMakeInstance, "FrRankForm");
		ActorFactory().AddActorClass("FrRankForm");
	}
};
static __impFrRankForm s_registerFrRankForm;

// scoreboard.cpp:140
const sFRESH_MSGMAP* FrRankForm::GetMessageMap(void) const {
	return &FrRankForm::_MsgMap;
}

sFRESH_MSGMAP FrRankForm::_MsgMap = { &FrForm::_MsgMap, &FrRankForm::_MsgEntries[0] };

sFRESH_ENTRY FrRankForm::_MsgEntries[] = {
	// scoreboard.cpp:142
	{ "list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm::OnRankInit },
	// scoreboard.cpp:143
	{ "list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm::OnRankOwnerDraw },
	// scoreboard.cpp:144
	{ "list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrRankForm::OnRankLBtnDown },
	// scoreboard.cpp:145
	{ "list", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrRankForm::OnRankRBtnUp },
	// scoreboard.cpp:146
	{ "list", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&FrRankForm::OnRankDClick },
	// scoreboard.cpp:147
	{ "center", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrRankForm::OnCenterBtnUp },
	// scoreboard.cpp:148
	{ "info", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrRankForm::OnInfoBtnUp },
	// scoreboard.cpp:149
	{ "size", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrRankForm::OnSizeBtnUp },
	// scoreboard.cpp:150
	{ "billboard", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm::OnBoardInit },
	// scoreboard.cpp:151
	{ "bar01", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm::OnBar01Init },
	// scoreboard.cpp:152
	{ "bar02", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm::OnBar02Init },

	// scoreboard.cpp:154
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// scoreboard.cpp:157
FrRankForm::FrRankForm(void) {
	m_pRankList = 0;
	m_pBoard = 0;
	m_rankIndex = -1;
	m_rankPage = -1;
	m_selectedUID = -1;
	m_announcedHole = 0;
	m_scrollCount = 0;

	memset(m_rankText, 0, sizeof(m_rankText));
}

// scoreboard.cpp:171
void FrRankForm::OnProc(const float dt) {
	// scoreboard.cpp:172
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
		if (g_input->Get("LSHIFT", true) || g_input->Get("RSHIFT", true)) {
			if (g_input->GetDown("PAGE_DOWN", true)) {
				m_pRankList->GetScrollBar()->ScrollDown(1);
			} else if (g_input->GetDown("PAGE_UP", true)) {
				m_pRankList->GetScrollBar()->ScrollUp(1);
			}
		}
	}

	// scoreboard.cpp:184
	switch (m_captionType) {
	case 0: {
		const char* text;
		CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
		if (pDoc->m_roomType == 5) {
			// scoreboard.cpp:190
			text = MakeStr("RED %d\xC1\xA1/%I64d\xC6\xCE  vs  BLUE %d\xC1\xA1/%I64d\xC6\xCE",
				pDoc->m_teamResult[0].m_score, pDoc->m_teamResult[0].m_pang, pDoc->m_teamResult[1].m_score, pDoc->m_teamResult[1].m_pang);
		} else if (m_rankIndex != -1) {
			// scoreboard.cpp:196
			if (pDoc->m_roomType == 10) {
				text = MakeStr("\xC7\xF6\xC0\xE7 1\xC0\xA7 %s\xB4\xD4, \xB3\xB2\xC0\xBA\xB0\xC5\xB8\xAE : %d.%d y",
					pDoc->m_gameResultList[pDoc->GetIndex(m_rankIndex)].m_nickname,
					pDoc->m_gameResultList[pDoc->GetIndex(m_rankIndex)].m_approachDistance / 10,
					pDoc->m_gameResultList[pDoc->GetIndex(m_rankIndex)].m_approachDistance % 10);
			} else {
				// scoreboard.cpp:202
				text = MakeStr("\xC7\xF6\xC0\xE7 1\xC0\xA7 %s\xB4\xD4, \xBD\xBA\xC4\xDA\xBE\xEE %d",
					pDoc->m_gameResultList[pDoc->GetIndex(m_rankIndex)].m_nickname,
					pDoc->m_gameResultList[pDoc->GetIndex(m_rankIndex)].m_score);
			}
		} else {
			// scoreboard.cpp:206
			text = MakeStr("\xC7\xF6\xC0\xE7 \xB0\xD4\xC0\xD3\xC1\xDF\xC0\xCE \xC0\xCE\xBF\xF8\xC0\xBA %d\xB8\xED\xC0\xD4\xB4\xCF\xB4\xD9.", m_rankCount);
		}
		m_pBoard->SetCaption(text);

		// scoreboard.cpp:209
		m_boardMessage = 4;
		break;
	}

	case 2: {
		// scoreboard.cpp:216
		WRect rect;
		m_pBoard->GetClientRect(rect);
		rect.x -= dt * 30.0f;
		m_pBoard->SetClientRect(rect);

		// scoreboard.cpp:221
		if (rect.x + rect.w < g_view->GetWidth() - 239.0f) {
			m_scrollCount++;
			if (m_scrollCount == 2) {
				m_scrollCount = 0;
				SetCaptionType(0);
			} else {
				// scoreboard.cpp:230
				SetCaptionType(2);
			}
		}
		break;
	}

	case 1:
		// scoreboard.cpp:237
		m_captionTime += dt;
		if (m_captionTime > 10.0f) {
			m_captionTime = 0;
			SetCaptionType(0);
		}
		break;
	}
}

// scoreboard.cpp:619
void FrRankForm::OnRankLBtnDown(void) {
	FrListItem* pItem = m_pRankList->GetItemUnderCursor();
	if (pItem) {
		m_selectedUID = ((sGameResultInfo*)pItem->pData)->m_uid;
	}
}

// scoreboard.cpp:630
void FrRankForm::OnRankRBtnUp(void) {
	FrListItem* pItem = m_pRankList->GetItemUnderCursor();
	if (!pItem)
		return;

	sGameResultInfo* pInfo = (sGameResultInfo*)pItem->pData;
	if (!pInfo)
		return;

	m_selectedUID = pInfo->m_uid;

	if (WSingleton<CUserInfo>::Instance() && pInfo->m_state != 3)
		WSingleton<CUserInfo>::Instance()->SetInfo(pInfo->m_id, m_selectedUID, false, true, false, false, pInfo->m_nickname);
}

// scoreboard.cpp:648
void FrRankForm::OnRankDClick(void) {
	OnRankRBtnUp();
}

// scoreboard.cpp:653
void FrRankForm::OnBoardInit(intptr_t param) {
	m_pBoard = DYNAMIC_CAST(FrStatic, param);
	m_pBoard->m_nFlags.Enable(0x1000);

	m_pBoard->SetCaption("");
	SetCaptionType(0);
	m_boardMessage = 4;

	m_captionTime = 0;
}

// scoreboard.cpp:665
void FrRankForm::OnBar01Init(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea) {
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
			pArea->SetBgImg("approach_base_01");
		}
	}
}

// scoreboard.cpp:677
void FrRankForm::OnBar02Init(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea) {
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
			pArea->SetBgImg("approach_base_02");
		}
	}
}

// scoreboard.cpp:689
void FrRankForm::OnCenterBtnUp(void) {
	Align();
}

// scoreboard.cpp:722
void FrRankForm::OnInfoBtnUp(void) {
	std::map<unsigned long, sBriefUserInfo>::iterator it = CSharedDoc::Instance()->m_userMap.find(m_selectedUID);
	if (it != CSharedDoc::Instance()->m_userMap.end()) {
		CUserInfo::Instance()->SetInfo(it->second.id, m_selectedUID, true, true, false, false, std::string(it->second.nick));
	}
}

// scoreboard.cpp:731
void FrRankForm::OnSizeBtnUp(void) {
	m_sizeType = (m_sizeType + 1) % 3;
	Resize();
}

// scoreboard.cpp:738
void FrRankForm::Resize(void) {
	if (m_pRankList) {
		WRect rect;
		m_pRankList->GetClientRect(rect);

		switch (m_sizeType) {
		case 0:
			rect.h = 0;
			break;
		case 1:
			rect.h = 90.0f;
			break;
		case 2:
			rect.h = 180.0f;
			break;
		}
		m_pRankList->SetClientRect(rect);

		rect.h += 70.0f;
		rect.y = 0;
		rect.w = m_pBaseFrm->GetRect().w;
		SetClientRect(rect);
	}
}

// scoreboard.cpp:888
void FrRankForm::SetCaptionType(unsigned char type) {
	m_captionType = type;

	if (m_pBoard == 0) {
		return;
	}

	WRect rect;
	m_pBoard->GetClientRect(rect);

	if (m_captionType == 2) {
		WRect clip(rect.x, rect.y, 219.0f, 16.0f);
		m_pBoard->SetClippingArea(&clip);

		rect.x = 800.0f;
		rect.w = m_pBoard->GDI()->GetTextExtend(m_pBoard->GetCaption());
		m_pBoard->SetClientRect(rect);
	} else {
		m_pBoard->SetClippingArea(0);
		m_pBoard->SetClientRect(WRect(rect.x, rect.y, 219.0f, 16.0f));
	}
}

// scoreboard.cpp:915 (run-time type and factory registration of FrRankForm2)
const WRTTI FrRankForm2::m_RTTI("FrRankForm2", &FrForm::m_RTTI);

IObject* FrRankForm2MakeInstance(void) {
	return new FrRankForm2;
}

struct __impFrRankForm2 {
	__impFrRankForm2(void) {
		ObjectFactory().AddObjectFunctor(FrRankForm2MakeInstance, "FrRankForm2");
		ActorFactory().AddActorClass("FrRankForm2");
	}
};
static __impFrRankForm2 s_registerFrRankForm2;

// scoreboard.cpp:917
const sFRESH_MSGMAP* FrRankForm2::GetMessageMap(void) const {
	return &FrRankForm2::_MsgMap;
}

sFRESH_MSGMAP FrRankForm2::_MsgMap = { &FrForm::_MsgMap, &FrRankForm2::_MsgEntries[0] };

sFRESH_ENTRY FrRankForm2::_MsgEntries[] = {
	// scoreboard.cpp:919
	{ "list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm2::OnRankInit },
	// scoreboard.cpp:920
	{ "list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm2::OnRankOwnerDraw },

	// scoreboard.cpp:922
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// scoreboard.cpp:925
FrRankForm2::FrRankForm2(void) : m_pRankData(0) {
}

// scoreboard.cpp:1210 (run-time type and factory registration of FrRankForm3)
const WRTTI FrRankForm3::m_RTTI("FrRankForm3", &FrForm::m_RTTI);

IObject* FrRankForm3MakeInstance(void) {
	return new FrRankForm3;
}

struct __impFrRankForm3 {
	__impFrRankForm3(void) {
		ObjectFactory().AddObjectFunctor(FrRankForm3MakeInstance, "FrRankForm3");
		ActorFactory().AddActorClass("FrRankForm3");
	}
};
static __impFrRankForm3 s_registerFrRankForm3;

// scoreboard.cpp:1212
const sFRESH_MSGMAP* FrRankForm3::GetMessageMap(void) const {
	return &FrRankForm3::_MsgMap;
}

sFRESH_MSGMAP FrRankForm3::_MsgMap = { &FrForm::_MsgMap, &FrRankForm3::_MsgEntries[0] };

sFRESH_ENTRY FrRankForm3::_MsgEntries[] = {
	// scoreboard.cpp:1214
	{ "list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm3::OnRankInit },
	// scoreboard.cpp:1215
	{ "list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm3::OnRankOwnerDraw },
	// scoreboard.cpp:1216
	{ "size", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrRankForm3::OnSizeBtnUp },
	// scoreboard.cpp:1217
	{ "billboard", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm3::OnBoardInit },
	// scoreboard.cpp:1218
	{ "score_gb", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm3::OnScoreGaugeInit },
	// scoreboard.cpp:1219
	{ "score_info", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankForm3::OnScoreInfoInit },
	// scoreboard.cpp:1220
	{ "score_info", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrRankForm3::OnScoreInfoOwnerDraw },

	// scoreboard.cpp:1222
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// scoreboard.cpp:1225
FrRankForm3::FrRankForm3(void) {
	m_pRankList = 0;
	m_pBoard = 0;
	m_pScoreGauge = 0;
	m_pScoreInfo = 0;
	m_bFlag129 = false;
	m_captionCycle = 0;
	m_index124 = -1;
	m_sizeType = 1;
	LoadBitmaps();
}

// scoreboard.cpp:1241
void FrRankForm3::OnProc(const float dt) {
	switch (m_captionType) {
	case 0:
		// "There are %d players in the game now."
		m_pBoard->SetCaption(MakeStr("\xC7\xF6\xC0\xE7 \xB0\xD4\xC0\xD3\xC1\xDF\xC0\xCE \xC0\xCE\xBF\xF8\xC0\xBA %d\xB8\xED\xC0\xD4\xB4\xCF\xB4\xD9.", m_playerCount));
		break;
	case 2: {
		// the caption scrolls left until it leaves the board
		WRect rect;
		m_pBoard->GetClientRect(rect);
		rect.x -= dt * 30.0f;
		m_pBoard->SetClientRect(rect);

		if (rect.x + rect.w < g_view->GetWidth() - 239.0f) {
			m_captionCycle++;
			if (m_captionCycle == 2) {
				m_captionCycle = 0;
				SetCaptionType(0);
			} else {
				SetCaptionType(2);
			}
		}
		break;
	}
	case 1:
		m_captionTime += dt;
		if (m_captionTime > 10.0f) {
			m_captionTime = 0;
			SetCaptionType(0);
		}
		break;
	}
}

// scoreboard.cpp:1477
void FrRankForm3::OnBoardInit(intptr_t param) {
	m_pBoard = DYNAMIC_CAST(FrStatic, param);
	m_pBoard->m_nFlags.Enable(0x1000);

	m_pBoard->SetCaption("");
	SetCaptionType(0);

	m_captionTime = 0;
}

// scoreboard.cpp:1489
void FrRankForm3::OnScoreGaugeInit(intptr_t param) {
	m_pScoreGauge = DYNAMIC_CAST(FrGaugeBar, param);
	m_pScoreGauge->SetRange(0, 10, 5);
}

// scoreboard.cpp:1496
void FrRankForm3::SetScore(int score, int remain) {
	m_score = score;
	m_scoreRemain = remain;

	int pos = score + 1;

	m_pScoreGauge->SetRange(0, pos + remain + 1, pos);
}

// scoreboard.cpp:1563
void FrRankForm3::OnSizeBtnUp(void) {
	if (m_pRankList == 0) {
		return;
	}

	m_sizeType = (m_sizeType + 1) % 2;

	WRect rect;
	m_pRankList->GetClientRect(rect);

	switch (m_sizeType) {
	case 0:
		rect.h = 0;
		break;
	case 1:
		rect.h = m_pRankList->GetItemHeight() * 3.0f;
		break;
	}
	m_pRankList->SetClientRect(rect);

	rect.h += 113.0f;
	rect.w = 296.0f;
	rect.y = 0;
	m_pBaseFrm->SetClientRect(rect);
}

// scoreboard.cpp:1596
void FrRankForm3::SetCaptionType(unsigned char type) {
	m_captionType = type;

	if (m_pBoard == 0) {
		return;
	}

	WRect rect;
	m_pBoard->GetClientRect(rect);

	if (m_captionType == 2) {
		WRect clip(rect.x, rect.y, 219.0f, 16.0f);
		m_pBoard->SetClippingArea(&clip);

		rect.x = 800.0f;
		rect.w = m_pBoard->GDI()->GetTextExtend(m_pBoard->GetCaption());
		m_pBoard->SetClientRect(rect);
	} else {
		m_pBoard->SetClippingArea(0);
		m_pBoard->SetClientRect(WRect(rect.x, rect.y, 219.0f, 16.0f));
	}
}

// scoreboard.cpp:1630
void FrRankForm3::OnScoreInfoInit(intptr_t param) {
	m_pScoreInfo = DYNAMIC_CAST(FrArea, param);

	m_pRedEmblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_guildMark[0]);
	m_pBlueEmblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_guildMark[1]);

	memset(&m_score, 0, sizeof(m_score) + sizeof(m_scoreRemain));

	m_pScoreInfo->SetVisible(true);
}

// scoreboard.cpp:1679
void FrRankForm3::LoadBitmaps(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_roomType != 6) {
		return;
	}

	for (int i = 0; i < 10; i++) {
		m_pDigit[i] = g_pFresh->GetManager()->GetBitmap("SCORECARD", MakeStr("%d", i));
	}
	m_pPlus = g_pFresh->GetManager()->GetBitmap("SCORECARD", "+");
	m_pMinus = g_pFresh->GetManager()->GetBitmap("SCORECARD", "-");
}

// scoreboard.cpp:1694 (run-time type and factory registration of FrRankFormEx)
const WRTTI FrRankFormEx::m_RTTI("FrRankFormEx", &FrForm::m_RTTI);

IObject* FrRankFormExMakeInstance(void) {
	return new FrRankFormEx;
}

struct __sFrRankFormEx {
	__sFrRankFormEx(void) {
		ObjectFactory().AddObjectFunctor(FrRankFormExMakeInstance, "FrRankFormEx");
	}
};
static __sFrRankFormEx s_registerFrRankFormEx;

// scoreboard.cpp:1696
const sFRESH_MSGMAP* FrRankFormEx::GetMessageMap(void) const {
	return &FrRankFormEx::_MsgMap;
}

sFRESH_MSGMAP FrRankFormEx::_MsgMap = { &FrForm::_MsgMap, &FrRankFormEx::_MsgEntries[0] };

sFRESH_ENTRY FrRankFormEx::_MsgEntries[] = {
	// scoreboard.cpp:1698
	{ "stage", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankFormEx::OnStageAreaInit },
	// scoreboard.cpp:1699
	{ "stage", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrRankFormEx::OnStageAreaOwnerDraw },
	// scoreboard.cpp:1700
	{ "list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankFormEx::OnRankInit },
	// scoreboard.cpp:1701
	{ "list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankFormEx::OnRankOwnerDraw },
	// scoreboard.cpp:1702
	{ "billboard", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRankFormEx::OnBoardInit },

	// scoreboard.cpp:1704
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// scoreboard.cpp:1712
CRankExData::CRankExData(void) : m_pRival(0), m_motionLength(3.0f), m_motionTime(0), m_leader(-1), m_showLeader(-1),
	m_pRankList(0), m_pAreaStage(0), m_pBoard(0) {
}

// scoreboard.cpp:1717
CRankExData::~CRankExData(void) {
}

// scoreboard.cpp:1722
void CRankExData::InitAreaStage(FrArea* pArea) {
	m_pAreaStage = pArea;
}

// scoreboard.cpp:1727
void CRankExData::Clear(void) {
	if (m_pExhibition) { delete m_pExhibition; m_pExhibition = 0; }
	if (m_pRival) { delete[] m_pRival; m_pRival = 0; }
}

// scoreboard.cpp:1734
void CRankExData::OnProc(float dt) {
	m_motionTime += dt;

	if (m_leader == -1) {
		SetLeader();
	} else if (m_showLeader != -1) {
		ShowLeader();
	} else if (m_pExhibition) {
		m_pExhibition->Process(dt, false);
	}
}

// scoreboard.cpp:1866
void CRankExData::CreateLeaderPet(const WRect& rect) {
	m_pExhibition = new CExhibition(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f, 0.3f, false);
	m_pExhibition->SetDistance(g_view->GetWidth() * 0.028125f, true, false);
	m_pExhibition->SetArea(rect.w, rect.h);
}

// scoreboard.cpp:1873
void CRankExData::SetActive(bool bActive) {
	m_showLeader = -1;
	m_leader = -1;
	m_motionLength = 3.0f;
	m_motionTime = 0;
	if (bActive) {
		SetLeader();
	}
}

// scoreboard.cpp:1883
bool CRankExData::IsMotionFinished(void) {
	return m_motionTime > (m_motionLength + 1.0f > 10.0f ? 10.0f : m_motionLength + 1.0f);
}

// scoreboard.cpp:1888
void CRankExData::ResetStage(void) {
	if (m_pExhibition && m_pAreaStage) {
		const WRect& rect = m_pAreaStage->GetRect();
		m_pExhibition->SetCenter(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f);
		m_pExhibition->SetDistance(g_view->GetWidth() * 0.028125f, true, false);
	}
}

// scoreboard.cpp:1899
void CRankExData::InitRivalData(void) {
	int count = WSingleton<CSharedDoc>::Instance()->m_gameResultList.size();
	m_pRival = new sRivalData[count];

	for (int i = 0; i < count; i++) {
		m_pRival[i] = WSingleton<CSharedDoc>::Instance()->m_gameResultList[i];

		m_pRankList->AddItem(&m_pRival[i]);
	}
}

// scoreboard.cpp:1912
void CRankExData::InitMedalData(void) {
	m_pRankGold = g_pFresh->GetManager()->GetBitmap("FRAMES4", "rank_gold");
	m_pRankSilver = g_pFresh->GetManager()->GetBitmap("FRAMES4", "rank_silver");
	m_pRankBronze = g_pFresh->GetManager()->GetBitmap("FRAMES4", "rank_bronze");
	m_pRankNoAward = g_pFresh->GetManager()->GetBitmap("FRAMES4", "rank_noaward");

	m_pRedTeamRank = g_pFresh->GetManager()->GetBitmap("FRAMES4", "30in_team_redteam_rank");
	m_pBlueTeamRank = g_pFresh->GetManager()->GetBitmap("FRAMES4", "30in_team_blueteam_rank");
	m_pRank = g_pFresh->GetManager()->GetBitmap("FRAMES4", "30in_rank");

	m_pMassListIcon = g_pFresh->GetManager()->GetBitmap("30s", "pc_icon_mass_list");

	m_pZeroIcon = g_pFresh->GetManager()->GetBitmap("30s", "icon_zero");
}

// scoreboard.cpp:2391
void CRankExData::ClearRival(void) {
	if (m_pRival) { delete[] m_pRival; m_pRival = 0; }
}

// scoreboard.cpp:2396
void CRankExData::SetRivalData(void) {
	int count = WSingleton<CSharedDoc>::Instance()->m_gameResultList.size();

	m_pRival = new sRivalData[count];
	m_pRankList->ClearItem();
	for (int i = 0; i < count; i++) {
		m_pRival[i] = WSingleton<CSharedDoc>::Instance()->m_gameResultList[i];
		m_pRankList->AddItem(&m_pRival[i]);
	}
}

// scoreboard.cpp:2414
FrRankFormEx::FrRankFormEx(void) {
}

// scoreboard.cpp:2425
FrRankFormEx::~FrRankFormEx(void) {
}

// scoreboard.cpp:2432
void FrRankFormEx::OnStageAreaInit(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (!pArea) {
		return;
	}

	char bgName[64] = { 0 };
	sprintf(bgName, "30in_back_%s", WSingleton<CSharedDoc>::Instance()->m_pCurCourse ? WSingleton<CSharedDoc>::Instance()->m_pCurCourse->m_mapName : 0);
	pArea->SetBgImg(bgName);

	// the leader's pet stands on the stage area
	m_rankEx.m_pAreaStage = pArea;
	m_rankEx.CreateLeaderPet(pArea->GetRect());
}

// scoreboard.cpp:2447
void FrRankFormEx::OnStageAreaOwnerDraw(void) {
	if (IsVisible()) {
		char bgName[64] = { 0 };
		sprintf(bgName, "30in_back_%s", WSingleton<CSharedDoc>::Instance()->m_pCurCourse ? WSingleton<CSharedDoc>::Instance()->m_pCurCourse->m_mapName : 0);
		m_rankEx.m_pAreaStage->SetBgImg(bgName);

		m_rankEx.DrawStageArea();
	}
}

// scoreboard.cpp:2463
void FrRankFormEx::OnRankInit(intptr_t param) {
	m_rankEx.m_pRankList = DYNAMIC_CAST(FrListBox, param);
	m_rankEx.InitRivalData();
	m_rankEx.InitMedalData();
}

// scoreboard.cpp:2513
sRivalData* FrRankFormEx::GetRival(void) {
	return m_rankEx.m_pRival;
}

// scoreboard.cpp:2517
void FrRankFormEx::OnRankOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0) {
		return;
	}
	m_rankEx.DrawRankItem(pItem);
}

// scoreboard.cpp:2526
void FrRankFormEx::OnBoardInit(intptr_t param) {
	m_rankEx.m_pBoard = DYNAMIC_CAST(FrStatic, param);
	m_rankEx.m_pBoard->m_nFlags.Enable(0x1000);
}

// scoreboard.cpp:2532
void FrRankFormEx::Clear(void) {
	m_rankEx.Clear();
}

// scoreboard.cpp:2537
void FrRankFormEx::SetActive(bool bActive) {
	m_rankEx.SetActive(bActive);
	Enable(bActive);
	SetVisible(bActive);
}

// scoreboard.cpp:2545
void FrRankFormEx::SetLeader(void) {
	m_rankEx.SetLeader();
}

// scoreboard.cpp:2550
void FrRankFormEx::ShowLeader(void) {
	m_rankEx.ShowLeader();
}

// scoreboard.cpp:2555
bool FrRankFormEx::OnInit(void) {
	Enable(false);
	SetVisible(false);
	return true;
}

// scoreboard.cpp:2562
void FrRankFormEx::OnProc(const float dt) {
	if (!m_dwStyle.GetFlag(0x1)) {
		return;
	}

	m_rankEx.OnProc(dt);
}

// scoreboard.cpp:2570
bool FrRankFormEx::IsMotionFinished(void) {
	return m_rankEx.IsMotionFinished();
}

// scoreboard.cpp:2575
void FrRankFormEx::ResetStage(void) {
	m_rankEx.ResetStage();
}

// scoreboard.cpp:2582
// Fills slots (the player index of each row of the award list, best first)
// by the room type.
static void __stdcall SortAwardSlots(int* slots) {
	unsigned char roomType = WSingleton<CSharedDoc>::Instance()->m_roomType;
	if (roomType == 1) {
		if (GOLFDOC()->GetTeam(0)->m_resetCount > GOLFDOC()->GetTeam(1)->m_resetCount
			|| (GOLFDOC()->GetTeam(0)->m_resetCount == GOLFDOC()->GetTeam(1)->m_resetCount
				&& (int)GOLFDOC()->GetTeam(0)->m_teamValue >= (int)GOLFDOC()->GetTeam(1)->m_teamValue)) {
			slots[0] = 0;
			slots[1] = 1;
		} else {
			slots[0] = 1;
			slots[1] = 0;
		}
	} else if (roomType == 3) {
		if (GOLFDOC()->GetPlayer(0)->m_state == 3) {
			slots[0] = 1;
			slots[1] = 0;
		} else if (PLAYER(1)->m_state == 3
			|| GOLFDOC()->GetPlayer(0)->m_ghostResult > GOLFDOC()->GetPlayer(1)->m_ghostResult
			|| (PLAYER(0)->m_ghostResult == PLAYER(1)->m_ghostResult
				&& (int)PLAYER(0)->m_ghostAward >= (int)PLAYER(1)->m_ghostAward)) {
			slots[0] = 0;
			slots[1] = 1;
		} else {
			slots[0] = 1;
			slots[1] = 0;
		}
	} else if (roomType == 7) {
		int n = 0;
		for (int rank = 0; rank < GOLFDOC()->GetPlayerNum(); rank++) {
			for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
				if (GOLFDOC()->GetPlayer(i)->m_state != 3 && GOLFDOC()->GetPlayer(i)->m_joinState == rank + 1) {
					slots[n++] = i;
				}
			}
		}
		for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
			if (GOLFDOC()->GetPlayer(i)->m_state == 3) {
				slots[n++] = i;
			}
		}
	} else if (roomType == 6) {
		slots[0] = 0;
		slots[1] = 1;
	} else {
		for (unsigned char i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
			if (GOLFDOC()->GetPlayer(i)->m_state == 3
				&& (WSingleton<CSharedDoc>::Instance()->m_gameMode == 1 || WSingleton<CSharedDoc>::Instance()->m_gameMode == 5)) {
				continue;
			}
			slots[GOLFDOC()->GetPlayer(i)->m_turnOrder - 1] = i;
		}
	}
}

// scoreboard.cpp:2672 (run-time type and factory registration of FrAwardForm)
const WRTTI FrAwardForm::m_RTTI("FrAwardForm", &FrForm::m_RTTI);

IObject* FrAwardFormMakeInstance(void) {
	return new FrAwardForm;
}

struct __sFrAwardForm {
	__sFrAwardForm(void) {
		ObjectFactory().AddObjectFunctor(FrAwardFormMakeInstance, "FrAwardForm");
	}
};
static __sFrAwardForm s_registerFrAwardForm;

// scoreboard.cpp:2674
const sFRESH_MSGMAP* FrAwardForm::GetMessageMap(void) const {
	return &FrAwardForm::_MsgMap;
}

sFRESH_MSGMAP FrAwardForm::_MsgMap = { &FrForm::_MsgMap, &FrAwardForm::_MsgEntries[0] };

sFRESH_ENTRY FrAwardForm::_MsgEntries[] = {
	// scoreboard.cpp:2676
	{ "tab0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnTab0Init },
	// scoreboard.cpp:2677
	{ "tab1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnTab1Init },
	// scoreboard.cpp:2678
	{ "userlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnUserListInit },
	// scoreboard.cpp:2679
	{ "userlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnUserListOwnerDraw },
	// scoreboard.cpp:2680
	{ "expbar0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExpBar0Init },
	// scoreboard.cpp:2681
	{ "expbar1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExpBar1Init },
	// scoreboard.cpp:2682
	{ "expbar2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExpBar2Init },
	// scoreboard.cpp:2683
	{ "expbar3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExpBar3Init },
	// scoreboard.cpp:2684
	{ "exp0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExp0Init },
	// scoreboard.cpp:2685
	{ "exp1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExp1Init },
	// scoreboard.cpp:2686
	{ "exp2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExp2Init },
	// scoreboard.cpp:2687
	{ "exp3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExp3Init },
	// scoreboard.cpp:2688
	{ "expend0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExpEnd0Init },
	// scoreboard.cpp:2689
	{ "expend1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExpEnd1Init },
	// scoreboard.cpp:2690
	{ "expend2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExpEnd2Init },
	// scoreboard.cpp:2691
	{ "expend3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnExpEnd3Init },
	// scoreboard.cpp:2692
	{ "level0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnLevel0Init },
	// scoreboard.cpp:2693
	{ "level1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnLevel1Init },
	// scoreboard.cpp:2694
	{ "level2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnLevel2Init },
	// scoreboard.cpp:2695
	{ "level3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnLevel3Init },
	// scoreboard.cpp:2696
	{ "levelup0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnLevelUp0Init },
	// scoreboard.cpp:2697
	{ "levelup1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnLevelUp1Init },
	// scoreboard.cpp:2698
	{ "levelup2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnLevelUp2Init },
	// scoreboard.cpp:2699
	{ "levelup3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnLevelUp3Init },
	// scoreboard.cpp:2700
	{ "match_res0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnMatchRes0Init },
	// scoreboard.cpp:2701
	{ "match_res1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnMatchRes1Init },
	// scoreboard.cpp:2702
	{ "GainedLadderPoint0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnGainedLadderPoint0Init },
	// scoreboard.cpp:2703
	{ "GainedLadderPoint1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnGainedLadderPoint1Init },
	// scoreboard.cpp:2704
	{ "LadderPoint0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnLadderPoint0Init },
	// scoreboard.cpp:2705
	{ "LadderPoint1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnLadderPoint1Init },
	// scoreboard.cpp:2706
	{ "nextlevel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrAwardForm::OnNextLevelTextInit },

	// scoreboard.cpp:2708
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// scoreboard.cpp:2712
FrAwardForm::FrAwardForm(void) {
	m_sfx = 0;
	m_expTick = 0.0f;
	m_pUserList = 0;
	m_numRows = 0;
	memset(m_pExpBar, 0, sizeof(m_pExpBar));
	memset(m_pExp, 0, sizeof(m_pExp));
	memset(m_pExpEnd, 0, sizeof(m_pExpEnd));
	memset(m_pLevel, 0, sizeof(m_pLevel));
	memset(m_pLevelUp, 0, sizeof(m_pLevelUp));
	memset(m_pMatchRes, 0, sizeof(m_pMatchRes));

	memset(m_slotPlayer, 0, sizeof(m_slotPlayer));
	memset(m_level, 0, sizeof(m_level));
	memset(m_gainExp, 0, sizeof(m_gainExp));
	memset(m_exp, 0, sizeof(m_exp));
	memset(m_expDone, 0, sizeof(m_expDone));

	memset(m_pGainedLadderPoint, 0, sizeof(m_pGainedLadderPoint));
	memset(m_pLadderPoint, 0, sizeof(m_pLadderPoint));
	m_pNextLevelText = 0;
	m_elapsed = 0.0f;

	m_pMascotBonus01 = 0;
	m_pMascotBonus02 = 0;
	m_pMascotBonus09 = 0;
	m_pMascotBonus10 = 0;
	m_pMascotBonus11 = 0;
	m_pMascotBonus12 = 0;
}

// scoreboard.cpp:2740
FrAwardForm::~FrAwardForm(void) {
}

// scoreboard.cpp:2744
bool FrAwardForm::Close(bool bDestroy) {
	if (m_sfx) {
		g_audio->StopSfx(m_sfx);
	}
	return FrForm::Close(bDestroy);
}

// scoreboard.cpp:2752
void FrAwardForm::OnUserListInit(intptr_t param) {
	m_pUserList = DYNAMIC_CAST(FrListBox, param);

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1) {
		m_numRows = GOLFDOC()->GetPlayerNum();
		unsigned char team = GOLFDOC()->GetTeamGameWinner() == 2 ? 0 : (unsigned char)GOLFDOC()->GetTeamGameWinner();
		for (int i = 0; i < m_numRows; i++) {
			if (GOLFDOC()->GetPlayer(i)->m_partnerIndex == 0xFF) {
				if (GOLFDOC()->GetPlayer(i)->m_attribute == team) {
					m_slotPlayer[0] = i;
					GOLFDOC()->GetPlayer(i)->m_joinState = 1;
					GOLFDOC()->GetPlayer(i)->m_turnOrder = 1;
				} else {
					m_slotPlayer[1] = i;
					GOLFDOC()->GetPlayer(i)->m_joinState = 2;
					GOLFDOC()->GetPlayer(i)->m_turnOrder = 2;
				}
			} else if (GOLFDOC()->GetPlayer(i)->m_attribute == team) {
				m_slotPlayer[i >= GOLFDOC()->GetPlayer(i)->m_partnerIndex] = i;
				GOLFDOC()->GetPlayer(i)->m_joinState = (i >= GOLFDOC()->GetPlayer(i)->m_partnerIndex) + 1;
				GOLFDOC()->GetPlayer(i)->m_turnOrder = GOLFDOC()->GetPlayer(i)->m_joinState;
			} else {
				m_slotPlayer[(i >= GOLFDOC()->GetPlayer(i)->m_partnerIndex) + 2] = i;
				GOLFDOC()->GetPlayer(i)->m_joinState = (i >= GOLFDOC()->GetPlayer(i)->m_partnerIndex) + 3;
				GOLFDOC()->GetPlayer(i)->m_turnOrder = GOLFDOC()->GetPlayer(i)->m_joinState;
			}
		}
	} else {
		m_numRows = GOLFDOC()->GetPlayerNum();
		SortAwardSlots(m_slotPlayer);
	}

	for (int i = 0; i < m_numRows; i++) {
		m_pUserList->AddItem((void*)m_slotPlayer[i]);
	}
}

// scoreboard.cpp:3054
void FrAwardForm::OnTab0Init(int param) {
	if ((__rtti_obj = (void*)param) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&FrArea::m_RTTI)) {
	}
}

// scoreboard.cpp:3061
void FrAwardForm::OnTab1Init(int param) {
	if ((__rtti_obj = (void*)param) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&FrArea::m_RTTI)) {
	}
}

// scoreboard.cpp:3068
void FrAwardForm::OnExpBar0Init(intptr_t param) {
	m_pExpBar[0] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pExpBar[0]->SetVisible(false);
}

// scoreboard.cpp:3074
void FrAwardForm::OnExpBar1Init(intptr_t param) {
	m_pExpBar[1] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pExpBar[1]->SetVisible(false);
}

// scoreboard.cpp:3080
void FrAwardForm::OnExpBar2Init(intptr_t param) {
	m_pExpBar[2] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pExpBar[2]->SetVisible(false);
}

// scoreboard.cpp:3086
void FrAwardForm::OnExpBar3Init(intptr_t param) {
	m_pExpBar[3] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pExpBar[3]->SetVisible(false);
}

// scoreboard.cpp:3092
void FrAwardForm::OnExp0Init(intptr_t param) {
	m_pExp[0] = DYNAMIC_CAST(FrStatic, param);
	m_pExp[0]->SetVisible(false);
}

// scoreboard.cpp:3098
void FrAwardForm::OnExp1Init(intptr_t param) {
	m_pExp[1] = DYNAMIC_CAST(FrStatic, param);
	m_pExp[1]->SetVisible(false);
}

// scoreboard.cpp:3104
void FrAwardForm::OnExp2Init(intptr_t param) {
	m_pExp[2] = DYNAMIC_CAST(FrStatic, param);
	m_pExp[2]->SetVisible(false);
}

// scoreboard.cpp:3110
void FrAwardForm::OnExp3Init(intptr_t param) {
	m_pExp[3] = DYNAMIC_CAST(FrStatic, param);
	m_pExp[3]->SetVisible(false);
}

// scoreboard.cpp:3116
void FrAwardForm::OnExpEnd0Init(intptr_t param) {
	m_pExpEnd[0] = DYNAMIC_CAST(FrStatic, param);
	m_pExpEnd[0]->SetVisible(false);
}

// scoreboard.cpp:3122
void FrAwardForm::OnExpEnd1Init(intptr_t param) {
	m_pExpEnd[1] = DYNAMIC_CAST(FrStatic, param);
	m_pExpEnd[1]->SetVisible(false);
}

// scoreboard.cpp:3128
void FrAwardForm::OnExpEnd2Init(intptr_t param) {
	m_pExpEnd[2] = DYNAMIC_CAST(FrStatic, param);
	m_pExpEnd[2]->SetVisible(false);
}

// scoreboard.cpp:3134
void FrAwardForm::OnExpEnd3Init(intptr_t param) {
	m_pExpEnd[3] = DYNAMIC_CAST(FrStatic, param);
	m_pExpEnd[3]->SetVisible(false);
}

// scoreboard.cpp:3140
void FrAwardForm::OnLevel0Init(intptr_t param) {
	m_pLevel[0] = DYNAMIC_CAST(FrArea, param);
	m_pLevel[0]->SetVisible(false);
}

// scoreboard.cpp:3146
void FrAwardForm::OnLevel1Init(intptr_t param) {
	m_pLevel[1] = DYNAMIC_CAST(FrArea, param);
	m_pLevel[1]->SetVisible(false);
}

// scoreboard.cpp:3152
void FrAwardForm::OnLevel2Init(intptr_t param) {
	m_pLevel[2] = DYNAMIC_CAST(FrArea, param);
	m_pLevel[2]->SetVisible(false);
}

// scoreboard.cpp:3158
void FrAwardForm::OnLevel3Init(intptr_t param) {
	m_pLevel[3] = DYNAMIC_CAST(FrArea, param);
	m_pLevel[3]->SetVisible(false);
}

// scoreboard.cpp:3164
void FrAwardForm::OnLevelUp0Init(intptr_t param) {
	m_pLevelUp[0] = DYNAMIC_CAST(FrArea, param);
	m_pLevelUp[0]->SetVisible(false);
}

// scoreboard.cpp:3170
void FrAwardForm::OnLevelUp1Init(intptr_t param) {
	m_pLevelUp[1] = DYNAMIC_CAST(FrArea, param);
	m_pLevelUp[1]->SetVisible(false);
}

// scoreboard.cpp:3176
void FrAwardForm::OnLevelUp2Init(intptr_t param) {
	m_pLevelUp[2] = DYNAMIC_CAST(FrArea, param);
	m_pLevelUp[2]->SetVisible(false);
}

// scoreboard.cpp:3182
void FrAwardForm::OnLevelUp3Init(intptr_t param) {
	m_pLevelUp[3] = DYNAMIC_CAST(FrArea, param);
	m_pLevelUp[3]->SetVisible(false);
}

// scoreboard.cpp:3188
void FrAwardForm::OnMatchRes0Init(intptr_t param) {
	m_pMatchRes[0] = DYNAMIC_CAST(FrArea, param);

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 0) {
		m_pMatchRes[0]->SetVisible(false);
	}
}

// scoreboard.cpp:3196
void FrAwardForm::OnMatchRes1Init(intptr_t param) {
	m_pMatchRes[1] = DYNAMIC_CAST(FrArea, param);

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 0) {
		m_pMatchRes[1]->SetVisible(false);
	}
}

// scoreboard.cpp:3205
void FrAwardForm::OnGainedLadderPoint0Init(intptr_t param) {
	m_pGainedLadderPoint[0] = DYNAMIC_CAST(FrStatic, param);
}

// scoreboard.cpp:3211
void FrAwardForm::OnGainedLadderPoint1Init(intptr_t param) {
	m_pGainedLadderPoint[1] = DYNAMIC_CAST(FrStatic, param);
}

// scoreboard.cpp:3217
void FrAwardForm::OnLadderPoint0Init(intptr_t param) {
	m_pLadderPoint[0] = DYNAMIC_CAST(FrStatic, param);
}

// scoreboard.cpp:3223
void FrAwardForm::OnLadderPoint1Init(intptr_t param) {
	m_pLadderPoint[1] = DYNAMIC_CAST(FrStatic, param);
}

// scoreboard.cpp:3228
void FrAwardForm::OnNextLevelTextInit(int param) {
	if (!m_pNextLevelText) {
		m_pNextLevelText = DYNAMIC_CAST(FrEdit, param);
	}
}

// scoreboard.cpp:3236
bool FrAwardForm::OnInit(void) {
	FrForm::OnInit();

	if (m_pUserList) {
		m_pUserList->EnableScrollBar(false);

		WRect rect = m_pUserList->GetRect();
		float height = (float)(m_pUserList->GetItemHeight() * m_numRows);
		float grow = height - rect.h;
		rect.h = height;
		m_pUserList->SetRect(rect);

		m_rect.h += grow;
		MoveWindow(WPoint(m_rect.x, g_view->GetHeight() - m_rect.h + 5.0f));
		SetRect(m_rect);

		float textHeight = m_pNextLevelText->GetRect().h;
		m_pNextLevelText->MoveWindow(WPoint(m_rect.x + 25.0f, g_view->GetHeight() - textHeight));
	}

	unsigned char gameType = WSingleton<CSharedDoc>::Instance()->m_roomType;
	if (gameType == 0) {
		CalcResult();
	} else if (gameType == 1) {
		// Team game: each row moves up or down by its team.
		float offset = 0.0f;
		if (m_numRows < 4)
			offset = -3.0f;

		for (int i = 0; i < 4; i++) {
			bool upper;
			if (GOLFDOC()->GetPlayer((unsigned char)m_slotPlayer[i])->m_partnerIndex != 0xFF)
				upper = m_slotPlayer[i] < GOLFDOC()->GetPlayer((unsigned char)m_slotPlayer[i])->m_partnerIndex;
			else
				upper = true;

			const WRect& barRect = m_pExpBar[i]->GetRect();
			m_pExpBar[i]->MoveWindow(WPoint(barRect.x, barRect.y + offset + (upper ? 3.0f : -3.0f)));

			const WRect& expRect = m_pExp[i]->GetRect();
			m_pExp[i]->MoveWindow(WPoint(expRect.x, expRect.y + offset + (upper ? 3.0f : -3.0f)));

			const WRect& expEndRect = m_pExpEnd[i]->GetRect();
			m_pExpEnd[i]->MoveWindow(WPoint(expEndRect.x, expEndRect.y + offset + (upper ? 3.0f : -3.0f)));

			const WRect& levelRect = m_pLevel[i]->GetRect();
			m_pLevel[i]->MoveWindow(WPoint(levelRect.x, levelRect.y + offset + (upper ? 3.0f : -3.0f)));

			const WRect& levelUpRect = m_pLevelUp[i]->GetRect();
			m_pLevelUp[i]->MoveWindow(WPoint(levelUpRect.x, levelUpRect.y + offset + (upper ? 3.0f : -3.0f)));
		}

		if (GOLFDOC()->GetTeamGameWinner() == 2) {
			m_pMatchRes[0]->SetBgImg("draw");
			m_pMatchRes[1]->SetBgImg("draw");
		} else {
			m_pMatchRes[0]->SetBgImg("win");
			m_pMatchRes[1]->SetBgImg("lose");
		}

		if (m_numRows < 4) {
			for (int i = 0; i < 2; i++) {
				const WRect& resRect = m_pMatchRes[i]->GetRect();
				m_pMatchRes[i]->MoveWindow(WPoint(resRect.x, resRect.y + (i == 0 ? -18.0f : -43.0f)));
			}
		}

		CalcResult();
	} else if (gameType == 7) {
		if (GOLFDOC()->GetPlayer(0)->m_joinState == GOLFDOC()->GetPlayer(1)->m_joinState) {
			m_pMatchRes[0]->SetBgImg("draw");
			m_pMatchRes[1]->SetBgImg("draw");

			const WRect& resRect = m_pMatchRes[0]->GetRect();
			m_pMatchRes[0]->MoveWindow(WPoint(resRect.x, resRect.y - 18.0f));
			m_pMatchRes[1]->SetVisible(false);
		} else {
			m_pMatchRes[0]->SetBgImg("win");
			m_pMatchRes[1]->SetBgImg("lose");

			for (int i = 0; i < 2; i++) {
				const WRect& resRect = m_pMatchRes[i]->GetRect();
				m_pMatchRes[i]->MoveWindow(WPoint(resRect.x, resRect.y + (i == 0 ? -18.0f : -43.0f)));
			}
		}

		CalcResult();
	} else if (gameType == 3) {
		if (GOLFDOC()->GetPlayer(0)->m_joinState == GOLFDOC()->GetPlayer(1)->m_joinState) {
			m_pMatchRes[0]->SetBgImg("draw");
			m_pMatchRes[1]->SetBgImg("draw");
		} else {
			m_pMatchRes[0]->SetBgImg("win");
			m_pMatchRes[1]->SetBgImg("lose");
		}

		for (int i = 0; i < 2; i++) {
			const WRect& resRect = m_pMatchRes[i]->GetRect();
			m_pMatchRes[i]->MoveWindow(WPoint(resRect.x, resRect.y + (i == 0 ? -18.0f : -43.0f)));
		}

		if (GOLFDOC()->GetPlayer(0)->m_state == 3 || GOLFDOC()->GetPlayer(1)->m_state == 3)
			m_pMatchRes[1]->SetVisible(false);

		CalcResultMatch();
	}

	m_pVsIcon = g_pFresh->GetManager()->GetBitmap("golf", "pc_icon_vs");
	m_pBonus20Icon = g_pFresh->GetManager()->GetBitmap("golf", "pc_icon_bonus_20");

	m_pMascotBonus01 = g_pFresh->GetManager()->GetBitmap("golf", "bonus_mascot_01");
	m_pMascotBonus02 = g_pFresh->GetManager()->GetBitmap("golf", "bonus_mascot_02");

	m_pMascotBonus09 = g_pFresh->GetManager()->GetBitmap("golf", "bonus_mascot_09");
	m_pMascotBonus10 = g_pFresh->GetManager()->GetBitmap("golf", "bonus_mascot_10");
	m_pMascotBonus11 = g_pFresh->GetManager()->GetBitmap("golf", "bonus_mascot_11");
	m_pMascotBonus12 = g_pFresh->GetManager()->GetBitmap("golf", "bonus_mascot_12");

	m_pBonusMa = g_pFresh->GetManager()->GetBitmap("golf", "bonus_ma");
	m_pBonusMa2 = g_pFresh->GetManager()->GetBitmap("golf", "bonus_ma2");
	m_pZeroVsIcon = g_pFresh->GetManager()->GetBitmap("golf", "icon_zero_vs");

	m_pNextLevelText->SetVisible(IsLocalContent((localContentType_t)0x29));

	// The next level's name for the local player, unless the round's exp
	// already passes it.
	for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		if (GOLFDOC()->GetPlayer((unsigned char)i)->m_uid == WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid) {
			// TODO: the round's gained exp is a short at CGolfDoc 0x32F + i * 0x21
			// (inside the private m_zeroedBlock328: four 0x21-byte records); it
			// needs a typed member in GolfDoc.h.
			int roundExp = 0;
			unsigned long exp = roundExp + WSingleton<CSharedDoc>::Instance()->m_roomPlayers[i].m_value155;

			if (exp > WSingleton<CSharedDoc>::Instance()->m_levelExp[m_level[i]].m_exp) {
				m_pNextLevelText->SetVisible(false);
			} else {
				std::string text;

				if (m_level[i] < 21) {
					text = data_a81a50[m_level[i]].m_name;
				} else if (m_level[i] <= 65) {
					text = data_9f19c4[m_level[i]];
					text += data_a81a50[21].m_name;
				}

				m_pNextLevelText->AddLine(text.c_str(), 0, false);
			}
		}
	}

	// The result voice of the local player's character.
	int result = 0;   // 0 won, 1 draw, 2 lost
	unsigned char roomType = WSingleton<CSharedDoc>::Instance()->m_roomType;
	if (roomType == 1) {
		unsigned char me = GalleryMode() ? GOLFDOC()->m_myPlayerIndex : WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid);
		if (GOLFDOC()->GetTeam(GOLFDOC()->GetPlayer(me)->m_attribute)->m_resetCount < GOLFDOC()->GetTeam(GOLFDOC()->GetPlayer(MyIndex())->m_attribute == 0)->m_resetCount)
			result = 2;
		if (GOLFDOC()->GetTeam(GOLFDOC()->GetPlayer(MyIndex())->m_attribute)->m_resetCount == GOLFDOC()->GetTeam(GOLFDOC()->GetPlayer(MyIndex())->m_attribute == 0)->m_resetCount)
			result = 1;
	} else if (roomType == 0) {
		unsigned char me = GalleryMode() ? GOLFDOC()->m_myPlayerIndex : WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid);
		int myScore = (int)GOLFDOC()->GetPlayer(me)->m_ghostResult;
		int myPang = GOLFDOC()->GetPlayer(MyIndex())->m_ghostAward;

		int best = 0;
		for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
			if (i == MyIndex())
				continue;

			if ((int)GOLFDOC()->GetPlayer((unsigned char)i)->m_ghostResult < (int)GOLFDOC()->GetPlayer((unsigned char)best)->m_ghostResult
				|| (GOLFDOC()->GetPlayer((unsigned char)i)->m_ghostResult == GOLFDOC()->GetPlayer((unsigned char)best)->m_ghostResult
					&& (int)GOLFDOC()->GetPlayer((unsigned char)i)->m_ghostAward > (int)GOLFDOC()->GetPlayer((unsigned char)best)->m_ghostAward))
				best = i;

			if ((int)GOLFDOC()->GetPlayer((unsigned char)i)->m_ghostResult < myScore
				|| ((int)GOLFDOC()->GetPlayer((unsigned char)i)->m_ghostResult == myScore
					&& (int)GOLFDOC()->GetPlayer((unsigned char)i)->m_ghostAward > myPang))
				result = 2;
		}

		if (result != 0) {
			if ((int)GOLFDOC()->GetPlayer((unsigned char)best)->m_ghostResult == myScore
				&& (int)GOLFDOC()->GetPlayer((unsigned char)best)->m_ghostAward == myPang)
				result = 1;
		}
	} else if (roomType == 7) {
		// TODO: CGolfDoc 0x190 (the private m_selectedIndexB) holds the
		// winner's UID here, 0 for a draw; GolfDoc.h must expose it.
		unsigned long winner = 0;
		if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid != winner)
			result = 2;
		else if (winner == 0)
			result = 1;
	}

	int waveState;
	switch (result) {
	case 0:
		waveState = 0x40;
		break;
	case 2:
		waveState = 0x200;
		break;
	default:
		return true;
	}

	WSingleton<CVoiceItem>::Instance()->PlaySound(
		WSingleton<CSharedDoc>::Instance()->m_roomPlayers[MyIndex()].m_clubSetTypeId,
		(CVoiceItem::eWaveState)waveState,
		WSingleton<CSharedDoc>::Instance()->m_roomPlayers[MyIndex()].m_characterInfo.typeId & 0x3FFFFFF);

	return true;
}

// scoreboard.cpp:3537
void FrAwardForm::OnProc(const float deltaTime) {
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 3)
		return;

	m_expTick += deltaTime;
	if (m_expTick < 1.0f)
		return;

	if (!m_sfx && !m_expDone[MyIndex()]) {
		if (m_gainExp[MyIndex()] > 0 || m_gainExp[MyIndex()] < 0)
			m_sfx = "player_exp_up";

		if (m_sfx && !g_audio->IsPlaying(m_sfx))
			g_audio->PlaySfx(m_sfx, 0, 1, m_sfx, 0, 0.5f, 200.0f);
	}

	if (m_expTick - 1.0f < 0.01f)
		return;

	// The exp counts one tenth per 10 ms.
	int steps = (int)((m_expTick - 1.0f) * 100.0f);
	m_expTick -= steps * 0.01f;

	for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		if (m_expDone[i])
			continue;
		if (WSingleton<CSharedDoc>::Instance()->m_roomType != 1 && GOLFDOC()->GetPlayer((unsigned char)i)->m_state == 3)
			continue;

		int levelExp = WSingleton<CSharedDoc>::Instance()->m_levelExp[m_level[i]].m_exp * 10;

		if (steps > abs(m_gainExp[i])) {
			m_expDone[i] = 1;
			steps = abs(m_gainExp[i]);
			int sign = (m_gainExp[i] > 0) * 2 - 1;
			m_gainExp[i] = 0;
			m_exp[i] += sign * steps;
		} else {
			int delta = ((m_gainExp[i] > 0) * 2 - 1) * steps;
			m_exp[i] += delta;
			m_gainExp[i] -= delta;
		}

		if (m_exp[i] < 0 && !m_expDone[i]) {
			m_expDone[i] = 1;
			m_exp[i] = 0;
		}

		int row = GOLFDOC()->GetPlayer((unsigned char)i)->m_turnOrder - 1;
		if (row < 0 || row >= m_numRows)
			continue;

		char buf[32];
		if (m_exp[i] >= levelExp) {
			g_audio->PlaySfx("level_up", 0, 0, 0, 0, 0.5f, 200.0f);

			m_exp[i] -= levelExp;
			m_level[i]++;

			if (m_pExpBar[row]) {
				m_pExpBar[row]->SetRange(0, WSingleton<CSharedDoc>::Instance()->m_levelExp[m_level[i]].m_exp * 10, 0);
				m_pExpBar[row]->SetPos(m_exp[i]);
			}

			if (m_pExpEnd[row]) {
				sprintf(buf, "/ %d", WSingleton<CSharedDoc>::Instance()->m_levelExp[m_level[i]].m_exp);
				m_pExpEnd[row]->SetCaption(buf);
			}

			if (m_pLevel[row]) {
				sprintf(buf, "level_%03d", m_level[i] + 1);
				m_pLevel[row]->SetBgImg(buf);
			}

			AfxGetTask()->GetActor("Player") << MsgObject(0, 0x1F8, i, 0, 0, 0, 0);
		} else if (m_pExpBar[row]) {
			m_pExpBar[row]->SetPos(m_exp[i]);
		}

		sprintf(buf, "%d", m_exp[i] / 10);
		m_pExp[row]->SetCaption(buf);

		if (m_expDone[i]) {
			if (m_level[i] != WSingleton<CSharedDoc>::Instance()->m_roomPlayers[i].m_level) {
				if (m_pExp[row])
					m_pExp[row]->SetVisible(false);
				if (m_pExpEnd[row])
					m_pExpEnd[row]->SetVisible(false);
				if (m_pLevelUp[row])
					m_pLevelUp[row]->SetVisible(true);

				std::list<sRoomPlayer>& players = WSingleton<CSharedDoc>::Instance()->m_roomPlayerList;
				for (std::list<sRoomPlayer>::iterator it = players.begin(); it != players.end(); ++it) {
					if (it->m_uid == WSingleton<CSharedDoc>::Instance()->m_roomPlayers[i].m_guid) {
						it->m_level = (unsigned char)m_level[i];
						break;
					}
				}

				// The head icons show the new levels; the room keeps its own.
				if (WSingleton<CHeadIcon>::Instance()) {
					unsigned char savedLevels[4];
					for (int j = 0; j < GOLFDOC()->GetPlayerNum(); j++) {
						savedLevels[j] = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[j].m_level;
						WSingleton<CSharedDoc>::Instance()->m_roomPlayers[j].m_level = (unsigned char)m_level[j];
					}

					WSingleton<CHeadIcon>::Instance()->SetLevel();

					for (int j = 0; j < GOLFDOC()->GetPlayerNum(); j++)
						WSingleton<CSharedDoc>::Instance()->m_roomPlayers[j].m_level = savedLevels[j];
				}
			}

			if (i == MyIndex() && m_sfx)
				g_audio->StopSfx(m_sfx);
		}
	}

	m_elapsed += deltaTime;
	if (WSingleton<CTHunter>::Instance()->IsReceiveGiftPacket() && m_elapsed > 2.0f)
		WSingleton<CSharedDoc>::Instance()->m_value4844 = 1;
}

// scoreboard.cpp:4106
void cAquireItem::Reset(void) {
	m_value = 0;
}

// scoreboard.cpp:4111
void cAquireItem::OnProc(float dt) {
	if (m_value < m_maxValue) {
		m_value += dt * 20.0f;
	}
}

// scoreboard.cpp:4258
WVector cAquireItem::GetPlayer2DPos(int player) {
	WMatrix mat;
	AfxGetTask()->GetActor("Player") << MsgObject(0, 0x124, player, (intptr_t)&mat, 0, 0, 0);

	WVector pos = g_view->Projection(mat.pivot);
	return WVector(pos.x, pos.y, pos.z);
}

// scoreboard.cpp:4273 (run-time type and factory registration of FrScoreForm)
const WRTTI FrScoreForm::m_RTTI("FrScoreForm", &FrForm::m_RTTI);

IObject* FrScoreFormMakeInstance(void) {
	return new FrScoreForm;
}

struct __sFrScoreForm {
	__sFrScoreForm(void) {
		ObjectFactory().AddObjectFunctor(FrScoreFormMakeInstance, "FrScoreForm");
	}
};
static __sFrScoreForm s_registerFrScoreForm;

// scoreboard.cpp:4275
const sFRESH_MSGMAP* FrScoreForm::GetMessageMap(void) const {
	return &FrScoreForm::_MsgMap;
}

sFRESH_MSGMAP FrScoreForm::_MsgMap = { &FrForm::_MsgMap, &FrScoreForm::_MsgEntries[0] };

sFRESH_ENTRY FrScoreForm::_MsgEntries[] = {
	// scoreboard.cpp:4278
	{ "bar2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnBar2Init },
	// scoreboard.cpp:4279
	{ "bar3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnBar3Init },
	// scoreboard.cpp:4280
	{ "front9btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnFront9BtnInit },
	// scoreboard.cpp:4281
	{ "front9btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreForm::OnFront9BtnUp },
	// scoreboard.cpp:4282
	{ "back9btn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnBack9BtnInit },
	// scoreboard.cpp:4283
	{ "back9btn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreForm::OnBack9BtnUp },
	// scoreboard.cpp:4284
	{ "shotbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnShotBtnInit },
	// scoreboard.cpp:4285
	{ "shotbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreForm::OnShotBtnUp },
	// scoreboard.cpp:4286
	{ "graphbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnGraphBtnInit },
	// scoreboard.cpp:4287
	{ "graphbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreForm::OnGraphBtnUp },
	// scoreboard.cpp:4288
	{ "versusbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnVersusBtnInit },
	// scoreboard.cpp:4289
	{ "versusbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreForm::OnVersusBtnUp },
	// scoreboard.cpp:4290
	{ "puttbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnPuttBtnInit },
	// scoreboard.cpp:4291
	{ "puttbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreForm::OnPuttBtnUp },
	// scoreboard.cpp:4292
	{ "pangbtn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnPangBtnInit },
	// scoreboard.cpp:4293
	{ "pangbtn", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreForm::OnPangBtnUp },
	// scoreboard.cpp:4294
	{ "hole_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnHoleListInit },
	// scoreboard.cpp:4295
	{ "hole_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnHoleListOwnerDraw },
	// scoreboard.cpp:4296
	{ "par_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnParListInit },
	// scoreboard.cpp:4297
	{ "par_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnParListOwnerDraw },
	// scoreboard.cpp:4298
	{ "pang_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnPangListInit },
	// scoreboard.cpp:4299
	{ "pang_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnPangListOwnerDraw },
	// scoreboard.cpp:4300
	{ "golfer_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnGolferListInit },
	// scoreboard.cpp:4301
	{ "golfer_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnGolferListOwnerDraw },
	// scoreboard.cpp:4302
	{ "record_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnRecordListInit },
	// scoreboard.cpp:4303
	{ "record_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnRecordListOwnerDraw },
	// scoreboard.cpp:4304
	{ "total_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnTotalListInit },
	// scoreboard.cpp:4305
	{ "total_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnTotalListOwnerDraw },
	// scoreboard.cpp:4306
	{ "total", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnTotalInit },
	// scoreboard.cpp:4307
	{ "ppl", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnPPLInit },
	// scoreboard.cpp:4308
	{ "board1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnBoard1Init },
	// scoreboard.cpp:4309
	{ "board2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnBoard2Init },
	// scoreboard.cpp:4310
	{ "drawlines", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnDrawLinesInit },
	// scoreboard.cpp:4311
	{ "drawlines", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnDrawLinesOwnerDraw },
	// scoreboard.cpp:4312
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnCancelBtnInit },
	// scoreboard.cpp:4313
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreForm::OnCancelBtnUp },
	// scoreboard.cpp:4314
	{ "close", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnCloseBtnInit },
	// scoreboard.cpp:4315
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrScoreForm::OnCloseBtnUp },
	// scoreboard.cpp:4317
	{ "blankpanel1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnBlankPanel01Init },
	// scoreboard.cpp:4318
	{ "blankpanel1", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnBlankPanel01OwnerDraw },
	// scoreboard.cpp:4319
	{ "blankpanel2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnBlankPanel02Init },
	// scoreboard.cpp:4320
	{ "blankpanel2", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnBlankPanel02OwnerDraw },
	// scoreboard.cpp:4321
	{ "blankpanel3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnBlankPanel03Init },
	// scoreboard.cpp:4322
	{ "blankpanel3", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScoreForm::OnBlankPanel03OwnerDraw },

	// scoreboard.cpp:4325
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// scoreboard.cpp:4330
FrScoreForm::FrScoreForm(void) {
	m_state = STATE_NORMAL;

	m_pFront9Btn = 0;
	m_pBack9Btn = 0;
	m_pHoleList = 0;
	m_pParList = 0;
	m_bFront9 = true;

	m_pCancelBtn = 0;
	m_pCloseBtn = 0;

	m_pBar2 = 0;
	m_pBar3 = 0;
	m_pPPLArea = 0;

	unsigned char roomType = WSingleton<CSharedDoc>::Instance()->m_roomType;
	if (roomType == 7) {
		m_scoreType = 4;
	} else if (roomType == 6 || roomType == 1 || roomType == 3) {
		m_scoreType = 5;
	} else {
		m_scoreType = 0;
	}

	m_typeTime = 0;
	m_unknown1d0 = 0;

	memset(m_pTypeBtn, 0, sizeof(m_pTypeBtn));
	memset(m_pScoreList, 0, sizeof(m_pScoreList));
	memset(m_rowData, 0, sizeof(m_rowData));
	memset(m_unknown1a0, 0, sizeof(m_unknown1a0));

	m_unknown180 = 0;
	m_unknown184 = 0;
	m_unknown188 = 0;
	m_unknown18c = 0;
	m_rowCount = 1;

	m_bShowBlankPanel = WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomMode == 14;

	m_pBlankPanel01 = 0;
	m_pBlankPanel02 = 0;
	m_pBlankPanel03 = 0;
}

// scoreboard.cpp:4497
void FrScoreForm::SetState(eState state) {
	if (m_state == state) {
		return;
	}
	m_state = state;

	if (state == STATE_HIDDEN) {
		Enable(false);
		SetVisible(false);
	} else {
		Enable(true);
		SetVisible(true);

		if (WSingleton<CSharedDoc>::Instance()->m_holes == 18) {
			CGolfDoc* pGolfDoc = GOLFDOC();
			m_bFront9 = (GetHoleIndex(pGolfDoc->m_currentHole) < 10 || pGolfDoc->m_approachMode) ? true : false;
		} else {
			m_bFront9 = true;
		}
		SetFrontBackBtn();
	}

	if (m_pPPLArea) {
		m_pPPLArea->SetVisible(m_bPPL && m_state == 3);
	}
}

// scoreboard.cpp:4624
void FrScoreForm::ChangeTurn(void) {
	UpdatePang();
}

// scoreboard.cpp:4647
void FrScoreForm::OnBar2Init(intptr_t param) {
	m_pBar2 = DYNAMIC_CAST(FrArea, param);
}

// scoreboard.cpp:4652
void FrScoreForm::OnBar3Init(intptr_t param) {
	m_pBar3 = DYNAMIC_CAST(FrArea, param);
}

// scoreboard.cpp:4659
void FrScoreForm::OnFront9BtnInit(intptr_t param) {
	m_pFront9Btn = DYNAMIC_CAST(FrButton, param);
	if (WSingleton<CSharedDoc>::Instance()->m_holes != 18) {
		m_pFront9Btn->SetVisible(false);
	}
}

// scoreboard.cpp:4666
void FrScoreForm::OnFront9BtnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_holes != 18) {
		return;
	}
	m_bFront9 = true;
	SetFrontBackBtn();
}

// scoreboard.cpp:4675
void FrScoreForm::OnBack9BtnInit(intptr_t param) {
	m_pBack9Btn = DYNAMIC_CAST(FrButton, param);
	if (WSingleton<CSharedDoc>::Instance()->m_holes != 18) {
		m_pBack9Btn->SetVisible(false);
	}
}

// scoreboard.cpp:4682
void FrScoreForm::OnBack9BtnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_holes != 18) {
		return;
	}
	m_bFront9 = false;
	SetFrontBackBtn();
}

// scoreboard.cpp:4691
void FrScoreForm::OnShotBtnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		if (m_scoreType == 4) {
			return;
		}
		m_scoreType = 4;
	} else {
		if (m_scoreType == 0) {
			return;
		}
		m_scoreType = 0;
	}
	SetTypeBtn();
}

// scoreboard.cpp:4709
void FrScoreForm::OnGraphBtnUp(void) {
	if (m_scoreType == 1) {
		return;
	}
	m_scoreType = 1;
	SetTypeBtn();
}

// scoreboard.cpp:4718
void FrScoreForm::OnVersusBtnUp(void) {
	if (m_scoreType == 5) {
		return;
	}
	m_scoreType = 5;
	SetTypeBtn();
}

// scoreboard.cpp:4727
void FrScoreForm::OnPuttBtnUp(void) {
	if (m_scoreType == 2) {
		return;
	}
	m_scoreType = 2;
	SetTypeBtn();
}

// scoreboard.cpp:4736
void FrScoreForm::OnPangBtnUp(void) {
	if (m_scoreType == 3) {
		return;
	}
	m_scoreType = 3;
	SetTypeBtn();
}

// scoreboard.cpp:4755
void FrScoreForm::OnShotBtnInit(intptr_t param) {
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		m_pTypeBtn[4] = DYNAMIC_CAST(FrButton, param);
		if (m_pTypeBtn[4]) {
			m_pTypeBtn[4]->SetButtonImg("skin_n", FrButton::NORMAL);
			m_pTypeBtn[4]->SetButtonImg("skin_o", FrButton::OVER);
			m_pTypeBtn[4]->SetButtonImg("skin_o", FrButton::PRESSED);
		}
	} else {
		m_pTypeBtn[0] = DYNAMIC_CAST(FrButton, param);
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6 || WSingleton<CSharedDoc>::Instance()->m_roomType == 1 || WSingleton<CSharedDoc>::Instance()->m_roomType == 3) {
		m_pTypeBtn[0]->SetVisible(false);
	}
}

// scoreboard.cpp:4779
void FrScoreForm::OnGraphBtnInit(intptr_t param) {
	m_pTypeBtn[1] = DYNAMIC_CAST(FrButton, param);
	if (!m_pTypeBtn[1]) {
		return;
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6 || WSingleton<CSharedDoc>::Instance()->m_roomType == 1 || WSingleton<CSharedDoc>::Instance()->m_roomType == 3 || WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		m_pTypeBtn[1]->SetVisible(false);
	}
}

// scoreboard.cpp:4789
void FrScoreForm::OnPuttBtnInit(intptr_t param) {
	m_pTypeBtn[2] = DYNAMIC_CAST(FrButton, param);
	if (!m_pTypeBtn[2]) {
		return;
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6 || WSingleton<CSharedDoc>::Instance()->m_roomType == 1 || WSingleton<CSharedDoc>::Instance()->m_roomType == 3 || WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		m_pTypeBtn[2]->SetVisible(false);
	}
}

// scoreboard.cpp:4800
void FrScoreForm::OnPangBtnInit(intptr_t param) {
	m_pTypeBtn[3] = DYNAMIC_CAST(FrButton, param);
}

// scoreboard.cpp:4819
void FrScoreForm::OnVersusBtnInit(intptr_t param) {
	m_pTypeBtn[5] = DYNAMIC_CAST(FrButton, param);
	if (!m_pTypeBtn[5]) {
		return;
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6 || WSingleton<CSharedDoc>::Instance()->m_roomType == 1 || WSingleton<CSharedDoc>::Instance()->m_roomType == 3) {
		// the versus button takes the shot button's place
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1 || WSingleton<CSharedDoc>::Instance()->m_roomType == 3) {
			m_pTypeBtn[5]->MoveWindow(WPoint(m_pTypeBtn[0]->GetRect().x, m_pTypeBtn[0]->GetRect().y));
		}
		m_pTypeBtn[5]->SetVisible(true);
	} else {
		m_pTypeBtn[5]->SetVisible(false);
	}
}

// scoreboard.cpp:4846
void FrScoreForm::OnHoleListInit(intptr_t param) {
	m_pHoleList = DYNAMIC_CAST(FrListBox, param);
	if (m_pHoleList) {
		m_pHoleList->AddItem(0);
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		m_pHoleList->SetItemSize(470, 22);
		m_pHoleList->SetRect(WRect(m_pHoleList->GetRect().x - 22.0f, m_pHoleList->GetRect().y, m_pHoleList->GetRect().w, m_pHoleList->GetRect().h));
	}
}

// scoreboard.cpp:4870
void FrScoreForm::OnHoleListOwnerDraw(intptr_t param) {
	if (m_bShowBlankPanel) {
		return;
	}

	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0) {
		return;
	}

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0) {
		return;
	}

	int width = m_pHoleList->GetItemWidth();
	int height = m_pHoleList->GetItemHeight();
	unsigned char holes = WSingleton<CSharedDoc>::Instance()->m_holes;

	float x = width * (1.0f / 18.0f) + pItem->pos.x;
	float y = height * 0.5f + pItem->pos.y - 7.0f;
	int start = m_bFront9 ? 0 : 9;
	int end = start + 9;
	if (end >= holes) {
		end = holes;
	}

	if (start < end) {
		float step = width * (1.0f / 9.0f);
		for (int i = start; i < end; i++) {
			DrawDigit(pGDI, MakeStr("%d", WSingleton<CSharedDoc>::Instance()->m_holeOrder[i]), WPoint(x, y), 0xFFFFFFFF, FrGDI::ALIGN_CENTER, false);
			x = (float)(int)(step + x);
		}
	}
}

// scoreboard.cpp:4912
void FrScoreForm::OnParListInit(intptr_t param) {
	m_pParList = DYNAMIC_CAST(FrListBox, param);
	if (m_pParList) {
		m_pParList->AddItem(0);
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		m_pParList->SetItemSize(470, 22);
		m_pParList->SetRect(WRect(m_pParList->GetRect().x - 22.0f, m_pParList->GetRect().y, m_pParList->GetRect().w, m_pParList->GetRect().h));
	}
}

// scoreboard.cpp:4984
void FrScoreForm::OnPangListInit(int param) {
	if (CSharedDoc::Instance()->m_roomType == 7) {
		m_pPangList = DYNAMIC_CAST(FrListBox, param);
		m_pPangList->AddItem(0);

		m_pPangList->SetItemSize(0x1d6, 0x16);
	}
}

// scoreboard.cpp:5009
void FrScoreForm::OnBlankPanel01Init(intptr_t param) {
	m_pBlankPanel01 = DYNAMIC_CAST(FrArea, param);
	if (m_pBlankPanel01) {
		m_pBlankPanel01->SetVisible(m_bShowBlankPanel);
	}
}

// scoreboard.cpp:5018
void FrScoreForm::OnBlankPanel01OwnerDraw(int param) {
	if (m_pBlankPanel01) {
		m_pBlankPanel01->SetVisible(m_bShowBlankPanel);
	}
}

// scoreboard.cpp:5027
void FrScoreForm::OnBlankPanel02Init(intptr_t param) {
	m_pBlankPanel02 = DYNAMIC_CAST(FrArea, param);
	if (m_pBlankPanel02) {
		m_pBlankPanel02->SetVisible(m_bShowBlankPanel);
	}
}

// scoreboard.cpp:5036
void FrScoreForm::OnBlankPanel02OwnerDraw(int param) {
	if (m_pBlankPanel02) {
		m_pBlankPanel02->SetVisible(m_bShowBlankPanel);
	}
}

// scoreboard.cpp:5045
void FrScoreForm::OnBlankPanel03Init(intptr_t param) {
	m_pBlankPanel03 = DYNAMIC_CAST(FrArea, param);
	if (m_pBlankPanel03) {
		m_pBlankPanel03->SetVisible(m_bShowBlankPanel);
	}
}

// scoreboard.cpp:5054
void FrScoreForm::OnBlankPanel03OwnerDraw(int param) {
	if (m_pBlankPanel03) {
		m_pBlankPanel03->SetVisible(m_bShowBlankPanel);
	}
}

// scoreboard.cpp:5142
void FrScoreForm::OnGolferListInit(intptr_t param) {
	m_pScoreList[0] = DYNAMIC_CAST(FrListBox, param);

	for (int i = 0; i < m_rowCount; i++) {
		m_pScoreList[0]->AddItem(m_rowData[i]);
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		m_pScoreList[0]->SetItemSize(103, 58);
		m_pScoreList[0]->SetRect(WRect(m_pScoreList[0]->GetRect().x - 1.0f, m_pScoreList[0]->GetRect().y + 22.0f, m_pScoreList[0]->GetRect().w, m_pScoreList[0]->GetRect().h));
	}
}

// scoreboard.cpp:5468
void FrScoreForm::OnRecordListInit(intptr_t param) {
	m_pScoreList[1] = DYNAMIC_CAST(FrListBox, param);

	if (m_pScoreList[1]) {
		for (int i = 0; i < m_rowCount; i++) {
			m_pScoreList[1]->AddItem(m_rowData[i]);
		}
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		m_pScoreList[1]->SetItemSize(470, 58);
		m_pScoreList[1]->SetRect(WRect(m_pScoreList[1]->GetRect().x - 24.0f, m_pScoreList[1]->GetRect().y + 22.0f, m_pScoreList[1]->GetRect().w, m_pScoreList[1]->GetRect().h));
	}
}

// scoreboard.cpp:5618
void FrScoreForm::OnTotalListInit(intptr_t param) {
	m_pScoreList[2] = DYNAMIC_CAST(FrListBox, param);

	if (!m_pScoreList[2]) {
		return;
	}

	for (int i = 0; i < m_rowCount; i++) {
		m_pScoreList[2]->AddItem(m_rowData[i]);
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		m_pScoreList[2]->SetItemSize(63, 58);
		m_pScoreList[2]->SetRect(WRect(m_pScoreList[2]->GetRect().x + 104.0f, m_pScoreList[2]->GetRect().y + 22.0f, m_pScoreList[2]->GetRect().w, m_pScoreList[2]->GetRect().h));
	}
}

// scoreboard.cpp:5846
bool FrScoreForm::OnInit(void) {
	Enable(m_state != STATE_HIDDEN);
	SetVisible(m_state != STATE_HIDDEN);

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		WRect rect = m_rect;
		rect.w += 36.0f;
		SetRect(rect);
	}
	SortLists(false);
	LoadBitmaps();

	return true;
}

// scoreboard.cpp:5860
void FrScoreForm::OnProc(const float dt) {
	if (m_state == STATE_HIDDEN) {
		return;
	}
	if (m_typeTime < 5.0f) {
		m_typeTime += dt;
	}
}

// scoreboard.cpp:5869
void FrScoreForm::OnDraw(void) {
	// scoreboard.cpp:5876
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		return;
	}

	// scoreboard.cpp:5882
	WPoint p1, p2;
	p1.x = p2.x = m_rect.x + 129.0f;
	p1.y = m_rect.y + 116.0f;
	p2.y = m_rowCount * 58.0f + p1.y;

	// scoreboard.cpp:5887
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		p1.x = p2.x - 24.0f;
		p2.x = p1.x;
		p1.y += 22.0f;
		p2.y += 22.0f;
	}

	// scoreboard.cpp:5895
	for (int i = 0; i < 10; i++) {
		g_view->DrawLine2D(p1, p2, 0xFF000000, 0xFF000000, 0);
		p1.x += 38.0f;
		p2.x += 38.0f;

		// scoreboard.cpp:5902
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
			p1.x += 9.0f;
			p2.x += 9.0f;
		}
	}

	// scoreboard.cpp:5909
	p1.x += 27.0f;
	p2.x += 27.0f;

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		p1.x -= 13.0f;
		p2.x -= 13.0f;
	}

	// scoreboard.cpp:5919
	g_view->DrawLine2D(p1, p2, 0xFF000000, 0xFF000000, 0);

	// scoreboard.cpp:5922
	p1.y = p2.y = m_rect.y + 174.0f;

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		p1.y = p2.y + 22.0f;
		p2.y = p1.y;
	}

	// scoreboard.cpp:5930
	for (int i = 0; i < m_rowCount - 1; i++) {
		p1.x = m_rect.x + 3.0f;
		p2.x = m_rect.x + 28.0f;
		g_view->DrawLine2D(p1, p2, 0, 0xFF000000, 0);

		// scoreboard.cpp:5937
		p1.x = m_rect.x + m_rect.w - 36.0f;
		p2.x = m_rect.x + m_rect.w - 11.0f;
		g_view->DrawLine2D(p1, p2, 0xFF000000, 0, 0);

		// scoreboard.cpp:5941
		p1.x = m_rect.x + 28.0f;
		p2.x = m_rect.x + m_rect.w - 36.0f;
		g_view->DrawLine2D(p1, p2, 0xFF000000, 0xFF000000, 0);

		// scoreboard.cpp:5945
		p1.y += 58.0f;
		p2.y += 58.0f;
	}
}

// scoreboard.cpp:5951
void FrScoreForm::SetFrontBackBtn(void) {
	m_typeTime = 0;
	if (WSingleton<CSharedDoc>::Instance()->m_holes == 18) {
		if (m_pFront9Btn) {
			m_pFront9Btn->SetVisible(true);
			m_pFront9Btn->SetStatus(m_bFront9 ? FrButton::PRESSED : FrButton::NORMAL);
		}

		if (m_pBack9Btn) {
			m_pBack9Btn->SetVisible(true);
			m_pBack9Btn->SetStatus(m_bFront9 ? FrButton::NORMAL : FrButton::PRESSED);
		}
	} else {
		if (m_pFront9Btn) {
			m_pFront9Btn->SetVisible(false);
		}
		if (m_pBack9Btn) {
			m_pBack9Btn->SetVisible(false);
		}
	}
}

// scoreboard.cpp:5983
void FrScoreForm::SetTypeBtn(void) {
	m_typeTime = 0.0f;

	for (int i = 0; i < 6; i++) {
		if (m_pTypeBtn[i]) {
			m_pTypeBtn[i]->SetStatus(m_scoreType == i ? FrButton::PRESSED : FrButton::NORMAL);
		}
	}
}

// scoreboard.cpp:6582
unsigned long FrScoreForm::GetScoreColor(int score) {
	switch (score) {
	case -4:
	case -3:
	case -2:
		return 0xffff0000;
	case -1:
		return 0xffffbb4f;
	case 0:
		return 0xffffffff;
	case 1:
		return 0xff82c3ff;
	default:
		return 0xff0036ff;
	}
}

// scoreboard.cpp:6601
void FrScoreForm::OnTotalInit(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (!pArea) {
		return;
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1 || WSingleton<CSharedDoc>::Instance()->m_roomType == 3 || WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
		pArea->SetBgImg("header04");
	} else if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		pArea->SetBgImg("header03_skins");
		pArea->SetRect(pArea->GetRect() + WRect(-52.0f, 0.0f, 0.0f, 0.0f));
	} else {
		pArea->SetBgImg("header03");
	}
}

// scoreboard.cpp:6621
void FrScoreForm::OnPPLInit(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	m_pPPLArea = pArea;
	if (!pArea) {
		return;
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1 || WSingleton<CSharedDoc>::Instance()->m_roomType == 6 || WSingleton<CSharedDoc>::Instance()->m_roomType == 3) {
		pArea->SetRect(WRect(pArea->GetRect().x - 65.0f, pArea->GetRect().y, pArea->GetRect().w, pArea->GetRect().h));
	} else if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		pArea->SetRect(pArea->GetRect() + WRect(43.0f, 0.0f, 0.0f, 0.0f));
	}

	m_pPPLArea->SetVisible(false);
	m_bPPL = false;
}

// scoreboard.cpp:6641
void FrScoreForm::SetPPL(const char* name) {
	if (m_pPPLArea) {
		const Bitmap* pOldBg = m_pPPLArea->GetBgImg();
		if (m_pPPLArea->SetBgImg(name)) {
			m_bPPL = true;
			m_pPPLArea->SetVisible(true);
		} else {
			m_bPPL = false;
			m_pPPLArea->SetBgImg(pOldBg);
			m_pPPLArea->SetVisible(false);
		}
	}
}

// scoreboard.cpp:6663
void FrScoreForm::OnBoard1Init(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);

	if (pArea && WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		pArea->SetBgImg("header01_skins");
		WRect rect = pArea->GetRect(); rect.x -= 1.0f;
		pArea->SetRect(rect);
	} else {
		pArea->SetBgImg("header01");
	}
}

// scoreboard.cpp:6676
void FrScoreForm::OnBoard2Init(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);

	if (pArea && WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		pArea->SetBgImg("header02_skins");
		WRect rect = pArea->GetRect(); rect.x -= 32.0f;
		pArea->SetRect(rect);
	} else {
		pArea->SetBgImg("header02");
	}
}

// scoreboard.cpp:6689
void FrScoreForm::OnDrawLinesInit(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);

	if (WSingleton<CSharedDoc>::Instance()->m_roomType != 7) {
		pArea->SetVisible(false);
	}
}

// scoreboard.cpp:6774
void FrScoreForm::OnCancelBtnInit(intptr_t param) {
	m_pCancelBtn = DYNAMIC_CAST(FrButton, param);
	m_pCancelBtn->SetVisible(false);
}

// scoreboard.cpp:6780
void FrScoreForm::OnCancelBtnUp(void) {
	Close(FrOK, true);
}

// scoreboard.cpp:6785
void FrScoreForm::OnCloseBtnInit(intptr_t param) {
	if (!m_pCloseBtn) {
		m_pCloseBtn = DYNAMIC_CAST(FrButton, param);
	}

	if (m_pCloseBtn) {
		m_pCloseBtn->SetVisible(false);
	}
}

// scoreboard.cpp:6827
void FrScoreForm::OnCloseBtnUp(void) {
	SetVisible(false);
}

// scoreboard.cpp:7018
void FrScoreForm::ConditionalStrCat(char* dst, const char* src) {
	int len = strlen(dst);

	char last = dst[len - 1];
	if (last == '0' || last == '3' || last == '6') {
		strcat(dst, "\xC0\xB8");
	}

	strcat(dst, src);
}

// scoreboard.cpp:7038 (run-time type and factory registration of FrMatchItems)
const WRTTI FrMatchItems::m_RTTI("FrMatchItems", &FrForm::m_RTTI);

IObject* FrMatchItemsMakeInstance(void) {
	return new FrMatchItems;
}

struct __sFrMatchItems {
	__sFrMatchItems(void) {
		ObjectFactory().AddObjectFunctor(FrMatchItemsMakeInstance, "FrMatchItems");
	}
};
static __sFrMatchItems s_registerFrMatchItems;

// scoreboard.cpp:7040
const sFRESH_MSGMAP* FrMatchItems::GetMessageMap(void) const {
	return &FrMatchItems::_MsgMap;
}

sFRESH_MSGMAP FrMatchItems::_MsgMap = { &FrForm::_MsgMap, &FrMatchItems::_MsgEntries[0] };

sFRESH_ENTRY FrMatchItems::_MsgEntries[] = {
	// scoreboard.cpp:7042
	{ "view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMatchItems::OnViewInit },
	// scoreboard.cpp:7043
	{ "view", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMatchItems::OnViewOwnerDraw },

	// scoreboard.cpp:7045
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// scoreboard.cpp:7053
FrMatchItems::FrMatchItems(void) {
	m_pSimpleUI = 0;
}

// scoreboard.cpp:7058
FrMatchItems::~FrMatchItems(void) {
	if (m_pSimpleUI) { delete m_pSimpleUI; m_pSimpleUI = 0; }
}

// scoreboard.cpp:7063
bool FrMatchItems::OnInit(void) {
	// scoreboard.cpp:7064
	m_pos.x = (g_view->GetWidth() - 440.0f) * 0.5f;
	m_pos.y = g_view->GetHeight() - 100.0f;

	// scoreboard.cpp:7067
	m_pSimpleUI = new CSimpleUI("[emoti_object.png");
	m_pSimpleUI->Add("0", WRect(0.0f, 128.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("1", WRect(64.0f, 128.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("2", WRect(128.0f, 128.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("3", WRect(192.0f, 128.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("4", WRect(0.0f, 192.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("5", WRect(64.0f, 192.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("6", WRect(128.0f, 192.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("7", WRect(192.0f, 192.0f, 64.0f, 64.0f), false);

	// scoreboard.cpp:7084
	m_unknown14c = 0;
	m_bRefresh = true;
	m_bRedraw = true;
	m_unknown150 = 0;
	m_unknown154 = -3.0f;
	return true;
}

// scoreboard.cpp:7087
void FrMatchItems::OnViewInit(intptr_t param) {
	m_pView = DYNAMIC_CAST(FrArea, param);
}

// scoreboard.cpp:7269 (run-time type and factory registration of CScore)
const WRTTI CScore::m_RTTI("CScore", &IActor::m_RTTI);

IObject* CScoreMakeInstance(void) {
	return new CScore;
}

struct __impCScore {
	__impCScore(void) {
		ObjectFactory().AddObjectFunctor(CScoreMakeInstance, "CScore");
		ActorFactory().AddActorClass("CScore");
	}
};
static __impCScore s_registerCScore;

// scoreboard.cpp:7271
const sFRESH_MSGMAP* CScore::GetMessageMap(void) const {
	return &CScore::_MsgMap;
}

sFRESH_MSGMAP CScore::_MsgMap = { &FrCmdTarget::_MsgMap, &CScore::_MsgEntries[0] };

sFRESH_ENTRY CScore::_MsgEntries[] = {
	// scoreboard.cpp:7275
	{ ".score.close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CScore::OnCloseBtnUp },

	// scoreboard.cpp:7277
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// scoreboard.cpp:7284
CScore::CScore(void) : m_unknown1c(0), m_awardTime(0), m_bOpen(0), m_unknown28(1), m_unknown2c(1), m_unknown30(0),
	m_pScoreForm(0), m_pRankForm(0), m_pRankForm2(0), m_pRankForm3(0), m_pRankFormEx(0), m_pAwardForm(0), m_pMatchItems(0),
	m_pGuildScoreForm(0), m_pTreasureGauge(0), m_pNextHole(0), m_pTreasureScore(0), m_pTreasureGift(0), m_fadeTime(0),
	m_pAquireItem(0) {
	m_priority = 5;
	m_noticeImages.reserve(10);

	LoadNoticeImageFile(MakeStr("%s/score_ppl.ini", SCORE_PPL_DIR), m_noticeImages, 10, 0);

	ImageSortByCharacterIndex(m_noticeImages);
}

// scoreboard.cpp:7329
void CScore::OnDestroy(void) {
	if (m_pRankFormEx) {
		m_pRankFormEx->Close(FrOK, true);
		m_pRankFormEx = 0;
	}

	if (m_pGuildScoreForm) {
		m_pGuildScoreForm->Close(true);
		m_pGuildScoreForm = 0;
	}

	if (m_pAquireItem) { delete m_pAquireItem; m_pAquireItem = 0; }
}

// scoreboard.cpp:7518
void CScore::OnLoad(void) {
	// scoreboard.cpp:7528
	switch (WSingleton<CSharedDoc>::Instance()->m_roomType) {
	case 4:
	case 5:
	case 6:
	case 7:
	case 10:
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
			break;
		}

		if ((WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity & 0xE) == 0xE) {
			m_pRankForm2 = CreateForm<FrRankForm2>(g_pFresh->GetManager(), this, "rank2", 0);
			m_pRankForm2->SetFixed(true);
			m_pRankForm2->EnableDrag(false);
			m_pRankForm2->Open(0, 0xC);
			m_pRankForm2->SetVisible(false);
		} else {
			m_pRankForm = CreateForm<FrRankForm>(g_pFresh->GetManager(), this, "rank", 0);

			m_rankPos.x = g_view->GetWidth() - m_pRankForm->GetRect().w;
			m_rankPos.y = 40.0f;

			m_pRankForm->SetFixed(true);
			m_pRankForm->EnableDrag(false);
			m_pRankForm->Open(0, m_rankPos, 0xC);
			m_pRankForm->SetVisible(false);
			m_pRankForm->Resize();
		}

		if (WSingleton<CSharedDoc>::Instance()->m_roomType != 10) {
			m_pRankFormEx = CreateForm<FrRankFormEx>(g_pFresh->GetManager(), this, "rankex", 0);
			m_pRankFormEx->EnableDrag(false);

			WPoint pt;
			pt.x = (float)(int)((g_view->GetWidth() - m_pRankFormEx->GetRect().w) * 0.5f);
			pt.y = (float)(int)((g_view->GetHeight() - m_pRankFormEx->GetRect().h) * 0.5f);
			m_pRankFormEx->SetFixed(true);
			m_pRankFormEx->Open((FRESH_RESULT_CALLBACK)&CScore::OnRankFormExResult, pt, 4);
			m_pRankFormEx->SetVisible(false);
			m_pRankFormEx->ResetStage();
		}
		break;
	}

	// scoreboard.cpp:7568
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
		m_pRankForm3 = CreateForm<FrRankForm3>(g_pFresh->GetManager(), this, "rank3", 0);

		m_rankPos.x = g_view->GetWidth() - m_pRankForm3->GetRect().w + 5.0f;
		m_rankPos.y = 40.0f;

		m_pRankForm3->SetFixed(true);
		m_pRankForm3->EnableDrag(false);
		m_pRankForm3->Open(0, m_rankPos, 0xC);
		m_pRankForm3->SetVisible(false);

		m_pGuildScoreForm = CreateForm<FrScoreDlg>(g_pFresh->GetManager(), this, "score_guild", 0);

		WPoint pt;
		pt.x = (g_view->GetWidth() - m_pGuildScoreForm->GetRect().w + 1.0f) * 0.5f;
		pt.y = m_pGuildScoreForm->GetRect().y;
		m_pGuildScoreForm->EnableDrag(false);
		m_pGuildScoreForm->SetFixed(true);
		m_pGuildScoreForm->Open((FRESH_RESULT_CALLBACK)&CScore::OnGuildScoreFormResult, pt, 0xC);
		m_pGuildScoreForm->SetVisible(false);

		// scoreboard.cpp:7591
		m_pGuildScoreForm->SetVisibleCloseBtn(false);
		return;
	}

	// scoreboard.cpp:7599
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
		m_pScoreForm = CreateForm<FrScoreForm>(g_pFresh->GetManager(), this, "score_skins", 0);
		m_pScoreForm->EnableDrag(false);
		m_pScoreForm->SetFixed(true);
		m_pScoreForm->Open(0, 0xC);
		m_pScoreForm->Resize();
		m_pScoreForm->SetState(FrScoreForm::STATE_HIDDEN);
		m_pScoreForm->SetTypeBtn();
		m_pScoreForm->SetVisible(false);
		m_pScoreForm->GetBaseFrame()->SetCaptionOffset(true);
		m_pScoreForm->GetBaseFrame()->m_nFlags.Enable(0x1000);
	} else {
		// scoreboard.cpp:7613
		m_pScoreForm = CreateForm<FrScoreForm>(g_pFresh->GetManager(), this, "score", 0);
		m_pScoreForm->EnableDrag(false);
		m_pScoreForm->SetFixed(true);
		m_pScoreForm->Open(0, 0xC);
		m_pScoreForm->Resize();
		m_pScoreForm->SetState(FrScoreForm::STATE_HIDDEN);
		m_pScoreForm->SetTypeBtn();
		m_pScoreForm->SetVisible(false);
		m_pScoreForm->GetBaseFrame()->SetCaptionOffset(true);
		m_pScoreForm->GetBaseFrame()->m_nFlags.Enable(0x1000);

		// scoreboard.cpp:7626
		m_pTreasureGauge = CreateForm<FrTreasureGauge>(g_pFresh->GetManager(), this, "tPointdlg", 0);
		m_pTreasureGauge->EnableDrag(false);
		m_pTreasureGauge->SetFixed(true);
		m_pTreasureGauge->Open(0, 0xC);
		m_pTreasureGauge->SetState((FrTreasureGauge::eState)1);

		// scoreboard.cpp:7633
		m_pNextHole = CreateForm<FrNextHole>(g_pFresh->GetManager(), this, "nexthole", 0);
		m_pNextHole->EnableDrag(false);
		m_pNextHole->SetFixed(true);
		m_pNextHole->Open(0, 0xC);
		m_pNextHole->SetState(FrNextHole::STATE_HIDE);
	}

	// scoreboard.cpp:7641
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 3) {
		m_pMatchItems = CreateForm<FrMatchItems>(g_pFresh->GetManager(), this, "items", 0);
		m_pMatchItems->SetFixed(true);

		WPoint pt;
		pt.x = (g_view->GetWidth() - 440.0f) * 0.5f;
		pt.y = g_view->GetHeight() - 100.0f;
		m_pMatchItems->MoveWindow(pt);
		m_pMatchItems->Open(0, 0xC);
		m_pMatchItems->SetVisible(false);
	}
}

// scoreboard.cpp:7667
void CScore::OnReload(void) {
	this << MsgObject(0, 0x131, 0, 0, 0, 0, 0);
	this << MsgObject(0, 0x2a, 0, 0, 0, 0, 0);
}

// scoreboard.cpp:7673
void CScore::OnProcess(float dt) {
	// scoreboard.cpp:7674
	if (GOLFDOC()->m_gameState < 7) {
		return;
	}

	// scoreboard.cpp:7677
	if (IsLocalContent((localContentType_t)0x4F) && WSingleton<CSharedDoc>::Instance()->m_playingMode == 2) {
		return;
	}

	// scoreboard.cpp:7681
	if (m_pRankForm) {
		if (!WSingleton<CCapturedBg>::Instance()->IsCaptured() && !(GOLFDOC()->GameMode() & 0x1E84)) {
			// scoreboard.cpp:7700
			bool bShow = g_pFresh->GetManager()->GetDesktop()->IsWallPaperVisible() == false;
			if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
				bShow = false;
			}

			// scoreboard.cpp:7707
			m_pRankForm->SetVisible(bShow);

			if (bShow) {
				// scoreboard.cpp:7711
				switch (GOLFDOC()->GameMode()) {
				case 0x40:
					if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
						m_rankPos.x = g_view->GetWidth() - m_pRankForm->GetRect().w;
						m_pRankForm->MoveWindow(m_rankPos);
						break;
					}
					// fall through
				case 0x100:
				case 0x40000:
					// scoreboard.cpp:7722
					m_rankPos.x = Min(m_rankPos.x + dt * 600.0f, g_view->GetWidth());
					m_pRankForm->MoveWindow(m_rankPos);
					break;

				case 8: {
					// scoreboard.cpp:7737
					float mouseX, mouseY;
					SendMsg(this, "Screen", 0x1C6, (intptr_t)&mouseX, (intptr_t)&mouseY, 0, 0);

					float x = g_view->GetWidth() - m_pRankForm->GetRect().w;
					if (x - 60.0f > mouseX) {
						m_rankPos.x = Max(m_rankPos.x - dt * 600.0f, x);
					} else {
						m_rankPos.x = Min(m_rankPos.x + dt * 600.0f, g_view->GetWidth());
					}
					m_pRankForm->MoveWindow(m_rankPos);
					break;
				}

				case 0x10:
				case 0x20:
					break;

				default:
					// scoreboard.cpp:7754
					m_pRankForm->SetVisible(false);
					break;
				}
			}
		} else if (WSingleton<CSharedDoc>::Instance()->m_roomType != 10 || !(GOLFDOC()->GameMode() & 0x880)) {
			// scoreboard.cpp:7695
			m_pRankForm->SetVisible(false);
		}
	}

	// scoreboard.cpp:7762
	if (m_pRankForm3) {
		m_pRankForm3->SetVisible(m_unknown28 == 1);

		// scoreboard.cpp:7771
		if (!WSingleton<CQuitWindow>::Instance()->IsActive() && !WSingleton<CUserInfo>::Instance()->IsVisible()
			&& WSingleton<CChatMsg>::Instance()->GetOpenState() != 3 && !g_pFresh->GetManager()->IsKeyExclusive()
			&& g_input->GetDown("RANK", true) && GOLFDOC()->m_gameState >= 15) {
			if (m_unknown28 == 0) {
				if (GOLFDOC()->GameMode() & ~0x401) {
					m_unknown2c = 1;
					m_unknown28 = 1;
				}
			} else {
				m_unknown2c = 0;
				m_unknown28 = 0;
			}
		}
	}

	// scoreboard.cpp:7799
	if (m_pScoreForm && !WSingleton<CQuitWindow>::Instance()->IsActive() && !WSingleton<CUserInfo>::Instance()->IsVisible()
		&& WSingleton<CChatMsg>::Instance()->GetOpenState() != 3 && !g_pFresh->GetManager()->IsKeyExclusive()
		&& g_input->GetDown("TAB", true) && !g_input->Get("LALT", true) && !g_input->Get("RALT", true)
		&& !g_input->Get("RCONTROL", true) && !g_input->Get("LCONTROL", true) && GOLFDOC()->m_gameState >= 15) {
		if (m_bOpen == 0) {
			if (GOLFDOC()->GameMode() & ~0x401) {
				m_bOpen = 1;
				m_pScoreForm->SetState((FrScoreForm::eState)2);
				m_pScoreForm->SetDesc();

				// scoreboard.cpp:7809
				if (GOLFDOC()->GameMode() & ~0x421) {
					if (m_pTreasureGauge) {
						m_pTreasureGauge->SetState((FrTreasureGauge::eState)2);
					}
					if (m_pNextHole) {
						m_pNextHole->SetState((FrNextHole::eState)2);
					}
				}
			}
		} else if (m_bOpen == 1) {
			// scoreboard.cpp:7847
			m_bOpen = 0;
			m_pScoreForm->SetState(FrScoreForm::STATE_HIDDEN);

			// scoreboard.cpp:7850
			if (GOLFDOC()->GameMode() & ~0x421) {
				if (m_pTreasureGauge) {
					m_pTreasureGauge->SetState((FrTreasureGauge::eState)1);
				}
				if (m_pNextHole) {
					m_pNextHole->SetState(FrNextHole::STATE_HIDE);
				}
			}
		}
	}

	// scoreboard.cpp:7892
	if (m_pGuildScoreForm && !WSingleton<CQuitWindow>::Instance()->IsActive() && !WSingleton<CUserInfo>::Instance()->IsVisible()
		&& WSingleton<CChatMsg>::Instance()->GetOpenState() != 3 && !g_pFresh->GetManager()->IsKeyExclusive()
		&& g_input->GetDown("TAB", true) && !g_input->Get("LALT", true) && !g_input->Get("RALT", true)
		&& !g_input->Get("RCONTROL", true) && !g_input->Get("LCONTROL", true) && GOLFDOC()->m_gameState >= 15) {
		if (m_bOpen == 0) {
			if (GOLFDOC()->GameMode() & ~0x401) {
				m_bOpen = 1;
				m_pGuildScoreForm->SetVisible(true);
				m_pGuildScoreForm->SetGuildPlayer(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid);

				// scoreboard.cpp:7904
				m_pGuildScoreForm->SetVisibleCloseBtn(false);
				m_pGuildScoreForm->SetVisibleCancelBtn(false);
			}
		} else if (m_bOpen == 1) {
			// scoreboard.cpp:7912
			m_bOpen = 0;
			m_pGuildScoreForm->SetVisible(false);
		}
	}

	// scoreboard.cpp:7943
	if (m_bOpen == 2) {
		if (m_fadeTime < 1.0f) {
			m_fadeTime += dt;
			if (m_fadeTime >= 1.0f) {
				m_fadeTime = 1.0f;
			}
			unsigned char alpha = (unsigned char)(int)(m_fadeTime * 255.0f);
			g_pFresh->GetManager()->GetDesktop()->SetBgColor((alpha << 24) | 0xFFFFFF);
		}
	} else if (m_bOpen == 3) {
		if (m_fadeTime > 0.0f) {
			m_fadeTime -= dt;
			unsigned char alpha = (unsigned char)float2int(Max(0.0f, m_fadeTime) * 255.0f);
			g_pFresh->GetManager()->GetDesktop()->SetBgColor((alpha << 24) | 0xFFFFFF);

			// scoreboard.cpp:7961
			if (m_fadeTime < 0.0f) {
				m_bOpen = 0;
				m_fadeTime = 0;
			}
		}
	}

	// scoreboard.cpp:7969
	if (m_unknown1c == 1 && WSingleton<CSharedDoc>::Instance()->m_gameMode == 0) {
		m_awardTime += dt;

		if (m_awardTime > 3.0f && m_pAwardForm == 0) {
			m_pAwardForm = CreateForm<FrAwardForm>(g_pFresh->GetManager(), this, "award", 0);
			m_pAwardForm->SetFixed(true);
			m_pAwardForm->Open(0, 0xC);
			m_pAwardForm->EnableDrag(false);

			// scoreboard.cpp:7982
			if (m_pAquireItem) {
				m_pAquireItem->m_value = 0;
			} else {
				m_pAquireItem = new cAquireItem;
			}
		}

		// scoreboard.cpp:7989
		if (m_pAquireItem) {
			m_pAquireItem->OnProc(dt);
		}
	}

	// scoreboard.cpp:7994
	if (WSingleton<CSharedDoc>::Instance()->m_value4844 == 1 && WSingleton<CTHunter>::Instance()->IsReceiveGiftPacket()) {
		m_awardTime += dt;

		if (m_awardTime > 2.0f) {
			// scoreboard.cpp:8000
			if (m_pTreasureScore == 0) {
				m_pTreasureScore = CreateForm<FrTreasureScore>(g_pFresh->GetManager(), this, "trscore", 0);
				m_pTreasureScore->Open(0, 0xC);
				m_pTreasureScore->EnableDrag(false);
				m_pTreasureScore->SetFixed(true);
			}

			// scoreboard.cpp:8008
			if (m_pTreasureGift == 0) {
				m_pTreasureGift = CreateForm<FrTreasureGift>(g_pFresh->GetManager(), this, "trgift", 0);
				m_pTreasureGift->Open(0, 0xC);
				m_pTreasureGift->EnableDrag(false);
				m_pTreasureGift->SetFixed(true);
			}
		}
	}
}

// scoreboard.cpp:8020
void CScore::OnDisplay(void) {
	switch (WSingleton<CSharedDoc>::Instance()->m_roomType) {
	case 4:
	case 5:
	case 6:
	case 9:
	case 10:
		return;
	}

	if (m_unknown1c == 1) {
		if (m_pAquireItem && m_bOpen != 1) {
			for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
				m_pAquireItem->DisplayPrizeList(i);
			}
		}
	}
}

// scoreboard.cpp:8035
void CScore::OnCloseBtnUp(void) {
	m_bOpen = 0;
}

// scoreboard.cpp:8097
void CScore::HandleMsg(const MsgObject& msg) {
	// scoreboard.cpp:8098
	switch (msg.m_msg) {
	case 0x12D: {
		// scoreboard.cpp:8102
		FrLevelupItemForm* pForm = CreateForm<FrLevelupItemForm>(g_pFresh->GetManager(), this, "levelupitem", 0);
		if (pForm) {
			pForm->SetData(*(sLevelUpDone*)msg.m_param1);
			pForm->SetTopmost(true);
			pForm->Open((FRESH_RESULT_CALLBACK)&CScore::OnLevelupFormResult, 3);

			// scoreboard.cpp:8110
			m_levelupForms.push_back(pForm);
		}
		break;
	}

	case 0x12E:
		// scoreboard.cpp:8116
		if (msg.m_param1 == 1) {
			CheckOpenedDlg();
		}
		m_unknown1c = msg.m_param1;
		break;

	case 0x12F:
		// scoreboard.cpp:8125
		if (msg.m_param1 == 0) {
			m_bOpen = 0;

			if (IsMassGame() && m_pRankFormEx) {
				m_pRankFormEx->SetActive(false);
			}
			if (m_pScoreForm) {
				m_pScoreForm->SetState(FrScoreForm::STATE_HIDDEN);
			}
			if (m_pTreasureGauge) {
				m_pTreasureGauge->SetState((FrTreasureGauge::eState)1);
			}
			if (m_pNextHole) {
				m_pNextHole->SetState(FrNextHole::STATE_HIDE);
			}

			// scoreboard.cpp:8155
			if (WSingleton<CSharedDoc>::Instance()->m_roomType == 3 && m_pMatchItems) {
				m_pMatchItems->SetVisible(false);
				m_pMatchItems->Refresh();
			}

			// scoreboard.cpp:8161
			if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
				if (m_pGuildScoreForm) {
					m_pGuildScoreForm->SetVisible(false);
				}
				if (m_unknown2c == 1) {
					m_unknown28 = 1;
				}
			}
		} else if (msg.m_param1 == 1) {
			// scoreboard.cpp:8180
			m_bOpen = 2;
			m_fadeTime = 0;
			if (m_pRankForm) {
				m_pRankForm->SetVisible(false);
			}
			if (m_pRankForm3) {
				m_pRankForm3->SetVisible(m_unknown28 == 1);
			}

			// scoreboard.cpp:8186
			if (IsMassGame()) {
				if (m_pRankFormEx) {
					m_pRankFormEx->SetActive(true);
				}
				if (m_pScoreForm) {
					m_pScoreForm->SetState(FrScoreForm::STATE_HIDDEN);
				}

				// scoreboard.cpp:8241
				if (m_pTreasureGauge) {
					m_pTreasureGauge->SetState((FrTreasureGauge::eState)3);
				}
				if (m_pNextHole) {
					m_pNextHole->SetState((FrNextHole::eState)2);
				}

				// scoreboard.cpp:8252
				if (WSingleton<CSharedDoc>::Instance()->m_roomType == 3 && m_pMatchItems) {
					m_pMatchItems->SetVisible(false);
					m_pMatchItems->Refresh();
				}

				// scoreboard.cpp:8258
				if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
					if (m_pGuildScoreForm) {
						m_pGuildScoreForm->SetVisible(true);
						m_pGuildScoreForm->SetGuildPlayer(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid);
						m_pGuildScoreForm->SetVisibleCloseBtn(false);
						m_pGuildScoreForm->SetVisibleCancelBtn(false);
					}
					m_unknown28 = 0;
				}
			} else {
				// scoreboard.cpp:8273
				if (m_pScoreForm) {
					if (m_noticeImages.size() > 0) {
						const char* name = m_noticeImages[WSingleton<CSharedDoc>::Instance()->GetMyPartTidList().m_tidChar & 0x3FFFFFF].name;
						if (name) {
							m_pScoreForm->SetPPL(MakeStr("%s/%s", SCORE_PPL_DIR, name));
						}
					}

					// scoreboard.cpp:8291
					m_pScoreForm->SetState((FrScoreForm::eState)3);
					m_pScoreForm->SetDesc();
				}

				// scoreboard.cpp:8342
				if (m_pTreasureGauge) {
					m_pTreasureGauge->SetState((FrTreasureGauge::eState)3);
				}
				if (m_pNextHole) {
					m_pNextHole->SetState((FrNextHole::eState)2);
				}

				// scoreboard.cpp:8353
				// TODO scoreboard.cpp:8353-8383 (0x52ddd7): with m_pMatchItems in room 3 and CGolfDoc byte 0x14c
				// clear, shows the form when GetHoleIndex(CGolfDoc byte 0x168) is a non-zero multiple of
				// max(1, CSharedDoc byte 0x476d / 9), else hides it.
			}
		} else if (msg.m_param1 == 4) {
			// scoreboard.cpp:8385
			if (m_pTreasureGauge) {
				m_pTreasureGauge->SetTPoint();
			}
		}

		// scoreboard.cpp:8391
		m_scoreBg.ScoreBgCommand(msg.m_param1);
		break;

	case 0x130:
		// scoreboard.cpp:8398
		m_bOpen = 3;
		m_fadeTime = 1.0f;
		if (m_pRankForm && WSingleton<CSharedDoc>::Instance()->m_roomType != 10) {
			m_pRankForm->SetVisible(true);
		}

		// scoreboard.cpp:8410
		if (m_pRankForm3) {
			m_pRankForm3->SetVisible(m_unknown28 == 1);
		}
		if (m_pRankFormEx) {
			m_pRankFormEx->SetActive(false);
		}
		if (m_pScoreForm) {
			m_pScoreForm->SetState(FrScoreForm::STATE_HIDDEN);
		}
		if (m_pMatchItems) {
			m_pMatchItems->SetVisible(false);
		}
		break;

	case 0x131:
		// scoreboard.cpp:8425
		if (m_pScoreForm) {
			m_pScoreForm->SortLists(true);
		}
		break;

	case 0x132:
	case 0x1A0:
		// scoreboard.cpp:8436
		if (m_pScoreForm) {
			m_pScoreForm->UpdatePang();
		}
		break;

	case 0x133:
		// scoreboard.cpp:8462
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 4 && m_pRankForm) {
			m_pRankForm->m_rankCount--;
			m_pRankForm->m_rankText[msg.m_param1]--;
		}
		break;

	case 0x134:
		// scoreboard.cpp:8475
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 4 && m_pRankForm) {
			unsigned char myRank = WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_rank;
			int score = WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_score + msg.m_param1;
			__int64 pang = WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_pang;
			unsigned char rank = 1;

			// scoreboard.cpp:8485
			for (std::vector<sRivalData>::iterator it = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin();
				it != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++it) {
				if (score > it->m_score || (score == it->m_score && pang < it->m_pang)) {
					rank++;
				}
			}

			// scoreboard.cpp:8491
			int gain = myRank - rank;
			if (gain >= 3) {
				m_pRankForm->m_pBoard->SetCaption(MakeStr("%s\xB4\xD4 \xC0\xCC\xB9\xF8\xBF\xA1 \xC8\xA6\xC0\xCE\xC7\xCF\xB8\xE9 %d\xB8\xED\xC0\xBB \xC0\xCC\xB1\xE6 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.",
					WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().nickname, gain));
				m_pRankForm->m_boardMessage = 1;
				m_pRankForm->SetCaptionType(2);
			}
		}
		break;

	case 0x135:
		// scoreboard.cpp:8501
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 4) {
			if (m_pRankForm) {
				m_pRankForm->m_pBoard->SetCaption("\xC0\xDA~ \xB8\xB6\xC1\xF6\xB8\xB7 \xC8\xA6\xC0\xD4\xB4\xCF\xB4\xD9. \xB8\xB7\xC6\xC7 \xC6\xC4\xC0\xCC\xC6\xC3!");
				m_pRankForm->m_boardMessage = 1;
				m_pRankForm->SetCaptionType(1);
			}

			// scoreboard.cpp:8511
			if (m_pRankForm3) {
				m_pRankForm3->m_pBoard->SetCaption("\xC0\xDA~ \xB8\xB6\xC1\xF6\xB8\xB7 \xC8\xA6\xC0\xD4\xB4\xCF\xB4\xD9. \xB8\xB7\xC6\xC7 \xC6\xC4\xC0\xCC\xC6\xC3!");
				m_pRankForm3->SetCaptionType(1);
			}
		}
		break;

	case 0x136:
		// scoreboard.cpp:8520
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 4 && m_pRankForm) {
			m_pRankForm->m_rankText[msg.m_param1]++;
			m_pRankForm->m_rankText[msg.m_param1 - 1]--;

			// scoreboard.cpp:8526
			unsigned char hole = GOLFDOC()->m_currentHole;
			if (m_pRankForm->m_boardMessage > 2 && m_pRankForm->m_announcedHole != hole) {
				unsigned char maxCount = 0;
				unsigned char maxHole = 0;
				for (int i = 0; i < 0x13; i++) {
					if ((unsigned char)m_pRankForm->m_rankText[i] > maxCount) {
						maxHole = i + 1;
						maxCount = m_pRankForm->m_rankText[i];
					}
				}

				// scoreboard.cpp:8541
				if (GetHoleIndex(maxHole) >= GetHoleIndex(hole) + 5) {
					m_pRankForm->m_announcedHole = hole;
					m_pRankForm->m_pBoard->SetCaption(MakeStr("%s\xB4\xD4 \xB8\xB9\xC0\xCC \xB5\xDA\xC3\xC4\xC1\xAE \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9. \xBC\xAD\xB5\xCE\xB8\xA3\xBC\xBC\xBF\xE4.",
						WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().nickname));
					m_pRankForm->m_boardMessage = 2;
					m_pRankForm->SetCaptionType(2);
				}
			}
		}
		break;

	case 0x137:
		// scoreboard.cpp:8554
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 4 && m_pRankForm) {
			// scoreboard.cpp:8557
			if (m_pRankForm->m_boardMessage > 3) {
				CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
				if ((unsigned long)m_pRankForm->m_rankPage == ((pDoc->GetPangyaUserInfo().identity & 2) ? pDoc->GetPangyaUserInfo().galleryGuid : pDoc->GetPangyaUserInfo().guid)) {
					// scoreboard.cpp:8561
					m_pRankForm->m_pBoard->SetCaption(MakeStr("%s\xB4\xD4 \xB2\xC3\xC2\xEE\xBF\xB9\xBF\xE4. \xC8\xFB\xB3\xBB\xBC\xBC\xBF\xE4.",
						pDoc->m_gameResultList[pDoc->GetIndex((pDoc->GetPangyaUserInfo().identity & 2) ? pDoc->GetPangyaUserInfo().galleryGuid : pDoc->GetPangyaUserInfo().guid)].m_nickname));
					m_pRankForm->m_boardMessage = 3;
					m_pRankForm->SetCaptionType(1);
				}
			}
		}
		break;

	case 0x138:
		// scoreboard.cpp:8569
		if (m_pRankFormEx) {
			*(bool*)msg.m_param1 = m_pRankFormEx->IsMotionFinished();
		} else if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
			// scoreboard.cpp:8572
			*(bool*)msg.m_param1 = m_fadeTime < 1.0f ? false : true;
		} else {
			// scoreboard.cpp:8574
			*(bool*)msg.m_param1 = true;
		}
		break;

	case 0x139: {
		// scoreboard.cpp:8638
		bool* pResult = (bool*)msg.m_param1;
		*pResult = m_bOpen != 0;

		if (m_pRankForm) {
			WRect rect;
			m_pRankForm->GetClientRect(rect);
			if (rect.IsInRect(WPoint(g_input->GetMousePoint().x, g_input->GetMousePoint().y))) {
				*pResult = true;
			}
		}

		// scoreboard.cpp:8648
		if (m_pRankForm3) {
			WRect rect;
			m_pRankForm3->GetClientRect(rect);
			if (rect.IsInRect(WPoint(g_input->GetMousePoint().x, g_input->GetMousePoint().y))) {
				*pResult = true;
			}
		}
		break;
	}

	case 0x13A: {
		// scoreboard.cpp:8663
		CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
		unsigned char par3 = 0;
		unsigned char par4 = 0;
		unsigned char par5 = 0;
		for (int i = pDoc->m_holeOrder[0]; i <= pDoc->m_holeOrder[pDoc->m_holes - 1]; i++) {
			switch (GOLFDOC()->GetHoleData((unsigned char)i).m_kind) {
			case 2:
				par3++;
				break;
			case 3:
				par4++;
				break;
			case 4:
				par5++;
				break;
			}
		}

		// scoreboard.cpp:8685
		int difficulty = GetDifficulty() - 1;
		*(int*)msg.m_param1 = data_9f1f48[difficulty][par5][5] + data_9f1f48[difficulty][par4][2] + data_9f1f48[difficulty][par3][0];
		break;
	}

	case 0x13B:
		// scoreboard.cpp:8705
		if (m_pRankForm2) {
			if (msg.m_param1 == -1) {
				m_pRankForm2->SetVisible(!m_pRankForm2->IsVisible());
			} else if (msg.m_param1 == 0) {
				m_pRankForm2->SetVisible(false);
			} else if (msg.m_param1 == 1) {
				m_pRankForm2->SetVisible(true);
			}
		}
		break;

	case 0x13C:
		// scoreboard.cpp:8729
		UpdateRivals();
		break;

	case 0x57:
		// scoreboard.cpp:8725
		m_pRankForm3->SetScore(msg.m_param1, msg.m_param2);
		break;

	case 0x2A:
		// scoreboard.cpp:8442
		if (IsMassGame()) {
			if (m_pRankForm) {
				m_pRankForm->SortRank();
				if (msg.m_param1) {
					m_pRankForm->Align();
				}
			}
			if (m_pRankForm2) {
				m_pRankForm2->SortRank();
			}
			if (m_pRankFormEx) {
				m_pRankFormEx->SortRank();
			}
		}
		break;

	case 0x271B: {
		// scoreboard.cpp:8582
		WPoint pt;
		if (m_pRankForm) {
			pt.x = g_view->GetWidth() - m_pRankForm->GetRect().w;
			pt.y = 40.0f;
			m_pRankForm->MoveWindow(pt);
		}

		// scoreboard.cpp:8590
		if (m_pRankForm2) {
			pt.x = (g_view->GetWidth() - m_pRankForm2->GetRect().w) * 0.5f;
			pt.y = 40.0f;
			m_pRankForm->MoveWindow(pt);   // the original moves m_pRankForm here
		}

		// scoreboard.cpp:8598
		if (m_pRankForm3) {
			pt.x = g_view->GetWidth() - m_pRankForm3->GetRect().w + 5.0f;
			pt.y = 40.0f;
			m_pRankForm3->MoveWindow(pt);
		}

		// scoreboard.cpp:8606
		if (m_pRankFormEx) {
			pt.x = (float)float2int((g_view->GetWidth() - m_pRankFormEx->GetRect().w) * 0.5f);
			pt.y = (float)float2int((g_view->GetHeight() - m_pRankFormEx->GetRect().h) * 0.5f);
			m_pRankFormEx->MoveWindow(pt);
			m_pRankFormEx->ResetStage();
		}

		// scoreboard.cpp:8615
		if (m_pScoreForm) {
			pt.x = (float)float2int((g_view->GetWidth() - m_pScoreForm->GetRect().w) * 0.5f);
			pt.y = 20.0f;
			m_pScoreForm->MoveWindow(pt);
		}

		// scoreboard.cpp:8623
		if (m_pAwardForm) {
			float barY = WSingleton<COption>::Instance()->gGetBar_Y();
			pt.x = (float)float2int((g_view->GetWidth() - m_pAwardForm->GetRect().w) * 0.5f);
			pt.y = barY - 100.0f;
			m_pAwardForm->MoveWindow(pt);
		}
		break;
	}
	}
}

// scoreboard.cpp:8735
bool CScore::OnLevelupFormResult(int ret, FrForm* pForm) {
	if (m_levelupForms.size() != 0) {
		m_levelupForms[m_levelupForms.size() - 1] = 0;
		m_levelupForms.erase(m_levelupForms.end() - 1);
	}
	return true;
}

// scoreboard.cpp:8746
bool CScore::OnRankFormExResult(int ret, FrForm* pForm) {
	if (m_pRankFormEx) {
		m_pRankFormEx->Clear();
		m_pRankFormEx = 0;
	}
	return true;
}

// scoreboard.cpp:8758
bool CScore::UpdateRivals(void) {
	if (m_pRankForm) {
		FrListBox* pList = m_pRankForm->m_pRankList;

		if (pList) {
			pList->ClearItem();
			for (std::vector<sGameResultInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++it) {
				pList->AddItem(&*it);
			}

			pList->UseRightButton(true);

			m_pRankForm->m_rankCount = WSingleton<CSharedDoc>::Instance()->m_gameResultList.size();
			m_pRankForm->m_rankText[0] = WSingleton<CSharedDoc>::Instance()->m_gameResultList.size();
			m_pRankForm->SortRank();
		}
	}

	FrRankFormEx* pRankEx = m_pRankFormEx;
	if (pRankEx) {
		if (pRankEx->m_rankEx.m_pRival) {
			if (pRankEx->m_rankEx.m_pRival) {
				delete[] pRankEx->m_rankEx.m_pRival;
				pRankEx->m_rankEx.m_pRival = 0;
			}
		}

		m_pRankFormEx->m_rankEx.SetRivalData();
	}

	return true;
}

// scoreboard.cpp:8802
CScore::CScoreBg::CScoreBg(void) {
	m_pImage = 0;
	m_bShow = false;
}

// scoreboard.cpp:8808
CScore::CScoreBg::~CScoreBg(void) {
	if (m_pImage) { delete m_pImage; m_pImage = 0; }
	m_bShow = false;
	g_pFresh->GetManager()->GetDesktop()->LockWallPaper(false);
}

// scoreboard.cpp:8815
void CScore::CScoreBg::SetDesktopWallPaper(bool bShow, bool bDefault) {
	if (m_bShow) {
		g_pFresh->GetManager()->GetDesktop()->SetWallPaperFromBitmap(m_pImage, bShow);
		if (m_pImage) { delete m_pImage; m_pImage = 0; }
	} else if (bDefault && bShow) {
		g_pFresh->GetManager()->GetDesktop()->SetWallPaper("scoreboard_background.jpg", bShow);
	}
}

// scoreboard.cpp:8880
bool CScore::OnGuildScoreFormResult(int ret, FrForm* pForm) {
	if (m_pGuildScoreForm) {
		m_pGuildScoreForm = 0;
	}
	return true;
}

// scoreboard.h:717
CScore::~CScore(void) {
}
