#pragma once
#include "rtti.h"

class IObject
{
public:
	virtual const WRTTI* GetRTTI() const = 0;
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
