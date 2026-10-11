// Original: d:\build\custom\temp\code\client\projectg\objectfactory.cpp
#include "ObjectFactory.h"

// objectfactory.cpp:14
const WRTTI IObject::m_RTTI("IObject", 0);

// objectfactory.cpp:17
IObject::IObject(void) {
}

// objectfactory.cpp:22
IObject::~IObject(void) {
}

// objectfactory.cpp:31
CObjectFactory& ObjectFactory(void) {
	static CObjectFactory s_objectFactory;
	return s_objectFactory;
}

// objectfactory.cpp:37
void CObjectFactory::Reset(void) {
	ObjectTypeMap::iterator it = m_objectTypes.begin();

	while (it != m_objectTypes.end()) {
		delete it->second;
		++it;
	}

	m_objectTypes.erase(m_objectTypes.begin(), m_objectTypes.end());
	m_objectFunctors.erase(m_objectFunctors.begin(), m_objectFunctors.end());
}

// objectfactory.cpp:56
void CObjectFactory::AddObjectType(IObject* pObject, const char* name) {
	m_objectTypes[name] = pObject;
}

// objectfactory.cpp:64
void CObjectFactory::AddObjectFunctor(IObjectFunc pfnMake, const char* name) {
	m_objectFunctors[name] = pfnMake;
}
