// Original: d:\build\custom\temp\code\client\projectg\effectmanager.cpp
#include "EffectManager.h"
#include "fx.h"
#include "fxspray.h"
#include "golfball.h"

template <> CAztecHoleinEffect* WSingleton<CAztecHoleinEffect>::m_pInstance = NULL;

// effectmanager.cpp:10
CAztecHoleinEffect::CAztecHoleinEffect() {
	Clear();
}

// effectmanager.cpp:17
CAztecHoleinEffect::~CAztecHoleinEffect() {
	Clear();
}

// effectmanager.cpp:23
void CAztecHoleinEffect::Clear() {
	m_lastEffectId = 0;
	m_lastEffectTime = 0;

	EffectNameMap::iterator it;
	for (it = m_iffEffects.begin(); it != m_iffEffects.end(); ++it) {
		if (it->second) {
			delete it->second;
			it->second = NULL;
		}
	}
	m_iffEffects.clear();

	for (it = m_registeredEffects.begin(); it != m_registeredEffects.end(); ++it) {
		if (it->second) {
			delete it->second;
			it->second = NULL;
		}
	}
	m_registeredEffects.clear();
}

// effectmanager.cpp:52
bool CAztecHoleinEffect::LoadEffectFromIFF() {
	return true;
}

// effectmanager.cpp:61
bool CAztecHoleinEffect::Initialize() {
	return false;
}

// effectmanager.cpp:68
bool CAztecHoleinEffect::RegisterEffect(unsigned long id, const char* name) {
	return m_registeredEffects.insert(EffectNameMap::value_type(id, name)).second;
}

// effectmanager.cpp:77
const char* CAztecHoleinEffect::GetEffectName(unsigned long id) {
	const char* name = NULL;
	EffectNameMap::iterator it;

	it = m_iffEffects.find(id);
	if (it != m_iffEffects.end()) {
		name = it->second;
		if (name) {
			return name;
		}
	}

	it = m_registeredEffects.find(id);
	if (it != m_registeredEffects.end()) {
		name = it->second;
		if (name) {
			return name;
		}
	}

	return name;
}

// effectmanager.cpp:114
bool CAztecHoleinEffect::PlayEffect(unsigned long id) {
	bool result = false;
	const char* name = GetEffectName(id);

	if (name) {
		CFxSpray* pSpray = CFx::Instance()->OpenSpray(name);

		if (pSpray) {
			WVector pos = GolfBall().m_pos;
			pSpray->m_pos = pos;

			return true;
		}
	}

	return result;
}
