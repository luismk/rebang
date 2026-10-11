// Original: d:\build\custom\temp\code\client\projectg\rtti.h
#ifndef PG_CLIENT_PROJECTG_RTTI_H
#define PG_CLIENT_PROJECTG_RTTI_H

#include "..\..\defines.h"

// The game's own run-time type information: every polymorphic class keeps a
// `static const WRTTI m_RTTI` naming itself and its base's m_RTTI, and returns
// it from `virtual const WRTTI* GetRTTI(void) const`. Layout from the PDB.
class WRTTI {
	LAYOUT_CHECK_FRIEND;
public:
	WRTTI(const char* pName, const WRTTI* pBaseRTTI);

	// rtti.h:18
	const char* GetName(void) const { return m_pName; }
	// rtti.h:19
	const WRTTI* GetBaseRTTI(void) const { return m_pBaseRTTI; }

protected:
	const char* m_pName;        // 0x0
	const WRTTI* m_pBaseRTTI;   // 0x4
};

CHECK_SIZE(WRTTI, 0x8);
BEGIN_LAYOUT_CHECK(WRTTI)
	CHECK_FIELD(WRTTI, m_pName, 0x0);
	CHECK_FIELD(WRTTI, m_pBaseRTTI, 0x4);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_RTTI_H
