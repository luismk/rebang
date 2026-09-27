#pragma once

class WRTTI
{
public:
	WRTTI(const char* name, const WRTTI* baseRTTI);
	const WRTTI* GetBaseRTTI() const { return m_pBaseRTTI; }

protected:
	const char* m_pName;
	const WRTTI* m_pBaseRTTI;
};
