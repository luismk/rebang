// Original: d:\build\custom\temp\code\client\projectg\objectfactory.h
#ifndef PG_CLIENT_PROJECTG_OBJECTFACTORY_H
#define PG_CLIENT_PROJECTG_OBJECTFACTORY_H

#include "..\..\defines.h"
#include "RTTI.h"

#include <map>
#include <string>

// Root of the game's run-time-typed classes (layout and vtable from the PDB:
// slot 0 GetRTTI, slot 1 the destructor). The RTTI members of the class
// body are all marked objectfactory.h:54.
class IObject {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }
	bool IsExactKindOf(const WRTTI* pRTTI) const { return GetRTTI() == pRTTI; }
	bool IsKindOf(const WRTTI* pRTTI) const {
		const WRTTI* pCur = GetRTTI();
		while (pCur != 0) {
			if (pCur == pRTTI) {
				return true;
			}
			pCur = pCur->GetBaseRTTI();
		}
		return false;
	}
	void* DynamicCast(const WRTTI* pRTTI) { return IsKindOf(pRTTI) ? this : 0; }

	virtual ~IObject(void);

protected:
	IObject(void);
	IObject(const IObject&);
};

CHECK_SIZE(IObject, 0x4);

// Factory function registered for each class name (PDB typedef).
typedef IObject* (*IObjectFunc)(void);

// Name -> object and name -> factory function registries. The constructor
// and destructor are defined in the class body (objectfactory.h:83-90); the
// key type is `const std::string` (decorated names of the map instances).
class CObjectFactory {
	LAYOUT_CHECK_FRIEND;
public:
	typedef std::map<const std::string, IObject*> ObjectTypeMap;
	typedef std::map<const std::string, IObjectFunc> ObjectFunctorMap;

	// objectfactory.h:83
	CObjectFactory(void) {
	}
	// objectfactory.h:88
	~CObjectFactory(void) {
		Reset();
	}

	void AddObjectFunctor(IObjectFunc pfnMake, const char* name);
	void AddObjectType(IObject* pObject, const char* name);
	IObject* CreateObject(const char* name);
	void Reset(void);

protected:
	ObjectTypeMap m_objectTypes;            // 0x0
	ObjectFunctorMap m_objectFunctors;      // 0xC
};

CHECK_SIZE(CObjectFactory, 0x18);
BEGIN_LAYOUT_CHECK(CObjectFactory)
	CHECK_FIELD(CObjectFactory, m_objectTypes, 0x0);
	CHECK_FIELD(CObjectFactory, m_objectFunctors, 0xC);
END_LAYOUT_CHECK

CObjectFactory& ObjectFactory(void);

#endif // PG_CLIENT_PROJECTG_OBJECTFACTORY_H
