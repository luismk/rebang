// Original: d:\build\custom\temp\code\client\projectg\approachdesc.h
#ifndef PG_CLIENT_PROJECTG_APPROACHDESC_H
#define PG_CLIENT_PROJECTG_APPROACHDESC_H

#include <string.h>
#include "..\..\defines.h"

// Description text of an approach mission; each Mission_<n> builds it.
class DescBlock {
public:
	DescBlock() {
		memset(m_szDesc, 0, sizeof(m_szDesc));
	}
	virtual ~DescBlock() {}

protected:
	LAYOUT_CHECK_FRIEND;

	virtual void BuildDesc(unsigned long dwParam1, unsigned long dwParam2, char const* pszParam) {}

public:
	char* GetDesc() {
		return m_szDesc;
	}

protected:
	char m_szDesc[128]; // 0x4
};
// Owns the DescBlock of an approach mission number.
class CApproachDesc {
public:
	~CApproachDesc() {
		if (m_pDesc)
			delete m_pDesc;
		m_pDesc = NULL;
	}

	CApproachDesc(int nMission, unsigned long dwParam1, unsigned long dwParam2, char const* pszParam);

	// The mission's description text, NULL when the mission number is
	// unknown.
	char* GetDesc() {
		return m_pDesc ? m_pDesc->GetDesc() : NULL;
	}

private:
	LAYOUT_CHECK_FRIEND;

	DescBlock* m_pDesc; // 0x0
};
CHECK_SIZE(DescBlock, 0x84);
BEGIN_LAYOUT_CHECK(DescBlock)
	CHECK_FIELD(DescBlock, m_szDesc, 0x4);
END_LAYOUT_CHECK
CHECK_SIZE(CApproachDesc, 0x4);
BEGIN_LAYOUT_CHECK(CApproachDesc)
	CHECK_FIELD(CApproachDesc, m_pDesc, 0x0);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_APPROACHDESC_H
