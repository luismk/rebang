#pragma once

class WRTTI
{
public:
	WRTTI(const char* name, const WRTTI* baseRTTI);
	const char* GetName() const { return m_pName; }
	const WRTTI* GetBaseRTTI() const { return m_pBaseRTTI; }

protected:
	const char* m_pName;
	const WRTTI* m_pBaseRTTI;
};

#define DYNAMIC_CAST(type, obj) \
	((__rtti_obj = (obj)) \
			? (type*)((IObject*)__rtti_obj)->DynamicCast(&type::m_RTTI) \
			: NULL)

#define IS_KINDOF(type, obj) \
	((__rtti_obj = (obj)) ? ((IObject*)__rtti_obj)->IsKindOf(&type::m_RTTI) \
						  : false)

#define IS_EXACTKINDOF(type, obj) \
	((__rtti_obj = (obj)) \
			? ((IObject*)__rtti_obj)->IsExactKindOf(&type::m_RTTI) \
			: false)
