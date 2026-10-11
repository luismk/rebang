// Original: d:\build\custom\temp\code\client\wangreal\include\baseobject.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_BASEOBJECT_H
#define PG_CLIENT_WANGREAL_INCLUDE_BASEOBJECT_H

#include <string>

#include "..\..\defines.h"

// The root of Wangreal's reference-counted objects (PDB layout, size 0x28).
// The constructor and destructor live in baseobject.cpp (Wangreal).
class BaseObject {
	LAYOUT_CHECK_FRIEND;
public:
	typedef void (*ConstructionCallback)(BaseObject*);
	typedef void (*DestructionCallback)(BaseObject*);

	BaseObject(void);
	virtual ~BaseObject(void);

	static int GetTotalCount(void);
	static void SetConstructionCallback(ConstructionCallback fn);
	static void SetDestructionCallback(DestructionCallback fn);

	const char* GetLeakHint(void);
	void SetLeakHint(const char* hint);

private:
	static int ms_nextId;
	static int ms_count;
	static ConstructionCallback ms_fnConstruct;
	static DestructionCallback ms_fnDestruct;

	int m_id;                       // 0x4
	int m_ref;                      // 0x8
	std::string m_leakHint;         // 0xc
};
CHECK_SIZE(BaseObject, 0x28);
BEGIN_LAYOUT_CHECK(BaseObject)
	CHECK_FIELD(BaseObject, m_id, 0x4);
	CHECK_FIELD(BaseObject, m_ref, 0x8);
	CHECK_FIELD(BaseObject, m_leakHint, 0xc);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_BASEOBJECT_H
