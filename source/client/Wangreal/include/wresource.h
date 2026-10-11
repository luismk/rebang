// Original: d:\build\custom\temp\code\client\wangreal\include\wresource.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WRESOURCE_H
#define PG_CLIENT_WANGREAL_INCLUDE_WRESOURCE_H

#include "..\..\defines.h"
#include "baseobject.h"

class WResourceManager;

// A BaseObject owned by a WResourceManager (PDB layout, size 0x2c).
class WResource : public BaseObject {
	LAYOUT_CHECK_FRIEND;
public:
	WResource(void) : m_resrcMng(0) {}                                      // wresource.h:23
	virtual ~WResource(void) {}                                             // wresource.h:24

	void SetResourceManager(WResourceManager* mng) { m_resrcMng = mng; }   // wresource.h:26

protected:
	WResourceManager* GetResrcManager(void) const { return m_resrcMng; }   // wresource.h:29

private:
	WResourceManager* m_resrcMng;   // 0x28
};
CHECK_SIZE(WResource, 0x2c);
BEGIN_LAYOUT_CHECK(WResource)
	CHECK_FIELD(WResource, m_resrcMng, 0x28);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WRESOURCE_H
