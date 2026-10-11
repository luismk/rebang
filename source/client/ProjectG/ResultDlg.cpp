// Original: d:\build\custom\temp\code\client\projectg\resultdlg.cpp
#include "ResultDlg.h"
#include "InputManager.h"
#include "ObjectFactory.h"
#include "mousecursor.h"
#include "SharedDoc.h"
#include "../shared/Localize.h"
#include "fresh.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frgaugebar.h"
#include "frlistbox.h"
#include "frstatic.h"
#include "user_info.h"
#include "commonutil.h"
#include "NetResourceManager.h"
#include "ScoreDlg.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "wview.h"
#include "wresrcmng.h"
#include "wfont.h"
#include "woverlay.h"
#include "exhibition.h"
#include "parttidlist.h"
#include "ProjectG.h"
#include <stdio.h>
#include <string.h>

// resultdlg.cpp:33 (run-time type and factory registration of FrResultDlg)
const WRTTI FrResultDlg::m_RTTI("FrResultDlg", &FrForm::m_RTTI);

IObject* FrResultDlgMakeInstance(void) {
	return new FrResultDlg;
}

struct __sFrResultDlg {
	__sFrResultDlg(void) {
		ObjectFactory().AddObjectFunctor(FrResultDlgMakeInstance, "FrResultDlg");
	}
};
static __sFrResultDlg s_registerFrResultDlg;

// resultdlg.cpp:35
const sFRESH_MSGMAP* FrResultDlg::GetMessageMap(void) const {
	return &FrResultDlg::_MsgMap;
}

// The title font's texture list (data at 0xab7900).
static char* s_resultFontFiles[] = { "[bigfont.jpg" };

// resultdlg.cpp:61
FrResultDlg::FrResultDlg(void) {
	m_elapsed = 0;
	m_pStageArea = 0;
	m_pLevelArea = 0;
	m_pExpStatic = 0;
	m_pExpGaugeBar = 0;
	m_pExhibition = 0;
	m_pLevelUpArea = 0;
	m_pPartTidList = 0;
	memset(&m_winnerData, 0, sizeof(m_winnerData));
	FindWinner();

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = s_resultFontFiles;
	info.fontw = 32;
	info.fonth = 32;
	info.numPages = 1;
	info.texw = 256;
	info.texh = 128;
	info.pCharSet = "1234567890ym%-./x ";

	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);

	if (IsLocalContent((localContentType_t)0x17)) {
		m_pHalloweenOverlay = g_resrcmng->GetOverlay(MakeStr("%s.tga", "z_Halloween_Male"), 0);
	}
}

// resultdlg.cpp:88
FrResultDlg::~FrResultDlg(void) {
	if (IsLocalContent((localContentType_t)0x17)) {
		if (g_resrcmng && m_pHalloweenOverlay) { g_resrcmng->Release(m_pHalloweenOverlay); m_pHalloweenOverlay = 0; }
	}

	if (m_pPartTidList) { delete m_pPartTidList; m_pPartTidList = 0; }
	if (m_pExhibition) { delete m_pExhibition; m_pExhibition = 0; }
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// resultdlg.cpp:99
void FrResultDlg::ResetVars(void) {
	m_pStageArea = 0;
	m_pLevelArea = 0;
	m_pExpStatic = 0;
	m_pExpGaugeBar = 0;
	m_pExhibition = 0;
	m_pLevelUpArea = 0;
	m_pPartTidList = 0;
	memset(&m_winnerData, 0, sizeof(m_winnerData));
}

// resultdlg.cpp:112
void FrResultDlg::FindWinner(void) {
	memset(WSingleton<CSharedDoc>::Instance()->m_teamResult, 0, sizeof(WSingleton<CSharedDoc>::Instance()->m_teamResult));

	for (std::vector<sRivalData>::iterator it = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++it) {
		if (it->m_rank == 1) {
			m_winnerData = *it;

			if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 4)
				return;

			if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 10)
				return;
		}

		WSingleton<CSharedDoc>::Instance()->m_teamResult[it->m_team].m_score += it->m_rank;
		WSingleton<CSharedDoc>::Instance()->m_teamResult[it->m_team].m_pang += it->m_pang;
	}
}

// resultdlg.cpp:202
void FrResultDlg::OnProc(const float dt) {
	m_elapsed += dt;

	if (m_pExhibition) {
		const WRect& rect = m_pStageArea->GetRect();

		if (m_stageRect.x != rect.x || m_stageRect.y != rect.y) {
			m_pExhibition->SetCenter(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f);
			m_stageRect = rect;
		}

		m_pExhibition->Process(dt, true);
	}

	if (!m_pExpGaugeBar || m_bExpDone) {
		if (m_bExpDone && m_pLevelUpArea && m_startLevel < m_level)
			m_pLevelUpArea->SetVisible(true);
		return;
	}

	if (m_elapsed < 1.0f)
		return;

	float over = m_elapsed - 1.0f;
	if (over < 0.01f)
		return;

	int step = float2int(over * 100.0f);
	int level = m_level;
	m_elapsed -= step * 0.01f;
	int nextExp = WSingleton<CSharedDoc>::Instance()->m_levelExp[level].m_exp * 10;

	if (step > abs(m_expGain)) {
		m_bExpDone = true;
		m_exp += ((m_expGain > 0) * 2 - 1) * abs(m_expGain);
		m_expGain = 0;
	} else {
		int delta = ((m_expGain > 0) * 2 - 1) * step;
		m_exp += delta;
		m_expGain -= delta;
	}

	if (m_exp < 0 && !m_bExpDone) {
		m_bExpDone = true;
		m_exp = 0;
	}

	if (m_exp >= nextExp) {
		m_exp -= nextExp;
		m_level = level + 1;

		nextExp = WSingleton<CSharedDoc>::Instance()->m_levelExp[m_level].m_exp * 10;

		m_pExpGaugeBar->SetRange(0, nextExp, 0);
		m_pExpGaugeBar->SetPos(m_exp);

		if (m_pLevelArea) {
			char buf[32];
			sprintf(buf, "level_%03d", m_level + 1);
			m_pLevelArea->SetBgImg(buf);
		}
	} else {
		m_pExpGaugeBar->SetPos(m_exp);
	}

	if (m_pExpStatic) {
		char buf[32];
		sprintf(buf, "%d / %d", m_exp / 10, nextExp / 10);
		m_pExpStatic->SetCaption(buf);
	}
}

// resultdlg.cpp:300
void FrResultDlg::OnStageInit(int param) {
	m_pStageArea = DYNAMIC_CAST(FrArea, param);

	if (m_pStageArea) {
		char bgName[64] = { 0 };
		sprintf(bgName, "30in_back_%s", WSingleton<CSharedDoc>::Instance()->m_pCurCourse ? WSingleton<CSharedDoc>::Instance()->m_pCurCourse->m_mapName : 0);
		m_pStageArea->SetBgImg(bgName);
	}

	ShowPet();
}

// resultdlg.cpp:314
void FrResultDlg::OnStageOwnerDraw(void) {
	char bgName[64] = { 0 };
	sprintf(bgName, "30in_back_%s", WSingleton<CSharedDoc>::Instance()->m_pCurCourse ? WSingleton<CSharedDoc>::Instance()->m_pCurCourse->m_mapName : 0);
	m_pStageArea->SetBgImg(bgName);

	if (m_pExhibition) {
		m_pExhibition->Display(0, 0);
	}

	const WRect& rect = m_pStageArea->GetRect();


	float x = rect.x + 333.0f;
	float y = rect.y + 34.0f;

	char* text;





	if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 10 && (WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_approachPoint == -1 || WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_approachDistance == -1)) {

		text = MakeStr("-");

	} else {

		text = MakeStr("%d", WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_rank);
	}


	if (m_pTitleFont) {
		m_pTitleFont->Print(g_view, x, y, text, 0, 0xFFFFFFFF, 0);
	}
}

// resultdlg.cpp:379
void FrResultDlg::OnMatchStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (pStatic == 0)
		return;

	if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 10) {
		pStatic->SetCaption("\xB3\xB2\xC0\xBA \xB0\xC5\xB8\xAE :");
	}
}

// resultdlg.cpp:436
void FrResultDlg::OnIdInit(intptr_t param) {
	m_pIdArea = DYNAMIC_CAST(FrArea, param);
}

// resultdlg.cpp:441
void FrResultDlg::OnIdOwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		float offset = 0.0f;
		sGameResultInfo& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()];

		if (info.m_hasEmblem) {
			const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

			if (emblem) {
				gdi->DrawTexture(emblem, WRect(m_pIdArea->GetRect().x, m_pIdArea->GetRect().y - 6.0f, emblem->Width(), emblem->Height()), 0xFFFFFFFF, 0);
				offset = 27.0f;
			}
		}

		if (!WSingleton<CProjectG>::Instance()->HidePrivacy()) {
			gdi->SetTextColor(0xFF000000, 0xFFFFFFFF);
			gdi->SetTextStyle(0);
			g_pFresh->GetManager()->PrintText(WPoint(m_pIdArea->GetRect().x + offset, m_pIdArea->GetRect().y), 0, info.m_nickname, -1.0f, 0xFFFFFFFF);
		}
	}
}

// resultdlg.cpp:477
void FrResultDlg::OnScoreStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (pStatic == 0)
		return;

	if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 10) {
		pStatic->SetCaption("\xB3\xB2\xC0\xBA \xBD\xC3\xB0\xA3 :");
	}
}

// resultdlg.cpp:489
void FrResultDlg::OnScoreInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (pStatic == 0)
		return;


	if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 10) {

		int point = WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_approachPoint;

		if (point == -1) {
			pStatic->SetCaption("-");
		} else {
			pStatic->SetCaption(MakeStr("%d.%03d \xC3\xCA", point / 1000, point % 1000));
		}
	} else {
		pStatic->SetCaption(MakeStr(WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_score > 0 ? "+%d" : "%d", WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_score));
	}
}

// resultdlg.cpp:532
void FrResultDlg::OnBonusStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (pStatic == 0)
		return;

	if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 10) {
		pStatic->SetVisible(false);
	}
}

// resultdlg.cpp:545
void FrResultDlg::OnBonusInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);

	if (pStatic == 0)
		return;


	if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 10) {

		pStatic->SetVisible(false);
	}











	int bonus = (int)((float)(WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_pang + WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_extraPang) * WSingleton<CSharedDoc>::Instance()->m_pangPercent * WSingleton<CSharedDoc>::Instance()->m_pangRate * 0.01f - WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()].m_pang);


	pStatic->SetCaption(MakeStr("%I64d", (__int64)bonus));
}

// resultdlg.cpp:572
void FrResultDlg::OnGetItemInit(int param) {
	m_pGetItemArea = DYNAMIC_CAST(FrArea, param);
}

// resultdlg.cpp:674
void FrResultDlg::OnLevelInit(intptr_t param) {
	m_pLevelArea = DYNAMIC_CAST(FrArea, param);
}

// resultdlg.cpp:679
void FrResultDlg::OnExpInit(int param) {
	m_pExpStatic = DYNAMIC_CAST(FrStatic, param);
}

// resultdlg.cpp:684
void FrResultDlg::OnExpGaugeBarInit(int param) {
	m_pExpGaugeBar = DYNAMIC_CAST(FrGaugeBar, param);
}

// resultdlg.cpp:689
void FrResultDlg::OnLevelUpInit(int param) {
	m_pLevelUpArea = DYNAMIC_CAST(FrArea, param);

	if (m_pLevelUpArea) {
		m_pLevelUpArea->SetVisible(false);
	}
}

// resultdlg.cpp:803 (run-time type and factory registration of FrResultTeamDlg)
const WRTTI FrResultTeamDlg::m_RTTI("FrResultTeamDlg", &FrResultDlg::m_RTTI);

IObject* FrResultTeamDlgMakeInstance(void) {
	return new FrResultTeamDlg;
}

struct __sFrResultTeamDlg {
	__sFrResultTeamDlg(void) {
		ObjectFactory().AddObjectFunctor(FrResultTeamDlgMakeInstance, "FrResultTeamDlg");
	}
};
static __sFrResultTeamDlg s_registerFrResultTeamDlg;

// resultdlg.cpp:805
const sFRESH_MSGMAP* FrResultTeamDlg::GetMessageMap(void) const {
	return &FrResultTeamDlg::_MsgMap;
}

// resultdlg.cpp:840
void FrResultTeamDlg::OnStageInit(int param) {
	FrResultDlg::OnStageInit(param);

	m_pTrophyBitmap = 0;

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6 && WSingleton<CSharedDoc>::Instance()->m_winningTeam == GUILDMATCHUP(0)->m_team && WSingleton<CSharedDoc>::Instance()->m_winningTeam != 2) {
		m_pTrophyBitmap = g_pFresh->GetManager()->GetBitmap("TROPHY", "guild");
	}
}

// resultdlg.cpp:862
bool FrResultTeamDlg::OnInit(void) {
	FrResultDlg::OnInit();

	// a guild match: name the winning guild
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
		memset(&m_guildMatch, 0, sizeof(m_guildMatch));

		if (WSingleton<CSharedDoc>::Instance()->m_winningTeam == 2) {
			SetDesc(MakeStr("%49s \xB4\xD9\xBD\xC3 \xC7\xD1\xB9\xF8 \xC0\xDA\xBF\xF5\xC0\xBB \xB0\xA1\xB8\xAE\xBC\xBC\xBF\xE4^^", ""));
		} else if (WSingleton<CSharedDoc>::Instance()->m_winningTeam == GUILDMATCHUP(0)->m_team) {
			SetDesc(MakeStr("%49s %s\xB1\xE6\xB5\xE5\xB0\xA1 \xBD\xC2\xB8\xAE\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", "", GUILDMATCHUP(0)->m_guildName));
		} else if (WSingleton<CSharedDoc>::Instance()->m_winningTeam == GUILDMATCHUP(1)->m_team) {
			SetDesc(MakeStr("%49s %s\xB1\xE6\xB5\xE5\xB0\xA1 \xBD\xC2\xB8\xAE\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", "", GUILDMATCHUP(1)->m_guildName));
		}

		// The room's guild block (sRoomInfo from m_guildUID on) is an sGuildMatchInfo.
		memcpy(&m_guildMatch, WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_guildUID, sizeof(m_guildMatch));
	}

	return true;
}

// resultdlg.cpp:959
void FrResultTeamDlg::OnTrophyInit(int param) {
	FrResultDlg::OnTrophyInit(param);
}

// resultdlg.cpp:974
void FrResultTeamDlg::OnMatchTypeInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
		pStatic->SetCaption("\xB1\xE6\xB5\xE5 \xB1\xE2\xBF\xA9\xB5\xB5 :");
	} else {
		pStatic->SetCaption("\xB4\xEB   \xC8\xB8 :");
	}
}

// resultdlg.cpp:1020
void FrResultTeamDlg::OnIdInit(intptr_t param) {
	m_pTeamIdArea = DYNAMIC_CAST(FrArea, param);
}

// resultdlg.cpp:1025
void FrResultTeamDlg::OnIdOwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		float offset = 0.0f;
		sGameResultInfo& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[MyIndex()];

		if (info.m_hasEmblem) {
			const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

			if (emblem) {
				gdi->DrawTexture(emblem, WRect(m_pTeamIdArea->GetRect().x, m_pTeamIdArea->GetRect().y - 6.0f, emblem->Width(), emblem->Height()), 0xFFFFFFFF, 0);
				offset = 27.0f;
			}
		}

		if (!WSingleton<CProjectG>::Instance()->HidePrivacy()) {
			gdi->SetTextColor(0xFF000000, 0xFFFFFFFF);
			gdi->SetTextStyle(0);
			g_pFresh->GetManager()->PrintText(WPoint(m_pTeamIdArea->GetRect().x + offset, m_pTeamIdArea->GetRect().y), 0, info.m_nickname, -1.0f, 0xFFFFFFFF);
		}
	}
}

// resultdlg.cpp:1061
void FrResultTeamDlg::OnScoreInit(int param) {
	FrResultDlg::OnScoreInit(param);
}

// resultdlg.cpp:1066
void FrResultTeamDlg::OnPangInit(int param) {
	FrResultDlg::OnPangInit(param);
}

// resultdlg.cpp:1081
void FrResultTeamDlg::OnBonusStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
		pStatic->SetCaption("\xB1\xE6\xB5\xE5 \xC6\xCE :");
	} else {
		pStatic->SetCaption("\xBA\xB8\xB3\xCA\xBD\xBA :");
	}
}

// resultdlg.cpp:1139
void FrResultTeamDlg::OnGetItemInit(int param) {
	FrResultDlg::OnGetItemInit(param);
}

// resultdlg.cpp:1144
void FrResultTeamDlg::OnGetItemOwnerDraw(void) {
	FrResultDlg::OnGetItemOwnerDraw();
}

// resultdlg.cpp:1149
void FrResultTeamDlg::OnLevelInit(intptr_t param) {
	FrResultDlg::OnLevelInit(param);
}

// resultdlg.cpp:1154
void FrResultTeamDlg::OnExpInit(int param) {
	FrResultDlg::OnExpInit(param);
}

// resultdlg.cpp:1159
void FrResultTeamDlg::OnExpGaugeBarInit(int param) {
	FrResultDlg::OnExpGaugeBarInit(param);
}

// resultdlg.cpp:1164
void FrResultTeamDlg::OnLevelUpInit(int param) {
	FrResultDlg::OnLevelUpInit(param);
}

// resultdlg.cpp:1170
void FrResultTeamDlg::OnRankStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (pStatic == 0)
		return;

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6)
		pStatic->SetVisible(false);
}

// resultdlg.cpp:1181
void FrResultTeamDlg::OnResultStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (pStatic == 0)
		return;

	if (WSingleton<CSharedDoc>::Instance()->m_roomType != 6)
		pStatic->SetVisible(false);
}

// resultdlg.cpp:1191
void FrResultTeamDlg::OnRedInit(int param) {
	m_pRedArea = DYNAMIC_CAST(FrArea, param);

	if (m_pRedArea) {
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
			m_pRedEmblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_guildMark[0]);
		} else {
			m_pRedArea->SetBgImg("30in_team_redteam_dark");
		}
	}
}

// resultdlg.cpp:1218
void FrResultTeamDlg::OnRedOwnerDraw(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
		WPoint pos = m_pRedArea->GetRect().TopLeft();

		if (m_pRedEmblem) {
			g_pFresh->GetManager()->GetGDI()->DrawTexture(m_pRedEmblem, WRect(pos.x + 10.0f, pos.y + 1.0f, m_pRedEmblem->Width(), m_pRedEmblem->Height()), 0xFFFFFFFF, 0);
		}

		g_pFresh->GetManager()->PrintText(WPoint(pos.x, pos.y + 30.0f), 0, m_guildMatch.m_guildName[0], -1.0f, 0xFFFFFFFF);
	}
}

// resultdlg.cpp:1234
void FrResultTeamDlg::OnBlueInit(int param) {
	m_pBlueArea = DYNAMIC_CAST(FrArea, param);

	if (m_pBlueArea) {
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
			m_pBlueEmblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_guildMark[1]);
		} else {
			m_pBlueArea->SetBgImg("30in_team_blueteam_dark");
		}
	}
}

// resultdlg.cpp:1261
void FrResultTeamDlg::OnBlueOwnerDraw(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
		WPoint pos = m_pBlueArea->GetRect().TopLeft();

		if (m_pBlueEmblem) {
			g_pFresh->GetManager()->GetGDI()->DrawTexture(m_pBlueEmblem, WRect(pos.x + 10.0f, pos.y + 1.0f, m_pBlueEmblem->Width(), m_pBlueEmblem->Height()), 0xFFFFFFFF, 0);
		}

		g_pFresh->GetManager()->PrintText(WPoint(pos.x, pos.y + 30.0f), 0, m_guildMatch.m_guildName[1], -1.0f, 0xFFFFFFFF);
	}
}

// resultdlg.cpp:1277
void FrResultTeamDlg::OnRedScoreInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (pStatic == 0)
		return;

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 5)
		pStatic->SetCaption(MakeStr("%d", WSingleton<CSharedDoc>::Instance()->m_teamResult[0].m_score));
	else
		pStatic->SetCaption(MakeStr("%d", WSingleton<CSharedDoc>::Instance()->m_teamPoint[0]));
}

// resultdlg.cpp:1289
void FrResultTeamDlg::OnBlueScoreInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (pStatic == 0)
		return;

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 5)
		pStatic->SetCaption(MakeStr("%d", WSingleton<CSharedDoc>::Instance()->m_teamResult[1].m_score));
	else
		pStatic->SetCaption(MakeStr("%d", WSingleton<CSharedDoc>::Instance()->m_teamPoint[1]));
}

// resultdlg.cpp:1301
void FrResultTeamDlg::OnRedPangInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (pStatic == 0)
		return;
	pStatic->SetCaption(MakeStr("%I64dp", WSingleton<CSharedDoc>::Instance()->m_teamResult[0].m_pang));
}

// resultdlg.cpp:1308
void FrResultTeamDlg::OnBluePangInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);
	if (pStatic == 0)
		return;
	pStatic->SetCaption(MakeStr("%I64dp", WSingleton<CSharedDoc>::Instance()->m_teamResult[1].m_pang));
}

// resultdlg.cpp:1402
bool UniteRankCompare(const void* a, const void* b) {
	const sGameResultInfo* pA = (const sGameResultInfo*)a;
	const sGameResultInfo* pB = (const sGameResultInfo*)b;

	if (pA->m_rank < pB->m_rank) {
		return true;
	} else if (pA->m_rank > pB->m_rank) {
		return false;
	}

	if (pA->m_sortGroup > pB->m_sortGroup) {
		return true;
	} else if (pA->m_sortGroup < pB->m_sortGroup) {
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

// resultdlg.cpp:1435
bool ReportRankCompare(const void* a, const void* b) {
	const sGameResultInfo* pA = (const sGameResultInfo*)a;
	const sGameResultInfo* pB = (const sGameResultInfo*)b;

	if (pA->m_rank < pB->m_rank) {
		return true;
	} else if (pA->m_rank > pB->m_rank) {
		return false;
	}

	if (pA->m_sortGroup > pB->m_sortGroup) {
		return true;
	} else if (pA->m_sortGroup < pB->m_sortGroup) {
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

// resultdlg.cpp:1470
bool UniteGuildIndexCompare(const void* a, const void* b) {
	const sGameResultInfo* pA = (const sGameResultInfo*)a;
	const sGameResultInfo* pB = (const sGameResultInfo*)b;

	if (pA->m_sortGroup > pB->m_sortGroup) {
		return true;
	} else if (pA->m_sortGroup < pB->m_sortGroup) {
		return false;
	}

	if (pA->m_uniteGuildIndex > pB->m_uniteGuildIndex) {
		return true;
	} else if (pA->m_uniteGuildIndex < pB->m_uniteGuildIndex) {
		return false;
	}

	if (pA->m_pang > pB->m_pang) {
		return true;
	} else if (pA->m_pang < pB->m_pang) {
		return false;
	}

	return pA->m_value13F < pB->m_value13F;
}

// resultdlg.cpp:1499 (run-time type and factory registration of FrUniteResultDlg)
const WRTTI FrUniteResultDlg::m_RTTI("FrUniteResultDlg", &FrForm::m_RTTI);

IObject* FrUniteResultDlgMakeInstance(void) {
	return new FrUniteResultDlg;
}

struct __sFrUniteResultDlg {
	__sFrUniteResultDlg(void) {
		ObjectFactory().AddObjectFunctor(FrUniteResultDlgMakeInstance, "FrUniteResultDlg");
	}
};
static __sFrUniteResultDlg s_registerFrUniteResultDlg;

// resultdlg.cpp:1501
const sFRESH_MSGMAP* FrUniteResultDlg::GetMessageMap(void) const {
	return &FrUniteResultDlg::_MsgMap;
}

// resultdlg.cpp:1532
void FrUniteResultDlg::OnCancelButtonInit(int param) {
	if (m_pCancelButton == 0) {
		m_pCancelButton = DYNAMIC_CAST(FrButton, param);
	}
}

// resultdlg.cpp:1542
void FrUniteResultDlg::OnAwardInfoInit(int param) {
	if (m_pAwardInfoButton == 0) {
		m_pAwardInfoButton = DYNAMIC_CAST(FrButton, param);
	}
}

// resultdlg.cpp:1552
void FrUniteResultDlg::OnAwardInfoClick(void) {
	if (m_pAwardInfoDlg == 0) {
		m_pAwardInfoDlg = CreateForm<FrAwardInfoDlg>(g_pFresh->GetManager(), this, "awardinfomation", 0);
		m_pAwardInfoDlg->Open((FRESH_RESULT_CALLBACK)&FrUniteResultDlg::OnAwardInfoDlgResult, 0);
	}
}

// resultdlg.cpp:1565
void FrUniteResultDlg::OnMyInfoAreaInit(int param) {
	if (m_pMyInfoArea == 0) {
		m_pMyInfoArea = DYNAMIC_CAST(FrArea, param);
	}
}

// resultdlg.cpp:1921
void FrUniteResultDlg::OnExpGaugeBarInit(int param) {
	if (m_pExpGaugeBar == 0) {
		m_pExpGaugeBar = DYNAMIC_CAST(FrGaugeBar, param);
	}
}

// resultdlg.cpp:1931
void FrUniteResultDlg::OnExpAreaInit(int param) {
	if (m_pExpArea == 0) {
		m_pExpArea = DYNAMIC_CAST(FrArea, param);
	}
}

// resultdlg.cpp:1941
void FrUniteResultDlg::OnExpAreaOwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi) {
		int space = gdi->SetSpace(0);

		WRect rect = m_pExpGaugeBar->GetRect();
		WPoint pos(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f - 5.0f);

		if (!m_bLevelUp) {
			gdi->SetTextColor(0xFFFFFFFF, 0xFFFFFFFF);
			gdi->SetTextStyle(1);

			gdi->Print(pos, 1, "%d / %d", m_exp / 10, m_nextLevelExp / 10);

			gdi->SetTextColor(0xFF000000, 0xFFFFFFFF);
			gdi->SetTextStyle(0);
		} else {
			const Bitmap* bitmap = g_pFresh->GetBitmap("font_levelup");

			if (bitmap) {
				gdi->DrawTexture(bitmap, WRect(pos.x - bitmap->Width() / 2, pos.y - (rect.h * 0.5f - 5.0f), bitmap->Width(), bitmap->Height()), 0xFFFFFFFF, 0);
			}
		}

		gdi->SetSpace(space);
	}
}

// resultdlg.cpp:1988
void FrUniteResultDlg::OnRankListBoxInit(int param) {
	if (m_pRankListBox == 0) {
		m_pRankListBox = DYNAMIC_CAST(FrListBox, param);
	}

	m_pRankListBox->UseRightButton(true);
}

// resultdlg.cpp:2493
void FrUniteResultDlg::OnRankListLBtnUp(void) {
	if (m_bTikiReport)
		return;

	FrListItem* pItem = m_pRankListBox->GetItemUnderCursor();

	if (pItem == 0)
		return;

	sGameResultInfo* pInfo = (sGameResultInfo*)pItem->pData;

	if (pInfo == 0)
		return;

	m_selectedUid = pInfo->m_uid;

	m_pRankListBox->SelectItem(pItem, true);

	if (m_pScoreDlg == 0) {
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
			m_pScoreDlg = CreateForm<FrScoreDlg>(g_pFresh->GetManager(), this, "score_guild", 0);
			m_pScoreDlg->Open((FRESH_RESULT_CALLBACK)&FrUniteResultDlg::OnScoreDlgResult, 0);
			m_pScoreDlg->EnableDrag(false);

			m_pScoreDlg->SetGuildPlayer(m_selectedUid);
			m_pScoreDlg->SetVisibleCloseBtn(false);
		} else {
			m_pScoreDlg = CreateForm<FrScoreDlg>(g_pFresh->GetManager(), this, "score", 0);
			m_pScoreDlg->Open((FRESH_RESULT_CALLBACK)&FrUniteResultDlg::OnScoreDlgResult, 0);
			m_pScoreDlg->EnableDrag(false);

			if (m_selectedUid == 0xFFFFFFFF)
				m_selectedUid = MyUID();

			m_pScoreDlg->SetPlayer(m_selectedUid);
			m_pScoreDlg->SetVisibleCloseBtn(true);
		}
	}

	if (m_pScoreDlg)
		m_pScoreDlg->SetPlayer(m_selectedUid);
}

// resultdlg.cpp:2543
void FrUniteResultDlg::OnRankListRBtnUp(void) {
	if (m_bTikiReport)
		return;

	FrListItem* pItem = m_pRankListBox->GetItemUnderCursor();

	if (pItem == 0)
		return;

	sGameResultInfo* pInfo = (sGameResultInfo*)pItem->pData;

	if (pInfo == 0)
		return;

	m_selectedUid = pInfo->m_uid;

	if (m_pScoreDlg)
		m_pScoreDlg->SetPlayer(m_selectedUid);

	if (WSingleton<CUserInfo>::Instance() && pInfo->m_state != 3) {
		CSharedDoc* doc = WSingleton<CSharedDoc>::Instance();
		std::map<unsigned long, sBriefUserInfo>::iterator it = doc->m_userMap.find(pInfo->m_uid);
		if (it != doc->m_userMap.end())
			it->second.roomNumber = doc->m_roomInfo.m_roomNumber;

		WSingleton<CUserInfo>::Instance()->SetInfo(pInfo->m_id, pInfo->m_uid, true, true, false, false, pInfo->m_nickname);

		FrUserInfoForm* pDlg = USERINFODLG();
		if (pDlg)
			pDlg->DisableFlag(1);
	}
}

// resultdlg.cpp:2580
void FrUniteResultDlg::OnDescAreaInit(int param) {
	if (m_pDescArea == 0) {
		m_pDescArea = DYNAMIC_CAST(FrArea, param);
	}
}

// resultdlg.cpp:2590
void FrUniteResultDlg::OnDescAreaOwnerDraw(int param) {
	const WRect& rect = m_pDescArea->GetRect();
	WPoint pos(rect.x + 15.0f, rect.y - 1.0f);

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	pGDI->SetTextStyle(0);

	const char* text;
	if (m_resultDlgType == 0) {
		text = MakeStr("1\xb5\xee\xc0\xba %s\xb4\xd4\xc0\xcc \xc2\xf7\xc1\xf6\xc7\xcf\xbc\xcc\xbd\xc0\xb4\xcf\xb4\xd9.", m_winnerData.m_nickname);
	} else {
		if (WSingleton<CSharedDoc>::Instance()->m_winningTeam == 0) {
			text = MakeStr("RED \xc6\xc0\xc0\xcc \xbd\xc2\xb8\xae\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.", m_winnerData.m_nickname);
		} else {
			text = MakeStr("BLUE \xc6\xc0\xc0\xcc \xbd\xc2\xb8\xae\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.", m_winnerData.m_nickname);
		}
	}

	WPoint textPos = pos;
	if (text) {
		g_pFresh->GetManager()->PrintText(textPos, 0, text, -1.0f, 0xFFFFFFFF);
	}
}

// resultdlg.cpp:2630
void FrUniteResultDlg::OnAwardAreaInit(int param) {
	if (m_pAwardArea == 0) {
		m_pAwardArea = DYNAMIC_CAST(FrArea, param);
	}
}

// resultdlg.cpp:2802
void FrUniteResultDlg::OnItemAreaInit(int param) {
	if (m_pItemArea == 0) {
		m_pItemArea = DYNAMIC_CAST(FrArea, param);
	}
}

// resultdlg.cpp:2960
void FrUniteResultDlg::OnTeamResultInit(int param) {
	if (m_pTeamResultArea == 0) {
		m_pTeamResultArea = DYNAMIC_CAST(FrArea, param);
	}
}

// resultdlg.cpp:3121
void FrUniteResultDlg::OnTooltipOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI == 0)
		return;

	if (m_pCurTooltip == 0)
		return;

	WVector mousePos = g_input->GetMousePoint();
	WVector cursorPos = g_input->GetMousePoint();
	float width = (float)m_pCurTooltip->Width();
	float height = (float)m_pCurTooltip->Height();

	WRect rect(mousePos.x + 12.0f, cursorPos.y + 23.0f, width, height);
	if (rect.x >= g_view->GetWidth() - width)
		rect.x = g_view->GetWidth() - width;
	if (rect.y >= g_view->GetHeight() - height)
		rect.y = g_view->GetHeight() - height;

	pGDI->DrawTexture(m_pCurTooltip, rect, 0xFFFFFFFF, 0);
}

// resultdlg.cpp:3147
void FrUniteResultDlg::OnNextLevelTextInit(int param) {
	if (m_pNextLevelText == 0) {
		m_pNextLevelText = DYNAMIC_CAST(FrEdit, param);
	}
}

// resultdlg.cpp:3276
FrUniteResultDlg::~FrUniteResultDlg(void) {
	if (g_resrcmng && m_pTitleFont2) { g_resrcmng->Release(m_pTitleFont2); m_pTitleFont2 = 0; }
	if (g_resrcmng && m_pTitleFont1) { g_resrcmng->Release(m_pTitleFont1); m_pTitleFont1 = 0; }

	m_bTikiReport = false;
	m_tikiReportRank = 0;
	if (USERINFODLG()) {
		WSingleton<CUserInfo>::Instance()->Close();
	}

	m_pCancelButton = 0;
}

// resultdlg.cpp:3295
bool FrUniteResultDlg::Close(bool bDestroy) {
	if (m_pTreasureScoreDlg) {
		m_pTreasureScoreDlg->Close(bDestroy);
	}

	if (m_pTreasureGiftDlg) {
		m_pTreasureGiftDlg->Close(bDestroy);
	}

	return FrForm::Close(bDestroy);
}

// resultdlg.cpp:3311
bool FrUniteResultDlg::CheckChildDlgVisible(void) {
	if (m_pAwardInfoDlg) {
		if (m_pAwardInfoDlg->IsVisible()) {
			m_pAwardInfoDlg->Close(true);
			return true;
		}
	}

	if (m_pScoreDlg) {
		if (m_pScoreDlg->IsVisible()) {
			m_pScoreDlg->Close(true);
			return true;
		}
	}

	if (m_pTreasureScoreDlg || m_pTreasureGiftDlg)
		return true;

	if (USERINFODLG()) {
		CUserInfo::Instance()->Close();
		return true;
	}

	return false;
}

// resultdlg.cpp:3926
void FrUniteResultDlg::SetResultDlgType(void) {
	switch (WSingleton<CSharedDoc>::Instance()->m_roomType) {
	case 4:
		m_resultDlgType = 0;
		break;
	case 5:
		m_resultDlgType = 1;
		break;
	case 6:
		m_resultDlgType = 2;
		break;
	}
}

// resultdlg.cpp:3945
void FrUniteResultDlg::SetTikiReportState(SYSTEMTIME& time) {
	if (!IsLocalContent((localContentType_t)0x14))
		return;

	m_bTikiReport = true;
	m_tikiReportTime = time;
	m_tikiReportRank = 0;

	CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
	int index = 0;
	for (std::vector<sGameResultInfo>::iterator it = pDoc->m_gameResultList.begin(); it != pDoc->m_gameResultList.end(); ++it, ++index) {
		if (it->m_id == pDoc->m_myUID) {
			m_tikiReportRank = index;
			break;
		}
	}
}

// resultdlg.cpp:4024
bool FrUniteResultDlg::SetTooltip(WRect rect, int index) {
	if (index >= 0 && index <= 6) {
		if (rect.IsInRect(WPoint(g_input->GetMousePoint().x, g_input->GetMousePoint().y))) {
			m_pCurTooltip = m_pTooltips[index];
			CMouseCursor::Instance()->SetCursor(1);
			return true;
		}

		if (!m_bKeepTooltip[0] && !m_bKeepTooltip[1]) {
			m_pCurTooltip = 0;
		}
	}

	return false;
}

// resultdlg.cpp:4044
void FrUniteResultDlg::FindWinner(void) {

	memset(WSingleton<CSharedDoc>::Instance()->m_teamResult, 0, sizeof(WSingleton<CSharedDoc>::Instance()->m_teamResult));

	for (std::vector<sRivalData>::iterator it = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++it) {

		if (it->m_rank == 1) {

			m_winnerData = *it;

			if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 4)
				return;


			if (WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_gameType == 10)
				return;
		}



		switch (m_resultDlgType) {
		case 2:

			WSingleton<CSharedDoc>::Instance()->m_teamResult[it->m_team].m_score += it->m_uniteGuildIndex;
			WSingleton<CSharedDoc>::Instance()->m_teamResult[it->m_team].m_pang += it->m_pang;
			break;
		case 1:

			WSingleton<CSharedDoc>::Instance()->m_teamResult[it->m_team].m_score += it->m_rank;
			WSingleton<CSharedDoc>::Instance()->m_teamResult[it->m_team].m_pang += it->m_pang;
			break;
		}
	}
}

// resultdlg.cpp:4080
bool FrUniteResultDlg::OnAwardInfoDlgResult(int ret, FrForm* pForm) {
	m_pAwardInfoDlg = 0;
	return true;
}

// resultdlg.cpp:4088
bool FrUniteResultDlg::OnScoreDlgResult(int ret, FrForm* pForm) {
	m_pScoreDlg = 0;
	return true;
}

// resultdlg.cpp:4095
bool FrUniteResultDlg::OnTreasureScoreDlgResult(int ret, FrForm* pForm) {
	m_pTreasureScoreDlg = 0;
	return true;
}

// resultdlg.cpp:4103
bool FrUniteResultDlg::OnTreasureGiftDlgResult(int ret, FrForm* pForm) {
	g_input->ExclusiveGetDownUseDone("ESCAPE");

	if (m_pTreasureScoreDlg) {
		m_pTreasureScoreDlg->Close(true);
	}

	if (m_pCancelButton) {
		m_pCancelButton->Enable(true);
	}

	EnableFlag(1);
	m_pTreasureGiftDlg = 0;

	return true;
}

// resultdlg.cpp:4126
void FrUniteResultDlg::SetPrizeResult(void) {
	m_prizeCountMap.clear();

	std::vector<unsigned long>& gainedItems = WSingleton<CSharedDoc>::Instance()->m_gainedItemList[0];

	for (std::vector<unsigned long>::iterator it = gainedItems.begin(); it != gainedItems.end(); ++it) {
		unsigned long typeId = *it;

		std::map<unsigned long, int>::iterator found = m_prizeCountMap.find(typeId);
		if (found == m_prizeCountMap.end()) {
			m_prizeCountMap.insert(std::pair<const unsigned long, int>(typeId, 1));
		} else {
			found->second++;
		}
	}
}

// resultdlg.cpp:4205 (run-time type and factory registration of FrAwardInfoDlg)
const WRTTI FrAwardInfoDlg::m_RTTI("FrAwardInfoDlg", &FrForm::m_RTTI);

IObject* FrAwardInfoDlgMakeInstance(void) {
	return new FrAwardInfoDlg;
}

struct __sFrAwardInfoDlg {
	__sFrAwardInfoDlg(void) {
		ObjectFactory().AddObjectFunctor(FrAwardInfoDlgMakeInstance, "FrAwardInfoDlg");
	}
};
static __sFrAwardInfoDlg s_registerFrAwardInfoDlg;

// resultdlg.cpp:4207
const sFRESH_MSGMAP* FrAwardInfoDlg::GetMessageMap(void) const {
	return &FrAwardInfoDlg::_MsgMap;
}

sFRESH_MSGMAP FrAwardInfoDlg::_MsgMap = { &FrForm::_MsgMap, &FrAwardInfoDlg::_MsgEntries[0] };

sFRESH_ENTRY FrAwardInfoDlg::_MsgEntries[] = {
	// resultdlg.cpp:4210
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// resultdlg.cpp:4215
FrAwardInfoDlg::FrAwardInfoDlg(void) {
	m_elapsed = 0;
}

// resultdlg.cpp:4222
FrAwardInfoDlg::~FrAwardInfoDlg(void) {
}

// resultdlg.cpp:4229
bool FrAwardInfoDlg::OnInit(void) {
	SetCaption("\xbc\xf6\xbb\xf3\xb3\xbb\xbf\xeb");
	return true;
}

// resultdlg.cpp:4237
void FrAwardInfoDlg::OnProc(const float dt) {
	m_elapsed += dt;
}
