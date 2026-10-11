// Original: d:\build\custom\temp\code\client\projectg\extendedeffect.h
#ifndef PG_CLIENT_PROJECTG_EXTENDEDEFFECT_H
#define PG_CLIENT_PROJECTG_EXTENDEDEFFECT_H

#include <list>
#include <map>
#include <string>

#include "..\..\defines.h"

class CFxSequence;
class CPetFrame;

// Effect sequences attached to the bones of pet frames (avatars): a table of
// effect definitions keyed by part id, and for each registered pet frame the
// sequences opened for the parts it wears. Layout from the constructor
// (extendedeffect.cpp:16) and Update (extendedeffect.cpp:165).
class CExtendedEffect {
	LAYOUT_CHECK_FRIEND;
public:
	// One effect: the bone it follows and the .fx sequence it runs.
	struct sEffectData {
		unsigned long m_id;             // 0x0, part id the effect belongs to
		std::string m_boneName;         // 0x4, bone of the pet frame's bone puppet
		std::string m_fxName;           // 0x20, sequence opened with CFx::OpenSequence
		bool m_bLoop;                   // 0x3C, reopen the sequence once it ended
		CFxSequence* m_pSequence;       // 0x40
	};

	// A registered pet frame and the effects opened for its parts.
	struct sAvatarInfo {
		CPetFrame* m_pPetFrame;                 // 0x0
		std::list<sEffectData> m_effects;       // 0x4
	};

	CExtendedEffect(void);
	virtual ~CExtendedEffect(void);

	void Update(CPetFrame* pPetFrame);
	int RegistExtenedEffect(CPetFrame* pPetFrame);

private:
	int AddEffect(unsigned long id, const char* boneName, const char* fxName, bool bLoop);
	sEffectData* FindEffect(unsigned long id);
	int RegistSequence(unsigned long id, std::list<sEffectData>* pList);

	std::map<unsigned long, sEffectData> m_effectMap;  // 0x4
	std::list<sAvatarInfo> m_avatarList;              // 0x10
};

CHECK_SIZE(CExtendedEffect::sEffectData, 0x44);
CHECK_FIELD(CExtendedEffect::sEffectData, m_boneName, 0x4);
CHECK_FIELD(CExtendedEffect::sEffectData, m_fxName, 0x20);
CHECK_FIELD(CExtendedEffect::sEffectData, m_bLoop, 0x3C);
CHECK_FIELD(CExtendedEffect::sEffectData, m_pSequence, 0x40);
CHECK_SIZE(CExtendedEffect::sAvatarInfo, 0x10);
CHECK_FIELD(CExtendedEffect::sAvatarInfo, m_effects, 0x4);

BEGIN_LAYOUT_CHECK(CExtendedEffect)
	CHECK_FIELD(CExtendedEffect, m_effectMap, 0x4);
	CHECK_FIELD(CExtendedEffect, m_avatarList, 0x10);
	CHECK_SIZE(CExtendedEffect, 0x1C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_EXTENDEDEFFECT_H
