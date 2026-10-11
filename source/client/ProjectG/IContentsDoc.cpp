// Original: d:\build\custom\temp\code\client\projectg\icontentsdoc.cpp
#include "icontentsdoc.h"
#include "contentsdoc.h"
#include "GatewayDoc.h"

#include <windows.h>

#include <utility>

// icontentsdoc.cpp:17
CContentsDoc::CContentsDoc(void) : m_allInserted(TRUE) {
}

// icontentsdoc.cpp:24
CContentsDoc::~CContentsDoc(void) {
	Release();
}

// icontentsdoc.cpp:30
bool CContentsDoc::Initialize(void) {
	InsertContainer((localContentType_t)0x1d, new CGiftBoxEvent, true);
	InsertContainer((localContentType_t)0x1e, new CWorldTourEvent, false);
	InsertContainer((localContentType_t)0x21, new CHalloweenEvent, false);
	InsertContainer((localContentType_t)0x24, new CGiftBoxEvent, false);
	InsertContainer((localContentType_t)0x4c, new CChristmasEvent, false);
	InsertContainer((localContentType_t)0x7a, new CTicketExchange, false);
	InsertContainer((localContentType_t)0x83, new CMissionEvent, false);

	InsertContainer((localContentType_t)0x92, new CPeriodContents, false);

	InsertContainer((localContentType_t)0x97, new _guild::CGuildInfo, false);

	InsertContainer((localContentType_t)0x9c, new CChristmasSockEvent, false);

	InsertContainer((localContentType_t)0x9d, new _gateway::CSecondPwdDoc, false);

	InsertContainer((localContentType_t)0x9e, new _gateway::CGolfItemDoc, false);

	InsertContainer((localContentType_t)0x9f, new _gateway::CTradeDoc, false);

	InsertContainer((localContentType_t)0xa2, new CVectorSlideTestContainer, false);

	// TODO: InsertContainer((localContentType_t)0xa3, new CGimmickContainer, false);
	// needs FieldItem::AcquiredItemInfo complete and CGimmickContainer's full
	// 0x98-byte layout in contentsdoc.h.

	InsertContainer((localContentType_t)0xa0, new _gateway::CSpecialBoxDoc, false);

	return m_allInserted == TRUE;
}

// icontentsdoc.cpp:100
bool CContentsDoc::Release(void) {
	bool result = true;

	for (std::map<localContentType_t, IContentsDataContainer *>::iterator it = m_containers.begin(); it != m_containers.end(); ++it) {
		if (it->second != NULL) {
			if (!it->second->Release()) {
				result = false;
			}

			delete it->second;
			it->second = NULL;
		}
	}

	m_containers.clear();

	return result;
}

// icontentsdoc.cpp:143
int CContentsDoc::InsertContainer(localContentType_t type, IContentsDataContainer *container, bool force) {
	int result = TRUE;

	if ((IsLocalContent(type) && container != NULL) || force == true) {
		if (container->Initialize()) {
			m_containers.insert(std::make_pair(type, container));
		} else {
			result = FALSE;
		}
	} else {
		if (container != NULL) {
			delete container;
		} else {
			result = FALSE;
		}
	}

	m_allInserted &= result;

	return result;
}

// icontentsdoc.cpp:181
IContentsDataContainer *CContentsDoc::GetContainer(localContentType_t type) {
	std::map<localContentType_t, IContentsDataContainer *>::iterator it;

	it = m_containers.find(type);

	if (it != m_containers.end()) {
		return it->second;
	}

	return NULL;
}
