// Original: d:\build\custom\temp\code\client\projectg\ghostgamedlg.cpp
#include <limits.h>
#include <stdlib.h>
#include <algorithm>

#include "GhostGameDlg.h"
#include "ghosttypes.h"
#include "ghostshared.h"
#include "formbar.h"
#include "ObjectFactory.h"
#include "Localize.h"
#include "Packet.h"
#include "bitmap.h"
#include "frarea.h"
#include "frbutton.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "../shared/globalgamedefine.h"
#include "wfont.h"
#include "wresrcmng.h"
#include "frgaugebar.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frstatic.h"
#include "frviewer.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "commonutil.h"
#include "../shared/classdefine.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "TaskManager.h"
#include "actor.h"
#include "UccManager.h"

// The level table (0xa029a0, no symbol; 0x41 levels): the experience each
// level needs and its name.
struct sLevelTableEntry {
	unsigned long m_exp;    // 0x00
	char m_name[16];        // 0x04
};
CHECK_SIZE(sLevelTableEntry, 0x14);
extern "C" const sLevelTableEntry data_a029a0[];

// ghosttypes.h:275
bool GHOST_GAME_BRIEF::operator==(int gameId) const {
	return m_gameId == gameId;
}

// ghostshared.h:43
CGhostGameBonusUtil::GAMEBONUS::GAMEBONUS(int pang, unsigned char rate, int bonus)
	: m_pang(pang), m_rate(rate), m_bonus(bonus) {
}

// ghostshared.h:48
CGhostGameBonusUtil::CGhostGameBonusUtil(void) {
	m_pangBonus.reserve(10);

	m_pangBonus.push_back(GAMEBONUS(-2000, 10, 500));
	m_pangBonus.push_back(GAMEBONUS(0, 20, 1000));
	m_pangBonus.push_back(GAMEBONUS(1000, 30, 1500));
	m_pangBonus.push_back(GAMEBONUS(2000, 40, 2000));
	m_pangBonus.push_back(GAMEBONUS(3000, 50, 2500));
	m_pangBonus.push_back(GAMEBONUS(4000, 60, 3000));
	m_pangBonus.push_back(GAMEBONUS(5000, 70, 3500));
	m_pangBonus.push_back(GAMEBONUS(6000, 80, 4000));
	m_pangBonus.push_back(GAMEBONUS(7000, 100, 5000));
	m_pangBonus.push_back(GAMEBONUS(INT_MAX, 200, 10000));
}

// ghostshared.h:67
CGhostGameBonusUtil::~CGhostGameBonusUtil(void) {
}

// ghostshared.h:76
bool CGhostGameBonusUtil::IsUserWin(int userScore, int userShot, int ghostScore, int ghostShot) {
	int userAbs = abs(userScore);
	int ghostAbs = abs(ghostScore);

	if (userAbs > ghostAbs)
		return true;
	else if (userAbs == ghostAbs && userShot > ghostShot)
		return true;
	else
		return false;
}

// ghostshared.h:87
int CGhostGameBonusUtil::GetOneClickBonus(unsigned char rate, bool win, int basePoint, int holeCount, unsigned char userRank, unsigned char ghostRank) {
	int point = GetBonusPoint(win, basePoint, holeCount, userRank, ghostRank);
	int bonus = m_pangBonus[GetPangBonusIndex(point)].m_rate * rate / 18;
	if (bonus > 1)
		bonus = 1;
	return bonus;
}

// ghostshared.h:102
int CGhostGameBonusUtil::GetBonusPoint(bool win, int basePoint, int holeCount, unsigned char userRank, unsigned char ghostRank) {
	if (holeCount >= 5)
		holeCount = 5;

	return (basePoint + holeCount * 20 + (win ? 10 : -5) * 6) * 50 + abs(userRank - ghostRank) * 20;
}

// ghostshared.h:117
int CGhostGameBonusUtil::GetPangBonusIndex(int pang) {
	int index = 0;
	for (std::vector<GAMEBONUS>::iterator it = m_pangBonus.begin(); it != m_pangBonus.end(); ++it) {
		if (pang <= it->m_pang)
			break;
		++index;
	}
	return index;
}

// formbar.h:41
const WRect& FrFormBar::GetGaugeRect(void) {
	return m_gaugeRect;
}

// ghostgamedlg.cpp:33 (run-time type and factory registration of FrRecentMatch)
const WRTTI FrRecentMatch::m_RTTI("FrRecentMatch", &FrForm::m_RTTI);

IObject* FrRecentMatchMakeInstance(void) {
	return new FrRecentMatch;
}

struct __sFrRecentMatch {
	__sFrRecentMatch(void) {
		ObjectFactory().AddObjectFunctor(FrRecentMatchMakeInstance, "FrRecentMatch");
	}
};
static __sFrRecentMatch s_registerFrRecentMatch;

// The title font's texture file names (a char* table in .data).
extern "C" char* data_a91de0[];

// ghostgamedlg.cpp:34
const sFRESH_MSGMAP* FrRecentMatch::GetMessageMap(void) const {
	return &FrRecentMatch::_MsgMap;
}

sFRESH_MSGMAP FrRecentMatch::_MsgMap = { &FrForm::_MsgMap, &FrRecentMatch::_MsgEntries[0] };

sFRESH_ENTRY FrRecentMatch::_MsgEntries[] = {
	// ghostgamedlg.cpp:36
	{ "bg_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRecentMatch::OnInitBgArea },
	// ghostgamedlg.cpp:37
	{ "bg_area", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrRecentMatch::OnDrawBgArea },
	// ghostgamedlg.cpp:38
	{ "match_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRecentMatch::OnInitHistoryList },
	// ghostgamedlg.cpp:39
	{ "match_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRecentMatch::OnDrawHistoryList },
	// ghostgamedlg.cpp:40
	{ "text1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrRecentMatch::OnInitStaticText1 },

	// ghostgamedlg.cpp:42
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ghostgamedlg.cpp:47
FrRecentMatch::FrRecentMatch(void)
	: m_pCourseImg(0), m_pItemImg(0), m_number1(0), m_number2(0), m_pHistoryList(0),
	m_pBgArea(0), m_pStaticText1(0), m_pTitleFont(0) {
	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = data_a91de0;
	info.numPages = 1;
	info.fontw = 16;
	info.fonth = 16;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ymP-.?/:";
	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);
}

// ghostgamedlg.cpp:64
FrRecentMatch::~FrRecentMatch(void) {
	std::for_each(m_history.begin(), m_history.end(), Delete_Object());
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// ghostgamedlg.cpp:70
void FrRecentMatch::Init(const std::string& ghostNick, int courseId, int itemTypeId, int number1, int number2) {
	m_string140 = ghostNick;

	IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(0x28000000 | ((courseId > 0x7F && courseId != 0xFD) ? courseId - 0x80 : courseId));
	IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(0x4000000 | itemTypeId);

	if (pCourse && pItem) {

		m_pCourseImg = g_pFresh->GetBitmap(MakeStr("ghost_mid_%s", pCourse->m_icon));
		m_pItemImg = g_pFresh->GetBitmap(MakeStr("%s", pItem->m_icon));
		m_pPangImg = g_pFresh->GetBitmap("ghost_pang");

		m_number1 = number1;
		m_number2 = number2;
	}
}

// ghostgamedlg.cpp:90
void FrRecentMatch::AddHistory(const sRecentMatchForm& form) {
	sRecentMatchForm* pForm = new sRecentMatchForm;
	*pForm = form;
	m_history.push_back(pForm);

	m_pHistoryList->AddItem(pForm);
}

// ghostgamedlg.cpp:99
void FrRecentMatch::AddFinish(void) {
	m_pStaticText1->SetCaption("");
	int count = m_pHistoryList->GetCurrentItemSize(true);
	for (int i = count; i < 5; i++) {
		m_pHistoryList->AddItem(0);
	}
}

// ghostgamedlg.cpp:109
void FrRecentMatch::Exception(void) {
	// "Too many requests to the server. Please try again later."
	m_pStaticText1->SetCaption("\xbc\xad\xb9\xf6\xc0\xc7 \xbf\xe4\xc3\xbb\xc0\xcc \xb8\xb9\xbd\xc0\xb4\xcf\xb4\xd9. \xc0\xe1\xbd\xc3\xc8\xc4 \xb4\xd9\xbd\xc3 \xc0\xcc\xbf\xeb\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.");
}

// ghostgamedlg.cpp:116
void FrRecentMatch::OnInitBgArea(intptr_t param) {
	m_pBgArea = DYNAMIC_CAST(FrArea, param);
}

// ghostgamedlg.cpp:123
void FrRecentMatch::OnDrawBgArea(void) {
	const WRect& areaRect = m_pBgArea->GetRect();
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI && m_pCourseImg && m_pItemImg && m_pPangImg) {

		float x = areaRect.x + 20.0f;
		float y = areaRect.y + 7.0f;

		WRect rect(x, y, (float)m_pCourseImg->Width(), (float)m_pCourseImg->Height());
		pGDI->DrawTexture(m_pCourseImg, rect, 0xffffffff, 0);

		float textX = rect.w + x + 20.0f;

		pGDI->SetTextStyle(4);
		pGDI->SetTextColor(0xff000000, 0xffc8c8c8);
		WPoint pt(textX + 60.0f, y + 5.0f);
		g_pFresh->GetManager()->PrintText(pt, 0, m_string140.c_str(), -1.0f, 0xffffffff);
		pGDI->SetTextStyle(0);
		pGDI->SetTextColor(0xff000000, 0xffffffff);

		WRect itemRect(textX - 5.0f, y + 25.0f, (float)m_pItemImg->Width(), (float)m_pItemImg->Height());
		pGDI->DrawTexture(m_pItemImg, itemRect, 0xffffffff, 0);

		float textY = y + 35.0f;
		m_pTitleFont->Print(g_view, textX, textY, MakeStr("-%d", m_number1), 0, 0xffffffff, 0);

		textY = y + 55.0f;
		float width = m_pTitleFont->Print(g_view, textX, textY, MakeStr("%d", m_number2), 0, 0xffffffff, 0);

		WRect pangRect(width + textX + 5.0f, textY, (float)m_pPangImg->Width(), (float)m_pPangImg->Height());
		pGDI->DrawTexture(m_pPangImg, pangRect, 0xffffffff, 0);
	}
}

// ghostgamedlg.cpp:156
void FrRecentMatch::OnInitHistoryList(intptr_t param) {
	m_pHistoryList = DYNAMIC_CAST(FrListBox, param);
}

// ghostgamedlg.cpp:163
void FrRecentMatch::OnDrawHistoryList(intptr_t param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	FrListItem* pItem = (FrListItem*)param;
	if (pItem && pGDI) {

		pGDI->SetTextStyle(0);
		pGDI->SetTextColor(0xff000000, 0xffffffff);

		float x = pItem->pos.x;
		float y = pItem->pos.y + 8.0f;

		sRecentMatchForm* pForm = (sRecentMatchForm*)pItem->pData;
		if (pForm) {

			const Bitmap* pSexBitmap;
			if (pForm->m_byte00)
				pSexBitmap = g_pFresh->RegisterBitmap("i_female");
			else
				pSexBitmap = g_pFresh->RegisterBitmap("i_male");
			if (pSexBitmap) {

				WRect sexRect(x + 3.0f, pItem->pos.y, (float)pSexBitmap->Width(), (float)pSexBitmap->Height());
				pGDI->DrawTexture(pSexBitmap, sexRect, 0xffffffff, 0);
				x = (float)pSexBitmap->Width() + x + 10.0f;
			}

			g_pFresh->GetManager()->PrintText11(WPoint(x, y), 0, pForm->m_name.c_str(), -1.0f, 0xffffffff);
			x += 130.0f;

			pGDI->SetTextColor(pForm->m_int20 ? 0xff1c1c1c : 0xff999999, 0xffffffff);

			// "win" / "lose"
			g_pFresh->GetManager()->PrintText11(WPoint(x, y), 0, pForm->m_int20 ? "��" : "��", -1.0f, 0xffffffff);
			x += 30.0f;

			pGDI->SetTextColor(0xff000000, 0xffffffff);

			const Bitmap* pGradeBitmap = g_pFresh->GetBitmap(MakeStr("ghost_grade_%d", pForm->m_byte24));
			if (pGradeBitmap) {

				WRect gradeRect(x - 10.0f, y - 2.0f, (float)pGradeBitmap->Width(), (float)pGradeBitmap->Height());
				pGDI->DrawTexture(pGradeBitmap, gradeRect, 0xffffffff, 0);
			}
			x += 35.0f;

			WRect pangRect(x, y - 1.0f, (float)m_pPangImg->Width(), (float)m_pPangImg->Height());
			pGDI->DrawTexture(m_pPangImg, pangRect, 0xffffffff, 0);
			x = (float)m_pPangImg->Width() + x + 5.0f;

			pGDI->Print11(WPoint(x, y), 0, "%d", pForm->m_int38);
			x += 50.0f;

			g_pFresh->GetManager()->PrintText11(WPoint(x, y), 0, MakeStr("%02d.%02d.%02d", pForm->m_time.wYear, pForm->m_time.wMonth, pForm->m_time.wDay), -1.0f, 0xffffffff);
		} else {

			pGDI->SetTextStyle(4);
			pGDI->SetTextColor(0xffc8c8c8, 0xff000000);
			x += 55.0f;
			// "no record"
			pGDI->Print(WPoint(x, y), 1, "��� ����");
			x += 105.0f;
			m_pTitleFont->Print(g_view, x, y, "-", 0, 0xffffffff, 0);
			x += 40.0f;
			m_pTitleFont->Print(g_view, x, y, "-", 0, 0xffffffff, 0);
			x += 65.0f;
			m_pTitleFont->Print(g_view, x, y, "-", 0, 0xffffffff, 0);
			x += 70.0f;
			m_pTitleFont->Print(g_view, x, y, "-", 0, 0xffffffff, 0);
		}

		pGDI->SetTextStyle(0);
		pGDI->SetTextColor(0xff000000, 0xffffffff);
	}
}

// ghostgamedlg.cpp:238
void FrRecentMatch::OnInitStaticText1(intptr_t param) {
	m_pStaticText1 = DYNAMIC_CAST(FrStatic, param);
	// "Requesting the information from the server."
	m_pStaticText1->SetCaption("\xbc\xad\xb9\xf6\xbf\xa1 \xc1\xa4\xba\xb8\xb8\xa6 \xbf\xe4\xc3\xbb\xc7\xcf\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
}

// ghostgamedlg.cpp:252 (run-time type and factory registration of FrGhostGameMapDlg)
const WRTTI FrGhostGameMapDlg::m_RTTI("FrGhostGameMapDlg", &FrForm::m_RTTI);

IObject* FrGhostGameMapDlgMakeInstance(void) {
	return new FrGhostGameMapDlg;
}

struct __sFrGhostGameMapDlg {
	__sFrGhostGameMapDlg(void) {
		ObjectFactory().AddObjectFunctor(FrGhostGameMapDlgMakeInstance, "FrGhostGameMapDlg");
	}
};
static __sFrGhostGameMapDlg s_registerFrGhostGameMapDlg;

// ghostgamedlg.cpp:253
const sFRESH_MSGMAP* FrGhostGameMapDlg::GetMessageMap(void) const {
	return &FrGhostGameMapDlg::_MsgMap;
}

sFRESH_MSGMAP FrGhostGameMapDlg::_MsgMap = { &FrForm::_MsgMap, &FrGhostGameMapDlg::_MsgEntries[0] };

sFRESH_ENTRY FrGhostGameMapDlg::_MsgEntries[] = {
	// ghostgamedlg.cpp:255
	{ "no", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrGhostGameMapDlg::OnDownCancel },
	// ghostgamedlg.cpp:256
	{ "yes", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrGhostGameMapDlg::OnDownOK },
	// ghostgamedlg.cpp:257
	{ "recent", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostGameMapDlg::OnInitRecentMatch },
	// ghostgamedlg.cpp:258
	{ "recent", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrGhostGameMapDlg::OnDownRecentMatch },
	// ghostgamedlg.cpp:259
	{ "bg_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostGameMapDlg::OnInitAreaBg },
	// ghostgamedlg.cpp:260
	{ "bg_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostGameMapDlg::OnDrawAreaBg },
	// ghostgamedlg.cpp:261
	{ "map_select", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostGameMapDlg::OnInitSelMapList },
	// ghostgamedlg.cpp:262
	{ "map_select", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostGameMapDlg::OnDrawSelMapList },
	// ghostgamedlg.cpp:263
	{ "map_select", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrGhostGameMapDlg::OnDownSelMapList },
	// ghostgamedlg.cpp:264
	{ "choice_text", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostGameMapDlg::OnInitAreaSelecText },
	// ghostgamedlg.cpp:265
	{ "choice_text", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrGhostGameMapDlg::OnDrawSelecText },
	// ghostgamedlg.cpp:266
	{ "choice_map", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostGameMapDlg::OnInitAreaMap },
	// ghostgamedlg.cpp:267
	{ "choice_map", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostGameMapDlg::OnDrawChoiceMap },

	// ghostgamedlg.cpp:269
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ghostgamedlg.cpp:272
void FrGhostGameMapDlg::sImgTag::Animate(float dt) {
	if (m_alpha < 255.0f) {
		m_alpha += dt * 204.0f;
		if (m_alpha > 255.0f)
			m_alpha = 255.0f;
	}

	if (m_alphaEnd < 255.0f && m_grow + m_base > m_current) {
		m_alphaEnd += dt * 357.0f;
		if (m_alphaEnd > 255.0f)
			m_alphaEnd = 255.0f;
	}

	if (m_base < m_current) {
		m_current -= (800.0f - m_base) * dt * 1.5f;
		if (m_current < m_base)
			m_current = m_base;
	}
}

// ghostgamedlg.cpp:296
void FrGhostGameMapDlg::sImgTag::Reset(void) {
	m_alphaEnd = 0.0f;
	m_alpha = 0.0f;
	m_current = 800.0f;
}

// ghostgamedlg.cpp:303
void FrGhostGameMapDlg::sImgInfo::AnimateLock(float dt) {
	if (m_flag12) {
		if (!m_flag10) {
			if (m_lockOffset < 200.0f) {
				m_lockOffset += dt * 400.0f;
				if (m_lockOffset > 200.0f)
					m_lockOffset = 200.0f;
			}
		}
	}
}

// ghostgamedlg.cpp:320
FrGhostGameMapDlg::FrGhostGameMapDlg(void)
	: m_pSelMapList(NULL), m_selectId(0), m_pSelecTextArea(NULL), m_selecTextAlpha(0), m_pRecentBtn(NULL) {
	m_imgInfos.reserve(20);
	m_imgTags.reserve(20);

	for (int i = 0; i < 20; i++) {
		sImgInfo info(0.0f, 0.0f); info.m_mapId = 20; m_imgInfos.push_back(info);

		sImgTag tag;
		memset(&tag, 0, sizeof(tag));
		m_imgTags.push_back(tag);
	}
}

// ghostgamedlg.cpp:337
FrGhostGameMapDlg::~FrGhostGameMapDlg(void) {
}

// ghostgamedlg.cpp:341
void* FrGhostGameMapDlg::GetSelectItem(void) {
	std::list<GHOST_GAME_BRIEF>::iterator it = std::find(m_gameBriefs.begin(), m_gameBriefs.end(), m_selectId);
	if (it != m_gameBriefs.end())
		return &(*it);

	return NULL;
}

// ghostgamedlg.cpp:350
void FrGhostGameMapDlg::Init(unsigned long ghostUserId, const std::string& ghostNick, const std::list<GHOST_GAME_BRIEF>& briefs) {
	if (ghostUserId == WSingleton<CSharedDoc>::Instance()->m_myUID) {

		m_pRecentBtn->SetVisible(true);
		m_pRecentBtn->Enable(true);
	}

	m_gameBriefs = briefs;
	m_ghostUserId = ghostUserId;
	m_ghostNick = ghostNick;

	if (m_pSelMapList) {

		std::list<int> playableMaps;
		std::list<int> lockedMaps;

		for (int mapId = 0x13; mapId >= 0; mapId--) {

			if (mapId == 0x11 || mapId == 0xc)
				continue;

			IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(0x28000000 | ((mapId > 0x7F && mapId != 0xFD) ? mapId - 0x80 : mapId));
			if (pCourse == 0)
				continue;

			sImgInfo info(-100.0f, 0.0f);
			info.m_mapId = mapId;

			std::list<GHOST_GAME_BRIEF>::iterator it = std::find(m_gameBriefs.begin(), m_gameBriefs.end(), mapId);
			if (it != m_gameBriefs.end()) {

				if (it->m_hasData) {

					playableMaps.push_back(mapId);
					info.m_flag10 = true;
				} else {

					info.m_flag11 = true;
					lockedMaps.push_back(mapId);
				}
			} else {

				lockedMaps.push_back(mapId);
				info.m_flag10 = false;
			}

			m_imgInfos[mapId] = info;

			sImgTag tag;
			tag.m_pBitmap = g_pFresh->GetBitmap(MakeStr("30in_back_%s", pCourse->m_icon));
			if (tag.m_pBitmap) {

				WRect rect(m_mapRect.x, m_mapRect.y, (float)tag.m_pBitmap->Width(), (float)tag.m_pBitmap->Height());
				tag.m_rect = rect;
			}

			tag.m_pEndBitmap = g_pFresh->GetBitmap("ghost_mapdlg_info");
			if (tag.m_pEndBitmap) {

				tag.m_base = m_mapRect.x + 1.0f;
				tag.m_endY = m_mapRect.y + 6.0f;
				tag.m_grow = (float)tag.m_pEndBitmap->Width();
				tag.m_endH = (float)tag.m_pEndBitmap->Height();
			}

			tag.m_alphaEnd = 0;
			tag.m_alpha = 0;
			tag.m_current = 800.0f;
			m_imgTags[mapId] = tag;
		}

		if (playableMaps.size() != 0)
			m_selectId = playableMaps.front();
		else
			m_selectId = 0x13;

		for (std::list<int>::iterator it = playableMaps.begin(); it != playableMaps.end(); ++it) {

			IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(0x28000000 | ((*it > 0x7F && *it != 0xFD) ? *it - 0x80 : *it));
			if (pCourse)
				m_pSelMapList->AddItem(pCourse);
		}

		for (std::list<int>::iterator it = lockedMaps.begin(); it != lockedMaps.end(); ++it) {

			IFF_STRUCT::sCourse* pCourse = ItemManager()->FindCourse(0x28000000 | ((*it > 0x7F && *it != 0xFD) ? *it - 0x80 : *it));
			if (pCourse)
				m_pSelMapList->AddItem(pCourse);
		}
	}
}

// ghostgamedlg.cpp:450
void FrGhostGameMapDlg::OnProc(const float dt) {
	for (std::vector<sImgInfo>::iterator it = m_imgInfos.begin(); it != m_imgInfos.end(); ++it) {
		if (it->m_mapId == 20)
			continue;
		if (!it->m_flag18)
			continue;

		if (it->m_y > it->m_x) {
			it->m_x += (it->m_y + 100.0f) * dt * 1.3f;
			if (it->m_x > it->m_y)
				it->m_x = it->m_y;
		}

		if (it->m_unknown08 < 255.0f && it->m_x > 0.0f) {
			it->m_unknown08 += dt * 331.0f;
			if (it->m_unknown08 > 255.0f)
				it->m_unknown08 = 255.0f;
		}

		if (it->m_x == it->m_y)
			it->m_flag12 = true;

		if (it->m_flag12 && !it->m_flag10 && it->m_lockOffset < 200.0f) {
			it->m_lockOffset += dt * 400.0f;
			if (it->m_lockOffset > 200.0f)
				it->m_lockOffset = 200.0f;
		}
	}

	m_imgTags[m_selectId].Animate(dt);

	if (m_selecTextAlpha < 255.0f) {
		m_selecTextAlpha += dt * 127.5f;
		if (m_selecTextAlpha >= 255.0f)
			m_selecTextAlpha = 255.0f;
	}

	FrForm::OnProc(dt);
}

// ghostgamedlg.cpp:494
void FrGhostGameMapDlg::OnDownOK(void) {
}

// ghostgamedlg.cpp:499
void FrGhostGameMapDlg::OnDownCancel(void) {
	if (IsLocalContent((localContentType_t)0x59)) {

		WSendPacket packet((enumClientPacket)0xB9);
		packet.Encode1(1);
		packet.Send(SEND_TO_DEFAULT);
	}

	FrForm::Close(true);
}

// ghostgamedlg.cpp:511
void FrGhostGameMapDlg::OnDownRecentMatch(void) {
	FrRecentMatch* pForm = CreateForm<FrRecentMatch>(g_pFresh->GetManager(), 0, "GHOST_HISTORY", 0);

	std::list<GHOST_GAME_BRIEF>::iterator it = std::find(m_gameBriefs.begin(), m_gameBriefs.end(), m_selectId);
	if (it != m_gameBriefs.end()) {

		pForm->Init(m_ghostNick, m_selectId, it->m_fileNameParts[0], it->m_fileNameParts[1], it->m_fileNameParts[2]);
		pForm->SetTopmost(true);
		pForm->FindNextTopFocus(false);

		pForm->Open(0, 1);

		WSendPacket packet((enumClientPacket)0xBA);
		packet.Encode1(9);
		packet.Encode1(m_selectId);
		packet.Encode4(it->m_fileNameParts[3]);
		packet.Encode4(m_ghostUserId);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// ghostgamedlg.cpp:535
void FrGhostGameMapDlg::OnInitRecentMatch(intptr_t param) {
	m_pRecentBtn = DYNAMIC_CAST(FrButton, param);
	m_pRecentBtn->SetVisible(false);
}

// ghostgamedlg.cpp:543
void FrGhostGameMapDlg::OnInitSelMapList(intptr_t param) {
	m_pSelMapList = DYNAMIC_CAST(FrListBox, param);
}

// ghostgamedlg.cpp:549
void FrGhostGameMapDlg::OnDownSelMapList(void) {
	FrListItem* pItem = m_pSelMapList->GetSelected();
	if (pItem == NULL)
		return;

	IFF_STRUCT::sCourse* pCourse = (IFF_STRUCT::sCourse*)pItem->pData;
	if (pCourse == NULL)
		return;

	int index = pCourse->m_typeId & 0x3FFFFFF;
	sImgInfo& info = m_imgInfos[index];
	if (info.m_flag10 != false) {

		m_selectId = index;
		m_imgTags[index].Reset();
	}
}

// ghostgamedlg.cpp:569
void FrGhostGameMapDlg::OnDrawSelMapList(intptr_t param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	FrListItem* pItem = (FrListItem*)param;
	if (pItem && pGDI) {

		IFF_STRUCT::sCourse* pCourse = (IFF_STRUCT::sCourse*)pItem->pData;
		if (pCourse) {

			const Bitmap* pBitmap = g_pFresh->GetBitmap(MakeStr("ghost_mid_%s", pCourse->m_icon));
			if (pBitmap) {

				int index = pCourse->m_typeId & 0x3FFFFFF;
				sImgInfo& info = m_imgInfos[index];
				info.m_flag18 = true;
				info.m_y = pItem->pos.x;

				WRect rect(info.m_x, pItem->pos.y + 6.0f, (float)pBitmap->Width(), (float)pBitmap->Height());
				pGDI->DrawTexture(pBitmap, rect, ((unsigned char)info.m_unknown08 << 24) | 0xffffff, 0);

				if (m_selectId == index) {

					const Bitmap* pSelBitmap = g_pFresh->GetBitmap("ghost_mid_select_o");
					if (pSelBitmap) {

						WRect selRect(pItem->pos.x, pItem->pos.y + 6.0f, (float)pSelBitmap->Width(), (float)pSelBitmap->Height());
						pGDI->DrawTexture(pSelBitmap, selRect, ((unsigned char)info.m_unknown08 << 24) | 0xffffff, 0);
					}
				}

				if (info.m_flag10 == false) {

					const Bitmap* pLockBitmap;
					if (info.m_flag11)
						pLockBitmap = g_pFresh->GetBitmap("ghost_mid_map_lock");
					else
						pLockBitmap = g_pFresh->GetBitmap("ghost_mid_map_nodata");

					if (pLockBitmap) {

						WRect lockRect(info.m_x, pItem->pos.y + 6.0f, (float)pLockBitmap->Width(), (float)pLockBitmap->Height());
						pGDI->DrawTexture(pLockBitmap, lockRect, ((unsigned char)info.m_lockOffset << 24) | 0xffffff, 0);
					}
				}
			}
		}
	}
}

// ghostgamedlg.cpp:623
void FrGhostGameMapDlg::OnInitAreaSelecText(intptr_t param) {
	m_pSelecTextArea = DYNAMIC_CAST(FrArea, param);
}

// ghostgamedlg.cpp:629
void FrGhostGameMapDlg::OnDrawSelecText(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {

		// The tip image is drawn at its own size over the "choice_text" area.
		const Bitmap* pBitmap = g_pFresh->GetBitmap("ghost_map_select_tip");
		if (pBitmap) {

			WRect rect = m_pSelecTextArea->GetRect();
			rect.w = (float)pBitmap->Width();
			rect.h = (float)pBitmap->Height();
			pGDI->DrawTexture(pBitmap, rect, ((unsigned char)m_selecTextAlpha << 24) | 0xffffff, 0);
		}
	}
}

// ghostgamedlg.cpp:646
void FrGhostGameMapDlg::OnInitAreaMap(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea) {

		m_mapRect = pArea->GetRect();
	}
}

// ghostgamedlg.cpp:656
void FrGhostGameMapDlg::OnDrawChoiceMap(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {

		sImgTag& tag = m_imgTags[m_selectId];
		if (tag.m_pBitmap) {
			pGDI->DrawTexture(tag.m_pBitmap, tag.m_rect, ((unsigned char)tag.m_alpha << 24) | 0xffffff, 0);
		}

		if (tag.m_pEndBitmap) {

			float left = tag.m_grow + tag.m_rect.x + 1.0f;
			float cur = tag.m_current;
			if (cur < left) {

				WRect src(0.0f, 0.0f, left - cur, tag.m_endH);
				WRect dest(cur, tag.m_endY, left - cur, tag.m_endH);
				pGDI->DrawTexture(tag.m_pEndBitmap, src, dest, ((unsigned char)tag.m_alphaEnd << 24) | 0xffffff, 0);

				std::list<GHOST_GAME_BRIEF>::iterator it = std::find(m_gameBriefs.begin(), m_gameBriefs.end(), m_selectId);
				if (it != m_gameBriefs.end()) {

					IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(0x4000000 | it->m_fileNameParts[0]);
					const Bitmap* pItemBitmap = g_pFresh->GetBitmap(MakeStr("%s", pItem->m_icon));
					if (pItemBitmap) {

						WRect itemRect(cur + 7.0f, tag.m_endY + 6.0f, (float)pItemBitmap->Width(), (float)pItemBitmap->Height());
						pGDI->DrawTexture(pItemBitmap, itemRect, ((unsigned char)tag.m_alphaEnd << 24) | 0xffffff, 0);
					}

					if (tag.m_alphaEnd >= 184.0f) {

						pGDI->SetTextStyle(2);
						pGDI->SetTextColor(0xffffffff, 0xff000000);

						g_pFresh->GetManager()->PrintText(WPoint(tag.m_rect.x + 108.0f, tag.m_rect.y + 19.0f), 0, m_ghostNick.c_str(), -1.0f, 0xffffffff);
						pGDI->Print(WPoint(tag.m_rect.x + 108.0f, tag.m_rect.y + 37.0f), 0, "%d", -it->m_fileNameParts[1]);
						pGDI->Print(WPoint(tag.m_rect.x + 108.0f, tag.m_rect.y + 52.0f), 0, "%d", it->m_fileNameParts[2]);

						const Bitmap* pPangBitmap = g_pFresh->GetBitmap("ghost_pang");
						if (pPangBitmap) {

							WRect pangRect(tag.m_rect.x + 110.0f, tag.m_rect.y + 52.0f, (float)pPangBitmap->Width(), (float)pPangBitmap->Height());
							pGDI->DrawTexture(pPangBitmap, pangRect, 0xffffffff, 0);
						}

						pGDI->SetTextStyle(0);
						pGDI->SetTextColor(0xff000000, 0xffffffff);
					}
				}
			}
		}
	}
}

// ghostgamedlg.cpp:720
void FrGhostGameMapDlg::OnDrawAreaBg(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {

		WRect rect = GetRect();

		pGDI->SetTextStyle(2);
		pGDI->SetTextColor(0xffffffff, 0xff000000);
		// "Ghost data may be deleted at any time."
		pGDI->PrintText(WPoint(rect.x + 45.0f, rect.y + rect.h - 40.0f), 0, "\xb0\xed\xbd\xba\xc6\xae \xb5\xa5\xc0\xcc\xc5\xcd\xb4\xc2 \xc0\xd3\xc0\xc7\xb7\xce \xbb\xe8\xc1\xa6 \xb5\xc9 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", 0);

		pGDI->SetTextStyle(0);
		pGDI->SetTextColor(0xff000000, 0xff000000);
	}
}

// ghostgamedlg.cpp:736
void FrGhostGameMapDlg::OnInitAreaBg(intptr_t param) {
	FrViewer* pViewer = DYNAMIC_CAST(FrViewer, param);
	if (pViewer) {

		pViewer->Open("ghost_mapdlg_bg.jpg");
		pViewer->SetAlpha(0.8f);
	}
}

// ghostgamedlg.cpp:750 (run-time type and factory registration of FrGhostTimeBar)
const WRTTI FrGhostTimeBar::m_RTTI("FrGhostTimeBar", &FrFormBar::m_RTTI);

IObject* FrGhostTimeBarMakeInstance(void) {
	return new FrGhostTimeBar;
}

struct __sFrGhostTimeBar {
	__sFrGhostTimeBar(void) {
		ObjectFactory().AddObjectFunctor(FrGhostTimeBarMakeInstance, "FrGhostTimeBar");
	}
};
static __sFrGhostTimeBar s_registerFrGhostTimeBar;

// ghostgamedlg.cpp:751
const sFRESH_MSGMAP* FrGhostTimeBar::GetMessageMap(void) const {
	return &FrGhostTimeBar::_MsgMap;
}

sFRESH_MSGMAP FrGhostTimeBar::_MsgMap = { &FrFormBar::_MsgMap, &FrGhostTimeBar::_MsgEntries[0] };

sFRESH_ENTRY FrGhostTimeBar::_MsgEntries[] = {
	// ghostgamedlg.cpp:753
	{ "gauge_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFormBar::OnInitGaugeArea },
	// ghostgamedlg.cpp:754
	{ "gauge_area", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrFormBar::OnDrawGaugeArea },
	// ghostgamedlg.cpp:755
	{ "mouse_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostTimeBar::OnInitMouseArea },
	// ghostgamedlg.cpp:756
	{ "mouse_area", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrGhostTimeBar::OnDrawMouseArea },

	// ghostgamedlg.cpp:758
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ghostgamedlg.cpp:763
FrGhostTimeBar::FrGhostTimeBar(void)
	: m_gaugePos(0), m_timeOut(false), m_pMouseArea(0), m_pClickImg(0) {
	m_time = 4.0f;
	m_closeDelay = 1.0f;
}

// ghostgamedlg.cpp:768
FrGhostTimeBar::~FrGhostTimeBar(void) {
}

// ghostgamedlg.cpp:774
void FrGhostTimeBar::OnInitMouseArea(intptr_t param) {
	m_pMouseArea = DYNAMIC_CAST(FrArea, param);
}

// ghostgamedlg.cpp:781
void FrGhostTimeBar::OnDrawMouseArea(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {

		if (m_pClickImg) {

			WPoint pt(GetRect().x, GetRect().y);
			pGDI->DrawTexture(m_pClickImg, WRect(pt.x + 256.0f, pt.y + 18.0f, m_pClickImg->Width(), m_pClickImg->Height()), 0xffffffff, 0);
		}
	}
}

// ghostgamedlg.cpp:794
void FrGhostTimeBar::OnProc(const float dt) {
	FrFormBar::OnProc(dt);

	if (m_timeOut) {

		m_closeDelay -= dt;
		if (m_closeDelay < 0.0f)
			OnCancel();
		return;
	}

	m_time -= dt;
	if (m_time < 0.0f) {

		m_time = 0.0f;
		m_timeOut = true;

		m_pClickImg = g_pFresh->GetBitmap("ghost_bonus_nclick");
		SetPos(0.0f);
		return;
	}

	m_gaugePos = m_time * 25.0f;
	SetPos(m_gaugePos);

	int state = 0;
	if (g_input->GetButton(LEFT_BUTTON) == 3)
		state = 4;
	if (g_input->GetButton(LEFT_BUTTON) == 1)
		state |= 1;
	if (g_input->GetButton(RIGHT_BUTTON) == 3)
		state |= 8;
	if (g_input->GetButton(RIGHT_BUTTON) == 1)
		state |= 2;

	if (state) {

		if ((state & 3) == 0) {

			SetGaugeImg("formbar_gauge_n");
		}
		else {

			g_audio->PlaySfx(MakeStr("pang_coin_%d", rand() % 3 + 1), 0, 0, 0, 0, 0.5f, 200.0f);
			SetGaugeImg("formbar_gauge_o");
		}

		if (state & 4) {

			if (state & 8)
				m_pClickImg = g_pFresh->GetBitmap("ghost_bonus_aclick");
			else
				m_pClickImg = g_pFresh->GetBitmap("ghost_bonus_lclick");
		}
		else {

			m_pClickImg = g_pFresh->GetBitmap("ghost_bonus_rclick");
		}
	}
	else {

		SetGaugeImg("formbar_gauge_n");
		m_pClickImg = g_pFresh->GetBitmap("ghost_bonus_nclick");
	}
}

// ghostgamedlg.cpp:874
bool FrGhostTimeBar::Open(FRESH_RESULT_CALLBACK pfnResult, unsigned long flag) {
	SetFixedWnd();
	bool ret = FrFormBar::Open(pfnResult, flag);

	WPoint pt(GetGaugeRect().x, GetGaugeRect().y);
	MoveBar(WPoint(pt.x - 47.0f, pt.y - 26.0f));
	return ret;
}

// ghostgamedlg.cpp:890 (run-time type and factory registration of FrGhostAward)
const WRTTI FrGhostAward::m_RTTI("FrGhostAward", &FrForm::m_RTTI);

IObject* FrGhostAwardMakeInstance(void) {
	return new FrGhostAward;
}

struct __sFrGhostAward {
	__sFrGhostAward(void) {
		ObjectFactory().AddObjectFunctor(FrGhostAwardMakeInstance, "FrGhostAward");
	}
};
static __sFrGhostAward s_registerFrGhostAward;

// ghostgamedlg.cpp:891
const sFRESH_MSGMAP* FrGhostAward::GetMessageMap(void) const {
	return &FrGhostAward::_MsgMap;
}

sFRESH_MSGMAP FrGhostAward::_MsgMap = { &FrForm::_MsgMap, &FrGhostAward::_MsgEntries[0] };

sFRESH_ENTRY FrGhostAward::_MsgEntries[] = {
	// ghostgamedlg.cpp:893
	{ "text_desc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostAward::OnInitDescText },
	// ghostgamedlg.cpp:894
	{ "gauge_exp", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostAward::OnInitGaugeBar },
	// ghostgamedlg.cpp:895
	{ "gauge_text", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostAward::OnInitGaugeText },
	// ghostgamedlg.cpp:896
	{ "bg_pouch", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostAward::OnInitPouchArea },
	// ghostgamedlg.cpp:897
	{ "bg_score", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostAward::OnDrawScore },
	// ghostgamedlg.cpp:898
	{ "bg_pouch", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGhostAward::OnDrawPouch },

	// ghostgamedlg.cpp:900
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ghostgamedlg.cpp:910
FrGhostAward::cPangFx::cPangFx(void) {
}

// ghostgamedlg.cpp:911
FrGhostAward::cPangFx::~cPangFx(void) {
}

// ghostgamedlg.cpp:913
bool FrGhostAward::cPangFx::IsAlive(void) {
	return m_rect.y <= m_targetY;
}

// ghostgamedlg.cpp:916
void FrGhostAward::cPangFx::Create(const WRect& rect) {
	m_pBitmap = g_pFresh->GetBitmap(MakeStr("ghost_pang_0%d", rand() % 5 + 1));

	float offset = (float)(rand() % (int)(rect.w - (float)m_pBitmap->Width() * 2.5f));

	unsigned long w = m_pBitmap->Width();
	unsigned long h = m_pBitmap->Height();
	m_rect = WRect((float)w * 0.8f + rect.x + offset, (float)(rand() % 5 - 5) + rect.y, (float)w, (float)h);

	m_speed = (float)(rand() % 50 + 200) * 0.01f;
	m_alpha = 0.0f;
	m_targetY = rect.y + 30.0f;
}

// ghostgamedlg.cpp:926
void FrGhostAward::cPangFx::Proc(float dt) {
	if (m_alpha < 255.0f) {
		m_alpha += dt * 1020.0f;
		if (m_alpha >= 255.0f)
			m_alpha = 255.0f;
	}

	if (m_rect.y < m_targetY)
		m_rect.y += dt * m_speed * 30.0f;
}

// ghostgamedlg.cpp:939
void FrGhostAward::cPangFx::Render(FrGraphicInterface* pGI) {
	if (m_pBitmap)
		pGI->DrawTexture(m_pBitmap, m_rect, ((unsigned char)m_alpha << 24) | 0xffffff, 0);
}

// ghostgamedlg.cpp:948
FrGhostAward::FrGhostAward(void)
	: m_pTimeBar(0), m_pPouchArea(0), m_pExpGauge(0), m_pDescText(0), m_pGaugeText(0),
	m_calcBonusTime(0), m_clickCount(0), m_autoBonus(0), m_clickBonus(10), m_basePang(0),
	m_shownBasePang(0), m_gainExp(0), m_remainExp(0) {
	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = &data_a91de0[1];
	info.numPages = 1;
	info.fontw = 16;
	info.fonth = 16;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:,";
	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);
}

// ghostgamedlg.cpp:965
FrGhostAward::~FrGhostAward(void) {
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
	std::for_each(m_pangFxList.begin(), m_pangFxList.end(), Delete_Object());
}

// ghostgamedlg.cpp:1085
void FrGhostAward::SetResultInfo(sResultInfo& info, int index) {
	info.m_isGhost = index == 1;
	info.m_exp = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[index].m_value155;
	info.m_name = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[index].m_name;
	int level = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[index].m_level + 1;
	info.m_levelImage = MakeStr("level_%03d", level < 0x47 ? level : 0x47);
	info.m_gameResult = GOLFDOC()->GetPlayer(index)->m_ghostResult;
}

// ghostgamedlg.cpp:1094
bool FrGhostAward::IsFinishCalcBonus(void) {
	return m_calcBonusTime > 4.0f;
}

// ghostgamedlg.cpp:1099
void FrGhostAward::OnProc(const float dt) {
	static float s_pangTimer = 0.0f;
	static float s_pangInterval = 0.3f;

	s_pangTimer += dt;
	m_calcBonusTime += dt;

	float basePang = (float)m_basePang;
	if (basePang > m_shownBasePang) {
		m_shownBasePang += dt * basePang * 0.2f;
		if (m_shownBasePang > basePang)
			m_shownBasePang = basePang;
	}

	if (s_pangTimer > s_pangInterval) {
		s_pangTimer = 0.0f;
		s_pangInterval = (float)(rand() % 2 + 2) * 0.1f;

		if (m_calcBonusTime < 4.0f) {
			cPangFx *fx = new cPangFx;
			fx->Create(m_pPouchArea->GetRect());
			m_pangFxList.push_back(fx);
			m_autoBonus += 10;
			g_audio->PlaySfx(MakeStr("pang_coin_%d", rand() % 3 + 1), 0, 0, 0, 0, 0.5f, 200.0f);
		}
	}

	if (m_calcBonusTime < 4.0f) {
		if (g_input->GetButton(LEFT_BUTTON) == 1 || g_input->GetButton(RIGHT_BUTTON) == 1) {
			m_clickCount += 1;
			cPangFx *fx = new cPangFx;
			fx->Create(m_pPouchArea->GetRect());
			m_pangFxList.push_back(fx);
		}
	}

	std::list<cPangFx *>::iterator it = m_pangFxList.begin();
	while (it != m_pangFxList.end()) {
		cPangFx *fx = *it;
		fx->Proc(dt);
		if (!fx->IsAlive()) {
			delete fx;
			m_pangFxList.erase(it++);
		} else {
			++it;
		}
	}

	AwardProc(dt);
}

// ghostgamedlg.cpp:1160
void FrGhostAward::AwardProc(float dt) {
	if (m_calcBonusTime < 1.0f)
		return;

	if (m_gainExp <= 0)
		return;

	sUserInfo& my = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0];
	if (my.m_level < 0x41) {

		if (my.m_value155 < data_a029a0[my.m_level].m_exp) {

			if (m_remainExp > 0) {

				my.m_value155 = my.m_value155 + 1;
				WSingleton<CSharedDoc>::Instance()->m_myExp++;

				m_remainExp--;
			}
		}
		else {
			my.m_value155 = 0;
			my.m_level++;

			WSingleton<CSharedDoc>::Instance()->m_myExp = 0;
			WSingleton<CSharedDoc>::Instance()->m_myLevel++;

			m_pExpGauge->SetRange(0, data_a029a0[my.m_level].m_exp, 0);

			if (m_myResult.m_isGhost)
				m_ghostResult.m_levelImage = MakeStr("level_%03d", WSingleton<CSharedDoc>::Instance()->m_myLevel);
			else
				m_myResult.m_levelImage = MakeStr("level_%03d", WSingleton<CSharedDoc>::Instance()->m_myLevel);

			AfxGetTask()->GetActor("Player") << MsgObject(0, 0x1f8, 0, 0, 0, 0, 0);
		}

		m_pExpGauge->SetPos(my.m_value155);
		m_pGaugeText->SetCaption(MakeStr("%d / %d", my.m_value155, data_a029a0[my.m_level].m_exp));
	}
	else {
		m_gainExp = 0;
	}
}

// ghostgamedlg.cpp:1212
bool FrGhostAward::IsLevelUpState(void) {
	sUserInfo& my = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0];
	if (my.m_level < 0x41) {

		if (my.m_value155 + m_gainExp >= data_a029a0[my.m_level].m_exp)
			return true;
	}

	return false;
}

// ghostgamedlg.cpp:1224
void FrGhostAward::OnInitDescText(intptr_t param) {
	m_pDescText = DYNAMIC_CAST(FrStatic, param);
}

// ghostgamedlg.cpp:1229
void FrGhostAward::OnInitGaugeText(intptr_t param) {
	m_pGaugeText = DYNAMIC_CAST(FrStatic, param);
}

// ghostgamedlg.cpp:1235
void FrGhostAward::OnInitGaugeBar(intptr_t param) {
	m_pExpGauge = DYNAMIC_CAST(FrGaugeBar, param);
}

// ghostgamedlg.cpp:1241
void FrGhostAward::OnInitPouchArea(intptr_t param) {
	m_pPouchArea = DYNAMIC_CAST(FrArea, param);
}

// ghostgamedlg.cpp:1247
void FrGhostAward::OnDrawScore(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {

		const WRect& rect = GetRect();
		float x = rect.x + 2.0f;
		float y = rect.y + 49.0f;

		pGDI->SetTextStyle(1);

		const WRect& pouchRect = m_pPouchArea->GetRect();
		if (m_myResult.m_isGhost == false) {

			WRect box(x, y, 498.0f, 30.0f);
			WOverlay::DrawBox(g_view, box, 0, 0x999b9dff, 0.0f);
			m_pTitleFont->Print(g_view, pouchRect.x + 170.0f, pouchRect.y - 25.0f, "-", 0, 0xffffffff, 0);
		} else {

			WRect box(x, y + 27.0f, 498.0f, 30.0f);
			WOverlay::DrawBox(g_view, box, 0, 0x999b9dff, 0.0f);
			m_pTitleFont->Print(g_view, pouchRect.x + 170.0f, pouchRect.y - 55.0f, "-", 0, 0xffffffff, 0);
		}

		const Bitmap* pMyLevel = g_pFresh->GetBitmap(m_myResult.m_levelImage.c_str());
		WRect myLevelRect(m_myResult.m_pos.x - (float)pMyLevel->Width() - 90.0f, m_myResult.m_pos.y - 2.0f, (float)pMyLevel->Width(), (float)pMyLevel->Height());
		pGDI->DrawTexture(pMyLevel, myLevelRect, 0xffffffff, 0);
		g_pFresh->GetManager()->PrintText(m_myResult.m_pos, 1, m_myResult.m_name.c_str(), -1.0f, 0xffffffff);
		WPoint myScorePos(m_myResult.m_pos.x + 100.0f, m_myResult.m_pos.y);
		g_pFresh->GetManager()->PrintText(myScorePos, 1, MakeStr("%d", m_myResult.m_gameResult), -1.0f, 0xffffffff);

		const Bitmap* pGhostLevel = g_pFresh->GetBitmap(m_ghostResult.m_levelImage.c_str());
		WRect ghostLevelRect(m_ghostResult.m_pos.x - (float)pGhostLevel->Width() - 90.0f, m_ghostResult.m_pos.y - 2.0f, (float)pGhostLevel->Width(), (float)pGhostLevel->Height());
		pGDI->DrawTexture(pGhostLevel, ghostLevelRect, 0xffffffff, 0);
		g_pFresh->GetManager()->PrintText(m_ghostResult.m_pos, 1, m_ghostResult.m_name.c_str(), -1.0f, 0xffffffff);
		WPoint ghostScorePos(m_ghostResult.m_pos.x + 100.0f, m_ghostResult.m_pos.y);
		g_pFresh->GetManager()->PrintText(ghostScorePos, 1, MakeStr("%d", m_ghostResult.m_gameResult), -1.0f, 0xffffffff);

		pGDI->SetTextStyle(0);
	}
}

// ghostgamedlg.cpp:1292
void FrGhostAward::OnDrawPouch(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {

		const Bitmap* pBag2 = g_pFresh->GetBitmap("pang_bag2");
		const Bitmap* pBag1 = g_pFresh->GetBitmap("pang_bag1");

		const WRect& rect = m_pPouchArea->GetRect();
		if (pBag2)
			pGDI->DrawTexture(pBag2, rect, 0xffffffff, 0);

		for (std::list<cPangFx*>::iterator it = m_pangFxList.begin(); it != m_pangFxList.end(); ++it) {
			cPangFx* fx = *it;
			if (fx->m_pBitmap)
				pGDI->DrawTexture(fx->m_pBitmap, fx->m_rect, ((unsigned char)(int)fx->m_alpha << 24) | 0xffffff, 0);
		}

		if (pBag1)
			pGDI->DrawTexture(pBag1, rect, 0xffffffff, 0);

		std::string basePang = MakeStr("%05d", (int)m_shownBasePang);
		int autoBonus = m_autoBonus < 200 ? m_autoBonus : 200;
		int clickBonus = m_clickBonus < 200 ? m_clickBonus : 200;
		unsigned char clickCount = m_clickCount < 40 ? m_clickCount : 40;
		std::string total = MakeStr("%07d", (int)m_shownBasePang + (clickCount * clickBonus + autoBonus));

		m_pTitleFont->Print(g_view, rect.x - 130.0f, rect.y + 24.0f, basePang.c_str(), 0, 0xffffffff, 0);
		m_pTitleFont->Print(g_view, rect.x + 150.0f, rect.y + 24.0f, total.c_str(), 0, 0xffffffff, 0);
	}
}
