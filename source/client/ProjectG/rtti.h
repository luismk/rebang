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

#define DYNAMIC_CAST(type, obj) \
	((__rtti_obj = (obj)) \
			? (type*)((IObject*)__rtti_obj)->DynamicCast(&type::m_RTTI) \
			: NULL)
