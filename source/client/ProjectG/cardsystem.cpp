// Original: d:\build\custom\temp\code\client\projectg\cardsystem.cpp
#include "cardsystem.h"
#include "CardDlg.h"
#include "wind.h"
#include "SharedDoc.h"
#include "itembuffactor.h"

#include <string.h>

extern unsigned long g_CurrentTime;

// cardsystem.cpp:23
CCardManager::CCardManager(void) : m_characterInfo() {
	Init();
	for (int i = 0; i < 5; i++) {
		m_playerIndexMap[i] = 0;
	}
	m_cardBuffDlg = NULL;
	m_spCardAlarm = false;
	m_windColor = 0;
}

// cardsystem.cpp:35
CCardManager::~CCardManager(void) {
}

// cardsystem.cpp:40
void CCardManager::Init(void) {
	m_playerIndex = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			m_cardStatusSlot[i][j] = 0;
			m_cardPeriodStatus[i][j] = 0;
		}

		m_powerRangeDown[i] = 0;

		m_cardPeriodComboGauge[i] = 0.0f;
		m_cardPeriodSlot[i] = 0;
		m_cardPeriodPangRate[i] = 0.0f;
		m_specialCardPangyaZoneUp[i] = 0.0f;
		m_specialClearBonusProb[i] = 0.0f;

		for (int k = 0; k < 13; k++) {
			m_cardAvility[i][k].kind = 0;
			m_cardAvility[i][k].value = 0;
			m_cardAvility[i][k].extra = 0;
		}
	}
	m_specialClearBonusType = 20;
}

// cardsystem.cpp:74
void CCardManager::OnProc(float dt) {
}

// cardsystem.cpp:80
void CCardManager::CalcCardPeriodAndStatus(void) {
	Init();

	int index = 0;

	for (int i = 0; i < 4; i++) {
		if (CSharedDoc::Instance()->m_gameMode == 1) {
			i = 0;
			index++;
			if (index > 5) {
				break;
			}
		} else {
			index = i;
		}

		std::list<sSCardAvilityPeriodInfo>::iterator it = CSharedDoc::Instance()->m_cardPeriodList[i].begin();
		while (it != CSharedDoc::Instance()->m_cardPeriodList[i].end()) {
			if (it->kind == 0 && it->partsTypeId != 0) {
				SetCharacterCardAvility(*it, index);
			} else if (it->kind == 1 && it->partsTypeId != 0) {
				SetCaddieCardAvility(*it, index);
			} else if (it->kind == 2) {
				if (SetSpecialCardAvility(*it, index) == 0) {
					it = CSharedDoc::Instance()->m_cardPeriodList[i].erase(it);
					if (m_cardBuffDlg) {
						CItemBuff::UpdateBuffDlg(m_cardBuffDlg);
					}
					continue;
				}
			}
			++it;
		}
	}
}

// cardsystem.cpp:132
void CCardManager::SetCharacterCardAvility(const sSCardAvilityPeriodInfo& info, int index) {
	sCharacterInfo character;
	if (index == 0) {
		character = CSharedDoc::Instance()->m_characterMap[CSharedDoc::Instance()->m_myCharacterId];
	} else {
		character = m_characterInfo[index];
	}

	IFF_STRUCT::sCard *card = ItemManager()->FindCard(info.typeId);
	if (card == NULL) return;

	for (int i = 0; i < 24; i++) {
		if (info.partsTypeId == character.partTypeIds[i] && info.partsId == character.partIds[i]) {
			for (int k = 0; k < 5; k++) {
				if (card->m_stat[k] > 0)
					m_cardStatusSlot[index][k] += card->m_stat[k];
			}

			switch (card->m_effectType) {
			case 1:
				m_powerRangeDown[index] += card->m_effectValue;
				break;
			}
		}
	}
}

// cardsystem.cpp:166
int CCardManager::SetSpecialCardAvility(const sSCardAvilityPeriodInfo& info, int index) {
	SYSTEMTIME endTime = info.endTime;

	IFF_STRUCT::sCard *card = ItemManager()->FindCard(info.typeId);
	if (card == NULL) {
		return -1;
	}

	int result = ItemManager()->CompareSystemTime(endTime, CSharedDoc::Instance()->GetServerTime());

	if (CSharedDoc::Instance()->m_gameMode == 5 || CSharedDoc::Instance()->m_playingMode == 2)
		result = 1;

	if (result > 0) {
		switch (card->m_effectType) {
		case 5:
			m_cardPeriodStatus[index][0] += card->m_effectValue;
			break;
		case 6:
			m_cardPeriodStatus[index][1] += card->m_effectValue;
			break;
		case 7:
			m_cardPeriodStatus[index][2] += card->m_effectValue;
			break;
		case 8:
			m_cardPeriodStatus[index][3] += card->m_effectValue;
			break;
		case 9:
			m_cardPeriodStatus[index][4] += card->m_effectValue;
			break;
		case 10:
			m_cardPeriodComboGauge[index] = card->m_effectValue;
			break;
		case 11:
			m_cardPeriodSlot[index] = card->m_effectValue;
			break;
		case 2:
			m_cardPeriodPangRate[index] = card->m_effectValue;
			break;
		case 12:
			m_specialCardPangyaZoneUp[index] = card->m_effectValue;
			break;
		case 13:
			m_specialClearBonusProb[index] = (float)(card->m_effectValue * 0.01);
			m_specialClearBonusType = 2;
			break;
		case 14:
			m_specialClearBonusProb[index] = (float)(card->m_effectValue * 0.01);
			m_specialClearBonusType = 3;
			break;
		case 15:
			m_specialClearBonusProb[index] = (float)(card->m_effectValue * 0.01);
			m_specialClearBonusType = 11;
			break;
		case 16:
			m_specialClearBonusProb[index] = (float)(card->m_effectValue * 0.01);
			m_specialClearBonusType = 6;
			break;
		}
	}

	return result;
}

// cardsystem.cpp:276
int CCardManager::GetTotalRange(bool withPowerRange) {
	int range = (int)GetCardRangeUp();
	if (withPowerRange == true) {
		range += (int)GetCardPowerRangeUp();
	}
	return range - GetPowerRangeDown();
}

// cardsystem.cpp:290
float CCardManager::GetTotalPangyaZone(void) {
	float zone = CCardManager::Instance()->GetCardPangyaZoneUp();
	zone += CCardManager::Instance()->GetSpecialCardPangyaZoneUp();
	if (Wind().GetGlobalIntensity() == 1.0f) {
		return zone + CCardManager::Instance()->GetCaddieCardPangYaZoneUpWind1();
	}
	return zone;
}

// cardsystem.cpp:304
int CCardManager::GetPowerRangeDown(void) {
	return m_powerRangeDown[m_playerIndex];
}

// cardsystem.cpp:310
int CCardManager::GetCardPeriodStatus(int slot) {
	int status = 0;
	if (m_cardPeriodStatus[m_playerIndex][slot] > 0) {
		status = m_cardPeriodStatus[m_playerIndex][slot];
	}
	return status;
}

// cardsystem.cpp:319
int CCardManager::GetCardStatusSlot(int slot) {
	int status = 0;
	if (m_cardStatusSlot[m_playerIndex][slot] > 0) {
		status = m_cardStatusSlot[m_playerIndex][slot];
	}
	return status;
}

// cardsystem.cpp:328
float CCardManager::GetCardPeriodComboGauge(void) {
	return m_cardPeriodComboGauge[m_playerIndex];
}

// cardsystem.cpp:333
int CCardManager::GetCardPeriodSlot(void) {
	return m_cardPeriodSlot[m_playerIndex];
}

// cardsystem.cpp:338
float CCardManager::GetCardPeriodPangRate(void) {
	return m_cardPeriodPangRate[m_playerIndex];
}

// cardsystem.cpp:343
int CCardManager::GetCardSuccesssProbUp(void) {
	return m_cardAvility[m_playerIndex][0].value;
}

// cardsystem.cpp:349
float CCardManager::GetCardRangeUp(void) {
	return (float)m_cardAvility[m_playerIndex][1].value;
}

// cardsystem.cpp:355
__int64 CCardManager::GetCardBounceBonusUp(void) {
	return m_cardAvility[m_playerIndex][3].value;
}

// cardsystem.cpp:361
float CCardManager::GetCardPowerRangeUp(void) {
	return (float)m_cardAvility[m_playerIndex][4].value;
}

// cardsystem.cpp:367
int CCardManager::GetCardComboGaugeUp(void) {
	return m_cardAvility[m_playerIndex][5].value;
}

// cardsystem.cpp:373
float CCardManager::GetCardPangyaZoneUp(void) {
	return m_cardAvility[m_playerIndex][6].value * 0.5f;
}

// cardsystem.cpp:379
unsigned char CCardManager::GetWindPowerDown(void) {
	return (unsigned char)m_cardAvility[m_playerIndex][2].value;
}

// cardsystem.cpp:386
float CCardManager::GetSpecialCardPangyaZoneUp(void) {
	return m_specialCardPangyaZoneUp[m_playerIndex] * 0.5f;
}

// cardsystem.cpp:390
float CCardManager::GetSpecialClearBonusProb(unsigned char type) {
	if (type > 19 || type != m_specialClearBonusType) {
		return 0.0f;
	}
	return m_specialClearBonusProb[m_playerIndex];
}

// cardsystem.cpp:400
float CCardManager::GetCaddieCardPangYaZoneUpWind1(void) {
	return m_cardAvility[m_playerIndex][10].value * 0.5f;
}

// cardsystem.cpp:407
void CCardManager::UpdateCardBuffDlg(void) {
	if (m_cardBuffDlg) {
		CItemBuff::UpdateBuffDlg(m_cardBuffDlg);
	}
}

// cardsystem.cpp:415
void CCardManager::InitCardBuffDlgPosition(void) {
	FrSPCardBuffDlg::SetPosition(WPoint(-1.0f, 0.0f));
	m_spCardAlarm = false;
}

// cardsystem.cpp:421
void CCardManager::ResetSpecialBuffTick(void) {
	FrSPCardBuffDlg::SetSpecialBuffStartTick(g_CurrentTime);
}

// cardsystem.cpp:427
void CCardManager::SetPlayerIndex(unsigned char index) {
	if (index == 0xFF) {
		m_playerIndex = 0;
	} else {
		int gameMode = CSharedDoc::Instance()->m_gameMode;
		if (gameMode == 5) {
			CCardManager::Instance()->m_playerIndex = index;
		} else if (CSharedDoc::Instance()->m_playingMode == 2) {
			CCardManager::Instance()->m_playerIndex = 2;
		} else if (gameMode == 1) {
			CCardManager::Instance()->m_playerIndex = index + gameMode;
		} else {
			m_playerIndex = m_playerIndexMap[index];
		}
	}
}

// cardsystem.cpp:443
void CCardManager::SetOffLinePlayerIndex(unsigned char index) {
	m_playerIndex = index;
}
