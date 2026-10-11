// Original: d:\build\custom\temp\code\client\projectg\effectmanager.h
#ifndef PG_CLIENT_PROJECTG_EFFECTMANAGER_H
#define PG_CLIENT_PROJECTG_EFFECTMANAGER_H

#include <map>

#include "..\..\defines.h"
#include "singleton.h"

// The hole-in-one effects of the Aztec course: effect id -> spray name, looked
// up first in the table loaded from the IFF, then in the registered one.
class CAztecHoleinEffect : public WSingleton<CAztecHoleinEffect> {
	LAYOUT_CHECK_FRIEND;
public:
	typedef std::map<unsigned long, const char*> EffectNameMap;

	CAztecHoleinEffect();
	virtual ~CAztecHoleinEffect();

	bool LoadEffectFromIFF();
	bool Initialize();
	bool PlayEffect(unsigned long id);

protected:
	void Clear();
	const char* GetEffectName(unsigned long id);
	bool RegisterEffect(unsigned long id, const char* name);

	unsigned long m_lastEffectId;       // 0x28, only reset by Clear
	unsigned long m_lastEffectTime;     // 0x2C, only reset by Clear
	EffectNameMap m_iffEffects;         // 0x30, owns its names
	EffectNameMap m_registeredEffects;  // 0x3C, owns its names
};

CHECK_SIZE(CAztecHoleinEffect, 0x48);

BEGIN_LAYOUT_CHECK(CAztecHoleinEffect)
	CHECK_FIELD(CAztecHoleinEffect, m_lastEffectId, 0x28);
	CHECK_FIELD(CAztecHoleinEffect, m_lastEffectTime, 0x2C);
	CHECK_FIELD(CAztecHoleinEffect, m_iffEffects, 0x30);
	CHECK_FIELD(CAztecHoleinEffect, m_registeredEffects, 0x3C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_EFFECTMANAGER_H
