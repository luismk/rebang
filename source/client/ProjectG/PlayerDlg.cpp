// Original: d:\build\custom\temp\code\client\projectg\playerdlg.cpp
#include "playerdlg.h"
#include "ObjectFactory.h"
#include "fresh.h"
#include "frbutton.h"
#include "fredit.h"
#include "exhibition.h"
#include "commonutil.h"
#include "frarea.h"
#include "frgaugebar.h"
#include "frlistbox.h"
#include "parttidlist.h"
#include "SharedDoc.h"
#include "cardsystem.h"
#include "TaskManager.h"
#include "actor.h"
#include "../shared/classdefine.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "InputManager.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

// playerdlg.cpp:40
CStatBar::CStatBar(float barGap, float barHeight) : m_barGap(barGap), m_barHeight(barHeight) {
	m_bars.reserve(5);

	for (int i = 0; i < 5; i++) {
		sBar bar(MakeStr("stat_gauge_%d", i));
		if (i == 0)
			bar.m_rangeLimit = 50.0f;
		else
			bar.m_rangeLimit = 30.0f;

		m_bars.push_back(bar);
	}

	m_destTime = 0.0f;
}

// playerdlg.cpp:58
CStatBar::~CStatBar(void) {
}

// playerdlg.cpp:64
void CStatBar::SetRange(eLvlType type, int pos, int range, int maxValue) {
	m_bars[type].m_destPos = 0.0f > (float)pos ? 0.0f : (float)pos;
	m_bars[type].m_curPos = 0.0f;
	m_bars[type].m_rangePos = (float)range < m_bars[type].m_rangeLimit ? (float)range : m_bars[type].m_rangeLimit;
	m_bars[type].m_maxValue = maxValue;
}

// playerdlg.cpp:120
void CStatBar::Render(const WPoint& pos) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0)
		return;

	for (int i = 0; i < 5; i++) {
		float index = (float)i;

		WRect rect;
		rect.x = pos.x;
		rect.y = (m_barHeight + m_barGap) * index + pos.y;
		rect.w = m_bars[i].m_curPos / m_bars[i].m_rangeLimit * 100.0f;
		rect.h = m_barHeight;
		pGDI->DrawTexture(m_bars[i].m_pBitmap, rect, 0xFFFFFFFF, 0);

		pGDI->SetTextStyle(0);
		pGDI->SetTextColor(0xFFFFFFFF, 0xFFFFFFFF);

		pGDI->Print(WPoint(pos.x + 100.0f, (m_barHeight + m_barGap) * index + pos.y - 2.0f), 0, "%d/%d", (int)m_bars[i].m_curPos, (int)m_bars[i].m_rangePos);
	}
}

// playerdlg.cpp:72
void CStatBar::SetDestTime(float destTime) {
	m_destTime = destTime;
}

// playerdlg.cpp:77
void CStatBar::Process(float dt) {
	for (int i = 0; i < 5; i++) {
		if (m_bars[i].m_curPos < m_bars[i].m_destPos) {
			if (m_destTime > FLT_EPSILON) {
				m_bars[i].m_curPos += m_bars[i].m_destPos / m_destTime * dt;
			} else {
				m_bars[i].m_curPos = m_bars[i].m_destPos;
			}

			if (m_bars[i].m_curPos > m_bars[i].m_destPos)
				m_bars[i].m_curPos = m_bars[i].m_destPos;
		}

		if (m_bars[i].m_curPos > m_bars[i].m_destPos) {
			if (m_destTime > FLT_EPSILON) {
				m_bars[i].m_curPos -= m_bars[i].m_destPos / m_destTime * dt;
			} else {
				m_bars[i].m_curPos = m_bars[i].m_destPos;
			}

			if (m_bars[i].m_curPos < m_bars[i].m_destPos)
				m_bars[i].m_curPos = m_bars[i].m_destPos;
		}
	}
}

// playerdlg.cpp:113
void CStatBar::SetPos(eLvlType type, int pos, int range, int maxValue) {
	m_bars[type].m_destPos = 0.0f > (float)pos ? 0.0f : (float)pos;
	m_bars[type].m_rangePos = (float)range;
	m_bars[type].m_maxValue = maxValue;
}

// playerdlg.cpp:146 (run-time type and factory registration of FrSubEquipBar)
const WRTTI FrSubEquipBar::m_RTTI("FrSubEquipBar", &FrForm::m_RTTI);

IObject* FrSubEquipBarMakeInstance(void) {
	return new FrSubEquipBar;
}

struct __sFrSubEquipBar {
	__sFrSubEquipBar(void) {
		ObjectFactory().AddObjectFunctor(FrSubEquipBarMakeInstance, "FrSubEquipBar");
	}
};
static __sFrSubEquipBar s_registerFrSubEquipBar;

// playerdlg.cpp:147
const sFRESH_MSGMAP* FrSubEquipBar::GetMessageMap(void) const {
	return &FrSubEquipBar::_MsgMap;
}

sFRESH_MSGMAP FrSubEquipBar::_MsgMap = { &FrForm::_MsgMap, &FrSubEquipBar::_MsgEntries[0] };

sFRESH_ENTRY FrSubEquipBar::_MsgEntries[] = {
	// playerdlg.cpp:149
	{ "sub_equip", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSubEquipBar::OnInitSubEquipList },
	// playerdlg.cpp:150
	{ "sub_equip", FRCMD_LBUTTONDOWN, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSubEquipBar::OnDownSubEquipList },
	// playerdlg.cpp:151
	{ "sub_equip", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSubEquipBar::OnDrawSubEquipList },
	// playerdlg.cpp:153
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// playerdlg.cpp:156
FrSubEquipBar::FrSubEquipBar(void) {
	m_result = 0;
}

// playerdlg.cpp:161
FrSubEquipBar::~FrSubEquipBar(void) {
}

// playerdlg.cpp:471
void FrSubEquipBar::OnInitSubEquipList(intptr_t param) {
	m_pSubEquipList = DYNAMIC_CAST(FrListBox, param);
	m_pSubEquipList->SetVisible(true);
}

// playerdlg.cpp:478
void FrSubEquipBar::OnDownSubEquipList(int param) {
	FrListItem* pSelected = m_pSubEquipList->GetSelected();
	if (pSelected == NULL) {
		return;
	}

	_sortitem* pItem = (_sortitem*)pSelected->pData;
	if (pItem != NULL) {
		m_result = pItem->m_pItem;
		m_lastSelectedGuid = pItem->m_guid;
	} else {
		if (m_category != 7 && m_category != 0x10) {
			return;
		}
		m_result = NULL;
		m_lastSelectedGuid = 0;
	}

	FrForm::OnOK();
}

// playerdlg.cpp:629
void FrSubEquipBar::OnDraw(void) {
	FrForm::OnDraw();
}

// playerdlg.cpp:634
void FrSubEquipBar::OnProc(const float dt) {
	FrForm::OnProc(dt);
	if (g_input->GetButton(LEFT_BUTTON) == 1) {
		WRect rect = m_rect;
		WPoint pt(g_input->GetMousePoint().x, g_input->GetMousePoint().y);
		if (!(pt.x >= rect.x && pt.x <= rect.x + rect.w && pt.y >= rect.y && rect.y + rect.h >= pt.y))
			FrForm::OnCancel();
	}
}

// playerdlg.cpp:647
void FrSubEquipBar::OnOK(void) {
	FrForm::OnOK();
}

// playerdlg.cpp:652
void FrSubEquipBar::OnCancel(void) {
	FrForm::OnCancel();
}

// playerdlg.cpp:657
void FrSubEquipBar::Open(FRESH_RESULT_CALLBACK pfnResult, eCategory category, const WPoint& pt, eSubType subType) {
	m_category = category;
	m_subType = subType;
	Initialize();
	FrForm::Open(pfnResult, pt, 1);
}

// playerdlg.cpp:741
CEquipHelper::CEquipHelper(void) {
}

// playerdlg.cpp:746
CEquipHelper::~CEquipHelper(void) {
}

// playerdlg.cpp:1051 (run-time type and factory registration of FrPlayerDlg)
const WRTTI FrPlayerDlg::m_RTTI("FrPlayerDlg", &FrForm::m_RTTI);

IObject* FrPlayerDlgMakeInstance(void) {
	return new FrPlayerDlg;
}

struct __sFrPlayerDlg {
	__sFrPlayerDlg(void) {
		ObjectFactory().AddObjectFunctor(FrPlayerDlgMakeInstance, "FrPlayerDlg");
	}
};
static __sFrPlayerDlg s_registerFrPlayerDlg;

// playerdlg.cpp:1053
const sFRESH_MSGMAP* FrPlayerDlg::GetMessageMap(void) const {
	return &FrPlayerDlg::_MsgMap;
}

// playerdlg.cpp:1099
FrPlayerDlg::~FrPlayerDlg(void) {
	if (m_pExhibition != NULL) { delete m_pExhibition; m_pExhibition = NULL; } if (m_pPartTidList != NULL) { delete m_pPartTidList; m_pPartTidList = NULL; }
}

// playerdlg.cpp:1104
void FrPlayerDlg::Init(int index) {
	m_playerIndex = index;

	if (m_pIndexArea != NULL)
		m_pIndexArea->SetBgImg(MakeStr("%dp", index + 1));

	if (m_pRemoveBtn != NULL)
		m_pRemoveBtn->SetVisible(m_playerIndex >= 2);

	SetVisible(strlen(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_nickname) != 0);
}

// playerdlg.cpp:1117
void FrPlayerDlg::ShowPet(bool show) {
	HidePet();

	if (m_pPetArea == NULL)
		return;

	if (strlen(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_nickname) == 0)
		return;

	WRect rect = m_pPetArea->GetRect();

	m_pExhibition = new CExhibition(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f, 0.35f, true);
	m_pExhibition->SetDistanceRange(15.0f, 25.0f);
	m_pExhibition->SetDistance(20.0f, true, false);
	m_pExhibition->SetArea(rect.w, rect.h);

	m_pPartTidList = new CPartTidList;

	if (show)
		SetRandomState();
	else
		SetState();
}

// playerdlg.cpp:1142
void FrPlayerDlg::HidePet(void) {
	if (m_pExhibition != NULL) { delete m_pExhibition; m_pExhibition = NULL; }
	if (m_pPartTidList != NULL) { delete m_pPartTidList; m_pPartTidList = NULL; }
}

// playerdlg.cpp:1148
void FrPlayerDlg::SetState(void) {
	int length = strlen(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_nickname);
	if (length != 0) {
		if (m_pNameEdit)
			m_pNameEdit->SetLine(1, WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_nickname, 0, false, 0);

		SetCharControl(eButtonDirCurrent);
		SetCaddieControl(eButtonDirCurrent);
		SetClubControl(eButtonDirCurrent);
		SetBallControl(eButtonDirCurrent);
	}
}

// playerdlg.cpp:1162
void FrPlayerDlg::SetRandomState(void) {
	if (m_pNameEdit)
		m_pNameEdit->SetLine(1, WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_nickname, 0, false, 0);

	SetCharControl(eButtonDirRandom);
	SetCaddieControl(eButtonDirRandom);
	SetClubControl(eButtonDirRandom);
	SetBallControl(eButtonDirRandom);
}

// playerdlg.cpp:1173
void FrPlayerDlg::OnCustomProc(float dt) {
	if (m_pExhibition != NULL) {
		m_pExhibition->Process(dt, true);
	}
}

// playerdlg.cpp:1179
void FrPlayerDlg::OnPetOwnerDraw(void) {
	if (m_pExhibition != NULL) {
		m_pExhibition->Display(0, 0);
	}
}

// playerdlg.cpp:1185
void FrPlayerDlg::SetTeam(unsigned char team) {
	m_team = team;

	if (m_pTeamBtn == NULL) {
		return;
	}
	switch (team) {
	case 0:
		m_pTeamBtn->SetButtonImg("team_base_red", FrButton::NORMAL);
		break;
	case 1:
		m_pTeamBtn->SetButtonImg("team_base_blue", FrButton::NORMAL);
		break;
	case 2:
		m_pTeamBtn->SetButtonImg("team_base", FrButton::NORMAL);
		break;
	}
}

// playerdlg.cpp:1208
void FrPlayerDlg::OnBaseInit(intptr_t param) {
	m_pTeamBtn = DYNAMIC_CAST(FrButton, param);
}

// playerdlg.cpp:1213
void FrPlayerDlg::OnBaseBtnUp(void) {
	AfxGetTask()->GetActor("Family") << MsgObject(0, 0xE3, m_playerIndex, 0, 0, 0, 0);
}

// playerdlg.cpp:1218
void FrPlayerDlg::OnSelectInit(intptr_t param) {
	m_pSelectWnd = DYNAMIC_CAST(FrArea, param);
	m_pSelectWnd->SetVisible(false);
}

// playerdlg.cpp:1224
void FrPlayerDlg::Select(void) {
	if (m_pSelectWnd != NULL) {
		m_pSelectWnd->SetVisible(true);
	}
}

// playerdlg.cpp:1230
void FrPlayerDlg::UnSelect(void) {
	if (m_pSelectWnd != NULL) {
		m_pSelectWnd->SetVisible(false);
	}
}

// playerdlg.cpp:1236
void FrPlayerDlg::OnPetInit(int param) {
	m_pPetArea = DYNAMIC_CAST(FrArea, param);
}

// playerdlg.cpp:1241
void FrPlayerDlg::OnCharPrevInit(int param) {
	m_pCharPrevBtn = DYNAMIC_CAST(FrButton, param);
	m_pCharPrevBtn->SetPushDelay(0.0f);
}

// playerdlg.cpp:1247
void FrPlayerDlg::OnCharPrevBtnUp(void) {
	std::map<unsigned int, sCharacterInfo>& characterMap = WSingleton<CSharedDoc>::Instance()->m_characterMap;
	if (characterMap.size() != 0) {
		std::map<unsigned int, sCharacterInfo>::const_iterator it = characterMap.find(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_characterId);
		--it;
		WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_characterId = it->second.id;
		WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_characterInfo = it->second;
		SetCharacter();
		SetLevelBar();
		if (m_pCharPrevBtn != NULL)
			m_pCharPrevBtn->Enable(it != characterMap.begin());
		if (m_pCharNextBtn != NULL)
			m_pCharNextBtn->Enable(++it != characterMap.end());
	}
}

// playerdlg.cpp:1252
void FrPlayerDlg::OnCharNextInit(int param) {
	m_pCharNextBtn = DYNAMIC_CAST(FrButton, param);
	m_pCharNextBtn->SetPushDelay(0.0f);
}

// playerdlg.cpp:1258
void FrPlayerDlg::OnCharNextBtnUp(void) {
	std::map<unsigned int, sCharacterInfo>& characterMap = WSingleton<CSharedDoc>::Instance()->m_characterMap;
	if (characterMap.size() != 0) {
		std::map<unsigned int, sCharacterInfo>::const_iterator it = characterMap.find(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_characterId);
		++it;
		WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_characterId = it->second.id;
		WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_characterInfo = it->second;
		SetCharacter();
		SetLevelBar();
		if (m_pCharPrevBtn != NULL)
			m_pCharPrevBtn->Enable(it != characterMap.begin());
		if (m_pCharNextBtn != NULL)
			m_pCharNextBtn->Enable(++it != characterMap.end());
	}
}

// playerdlg.cpp:1263
void FrPlayerDlg::OnCaddieInit(int param) {
	m_pCaddieArea = DYNAMIC_CAST(FrArea, param);
}

// playerdlg.cpp:1268
void FrPlayerDlg::OnCaddieLimitInit(int param) {
	m_pCaddieLimitArea = DYNAMIC_CAST(FrArea, param);
}

// playerdlg.cpp:1273
void FrPlayerDlg::OnCaddiePrevInit(int param) {
	m_pCaddiePrevBtn = DYNAMIC_CAST(FrButton, param);
	m_pCaddiePrevBtn->SetPushDelay(0.0f);
}

// playerdlg.cpp:1279
void FrPlayerDlg::OnCaddiePrevBtnUp(void) {
	SetCaddieControl(eButtonDirPrev);
}

// playerdlg.cpp:1284
void FrPlayerDlg::OnCaddieNextInit(int param) {
	m_pCaddieNextBtn = DYNAMIC_CAST(FrButton, param);
	m_pCaddieNextBtn->SetPushDelay(0.0f);
}

// playerdlg.cpp:1290
void FrPlayerDlg::OnCaddieNextBtnUp(void) {
	SetCaddieControl(eButtonDirNext);
}

// playerdlg.cpp:1295
void FrPlayerDlg::OnClubInit(int param) {
	m_pClubArea = DYNAMIC_CAST(FrArea, param);
}

// playerdlg.cpp:1300
void FrPlayerDlg::OnClubLimitInit(int param) {
	m_pClubLimitArea = DYNAMIC_CAST(FrArea, param);
}

// playerdlg.cpp:1305
void FrPlayerDlg::OnClubPrevInit(int param) {
	m_pClubPrevBtn = DYNAMIC_CAST(FrButton, param);
	m_pClubPrevBtn->SetPushDelay(0.0f);
}

// playerdlg.cpp:1316
void FrPlayerDlg::OnClubNextInit(int param) {
	m_pClubNextBtn = DYNAMIC_CAST(FrButton, param);
	m_pClubNextBtn->SetPushDelay(0.0f);
}

// playerdlg.cpp:1327
void FrPlayerDlg::OnBallInit(int param) {
	m_pBallArea = DYNAMIC_CAST(FrArea, param);
}

// playerdlg.cpp:1332
void FrPlayerDlg::OnBallPrevInit(int param) {
	m_pBallPrevBtn = DYNAMIC_CAST(FrButton, param);
	m_pBallPrevBtn->SetPushDelay(0.0f);
}

// playerdlg.cpp:1338
void FrPlayerDlg::OnBallPrevBtnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_myGearList.size() != 0) {
		unsigned long& ballId = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_ballId;
		unsigned long typeId = ballId;
		std::list<sItemInfo>& gearList = WSingleton<CSharedDoc>::Instance()->m_myGearList;
		std::list<sItemInfo>::iterator it = gearList.begin();
		while (it != gearList.end() && it->m_typeId != typeId)
			++it;
		--it;
		ballId = it->m_typeId;
		SetBall(it->m_typeId);
		SetLevelBar();
		m_pBallNextBtn->Enable(++it != gearList.end());
		if (m_pBallPrevBtn != NULL)
			m_pBallPrevBtn->Enable(it != gearList.begin());
	}
}

// playerdlg.cpp:1343
void FrPlayerDlg::OnBallNextInit(int param) {
	m_pBallNextBtn = DYNAMIC_CAST(FrButton, param);
	m_pBallNextBtn->SetPushDelay(0.0f);
}

// playerdlg.cpp:1349
void FrPlayerDlg::OnBallNextBtnUp(void) {
	if (WSingleton<CSharedDoc>::Instance()->m_myGearList.size() != 0) {
		unsigned long& ballId = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_ballId;
		unsigned long typeId = ballId;
		std::list<sItemInfo>& gearList = WSingleton<CSharedDoc>::Instance()->m_myGearList;
		std::list<sItemInfo>::iterator it = gearList.begin();
		while (it != gearList.end() && it->m_typeId != typeId)
			++it;
		++it;
		ballId = it->m_typeId;
		SetBall(it->m_typeId);
		SetLevelBar();
		m_pBallNextBtn->Enable(++it != gearList.end());
		if (m_pBallPrevBtn != NULL)
			m_pBallPrevBtn->Enable(it != gearList.begin());
	}
}

// playerdlg.cpp:1354
void FrPlayerDlg::OnRemoveInit(int param) {
	m_pRemoveBtn = DYNAMIC_CAST(FrButton, param);
	m_pRemoveBtn->SetPushDelay(0.0f);
}

// playerdlg.cpp:1360
void FrPlayerDlg::OnRemoveBtnUp(void) {
	SetVisible(false);

	if (m_pLinkedWnd) {
		m_pLinkedWnd->SetVisible(true);
	}

	WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_nickname[0] = 0;
	WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_name[0] = 0;

	AfxGetTask()->GetActor("Family") << MsgObject(0, 0xE2, m_playerIndex, 0, 0, 0, 0);
}

// playerdlg.cpp:1373
void FrPlayerDlg::OnNameInit(intptr_t param) {
	m_pNameEdit = DYNAMIC_CAST(FrEdit, param);
}

// playerdlg.cpp:1378
const char* FrPlayerDlg::GetName(void) {
	if (m_pNameEdit != NULL) {
		return m_pNameEdit->GetLine(1, false);
	}
	return NULL;
}

// playerdlg.cpp:1386
void FrPlayerDlg::SetName(const char* name) {
	if (m_pNameEdit) {
		m_pNameEdit->SetLine(1, name, 0, false, 0);
	}

	strcpy(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_nickname, name);
	strcpy(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_name, name);
}

// playerdlg.cpp:1395
void FrPlayerDlg::OnIndexInit(intptr_t param) {
	m_pIndexArea = DYNAMIC_CAST(FrArea, param);
}

int CalcPowerPenalty(int power);

int CalcPowerPenalty(int power);

// playerdlg.cpp:1400
void FrPlayerDlg::SetLevelBar(void) {
	sUserInfo& player = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex];
	unsigned long charTypeId = player.m_characterInfo.typeId;
	const char* charLevels = (const char*)&player.m_characterInfo.unknown6A;
	unsigned long clubTypeId = player.m_clubSetTypeId;
	const short* clubLevels = player.m_clubStat;
	unsigned long caddieId = player.m_equipHead;
	std::map<unsigned int, sCharacterInfo>::iterator charIt = WSingleton<CSharedDoc>::Instance()->m_characterMap.find(player.m_characterId);
	const unsigned long* parts = charIt->second.partTypeIds;
	const unsigned long* cards = charIt->second.auxParts.tids;
	unsigned long caddieTypeId;

	if (caddieId == 0) {
		caddieTypeId = caddieId;
	} else {
		caddieTypeId = WSingleton<CSharedDoc>::Instance()->m_caddieMap.find(caddieId)->second.typeId;
	}

	unsigned char myLevel = WSingleton<CSharedDoc>::Instance()->m_myLevel;
	int penalty = CalcPowerPenalty(myLevel);

	if (IsLocalContent((localContentType_t)0x51)) {
		WSingleton<CCardManager>::Instance()->SetPlayerIndex(0xFF);
		WSingleton<CCardManager>::Instance()->CalcCardPeriodAndStatus();
	}

	for (int i = 0; i < 5; i++) {
		unsigned char level = ItemManager()->GetLevel((eLvlType)i, charTypeId, charLevels, parts, cards, clubTypeId, clubLevels, caddieTypeId, myLevel);
		unsigned char shown = level;

		switch (i) {
		case 0:
			if (m_pStatGauge[0] != NULL) {
				int excess = level - penalty - 20;
				m_pStatGauge[1]->SetExpand(-(excess > 0 ? excess : 0));
				m_pStatGauge[2]->SetExpand(-(excess > 0 ? excess : 0));
			}
			break;
		case 1:
		case 2:
			shown = ItemManager()->GetLevelWithPenalty((eLvlType)i, charTypeId, charLevels, parts, cards, clubTypeId, clubLevels, caddieTypeId, myLevel).m_level;
			break;
		}

		if (m_pLevelEdit[(unsigned char)i] != NULL) {
			m_pLevelEdit[(unsigned char)i]->SetLine(1, MakeStr("\\c0xffff0000\\c%d\\c0xff000000\\c/%d", shown, level), 0, false, 0);
		}

		if (m_pStatGauge[i] != NULL) {
			m_pStatGauge[i]->SetDestPos(level, false);
		}
	}
}

// playerdlg.cpp:1484
void FrPlayerDlg::SetLevelNum(unsigned char type, unsigned char level, unsigned char maxLevel) {
	if (m_pLevelEdit[type] != NULL) {
		m_pLevelEdit[type]->SetLine(1, MakeStr("\\c0xffff0000\\c%d\\c0xff000000\\c/%d", level, maxLevel), 0, false, 0);
	}
}

// playerdlg.cpp:1490
void FrPlayerDlg::SetCharControl(eButtonDir dir) {
	std::map<unsigned int, sCharacterInfo>& characterMap = WSingleton<CSharedDoc>::Instance()->m_characterMap;

	if (characterMap.size() == 0)
		return;

	std::map<unsigned int, sCharacterInfo>::const_iterator it;

	if (dir == eButtonDirRandom) {
		unsigned int count = rand() % characterMap.size();

		for (it = characterMap.begin(); it != characterMap.end(); ++it, count--) {
			if (count == 0)
				break;
		}
	} else {
		it = characterMap.find(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_characterId);
	}

	switch (dir) {
	case eButtonDirPrev:
		--it;
		break;
	case eButtonDirNext:
		++it;
		break;
	}

	WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_characterId = it->second.id;
	WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_characterInfo = it->second;
	SetCharacter();
	SetLevelBar();

	if (m_pCharPrevBtn != NULL)
		m_pCharPrevBtn->Enable(it != characterMap.begin());

	if (m_pCharNextBtn != NULL)
		m_pCharNextBtn->Enable(++it != characterMap.end());
}

// playerdlg.cpp:1672
void FrPlayerDlg::SetClubControl(eButtonDir dir) {
	std::map<unsigned int, sItemInfo>& itemMap = WSingleton<CSharedDoc>::Instance()->m_myItemMap;

	if (itemMap.size() == 0)
		return;

	std::map<unsigned int, sItemInfo>::iterator it;

	if (dir == eButtonDirRandom) {
		unsigned int count = rand() % itemMap.size();

		for (it = WSingleton<CSharedDoc>::Instance()->m_myItemMap.begin(); it != WSingleton<CSharedDoc>::Instance()->m_myItemMap.end(); ++it, count--) {
			if (count == 0)
				break;
		}
	} else {
		unsigned int clubId = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_clubSetId;

		it = WSingleton<CSharedDoc>::Instance()->m_myItemMap.find(clubId);
	}

	switch (dir) {
	case eButtonDirPrev:
		--it;
		break;
	case eButtonDirNext:
		++it;
		break;
	}

	WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_clubSetTypeId = it->second.m_typeId;
	WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_clubSetId = it->second.m_serial;
	memcpy(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_clubStat, it->second.m_clubStatLevel, sizeof(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_clubStat));
	SetClub(it->second.m_typeId);
	SetLevelBar();

	if (m_pClubPrevBtn != NULL)
		m_pClubPrevBtn->Enable(it != WSingleton<CSharedDoc>::Instance()->m_myItemMap.begin());

	if (m_pClubNextBtn != NULL)
		m_pClubNextBtn->Enable(++it != WSingleton<CSharedDoc>::Instance()->m_myItemMap.end());
}

// playerdlg.cpp:1726
void FrPlayerDlg::SetBallControl(eButtonDir dir) {
	std::list<sItemInfo>& gearList = WSingleton<CSharedDoc>::Instance()->m_myGearList;

	if (gearList.size() == 0)
		return;

	std::list<sItemInfo>::iterator it;

	if (dir == eButtonDirRandom) {
		unsigned int count = rand() % gearList.size();

		for (it = gearList.begin(); it != gearList.end(); ++it, count--) {
			if (count == 0)
				break;
		}
	} else {
		unsigned long typeId = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_ballId;

		for (it = gearList.begin(); it != gearList.end(); ++it) {
			if (it->m_typeId == typeId)
				break;
		}
	}

	switch (dir) {
	case eButtonDirPrev:
		--it;
		break;
	case eButtonDirNext:
		++it;
		break;
	}

	WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_ballId = it->m_typeId;
	SetBall(it->m_typeId);
	SetLevelBar();

	if (m_pBallPrevBtn != NULL)
		m_pBallPrevBtn->Enable(it != gearList.begin());

	if (m_pBallNextBtn != NULL)
		m_pBallNextBtn->Enable(++it != gearList.end());
}

// playerdlg.cpp:1849
void FrPlayerDlg::SetCharacter(void) {
	if (m_pPartTidList != NULL) {
		m_pPartTidList->SetTids(&WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_characterInfo, 0xFF);
	}
	if (m_pExhibition != NULL) {
		m_pExhibition->SetModel(*m_pPartTidList, WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_playerIndex].m_characterInfo.auxParts.tids, 0, 3.0f, m_playerIndex);
	}
}

// playerdlg.cpp:1926
void FrPlayerDlg::SetClub(unsigned long typeId) {
	if (m_pClubArea == NULL) {
		return;
	}

	IFF_STRUCT::sClubSet* pClubSet = ItemManager()->FindClubSet(typeId);

	if (pClubSet == NULL) {
		return;
	}

	m_pClubArea->SetBgImg(pClubSet->m_icon);

	if (m_pClubLimitArea != NULL)
		m_pClubLimitArea->SetVisible(WSingleton<CSharedDoc>::Instance()->m_myLevel < (pClubSet->m_level & 0x7F));
}

// playerdlg.cpp:1942
void FrPlayerDlg::SetBall(unsigned long typeId) {
	if (m_pBallArea == NULL) {
		return;
	}

	IFF_STRUCT::sBall* pBall = ItemManager()->FindBall(typeId);

	if (pBall == NULL) {
		return;
	}

	if (m_pBallArea != NULL) {
		m_pBallArea->SetBgImg(pBall->m_icon);
	}
}

// playerdlg.cpp:1956
void FrPlayerDlg::OnPowerBarInit(intptr_t param) {
	m_pStatGauge[0] = DYNAMIC_CAST(FrGaugeBar, param);
}

// playerdlg.cpp:1961
void FrPlayerDlg::OnControlBarInit(intptr_t param) {
	m_pStatGauge[1] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pStatGauge[1]->SetRange(0, 30, 0);
}

// playerdlg.cpp:1967
void FrPlayerDlg::OnImpactBarInit(intptr_t param) {
	m_pStatGauge[2] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pStatGauge[2]->SetRange(0, 30, 0);
}

// playerdlg.cpp:1973
void FrPlayerDlg::OnSpinBarInit(intptr_t param) {
	m_pStatGauge[3] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pStatGauge[3]->SetRange(0, 30, 0);
}

// playerdlg.cpp:1979
void FrPlayerDlg::OnCurveBarInit(intptr_t param) {
	m_pStatGauge[4] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pStatGauge[4]->SetRange(0, 30, 0);
}

// playerdlg.cpp:1987
void FrPlayerDlg::OnPowerEditInit(int param) {
	m_pLevelEdit[0] = DYNAMIC_CAST(FrEdit, param);
}

// playerdlg.cpp:1992
void FrPlayerDlg::OnControlEditInit(int param) {
	m_pLevelEdit[1] = DYNAMIC_CAST(FrEdit, param);
}

// playerdlg.cpp:1997
void FrPlayerDlg::OnImpactEditInit(int param) {
	m_pLevelEdit[2] = DYNAMIC_CAST(FrEdit, param);
}

// playerdlg.cpp:2002
void FrPlayerDlg::OnSpinEditInit(int param) {
	m_pLevelEdit[3] = DYNAMIC_CAST(FrEdit, param);
}

// playerdlg.cpp:2007
void FrPlayerDlg::OnCurveEditInit(int param) {
	m_pLevelEdit[4] = DYNAMIC_CAST(FrEdit, param);
}

// playerdlg.cpp:2014 (run-time type and factory registration of FrPlayerEmptyDlg)
const WRTTI FrPlayerEmptyDlg::m_RTTI("FrPlayerEmptyDlg", &FrForm::m_RTTI);

IObject* FrPlayerEmptyDlgMakeInstance(void) {
	return new FrPlayerEmptyDlg;
}

struct __sFrPlayerEmptyDlg {
	__sFrPlayerEmptyDlg(void) {
		ObjectFactory().AddObjectFunctor(FrPlayerEmptyDlgMakeInstance, "FrPlayerEmptyDlg");
	}
};
static __sFrPlayerEmptyDlg s_registerFrPlayerEmptyDlg;

// playerdlg.cpp:2016
const sFRESH_MSGMAP* FrPlayerEmptyDlg::GetMessageMap(void) const {
	return &FrPlayerEmptyDlg::_MsgMap;
}

sFRESH_MSGMAP FrPlayerEmptyDlg::_MsgMap = { &FrForm::_MsgMap, &FrPlayerEmptyDlg::_MsgEntries[0] };

sFRESH_ENTRY FrPlayerEmptyDlg::_MsgEntries[] = {
	// playerdlg.cpp:2018
	{ "base", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPlayerEmptyDlg::OnBaseInit },
	// playerdlg.cpp:2019
	{ "create", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPlayerEmptyDlg::OnCreateInit },
	// playerdlg.cpp:2020
	{ "create", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPlayerEmptyDlg::OnCreateBtnUp },
	// playerdlg.cpp:2021
	{ "index", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPlayerEmptyDlg::OnIndexInit },
	// playerdlg.cpp:2023
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// playerdlg.cpp:2026
void FrPlayerEmptyDlg::Init(int index) {
	m_index = index;

	if (m_pBgArea != NULL)
		m_pBgArea->SetBgImg(MakeStr("%dp", index + 1));

	SetVisible(strlen(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_index].m_nickname) == 0);
}

// playerdlg.cpp:2036
void FrPlayerEmptyDlg::OnBaseInit(intptr_t param) {
	m_pBaseArea = DYNAMIC_CAST(FrArea, param);
}

// playerdlg.cpp:2041
void FrPlayerEmptyDlg::OnCreateInit(intptr_t param) {
	m_pCreateBtn = DYNAMIC_CAST(FrButton, param);
	m_pCreateBtn->SetPushDelay(0.0f);
}

// playerdlg.cpp:2047
void FrPlayerEmptyDlg::OnCreateBtnUp(void) {
	SetVisible(false);

	if (m_pPlayerDlg != NULL) {
		sprintf(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_index].m_nickname, "PLAYER%d", m_index + 1);
		sprintf(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[m_index].m_name, "PLAYER%d", m_index + 1);
		m_pPlayerDlg->ShowPet(true);
		m_pPlayerDlg->SetVisible(true);
	}

	AfxGetTask()->GetActor("Family") << MsgObject(0, 0xE2, m_index, 1, 0, 0, 0);
	AfxGetTask()->GetActor("Family") << MsgObject(0, 0xE3, m_index, 0, 0, 0, 0);
}

// playerdlg.cpp:2063
void FrPlayerEmptyDlg::OnIndexInit(intptr_t param) {
	m_pBgArea = DYNAMIC_CAST(FrArea, param);
}
