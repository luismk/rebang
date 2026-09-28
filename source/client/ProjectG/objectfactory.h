#pragma once
#include "rtti.h"

class IObject
{
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI() const = 0;
	bool IsExactKindOf(const WRTTI* pRTTI) const { return GetRTTI() == pRTTI; }
	bool IsKindOf(const WRTTI* pRTTI) const
	{
		const WRTTI* pBase = GetRTTI();
		while (pBase)
		{
			if (pBase == pRTTI)
				return true;
			pBase = pBase->GetBaseRTTI();
		}
		return false;
	}
	void* DynamicCast(const WRTTI* pRTTI) { return IsKindOf(pRTTI) ? this : 0; }
	virtual ~IObject();

protected:
	IObject();
};

class CObjectFactory
{
public:
	typedef IObject* (*ObjectFunctor)();
	void AddObjectFunctor(ObjectFunctor function, const char* name);
};

CObjectFactory& ObjectFactory();
