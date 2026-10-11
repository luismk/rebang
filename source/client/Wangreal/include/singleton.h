// Original: d:\build\custom\temp\code\client\wangreal\include\singleton.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_SINGLETON_H
#define PG_CLIENT_WANGREAL_INCLUDE_SINGLETON_H

#include "..\..\defines.h"
#include "baseobject.h"

// PDB: WSingleton<T> (size 0x28) derives from BaseObject and has a virtual
// destructor; the constructor registers the instance, the destructor clears
// it.
template <class T>
class WSingleton : public BaseObject {
public:
	// singleton.h:34
	WSingleton(void) {
		m_pInstance = static_cast<T*>(this);
	}

	// singleton.h:41
	virtual ~WSingleton(void) {
		m_pInstance = 0;
	}

	// singleton.h:48
	static T* Instance() {
		return m_pInstance;
	}

	// singleton.h:56
	static bool IsInstantiated() {
		return m_pInstance ? true : false;
	}

protected:
	static T* m_pInstance;
};

#endif
