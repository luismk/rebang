// Original: d:\build\custom\temp\code\client\projectg\fxbox.cpp

#include "FxBox.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ClientSetting.h"
#include "binstr.h"
#include "fxsequence.h"
#include "fxspray.h"
#include "w3danispr.h"
#include "wresrcmng.h"

// fxbox.cpp:32
CFxBox::CFxBox(void) {
	m_pCorners[0] = &m_corners[0];
	m_pCorners[1] = &m_corners[1];
	m_pCorners[2] = &m_corners[2];
	m_pCorners[3] = &m_corners[3];
	m_pResourceManager = 0;
	m_flag110 = false;
	m_value114 = 0;
	m_frameTime = 33.333332f;
	m_flag118 = true;
	m_timeScale = 1.0f;
	m_bPaused = false;
	m_displayedCount = 0;
	m_displayedCount2 = 0;
	m_flag124 = false;
	m_pCamera = 0;
	m_groundAreaHigh = 99.0f;
	m_groundAreaLow = -99.0f;
	m_cornerCount = 0;

	for (int i = 0; i < 4; i++) {
		m_corners[i].m_u = (i & 2) ? 1.0f : 0.0f;
		m_corners[i].m_v = (i & 1) ? 0.0f : 1.0f;
		m_corners[i].m_index = -1;
	}
}

// fxbox.cpp:73
CFxBox::~CFxBox(void) {
	ClearFxKeeper();
	Clear();
}

// fxbox.cpp:80
void CFxBox::Clear(void) {
	ClearAll(true);
	ClearSprite();
	ClearOriginalSpray();
	ClearOriginalSequence();
}

// fxbox.cpp:89
void CFxBox::Init(WResourceManager* pResourceManager) {
	m_pResourceManager = pResourceManager;
}

// fxbox.cpp:95
void CFxBox::SetCamera(const WMatrix* pCamera) {
	m_pCamera = pCamera;
}

// fxbox.cpp:111
void CFxBox::ClearSprite(void) {
	for (std::map<std::string, W3dAniSpr*>::iterator it = m_sprites.begin(); it != m_sprites.end(); ++it) {
		if (m_pResourceManager && it->second) {
			m_pResourceManager->Release(it->second);
			it->second = 0;
		}
	}

	m_sprites.clear();
}

// fxbox.cpp:120
void CFxBox::ClearOriginalSpray(void) {
	std::map<std::string, CFxSpray*>::iterator it = m_originalSprays.begin();
	for (; it != m_originalSprays.end(); ++it) {
		CFxSpray* pSpray = it->second;
		if (g_resrcmng != NULL && pSpray->m_param.m_hTexture != 0) { g_resrcmng->Release(pSpray->m_param.m_hTexture); pSpray->m_param.m_hTexture = 0; }

		delete pSpray;
	}

	m_originalSprays.clear();
}

// fxbox.cpp:136
void CFxBox::ClearOriginalSequence(void) {
	std::map<std::string, CFxSequence*>::iterator it = m_originalSequences.begin();
	for (; it != m_originalSequences.end(); ++it) {
		CFxSequence* pSequence = it->second;
		delete pSequence;
	}

	m_originalSequences.clear();
}

// fxbox.cpp:149
void CFxBox::ClearAll(bool bKeepActive) {
	ClearSequence(bKeepActive);
	ClearSpray(bKeepActive);
}

// fxbox.cpp:156
void CFxBox::ClearSpray(bool bKeepActive) {
	for (std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.begin(); it != m_sprays.end();) {
		if (it->second) {
			if (bKeepActive && !it->second->m_bAlive) {
				++it;
				continue;
			}

			DeleteSpray(it);
		}
	}
}

// fxbox.cpp:176
void CFxBox::DeleteSpray(CFxSpray* pSpray) {
	std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.find(pSpray);
	if (it != m_sprays.end()) {
		DeleteSpray(it);
	}
}

// fxbox.cpp:184
void CFxBox::DeleteSpray(std::map<CFxSpray*, CFxSpray*>::iterator& it) {
	CFxSpray* pSpray = it->second;
	std::map<CFxSpray*, CFxSpray*>::iterator itErase = it;
	++it;
	m_sprays.erase(itErase);
	OnSprayDeleted(pSpray);

	if (pSpray->m_pParent) {
		pSpray->m_pParent->DeleteChildSpray(pSpray);
	}
	for (std::list<sFxKeeper>::iterator keeper = m_fxKeepers.begin(); keeper != m_fxKeepers.end(); ++keeper) ((*keeper).m_pKeeper->*(*keeper).m_pfnCallBack)(pSpray);

	delete pSpray;
}

// fxbox.cpp:201
void CFxBox::ClearSequence(bool bKeepActive) {
	for (std::map<CFxSequence*, CFxSequence*>::iterator it = m_sequences.begin(); it != m_sequences.end();) {
		if (it->second) {
			if (bKeepActive && !it->second->m_bAlive) {
				++it;
				continue;
			}

			DeleteSequence(it, false);
		}
	}
}

// fxbox.cpp:221
void CFxBox::DeleteSequence(CFxSequence* pSequence, bool bImmediate) {
	std::map<CFxSequence*, CFxSequence*>::iterator it = m_sequences.find(pSequence);
	if (it != m_sequences.end()) {
		DeleteSequence(it, bImmediate);
	}
}

// fxbox.cpp:229
void CFxBox::DeleteSequence(std::map<CFxSequence*, CFxSequence*>::iterator& it, bool bImmediate) {
	CFxSequence* pSequence = it->second;
	std::map<CFxSequence*, CFxSequence*>::iterator itErase = it;
	++it;
	m_sequences.erase(itErase);

	for (std::map<CFxSpray*, CFxSpray*>::iterator child = pSequence->m_sprays.begin(); child != pSequence->m_sprays.end();) {
		CFxSpray* pSpray = child->second;

		if (pSpray == 0) {
			std::map<CFxSpray*, CFxSpray*>::iterator childErase = child++;
			pSequence->m_sprays.erase(childErase);
			continue;
		}

		if (bImmediate) {
			++child;
			DeleteSpray(pSpray);
		} else {
			pSpray->m_pos += pSequence->m_pos;

			pSpray->m_pParent = 0;
			if (pSpray->m_param.m_bSurvivesSequence && pSequence->m_pAttacher) {
				pSpray->m_pAttacher = pSequence->m_pAttacher;
				AddSpray(pSpray);
			}

			++child;
		}
	}

	OnSequenceDeleted(pSequence);

	if (pSequence->m_bOriginal) {
		CallFxKeeperCallBack(pSequence);
		delete pSequence;
	}
}

// fxbox.cpp:276
bool CFxBox::Finished(void) {
	return m_sequences.empty() && m_sprays.empty();
}

// fxbox.cpp:282
const CFxSequence::sLine& CFxBox::GetLastError(void) {
	return m_lastError;
}

// fxbox.cpp:288
CFxSequence* CFxBox::LoadSequence(const char* name, bool bOriginal, defines_t* pDefines) {
	CFxSequence* pSequence = new CFxSequence(*this, bOriginal);

	ClearDefines(pSequence->m_defines, pDefines);

	if (pSequence->LoadFile(name)) {
		pSequence->m_name = name;
		return pSequence;
	}

	delete pSequence;
	return 0;
}

// fxbox.cpp:305
void CFxBox::AddSequence(CFxSequence* pSequence) {
	pSequence->SetLineTime();
	m_sequences[pSequence] = pSequence;
	OnSequenceCreated(pSequence);
}

// fxbox.cpp:313
CFxSpray* CFxBox::LoadSpray(const char* name, defines_t* pDefines) {
	CFxSpray* pSpray = new CFxSpray(*this);

	ClearDefines(pSpray->m_defines, pDefines);

	if (pSpray->ParseFile(name, pDefines == 0)) {
		return pSpray;
	}

	delete pSpray;
	return 0;
}

// fxbox.cpp:327
void CFxBox::AddSpray(CFxSpray* pSpray) {
	m_sprays[pSpray] = pSpray;
	OnSprayCreated(pSpray);
}

// fxbox.cpp:334
float CFxBox::ParseNumber(const char* str) {
	int len = strlen(str);
	if (len == 0) {
		return 0.0f;
	}

	char suffix = str[len - 1];
	if (suffix == 'd') {
		return (float)atof(str) * 0.017453292f;
	} else if (suffix == 'f') {
		return (float)atof(str) * m_frameTime;
	}
	return (float)atof(str);
}

// fxbox.cpp:357
W3dAniSpr* CFxBox::LoadSprite(const char* name, int width, int height) {
	char key[272];
	sprintf(key, "%s_%dx%d", name, width, height);

	std::map<std::string, W3dAniSpr*>::iterator it = m_sprites.find(key);
	if (it != m_sprites.end()) {
		return it->second;
	}

	if (!m_pResourceManager) {
		return 0;
	}

	W3dAniSpr* pSprite = new W3dAniSpr;
	pSprite->SetResourceManager(m_pResourceManager);
	pSprite->LoadSpritesInOneTexture(name, 0, (float)width, (float)height);

	m_sprites[key] = pSprite;
	return pSprite;
}

// fxbox.cpp:379
bool CFxBox::Process(float fDelta) {
	if (m_flag110) {
		return true;
	}

	if (m_flag118) {
		if (fDelta > m_frameTime * 1.5f) {
			fDelta = m_frameTime * 1.5f;
		}
	}

	std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.begin();
	while (it != m_sprays.end()) {
		CFxSpray* pSpray = it->second;

		if (!pSpray) {
			std::map<CFxSpray*, CFxSpray*>::iterator itErase = it++;
			m_sprays.erase(itErase);
			continue;
		}

		if (!pSpray->Process(fDelta * m_timeScale)) {
			DeleteSpray(it);
		} else {
			++it;
		}
	}

	std::map<CFxSequence*, CFxSequence*>::iterator itSeq = m_sequences.begin();
	while (itSeq != m_sequences.end()) {
		CFxSequence* pSequence = itSeq->second;

		if (!pSequence) {
			std::map<CFxSequence*, CFxSequence*>::iterator itErase = itSeq++;
			m_sequences.erase(itErase);
			continue;
		}

		bool bError;
		if (!pSequence->Process(fDelta * m_timeScale, bError)) {
			DeleteSequence(itSeq, false);
		} else {
			++itSeq;
		}

		if (bError) {
			m_lastError = pSequence->GetLastError();
			return false;
		}
	}

	return true;
}

// fxbox.cpp:449
void CFxBox::Display(WView* pView) {
	m_displayedCount = 0;
	m_displayedCount2 = 0;

	for (std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.begin(); it != m_sprays.end(); ++it) {
		CFxSpray* pSpray = it->second;

		pSpray->Display(pView);

		m_displayedCount += pSpray->m_displayedCount;
		m_displayedCount2 += pSpray->m_displayedCount2;
	}
}

// fxbox.cpp:465
void CFxBox::ClearDefines(defines_t& defines, defines_t* pParent) {
	for (int i = 0; i < 16; i++) {
		defines.m_names[i][0] = '\0';
	}

	strcpy(defines.m_names[0], "particle_level");
	if (WSingleton<COption>::IsInstantiated()) {
		int level = 2 - WSingleton<COption>::Instance()->vGetParticleLevel();
		defines.m_values[0] = (float)(level < 1 ? 1 : level);
	} else {
		defines.m_values[0] = 0;
	}

	if (pParent) {
		for (int i = 0; i < pParent->m_count; i++) {
			strcpy(defines.m_names[i + 1], pParent->m_names[i]);
			defines.m_values[i + 1] = pParent->m_values[i];
		}
		defines.m_count = pParent->m_count + 1;
	} else {
		defines.m_count = 1;
	}
}

// fxbox.cpp:513
// "#define <name> <a> [op <b>]": adds name = a op b (a and b numbers or
// earlier names) to the defines; false when the line is not a #define.
bool CFxBox::PreprocessDefines(defines_t& defines, const char* line) {
	char word[0x104];
	char name[0x104];
	int op;
	float values[2];
	float* pValue;

	cTokenV token;
	token.Init(line, strlen(line));

	token.GetToken(word, " ", 1);

	if (stricmp(word, "#define") == 0) {
		token.GetTokenWithoutFail(name, " \r\n\t", 4);

		int i;
		for (i = 0; i < defines.m_count; i++) {
			if (stricmp(name, defines.m_names[i]) == 0) {
				break;
			}
		}

		if (i < defines.m_count) {
			return true;
		}

		op = 0;
		pValue = values;

		for (int n = 0; n < 16; n++) {
			token.GetTokenWithoutFail(word, " \r\n\t", 4);
			if (word[0] == '\0') {
				continue;
			}

			if (stricmp(word, "*") == 0) {
				op = 3;
				continue;
			}
			if (stricmp(word, "/") == 0) {
				op = 4;
				continue;
			}
			if (stricmp(word, "+") == 0) {
				op = 1;
				continue;
			}
			if (stricmp(word, "-") == 0) {
				op = 2;
				continue;
			}

			for (i = 0; i < defines.m_count; i++) {
				if (stricmp(word, defines.m_names[i]) == 0) {
					*pValue++ = defines.m_values[i];
					break;
				}
			}

			if (i == defines.m_count) {
				*pValue++ = (float)atof(word);
			}
		}

		float value;
		switch (op) {
		case 1:
			value = values[0] + values[1];
			break;
		case 2:
			value = values[0] - values[1];
			break;
		case 3:
			value = values[0] * values[1];
			break;
		case 4:
			value = values[0] / values[1];
			break;
		default:
			value = values[0];
			break;
		}

		strcpy(defines.m_names[defines.m_count], name);
		defines.m_values[defines.m_count] = value;
		defines.m_count++;

		return true;
	}

	return false;
}

// fxbox.cpp:622
void CFxBox::ProcessDefines(defines_t& defines, char* token) {
	for (int i = 0; i < defines.m_count; i++) {
		if (_stricmp(token, defines.m_names[i]) == 0) {
			_gcvt(defines.m_values[i], 10, token);
		}
	}
}

// fxbox.cpp:634
void CFxBox::AddFxKeeper(sFxKeeper& keeper) {
	if (keeper.m_pKeeper) {
		keeper.m_pKeeper->SetFxManager(this);
	}
	m_fxKeepers.push_back(keeper);
}

// fxbox.cpp:642
void CFxBox::CallFxKeeperCallBack(void* pParam) {
	for (std::list<sFxKeeper>::iterator it = m_fxKeepers.begin(); it != m_fxKeepers.end(); ++it) {
		((*it).m_pKeeper->*(*it).m_pfnCallBack)(pParam);
	}
}

// fxbox.cpp:653
void CFxBox::EraseFxKeeper(CFxKeeper* pKeeper) {
	if (pKeeper == 0) {
		return;
	}

	for (std::list<sFxKeeper>::iterator it = m_fxKeepers.begin(); it != m_fxKeepers.end(); ++it) {
		if (it->m_pKeeper == pKeeper) {
			m_fxKeepers.erase(it);
			return;
		}
	}
}

// fxbox.cpp:670
void CFxBox::ClearFxKeeper(void) {
	m_fxKeepers.clear();
}

// fxbox.cpp:676
void CFxBox::SetScreenCenter(float x, float y) {
	for (std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.begin(); it != m_sprays.end(); ++it) {
		CFxSpray* pSpray = it->second;

		if (pSpray) {
			pSpray->m_fAttachLimitA = x;
			pSpray->m_fAttachLimitB = y;
		}
	}

	for (std::map<CFxSequence*, CFxSequence*>::iterator it2 = m_sequences.begin(); it2 != m_sequences.end(); ++it2) {
		CFxSequence* pSequence = it2->second;

		if (pSequence) {
			pSequence->m_fDistLimitA = x;
			pSequence->m_fDistLimitB = y;
		}
	}
}
