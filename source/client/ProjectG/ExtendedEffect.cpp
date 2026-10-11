// Original: d:\build\custom\temp\code\client\projectg\extendedeffect.cpp
#include <windows.h>

#include "ExtendedEffect.h"

#include "fx.h"
#include "fxsequence.h"
#include "partsinfo.h"
#include "wbone.h"
#include "wpuppet.h"

// extendedeffect.cpp:16
CExtendedEffect::CExtendedEffect(void) {
}

// extendedeffect.cpp:36
CExtendedEffect::~CExtendedEffect(void) {
	std::list<sAvatarInfo>::iterator it = m_avatarList.begin();
	for (; it != m_avatarList.end(); ++it) {
		std::list<sEffectData>::iterator effect = it->m_effects.begin();
		for (; effect != it->m_effects.end(); ++effect) {
			CFxSequence* pSequence = effect->m_pSequence;
			if (pSequence != NULL) {
				CFx::Instance()->CloseSequence(pSequence, false);
			}
		}

		it->m_effects.clear();
	}
	m_avatarList.clear();

	m_effectMap.clear();
}

// extendedeffect.cpp:61
int CExtendedEffect::AddEffect(unsigned long id, const char* boneName, const char* fxName, bool bLoop) {
	if (id == 0 || boneName == NULL || fxName == NULL) {
		return FALSE;
	}

	std::map<unsigned long, sEffectData>::iterator it = m_effectMap.find(id);
	if (it != m_effectMap.end()) {
		return FALSE;
	}

	sEffectData data;
	data.m_id = id;
	data.m_boneName = boneName;
	data.m_fxName = fxName;
	data.m_bLoop = bLoop;
	data.m_pSequence = NULL;

	m_effectMap[id] = data;

	return TRUE;
}

// extendedeffect.cpp:85
CExtendedEffect::sEffectData* CExtendedEffect::FindEffect(unsigned long id) {
	sEffectData* pData = NULL;

	std::map<unsigned long, sEffectData>::iterator it = m_effectMap.find(id);
	if (it != m_effectMap.end()) {
		pData = &it->second;
	}

	return pData;
}

// extendedeffect.cpp:100
int CExtendedEffect::RegistSequence(unsigned long id, std::list<sEffectData>* pList) {
	if (id == 0 || pList == NULL) {
		return FALSE;
	}

	sEffectData* pData = FindEffect(id);
	if (pData == NULL) {
		return FALSE;
	}

	sEffectData data;
	data.m_id = pData->m_id;
	data.m_boneName = pData->m_boneName;
	data.m_fxName = pData->m_fxName;
	data.m_bLoop = pData->m_bLoop;
	data.m_pSequence = NULL;

	data.m_pSequence = CFx::Instance()->OpenSequence(data.m_fxName.c_str(), true);
	if (data.m_pSequence == NULL) {
		return FALSE;
	}

	pList->push_back(data);

	return TRUE;
}

// extendedeffect.cpp:165
void CExtendedEffect::Update(CPetFrame* pPetFrame) {
	std::list<sAvatarInfo>::iterator it = m_avatarList.begin();
	for (; it != m_avatarList.end(); ++it) {
		bool bActive = true;
		if (pPetFrame != NULL && pPetFrame != it->m_pPetFrame) {
			bActive = false;
		}

		std::list<sEffectData>::iterator effect = it->m_effects.begin();
		for (; effect != it->m_effects.end(); ++effect) {
			if (!CFx::Instance()->FindSequence(effect->m_pSequence)) {
				if (effect->m_bLoop == true) {
					effect->m_pSequence = CFx::Instance()->OpenSequence(effect->m_fxName.c_str(), true);
				} else {
					effect->m_pSequence = NULL;
				}
			}

			if (effect->m_pSequence != NULL) {
				effect->m_pSequence->SetActive(bActive, false);
				WBone* pBone = it->m_pPetFrame->GetBonePet()->FindBone(effect->m_boneName.c_str());
				effect->m_pSequence->Pos() = pBone->GetMatrix().pivot;
			}
		}
	}
}
