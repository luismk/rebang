// Original: d:\build\custom\temp\code\client\projectg\icontentsdoc.h
#ifndef PG_CLIENT_PROJECTG_ICONTENTSDOC_H
#define PG_CLIENT_PROJECTG_ICONTENTSDOC_H

#include "..\..\defines.h"
#include "singleton.h"
#include "Localize.h"

#include <map>

namespace _gateway { class CGatewayHandler; }

// Base of the per-content data containers (vtable ??_7CGimmickContainer@@6B@:
// the destructor, Initialize, Release, Reset, then one more slot). Only the
// members the decompiled units use are declared.
struct IContentsDataContainer {
	IContentsDataContainer(void) {}
	virtual ~IContentsDataContainer(void) {}

	virtual bool Initialize(void) = 0;
	virtual bool Release(void) = 0;

	virtual bool Reset(void) { return true; }

	// Slot 4: the content's gateway handler (0 when it has none).
	virtual _gateway::CGatewayHandler* GetGatewayHandler(void) { return 0; }
};

// The document holding the content containers, keyed by content type. Only
// the members the decompiled units use are declared.
class CContentsDoc : public WSingleton<CContentsDoc> {
	LAYOUT_CHECK_FRIEND;
public:
	CContentsDoc(void);
	virtual ~CContentsDoc(void);

	bool Initialize(void);
	bool Release(void);

	IContentsDataContainer *GetContainer(localContentType_t type);
	int InsertContainer(localContentType_t type, IContentsDataContainer *container, bool force);

	// icontentsdoc.h:41
	template <class T> bool GetContainer(localContentType_t type, T &container) {
		IContentsDataContainer *found = GetContainer(type);

		if (found) {
			container = static_cast<T>(found);
			return true;
		}

		return false;
	}

private:
	std::map<localContentType_t, IContentsDataContainer *> m_containers;   // 0x28
	int m_allInserted;                                                     // 0x34: AND of the InsertContainer results
};

CHECK_SIZE(CContentsDoc, 0x38);
BEGIN_LAYOUT_CHECK(CContentsDoc)
	CHECK_FIELD(CContentsDoc, m_containers, 0x28);
	CHECK_FIELD(CContentsDoc, m_allInserted, 0x34);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_ICONTENTSDOC_H
