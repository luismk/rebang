// Original: d:\build\custom\temp\code\client\projectg\voiceitem.cpp
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "voiceitem.h"
#include "BonusTicket.h"
#include "commonutil.h"
#include "gdvoiceitem.h"
#include "gamedatadb.h"
#include "GolfDoc.h"
#include "parttidlist.h"
#include "SharedDoc.h"
#include "SoundManager.h"
#include "wresrcmng.h"

// Nameless global: points to the CP949 string "\xC6\xCE\xBE\xDF" (0x9E90AC).
extern "C" const char* data_a7d720;

// SharedDoc.cpp (?IsMassGame@@YIHXZ); not declared in SharedDoc.h yet.
int IsMassGame(void);

template <> CVoiceItem* WSingleton<CVoiceItem>::m_pInstance = NULL;

// voiceitem.cpp:19
void sVOICECLUBINSERT::operator()(const sGD_VOICEITEM& item) const {
	CVoiceItem::Instance()->SetSound(item.nTypeID, item.strName, item.nState, item.byNumOfSounds);
}

// voiceitem.cpp:25
CVoiceItem::CVoiceItem(void) {
}

// voiceitem.cpp:29
CVoiceItem::~CVoiceItem(void) {
}

// voiceitem.cpp:33
bool CVoiceItem::PlaySound(unsigned long dwTypeID, eWaveState eState, int nIndex) {
	unsigned long dwKind = dwTypeID >> 26;
	switch (dwKind) {
	case 4:
		return ClubPlaySound(dwTypeID, eState, nIndex);
	case 5:
		return AztecPlaySound(dwTypeID, eState);
	}
	return false;
}

// voiceitem.cpp:48
void CVoiceItem::Initialize(void) {
	CGDVoiceItem* pVoiceItem = (CGDVoiceItem*)CGameDataDB::Instance()->GetGameData(eGAMEDATADB_VOICEITEM, "VoiceClub");
	if (pVoiceItem) {
		std::for_each(pVoiceItem->GetVoiceList().begin(), pVoiceItem->GetVoiceList().end(), sVOICECLUBINSERT());
	}
}

// voiceitem.cpp:60
void CVoiceItem::Destroy(void) {
	std::map<unsigned long, cVoice*>::iterator it = m_mapVoice.begin();
	while (it != m_mapVoice.end()) {
		if (it->second) {
			delete it->second;
			it->second = 0;
			m_mapVoice.erase(it++);
		} else {
			++it;
		}
	}
	m_mapVoice.clear();
}

// voiceitem.cpp:80
void CVoiceItem::SetSound(unsigned long dwTypeID, std::string strName, unsigned long dwState, unsigned char byNumOfSounds) {
	cVoice* pVoice = new cVoice;
	pVoice->m_waveState = dwState;
	pVoice->m_fileName = strName;
	pVoice->m_numOfSounds = byNumOfSounds;

	m_mapVoice.insert(std::map<unsigned long, cVoice*>::value_type(dwTypeID, pVoice));
}

// voiceitem.cpp:91
void CVoiceItem::SetSound(unsigned long dwTypeID, std::string strName, eWaveState eState, unsigned char byNumOfSounds) {
	cVoice* pVoice = new cVoice;
	pVoice->m_waveState = eState;
	pVoice->m_fileName = strName;
	pVoice->m_numOfSounds = byNumOfSounds;

	m_mapVoice.insert(std::map<unsigned long, cVoice*>::value_type(dwTypeID, pVoice));
}

// voiceitem.cpp:102
bool CVoiceItem::ClubPlaySound(unsigned long dwTypeID, eWaveState eState, int nIndex) {
	std::string strFileName = "";
	cVoice* pVoice = NULL;

	std::map<unsigned long, cVoice*>::iterator it = m_mapVoice.find(dwTypeID);
	if (it != m_mapVoice.end()) {
		while (it != m_mapVoice.upper_bound(dwTypeID)) {
			cVoice* pCur = it->second;
			if (pCur->m_waveState & eState) {
				pVoice = pCur;
				strFileName = pCur->m_fileName;
				break;
			}
			++it;
		}
	}

	if (strFileName.empty()) {
		return false;
	}

	int nCharacter = nIndex;
	if (nCharacter == -1) {
		nCharacter = CSharedDoc::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_characterInfo.typeId & 0x3FFFFFF;
	}

	char szFileName[128] = { 0 };
	unsigned long dwClub = dwTypeID & 0x3FFFFFF;
	switch (dwClub) {
	case 0x17:
		if (pVoice->m_numOfSounds > 1) {
			int nRand;
			if (IsMassGame()) {
				nRand = rand() % 2;
			} else {
				nRand = CBonusTicket::Instance().GetRandomTicket(std::string("CLUBSET_MIRACLE")) % pVoice->m_numOfSounds;
			}
			sprintf(szFileName, strFileName.c_str(), nCharacter, nRand);
			if (!IsExistVoiceFile(szFileName)) {
				sprintf(szFileName, strFileName.c_str(), nCharacter, 0);
			}
			strFileName = szFileName;
		} else {
			sprintf(szFileName, strFileName.c_str(), nCharacter);
			strFileName = szFileName;
		}
		break;

	case 0x1F:
		sprintf(szFileName, strFileName.c_str(), nCharacter);
		strFileName = szFileName;
		break;

	case 0x35:
		if (pVoice->m_numOfSounds > 1) {
			int nRand = rand() % 4;
			sprintf(szFileName, strFileName.c_str(), nRand);
			strFileName = szFileName;
		}
		break;

	case 0x3F: {
		int nRand = 0;
		if (pVoice->m_numOfSounds > 1) {
			nRand = rand() % pVoice->m_numOfSounds;
		}
		sprintf(szFileName, strFileName.c_str(), nCharacter, nRand);
		strFileName = szFileName;
		if (!IsExistVoiceFile(szFileName)) {
			return false;
		}
		break;
	}

	case 0x44:
	case 0x45: {
		std::string strFormat = strFileName;
		if (nCharacter == 8) {
			std::string strNegima = (dwClub == 0x44) ? "negima_club_a_" : "negima_club_b_";
			str_Replace(strFormat, std::string("negima_club_"), strNegima);
		}

		int nRand = 0;
		if (pVoice->m_numOfSounds > 1) {
			nRand = rand() % pVoice->m_numOfSounds;
		}
		sprintf(szFileName, strFormat.c_str(), nCharacter, nRand);
		strFileName = szFileName;
		if (!IsExistVoiceFile(szFileName)) {
			return false;
		}
		break;
	}
	}

	g_audio->PlaySfx(strFileName.c_str(), 0, 0, 0, 0, 0.5f, 200.0f);
	return true;
}

// voiceitem.cpp:413
bool CVoiceItem::AztecPlaySound(unsigned long dwTypeID, eWaveState eState) {
	std::string strFileName = "";

	std::map<unsigned long, cVoice*>::iterator it = m_mapVoice.find(dwTypeID);
	if (it != m_mapVoice.end()) {
		while (it != m_mapVoice.upper_bound(dwTypeID)) {
			cVoice* pVoice = it->second;
			if (pVoice->m_waveState & eState) {
				strFileName = pVoice->m_fileName;
				break;
			}
			++it;
		}
	}

	if (strFileName.empty()) {
		return false;
	}
	g_audio->PlaySfx(strFileName.c_str(), 0, 0, 0, 0, 0.5f, 200.0f);
	return true;
}

// voiceitem.cpp:439
bool CVoiceItem::IsExistVoiceFile(char* szName) {
	std::string strFileName = "";
	std::string strFound = "";

	strFileName = szName;
	strFileName += ".wav";

	strFound = g_resrcmng->FindMatchFile(strFileName.c_str());

	if (strFound.compare(strFileName) == 0) {
		strFound.clear();
		strFileName.clear();
		return false;
	}

	strFileName.clear();
	strFound.clear();
	return true;
}

// voiceitem.cpp:774
void CSoundItemManager::PlayCurrentSoundItem(void) {
	bool bPlayed = false;

	unsigned long dwBallId = CSharedDoc::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId;
	switch (dwBallId >> 26) {
	case 4:
		bPlayed = CVoiceItem::Instance()->ClubPlaySound(dwBallId, (CVoiceItem::eWaveState)2, -1);
		break;
	case 5:
		bPlayed = CVoiceItem::Instance()->AztecPlaySound(dwBallId, (CVoiceItem::eWaveState)2);
		break;
	}
	if (!bPlayed) {
		if (dwBallId == 0x1400001D) {
			g_audio->PlaySfx("HVA_niceshot", 0, 0, 0, 0, 0.5f, 200.0f);
		} else if (dwBallId == 0x1400001C) {
			int nRand = rand() % 8;
			char szName[255] = {0};
			sprintf(szName, "nagashima00%d", nRand + 1);
			g_audio->PlaySfx(szName, 0, 0, 0, 0, 0.5f, 200.0f);
		}
	}

	unsigned long dwClubId = CSharedDoc::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_clubSetTypeId;
	bPlayed = false;
	switch (dwClubId >> 26) {
	case 4:
		bPlayed = CVoiceItem::Instance()->ClubPlaySound(dwClubId, (CVoiceItem::eWaveState)2, -1);
		break;
	case 5:
		bPlayed = CVoiceItem::Instance()->AztecPlaySound(dwClubId, (CVoiceItem::eWaveState)2);
		break;
	}
	if (!bPlayed) {
		if (WearWig(GOLFDOC()->m_myPlayerIndex)) {
			g_audio->PlaySfx(MakeStr("t_Ex8-%d", rand() % 4 + 1), 0, 0, 0, 0, 0.5f, 200.0f);
		} else {
			g_audio->PlaySfx(data_a7d720, 0, 0, 0, 0, 0.5f, 200.0f);
		}
	}
}

// voiceitem.cpp:817
bool WearWig(unsigned char byIndex) {
	// The wig part of each character (indexed by the character's type id).
	static unsigned long s_wigTypeId[7] = {
		0x08000809,
		0x08040811,
		0x08080808,
		0x080C0003,
		0x08100005,
		0x08140003,
		0x08180005,
	};

	CPartTidList* pPartTidList = &GOLFDOC()->GetPartTidList(byIndex);
	if (pPartTidList == NULL) {
		return false;
	}

	unsigned long dwCharacter = CSharedDoc::Instance()->m_roomPlayers[byIndex].m_characterInfo.typeId & 0x3FFFFFF;
	if (dwCharacter >= 7) {
		return false;
	}

	return pPartTidList->IsEquipped(s_wigTypeId[dwCharacter], 0);
}
